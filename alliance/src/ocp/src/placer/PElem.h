
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 
#ifndef __PELEM_H
#define __PELEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include "PPos.h"
using namespace std;

class PNet;

class PElem {
    
  public:
    typedef vector <PNet*> PNets; 
    
  protected:  
    PNets		_nets;
  
  public:
    PElem() : _nets() {} 

    PNets&		GetNets() 				{ return _nets; }
    const PNets&	GetConstNets() const			{ return _nets; }
    
    virtual PPos	GetPos() const = 0;
    virtual ~PElem() {}
    
    double		GetPosX() const				{ return GetPos().GetX(); }
    double		GetPosY() const				{ return GetPos().GetY(); }
    
    virtual ostream&	Print(ostream& os) const = 0;
    void		DescribeOn(ostream& os) const;
};

static inline ostream& operator<<(ostream& os, const PElem& elem) {
    return elem.Print(os);
}

static inline ostream& operator<<(ostream& os, const PElem* elem) {
    return elem ? elem->Print(os) : os << "(nil)";
}

#endif /* __PELEM_H */
