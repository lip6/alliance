#ifndef __PPLACEMENT_H
#define __PPLACEMENT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <hash_map.h>
#include "PToPlaceIns.h"
#include "PFixedIns.h"
#include "PCon.h"
#include "PONet.h"
#include "PBBox.h"
#include "PDetSubRow.h"
#include "mut.h"
#include "mlo.h"
#include "iocheader.h"
using namespace std;

struct eqstr
{
    bool operator()(const char* s1, const char* s2) const
    {
	return strcmp(s1, s2) == 0;
    }
};

class PPlacement {
    public:
	typedef vector<PRow>	PRows;
	typedef vector<PONet*> PONets;
	typedef vector<PToPlaceIns*> PToPlaceInss;
	typedef vector<PFixedIns*> PFixedInss;
	typedef vector<PCon*> PCons;
	typedef vector <bool> PrePlaceRow;
	typedef vector<PrePlaceRow> PrePlaceTab;
	typedef vector<PDetSubRow> PDetSubRows;

    private:
	typedef map<double, unsigned, less<double> > PRowsYMax;
	typedef map<double, unsigned, greater<double> > PRowsYMinInv;
	typedef hash_map<long, PONet*> PNetMap;
	typedef hash_map<const char*, PIns*, hash<const char *>, eqstr>PInsMap;
	typedef hash_map<const char*, PCon*, hash<const char *>, eqstr> PConMap;
	typedef hash_map<const char*, locon*, hash<const char *>, eqstr> PLoconMap;
	typedef hash_map<const char*, int, hash<const char *>, eqstr> PFixedMap;
	typedef hash_map<const char*, PDetToPlaceIns*, hash<const char*>, eqstr> PDetInsMap;

	PBBox		BBox;
	int			_dx;
	int			_dy;

	lofig*		_fig;   
	phfig*		_prePlaceFig;
	PToPlaceInss	_toPlaceInss;
	PFixedInss	_fixedInss;
	PCons		_cons;
	PONets		_nets;
	PRows		_rows;
	PRowsYMax	_rowsYMax;
	PRowsYMinInv	_rowsYMinInv;
	// placement detaille
	PDetSubRows	_detSubRows;
	double		_detInitNetCost;

	bool		_rowZeroOrientation;

	double		_initNetCost;
	double		_initRowCost;
	double		_initBinCost;

	// parametres
	double		_margin;
	int		_maxDetLoop; // Maximum nb of loops for detailed placement
	double		RowMult;
	double		BinMult;
	double		NetMult;
	bool		_placeCons;
	bool		_ringPlaceCons;
	con_list*	_PtList;
	bool		_iocFile;
	char*		_iocFileName;
	bool		_boolPlot;
	bool		_verbose;
	bool		_prePlace;
	bool		_eqMargin;
	unsigned	_totalMoves;

	// Placement caracteristics
	char*		_fileName;
	unsigned	_elems;
	unsigned	_nIns;
	unsigned	_nInsToPlace;
	double		_binsWidth;
	double		_binsCapa;
	double		_binsMinWidth;
	double		_binsMaxWidth;
	double		_sumToPlaceInssWidth;
	double		_biggestToPlaceInsWidth;

	void		Init(lofig* cell, int NbRows);
	double		GetRowCost();
	double		GetBinCost();
	double		GetNetCost();
	double		GetCost(double RowCost, double BinCost, double NetCost);
	void		PlaceGlobal();
	void		PlaceFinal();

    public:
	PPlacement(bool conflg, bool ringflg, double rowmult, double binmult, double netmult,
		bool iocfile, char *iocfilename, bool plotflg,
		bool verbose, bool preflg, bool eqmargin,
		struct phfig* physfig,
		char* filename):
	    _prePlaceFig(physfig),
	RowMult(rowmult), BinMult(binmult), NetMult(netmult),
	_placeCons(conflg), _ringPlaceCons(ringflg), _iocFile(iocfile), _iocFileName(iocfilename),
	_boolPlot(plotflg), _verbose(verbose), _prePlace(preflg), _eqMargin(eqmargin),
	_totalMoves(0),_fileName(filename)
	{}

	~PPlacement();

	void		SetMargin(const double Value)	{ _margin = Value; }
	double		GetMargin() const		{ return _margin; }
	void		SetMaxDetLoop(const int loop)	{ _maxDetLoop = loop; }
	void		SetRowMult(const double Value)	{ RowMult = Value; }
	double		GetRowMult() const		{ return RowMult; }
	void		SetBinMult(const double Value)	{ BinMult = Value; }
	double		GetBinMult() const		{ return BinMult; }
	void		SetNetMult(const double Value)	{ NetMult = Value; }
	double		GetNetMult() const		{ return NetMult; }
	void		Place(lofig* cell, int NbRows);

	void		PlotAll(char* output) const;
	void		PlotFinal(char* output) const;
	void		PlotOnlyInstances(char* output) const; 
	void		PlotStat();
	void		PlotOnlyBins(char* output) const;

	double		GetMinX() const			{ return BBox.GetMinX(); }
	double		GetMinY() const			{ return BBox.GetMinY(); }
	double		GetMaxX() const			{ return BBox.GetMaxX(); }
	double		GetMaxY() const			{ return BBox.GetMaxY(); }
	double		GetWidth() const		{ return BBox.GetWidth(); }
	double		GetHeight() const		{ return BBox.GetHeight(); }
	int		GetNInsToPlace() const		{ return _nInsToPlace; }

	bool		GetBoolPlot() const		{ return _boolPlot; }
	void		SetBoolPlot(bool value)		{ _boolPlot = value; }

	double		GetOccCost() const;
	void		InitBBoxCost();
	double		TempBBoxCost();
	void		ParseIocFile(PLoconMap& ploconmap);
	void		SetPosIocFile(PConMap& pconmap);

	PToPlaceIns&	GetRandIns();
	PRow&		GetRow(const PRow* row, const double distance);

	void		FinalInitialize();
	bool		FinalOptimize();

	ostream&	Print(ostream& os) const;

	int		Save();

	// Debug Methods

	double		DetPlaceDebugNetCost();
	double		GlobalPlaceDebugNetCost();
	double		DebugRowCost();
	double		DebugBinCost();

    private:
	PFixedIns*		InsertFixedIns(const loins* ins, const phins* pins, const int dx, const int dy);
	PToPlaceIns*	InsertToPlaceIns(const loins* ins);
	PCon*		InsertCon(const locon* con);
	PCon*		InsertCon(const locon* con, PPos position, const char orientation);
	PONet*		InsertNet(const losig* sig);
	int		AddRowend(struct phfig* physicalfig);
	double		DetPlaceNetCost();
	void		GenerateConsPlacement();
	void		GenerateRingConsPlacement();
	void		InitPlace(int nbrows);
	void		InitPlaceWithPrePlace();
	void		CreateSubRows(PRow* row, PrePlaceTab& tabpreplace,int coordy, int nbsubrows, int Width);
	int		CheckCreateRow(PrePlaceTab& tabpreplace, int coordy, int Width);
	void		PlotInstances(ofstream& out) const;
};

static inline ostream& operator<<(ostream& os, const PPlacement& placement)
{
    return placement.Print(os);
}

static inline ostream& operator<<(ostream& os, const PPlacement* placement)
{
    return placement ? placement->Print(os) : os << "(nil)";
}

#endif /* __PPLACEMENT_H */
