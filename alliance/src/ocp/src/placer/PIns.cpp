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
#include "mut.h"
#include "mph.h"

#include "PConstants.h"

#include "PIns.h"

PIns::PIns(const loins *instance):
PElem(), _instance(instance)
{
    phfig* phmodel = getphfig(instance->FIGNAME, '0');
    double InsWidth = (double)(phmodel->XAB2 - phmodel->XAB1);
    _width = (double)(int)(InsWidth / PITCH + 0.5);
}

PBBox
PIns::GetBBox() const
{
    return PBBox(PPos(GetPosX() - GetWidth() / 2.0, GetPosY() - GetHeight() / 2.0),
                 PPos(GetPosX() + GetWidth() / 2.0, GetPosY() + GetHeight() / 2.0));
}

ofstream& 
PIns::Plot(ofstream& out) const
{
  out << GetBBox().GetMinX()+0.4 << " " << GetBBox().GetMinY()+0.4 << endl
      << GetBBox().GetMinX()+0.4 << " " << GetBBox().GetMaxY()-0.4 << endl
      << GetBBox().GetMaxX()-0.4 << " " << GetBBox().GetMaxY()-0.4 << endl
      << GetBBox().GetMaxX()-0.4 << " " << GetBBox().GetMinY()+0.4 << endl
      << GetBBox().GetMinX()+0.4 << " " << GetBBox().GetMinY()+0.4 << endl << endl;

  return out;
}
