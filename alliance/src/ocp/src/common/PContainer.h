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
#ifndef __PContainer_h
#define __PContainer_h

#include <iostream>
#include <fstream>
#include "PBBox.h"
using namespace std;

class PContainer
{

  protected:
    PBBox               _bBox;   // la bbox du container

  public:
    			PContainer():
			    _bBox() {};
                        PContainer(PBBox bbox):
			    _bBox(bbox) {};
			
			virtual ~PContainer() {};

    double		GetMinX() const                 { return _bBox.GetMinX(); }
    double		GetMinY() const                 { return _bBox.GetMinY(); }
    double              GetMaxX() const                 { return _bBox.GetMaxX(); }
    double              GetMaxY() const                 { return _bBox.GetMaxY(); }
    double              GetWidth() const                { return _bBox.GetWidth(); }
    double              GetHeight() const               { return _bBox.GetHeight(); }
    PBBox		GetBBox() const			{ return _bBox; };
    void		MergeBBox(const PBBox bbox)	{_bBox.Merge(bbox); };

    virtual ostream&	Print(ostream& os) const = 0;
};

static inline ostream& operator<<(ostream& os, const PContainer& container) {
    return container.Print(os);
}

static inline ostream& operator<<(ostream& os, const PContainer* container) {
    return container ? container->Print(os) : os << "(nil)";
}
#endif /* __PContainer_h */
