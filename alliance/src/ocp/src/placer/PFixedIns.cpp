
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
#include "PConstants.h"

#include "PFixedIns.h"

PFixedIns::PFixedIns(const loins *instance, const phins* pins, int dx, int dy):
		PIns(instance),
		_pins(pins),
		_pos(), _height(0.0)
{
    phfig* phmodel = getphfig(pins->FIGNAME, '0');
    double pinsheight = (double)(phmodel->YAB2 - phmodel->YAB1);
    _height = (double)(int)(pinsheight / PITCH + 0.5);
    double x = (double)((_pins->XINS - dx) / PITCH) + _width / 2.0;
    double y = (double)((_pins->YINS - dy) / PITCH) + _height / 2.0;
    _pos = PPos(x, y);
}

ostream&
PFixedIns::Print(ostream& os) const
{
    return os << "PFixedIns: " <<
	_instance->FIGNAME << ':' << _instance->INSNAME << ':' << GetPos();
}
