
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
#ifndef __PTOPLACEINS_H
#define __PTOPLACEINS_H

#include "PIns.h"
#include "PBin.h"


class PToPlaceIns : public PIns {
    
  private:  
    PBin*		_bin;
  
  public:
    PToPlaceIns(const loins* ins); 

    void		SetBin(PBin* NewBin)			{ _bin = NewBin; }
    PBin&               GetBin() const				{ return *_bin; }

    PPos		GetPos() const				{ return _bin->GetPos(); }
    double		GetHeight() const			{ return _bin->GetHeight(); }
    bool		GetOrientation() const			{ return _bin->GetOrientation(); }
    
    ostream&	Print(ostream& os) const;
};
#endif /* __PTOPLACEINS_H */
