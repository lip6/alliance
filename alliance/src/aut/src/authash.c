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
| File    :                   authash.c                       |
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

# include "authash.h"
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

  long AUT_HASH_PRIME_NUMBER[ AUT_MAX_PRIME_NUMBER ] =
  {
            1,
            3,
            5,
            7,
           13,
           23,
           59,
          101,
          503,
         1009,
         2003,
         3001,
         4001,
         5003,
         6007,
         7001,
         8009,
         9001,
        10007,
        12007,
        14009,
        16001,
        18013,
        20011,
        25013,
        30011,
        35023,
        40009,
        50021,
        60013,
        80021,
       100003,
       150001,
       200003,
       250007,
       300007,
       400009,
       600011,
       800011,
      1000003,
      1500007,
      2000003,
      2642201,
      3328979,
      4194287,
      5284393,
      6657919,
      8388593,
     10568797,
     13315831,
     16777199,
     33554393,
     67108859,
    134217689,
    268435399,
    536870879,
    1073741789,
    2147483629
  };

  long AUT_HASH_STRETCH_FACTOR = HASH_STRETCH_FACTOR;
  long AUT_HASH_MAX_SCAN       = HASH_MAX_SCAN;

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

void setauthfunc( HashTable, FuncSize, FuncKey, FuncIndex )

  authtable *HashTable;
  long     (*FuncSize)();
  long     (*FuncKey)();
  long     (*FuncIndex)();
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

long getauthsize( Size )

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

long getauthkey( Table, Key )

  authtable *Table;
  char      *Key;
{
  return( ( (unsigned long)Key >> 2 ) % Table->TABLE_SIZE );
}
 
/*------------------------------------------------------------\
|                                                             |
|                       Aut Get Hash Index                    |
|                                                             |
\------------------------------------------------------------*/

long getauthindex( Table, Key, Index )

  authtable *Table;
  char      *Key;
  long       Index;
{
  Index = Index + 1;

  if ( Index >= Table->TABLE_SIZE ) Index = 0;

  return( Index );
}

/*------------------------------------------------------------\
|                                                             |
|                       Aut Check Hash Key                    |
|                                                             |
\------------------------------------------------------------*/

int checkauthkey( Key, Severity )

  char *Key;
  int   Severity;
{
  if ( ( Key == AUT_HASH_KEY_EMPTY   ) ||
       ( Key == AUT_HASH_KEY_DELETED ) )
  {
    if ( Severity )
    {
      auterror( AUT_HASH_KEY_ERROR, Key );
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

authtable *createauthtable( Length )

  long Length;
{
  authtable *HashTable;
  authelem  *Table;

  if ( Length < 0 )
  {
    auterror( AUT_HASH_SIZE_ERROR, Length );
  }

  Length = getauthsize( Length );

  HashTable = allocauthtable();
  Table     = allocauthelem( Length );

  HashTable->TABLE      = Table;
  HashTable->TABLE_SIZE = Length;
  HashTable->FUNC_SIZE  = getauthsize;
  HashTable->FUNC_INDEX = getauthindex;
  HashTable->FUNC_KEY   = getauthkey;

  return( HashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Destroy Hash Table                  |
|                                                             |
\------------------------------------------------------------*/

void destroyauthtable( HashTable )

  authtable *HashTable;
{
  freeauthelem( HashTable->TABLE );
  freeauthtable( HashTable       );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Reset Hash Table                    |
|                                                             |
\------------------------------------------------------------*/

void resetauthtable( HashTable )

  authtable *HashTable;
{
  if ( HashTable->NUMBER_ELEM > 0 )
  {
    memset( (void *)HashTable->TABLE, 0,
            (size_t)HashTable->TABLE_SIZE * sizeof( authelem ) );
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

void stretchauthtable( HashTable )

  authtable *HashTable;
{
  authelem *NewTable;
  authelem *Table;
  authelem *Element;
  long      NewLength;
  long      Length;

  Length    = HashTable->TABLE_SIZE;
  Table     = HashTable->TABLE;
  NewLength = (*HashTable->FUNC_SIZE)( Length * AUT_HASH_STRETCH_FACTOR / 100 );
  NewTable  = allocauthelem( NewLength );

  HashTable->TABLE           = NewTable;
  HashTable->TABLE_SIZE      = NewLength;
  HashTable->NUMBER_ELEM     = 0;
  HashTable->NUMBER_STRETCH += 1;

  while ( Length > 0 )
  {
    Length  = Length - 1;
    Element = &Table[ Length ];

    if ( ( Element->KEY != AUT_HASH_KEY_EMPTY   ) &&
         ( Element->KEY != AUT_HASH_KEY_DELETED ) )
    {
      addauthelem( HashTable, 
                   Element->KEY,
                   Element->VALUE );
    }
  }

  freeauthelem( Table );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut Add Hash Element                   |
|                                                             |
\------------------------------------------------------------*/

authelem *addauthelem( HashTable, Key, Value )

  authtable *HashTable;
  char      *Key;
  long       Value;
{
  authelem *Element;
  long      HashIndex;
  long      Deleted;
  int       NumberScan;
  char      Stop;

  check_authkey( Key );

  HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key );
  Deleted    = -1;
  NumberScan = 0;
  Stop       = 0;

  HashTable->NUMBER_ADD++;

  do
  {
    Element = &HashTable->TABLE[ HashIndex ];

    if ( Element->KEY == AUT_HASH_KEY_DELETED )
    {
      if ( Deleted == -1 ) Deleted = HashIndex;
    }
    else
    if ( ( Element->KEY == AUT_HASH_KEY_EMPTY ) ||
         ( Element->KEY == Key                ) )
    {
      if ( Element->KEY == AUT_HASH_KEY_EMPTY )
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
        Element->KEY = AUT_HASH_KEY_DELETED;
        Element = &HashTable->TABLE[ Deleted ];
      }
      
      Element->VALUE = Value;
      Element->KEY   = Key;

      Stop = 1;
    }

    NumberScan += 1;

    if ( Stop == 0 )
    {
      if ( NumberScan > AUT_HASH_MAX_SCAN )
      {
        if ( Deleted != -1 )
        {
          HashTable->TABLE[ Deleted ].KEY = AUT_HASH_KEY_EMPTY;
          HashIndex = Deleted;
          Deleted   = -1;
        }
        else
        {
          stretchauthtable( HashTable );
        
          HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key );
          NumberScan = 0;
        }
      }
      else
      {
        HashIndex = (*HashTable->FUNC_INDEX)( HashTable, Key, HashIndex );
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

int delauthelem( HashTable, Key )

  authtable *HashTable;
  char      *Key;
{
  authelem *Element;
  long      HashIndex;
  int       NumberScan;

  check_authkey( Key );

  HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key );
  NumberScan = 0;

  HashTable->NUMBER_DEL++;

  do
  {
    Element = &HashTable->TABLE[ HashIndex ];

    if ( Element->KEY == Key )
    {
      HashTable->NUMBER_ELEM--;

      Element->KEY = AUT_HASH_KEY_DELETED;

      HashTable->NUMBER_SCAN += NumberScan + 1;

      return( 1 );
    }

    if ( Element->KEY == AUT_HASH_KEY_EMPTY )
    {
      return( 0 );
    }

    NumberScan += 1;
    HashIndex   = (*HashTable->FUNC_INDEX)( HashTable, Key, HashIndex );
  }
  while ( NumberScan <= AUT_HASH_MAX_SCAN );

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut Search Hash Element                |
|                                                             |
\------------------------------------------------------------*/

authelem *searchauthelem( HashTable, Key )

  authtable *HashTable;
  char      *Key;
{
  authelem *Element;
  long      HashIndex;
  int       NumberScan;

  check_authkey( Key );

  HashIndex  = (*HashTable->FUNC_KEY)( HashTable, Key );
  NumberScan = 0;

  do
  {
    Element = &HashTable->TABLE[ HashIndex ];

    if ( Element->KEY == Key )
    {
      return( Element );
    }

    if ( Element->KEY == AUT_HASH_KEY_EMPTY )
    {
      return( (authelem *)0 );
    }

    NumberScan += 1;
    HashIndex   = (*HashTable->FUNC_INDEX)( HashTable, Key, HashIndex );
  }
  while ( NumberScan <= AUT_HASH_MAX_SCAN );

  return( (authelem *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut View Hash Element                  |
|                                                             |
\------------------------------------------------------------*/

void viewauthelem( Element )

  authelem *Element;
{
  fprintf( stdout, "VALUE: %10ld  KEY: %s\n", 
           Element->VALUE, Element->KEY );
}

/*------------------------------------------------------------\
|                                                             |
|                      Aut View Hash Table                    |
|                                                             |
\------------------------------------------------------------*/

void viewauthtable( HashTable, FuncView )

  authtable *HashTable;
  void       (*FuncView)();
{
  authelem *Element;
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
  
      if ( checkauthkey( Element->KEY, 0 ) )
      {
        fprintf( stdout, "  INDEX   : %-4ld ", HashIndex );

        (*FuncView)( Element );
      }
    }
  }

  fprintf( stdout, "<-- HashTable\n" );
}
