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
| File    :                   bddhoper.c                      |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
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
# include <string.h>
# include <memory.h>
# include "bddhoper.h"
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

  static long (*funcbddhopersize )() = getbddhopersize;
  static long (*funcbddhoperkey  )() = getbddhoperkey;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Set Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Bdd Set Hash Size Function               |
|                                                             |
\------------------------------------------------------------*/

void setbddhoperfunc( FuncSize, FuncKey )

  long (*FuncSize)();
  long (*FuncKey)();
{
  if ( FuncSize != 0 ) funcbddhopersize = FuncSize;
  else                 funcbddhopersize = getbddhopersize;

  if ( FuncKey != 0 ) funcbddhoperkey  = FuncKey;
  else                funcbddhoperkey  = getbddhoperkey;
}

/*------------------------------------------------------------\
|                                                             |
|                         Get Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Bdd Get Hash Size                     |
|                                                             |
\------------------------------------------------------------*/

long getbddhopersize( Size )

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
|                       Bdd Get Hash Key                      |
|                                                             |
\------------------------------------------------------------*/

long getbddhoperkey( Table, Oper, BddLeft, BddRight )

  bddhopertable *Table;
  long           Oper;
  bddnode       *BddLeft;
  bddnode       *BddRight;
{
  return( ( ((unsigned long)Oper          ) +
            ((unsigned long)BddLeft  >> 1 ) +
            ((unsigned long)BddRight << 5 ) ) % Table->TABLE_SIZE );
}
 
/*------------------------------------------------------------\
|                                                             |
|                      Create Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Bdd Create Hash Oper Table                 |
|                                                             |
\------------------------------------------------------------*/

bddhopertable *createbddhopertable( Length )

  long Length;
{
  bddhopertable *HashTable;
  bddhoper      *Table;

  if ( Length <= 0 )
  {
    bdderror( BDD_HOPER_SIZE_ERROR, Length );
  }

  Length = (*funcbddhopersize)( Length );

  HashTable = allocbddhopertable();
  Table     = allocbddhoper( Length );

  HashTable->TABLE      = Table;
  HashTable->TABLE_SIZE = Length;

  return( HashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                      Destroy Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Bdd Destroy Hash Oper Table                |
|                                                             |
\------------------------------------------------------------*/

void destroybddhopertable( HashTable )

  bddhopertable *HashTable;
{
  freebddhoper( HashTable->TABLE );
  freebddhopertable( HashTable   );
}

/*------------------------------------------------------------\
|                                                             |
|                      Reset Functions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Bdd Reset Hash Oper Table                  |
|                                                             |
\------------------------------------------------------------*/

void resetbddhopertable( HashTable )

  bddhopertable *HashTable;
{
  bddassoc *ScanAssoc;

  if ( HashTable->NUMBER_OPER > 0 )
  {
    memset( (void *)HashTable->TABLE, 0,
            (size_t)HashTable->TABLE_SIZE * sizeof( bddhoper ) );

    HashTable->NUMBER_OPER  = 0;
    HashTable->NUMBER_RESET++;

    if ( BddLocalSystem->ASSOC_RESET )
    {
      BddLocalSystem->ASSOC_RESET = 0;

      for ( ScanAssoc  = BddLocalSystem->ASSOC;
            ScanAssoc != (bddassoc *)0;
            ScanAssoc  = ScanAssoc->NEXT )
      {
        ScanAssoc->CACHE = 0;
      }
    }

    execbdduserfunc( BddLocalSystem, BDD_FUNC_RESET_HASH_OPER );
  }

  HashTable->NUMBER_ADD  = 0;
  HashTable->NUMBER_HIT  = 0;
  HashTable->NUMBER_MISS = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                          Add Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd Add Hash Element                   |
|                                                             |
\------------------------------------------------------------*/

bddhoper *addbddhoper( HashTable, Oper, BddLeft, BddRight, BddFather )

  bddhopertable *HashTable;
  long           Oper;
  bddnode       *BddLeft;
  bddnode       *BddRight;
  bddnode       *BddFather;
{
  bddhoper      *HashOper;
  bddnode       *BddSwap;
  long           HashIndex;

  if ( HashTable->TABLE_FREEZE ) return( (bddhoper *)0 );

  if ( ( BddLeft  == (bddnode *)0 ) ||
       ( BddRight == (bddnode *)0 ) )
  {
    bdderror( BDD_HOPER_KEY_ERROR, 0 );
  }

  if ( ( BddLeft < BddRight         ) &&
       ( ! ( Oper & BDD_OPER_MASK ) ) )
  {
    BddSwap  = BddRight;
    BddRight = BddLeft;
    BddLeft  = BddSwap;
  }

  HashIndex = (*funcbddhoperkey)( HashTable, Oper, BddLeft, BddRight );
  HashOper  = &HashTable->TABLE[ HashIndex ];

  HashTable->NUMBER_ADD++;

  if ( HashOper->LEFT == (bddnode *)0 )
  {
    HashTable->NUMBER_OPER++;
  }

  HashOper->LEFT     = BddLeft;
  HashOper->RIGHT    = BddRight;
  HashOper->OPERATOR = Oper;
  HashOper->FATHER = BddFather;

  return( HashOper );
}

/*------------------------------------------------------------\
|                                                             |
|                          Search Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd Search Hash Element                |
|                                                             |
\------------------------------------------------------------*/

bddhoper *searchbddhoper( HashTable, Oper, BddLeft, BddRight )

  bddhopertable *HashTable;
  long           Oper;
  bddnode       *BddLeft;
  bddnode       *BddRight;
{
  bddhoper      *HashOper;
  bddnode       *BddSwap;
  long           HashIndex;

  if ( HashTable->TABLE_FREEZE ) return( (bddhoper *)0 );

  if ( ( BddLeft  == (bddnode *)0 ) ||
       ( BddRight == (bddnode *)0 ) )
  {
    bdderror( BDD_HOPER_KEY_ERROR, 0 );
  }

  if ( ( BddLeft < BddRight         ) &&
       ( ! ( Oper & BDD_OPER_MASK ) ) )
  {
    BddSwap  = BddRight;
    BddRight = BddLeft;
    BddLeft  = BddSwap;
  }

  HashIndex = (*funcbddhoperkey)( HashTable, Oper, BddLeft, BddRight );
  HashOper  = &HashTable->TABLE[ HashIndex ];

  if ( ( HashOper->OPERATOR == Oper      ) &&
       ( HashOper->LEFT     == BddLeft   ) &&
       ( HashOper->RIGHT    == BddRight  ) )
  {
    HashTable->NUMBER_HIT++;

    return( HashOper );
  }

  HashTable->NUMBER_MISS++;

  return( (bddhoper *)0 );
}


/*------------------------------------------------------------\
|                                                             |
|                         View Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd View Hash Oper                     |
|                                                             |
\------------------------------------------------------------*/

void viewbddhoper( HashOper )

  bddhoper *HashOper;
{
  fprintf( stdout, "OPERATOR: %lx  LEFT : %lx  RIGHT : %lx  FATHER : %lx\n",
           HashOper->OPERATOR, 
           (long)HashOper->LEFT,
           (long)HashOper->RIGHT,
           (long)HashOper->FATHER );
}

/*------------------------------------------------------------\
|                                                             |
|                   Bdd View Hash Oper Table                  |
|                                                             |
\------------------------------------------------------------*/

void viewbddhopertable( HashTable, FuncView )

  bddhopertable *HashTable;
  void          (*FuncView)();
{
  bddhoper *HashOper;
  long      HashIndex;

  fprintf( stdout, "--> HashOperTable\n" );
  fprintf( stdout, "  SIZE   : %ld\n", HashTable->TABLE_SIZE   );
  fprintf( stdout, "  FREEZE : %ld\n", HashTable->TABLE_FREEZE );
  fprintf( stdout, "  OPER   : %ld\n", HashTable->NUMBER_OPER  );
  fprintf( stdout, "  ADD    : %ld\n", HashTable->NUMBER_ADD   );
  fprintf( stdout, "  HIT    : %ld\n", HashTable->NUMBER_HIT   );
  fprintf( stdout, "  MISS   : %ld\n", HashTable->NUMBER_MISS  );
  fprintf( stdout, "  RESET  : %ld\n", HashTable->NUMBER_RESET );

  if ( FuncView )
  {
    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      HashOper = &HashTable->TABLE[ HashIndex ];

      if ( HashOper->LEFT != (bddnode *)0 )
      {
        fprintf( stdout, "  INDEX: %10ld  ", HashIndex );

        (*FuncView)( HashOper );
      }
    }
  }

  fprintf( stdout, "<-- HashOperTable\n" );
}
