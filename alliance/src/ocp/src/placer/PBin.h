#ifndef __PBIN_H
#define __PBIN_H

#include <iostream>
#include <fstream>
#include <list>
#include "PSubRow.h"
using namespace std;

class PToPlaceIns;

class PBin : public PContainer{
    friend class PSubRow;

  public:
    typedef list<PToPlaceIns*> PToPlaceInss;

  private:
    PSubRow*		_subRow;
    double		_capa;   	// Objective Occupation of the bin
    double		_size;   	// Sum of instances widths
    PToPlaceInss	_toPlaceInss;  	// Instances of bin.
    PPos		_pos;		// Position of the center of the bin
    unsigned		_nHits;

  public:
			PBin();
    
    void		Init(const PBBox bbox, const double capacity, PSubRow &subrow);
    
    PSubRow*		GetSubRow() const	{ return _subRow; }
    bool		GetOrientation() const	{ return _subRow->GetOrientation(); }
    double		GetCapa() const        	{ return _capa; }
    double		GetSize() const        	{ return _size; }
    PToPlaceInss&	GetToPlaceInss()       	{ return _toPlaceInss; }
    unsigned		GetNIns() const		{ return _toPlaceInss.size(); }
    PPos		GetPos() const          { return _pos; }
    void		IncrNbHits();

    void		AddSize(const double value) { _size += value; _subRow->AddSize(value); }

    bool		UnderOccupied(const double margin) const;
    void		AddIns(PToPlaceIns *ins);
    void		RemoveIns(PToPlaceIns *ins);

    ostream&	Print(ostream& os) const;
    ofstream&	Plot(ofstream& out) const;
    ofstream&	PlotLabel(ofstream& out, unsigned totalMoves) const;
};
#endif /* __PBIN_H */
