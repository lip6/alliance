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
| File    :                   bddhnode.c                      |
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

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H

# include <stdio.h>
# include <string.h>
# include <memory.h>
# include "bddhnode.h"
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

  static long (*funcbddhnodesize )() = getbddhnodesize;
  static long (*funcbddhnodekey  )() = getbddhnodekey;
  static long (*funcbddhnodeindex)() = getbddhnodeindex;

  long BDD_HNODE_STRETCH_FACTOR  = HNODE_STRETCH_FACTOR;
  long BDD_HNODE_RESIZE_FACTOR   = HNODE_RESIZE_FACTOR;
  long BDD_HNODE_MIN_FILL_FACTOR = HNODE_MIN_FILL_FACTOR;
  long BDD_HNODE_MAX_SCAN        = HNODE_MAX_SCAN;

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

void setbddhnodefunc( FuncSize, FuncKey, FuncIndex )

  long (*FuncSize)();
  long (*FuncKey)();
  long (*FuncIndex)();
{
  if ( FuncSize != 0 ) funcbddhnodesize = FuncSize;
  else                 funcbddhnodesize = getbddhnodesize;

  if ( FuncKey != 0 ) funcbddhnodekey  = FuncKey;
  else                funcbddhnodekey  = getbddhnodekey;

  if ( FuncIndex != 0 ) funcbddhnodeindex = FuncIndex;
  else                  funcbddhnodeindex = getbddhnodeindex;
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

long getbddhnodesize( Size )

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

long getbddhnodekey( Table, BddNode )

  bddhnodetable *Table;
  bddnode       *BddNode;
{
  return( ( ((unsigned long)BddNode->HIGH >> 2 ) +
            ((unsigned long)BddNode->LOW  << 1 ) ) % Table->TABLE_SIZE );
}
 
/*------------------------------------------------------------\
|                                                             |
|                       Bdd Get Hash Index                    |
|                                                             |
\------------------------------------------------------------*/

long getbddhnodeindex( Table, BddNode, Index )

  bddhnodetable *Table;
  bddnode       *BddNode;
  long           Index;
{
  return( ( ((unsigned long)Index             ) +
            ((unsigned long)Index        << 2 ) + 
            ((unsigned long)BddNode->HIGH     ) + 
            ((unsigned long)BddNode->LOW << 1 ) ) % Table->TABLE_SIZE );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Check Hash Node                   |
|                                                             |
\------------------------------------------------------------*/

int checkbddhnode( HashNode, Severity )

  bddnode *HashNode;
  int      Severity;
{
  if ( ( HashNode == BDD_HASH_NODE_EMPTY   ) ||
       ( HashNode == BDD_HASH_NODE_DELETED ) )
  {
    if ( Severity )
    {
      bdderror( BDD_HNODE_KEY_ERROR, HashNode );
    }

    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Create Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd Create Hash Table                  |
|                                                             |
\------------------------------------------------------------*/

bddhnodetable *createbddhnodetable( Length )

  long Length;
{
  bddhnodetable *HashTable;
  bddhnode      *Table;

  if ( Length < 0 )
  {
    bdderror( BDD_HNODE_SIZE_ERROR, Length );
  }

  Length = (*funcbddhnodesize)( Length );

  HashTable = allocbddhnodetable();
  Table     = allocbddhnode( Length );

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
|                     Bdd Destroy Hash Table                  |
|                                                             |
\------------------------------------------------------------*/

void destroybddhnodetable( HashTable )

  bddhnodetable *HashTable;
{
  freebddhnode( HashTable->TABLE );
  freebddhnodetable( HashTable       );
}

/*------------------------------------------------------------\
|                                                             |
|                        Reset Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd Reset Hash Table                   |
|                                                             |
\------------------------------------------------------------*/

void resetbddhnodetable( HashTable )

  bddhnodetable *HashTable;
{
  if ( HashTable->NUMBER_NODE > 0 )
  {
    memset( (void *)HashTable->TABLE, 0,
            (size_t)HashTable->TABLE_SIZE * sizeof( bddhnode ) );

    HashTable->NUMBER_NODE = 0;
  }

  HashTable->NUMBER_ADD     = 0;
  HashTable->NUMBER_SCAN    = 0;
  HashTable->NUMBER_STRETCH = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                       Stretch Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd Resize Hash Table                  |
|                                                             |
\------------------------------------------------------------*/

void resizebddhnodetable( HashTable )

  bddhnodetable *HashTable;
{
  bddhnode *NewTable;
  bddhnode *Table;
  long      NewLength;
  long      Length;
  long      NumberNode;

  Length     = HashTable->TABLE_SIZE;
  NumberNode = HashTable->NUMBER_NODE;

  if ( NumberNode >= ( Length * BDD_HNODE_MIN_FILL_FACTOR ) / 100 )
  {
    return;
  }
  
  Table      = HashTable->TABLE;
  NewLength  = (*funcbddhnodesize)( NumberNode * BDD_HNODE_RESIZE_FACTOR / 100 );
  NewTable   = allocbddhnode( NewLength );

  HashTable->TABLE          = NewTable;
  HashTable->TABLE_SIZE     = NewLength;
  HashTable->NUMBER_NODE    = 0;
  HashTable->NUMBER_RESIZE += 1;

  while ( Length > 0 )
  {
    Length = Length - 1;

    if ( ( Table[ Length ] != BDD_HASH_NODE_EMPTY   ) &&
         ( Table[ Length ] != BDD_HASH_NODE_DELETED ) )
    {
      addbddhnode( HashTable, Table[ Length ] );
    }
  }

  freebddhnode( Table );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Stretch Hash Table                 |
|                                                             |
\------------------------------------------------------------*/

void stretchbddhnodetable( HashTable )

  bddhnodetable *HashTable;
{
  bddhnode *NewTable;
  bddhnode *Table;
  long      NewLength;
  long      Length;

  Length    = HashTable->TABLE_SIZE;
  Table     = HashTable->TABLE;
  NewLength = (*funcbddhnodesize)( Length * BDD_HNODE_STRETCH_FACTOR / 100 );
  NewTable  = allocbddhnode( NewLength );

  HashTable->TABLE           = NewTable;
  HashTable->TABLE_SIZE      = NewLength;
  HashTable->NUMBER_NODE     = 0;
  HashTable->NUMBER_STRETCH += 1;

  while ( Length > 0 )
  {
    Length = Length - 1;

    if ( ( Table[ Length ] != BDD_HASH_NODE_EMPTY   ) &&
         ( Table[ Length ] != BDD_HASH_NODE_DELETED ) )
    {
      addbddhnode( HashTable, Table[ Length ] );
    }
  }

  freebddhnode( Table );
}

/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd Add Hash Element                   |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddhnode( HashTable, BddNode )

  bddhnodetable *HashTable;
  bddnode       *BddNode;
{
  bddnode  *HashNode;
  long      HashIndex;
  long      Deleted;
  long      NumberScan;
  char      Stop;

  check_bddhnode( BddNode );

  HashIndex = (*funcbddhnodekey)( HashTable, BddNode );
  Deleted   = -1;
  
  NumberScan = 0;
  Stop       = 0;

  HashTable->NUMBER_ADD++;

  do
  {
    HashNode = HashTable->TABLE[ HashIndex ];

    if ( HashNode == BDD_HASH_NODE_DELETED )
    {
      if ( Deleted == -1 ) Deleted = HashIndex;
    }
    else
    if ( ( HashNode == BDD_HASH_NODE_EMPTY       ) ||
         ( ( HashNode->INDEX == BddNode->INDEX ) &&
           ( HashNode->HIGH  == BddNode->HIGH  ) &&
           ( HashNode->LOW   == BddNode->LOW   ) ) )
    {
      if ( HashNode == BDD_HASH_NODE_EMPTY )
      {
        HashTable->NUMBER_NODE++;

        if ( Deleted != -1 )
        {
          HashIndex = Deleted;
        }
       
        HashNode = BddNode;
      }
      else
      if ( Deleted != -1 )
      {
        HashTable->TABLE[ HashIndex ] = BDD_HASH_NODE_DELETED;

        HashIndex = Deleted;
      }

      Stop = 1;
    }

    NumberScan += 1;

    if ( Stop == 0 )
    {
      if ( NumberScan > BDD_HNODE_MAX_SCAN )
      {
        if ( Deleted != -1 )
        {
          HashTable->TABLE[ Deleted ] = BDD_HASH_NODE_EMPTY;
          HashIndex = Deleted;
          Deleted   = -1;
        }
        else
        {
          stretchbddhnodetable( HashTable );
      
          HashIndex  = (*funcbddhnodekey)( HashTable, BddNode );
          NumberScan = 0;
        }
      }
      else
      {
        HashIndex = (*funcbddhnodeindex)( HashTable, BddNode, HashIndex );
      }
    }
  }
  while ( Stop == 0 );

  HashTable->NUMBER_SCAN += NumberScan;

  HashTable->TABLE[ HashIndex ] = HashNode;

  return( HashNode );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Del Hash Element                   |
|                                                             |
\------------------------------------------------------------*/

int delbddhnode( HashTable, BddNode )

  bddhnodetable *HashTable;
  bddnode       *BddNode;
{
  bddnode  *HashNode;
  long      HashIndex;
  int       NumberScan;

  check_bddhnode( BddNode );

  HashIndex  = (*funcbddhnodekey)( HashTable, BddNode );
  NumberScan = 0;

  do
  {
    HashNode = HashTable->TABLE[ HashIndex ];

    if ( HashNode == BDD_HASH_NODE_EMPTY )
    {
      return( 0 );
    }

    if ( ( HashNode != BDD_HASH_NODE_DELETED ) &&
         ( HashNode->INDEX == BddNode->INDEX ) &&
         ( HashNode->HIGH  == BddNode->HIGH  ) &&
         ( HashNode->LOW   == BddNode->LOW   ) )
    {
      HashTable->NUMBER_NODE--;

      HashTable->TABLE[ HashIndex ] = BDD_HASH_NODE_DELETED;

      HashTable->NUMBER_SCAN += NumberScan + 1;

      return( 1 );
    }

    NumberScan += 1;
    HashIndex   = (*funcbddhnodeindex)( HashTable, BddNode, HashIndex );

  }
  while ( NumberScan <= BDD_HNODE_MAX_SCAN );

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd View Hash Node                     |
|                                                             |
\------------------------------------------------------------*/

void viewbddhnode( HashNode )

  bddnode *HashNode;
{
  fprintf( stdout, "Node INDEX: %ld  MARK: %ld  REF_EXT: %ld  REF_INT: %ld\n",
           (long)( HashNode->INDEX   ),
           (long)( HashNode->MARK    ),
           (long)( HashNode->REF_EXT ),
           (long)( HashNode->REF_INT ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd View Hash Table                    |
|                                                             |
\------------------------------------------------------------*/

void viewbddhnodetable( HashTable, FuncView )

  bddhnodetable *HashTable;
  void          (*FuncView)();
{
  bddnode *HashNode;
  long     HashIndex;

  fprintf( stdout, "--> HashNodeTable\n" );
  fprintf( stdout, "  SIZE    : %ld\n", HashTable->TABLE_SIZE     );
  fprintf( stdout, "  NODE    : %ld\n", HashTable->NUMBER_NODE    );
  fprintf( stdout, "  STRETCH : %ld\n", HashTable->NUMBER_STRETCH );
  fprintf( stdout, "  RESIZE  : %ld\n", HashTable->NUMBER_RESIZE  );
  fprintf( stdout, "  SCAN    : %ld\n", HashTable->NUMBER_SCAN    );
  fprintf( stdout, "  ADD     : %ld\n", HashTable->NUMBER_ADD     );

  if ( FuncView )
  {
    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      HashNode = HashTable->TABLE[ HashIndex ];

      if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
           ( HashNode != BDD_HASH_NODE_DELETED ) )
      {
        fprintf( stdout, "  INDEX: %10ld  ", HashIndex );

        (*FuncView)( HashNode );
      }
    }
  }

  fprintf( stdout, "<-- HashNodeTable\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd View Index Node                    |
|                                                             |
\------------------------------------------------------------*/

void viewbddindexnode( BddSystem, FuncView )

  bddsystem *BddSystem;
  void      (*FuncView)();
{
  bddindexnode *IndexNode;
  bddindex      Index;
  bddindex      NumberIndex;

  if ( BddSystem == (bddsystem *)0 )
  {
    BddSystem = BddLocalSystem;
  }

  IndexNode   = BddSystem->INDEX_NODE;
  NumberIndex = BddSystem->NUMBER_INDEX;

  fprintf( stdout, "--> BddIndexNode\n" );

  for ( Index = 0; Index < NumberIndex; Index++ )
  {
    fprintf( stdout, "--> INDEX_NODE[ %d ]\n", Index );

    viewbddhnodetable( IndexNode[ Index ], FuncView );

    fprintf( stdout, "<-- INDEX_NODE[ %d ]\n", Index );
  }

  fprintf( stdout, "<-- BddIndexNode\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd View Variable Node                 |
|                                                             |
\------------------------------------------------------------*/

void viewbddvarnode( BddSystem, FuncView )

  bddsystem *BddSystem;
  void      (*FuncView)();
{
  bddvarnode *VarNode;
  bddindex    Index;
  bddindex    NumberIndex;

  if ( BddSystem == (bddsystem *)0 )
  {
    BddSystem = BddLocalSystem;
  }

  VarNode     = BddSystem->VAR_NODE;
  NumberIndex = BddSystem->NUMBER_INDEX;

  fprintf( stdout, "--> BddVarNode\n" );

  for ( Index = 0; Index < NumberIndex; Index++ )
  {
    fprintf( stdout, "--> VAR_NODE[ %d ] : ", Index );

    if ( FuncView )
    {
      (*FuncView)( VarNode[ Index ] );
    }
    else
    {
      fprintf( stdout, "\n" );
    }
  }

  fprintf( stdout, "<-- BddVarNode\n" );
}
