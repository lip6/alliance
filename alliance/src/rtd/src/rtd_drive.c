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
| File    :                   rtd_drive.c                     |
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "rtd_error.h"
# include "rtd_drive.h"

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

  static FILE *RtlFile;
  static long  RtlExprLength;
  static char  RtlBuffer[ 128 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     RtlWriteNameFile                        |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteNameFile( Name )

  char *Name;
{
  int Length;

  Length = strlen( Name );

  RtlExprLength += Length;

  if ( RtlExprLength > 80 )
  {
    fprintf( RtlFile, "\n" );
    RtlExprLength = Length;
  }

  fprintf( RtlFile, Name );
}

/*------------------------------------------------------------\
|                                                             |
|                     RtlWriteExprPrefix                      |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteExprPrefix( Expr )

  vexexpr *Expr;
{
  char       *Name;
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    sprintf( RtlBuffer, "{%d,%d,%x}%s", 
             Expr->LEFT, Expr->RIGHT, Expr->TYPE, GetVexAtomValue( Expr ) );
    RtlWriteNameFile( RtlBuffer );
  }
  else
  {
    if ( IsVexNodeOper( Expr ) )
    {
      Name = getvexoperuppername( GetVexOperValue( Expr ) );
    }
    else
    {
      Name = GetVexFuncValue( Expr );
    }

    sprintf( RtlBuffer, "({%d,%d,%x}%s", 
             Expr->LEFT, Expr->RIGHT, Expr->TYPE, Name );
    RtlWriteNameFile( RtlBuffer );

    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      RtlWriteNameFile( "~" );
      RtlWriteExprPrefix( GetVexOperand( ScanOper ) );
    }

    RtlWriteNameFile( ")" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteExpr                         |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteExpr( Label, Expr )

  char    *Label;
  vexexpr *Expr;
{
  if ( Expr != (vexexpr *)0 )
  {
    RtlExprLength = 0;

    fprintf( RtlFile, "%s\t\n", Label );
    RtlWriteExprPrefix( Expr );
    fprintf( RtlFile, "\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      RtlWriteOneSymbol                      |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteOneSymbol( RtlSymbol )

  rtlsym *RtlSymbol;
{
  fprintf( RtlFile, "SYMBOL\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\n", 
           (long)RtlSymbol->INDEX,
           (long)RtlSymbol->INIT,
           (long)RtlSymbol->DRIVE,
           (long)RtlSymbol->EFFEC,
           (long)RtlSymbol->EVENT,
           (long)RtlSymbol->FLAGS,
           RtlSymbol->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteOneDeclar                    |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteOneDeclar( RtlDeclar )

  rtldecl_list *RtlDeclar;
{
  int Width;
  int Scan;

  fprintf( RtlFile, "DECLAR\t%ld\t%ld\t%ld\t%ld\t%ld", 
           (long)RtlDeclar->DIR,
           (long)RtlDeclar->TYPE,
           (long)RtlDeclar->KIND,
           (long)RtlDeclar->BASE,
           (long)RtlDeclar->FLAGS );

  RtlWriteExpr( "", RtlDeclar->VEX_ATOM );
  RtlWriteExpr( "INIT", RtlDeclar->VEX_INIT );

  Width = RtlDeclar->VEX_ATOM->WIDTH;

  for ( Scan = 0; Scan < Width; Scan++ )
  {
    RtlWriteOneSymbol( &RtlDeclar->DECL_SYM[ Scan ] );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteDeclar                       |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteDeclar( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtldecl_list *ScanDeclar;
  int           Type;

  for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
  {
    for ( ScanDeclar  = RtlFigure->DECLAR[ Type ];
          ScanDeclar != (rtldecl_list *)0;
          ScanDeclar  = ScanDeclar->NEXT )
    {
      RtlWriteOneDeclar( ScanDeclar);
    }
  }
}


/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteAssign                       |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteAssign( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlasg_list   *ScanAsg;
  rtlbivex_list *ScanBiVex;

  for ( ScanAsg  = RtlFigure->ASSIGN;
        ScanAsg != (rtlasg_list *)0;
        ScanAsg  = ScanAsg->NEXT )
  {
    fprintf( RtlFile, "ASG\t%ld\t%ld\t%ld\n", 
             (long)ScanAsg->TYPE,
             (long)ScanAsg->REG_TYPE,
             (long)ScanAsg->FLAGS );

    RtlWriteExpr( "ASG_ATOM", ScanAsg->VEX_ATOM );
    RtlWriteExpr( "ASG_DATA", ScanAsg->VEX_DATA );

    for ( ScanBiVex  = ScanAsg->BIVEX;
          ScanBiVex != (rtlbivex_list *)0;
          ScanBiVex  = ScanBiVex->NEXT )
    {
      fprintf( RtlFile, "ASG_BIVEX\t%ld\t%ld\n",
               (long)ScanBiVex->TYPE,
               (long)ScanBiVex->FLAGS );

      RtlWriteExpr( "BIVEX_COND", ScanBiVex->VEX_COND );
      RtlWriteExpr( "BIVEX_DATA", ScanBiVex->VEX_DATA );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteFsm                          |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteFsm( RtlFigure )

  rtlfig_list *RtlFigure;
{
  /* TO BE DONE */
}

/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteModel                        |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteModel( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlmod_list  *ScanMod;
  rtlport_list *ScanPort;
  rtlgen_list  *ScanGen;

  for ( ScanMod  = RtlFigure->MODEL;
        ScanMod != (rtlmod_list *)0;
        ScanMod  = ScanMod->NEXT )
  {
    fprintf( RtlFile, "MODEL\t%ld\t%s\n", 
             (long)ScanMod->FLAGS,
             ScanMod->NAME );

    for ( ScanPort  = ScanMod->PORT;
          ScanPort != (rtlport_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      fprintf( RtlFile, "PORT\t%ld\t%ld\t%ld", 
               (long)ScanPort->DIR,
               (long)ScanPort->BASE,
               (long)ScanPort->FLAGS );

      RtlWriteExpr( "", ScanPort->VEX_ATOM );
    }

    for ( ScanGen  = ScanMod->GENERIC;
          ScanGen != (rtlgen_list *)0;
          ScanGen  = ScanGen->NEXT )
    {
      fprintf( RtlFile, "GEN_MOD\t%ld\t%ld\t",
               (long)ScanGen->BASE,
               (long)ScanGen->FLAGS );

      RtlWriteExpr( "", ScanGen->VEX_ATOM );

      RtlWriteExpr( "GEN_EXPR", ScanGen->VEX_EXPR );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteInstance                     |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteInstance( RtlFigure )

  rtlfig_list *RtlFigure;
{
  rtlins_list *ScanIns;
  rtlmap_list *ScanMap;
  rtlgen_list *ScanGen;

  for ( ScanIns  = RtlFigure->INSTANCE;
        ScanIns != (rtlins_list *)0;
        ScanIns  = ScanIns->NEXT )
  {
    fprintf( RtlFile, "INSTANCE\t%ld\t%s\n", 
             (long)ScanIns->FLAGS,
             ScanIns->NAME );

    fprintf( RtlFile, "INS_MOD\t%s\n", ScanIns->MODEL->NAME );

    for ( ScanMap  = ScanIns->MAP;
          ScanMap != (rtlmap_list *)0;
          ScanMap  = ScanMap->NEXT )
    {
      fprintf( RtlFile, "MAP\t%ld\n", (long)ScanMap->FLAGS );

      RtlWriteExpr( "MAP_FOR", ScanMap->VEX_FORMAL );
      RtlWriteExpr( "MAP_ACT", ScanMap->VEX_ACTUAL );
    }

    for ( ScanGen  = ScanIns->GENERIC;
          ScanGen != (rtlgen_list *)0;
          ScanGen  = ScanGen->NEXT )
    {
      fprintf( RtlFile, "GEN_MAP\t%ld\t%ld\t",
               (long)ScanGen->BASE,
               (long)ScanGen->FLAGS );

      RtlWriteExpr( "", ScanGen->VEX_ATOM );

      RtlWriteExpr( "GEN_EXPR", ScanGen->VEX_EXPR );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        RtlWriteFigure                       |
|                                                             |
\------------------------------------------------------------*/

static void RtlWriteFigure( RtlFigure )

  rtlfig_list *RtlFigure;
{
  fprintf( RtlFile, "FIGURE\t%ld\t%s\n",
           (long)RtlFigure->FLAGS,
           RtlFigure->NAME );

  RtlWriteDeclar( RtlFigure );
  RtlWriteAssign( RtlFigure );
  RtlWriteFsm( RtlFigure );
  RtlWriteModel( RtlFigure );
  RtlWriteInstance( RtlFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         savertlfig                          |
|                                                             |
\------------------------------------------------------------*/

void savertlfig( RtlFigure )

  rtlfig_list *RtlFigure;
{
  RtlFile = mbkfopen( RtlFigure->NAME, "rtl", "w" );

  if ( RtlFile == (FILE *)0 )
  {
    rtderror( RTD_ERROR_OPEN_FILE, RtlFigure->NAME, 0 );
  }

  RtlWriteFigure( RtlFigure );

  fclose( RtlFile );
}
