
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
#ifndef __PCON_H
#define __PCON_H

#include "PElem.h"
#include "mut.h"
#include "mlo.h"

class PCon: public PElem {
  private:
      const locon*	_con;
      PPos		_pos;
      char		_orient;

  public:
      PCon(const locon* con);
      PCon(const locon* con, PPos pos, char orient);
      PPos		GetPos() const			{ return _pos; }
      const locon*	GetLocon() const		{ return _con; }
      void		SetPos(const PPos pos)		{_pos = pos;}
      void		SetOrient(const char orient)	{_orient = orient;}
      void		Save(struct phfig *physicalfig, const double dx, const double dy) const;
      void		RingSave(struct phfig *physicalfig, const double dx, const double dy) const;

      ostream&	Print(ostream& os) const;
};

#endif /* __PCON_H */
