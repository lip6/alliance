
// -*- C++ -*-
//
// $Id: MNet.cpp,v 1.12 2006/04/28 07:51:54 jpc Exp $
//
//  +----------------------------------------------------------------+ 
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./MNet.cpp"                              |
//  +----------------------------------------------------------------+




# include  "MDefs.h"




//  +----------------------------------------------------------------+
//  |                     Methods Definitions                        |
//  +----------------------------------------------------------------+


// -------------------------------------------------------------------
// Constructor  :  "CTerm::CTerm()".

CTerm::CTerm (string termName, int ident)
  : id  (ident)
  , name(termName)
{ }




// -------------------------------------------------------------------
// Destructor  :  "CTerm::~CTerm()".

CTerm::~CTerm (void)
{
  //list<CNode*>::iterator  pNode;


  //for (pNode = nodes.begin (); pNode != nodes.end (); pNode++)
  //  (*pNode)->ungrab ();

  nodes.clear ();
  rects.clear ();
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

CNode *CTerm::newaccess (int x, int y, int z, int ident, CNet *net)
{
  list<CDRGrid::iterator>::iterator  itNode;
       CDRGrid::iterator             coord;

  CNode *pNode;

  coord = net->_drgrid->origin;
  coord.set (x, y, z);

  pNode = &coord.node ();
  if ((z == 0) && coord.isnodehole()) {
    pNode = &coord.addnode ();
  }

  if (pNode->data.owner) {
    // Check if the node has already been took by another terminal.
    if (pNode->data.owner != net)
      throw bad_grab ( pNode->data.owner->terms[pNode->getid()]->name
                     , net->name
                     , coord.x()
                     , coord.y()
                     , coord.z()
                     , 0
                     , pNode->data.pri
                     , pNode->terminal()
                     , pNode->data.ident
                     );

    // Check if the node belongs to another terminal of this net.
    // If so, send a merging exception to CNet::newaccess ().
    if (pNode->getid () != ident) throw merge_term ( pNode->getid()
                                                   , pNode->data.owner->terms[pNode->getid()]->name
                                                   , pNode->data.owner->terms[ident]->name
                                                   , net->name
                                                   , coord.x()
                                                   , coord.y()
                                                   , coord.z()
                                                   );

    return (NULL);
  }

  // Check if the node is already in the list (this should never appens !)
  for (itNode = nodes.begin (); itNode != nodes.end (); itNode++) {
    if (*itNode == coord) {
      throw dup_term (name, *itNode);
    }
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
  // if ((rect.x1 != rect.x2) || (rect.y1 != rect.y2))
  // No! Store all ALU1 rectangles.
  rects.push_back (rect);
}




// -------------------------------------------------------------------
// Method  :  "CTerm::merge()".

void CTerm::merge (CTerm *other, int ident, CNet *Net)
{
  list<CDRGrid::iterator>::iterator  coord;


  for (coord = other->nodes.begin (); coord != other->nodes.end (); coord++) {
    (*coord).node().setid (ident);
    nodes.push_back (*coord);
  }
}




// -------------------------------------------------------------------
// Method  :  "CTerm::lockalone()".

void  CTerm::lockalone (bool global)
{
  CDRGrid::iterator  coord;
  CDRGrid::iterator  coord2;

  if (nodes.size() != 1) return;


  coord  = nodes.back ();
  coord2 = coord;

  CDRGrid* drgrid = coord._drgrid;

  if ( (coord.z() > 0) && !global ) return;
  if (  coord.onAB()) return;

  //cerr << "+ locking lone terminal : " << coord.node().data.owner->name
  //     << " at " << coord
  //     << endl;

  // All terminal case, eat up z=1 (ALU2) if not already took.
  if (coord.z() == 0) {
    // Go to z=1 (ALU2).
    //cerr << "+   locking z=1          " << coord << endl;
    newaccess ( coord.x()
              , coord.y()
              , 1
              , coord.node().getid()
              , coord.node().data.owner
              );

    coord2.top();
  }

  if (!global) return;


  if (coord.z() < 2) {
    // Go to z=2 (ALU3).
    //cerr << "+   locking z=2          " << coord2 << endl;
    newaccess ( coord2.x()
              , coord2.y()
              , 2
              , coord.node().getid()
              , coord.node().data.owner
              );
  }


  if ( drgrid->Z < 3 ) return;
  // Global terminal : when zupper=4, find the nearest VIA on the
  // double pitch grid.

  // Is the terminal in ALU3 (or less).
  if ( (coord.z() < 3) && (coord.zupper () == 4) ) {
    if ( coord2.y() % 2 ) {
      // We are not on the double pitch grid. Try to go there.
      // Look for up and down grid node.
      bool adjust = true;

      for (int i = 0; i < 3; i++) {
        switch (i) {
          case 0: coord2.dy (+1); break;
          case 1: coord2.dy (-2); break;
        }

        // Neither node are accessibles, we are probably doomed ...
        if (i == 2) { coord2.dy (+1); adjust = false; break; }

        if (coord2.inside()) {
          if (    !coord2.node().data.obstacle
              &&  (   (coord2.node().data.owner == NULL )
                   || (coord2.node().data.owner == coord.node().data.owner) ) )
            { break; }
        }
      }

      if (adjust) {
        // Adjust to the double grid pitch to z=2 (ALU3).
        //cerr << "+   locking z=2 (ADJUST) " << coord2 << endl;
        newaccess ( coord2.x()
                  , coord2.y()
                  , 2
                  , coord.node().getid()
                  , coord.node().data.owner
                  );
      }
    }
  }

  if ( drgrid->Z < 4 ) return;

  if (coord.z() < 4) {
    // Go to z=3 (ALU3).
    //cerr << "+   locking z=3          " << coord2 << endl;
    newaccess ( coord2.x()
              , coord2.y()
              , 3
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

CNet::CNet (CDRGrid *drgrid, string netName)
  : pri(0)
  , name(netName)
  , terms()
  , rtree(NULL)
  , bb()
  , size(0)
  , external(false)
  , fixed(false)
  , _drgrid(drgrid)
{
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
// Method  :  "CNet::global()".

bool CNet::global (bool rglobal)
{
  return ((bb.hp >= D::GLOBAL_HP) && rglobal);
}




// -------------------------------------------------------------------
// Method  :  "CNet::newaccess()".

void CNet::newaccess (string termName, int x, int y, int z)
{
  if ( z >= _drgrid->Z ) return;

  CRect  rect;

  rect.x1 = rect.x2 = x;
  rect.y1 = rect.y2 = y;

  newaccess (termName, rect, z);
}




// -------------------------------------------------------------------
// Method  :  "CNet::newaccess()".

void CNet::newaccess (string termName, CRect &rect, int z)
{
  if ( z >= _drgrid->Z ) return;

  CCoord  coord;
  int     id, mergeid;


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
  do {
    mergeid = INT_MAX;

    try {
      terms[id]->newaccess (rect, z, id, this);
    }

    catch (merge_term &e) {
      if ( e.id > id ) {
        mergeid = id;
        id      = e.id;
      } else {
        mergeid = e.id;
      }
      cerr << "Merging terminals : [" << id << "] absorbed by [" << mergeid << "]" << endl;

      // Merge terminals ...
      terms[mergeid]->merge (terms[id], mergeid, this);

      // Erase the merged terminal.
      delete terms[id];
      terms.erase (terms.begin () + id);

      size -= 1;

      // Re-computes the terminal's ids.
      for (id = 0; id < size; id++) terms[id]->setid (id);
      cerr << "New terminal max id " << id-1 << endl;

      id = mergeid;
    }
  } while (mergeid != INT_MAX);

  // Update the bounding box.
  bb.merge (coord.set (rect.x1, rect.y1, z));
  bb.merge (coord.set (rect.x2, rect.y2, z));

}





// -------------------------------------------------------------------
// Method  :  "CNet::lockalone()".

void CNet::lockalone (bool rglobal)
{
  int  id;

  
  for (id = 0; id < size; id++) terms[id]->lockalone (global(rglobal));
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
