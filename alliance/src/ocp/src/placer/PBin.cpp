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
#include <stdio.h>
#include <iomanip>
using namespace std;

#include "PToPlaceIns.h"

#include "PBin.h"

PBin::PBin()
    : PContainer(), _toPlaceInss(), _nHits(0)
{
}

void
PBin::Init(const PBBox bbox, const double capa, PSubRow &row)
{
    _bBox = bbox;
    _pos = _bBox.GetCenter();
    _capa = capa;
    _size = 0.0;
    _subRow = &row;
}


bool
PBin::UnderOccupied(const double margin) const
{
    // On Considère pour l'instant qu'un Bin est sous-occupé
    // et donc qu'il est intéressant d'y déplacer une instance
    // si il a 20% de vide par rapport à son remplissage idéal

    return (_size <= (1.0 - margin - 0.2) * _capa);
}

void
PBin::AddIns(PToPlaceIns *ins)
{
    _toPlaceInss.push_back(ins);
    AddSize(ins->GetWidth());
    ins->SetBin(this);
}

void
PBin::IncrNbHits()
{
    _nHits++;
}
  
void
PBin::RemoveIns(PToPlaceIns* ins)
{
    _toPlaceInss.remove(ins);
    AddSize(-ins->GetWidth());
    ins->SetBin(NULL);
}

ostream&
PBin::Print(ostream& os) const
{
    return os << "PBin: " << GetMinX() << ',' << GetMinY() << " : " << GetMaxX() << ',' << GetMaxY();
}

// ==================================================
// ofstream& Plot(ofstream& out)
// ==================================================
ofstream& 
PBin::Plot(ofstream& out) const
{
  out << GetMinX() + 0.3 << " " << GetMinY() + 0.3 << endl
      << GetMinX() + 0.3 << " " << GetMaxY() - 0.3 << endl
      << GetMaxX() - 0.3 << " " << GetMaxY() - 0.3 << endl
      << GetMaxX() - 0.3 << " " << GetMinY() + 0.3 << endl
      << GetMinX() + 0.3 << " " << GetMinY() + 0.3 << endl << endl;

  return out;
}

ofstream& 
PBin::PlotLabel(ofstream& out, unsigned totalMoves) const
{
  unsigned x = (unsigned)(GetMinX() + GetMaxX()) / 2;
  unsigned y = (unsigned)(GetMinY() + GetMaxY()) / 2;
  double percent;
  if (totalMoves != 0)
  {
    percent = (_nHits * 100.0) / totalMoves;
    out << "set label \""  
	<< percent << "%\" at " << x << "," << y << " center"
	<< endl;
  }
  return out;
}
