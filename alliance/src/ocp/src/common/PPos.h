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
