
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
