
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
