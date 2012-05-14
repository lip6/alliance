
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
#ifndef __PPOS_H
#define __PPOS_H

#include <iostream>
using namespace std;

class PPos {

  protected:

    double X;
    double Y;

  public:

    static const PPos Min;
    static const PPos Max;
    
    PPos() : X(0), Y(0) {}
    PPos(double x, double y) : X(x), Y(y) {}
    PPos(const PPos& pos) : X(pos.X), Y(pos.Y) {}

    PPos& operator=(const PPos& pos)
    {
	X = pos.X;
	Y = pos.Y;
	return *this;
    };
    bool operator==(const PPos& pos) const;
    bool operator!=(const PPos& pos) const;
    bool operator<(const PPos& pos) const;
    bool operator<=(const PPos& pos) const { return pos > *this; }
    bool operator>(const PPos& pos) const;
    bool operator>=(const PPos& pos) const { return pos < *this; }
  
    double GetX() const { return X; }
    double GetY() const { return Y; }

    void SetX(const double& x) { X = x; }
    void SetY(const double& y) { Y = y; }
    
    ostream& Print(ostream& os) const;
};

inline ostream& operator<<(ostream& os, const PPos& pos)
{
    return pos.Print(os);
}

inline ostream& operator<<(ostream& os, const PPos* pos)
{
    return pos ? pos->Print(os) : os << "(nil)";
}

#endif /* __PPOS_H */
