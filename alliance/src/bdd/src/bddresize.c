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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                  bddresize.c                      |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Bddhor  :               Jacomme Ludovic                     |
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

# include <stdio.h>
# include "bddresize.h"
# include "bdderror.h"

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
|                     Bdd Resize Variable Node                |
|                                                             |
\------------------------------------------------------------*/

bddvarnode *resizebddvarnode( BddVarNode, OldNum, NewNum )

  bddvarnode *BddVarNode;
  long        OldNum;
  long        NewNum;
{
  BddAllocInfo.VAR_NODE += NewNum - OldNum;

  return( (bddvarnode *)autresizeblock( (char *)BddVarNode,
                                        sizeof( bddvarnode ) * OldNum,
                                        sizeof( bddvarnode ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Resize Variable                   |
|                                                             |
\------------------------------------------------------------*/

bddvar *resizebddvar( BddVar, OldNum, NewNum )

  bddvar *BddVar;
  long    OldNum;
  long    NewNum;
{
  BddAllocInfo.VAR += NewNum - OldNum;

  return( (bddvar *)autresizeblock( (char *)BddVar,
                                    sizeof( bddvar ) * OldNum,
                                    sizeof( bddvar ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Resize Index Node                    |
|                                                             |
\------------------------------------------------------------*/

bddindexnode *resizebddindexnode( BddIndexNode, OldNum, NewNum )

  bddindexnode *BddIndexNode;
  long          OldNum;
  long          NewNum;
{
  BddAllocInfo.INDEX_NODE += NewNum - OldNum;

  return( (bddindexnode *)autresizeblock( (char *)BddIndexNode,
                                          sizeof( bddindexnode ) * OldNum,
                                          sizeof( bddindexnode ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Resize Index                      |
|                                                             |
\------------------------------------------------------------*/

bddindex *resizebddindex( BddIndex, OldNum, NewNum )

  bddindex *BddIndex;
  long      OldNum;
  long      NewNum;
{
  BddAllocInfo.INDEX += NewNum - OldNum;

  return( (bddindex *)autresizeblock( (char *)BddIndex,
                                      sizeof( bddindex ) * OldNum,
                                      sizeof( bddindex ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Resize Assoc Node                   |
|                                                             |
\------------------------------------------------------------*/

bddassocnode *resizebddassocnode( BddAssocNode, OldNum, NewNum )

  bddassocnode *BddAssocNode;
  long          OldNum;
  long          NewNum;
{
  BddAllocInfo.ASSOC_NODE += NewNum - OldNum;

  return( (bddassocnode *)autresizeblock( (char *)BddAssocNode,
                                          sizeof( bddassocnode ) * OldNum,
                                          sizeof( bddassocnode ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Resize Name Input                   |
|                                                             |
\------------------------------------------------------------*/

bddnamein *resizebddnamein( NameIn, OldNum, NewNum )

  bddnamein *NameIn;
  long       OldNum;
  long       NewNum;
{
  BddAllocInfo.NAME_IN += NewNum - OldNum;

  return( (bddnamein *)autresizeblock( (char *)NameIn,
                                       sizeof( bddnamein ) * OldNum,
                                       sizeof( bddnamein ) * NewNum ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Resize Index Input                  |
|                                                             |
\------------------------------------------------------------*/

bddindex *resizebddindexin( IndexIn, OldNum, NewNum )

  bddindex *IndexIn;
  long      OldNum;
  long      NewNum;
{
  BddAllocInfo.INDEX_IN += NewNum - OldNum;

  return( (bddindex *)autresizeblock( (char *)IndexIn,
                                      sizeof( bddindex ) * OldNum,
                                      sizeof( bddindex ) * NewNum ) );
}
