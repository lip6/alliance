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
