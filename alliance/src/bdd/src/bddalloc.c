/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                   bddalloc.c                      |
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
# include "bddalloc.h"
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

   bddallocinfo BddAllocInfo = 

   {
     0, /*  CIRCUIT          */
     0, /*  SYSTEM           */
     0, /*  BLOCK            */
     0, /*  NODE_BLOCK       */
     0, /*  HASH_NODE_TABLE  */
     0, /*  HASH_NODE        */
     0, /*  HASH_OPER_TABLE  */
     0, /*  HASH_OPER        */
     0, /*  VAR              */
     0, /*  VAR_NODE         */
     0, /*  INDEX_NODE       */
     0, /*  INDEX            */
     0, /*  ASSOC            */
     0, /*  ASSOC_NODE       */
     0, /*  NAME_IN          */
     0, /*  INDEX_IN         */
     0, /*  HEATH            */
     0  /*  USER_FUNC        */
   };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc User Func                   |
|                                                             |
\------------------------------------------------------------*/

bdduserfunc *allocbdduserfunc()
{
  BddAllocInfo.USER_FUNC++;

  return( (bdduserfunc *)autallocheap( sizeof( bdduserfunc ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Heath                       |
|                                                             |
\------------------------------------------------------------*/

bddheath *allocbddheath()
{
  BddAllocInfo.HEATH++;

  return( (bddheath *)autallocheap( sizeof( bddheath ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Alloc Hash Node                  |
|                                                             |
\------------------------------------------------------------*/

bddhnode *allocbddhnode( Number )

  long Number;
{
  BddAllocInfo.HASH_NODE += Number;

  return( (bddhnode *)autallocblock( sizeof( bddhnode ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Alloc Hash Oper                  |
|                                                             |
\------------------------------------------------------------*/

bddhoper *allocbddhoper( Number )

  long Number;
{
  BddAllocInfo.HASH_OPER += Number;

  return( (bddhoper *)autallocblock( sizeof( bddhoper ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   Bdd Alloc Hash Node Table                 |
|                                                             |
\------------------------------------------------------------*/

bddhnodetable *allocbddhnodetable()
{
  BddAllocInfo.HASH_NODE_TABLE++;

  return( (bddhnodetable *)autallocheap( sizeof( bddhnodetable ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   Bdd Alloc Hash Oper Table                 |
|                                                             |
\------------------------------------------------------------*/

bddhopertable *allocbddhopertable()
{
  BddAllocInfo.HASH_OPER_TABLE++;

  return( (bddhopertable *)autallocblock( sizeof( bddhopertable ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Block                       |
|                                                             |
\------------------------------------------------------------*/

bddblock *allocbddblock()
{
  BddAllocInfo.BLOCK++;

  return( (bddblock *)autallocheap( sizeof( bddblock ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Node Block                  |
|                                                             |
\------------------------------------------------------------*/

bddnode *allocbddnodeblock( Number )

  long Number;
{
  BddAllocInfo.NODE_BLOCK += Number;

  return( (bddnode *)autallocblock( sizeof( bddnode ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Variable Node               |
|                                                             |
\------------------------------------------------------------*/

bddvarnode *allocbddvarnode( Number )

  long Number;
{
  BddAllocInfo.VAR_NODE += Number;

  return( (bddvarnode *)autallocblock( sizeof( bddvarnode ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Index Node                  |
|                                                             |
\------------------------------------------------------------*/

bddindexnode *allocbddindexnode( Number )

  long Number;
{
  BddAllocInfo.INDEX_NODE += Number;

  return( (bddindexnode *)autallocblock( sizeof( bddindexnode ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Variable                    |
|                                                             |
\------------------------------------------------------------*/

bddvar *allocbddvar( Number )

  long Number;
{
  BddAllocInfo.VAR += Number;

  return( (bddvar *)autallocblock( sizeof( bddvar ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Index                       |
|                                                             |
\------------------------------------------------------------*/

bddindex *allocbddindex( Number )

  long Number;
{
  BddAllocInfo.INDEX += Number;

  return( (bddindex *)autallocblock( sizeof( bddindex ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Alloc Assoc                      |
|                                                             |
\------------------------------------------------------------*/

bddassoc *allocbddassoc()
{
  BddAllocInfo.ASSOC++;

  return( (bddassoc *)autallocheap( sizeof( bddassoc ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Alloc Assoc Node                 |
|                                                             |
\------------------------------------------------------------*/

bddassocnode *allocbddassocnode( Number )

  long Number;
{
  BddAllocInfo.ASSOC_NODE += Number;

  return( (bddassocnode *)autallocblock( sizeof( bddassocnode ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Name Input                  |
|                                                             |
\------------------------------------------------------------*/

bddnamein *allocbddnamein( Number )

  long Number;
{
  BddAllocInfo.NAME_IN += Number;

  return( (bddnamein *)autallocblock( sizeof( bddnamein ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Index Input                 |
|                                                             |
\------------------------------------------------------------*/

bddindex *allocbddindexin( Number )

  long Number;
{
  BddAllocInfo.INDEX_IN += Number;

  return( (bddindex *)autallocblock( sizeof( bddindex ) * Number ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc System                      |
|                                                             |
\------------------------------------------------------------*/

bddsystem *allocbddsystem()
{
  BddAllocInfo.SYSTEM++;

  return( (bddsystem *)autallocblock( sizeof( bddsystem ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Alloc Circuit                     |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *allocbddcircuit()
{
  BddAllocInfo.CIRCUIT++;

  return( (bddcircuit *)autallocblock( sizeof( bddcircuit ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       View Bdd Alloc Info                   |
|                                                             |
\------------------------------------------------------------*/

void viewbddallocinfo()
{
  fprintf( stdout, "--> BddAllocInfo\n" );
  fprintf( stdout, "  CIRCUIT         : %ld bytes\n",
           BddAllocInfo.CIRCUIT * sizeof( bddcircuit ) );
  fprintf( stdout, "  SYSTEM          : %ld bytes\n", 
           BddAllocInfo.SYSTEM * sizeof( bddsystem ) );
  fprintf( stdout, "  BLOCK           : %ld bytes\n", 
           BddAllocInfo.BLOCK * sizeof( bddblock ) );
  fprintf( stdout, "  NODE_BLOCK      : %ld bytes\n",
           BddAllocInfo.NODE_BLOCK * sizeof( bddnode ) );
  fprintf( stdout, "  HASH_NODE_TABLE : %ld bytes\n",
           BddAllocInfo.HASH_NODE_TABLE * sizeof( bddhnodetable ) );
  fprintf( stdout, "  HASH_NODE       : %ld bytes\n",
           BddAllocInfo.HASH_NODE * sizeof( bddhnode ) );
  fprintf( stdout, "  HASH_OPER_TABLE : %ld bytes\n",
           BddAllocInfo.HASH_OPER_TABLE * sizeof( bddhopertable ) );
  fprintf( stdout, "  HASH_OPER       : %ld bytes\n",
           BddAllocInfo.HASH_OPER * sizeof( bddhoper ) );
  fprintf( stdout, "  VAR             : %ld bytes\n",
           BddAllocInfo.VAR * sizeof( bddvar ) );
  fprintf( stdout, "  VAR_NODE        : %ld bytes\n",
           BddAllocInfo.VAR_NODE * sizeof( bddvarnode ) );
  fprintf( stdout, "  INDEX_NODE      : %ld bytes\n",
           BddAllocInfo.INDEX_NODE * sizeof( bddindexnode ) );
  fprintf( stdout, "  INDEX           : %ld bytes\n",
           BddAllocInfo.INDEX * sizeof( bddindex ) );
  fprintf( stdout, "  ASSOC           : %ld bytes\n",
           BddAllocInfo.ASSOC * sizeof( bddassoc ) );
  fprintf( stdout, "  ASSOC_NODE      : %ld bytes\n",
           BddAllocInfo.ASSOC_NODE * sizeof( bddassocnode ) );
  fprintf( stdout, "  NAME_IN         : %ld bytes\n",
           BddAllocInfo.NAME_IN * sizeof( bddnamein ) );
  fprintf( stdout, "  INDEX_IN        : %ld bytes\n",
           BddAllocInfo.INDEX_IN * sizeof( bddindex ) );
  fprintf( stdout, "  HEATH           : %ld bytes\n",
           BddAllocInfo.HEATH * sizeof( bddheath ) );
  fprintf( stdout, "  USER_FUNC       : %ld bytes\n",
           BddAllocInfo.USER_FUNC * sizeof( bdduserfunc ) );
  fprintf( stdout, "<-- BddAllocInfo\n" );

}
