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
ostream&
PRow::Print(ostream& os) const
{
    return os << "PRow: " << GetMinX() << ',' << GetMinY() << " : " << GetMaxX() << ',' << GetMaxY();
}
