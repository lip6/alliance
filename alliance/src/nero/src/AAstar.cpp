
// -*- C++ -*-
//
// $Id: AAstar.cpp,v 1.1 2002/10/02 21:23:47 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./AAStar.cpp"                            |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "ADefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Constructor  :  "CAStar::CNodeASSet::CNodeASSet()".

CAStar::CNodeASSet::CNodeASSet (void)
{
  _maxalloc = 0;
  _maxchunk = 0;
  _maxused  = 0;
  target    = NULL;
}




// -------------------------------------------------------------------
// Destructor  :  "CAStar::CNodeASSet::~CNodeASSet()".
//
// Remove all allocated CNodeAS objects.

CAStar::CNodeASSet::~CNodeASSet (void)
{
  int  chunk, index, maxindex;


  for (chunk = 0; chunk < _maxchunk; chunk++) {
    if (chunk > _maxalloc / D::CHUNK_SIZE) {
      maxindex = 0;
    } else {
      if (chunk == _maxalloc / D::CHUNK_SIZE)
        maxindex = _maxalloc % D::CHUNK_SIZE;
      else
        maxindex = D::CHUNK_SIZE;
    }

    for (index = 0; index < maxindex; index++) {
      _chunks[chunk][index].point.node().algo = NULL;
    }

    free ((void*)_chunks[chunk]);
  }

  _maxalloc = 0;
  _maxused  = 0;
  _maxchunk = 0;
  _chunks.clear ();
}




// -------------------------------------------------------------------
// Allocator  :  "CAStar::CNodeASSet::check()".
//
// Check all allocated CNodeAS objects.

void  CAStar::CNodeASSet::check (void)
{
  int  chunk, index, maxindex;


  //cdebug << "+ Check all CNodeAS (" << _maxalloc << ")\n";

  for (chunk = 0; chunk < _maxchunk; chunk++) {
    if (chunk > _maxalloc / D::CHUNK_SIZE) {
      maxindex = 0;
    } else {
      if (chunk == _maxalloc / D::CHUNK_SIZE)
        maxindex = _maxalloc % D::CHUNK_SIZE;
      else
        maxindex = D::CHUNK_SIZE;
    }

    for (index = 0; index < maxindex; index++) {
      if ( _chunks[chunk][index].point.inside() ) {
        //cdebug << "+ Not reset CNodeAS found (id := "
        //     << _chunks[chunk][index].id
        //     << " (point := " << ")"
        //     << _chunks[chunk][index].point
        //     << endl;
      }
    }
  }
}




// -------------------------------------------------------------------
// Allocator  :  "CAStar::CNodeASSet::reset()".
//
// Reset all CNodeAS objects after an algorithm pass.

void  CAStar::CNodeASSet::reset (void)
{
  int  chunk, maxchunk, index, maxindex, maxused_div, maxused_mod;


  //cdebug << "+ CAStar::CNodeAS::resetall()." << endl;
  //cdebug << "+   _maxused  := " << _maxused << endl;
  //cdebug << "+   _maxalloc := " << _maxalloc << endl;
  //cdebug << "+   _maxchunk := " << _maxchunk << endl;


  if (_maxused > 0) {
    maxused_div = (_maxused - 1) / D::CHUNK_SIZE;
    maxused_mod = (_maxused - 1) % D::CHUNK_SIZE;

    maxchunk = maxused_div + ( (maxused_mod) ? 1 : 0 );
  } else {
    maxchunk = 0;
  }

  for (chunk = 0; chunk < maxchunk; chunk++) {
    maxindex = D::CHUNK_SIZE - 1;

    // Incomplete last chunk.
    if ( (chunk == maxchunk - 1) && (maxused_mod != 0) )
      maxindex = maxused_mod;

    //cdebug << "+   Index range := [" << chunk * D::CHUNK_SIZE
    //     << "," <<  chunk * D::CHUNK_SIZE + maxindex << "]"
    //     << " (chunk := " << chunk << ", maxindex := " << maxindex << ")"
    //     << endl;

    for (index = 0; index <= maxindex; index++) {
      //cdebug << "+     index := " << index
      //     << " (" << &_chunks[chunk][index] << ")"
      //     << endl;
      _chunks[chunk][index].reset ();
    }
  }

  _maxused = 0;
}




// -------------------------------------------------------------------
// Constructor  :  "CAStar::CNodeAS::CNodeAS()".

CAStar::CNodeAS::CNodeAS (CDRGrid::iterator &pos)
{
  point             = pos;
  point.node().algo = (void*)this;

  back              = NULL;
  distance          = 0;
  remains           = LONG_MAX;
  queued            = false;
  tagged            = false;
  intree            = false;

  //cdebug << "+   new CNodeAS (id := " << id << ", " << point << ")" << endl;
}




// -------------------------------------------------------------------
// Operator  :  "CAStar::CNodeAS::operator<".

bool CAStar::CNodeAS::operator< (const CNodeAS &other) const
{
  if (D::optim_AStar_order) {
    if (this->distance == other.distance)
      return (this->remains > other.remains);
  }

  return (this->distance > other.distance);
}




// -------------------------------------------------------------------
// Allocator  :  "CAStar::CNodeAS::operator new ()".

void *CAStar::CNodeAS::operator new (size_t size, CNodeASSet &NS)
{
  int   chunk, index;
  bool  new_chunk;


  new_chunk = false;

  // Return first element not part of the tree (flag "intree" set).
  do {
    chunk = NS._maxused / D::CHUNK_SIZE;
    index = NS._maxused % D::CHUNK_SIZE;

    // Allocate a new chunk.
    if (NS._maxchunk <= chunk) {
      NS._chunks.push_back ((CNodeAS*)malloc (sizeof (CNodeAS) * D::CHUNK_SIZE));

      NS._maxchunk++;
      new_chunk = true;
    }

    // Use a new element.
    NS._maxused++;

  } while (!new_chunk                        &&
           (NS._maxused <= NS._maxalloc) &&
           NS._chunks[chunk][index].intree );

  NS._maxalloc = max (NS._maxalloc, NS._maxused);
  NS._chunks[chunk][index].id = NS._maxused - 1;


  return ( (void*)( &(NS._chunks[chunk][index]) ) );
}




// -------------------------------------------------------------------
// Method  :  "CAStar::CNodeAS::reset()".

void CAStar::CNodeAS::reset (void)
{
  back     = NULL;
  distance = 0;
  remains  = LONG_MAX;
  queued   = false;
  tagged   = false;

  //CDRGrid::iterator  old_point;

  if ( !intree ) {
    if ( point.inside() ) {
      //cdebug << "+   reset CNodeAS (" << this
      //     << ") id := " << id << ", point " << point
      //     << " (node := " << &point.node() << ")"
      //     << endl;
      //old_point = point;
      point.node().algo = NULL;
      point.unlink ();

      //old_point.node().check ();
    //} else {
    // cdebug << "+   reset CNodeAS id := " << id << " has point outside!." << endl;
    }
  //} else {
  //  cdebug << "+   reset CNodeAS id := " << id << " is in tree." << endl;
  }
}




// -------------------------------------------------------------------
// Method  :  "CAStar::CNodeAS::successors()".

void  CAStar::CNodeAS::successors (CNodeASSet &NS, CNet *net, CNodeAS *(*success)[6])
{
  CDRGrid::iterator  neighbor;
               CNet *pNet;
            CNodeAS *pNodeAS;

  int   cost_x, cost_y, cost_z, cost, edge;
  long  new_distance, new_remains;



  //cdebug << "+ CAStar::CNodeAS::successors()." << endl;

  cost_z = D::cost_VIA;
  // Alternate costs of edges :
  // - z = 0 : ALU1 (unused).
  // - z = 1 : ALU2 (horizontal).
  // - z = 2 : ALU3 (vertical).
  // ...
  if (point.z() % 2) {
    cost_x = D::cost_ALU2_X;
    cost_y = D::cost_ALU2_Y;
  } else {
    cost_x = D::cost_ALU3_X;
    cost_y = D::cost_ALU3_Y;
  }

  for (edge = 0; edge < 6; edge++) {
    neighbor = point;
    switch (edge) {
      case 0: cost = cost_x; neighbor.left();   break; 
      case 1: cost = cost_x; neighbor.right();  break; 
      case 2: cost = cost_y; neighbor.down();   break; 
      case 3: cost = cost_y; neighbor.up();     break; 
      case 4: cost = cost_z; neighbor.bottom(); break; 
      case 5: cost = cost_z; neighbor.top();    break; 
    }

    (*success)[edge] = NULL;

    if (neighbor.inside() && !neighbor.isnodehole()) {
      //cdebug << "+   " << neighbor << endl;

      pNodeAS = AS (neighbor);
      if (!pNodeAS) {
        //cdebug << "+     new CNodeAS." << endl;
        pNodeAS = new (NS) CNodeAS (neighbor);
      }

      // Check if the node is an obstacle.
      if (neighbor.node().data.obstacle) continue;

      // Check the tag map (already reached if tagged.)
      if (pNodeAS->tagged) continue;

      if (neighbor.z()) {
        // Check if the current net can take the node.
        if (!neighbor.take (neighbor.node().data.pri)) continue;
      }

      // Case of locked nodes : belongs to global nets, never use it.
      if (    (neighbor.node().data.owner != net)
           && (neighbor.node().locked()         ) ) continue;

      // Case of terminal nets : look if it belongs to the net.
      if (    (neighbor.node().data.owner != net)
           &&  neighbor.node().terminal()         ) continue;

      // Check if the node is an already reached part of the
      // partially built routing tree.
      if (pNodeAS->isintree ()) continue;

      // Compute new node distance.
      new_remains  = neighbor.manhattan (*NS.target);
      new_distance = distance + cost + new_remains - point.manhattan (*NS.target);

      // If the node is in the priority queue (i.e. has not been popped
      // yet) and the new distance is shorter, push it a second time,
      // with the new priority.

      if (       !pNodeAS->queued
          ||     (pNodeAS->distance >  new_distance)
          || (   (pNodeAS->distance == new_distance)
              && (pNodeAS->remains  <  new_remains ))) {
        // Set backtracking & cost information.
        pNodeAS->back     = this;
        pNodeAS->distance = new_distance;
        pNodeAS->remains  = new_remains;

        // Tag the node here : prevent double write in queue.
        if (!D::optim_AStar_queue) pNodeAS->tagged = true;

        //cdebug << "+     Added to successor list." << endl;
        (*success)[edge] = pNodeAS;
      }
    }
  }
}




// -------------------------------------------------------------------
// Method  :  "CAStar::CTree::addterm()".

void CAStar::CTree::addterm (CTerm &term)
{
  list<CDRGrid::iterator>::iterator  endNode, itNode;
  CNodeAS* pNodeAS;


  //cdebug << "+   Adding terminal nodes to the tree." << endl;

  endNode = term.nodes.end ();
  for (itNode = term.nodes.begin (); itNode != endNode; itNode++) {
    pNodeAS = AS(*itNode);
    if (!pNodeAS)
      pNodeAS = new (*_NS) CNodeAS (*itNode);

    addnode (pNodeAS);
    pNodeAS->reset();

  }

  //cdebug << "+   Done." << endl;

  reached.insert (term.id);
}




// -------------------------------------------------------------------
// Method  :  "CAStar::CTree::settarget()".

void CAStar::CTree::settarget (CDRGrid::iterator &node)
{
  LNodeAS::iterator  endNodeAS, itNodeAS;


  endNodeAS = nodes.end ();
  for (itNodeAS = nodes.begin (); itNodeAS != endNodeAS; itNodeAS++) {
    (*itNodeAS)->distance =
    (*itNodeAS)->remains  = node.manhattan ((*itNodeAS)->point);
  }

  // Set the target for all CNodeAS objets. I don't like it.
  _NS->target = &node;
}




// -------------------------------------------------------------------
// Method  :  "CAStar::CTree::clear()".

void CAStar::CTree::clear (void)
{
  LNodeAS::iterator  endNodeAS, itNodeAS;


  endNodeAS = nodes.end ();
  for (itNodeAS = nodes.begin (); itNodeAS != endNodeAS; itNodeAS++) {
    //cdebug << "+ Removing from tree id := " << (*itNodeAS)->id << endl;
    (*itNodeAS)->unsettree ();
    (*itNodeAS)->reset     ();
  }

  reached.clear();
  nodes.clear();
}




// -------------------------------------------------------------------
// Method  :  "CAStar::CQueue::load()".

void CAStar::CQueue::load (CTree &tree, bool start)
{
  LNodeAS::iterator  endNodeAS, itNodeAS;


  endNodeAS = tree.nodes.end ();
  for (itNodeAS = tree.nodes.begin (); itNodeAS != endNodeAS; itNodeAS++) {
    if (!start && ((*itNodeAS)->point.z() == 0)) continue;

    push (*itNodeAS);
  }
}




// -------------------------------------------------------------------
// Constructor  :  "CAStar::CAStar()".

CAStar::CAStar (CDRGrid *drgrid, CASimple *netsched)
{
  _drgrid   = drgrid;
  _netsched = netsched;

  _tree._NS = &_NS;
}




// -------------------------------------------------------------------
// Modifiers  :  "CAStar::clear()".

void  CAStar::clear (void)
{
  if (_skip) { return; }

  net        = NULL;
  iterations = 0;

  _skip    = false;
  _trapped = false;
  _reached = NULL;

  _drgrid->pri->clear ();
  _queue.reset ();
  _tree.clear ();
  _NS.reset ();
}




// -------------------------------------------------------------------
// Modifiers  :  "CAStar::load()".

void  CAStar::load (CNet *pNet, int delta=0, int expand=0)
{
  net        = pNet;

  _skip = false;
  if (net->size < 2) { _skip = true; return; }

  _drgrid->pri->load (*pNet, _netsched->rglobal && pNet->global(), expand);
  _drgrid->pri->delta = delta;

  net->unroute ();
  _tree.addterm (*(net->terms[0]));
}




// -------------------------------------------------------------------
// Method  :  "CAStar::step()".

bool  CAStar::step (void) throw (trapped_astar)
{
  CNodeAS *successors[6], *pNodeAS;
      int  edge;


  //cdebug << "+ CAStar::step()." << endl;

  iterations++;

  // Pop node until :
  //     1. - The queue is emptied : we cannot route the signal.
  //     2. - An untagged node is found, that is a node which is not
  //            a duplicata of an already processed node.
  do {
    if (_queue.empty()) {
      //emergency (); exit (1);
      throw trapped_astar(net);
    }

    pNodeAS = _queue.pop ();

  } while ( pNodeAS->tagged && D::optim_AStar_queue );

  //cdebug << "+   " << pNodeAS->point << endl;

  // We process the node : tag it.
  if (D::optim_AStar_queue) pNodeAS->tagged = true;

  pNodeAS->successors (_NS, net, &successors);

  for (edge = 0; edge < 6; edge++) {
    if (successors[edge] == NULL) continue;

    //cdebug << "+     " << successors[edge]->point << endl;
    // The successor belongs to the current net.
    // (it may be not the actual target).
    if (   (successors[edge]->point.node().data.owner == net)
        && (_tree.reached.find (successors[edge]->point.node().getid())
            == _tree.reached.end())) {
      _reached = successors[edge];
      return (false);
    }

    // Otherwise, put it into the queue.
    _queue.push (successors[edge]);
  }

  //cdebug << "+   Successors processed." << endl;


  return (true);
}




// -------------------------------------------------------------------
// Method  :  "CAStar::nexttarget()".

bool  CAStar::nexttarget (void)
{
  int                 i;
  set<int>::iterator  endSet;

  // Are all targets reacheds?
  if (_tree.reached.size () >= net->size) return (false);

  // Reset all the nodeAS objects.
  _NS.reset ();

  //CNodeAS::checkall ();
  //net->_drgrid->nodes->check ();

  // Select the next target.
  endSet = _tree.reached.end ();
  for (i = 0; i < net->size; i++) {
    if (_tree.reached.find (i) == endSet) {
      _tree.settarget ( net->terms[i]->lowest() );
      break;
    }
  }

  // Reset & init the queue.
  _queue.reset ();
  _queue.load  (_tree, (_tree.reached.size() == 1));


  return (true);
}




// -------------------------------------------------------------------
// Method  :  "CAStar::backtrack()".

void CAStar::backtrack (void)
{
  CNodeAS *node, *node_prev;
  CNet    *del_net;


  //cdebug << "+   Backtracking." << endl;

  // Do the backtracking.
  node = _reached->back;

  while (node->back != NULL) {
    if (node->point.node().data.pri > 0) {
      del_net = node->point.node().data.owner;
      del_net->unroute ();
      _netsched->queue (del_net);
    }

    _tree.addnode (node);

    node_prev = node;
    node = node->back;

    node_prev->reset ();
  }


  // Add the reached terminal to the tree.
  _tree.addterm ( *( net->terms[_reached->point.node().getid()] ) );

  //cdebug << "+   Done." << endl;
}




// -------------------------------------------------------------------
// Method  :  "CAStar::search()".

bool CAStar::search (void)
{
  if (_skip) return (false);

  try {
    if (_drgrid->pri->delta) {
      cmess2 << "              > Re-routing with pri := "
             << _drgrid->pri->delta << ".\n";
    }

    while ( nexttarget() ) {
      for (; step(); );
  
      backtrack ();
    }

    //CNodeAS::checkall ();
    //net->_drgrid->nodes->check ();

    return (false);
  }

  catch (trapped_astar &e) {
    abort ();
    cmess2 << "              > AStar unable to found a path.\n";

    //CNodeAS::checkall ();
    //net->_drgrid->nodes->check ();
  }

  return (true);
}



// -------------------------------------------------------------------
// Method  :  "CAStar::abort()".

void CAStar::abort (void)
{
  _tree.clear();
}



// -------------------------------------------------------------------
// Method  :  "CAStar::dump()".

void CAStar::dump (void)
{
  LNodeAS::iterator  endNode, itNode;
  CDRGrid::iterator  point;
  int                num_nodes;


  if (_skip) return;

  num_nodes = 0;
  endNode = _tree.nodes.end ();
  for (itNode = _tree.nodes.begin (); itNode != endNode; itNode++) {
    point = (*itNode)->point;
    point.node().grab (net, point.pri() + _drgrid->pri->delta, point);
    num_nodes++;
  }

  _tree.clear();
}



// -------------------------------------------------------------------
// Method  :  "CAStar::route()".

void CAStar::route (CNet *pNet)
{
  int   pri;
  int   increase, expand;
  bool  hysteresis, routed;
  long *iterations_kind;


  hysteresis         = false;
  increase           = 0;
  expand             = 0;
  iterations_route   = 0;
  iterations_reroute = 0;
  iterations_kind    = &iterations_route;

  //if (pNet->name == "ctl.seq_ep_30") cdebug.on ();

  do {
    if (hysteresis) {
      clear ();

      pri = 255 + (1 << increase++);

      if (increase > 1) expand = 200;

      iterations_kind = &iterations_reroute;
    }
    else
      pri = 0;

    load  (pNet, pri, expand);

    routed = !search ();
    *iterations_kind += iterations;

    hysteresis = true;
  } while ((increase < 15) && !routed);

  if (routed) dump();

  clear ();

  //CNodeAS::checkall ();
  //pNet->_drgrid->nodes->check ();

  //if (pNet->name == "ctl.seq_ep_30") exit (0);
}




// -------------------------------------------------------------------
// Method  :  "CMatrixNodes::check()".

void  CMatrixNodes::check (void)
{
  int      index;


  cdebug << "+ Check routing DB.\n";
  for (index = 0; index < _drgrid->XYZ; index++) {
    if ( &(*this)[index] == &hole ) continue;

    if ( ! (*this)[index].check() )
      cdebug << " (point := (" << _drgrid->x(index)
           << "," << _drgrid->y(index)
           << "," << _drgrid->z(index) << "))\n";
                                      
  }
}




// -------------------------------------------------------------------
// Method  :  "CNode::check()".

bool  CNode::check (void)
{
  CAStar::CNodeAS *nodeAS;


  if ( algo != NULL ) {
    cdebug << "+ Residual algo structure found!\n";

    nodeAS = (CAStar::CNodeAS*) algo; 

    if ( nodeAS->point.outside() ) {
      cdebug << "+ Incoherent algo structure found (node := "
           << this << ") : "
           << nodeAS << " (id := " << nodeAS->id << ")";
      return ( false );
    }
  }

  return ( true );
}
