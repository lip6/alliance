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
| File    :                   auttest.c                       |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <mut.h>
# include "aut.h"

# include "auttest.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static char        TestBuffer[ AUT_TEST_BUFFER_SIZE ];
  static authtable  *TestHashTable  = (authtable  *)0;
  static auth2table *TestHashTable2 = (auth2table *)0;
  static char       TestExit   = 0;
  static char       TestMode2  = 0;
  static long       TestIndex  = 0;

  static command TestCommandDefine[ AUT_TEST_MAX_COMMAND ] =
  {
    { "addelem"   , TestCommandAddElement     },
    { "addelem2"  , TestCommandAddElement2    },
    { "delelem"   , TestCommandDelElement     },
    { "delelem2"  , TestCommandDelElement2    },
    { "exit"      , TestCommandExit           },
    { "help"      , TestCommandHelp           },
    { "list"      , TestCommandHelp           },
    { "quit"      , TestCommandExit           },
    { "reset"     , TestCommandReset          },
    { "reset2"    , TestCommandReset2         },
    { "viewelem"  , TestCommandViewElement    },
    { "viewelem2" , TestCommandViewElement2   },
    { "viewhash"  , TestCommandViewHash       },
    { "viewhash2" , TestCommandViewHash2      }
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Test Command Compare                  |
|                                                             |
\------------------------------------------------------------*/

static int TestCommandCompare( FirstKey, SecondKey )

    command *FirstKey;
    command *SecondKey;
{
  return( strcmp( FirstKey->NAME, SecondKey->NAME ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Get Command Value                 |
|                                                             |
\------------------------------------------------------------*/

static command *TestGetCommand( String )

  char *String;
{
  command  Entry;

  Entry.NAME = String;

  return( (command *)bsearch( (char *)(&Entry),
                              (char *)TestCommandDefine,
                              AUT_TEST_MAX_COMMAND,
                              sizeof( command ),
                              TestCommandCompare ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Command Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Test Command Exit                      |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandExit( String )

  char *String;
{
  fprintf( stdout, "--> exit\n" );
  TestExit = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Help                      |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandHelp( String )

  char *String;
{
  int Counter;

  fprintf( stdout, "--> help" );

  for ( Counter = 0; Counter < AUT_TEST_MAX_COMMAND; Counter++ )
  {
    if ( ( Counter & 0x3 ) == 0 )
    {
      fprintf( stdout, "\n" );
    }

    fprintf( stdout, " %-8s", TestCommandDefine[ Counter ].NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Add Element               |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandAddElement( String )

  char *String;
{
  authelem *AutElement;
  char     *Scan;

  if ( TestMode2 ) return;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = namealloc( String );
    fprintf( stdout, "--> addelem %s\n", String );

    AutElement = addauthelem( TestHashTable, String, TestIndex++ );
    viewauthelem( AutElement );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Add Element 2             |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandAddElement2( String )

  char *String;
{
  auth2elem *AutElement;
  char      *Scan;

  if ( ! TestMode2 ) return;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = namealloc( String   );
    Scan   = namealloc( Scan + 1 );

    fprintf( stdout, "--> addelem %s %s\n", String, Scan );

    AutElement = addauth2elem( TestHashTable2, String, Scan, TestIndex++ );
    viewauth2elem( AutElement );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Element               |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewElement( String )

  char *String;
{
  authelem *AutElement;
  char     *Scan;

  if ( TestMode2 ) return;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = namealloc( String );
    fprintf( stdout, "--> viewelem %s\n", String );

    AutElement = searchauthelem( TestHashTable, String );

    if ( AutElement != (authelem *)0 )
    {
      viewauthelem( AutElement );
    }
    else
    {
      fprintf( stderr, "Element %s doesn't exist\n", String );
    }
  }
  else
  {
    viewauthtable( TestHashTable, viewauthelem );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Element 2             |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewElement2( String )

  char *String;
{
  auth2elem *AutElement;
  char      *Scan;

  if ( ! TestMode2 ) return;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = namealloc( String   );
    Scan   = namealloc( Scan + 1 );
    fprintf( stdout, "--> viewelem2 %s %s\n", String, Scan );

    AutElement = searchauth2elem( TestHashTable2, String, Scan );

    if ( AutElement != (auth2elem *)0 )
    {
      viewauth2elem( AutElement );
    }
    else
    {
      fprintf( stderr, "Element %s %s doesn't exist\n", String, Scan );
    }
  }
  else
  {
    viewauth2table( TestHashTable2, viewauth2elem );
  }
}


/*------------------------------------------------------------\
|                                                             |
|                      Test Command Del Element               |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandDelElement( String )

  char *String;
{
  char *Scan;

  if ( TestMode2 ) return;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = namealloc( String );
    if ( delauthelem( TestHashTable, String ) )
    {
      fprintf( stdout, "--> delelem %s\n", String );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Del Element 2             |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandDelElement2( String )

  char *String;
{
  char *Scan;

  if ( ! TestMode2 ) return;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = namealloc( String   );
    Scan   = namealloc( Scan + 1 );
    if ( delauth2elem( TestHashTable2, String, Scan ) )
    {
      fprintf( stdout, "--> delelem2 %s %s\n", String, Scan );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Reset                     |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandReset( String )

  char *String;
{
  if ( TestMode2 ) return;

  fprintf( stdout, "--> reset\n" );
  resetauthtable( TestHashTable );
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Reset 2                   |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandReset2( String )

  char *String;
{
  if ( ! TestMode2 ) return;

  fprintf( stdout, "--> reset2\n" );
  resetauth2table( TestHashTable2 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Hash                  |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewHash( String )

  char *String;
{
  if ( TestMode2 ) return;

  fprintf( stdout, "--> viewhash\n" );
  viewauthtable( TestHashTable, 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Hash 2                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewHash2( String )

  char *String;
{
  if ( ! TestMode2 ) return;

  fprintf( stdout, "--> viewhash2\n" );
  viewauth2table( TestHashTable2, 0 );
}


/*------------------------------------------------------------\
|                                                             |
|                            Main Loop                        |
|                                                             |
\------------------------------------------------------------*/

static int TestMainLoop()
{
  command *Command;
  char    *Scan;

  TestExit  = 0;
  TestIndex = 0;

  do
  {
    fprintf( stdout, "\ncommand > " );

    if ( ! fgets( TestBuffer, AUT_TEST_BUFFER_SIZE, stdin ) )
    {
      fprintf( stderr, "Interrupt by user\n" );

      return( 0 );
    }

    Scan = strchr( TestBuffer, '\n' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    Scan = strchr( TestBuffer, ' '  );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0'; Scan = Scan + 1;
    }

    Command = TestGetCommand( TestBuffer );

    if ( Command != (command *)0 )
    {
      (*Command->FUNCTION)( Scan );
    }
    else
    {
      fprintf( stderr, "syntax error\n" );
    }
  }
  while ( TestExit == 0 );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                         Test Aut Hash Table                 |
|                                                             |
\------------------------------------------------------------*/

int testauthtable( HashTable )

  authtable *HashTable;
{
  TestHashTable = HashTable;
  TestMode2     = 0;

  return( TestMainLoop() );
}

/*------------------------------------------------------------\
|                                                             |
|                         Test Aut Hash Table 2               |
|                                                             |
\------------------------------------------------------------*/

int testauth2table( HashTable )

  auth2table *HashTable;
{
  TestHashTable2 = HashTable;
  TestMode2      = 1;

  return( TestMainLoop() );
}
