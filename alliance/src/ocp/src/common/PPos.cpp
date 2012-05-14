
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
#include "PPos.h"
#include <float.h>

const PPos PPos::Min(-DBL_MAX, -DBL_MAX);
const PPos PPos::Max(+DBL_MAX, +DBL_MAX);

bool  PPos::operator==(const PPos& pos) const
{
    return (X == pos.X) && (Y == pos.Y);
}

bool  PPos::operator!=(const PPos& pos) const
{
    return (X != pos.X) || (Y != pos.Y);
}

bool  PPos::operator<(const PPos& pos) const
{
    return (X < pos.X) || ((X == pos.X) && (Y < pos.Y));
}

bool  PPos::operator>(const PPos& pos) const
{
    return (X > pos.X) || ((X == pos.X) && (Y > pos.Y));
}

ostream&
PPos::Print(ostream& os) const
{
    return os << '(' << X << " , " << Y << ')';
}
