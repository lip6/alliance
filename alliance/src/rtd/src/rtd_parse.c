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
| Tool    :                     RTD                           |
|                                                             |
| File    :                  rtd_parse.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.01.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "rtn.h"
# include "rtd.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "rtd_error.h"
# include "rtd_parse.h"

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

  static FILE  *RtlFile;
  static char   RtlBuffer[ 512 ];
  static long   RtlLine;

  static char  *RtlKeyFigure    = (char *)0;
  static char  *RtlKeyDeclar    = (char *)0;
  static char  *RtlKeyInit      = (char *)0;
  static char  *RtlKeySymbol    = (char *)0;
  static char  *RtlKeyAsg       = (char *)0;
  static char  *RtlKeyAsgData   = (char *)0;
  static char  *RtlKeyAsgAtom   = (char *)0;
  static char  *RtlKeyAsgBiVex  = (char *)0;
  static char  *RtlKeyBiVexCond = (char *)0;
  static char  *RtlKeyBiVexData = (char *)0;
  static char  *RtlKeyInstance  = (char *)0;
  static char  *RtlKeyInsMod    = (char *)0;
  static char  *RtlKeyMap       = (char *)0;
  static char  *RtlKeyMapFor    = (char *)0;
  static char  *RtlKeyMapAct    = (char *)0;
  static char  *RtlKeyModel     = (char *)0;
  static char  *RtlKeyPort      = (char *)0;
  static char  *RtlKeyGenMod    = (char *)0;
  static char  *RtlKeyGenMap    = (char *)0;
  static char  *RtlKeyGenExpr   = (char *)0;

  static char  *RtlScanString = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         RtlInitKeyword                      |
|                                                             |
\------------------------------------------------------------*/

static void RtlInitKeyword()
{
  if ( RtlKeyFigure == (char *)0 )
  {
    RtlKeyFigure    = namealloc( "FIGURE"     );
    RtlKeyDeclar    = namealloc( "DECLAR"     );
    RtlKeyInit      = namealloc( "INIT"       );
    RtlKeySymbol    = namealloc( "SYMBOL"     );
    RtlKeyAsg       = namealloc( "ASG"        );
    RtlKeyAsgData   = namealloc( "ASG_DATA"   );
    RtlKeyAsgAtom   = namealloc( "ASG_ATOM"   );
    RtlKeyAsgBiVex  = namealloc( "ASG_BIVEX"  );
    RtlKeyBiVexCond = namealloc( "BIVEX_COND" );
    RtlKeyBiVexData = namealloc( "BIVEX_DATA" );
    RtlKeyInstance  = namealloc( "INSTANCE"   );
    RtlKeyInsMod    = namealloc( "INS_MOD"    );
    RtlKeyMap       = namealloc( "MAP"        );
    RtlKeyMapFor    = namealloc( "MAP_FOR"    );
    RtlKeyMapAct    = namealloc( "MAP_ACT"    );
    RtlKeyModel     = namealloc( "MODEL"      );
    RtlKeyPort      = namealloc( "PORT"       );
    RtlKeyGenMap    = namealloc( "GEN_MAP"  );
    RtlKeyGenMod    = namealloc( "GEN_MOD"  );
    RtlKeyGenExpr   = namealloc( "GEN_EXPR" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl Read Name File                      |
|                                                             |
\------------------------------------------------------------*/

static char *RtlReadNameFile( Line )

  long Line;
{
  char *Name;
  char  SaveChar;

  while ( ( RtlScanString  == (char *)0 ) ||
          ( *RtlScanString == '\0'      ) ||
          ( *RtlScanString == '\n'      ) )
  {
    if ( fgets( RtlBuffer, 512, RtlFile ) == (char *)0 )
    {
      return( (char *)0 );
    }

    RtlScanString = RtlBuffer;
    RtlLine = RtlLine + 1;
  }

  if ( ( *RtlScanString == '~' ) ||
       ( *RtlScanString == '(' ) ||
       ( *RtlScanString == ')' ) )
  {
    return( RtlScanString );
  }

  Name = RtlScanString;

  while ( ( *RtlScanString != '~'  ) &&
          ( *RtlScanString != ')'  ) &&
          ( *RtlScanString != '\0' ) &&
          ( *RtlScanString != '\n' ) )
  {
    RtlScanString = RtlScanString + 1;
  }

  SaveChar = *RtlScanString;
  *RtlScanString = '\0';
  Name = namealloc( Name );
  *RtlScanString = SaveChar;

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl Read Prefix Expr                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *RtlReadPrefixExpr()
{
  vexexpr     *Expr;
  chain_list **PrevOper;
  chain_list  *ScanOper;
  vexexpr     *ExprStr;
  char        *Name;
  char        *Scan;
  long         Oper;
  long         Left;
  long         Right;
  long         Type;

  Scan = RtlReadNameFile();

  if ( ( Scan == (char *)0 ) ||
       ( *Scan == '~'      ) ||
       ( *Scan == ')'      ) )
  {
    return( (vexexpr *)0 );
  }

  if ( *RtlScanString == '(' )
  {
    RtlScanString = RtlScanString + 1;
    Name = RtlReadNameFile();

    if ( ( Name == (char *)0 ) ||
         ( Name[ 0 ] != '{'  ) )
    {
      return( (vexexpr *)0 );
    }

    Scan = strchr( Name, '}' );

    if ( Scan == (char *)0 )
    {
      return( (vexexpr *)0 );
    }

    sscanf( Name, "{%ld,%ld,%lx}", &Left, &Right, &Type );
    Name = Scan + 1;

    Oper = getvexoperbyname( Name );
    Scan = RtlReadNameFile();

    if ( Scan == (char *)0 )
    {
      return( (vexexpr *)0 );
    }

    if ( *RtlScanString == ')' )
    {
      if ( Oper == -1 ) 
      {
        Expr = createvexatomvec( Name, Left, Right );
        Expr->TYPE = Type;
      }
      else
      {
        return( (vexexpr *)0 );
      }
    }
    else
    if ( *RtlScanString == '~' )
    {
      RtlScanString = RtlScanString + 1;

      if ( Oper == -1 ) 
      {
        Expr = createvexfunc( Name, 0 );
      }
      else
      {
        Expr = createvexoper( Oper, 0 );
      }

      Expr->LEFT  = Left;
      Expr->RIGHT = Right;
      Expr->TYPE  = Type;

      if ( Left > Right ) Expr->WIDTH = ( Left  - Right ) + 1;
      else                Expr->WIDTH = ( Right - Left  ) + 1;

      PrevOper = &Expr->OPERAND;

      do
      {
        ExprStr = RtlReadPrefixExpr();

        if ( ExprStr == (vexexpr *)0 )
        {
          freevexexpr( Expr );
          return( (vexexpr *)0 );
        }

        ScanOper  = addchain( (chain_list *)0, (void *)ExprStr );
        *PrevOper = ScanOper;
        PrevOper  = &ScanOper->NEXT;
  
        Scan = RtlReadNameFile();

        if ( Scan == (char *)0 )
        {
          freevexexpr( Expr );
          return( (vexexpr *)0 );
        }

        if ( *RtlScanString == '~' )
        {
          RtlScanString = RtlScanString + 1;
        }
      }
      while ( *RtlScanString != ')');
    }
    else
    {
      return( (vexexpr *)0 );
    }

    if ( *RtlScanString == ')' ) 
    {
      RtlScanString = RtlScanString + 1;
    }
    else
    {
      freevexexpr( Expr );
      return( (vexexpr *)0 );
    }
  }
  else
  {
    Name = Scan;

    if ( ( Name == (char *)0 ) ||
         ( Name[ 0 ] != '{'  ) )
    {
      return( (vexexpr *)0 );
    }

    Scan = strchr( Name, '}' );

    if ( Scan == (char *)0 )
    {
      return( (vexexpr *)0 );
    }

    sscanf( Name, "{%ld,%ld,%lx}", &Left, &Right, &Type );
    Name = Scan + 1;

    Oper = getvexoperbyname( Name );

    if ( Oper == -1 )
    {
      Expr = createvexatomvec( Name, Left, Right );
      Expr->TYPE = Type;
    }
    else
    {
      return( (vexexpr *)0 );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                           RtlReadExpr                       |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *RtlReadExpr()
{
  RtlScanString = (char *)0;

  return( RtlReadPrefixExpr() );
}

/*------------------------------------------------------------\
|                                                             |
|                         RtlLoadFigure                       |
|                                                             |
\------------------------------------------------------------*/

static int RtlLoadFigure( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list  *RtlDeclar;
  rtlasg_list   *RtlAsg;
  rtlgen_list   *RtlGen;
  rtlbivex_list *RtlBiVex;
  rtlsym        *RtlSymbol;
  rtlmod_list   *RtlModel;
  rtlport_list  *RtlPort;
  rtlins_list   *RtlInst;
  rtlmap_list   *RtlMap;
  vexexpr       *Expr;
  vexexpr       *Atom;
  char          *ScanBuffer;
  char          *Keyword;
  char          *Name;
  int            Index;
  int            Length;
  int            Error;
  long           Value1;
  long           Value2;
  long           Value3;
  long           Value4;
  long           Value5;
  long           Value6;

  RtlInitKeyword();

  RtlDeclar = (rtldecl_list  *)0;
  RtlSymbol = (rtlsym        *)0;
  RtlAsg    = (rtlasg_list   *)0;
  RtlBiVex  = (rtlbivex_list *)0;

  RtlLine = 0;

  while ( fgets( RtlBuffer, 512, RtlFile ) != (char *)0 )
  {
    RtlLine = RtlLine + 1;

    if ( ( RtlBuffer[ 0 ] == '#'  ) ||
         ( RtlBuffer[ 0 ] == '\0' ) ||
         ( RtlBuffer[ 0 ] == '\n' ) ) continue;

    Length = strlen( RtlBuffer );
    if ( RtlBuffer[ Length - 1 ] == '\n' )
    {
      Length = Length - 1;
      RtlBuffer[ Length ] = '\0';
    }

    for ( Index = Length; Index >= 0; Index-- )
    {
      if ( RtlBuffer[ Index ] == '\t' ) break;
    }

    if ( Index < 0 ) return( 1 );
    Name = &RtlBuffer[ Index + 1 ];

    ScanBuffer = strchr( RtlBuffer, '\t' );
    if ( ScanBuffer == (char *)0 ) return( 1 );

    *ScanBuffer = '\0';
    Keyword     = namealloc( RtlBuffer );
    ScanBuffer  = ScanBuffer + 1;

    if ( Keyword == RtlKeyAsgBiVex )
    {
      Error = 2 - sscanf( ScanBuffer, "%ld\t%ld",
                          &Value1, &Value2 );

      RtlBiVex = addrtlasgbivex( RtlFigure, RtlAsg, (vexexpr *)0, (vexexpr *)0, Value1 );
      RtlBiVex->FLAGS = Value2;
    }
    else
    if ( Keyword == RtlKeyBiVexData )
    {
      Expr = RtlReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      RtlBiVex->VEX_DATA = Expr;
    }
    else
    if ( Keyword == RtlKeyBiVexCond )
    {
      Expr = RtlReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      RtlBiVex->VEX_COND = Expr;
    }
    else
    if ( Keyword == RtlKeyAsg )
    {
      Error = 3 - sscanf( ScanBuffer, "%ld\t%ld\t%ld",
                          &Value1, &Value2, &Value3 );

      if ( Error ) return( 1 );

      RtlAsg = addrtlasg( RtlFigure, (vexexpr *)0, Value1 );
      RtlAsg->REG_TYPE = Value2;
      RtlAsg->FLAGS    = Value3;
    }
    else
    if ( Keyword == RtlKeyAsgData   )
    {
      Expr = RtlReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      RtlAsg->VEX_DATA = Expr;
    }
    else
    if ( Keyword == RtlKeyAsgAtom   )
    {
      Atom = RtlReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      RtlAsg->VEX_ATOM = Atom;
    }
    else
    if ( Keyword == RtlKeyDeclar )
    {
      Error = 5 - sscanf( ScanBuffer, "%ld\t%ld\t%ld\t%ld\t%ld",
                          &Value1, &Value2, &Value3, &Value4, &Value5 );

      if ( Error ) return( 1 );

      Atom = RtlReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      RtlDeclar = addrtldecl( RtlFigure, Atom, Value2 );

      RtlDeclar->DIR   = Value1;
      RtlDeclar->TYPE  = Value2;
      RtlDeclar->KIND  = Value3;
      RtlDeclar->BASE  = Value4;
      RtlDeclar->FLAGS = Value5;
    }
    else
    if ( Keyword == RtlKeyInit )
    {
      Expr = RtlReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      RtlDeclar->VEX_INIT = Expr;
    }
    else
    if ( Keyword == RtlKeySymbol )
    {
      Error = 6 - sscanf( ScanBuffer, "%ld\t%ld\t%ld\t%ld\t%ld\t%ld",
        &Value1, &Value2, &Value3, &Value4, &Value5, &Value6 );

      if ( Error ) return( 1 );

      RtlSymbol = searchrtlsym( RtlFigure, Name, Value1 );

      RtlSymbol->INIT  = Value2;
      RtlSymbol->DRIVE = Value3;
      RtlSymbol->EFFEC = Value4;
      RtlSymbol->EVENT = Value5;
      RtlSymbol->FLAGS = Value6;
    }
    else
    if ( Keyword == RtlKeyInstance )
    {
      Error = 1 - sscanf( ScanBuffer, "%ld", &Value1 );

      if ( Error ) return( 1 );

      RtlInst = addrtlins( RtlFigure, Name, (rtlmod_list *)0 );

      RtlInst->FLAGS = Value1;
    }
    else
    if ( Keyword == RtlKeyInsMod )
    {
      RtlModel = searchrtlmod( RtlFigure, Name );

      if ( RtlModel == (rtlmod_list *)0 ) return( 1 );

      RtlInst->MODEL = RtlModel;
    }
    else
    if ( Keyword == RtlKeyMap )
    {
      Error = 1 - sscanf( ScanBuffer, "%ld", &Value1 );

      if ( Error ) return( 1 );

      RtlMap = addrtlinsmap( RtlFigure, RtlInst, (vexexpr *)0, (vexexpr *)0 );

      RtlMap->FLAGS = Value1;
    }
    else
    if ( Keyword == RtlKeyMapFor )
    {
      Atom = RtlReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      RtlMap->VEX_FORMAL = Atom;
    }
    else
    if ( Keyword == RtlKeyMapAct )
    {
      Atom = RtlReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      RtlMap->VEX_ACTUAL = Atom;
    }
    else
    if ( Keyword == RtlKeyModel )
    {
      Error = 1 - sscanf( ScanBuffer, "%ld", &Value1 );

      if ( Error ) return( 1 );

      RtlModel = addrtlmod( RtlFigure, Name );

      RtlModel->FLAGS = Value1;
    }
    else
    if ( Keyword == RtlKeyPort )
    {
      Error = 3 - sscanf( ScanBuffer, "%ld\t%ld\t%ld", &Value1, &Value2, &Value3 );

      if ( Error ) return( 1 );

      Atom = RtlReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      RtlPort = addrtlmodport( RtlFigure, RtlModel, Atom, Value2, Value1 );
      RtlPort->FLAGS = Value3;
    }
    else
    if ( Keyword == RtlKeyFigure )
    {
      Error = 1 - sscanf( ScanBuffer, "%ld",
                          &Value1 );

      if ( Error ) return( 1 );

      RtlFigure->NAME  = namealloc( Name );
      RtlFigure->FLAGS = Value1;
    }
    else
    if ( Keyword == RtlKeyGenMap )
    {
      Error = 2 - sscanf( ScanBuffer, "%ld\t%ld", &Value1, &Value2 );

      if ( Error ) return( 1 );

      Atom = RtlReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      RtlGen = addrtlinsgen( RtlFigure, RtlInst, Atom, (vexexpr *)0 );

      RtlGen->BASE  = Value1;
      RtlGen->FLAGS = Value2;
    }
    else
    if ( Keyword == RtlKeyGenMod )
    {
      Error = 2 - sscanf( ScanBuffer, "%ld\t%ld", &Value1, &Value2 );

      if ( Error ) return( 1 );

      Atom = RtlReadExpr();

      if ( Atom == (vexexpr *)0 ) return( 1 );

      RtlGen = addrtlmodgen( RtlFigure, RtlModel, Atom, (vexexpr *)0, Value1 );

      RtlGen->FLAGS = Value2;
    }
    else
    if ( Keyword == RtlKeyGenExpr )
    {
      Expr = RtlReadExpr();

      if ( Expr == (vexexpr *)0 ) return( 1 );

      RtlGen->VEX_EXPR = Expr;
    }
    else return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        loadrtlfig                           |
|                                                             |
\------------------------------------------------------------*/

void loadrtlfig( RtlFigure, FigureName )

  rtlfig_list *RtlFigure;
  char        *FigureName;
{
  int Value;

  RtlFile = mbkfopen( FigureName, "rtl", "r" );

  if ( RtlFile == (FILE *)0 )
  {
    rtderror( RTD_ERROR_OPEN_FILE, FigureName, 0 );
  }

  Value = RtlLoadFigure( RtlFigure );

  if ( Value ) rtderror( RTD_ERROR_PARSE_FILE, FigureName, RtlLine );

  fclose( RtlFile );
}
