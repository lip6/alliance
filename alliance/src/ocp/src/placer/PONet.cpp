#include "PONet.h"
#include "PDetToPlaceIns.h"

PONet::PONet(const losig* sig):
	PNet(sig),
	_bBoxMark(0), _currCostMark(0),
	_saveMark(0), _curr(0)
{
    BBox[0].Clear();
    BBox[1].Clear();
    Cost[0] = 0.0;
    Cost[1] = 0.0;
}

// ======================================================================
// double CurrentCost(const unsigned Flag)
// ======================================================================
double
PONet::CurrentCost(const unsigned Flag)
{
    if (_currCostMark != Flag)
    {
        _currCostMark = Flag;
        return Cost[_curr];
    }
    return 0.0;
}

void
PONet::SaveTemp(const unsigned saveflag)
{
    if (_saveMark != saveflag)
    {
	_curr = !_curr;
    }
    return;
}

// ======================================================================
// void DetPlaceComputeBBox(const unsigned Flag)
// ======================================================================
void
PONet::DetPlaceComputeBBox(const unsigned Flag)
{
#if 0
    if (Flag != _bBoxMark)
    {
        _bBoxMark = Flag;
#endif
        BBox[!_curr].Clear();
        for (PElems::iterator eit = _elems.begin(); eit != _elems.end(); eit++)
	{
	    PDetToPlaceIns* detins = dynamic_cast<PDetToPlaceIns*>(*eit);
	    if (detins)
	    {
		if (detins->IsPlaced())
		{
		    BBox[!_curr].Merge(detins->GetPos());
		}
	    }
	    else
		BBox[!_curr].Merge((*eit)->GetPos());
	}
        Cost[!_curr] = DetPlaceComputeCost(BBox[!_curr]);
#if 0
    }
#endif
}

// ======================================================================
// double DetPlaceComputeCost(PBBox &Box) const
// ======================================================================
double
PONet::DetPlaceComputeCost(PBBox &Box) const
{
    if (!Box.Empty())
	return Box.GetMaxY() - Box.GetMinY()
	     + Box.GetMaxX() - Box.GetMinX();
    else
	return 0.0;
}

double
PONet::DetPlaceUpdateCost()
{
    double save = Cost[_curr];
    BBox[_curr].Clear();
    for (PElems::iterator eit = _elems.begin(); eit != _elems.end(); eit++)
    {
	PDetToPlaceIns* detins = dynamic_cast<PDetToPlaceIns*>(*eit);
	if (detins)
	{
	    if (detins->IsPlaced())
	    {
		BBox[_curr].Merge(detins->GetPos());
	    }
	}
	else
	    BBox[_curr].Merge((*eit)->GetPos());
    }
    Cost[_curr] = DetPlaceComputeCost(BBox[_curr]);
    return (Cost[_curr] - save);
}
