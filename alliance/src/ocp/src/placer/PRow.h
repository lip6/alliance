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
#ifndef __PROW_H
#define __PROW_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "PContainer.h"
using namespace std;

class PSubRow;
class PPlacement;

class PRow : public PContainer
{

    friend class PPlacement;

  public:
   typedef vector <PSubRow*> PSubRows; 
   typedef map<double, unsigned, less<double> > PSubRowsXMax;
   typedef map<double, unsigned, greater<double> > PSubRowsXMaxInv;
  
  private:
    PPlacement*         _placement;
    PSubRows            _subRows;   
    PSubRowsXMax	_subRowsXMax;
    PSubRowsXMaxInv	_subRowsXMaxInv;
    bool		_orientation; // false -> sym/Y ; true -> no sym/Y

  public:
                        PRow(unsigned nbofsubrows);
			PRow(PPlacement& placement, const double ymin, const double xmax,
			    const unsigned nbofsubrows, const bool orientation);
			~PRow();

    void                Init(double Y, double MinX, PPlacement &Placement, const bool RowOrientation);
    bool		GetOrientation() const		{ return _orientation; }
    PSubRows&           GetSubRows() 			{ return _subRows; }
    PSubRow&		GetSubRow(const double X);
    double		GetSubRowCost() const;
    double		GetBinCost() const;

    ostream&	Print(ostream& os) const;
    ofstream&		Plot(ofstream& out) const;
    ofstream&		PlotLabel(ofstream& out, unsigned TotalMoves) const;
};

static inline ostream& operator<<(ostream& os, const PRow& row) {
    return row.Print(os);
}

static inline ostream& operator<<(ostream& os, const PRow* row) {
    return row ? row->Print(os) : os << "(nil)";
}
#endif /* __PROW_H */
