
// -*- C++ -*-
//
// $Id: ADefs.h,v 1.4 2008/06/11 09:20:34 jpc Exp $
//
// /-----------------------------------------------------------------\ 
// |                                                                 |
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ADefs.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// \-----------------------------------------------------------------/




# ifndef  __ADefs__
# define  __ADefs__  1


# include  "MDefs.h"




// -------------------------------------------------------------------
// Class declaration.

  class  CAStar;
  class  CASimple;



// -------------------------------------------------------------------
// Module  :  "AAstar.cpp".


  // ---------------------------------------------------------------
  // AStar trapped exception.

  class trapped_astar : public except_done {

    // Attributes.
    public: CNet *net;

    // Constructor.
    public: trapped_astar (CNet *pNet) { net = pNet; }

    // Overridables.
    public: const char* what () const throw () {
              return ((char*)"AStar algorithm can't find a path.");
            }
  };




  // ---------------------------------------------------------------
  // Unable to route exception.

  class no_route : public except_done {

    // Attributes.
    public: CNet *net;

    // Constructor.
    public: no_route (CNet *pNet) { net = pNet; }

    // Overridables.
    public: const char* what () const throw () {
              return ((char*)"No route.");
            }
  };




  // ---------------------------------------------------------------
  // Maximum priority reached exception.

  class reach_max_pri : public except_done {

    // Attributes.
    public: CNet *net;

    // Constructor.
    public: reach_max_pri (CNet *pNet) { net = pNet; }

    // Overridables.
    public: const char* what () const throw () {
              return ((char*)"Maximum priority reached in AStar.");
            }
  };




  // ---------------------------------------------------------------
  // AStar algorithm class.
  //
  // This class includes all subclasses it needs.

  class CAStar {

    public: class CNodeAS;
            class CNodeASSet;
            class CTree;
            class CQueue;


    typedef  list<CNodeAS*>  LNodeAS;


    // Additionnal datas needed on nodes. ------------------------
    public: class CNodeAS {

      // Attributes.
      public: CDRGrid::iterator  point;
      public: CNodeAS           *back;
      public: long               distance;
      public: long               remains;
      public: bool               queued;
      public: bool               tagged;
      public: bool               intree;
      public: int                id;

      // Operator.
      public: bool  operator< (const CNodeAS &other) const;

      // Constructor.
      public: CNodeAS (CDRGrid::iterator &pos);

      // Allocators.
      private: static void *operator new    (size_t size);
      public: static void  operator delete (void *zone);
      public:  static void *operator new    (size_t size, CNodeASSet &NS);

      // Modifiers.
      public:        void  reset     (void);
      public:        void  settree   (void) { intree = true;  }
      public:        void  unsettree (void) { intree = false; }

      // Accessors.
      public: void  successors (CNodeASSet &NS, CNet *net, CNodeAS *(*success)[6]);
      public: bool  isintree   (void) { return (intree); }

    };


    inline static CNodeAS *AS (CDRGrid::iterator  point)
                    { return ((CNodeAS*)(point.node().algo)); }


    // Heap Allocator for CNodeAS. -------------------------------
    class CNodeASSet {

      // Internal attributes.
      private: vector<CNodeAS*> _chunks;
      private: int              _maxchunk;
      private: int              _maxalloc;
      private: int              _maxused;

      // Shared attribute (ugly one).
      public: CDRGrid::iterator *target;

      // Constructor.
      public: CNodeASSet (void);

      // Destructor.
      public: ~CNodeASSet (void);

      // Modifiers.
      public: void  reset (void);
      public: void  check (bool cleared);

      // Friend class (for CNodeAS allocator).
      friend class CNodeAS;
 
    };


    // Partially build routing tree. -----------------------------
    class CTree {

      // Internal attributes.
      public: CNodeASSet *_NS;

      // Attributes.
      public: LNodeAS   nodes;
      public: set<int>  reached;

      // Modifiers.
      public: void addnode   (CNodeAS *node) {
                                node->settree ();
                                nodes.push_back (node);
                             }
      public: void addterm   (CTerm &term);
      public: void settarget (CDRGrid::iterator &node);
      public: int  size      (void) { return (reached.size()); }
      public: void clear     (void);

    };




    // Priority queue. -------------------------------------------
    class CQueue {

      // Queue element class.
      class CQelem {

        // Attributes.
        public: CNodeAS *node;

        // Constructor.
        public: CQelem (CNodeAS *pNode=NULL) { node = pNode; }

        // Operator : ordering function.
        public: bool operator< (const CQelem &other) const {
                  return ((*this->node) < (*other.node));
                }
      };

      // Attribute.
      private: priority_queue<CQelem>  queue;

      // Accessors.
      public: CNodeAS* pop (void) {
                         CQelem  elem;

                         elem = queue.top ();
                         elem.node->queued = false;
                         queue.pop ();

                         return (elem.node);
                       }
      public: void     push  (CNodeAS *node) {
                         node->queued = true;

                         queue.push(CQelem (node));
                       }
      public: bool     empty (void) { return (queue.empty()); }
      public: void     reset (void) { while (!empty()) pop(); }
      public: void     load  (CTree &tree, bool start);

    };




    // Internal CAStar attributes : the state of the algorithm.
    private: CNodeASSet _NS;
    private: CTree      _tree;
    private: CQueue     _queue;
    private: bool       _skip;
    private: bool       _trapped;
    private: CNodeAS   *_reached;

    // Attributes.
    public: CNet     *net;
    public: long      iterations;
    public: long      iterations_route;
    public: long      iterations_reroute;
    public: CDRGrid  *_drgrid;
    public: CASimple *_netsched;

    // Constructor.
    public: CAStar (CDRGrid *drgrid, CASimple *netsched);

    // Modifiers.
    private: bool  step       (void) throw (trapped_astar);
    private: bool  nexttarget (void);
    private: void  backtrack  (void);
    private: void  abort      (void);
    public:  void  dump       (void);
    public:  void  clear      (void);
    public:  void  load       (CNet *pNet, int delta=0, int expand=0);
    public:  bool  search     (void);
    public:  void  route      (CNet *pNet) throw (reach_max_pri);


    // Database integrity check.
    public:  void  check (bool cleared);
  };




// -------------------------------------------------------------------
// Module  :  "ASimple.cpp".


  // ---------------------------------------------------------------
  // ASimple algorithm class.
  //
  // This class includes all subclasses it needs.

  class CASimple {


    // Priority queue. -------------------------------------------
    class CQueue {

      // Queue element class.
      class CQelem {

        // Attributes.
        public: CNet *net;

        // Constructor.
        public: CQelem (CNet *pNet=NULL) { net = pNet; }

        // Operator : ordering function.
        public: bool operator< (const CQelem &other) const {
                  return ((*this->net) < (*other.net));
                }
      };

      // Attribute.
      public: priority_queue<CQelem>  queue;

      // Accessors.
      public: CNet* pop (void) {
                      CQelem  elem;

                      elem = queue.top ();
                      queue.pop ();

                      return (elem.net);
                    }
      public: void  push  (CNet *net)     { queue.push(CQelem (net)); }
      public: bool  empty (void)          { return (queue.empty()); }
      public: void  reset (void)          { while (!empty()) pop(); }
      public: void  load  (MNet *nets, bool rglobal, bool global);

    };


    // Internal CASimple attributes : the state of the algorithm.
    public: CQueue   _queue;
    public: CAStar   _astar;
    public: CDRGrid *_drgrid;
    public: bool     rglobal;

    // Attributes.
    public: MNet *nets;
    public: long  iterations_route;
    public: long  iterations_reroute;

    // Constructor.
    public: CASimple (MNet *mNets, CDRGrid *drgrid);

    // Modifiers.
    public: void  queue  (CNet *net) { _queue.push (net); }
    public: bool  step   (void);
    public: void  global (void);
    public: void  local  (void);
    public: void  stats  (void);
    public: void  run    (bool rtype);

  };




# endif
