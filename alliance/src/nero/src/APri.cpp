
// -*- C++ -*-
//
// $Id: APri.cpp,v 1.1 2002/10/02 21:23:47 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./APri.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "ADefs.h"




  // ---------------------------------------------------------------
  // Pre-defined objects.

  // Signal priority map.
  CMapPri *netpri;




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Constructor  :  "CMapPri::CMapPri()".

CMapPri::CMapPri (CCoordBase *coordBase)
{
  base = coordBase;

  _map     = new     char [base->XYZ];
  _mapsize = sizeof (char [base->XYZ]);

  clear();

  ::netpri = this;
}




// -------------------------------------------------------------------
// Destructor  :  "CMapPri::~CMapPri()".

CMapPri::~CMapPri (void)
{
  delete [] _map;
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::operator[]".

int CMapPri::operator[] (CCoord &coord) throw (primap_clear)
{
  if (cleared) throw primap_clear ();

  if ((coord.x() < 0) || (coord.x() > base->X - 1)) return (0);
  if ((coord.y() < 0) || (coord.y() > base->Y - 1)) return (0);
  if ((coord.z() < 1) || (coord.z() > base->Z - 1)) return (0);

  return (offset + (int)(_map[coord.index() - base->XY]));
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::map()".

char *CMapPri::map (CCoord coord)
{
  if ((coord.x() < 0) || (coord.x() > base->X - 1)) return (NULL);
  if ((coord.y() < 0) || (coord.y() > base->Y - 1)) return (NULL);
  if ((coord.z() < 1) || (coord.z() > base->Z - 1)) return (NULL);

  return (&(_map[coord.index() - base->XY]));
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::map()".

char *CMapPri::map (int index)
{
  CCoord nodeCoord = CCoord (index);

  return (map(nodeCoord));
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::findfree()".

void  CMapPri::findfree (CCoord &center)
{
  int     radius, i, j, cx, cy;
  CCoord  coord;
  bool    freed;


  freed  = false;
  radius = 0;
  cx = center.x();
  cy = center.y();

  while (!freed) {
    radius += 1;

    for (i = cx - radius; i < cx + radius + 1; i++) {
      for (j = cy - radius; j < cy + radius + 1; j++) {
        // Proccess only nodes of the ring.
        // if ( (i > cx - radius) || (i < cx + radius) ) continue;
        // if ( (j > cy - radius) || (j < cy + radius) ) continue;

        if ( !(*::grid)[coord.set(i,j,2)]->data.obstacle ) freed = true;

        *map (coord.set(i,j,1)) = (char)1;
        *map (coord.set(i,j,2)) = (char)1;
      }
    }
  }
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::clear()".

void CMapPri::clear (void)
{
  //int  index;
  //
  //for (index = 0; index < base->XYZ; index++) {
  //  _map[index] = (char)0;
  //}

  memset (_map, (char)0, _mapsize);

  cleared = true;
  offset  = 0;
  delta   = 0;
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::nextPri()".

char CMapPri::nextPri (char curpri)
{
  return ((curpri > 1) ? (curpri >> 1) : 1);
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::load()".

void CMapPri::load (CNet &net, bool global, int expand=0)
{
  list<CNode*>::iterator itNode, beginNode, endNode;

  queue<CCoord*>  queue1, queue2;
  queue<CCoord*>  *currentBorder, *nextBorder;

  CCoord *pCoord, nodeCoord;
  char    currentPri, *pmap;
  int     x, y, z, nx, ny, nz, edge, id, ex, ey;
  bool    breakLoop;


  clear ();

  offset = net.pri;
  delta  = 0;

  currentBorder = &queue1;
  nextBorder    = &queue2;
  currentPri    = 64;


  // Expand the original BB if too small.
  ex = 0;
  ey = 0;

  if (net.bb.x2 - net.bb.x1 < 5) ex = 5;
  if (net.bb.y2 - net.bb.y1 < 5) ey = 5;

  if (expand) ex = ey = expand;

  _bb.x1 = max (0          , net.bb.x1 - ex);
  _bb.x2 = min (base->X - 1, net.bb.x2 + ex);
  _bb.y1 = max (0          , net.bb.y1 - ey);
  _bb.y2 = min (base->Y - 1, net.bb.y2 + ey);


  // Build the initials border lists : coordinates of the terminals.
  // The table doesn't have a z = 0 layer (never used for routing),
  //   so when a terminal on layer 0 is encountered, we queue the
  //   the coordinate on top in the next border queue.
  // That is, in the first step of the algorithm we fill both queue
  //   at the same time.
  // In the case of the map of a global signal (i.e. using z=3&4 for
  //   the time beeing, set to one the map points above the terminal
  //   in z=1&2, as they will not be set by the _bb loop.

  for (id = 0; id < net.size; id++) {
    beginNode = (net.terms[id])->nodes.begin ();
      endNode = (net.terms[id])->nodes.end ();
    
    for (itNode = beginNode; itNode != endNode; itNode++) {
      nodeCoord = (*itNode)->coord;

      // Initial queues filling.
      if (nodeCoord.z() > 0) {
        *(map (nodeCoord)) = currentPri; 
        currentBorder->push (new CCoord (nodeCoord));
        
        // Enable z=1 (in case of global signal, no effet otherwise).
        if (nodeCoord.z() < base->Z - 1) *(map (nodeCoord.dz(1))) = (char)1; 

        continue;
      }

      *(map (nodeCoord.dz(1))) = nextPri (currentPri); 
      nextBorder->push (new CCoord (nodeCoord));
        
      // Enable z=2 (in case of global signal, no effet otherwise).
      *(map (nodeCoord.dz(1))) = (char)1; 

      // Look if the upper node is blocked, in that case expand the
      // allowed zone till a non-blocked node is found.
      if ((*::grid)[nodeCoord]->data.obstacle) findfree (nodeCoord);
    }
  }


  // Set to one all the points inside the enclosing box.
  // (except those in the initial queues)
  for (x = _bb.x1; x <= _bb.x2; x++) {
    for (y = _bb.y1; y <= _bb.y2; y++) {
      for (z = (global) ? 3 : 1; z < base->Z; z++) {
        pmap = map (nodeCoord.set (x, y, z));
        if (pmap && (!(*pmap))) *pmap = (char)1;
      }
    }
  }


  breakLoop  = false;
  currentPri = nextPri (currentPri);

  // And here we go!
  while (true) {
    // Checks if the current border is finished. If so swap it with the
    // nextBorder. If the current border is still empty, we are done.
    if (currentBorder->empty ()) {
      currentPri = nextPri (currentPri);
      swap (currentBorder, nextBorder);

      if (currentBorder->empty ()) {
        breakLoop = true;
      }
    }
    if (breakLoop) break;

    pCoord = currentBorder->front ();
    currentBorder->pop ();

    x = pCoord->x ();
    y = pCoord->y ();
    z = pCoord->z ();

    delete pCoord;

    for (edge = 0; edge < 6; edge++) {
      nx = x; ny = y; nz =z;

      // Look at all six neighbors.
      switch (edge) {
        case 0: nx -= 1; break;
        case 1: nx += 1; break;
        case 2: ny -= 1; break;
        case 3: ny += 1; break;
        case 4: nz -= 1; break;
        case 5: nz += 1; break;
      }

      try { pmap = map (nodeCoord.set (nx, ny, nz)); }
      catch (coord_outbound &e) { continue; }

      // Usable nodes have been set to at least one, if not skip it.
      if ( (pmap == NULL) || (*pmap == (char)0) ) continue;

      if (*pmap == (char)1) {
        *pmap = currentPri;
        // Push only nodes that are not of minimal priority.
        if (currentPri > (char)1)
          nextBorder->push (new CCoord (nodeCoord));
      }
    }

  }


  cleared = false;
}




// -------------------------------------------------------------------
// Method  :  "CMapPri::cmp()".

bool CMapPri::cmp (int pri, CCoord &coord)
{
  char  mappri;


  mappri = (*this)[coord];

  if (!mappri) return (true);
  if (!pri)    return (false);

  return (pri + 256 >= mappri + delta);
}




// -------------------------------------------------------------------
// Friend  :  "&operator<<()".

ostream &operator<< (ostream &o, CMapPri *self)
{
  int     x, y, z;
  CCoord  nodeCoord;

  o << "cleared := " << self->cleared << "\n"
    << "offset  := " << self->offset  << "\n"
    << "delta   := " << self->delta   << "\n";

  for (z = 1; z < self->base->Z; z++) {
    o << "  (layer) z := " << z << "\n";

    for (y = 1; y <= self->base->Y; y++) {
      o << "    ";
      for (x = 0; x < self->base->X; x++) {
        o << setw(5) << (int)((*self)[
           nodeCoord.set (x, self->base->Y - y, z)]);
      }
      o << "\n";
    }
  }

  return (o);
}
