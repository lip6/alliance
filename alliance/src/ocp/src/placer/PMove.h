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
#ifndef __PMove_h
#define __PMove_h

#include <PPlacement.h>
#include <PPos.h>
#include <map>

class PMove {
  private:
      
    PPlacement&			_placement;

    PToPlaceIns*		_srcIns;
    PBin*			_srcBin;
    double			_srcBinInitCost;
    PSubRow*			_srcSubRow;
    PRow*			_srcRow;
    double			_srcRowInitCost;
    double			_srcWidth;

    PBin*			_dstBin;
    double			_dstBinInitCost;
    PSubRow*			_dstSubRow;
    PRow*			_dstRow;
    double			_dstRowInitCost;
    PToPlaceIns*		_dstIns;
    double			_dstWidth;

    map<PONet*, unsigned>	_affectedNets;

  public:
			PMove(PPlacement& placement);

    double		GetDeltaRowCost();
    double		GetDeltaBinCost();
    double		GetDeltaNetCost();

    bool		Next(const double dist);
    void		Accept();
    void		Reject();
    void		Move();
};

#endif /* __PMove_h */
