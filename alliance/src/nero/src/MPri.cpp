
// -*- C++ -*-
//
// $Id: MPri.cpp,v 1.3 2002/10/17 21:57:27 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./MPri.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "MDefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Method  :  "CMatrixPri::findfree()".

void  CMatrixPri::findfree (int index, CNet &net)
{
  CDRGrid::iterator  coord;
                int  radius, i, j, cx, cy;
               bool  freed;
               CNet *other;


  freed  = false;
  radius = 0;
  cx     = _drgrid->x(index);
  cy     = _drgrid->y(index);
  coord  = _drgrid->origin;

  while (!freed) {
    radius += 1;

    for (i = cx - radius; i < cx + radius + 1; i++) {
      for (j = cy - radius; j < cy + radius + 1; j++) {
        // Proccess only nodes of the ring.
        // if ( (i > cx - radius) || (i < cx + radius) ) continue;
        // if ( (j > cy - radius) || (j < cy + radius) ) continue;

        coord.set (i,j,2);

        // Check if we are outside.
        if (coord.outside()) continue;

        other = (*_drgrid->nodes)[coord].data.owner;
        if (    ( ! (*_drgrid->nodes)[coord].data.obstacle )
            && ((other == NULL) || (other == &net)) ) {
          if (!freed)
            cdebug << "Escape found at " << coord << "\n";

          freed = true;
        }

        (*this)[ coord.set(i,j,1) ] = (char)1;
        (*this)[ coord.set(i,j,2) ] = (char)1;
      }
    }
  }

  cdebug << "Escape radius := " << radius << "\n";
}




// -------------------------------------------------------------------
// Method  :  "CMatrixPri::clear()".

void CMatrixPri::clear (void)
{
  memset (_grid, (char)0, (size_t)(_drgrid->size));

  cleared = true;
  offset  = 0;
  delta   = 0;
}




// -------------------------------------------------------------------
// Method  :  "CMatrixPri::nextPri()".

char CMatrixPri::nextPri (char curpri)
{
  return ((curpri > 1) ? (curpri >> 1) : 1);
}




// -------------------------------------------------------------------
// Method  :  "CMatrixPri::load()".

void CMatrixPri::load (CNet &net, bool global, int expand=0)
{
   list<CDRGrid::iterator>::iterator   itNode, beginNode, endNode;
  queue<CDRGrid::iterator*>            queue1, queue2;
  queue<CDRGrid::iterator*>           *currentBorder, *nextBorder;
        CDRGrid::iterator             *pCoord, coord;

  CNet *other;
  char  currentPri, *pmap;
  int   x, y, z, nx, ny, nz, edge, id, ex, ey;
  bool  breakLoop;


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

  _bb.x1 = max (0             , net.bb.x1 - ex);
  _bb.x2 = min (_drgrid->X - 1, net.bb.x2 + ex);
  _bb.y1 = max (0             , net.bb.y1 - ey);
  _bb.y2 = min (_drgrid->Y - 1, net.bb.y2 + ey);


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
      coord = *itNode;

      // Initial queues filling.
      if (coord.z() > 0) {
        (*this)[ coord ] = currentPri; 
        currentBorder->push (new CDRGrid::iterator (coord));
        
        // Enable z=1 (in case of global signal, no effet otherwise).
        if (coord.z() < _drgrid->Z - 1) (*this)[ coord.dz(1) ] = (char)1; 

        if (global) {
          // Check for blocked upper nodes.
          other = (*_drgrid->nodes)[coord].data.owner;
          if (    (*_drgrid->nodes)[coord].data.obstacle
              || ((other != NULL) && (other != &net)) ) {
            cdebug << "Looking for an escape!\n" << "\n";
            findfree (coord, net);
          }
        }

        continue;
      }

      (*this)[ coord.dz(1) ] = nextPri (currentPri); 
      nextBorder->push (new CDRGrid::iterator (coord));

      if (global) {
        // Check for blocked upper nodes.
        other = (*_drgrid->nodes)[coord].data.owner;
        if (    (*_drgrid->nodes)[coord].data.obstacle
            || ((other != NULL) && (other != &net)) ) {
          cdebug << "Looking for an escape!\n" << "\n";
          findfree (coord, net);
        }
      }
        
      // Enable z=2 (in case of global signal, no effet otherwise).
      (*this)[ coord.dz(1) ] = (char)1; 

      // Look if the upper node is blocked, in that case expand the
      // allowed zone till a non-blocked node is found.
      if (global) {
        other = (*_drgrid->nodes)[coord].data.owner;
        if (    (*_drgrid->nodes)[coord].data.obstacle
            || ((other != NULL) && (other != &net)) ) {
          cdebug << "Looking for an escape!\n" << "\n";
          findfree (coord, net);
        }
      }
    }
  }


  // Set to one all the points inside the enclosing box.
  // (except those in the initial queues)
  cdebug << _bb << "\n";
  for (x = _bb.x1; x <= _bb.x2; x++) {
    for (y = _bb.y1; y <= _bb.y2; y++) {
      for (z = (global) ? 3 : 1; z < _drgrid->Z; z++) {
        pmap = & ( (*this)[ coord.set (x, y, z) ] );
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

      pmap = & ( (*this)[ coord.set (nx, ny, nz) ] );
      if (pmap == &(this->hole)) { continue; }

      // Usable nodes have been set to at least one, if not skip it.
      if ( (pmap == NULL) || (*pmap == (char)0) ) continue;

      if (*pmap == (char)1) {
        *pmap = currentPri;
        // Push only nodes that are not of minimal priority.
        if (currentPri > (char)1)
          nextBorder->push (new CDRGrid::iterator (coord));
      }
    }

  }


  cleared = false;
}




// -------------------------------------------------------------------
// Method  :  "CMatrixPri::take()".

bool CMatrixPri::take (int pri, int index)
{
  char  *mappri;


  mappri = &(*this)[index];

  if (mappri == &hole) return (false);
  if (!*mappri)        return (false);
  if (!pri)            return (true);

  return (pri + 256 < *mappri + delta);
}




// -------------------------------------------------------------------
// Friend  :  "&operator<<()".

ostream &operator<< (ostream &o, CMatrixPri &self)
{
  CDRGrid::iterator  coord;
                int  x, y, z;

  o << "cleared := " << self.cleared << "\n"
    << "offset  := " << self.offset  << "\n"
    << "delta   := " << self.delta   << "\n";

  coord = self._drgrid->origin;

  for (z = 1; z < self._drgrid->Z; z++) {
    o << "  (layer) z := " << z << "\n";

    for (y = 1; y <= self._drgrid->Y; y++) {
      o << "    ";
      for (x = 0; x < self._drgrid->X; x++) {
        o << setw(5) << (int)(self[
               coord.set (x, self._drgrid->Y - y, z)]);
      }
      o << "\n";
    }
  }

  return (o);
}
