
// -*- C++ -*-
//
// $Id: ATag.cpp,v 1.1 2002/10/02 21:23:47 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./ATag.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "ADefs.h"




  // ---------------------------------------------------------------
  // Pre-defined objects.

  // Global tag map.
  CMapTag *nettag;




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Constructor  :  "CMapTag::CMapTag()".

CMapTag::CMapTag (CCoordBase *coordBase)
{
  base = coordBase;

  _map     = new     bool [base->XYZ];
  _mapsize = sizeof (bool [base->XYZ]);

  clear();

  ::nettag = this;
}




// -------------------------------------------------------------------
// Destructor  :  "CMapTag::~CMapTag()".

CMapTag::~CMapTag (void)
{
  delete [] _map;
}




// -------------------------------------------------------------------
// Method  :  "CMapTag::&operator[]".

bool &CMapTag::operator[] (CCoord &coord)
{
  static bool outbound;

  outbound = false;

  if ((coord.x() < 0) || (coord.x() > base->X - 1)) return (outbound);
  if ((coord.y() < 0) || (coord.y() > base->Y - 1)) return (outbound);
  if ((coord.z() < 1) || (coord.z() > base->Z - 1)) return (outbound);

  return (_map[coord.index() - base->XY]);
}




// -------------------------------------------------------------------
// Method  :  "CMapTag::clear()".

void CMapTag::clear (void)
{
  memset (_map, (char)0, _mapsize);
}




// -------------------------------------------------------------------
// Friend  :  "&operator<<()".

ostream &operator<< (ostream &o, CMapTag *self)
{
  int     x, y, z;
  CCoord  nodeCoord;

  o << "Map Tag state := \n";

  for (z = 1; z < self->base->Z; z++) {
    o << "  (layer) z := " << z << "\n";

    for (y = 1; y <= self->base->Y; y++) {
      o << "    ";
      for (x = 0; x < self->base->X; x++) {
        o << (*self)[nodeCoord.set (x, self->base->Y - y, z)];
      }
      o << "\n";
    }
  }

  return (o);
}
