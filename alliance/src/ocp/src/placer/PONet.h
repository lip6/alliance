
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
#ifndef __PONET_H
#define __PONET_H

#include "PNet.h"
#include "PBBox.h"

class PONet: public PNet {

  private:
    unsigned		_bBoxMark;
    unsigned            _currCostMark;
    unsigned		_saveMark;
    double		Cost[2];
    PBBox		BBox[2];
    unsigned		_curr;

  public:
    
    PONet(const losig* sig);

    double&		CurrentCost() 		{ return Cost[_curr]; }
    double		CurrentCost(const unsigned Flag);
    double&		TempCost() 		{ return Cost[!_curr]; }
    PBBox&		CurrentBBox()		{ return BBox[_curr]; }
    PBBox&		TempBBox()              { return BBox[!_curr]; }

    void		SaveTemp()		{ _curr = !_curr; }
    void		SaveTemp(const unsigned saveflag);

    void                DetPlaceComputeBBox(const unsigned Flag);
    double		DetPlaceUpdateCost();

  private:
    double 		DetPlaceComputeCost(PBBox &Box) const;
};
#endif /* __PONET_H */
