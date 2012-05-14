
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
#include "PBin.h"
#include "PToPlaceIns.h"
#include "PConstants.h"

#include "PSubRow.h"

PSubRow::PSubRow()
    : PContainer(), _bins(),
    _binsXMax()
{}

PSubRow::~PSubRow()
{
    for (PBins::iterator bit = _bins.begin();
	    bit !=_bins.end();
	    bit++)
	delete *bit;
}

unsigned
PSubRow::GetNBins() const
{
    return _bins.size();
}

void
PSubRow::Init(PRow* row, double y, double minx, double maxx, double margin, double maxbinwidth, double minbinwidth)
{
    _row = row;
    PBBox binbbox;
    double xpos;
    int binswidth;
    int modulo = 0;
    
    _bBox.SetMinY(y);
    _bBox.SetMaxY(y + ROWHEIGHT);
    _bBox.SetMinX(minx);
    _bBox.SetMaxX(maxx);
    _size = 0.0;
    _capa = (maxx - minx) * (1.0 - margin);
    _max = maxx - minx;
      
    binbbox.SetMinY(_bBox.GetMinY());
    binbbox.SetMaxY(_bBox.GetMaxY());
    xpos = minx;

    if ( ((int)(maxx - minx) % (int)(maxbinwidth)) > 0)
      _nBins = (unsigned)((maxx - minx)/maxbinwidth) + 1;
    else
      _nBins = (unsigned)((maxx - minx)/maxbinwidth);
     
    modulo = ((int)(maxx - minx) % _nBins);
    
    binswidth = (int)((maxx - minx) / _nBins);
    
    _bins.reserve(_nBins);
    for (unsigned binnumber = 0; binnumber < _nBins; binnumber++)
    {
        _bins.push_back(new PBin());
        binbbox.SetMinX(xpos);
	if (modulo > 0)
	{
	    xpos += (binswidth+1);
	    binbbox.SetMaxX(xpos);
	    modulo--;
	}
	else
	{
	    xpos += binswidth;
	    binbbox.SetMaxX(xpos);
	}
	_binsXMax[xpos] = binnumber;
        _bins.back()->Init(binbbox, margin, *this);
    }
    _row->MergeBBox(_bBox);
}

PBin&
PSubRow::GetBin(const double X)
{
    if (X >= (*_bins.rbegin())->GetMaxX())
	return **(_bins.rbegin());

    if (X <= (*_bins.begin())->GetMinX())
	return **(_bins.begin());

    PBinsXMax::iterator srpos = _binsXMax.upper_bound(X);
    return *_bins[srpos->second];
}
    
bool
PSubRow::InsertIns(PToPlaceIns& Ins, int BinNumber)
{
    if ((_bins[BinNumber]->GetCapa() - _bins[BinNumber]->GetSize())
            < Ins.GetWidth())
        return false;
    else
    {
        _bins[BinNumber]->AddIns(&Ins);
        return true;
    }
}

unsigned
PSubRow::GetNIns() const
{
    unsigned nins = 0;
    for (PBins::const_iterator bit =_bins.begin(); bit !=_bins.end(); bit++)
	nins += (*bit)->GetNIns();
    return nins;
}
    
void
PSubRow::ForceIns(PToPlaceIns& Ins, int BinNumber)
{
    _bins[BinNumber]->AddIns(&Ins);
}

#ifndef Abs
#define Abs(x) ((x) < 0.0 ? -(x) : (x))
#endif
double PSubRow::GetBinCost() const
{
    double binCost = 0.0;
    for (PBins::const_iterator bit = _bins.begin(); bit != _bins.end(); bit++)
    {
        binCost += Abs((*bit)->GetSize() - (*bit)->GetCapa());
    }
    return binCost;
}

ofstream& 
PSubRow::Plot(ofstream& out) const
{
  for (PBins::const_iterator bit=_bins.begin(); bit!=_bins.end(); bit++)
  {
      (*bit)->Plot(out);
  }
  return out;
}

ofstream& 
PSubRow::PlotLabel(ofstream& out, unsigned TotalMoves) const
{
    for (PBins::const_iterator bit=_bins.begin(); bit!=_bins.end(); bit++)
    {
	(*bit)->PlotLabel(out, TotalMoves);
    }
    return out;
}

ostream&
PSubRow::Print(ostream& os) const
{
    return os << "PSubRow: " << GetMinX() << ',' << GetMinY() << " : " << GetMaxX() << ',' << GetMaxY();
}


double PSubRow::GetBinsSize() const
{
    double binsSize = 0.0;
    for (PBins::const_iterator bit=_bins.begin(); bit!=_bins.end(); bit++)
    {
        binsSize += (*bit)->GetSize();
    }
    return binsSize;
}

double PSubRow::GetBinsCapa() const
{
    double binsCapa = 0.0;
    for (PBins::const_iterator bit=_bins.begin(); bit!=_bins.end(); bit++)
    {
        binsCapa += (*bit)->GetCapa(); 
    }
    return binsCapa;
}
