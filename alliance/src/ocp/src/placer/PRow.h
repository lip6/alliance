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
   typedef vector <PSubRow> PSubRows; 
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
