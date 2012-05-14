
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

#include "PCon.h"

PCon::PCon(const locon* con, phcon* pcon, int dx, int dy)
    : PElem()
    , _con(con)
    , _phcon(pcon)
    , _pos()
{
    if (_phcon)
    {
        _pos = PPos((_phcon->XCON - dx)/PITCH, (_phcon->YCON - dy)/PITCH); 
    }
}

PCon::PCon(const locon* con, PPos pos, char orient)
    : PElem()
    , _con(con)
    , _phcon(NULL)
    , _pos(pos)
    , _orient(orient)
{}

void
PCon::Save(struct phfig *physicalfig, const double dx, const double dy) const {
    if (!_phcon) {
        addphcon(physicalfig
                , _orient
                , _con->NAME
                , (int)(GetPosX() * PITCH + dx)
                , (int)(GetPosY() * PITCH + dy)
                , _orient==NORTH || _orient == SOUTH ? ALU2 : ALU3
                , (_orient==NORTH || _orient == SOUTH ? 2 : 1) * (PITCH/MBK_X_GRID));
    } else {
#if 0
        addphcon(physicalfig,
                _phcon->ORIENT,
                _phcon->NAME,
                _phcon->XCON,
                _phcon->YCON,
                _phcon->LAYER,
                _phcon->WIDTH);
        //modification 15/07/2008 (xtof)
        //why should we add the physical connectors
        //on the existing phfig ?
#endif 
    }

                
                
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
			2 * (PITCH/MBK_X_GRID));
}

ostream&
PCon::Print(ostream& os) const
{
    return os << _con->NAME <<  GetPos();
}
