
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
#ifndef __PSubRow_h
#define __PSubRow_h

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "PContainer.h"
#include "PRow.h"

class PBin;
class PToPlaceIns;

class PSubRow : public PContainer {

    friend class PRow;

  public:
   typedef vector <PBin*> PBins;
   typedef map <double, unsigned> PBinsXMax;
  
  private:
    PRow*		_row;
    PBins               _bins;
    PBinsXMax		_binsXMax;
    double              _size;   // somme des Width des bins de la row
    double              _capa;   // l'occupation ideale de la subrow
    double		_max;	// seuil de la sousligne !!ne pas dépasser
    unsigned            _nBins;  // Bins.size();

  public:
    			PSubRow();
    			~PSubRow();
    void		Init(PRow* row, double y, double minx, double maxx, double margin, double maxbinwidth, double minbinwidth);
    bool                InsertIns(PToPlaceIns& Ins, int BinNumber);
    void                ForceIns(PToPlaceIns& Ins, int BinNumber);
    bool		GetOrientation() const		{ return _row->GetOrientation(); }
    PRow*		GetRow()			{ return _row; }
    PBin&               GetBin(const double X);
    PBins&		GetBins()			{ return _bins; }
    double              GetBinsSize() const;
    double              GetBinsCapa() const;
    
    double		GetCapa() const                 { return _capa; }
    double              GetSize() const                 { return _size; }
    double		GetMax() const			{ return _max; }
    unsigned		GetNIns() const;
    double              GetBinCost() const;
    void		AddSize(const double Value)     { _size += Value; }
    unsigned		GetNBins() const;

    ostream&	Print(ostream& os) const;
    ofstream&	Plot(ofstream& out) const;
    ofstream&	PlotLabel(ofstream& out, unsigned TotalMoves) const;
};
#endif /* __PSubRow_h */
