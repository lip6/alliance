
// -*- C++ -*-

# include  <iomanip>
# include  <fstream>
# include  <sstream>
# include  <exception>
# include  <iterator>
# include  <utility>
# include  <string>
# include  <vector>
# include  <queue>
# include  <list>
# include  <map>
# include  <set>
# include  <stdexcept>
# include  <cstdlib>
# include  <cmath>

# include  <unistd.h>





  // ---------------------------------------------------------------
  // Use of a cleared priority map exception.

  class primap_clear : public exception {

    // Overridables.
    public: virtual const char* what () const {
              return ((char*)"Use of cleared signal priority map.");
            }
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

  };




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


  class CNode;
  class CNet;


  class CNodeData {

    // Attributes.
    public: int    pri;
    public: CNet  *owner;
    public: CNode *rtree;
    public: int    ident;
    public: bool   obstacle;
    public: bool   lock;

  };


  class CNode {

      public: CNodeData data;

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
  // Matrix iterator exception class.

  class e_matrix_iterator : public exception {

    // Attributes.
    string  message;

    // Constructor.
    public: e_matrix_iterator (string msg) { message = msg; }

    // Overridables.
    public: virtual const char* what () const { return (message.c_str()); }

  };




  class CDRGrid;




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
    public: _CHollow  *_zero;
    public: __CNode__ *_grid;
    public: __CNode__   hole;


    // Constructor.
    public: TMatrix  (CDRGrid *drgrid);
    private: TMatrix ( const TMatrix& );

    // Destructor.
    public: ~TMatrix (void);

    // Accessors.
    public: __CNode__ &operator[] (int index);

    // Modifiers.
    public: __CNode__ &add (int index);

  };


  typedef  TMatrix<CNode>  CMatrixNodes;


  class CMatrixPri : public TMatrix<char> {

    // Internal attributes.
    protected: CBB   _bb;

    // Attributes.
    public: int   offset;
    public: int   delta;
    public: bool  cleared;


    // Constructor.
    public: CMatrixPri (CDRGrid *drgrid)
              : TMatrix<char>(drgrid)
              , offset(0)
              , delta(0)
              , cleared(false)
            { }

    // Modifiers.
    public: void clear    (void);
    public: void load     (CNet &net, bool global, int expand=0);
    public: bool cmp      (int pri, int index);
    public: void findfree (int index);

    // Internal methods.
    private: char  nextPri (char curpri);

    // Friends.
    public: friend ostream &operator<< (ostream &o, CMatrixPri &self);

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
      public: iterator (iterator &other);

      // Accessors.
      public:            operator   int (void) { return ( _index ); }
      public:            operator   CCoord (void)
                                    { return ( CCoord (x(), y(), z()) ); }
      public: bool       operator== (iterator &other)
                                    { return (    (_drgrid == other._drgrid)
                                               && (_index  == other._index ) ); }
      public: char      &pri        (void);
      public: CNode     &node       (void);
      public: CNode     &addnode    (void);

      public: friend ostream &operator<< (ostream &o, iterator &self);

      public: void   valid   (bool validindex) throw (e_matrix_iterator);
      public: int    x       (void) { return ( _drgrid->x (_index) ); }
      public: int    y       (void) { return ( _drgrid->y (_index) ); }
      public: int    z       (void) { return ( _drgrid->z (_index) ); }
      public: bool   outside (void) { return ( _index == INT_MAX ); }
      public: bool   inside  (void) { return ( !outside() ); }
      public: int    manhattan (iterator &other) throw (e_matrix_iterator);

      // Modifiers.
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
    private: CDRGrid ( const CDRGrid& );

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
// Constructor  :  "CDRGrid::iterator::iterator ()".

CDRGrid::iterator::iterator (void)
{
  _drgrid = NULL;
  _index  = INT_MAX;
}




// -------------------------------------------------------------------
// Constructor  :  "CDRGrid::iterator::iterator ()".

CDRGrid::iterator::iterator (CDRGrid::iterator &other)
{
  _drgrid = other._drgrid;
  _index  = other._index;
}




// -------------------------------------------------------------------
// Method  :  "CDRGrid::iterator::valid ()".

void  CDRGrid::iterator::valid (bool validindex)
  throw (e_matrix_iterator)
{
  if (_drgrid == NULL) {
    throw e_matrix_iterator ("Attempt to use an uninitialized grid iterator.");
  }

  if ( (validindex) && (_index == INT_MAX) )
    throw e_matrix_iterator ("Attempt to use iterator outside the matrix.");
}




// -------------------------------------------------------------------
// Method  :  "CDRGrid::iterator::set ()".

CDRGrid::iterator &CDRGrid::iterator::set (int x, int y, int z)
{
  valid (false);

  _index = _drgrid->dx (_index, x);
  _index = _drgrid->dy (_index, y);
  _index = _drgrid->dz (_index, z);

  return ( *this );
}




// -------------------------------------------------------------------
// Method  :  "CDRGrid::iterator::dx ()".

CDRGrid::iterator &CDRGrid::iterator::dx (int d)
{
  valid (false);

  _index = _drgrid->dx (_index, d);

  return ( *this );
}




// -------------------------------------------------------------------
// Method  :  "CDRGrid::iterator::dy ()".

CDRGrid::iterator &CDRGrid::iterator::dy (int d)
{
  valid (false);

  _index = _drgrid->dy (_index, d);

  return ( *this );
}




// -------------------------------------------------------------------
// Method  :  "CDRGrid::iterator::dz ()".

CDRGrid::iterator &CDRGrid::iterator::dz (int d)
{
  valid (false);

  _index = _drgrid->dz (_index, d);

  return ( *this );
}




// -------------------------------------------------------------------
// Accessor  :  "CDRGrid::iterator::pri".

char &CDRGrid::iterator::pri (void)
{
  valid (false);

  return ( (*(_drgrid->pri))[*this] );
}




// -------------------------------------------------------------------
// Accessor  :  "CDRGrid::iterator::node".

CNode &CDRGrid::iterator::node (void)
{
  valid (false);

  return ( (*(_drgrid->nodes))[*this] );
}




// -------------------------------------------------------------------
// Modifier  :  "CDRGrid::iterator::addnode".

CNode &CDRGrid::iterator::addnode (void)
{
  valid (true);

  return ( _drgrid->nodes->add (this->_index) );
}




// -------------------------------------------------------------------
// Friend : "CDRGrid::iterator::operator<< ()".

ostream &operator<< (ostream &o, CDRGrid::iterator &self)
{
  o << "CDRGrid<>::iterator (_drgrid := " << self._drgrid
    << ", _index := " << self._index
    << " (" << self.x() << "," << self.y() << "," << self.z() << "))";

  return ( o );
}




// -------------------------------------------------------------------
// Method : "CDRGrid::iterator::manhattan ()".

int  CDRGrid::iterator::manhattan (iterator &other)
  throw (e_matrix_iterator)
{
  long  manhattan;


  valid (true);
  other.valid (true);

  if (_drgrid != other._drgrid)
    throw (e_matrix_iterator ("Attempt to use iterators from different grids."));

  manhattan  = abs (x() - other.x()) * _drgrid->cost_x_hor;
  manhattan += abs (y() - other.y()) * _drgrid->cost_y_ver;
  manhattan += abs (z() - other.z()) * _drgrid->cost_z;

  // As we never use ALU1 layer, add the cost of VIA.
  if (z() == 0)       manhattan += _drgrid->cost_z;
  if (other.z() == 0) manhattan += _drgrid->cost_z;

  return (manhattan);
}




// -------------------------------------------------------------------
// Constructor  :  "CDRGrid::CDRGrid()".

CDRGrid::CDRGrid (int x, int y, int z)
{
  X    = x;
  Y    = y;
  Z    = z;
  XY   = X  * Y;
  XYZ  = XY * Z;
  size = XY * (Z - 1);

  nodes = new CMatrixNodes (this);
  pri   = new CMatrixPri   (this);

  cost_x_hor = cost_y_ver = cost_z = 1;
  cost_x_ver = cost_y_hor =          2;

  // Reference iterator initialisation.
  origin._drgrid = this;
  origin._index  = XY;
}




// -------------------------------------------------------------------
// Destructor  :  "CDRGrid::~CDRGrid()".

CDRGrid::~CDRGrid (void)
{
  delete [] nodes;
  delete [] pri;
}




// -------------------------------------------------------------------
// Modifier  :  "CDRGrid::costs ()".

void  CDRGrid::costs (int x_hor, int x_ver, int y_hor, int y_ver, int z)
{
  cost_x_hor = x_hor;
  cost_x_ver = x_ver;
  cost_y_hor = y_hor;
  cost_y_ver = y_ver;
  cost_z     = z;
}




// -------------------------------------------------------------------
// Modifier  :  "CDRGrid::dx ()".

int  CDRGrid::dx (int index, int d)
{
  if ( (index == INT_MAX) || (x(index) + d < 0) || (x(index) + d >= X) )
    return (INT_MAX);

  return (index + d);
}




// -------------------------------------------------------------------
// Modifier  :  "CDRGrid::dy ()".

int  CDRGrid::dy (int index, int d)
{
  if ( (index == INT_MAX) || (y(index) + d < 0) || (y(index) + d >= Y) )
    return (INT_MAX);

  return (index + d*X);
}




// -------------------------------------------------------------------
// Modifier  :  "CDRGrid::dz ()".

int  CDRGrid::dz (int index, int d)
{
  if ( (index == INT_MAX) || (z(index) + d < 0) || (z(index) + d >= Z) )
    return (INT_MAX);

  return (index + d*XY);
}




// -------------------------------------------------------------------
// Destructor  :  "TMatrix::_CHollow::~_CHollow()".

template<class __CNode__>
TMatrix<__CNode__>::_CHollow::~_CHollow (void)
{
    _CRow::iterator  itRow;
  _CLayer::iterator  itLayer;


  for (itLayer  = nodes.begin ();
       itLayer != nodes.end   (); itLayer++) {
    for (itRow  = itLayer->second.begin ();
         itRow != itLayer->second.end   (); itRow++) {
      delete itRow->second;
    }
  }
}




// -------------------------------------------------------------------
// Method  :  "TMatrix::_CHollow::add()".

template<class __CNode__>
__CNode__ &TMatrix<__CNode__>::_CHollow::add (int x, int y)
{
    _CRow::iterator  itRow;
  _CLayer::iterator  itLayer;


  itLayer = nodes.find (x);
  if (itLayer == nodes.end ()) { nodes[x] = _CRow (); }

  itRow = nodes[x].find (y);
  if (itRow == nodes[x].end ()) { nodes[x][y] = new __CNode__ (); }

  return (*(nodes[x][y]));
}




// -------------------------------------------------------------------
// Method  :  "TMatrix::_CHollow::get()".

template<class __CNode__>
__CNode__ *TMatrix<__CNode__>::_CHollow::get (int x, int y)
{
    _CRow::iterator  itRow;
  _CLayer::iterator  itLayer;


  itLayer = nodes.find (x);
  if (itLayer == nodes.end ()) { return (NULL); }

  itRow = nodes[x].find (y);
  if (itRow == nodes[x].end ()) { return (NULL); }

  return ((*itRow).second);
}




// -------------------------------------------------------------------
// Constructor  :  "TMatrix::TMatrix()".

template<class __CNode__>
TMatrix<__CNode__>::TMatrix (CDRGrid *drgrid)
  : _drgrid(drgrid)
  , _zero  ()
  , _grid  (new (__CNode__) [_drgrid->size])
  , hole   ()
{ }




// -------------------------------------------------------------------
// Destructor  :  "~TMatrix::TMatrix()".

template<class __CNode__>
TMatrix<__CNode__>::~TMatrix (void)
{
  delete [] _grid;
}




// -------------------------------------------------------------------
// Accessor  :  "TMatrix::&operator[]".

template<class __CNode__>
__CNode__ &TMatrix<__CNode__>::operator[] (int index)
{
  if (index < _drgrid->XY ) {
    __CNode__ *node = _zero->get (_drgrid->x(index), _drgrid->y(index)) ;
    if ( node != NULL ) return ( *node );
  } else {
    if (index < _drgrid->XYZ) return ( _grid[index - _drgrid->XY] );
  }

  return ( hole );
}




// -------------------------------------------------------------------
// Modifier  :  "TMatrix::add ()".

template<class __CNode__>
__CNode__ &TMatrix<__CNode__>::add (int index)
{
  if (index < _drgrid->XY) {
    return ( _zero->add (_drgrid->x(index), _drgrid->y(index)) );
  } else {
    if (index < _drgrid->XYZ) return ( (*this)[index] );
  }

  return ( hole );
}




// -------------------------------------------------------------------
// Module  :  "UNet.cpp".


  // ---------------------------------------------------------------
  // Duplicate terminal node exception.

  class dup_term : public exception {

    // Attributes.
    public: string  name;
    public: CNode  *node;

    // Constructor.
    public: dup_term (string termName, CNode *dupNode) {
              name = termName;
              node = dupNode;
            }

    // Overridables.
    public: virtual const char* what () const {
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
    public: int    distance (CTerm &other);
    public: CTerm &nearest  (CTerm &term1, CTerm &term2);
    public: CNode &lowest   (void);

    // Modifiers.
    public: CNode *newaccess (int x, int y, int z, int ident, CNet *net) throw (dup_term);
    public: void   newaccess (CRect &rect, int z, int ident, CNet *net);
    public: void   lockalone (bool global=false);
    public: void   setid     (int ident);

  };




  // ---------------------------------------------------------------
  // Duplicate net exception.

  class dup_net : public exception {

    // Attributes.
    public: string  name;

    // Constructor.
    public: dup_net (string netName) { name = netName; }

    // Overridables.
    public: virtual const char* what () const {
              return ((char*)"Duplicated net.");
            }
  };




  // ---------------------------------------------------------------
  // Unknown terminal exception.

  class term_unknown : public exception {

    // Attributes.
    public: string  netName;
    public: string  termName;

    // Constructor.
    public: term_unknown (string nName, string tName) {
              netName  = nName;
              termName = tName;
            }

    // Overridables.
    public: virtual const char* what () const {
              return ((char*)"Unknown terminal.");
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

    // Constructor.
    public: CNet (string netName="noname");

    // Destructor.
    public: ~CNet (void);

    // Operator.
    public: bool operator< (CNet &other);

    // Accessor.
    public: bool global (void) { return (bb.hp >= 400); }

    // Modifiers.
    public: void newaccess (string termName, int x, int y, int z);
    public: void newaccess (string termName, CRect &rect, int z);
    public: void order     (void);
    public: void lockalone (void);
    public: void locktree  (void);
    public: void route     (void);
    public: void unroute   (void);

    // Friends.
    public: friend ostream &operator<< (ostream &o, CNet *self);

  };

  typedef  map<string, CNet>  MNet;




// -------------------------------------------------------------------
// Constructor  :  "CBB::CBB()".

CBB::CBB (void)
{
  x1 = INT_MAX;
  y1 = INT_MAX;
  x2 = 0;
  y2 = 0;
  hp = 0;
}




// -------------------------------------------------------------------
// Method  :  "CBB::merge()".

void CBB::merge (CCoord &coord)
{
  x1 = min (x1, coord.x);
  y1 = min (y1, coord.y);
  x2 = max (x2, coord.x);
  y2 = max (y2, coord.y);

  hp = (x2 - x1) + (y2 - y1);
}




// -------------------------------------------------------------------
// Friend of "CBB"  :  "&operator<<()".

ostream &operator<< (ostream &o, CBB &self)
{
  o << "CBB (" << self.x1 << "," << self.y1 << ") ("
    << self.x2 << "," << self.y2 << ") HP := " << self.hp;

  return (o);
}




// -------------------------------------------------------------------
// Method  :  "CMatrixPri::findfree()".

void  CMatrixPri::findfree (int index)
{
  CDRGrid::iterator  coord;
                int  radius, i, j, cx, cy;
               bool  freed;


  freed  = false;
  radius = 0;
  cx     = _drgrid->x(index);
  cy     = _drgrid->y(index);
  coord  = _drgrid->origin;

  while (!freed) {
    radius += 1;

    for (i = cx - radius; i < cx + radius + 1; i++) {
      for (j = cy - radius; j < cy + radius + 1; j++) {
        // Process only nodes of the ring.
        // if ( (i > cx - radius) || (i < cx + radius) ) continue;
        // if ( (j > cy - radius) || (j < cy + radius) ) continue;

        if ( !( (*_drgrid->nodes)[ coord.set(i,j,2) ].data.obstacle ) ) freed = true;

        (*this)[ coord.set(i,j,1) ] = (char)1;
        (*this)[ coord.set(i,j,2) ] = (char)1;
      }
    }
  }
}




// -------------------------------------------------------------------
// Method  :  "CMatrixPri::clear()".

void CMatrixPri::clear (void)
{
  memset (_grid, (char)0, _drgrid->size);

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
  //   the time being, set to one the map points above the terminal
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

        continue;
      }

      (*this)[ coord.dz(1) ] = nextPri (currentPri); 
      nextBorder->push (new CDRGrid::iterator (coord));
        
      // Enable z=2 (in case of global signal, no effet otherwise).
      (*this)[ coord.dz(1) ] = (char)1; 

      // Look if the upper node is blocked, in that case expand the
      // allowed zone till a non-blocked node is found.

      if ( (*_drgrid->nodes)[ coord ].data.obstacle ) findfree (coord);
    }
  }


  // Set to one all the points inside the enclosing box.
  // (except those in the initial queues)
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
// Method  :  "CMatrixPri::cmp()".

bool CMatrixPri::cmp (int pri, int index)
{
  char  mappri;


  mappri = (*this)[index];

  if (!mappri) return (true);
  if (!pri)    return (false);

  return (pri + 256 >= mappri + delta);
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



int main (int argc, char *argv[])
{

  CDRGrid            *grid;
  CDRGrid::iterator   itNode, itNode2;

  try {
    //cout << &(*itNode) << endl;

    grid = new CDRGrid (10, 5, 3);

    itNode      = grid->origin;
    itNode2     = grid->origin;

    cout << "Browsing a row." << endl;

    for (; itNode.inside (); itNode.right ()) {
      cout << "  " << itNode
           << "\n  distance from origin := "
           << itNode.manhattan (itNode2) << endl;
    }

    cout << "Browsing a column." << endl;

    itNode = grid->origin;
    for (; itNode.inside (); itNode.up ()) {
      cout << "  " << itNode << endl;
    }

    cout << "Browsing layers." << endl;

    itNode = grid->origin;
    for (; itNode.inside (); itNode.top ()) {
      cout << "  " << itNode << endl;
    }

    itNode.manhattan (itNode2);
  }

  catch (e_matrix_iterator &e) {
    cerr << e.what () << endl;
  }
}
