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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                    VASY                           |
|                                                             |
| File    :                 vasy_redwait.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   25.08.97                        |
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
# include "vex.h"
# include "bdd.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_debug.h"
# include "vasy_error.h"
# include "vasy_shared.h"
# include "vasy_redact.h"
# include "vasy_redinst.h"
# include "vasy_redwait.h"

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

  static vpnfig_list   *VasyFigure    = (vpnfig_list   *)0;
  static vpnproc_list  *VasyProcess   = (vpnproc_list  *)0;

  static vasystack_list *VasyStackHead      = (vasystack_list *)0;
  static vasystack_list *VasyStackQueue     = (vasystack_list *)0;
  static authtable      *VasyHashStackTrans = (authtable *)0;
  static long           VasyStackSize       = 0;
  static long           VasyNumberTrans     = 0;

  static authtable     *VasyHashAssign  = (authtable *)0;
  static authtable     *VasyHashRenDef  = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitAddVpnTrans                      |
|                                                             |
\------------------------------------------------------------*/

static vpntrans_list *VasyRedWaitAddVpnTrans()
{
  vpntrans_list *VpnTrans;
  char           Buffer[ 128 ];

  sprintf( Buffer, "%s.redwait.%ld", VasyProcess->NAME, VasyNumberTrans++ );
  VpnTrans  = addvpnproctrans( VasyFigure, VasyProcess, Buffer );

  return( VpnTrans );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitAddVpnPlace                      |
|                                                             |
\------------------------------------------------------------*/

static vpnplace_list *VasyRedWaitAddVpnPlace()
{
  vpnplace_list *VpnPlace;
  char           Buffer[ 128 ];

  sprintf( Buffer, "%s.redwait.%ld", VasyProcess->NAME, VasyNumberTrans++ );
  VpnPlace = addvpnprocplace( VasyFigure, VasyProcess, Buffer );

  return( VpnPlace );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitDelDriverVexExpr                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedWaitDelDriverVexExpr( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  vexexpr    *Atom;
  long        Oper;

  if ( IsVexNodeAtom( Expr ) )
  {
    return;
  }

  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( Oper == VEX_DRIVER )
    {
      Atom = GetVexOperand( Expr->OPERAND );
      freechain( Expr->OPERAND );
      copyvexnode( Expr, Atom );
      freevexexpr( Atom );

      return;
    }
  }

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    VasyRedWaitDelDriverVexExpr( GetVexOperand( ScanOper ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitDelDriverVpnProc                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedWaitDelDriverVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAct;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasyRedWaitDelDriverVpnProc\n" );
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    for ( VpnAct  = VpnTrans->ACT;
          VpnAct != (vpnact_list *)0;
          VpnAct  = VpnAct->NEXT )
    {
      /*
      VpnAct->VEX_EXPR = simpvexexpreq( VpnAct->VEX_EXPR );
      */
      VpnAct->VEX_EXPR = simpvexexpr( VpnAct->VEX_EXPR );
      VasyRedWaitDelDriverVexExpr( VpnAct->VEX_EXPR );
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );

    VasyPrintf( stdout, "  <-- VasyRedWaitDelDriverVpnProc\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyRedWaitPushVpnTrans                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedWaitPushVpnTrans( VpnTrans )

  vpntrans_list *VpnTrans;
{
  vasystack_list *NewStack;
  authelem       *Element;

  Element = searchauthelem( VasyHashStackTrans, VpnTrans->NAME );

  if ( Element != (authelem *)0 )
  {
    VasyError( VASY_LOOP_IN_REDUCTION, VpnTrans->NAME );
  }

  if ( VpnTrans->TYPE != VPN_TRANS_INF_WAIT )
  {
    addauthelem( VasyHashStackTrans, VpnTrans->NAME, 0 );
  }

  NewStack = (vasystack_list *)autallocheap( sizeof( vasystack_list ) );
  NewStack->TRANS = VpnTrans;

  if ( VasyStackQueue == (vasystack_list *)0 )
  {
    VasyStackHead = NewStack;
  }
  else
  {
    VasyStackQueue->NEXT = NewStack;
    NewStack->PREV = VasyStackQueue;
  }

  VasyStackQueue = NewStack;
  VasyStackSize++;
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyRedWaitPopVpnTrans                   |
|                                                             |
\------------------------------------------------------------*/

static vpntrans_list *VasyRedWaitPopVpnTrans()

{
  vasystack_list *DelStack;
  vpntrans_list *VpnTrans;

  DelStack = VasyStackQueue;
  VasyStackQueue = DelStack->PREV;

  if ( VasyStackQueue == (vasystack_list *)0 )
  {
    VasyStackHead = (vasystack_list *)0;
  }
  else
  {
    VasyStackQueue->NEXT = (vasystack_list *)0;
  }

  VpnTrans = DelStack->TRANS;

  autfreeheap( DelStack, sizeof( vasystack_list ) );
  VasyStackSize--;

  if ( VpnTrans->TYPE != VPN_TRANS_INF_WAIT )
  {
    delauthelem( VasyHashStackTrans, VpnTrans->NAME );
  }

  return( VpnTrans );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitStackVpnTrans                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedWaitStackVpnTrans()
{
  vasystack_list *ScanStack;
  vpntrans_list *VpnTrans;
  vpntrans_list *BeginWait;
  vpntrans_list *EndWait;
  vpntrans_list *StackTrans;
  vpnplace_list *EndPlace;
  vpnplace_list *BeginPlace;
  vpnact_list   *VpnAct;
  vpnline_list  *VpnLine;
  vpnarc        *VpnArc;
  vexexpr       *VexGuard;
  short          RedAction;

  BeginWait = VasyStackHead->TRANS;
  EndWait   = VasyStackQueue->TRANS;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasyRedWaitStackVpnTrans %s -> %s Size %ld\n", 
               BeginWait->NAME, EndWait->NAME, VasyStackSize );
  }

  VpnTrans   = VasyRedWaitAddVpnTrans();
  BeginPlace = GetVasyRedWaitPlace( BeginWait );

  VpnArc = GetVpnArc( EndWait->PLACE_IN );
  EndPlace = GetVpnArcSourcePlace( VpnArc );

  addvpnarcplace( VasyFigure, BeginPlace, VpnTrans );
  addvpnarctrans( VasyFigure, VpnTrans  , EndPlace );

  RedAction = 0;

  for ( ScanStack  = VasyStackHead->NEXT;
        ScanStack != VasyStackQueue;
        ScanStack  = ScanStack->NEXT )
  {
    StackTrans = ScanStack->TRANS;

    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, " +++ %s\n", StackTrans->NAME );
    }

    VexGuard = StackTrans->VEX_GUARD;

    if ( VexGuard != (vexexpr *)0 )
    {
      VexGuard = dupvexexpr( VexGuard );
  
      if ( VpnTrans->VEX_GUARD != (vexexpr *)0 )
      {
        VexGuard = optimvexbinexpr( VEX_AND, 1, VpnTrans->VEX_GUARD, VexGuard );
      }

      VpnTrans->VEX_GUARD = VexGuard;
    }

    for ( VpnAct  = StackTrans->ACT;
          VpnAct != (vpnact_list *)0;
          VpnAct  = VpnAct->NEXT )
    {
      addvpnact( VasyFigure, VpnTrans, 
                 dupvexexpr( VpnAct->VEX_ATOM ),
                 dupvexexpr( VpnAct->VEX_EXPR ), VpnAct->TYPE );

      for ( VpnLine  = VpnAct->LINE;
            VpnLine != (vpnline_list *)0;
            VpnLine  = VpnLine->NEXT )
      {
        addvpnline( VasyFigure, &VpnAct->LINE, VpnLine->LINE );
      }
    }

    if ( StackTrans->ACT != (vpnact_list *)0 ) RedAction = 1;
  }

  if ( VpnTrans->VEX_GUARD != (vexexpr *)0 )
  {
    if ( VpnTrans->ACT == (vpnact_list *)0 ) VpnTrans->TYPE = VPN_TRANS_GUARDED;
    else                                     VpnTrans->TYPE = VPN_TRANS_ACT_GUARDED;
  }
  else
  {
    if ( VpnTrans->ACT == (vpnact_list *)0 ) VpnTrans->TYPE = VPN_TRANS_IMMEDIATE;
    else                                     VpnTrans->TYPE = VPN_TRANS_ACT_EXEC;
  }

  if ( RedAction )
  {
    VasyRedActVpnTrans( VasyFigure, VasyProcess, VpnTrans, 1 );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VasyFigure );
    VasyPrintf( stdout, "  <-- VasyRedWaitStackVpnTrans\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyRedWait2WaitVpnTrans                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedWait2WaitVpnTrans( VpnTrans )

  vpntrans_list *VpnTrans;
{
  chain_list    *ScanPlace;
  chain_list    *ScanTrans;
  vpnplace_list *VpnPlace;
  vpntrans_list *NewTrans;
  vpnarc        *VpnArc;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasyRedWait2WaitVpnTrans %s\n", VpnTrans->NAME );
  }

  VasyRedWaitPushVpnTrans( VpnTrans );

  for ( ScanPlace  = VpnTrans->PLACE_OUT;
        ScanPlace != (chain_list *)0;
        ScanPlace  = ScanPlace->NEXT )
  {
    VpnArc   = GetVpnArc( ScanPlace );
    VpnPlace = GetVpnArcTargetPlace( VpnArc );

    for ( ScanTrans  = VpnPlace->TRANS_OUT;
          ScanTrans != (chain_list *)0;
          ScanTrans  = ScanTrans->NEXT )
    {
      VpnArc   = GetVpnArc( ScanTrans );
      NewTrans = GetVpnArcTargetTrans( VpnArc );

      if ( NewTrans->TYPE == VPN_TRANS_INF_WAIT )
      {
        VasyRedWaitPushVpnTrans( NewTrans );
        VasyRedWaitStackVpnTrans( NewTrans );
        VasyRedWaitPopVpnTrans();
      }
      else
      {
        VasyRedWait2WaitVpnTrans( NewTrans );
      }
    }
  }

  VasyRedWaitPopVpnTrans();
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitRenameVex                        |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedWaitRenameVex( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  authelem   *Element;

  if ( Expr != (vexexpr *)0 )
  {
    if ( IsVexNodeAtom( Expr ) )
    {
      if ( ! IsVexAtomLiteral( Expr ) )
      {
        Element = searchauthelem( VasyHashRenDef, GetVexAtomValue( Expr ) );
  
        if ( Element != (authelem *)0 )
        {
          SetVexAtomValue( Expr, Element->VALUE );
        }
      }
    }
  
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasyRedWaitRenameVex( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitRenameDefine                     |
|                                                             |
\------------------------------------------------------------*/


static void VasyRedWaitRenameDefine( VpnFigure, VpnProc, VpnAct )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnact_list  *VpnAct;
{
  vpndecl_list *DefDeclar;
  authelem     *Element;
  vexexpr      *AsgAtom;
  vexexpr      *AsgExpr;
  vexexpr      *DefAtom;
  char         *AtomValue; 
  char         *DefValue;
  int           DefBase;
  int           DefWidth;

  AsgAtom = VpnAct->VEX_ATOM;
  AsgExpr = VpnAct->VEX_EXPR;

  if ( VpnAct->TYPE == VPN_ACT_ASG_DEFINE )
  {
    AtomValue = GetVexAtomValue( AsgAtom );
    Element   = searchauthelem( VasyHashRenDef, AtomValue );
    
    if ( Element == (authelem *)0 )
    {
      DefDeclar = searchvpndecldef( VpnFigure, AtomValue );
      DefAtom   = DefDeclar->VEX_ATOM;
      DefBase   = DefDeclar->BASE;
      DefWidth  = DefAtom->WIDTH;
      DefDeclar = VasyRedActAddVpnDefine( VpnFigure, VpnProc, DefWidth, DefBase );

      DefAtom   = DefDeclar->VEX_ATOM;
      DefValue  = GetVexAtomValue( DefAtom );

      addauthelem( VasyHashRenDef, AtomValue, (long)DefValue );
    }
    else
    {
      DefValue = (char *)Element->VALUE;
    }

    SetVexAtomValue( AsgAtom, DefValue );
  }

  VasyRedWaitRenameVex( AsgExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitLateralWait                      |
|                                                             |
\------------------------------------------------------------*/

static void VasyRedWaitLateralWait( VpnFigure, VpnProc, WaitTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *WaitTrans;
{
  vasyprocinfo  *ProcInfo;
  vpndecl_list  *DefDeclar;
  vpntrans_list *DefTrans;
  vpntrans_list *VpnTrans;
  vpnplace_list *WaitPlace;
  vpnplace_list *BeginPlace;
  vpnplace_list *DefPlace;
  vpnplace_list *VpnPlace;
  vpnplace_list *EndPlace;
  vpnact_list   *VpnAct;
  vpnact_list   *VpnNextAct;
  vpnarc        *VpnArc;
  vexexpr       *DefAtom;
  vexexpr       *VexGuard;
  chain_list    *ScanTrans;
  chain_list   **PrevTrans;
  chain_list    *NewTrans;
  chain_list    *OldTransList;
  chain_list    *NewTransList;
  chain_list   **PrevTransList;
  char          *AtomValue;
  authelem      *Element;
  int            NumberTrans;
  short          CheckLoop;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasyRedWaitLateralWait %s\n", WaitTrans->NAME );
  }

  ProcInfo = GetVasyVpnProcInfo( VpnProc );

  VpnArc     = GetVpnArc( WaitTrans->PLACE_IN );
  WaitPlace  = GetVpnArcSourcePlace( VpnArc );
  VpnArc     = GetVpnArc( WaitTrans->PLACE_OUT );
  BeginPlace = GetVpnArcTargetPlace( VpnArc );

  if ( BeginPlace->NUMBER_OUT <= 1 ) return;

  if ( VasyHashAssign == (authtable *)0 )
  {
    VasyHashAssign = createauthtable( 100 );
    VasyHashRenDef = createauthtable( 100 );
  }
/*
**  Rename all defines for all transition
*/
  for ( ScanTrans  = BeginPlace->TRANS_OUT;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    for ( VpnAct  = VpnTrans->ACT;
          VpnAct != (vpnact_list *)0;
          VpnAct  = VpnAct->NEXT )
    {
      VasyRedWaitRenameDefine( VpnFigure, VpnProc, VpnAct );

    }

    VasyRedWaitRenameVex( VpnTrans->VEX_GUARD );

    resetauthtable( VasyHashRenDef );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }
/*
**  Move all defines assignment on the define transition
*/
  DefTrans = VasyRedWaitAddVpnTrans();
  DefPlace = VasyRedWaitAddVpnPlace();

  addvpnarcplace( VpnFigure, DefPlace, DefTrans );

  for ( ScanTrans  = BeginPlace->TRANS_OUT;
        ScanTrans != (chain_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    VpnArc   = GetVpnArc( ScanTrans );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    VpnAct = VpnTrans->ACT;

    while ( VpnAct != (vpnact_list *)0 )
    {
      VpnNextAct = VpnAct->NEXT;

      if ( VpnAct->TYPE == VPN_ACT_ASG_DEFINE )
      {
        DefAtom   = VpnAct->VEX_ATOM;
        AtomValue = GetVexAtomValue( DefAtom );

        Element = searchauthelem( VasyHashAssign, AtomValue );

        if ( Element == (authelem *)0 )
        {
          *VpnAct->PREV = VpnNextAct;

          if ( VpnNextAct != (vpnact_list *)0 )
          {
            VpnNextAct->PREV = VpnAct->PREV;
          }
          else
          {
            VpnTrans->LAST_ACT = VpnAct->PREV;
          }

          *DefTrans->LAST_ACT = VpnAct;
          VpnAct->PREV        = DefTrans->LAST_ACT;
          VpnAct->NEXT        = (vpnact_list *)0;
          DefTrans->LAST_ACT  = &VpnAct->NEXT;

          addauthelem( VasyHashAssign, AtomValue, (long)VpnAct );
        }
        else
        {
          delvpnact( VpnFigure, VpnTrans, VpnAct );
        }
      }

      VpnAct = VpnNextAct;
    }
  }

  resetauthtable( VasyHashAssign );

  NewTransList  = (chain_list *)0;
  PrevTransList = &NewTransList;
  OldTransList  = BeginPlace->TRANS_OUT;

  BeginPlace->TRANS_OUT  = (chain_list *)0;
  BeginPlace->NUMBER_OUT = 0;

  NumberTrans = 0;

  if ( ProcInfo->NUMBER_WAIT > 1 ) CheckLoop = 1;
  else                             CheckLoop = 0;

  while ( OldTransList != (chain_list *)0 )
  {
    PrevTrans = &OldTransList;
    ScanTrans =  OldTransList;
    EndPlace  = (vpnplace_list *)0;

    while ( ScanTrans != (chain_list *)0 )
    {
      VpnArc   = GetVpnArc( ScanTrans );
      VpnTrans = GetVpnArcTargetTrans( VpnArc );
  
      if ( ( VpnTrans->TYPE != VPN_TRANS_GUARDED     ) &&
           ( VpnTrans->TYPE != VPN_TRANS_ACT_GUARDED ) )
      {
        VasyPrintf( stdout, "ERROR Should not happen %s!!!\n",
                   VPN_TRANS_TYPE[ VpnTrans->TYPE ] );
        autexit( 1 );
      }
  
      VpnArc   = GetVpnArc( VpnTrans->PLACE_OUT );
      VpnPlace = GetVpnArcTargetPlace( VpnArc );

      if ( ( ( EndPlace    == (vpnplace_list *)0 ) ||
             ( VpnPlace    == EndPlace           ) ) &&
           ( ( ! CheckLoop                       ) ||
             ( VpnPlace      != WaitPlace        ) ||
             ( VpnTrans->ACT == (vpnact_list *)0 ) ) )
      {
        EndPlace = VpnPlace;
        NewTrans = ScanTrans;

        *PrevTrans = ScanTrans->NEXT;
        ScanTrans  = ScanTrans->NEXT;
  
        NewTrans->NEXT        = BeginPlace->TRANS_OUT;
        BeginPlace->TRANS_OUT = NewTrans;
        BeginPlace->NUMBER_OUT++;
      }
      else
      {
        PrevTrans = &ScanTrans->NEXT;
        ScanTrans =  ScanTrans->NEXT;
      }
    }

    if ( EndPlace == (vpnplace_list *)0 )
    {
      CheckLoop = 0; continue;
    }

    if ( EndPlace == WaitPlace )
    {
      CheckLoop = 0;
    }

    VasyRedInstLateralWait( VpnFigure, VpnProc, BeginPlace, EndPlace );

    VpnArc   = GetVpnArc( BeginPlace->TRANS_OUT );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );
    VpnAct   = VpnTrans->ACT;

    while ( VpnAct != (vpnact_list *)0 )
    {
      VpnNextAct = VpnAct->NEXT;

      if ( VpnAct->TYPE == VPN_ACT_ASG_DEFINE )
      {
        DefAtom   = VpnAct->VEX_ATOM;
        AtomValue = GetVexAtomValue( DefAtom );

        *VpnAct->PREV = VpnNextAct;

        if ( VpnNextAct != (vpnact_list *)0 )
        {
          VpnNextAct->PREV = VpnAct->PREV;
        }
        else
        {
          VpnTrans->LAST_ACT = VpnAct->PREV;
        }

        *DefTrans->LAST_ACT = VpnAct;
        VpnAct->PREV        = DefTrans->LAST_ACT;
        VpnAct->NEXT        = (vpnact_list *)0;
        DefTrans->LAST_ACT  = &VpnAct->NEXT;
      }

      VpnAct = VpnNextAct;
    }
  
    NewTrans       = BeginPlace->TRANS_OUT;
    *PrevTransList = NewTrans;
    PrevTransList  = &NewTrans->NEXT;

    BeginPlace->TRANS_OUT  = (chain_list *)0;
    BeginPlace->NUMBER_OUT = 0;
  
    NumberTrans++;
  }

  BeginPlace->TRANS_OUT  = NewTransList;
  BeginPlace->NUMBER_OUT = NumberTrans;

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }

  if ( BeginPlace->NUMBER_OUT == 1 )
  {
    VpnArc   = GetVpnArc( BeginPlace->TRANS_OUT );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    freevexexpr( VpnTrans->VEX_GUARD );
    VpnTrans->VEX_GUARD = (vexexpr *)0;

    if ( ( VpnTrans->ACT != (vpnact_list *)0 ) ||
         ( DefTrans->ACT != (vpnact_list *)0 ) ) VpnTrans->TYPE = VPN_TRANS_ACT_EXEC;
    else                                         VpnTrans->TYPE = VPN_TRANS_IMMEDIATE;
  }
  else
  {
    for ( ScanTrans  = BeginPlace->TRANS_OUT;
          ScanTrans != (chain_list *)0;
          ScanTrans  = ScanTrans->NEXT )
    {
      VpnArc   = GetVpnArc( ScanTrans );
      VpnTrans = GetVpnArcTargetTrans( VpnArc );

      VexGuard = VpnTrans->VEX_GUARD;

      if ( ! IsVasyRedInstGuardAtomic( VexGuard ) )
      {
        DefDeclar = VasyRedActAddVpnDefine( VpnFigure, VpnProc, 1, VEX_TYPE_BOOLEAN );

        DefAtom = dupvexnode( DefDeclar->VEX_ATOM );
        VpnTrans->VEX_GUARD = dupvexnode( DefAtom );

        VpnAct = addvpnactasg( VpnFigure, DefTrans, DefAtom, VexGuard );
      }
    }
  }

  if ( DefTrans->ACT != (vpnact_list *)0 )
  {
    if ( BeginPlace->NUMBER_OUT > 1 )
    {
      VpnArc = GetVpnArc( WaitTrans->PLACE_OUT );
      delvpnarc( VpnFigure, VpnArc );

      addvpnarctrans( VpnFigure, WaitTrans, DefPlace   );
      addvpnarctrans( VpnFigure, DefTrans  , BeginPlace );

      DefTrans->TYPE = VPN_TRANS_ACT_EXEC;
    }
    else
    {
      VpnArc   = GetVpnArc( BeginPlace->TRANS_OUT );
      VpnTrans = GetVpnArcTargetTrans( VpnArc );

      if ( VpnTrans->ACT != (vpnact_list *)0 )
      {
        *DefTrans->LAST_ACT = VpnTrans->ACT;
        VpnTrans->ACT->PREV = DefTrans->LAST_ACT;
      }
      else
      {
        VpnTrans->LAST_ACT = DefTrans->LAST_ACT;
      }

      VpnTrans->ACT       = DefTrans->ACT;
      VpnTrans->ACT->PREV = &VpnTrans->ACT;

      DefTrans->ACT = (vpnact_list *)0;
      DefTrans->LAST_ACT = &DefTrans->ACT;
    }
  }

  if ( DefTrans->ACT == (vpnact_list *)0 )
  {
    delvpntrans( VpnFigure, DefTrans );
    delvpnplace( VpnFigure, DefPlace );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VasyFigure );
    VasyPrintf( stdout, "  <-- VasyRedWaitLateralWait %s\n", WaitTrans->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWait2WaitVpnProc                     |
|                                                             |
\------------------------------------------------------------*/

static int VasyRedWait2WaitVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vasyprocinfo  *ProcInfo;
  vpntrans_list *VpnTrans;
  vpntrans_list *DelTrans;
  vpnplace_list *VpnPlace;
  vpnplace_list *DelPlace;
  vpnarc        *VpnArc;
  long           NumberTrans;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasyRedWait2WaitVpnProc %s\n", VpnProc->NAME );
  }

  VasyFigure  = VpnFigure;
  VasyProcess = VpnProc;
  ProcInfo   = GetVasyVpnProcInfo( VpnProc );

  NumberTrans = 0;

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT ) ProcInfo->NUMBER_WAIT++;
    else                                        NumberTrans++;
  }
/*
**  Is a reduction is needed ?
*/
  if ( ( ProcInfo->NUMBER_WAIT == 1 ) &&
       ( NumberTrans           <= 1 ) )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "  No reduction is needed for %s\n", VpnProc->NAME );
    }

    if ( VpnProc->TRANS == (vpntrans_list *)0 )
    {
      VasyErrorLine( VASY_NO_WAIT_IN_PROCESS_ERROR, VpnProc->LINE, VpnProc->NAME );
    }

    return( 1 );
  }

  for ( VpnPlace  = VpnProc->PLACE;
        VpnPlace != (vpnplace_list *)0;
        VpnPlace  = VpnPlace->NEXT )
  {
    SetVasyRedWaitDelete( VpnPlace );
  }
/*
**  Initialization of all the wait transition
*/
  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT )
    {
      VpnArc   = GetVpnArc( VpnTrans->PLACE_IN );
      VpnPlace = GetVpnArcSourcePlace( VpnArc );
      ClearVasyRedWaitDelete( VpnPlace );

      VpnPlace = VasyRedWaitAddVpnPlace();
      SetVasyRedWaitPlace( VpnTrans, VpnPlace );
    }
    else
    {
      SetVasyRedWaitDelete( VpnTrans );
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }

  if ( VasyHashStackTrans == (authtable *)0 )
  {
    VasyHashStackTrans = createauthtable( 100 );
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT )
    {
      VasyRedWait2WaitVpnTrans( VpnTrans );
    }
  }

  resetauthtable( VasyHashStackTrans ); 

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT )
    {
      VpnPlace = GetVasyRedWaitPlace( VpnTrans );
      ClearVasyRedWaitPlace( VpnTrans );

      VpnArc = GetVpnArc( VpnTrans->PLACE_OUT );
      delvpnarc( VpnFigure, VpnArc );

      addvpnarctrans( VpnFigure, VpnTrans, VpnPlace );
    }
  }

  VpnTrans = VpnProc->TRANS;

  while ( VpnTrans != (vpntrans_list *)0 )
  {
    DelTrans = VpnTrans;
    VpnTrans = VpnTrans->NEXT;

    if ( IsVasyRedWaitDelete( DelTrans ) )
    {
      delvpntrans( VpnFigure, DelTrans );
    }
  }

  VpnPlace = VpnProc->PLACE;

  while ( VpnPlace != (vpnplace_list *)0 )
  {
    DelPlace = VpnPlace;
    VpnPlace = VpnPlace->NEXT;

    if ( IsVasyRedWaitDelete( DelPlace ) )
    {
      delvpnplace( VpnFigure, DelPlace );
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT )
    {
      VasyRedWaitLateralWait( VpnFigure, VpnProc, VpnTrans );
    }
  }

  if ( VpnProc->TRANS == (vpntrans_list *)0 )
  {
    VasyErrorLine( VASY_NO_WAIT_IN_PROCESS_ERROR, VpnProc->LINE, VpnProc->NAME );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  <-- VasyRedWait2WaitVpnProc %s\n", VpnProc->NAME );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyRedWaitVpnProc                          |
|                                                             |
\------------------------------------------------------------*/

int VasyRedWaitVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  int Reduced;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyRedWaitVpnProc\n" );
    VasyDebugSaveVpnFig( VpnFigure );
  }
/*
**  Reduce Wait to Wait
*/
  Reduced = VasyRedWait2WaitVpnProc( VpnFigure, VpnProc );
/*
**  Delete All Driver attributes on all assignment statement
*/
  VasyRedWaitDelDriverVpnProc( VpnFigure, VpnProc );

  if ( IsVasyDebugLevel0() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
    VasyPrintf( stdout, "  <-- VasyRedWaitVpnProc\n" );
  }

  return( Reduced );
}
