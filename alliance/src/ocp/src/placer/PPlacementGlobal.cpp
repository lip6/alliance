#include <stdio.h>
#include <math.h>
#include <unistd.h>
using namespace std;

#include "PMove.h"
#include "PConstants.h"

#include "PPlacement.h"

static double
DoubleRand(void)
{
    return (double) rand() / (RAND_MAX + 1.0);
}

static bool
Accept(double Temperature, double DeltaCost)
{
    if ((DeltaCost <= 0.0) || ((Temperature != 0.0) && (exp(-DeltaCost / Temperature) > DoubleRand())))
        return true;
    else
        return false;
}

static double
GetStdDev(double Sum, double Square, double n)
{
    if (n <= 1.0)
	return 0.0;
    
    double StdDev = (Square - Sum * Sum / n) / (n - 1.0);
    if (StdDev > 0.0)
	StdDev = sqrt(StdDev);
    else
	StdDev = 0.0;

    return StdDev;
}


#ifdef PLACE_DEBUG
double
PPlacement::GlobalPlaceDebugNetCost()
{
    double NetCost = 0.0;

    vector<PNet>::iterator nfirst = _nets.begin();
    vector<PNet>::iterator nlast  = _nets.end();
    while (nfirst != nlast) {
	PNet& net = *nfirst++;
	PBBox bbox;
	vector<PElem*>::iterator ifirst = net.GetElems().begin();
	vector<PElem*>::iterator ilast  = net.GetElems().end();
	if (ifirst != ilast)
	{
	    while (ifirst != ilast)
		bbox.Merge((*ifirst++)->GetPos());
	    double width = bbox.GetWidth();
	    if (width == 0.0)
	    {
		//toutes les instances dans le meme bin
		//on prend comme largeur la moitie du bin
		ifirst = net.GetElems().begin();
		PToPlaceIns* toplaceins = NULL;
		while (ifirst != ilast)
		{
		    toplaceins = dynamic_cast<PToPlaceIns*>(*ifirst++);
		    if (toplaceins)
			break;
		}
		if (!toplaceins)
		    width = 0;
		else
		    width = toplaceins->GetBin().GetWidth() / 2.0;
	    }

	    NetCost += bbox.GetHeight() + width;
	}
    }
    return NetCost;
}
#endif


#ifndef Abs
#define Abs(x) ((x) < 0.0 ? -(x) : (x))
#endif
double
PPlacement::GetRowCost()
{
    double RowCost = 0.0;
    for (PRows::iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
	RowCost += (*rit)->GetSubRowCost();
    }
    return RowCost;
}

double
PPlacement::GetBinCost()
{
    double BinCost = 0.0;

    for (PRows::iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
	BinCost += (*rit)->GetBinCost();
    }
    return BinCost;
}

double
PPlacement::GetNetCost()
{
    double NetCost = 0.0;

    vector<PONet*>::iterator nfirst = _nets.begin();
    vector<PONet*>::iterator nlast  = _nets.end();
    while (nfirst != nlast) {
	PONet& net = **nfirst++;
	PBBox& bbox = net.CurrentBBox();
	vector<PElem*>::iterator ifirst = net.GetElems().begin();
	vector<PElem*>::iterator ilast  = net.GetElems().end();
	if (ifirst == ilast)
	{
	    cout << " o Placer Warning : Net " << net << " is not connected..." << endl;
	}
	else
	{
	    while (ifirst != ilast)
		bbox.Merge((*ifirst++)->GetPos());
	    double width = bbox.GetWidth();
	    if (width == 0.0)
	    {
		//toutes les instances dans le meme bin
		//on prend comme largeur la moitie du bin
		ifirst = net.GetElems().begin();
		PToPlaceIns* toplaceins = NULL;
		while (ifirst != ilast)
		{
		    toplaceins = dynamic_cast<PToPlaceIns*>(*ifirst++);
		    if (toplaceins)
			break;
		}
		if (!toplaceins)
		    width = 0;
		else
		    width = toplaceins->GetBin().GetWidth() / 2.0;
	    }
	    net.CurrentCost() = bbox.GetHeight() + width;
	    NetCost += net.CurrentCost();
	}
    }
    return NetCost;
}

double
PPlacement::GetCost(double RowCost, double BinCost, double NetCost)
{
    return RowCost / _initRowCost * RowMult + BinCost / _initBinCost * BinMult + NetCost / _initNetCost * NetMult;
}

void
PPlacement::PlaceGlobal()
{
    int	   Iteration;
    double Cost, RowCost, BinCost, NetCost;
    double InitCost;
    double Temperature = 1e30, OldTemperature = 1e30;
    int	   Loop = 0;
    double StdDev = 0, StdDevRow = 0, StdDevBin = 0, StdDevNet = 0;
    double SumCost = 0, SumCostRow = 0, SumCostBin = 0, SumCostNet = 0;
    double SumSquare = 0, SumSquareRow = 0, SumSquareBin = 0, SumSquareNet = 0;
    int	   Accepted = 0;
    double SucRatio = 1.0;
    double Dist = 1.0;
    
    double maxTemperature = 0;
    double maxCost = 0;
    double maxRowCost = 0;
    double maxBinCost = 0;
    double maxNetCost = 0;
    double maxSucRatio = 0;
    double maxDelta = 0;

    ofstream	ofout("alldata.dat");
    if (_verbose)
	cout << " o Beginning global placement ...." << endl;

    PMove Move(*this);

    _initRowCost = RowCost = GetRowCost();
    _initBinCost = BinCost = GetBinCost();
    _initNetCost = NetCost = GetNetCost();

    InitCost = Cost = GetCost(RowCost, BinCost, NetCost);

    if (_verbose) 
    {
      cout << " o Initial RowCost = " << RowCost << endl;
      cout << " o Initial BinCost = " << BinCost << endl;
      cout << " o Initial NetCost = " << NetCost << endl;
      cout << " o Initial Cost = " << Cost << endl;
      cout << " o Computing Initial Temperature ..." << endl;
    }
    // Calcul de la temperature initiale
    for (int i = 0; i < GetNInsToPlace(); ++i) {
	if (!Move.Next(Dist))
	{
	    cout << " o No More Mouvement Possible ....." << endl;
	    return;
	}

	double DeltaRowCost = Move.GetDeltaRowCost();
	double DeltaBinCost = Move.GetDeltaBinCost();
	double DeltaNetCost = Move.GetDeltaNetCost();

	double DeltaCost = GetCost(DeltaRowCost, DeltaBinCost, DeltaNetCost);

        if (Accept(Temperature, DeltaCost)) {
            Move.Accept();

	    Accepted += 1;
	    RowCost += DeltaRowCost;
	    BinCost += DeltaBinCost;
	    NetCost += DeltaNetCost;
	    Cost += DeltaCost;

	    SumCost += Cost; SumSquare += Cost * Cost;
	    SumCostRow += RowCost; SumSquareRow += RowCost * RowCost;
	    SumCostBin += BinCost; SumSquareBin += BinCost * BinCost;
	    SumCostNet += NetCost; SumSquareNet += NetCost * NetCost;
        } else {
            Move.Reject();
	}

	_totalMoves += 1;
    }

    StdDev = GetStdDev(SumCost, SumSquare, Accepted);
    StdDevRow = GetStdDev(SumCostRow, SumSquareRow, Accepted);
    StdDevBin = GetStdDev(SumCostBin, SumSquareBin, Accepted);
    StdDevNet = GetStdDev(SumCostNet, SumSquareNet, Accepted);

    Temperature = 20.0 * StdDev;
    Iteration   = (int)(5.0 * pow(GetNInsToPlace(), 1.33));
#ifdef PLACE_DEBUG
    double Debug = GlobalPlaceDebugNetCost();
      cout << "Debug = " << Debug << endl;
      cout << "NetCost = " << NetCost << endl << endl;
    //assert((NetCost - 1.0 <= Debug) && (Debug <= NetCost + 1.0));
#endif

    // Placement

    double firstTemperature = Temperature;
    double firstCost = Cost;
    double firstRowCost = RowCost;
    double firstBinCost = BinCost;
    double firstNetCost = NetCost;
    double firstSucRatio = SucRatio;
    double firstDist = Dist;
    bool stucked = false;

    do {
	Accepted = 0;
	SumCost = 0, SumCostRow = 0, SumCostBin = 0, SumCostNet = 0;
	SumSquare = 0, SumSquareRow = 0, SumSquareBin = 0, SumSquareNet = 0;

	for (int i = 0; i < Iteration; ++i) {
	    if (!Move.Next(Dist))
	    {
		cout << " o No More Mouvement Possible ....." << endl;
		stucked = true;
		break;
	    }
	    
	    double DeltaRowCost = Move.GetDeltaRowCost();
	    double DeltaBinCost = Move.GetDeltaBinCost();
	    double DeltaNetCost = Move.GetDeltaNetCost();

	    double DeltaCost = GetCost(DeltaRowCost, DeltaBinCost, DeltaNetCost);

	    if (Accept(Temperature, DeltaCost)) {
		Move.Accept();

		Accepted += 1;
		RowCost += DeltaRowCost;
		BinCost += DeltaBinCost;
		NetCost += DeltaNetCost;
		Cost += DeltaCost;

		SumCost += Cost; SumSquare += Cost * Cost;
		SumCostRow += RowCost; SumSquareRow += RowCost * RowCost;
		SumCostBin += BinCost; SumSquareBin += BinCost * BinCost;
		SumCostNet += NetCost; SumSquareNet += NetCost * NetCost;
	    } else {
		Move.Reject();
	    }

	    _totalMoves += 1;
	}
	if (stucked)
	    break;

	Loop += 1;
	OldTemperature = Temperature;
	StdDev = GetStdDev(SumCost, SumSquare, Accepted);
	StdDevRow = GetStdDev(SumCostRow, SumSquareRow, Accepted);
	StdDevBin = GetStdDev(SumCostBin, SumSquareBin, Accepted);
	StdDevNet = GetStdDev(SumCostNet, SumSquareNet, Accepted);
	
	if (StdDev == 0.0)
	    Temperature = 0.0;
	else
	    Temperature = Temperature * max(0.5, exp(-0.7 * Temperature / StdDev));
	SucRatio = Accepted / (double)Iteration;
	Dist = max(0.1, min(Dist * (1.0 - 0.44 + SucRatio), 1.0));
	
	if (_verbose) 
	{
	  cout << "Loop = " << Loop << ", Temperature = " << Temperature << ", Cost = " << Cost << endl;
	cout << "  RowCost = " << RowCost << ", BinCost = " << BinCost << ", NetCost = " << NetCost << endl;
	cout << "  Success Ratio = " << SucRatio * 100.0 << "%, Dist = " << Dist << ", Delta = " << Temperature / OldTemperature << endl;
	}
	else cerr << ".";

	if (_boolPlot)
	{
	  if (Loop == 1)
	  {
	    maxTemperature = Temperature;
	    maxCost = Cost;
	    maxRowCost = firstRowCost;
	    maxBinCost = BinCost;
	    maxNetCost = NetCost;
	    maxSucRatio = SucRatio*100.0;
	    maxDelta = Temperature / OldTemperature * 2;
	    
     ofout << 0 << " " << log10(((firstTemperature/maxTemperature)+1.0)) << " "
	   << firstCost/maxCost << " "<< firstRowCost/maxRowCost << " "
	   << firstBinCost/maxBinCost << " " << firstNetCost/maxNetCost << " "
	   << (firstSucRatio*100.0)/maxSucRatio << " " 
	   << firstDist << " "
	   << 1 << endl;
	  }
	  
	  ofout << Loop << " " << log10(Temperature/maxTemperature+1.0) << " "
	    << Cost/maxCost << " " << RowCost/maxRowCost << " "
	    << BinCost/maxBinCost << " " << NetCost/maxNetCost << " "
	   << (SucRatio*100.0)/maxSucRatio << " " << Dist
	   << " " << (Temperature/OldTemperature)/maxDelta << endl;
	}


#ifdef PLACE_DEBUG
	double Debug = GlobalPlaceDebugNetCost();
	cout << "Debug = " << Debug << endl;
	cout << "NetCost = " << NetCost << endl << endl;
//	assert ((NetCost - 1.0 <= Debug) && (Debug <= NetCost + 1.0));
#endif

    } while (Temperature != 0.0 && StdDev > 0.0001 / Cost);

    if (!_boolPlot)
       unlink("alldata.dat");

    if (!stucked)
    {
	// Freeze out

	Accepted = 0;
	SumCost = 0, SumCostRow = 0, SumCostBin = 0, SumCostNet = 0;
	SumSquare = 0, SumSquareRow = 0, SumSquareBin = 0, SumSquareNet = 0;
	
	for (int i = 0; i < Iteration; ++i) {
	    if (!Move.Next(Dist))
	    {
		cout << " o No More Mouvement Possible ....." << endl;
		stucked = true;
		break;
	    }

	    double DeltaRowCost = Move.GetDeltaRowCost();
	    double DeltaBinCost = Move.GetDeltaBinCost();
	    double DeltaNetCost = Move.GetDeltaNetCost();

	    double DeltaCost = GetCost(DeltaRowCost, DeltaBinCost, DeltaNetCost);

	    if (Accept(Temperature, DeltaCost)) {
		Move.Accept();

		Accepted += 1;
		RowCost += DeltaRowCost;
		BinCost += DeltaBinCost;
		NetCost += DeltaNetCost;
		Cost += DeltaCost;

		SumCost += Cost; SumSquare += Cost * Cost;
		SumCostRow += RowCost; SumSquareRow += RowCost * RowCost;
		SumCostBin += BinCost; SumSquareBin += BinCost * BinCost;
		SumCostNet += NetCost; SumSquareNet += NetCost * NetCost;
	    } else {
		Move.Reject();
	    }

	    _totalMoves += 1;
	}
    }

    if (_verbose) 
    {
	cout << " o Global Placement finished ....." << endl;
	cout << " o Gain for RowCost      = " << 100.0 * (_initRowCost - RowCost) / _initRowCost << "%" << endl;
	cout << " o Gain for BinCost      = " << 100.0 * (_initBinCost - BinCost) / _initBinCost << "%" << endl;
	cout << " o Gain for NetCost      = " << 100.0 * (_initNetCost - NetCost) / _initNetCost << "%" << endl;
	cout << " o NetCost Estimated = " << NetCost << endl;
    }
}

PToPlaceIns&
PPlacement::GetRandIns()
{
    return *_toPlaceInss[(int) ((double)_nInsToPlace * rand() / (RAND_MAX + 1.0))];
}
