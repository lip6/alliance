/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
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
    double              _capa;   // l'occupation ideale de la row
    double		_max;	// seuil de la ligne !!ne pas dépasser
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
