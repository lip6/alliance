
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
#ifndef __PPLACEMENT_H
#define __PPLACEMENT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

#include "mut.h"
#include "mlo.h"

#include "PToPlaceIns.h"
#include "PFixedIns.h"
#include "PCon.h"
#include "PONet.h"
#include "PBBox.h"
#include "PDetSubRow.h"
#include "iocheader.h"

struct eqstr
{
    inline size_t operator()(const char *s) const
    {
        size_t hash = 1;
        for (; *s; ++s) hash = hash * 5 + *s;
        return hash;
    }

    inline bool operator()(const char* s1, const char* s2) const
    {
	return strcmp(s1, s2) == 0;
    }
};

class PPlacement {
  public:
    typedef vector<PRow*>    PRows;
    typedef vector<PONet*> PONets;
    typedef vector<PToPlaceIns*> PToPlaceInss;
    typedef vector<PFixedIns*> PFixedInss;
    typedef vector<PCon*> PCons;
    typedef vector <bool> PrePlaceRow;
    typedef vector<PrePlaceRow> PrePlaceTab;
    typedef vector<PDetSubRow*> PDetSubRows;

  private:
    typedef map< double, unsigned, less<double> > PRowsYMax;
    typedef map< double, unsigned, greater<double> > PRowsYMinInv;
    typedef unordered_map<long, PONet*> PNetMap;
    typedef unordered_map<const char*, PIns*, eqstr, eqstr> PInsMap;
    typedef unordered_map<const char*, PCon*, eqstr, eqstr> PConMap;
    typedef unordered_map<const char*, locon*, eqstr, eqstr> PLoconMap;
    typedef unordered_map<const char*, int, eqstr, eqstr> PFixedMap;
    typedef unordered_map<const char*, PDetToPlaceIns*, eqstr, eqstr> PDetInsMap;

    PBBox         BBox;
    int           _dx;
    int           _dy;

    lofig*        _fig;   
    phfig*        _prePlaceFig;
    PToPlaceInss  _toPlaceInss;
    PFixedInss    _fixedInss;
    PCons         _cons;
    PONets        _nets;
    PRows         _rows;
    PRowsYMax     _rowsYMax;
    PRowsYMinInv  _rowsYMinInv;
    // placement detaille
    PDetSubRows   _detSubRows;
    double        _detInitNetCost;

    bool          _rowZeroOrientation;

    double        _initNetCost;
    double        _initRowCost;
    double        _initBinCost;

    // parametres
    double        _margin;
    double        _realMargin;
    int           _maxDetLoop; // Maximum nb of loops for detailed placement
    double        RowMult;
    double        BinMult;
    double        NetMult;
    bool          _placeCons;
    bool          _ringPlaceCons;
    con_list*     _PtList;
    bool          _iocFile;
    char*         _iocFileName;
    bool          _boolPlot;
    bool          _verbose;
    bool          _prePlace;
    bool          _eqMargin;
    unsigned      _totalMoves;
    unsigned      _sourceEqualTargetMovementNumber;
    unsigned      _surOccupationTargetMovementNumber;
    unsigned      _impossibleExchangeMovementNumber;
    unsigned      _acceptedMoveNumber;
    unsigned      _acceptedExchangeNumber;
    unsigned      _rejectedMoveNumber;
    unsigned      _rejectedExchangeNumber;

    // Placement caracteristics
    char*         _fileName;
    unsigned      _elems;
    unsigned      _nIns;
    unsigned      _nInsToPlace;
    double        _binsWidth;
    double        _binsCapa;
    double        _binsMinWidth;
    double        _binsMaxWidth;
    double        _sumToPlaceInssWidth;
    double        _biggestToPlaceInsWidth;

    void        Init(lofig* cell, int NbRows);
    double      GetRowCost();
    double      GetBinCost();
    double      GetNetCost();
    double      GetCost(double RowCost, double BinCost, double NetCost);
    void        PlaceGlobal();
    void        PlaceFinal();

  public:
    PPlacement(bool conflg, bool ringflg, double rowmult, double binmult, double netmult,
               bool iocfile, char *iocfilename, bool plotflg,
               bool verbose, bool preflg, bool eqmargin,
               struct phfig* physfig,
               char* filename)
      : _dx                               (0)
      , _dy                               (0)
      , _fig                              (NULL)
      , _prePlaceFig                      (physfig)
      , _detInitNetCost                   (0.0)
      , _rowZeroOrientation               (false)
      , _initNetCost                      (0.0)
      , _initRowCost                      (0.0)
      , _initBinCost                      (0.0)
      , _margin                           (0.0)
      , _realMargin                       (0.0)
      , _maxDetLoop                       (0)
      , RowMult                           (rowmult)
      , BinMult                           (binmult)
      , NetMult                           (netmult)
      , _placeCons                        (conflg)
      , _ringPlaceCons                    (ringflg)
      , _PtList                           (NULL)
      , _iocFile                          (iocfile)
      , _iocFileName                      (iocfilename)
      , _boolPlot                         (plotflg)
      , _verbose                          (verbose)
      , _prePlace                         (preflg)
      , _eqMargin                         (eqmargin)
      , _totalMoves                       (0)
      , _sourceEqualTargetMovementNumber  (0)
      , _surOccupationTargetMovementNumber(0)
      , _impossibleExchangeMovementNumber (0)
      , _acceptedMoveNumber               (0)
      , _acceptedExchangeNumber           (0)
      , _rejectedMoveNumber               (0)
      , _rejectedExchangeNumber           (0)
      , _fileName                         (filename)
      , _elems                            (0)
      , _nIns                             (0)
      , _nInsToPlace                      (0)
      , _binsWidth                        (0.0)
      , _binsCapa                         (0.0)
      , _binsMinWidth                     (0.0)
      , _binsMaxWidth                     (0.0)
      , _sumToPlaceInssWidth              (0.0)
      , _biggestToPlaceInsWidth           (0.0)
    { }

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

	void		PlotAll(const string& output) const;
	void		PlotFinal(const string& output) const;
	void		PlotOnlyInstances(const string& output) const; 
	void		PlotStat();
	void		PlotOnlyBins(const string& output) const;

        double          GetBinsSize() const;
        double          GetBinsCapa() const;
        double          GetSubRowsCapa() const;

	double		GetMinX() const			{ return BBox.GetMinX(); }
	double		GetMinY() const			{ return BBox.GetMinY(); }
	double		GetMaxX() const			{ return BBox.GetMaxX(); }
	double		GetMaxY() const			{ return BBox.GetMaxY(); }
	double		GetWidth() const		{ return BBox.GetWidth(); }
	double		GetHeight() const		{ return BBox.GetHeight(); }
	int		GetNInsToPlace() const		{ return _nInsToPlace; }

	bool		GetBoolPlot() const		{ return _boolPlot; }
	void		SetBoolPlot(bool value)		{ _boolPlot = value; }

        void            IncrImpossibleExchangeMovementNumber()
        { ++_impossibleExchangeMovementNumber; }
        void            IncrSourceEqualTargetMovementNumber()
        { ++_sourceEqualTargetMovementNumber; }
        void            IncrSurOccupationTargetMovementNumber()
        { ++_surOccupationTargetMovementNumber; }
        void            IncrAcceptedMoveNumber()
        { ++_acceptedMoveNumber; }
        void            IncrAcceptedExchangeNumber()
        { ++_acceptedExchangeNumber; }
        void            IncrRejectedMoveNumber()
        { ++_rejectedMoveNumber; }
        void            IncrRejectedExchangeNumber()
        { ++_rejectedExchangeNumber; }

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
	PCon*		InsertCon(const locon* con, phcon* pcon=NULL, int dx=0, int dy=0);
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
