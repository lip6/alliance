
// This file is part of the Alliance Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// The Alliance Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Alliance Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Alliance Project;  if  not,  write to the  Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 
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
