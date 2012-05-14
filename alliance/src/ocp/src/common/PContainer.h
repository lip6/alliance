
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
