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
