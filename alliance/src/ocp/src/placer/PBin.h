
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
    unsigned		_sourceHits;
    unsigned		_targetHits;

  public:
			PBin();
    
    void		Init(const PBBox& bbox, double margin, PSubRow &subrow);
    
    PSubRow*		GetSubRow() const	{ return _subRow; }
    bool		GetOrientation() const	{ return _subRow->GetOrientation(); }
    double		GetCapa() const        	{ return _capa; }
    double		GetSize() const        	{ return _size; }
    PToPlaceInss&	GetToPlaceInss()       	{ return _toPlaceInss; }
    unsigned		GetNIns() const		{ return _toPlaceInss.size(); }
    PPos		GetPos() const          { return _pos; }
    void		IncrementSourceHits() { ++_sourceHits; }
    void		IncrementTargetHits() { ++_targetHits; }

    void		AddSize(const double value) { _size += value; _subRow->AddSize(value); }

    bool		UnderOccupied(const double margin) const;
    void		AddIns(PToPlaceIns *ins);
    void		RemoveIns(PToPlaceIns *ins);
    void		RemoveFrontIns(PToPlaceIns *ins);
    void		RemoveBackIns(PToPlaceIns *ins);

    ostream&	Print(ostream& os) const;
    ofstream&	Plot(ofstream& out) const;
    ofstream&	PlotLabel(ofstream& out, unsigned totalMoves) const;
};
#endif /* __PBIN_H */
