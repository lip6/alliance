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
