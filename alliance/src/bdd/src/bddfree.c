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
| File    :                   bddfree.c                       |
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
# include "bddfree.h"
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
|                         Bdd Free User Func                  |
|                                                             |
\------------------------------------------------------------*/

void freebdduserfunc( BddUserFunc )

  bdduserfunc *BddUserFunc;
{
  autfreeheap( (char *)BddUserFunc, sizeof( bdduserfunc ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Bdd Free Heath                      |
|                                                             |
\------------------------------------------------------------*/

void freebddheath( BddHeath )

  bddheath *BddHeath;
{
  autfreeheap( (char *)BddHeath, sizeof( bddheath ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Free Hash Node                   |
|                                                             |
\------------------------------------------------------------*/

void freebddhnode( HashNode )

  bddhnode *HashNode;
{
  autfreeblock( (char *)HashNode );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Free Hash Oper                   |
|                                                             |
\------------------------------------------------------------*/

void freebddhoper( HashOper )

  bddhoper *HashOper;
{
  autfreeblock( (char *)HashOper );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Free Hash Node Table                |
|                                                             |
\------------------------------------------------------------*/

void freebddhnodetable( HashTable )

  bddhnodetable *HashTable;
{
  autfreeheap( (char *)HashTable, sizeof( bddhnodetable ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Free Hash Oper Table                |
|                                                             |
\------------------------------------------------------------*/

void freebddhopertable( HashTable )

  bddhopertable *HashTable;
{
  autfreeblock( (char *)HashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free BLock                        |
|                                                             |
\------------------------------------------------------------*/

void freebddblock( BddBlock )

  bddblock *BddBlock;
{
  autfreeheap( (char *)BddBlock, sizeof( bddblock ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Node Block                   |
|                                                             |
\------------------------------------------------------------*/

void freebddnodeblock( BddNode )

  bddnode *BddNode;
{
  autfreeblock( (char *)BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Free Variable Node                  |
|                                                             |
\------------------------------------------------------------*/

void freebddvarnode( BddVariableNode )

  bddvarnode *BddVariableNode;
{
  autfreeblock( (char *)BddVariableNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Free Hash Variable Node             |
|                                                             |
\------------------------------------------------------------*/

void freebddindexnode( BddIndexNode )

  bddindexnode *BddIndexNode;
{
  autfreeblock( (char *)BddIndexNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Variable                     |
|                                                             |
\------------------------------------------------------------*/

void freebddvar( BddVariable )

  bddvar *BddVariable;
{
  autfreeblock( (char *)BddVariable );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Index                        |
|                                                             |
\------------------------------------------------------------*/

void freebddindex( BddIndex )

  bddindex *BddIndex;
{
  autfreeblock( (char *)BddIndex );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Assoc                        |
|                                                             |
\------------------------------------------------------------*/

void freebddassoc( BddAssoc )

  bddassoc *BddAssoc;
{
  autfreeheap( (char *)BddAssoc, sizeof( bddassoc ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Assoc Node                   |
|                                                             |
\------------------------------------------------------------*/

void freebddassocnode( BddAssocNode )

  bddassocnode *BddAssocNode;
{
  autfreeblock( (char *)BddAssocNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Free Name Input                   |
|                                                             |
\------------------------------------------------------------*/

void freebddnamein( NameIn )

  bddnamein *NameIn;
{
  autfreeblock( (char *)NameIn );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Free Index Input                   |
|                                                             |
\------------------------------------------------------------*/

void freebddindexin( IndexIn )

  bddindex *IndexIn;
{
  autfreeblock( (char *)IndexIn );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Free System                      |
|                                                             |
\------------------------------------------------------------*/

void freebddsystem( BddSystem )

  bddsystem *BddSystem;
{
  autfreeblock( (char *)BddSystem );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Free Circuit                     |
|                                                             |
\------------------------------------------------------------*/

void freebddcircuit( BddCircuit )

  bddcircuit *BddCircuit;
{
  autfreeblock( (char *)BddCircuit );
}
