
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
#include <stdlib.h>
#include <limits.h>
using namespace std;

#include "PBin.h"

#include "PMove.h"

#ifndef Abs
#define Abs(x) ((x) < 0.0 ? -(x) : (x))
#endif

static double
PositionRand(const double Position, const double Distance, const double Max, const double Min)
{
    double BorneInf, BorneSup;

    if ((BorneSup = Position + (double)(int)(Distance * Max + 0.5) ) > Max )
        BorneSup = Max;

    if ((BorneInf = Position - (double)(int)(Distance * Max + 0.5) ) < Min )
        BorneInf = Min;
    
    return BorneInf + (double)(int)((BorneSup - BorneInf) * rand() / (RAND_MAX+1.0));
}

PMove::PMove(PPlacement& placement)
    : _placement(placement)
, _srcIns (NULL)
    , _srcBin(NULL)
    , _srcBinInitCost(0.0)
    , _srcSubRow(NULL)
    , _srcRow(NULL)
    , _srcRowInitCost(0.0)
    , _srcWidth(0.0)
    , _dstBin(NULL)
    , _dstBinInitCost(0.0)
    , _dstSubRow(NULL)
    , _dstRow(NULL)
    , _dstRowInitCost(0.0)
    , _dstIns(NULL)
    , _dstWidth(0.0)
{}

double
PMove::GetDeltaRowCost()
{
    double DeltaRowCost = -_srcRowInitCost;
    DeltaRowCost -= _dstRowInitCost;
    DeltaRowCost += Abs(_srcSubRow->GetCapa() - _srcSubRow->GetSize());
    DeltaRowCost += Abs(_dstSubRow->GetCapa() - _dstSubRow->GetSize());
    return DeltaRowCost;
}

double
PMove::GetDeltaBinCost()
{
    double DeltaBinCost = -_srcBinInitCost;
    DeltaBinCost -= _dstBinInitCost;
    DeltaBinCost += Abs(_srcBin->GetCapa() - _srcBin->GetSize());
    DeltaBinCost += Abs(_dstBin->GetCapa() - _dstBin->GetSize());
    return DeltaBinCost;
}

static const unsigned	PONetSrc		= 1;
static const unsigned	PONetDst		= 2;
static const unsigned	PONetSrcDst		= 3;

double
PMove::GetDeltaNetCost()
{
    // Find affected nets
    // ==================

    _affectedNets.clear();

    for (PIns::PNets::const_iterator net = _srcIns->GetNets().begin(); net != _srcIns->GetNets().end(); ++net)
	_affectedNets[static_cast<PONet*>(*net)] = PONetSrc;

    if (_dstIns != NULL)
	for (PIns::PNets::const_iterator net = _dstIns->GetNets().begin(); net != _dstIns->GetNets().end(); ++net)
	{
	    PONet* ponet = static_cast<PONet*>(*net);
	    
	    if (_affectedNets.find(ponet) == _affectedNets.end())
		_affectedNets[ponet] = PONetDst;
	    else
		_affectedNets[ponet] = PONetSrcDst;
	}
    
    // Compute delta
    // =============

    double Delta = 0;

    for (map<PONet*, unsigned>::iterator it = _affectedNets.begin(); it != _affectedNets.end(); ++it) {
	PONet*    net  = (*it).first;
	unsigned Flag = (*it).second;

	if (Flag == PONetSrc) {
	    net->TempBBox() = net->CurrentBBox();
	    if (net->TempBBox().Update(_srcBin->GetPos(), _dstBin->GetPos()).Empty()) {
	    	for (vector<PElem*>::iterator elem = net->GetElems().begin(); elem != net->GetElems().end(); ++elem) {
		    net->TempBBox().Merge((*elem)->GetPos());
		}
	    }

	    double width = net->TempBBox().GetWidth();
	    if (width == 0.0)
	    {
		width = _srcBin->GetWidth() / 2.0;
	    }
	    net->TempCost() = net->TempBBox().GetHeight() + width;

	    Delta += net->TempCost() - net->CurrentCost();
#ifdef CHECK_COST
	    PBBox check_bbox;
	    vector<PElem*>::iterator efirst = net->GetElems().begin();
	    vector<PElem*>::iterator elast  = net->GetElems().end();
	    while (efirst != elast)
		check_bbox.Merge((*efirst++)->GetPos());

	    if (check_bbox != net->TempBBox()) {
		cout << "error: mauvaise bbox : PONetSrc" << endl;
		cout << "  check_bbox = " << check_bbox << endl;
		cout << "  TempBBox   = " << net->TempBBox() << endl;
		cout << "  CurrentBBox   = " << net->CurrentBBox() << endl;
		cout << "  SrcPos     = " << _srcBin->GetPos() << endl;
		cout << "  DstPos     = " << _dstBin->GetPos() << endl;
	    }
#endif
	} else if (Flag == PONetDst) {
	    net->TempBBox() = net->CurrentBBox();
	    if (net->TempBBox().Update(_dstBin->GetPos(), _srcBin->GetPos()).Empty()) {
	    	for (vector<PElem*>::iterator elem = net->GetElems().begin(); elem != net->GetElems().end(); ++elem) {
		    net->TempBBox().Merge((*elem)->GetPos());
		}
	    }
	    double width = net->TempBBox().GetWidth();
	    if (width == 0.0)
	    {
		width = _dstBin->GetWidth() / 2.0;
	    }
	    net->TempCost() = net->TempBBox().GetHeight() + width;
	    Delta += net->TempCost() - net->CurrentCost();
#ifdef CHECK_COST
	    PBBox check_bbox;
	    vector<PElem*>::iterator efirst = net->GetElems().begin();
	    vector<PElem*>::iterator elast  = net->GetElems().end();
	    while (efirst != elast)
		check_bbox.Merge((*efirst++)->GetPos());

	    if (check_bbox != net->TempBBox()) {
		cout << "error: mauvaise bbox : PONetDst" << endl;
		cout << "  check_bbox = " << check_bbox << endl;
		cout << "  TempBBox   = " << net->TempBBox() << endl;
		cout << "  CurrentBBox   = " << net->CurrentBBox() << endl;
		cout << "  SrcPos     = " << _dstBin->GetPos() << endl;
		cout << "  DstPos     = " << _srcBin->GetPos() << endl;
	    }
#endif
	}
    }
    
    return Delta;
}

void
PMove::Move()
{
    if (_dstIns == NULL) {
	_srcBin->RemoveIns(_srcIns);
	_dstBin->AddIns(_srcIns);
    } else {
	_srcBin->RemoveIns(_srcIns);
	_dstBin->AddIns(_srcIns);
	_dstBin->RemoveIns(_dstIns);
	_srcBin->AddIns(_dstIns);
    }
}
    
bool
PMove::Next(double Dist)
{
    bool MoveCondition;
    unsigned nbrefused = 0;

    // Choisi un mouvement
    // ===================

    do {
	PPos SrcPos;
	double DstX;
        _srcIns = NULL;
        _dstIns = NULL;
        MoveCondition = true;

        _srcIns = &_placement.GetRandIns();
        _srcBin = &(_srcIns->GetBin());
        _srcSubRow = _srcBin->GetSubRow();
	_srcRow = _srcSubRow->GetRow();
        SrcPos = _srcBin->GetPos();
        _srcWidth = _srcIns->GetWidth();
	_srcBinInitCost = Abs(_srcBin->GetCapa() - _srcBin->GetSize());
	_srcRowInitCost = Abs(_srcSubRow->GetCapa() - _srcSubRow->GetSize());
        
	_dstRow = &_placement.GetRow(_srcRow, Dist);
	DstX = PositionRand(SrcPos.GetX(), Dist, _dstRow->GetMaxX(), _dstRow->GetMinX());
	
	_dstSubRow = &(_dstRow->GetSubRow(DstX));
        _dstBin = &(_dstSubRow->GetBin(DstX));
	
	_dstBinInitCost = Abs(_dstBin->GetCapa() - _dstBin->GetSize());
	_dstRowInitCost = Abs(_dstSubRow->GetCapa() - _dstSubRow->GetSize());

	if (_dstBin == _srcBin)
	    MoveCondition = false;

        if (_dstBin->UnderOccupied(_placement.GetMargin())) {
            // Le bin destination est sous-occupé
            // On déplace l'instance
            if (_dstSubRow->GetMax() - _dstSubRow->GetSize() < _srcWidth)
                MoveCondition = false;
        } else {
	    _dstIns = _dstBin->GetToPlaceInss().front();
            _dstWidth = _dstIns->GetWidth();
	    if (_srcSubRow->GetMax() - _srcSubRow->GetSize() < _dstWidth - _srcWidth)
		MoveCondition = false;
	    if (_dstSubRow->GetMax() - _dstSubRow->GetSize() < _srcWidth - _dstWidth)
		MoveCondition = false;
	}
	if (!MoveCondition)
	    ++nbrefused;
	if (nbrefused > (unsigned)(1.5 * _placement.GetNInsToPlace()))
		return false;
    } while (!MoveCondition);
   
    // Deplace les instances
    // =====================
    _dstBin->IncrNbHits();
    
    Move();
    return true;
}


void
PMove::Accept()
{
    // Sauvegarde des cout des nets
    for (map<PONet*, unsigned>::iterator it = _affectedNets.begin(); it != _affectedNets.end(); ++it) {
	PONet*    net  = (*it).first;
	unsigned Flag = (*it).second;

	if (Flag == PONetSrc || Flag == PONetDst) {
	    net->SaveTemp();
	}
    }
}

void
PMove::Reject()
{
    if (_dstIns == NULL) {
	_dstBin->RemoveIns(_srcIns);
	_srcBin->AddIns(_srcIns);
    } else {
	_srcBin->RemoveIns(_dstIns);
	_dstBin->AddIns(_dstIns);
	_dstBin->RemoveIns(_srcIns);
	_srcBin->AddIns(_srcIns);
    }
}
