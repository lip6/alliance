
// -*- C++ -*-
//
// $Id: AAstar.cpp,v 1.10 2005/04/07 14:56:18 jpc Exp $
//
//  +----------------------------------------------------------------+ 
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./AAStar.cpp"                            |
//  +----------------------------------------------------------------+




# include  "ADefs.h"




//  +----------------------------------------------------------------+
//  |                     Methods Definitions                        |
//  +----------------------------------------------------------------+


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

void  CAStar::CNodeASSet::check (bool cleared)
{
  int  chunk, index, maxindex;


  cerr << "+   Check all CNodeAS ( used := " << _maxused
       << ", allocated := " << _maxalloc << endl;

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
        if (cleared) {
          cerr << "+     Not reset CNodeAS found (id := "
               << _chunks[chunk][index].id
               << " " << (void*)&(_chunks[chunk][index])
               << " (point := " << _chunks[chunk][index].point << ")"
               << endl;
        }
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
  if (_maxused > 0) {
    int maxused_div = (_maxused - 1) / D::CHUNK_SIZE;
    int maxused_mod = (_maxused - 1) % D::CHUNK_SIZE;

    for (int chunk = 0; chunk <= maxused_div; chunk++) {
      int maxindex = D::CHUNK_SIZE - 1;

      // Incomplete last chunk.
      if (chunk == maxused_div) maxindex = maxused_mod;

      for (int index = 0; index <= maxindex; index++) {
        _chunks[chunk][index].reset ();
      }
    }
  }

  _maxused = 0;
}




// -------------------------------------------------------------------
// Constructor  :  "CAStar::CNodeAS::CNodeAS()".

CAStar::CNodeAS::CNodeAS (CDRGrid::iterator &pos)
  : point   (pos)
  , back    (NULL)
  , distance(0)
  , remains (LONG_MAX)
  , queued  (false)
  , tagged  (false)
  , intree  (false)
  , id      (-1)
{
  point.node().algo = (void*)this;

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
      NS._chunks.push_back( static_cast<CNodeAS*>(malloc(sizeof (CNodeAS) * D::CHUNK_SIZE)) );

      NS._maxchunk++;
      new_chunk = true;
    }

    // Use a new element.
    NS._maxused++;

  } while (    !new_chunk
           && (NS._maxused <= NS._maxalloc)
           &&  NS._chunks[chunk][index].intree );

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


  if ( !intree ) {
    if ( point.inside() ) {
      point.node().algo = NULL;
      point.unlink ();
    }
  }
}




// -------------------------------------------------------------------
// Method  :  "CAStar::CNodeAS::successors()".

void  CAStar::CNodeAS::successors (CNodeASSet &NS, CNet *net, CNodeAS *(*success)[6])
{
  CDRGrid::iterator  neighbor;
  CDRGrid::iterator  neighbor2;
            CNodeAS *pNodeAS;

  int   cost_x, cost_y, cost_z, cost, edge, i, j;
  long  new_distance, new_remains;
  bool  skip;



  //cdebug << "+ CAStar::CNodeAS::successors()." << "\n";

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

  for (edge = 0; edge < 6; edge++) (*success)[edge] = NULL;

  for (edge = 0; edge < 6; edge++) {
    cdebug << "+     successors[] : (before edge " << edge << ")\n";
    for (j = 0; j < 6; j++) {
      cdebug << "+       " << (void*)(*success)[j];
      if ((*success)[j] != NULL)
        cdebug << " " << (*success)[j]->point << "\n";
      else
        cdebug << "\n";
    }

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

      // For z==0 (ALU1), the only allowed neighbor is the "top",
      // as we must never use z==0 layer to route even contiguous pins.
      if ((point.z() == 0) && edge < 5) continue;

      pNodeAS = AS (neighbor);
      if (!pNodeAS) {
        pNodeAS = new (NS) CNodeAS (neighbor);
        cdebug << "+     new CNodeAS " << (void*)pNodeAS << "\n";
      }

      // Check if the node is an obstacle.
      if (neighbor.node().data.obstacle) {
        cdebug << "+     neighbour is an obstacle." << "\n";
        continue;
      }

      // Check the tag map (already reached if tagged.)
      if (pNodeAS->tagged) continue;

      if (neighbor.z()) {
        // Check if the current net can take the node.
        if (!neighbor.take (neighbor.node().data.pri)) continue;

        // For the junction layer, allow up neighbor (i.e. VIAs)
        // only on the 2 pitch grid.
        if (   ((neighbor.z() == neighbor.zupper()    ) && (edge == 5))
            || ((neighbor.z() == neighbor.zupper() - 1) && (edge == 4)) ) {
          if (neighbor.zupper() % 2) {
            if (neighbor.x() % 2) continue;
          } else {
            if (neighbor.y() % 2) continue;
          }
        }

        if (neighbor.z() >= neighbor.zupper ()) {
          skip = false;

          for (i = 0; i < 2; i++) {
            neighbor2 = neighbor;

            switch (i) {
              case 0: neighbor2.pprev (); break;
              case 1: neighbor2.pnext (); break;
            }

            if (neighbor2.inside()) {
              if (    neighbor2.node().data.obstacle
                  || !neighbor2.take (neighbor2.node().data.pri) ) {
                skip = true; break;
              }
            }
          }

          if (skip) continue;
        }
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

        cdebug << "+     Added to successor list [" << edge << "]\n";
        cdebug << "+       " << (void*)pNodeAS << " " << neighbor << "\n";
        cdebug << "+       " << (void*)pNodeAS << " " << pNodeAS->point << "\n";
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
  //cdebug << "+   Adding terminal nodes to the tree." << endl;

  endNode = term.nodes.end ();
  for (itNode = term.nodes.begin (); itNode != endNode; itNode++) {
    CNodeAS* pNodeAS = AS(*itNode);
    if (!pNodeAS)
      pNodeAS = new (*_NS) CNodeAS (*itNode);

    addnode (pNodeAS);
    pNodeAS->reset();

    if (pNodeAS->point.outside ()) {
      cerr << "+ TERMINAL NODE OUTSIDE : "
           << (void*)pNodeAS
           << " " << *itNode
           << endl;
    }
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
  : _NS               ()
  , _tree             ()
  , _queue            ()
  , _skip             (false)
  , _trapped          (false)
  , _reached          (NULL)
  , net               (NULL)
  , iterations        (0)
  , iterations_route  (0)
  , iterations_reroute(0)
  , _drgrid           (drgrid)
  , _netsched         (netsched)
{
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

void  CAStar::load (CNet *pNet, int delta, int expand)
{
  net        = pNet;

  _skip = false;
  if (net->size < 2) { _skip = true; return; }

  _drgrid->pri->load (*pNet, pNet->global(_netsched->rglobal), expand);
  _drgrid->pri->delta = delta;

  net->unroute ();
  _tree.addterm (*(net->terms[0]));

  cdebug << "    Starting term := \""
         << net->terms[0]->name
         << "\"\n";
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

  cdebug << "+   Examining " << pNodeAS->point << "\n";

  // We process the node : tag it.
  if (D::optim_AStar_queue) pNodeAS->tagged = true;

  pNodeAS->successors (_NS, net, &successors);

  for (edge = 0; edge < 6; edge++) {
    if (successors[edge] == NULL) continue;

    cdebug << "+     " << "[" << edge << "] "
           << (void*)successors[edge] << " "
           << successors[edge]->point << "\n";
    // The successor belongs to the current net.
    // (it may be not the actual target).
    if (   (successors[edge]->point.node().data.owner == net)
        && (_tree.reached.find (successors[edge]->point.node().getid())
            == _tree.reached.end())) {
      //cerr << "    Reached target := \""
      //     << net->terms[successors[edge]->point.node().getid()]->name
      //     << " (index := " << successors[edge]->point.node().getid() << ")"
      //     << endl;
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
  if ((long)_tree.reached.size () >= net->size) return (false);

  // Reset all the nodeAS objects.
  _NS.reset ();
  //net->_drgrid->nodes->check ();

  // Select the next target.
  endSet = _tree.reached.end ();
  for (i = 0; i < net->size; i++) {
    if (_tree.reached.find (i) == endSet) {
      _tree.settarget ( net->terms[i]->lowest() );
      cdebug << "    Next target := \""
             << net->terms[i]->name
             << " (index := " << i << ")"
             << "\n";
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
  CDRGrid::iterator  neighbor;
            CNodeAS *node;
               CNet *del_net;
                int  i;


  //cdebug << "+   Backtracking." << endl;

  // Do the backtracking.
  node = _reached->back;

  while (node->back != NULL) {
    if (node->point.node().data.pri > 0) {
      del_net = node->point.node().data.owner;
      del_net->unroute ();
      _netsched->queue (del_net);
    }

    if (node->point.z() >= node->point.zupper()) {
      for (i = 0; i < 2; i++) {
        neighbor = node->point;

        switch (i) {
          case 0: neighbor.pnext(); break;
          case 1: neighbor.pprev(); break;
        }

        if (neighbor.inside() && (neighbor.node().data.pri > 0)) {
          del_net = neighbor.node().data.owner;
          del_net->unroute ();
          _netsched->queue (del_net);
        }
      }
    }

    _tree.addnode (node);

    CNodeAS* node_prev = node;
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

      //check (false);
    }

    return (false);
  }

  catch (trapped_astar &e) {
    abort ();
    cmess2 << "              > AStar unable to find a path.\n";

    //check (false);
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

  void CAStar::route (CNet *pNet) throw (reach_max_pri)
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

  //if (pNet->name == "cmdy_ts6") cdebug.on ();

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

  if (increase >= 15) throw reach_max_pri (pNet);

  if (routed) dump();

  clear ();

  //check (true);

  //if (pNet->name == "cal_q_alu 0") { emergency (); exit (1); }
}




// -------------------------------------------------------------------
// Method  :  "CMatrixNodes::check()".

void  CMatrixNodes::check (bool cleared)
{
  int      index;


  cerr << "+   Check Nodes Matrix.\n";
  for (index = 0; index < _drgrid->XYZ; index++) {
    if ( &(*this)[index] == &hole ) continue;

    if (! (*this)[index].check(cleared))
      cerr << "+     Grid point := (" << _drgrid->x(index)
           << "," << _drgrid->y(index)
           << "," << _drgrid->z(index) << "))"
           << endl;
  }
}




// -------------------------------------------------------------------
// Method  :  "CNode::check()".

bool  CNode::check (bool cleared)
{
  if ( algo != NULL ) {
    if (cleared)
      cerr << "+   Residual algo structure found !" << endl;

    CAStar::CNodeAS *nodeAS = (CAStar::CNodeAS*) algo; 

    if ( nodeAS->point.outside() ) {
      cerr << "+     Incoherent algo structure found (node := "
           << this << ") : "
           << nodeAS << " (id := " << nodeAS->id << ")"
           << endl;

      return ( false );
    }
  }

  return ( true );
}




// -------------------------------------------------------------------
// Method  :  "CAStar::check()".

void  CAStar::check (bool cleared)
{
  cerr << "+ Check routing DB.\n";

  _drgrid->nodes->check (cleared);
  _NS.check (cleared);
}
