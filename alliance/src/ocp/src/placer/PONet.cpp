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
#include "PDetToPlaceIns.h"

#include "PONet.h"

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
