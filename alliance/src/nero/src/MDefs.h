
// -*- C++ -*-
//
// $Id: MDefs.h,v 1.3 2002/10/17 21:57:27 jpc Exp $
//
// /-----------------------------------------------------------------\ 
// |                                                                 |
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./MDefs.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// \-----------------------------------------------------------------/




# ifndef  __MDefs__
# define  __MDefs__  1


# include  "UDefs.h"




// -------------------------------------------------------------------
// Class declarations.

  class CCoord;
  class CNode;
  class CNodeData;
  class CMatrixPri;
  class CMatrixNodes;
  class CDRGrid;
  class CNet;




// -------------------------------------------------------------------
// Module  :  "MDRGrid.cpp".


  // ---------------------------------------------------------------
  // Rectangle class.

  struct CRect {

    // Attributes.
    long x1;
    long y1;
    long x2;
    long y2;

    // Friend.
    friend ostream &operator<< (ostream &, const CRect *);

  };




  // ---------------------------------------------------------------
  // Bounding Box class.

  class CBB {

    // Attributes.
    public: int  x1;
    public: int  y1;
    public: int  x2;
    public: int  y2;
    public: int  hp;

    // Constructor.
    public: CBB (void);

    // Modifiers.
    public: void merge (CCoord &coord);

    // Friends.
    public: friend ostream &operator<< (ostream &o, CBB &self);
  };




  // ---------------------------------------------------------------
  // Coordinate class.

  class CCoord {

    public: int  x;
    public: int  y;
    public: int  z;

    // Constructors.
    public: CCoord (void)                { x = y = z = 0; }
    public: CCoord (int i, int j, int k) { x = i; y = j; z = k; }

    // Modifiers.
    public: CCoord &set (int i, int j, int k)
                        { x = i; y = j; z = k; return (*this); }

    // Friends.
    public: friend ostream &operator<< (ostream &o, CCoord &self) {
                     o << "(" << self.x
                       << "," << self.y
                       << "," << self.z << ")";

                     return (o);
                   }

  };




  // ---------------------------------------------------------------
  // Matrix iterator exception class.

  class e_matrix_iterator : public except_done {

    // Attributes.
    string  message;

    // Constructor.
    public: e_matrix_iterator (string msg) { message = msg; }

    // Destructor.
    public: ~e_matrix_iterator (void) throw () { };

    // Overridables.
    public: const char* what () const throw () { return (message.c_str()); }

  };




  // ---------------------------------------------------------------
  // Detailed Routing Matrix Class.

  template<class __CNode__> class TMatrix {

    // Matrix hollow level class ---------------------------------

    public: struct _CHollow {
 
      typedef map<int, __CNode__*>  _CRow;
      typedef map<int, _CRow>       _CLayer;

      // Attributes.       
      public: _CLayer  nodes;

      // Destructor.
      ~_CHollow (void);

      // Modifier.
      public: __CNode__ &add (int x, int y);

      // Accessor.
      public: __CNode__ *get (int x, int y);

    };


    // Internal attributes.
    public: CDRGrid   *_drgrid;
    public: _CHollow   _zero;
    public: __CNode__ *_grid;
    public: __CNode__   hole;


    // Constructor.
    public: TMatrix  (CDRGrid *drgrid);

    // Destructor.
    public: ~TMatrix (void);

    // Accessors.
    public: __CNode__ &operator[] (int index);

    // Modifiers.
    public: __CNode__ &add (int index);

  };




  // ---------------------------------------------------------------
  // Detailed Routing Grid Class.

  class CDRGrid {

    // Matrix common iterator class ------------------------------

    public: class iterator {

      // Attributes.
      public: int       _index;
      public: CDRGrid  *_drgrid;

      // Constructors.
      public: iterator (void);
      public: iterator (const iterator &other);

      // Accessors.
      public:         operator   int (void) { return ( _index ); }
      public:         operator   CCoord (void)
                                 { return ( CCoord (x(), y(), z()) ); }
      public: bool    operator== (iterator &other)
                                 { return (    (_drgrid == other._drgrid)
                                            && (_index  == other._index ) ); }

      // Accessors for matrix objects.
      public: char   &pri        (void);
      public: bool    isprihole  (void);
      public: CNode  &node       (void);
      public: CNode  &addnode    (void);
      public: bool    isnodehole (void);
      public: bool    take       (int pri);

      public: friend ostream &operator<< (ostream &o, iterator &self);

      public: void   valid   (bool validindex) throw (e_matrix_iterator);
      public: int    x       (void) { return ( _drgrid->x (_index) ); }
      public: int    y       (void) { return ( _drgrid->y (_index) ); }
      public: int    z       (void) { return ( _drgrid->z (_index) ); }
      public: bool   outside (void) { return ( _index == INT_MAX ); }
      public: bool   inside  (void) { return ( !outside() ); }
      public: int    manhattan (iterator &other) throw (e_matrix_iterator);

      // Modifiers.
      public: void      unlink (void)
                                 { _index = INT_MAX; _drgrid = NULL; }
      public: iterator &set    (int x, int y, int z);
      public: iterator &set    (CCoord &coord) 
                                 { return ( set(coord.x, coord.y, coord.z) ); }
      public: iterator &dx     (int d);
      public: iterator &dy     (int d);
      public: iterator &dz     (int d);
      public: iterator &left   (void) { return ( dx(-1) ); };
      public: iterator &right  (void) { return ( dx(+1) ); };
      public: iterator &down   (void) { return ( dy(-1) ); };
      public: iterator &up     (void) { return ( dy(+1) ); };
      public: iterator &bottom (void) { return ( dz(-1) ); };
      public: iterator &top    (void) { return ( dz(+1) ); };

    };


    // Matrix class ----------------------------------------------

    // Attributes.
    public: int X;
    public: int Y;
    public: int Z;
    public: int XY;
    public: int XYZ;
    public: int size;
    public: int cost_x_hor;
    public: int cost_x_ver;
    public: int cost_y_hor;
    public: int cost_y_ver;
    public: int cost_z;

    public: iterator      origin;
    public: CMatrixPri   *pri;
    public: CMatrixNodes *nodes;

    // Constructor.
    public: CDRGrid  (int x, int y, int z);

    // Destructor.
    public: ~CDRGrid (void);

    // Modifiers.
    public: void  costs (int x_hor, int x_ver, int y_hor, int y_ver, int z);

    // Utilities.
    public: int x  (int index) { return ( index       % X); }
    public: int y  (int index) { return ((index / X ) % Y); }
    public: int z  (int index) { return ((index / XY) % Z); }
    public: int dx (int index, int d);
    public: int dy (int index, int d);
    public: int dz (int index, int d);
    public: int id (int x, int y, int z)
                               { return ( x + (X * (y + (Y * z))) ); }

  };




// -------------------------------------------------------------------
// Module  :  "MNodes.cpp".


  // ---------------------------------------------------------------
  // Node grabbing exception.

  class bad_grab : public except_done {

    // Attributes.
    public: int    x;
    public: int    y;
    public: int    z;
    public: string ownerName;
    public: string netName;
    public: int    pri;
    public: int    nodepri;
    public: bool   terminal;
    public: int    ident;

    // Constructor.
    public: bad_grab ( string owner
                     , string net
                     , int i
                     , int j
                     , int k
                     , int p
                     , int np
                     , bool term
                     , int id
                     ) {
              x = i;
              y = j;
              z = k;
              pri       = p;
              nodepri   = np;
              terminal  = term;
              ident     = id;
              ownerName = owner;
              netName   = net;
            }

    // Destructor.
    public: ~bad_grab (void) throw () { };

    // Overridables.
    public: const char* what () const throw () {
              return ((char*)"Attempt to grab a non-free node.");
            }

  };




  // ---------------------------------------------------------------
  // Node data class.

  class CNodeData {

    // Attributes.
    public: int    pri;
    public: CNet  *owner;
    public: CNode *rtree;
    public: int    ident;
    public: bool   obstacle;
    public: bool   lock;

    // Constructor.
    CNodeData (void);

  };




  // ---------------------------------------------------------------
  // Node class.

  class CNode {

    // Attributes.
    public: CNodeData  data;
    public: void      *algo;

    // Constructor.
    CNode ();

    // Modifiers.
    public: inline int  setid  (int id) { return (data.ident = id + 1); }
    public:        void grab   ( CNet *net
                               , int pri
                               , CDRGrid::iterator &coord
                               ) throw (bad_grab);
    public:        void ungrab (void);
    public:        void lock   (void)   { data.lock = true; }
    public:        void unlock (void)   { data.lock = false; }

    // Accessors.
    public: inline int  getid    (void) { return (data.ident - 1); }
    public: inline bool terminal (void) { return (data.ident != 0); }
    public: inline bool locked   (void) { return (data.lock); }
    public:        bool check    (void);

  };




  // ---------------------------------------------------------------
  // Matrix Node class (derived from TMatrix template).

  class CMatrixNodes : public TMatrix<CNode> {

    // Constructor.
    public: CMatrixNodes (CDRGrid *drgrid) : TMatrix<CNode>(drgrid) { }

    // Modifiers.
    public: void  obstacle (CRect &rect, int z);
    public: void  check    (void);

  };




// -------------------------------------------------------------------
// Module  :  "MPri.cpp".


  // ---------------------------------------------------------------
  // Matrix Priority class (derived from TMatrix template).

  class CMatrixPri : public TMatrix<char> {

    // Internal attributes.
    protected: CBB   _bb;

    // Attributes.
    public: int   offset;
    public: int   delta;
    public: bool  cleared;


    // Constructor.
    public: CMatrixPri (CDRGrid *drgrid) : TMatrix<char>(drgrid) { }

    // Modifiers.
    public: void clear    (void);
    public: void load     (CNet &net, bool global, int expand=0);
    public: bool take     (int pri, int index);
    public: void findfree (int index, CNet &net);

    // Internal methods.
    private: char  nextPri (char curpri);

    // Friends.
    public: friend ostream &operator<< (ostream &o, CMatrixPri &self);

  };




// -------------------------------------------------------------------
// Module  :  "MNet.cpp".


  // ---------------------------------------------------------------
  // Duplicate terminal node exception.

  class dup_term : public except_done {

    // Attributes.
    public: string             name;
    public: CDRGrid::iterator  node;

    // Constructor.
    public: dup_term (string termName, CDRGrid::iterator &dupNode) {
              name = termName;
              node = dupNode;
            }

    // Destructor.
    public: ~dup_term (void) throw () { };

    // Overridables.
    public: const char* what () const throw () {
              return ((char*)"Duplicated terminal node.");
            }
  };




  // ---------------------------------------------------------------
  // Terminal class.

  class CTerm {

    // Attributes.
    public: int                      id;
    public: string                   name;
    public: list<CDRGrid::iterator>  nodes;
    public: list<CRect>              rects;

    // Constructor.
    public: CTerm (string termName, int ident);

    // Destructor.
    public: ~CTerm (void);

    // Accessors.
    public: int                distance (CTerm &other);
    public: CTerm             &nearest  (CTerm &term1, CTerm &term2);
    public: CDRGrid::iterator &lowest   (void);

    // Modifiers.
    public: CNode *newaccess (int x, int y, int z, int ident, CNet *net)
                     throw (dup_term, bad_grab);
    public: void   newaccess (CRect &rect, int z, int ident, CNet *net);
    public: void   lockalone (bool global=false);
    public: void   setid     (int ident);

    // Friends.
    public: friend ostream &operator<< (ostream &o, CTerm &self);

  };




  // ---------------------------------------------------------------
  // Duplicate net exception.

  class dup_net : public except_done {

    // Attributes.
    public: string  name;

    // Constructor.
    public: dup_net (string netName) { name = netName; }

    // Destructor.
    public: ~dup_net (void) throw () { };

    // Overridables.
    public: const char* what () const throw () {
              return ((char*)"Duplicated net.");
            }
  };




  // ---------------------------------------------------------------
  // Unknown terminal exception.

  class term_unknown : public except_done {

    // Attributes.
    public: string  netName;
    public: string  termName;

    // Constructor.
    public: term_unknown (string nName, string tName) {
              netName  = nName;
              termName = tName;
            }

    // Destructor.
    public: ~term_unknown (void) throw () { };

    // Overridables.
    public: virtual const char* what () const throw () {
              return ((char*)"Unkown terminal.");
            }
  };




  // ---------------------------------------------------------------
  // Net class.

  class CNet {

    // Attributes.
    public: int            pri;
    public: string         name;
    public: vector<CTerm*> terms;
    public: CNode*         rtree;
    public: CBB            bb;
    public: int            size;
    public: bool           external;
    public: CDRGrid       *_drgrid;

    // Constructor.
    public: CNet (CDRGrid *drgrid, string netName="noname");

    // Destructor.
    public: ~CNet (void);

    // Operator.
    public: bool operator< (CNet &other);

    // Accessor.
    public: bool global (void) { return (bb.hp >= D::GLOBAL_HP); }

    // Modifiers.
    public: void newaccess (string termName, int x, int y, int z);
    public: void newaccess (string termName, CRect &rect, int z);
    public: void order     (void);
    public: void lockalone (void);
    public: void locktree  (void);
    public: void unroute   (void);

    // Friends.
    public: friend ostream &print      (ostream &o, CNet *self);
    public: friend ostream &operator<< (ostream &o, CNet *self);

  };

  typedef  map<string, CNet*>  MNet;




// -------------------------------------------------------------------
// Module  :  "MMBK.h".


# include  "MMBK.h"




#endif
