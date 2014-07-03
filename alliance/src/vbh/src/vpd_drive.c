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
| Tool    :                     VPD                           |
|                                                             |
| File    :                   vpd_drive.c                     |
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
# include "vpn.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "vpd_error.h"
# include "vpd_drive.h"

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

  static FILE *VpnFile;
  static long  VpnExprLength;
  static char  VpnBuffer[ 128 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     VpnWriteNameFile                        |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteNameFile( Name )

  char *Name;
{
  int Length;

  Length = strlen( Name );

  VpnExprLength += Length;

  if ( VpnExprLength > 80 )
  {
    fprintf( VpnFile, "\n" );
    VpnExprLength = Length;
  }

  fprintf( VpnFile, "%s", Name );
}

/*------------------------------------------------------------\
|                                                             |
|                     VpnWriteExprPrefix                      |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteExprPrefix( Expr )

  vexexpr *Expr;
{
  char       *Name;
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    sprintf( VpnBuffer, "{%d,%d,%x}%s", 
             Expr->LEFT, Expr->RIGHT, Expr->TYPE, GetVexAtomValue( Expr ) );
    VpnWriteNameFile( VpnBuffer );
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

    sprintf( VpnBuffer, "({%d,%d,%x}%s", 
             Expr->LEFT, Expr->RIGHT, Expr->TYPE, Name );
    VpnWriteNameFile( VpnBuffer );

    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VpnWriteNameFile( "~" );
      VpnWriteExprPrefix( GetVexOperand( ScanOper ) );
    }

    VpnWriteNameFile( ")" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteExpr                         |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteExpr( Label, Expr )

  char    *Label;
  vexexpr *Expr;
{
  if ( Expr != (vexexpr *)0 )
  {
    VpnExprLength = 0;

    fprintf( VpnFile, "%s\t\n", Label );
    VpnWriteExprPrefix( Expr );
    fprintf( VpnFile, "\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteLine                         |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteLine( Line )

  vpnline_list *Line;
{
  while ( Line != (vpnline_list *)0 )
  {
    if ( Line->NAME == (char *)0 )
    {
      fprintf( VpnFile, "LINE\t%ld\t\n", Line->LINE );
    }
    else
    {
      fprintf( VpnFile, "LINE\t%ld\t%s\n", Line->LINE, Line->NAME );
    }

    Line = Line->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VpnWriteOneSymbol                      |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteOneSymbol( VpnSymbol )

  vpnsym *VpnSymbol;
{
  fprintf( VpnFile, "SYMBOL\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\n", 
           (long)VpnSymbol->INDEX,
           (long)VpnSymbol->INIT,
           (long)VpnSymbol->DRIVE,
           (long)VpnSymbol->EFFEC,
           (long)VpnSymbol->EVENT,
           (long)VpnSymbol->FLAGS,
           VpnSymbol->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteOneDeclar                    |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteOneDeclar( VpnDeclar )

  vpndecl_list *VpnDeclar;
{
  int Width;
  int Scan;

  fprintf( VpnFile, "DECLAR\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld", 
           (long)VpnDeclar->DIR,
           (long)VpnDeclar->TYPE,
           (long)VpnDeclar->KIND,
           (long)VpnDeclar->CLASS,
           (long)VpnDeclar->BASE,
           (long)VpnDeclar->FLAGS );

  VpnWriteExpr( "", VpnDeclar->VEX_ATOM );
  VpnWriteExpr( "INIT", VpnDeclar->VEX_INIT );
  VpnWriteLine( VpnDeclar->LINE );

  Width = VpnDeclar->VEX_ATOM->WIDTH;

  for ( Scan = 0; Scan < Width; Scan++ )
  {
    VpnWriteOneSymbol( &VpnDeclar->DECL_SYM[ Scan ] );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteDeclar                       |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteDeclar( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpndecl_list *ScanDeclar;
  chain_list   *ScanChain;
  int           Type;

  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
  {
    if ( ( Type != VPN_DECLAR_PORT     ) &&
         ( Type != VPN_DECLAR_CONSTANT ) &&
         ( Type != VPN_DECLAR_SIGNAL   ) ) continue;

    for ( ScanDeclar  = VpnFigure->DECLAR[ Type ];
          ScanDeclar != (vpndecl_list *)0;
          ScanDeclar  = ScanDeclar->NEXT )
    {
      VpnWriteOneDeclar( ScanDeclar);
    }
  }

  for ( ScanChain  = VpnFigure->VAR_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanDeclar = (vpndecl_list *)ScanChain->DATA;
    VpnWriteOneDeclar( ScanDeclar);
  }

  for ( ScanChain  = VpnFigure->DEF_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanDeclar = (vpndecl_list *)ScanChain->DATA;
    VpnWriteOneDeclar( ScanDeclar);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteAttribute                    |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteAttribute( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnattr_list *ScanAttr;

  for ( ScanAttr  = VpnFigure->ATTRIBUTE;
        ScanAttr != (vpnattr_list *)0;
        ScanAttr  = ScanAttr->NEXT )
  {
    fprintf( VpnFile, "ATTR\t%ld\t%ld\t%s\n", 
             (long)ScanAttr->TYPE,
             (long)ScanAttr->FLAGS,
             ScanAttr->NAME );

    fprintf( VpnFile, "ATTR_ENT\t%s\t%s\n", 
             ScanAttr->VALUE,
             ScanAttr->ENTITY );

    VpnWriteLine( ScanAttr->LINE );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWritePlace                        |
|                                                             |
\------------------------------------------------------------*/

static void VpnWritePlace( VpnPlace )

  vpnplace_list *VpnPlace;
{
  vpnplace_list *ScanPlace;

  for ( ScanPlace  = VpnPlace;
        ScanPlace != (vpnplace_list *)0;
        ScanPlace  = ScanPlace->NEXT )
  {
    fprintf( VpnFile, "PLACE\t%ld\t%ld\t%ld\t%ld\t%s\n", 
             (long)ScanPlace->FATHER,
             (long)ScanPlace->TYPE,
             (long)ScanPlace->TOKEN,
             (long)ScanPlace->FLAGS,
             ScanPlace->NAME );

    if ( ScanPlace->LINK != (vpnplace_list *)0 )
    {
      fprintf( VpnFile, "LINK\t%s\n", ScanPlace->LINK->NAME );
    }

    VpnWriteLine( ScanPlace->LINE );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteTrans                        |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteTrans( VpnTrans )

  vpntrans_list *VpnTrans;
{
  vpntrans_list *ScanTrans;
  vpnplace_list *ScanPlace;
  vpnact_list   *ScanAct;
  vpnsym        *ScanSymbol;
  vpnarc        *ScanArc;
  chain_list    *ScanChain;

  for ( ScanTrans  = VpnTrans;
        ScanTrans != (vpntrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    fprintf( VpnFile, "TRANS\t%ld\t%ld\t%ld\t%s\n", 
             (long)ScanTrans->FATHER,
             (long)ScanTrans->TYPE,
             (long)ScanTrans->FLAGS,
             ScanTrans->NAME );

    VpnWriteExpr( "GUARD", ScanTrans->VEX_GUARD );

    for ( ScanChain  = ScanTrans->WAIT_SYM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanSymbol = (vpnsym *)ScanChain->DATA;

      fprintf( VpnFile, "WAIT\t%ld\t%s\n",
               (long)ScanSymbol->INDEX,
               ScanSymbol->NAME );
    }

    for ( ScanAct  = ScanTrans->ACT;
          ScanAct != (vpnact_list *)0;
          ScanAct  = ScanAct->NEXT )
    {
      fprintf( VpnFile, "ACT\t%ld\t%ld\n", 
               (long)ScanAct->TYPE,
               (long)ScanAct->FLAGS );

      VpnWriteExpr( "ACT_ATOM", ScanAct->VEX_ATOM );
      VpnWriteExpr( "ACT_EXPR", ScanAct->VEX_EXPR );

      VpnWriteLine( ScanAct->LINE );
    }

    for ( ScanChain  = ScanTrans->PLACE_OUT;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc   = (vpnarc        *)ScanChain->DATA;
      ScanPlace = (vpnplace_list *)ScanArc->TARGET;

      fprintf( VpnFile, "ARC_OUT\t%ld\t%ld\t%s\n",
               (long)ScanArc->TYPE,
               (long)ScanArc->FLAGS,
               ScanPlace->NAME );
    }

    for ( ScanChain  = ScanTrans->PLACE_IN;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc   = (vpnarc        *)ScanChain->DATA;
      ScanPlace = (vpnplace_list *)ScanArc->SOURCE;

      fprintf( VpnFile, "ARC_IN\t%ld\t%ld\t%s\n",
               (long)ScanArc->TYPE,
               (long)ScanArc->FLAGS,
               ScanPlace->NAME );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteProcess                      |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteProcess( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnproc_list *ScanProc;
  vpndecl_list *ScanDeclar;
  chain_list   *ScanChain;

  for ( ScanProc  = VpnFigure->PROCESS;
        ScanProc != (vpnproc_list *)0;
        ScanProc  = ScanProc->NEXT )
  {
    fprintf( VpnFile, "PROCESS\t%ld\t%ld\t%s\n", 
             (long)ScanProc->TYPE,
             (long)ScanProc->FLAGS,
             ScanProc->NAME );

    VpnWriteLine( ScanProc->LINE );

    for ( ScanChain  = ScanProc->VAR_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanDeclar = (vpndecl_list *)ScanChain->DATA;
      VpnWriteOneDeclar( ScanDeclar );
    }

    for ( ScanChain  = ScanProc->DEF_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanDeclar = (vpndecl_list *)ScanChain->DATA;
      VpnWriteOneDeclar( ScanDeclar );
    }

    VpnWritePlace( ScanProc->PLACE );
    VpnWriteTrans( ScanProc->TRANS );

    if ( ScanProc->FIRST != (vpntrans_list *)0 )
    {
      fprintf( VpnFile, "FIRST\t%s\n", ScanProc->FIRST->NAME );
    }

    if ( ScanProc->ELABO != (vpntrans_list *)0 )
    {
      fprintf( VpnFile, "ELABO\t%s\n", ScanProc->ELABO->NAME );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteFunction                     |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteFunction( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnfunc_list *ScanFunc;
  vpndecl_list *ScanDeclar;
  chain_list   *ScanChain;

  for ( ScanFunc  = VpnFigure->FUNCTION;
        ScanFunc != (vpnfunc_list *)0;
        ScanFunc  = ScanFunc->NEXT )
  {
    fprintf( VpnFile, "FUNCTION\t%ld\t%ld\t%s\n", 
             (long)ScanFunc->TYPE,
             (long)ScanFunc->FLAGS,
             ScanFunc->NAME );

    if ( ScanFunc->VEX_RET != (vexexpr *)0 )
    {
      VpnWriteExpr( "RET", ScanFunc->VEX_RET );
    }

    VpnWriteLine( ScanFunc->LINE );

    for ( ScanChain  = ScanFunc->ARG_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanDeclar = (vpndecl_list *)ScanChain->DATA;
      VpnWriteOneDeclar( ScanDeclar );
    }

    for ( ScanChain  = ScanFunc->VAR_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanDeclar = (vpndecl_list *)ScanChain->DATA;
      VpnWriteOneDeclar( ScanDeclar );
    }

    for ( ScanChain  = ScanFunc->DEF_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanDeclar = (vpndecl_list *)ScanChain->DATA;
      VpnWriteOneDeclar( ScanDeclar );
    }

    VpnWritePlace( ScanFunc->PLACE );
    VpnWriteTrans( ScanFunc->TRANS );

    if ( ScanFunc->FIRST != (vpntrans_list *)0 )
    {
      fprintf( VpnFile, "FIRST\t%s\n", ScanFunc->FIRST->NAME );
    }

    if ( ScanFunc->ELABO != (vpntrans_list *)0 )
    {
      fprintf( VpnFile, "ELABO\t%s\n", ScanFunc->ELABO->NAME );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteModel                        |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteModel( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnmod_list  *ScanMod;
  vpnport_list *ScanPort;
  vpngen_list  *ScanGen;

  for ( ScanMod  = VpnFigure->MODEL;
        ScanMod != (vpnmod_list *)0;
        ScanMod  = ScanMod->NEXT )
  {
    fprintf( VpnFile, "MODEL\t%ld\t%s\n", 
             (long)ScanMod->FLAGS,
             ScanMod->NAME );

    VpnWriteLine( ScanMod->LINE );

    for ( ScanPort  = ScanMod->PORT;
          ScanPort != (vpnport_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      fprintf( VpnFile, "PORT\t%ld\t%ld\t%ld", 
               (long)ScanPort->DIR,
               (long)ScanPort->BASE,
               (long)ScanPort->FLAGS );

      VpnWriteExpr( "", ScanPort->VEX_ATOM );
      VpnWriteLine( ScanPort->LINE );
    }

    for ( ScanGen  = ScanMod->GENERIC;
          ScanGen != (vpngen_list *)0;
          ScanGen  = ScanGen->NEXT )
    {
      fprintf( VpnFile, "GEN_MOD\t%ld\t%ld\t",
               (long)ScanGen->BASE,
               (long)ScanGen->FLAGS );

      VpnWriteExpr( "", ScanGen->VEX_ATOM );

      VpnWriteExpr( "GEN_EXPR", ScanGen->VEX_EXPR );

      VpnWriteLine( ScanGen->LINE );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteInstance                     |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteInstance( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnins_list *ScanIns;
  vpnmap_list *ScanMap;
  vpngen_list *ScanGen;

  for ( ScanIns  = VpnFigure->INSTANCE;
        ScanIns != (vpnins_list *)0;
        ScanIns  = ScanIns->NEXT )
  {
    fprintf( VpnFile, "INSTANCE\t%ld\t%s\n", 
             (long)ScanIns->FLAGS,
             ScanIns->NAME );

    fprintf( VpnFile, "INS_MOD\t%s\n", ScanIns->MODEL->NAME );

    VpnWriteLine( ScanIns->LINE );

    for ( ScanMap  = ScanIns->MAP;
          ScanMap != (vpnmap_list *)0;
          ScanMap  = ScanMap->NEXT )
    {
      fprintf( VpnFile, "MAP\t%ld\n", (long)ScanMap->FLAGS );

      VpnWriteExpr( "MAP_FOR", ScanMap->VEX_FORMAL );
      VpnWriteExpr( "MAP_ACT", ScanMap->VEX_ACTUAL );

      VpnWriteLine( ScanMap->LINE );
    }

    for ( ScanGen  = ScanIns->GENERIC;
          ScanGen != (vpngen_list *)0;
          ScanGen  = ScanGen->NEXT )
    {
      fprintf( VpnFile, "GEN_MAP\t%ld\t%ld\t",
               (long)ScanGen->BASE,
               (long)ScanGen->FLAGS );

      VpnWriteExpr( "", ScanGen->VEX_ATOM );

      VpnWriteExpr( "GEN_EXPR", ScanGen->VEX_EXPR );

      VpnWriteLine( ScanGen->LINE );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VpnWriteFigure                       |
|                                                             |
\------------------------------------------------------------*/

static void VpnWriteFigure( VpnFigure )

  vpnfig_list *VpnFigure;
{
  fprintf( VpnFile, "FIGURE\t%ld\t%s\n",
           (long)VpnFigure->FLAGS,
           VpnFigure->NAME );

  VpnWriteDeclar( VpnFigure );
  VpnWriteProcess( VpnFigure );
  VpnWriteFunction( VpnFigure );
  VpnWriteAttribute( VpnFigure );
  VpnWriteModel( VpnFigure );
  VpnWriteInstance( VpnFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         vpnsavevpnfig                       |
|                                                             |
\------------------------------------------------------------*/

void vpnsavevpnfig( VpnFigure )

  vpnfig_list *VpnFigure;
{
  VpnFile = mbkfopen( VpnFigure->NAME, "vpn", "w" );

  if ( VpnFile == (FILE *)0 )
  {
    vpderror( VPD_ERROR_OPEN_FILE, VpnFigure->NAME, 0 );
  }

  VpnWriteFigure( VpnFigure );

  fclose( VpnFile );
}
