#include "PDetToPlaceIns.h"
extern "C"{
#include "mut.h"
#include "mph.h"
}
#include "PConstants.h"
#include "PToPlaceIns.h"
#include "PDetSubRow.h"
#include "PONet.h"
#include <algorithm>

PDetToPlaceIns::PDetToPlaceIns(PToPlaceIns* toplaceins):
	PIns(toplaceins->GetInstance()),
        _pos(toplaceins->GetPos()),
	_marginWidth(toplaceins->GetWidth()),
	_leftCorner(PPos(_pos.GetX() - toplaceins->GetWidth() / 2.0,
		_pos.GetY() - toplaceins->GetHeight() / 2.0)),
	_placed(true)
{
    // initialisation des vecteurs de nets,
    // les adresses ne sont pas modifiees
    for (PElem::PNets::iterator nit = toplaceins->GetNets().begin();
	    nit != toplaceins->GetNets().end();
	    nit++)
    {
	_nets.push_back(*nit);
    }
}

void
PDetToPlaceIns::Save(struct phfig *physicalfig, const double dx, const double dy) const
{
    addphins(physicalfig, _instance->FIGNAME, _instance->INSNAME,
	    GetOrientation()?NOSYM:SYM_Y,
	    (int)(_leftCorner.GetX()  * PITCH + dx),
	    (int)(_leftCorner.GetY()  * PITCH + dy));
}

double
PDetToPlaceIns::GetHeight() const
{
    return _subRow->GetHeight();
}

bool
PDetToPlaceIns::GetOrientation() const
{
    return _subRow->GetOrientation();
}

void
PDetToPlaceIns::SetLeftCornerX(const double x)
{
    _leftCorner.SetX(x);
    _pos.SetX(x + GetWidth() / 2.0);
}

double
PDetToPlaceIns::InitCost(const unsigned BBoxFlag, const unsigned CostFlag, const unsigned SaveFlag)
{
    double Cost = 0.0;
    for (PNets::const_iterator nit = _nets.begin(); nit != _nets.end(); nit++)
    {
	PONet* ponet = static_cast<PONet*>(*nit);
	(ponet)->DetPlaceComputeBBox(BBoxFlag);
	(ponet)->SaveTemp(SaveFlag);
	Cost += (ponet)->CurrentCost(CostFlag);
    }
    return Cost;
}

double
PDetToPlaceIns::DetPlaceUpdateCost()
{
    double DeltaCost = 0.0;
    for (PNets::iterator nit = _nets.begin(); nit != _nets.end(); nit++)
	DeltaCost += (static_cast<PONet*>(*nit))->DetPlaceUpdateCost();
    return DeltaCost;
}

double
PDetToPlaceIns::CurrentCost(const unsigned costflag)
{
    double cost = 0.0;
    for (PNets::iterator nit = _nets.begin(); nit != _nets.end(); nit++)
    {
	cost += (static_cast<PONet*>(*nit))->CurrentCost(costflag);
    }
    return cost;
}

ostream&
PDetToPlaceIns::Print(ostream& os) const
{
    return os << "PDetToPlaceIns: " <<
	_instance->FIGNAME << ':' << _instance->INSNAME << ':' << GetPos();
}
