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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                   bddtest.c                       |
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

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"

# include "bddtest.h"
# include "bdderror.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static char     TestBuffer[ BDD_TEST_BUFFER_SIZE ];
  static char     TestExit        = 0;
  static char     TestLogout      = 0;
  static char     TestViewSumProd = 0;

  static command TestCommandDefine[ BDD_TEST_MAX_COMMAND ] =
  {
    { "addass"    , TestCommandAddAssoc   , "in out"            },
    { "addaux"    , TestCommandAddAuxiliar, "aux out [s|g]"     },
    { "addin"     , TestCommandAddInput   , "in [f|l|b|a] [in]" },
    { "addout"    , TestCommandAddOutput  , "out"               },
    { "cofact"    , TestCommandCofactor   , "out out"           },
    { "compose"   , TestCommandCompose    , "out in out"        },
    { "delass"    , TestCommandDelAssoc   , "[in]"              },
    { "delout"    , TestCommandDelOutput  , "out"               },
    { "dumpcir"   , TestCommandDumpCircuit, "name"              },
    { "exist"     , TestCommandExist      , "out [m]"           },
    { "forall"    , TestCommandForall     , "out [m]"           },
    { "fraction"  , TestCommandFraction   , "out"               },
    { "garbage"   , TestCommandGarbage    , "none"              },
    { "help"      , TestCommandHelp       , "[keyword]"         },
    { "imply"     , TestCommandImply      , "out out"           },
    { "inter"     , TestCommandIntersect  , "out out"           },
    { "list"      , TestCommandHelp       , "[keyword]"         },
    { "logout"    , TestCommandLogout     , "none"              },
    { "optimize"  , TestCommandOptimize   , "[s|n]"             },
    { "quit"      , TestCommandExit       , "none"              },
    { "reduce"    , TestCommandReduce     , "out out"           },
    { "relprod"   , TestCommandRelProduct , "out out"           },
    { "renin"     , TestCommandRenInput   , "in name"           },
    { "reorder"   , TestCommandReorder    , "[s|w|t]"           },
    { "restrict"  , TestCommandRestrict   , "out in 0|1"        },
    { "satisfy"   , TestCommandSatisfy    , "out [a]"           },
    { "simpoff"   , TestCommandSimpOff    , "out out"           },
    { "simpon"    , TestCommandSimpOn     , "out out"           },
    { "subst"     , TestCommandSubstitute , "out"               },
    { "support"   , TestCommandSupport    , "out"               },
    { "swapvar"   , TestCommandSwapVar    , "variable"          },
    { "sweep"     , TestCommandSweep      , "none"              },
    { "viewass"   , TestCommandViewAssoc  , "none"              },
    { "viewblo"   , TestCommandViewBlock  , "none"              },
    { "viewcir"   , TestCommandViewCircuit, "none"              },
    { "viewhea"   , TestCommandViewHeath  , "out"               },
    { "viewid"    , TestCommandViewIndex  , "[index]"           },
    { "viewin"    , TestCommandViewInput  , "[in]"              },
    { "viewop"    , TestCommandViewOper   , "none"              },
    { "viewout"   , TestCommandViewOutput , "[out]"             },
    { "viewsum"   , TestCommandViewSumProd, "none"              },
    { "viewsys"   , TestCommandViewSystem , "[i]"               },
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Test Get Name                        |
|                                                             |
\------------------------------------------------------------*/

static char *TestGetName( Name )

  char *Name;
{
  char *Scan;

  Scan = strchr( Name, '(' );
  if ( Scan != (char *)0 ) *Scan = ' ';

  Scan = strchr( Name, ')' );
  if ( Scan != (char *)0 ) *Scan = '\0';

  return( Name );
}

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
                              BDD_TEST_MAX_COMMAND,
                              sizeof( command ),
                              TestCommandCompare ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   Test View Input Element                   |
|                                                             |
\------------------------------------------------------------*/

static void TestViewInput( Element )

  authelem *Element;
{
  fprintf( stdout, "NAME: '%s' ", Element->KEY );

  viewbddnode( (bddsystem *)0, (bddnode *)( Element->VALUE ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   Test View Node Expression                 |
|                                                             |
\------------------------------------------------------------*/

static void TestViewNodeExpr( BddNode )

  bddnode *BddNode;
{
  chain_list *Expr;

  if ( TestViewSumProd != 2 )
  {
    viewbddnode( (bddsystem *)0, BddNode );

    if ( TestViewSumProd == 1 )
    {
      Expr = convertbddcircuitsumabl( (bddcircuit *)0, BddNode );
    }
    else
    {
      Expr = convertbddcircuitabl( (bddcircuit *)0, BddNode );
    }

    viewablexpr( Expr, ABL_VIEW_INFIX );
    delablexpr( Expr );
  }

  fprintf( stdout, "\n" );

}

/*------------------------------------------------------------\
|                                                             |
|                     Test View Assoc                         |
|                                                             |
\------------------------------------------------------------*/

static void TestViewAssoc( Variable, BddNode )

  bddvar   Variable;
  bddnode *BddNode;
{
  char     *NameIn;
  bddindex  BddIndex;

  BddIndex = getbddvarindex( BddLocalSystem, Variable );
  NameIn   = getbddcircuitinname( BddLocalCircuit, BddIndex );

  fprintf( stdout, "%s -> ", NameIn );
  TestViewNodeExpr( BddNode );
}


/*------------------------------------------------------------\
|                                                             |
|                   Test View Output Element                  |
|                                                             |
\------------------------------------------------------------*/

static void TestViewOutput( Element )

  authelem *Element;
{
  fprintf( stdout, "NAME: '%s' ", Element->KEY );

  TestViewNodeExpr( (bddnode *)( Element->VALUE ) );
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
|                      Test Command Logout                    |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandLogout( String )

  char *String;
{
  fprintf( stdout, "--> logout\n" );
  TestLogout = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                    Test Command Optimize                    |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandOptimize( String )

  char *String;
{
  if ( ( String != (char *)0 ) &&
       ( String[ 0 ] == 'n'  ) )
  {
    fprintf( stdout, "--> optimize num nodes\n" );
    fprintf( stdout, "Number nodes before optimization%ld\n", getbddcircuitnumnode( (bddcircuit *)0 ));

    optimizebddcircuit( (bddcircuit *)0, getbddcircuitnumnode, 2.0 );

    fprintf( stdout, "Number nodes after optimization%ld\n", getbddcircuitnumnode( (bddcircuit *)0 ));
  }

  if ( ( String != (char *)0 ) &&
       ( String[ 0 ] == 's'  ) )
  {
    fprintf( stdout, "--> optimize size\n" );
    fprintf( stdout, "Size before optimization %ld\n", getbddcircuitsize( (bddcircuit *)0 ));

    optimizebddcircuit( (bddcircuit *)0, getbddcircuitsize, 2.0 );

    fprintf( stdout, "Size after optimization %ld\n", getbddcircuitsize( (bddcircuit *)0 ));
  }

}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Help                      |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandHelp( String )

  char *String;
{
  command *Command;
  int      Counter;

  fprintf( stdout, "--> help" );

  if ( String != (char *)0 )
  {
    Command = TestGetCommand( String );

    if ( Command != (command *)0 )
    {
      fprintf( stdout, " %s %s\n", Command->NAME, Command->COMMENT );

      return;
    }
  }

  for ( Counter = 0; Counter < BDD_TEST_MAX_COMMAND; Counter++ )
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
|                      Test Command View SumProd              |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewSumProd( String )

  char *String;
{
  TestViewSumProd = ( TestViewSumProd + 1 ) % 3;

  fprintf( stdout, "--> viewsum %d\n", TestViewSumProd );
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Add Input                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandAddInput( String )

  char *String;
{
  bddnode *BddNode;
  bddnode *BddNodeIn;
  bddindex BddIndex;
  char    *Scan;
  char    *NameIn;
  char    *NameIn2;
  long     Mode;

  if ( String != (char *)0 )
  {
    BddIndex = 0;
    Mode     = BDD_IN_MODE_LAST;

    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
      NameIn = TestGetName( String );

      switch( Scan[ 1 ] )
      {
        case 'f' : Mode = BDD_IN_MODE_FIRST;
        break;
        case 'l' : Mode = BDD_IN_MODE_LAST;
        break;
        case 'b' : Mode = BDD_IN_MODE_BEFORE;
        break;
        case 'a' : Mode = BDD_IN_MODE_AFTER;
        break;
      }

      if ( ( Mode == BDD_IN_MODE_AFTER  ) ||
           ( Mode == BDD_IN_MODE_BEFORE ) )
      {
        String = Scan + 2;
        Scan   = strchr( String, ' ' );
        
        if ( Scan != (char *)0 )
        {
          *Scan  = '\0';
          NameIn2 = TestGetName( Scan + 1 );

          BddNodeIn = searchbddcircuitin( (bddcircuit *)0, NameIn2 );

          if ( BddNodeIn == (bddnode *)0 )
          {
            Mode = BDD_IN_MODE_LAST;
          }
          else
          {
            BddIndex = BddNodeIn->INDEX;
            String   = NameIn2;
          }
        }
        else
        {
          Mode = BDD_IN_MODE_LAST;
        }
      }
    }
    else
    {
      NameIn = TestGetName( String );
    }

    fprintf( stdout, "--> addin %s %ld %s\n", NameIn, Mode, String );
    BddNode = addbddcircuitin( (bddcircuit *)0, NameIn, BddIndex, Mode );

    viewbddnode( (bddsystem *)0, BddNode );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Ren Input                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandRenInput( String )

  char *String;
{
  bddnode *BddNode;
  char    *Scan;
  char    *NameIn;
  char    *NameIn2;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan   = '\0';
      NameIn  = TestGetName( String   );
      NameIn2 = TestGetName( Scan + 1 );

      fprintf( stdout, "--> renin %s %s\n", NameIn, NameIn2 );
      BddNode = renamebddcircuitin( (bddcircuit *)0, NameIn, NameIn2 );

      if ( BddNode != (bddnode *)0 )
      {
        viewbddnode( (bddsystem *)0, BddNode );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Add Auxiliar              |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandAddAuxiliar( String )

  char *String;
{
  bddnode *BddNode;
  bddnode *BddNodeOut;
  char    *Scan;
  char    *NameAux;
  long     Mode;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';

      NameAux = TestGetName( String );
      String  = Scan + 1;

      Scan = strchr( String, ' ' );
      Mode = BDD_AUX_MODE_SINGLE;

      if ( Scan != (char *)0 )
      {
        *Scan = '\0';

        switch( Scan[ 1 ] )
        {
          case 's' : Mode = BDD_AUX_MODE_SINGLE;
          break;
          case 'g' : Mode = BDD_AUX_MODE_GLOBAL;
          break;
        }
      }

      String     = TestGetName( String );
      BddNodeOut = searchbddcircuitout( (bddcircuit *)0, String );

      if ( BddNodeOut != (bddnode *)0 )
      {
        fprintf( stdout, "--> addaux %s %s %ld\n", NameAux, String, Mode );
        BddNode = addbddcircuitaux( (bddcircuit *)0, NameAux, BddNodeOut, (void *)0, Mode );

        viewbddnode( (bddsystem *)0, BddNode );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Add Assoc                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandAddAssoc( String )

  char *String;
{
  bddassoc *BddAssoc;
  bddnode  *BddNodeIn;
  bddnode  *BddNodeOut;
  char     *Scan;
  char     *NameIn;
  bddvar    Variable;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;

    *Scan = '\0';
    NameIn  = TestGetName( String );

    BddNodeIn = searchbddcircuitin( (bddcircuit *)0, NameIn );

    if ( BddNodeIn == (bddnode *)0 ) return;

    String = TestGetName( Scan + 1 );

    BddNodeOut = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeOut == (bddnode *)0 )
    {
      BddNodeOut = searchbddcircuitin( (bddcircuit *)0, String );

      if ( BddNodeOut == (bddnode *)0 ) return;
    }

    Variable = BddLocalSystem->INDEX_TO_VAR[ BddNodeIn->INDEX ];

    fprintf( stdout, "--> addassoc %s %s\n", NameIn, String );

    if ( BddLocalSystem->ASSOC == (bddassoc *)0 )
    {
      BddAssoc = addbddassoc( (bddsystem *)0 );
    }
    else
    {
      BddAssoc = BddLocalSystem->ASSOC;
    }

    BddAssoc = addbddnodeassoc( (bddsystem *)0, BddAssoc,
                                Variable      , BddNodeOut );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Del Assoc                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandDelAssoc( String )

  char *String;
{
  bddassoc *BddAssoc;
  bddnode  *BddNodeIn;
  bddvar    Variable;

  BddAssoc = BddLocalSystem->ASSOC;

  if ( BddAssoc != (bddassoc *)0 )
  {
    if ( String != (char *)0 )
    {
      String    = TestGetName( String );
      BddNodeIn = searchbddcircuitin( (bddcircuit *)0, String );

      if ( BddNodeIn == (bddnode *)0 ) return;

      Variable = BddLocalSystem->INDEX_TO_VAR[ BddNodeIn->INDEX ];

      fprintf( stdout, "--> delassoc %s\n", String );

      delbddnodeassoc( (bddsystem *)0, BddAssoc, Variable );
    }
    else
    {
      fprintf( stdout, "--> delassoc\n" );

      delbddassoc( (bddsystem *)0, BddAssoc );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command View Assoc                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewAssoc( String )

  char *String;
{
  bddassoc *BddAssoc;

  BddAssoc = BddLocalSystem->ASSOC;

  while ( BddAssoc != (bddassoc *)0 )
  {
    fprintf( stdout, "--> viewassoc %d\n", BddAssoc->IDENT );

    viewbddassoc( (bddsystem *)0, BddAssoc, TestViewAssoc );

    BddAssoc = BddAssoc->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Input                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewInput( String )

  char *String;
{
  bddnode *BddNode;

  if ( String != (char *)0 )
  {
    String = TestGetName( String );
    fprintf( stdout, "--> viewin %s\n", String );
    BddNode = searchbddcircuitin( (bddcircuit *)0, String );

    if ( BddNode != (bddnode *)0 )
    {
      viewbddnode( (bddsystem *)0, BddNode );
    }
    else
    {
      fprintf( stderr, "Input %s doesn't exist\n", String );
    }
  }
  else
  {
    viewauthtable( BddLocalCircuit->HASH_IN, TestViewInput );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Add Output                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandAddOutput( String )

  char *String;
{
  bddnode    *BddNode;
  char       *Scan;
  chain_list *Expr;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
      String = TestGetName( String );

      Expr = strablexpr( Scan + 1, ABL_VIEW_INFIX );

      if ( Expr != (chain_list *)0 )
      {
        BddNode = addbddcircuitabl( (bddcircuit *)0, Expr );
        decbddrefext( BddNode );

        delablexpr( Expr );

        fprintf( stdout, "--> addout %s\n", String );

        BddNode = addbddcircuitout( (bddcircuit *)0, String, BddNode );

        TestViewNodeExpr( BddNode );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Del Output                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandDelOutput( String )

  char *String;
{
  char       *Scan;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = TestGetName( String );

    if ( delbddcircuitout( (bddcircuit *)0, String ) )
    {
      fprintf( stdout, "--> delout %s\n", String );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command Support Output             |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandSupport( String )

  char *String;
{
  bddnode    *BddNode;
  chain_list *Support;
  chain_list *ScanChain;

  if ( String != (char *)0 )
  {
    String = TestGetName( String );
    fprintf( stdout, "--> support %s\n", String );
    BddNode = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNode != (bddnode *)0 )
    {
      Support = getbddnodesupportchain( (bddsystem *)0, BddNode );

      for ( ScanChain  = Support;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        viewbddnode( (bddsystem *)0, ScanChain->DATA );
      }

      freechain( Support );
    }
    else
    {
      fprintf( stderr, "Output %s doesn't exist\n", String );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Output                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewOutput( String )

  char *String;
{
  bddnode *BddNode;

  if ( String != (char *)0 )
  {
    String = TestGetName( String );
    fprintf( stdout, "--> viewout %s\n", String );
    BddNode = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNode != (bddnode *)0 )
    {
      TestViewNodeExpr( BddNode );
    }
    else
    {
      fprintf( stderr, "Output %s doesn't exist\n", String );
    }
  }
  else
  {
    viewauthtable( BddLocalCircuit->HASH_OUT, TestViewOutput );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Heath                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewHeath( String )

  char *String;
{
  bddnode    *BddNode;
  char       *Scan;
  bddheath   *BddHeath;
  bddheath   *ScanHeath;
  chain_list *Expr;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = TestGetName( String );
    fprintf( stdout, "--> viewheath %s\n", String );
    BddNode = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNode != (bddnode *)0 )
    {
      BddHeath = getbddheath( (bddsystem *)0, BddNode );

      for ( ScanHeath  = BddHeath;
            ScanHeath != (bddheath *)0;
            ScanHeath  = ScanHeath->NEXT )
      {
        fprintf( stdout, "Variable: " );
        Expr = convertbddcircuitabl( (bddcircuit *)0, ScanHeath->VAR );
        viewablexpr( Expr, ABL_VIEW_INFIX );
        delablexpr( Expr );
        fprintf( stdout, " Plus: " );
        Expr = convertbddcircuitabl( (bddcircuit *)0, ScanHeath->PLUS );
        viewablexpr( Expr, ABL_VIEW_INFIX );
        delablexpr( Expr );
        fprintf( stdout, " Minus: " );
        
        Expr = convertbddcircuitabl( (bddcircuit *)0, ScanHeath->MINUS );
        viewablexpr( Expr, ABL_VIEW_INFIX );
        fprintf( stdout, "\n" );
        delablexpr( Expr );
      }
    }
    else
    {
      fprintf( stderr, "Output %s doesn't exist\n", String );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 Test Command View Hash Oper                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewOper( String )

  char *String;
{
  fprintf( stdout, "--> viewop\n" );
  viewbddhopertable( BddLocalSystem->HASH_OPER, viewbddhoper );
}

/*------------------------------------------------------------\
|                                                             |
|                 Test Command View Index Node                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewIndex( String )

  char *String;
{
  char  *Scan;
  long   Index;

  fprintf( stdout, "--> viewid\n" );

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    Index = atol( String );

    if ( checkbddindex( (bddsystem *)0, Index, 0 ) )
    {
      viewbddhnodetable( BddLocalSystem->INDEX_NODE[ Index ], TestViewNodeExpr );

      return;
    }
  }

  viewbddindexnode( (bddsystem *)0, viewbddhnode );
}

/*------------------------------------------------------------\
|                                                             |
|                 Test Command View Block Node                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewBlock( String )

  char *String;
{
  fprintf( stdout, "--> viewblo\n" );
  viewbddblock( (bddsystem *)0, viewbddhnode );
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View System                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewSystem( String )

  char *String;
{
  fprintf( stdout, "--> viewsys\n" );

  if ( ( String != (char *)0 ) &&
       ( String[ 0 ] == 'i'  ) )
  {
    viewbddsystem( (bddsystem *)0, 1 );
  }
  else
  {
    viewbddsystem( (bddsystem *)0, 0 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command View Circuit               |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandViewCircuit( String )

  char *String;
{
  fprintf( stdout, "--> viewcir\n" );
  viewbddcircuit( (bddcircuit *)0, 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Dump Circuit              |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandDumpCircuit( String )

  char *String;
{
  if ( String != (char *)0 )
  {
    dumpbddcircuit( (bddcircuit *)0, String );
    fprintf( stdout, "--> dumpcir %s\n", String );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Swap Variable             |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandSwapVar( String )

  char *String;
{
  long Variable;

  if ( String != (char *)0 )
  {
    Variable = atol( String );

    if ( ( checkbddvar( (bddsystem *)0, Variable    , 0 ) ) &&
         ( checkbddvar( (bddsystem *)0, Variable + 1, 0 ) ) )
    {
      fprintf( stdout, "--> swapvar %ld with %ld\n", Variable, Variable + 1 );

      swapbddvar( (bddsystem *)0, Variable );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  Test Command Sweep                         |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandSweep( String )

  char *String;
{
  fprintf( stdout, "--> sweep\n" );
  fprintf( stdout, "Number nodes before %ld\n", BddLocalSystem->NUMBER_NODE );
  sweepbddsystem( (bddsystem *)0 );
  fprintf( stdout, "Number nodes after %ld\n", BddLocalSystem->NUMBER_NODE );
}

/*------------------------------------------------------------\
|                                                             |
|                  Test Command Garbage Collection            |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandGarbage( String )

  char *String;
{
  fprintf( stdout, "--> garbage\n" );
  fprintf( stdout, "Number nodes before %ld\n", BddLocalSystem->NUMBER_NODE );
  garbagebddsystem( (bddsystem *)0 );
  fprintf( stdout, "Number nodes after %ld\n", BddLocalSystem->NUMBER_NODE );
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Reorder                   |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandReorder( String )

  char *String;
{
  if ( ( String != (char *)0 ) &&
       ( String[ 0 ] == 'w'  ) )
  {
    fprintf( stdout, "--> reorder window\n" );
    fprintf( stdout, "Number nodes before %ld\n", BddLocalSystem->NUMBER_NODE );

    reorderbddsystemwindow( (bddsystem *)0 );
  }

  if ( ( String != (char *)0 ) &&
       ( String[ 0 ] == 's'  ) )
  {
    fprintf( stdout, "--> reorder simple\n" );
    fprintf( stdout, "Number nodes before %ld\n", BddLocalSystem->NUMBER_NODE );

    reorderbddsystemsimple( (bddsystem *)0 );
  }

  if ( ( String != (char *)0 ) &&
       ( String[ 0 ] == 't'  ) )
  {
    fprintf( stdout, "--> reorder top\n" );
    fprintf( stdout, "Number nodes before %ld\n", BddLocalSystem->NUMBER_NODE );

    reorderbddsystemtop( (bddsystem *)0 );
  }

  fprintf( stdout, "Number nodes after %ld\n", BddLocalSystem->NUMBER_NODE );
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Restrict                  |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandRestrict( String )

  char *String;
{
  bddnode *BddNodeOut;
  bddnode *BddNodeIn;
  bddnode *BddNodeOne;
  char    *Scan;
  char    *NameOut;
  long     Value;
  bddvar   Variable;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;

    *Scan = '\0';
    NameOut = TestGetName( String );
    BddNodeOut = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut == (bddnode *)0 ) return;

    String  = Scan + 1;
    Scan    = strchr( String, ' ' );
    if ( Scan == (char *)0 ) return;

    *Scan = '\0';
    String = TestGetName( String );
    BddNodeIn = searchbddcircuitin( (bddcircuit *)0, String );

    if ( BddNodeIn == (bddnode *)0 ) return;

    Variable = BddLocalSystem->INDEX_TO_VAR[ BddNodeIn->INDEX ];
    Value    = atol( Scan + 1 );

    if ( Value )
    {
      BddNodeOne = BddLocalSystem->ONE;
    }
    else
    {
      BddNodeOne = BddLocalSystem->ZERO;
    }

    fprintf( stdout, "--> restrict %s %s %ld\n", NameOut, String, Value );

    BddNodeOut = restrictbddnode( (bddsystem *)0, 
                                  BddNodeOut, Variable, BddNodeOne );
    decbddrefext( BddNodeOut );

    TestViewNodeExpr( BddNodeOut );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Compose                   |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandCompose( String )

  char *String;
{
  bddnode *BddNodeOut;
  bddnode *BddNodeIn;
  bddnode *BddNodeSubst;
  char    *Scan;
  char    *NameOut;
  char    *NameIn;
  bddvar   Variable;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;

    *Scan = '\0';
    NameOut = TestGetName( String );
    BddNodeOut = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut == (bddnode *)0 ) return;

    String  = Scan + 1;
    Scan    = strchr( String, ' ' );
    if ( Scan == (char *)0 ) return;

    *Scan = '\0';
    NameIn = TestGetName( String );
    BddNodeIn = searchbddcircuitin( (bddcircuit *)0, NameIn );

    if ( BddNodeIn == (bddnode *)0 ) return;

    Variable = BddLocalSystem->INDEX_TO_VAR[ BddNodeIn->INDEX ];

    String = TestGetName( Scan + 1 );
    BddNodeSubst = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeSubst == (bddnode *)0 ) return;

    fprintf( stdout, "--> compose %s %s %s\n", NameOut, NameIn, String );

    BddNodeOut = composebddnode( (bddsystem *)0, 
                                 BddNodeOut, Variable, BddNodeSubst );
    decbddrefext( BddNodeOut );
    addbddcircuitout( (bddcircuit *)0, NameOut, BddNodeOut );

    TestViewNodeExpr( BddNodeOut );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Substitute                |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandSubstitute( String )

  char *String;
{
  bddassoc *BddAssoc;
  bddnode  *BddNodeOut;
  bddnode  *BddNodeSubst;

  BddAssoc = BddLocalSystem->ASSOC;

  if ( BddAssoc != (bddassoc *)0 )
  {
    if ( String != (char *)0 )
    {
      String = TestGetName( String );
      BddNodeOut = searchbddcircuitout( (bddcircuit *)0, String );

      if ( BddNodeOut == (bddnode *)0 ) return;

      fprintf( stdout, "--> subst %s\n", String );

      BddNodeSubst = substbddnodeassoc( (bddsystem *)0, BddNodeOut, BddAssoc );
      decbddrefext( BddNodeSubst );

      TestViewNodeExpr( BddNodeSubst );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  Test Command Relational Product            |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandRelProduct( String )

  char *String;
{
  bddassoc *BddAssoc;
  bddnode  *BddNodeOut1;
  bddnode  *BddNodeOut2;
  bddnode  *BddNodeRelProd;
  char     *NameOut;
  char     *Scan;

  BddAssoc = BddLocalSystem->ASSOC;

  if ( BddAssoc != (bddassoc *)0 )
  {
    if ( String != (char *)0 )
    {
      Scan = strchr( String, ' ' );

      if ( Scan == (char *)0 ) return;
      *Scan = '\0';

      NameOut = TestGetName( String );
      BddNodeOut1 = searchbddcircuitout( (bddcircuit *)0, NameOut );

      if ( BddNodeOut1 == (bddnode *)0 ) return;

      String = TestGetName( Scan + 1 );
      BddNodeOut2 = searchbddcircuitout( (bddcircuit *)0, String );

      if ( BddNodeOut2 == (bddnode *)0 ) return;

      fprintf( stdout, "--> relprod %s %s\n", NameOut, String );

      BddNodeRelProd = relprodbddnodeassoc( (bddsystem *)0, BddNodeOut1, 
                                            BddNodeOut2, BddAssoc );
      decbddrefext( BddNodeRelProd );

      TestViewNodeExpr( BddNodeRelProd );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Test Command Forall                      |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandForall( String )

  char *String;
{
  bddassoc *BddAssoc;
  bddnode  *BddNodeOut;
  bddnode  *BddNodeForall;
  char     *Scan;
  int       Miss;

  BddAssoc = BddLocalSystem->ASSOC;

  if ( ( BddAssoc        != (bddassoc *)0 ) &&
       ( BddAssoc->FIRST != BDD_MAX_VAR   ) )
  {
    if ( String != (char *)0 )
    {
      Scan = strchr( String, ' ' );
      Miss = 0;

      if ( Scan != (char *)0 )
      {
        *Scan = '\0';
        if ( Scan[ 1 ] == 'm' ) Miss = 1;
      }

      String = TestGetName( String );
      BddNodeOut = searchbddcircuitout( (bddcircuit *)0, String );

      if ( BddNodeOut == (bddnode *)0 ) return;

      fprintf( stdout, "--> forall %s %d\n", String, Miss );

      if ( Miss )
      {
        BddNodeForall = forallbddnodemissassoc( (bddsystem *)0, BddNodeOut, BddAssoc );
      }
      else
      {
        BddNodeForall = forallbddnodeassoc( (bddsystem *)0, BddNodeOut, BddAssoc );
      }

      decbddrefext( BddNodeForall );

      TestViewNodeExpr( BddNodeForall );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command Exist                      |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandExist( String )

  char *String;
{
  bddassoc *BddAssoc;
  bddnode  *BddNodeOut;
  bddnode  *BddNodeExist;
  char     *Scan;
  int       Miss;

  BddAssoc = BddLocalSystem->ASSOC;

  if ( ( BddAssoc        != (bddassoc *)0 ) &&
       ( BddAssoc->FIRST != BDD_MAX_VAR   ) )
  {
    if ( String != (char *)0 )
    {
      Scan = strchr( String, ' ' );
      Miss = 0;

      if ( Scan != (char *)0 )
      {
        *Scan = '\0';
        if ( Scan[ 1 ] == 'm' ) Miss = 1;
      }

      String = TestGetName( String );
      BddNodeOut = searchbddcircuitout( (bddcircuit *)0, String );

      if ( BddNodeOut == (bddnode *)0 ) return;

      fprintf( stdout, "--> exist %s %d\n", String, Miss );

      if ( Miss )
      {
        BddNodeExist = existbddnodemissassoc( (bddsystem *)0, BddNodeOut, BddAssoc );
      }
      else
      {
        BddNodeExist = existbddnodeassoc( (bddsystem *)0, BddNodeOut, BddAssoc );
      }

      decbddrefext( BddNodeExist );

      TestViewNodeExpr( BddNodeExist );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Simp Off                  |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandSimpOff( String )

  char *String;
{
  bddnode *BddNodeOut;
  bddnode *BddNodeDc;
  char    *Scan;
  char    *NameOut;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;
    *Scan = '\0';

    NameOut = TestGetName( String );
    BddNodeOut = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut == (bddnode *)0 ) return;

    String = TestGetName( Scan + 1 );
    BddNodeDc = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeDc == (bddnode *)0 ) return;

    fprintf( stdout, "--> simp off %s with %s\n", NameOut, String );

    BddNodeOut = simpbddnodedcoff( (bddsystem *)0, BddNodeOut, BddNodeDc );

    decbddrefext( BddNodeOut );
    addbddcircuitout( (bddcircuit *)0, NameOut, BddNodeOut );

    TestViewNodeExpr( BddNodeOut );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Simp On                   |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandSimpOn( String )

  char *String;
{
  bddnode *BddNodeOut;
  bddnode *BddNodeDc;
  char    *Scan;
  char    *NameOut;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;
    *Scan = '\0';

    NameOut = TestGetName( String );
    BddNodeOut = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut == (bddnode *)0 ) return;

    String = TestGetName( Scan + 1 );
    BddNodeDc = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeDc == (bddnode *)0 ) return;

    fprintf( stdout, "--> simp on %s with %s\n", NameOut, String );

    BddNodeOut = simpbddnodedcon( (bddsystem *)0, BddNodeOut, BddNodeDc );

    decbddrefext( BddNodeOut );
    addbddcircuitout( (bddcircuit *)0, NameOut, BddNodeOut );

    TestViewNodeExpr( BddNodeOut );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Imply                     |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandImply( String )

  char *String;
{
  bddnode *BddNodeOut1;
  bddnode *BddNodeOut2;
  char    *Scan;
  char    *NameOut;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;
    *Scan = '\0';

    NameOut = TestGetName( String );
    BddNodeOut1 = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut1 == (bddnode *)0 ) return;

    String = TestGetName( Scan + 1 );
    BddNodeOut2 = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeOut2 == (bddnode *)0 ) return;

    fprintf( stdout, "--> imply %s and %s\n", NameOut, String );

    BddNodeOut1 = implybddnode( (bddsystem *)0, BddNodeOut1, BddNodeOut2 );
    decbddrefext( BddNodeOut1 );

    TestViewNodeExpr( BddNodeOut1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Intersect                 |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandIntersect( String )

  char *String;
{
  bddnode *BddNodeOut1;
  bddnode *BddNodeOut2;
  char    *Scan;
  char    *NameOut;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;
    *Scan = '\0';

    NameOut = TestGetName( String );
    BddNodeOut1 = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut1 == (bddnode *)0 ) return;

    String = TestGetName( Scan + 1 );
    BddNodeOut2 = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeOut2 == (bddnode *)0 ) return;

    fprintf( stdout, "--> intersect %s and %s\n", NameOut, String );

    BddNodeOut1 = intersectbddnode( (bddsystem *)0, BddNodeOut1, BddNodeOut2 );
    decbddrefext( BddNodeOut1 );

    TestViewNodeExpr( BddNodeOut1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Cofactor                  |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandCofactor( String )

  char *String;
{
  bddnode *BddNodeOut1;
  bddnode *BddNodeOut2;
  char    *Scan;
  char    *NameOut;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;
    *Scan = '\0';

    NameOut = TestGetName( String );
    BddNodeOut1 = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut1 == (bddnode *)0 ) return;

    String = TestGetName( Scan + 1 );
    BddNodeOut2 = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeOut2 == (bddnode *)0 ) return;

    fprintf( stdout, "--> cofactor %s by %s\n", NameOut, String );

    BddNodeOut1 = cofactorbddnode( (bddsystem *)0, BddNodeOut1, BddNodeOut2 );
    decbddrefext( BddNodeOut1 );

    TestViewNodeExpr( BddNodeOut1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Test Command Reduce                    |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandReduce( String )

  char *String;
{
  bddnode *BddNodeOut1;
  bddnode *BddNodeOut2;
  char    *Scan;
  char    *NameOut;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan == (char *)0 ) return;
    *Scan = '\0';

    NameOut = TestGetName( String );
    BddNodeOut1 = searchbddcircuitout( (bddcircuit *)0, NameOut );

    if ( BddNodeOut1 == (bddnode *)0 ) return;

    String = TestGetName( Scan + 1 );
    BddNodeOut2 = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNodeOut2 == (bddnode *)0 ) return;

    fprintf( stdout, "--> reduce %s by %s\n", NameOut, String );

    BddNodeOut1 = reducebddnode( (bddsystem *)0, BddNodeOut1, BddNodeOut2 );
    decbddrefext( BddNodeOut1 );

    TestViewNodeExpr( BddNodeOut1 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command Satisfy                    |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandSatisfy( String )

  char *String;
{
  bddnode    *BddNode;
  char       *Scan;
  bddnode    *BddSatisfy;
  int         Assoc;

  if ( String != (char *)0 )
  {
    Scan  = strchr( String, ' ' );
    Assoc = 0;

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
      if ( Scan[ 1 ] == 'a' ) Assoc = 1;
    }

    String = TestGetName( String );
    fprintf( stdout, "--> satisfy %s %d\n", String, Assoc );
    BddNode = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNode != (bddnode *)0 )
    {
      if ( Assoc )
      {
        if ( BddLocalSystem->ASSOC == (bddassoc *)0 ) return;
  
        BddSatisfy = satisfybddnodeassoc( (bddsystem *)0, BddNode,
                                          BddLocalSystem->ASSOC );
      }
      else
      {
        BddSatisfy = satisfybddnode( (bddsystem *)0, BddNode );
      }

      decbddrefext( BddSatisfy );

      TestViewNodeExpr( BddSatisfy );
    }
    else
    {
      fprintf( stderr, "Output %s doesn't exist\n", String );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Test Command Fraction                   |
|                                                             |
\------------------------------------------------------------*/

static void TestCommandFraction( String )

  char *String;
{
  bddnode    *BddNode;
  char       *Scan;
  double      Fraction;

  if ( String != (char *)0 )
  {
    Scan = strchr( String, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
    }

    String = TestGetName( String );
    fprintf( stdout, "--> fraction %s\n", String );
    BddNode = searchbddcircuitout( (bddcircuit *)0, String );

    if ( BddNode != (bddnode *)0 )
    {
      Fraction = fractionbddnode( (bddsystem *)0, BddNode );
      fprintf( stdout, "%f\n", Fraction );
    }
    else
    {
      fprintf( stderr, "Output %s doesn't exist\n", String );
    }
  }
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

  TestCommandViewSystem( (char *)0 );

  TestExit = 0;

  do
  {
    fprintf( stdout, "\ncommand > " );

    if ( ! fgets( TestBuffer, BDD_TEST_BUFFER_SIZE, stdin ) )
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
  while ( ( TestExit   == 0 ) &&
          ( TestLogout == 0 ) );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Test Bdd Circuit                   |
|                                                             |
\------------------------------------------------------------*/

int testbddcircuit( BddCircuit )

  bddcircuit *BddCircuit;
{
  setbddlocalcircuit( BddCircuit );

  if ( ! TestLogout )
  {
    return( TestMainLoop() );
  }

  return( 0 );
}
