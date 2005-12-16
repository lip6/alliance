
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
    double		GetBinsSize() const;
    double		GetBinsCapa() const;
    double		GetSubRowsCapa() const;

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
