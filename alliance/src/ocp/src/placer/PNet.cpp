/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
#include "PElem.h"

#include "PNet.h"

PNet::PNet(const losig* sig):
	_sig(sig), _elems()
{}

// ======================================================================
// ostream& Print(ostream& os) const;
// ======================================================================
ostream&
PNet::Print(ostream& os) const
{
    if (!_sig->NAMECHAIN)
	return os << "(noname) Index: " << _sig->INDEX;
    
    struct chain* cit = _sig->NAMECHAIN;
    if (!cit->NEXT)
	return os << (char*)cit->DATA;
    
    for (struct chain* cit = _sig->NAMECHAIN; cit; cit = cit->NEXT)
	os << (char*)cit->DATA << " ";
    return os;
}

void
PNet::DescribeOn(ostream& os) const
{
    os << this << endl;
    for (PElems::const_iterator eit = _elems.begin();
	    eit != _elems.end(); eit++)
    {
	os << *eit << endl;
    }
}

ofstream& 
PNet::Plot(ofstream& out) const
{
  int nbElems = 0;
  double baryX = 0;
  double baryY = 0;
  
  for (vector<PElem*>::const_iterator ElemIt = _elems.begin();
      ElemIt != _elems.end(); ElemIt++)
  {
    ++nbElems;
    baryX += (*ElemIt)->GetPosX();
    baryY += (*ElemIt)->GetPosY();
  }
  baryX = baryX / nbElems;
  baryY = baryY / nbElems;

  for (vector<PElem*>::const_iterator ElemIt = _elems.begin();
      ElemIt != _elems.end(); ElemIt++)
  {
    out << baryX << " " << baryY << endl
    << (*ElemIt)->GetPosX() << " " << (*ElemIt)->GetPosY() << endl << endl;
  }  
  
  return out;
}
