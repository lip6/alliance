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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_preanal.c                   |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                              Seb Leon                       |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vbh.h"
# include "vbl.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_shared.h"
# include "vasy_preanal.h"
# include "vasy_debug.h"
# include "vasy_error.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/

  static authtable *VasyHashName = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       VasyPreAnalysisVexExpr                |
|                                                             |
\------------------------------------------------------------*/

static int VasyPreAnalysisVexExpr( Expr )

   vexexpr *Expr;
{
  chain_list *ScanOper;
  authelem   *Element;
  char       *Name;
  int         FuncId;
  int         Index;
  char        Literal;

  if ( IsVexNodeAtom( Expr ) )
  {
    Name = GetVexAtomValue( Expr );

    if ( ! IsVexAtomLiteral( Expr ) )
    {
      Element = searchauthelem( VasyHashName, Name );

      if ( Element != (authelem *)0 ) return( 1 );
    }
    else
    {
      for ( Index = 0; Index < Expr->WIDTH; Index++ )
      {
        Literal = Name[ Index + 1 ];

        if ( ( Literal == VEX_TRISTATE  ) ||
             ( Literal == VEX_WEAK_ZERO ) ||
             ( Literal == VEX_WEAK_ONE  ) ) return( 1 );
      }
    }
  }
  else
  {
    if ( IsVexNodeFunc( Expr ) )
    {
      Name   = GetVexFuncValue( Expr );
      FuncId = getvexstdfuncid( Name );

      if ( FuncId == -1 ) return( 1 );
    }

    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      if ( VasyPreAnalysisVexExpr( GetVexOperand( ScanOper ) ) )
      {
        return( 1 );
      }
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyPreAnalysisVpnProc                |
|                                                             |
\------------------------------------------------------------*/

static void VasyPreAnalysisVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAct;
  vexexpr       *VexAtom;
  char          *Name;
  int            ToAnalyse;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyPreAnalysisVpnProc %s\n", VpnProc->NAME );
  }

  if ( ( IsVpnProcSequential( VpnProc ) ) ||
       ( IsVpnProcGuarded( VpnProc    ) ) )
  {
    SetVasyVpnProcToAnalyse( VpnProc );
  }
  else
  {
    if ( VasyHashName == (authtable *)0 )
    {
      VasyHashName  = createauthtable( 50 );
    }

    for ( VpnTrans  = VpnProc->TRANS;
          VpnTrans != (vpntrans_list *)0;
          VpnTrans  = VpnTrans->NEXT )
    {
      for ( VpnAct  = VpnTrans->ACT;
            VpnAct != (vpnact_list *)0;
            VpnAct  = VpnAct->NEXT )
      {
        if ( ( VpnAct->TYPE == VPN_ACT_ASG_PORT   ) ||
             ( VpnAct->TYPE == VPN_ACT_ASG_SIGNAL ) )
        {
          VexAtom = VpnAct->VEX_ATOM;
          Name    = GetVexAtomValue( VexAtom );
          addauthelem( VasyHashName, Name, 0 );
        }
      }
    }

    ToAnalyse = 0;

    for ( VpnTrans  = VpnProc->TRANS;
          VpnTrans != (vpntrans_list *)0;
          VpnTrans  = VpnTrans->NEXT )
    {
      if ( ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT ) ||
           ( VpnTrans->TYPE == VPN_TRANS_SUP_WAIT ) ) continue;

      if ( ( VpnTrans->VEX_GUARD != (vexexpr *)0           ) &&
           ( VasyPreAnalysisVexExpr( VpnTrans->VEX_GUARD ) ) )
      {
        ToAnalyse = 1;
      }
      else
      {
        for ( VpnAct  = VpnTrans->ACT;
              VpnAct != (vpnact_list *)0;
              VpnAct  = VpnAct->NEXT )
        {
          if ( VasyPreAnalysisVexExpr( VpnAct->VEX_EXPR ) )
          {
            ToAnalyse = 1; break;
          }
        }
      }

      if ( ToAnalyse )
      {
        SetVasyVpnProcToAnalyse( VpnProc ); break;
      }
    }

    resetauthtable( VasyHashName );
  }

  if ( IsVasyDebugLevel0() )
  {
    if ( IsVasyVpnProcToAnalyse( VpnProc ) )
    {
      VasyPrintf( stdout, "  +++ Should Analyse Process %s\n", VpnProc->NAME );
    }

    VasyPrintf( stdout, "  <-- VasyPreAnalysisVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyPreAnalysisVpnFig                 |
|                                                             |
\------------------------------------------------------------*/

void VasyPreAnalysisVpnFig( VpnFigure )

 vpnfig_list *VpnFigure;
{
  vpnproc_list *VpnProc;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyPreAnalysisVpnFig %s\n", VpnFigure->NAME );
  }

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    VasyPreAnalysisVpnProc( VpnFigure, VpnProc );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyPreAnalysisVpnFig %s\n\n", VpnFigure->NAME );
  }
}
