
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
#ifndef __PFIXEDINS_H
#define __PFIXEDINS_H

#include "PIns.h"
#include "PPos.h"
#include "mut.h"
#include "mph.h"

class PFixedIns : public PIns {
    
  private:
    const phins*	_pins;
    PPos		_pos;
    double		_height;
  
  public:
    PFixedIns(const loins* ins, const phins* pins, int dx, int dy);
    ~PFixedIns() {}

    double		GetHeight() const			{ return _height; }
    PPos		GetPos() const				{ return _pos; }
    
    ostream&	Print(ostream& os) const;
};

#endif /* __PFIXEDINS_H */
