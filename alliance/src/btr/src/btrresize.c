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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Btr                           |
|                                                             |
| File    :                  btrresize.c                      |
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
# include "btrresize.h"
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
|                    Btr Resize Variable Function             |
|                                                             |
\------------------------------------------------------------*/

btrvarfunc *resizebtrvarfunc( VarFunc, OldNum, NewNum )

  btrvarfunc *VarFunc;
  long        OldNum;
  long        NewNum;
{
  return( (btrvarfunc *)autresizeblock( (char *)VarFunc, 
                                        sizeof( btrvarfunc ) * OldNum,
                                        sizeof( btrvarfunc ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr Resize Variable Order                |
|                                                             |
\------------------------------------------------------------*/

btrvarorder *resizebtrvarorder( VarOrder, OldNum, NewNum )

  btrvarorder  *VarOrder;
  long          OldNum;
  long          NewNum;
{
  return( (btrvarorder *)autresizeblock( (char *)VarOrder,
                                         sizeof( btrvarorder ) * OldNum,
                                         sizeof( btrvarorder ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr Resize Variable Relation             |
|                                                             |
\------------------------------------------------------------*/

btrvarrel *resizebtrvarrel( VarRel, OldNum, NewNum )

  btrvarrel *VarRel;
  long       OldNum;
  long       NewNum;
{
  return( (btrvarrel *)autresizeblock( (char *)VarRel, 
                                        sizeof( btrvarrel ) * OldNum,
                                        sizeof( btrvarrel ) * NewNum ) );
}
