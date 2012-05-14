/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                   boom_hash.c                     |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
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
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "boom_error.h"
# include "boom_hash.h"

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
|                         Get Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Boom Get Hash Size                    |
|                                                             |
\------------------------------------------------------------*/

static long BoomGetHashAblSize( Size )

  long Size;
{
  int Index;

  for ( Index = 0; Index < AUT_MAX_PRIME_NUMBER; Index++ )
  {
    if ( AUT_HASH_PRIME_NUMBER[ Index ] > Size ) 
    {
      return( AUT_HASH_PRIME_NUMBER[ Index ] );
    }
  }

  if ( ( Size & 1 ) == 0 ) Size++;

  return( Size );
}

/*------------------------------------------------------------\
|                                                             |
|                       Boom Get Hash Key                     |
|                                                             |
\------------------------------------------------------------*/

static long BoomGetHashAblKey( Table, BddNode )

  boomhabltable *Table;
  bddnode      *BddNode;
{
  return( ((unsigned long)BddNode >> 2 ) % Table->TABLE_SIZE );
}
 
/*------------------------------------------------------------\
|                                                             |
|                      Create Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Boom Create Hash Abl Table                 |
|                                                             |
\------------------------------------------------------------*/

boomhabltable *BoomCreateHashAblTable( Length )

  long Length;
{
  boomhabltable *HashTable;
  boomhabl      *Table;

  Length = BoomGetHashAblSize( Length );

  HashTable = (boomhabltable *)autallocblock( sizeof( boomhabltable )     );
  Table     = (boomhabl      *)autallocblock( sizeof( boomhabl ) * Length );

  HashTable->TABLE      = Table;
  HashTable->TABLE_SIZE = Length;

  return( HashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                      Reset Functions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Boom Reset Hash Abl Table                  |
|                                                             |
\------------------------------------------------------------*/

void BoomResetHashAblTable( HashTable )

  boomhabltable *HashTable;
{
  boomhabl *HashAbl;
  long     HashIndex;

  if ( HashTable->NUMBER_ABL > 0 )
  {
    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      HashAbl = &HashTable->TABLE[ HashIndex ];

      if ( HashAbl->NODE != (bddnode *)0 )
      {
        freeablexpr( HashAbl->ABL );

        HashAbl->ABL  = (chain_list *)0;
        HashAbl->NODE = (bddnode    *)0;
      }
    }

    HashTable->NUMBER_ABL  = 0;
    HashTable->NUMBER_RESET++;
  }

  HashTable->NUMBER_ADD  = 0;
  HashTable->NUMBER_HIT  = 0;
  HashTable->NUMBER_MISS = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                      Destroy Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Boom Destroy Hash Abl Table                |
|                                                             |
\------------------------------------------------------------*/

void BoomDestroyHashAblTable( HashTable )

  boomhabltable *HashTable;
{
  BoomResetHashAblTable( HashTable );

  autfreeblock( HashTable->TABLE );
  autfreeblock( HashTable        );
}

/*------------------------------------------------------------\
|                                                             |
|                          Add Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Boom Add Hash Element                  |
|                                                             |
\------------------------------------------------------------*/

boomhabl *BoomAddHashAbl( HashTable, BddNode, Equation )

  boomhabltable *HashTable;
  bddnode      *BddNode;
  chain_list   *Equation;
{
  boomhabl *HashAbl;
  long     HashIndex;

  HashIndex = BoomGetHashAblKey( HashTable, BddNode, Equation );
  HashAbl   = &HashTable->TABLE[ HashIndex ];

  HashTable->NUMBER_ADD++;

  if ( HashAbl->NODE == (bddnode *)0 )
  {
    HashTable->NUMBER_ABL++;
  }
  else
  {
    freeablexpr( HashAbl->ABL );
  }
  
  HashAbl->NODE = BddNode;
  HashAbl->ABL  = dupablexpr( Equation );

  return( HashAbl );
}

/*------------------------------------------------------------\
|                                                             |
|                          Search Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Boom Search Hash Abl                 |
|                                                             |
\------------------------------------------------------------*/

boomhabl *BoomSearchHashAbl( HashTable, BddNode )

  boomhabltable *HashTable;
  bddnode      *BddNode;
{
  boomhabl *HashAbl;
  long     HashIndex;

  HashIndex = BoomGetHashAblKey( HashTable, BddNode );
  HashAbl  = &HashTable->TABLE[ HashIndex ];

  if ( HashAbl->NODE == BddNode )
  {
    HashTable->NUMBER_HIT++;

    return( HashAbl );
  }

  HashTable->NUMBER_MISS++;

  return( (boomhabl *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                   Boom View Hash Abl Table                  |
|                                                             |
\------------------------------------------------------------*/

void BoomViewHashAblTable( HashTable )

  boomhabltable *HashTable;
{
  fprintf( stdout, "--> HashAblTable\n" );
  fprintf( stdout, "  SIZE   : %ld\n", HashTable->TABLE_SIZE   );
  fprintf( stdout, "  ABL    : %ld\n", HashTable->NUMBER_ABL   );
  fprintf( stdout, "  ADD    : %ld\n", HashTable->NUMBER_ADD   );
  fprintf( stdout, "  HIT    : %ld\n", HashTable->NUMBER_HIT   );
  fprintf( stdout, "  MISS   : %ld\n", HashTable->NUMBER_MISS  );
  fprintf( stdout, "  RESET  : %ld\n", HashTable->NUMBER_RESET );

  fprintf( stdout, "<-- HashAblTable\n" );
}
