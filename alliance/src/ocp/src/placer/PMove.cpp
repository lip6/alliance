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
    : Placement(placement)
    , SrcIns(0)
    , DstIns(0)
{
}

double
PMove::GetDeltaRowCost()
{
    double DeltaRowCost = -SrcRowInitCost;
    DeltaRowCost -= DstRowInitCost;
    DeltaRowCost += Abs(_srcSubRow->GetCapa() - _srcSubRow->GetSize());
    DeltaRowCost += Abs(_dstSubRow->GetCapa() - _dstSubRow->GetSize());
    return DeltaRowCost;
}

double
PMove::GetDeltaBinCost()
{
    double DeltaBinCost = -SrcBinInitCost;
    DeltaBinCost -= DstBinInitCost;
    DeltaBinCost += Abs(SrcBin->GetCapa() - SrcBin->GetSize());
    DeltaBinCost += Abs(DstBin->GetCapa() - DstBin->GetSize());
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

    AffectedNets.clear();

    for (PIns::PNets::const_iterator net = SrcIns->GetNets().begin(); net != SrcIns->GetNets().end(); ++net)
	AffectedNets[static_cast<PONet*>(*net)] = PONetSrc;

    if (DstIns != NULL)
	for (PIns::PNets::const_iterator net = DstIns->GetNets().begin(); net != DstIns->GetNets().end(); ++net)
	{
	    PONet* ponet = static_cast<PONet*>(*net);
	    
	    if (AffectedNets.find(ponet) == AffectedNets.end())
		AffectedNets[ponet] = PONetDst;
	    else
		AffectedNets[ponet] = PONetSrcDst;
	}
    
    // Compute delta
    // =============

    double Delta = 0;

    for (map<PONet*, unsigned>::iterator it = AffectedNets.begin(); it != AffectedNets.end(); ++it) {
	PONet*    net  = (*it).first;
	unsigned Flag = (*it).second;

	if (Flag == PONetSrc) {
	    net->TempBBox() = net->CurrentBBox();
	    if (net->TempBBox().Update(SrcBin->GetPos(), DstBin->GetPos()).Empty()) {
	    	for (vector<PElem*>::iterator elem = net->GetElems().begin(); elem != net->GetElems().end(); ++elem) {
		    net->TempBBox().Merge((*elem)->GetPos());
		}
	    }

	    double width = net->TempBBox().GetWidth();
	    if (width == 0.0)
	    {
		width = SrcBin->GetWidth() / 2.0;
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
		cout << "  SrcPos     = " << SrcBin->GetPos() << endl;
		cout << "  DstPos     = " << DstBin->GetPos() << endl;
	    }
#endif
	} else if (Flag == PONetDst) {
	    net->TempBBox() = net->CurrentBBox();
	    if (net->TempBBox().Update(DstBin->GetPos(), SrcBin->GetPos()).Empty()) {
	    	for (vector<PElem*>::iterator elem = net->GetElems().begin(); elem != net->GetElems().end(); ++elem) {
		    net->TempBBox().Merge((*elem)->GetPos());
		}
	    }
	    double width = net->TempBBox().GetWidth();
	    if (width == 0.0)
	    {
		width = DstBin->GetWidth() / 2.0;
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
		cout << "  SrcPos     = " << DstBin->GetPos() << endl;
		cout << "  DstPos     = " << SrcBin->GetPos() << endl;
	    }
#endif
	}
    }
    
    return Delta;
}

void
PMove::Move()
{
    if (DstIns == NULL) {
	SrcBin->RemoveIns(SrcIns);
	DstBin->AddIns(SrcIns);
    } else {
	SrcBin->RemoveIns(SrcIns);
	DstBin->AddIns(SrcIns);
	DstBin->RemoveIns(DstIns);
	SrcBin->AddIns(DstIns);
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
        SrcIns = NULL;
        DstIns = NULL;
        MoveCondition = true;

        SrcIns = &Placement.GetRandIns();
        SrcBin = &(SrcIns->GetBin());
        _srcSubRow = SrcBin->GetSubRow();
	_srcRow = _srcSubRow->GetRow();
        SrcPos = SrcBin->GetPos();
        SrcWidth = SrcIns->GetWidth();
	SrcBinInitCost = Abs(SrcBin->GetCapa() - SrcBin->GetSize());
	SrcRowInitCost = Abs(_srcSubRow->GetCapa() - _srcSubRow->GetSize());
        
	_dstRow = &Placement.GetRow(_srcRow, Dist);
	DstX = PositionRand(SrcPos.GetX(), Dist, _dstRow->GetMaxX(), _dstRow->GetMinX());
	
	_dstSubRow = &(_dstRow->GetSubRow(DstX));
        DstBin = &(_dstSubRow->GetBin(DstX));
	
	DstBinInitCost = Abs(DstBin->GetCapa() - DstBin->GetSize());
	DstRowInitCost = Abs(_dstSubRow->GetCapa() - _dstSubRow->GetSize());

	if (DstBin == SrcBin)
	    MoveCondition = false;

        if (DstBin->UnderOccupied(Placement.GetMargin())) {
            // Le bin destination est sous-occupé
            // On déplace l'instance
            if (_dstSubRow->GetMax() - _dstSubRow->GetSize() < SrcWidth)
                MoveCondition = false;
        } else {
	    DstIns = DstBin->GetToPlaceInss().front();
            DstWidth = DstIns->GetWidth();
	    if (_srcSubRow->GetMax() - _srcSubRow->GetSize() < DstWidth - SrcWidth)
		MoveCondition = false;
	    if (_dstSubRow->GetMax() - _dstSubRow->GetSize() < SrcWidth - DstWidth)
		MoveCondition = false;
	}
	if (!MoveCondition)
	    ++nbrefused;
	if (nbrefused > (unsigned)(1.5 * Placement.GetNInsToPlace()))
		return false;
    } while (!MoveCondition);
   
    // Deplace les instances
    // =====================
    DstBin->IncrNbHits();
    
    Move();
    return true;
}


void
PMove::Accept()
{
    // Sauvegarde des cout des nets
    for (map<PONet*, unsigned>::iterator it = AffectedNets.begin(); it != AffectedNets.end(); ++it) {
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
    if (DstIns == NULL) {
	DstBin->RemoveIns(SrcIns);
	SrcBin->AddIns(SrcIns);
    } else {
	SrcBin->RemoveIns(DstIns);
	DstBin->AddIns(DstIns);
	DstBin->RemoveIns(SrcIns);
	SrcBin->AddIns(SrcIns);
    }
}
