
// -*- C++ -*-
//
// $Id: MNet.cpp,v 1.2 2002/10/13 14:22:47 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./MNet.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "MDefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Constructor  :  "CTerm::CTerm()".

CTerm::CTerm (string termName, int ident)
{
  id   = ident;
  name = termName;
}




// -------------------------------------------------------------------
// Destructor  :  "CTerm::~CTerm()".

CTerm::~CTerm (void)
{
  //list<CNode*>::iterator  pNode;


  //for (pNode = nodes.begin (); pNode != nodes.end (); pNode++) {
  //  if ((*pNode)->coord.z() == 0) delete (*pNode);
  //}

  nodes.clear ();
}




// -------------------------------------------------------------------
// Method  :  "CTerm::distance()".

int  CTerm::distance (CTerm &other)
{
  list<CDRGrid::iterator>::iterator  itNode1, itNode2, endNode1, endNode2;

  int  minimum;


  minimum  = INT_MAX;
  endNode1 = nodes.end ();
  endNode2 = other.nodes.end ();

  for (itNode1 = nodes.begin (); itNode1 != endNode1; itNode1++) {
    for (itNode2 = other.nodes.begin (); itNode2 != endNode2; itNode2++) {
      minimum = min (minimum, (*itNode1).manhattan (*itNode2));
    }
  }

  return (minimum);
}




// -------------------------------------------------------------------
// Method  :  "CTerm::nearest()".

CTerm &CTerm::nearest (CTerm &term1, CTerm &term2)
{
  if (distance(term1) < distance(term2)) return (term1);

  return (term2);
}




// -------------------------------------------------------------------
// Method  :  "CTerm::lowest()".

CDRGrid::iterator &CTerm::lowest (void)
{
  list<CDRGrid::iterator>::iterator  itNode, itLowest;
       CDRGrid::iterator             Top;

  int  lowpri;


  itNode   = nodes.begin ();
  itLowest = itNode;

  if ( !(*itNode).z() ) lowpri = (*itNode).pri ();
  else                  lowpri = INT_MAX;

  // Find the lowest priority node above the terminals.
  for (; itNode != nodes.end (); itNode++) {
    if ( !(*itNode).z() ) {
      Top = *itNode;
      Top.top ();

      if (Top.pri() < lowpri) {
        itLowest = itNode;
        lowpri   =  Top.pri ();
      }
    }
  }

  return (*itLowest);
}




// -------------------------------------------------------------------
// Method  :  "CTerm::newaccess()".

CNode *CTerm::newaccess (int x, int y, int z, int ident, CNet *net) throw (dup_term)
{
  list<CDRGrid::iterator>::iterator  itNode;
       CDRGrid::iterator             coord;

  CNode *pNode;


  coord = net->_drgrid->origin;
  coord.set (x, y, z);

  // Check if the node is already in the list...
  for (itNode = nodes.begin (); itNode != nodes.end (); itNode++) {
    if (*itNode == coord) {
      throw dup_term (name, *itNode);
    }
  }

  pNode = &coord.node ();
  if ((z == 0) && coord.isnodehole()) {
    pNode = &coord.addnode ();
  }

  pNode->data.owner    = net;
  pNode->data.obstacle = false;
  pNode->setid (ident);
  nodes.push_back (coord);

  return (pNode);
}




// -------------------------------------------------------------------
// Method  :  "CTerm::newaccess()".

void  CTerm::newaccess (CRect &rect, int z, int ident, CNet *net)
{
  int  x, y;


  for (x = rect.x1; x <= rect.x2; x++) {
    for (y = rect.y1; y <= rect.y2; y++) {
      newaccess (x, y, z, ident, net);
    }
  }

  if (z > 0) return;

  // Only z=0 rectangles are stored (ALU1 is not in the grid).
  //if ((rect.x1 != rect.x2) || (rect.y1 != rect.y2))
  // No! Store all ALU1 rectangles.
  rects.push_back (rect);
}




// -------------------------------------------------------------------
// Method  :  "CTerm::lockalone()".

void  CTerm::lockalone (bool global=false)
{
  CDRGrid::iterator  coord;
                int  zCoord, zMax, z;


  if (nodes.size() != 1) return;

  coord  = nodes.back ();
  zCoord = coord.z();
  zMax   = min(coord._drgrid->Z - 1, 3);

  if ( (zCoord > 0) && !global ) return;

  for (z = (global) ? zMax : 1; z > zCoord; z--) {
    newaccess ( coord.x()
              , coord.y()
              , z
              , coord.node().getid()
              , coord.node().data.owner
              );
  }
}




// -------------------------------------------------------------------
// Method  :  "CTerm::setid()".

void  CTerm::setid (int ident)
{
  list<CDRGrid::iterator>::iterator  itNode;


  id = ident;

  // Reset the id on all the nodes.
  for (itNode = nodes.begin (); itNode != nodes.end (); itNode++) {
    (*itNode).node().setid (ident);
  }
}




// -------------------------------------------------------------------
// Friend  :  "operator<<()".

ostream  &operator<< (ostream &o, CTerm &self)
{
  list<CDRGrid::iterator>::iterator  endPoint, itPoint;


  o << "  term \"" << self.name << "\", id := " << self.id << " :\n";

  endPoint = self.nodes.end ();
  for (itPoint = self.nodes.begin (); itPoint != endPoint; itPoint++)
    o << "    " << *itPoint << "\n";

  return (o);
}




// -------------------------------------------------------------------
// Constructor  :  "CNet::CNet()".

CNet::CNet (CDRGrid *drgrid, string netName="noname")
{
  name     = netName;
  external = false;
  pri      = 0;
  size     = 0;
  rtree    = NULL;
  _drgrid  = drgrid;
}




// -------------------------------------------------------------------
// Destructor  :  "CNet::~CNet()".

CNet::~CNet (void)
{
  int  i;


  for (i = 0; i < size; i++) {
    delete (terms[i]);
  }
}




// -------------------------------------------------------------------
// Operator  :  "CNet::operator<()".

bool CNet::operator< (CNet &other)
{
  return (this->bb.hp > other.bb.hp);
}




// -------------------------------------------------------------------
// Method  :  "CNet::newaccess()".

void CNet::newaccess (string termName, int x, int y, int z)
{
  CRect  rect;


  rect.x1 = rect.x2 = x;
  rect.y1 = rect.y2 = y;

  newaccess (termName, rect, z);
}




// -------------------------------------------------------------------
// Method  :  "CNet::newaccess()".

void CNet::newaccess (string termName, CRect &rect, int z)
{
  CCoord  coord;
  int     id;


  // Find the terminal in the table.
  for (id = 0; id < size; id++) {
    if (terms[id]->name.compare (termName)) continue;

    break;
  }

  // Terminal not found : add a new one.
  if (id == size) {
    terms.push_back (new CTerm (termName, size));
    size += 1;
  }

  // Add the access to the terminal & update the bounding box.
  terms[id]->newaccess (rect, z, id, this);

  // Update the bounding box.
  bb.merge (coord.set (rect.x1, rect.y1, z));
  bb.merge (coord.set (rect.x2, rect.y2, z));

}





// -------------------------------------------------------------------
// Method  :  "CNet::lockalone()".

void CNet::lockalone (void)
{
  int  id;

  
  for (id = 0; id < size; id++) terms[id]->lockalone (global());
}





// -------------------------------------------------------------------
// Method  :  "CNet::locktree()".

void CNet::locktree (void)
{
  CNode *node;


  for (node = rtree; node != NULL; node = node->data.rtree) node->lock ();
}





// -------------------------------------------------------------------
// Method  :  "CNet::order()".

void CNet::order (void)
{
  vector<CTerm*>::iterator  itTerm, endTerm, itUnord, endUnord, itNearest;
  vector<CTerm*>            unordered;
  int                       ident;


  // Do not sort less than 3 terminals.
  if (size < 3) return;

  ident = 0;

  terms.swap (unordered);
  terms.push_back (unordered.back());
  unordered.pop_back ();

  terms[ident]->setid (ident); ident++;

  while (unordered.size() > 0) {
    endTerm  = terms.end ();
    endUnord = unordered.end ();

    for (itTerm = terms.begin(); itTerm != endTerm; itTerm++) {
      itNearest = unordered.begin ();
      itUnord   = itNearest + 1;

      for (; itUnord != endUnord; itUnord++) {
        if (  (*itTerm)->distance (*(*itUnord))
            < (*itTerm)->distance (*(*itNearest)))
          itNearest = itUnord;
      }
    }

    terms.push_back (*itNearest);
    unordered.erase ( itNearest);

    terms[ident]->setid (ident); ident++;
  }
}





// -------------------------------------------------------------------
// Method  :  "CNet::unroute()".

void CNet::unroute (void)
{
  CNode *node, *next_node;


  node = rtree;
  while (node != NULL) {
    next_node = node->data.rtree;

    node->ungrab ();

    node = next_node;
  }

  rtree = NULL;
}




// -------------------------------------------------------------------
// Friend  :  "print()".

ostream &print (ostream &o, CNet *self)
{
  CDRGrid::iterator  coord;
                int  x, y, z, layer;


  coord = self->_drgrid->origin;

  o << "    +";
  for (x = 1; x <= self->_drgrid->X; x++) o << "-";
  o << "+\n";

  for (y = 1; y <= self->_drgrid->Y; y++) {
    o << "    |";
    for (x = 0; x < self->_drgrid->X; x++) {
      layer = 0;

      for (z = 1; z < self->_drgrid->Z; z++) {
        coord.set (x, self->_drgrid->Y - y, z);
        if (coord.node().data.owner == self)
          layer = z + 1;
      }

      if (layer) o << (char)('A' + layer - 1);
      else       o << ' ';
    }
    o << "|\n";
  }

  o << "    +";
  for (x = 0; x < self->_drgrid->X; x++) o << "-";
  o << "+\n";

  return (o);
}




// -------------------------------------------------------------------
// Friend  :  "operator<<()".

ostream  &operator<< (ostream &o, CNet *self)
{
  int  id;


  o << "net \"" << self->name << "\", size := " << self->size << " :\n";

  for (id = 0; id < self->size; id++) o << *( self->terms[id] );

  return (o);
}
