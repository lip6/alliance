/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                  vasy_onewait.c                   |
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
# include "rtn.h"

# include "vasy_error.h"
# include "vasy_debug.h"
# include "vasy_shared.h"
# include "vasy_support.h"
# include "vasy_vexbdd.h"
# include "vasy_synth.h"
# include "vasy_onewait.h"

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

  static authtable   *VasyHashSens      = (authtable  *)0;
  static authtable   *VasyHashBitVecDef = (authtable  *)0;
  static authtable   *VasyHashDefine    = (authtable  *)0;
  static authtable   *VasyHashSupport   = (authtable  *)0;

  static vpnfig_list *VasyFigure = (vpnfig_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyOneWaitCreateVexEvent                |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyOneWaitCreateVexEvent( SensSymbol )

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
|                    VasyOneWaitAddVexEvent                   |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyOneWaitAddVexEvent( AsgAtom, AsgExpr, SensSymbol )

  vexexpr *AsgAtom;
  vexexpr *AsgExpr;
  vpnsym  *SensSymbol;
{
  vexexpr *VexEvent;
  vexexpr *VexNEvent;

  VexEvent  = VasyOneWaitCreateVexEvent( SensSymbol );
  VexNEvent = createvexunaryexpr( VEX_NOT  , 1, dupvexexpr( VexEvent ) );

  VexEvent  = createvexbinexpr( VEX_IFT, AsgExpr->WIDTH, VexEvent , dupvexexpr( AsgExpr ) );
  VexNEvent = createvexbinexpr( VEX_IFT, AsgAtom->WIDTH, VexNEvent, dupvexexpr( AsgAtom ) );

  AsgExpr = createvexbinexpr( VEX_OR, AsgExpr->WIDTH, VexEvent, VexNEvent );

  return( AsgExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyOneWaitSubstVexAtom                 |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyOneWaitSubstVexAtom( Expr )

  vexexpr   *Expr;
{
  authelem     *Element;
  vpndecl_list *DefDeclar;
  vpnact_list  *VarAsg;
  vpnsym       *VarSymbol;
  vexexpr      *VarAtom;
  vpnsym       *DefSymbol;
  char         *DefName;
  vexexpr      *SubstExpr;
  int           Scan;
  int           Index;
  int           Width;
  int           Step;

  SubstExpr = (vexexpr *)0;
  DefName   = GetVexAtomValue( Expr );
  Element   = searchauthelem( VasyHashDefine, DefName );
/*
**  Check if define should be substituted
*/
  if ( Element != (authelem *)0 )
  {
    DefDeclar = searchvpndeclall( VasyFigure, DefName );
    VarAsg    = (vpnact_list *)Element->VALUE;

    if ( ! isvextypedivisible( DefDeclar->BASE ) )
    {
      freevexexpr( Expr );
      Expr = dupvexexpr( VarAsg->VEX_ATOM );
    }
    else
    {
      if ( Expr->LEFT > Expr->RIGHT ) Step = -1;
      else                            Step =  1;
      
      Index = Expr->LEFT;
      Width = Expr->WIDTH;

      for ( Scan = 0; Scan < Width; Scan++ )
      {
        DefSymbol = getvpnsymdecl( DefDeclar, Index );
        Element   = searchauthelem( VasyHashBitVecDef, (char*)DefSymbol );

        if ( Element != (authelem *)0 ) break;
      }

      if ( Scan < Width )
      {
        if ( Width > 1 ) SubstExpr = createvexoper( VEX_CONCAT, Width );

        for ( Scan = 0; Scan < Width; Scan++ )
        {
          DefSymbol = getvpnsymdecl( DefDeclar, Index );
          Element   = searchauthelem( VasyHashBitVecDef, (char*)DefSymbol );
      
          if ( Element != (authelem *)0 ) VarSymbol = (vpnsym *)Element->VALUE;
          else                            VarSymbol = DefSymbol;
      
          VarAtom = createvexatomvec( VarSymbol->NAME, VarSymbol->INDEX, VarSymbol->INDEX );
        
          if ( Width == 1 ) SubstExpr = VarAtom;
          else              addvexqexpr( SubstExpr, VarAtom );
        
          Index += Step;
        }
        
        freevexexpr( Expr );
        Expr = simpvexexpr( SubstExpr );
      }
    }

    if ( IsVasyDebugLevel0() )
    {
      VasyPrintf( stdout, "  --> VasyOneWaitSubstVexAtom\n" );
      VasyPrintf( stdout, "  +++ Replace %s by ", DefName );
      viewvexexprboundln( Expr );
      VasyPrintf( stdout, "  <-- VasyOneWaitSubstVexAtom\n" );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyOneWaitSubstVex                     |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyOneWaitSubstVex( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;
  vexexpr    *SubstExpr;

  if ( IsVexNodeAtom( Expr ) )
  {
    if ( ! IsVexAtomLiteral( Expr ) )
    {
      Expr = VasyOneWaitSubstVexAtom( Expr, 0 );
    }

    return( Expr );
  }

  for ( ScanOper  = Expr->OPERAND;
        ScanOper != (chain_list *)0;
        ScanOper  = ScanOper->NEXT )
  {
    SubstExpr = VasyOneWaitSubstVex( GetVexOperand( ScanOper ) );
    SetVexOperand( ScanOper, SubstExpr );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyOneWaitSubstVpnAct                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyOneWaitSubstVpnAct( VpnFigure, VpnProc, VpnTrans, VpnAct )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAct;
{
  VasyFigure = VpnFigure;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  --> VasyOneWaitSubstVpnAct\n" );
    VasyPrintf( stdout, "  +++ " );
    viewvexexprboundln( VpnAct->VEX_EXPR );
  }

  VpnAct->VEX_EXPR = VasyOneWaitSubstVex( VpnAct->VEX_EXPR );

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "  +++ " );
    viewvexexprboundln( VpnAct->VEX_EXPR );
    VasyPrintf( stdout, "  <-- VasyOneWaitSubstVpnAct\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyOneWaitAssignVpnSymbol               |
|                                                             |
\------------------------------------------------------------*/

static void VasyOneWaitAssignVpnSymbol( VpnFigure, VpnProc  , VpnTrans, 
                                       AsgAction, AsgSymbol, SensSymbol )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
  vpnact_list   *AsgAction;
  vpnsym        *AsgSymbol;
  vpnsym        *SensSymbol;
{
  vasyprocinfo   *ProcInfo;
  vasytransinfo  *TransInfo;
  vasyactinfo    *ActInfo;
  vasysyminfo    *SymInfo;
  vpnact_list   *DefAction;
  vpnact_list   *DefAction2;
  vpnact_list   *VpnAction2;
  vpnsym        *EventSymbol;
  vpnsym        *DefSymbol;
  vpnsym        *DefSymbol2;
  vpnsym        *AsgSymbol2;
  vpndecl_list  *DefDeclar;
  vpndecl_list  *DefDeclar2;
  vpndecl_list  *AsgDeclar;
  vpndecl_list  *AsgDeclar2;
  vexexpr       *AsgExpr;
  vexexpr       *DefAtom;
  vexexpr       *DefAtom2;
  vexexpr       *AsgAtom;
  vexexpr       *AsgAtom2;
  authelem      *Element;
  auth2elem     *Element2;
  chain_list   **Support;
  chain_list   **Literal;
  chain_list   **Event;
  chain_list   **Support2;
  chain_list   **Literal2;
  chain_list   **DefSupport;
  chain_list   **DefSupport2;
  unsigned char *DefFlags;
  unsigned char *DefFlags2;
  unsigned char *Flags;
  unsigned char *Flags2;
  chain_list    *ListOutput;
  chain_list    *SupportBdd;
  chain_list    *ScanChain;
  chain_list    *ScanChain2;
  int            DefPos;
  int            DefPos2;
  int            AsgPos;
  int            AsgPos2;
  int            DefIndex;
  int            DefIndex2;
  int            AsgIndex2;
  short          AddInput;
  short          Subst;
  short          InFollow;
  short          FreeAsgExpr;
  bddcircuit    *BddCircuit;
  bddsystem     *BddSystem;
  bddnode      **BddLiteral;
  bddassoc      *BddAssoc;
  bddnode       *BddAssign;
  bddnode       *BddAux;
  bddnode       *BddAux0;
  bddnode       *BddAux1;
  bddnode       *BddNotAux0;
  bddnode       *BddNotAux1;
  bddnode       *BddFollow;
  bddnode       *BddOscill;
  bddnode       *BddData;
  bddnode       *BddNode;
  bddnode       *BddStable;
  bddnode       *BddClock;
  bddnode       *BddFAsync;
  bddnode       *BddCAsync;
  bddnode       *BddCSetAsync;
  bddnode       *BddCResetAsync;
  bddnode       *BddCWriteAsync;
  bddnode       *BddDataAsync;
  bddnode       *BddCEdgeSync;
  bddnode       *BddFEdgeSync;
  bddnode       *BddFSync;
  bddnode       *BddCSync;
  bddnode       *BddCEdge;
  bddnode       *BddRising;
  bddnode       *BddFalling;
  bddnode       *BddCSetSync;
  bddnode       *BddCResetSync;
  bddnode       *BddCWriteSync;
  bddnode       *BddDataSync;
  bddvar         BddVar;
  vexexpr       *VexExpr;
  vexexpr       *VexData;
  authtable     *HashAssign;
  authtable     *HashBitVec;
  auth2table    *HashLiteral;
  auth2table    *HashSupport;
  long           Size;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyOneWaitAssignVpnSymbol\n" );
  }

  ProcInfo    = GetVasyVpnProcInfo( VpnProc );
  BddCircuit  = ProcInfo->BDD_CIRCUIT;
  BddSystem   = ProcInfo->BDD_SYSTEM;
  BddLiteral  = ProcInfo->BDD_LITERAL;

  TransInfo   = GetVasyVpnTransInfo( VpnTrans );
  HashAssign  = TransInfo->HASH_ASSIGN;
  HashBitVec  = TransInfo->HASH_BITVEC;
  HashLiteral = TransInfo->HASH_LITERAL;
  HashSupport = TransInfo->HASH_SUPPORT;

  ActInfo = GetVasyVpnActInfo( AsgAction );
  Literal = ActInfo->LITERAL;
  Support = ActInfo->SUPPORT;
  Flags   = ActInfo->FLAGS;
  Event   = ActInfo->EVENT;

  ActInfo->SPLIT = 1;

  AsgDeclar = AsgSymbol->DECL;
  AsgAtom   = AsgAction->VEX_ATOM;
  AsgExpr   = AsgAction->VEX_EXPR;
  AsgPos    = getvexvectorpos( AsgAtom, AsgSymbol->INDEX );

  SymInfo = &ActInfo->SYM_INFO[ AsgPos ];

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
  }

  if ( AsgDeclar->TYPE == VPN_DECLAR_VARIABLE )
  {
    DefSymbol = (vpnsym *)Support[ AsgPos ]->DATA;

    addauthelem( VasyHashBitVecDef, (char*)DefSymbol, (long)AsgSymbol );
    addauthelem( VasyHashDefine   , DefSymbol->NAME , (long)AsgAction );
  }

  FreeAsgExpr = 0;

  if ( Event[ AsgPos ] != (chain_list *)0 )
  {
    EventSymbol = (vpnsym *)Event[ AsgPos ]->DATA;
    
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "Event Symbol %s\n", EventSymbol->NAME );
    }
  }
  else
  {
    EventSymbol = SensSymbol;

    if ( IsVasyDebugLevel1() )
    {
      if ( EventSymbol != (vpnsym *)0 )
      {
        VasyPrintf( stdout, "Event Symbol %s\n", EventSymbol->NAME );
      }
    }
  }
/*
**  Special case for register with one wait sens list symbol => Add Event
*/
  if ( ( SensSymbol    != (vpnsym *)0      ) &&
       ( SymInfo->TYPE == VASY_SYM_REGISTER ) )
  {
    if ( SensSymbol != EventSymbol )
    {
      VasyPrintf( stdout, "ERROR bad event %s %d\n", EventSymbol->NAME, EventSymbol->INDEX );
    }

    AsgExpr = VasyOneWaitAddVexEvent( AsgAtom, AsgExpr, SensSymbol );

    if ( IsVasyDebugLevel0() )
    {
      VasyPrintf( stdout, "  --> Add Event %s %d\n", SensSymbol->NAME, SensSymbol->INDEX );
    }

    if ( IsVasyDebugLevel1() )
    {
      viewvexexprboundln( AsgExpr );
    }

    FreeAsgExpr = 1;
  }

  ListOutput = (chain_list *)0;

  for ( ScanChain  = Support[ AsgPos ];
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    AsgSymbol2 = (vpnsym *)ScanChain->DATA;
    AsgDeclar2 = AsgSymbol2->DECL;

    AddInput = 1;

    if ( AsgDeclar2->TYPE == VPN_DECLAR_DEFINE )
    {
      if ( isvextypedivisible( AsgDeclar2->BASE ) )
      {
        Element = searchauthelem( HashBitVec, (char*)AsgSymbol2 );
      }
      else
      {
        Element = searchauthelem( HashAssign, AsgSymbol2->NAME );
      }

      VpnAction2 = (vpnact_list *)Element->VALUE;
      Literal2   = GetVasyVpnActInfoLiteral( VpnAction2 );
      Support2   = GetVasyVpnActInfoSupport( VpnAction2 );
      Flags2     = GetVasyVpnActInfoFlags( VpnAction2 );
      AsgAtom2   = VpnAction2->VEX_ATOM;

      if ( AsgSymbol2->INDEX == -1 ) AsgIndex2 = 0;
      else                           AsgIndex2 = AsgSymbol2->INDEX;

      AsgPos2  = getvexvectorpos( AsgAtom2, AsgIndex2 );
/*
**  For all defines assignation where AsgSymbol appears (cycle) => Substitute defines
*/
      Element2 = searchauth2elem( HashSupport,
                                  (char*)&Support2[ AsgPos2 ], (char*)AsgSymbol );

      if ( ( SymInfo->TYPE == VASY_SYM_TRISTATE ) &&
           ( Element2      == (auth2elem *)0   ) )
      {
/*
**  For all defines assignation where 'z' appears => Substitute defines
*/
        Element2 = searchauth2elem( HashLiteral, 
                                    (char*)&Literal2[ AsgPos2 ],
                                    (char*)VEX_ATOM_BY_ID[ SymInfo->TRS_TYPE ] );
      }

      if ( ( Element2    == (auth2elem *)0 ) &&
           ( EventSymbol != (vpnsym    *)0 ) )
      {
        Element2 = searchauth2elem( HashSupport, 
                                    (char*)&Support2[ AsgPos2 ], (char*)EventSymbol );
      }

      if ( Element2 != (auth2elem *)0 )
      {
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "Should subst %s %d\n", AsgSymbol2->NAME, AsgSymbol2->INDEX );
        }

        if ( ! IsVasyVpnActInfoArith( Flags2[ AsgPos2 ] ) )
        {
          AddInput = 0;
        }
        else
        {
/*
**  Substitution is impossible when there's an arithmetic expression !
*/
          if ( IsVasyDebugLevel0() )
          {
            VasyPrintf( stdout, "WARNING: Arithmetic !! unable to substitute\n" );
          }
        }
      }
      else
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "Don't substitute %s %d\n", AsgSymbol2->NAME, AsgSymbol2->INDEX );
      }

      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "   >> " );
        viewvexexprboundln( VpnAction2->VEX_ATOM );
        VasyPrintf( stdout, "   #= " );
        viewvexexprboundln( VpnAction2->VEX_EXPR );
      }
    }
    else
    if ( AsgDeclar2->TYPE == VPN_DECLAR_CONSTANT )
    {
      AddInput = 0;
    }

    if ( AddInput )
    {
      BddNode = VasyVexBddAddBddCircuitInput( VpnFigure, VpnProc, AsgSymbol2 );
    }
    else
    {
      ListOutput = addchain( ListOutput, AsgSymbol2 );
    }
  }

  if ( ( SymInfo->TYPE == VASY_SYM_TRISTATE ) &&
       ( SymInfo->VEX_DATA != (vexexpr *)0 ) )
  {
    AsgPos  = 0;
    AsgExpr = SymInfo->VEX_DATA;
    SymInfo->VEX_DATA = (vexexpr *)0;

    FreeAsgExpr = 1;
  }

  do
  {
    Subst = 0;
/*
**  Create The Bdd of the register/tristate assignment
*/
    Size = VasyVexBddConvertVex2BddSize( VpnFigure, VpnProc, VpnTrans, AsgExpr, AsgPos );

    if ( Size > 400 )
    {
      VasyPrintf( stdout, "ERROR too complex assignment %s %d -> %ld\n",
                  AsgSymbol->NAME, AsgSymbol->INDEX, Size );
      break;
    }

    BddAssign = VasyVexBddConvertVex2Bdd( VpnFigure, VpnProc, VpnTrans, AsgExpr, AsgPos );

    if ( SymInfo->TYPE == VASY_SYM_TRISTATE )
    {
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "*** Check tristate !\n" );
      }

      BddNode = BddLiteral[ SymInfo->TRS_TYPE ];
      BddVar  = getbddvarbyindex( BddSystem, BddNode->INDEX );
  
      BddAux0 = restrictbddnode( BddSystem, BddAssign, BddVar, BddSystem->ZERO );
      BddAux1 = restrictbddnode( BddSystem, BddAssign, BddVar, BddSystem->ONE  );
  
      BddNotAux0 = applybddnodenot( BddSystem, BddAux0 );
      BddNotAux1 = applybddnodenot( BddSystem, BddAux1 );
  
      BddFollow = applybddnode( BddSystem, ABL_OR, BddAux0, BddNotAux1 );
      BddData   = incbddrefext( BddAux0 );
  
      if ( BddFollow == BddSystem->ZERO )
      {
        SymInfo->TYPE = VASY_SYM_COMBINATORIAL;
      }

      if ( IsVasyDebugLevel1() )
      {
        addbddcircuitout( BddCircuit, "follow", BddFollow );
        addbddcircuitout( BddCircuit, "z0"    , BddAux0   );
        addbddcircuitout( BddCircuit, "z1"    , BddAux1   );
        addbddcircuitout( BddCircuit, "data"  , BddData   );
        addbddcircuitout( BddCircuit, "assign", BddAssign );
    
        testbddcircuit( BddCircuit );
    
        delbddcircuitout( BddCircuit, "follow" );
        delbddcircuitout( BddCircuit, "z0"     );
        delbddcircuitout( BddCircuit, "z1"     );
        delbddcircuitout( BddCircuit, "data"   );
        delbddcircuitout( BddCircuit, "assign" );
      }

      decbddrefext( BddAux0    );
      decbddrefext( BddAux1    );
      decbddrefext( BddNotAux0 );
      decbddrefext( BddNotAux1 );
    }
    else
    {
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "*** Check register !\n" );
      }

      BddAux = VasyVexBddSearchBddCircuitInput( VpnFigure, VpnProc, AsgSymbol );
      BddVar = getbddvarbyindex( BddSystem, BddAux->INDEX );
  
      BddAux0 = restrictbddnode( BddSystem, BddAssign, BddVar, BddSystem->ZERO );
      BddAux1 = restrictbddnode( BddSystem, BddAssign, BddVar, BddSystem->ONE  );
  
      BddNotAux0 = applybddnodenot( BddSystem, BddAux0 );
      BddNotAux1 = applybddnodenot( BddSystem, BddAux1 );
/*
**  Check the possibility of an oscillation !!!
*/
      BddOscill = applybddnode( BddSystem, ABL_AND, BddAux0, BddNotAux1 );
  
      if ( BddOscill != BddLocalSystem->ZERO )
      {
        if ( IsVasyDebugLevel0() )
        {
          VasyPrintf( stdout, "WARNING Possibility of an oscillation !!\n" );
  
          VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddOscill );
          VasyPrintf( stdout, "    Oscill      : " );
          viewvexexprboundln( VexExpr );
  
          freevexexpr( VexExpr );
        }
      }
  
      BddFollow = applybddnode( BddSystem, ABL_OR, BddAux0, BddNotAux1 );
    
      if ( BddFollow == BddSystem->ONE )
      {
/*
**  The symbol assignment is not of type register => just dataflow
*/
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "It's not a register, symbol always assigned !\n" );
        }
    
        BddData = incbddrefext( BddAssign );
    
        SymInfo->TYPE = VASY_SYM_COMBINATORIAL;
      }
      else
      if ( BddFollow == BddSystem->ZERO )
      {
/*
**  The symbol assignment is not of type register => just delete it
*/
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "It's not a register, symbol never assigned !\n" );
        }
  
        BddData = BddSystem->ZERO;
  
        SymInfo->TYPE = VASY_SYM_CONSTANT;
      }
      else
      {
/*
**  The symbol assignment is of type register => analysis
*/
        /* ICI  BIG BUG !! Corrected ...
        BddData = incbddrefext( BddAux0 );
        */
        BddData = applybddnode( BddSystem, ABL_AND, BddAssign, BddFollow );
        SymInfo->TYPE = VASY_SYM_REGISTER;
      }

      if ( IsVasyDebugLevel1() )
      {
        addbddcircuitout( BddCircuit, "follow", BddFollow );
        addbddcircuitout( BddCircuit, "aux_0" , BddAux0   );
        addbddcircuitout( BddCircuit, "aux_1" , BddAux1   );
        addbddcircuitout( BddCircuit, "oscill", BddOscill );
        addbddcircuitout( BddCircuit, "data"  , BddData   );
        addbddcircuitout( BddCircuit, "assign", BddAssign );
 
        testbddcircuit( BddCircuit );

        delbddcircuitout( BddCircuit, "follow" );
        delbddcircuitout( BddCircuit, "aux_0"  );
        delbddcircuitout( BddCircuit, "aux_1"  );
        delbddcircuitout( BddCircuit, "oscill" );
        delbddcircuitout( BddCircuit, "data"   );
        delbddcircuitout( BddCircuit, "assign" );
      }

      decbddrefext( BddAux0    );
      decbddrefext( BddAux1    );
      decbddrefext( BddNotAux0 );
      decbddrefext( BddNotAux1 );
      decbddrefext( BddOscill  );

    }
/*
**  For Follow condition, verify there's no define !
*/
    if ( BddFollow != BddSystem->ZERO )
    {
      resetauthtable( VasyHashSupport );

      SupportBdd = VasyVexBddSupportVpnSymbol( VpnFigure, VpnProc, BddFollow );

      for ( ScanChain  = SupportBdd;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        DefSymbol = (vpnsym *)ScanChain->DATA;
        addauthelem( VasyHashSupport, (char *)DefSymbol, 0 );
      }

      freechain( SupportBdd );

      SupportBdd = VasyVexBddSupportVpnSymbol( VpnFigure, VpnProc, BddAssign );

      for ( ScanChain  = SupportBdd;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        DefSymbol = (vpnsym *)ScanChain->DATA;
        DefDeclar = DefSymbol->DECL;

        if ( DefDeclar->TYPE == VPN_DECLAR_DEFINE )
        {
          Element = searchauthelem( VasyHashSupport, (char *)DefSymbol );

          if ( Element != (authelem *)0 ) InFollow  = 1;
          else                            InFollow  = 0;

          if ( isvextypedivisible( DefDeclar->BASE ) )
          {
            Element = searchauthelem( HashBitVec, (char*)DefSymbol );
          }
          else
          {
            Element = searchauthelem( HashAssign, DefSymbol->NAME );
          }

          DefAction  = (vpnact_list *)Element->VALUE;
          DefFlags   = GetVasyVpnActInfoFlags( DefAction );
          DefSupport = GetVasyVpnActInfoSupport( DefAction );
          DefAtom    = DefAction->VEX_ATOM;

          if ( DefSymbol->INDEX == -1 ) DefIndex = 0;
          else                          DefIndex = DefSymbol->INDEX;

          DefPos = getvexvectorpos( DefAtom, DefIndex );

          if ( ! IsVasyVpnActInfoArith( DefFlags[ DefPos ] ) )
          {
            if ( ! InFollow )
            {
              for ( ScanChain2  = DefSupport[ DefPos ];
                    ScanChain2 != (chain_list *)0;
                    ScanChain2  = ScanChain2->NEXT )
              {
                DefSymbol2 = (vpnsym *)ScanChain2->DATA;
                Element = searchauthelem( VasyHashSupport, (char *)DefSymbol2 );

                if ( Element != (authelem *)0 )
                {
                  InFollow = 1; break;
                }
              }
            }

            if ( InFollow )
            {
              if ( IsVasyDebugLevel1() )
              {
                VasyPrintf( stdout, "Should subst %s %d\n", DefSymbol->NAME, DefSymbol->INDEX );
              }

              for ( ScanChain2  = DefSupport[ DefPos ];
                    ScanChain2 != (chain_list *)0;
                    ScanChain2  = ScanChain2->NEXT )
              {
                DefSymbol2 = (vpnsym *)ScanChain2->DATA;
                DefDeclar2 = DefSymbol2->DECL;
  
                if ( DefDeclar2->TYPE == VPN_DECLAR_DEFINE )
                {
                  if ( isvextypedivisible( DefDeclar2->BASE ) )
                  {
                    Element = searchauthelem( HashBitVec, (char*)DefSymbol2 );
                  }
                  else
                  {
                    Element = searchauthelem( HashAssign, DefSymbol2->NAME );
                  }
  
                  DefAction2  = (vpnact_list *)Element->VALUE;
                  DefSupport2 = GetVasyVpnActInfoSupport( DefAction2 );
                  DefFlags2   = GetVasyVpnActInfoFlags( DefAction2 );
                  DefAtom2    = DefAction2->VEX_ATOM;
  
                  if ( DefSymbol2->INDEX == -1 ) DefIndex2 = 0;
                  else                           DefIndex2 = DefSymbol2->INDEX;
  
                  DefPos2  = getvexvectorpos( DefAtom2, DefIndex2 );
  
                  if ( ! IsVasyVpnActInfoArith( DefFlags2[ DefPos2 ] ) )
                  {
                    VasyVexBddDelBddCircuitInput( VpnFigure, VpnProc, DefSymbol2 );
                    VasyVexBddDelBddCircuitOutput( VpnFigure, VpnProc, DefSymbol2 );
                  }
                  else
                  {
                    if ( IsVasyDebugLevel0() )
                    {
                      VasyPrintf( stdout, "WARNING: Arithmetic !! unable to substitute\n" );
                    }
                  }
  
                  if ( IsVasyDebugLevel0() )
                  {
                    VasyPrintf( stdout, "   >> " );
                    viewvexexprboundln( DefAction2->VEX_ATOM );
                    VasyPrintf( stdout, "   #= " );
                    viewvexexprboundln( DefAction2->VEX_EXPR );
                  }
                }
              }

              VasyVexBddDelBddCircuitInput( VpnFigure, VpnProc, DefSymbol );
            
              Subst = 1;
            }
          }
          else
          {
            if ( IsVasyDebugLevel0() )
            {
              VasyPrintf( stdout, "   >> " );
              viewvexexprboundln( DefAction->VEX_ATOM );
              VasyPrintf( stdout, "   #= " );
              viewvexexprboundln( DefAction->VEX_EXPR );
            }

            if ( IsVasyDebugLevel0() )
            {
              VasyPrintf( stdout, "WARNING: Arithmetic !! unable to substitute\n" );
            }
          }

          if ( IsVasyDebugLevel1() )
          {
            VasyPrintf( stdout, "   >> " );
            viewvexexprboundln( DefAction->VEX_ATOM );
            VasyPrintf( stdout, "   #= " );
            viewvexexprboundln( DefAction->VEX_EXPR );
          }
        }
      }

      freechain( SupportBdd );
    }
/*
**  No more define to substitute, convert bdd to vex and perform analysis 
*/
    if ( ! Subst )
    {
      reorderbddsystemsimple( BddSystem );

      if ( SymInfo->TYPE == VASY_SYM_COMBINATORIAL )
      {
        VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddData );
        SymInfo->VEX_DATA = VexExpr;
      }
      else
/*
**  Register analysis, extract sync/async set and reset, write enable and edge ...
*/
      if ( ( SymInfo->TYPE == VASY_SYM_REGISTER ) ||
           ( SymInfo->TYPE == VASY_SYM_TRISTATE ) )
      {
        BddFAsync       = BddSystem->ZERO;
        BddCAsync       = BddSystem->ZERO;
        BddCSetAsync    = BddSystem->ZERO;
        BddCResetAsync  = BddSystem->ZERO;
        BddCWriteAsync  = BddSystem->ZERO;
        BddDataAsync    = BddSystem->ZERO;

        if ( EventSymbol != (vpnsym *)0 )
        {
          BddStable = VasyVexBddSearchBddCircuitStable( VpnFigure, VpnProc, EventSymbol );
          BddVar    = getbddvarbyindex( BddSystem, BddStable->INDEX );
            
          BddCAsync = restrictbddnode( BddSystem, BddFollow, BddVar, BddSystem->ONE );
          BddFAsync = restrictbddnode( BddSystem, BddData  , BddVar, BddSystem->ONE  );
        }
        else
        {
          BddCAsync = incbddrefext( BddFollow );
          BddFAsync = incbddrefext( BddData   );
        }
/*
**  Build a Bdd association with all symbols of the BddCAsync
*/
        BddAssoc   = addbddassoc( BddSystem );
        SupportBdd = getbddnodesupportchain( BddSystem, BddCAsync );
    
        for ( ScanChain  = SupportBdd;
              ScanChain != (chain_list *)0;
              ScanChain  = ScanChain->NEXT )
        {
          BddNode = (bddnode *)ScanChain->DATA;
          BddVar  = getbddvarbyindex( BddSystem, BddNode->INDEX );
          addbddnodeassoc( BddSystem, BddAssoc, BddVar, BddNode );
        }
    
        freechain( SupportBdd );
/*
**  Search all asynchronous informations
*/
        BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCAsync, BddFAsync );
        decbddrefext( BddNode );
        BddNode = forallbddnodemissassoc( (bddsystem *)0, BddNode, BddAssoc );

        BddCSetAsync = BddNode;

        BddNode = applybddnodenot( (bddsystem *)0, BddFAsync );
        decbddrefext( BddNode );
        BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCAsync, BddNode );
        decbddrefext( BddNode );
        BddNode = forallbddnodemissassoc( (bddsystem *)0, BddNode, BddAssoc );

        BddCResetAsync = BddNode;

        BddNode = applybddnode( (bddsystem *)0, ABL_NOR, BddCSetAsync, BddCResetAsync );
        decbddrefext( BddNode );
        BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCAsync, BddNode );

        BddCWriteAsync = BddNode;

        BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteAsync, BddFAsync );
        decbddrefext( BddNode );
        BddNode = existbddnodeassoc( (bddsystem *)0, BddNode, BddAssoc );

        BddDataAsync = BddNode;
/*
**  Verify the correctness of asynchronous informations
*/
        BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteAsync, BddDataAsync );
        decbddrefext( BddNode );
        BddNode = applybddnode( (bddsystem *)0, ABL_OR , BddCSetAsync, BddNode );
        decbddrefext( BddNode );

        if ( ( BddNode        != BddFAsync       ) &&
             ( BddCWriteAsync != BddSystem->ZERO ) )
        {
          if ( IsVasyDebugLevel0() )
          {
            VasyPrintf( stdout, "Cofactor for asynchronous conditions %s\n", AsgSymbol->NAME );
          }

          decbddrefext( BddDataAsync );
          BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteAsync, BddFAsync );
          decbddrefext( BddNode );
          BddDataAsync = cofactorbddnode( (bddsystem *)0, BddNode, BddCWriteAsync );

          BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteAsync, BddDataAsync );
          decbddrefext( BddNode );
          BddNode = applybddnode( (bddsystem *)0, ABL_OR , BddCSetAsync, BddNode );
          decbddrefext( BddNode );
        }

        if ( BddNode != BddFAsync )
        {
          VasyPrintf( stdout, "Illegal asynchronous conditions %s\n", AsgSymbol->NAME );

          /* TO BE DONE ! A VERIFIER */

          decbddrefext( BddCSetAsync   );
          decbddrefext( BddCResetAsync );
          decbddrefext( BddCWriteAsync );
          decbddrefext( BddDataAsync   );

          BddCWriteAsync = incbddrefext( BddFollow );
          BddDataAsync   = incbddrefext( BddData   );
        }
/*
**  Add asynchronous informations to symbol
*/
        {
          if ( BddCSetAsync != BddSystem->ZERO )
          {
            VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddCSetAsync );
            VasyAddVpnSymBiVex( SymInfo, VexExpr, (vexexpr *)0, VASY_BIVEX_ASYNC_SET );

            if ( IsVasyDebugLevel0() )
            {
              VasyPrintf( stdout, "    SetAsync    : " );
              viewvexexprboundln( VexExpr );
            }
          }

          if ( BddCResetAsync != BddSystem->ZERO )
          {
            VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddCResetAsync );
            VasyAddVpnSymBiVex( SymInfo, VexExpr, (vexexpr *)0, VASY_BIVEX_ASYNC_RESET );

            if ( IsVasyDebugLevel0() )
            {
              VasyPrintf( stdout, "    ResetAsync  : " );
              viewvexexprboundln( VexExpr );
            }
          }

          if ( BddCWriteAsync != BddSystem->ZERO )
          {
            VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddCWriteAsync );
            VexData = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddDataAsync );
            VasyAddVpnSymBiVex( SymInfo, VexExpr, VexData, VASY_BIVEX_ASYNC_WEN );

            if ( IsVasyDebugLevel0() )
            {
              VasyPrintf( stdout, "    WriteAsync  : " );
              viewvexexprboundln( VexExpr );
              VasyPrintf( stdout, "    DataAsync   : " );
              viewvexexprboundln( VexData );
            }
          }

          if ( ( BddCSetAsync   != BddSystem->ZERO ) ||
               ( BddCResetAsync != BddSystem->ZERO ) ||
               ( BddCWriteAsync != BddSystem->ZERO ) )
          {
            SymInfo->REG_TYPE = VASY_SYM_REGISTER_ASYNC;
          }
        }

        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "Asynchronous analysis results\n" );

          addbddcircuitout( BddCircuit, "follow", BddFollow      );
          addbddcircuitout( BddCircuit, "data"  , BddData        );
          addbddcircuitout( BddCircuit, "casync", BddCAsync      );
          addbddcircuitout( BddCircuit, "fasync", BddFAsync      );
          addbddcircuitout( BddCircuit, "csa"   , BddCSetAsync   );
          addbddcircuitout( BddCircuit, "cra"   , BddCResetAsync );
          addbddcircuitout( BddCircuit, "cwa"   , BddCWriteAsync );
          addbddcircuitout( BddCircuit, "dwa"   , BddDataAsync   );
    
          testbddcircuit( BddCircuit );
    
          delbddcircuitout( BddCircuit, "follow" );
          delbddcircuitout( BddCircuit, "data"   );
          delbddcircuitout( BddCircuit, "casync" );
          delbddcircuitout( BddCircuit, "fasync" );
          delbddcircuitout( BddCircuit, "csa"    );
          delbddcircuitout( BddCircuit, "cra"    );
          delbddcircuitout( BddCircuit, "cwa"    );
          delbddcircuitout( BddCircuit, "dwa"    );
        }

        decbddrefext( BddCSetAsync    );
        decbddrefext( BddCResetAsync  );
        decbddrefext( BddCWriteAsync  );
        decbddrefext( BddDataAsync    );
/*
**  Search all synchronous informations
*/
        if ( EventSymbol != (vpnsym *)0 )
        {
          BddFSync        = BddSystem->ZERO;
          BddCSync        = BddSystem->ZERO;
          BddCEdge        = BddSystem->ZERO;
          BddRising       = BddSystem->ZERO;
          BddFalling      = BddSystem->ZERO;
          BddCSetSync     = BddSystem->ZERO;
          BddCResetSync   = BddSystem->ZERO;
          BddCWriteSync   = BddSystem->ZERO;
          BddDataSync     = BddSystem->ZERO;

          BddAux0  = applybddnodenot( (bddsystem *)0, BddCAsync );
          BddFEdgeSync = applybddnode( (bddsystem *)0, ABL_AND, BddData  , BddAux0 );
          BddCEdgeSync = applybddnode( (bddsystem *)0, ABL_AND, BddFollow, BddAux0 );
/*
**  Verify there is no intersection between CAsync and CEdge 
*/
          BddAux1 = existbddnodeassoc( (bddsystem *)0, BddCEdgeSync, BddAssoc );
          BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddAux1, BddAux0 );

          decbddrefext( BddAux0 );
          decbddrefext( BddAux1 );
          decbddrefext( BddNode );

          if ( BddNode == BddCEdgeSync )
          {
            decbddrefext( BddCEdgeSync );
            BddCEdgeSync = incbddrefext( BddAux1 );
          
            decbddrefext( BddFEdgeSync );
            BddFEdgeSync = existbddnodeassoc( (bddsystem *)0, BddFEdgeSync, BddAssoc );
          }
          else
          {
            VasyPrintf( stdout, "Conflict between asynchronous and synchronous condition !\n" );
          }

          delbddassoc( BddSystem, BddAssoc );
          BddAssoc = addbddassoc( BddSystem );

          BddStable = VasyVexBddSearchBddCircuitStable( VpnFigure, VpnProc, EventSymbol );
          BddVar    = getbddvarbyindex( BddSystem, BddStable->INDEX );
          addbddnodeassoc( BddSystem, BddAssoc, BddVar, BddStable );

          BddClock = VasyVexBddSearchBddCircuitInput( VpnFigure, VpnProc, EventSymbol );
          BddVar   = getbddvarbyindex( BddSystem, BddClock->INDEX );
          addbddnodeassoc( BddSystem, BddAssoc, BddVar, BddClock );

          BddCEdge = existbddnodemissassoc( (bddsystem *)0, BddCEdgeSync, BddAssoc );

          BddFalling = applybddnode( (bddsystem *)0, ABL_NOR, BddClock, BddStable );
          BddNode    = applybddnodenot( (bddsystem *)0, BddStable );
          decbddrefext( BddNode );
          BddRising  = applybddnode( (bddsystem *)0, ABL_AND, BddClock, BddNode );

          if ( ( BddCEdge != BddFalling ) &&
               ( BddCEdge != BddRising  ) )
          {
            if ( BddCEdge != BddLocalSystem->ZERO )
            {
              VasyPrintf( stdout, "Illegal edge condition on %s!\n", AsgSymbol->NAME );
            }

            BddCSync = incbddrefext( BddCEdgeSync );
            BddFSync = incbddrefext( BddFEdgeSync );

            decbddrefext( BddCEdge );
            BddCEdge = BddSystem->ZERO;
          }
          else
          {
            BddCSync = existbddnodeassoc( (bddsystem *)0, BddCEdgeSync, BddAssoc );
            BddFSync = existbddnodeassoc( (bddsystem *)0, BddFEdgeSync, BddAssoc );
          }

          delbddassoc( BddSystem, BddAssoc );

          if ( BddCSync != BddSystem->ONE )
          {
            BddAssoc = addbddassoc( BddSystem );
           
            SupportBdd = getbddnodesupportchain( BddSystem, BddCSync );
           
            for ( ScanChain  = SupportBdd;
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              BddNode = (bddnode *)ScanChain->DATA;
              BddVar  = getbddvarbyindex( BddSystem, BddNode->INDEX );
              addbddnodeassoc( BddSystem, BddAssoc, BddVar, BddNode );
            }
           
            freechain( SupportBdd );
/*
**  Search all synchronous informations
*/
            BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCSync, BddFSync );
            decbddrefext( BddNode );
            BddNode = forallbddnodemissassoc( (bddsystem *)0, BddNode, BddAssoc );
           
            BddCSetSync = BddNode;
           
            BddNode = applybddnodenot( (bddsystem *)0, BddFSync );
            decbddrefext( BddNode );
            BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCSync, BddNode );
            decbddrefext( BddNode );
            BddNode = forallbddnodemissassoc( (bddsystem *)0, BddNode, BddAssoc );
           
            BddCResetSync = BddNode;
           
            BddNode = applybddnode( (bddsystem *)0, ABL_NOR, BddCSetSync, BddCResetSync );
            decbddrefext( BddNode );
            BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCSync, BddNode );
           
            BddCWriteSync = BddNode;
  
            BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteSync, BddFSync );
            decbddrefext( BddNode );

            BddNode = existbddnodeassoc( (bddsystem *)0, BddNode, BddAssoc );
            BddDataSync = BddNode;

            BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteSync, BddDataSync );
            decbddrefext( BddNode );
            BddNode = applybddnode( (bddsystem *)0, ABL_OR , BddCSetSync, BddNode );
            decbddrefext( BddNode );
            BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCEdge, BddNode );
            decbddrefext( BddNode );

            if ( ( BddNode       != BddFEdgeSync    ) &&
                 ( BddCWriteSync != BddSystem->ZERO ) )
            {
              if ( IsVasyDebugLevel0() )
              {
                VasyPrintf( stdout, "Cofactor for synchronous conditions %s\n",
                            AsgSymbol->NAME );
              }

              decbddrefext( BddDataSync );

              BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteSync, BddFSync );
              decbddrefext( BddNode );

              BddNode = cofactorbddnode( (bddsystem *)0, BddNode, BddCWriteSync );
              BddDataSync = BddNode;
            }
          }
          else
          {
            BddCWriteSync = BddSystem->ONE;
            BddDataSync   = incbddrefext( BddFSync );
          }
/*
**  Verify the correctness of synchronous informations
*/
          BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCWriteSync, BddDataSync );
          decbddrefext( BddNode );
          BddNode = applybddnode( (bddsystem *)0, ABL_OR , BddCSetSync, BddNode );
          decbddrefext( BddNode );
          BddNode = applybddnode( (bddsystem *)0, ABL_AND, BddCEdge, BddNode );
          decbddrefext( BddNode );

          if ( BddNode != BddFEdgeSync )
          {
            VasyPrintf( stdout, "Illegal synchronous conditions %s\n", AsgSymbol->NAME );
          }
/*
**  Add synchronous informations to symbol
*/
          if ( BddCEdge != BddSystem->ZERO )
          {
            if ( SymInfo->REG_TYPE == VASY_SYM_REGISTER_ASYNC )
            {
              SymInfo->REG_TYPE = VASY_SYM_REGISTER_MIXED;
            }
            else
            {
              SymInfo->REG_TYPE = VASY_SYM_REGISTER_SYNC;
            }

            VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddClock );

            if ( BddCEdge == BddFalling )
            {
              if ( IsVasyDebugLevel0() )
              {
                VasyPrintf( stdout, "    FallingEdge : " );
                viewvexexprboundln( VexExpr );
              }
              VasyAddVpnSymBiVex( SymInfo, VexExpr, (vexexpr *)0, VASY_BIVEX_FALLING_EDGE );
            }
            else
            {
              if ( IsVasyDebugLevel0() )
              {
                VasyPrintf( stdout, "    RisingEdge  : " );
                viewvexexprboundln( VexExpr );
              }
              VasyAddVpnSymBiVex( SymInfo, VexExpr, (vexexpr *)0, VASY_BIVEX_RISING_EDGE );
            }

            if ( BddCSetSync != BddSystem->ZERO )
            {
              VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddCSetSync );
              VasyAddVpnSymBiVex( SymInfo, VexExpr, (vexexpr *)0, VASY_BIVEX_SYNC_SET );

              if ( IsVasyDebugLevel0() )
              {
                VasyPrintf( stdout, "    SetSync     : " );
                viewvexexprboundln( VexExpr );
              }
            }

            if ( BddCResetSync != BddSystem->ZERO )
            {
              VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddCResetSync );
              VasyAddVpnSymBiVex( SymInfo, VexExpr, (vexexpr *)0, VASY_BIVEX_SYNC_RESET );

              if ( IsVasyDebugLevel0() )
              {
                VasyPrintf( stdout, "    ResetSync   : " );
                viewvexexprboundln( VexExpr );
              }
            }

            if ( BddCWriteSync != BddSystem->ZERO )
            {
              VexExpr = (vexexpr *)0;

              if ( BddCWriteSync != BddSystem->ONE )
              {
                VexExpr = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddCWriteSync );
              }

              VexData = VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddDataSync );
              VasyAddVpnSymBiVex( SymInfo, VexExpr, VexData, VASY_BIVEX_SYNC_WEN );

              if ( IsVasyDebugLevel0() )
              {
                VasyPrintf( stdout, "    WriteSync   : " );
                viewvexexprboundln( VexExpr );
                VasyPrintf( stdout, "    DataSync    : " );
                viewvexexprboundln( VexData );
              }
            }
          }

          if ( IsVasyDebugLevel1() )
          {
            VasyPrintf( stdout, "Synchronous analysis results\n" );

            addbddcircuitout( BddCircuit, "follow" , BddFollow     );
            addbddcircuitout( BddCircuit, "data"   , BddData       );
            addbddcircuitout( BddCircuit, "cedge"  , BddCEdge      );
            addbddcircuitout( BddCircuit, "fedges" , BddFEdgeSync  );
            addbddcircuitout( BddCircuit, "falling", BddFalling    );
            addbddcircuitout( BddCircuit, "rising" , BddRising     );
            addbddcircuitout( BddCircuit, "fsync"  , BddFSync      );
            addbddcircuitout( BddCircuit, "csync"  , BddCSync      );
            addbddcircuitout( BddCircuit, "css"    , BddCSetSync   );
            addbddcircuitout( BddCircuit, "crs"    , BddCResetSync );
            addbddcircuitout( BddCircuit, "cws"    , BddCWriteSync );
            addbddcircuitout( BddCircuit, "dws"    , BddDataSync   );
      
            testbddcircuit( BddCircuit );

            delbddcircuitout( BddCircuit, "follow"  );
            delbddcircuitout( BddCircuit, "data"    );
            delbddcircuitout( BddCircuit, "cedge"   );
            delbddcircuitout( BddCircuit, "fedges"  );
            delbddcircuitout( BddCircuit, "falling" );
            delbddcircuitout( BddCircuit, "rising"  );
            delbddcircuitout( BddCircuit, "fsync"   );
            delbddcircuitout( BddCircuit, "csync"   );
            delbddcircuitout( BddCircuit, "css"     );
            delbddcircuitout( BddCircuit, "crs"     );
            delbddcircuitout( BddCircuit, "cws"     );
            delbddcircuitout( BddCircuit, "dws"     );
          }

          decbddrefext( BddFSync       );
          decbddrefext( BddCSync       );
          decbddrefext( BddCEdge       );
          decbddrefext( BddRising      );
          decbddrefext( BddFalling     );
          decbddrefext( BddCSetSync    );
          decbddrefext( BddCResetSync  );
          decbddrefext( BddCWriteSync  );
          decbddrefext( BddDataSync    );
        }

        decbddrefext( BddFAsync );
        decbddrefext( BddCAsync );

        delbddassoc( BddSystem, BddAssoc );
      }
    }

    decbddrefext( BddFollow );
    decbddrefext( BddData   );
    decbddrefext( BddAssign );

    for ( ScanChain  = ListOutput;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      DefSymbol = (vpnsym *)ScanChain->DATA;
      VasyVexBddDelBddCircuitOutput( VpnFigure, VpnProc, DefSymbol );
    }
  }
  while ( Subst );

  freechain( ListOutput );

  if ( FreeAsgExpr )
  {
    freevexexpr( AsgExpr );
  }

  if ( SymInfo->BIVEX != (vasybivex_list *)0 )
  {
    SymInfo->BIVEX = (vasybivex_list *)reverse( (chain_list *)SymInfo->BIVEX );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyOneWaitAssignVpnSymbol %d\n", SymInfo->TYPE );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyOneWaitVpnTrans                     |
|                                                             |
\------------------------------------------------------------*/

static void VasyOneWaitVpnTrans( VpnFigure, VpnProc, WaitTrans, VpnTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *WaitTrans;
  vpntrans_list *VpnTrans;
{
  vasytransinfo  *TransInfo;
  vasyactinfo    *ActInfo;
  vasysyminfo    *SymInfo;
  vpnact_list   *VpnAction;
  vpnsym        *AsgSymbol;
  vpnsym        *VpnSymbol;
  vpnsym        *SensSymbol;
  vpndecl_list  *AsgDeclar;
  vpndecl_list  *VpnDeclar;
  vexexpr       *AsgAtom;
  vexexpr       *VexBit;
  char          *AtomName;
  authelem      *Element;
  auth2elem     *Element2;
  chain_list   **Support;
  chain_list   **Literal;
  chain_list   **Event;
  chain_list    *ScanChain;
  chain_list    *SupportBit;
  unsigned char *Flags;
  int            NumberEvent;
  int            NumberSens;
  int            NumberInSens;
  int            NumberNotInSens;
  int            NumberSupport;
  int            TristateLoop;
  int            AsgIndex;
  int            AsgMin;
  int            AsgMax;
  int            AsgPos;
  auth2table    *HashSupport;
  authtable     *HashAssign;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyOneWaitVpnTrans %s\n", VpnTrans->NAME );
  }
/*
**  Create Bdd Circuit
*/
  VasyVexBddCreateBddCircuit( VpnFigure, VpnProc );
/*
**  Hash All symbol that appears in the sensitivity list
*/
  if ( VasyHashSens == (authtable *)0 )
  {
    VasyHashSens      = createauthtable( 100 );
    VasyHashDefine    = createauthtable( 100 );
    VasyHashBitVecDef = createauthtable( 100 );
    VasyHashSupport   = createauthtable( 100 );
  }

  NumberSens = 0;
  SensSymbol = (vpnsym *)0;

  for ( ScanChain  = WaitTrans->WAIT_SYM;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    SensSymbol = GetVpnSymbol( ScanChain );
    addauthelem( VasyHashSens, (char*)SensSymbol, (long)SensSymbol );

    NumberSens++;
  }

  if ( NumberSens != 1 ) SensSymbol = (vpnsym *)0;

  TransInfo   = GetVasyVpnTransInfo( VpnTrans );
  HashSupport = TransInfo->HASH_SUPPORT;
  HashAssign  = TransInfo->HASH_ASSIGN;
/*
**  Never assigned variable should be replaced by define  
*/
  for ( ScanChain  = VpnProc->VAR_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnDeclar = (vpndecl_list *)ScanChain->DATA;
    AsgAtom   = VpnDeclar->VEX_ATOM;
    AtomName  = GetVexAtomValue( AsgAtom );

    Element = searchauthelem( HashAssign, AtomName );

    if ( Element == (authelem *)0 )
    {
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, "Variable %s never assigned !\n", AtomName );
      }
      
      VpnDeclar->TYPE = VPN_DECLAR_CONSTANT;
    }
  }

  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    VasyOneWaitSubstVpnAct( VpnFigure, VpnProc, VpnTrans, VpnAction );

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
          VasyPrintf( stdout, "Skip Symbol %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
        }

        SymInfo->TYPE = VASY_SYM_UNUSED; continue;
      }
      else
      {
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "Treat Symbol %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
        }
      }
/*
**  Count the number of event that appear in the assignation
*/
      NumberEvent = 0;

      for ( ScanChain  = Event[ AsgPos ];
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        VpnSymbol = (vpnsym *)ScanChain->DATA;

        Element = searchauthelem( VasyHashSens, (char*)VpnSymbol );

        if ( Element == (authelem *)0 )
        {
          VasyPrintf( stdout, "ERROR Event symbol %s %d but not in sens list\n",
                     AsgSymbol->NAME, AsgSymbol->INDEX );
        }
        
        NumberEvent++;
      }

      if ( NumberEvent > 1 )
      {
        VasyPrintf( stdout, "ERROR more than one event %d\n", NumberEvent );
      }

      NumberInSens    = 0;
      NumberNotInSens = 0;
      NumberSupport   = 0;
/*
**  Count the number of signals that appear in the sensitivity list
*/
      for ( ScanChain  = Support[ AsgPos ];
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        VpnSymbol = (vpnsym *)ScanChain->DATA;
        VpnDeclar = VpnSymbol->DECL;

        if ( ( VpnDeclar->TYPE != VPN_DECLAR_DEFINE   ) &&
             ( VpnDeclar->TYPE != VPN_DECLAR_VARIABLE ) )
        {
          Element = searchauthelem( VasyHashSens, (char*)VpnSymbol );

          if ( Element != (authelem *)0 ) NumberInSens++;
          else                            NumberNotInSens++;
        }

        if ( VpnDeclar->TYPE != VPN_DECLAR_DEFINE )
        {
          NumberSupport++;
        }
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
          SymInfo->TYPE     = VASY_SYM_TRISTATE;
          SymInfo->TRS_TYPE = VEX_TRISTATE_ID;

          break;
        }
        else
        if ( ScanChain->DATA == VEX_ATOM_WEAK_ZERO )
        {
          SymInfo->TYPE     = VASY_SYM_TRISTATE;
          SymInfo->TRS_TYPE = VEX_WEAK_ZERO_ID;

          break;
        }
        else
        if ( ScanChain->DATA == VEX_ATOM_WEAK_ONE )
        {
          SymInfo->TYPE     = VASY_SYM_TRISTATE;
          SymInfo->TRS_TYPE = VEX_WEAK_ONE_ID;

          break;
        }
      }
/*
**  Check if there is a Cycle in the assignation
*/
      Element2 = searchauth2elem( HashSupport, 
                                  (char*)&Support[ AsgPos ], (char*)AsgSymbol );

      if ( ( Element2        != (auth2elem *)0       ) &&
           ( IsVasyVpnActInfoArith( Flags[ AsgPos ] ) ) )
      {
        VasyPrintf( stdout, "ERROR arithmetic loop !\n" ); continue;
      }

      if ( ( Element2      != (auth2elem *)0   ) &&
           ( SymInfo->TYPE == VASY_SYM_TRISTATE ) )
      {
        TristateLoop = 1;

        if ( IsVasyDebugLevel0() )
        {
          VasyPrintf( stdout, "WARNING tristate loop !\n" );
        }
      }
      else
      {
        TristateLoop = 0;
      }

      if ( NumberInSens == 0 )
      {
/*
**  None support's signal appear in the sensitivity list
*/
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "None support's signal appear in the sensitivity list\n" );
        }

        if ( Element2 != (auth2elem *)0 )
        {
/*
**  None support's signal in the sens list and there is a cyle => ERROR !
*/
          VasyPrintf( stdout, "ERROR none support'signal in the sens list and a cycle\n" );
          continue;
        }
        else
        if ( ( NumberSupport   == 0 ) ||
             ( NumberNotInSens == 0 ) )
        {
          if ( NumberSupport == 0 )
          {
/*
**  Support is empty and there is no cycle => Assign constant value 
*/
            if ( SymInfo->TYPE != VASY_SYM_TRISTATE )
            {
              SymInfo->TYPE = VASY_SYM_CONSTANT;
            }
          }
          else
          {
/*
**  Support is not empty and there is no cycle => Assign variable only
*/
            if ( SymInfo->TYPE != VASY_SYM_TRISTATE )
            {
              SymInfo->TYPE = VASY_SYM_COMBINATORIAL;
            }
          }
        }
        else
        {
/*
**  None support's signal in the sensitivity list => ERROR !
*/
          VasyPrintf( stdout, "ERROR none support'signal in the sensitivity list\n" );
          continue;
        }
      }
      else
      if ( NumberNotInSens == 0 )
      {
/*
**  All signals appear in the sensitivity list
*/
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "All signals appear in the sensitivity list\n" );
        }

        if ( Element2 == (auth2elem *)0 )
        {
/*
** There is no cycle => Assign dataflow
*/
          if ( SymInfo->TYPE != VASY_SYM_TRISTATE )
          {
            SymInfo->TYPE = VASY_SYM_COMBINATORIAL;
          }
        }
        else
        {
/*
** There is a cycle, should be a register (May be a Latch)
*/
          if ( SymInfo->TYPE != VASY_SYM_TRISTATE )
          {
            SymInfo->TYPE = VASY_SYM_REGISTER;
          }
        }
      }
      else
      {
/*
** Some signals appear in the sensitivity list => registers !
*/
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "Some signals appear in the sensitivity list !\n" );
        }

        if ( Element2 == (auth2elem *)0 )
        {
/*
**  Some signals in the sens list and there is no cyle => ERROR !
*/
          VasyPrintf( stdout, "ERROR Some signals are missing in the sens list" );
          VasyDisplayLines( VpnProc->LINE );
          fprintf( stdout, "\n" );

          continue;
        }
        else
        {
/*
**  There is a cycle, assign register (May be a D-register)
*/
          if ( SymInfo->TYPE != VASY_SYM_TRISTATE )
          {
            SymInfo->TYPE = VASY_SYM_REGISTER;
          }
        }
      }

      if ( ( SymInfo->TYPE == VASY_SYM_TRISTATE ) ||
           ( SymInfo->TYPE == VASY_SYM_REGISTER ) )
      {
        if ( TristateLoop )
        {
          SymInfo->TYPE = VASY_SYM_REGISTER;
          SensSymbol    = (vpnsym *)0;

          VasyOneWaitAssignVpnSymbol( VpnFigure, VpnProc  , VpnTrans, 
                                     VpnAction, AsgSymbol, SensSymbol );

          if ( SymInfo->TYPE != VASY_SYM_COMBINATORIAL )
          {
            VasyPrintf( stdout, "ERROR Tristate loop\n" ); continue;
          }

          SymInfo->TYPE = VASY_SYM_TRISTATE;
        }

        VasyOneWaitAssignVpnSymbol( VpnFigure, VpnProc  , VpnTrans, 
                                   VpnAction, AsgSymbol, SensSymbol );
      }

      if ( SymInfo->TYPE == VASY_SYM_REGISTER )
      {
/*
**  Verify that the follow, set and reset condition's support are included in the sens list 
*/
        /* TO BE DONE 
        if ( SymInfo->VEX_COND != (vexexpr *)0 )
        {
          VexBit     = SymInfo->VEX_COND;
          SupportBit = VasySupportVpnTransBitVex( VpnFigure, VpnProc, VpnTrans, VexBit );

          for ( ScanChain  = SupportBit;
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            VpnSymbol = (vpnsym *)ScanChain->DATA;
            VasyPrintf( stdout, "  + Sup Cond %s %d\n", VpnSymbol->NAME, VpnSymbol->INDEX );
          }

          freechain( SupportBit );
        }
        */

        if ( SymInfo->VEX_DATA != (vexexpr *)0 )
        {
/*
**  Verify that there is no variable in the data support
*/
          VexBit     = SymInfo->VEX_DATA;
          SupportBit = VasySupportVpnTransBitVex( VpnFigure, VpnProc, VpnTrans, VexBit );

          for ( ScanChain  = SupportBit;
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            VpnSymbol = (vpnsym *)ScanChain->DATA;
            VasyPrintf( stdout, "  + Sup Data %s %d\n", VpnSymbol->NAME, VpnSymbol->INDEX );
          }

          freechain( SupportBit );
        }
      }
      else
      if ( ( SymInfo->TYPE == VASY_SYM_COMBINATORIAL ) ||
           ( SymInfo->TYPE == VASY_SYM_TRISTATE      ) )
      {
/*
** Should verify again the sensitivity list !!
*/
        if ( NumberNotInSens != 0 )
        {
          if ( IsVasyDebugLevel0() )
          {
            VasyPrintf( stdout, "ERROR Some signals are missing in the sens list\n" );
          }
        }
      }
    }
  }
/*
**  Destroy Bdd Circuit
*/
  VasyVexBddDestroyBddCircuit( VpnFigure, VpnProc );

  resetauthtable( VasyHashSens      );
  resetauthtable( VasyHashDefine    );
  resetauthtable( VasyHashBitVecDef );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyOneWaitVpnTrans %s\n", VpnTrans->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyOneWaitVpnProc                    |
|                                                             |
\------------------------------------------------------------*/

void VasyOneWaitVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *WaitTrans;
  vpntrans_list *ActionTrans;
  vpnplace_list *SkipPlace;
  vpnarc        *VpnArc;
  vasyprocinfo   *ProcInfo;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyOneWaitVpnProc %s\n", VpnProc->NAME );
  }

  WaitTrans = VpnProc->ELABO;
/*
**  Sensitivity list is empty never waked up, could be a constant initialisation
*/
  if ( WaitTrans->WAIT_SYM == (chain_list *)0 )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "  +++ Sensitivity list is empty never waked up !\n" );
    }

    ProcInfo = GetVasyVpnProcInfo( VpnProc );
    ProcInfo->TYPE = VASY_PROC_UNUSED;

    return;
  }

  VpnArc      = GetVpnArc( WaitTrans->PLACE_OUT );
  SkipPlace   = GetVpnArcTargetPlace( VpnArc );
  VpnArc      = GetVpnArc( SkipPlace->TRANS_OUT );
  ActionTrans = GetVpnArcTargetTrans( VpnArc );

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  +++ Wait Trans %s\n"  , WaitTrans->NAME );
    VasyPrintf( stdout, "  +++ Action Trans %s\n", ActionTrans->NAME );
  }
/*
**  Initialize all action's for support computation
*/
  VasySupportCreateVpnTrans( VpnFigure, VpnProc, ActionTrans );
/*
**  Hash and compute the support and cycle of all actions
*/
  VasySupportVpnTrans( VpnFigure, VpnProc, ActionTrans, 1 );
/*
**  Compute all the used symbols for all actions
*/
  VasySupportUsedVpnTrans( VpnFigure, VpnProc, ActionTrans );
/*
**  Compute register symbol for all actions 
*/
  VasyOneWaitVpnTrans( VpnFigure, VpnProc, WaitTrans, ActionTrans );

  ProcInfo = GetVasyVpnProcInfo( VpnProc );
  ProcInfo->TYPE = VASY_PROC_DATA_FLOW;
/*
**  Reset all support information for all actions
*/
  VasySupportResetVpnTrans( VpnFigure, VpnProc, ActionTrans );
/*
**  Hash and compute the support and cycle of all actions
*/
  VasySupportVpnTrans( VpnFigure, VpnProc, ActionTrans, 0 );
/*
**  Compute all the used symbols for all actions
*/
  VasySupportUsedVpnTrans( VpnFigure, VpnProc, ActionTrans );
/*
**  Synthesis of all actions        
*/
  VasySynthesisOneWaitVpnProc( VpnFigure, RtlFigure, VpnProc );
/*
**  Destroy all action's information
*/
  VasySupportDestroyVpnTrans( VpnFigure, VpnProc, ActionTrans );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyOneWaitVpnProc %s\n", VpnProc->NAME );
  }
}
