
// -*- C++ -*-
//
// $Id: MDRGrid.cpp,v 1.4 2004/07/23 08:50:04 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./MDRGrid.cpp"                           |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "MDefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Friend of "CRect"  :  "&operator<<()".

ostream &operator<< (ostream &o, const CRect *rect)
{
  o << "("
    << rect->x1 << ", "
    << rect->y1 << "), ("
    << rect->x2 << ", "
    << rect->y2 << ")";

  return (o);
}




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
// Constructor  :  "CDRGrid::iterator::iterator ()".

CDRGrid::iterator::iterator (void)
{
  _drgrid = NULL;
  _index  = INT_MAX;
}




// -------------------------------------------------------------------
// Constructor  :  "CDRGrid::iterator::iterator ()".

CDRGrid::iterator::iterator (const CDRGrid::iterator &other)
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
    throw e_matrix_iterator ("Attempt to use an unitialized grid iterator.");
  }

  if ( (validindex) && (_index == INT_MAX) )
    throw e_matrix_iterator ("Attempt to use iterator outside the matrix.");
}




// -------------------------------------------------------------------
// Method  :  "CDRGrid::iterator::set ()".

CDRGrid::iterator &CDRGrid::iterator::set (int x, int y, int z)
{
  valid (false);

  _index = _drgrid->dx (0     , x);
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
// Modifier  :  "CDRGrid::iterator::addnode ()".

CNode &CDRGrid::iterator::addnode (void)
{
  valid (true);

  return ( _drgrid->nodes->add (this->_index) );
}




// -------------------------------------------------------------------
// Accessor  :  "CDRGrid::iterator::isnodehole ()".

bool  CDRGrid::iterator::isnodehole (void)
{
  return ( &(*_drgrid->nodes)[ *this ] == &(_drgrid->nodes->hole) );
}




// -------------------------------------------------------------------
// Accessor  :  "CDRGrid::iterator::isprihole ()".

bool  CDRGrid::iterator::isprihole (void)
{
  return ( &(*_drgrid->pri)[ *this ] == &(_drgrid->pri->hole) );
}




// -------------------------------------------------------------------
// Accessor  :  "CDRGrid::iterator::take ()".

bool  CDRGrid::iterator::take (int pri)
{
  valid(false);

  return (_drgrid->pri->take (pri, *this));
}




// -------------------------------------------------------------------
// Friend : "CDRGrid::iterator::operator<< ()".

ostream &operator<< (ostream &o, CDRGrid::iterator &self)
{
  o << "it(_drgrid := " << self._drgrid
    << ", _index := ";

  if (self._index != INT_MAX) {
    o << self._index
      << " (" << self.x() << "," << self.y() << "," << self.z() << ")";
  } else {
    o << "INT_MAX )";
  }

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

  CDRGrid::CDRGrid (int x, int y, int z, int zup) throw (e_zupper)
{
  int    index;


  X      = x;
  Y      = y;
  Z      = z;
  XY     = X  * Y;
  XYZ    = XY * Z;
  size   = XY * (Z - 1);
  zupper = zup;

  if (zupper < 4) throw e_zupper (zupper);


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
  delete nodes;
  delete pri;
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
  typename   _CRow::iterator  itRow;
  typename _CLayer::iterator  itLayer;


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
  typename   _CRow::iterator  itRow;
  typename _CLayer::iterator  itLayer;


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
  typename   _CRow::iterator  itRow;
  typename _CLayer::iterator  itLayer;


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
{
  _drgrid = drgrid;
  _grid   = new (__CNode__) [_drgrid->size];
}




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
  __CNode__ *node;


  if (index < _drgrid->XY ) {
    node = _zero.get (_drgrid->x(index), _drgrid->y(index)) ;
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
    return ( _zero.add (_drgrid->x(index), _drgrid->y(index)) );
  } else {
    if (index < _drgrid->XYZ) return ( (*this)[index] );
  }

  return ( hole );
}
