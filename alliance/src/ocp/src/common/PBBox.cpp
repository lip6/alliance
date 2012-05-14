
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
#include "PBBox.h"

PBBox&
PBBox::operator=(const PBBox& bbox)
{
    Min = bbox.Min;
    Max = bbox.Max;
    return *this;
}

bool
PBBox::operator==(const PBBox& bbox) const
{
    return (Min == bbox.Min) && (Max == bbox.Max);
}

bool
PBBox::operator!=(const PBBox& bbox) const
{
    return (Min != bbox.Min) || (Max != bbox.Max);
}

PBBox&
PBBox::Merge(const PBBox& bbox)
{
    if (bbox.Min.GetX() < Min.GetX()) Min.SetX(bbox.Min.GetX());
    if (bbox.Max.GetX() > Max.GetX()) Max.SetX(bbox.Max.GetX());
    if (bbox.Min.GetY() < Min.GetY()) Min.SetY(bbox.Min.GetY());
    if (bbox.Max.GetY() > Max.GetY()) Max.SetY(bbox.Max.GetY());
    return *this;
}

PBBox&
PBBox::Update(const PPos& src, const PPos& dst)
{
    if (dst.GetX() > src.GetX()) {
	if (src.GetX() == Min.GetX()) {
	    Clear();
	    return *this;
	} else if (dst.GetX() > Max.GetX()) {
	    Max.SetX(dst.GetX());
	}
    } else if (dst.GetX() < src.GetX()) {
	if (src.GetX() == Max.GetX()) {
	    Clear();
	    return *this;
	} else if (dst.GetX() < Min.GetX()) {
	    Min.SetX(dst.GetX());
	}
    }
    
    if (dst.GetY() > src.GetY()) {
	if (src.GetY() == Min.GetY()) {
	    Clear();
	    return *this;
	} else if (dst.GetY() > Max.GetY()) {
	    Max.SetY(dst.GetY());
	}
    } else if (dst.GetY() < src.GetY()) {
	if (src.GetY() == Max.GetY()) {
	    Clear();
	    return *this;
	} else if (dst.GetY() < Min.GetY()) {
	    Min.SetY(dst.GetY());
	}
    }
    
    return *this;
}

PBBox&
PBBox::Update(const PBBox& src, const PBBox& dst)
{
    if (dst.GetMinX() < src.GetMinX()) {
	if (dst.GetMinX() < Min.GetX()) {
	    Min.SetX(dst.GetMinX());
	}
    } else if (dst.GetMinX() > src.GetMinX()) {
	if (src.GetMinX() == Min.GetX()) {
	    Clear();
	    return *this;
	}
    }
    
    if (dst.GetMinY() < src.GetMinY()) {
	if (dst.GetMinY() < Min.GetY()) {
	    Min.SetY(dst.GetMinY());
	}
    } else if (dst.GetMinY() > src.GetMinY()) {
	if (src.GetMinY() == Min.GetY()) {
	    Clear();
	    return *this;
	}
    }
    
    if (dst.GetMaxX() > src.GetMaxX()) {
	if (dst.GetMaxX() > Max.GetX()) {
	    Max.SetX(dst.GetMaxX());
	}
    } else if (dst.GetMaxX() < src.GetMaxX()) {
	if (src.GetMaxX() == Max.GetX()) {
	    Clear();
	    return *this;
	}
    }
    
    if (dst.GetMaxY() > src.GetMaxY()) {
	if (dst.GetMaxY() > Max.GetY()) {
	    Max.SetY(dst.GetMaxY());
	}
    } else if (dst.GetMaxY() < src.GetMaxY()) {
	if (src.GetMaxY() == Max.GetY()) {
	    Clear();
	    return *this;
	}
    }
    
    return *this;
}

ostream&
PBBox::Print(ostream& os) const
{
    return os << '(' << Min << ", " << Max << ')';
}
