
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
#ifndef __PCOMMON_H
#define __PCOMMON_H

#include <iostream>
#include "mut.h"
#include "mlo.h"
#include "mph.h"

using namespace std;

const double SquareShape(const double margin, const double sumwidth,
	const double minwidth, int& nrows);

bool IsSpecialNet(const losig* const sig);
bool IsTie(const phins* ins);
ostream& PrintLocon(ostream& os, const locon* const con);
ostream& PrintLosig(ostream& os, const losig* const sig);
unsigned NbCons(const losig* const sig);

#define Max(i,j) (((i) > (j)) ? (i) : (j))
#define Min(i,j) (((i) < (j)) ? (i) : (j))

#endif /* __PCOMMON_H */
