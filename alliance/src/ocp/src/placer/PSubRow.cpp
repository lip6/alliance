#include "PSubRow.h"
#include "PBin.h"
#include "PToPlaceIns.h"
#include "PConstants.h"

PSubRow::PSubRow()
    : PContainer(), _bins(),
    _binsXMax()
{}

PSubRow::~PSubRow()
{}

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
    int binscapa;
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
    binscapa = (int)(binswidth * ( 1 - margin));
    
    _bins.reserve(_nBins);
    for (unsigned binnumber = 0; binnumber < _nBins; binnumber++)
    {
        _bins.push_back(PBin());
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
        _bins.back().Init(binbbox, binscapa, *this);
    }
    _row->MergeBBox(_bBox);
}

PBin&
PSubRow::GetBin(const double X)
{
    if (X >= _bins.rbegin()->GetMaxX())
	return *(_bins.rbegin());

    if (X <= _bins.begin()->GetMinX())
	return *(_bins.begin());

    PBinsXMax::iterator srpos = _binsXMax.upper_bound(X);
    return _bins[srpos->second];
}
    
bool
PSubRow::InsertIns(PToPlaceIns& Ins, int BinNumber)
{
    if ((_bins[BinNumber].GetCapa() - _bins[BinNumber].GetSize())
            < Ins.GetWidth())
        return false;
    else
    {
        _bins[BinNumber].AddIns(&Ins);
        return true;
    }
}

unsigned
PSubRow::GetNIns() const
{
    unsigned nins = 0;
    for (PBins::const_iterator BinIt=_bins.begin(); BinIt!=_bins.end(); BinIt++)
	nins += BinIt->GetNIns();
    return nins;
}
    
void
PSubRow::ForceIns(PToPlaceIns& Ins, int BinNumber)
{
    _bins[BinNumber].AddIns(&Ins);
}

#ifndef Abs
#define Abs(x) ((x) < 0.0 ? -(x) : (x))
#endif
double
PSubRow::GetBinCost() const
{
    double BinCost = 0.0;
    for (PBins::const_iterator BinIt = _bins.begin(); BinIt != _bins.end(); BinIt++)
    {
        BinCost += Abs(BinIt->GetSize() - BinIt->GetCapa());
    }
    return BinCost;
}

ofstream& 
PSubRow::Plot(ofstream& out) const
{
  for (PBins::const_iterator BinIt=_bins.begin(); BinIt!=_bins.end(); BinIt++)
  {
      BinIt->Plot(out);
  }
  return out;
}

ofstream& 
PSubRow::PlotLabel(ofstream& out, unsigned TotalMoves) const
{
    for (PBins::const_iterator BinIt=_bins.begin(); BinIt!=_bins.end(); BinIt++)
  {
      BinIt->PlotLabel(out, TotalMoves);
  }
  return out;
}

ostream&
PSubRow::Print(ostream& os) const
{
    return os << "PSubRow: " << GetMinX() << ',' << GetMinY() << " : " << GetMaxX() << ',' << GetMaxY();
}
