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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Btr                           |
|                                                             |
| File    :                   btrfree.c                       |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Btrhor  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "btr.h"

# include <stdio.h>
# include "btrfree.h"
# include "btrerror.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   Btr Free Function Transition              |
|                                                             |
\------------------------------------------------------------*/

void freebtrtransfunc( TransFunc )

  btrtransfunc *TransFunc;
{
  autfreeblock( (char *)TransFunc );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Variable Function            |
|                                                             |
\------------------------------------------------------------*/

void freebtrvarfunc( VarFunc )

  btrvarfunc *VarFunc;
{
  autfreeblock( (char *)VarFunc );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Variable Order               |
|                                                             |
\------------------------------------------------------------*/

void freebtrvarorder( VarOrder )

  btrvarorder *VarOrder;
{
  autfreeblock( (char *)VarOrder );
}

/*------------------------------------------------------------\
|                                                             |
|                   Btr Free Relation Transition              |
|                                                             |
\------------------------------------------------------------*/

void freebtrtransrel( TransRel )

  btrtransrel *TransRel;
{
  autfreeblock( (char *)TransRel );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Variable Relation            |
|                                                             |
\------------------------------------------------------------*/

void freebtrvarrel( VarRel )

  btrvarrel *VarRel;
{
  autfreeblock( (char *)VarRel );
}

