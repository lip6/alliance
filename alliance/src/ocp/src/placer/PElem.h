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
