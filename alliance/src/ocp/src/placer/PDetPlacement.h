
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
#ifndef __PDETPLACEMENT_H
#define __PDETPLACEMENT_H

#include <vector>
#include <deque>
#include "PDetToPlaceIns.h"
using namespace std;

class PDetPlacement {
  public:
      typedef vector<PDetToPlaceIns*> Problem;
  private:
    unsigned		BBoxFlag;
    // le _costFlag sert lors de la consultation
    // du cout des nets.
    // Chaque net doit etre compte une seule fois
    unsigned		_costFlag;
    unsigned		_saveFlag;

    Problem		_problem;
    
      
    double		LeftEdge;
    double		RightEdge;
    double		OldCost;
    double		Cost;
    double		BestCost;
    deque<unsigned>	Queue; // ordering 
    vector<unsigned>	Stack;
    vector<unsigned>    UVect; // use for loop
    vector<double>	BestSolution;
    int			Idx;

  public:
    PDetPlacement(Problem& problem);
    bool		Optimize();
  private:
    void		AddIns();
    void		RemoveIns();
    double		InitCost(const unsigned BBoxFlag);
    double		CurrentCost();
    void		UnPlaceAll();
    void		PlaceAll();
};

#endif /* __PDETPLACEMENT_H */
