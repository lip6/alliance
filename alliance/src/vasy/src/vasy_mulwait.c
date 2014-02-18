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
| File    :                vasy_mulwait.c                     |
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
# include "bdd.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"

# include "vasy_error.h"
# include "vasy_debug.h"
# include "vasy_shared.h"
# include "vasy_support.h"
# include "vasy_vexbdd.h"
# include "vasy_synth.h"
# include "vasy_mulwait.h"

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

   static authtable *VasyHashSens  = (authtable *)0;
   static authtable *VasyHashClock = (authtable *)0;
   static authtable *VasyHashWrite = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyMultiWaitCreateVexEvent              |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasyMultiWaitCreateVexEvent( SensSymbol )

  vpnsym *SensSymbol;
{
  vexexpr *VexEvent;

  if ( SensSymbol->INDEX == -1 )
  {
    VexEvent = createvexatombit( SensSymbol->NAME );
  }
  else
  {
    VexEvent = createvexatomvec( SensSymbol->NAME, SensSymbol->INDEX, SensSymbol->INDEX );
  }

  VexEvent = createvexunaryexpr( VEX_EVENT, 1, VexEvent );

  return( VexEvent );
}
/*------------------------------------------------------------\
|                                                             |
|                    VasyMultiWaitViewWrite                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyMultiWaitViewWrite( Element )

  authelem *Element;
{
  vpnsym *VpnSymbol;

  VpnSymbol = (vpnsym *)Element->VALUE;
  fprintf( stdout, "  %s %d\n", VpnSymbol->NAME, VpnSymbol->INDEX );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyMultiWaitHashWrite                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyMultiWaitHashWrite( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vasyactinfo    *ActInfo;
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAction;
  vpndecl_list  *AsgDeclar;
  vpnsym        *AsgSymbol;
  vexexpr       *AsgAtom;
  char          *AtomName;
  unsigned char *Flags;
  chain_list   **Support;
  int            AsgIndex;
  int            AsgMin;
  int            AsgMax;
  int            AsgPos;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyMultiWaitHashWrite %s\n", VpnProc->NAME );
  }

  if ( VasyHashWrite == (authtable *)0 )
  {
    VasyHashWrite = createauthtable( 100 );
  }
  else
  {
    resetauthtable( VasyHashWrite );
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    for ( VpnAction  = VpnTrans->ACT;
          VpnAction != (vpnact_list *)0;
          VpnAction  = VpnAction->NEXT )
    {
      if ( VpnAction->TYPE == VPN_ACT_ASG_DEFINE ) continue;

      ActInfo = GetVasyVpnActInfo( VpnAction );
      Support = ActInfo->SUPPORT;
      Flags   = ActInfo->FLAGS;

      AsgAtom   = VpnAction->VEX_ATOM;
      AtomName  = GetVexAtomValue( AsgAtom );
      AsgDeclar = searchvpndeclall( VpnFigure, AtomName );

      AsgMin = getvexvectormin( AsgAtom );
      AsgMax = getvexvectormax( AsgAtom );

      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgPos = getvexvectorpos( AsgAtom, AsgIndex );

        if ( IsVasyVpnActInfoUsed( Flags[ AsgPos ] ) )
        {
          AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
          addauthelem( VasyHashWrite, (char*)AsgSymbol, (long)AsgSymbol );
        }
        else
        {
          AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );

          if ( IsVasyDebugLevel1() )
          {
            VasyPrintf( stdout, "UNUSED %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
          }
        }
      }
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    viewauthtable( VasyHashWrite, VasyMultiWaitViewWrite );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyMultiWaitHashWrite %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyMultiWaitArcWait2Wait                |
|                                                             |
\------------------------------------------------------------*/

static void VasyMultiWaitArcWait2Wait( VpnFigure, VpnProc, BeginTrans, AsgTrans, EndTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *BeginTrans;
  vpntrans_list *AsgTrans;
  vpntrans_list *EndTrans;
{
  vasyprocinfo   *ProcInfo;
  vasytransinfo  *TransInfo;
  vasyactinfo    *ActInfo;
  vasysyminfo    *SymInfo;
  vpnact_list   *VpnAction;
  vpnsym        *AsgSymbol;
  vpnsym        *VpnSymbol;
  vpndecl_list  *AsgDeclar;
  vexexpr       *AsgAtom;
  char          *AtomName;
  chain_list   **Support;
  chain_list   **Literal;
  chain_list   **Event;
  chain_list    *ScanChain;
  unsigned char *Flags;
  int            AsgIndex;
  int            AsgMin;
  int            AsgMax;
  int            AsgPos;
  int            NumberWrite;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyMultiWaitArcWait2Wait %s (%s) %s\n",
               BeginTrans->NAME, AsgTrans->NAME, EndTrans->NAME );
  }

  ProcInfo = GetVasyVpnProcInfo( VpnProc );

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  Arc from %s to %s\n", BeginTrans->NAME, EndTrans->NAME  );
    VasyPrintf( stdout, "  Condition : " );
    viewvexexprboundln( AsgTrans->VEX_GUARD );
  }

  if ( VasyHashSens == (authtable *)0 )
  {
    VasyHashSens = createauthtable( 100 );
  }

  for ( ScanChain  = BeginTrans->WAIT_SYM;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnSymbol = GetVpnSymbol( ScanChain );
    addauthelem( VasyHashSens, (char*)VpnSymbol, (long)VpnSymbol );
  }

  TransInfo   = GetVasyVpnTransInfo( AsgTrans );

  NumberWrite = 0;

  for ( VpnAction  = AsgTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    if ( VpnAction->TYPE == VPN_ACT_ASG_DEFINE ) continue;

    ActInfo = GetVasyVpnActInfo( VpnAction );
    Support = ActInfo->SUPPORT;
    Literal = ActInfo->LITERAL;
    Event   = ActInfo->EVENT;
    Flags   = ActInfo->FLAGS;

    AsgAtom   = VpnAction->VEX_ATOM;
    AtomName  = GetVexAtomValue( AsgAtom );
    AsgDeclar = searchvpndeclall( VpnFigure, AtomName );

    AsgMin = getvexvectormin( AsgAtom );
    AsgMax = getvexvectormax( AsgAtom );

    for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
    {
      AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
      AsgPos    = getvexvectorpos( AsgAtom, AsgIndex );

      SymInfo = &ActInfo->SYM_INFO[ AsgPos ];
      SymInfo->TYPE = VASY_SYM_UNKNOWN;
/*
**  Symbol is not used just skip it !
*/
      if ( ! IsVasyVpnActInfoUsed( Flags[ AsgPos ] ) )
      {
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "  Skip Symbol %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
        }
        SymInfo->TYPE = VASY_SYM_UNUSED; continue;
      }
      else
      {
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "  Treat Symbol %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
        }
      }
/*
**  No event should appear in the assignation !
*/
      if ( Event[ AsgPos ] != (chain_list *)0 )
      {
        VasyPrintf( stdout, "ERROR Event in %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
      }
/*
**  Check if there is a Tristate literal in the assignation
*/
      for ( ScanChain  = Literal[ AsgPos ];
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        if ( ScanChain->DATA == VEX_ATOM_TRISTATE )
        {
          VasyPrintf( stdout, "ERROR Tristate in %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
        }
      }
# if 0 
/*
**  Check if there is a Cycle in the assignation
*/
      Element2 = searchauth2elem( HashSupport, (char*)&Support[ AsgPos ], (char*)AsgSymbol );

      if ( Element2 != (auth2elem *)0 )
      {
        VasyPrintf( stdout, "ERROR Loop in %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
      }
# endif

      NumberWrite++;
    }
  }

  if ( NumberWrite != VasyHashWrite->NUMBER_ELEM )
  {
    VasyPrintf( stdout, "WARNING Output assignation missing in %s (%s) %s\n", 
               BeginTrans->NAME, AsgTrans->NAME, EndTrans->NAME );
  }

  resetauthtable( VasyHashSens );

  ProcInfo->TYPE = VASY_PROC_FSM;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyMultiWaitArcWait2Wait %s (%s) %s\n",
               BeginTrans->NAME, AsgTrans->NAME, EndTrans->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasyMultiWaitCreateVpnTrans                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyMultiWaitCreateVpnTrans( VpnFigure, VpnProc, BeginTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *BeginTrans;
{
  vasywaitinfo   *WaitInfo;
  vpnplace_list *BeginPlace;
  vpnplace_list *VpnPlace;
  vpnplace_list *EndPlace;
  vpntrans_list *VpnTrans;
  vpntrans_list *DefTrans;
  vpntrans_list *AsgTrans;
//vpntrans_list *EndTrans;
  vpnarc        *VpnArc;
  chain_list    *ScanChain;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyMultiWaitCreateVpnTrans %s\n", BeginTrans->NAME );
  }

  WaitInfo = VasyAddVpnWaitInfo( BeginTrans );
  VpnArc   = GetVpnArc( BeginTrans->PLACE_OUT );
  VpnPlace = GetVpnArcTargetPlace( VpnArc );

  BeginPlace = VpnPlace;
  DefTrans   = (vpntrans_list *)0;

  if ( VpnPlace->NUMBER_OUT == 1 )
  {
    VpnArc   = GetVpnArc( VpnPlace->TRANS_OUT );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    if ( VpnTrans->TYPE != VPN_TRANS_IMMEDIATE )
    {
      VpnArc   = GetVpnArc( VpnTrans->PLACE_OUT );
      VpnPlace = GetVpnArcTargetPlace( VpnArc );

      if ( VpnPlace->NUMBER_OUT > 1 )
      {
        BeginPlace = VpnPlace;
        DefTrans   = VpnTrans;
      }
    }
  }

  WaitInfo->DEF_TRANS  = DefTrans;
  WaitInfo->WAIT_PLACE = BeginPlace;

  if ( DefTrans != (vpntrans_list *)0 )
  {
/*
**  Initialize all define action's for support computation
*/
    VasySupportCreateVpnTrans( VpnFigure, VpnProc, DefTrans );
/*
**  Hash and compute the support and cycle of all actions
*/
    VasySupportVpnTrans( VpnFigure, VpnProc, DefTrans, 0 );
  }

  for ( ScanChain  = BeginPlace->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnArc   = GetVpnArc( ScanChain );
    AsgTrans = GetVpnArcTargetTrans( VpnArc );
    VpnArc   = GetVpnArc( AsgTrans->PLACE_OUT );
    EndPlace = GetVpnArcTargetPlace( VpnArc );
    VpnArc   = GetVpnArc( EndPlace->TRANS_OUT );
  //EndTrans = GetVpnArcTargetTrans( VpnArc );
/*
**  Initialize all action's for support computation
*/
    VasySupportCreateVpnTrans( VpnFigure, VpnProc, AsgTrans );

    if ( DefTrans != (vpntrans_list *)0 )
    {
/*
**  Merge define assignation with all actions
*/
      VasySupportMergeVpnTrans( VpnFigure, VpnProc, DefTrans, AsgTrans );
    }
/*
**  Hash and compute the support and cycle of all actions
*/
    VasySupportVpnTrans( VpnFigure, VpnProc, AsgTrans, 0 );
/*
**  Compute all the used symbols for all actions
*/
    VasySupportUsedVpnTrans( VpnFigure, VpnProc, AsgTrans );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyMultiWaitCreateVpnTrans %s\n", BeginTrans->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                VasyMultiWaitDestroyVpnTrans                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyMultiWaitDestroyVpnTrans( VpnFigure, VpnProc, BeginTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *BeginTrans;
{
  vasywaitinfo   *WaitInfo;
  vpnplace_list *BeginPlace;
  vpntrans_list *DefTrans;
  vpntrans_list *AsgTrans;
  vpnarc        *VpnArc;
  chain_list    *ScanChain;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyMultiWaitDestroyVpnTrans %s\n", BeginTrans->NAME );
  }

  WaitInfo = GetVasyVpnWaitInfo( BeginTrans );

  DefTrans   = WaitInfo->DEF_TRANS;
  BeginPlace = WaitInfo->WAIT_PLACE;

  if ( DefTrans != (vpntrans_list *)0 )
  {
/*
**  Destroy all define action's information
*/
    VasySupportDestroyVpnTrans( VpnFigure, VpnProc, DefTrans );
  }

  for ( ScanChain  = BeginPlace->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnArc   = GetVpnArc( ScanChain );
    AsgTrans = GetVpnArcTargetTrans( VpnArc );
/*
**  Destroy all action's information
*/
    VasySupportDestroyVpnTrans( VpnFigure, VpnProc, AsgTrans );
  }

  VasyDelVpnWaitInfo( BeginTrans );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyMultiWaitDestroyVpnTrans %s\n", BeginTrans->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyMultiWaitFsmVpnProc                 |
|                                                             |
\------------------------------------------------------------*/

static void VasyMultiWaitFsmVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vasyprocinfo   *ProcInfo;
  vasywaitinfo   *WaitInfo;
  vpnplace_list *WaitPlace;
  vpnplace_list *EndPlace;
  vpntrans_list *LoopTrans;
  vpntrans_list *WaitTrans;
  vpntrans_list *AsgTrans;
  vpntrans_list *EndTrans;
  vpnsym        *SensSymbol;
  vpnsym        *EventSymbol;
  vpnarc        *VpnArc;
  vexexpr       *VexGuard;
  vexexpr       *VexLoop;
  vexexpr       *VexEvent;
  vexexpr       *VexEnable;
  chain_list    *ScanChain;
  chain_list    *DelChain;
  chain_list   **PrevChain;
  chain_list    *SupportBdd;
  chain_list    *SupportVex;
  authelem      *Element;
  bddcircuit    *BddCircuit;
  bddsystem     *BddSystem;
  bddassoc      *BddAssoc;
  bddnode       *BddStable;
  bddnode       *BddNode;
  bddnode       *BddGuard;
  bddnode       *BddLoop;
  bddnode       *BddAux0;
  bddnode       *BddAux1;
  bddvar         BddVar;
  long           NumberLoop;
  long           MissLoop;
  long           NumberSens;
  int            ClockType;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyMultiWaitFsmVpnProc %s\n", VpnProc->NAME );
  }
/*
**  Hash All Written and Used Vpn symbol
*/
  VasyMultiWaitHashWrite( VpnFigure, VpnProc );
/*
**  Create Bdd Circuit for the process
*/
  VasyVexBddCreateBddCircuit( VpnFigure, VpnProc );

  if ( VasyHashClock == (authtable *)0 )
  {
    VasyHashClock = createauthtable( 100 );
  }
  else
  {
    resetauthtable( VasyHashClock );
  }
/*
**  Compute the Fsm's clock condition
*/
  ProcInfo   = GetVasyVpnProcInfo( VpnProc );
  BddCircuit = ProcInfo->BDD_CIRCUIT;
  BddSystem  = ProcInfo->BDD_SYSTEM;

  BddLoop    = BddSystem->ONE;
  SupportBdd = (chain_list *)0;
  NumberLoop = 0;
  MissLoop   = 0;

  for ( WaitTrans  = VpnProc->TRANS;
        WaitTrans != (vpntrans_list *)0;
        WaitTrans  = WaitTrans->NEXT )
  {
    if ( WaitTrans->TYPE != VPN_TRANS_INF_WAIT ) continue;

    NumberSens = 0;
    SensSymbol = (vpnsym *)0;

    for ( ScanChain  = WaitTrans->WAIT_SYM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      SensSymbol = GetVpnSymbol( ScanChain );
      NumberSens++;
    }

    if ( NumberSens != 1 ) SensSymbol = (vpnsym *)0;

    WaitInfo  = GetVasyVpnWaitInfo( WaitTrans );
    WaitPlace = WaitInfo->WAIT_PLACE;
    LoopTrans = (vpntrans_list *)0;
  
    for ( ScanChain  = WaitPlace->TRANS_OUT;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      VpnArc   = GetVpnArc( ScanChain );
      AsgTrans = GetVpnArcTargetTrans( VpnArc );

      VpnArc   = GetVpnArc( AsgTrans->PLACE_OUT );
      EndPlace = GetVpnArcTargetPlace( VpnArc );

      VpnArc   = GetVpnArc( EndPlace->TRANS_OUT );
      EndTrans = GetVpnArcTargetTrans( VpnArc );

      if ( EndTrans == WaitTrans )
      {
        if ( ( LoopTrans     == (vpntrans_list *)0 ) ||
             ( AsgTrans->ACT == (vpnact_list   *)0 ) )
        {
          LoopTrans = AsgTrans;
        }
      }
    }

    if ( LoopTrans == (vpntrans_list *)0 )
    {
      VasyPrintf( stdout, "ERROR missing loop arc in wait trans %s\n", WaitTrans->NAME );
      MissLoop++;
    }
    else
    {
      WaitInfo->LOOP_TRANS = LoopTrans;
/*
**  If there is only one signal in the sensitivity list => Add event
*/
      if ( SensSymbol != (vpnsym *)0 )
      {
        VexEvent = VasyMultiWaitCreateVexEvent( SensSymbol );
        VexEvent = createvexunaryexpr( VEX_NOT, 1, VexEvent );

        LoopTrans->VEX_GUARD = createvexbinexpr( VEX_OR, 1, LoopTrans->VEX_GUARD, VexEvent );
      }

      VexGuard = LoopTrans->VEX_GUARD;

      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "  Loop condition in wait %s\n", WaitTrans->NAME );
        VasyPrintf( stdout, "  > " );
        viewvexexprboundln( LoopTrans->VEX_GUARD );
      }

      BddGuard = VasyVexBddConvertVex2Bdd( VpnFigure, VpnProc, LoopTrans, VexGuard, 0 );
/*
**  Build a Bdd association with all symbols of the BddGuard
*/
      freechain( SupportBdd );
      SupportBdd = getbddnodesupportchain( BddSystem, BddGuard );

      PrevChain = &SupportBdd;
      ScanChain =  SupportBdd;

      while ( ScanChain != (chain_list *)0 )
      {
        BddNode = (bddnode *)ScanChain->DATA;
        
        if ( NumberLoop == 0 )
        {
          addauthelem( VasyHashClock, (char*)BddNode, 1 );

          PrevChain = &ScanChain->NEXT;
          ScanChain =  ScanChain->NEXT;
        }
        else
        {
          Element = searchauthelem( VasyHashClock, (char*)BddNode );

          if ( ( Element != (authelem *)0     ) &&
               ( Element->VALUE == NumberLoop ) )
          {
            Element->VALUE++;

            PrevChain = &ScanChain->NEXT;
            ScanChain =  ScanChain->NEXT;
          }
          else
          {
            DelChain   = ScanChain;
            ScanChain  = ScanChain->NEXT;
            *PrevChain = ScanChain;

            DelChain->NEXT = (chain_list *)0;
            freechain( DelChain );
          }
        }
      }

      addbddcircuitout( BddCircuit, WaitTrans->NAME, BddGuard );

      BddLoop = applybddnode( BddSystem, ABL_AND, 
                              decbddrefext( BddLoop ),
                              decbddrefext( BddGuard ) );
      NumberLoop++;
    }
  }

  if ( MissLoop )
  {
    VasyPrintf( stdout, "ERROR missing loop condition in %s !\n", VpnProc->NAME );
    return; /* Should EXIT ! */
  }

  ProcInfo->TYPE = VASY_PROC_FSM;

  BddAssoc = addbddassoc( BddSystem );

  for ( ScanChain  = SupportBdd;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    BddNode = (bddnode *)ScanChain->DATA;
    BddVar  = getbddvarbyindex( BddSystem, BddNode->INDEX );
    addbddnodeassoc( BddSystem, BddAssoc, BddVar, BddNode );
  }

  freechain( SupportBdd );

  BddLoop = forallbddnodemissassoc( BddSystem, decbddrefext( BddLoop ), BddAssoc );
  BddLoop = applybddnodenot( BddSystem, decbddrefext( BddLoop ) );

  if ( IsVasyDebugLevel1() )
  {
    addbddcircuitout( BddCircuit, "loop", BddLoop );
    testbddcircuit( BddCircuit );
    BddLoop = decbddrefext( BddLoop );
    BddLoop = searchbddcircuitout( BddCircuit, "loop" );
    BddLoop = incbddrefext( BddLoop );
    delbddcircuitout( BddCircuit, "loop" );
  }

  delbddassoc( BddSystem, BddAssoc );
/*
**  Check if there is an edge in the loop condition
*/
  VexLoop    = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddLoop );
  SupportVex = VasySupportVpnTransEventBitVex( VpnFigure, VpnProc, VexLoop );
  freevexexpr( VexLoop );

  if ( SupportVex == (chain_list *)0 )
  {
    VasyPrintf( stdout, "ERROR missing edge in the FSM condition !\n" );

    return; /* Should EXIT ! */
  }
  else
  if ( SupportVex->NEXT != (chain_list *)0 )
  {
    VasyPrintf( stdout, "ERROR too many events in the FSM condition !\n" );

    return; /* Should EXIT ! */
  }

  EventSymbol = (vpnsym *)SupportVex->DATA;
  freechain( SupportVex );
/*
**  Modify all Guard's Transitions with BddLoop
*/
  for ( WaitTrans  = VpnProc->TRANS;
        WaitTrans != (vpntrans_list *)0;
        WaitTrans  = WaitTrans->NEXT )
  {
    if ( WaitTrans->TYPE != VPN_TRANS_INF_WAIT ) continue;
 
    WaitInfo  = GetVasyVpnWaitInfo( WaitTrans );
    WaitPlace = WaitInfo->WAIT_PLACE;

    DelChain = (chain_list *)0;

    for ( ScanChain  = WaitPlace->TRANS_OUT;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      VpnArc   = GetVpnArc( ScanChain );
      AsgTrans = GetVpnArcTargetTrans( VpnArc );
      VpnArc   = GetVpnArc( AsgTrans->PLACE_OUT );
      EndPlace = GetVpnArcTargetPlace( VpnArc );
      VpnArc   = GetVpnArc( EndPlace->TRANS_OUT );
      EndTrans = GetVpnArcTargetTrans( VpnArc );

      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "Arc %s -> %s -> %s\n", 
                    WaitTrans->NAME, AsgTrans->NAME, EndTrans->NAME );
      }

      VexGuard = AsgTrans->VEX_GUARD;
      BddGuard = VasyVexBddConvertVex2Bdd( VpnFigure, VpnProc, AsgTrans, VexGuard, 0 );
      BddGuard = cofactorbddnode( BddSystem, decbddrefext( BddGuard ), BddLoop );
 
      addbddcircuitout( BddCircuit, AsgTrans->NAME, BddGuard );

      if ( BddGuard != BddSystem->ZERO )
      {
        if ( BddGuard == BddSystem->ONE )
        {
          freevexexpr( AsgTrans->VEX_GUARD );
          AsgTrans->VEX_GUARD = (vexexpr *)0;

          if ( AsgTrans->TYPE == VPN_TRANS_GUARDED )
          {
            AsgTrans->TYPE = VPN_TRANS_IMMEDIATE;
          }
          else
          {
            AsgTrans->TYPE = VPN_TRANS_ACT_EXEC;
          }
        }
        else
        {
          VexGuard = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddGuard );
          freevexexpr( AsgTrans->VEX_GUARD );
          AsgTrans->VEX_GUARD = VexGuard;
  
          decbddrefext( BddGuard );
        }

        VasyMultiWaitArcWait2Wait( VpnFigure, VpnProc, WaitTrans, AsgTrans, EndTrans );
      }
      else
      {
        DelChain = addchain( DelChain, AsgTrans );
      }
    }

    for ( ScanChain  = DelChain;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      AsgTrans = (vpntrans_list *)ScanChain->DATA;
      delvpntrans( VpnFigure, AsgTrans );
    }

    freechain( DelChain );
  }

  VexEvent  = (vexexpr *)0;
  VexLoop   = (vexexpr *)0;
  VexEnable = (vexexpr *)0;

  BddStable = VasyVexBddSearchBddCircuitStable( VpnFigure, VpnProc, EventSymbol );
  BddVar    = getbddvarbyindex( BddSystem, BddStable->INDEX );

  BddAux0 = restrictbddnode( BddSystem, BddLoop, BddVar, BddSystem->ZERO );
  BddAux1 = restrictbddnode( BddSystem, BddLoop, BddVar, BddSystem->ONE  );
/*
**  Check the correctness of the event attribute in the loop condition
*/
  if ( BddAux1 == BddAux0 )
  {
    VasyPrintf( stdout, "ERROR missing edge in the FSM condition !\n" );
  }
/*
**  Stable True => Loop = 0 and Stable False => Loop != 0 !
*/
  if ( BddAux1 != BddSystem->ZERO )
  {
    VasyPrintf( stdout, "ERROR Illegal use of the event attribute !\n" );
  }

  if ( BddAux0 == BddSystem->ONE )
  {
    VasyPrintf( stdout, "ERROR Illegal use of the event attribute !\n" );
  }

  VexEvent = VasyMultiWaitCreateVexEvent( EventSymbol );

  decbddrefext( BddLoop );
  BddLoop = incbddrefext( BddAux0 );

  decbddrefext( BddAux0 );
  decbddrefext( BddAux1 );
/*
**  Check if there is a write enable condition in the follow condition
*/
  BddNode = VasyVexBddSearchBddCircuitInput( VpnFigure, VpnProc, EventSymbol );
  BddVar  = getbddvarbyindex( BddSystem, BddNode->INDEX );
            
  BddAux0 = restrictbddnode( BddSystem, BddLoop, BddVar, BddSystem->ZERO );
  BddAux1 = restrictbddnode( BddSystem, BddLoop, BddVar, BddSystem->ONE  );
/*
**  Positive Edge Condition -> CK'event and CK and WEN
*/
  ClockType = 0;

  if ( BddAux0 == BddSystem->ZERO )
  {
    if ( BddAux1 != BddSystem->ONE )
    {
      VexEnable = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddAux1 );

      decbddrefext( BddLoop );
      BddLoop = cofactorbddnode( BddSystem, BddLoop, BddAux1 );
    }

    ClockType = VASY_BIVEX_RISING_EDGE;
  }
  else
/*
**  Negative Edge Condition -> CK'event and CK and WEN
*/
  if ( BddAux1 == BddSystem->ZERO )
  {
    if ( BddAux0 != BddSystem->ONE )
    {
      VexEnable = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddAux0 );

      decbddrefext( BddLoop );
      BddLoop = cofactorbddnode( BddSystem, BddLoop, BddAux0 );
    }

    decbddrefext( BddLoop );
    BddLoop = applybddnodenot( BddSystem, BddLoop );

    ClockType = VASY_BIVEX_FALLING_EDGE;
  }
  else
  {
    VasyPrintf( stdout, "ERROR Illegal edge condition !\n" );
  }

  decbddrefext( BddAux0 );
  decbddrefext( BddAux1 );

  VexLoop = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddLoop );

  VasyAddVpnProcBiVex( ProcInfo, VexLoop, (vexexpr *)0, ClockType );

  if ( VexEnable != (vexexpr *)0 )
  {
    VasyAddVpnProcBiVex( ProcInfo, VexEnable, (vexexpr *)0, VASY_BIVEX_SYNC_WEN );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "    Loop : " );
    viewvexexprboundln( VexLoop );
    VasyPrintf( stdout, "    Event  : " );
    viewvexexprboundln( VexEvent  );
    VasyPrintf( stdout, "    Enable : " );
    viewvexexprboundln( VexEnable );
  }
/*
**  Destroy Bdd Circuit for the process
*/
  VasyVexBddDestroyBddCircuit( VpnFigure, VpnProc );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyMultiWaitFsmVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyMultiWaitVpnProc                    |
|                                                             |
\------------------------------------------------------------*/

void VasyMultiWaitVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *VpnTrans;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyMultiWaitVpnProc %s\n", VpnProc->NAME );
    VasyDebugSaveVpnFig( VpnFigure );
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT )
    {
      VasyMultiWaitCreateVpnTrans( VpnFigure, VpnProc, VpnTrans );
    }
  }

  VasyMultiWaitFsmVpnProc( VpnFigure, VpnProc );

  VasySynthesisMultiWaitVpnProc( VpnFigure, RtlFigure, VpnProc );

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT )
    {
      VasyMultiWaitDestroyVpnTrans( VpnFigure, VpnProc, VpnTrans );
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
    VasyPrintf( stdout, "  <-- VasyMultiWaitVpnProc %s\n", VpnProc->NAME );
  }
}
