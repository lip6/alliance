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
#ifndef __PNET_H
#define __PNET_H

#include <iostream>
#include <fstream>
#include <vector>
#include "mut.h"
#include "mlo.h"
using namespace std;

class PElem;

class PNet {

  public:

    typedef vector <PElem*> PElems; 
  
  protected:

    const losig*	_sig;
    PElems		_elems;

  public:
    
    PNet(const losig* sig);

    const losig*	GetSig() const		{ return _sig; }
    PElems&		GetElems()		{ return _elems; }
    const PElems&	GetConstElems() const	{ return _elems; }
    
    ostream&	        Print(ostream& os) const;
    void		DescribeOn(ostream& os) const;
    
    ofstream&	        Plot(ofstream& out) const;
};

static inline ostream& operator<<(ostream& os, const PNet& net) {
    return net.Print(os);
}

static inline ostream& operator<<(ostream& os, const PNet* net) {
    return net ? net->Print(os) : os << "(nil)";
}

#endif /* __PNET_H */
