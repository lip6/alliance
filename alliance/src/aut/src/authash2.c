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
| Tool    :                     Aut                           |
|                                                             |
| File    :                   authash2.c                      |
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

# include <stdio.h>
# include <string.h>
# include <memory.h>

# include <mut.h>
# include "aut.h"


# include "authash2.h"
# include "auterror.h"

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

  long AUT_HASH2_STRETCH_FACTOR = HASH2_STRETCH_FACTOR;
  long AUT_HASH2_MAX_SCAN       = HASH2_MAX_SCAN;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Aut Set Hash Size Function               |
|                                                             |
\------------------------------------------------------------*/

void setauth2func( HashTable, FuncSize, FuncKey, FuncIndex )

  auth2table *HashTable;
  long      (*FuncSize)();
  long      (*FuncKey)();
  long      (*FuncIndex)();
{
  if ( FuncSize  != 0 ) HashTable->FUNC_SIZE  = FuncSize;
  if ( FuncKey   != 0 ) HashTable->FUNC_KEY   = FuncKey;
  if ( FuncIndex != 0 ) HashTable->FUNC_INDEX = FuncIndex;
}

/*------------------------------------------------------------\
|                                                             |
|                       Aut Get Hash Size                     |
|                                                             |
\------------------------------------------------------------*/

long getauth2size( Size )

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

  if ( ( Size & 1 ) == 0 )
  {
    Size++;
  }

  return( Size );
}

/*------------------------------------------------------------\
|                                                             |
|                       Aut Get Hash Key                      |
|                                                             |
\------------------------------------------------------------*/

long getauth2key( Table, Key1, Key2 )

  auth2table *Table;
  char       *Key1;
  char       *Key2;
{
  return( ( ((unsigned long)Key1 >> 2 ) +
            ((unsigned long)Key2 << 1 ) ) % Table->TABLE_SIZE );
}
 
/*------------------------------------------------------------\
|                                                             |
|                       Aut Get Hash Index                    |
|                                                             |
\------------------------------------------------------------*/

long getauth2index( Table, Key1, Key2, Index )

  auth2table *Table;
  char       *Key1;
  char       *Key2;
  long        Index;
{
  return( ( ((unsigned long)Index       ) +
            ((unsigned long)Index  << 2 ) +
            ((unsigned long)Key2        ) +
            ((unsigned long)Key1   << 1 ) ) % Table->TABLE_SIZE );
}

/*------------------------------------------------------------\
|                                                             |
|                       Aut Check Hash Key                    |
|                                                             |
\------------------------------------------------------------*/

int checkauth2key( Key1, Key2, Severity )

  char *Key1;
  char *Key2;
  int   Severity;
{
  if ( ( Key1 == AUT_HASH_KEY_EMPTY   ) ||
       ( Key1 == AUT_HASH_KEY_DELETED ) )
  {
    if ( Severity )
    {
      auterror( AUT_HASH_KEY_ERROR, Key1 );
    }

    return( 0 );
  }

  if ( ( Key2 == AUT_HASH_KEY_EMPTY   ) ||
       ( Key2 == AUT_HASH_KEY_DELETED ) )
  {
    if ( Severity )
    {
      auterror( AUT_HASH_KEY_ERROR, Key2 );
    }

    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut Create Hash Table                  |
|                                                             |
\------------------------------------------------------------*/

auth2table *createauth2table( Length )

  long Length;
{
  auth2table *HashTable;
  auth2elem  *Table;

  if ( Length < 0 )
  {
    auterror( AUT_HASH_SIZE_ERROR, Length );
  }

  Length = getauth2size( Length );

  HashTable = allocauth2table();
  Table     = allocauth2elem( Length );

  HashTable->TABLE      = Table;
  HashTable->TABLE_SIZE = Length;
  HashTable->FUNC_SIZE  = getauth2size;
  HashTable->FUNC_INDEX = getauth2index;
  HashTable->FUNC_KEY   = getauth2key;

  return( HashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Destroy Hash Table                  |
|                                                             |
\------------------------------------------------------------*/

void destroyauth2table( HashTable )

  auth2table *HashTable;
{
  freeauth2elem( HashTable->TABLE );
  freeauth2table( HashTable       );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Reset Hash Table                    |
|                                                             |
\------------------------------------------------------------*/

void resetauth2table( HashTable )

  auth2table *HashTable;
{
  if ( HashTable->NUMBER_ELEM > 0 )
  {
    memset( (void *)HashTable->TABLE, 0,
            (size_t)HashTable->TABLE_SIZE * sizeof( auth2elem ) );
  }

  HashTable->NUMBER_ELEM    = 0;
  HashTable->NUMBER_ADD     = 0;
  HashTable->NUMBER_SCAN    = 0;
  HashTable->NUMBER_DEL     = 0;
  HashTable->NUMBER_STRETCH = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut Stretch Hash Table                 |
|                                                             |
\------------------------------------------------------------*/

void stretchauth2table( HashTable )

  auth2table *HashTable;
{
  auth2elem *NewTable;
  auth2elem *Table;
  auth2elem *Element;
  long       NewLength;
  long       Length;

  Length    = HashTable->TABLE_SIZE;
  Table     = HashTable->TABLE;
  NewLength = (*HashTable->FUNC_SIZE)( Length * AUT_HASH2_STRETCH_FACTOR / 100 );
  NewTable  = allocauth2elem( NewLength );

  HashTable->TABLE           = NewTable;
  HashTable->TABLE_SIZE      = NewLength;
  HashTable->NUMBER_ELEM     = 0;
  HashTable->NUMBER_STRETCH += 1;

  while ( Length > 0 )
  {
    Length  = Length - 1;
    Element = &Table[ Length ];

    if ( ( Element->KEY1 != AUT_HASH_KEY_EMPTY   ) &&
         ( Element->KEY1 != AUT_HASH_KEY_DELETED ) )
    {
      addauth2elem( HashTable, 
                    Element->KEY1,
                    Element->KEY2,
                    Element->VALUE );
    }
  }

  freeauth2elem( Table );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut Add Hash Element                   |
|                                                             |
\------------------------------------------------------------*/

auth2elem *addauth2elem( HashTable, Key1, Key2, Value )

  auth2table *HashTable;
  char       *Key1;
  char       *Key2;
  long        Value;
{
  auth2elem *Element;
  long       HashIndex;
  long       Deleted;
  int        NumberScan;
  char       Stop;

  check_auth2key( Key1, Key2 );

  HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key1, Key2 );
  Deleted    = -1;
  NumberScan = 0;
  Stop       = 0;

  HashTable->NUMBER_ADD++;

  do
  {
    Element = &HashTable->TABLE[ HashIndex ];

    if ( Element->KEY1 == AUT_HASH_KEY_DELETED )
    {
      if ( Deleted == -1 ) Deleted = HashIndex;
    }
    else
    if ( ( Element->KEY1 == AUT_HASH_KEY_EMPTY ) ||
         ( ( Element->KEY1 == Key1           ) &&
           ( Element->KEY2 == Key2           ) ) )
    {
      if ( Element->KEY1 == AUT_HASH_KEY_EMPTY )
      {
        HashTable->NUMBER_ELEM++;

        if ( Deleted != -1 )
        {
          Element = &HashTable->TABLE[ Deleted ];
        }
      }
      else
      if ( Deleted != -1 )
      {
        Element->KEY1 = AUT_HASH_KEY_DELETED;
        Element = &HashTable->TABLE[ Deleted ];
      }
      
      Element->VALUE = Value;
      Element->KEY1  = Key1;
      Element->KEY2  = Key2;

      Stop = 1;
    }

    NumberScan += 1;

    if ( Stop == 0 )
    {
      if ( NumberScan > AUT_HASH2_MAX_SCAN )
      {
        if ( Deleted != -1 )
        {
          HashTable->TABLE[ Deleted ].KEY1 = AUT_HASH_KEY_EMPTY;
          HashIndex = Deleted;
          Deleted   = -1;
        }
        else
        {
          stretchauth2table( HashTable );
        
          HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key1, Key2 );
          NumberScan = 0;
        }
      }
      else
      {
        HashIndex = (*HashTable->FUNC_INDEX)( HashTable, Key1, Key2, HashIndex );
      }
    }
  }
  while ( Stop == 0 );

  HashTable->NUMBER_SCAN += NumberScan + 1;

  return( Element );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut Del Hash Element                   |
|                                                             |
\------------------------------------------------------------*/

int delauth2elem( HashTable, Key1, Key2 )

  auth2table *HashTable;
  char       *Key1;
  char       *Key2;
{
  auth2elem *Element;
  long       HashIndex;
  int        NumberScan;

  check_auth2key( Key1, Key2 );

  HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key1, Key2 );
  NumberScan = 0;

  HashTable->NUMBER_DEL++;

  do
  {
    Element = &HashTable->TABLE[ HashIndex ];

    if ( ( Element->KEY1 == Key1 ) &&
         ( Element->KEY2 == Key2 ) )
    {
      HashTable->NUMBER_ELEM--;

      Element->KEY1 = AUT_HASH_KEY_DELETED;

      HashTable->NUMBER_SCAN += NumberScan + 1;

      return( 1 );
    }

    if ( Element->KEY1 == AUT_HASH_KEY_EMPTY )
    {
      return( 0 );
    }

    NumberScan += 1;
    HashIndex   = (*HashTable->FUNC_INDEX)( HashTable, Key1, Key2, HashIndex );
  }
  while ( NumberScan <= AUT_HASH2_MAX_SCAN );

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut Search Hash Element                |
|                                                             |
\------------------------------------------------------------*/

auth2elem *searchauth2elem( HashTable, Key1, Key2 )

  auth2table *HashTable;
  char       *Key1;
  char       *Key2;
{
  auth2elem *Element;
  long       HashIndex;
  int        NumberScan;

  check_auth2key( Key1, Key2 );

  HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key1, Key2 );
  NumberScan = 0;

  do
  {
    Element = &HashTable->TABLE[ HashIndex ];

    if ( ( Element->KEY1 == Key1 ) &&
         ( Element->KEY2 == Key2 ) )
    {
      return( Element );
    }

    if ( Element->KEY1 == AUT_HASH_KEY_EMPTY )
    {
      return( (auth2elem *)0 );
    }

    NumberScan += 1;
    HashIndex   = (*HashTable->FUNC_INDEX)( HashTable, Key1, Key2, HashIndex );
  }
  while ( NumberScan <= AUT_HASH2_MAX_SCAN );

  return( (auth2elem *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut View Hash Element                  |
|                                                             |
\------------------------------------------------------------*/

void viewauth2elem( Element )

  auth2elem *Element;
{
  fprintf( stdout, "VALUE: %10ld  KEY1: %s KEY2: %s\n", 
           Element->VALUE, Element->KEY1, Element->KEY2 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut View Hash Table                    |
|                                                             |
\------------------------------------------------------------*/

void viewauth2table( HashTable, FuncView )

  auth2table *HashTable;
  void       (*FuncView)();
{
  auth2elem *Element;
  long      HashIndex;

  fprintf( stdout, "--> HashTable\n" );
  fprintf( stdout, "  SIZE    : %ld\n", HashTable->TABLE_SIZE     );
  fprintf( stdout, "  ELEMENT : %ld\n", HashTable->NUMBER_ELEM    );
  fprintf( stdout, "  STRETCH : %ld\n", HashTable->NUMBER_STRETCH );
  fprintf( stdout, "  SCAN    : %ld\n", HashTable->NUMBER_SCAN    );
  fprintf( stdout, "  ADD     : %ld\n", HashTable->NUMBER_ADD     );
  fprintf( stdout, "  DEL     : %ld\n", HashTable->NUMBER_DEL     );

  if ( FuncView )
  {
    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      Element = &HashTable->TABLE[ HashIndex ];
  
      if ( checkauth2key( Element->KEY1, Element->KEY2, 0 ) )
      {
        fprintf( stdout, "  INDEX   : %-4ld ", HashIndex );

        (*FuncView)( Element );
      }
    }
  }

  fprintf( stdout, "<-- HashTable\n" );
}
