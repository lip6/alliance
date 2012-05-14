
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
#include <algorithm>
using namespace std;

#include "mut.h"
#include "mph.h"

#include "PConstants.h"
#include "PToPlaceIns.h"
#include "PDetSubRow.h"
#include "PONet.h"

#include "PDetToPlaceIns.h"

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
