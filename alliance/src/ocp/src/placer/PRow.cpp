
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
#include "PPlacement.h"
#include "PConstants.h"
#include "PBBox.h"

#include "PRow.h"

PRow::PRow(unsigned nbofsubrows)
    : PContainer(), _subRows(nbofsubrows)
{
    for (PSubRows::iterator srit=_subRows.begin();
	    srit!=_subRows.end();
	    srit++)
    {
	*srit = new PSubRow();
    }
}

PRow::PRow(PPlacement& placement,
    const double ymin, const double xmax,
    const unsigned nbofsubrows,
    const bool orientation)
    : PContainer(PBBox(PPos(0.0, ymin), PPos(xmax, ymin + ROWHEIGHT))),
    _placement(&placement),
      _subRows(nbofsubrows),
      _orientation(orientation)
{
    for (PSubRows::iterator srit=_subRows.begin();
	    srit!=_subRows.end();
	    srit++)
	*srit = new PSubRow();
}

PRow::~PRow()
{
    for (PSubRows::iterator srit=_subRows.begin();
	    srit!=_subRows.end();
	    srit++)
	delete *srit;
}

void
PRow::Init(double y, double minx, PPlacement &placement, const bool orientation)
{
    _placement = &placement;
    _orientation = orientation;
    
    _bBox.SetMinY(y);
    _bBox.SetMaxY(y);
    _bBox.SetMinX(minx);
    _bBox.SetMaxX(minx);
}

#ifndef Abs
#define Abs(x) ((x) < 0.0 ? -(x) : (x))
#endif

double
PRow::GetBinCost() const
{
    double bincost = 0.0;
    for (PSubRows::const_iterator srit = _subRows.begin(); srit != _subRows.end(); srit++)
    {
	bincost += (*srit)->GetBinCost();
    }
    return bincost;
}

double
PRow::GetSubRowCost() const
{
    double subrowcost = 0.0;
    for (PSubRows::const_iterator srit = _subRows.begin(); srit != _subRows.end(); srit++)
    {
	subrowcost += Abs((*srit)->GetSize() - (*srit)->GetCapa());
    }
    return subrowcost;
}

PSubRow&
PRow::GetSubRow(const double X)
{
    // s'il y a un seul subrow on le renvoie direct
    if (_subRows.size() == 1)
	return *_subRows[0];

    // si il n'y a rien a droite
    PSubRowsXMax::iterator right = _subRowsXMax.lower_bound(X);
    PSubRowsXMax::iterator left = _subRowsXMaxInv.lower_bound(X);

    if (right == _subRowsXMax.end())
    {
        if (left == _subRowsXMaxInv.end())
        {
            cerr << " o INTERNAL ERROR: GetSubRow()" << endl;
            exit(1);
        }
	return *_subRows[left->second];
    }

    // si on est tombe direct dans un subrow...
    double rightminx = _subRows[right->second]->GetMinX();
    if (X > rightminx)
	return *_subRows[right->second];
    

    // si il n'y a rien a gauche...
    if (left == _subRowsXMaxInv.end())
    {
	if (right == _subRowsXMax.end())
        {
            cerr << " o INTERNAL ERROR: GetSubRow()" << endl;
            exit(1);
        }
	return *_subRows[right->second];
    }

    // on est au milieu de deux subrows, on
    // renvoie le plus proche.

    if ((X - _subRows[left->second]->GetMaxX())
	    > (_subRows[right->second]->GetMinX() - X))
    {
	return *_subRows[right->second];
    }
    else
    {
	return *_subRows[left->second];
    }
}

ofstream& 
PRow::Plot(ofstream& out) const
{
  for (PSubRows::const_iterator srit=_subRows.begin(); srit!=_subRows.end(); srit++)
  {
      (*srit)->Plot(out);
  }
  return out;
}

ofstream& 
PRow::PlotLabel(ofstream& out, unsigned TotalMoves) const
{
    for (PSubRows::const_iterator srit=_subRows.begin(); srit!=_subRows.end(); srit++)
    {
	(*srit)->PlotLabel(out, TotalMoves);
    }
    return out;
}

double PRow::GetBinsSize() const
{
    double binsSize = 0.0;
    for (PSubRows::const_iterator srit=_subRows.begin(); srit!=_subRows.end(); srit++)
    {
        binsSize += (*srit)->GetBinsSize();
    }
    return binsSize;
}

double PRow::GetBinsCapa() const
{
    double binsCapa = 0.0;
    for (PSubRows::const_iterator srit=_subRows.begin(); srit!=_subRows.end(); srit++)
    {
        binsCapa += (*srit)->GetBinsCapa();
    }
    return binsCapa;
}

double PRow::GetSubRowsCapa() const
{
    double subRowsCapa = 0.0;
    for (PSubRows::const_iterator srit=_subRows.begin(); srit!=_subRows.end(); srit++)
    {
        subRowsCapa += (*srit)->GetCapa();
    }
    return subRowsCapa;
}

ostream& PRow::Print(ostream& os) const
{
    string orientation;
    if (_orientation) {
        orientation = "ON";
    } else {
        orientation = "OFF";
    }
    return os << "PRow: " << GetMinX() << ',' << GetMinY() <<
        " : " << GetMaxX() << ',' << GetMaxY() << "," << orientation;
}
