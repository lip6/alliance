
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
#include "mut.h"
#include "mph.h"

#include "PConstants.h"

#include "PCon.h"

PCon::PCon(const locon* con):
		PElem(), _con(con), _pos()
{}

PCon::PCon(const locon* con, PPos pos, char orient):
		PElem(), _con(con), _pos(pos), _orient(orient)
{}

void
PCon::Save(struct phfig *physicalfig, const double dx, const double dy) const
{

    addphcon(physicalfig,
			_orient,
			_con->NAME,
			(int)(GetPosX() * PITCH + dx),
			(int)(GetPosY() * PITCH + dy),
			_orient==NORTH || _orient == SOUTH ? ALU2 : ALU3,
			(_orient==NORTH || _orient == SOUTH ? 2 : 1) * (PITCH/5));
}

void
PCon::RingSave(struct phfig *physicalfig, const double dx, const double dy) const
{
    addphcon(physicalfig,
			_orient,
			_con->NAME,
			(int)(GetPosX() * PITCH + dx),
			(int)(GetPosY() * PITCH + dy),
			ALU2,
			2 * (PITCH/5));
}

ostream&
PCon::Print(ostream& os) const
{
    return os << _con->NAME <<  GetPos();
}
