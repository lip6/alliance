
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
#ifndef __PBBOX_H
#define __PBBOX_H

#include <iostream>
#include <PPos.h>
using namespace std;

class PBBox {

  protected:

    PPos Min;
    PPos Max;
    
  public:
    
    PBBox() : Min(PPos::Max), Max(PPos::Min) { }
    PBBox(const PPos& min, const PPos& max) : Min(min), Max(max) { }
    PBBox(const PBBox& box) : Min(box.Min), Max(box.Max) { }
    
    PBBox& operator=(const PBBox& bbox);
    bool operator==(const PBBox& bbox) const;
    bool operator!=(const PBBox& bbox) const;
  
    PPos GetMin() const { return Min; }
    PPos GetMax() const { return Max; }
    double GetMinX() const { return Min.GetX(); }
    double GetMinY() const { return Min.GetY(); }
    double GetMaxX() const { return Max.GetX(); }
    double GetMaxY() const { return Max.GetY(); }
    double GetWidth() const { return Max.GetX() - Min.GetX(); }
    double GetHeight() const { return Max.GetY() - Min.GetY(); }
    double GetCenterX() const { return GetMinX() + GetWidth() / 2; }
    double GetCenterY() const { return GetMinY() + GetHeight() / 2; }
    PPos GetCenter() const { return PPos(GetCenterX(), GetCenterY()); }

    void SetMin(const PPos& min) { Min = min; }
    void SetMax(const PPos& max) { Max = max; }
    void SetMinX(const double& x) { Min.SetX(x); }
    void SetMinY(const double& y) { Min.SetY(y); }
    void SetMaxX(const double& x) { Max.SetX(x); }
    void SetMaxY(const double& y) { Max.SetY(y); }

    bool Empty() const { return Min > Max; }
    void Clear() { Min = PPos::Max; Max = PPos::Min; }
    
    PBBox& Merge(const PPos& pos)
    {
	if (pos.GetX() < Min.GetX()) Min.SetX(pos.GetX());
	if (pos.GetX() > Max.GetX()) Max.SetX(pos.GetX());
	if (pos.GetY() < Min.GetY()) Min.SetY(pos.GetY());
	if (pos.GetY() > Max.GetY()) Max.SetY(pos.GetY());
	return *this;
    };

    PBBox& Merge(const PBBox& bbox);

    // Update une bbox suite a un mouvement. Si l'update est 
    // impossible, Clear() la bbox (on a alors bbox.Empty() == true).
    // utilisation:
    // if (bbox.Update(Src->GetPos(), Dst->GetPos()).Empty() == true) {
    //     foreach(Ins)
    //         bbox.Merge(Ins->GetPos());
    // }
    PBBox& Update(const PPos& src, const PPos& dst);
    PBBox& Update(const PBBox& src, const PBBox& dst);

    
    ostream& Print(ostream& os) const;
};

inline ostream& operator<<(ostream& os, const PBBox& bbox)
{
    return bbox.Print(os);
}

inline ostream& operator<<(ostream& os, const PBBox* bbox)
{
    return bbox ? bbox->Print(os) : os << ("nil");
}

#endif /* P_BBOX_H */
