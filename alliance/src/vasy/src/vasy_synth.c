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
| File    :                  vasy_synth.c                     |
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
# include "vasy_support.h"
# include "vasy_shared.h"
# include "vasy_synth.h"
# include "vasy_simprtl.h"

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

  static authtable   *VasyHashWait   = (authtable   *)0;
  static rtlfig_list *VasyFigure     = (rtlfig_list *)0;
  static rtlasg_list *VasyAssign     = (rtlasg_list *)0;
  static long         VasyNumberCond = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyGetFsmStateName                      |
|                                                             |
\------------------------------------------------------------*/

static char *VasyGetFsmStateName( VpnProc, WaitTrans )

  vpnproc_list  *VpnProc;
  vpntrans_list *WaitTrans;
{
  authelem *Element;
  char      Buffer[ 128 ];
  int       Index;

  Element = searchauthelem( VasyHashWait, (char *)WaitTrans );

  if ( Element == (authelem *)0 )
  {
    strcpy( Buffer, WaitTrans->NAME );

    for ( Index = 0; Buffer[ Index ] != '\0'; Index++ )
    {
      if ( Buffer[ Index ] == '.' ) Buffer[ Index ] = '_';
    }

    Element = addauthelem( VasyHashWait, (char *)WaitTrans, (long)namealloc( Buffer ) );
  }

  return( (char *)Element->VALUE );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySynthesisVpnDecl                     |
|                                                             |
\------------------------------------------------------------*/

void VasySynthesisVpnDecl( VpnFigure, RtlFigure )

  vpnfig_list *VpnFigure;
  rtlfig_list *RtlFigure;
{
  vpndecl_list *VpnDeclar;
  vpnsym       *VpnSymbol;
  rtldecl_list *RtlDeclar;
  rtlsym       *RtlSymbol;
  vexexpr      *VexAtom;
  vexexpr      *VexInit;
  int           ScanPos;
  int           RtlType;
  int           RtlKind;
  int           RtlBase;
  int           RtlDir;
  int           Type;
/*
**  Convert Vpn Declaration to Rtl Declaration
*/
  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
  {
    for ( VpnDeclar  = VpnFigure->DECLAR[ Type ];
          VpnDeclar != (vpndecl_list *)0;
          VpnDeclar  = VpnDeclar->NEXT )
    {
      RtlKind = VpnDeclar->KIND;
      RtlBase = VpnDeclar->BASE;
      RtlDir  = VpnDeclar->DIR;
  
      VexAtom = dupvexexpr( VpnDeclar->VEX_ATOM );

      if ( Type != VPN_DECLAR_DEFINE ) VexInit = dupvexexpr( VpnDeclar->VEX_INIT );
      else                             VexInit = (vexexpr *)0;
  
      if ( RtlKind == VPN_KIND_BUS      ) RtlKind = RTL_KIND_BUS;
      else
      if ( RtlKind == VPN_KIND_REGISTER ) RtlKind = RTL_KIND_REGISTER;
      else                                RtlKind = RTL_KIND_NONE;
  
      if ( RtlDir == VPN_DIR_IN  ) RtlDir = RTL_DIR_IN;
      else
      if ( RtlDir == VPN_DIR_OUT ) RtlDir = RTL_DIR_OUT;
      else
                                   RtlDir = RTL_DIR_INOUT;

      if ( Type == VPN_DECLAR_PORT     ) RtlType = RTL_DECLAR_PORT;
      else
      if ( Type == VPN_DECLAR_CONSTANT ) RtlType = RTL_DECLAR_CONSTANT;
      else
                                         RtlType = RTL_DECLAR_SIGNAL;

      RtlDeclar = addrtldecl( RtlFigure, VexAtom, RtlType );

      if ( Type == VPN_DECLAR_DEFINE )
      {
        SetVasyRtlDeclarDefine( RtlDeclar );
      }
  
      RtlDeclar->BASE = RtlBase;
      RtlDeclar->KIND = RtlKind;
      RtlDeclar->DIR  = RtlDir;
  
      RtlDeclar->VEX_INIT = VexInit;

      for ( ScanPos = 0; ScanPos < VexAtom->WIDTH; ScanPos++ )
      {
        RtlSymbol = &RtlDeclar->DECL_SYM[ ScanPos ];
        VpnSymbol = &VpnDeclar->DECL_SYM[ ScanPos ];

        RtlSymbol->INIT  = VpnSymbol->INIT;
        RtlSymbol->DRIVE = VpnSymbol->DRIVE;
        RtlSymbol->EFFEC = VpnSymbol->EFFEC;
        RtlSymbol->EVENT = VpnSymbol->EVENT;
      }
    }
  }

  for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
  {
    RtlDeclar = RtlFigure->DECLAR[ Type ];

    if ( RtlDeclar != (rtldecl_list *)0 )
    {
      RtlFigure->DECLAR[ Type ] = (rtldecl_list *)reverse( (chain_list *)RtlDeclar );
    }
  }

  VasyNumberCond = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySynthesisVpnModel                    |
|                                                             |
\------------------------------------------------------------*/

void VasySynthesisVpnModel( VpnFigure, RtlFigure )

  vpnfig_list *VpnFigure;
  rtlfig_list *RtlFigure;
{
  vpnmod_list   *VpnModel;
  rtlmod_list   *RtlModel;
  vpnport_list  *VpnPort;
  vpngen_list   *VpnGen;
  rtlport_list  *RtlPort;
  rtlgen_list   *RtlGen;
  vexexpr       *Atom;
  vexexpr       *Expr;
  unsigned char  Dir;
  unsigned char  Base;

  for ( VpnModel  = VpnFigure->MODEL;
        VpnModel != (vpnmod_list *)0;
        VpnModel  = VpnModel->NEXT )
  {
    RtlModel = addrtlmod( RtlFigure, VpnModel->NAME );

    for ( VpnPort  = VpnModel->PORT;
          VpnPort != (vpnport_list *)0;
          VpnPort  = VpnPort->NEXT )
    {
      Atom = dupvexexpr( VpnPort->VEX_ATOM );
      Base = VpnPort->BASE;
      Dir  = VpnPort->DIR;

      if ( Dir == VPN_DIR_IN  ) Dir = RTL_DIR_IN;
      else
      if ( Dir == VPN_DIR_OUT ) Dir = RTL_DIR_OUT;
      else
                                Dir = RTL_DIR_INOUT;

      RtlPort = addrtlmodport( RtlFigure, RtlModel, Atom, Base, Dir );
    }

    for ( VpnGen  = VpnModel->GENERIC;
          VpnGen != (vpngen_list *)0;
          VpnGen  = VpnGen->NEXT )
    {
      Atom = dupvexexpr( VpnGen->VEX_ATOM );
      Expr = dupvexexpr( VpnGen->VEX_EXPR );
      Base = VpnGen->BASE;

      RtlGen = addrtlmodgen( RtlFigure, RtlModel, Atom, Expr, Base );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySynthesisVpnInstance                 |
|                                                             |
\------------------------------------------------------------*/

void VasySynthesisVpnInstance( VpnFigure, RtlFigure )

  vpnfig_list *VpnFigure;
  rtlfig_list *RtlFigure;
{
  vpnins_list   *VpnInst;
  rtlins_list   *RtlInst;
  rtlmod_list   *RtlModel;
  vpnmap_list   *VpnMap;
  rtlmap_list   *RtlMap;
  vpngen_list   *VpnGen;
  rtlgen_list   *RtlGen;
  vexexpr       *Formal;
  vexexpr       *Actual;
  vexexpr       *Atom;
  vexexpr       *Expr;

  for ( VpnInst  = VpnFigure->INSTANCE;
        VpnInst != (vpnins_list *)0;
        VpnInst  = VpnInst->NEXT )
  {
    RtlModel = searchrtlmod( RtlFigure, VpnInst->MODEL->NAME );
    RtlInst  = addrtlins( RtlFigure, VpnInst->NAME, RtlModel );

    for ( VpnMap  = VpnInst->MAP;
          VpnMap != (vpnmap_list *)0;
          VpnMap  = VpnMap->NEXT )
    {
      Formal = dupvexexpr( VpnMap->VEX_FORMAL );
      Actual = dupvexexpr( VpnMap->VEX_ACTUAL );

      RtlMap = addrtlinsmap( RtlFigure, RtlInst, Formal, Actual );
    }

    for ( VpnGen  = VpnInst->GENERIC;
          VpnGen != (vpngen_list *)0;
          VpnGen  = VpnGen->NEXT )
    {
      Atom = dupvexexpr( VpnGen->VEX_ATOM );
      Expr = dupvexexpr( VpnGen->VEX_EXPR );

      RtlGen = addrtlinsgen( RtlFigure, RtlInst, Atom, Expr );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|               VasySynthesisAddRtlDefine                     |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasySynthesisAddRtlDefine( RtlFigure, RtlDeclar, VexCond )

  rtlfig_list  *RtlFigure;
  rtldecl_list *RtlDeclar;
  vexexpr      *VexCond;
{
  rtldecl_list *DefDeclar;
  rtlasg_list  *DefAssign;
  vexexpr      *VexAtom;
  vexexpr      *VexAtomZero;
  vexexpr      *VexAtomOne;
  char          Buffer[ 32 ];

  sprintf( Buffer, "rtldef_%ld", VasyNumberCond++ );
  VexAtom = createvexatombit( Buffer );

  DefDeclar = addrtldecl( RtlFigure, VexAtom, RTL_DECLAR_SIGNAL );

  DefDeclar->BASE = getvextypescalar( RtlDeclar->BASE );
  DefDeclar->KIND = RTL_KIND_NONE;
  DefDeclar->DIR  = RTL_DIR_INOUT;

  VexAtomOne  = createvexatomlit( VEX_ATOM_ONE  );
  VexAtomZero = createvexatomlit( VEX_ATOM_ZERO );

  DefAssign = addrtlasg( RtlFigure, dupvexexpr( VexAtom ), RTL_ASG_CONDITIONAL );

  addrtlasgbivex( RtlFigure, DefAssign, VexCond     , VexAtomOne , RTL_BIVEX_CONDITIONAL );
  addrtlasgbivex( RtlFigure, DefAssign, (vexexpr *)0, VexAtomZero, RTL_BIVEX_CONDITIONAL );

  return( dupvexexpr( VexAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|               VasySynthesisSubstIftVexExpr                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasySynthesisSubstIftVexExpr( RtlFigure, RtlDeclar, Expr )

  rtlfig_list  *RtlFigure;
  rtldecl_list *RtlDeclar;
  vexexpr      *Expr;
{
  chain_list  *ScanOper;
  vexexpr     *Operand;
  vexexpr     *VexCond;
  vexexpr     *VexData;
  long         Oper;
  int          ScanPos;
  int          Width;

  if ( ! IsVexNodeAtom( Expr ) )
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      Operand = GetVexOperand( ScanOper );
      Operand = VasySynthesisSubstIftVexExpr( RtlFigure, RtlDeclar, Operand );
      SetVexOperand( ScanOper, Operand );
    }

    if ( IsVexNodeOper( Expr ) )
    {
      Oper = GetVexOperValue( Expr );

      if ( Oper == VEX_IFT )
      {
        ScanOper = Expr->OPERAND;
        VexCond  = GetVexOperand( ScanOper );
        SetVexOperand( ScanOper, (vexexpr *)0 );

        ScanOper = ScanOper->NEXT;
        VexData  = GetVexOperand( ScanOper );
        SetVexOperand( ScanOper, (vexexpr *)0 );

        Width = Expr->WIDTH;

        freevexexpr( Expr );

        VexCond = VasySynthesisAddRtlDefine( RtlFigure, RtlDeclar, VexCond );

        if ( Width > 1 )
        {
          Expr = createvexoper( VEX_CONCAT, Width );

          for ( ScanPos = 0; ScanPos < Width; ScanPos++ )
          {
            addvexhexpr( Expr, dupvexexpr( VexCond ) );
          }
        }
        else
        {
          Expr = dupvexexpr( VexCond );
        }

        Expr = createvexbinexpr( VEX_AND, Expr->WIDTH, Expr, VexData );
      }

      return( Expr );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasySynthesisDupVexExpr                     |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasySynthesisDupVexExpr( RtlFigure, RtlDeclar, Expr )

  rtlfig_list  *RtlFigure;
  rtldecl_list *RtlDeclar;
  vexexpr      *Expr;
{
  Expr = dupvexexpr( Expr );

  if ( RtlDeclar != (rtldecl_list *)0 )
  {
    Expr = VasySynthesisSubstIftVexExpr( RtlFigure, RtlDeclar, Expr );
  }

  Expr = simpvexexpr( Expr );

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|               VasySynthesisOneWaitVpnTrans                  |
|                                                             |
\------------------------------------------------------------*/

static void VasySynthesisOneWaitVpnTrans( VpnFigure, RtlFigure, VpnTrans )

  vpnfig_list   *VpnFigure;
  rtlfig_list   *RtlFigure;
  vpntrans_list *VpnTrans;
{
  vpnact_list    *VpnAction;
  vasyactinfo     *ActInfo;
  vasysyminfo     *SymInfo;
  vasybivex_list  *SymBiVex;
  rtldecl_list   *RtlDeclar; 
  char           *Flags;
  vpndecl_list   *AsgDeclar;
  vpnsym         *AsgSymbol;
  vexexpr        *AsgAtom;
  vexexpr        *RtlAtom;
  vexexpr        *AsgExpr;
  vexexpr        *VexCond;
  vexexpr        *VexData;
  char           *AtomName;
  int             AsgMin;
  int             AsgMax;
  int             AsgWidth;
  int             AsgIndex;
  int             AsgPos;
  rtlasg_list    *RtlAssign;
  int             RtlType;
  int             Used;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasySynthesisOneWaitVpnTrans %s\n", VpnTrans->NAME );
  }

  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    ActInfo = GetVasyVpnActInfo( VpnAction );
    Flags   = GetVasyVpnActInfoFlags( VpnAction );

    AsgAtom   = VpnAction->VEX_ATOM;
    AsgExpr   = VpnAction->VEX_EXPR;
    AtomName  = GetVexAtomValue( AsgAtom );
    AsgDeclar = searchvpndeclall( VpnFigure, AtomName );

    AsgMin   = getvexvectormin( AsgAtom );
    AsgMax   = getvexvectormax( AsgAtom );
    AsgWidth = AsgAtom->WIDTH;

    RtlDeclar = searchrtldecl( RtlFigure, AtomName );
    Used      = 0;

    if ( ! ActInfo->SPLIT )
    {
      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgPos = getvexvectorpos( AsgAtom, AsgIndex );

        if ( IsVasyVpnActInfoUsed( Flags[ AsgPos ] ) )
        {
          Used = 1; break;
        }
      }

      if ( ! Used ) continue;

      RtlAtom = dupvexexpr( AsgAtom );

      RtlAssign = addrtlasg( RtlFigure, RtlAtom, RTL_ASG_COMBINATORIAL );
      RtlAssign->VEX_DATA = VasySynthesisDupVexExpr( RtlFigure, RtlDeclar, AsgExpr );

      SetVasyRtlDeclType( RtlDeclar, VASY_RTL_DECL_COMBINATORIAL );
    }
    else
    {
      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
        AsgPos    = getvexvectorpos( AsgAtom, AsgIndex );

        SymInfo = &ActInfo->SYM_INFO[ AsgPos ];

        if ( ! IsVasyVpnActInfoUsed( Flags[ AsgPos ] ) ) continue;

        Used = 1;

        if ( IsVasyDebugLevel0() )
        {
          VasyPrintf( stdout, "--> Symbol %s %d\n", AsgSymbol->NAME, AsgSymbol->INDEX );
          VasyPrintf( stdout, "+++ Type %s\n", VASY_SYM_TYPE[ SymInfo->TYPE ] );
          VasyPrintf( stdout, "+++ Data " );
          viewvexexprboundln( SymInfo->VEX_DATA );

          for ( SymBiVex  = SymInfo->BIVEX;
                SymBiVex != (vasybivex_list *)0;
                SymBiVex  = SymBiVex->NEXT )
          {
            VasyPrintf( stdout, "+++ %s ", VASY_BIVEX_TYPE[ SymBiVex->TYPE ] );
            VasyPrintf( stdout, "+++ Cond " );
            viewvexexprboundln( SymBiVex->VEX_COND );
            VasyPrintf( stdout, "+++ Data " );
            viewvexexprboundln( SymBiVex->VEX_DATA );
          }
        }
  
        if ( AsgSymbol->INDEX == -1 )
        {
          RtlAtom = createvexatombit( AsgSymbol->NAME );
        }
        else
        {
          RtlAtom = createvexatomvec( AsgSymbol->NAME, AsgSymbol->INDEX, AsgSymbol->INDEX );
        }

        if ( ( SymInfo->TYPE == VASY_SYM_REGISTER ) ||
             ( SymInfo->TYPE == VASY_SYM_TRISTATE ) )
        {
          RtlType = RTL_ASG_REGISTER; 

          if ( SymInfo->TYPE == VASY_SYM_TRISTATE )
          {
            if ( SymInfo->TRS_TYPE == VEX_TRISTATE_ID  )
            {
              RtlType = RTL_ASG_TRISTATE;
              SetVasyRtlDeclType( RtlDeclar, VASY_RTL_DECL_TRISTATE );
            }
            else
            if ( SymInfo->TRS_TYPE == VEX_WEAK_ONE_ID  )
            {
              RtlType = RTL_ASG_PULL_UP;
              SetVasyRtlDeclType( RtlDeclar, VASY_RTL_DECL_PULL_UP );
            }
            else
            if ( SymInfo->TRS_TYPE == VEX_WEAK_ZERO_ID )
            {
              RtlType = RTL_ASG_PULL_DOWN;
              SetVasyRtlDeclType( RtlDeclar, VASY_RTL_DECL_PULL_DOWN );
            }
          }
          else
          {
            SetVasyRtlDeclType( RtlDeclar, VASY_RTL_DECL_REGISTER );
          }

          RtlAssign = addrtlasg( RtlFigure, RtlAtom, RtlType );

          RtlType =  RTL_ASG_REGISTER_NONE;

          if ( SymInfo->REG_TYPE == VASY_SYM_REGISTER_SYNC  ) RtlType = RTL_ASG_REGISTER_SYNC;
          else
          if ( SymInfo->REG_TYPE == VASY_SYM_REGISTER_ASYNC ) RtlType = RTL_ASG_REGISTER_ASYNC;
          else
          if ( SymInfo->REG_TYPE == VASY_SYM_REGISTER_MIXED ) RtlType = RTL_ASG_REGISTER_MIXED;

          RtlAssign->REG_TYPE = RtlType;

          for ( SymBiVex  = SymInfo->BIVEX;
                SymBiVex != (vasybivex_list *)0;
                SymBiVex  = SymBiVex->NEXT )
          {
            VexCond = SymBiVex->VEX_COND;
            VexData = SymBiVex->VEX_DATA;

            VexCond = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexCond );
            VexData = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexData );

            addrtlasgbivex( RtlFigure, RtlAssign, VexCond, VexData, SymBiVex->TYPE );
          }
        }
        else
        if ( SymInfo->TYPE == VASY_SYM_COMBINATORIAL )
        {
          VexData = SymInfo->VEX_DATA;
          VexData = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexData );

          RtlAssign = addrtlasg( RtlFigure, RtlAtom, RTL_ASG_COMBINATORIAL );
          RtlAssign->VEX_DATA = VexData;
        }
      }
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasySynthesisOneWaitVpnTrans %s\n", VpnTrans->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|               VasySynthesisOneWaitVpnProc                   |
|                                                             |
\------------------------------------------------------------*/

void VasySynthesisOneWaitVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *WaitTrans;
  vpntrans_list *VpnTrans;
  vpnplace_list *SkipPlace;
  vpnarc        *VpnArc;
  vasyprocinfo   *ProcInfo;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasySynthesisOneWaitVpnProc %s\n", VpnProc->NAME );
  }

  ProcInfo = GetVasyVpnProcInfo( VpnProc );
  WaitTrans = VpnProc->ELABO;

  VpnArc    = GetVpnArc( WaitTrans->PLACE_OUT );
  SkipPlace = GetVpnArcTargetPlace( VpnArc );
  VpnArc    = GetVpnArc( SkipPlace->TRANS_OUT );
  VpnTrans  = GetVpnArcTargetTrans( VpnArc );

  VasySynthesisOneWaitVpnTrans( VpnFigure, RtlFigure, VpnTrans );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasySynthesisOneWaitVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|              VasySynthesisMultiWaitVpnProc                  |
|                                                             |
\------------------------------------------------------------*/

void VasySynthesisMultiWaitVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list    *WaitTrans;
  vpntrans_list    *DefTrans;
  vpntrans_list    *AsgTrans;
  vpntrans_list    *EndTrans;
  vpnplace_list    *WaitPlace;
  vpnplace_list    *EndPlace;
  vpnact_list      *VpnAction;
  vpnarc           *VpnArc;
  chain_list       *ScanChain;
  vasywaitinfo     *WaitInfo;
  rtlfsm_list      *RtlFsm;
  rtlfsmstate_list *RtlState;
  rtlfsmstate_list *RtlStateFrom;
  rtlfsmstate_list *RtlStateTo;
  rtlfsmtrans_list *RtlTrans;
  vasyprocinfo     *ProcInfo;
  vasybivex_list   *ScanBiVex;
  vexexpr          *VexCond;
  vexexpr          *VexData;
  vexexpr          *AsgAtom;
  vexexpr          *AsgExpr;
  char             *StateName;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasySynthesisMultiWaitVpnProc %s\n", VpnProc->NAME );
  }

  if ( VasyHashWait == (authtable *)0 )
  {
    VasyHashWait = createauthtable( 50 );
  }

  ProcInfo = GetVasyVpnProcInfo( VpnProc );

  RtlFsm = addrtlfsm( RtlFigure, VpnProc->NAME );

  for ( ScanBiVex  = ProcInfo->BIVEX;
        ScanBiVex != (vasybivex_list *)0;
        ScanBiVex  = ScanBiVex->NEXT )
  {
    VexCond = ScanBiVex->VEX_COND;
    VexData = ScanBiVex->VEX_DATA;

    VexCond = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexCond );
    VexData = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexData );

    addrtlfsmbivex( RtlFigure, RtlFsm, VexCond, VexData, ScanBiVex->TYPE );
  }
/*
**  Create a state in the FSM for all wait transitions
*/
  for ( WaitTrans  = VpnProc->TRANS;
        WaitTrans != (vpntrans_list *)0;
        WaitTrans  = WaitTrans->NEXT )
  {
    if ( WaitTrans->TYPE != VPN_TRANS_INF_WAIT ) continue;

    StateName = VasyGetFsmStateName( VpnProc, WaitTrans );
    RtlState  = addrtlfsmstate( RtlFigure, RtlFsm, StateName );

    WaitInfo = GetVasyVpnWaitInfo( WaitTrans );
    DefTrans = WaitInfo->DEF_TRANS;

    if ( DefTrans != (vpntrans_list *)0 )
    {
      VasySynthesisOneWaitVpnTrans( VpnFigure, RtlFigure, DefTrans );
    }
  }
/*
**  Create all transition for each state of the FSM
*/
  for ( WaitTrans  = VpnProc->TRANS;
        WaitTrans != (vpntrans_list *)0;
        WaitTrans  = WaitTrans->NEXT )
  {
    if ( WaitTrans->TYPE != VPN_TRANS_INF_WAIT ) continue;

    StateName    = VasyGetFsmStateName( VpnProc, WaitTrans );
    RtlStateFrom = searchrtlfsmstate( RtlFigure, RtlFsm, StateName );

    WaitInfo  = GetVasyVpnWaitInfo( WaitTrans );
    WaitPlace = WaitInfo->WAIT_PLACE;

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

      StateName  = VasyGetFsmStateName( VpnProc, EndTrans );
      RtlStateTo = searchrtlfsmstate( RtlFigure, RtlFsm, StateName );

      VexCond = AsgTrans->VEX_GUARD;
      VexCond = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexCond );

      RtlTrans = addrtlfsmtrans( RtlFigure, RtlFsm, RtlStateFrom, RtlStateTo, VexCond );

      if ( IsVasyDebugLevel0() )
      {
        VasyPrintf( stdout, "Arc %s ->  %s\nCond ", RtlStateFrom->NAME, RtlStateTo->NAME );
        viewvexexprboundln( VexCond );
      }

      for ( VpnAction  = AsgTrans->ACT;
            VpnAction != (vpnact_list *)0;
            VpnAction  = VpnAction->NEXT )
      {
        AsgAtom = dupvexexpr( VpnAction->VEX_ATOM );

        AsgExpr = VpnAction->VEX_EXPR;
        AsgExpr = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, AsgExpr );

        addrtlfsmtransasg( RtlFigure, RtlFsm, RtlTrans, AsgAtom, AsgExpr );
      }
    }
  }

  resetauthtable( VasyHashWait );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasySynthesisMultiWaitVpnProc %s\n", VpnProc->NAME );
  }
}
/*------------------------------------------------------------\
|                                                             |
|             VasySynthesisSimpleAssignVpnProc                |
|                                                             |
\------------------------------------------------------------*/

static void VasySynthesisSimpleAssignVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list  *VpnTrans;
  vpnact_list    *VpnAction;
  rtldecl_list   *RtlDeclar; 
  vexexpr        *AsgAtom;
  vexexpr        *RtlAtom;
  vexexpr        *AsgExpr;
  char           *AtomName;
  rtlasg_list    *RtlAssign;
  int             SimpleAsg;
/*
** Treat all simple assign
*/
  if ( ( ! IsVpnProcWithSelect( VpnProc   ) ) &&
       ( ! IsVpnProcConditionnal( VpnProc ) ) )
  {
    SimpleAsg = 1;
  }
  else
  {
    SimpleAsg = 0;
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( ( VpnTrans->TYPE == VPN_TRANS_SUP_WAIT ) ||
         ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT ) ) continue;

    for ( VpnAction  = VpnTrans->ACT;
          VpnAction != (vpnact_list *)0;
          VpnAction  = VpnAction->NEXT )
    {
      if ( ( SimpleAsg                             ) || 
           ( VpnAction->TYPE == VPN_ACT_ASG_DEFINE ) )
      {
        SetVasyVpnTransAsgDefine( VpnTrans );

        AsgAtom   = VpnAction->VEX_ATOM;
        AsgExpr   = VpnAction->VEX_EXPR;
        AtomName  = GetVexAtomValue( AsgAtom );
        RtlDeclar = searchrtldecl( RtlFigure, AtomName );
        
        RtlAtom   = dupvexexpr( AsgAtom );
        RtlAssign = addrtlasg( RtlFigure, RtlAtom, RTL_ASG_COMBINATORIAL );
        RtlAssign->VEX_DATA = VasySynthesisDupVexExpr( RtlFigure, RtlDeclar, AsgExpr );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|             VasySynthesisConditionnalScanPlace              |
|                                                             |
\------------------------------------------------------------*/

static void VasySynthesisConditionnalScanPlace( CondPlace )

  vpnplace_list *CondPlace;
{
  vpnplace_list *AsgPlace;
  vpnplace_list *IfPlace;
  vpntrans_list *SwapTrans;
  vpntrans_list *AsgTrans;
  vpntrans_list *TrueTrans;
  vpntrans_list *FalseTrans;
  vpnarc        *VpnArc;
  vexexpr       *VexCond;
  vexexpr       *VexData;

  if ( CondPlace->TYPE == VPN_PLACE_IF )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "CondPlace %s\n", CondPlace->NAME );
    }

    VpnArc     = GetVpnArc( CondPlace->TRANS_OUT );
    FalseTrans = GetVpnArcTargetTrans( VpnArc );
    VpnArc     = GetVpnArc( CondPlace->TRANS_OUT->NEXT );
    TrueTrans  = GetVpnArcTargetTrans( VpnArc );

    VpnArc    = GetVpnArc( TrueTrans->PLACE_OUT );
    AsgPlace  = GetVpnArcTargetPlace( VpnArc );
    VpnArc    = GetVpnArc( AsgPlace->TRANS_OUT );
    AsgTrans  = GetVpnArcTargetTrans( VpnArc );

    if ( ( AsgPlace->TYPE != VPN_PLACE_ASSIGN  ) ||
         ( IsVasyVpnTransAsgDefine( AsgTrans ) ) )
    {
      if ( IsVasyDebugLevel1() )
      {
        VasyPrintf( stdout, " +++ Swap True/False\n" );
      }

      SwapTrans  = TrueTrans;
      TrueTrans  = FalseTrans;
      FalseTrans = SwapTrans;

      VpnArc    = GetVpnArc( TrueTrans->PLACE_OUT );
      AsgPlace  = GetVpnArcTargetPlace( VpnArc );
      VpnArc    = GetVpnArc( AsgPlace->TRANS_OUT );
      AsgTrans  = GetVpnArcTargetTrans( VpnArc );
    }

    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "True : %s, False : %s, Asg : %s\n",
                  TrueTrans->NAME, FalseTrans->NAME, AsgTrans->NAME );
    }

    VexCond = TrueTrans->VEX_GUARD;
    VexData = AsgTrans->ACT->VEX_EXPR;

    VexCond = VasySynthesisDupVexExpr( VasyFigure, (rtldecl_list *)0, VexCond );
    VexData = VasySynthesisDupVexExpr( VasyFigure, (rtldecl_list *)0, VexData );

    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, " > Cond : " );
      viewvexexprboundln( VexCond );
      VasyPrintf( stdout, " > Data : " );
      viewvexexprboundln( VexData );
    }

    addrtlasgbivex( VasyFigure, VasyAssign, VexCond, VexData, RTL_BIVEX_CONDITIONAL );

    VpnArc   = GetVpnArc( FalseTrans->PLACE_OUT );
    IfPlace  = GetVpnArcTargetPlace( VpnArc );

    if ( IfPlace->TYPE == VPN_PLACE_ASSIGN ) 
    {
      VpnArc    = GetVpnArc( IfPlace->TRANS_OUT );
      AsgTrans  = GetVpnArcTargetTrans( VpnArc );

      if ( IsVasyVpnTransAsgDefine( AsgTrans ) )
      {
        VpnArc    = GetVpnArc( AsgTrans->PLACE_OUT );
        IfPlace   = GetVpnArcTargetPlace( VpnArc );
      }
      else
      {
        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, "Final ELSE %s\n", IfPlace->NAME );
        }

        VexData   = AsgTrans->ACT->VEX_EXPR;
        VexData   = VasySynthesisDupVexExpr( VasyFigure, (rtldecl_list *)0, VexData );

        if ( IsVasyDebugLevel1() )
        {
          VasyPrintf( stdout, " > Data : " );
          viewvexexprboundln( VexData );
        }

        addrtlasgbivex( VasyFigure, VasyAssign, (vexexpr *)0, VexData, RTL_BIVEX_CONDITIONAL );

        return;
      }
    }

    VasySynthesisConditionnalScanPlace( IfPlace );
  }
  else
  {
    VasyPrintf( stdout, "ERROR : Should not happen !!!\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|             VasySynthesisConditionnalVpnProc                |
|                                                             |
\------------------------------------------------------------*/

static void VasySynthesisConditionnalVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpnplace_list *VpnPlace;
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAction;
  vpnarc        *VpnArc;
  vexexpr       *RtlAtom;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasySynthesisConditionnalVpnProc %s\n", VpnProc->NAME );
  }

  VasyFigure = RtlFigure;
/*
** Look for the target assignment
*/
  RtlAtom = (vexexpr *)0;

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    if ( ( VpnTrans->TYPE == VPN_TRANS_INF_WAIT ) ||
         ( VpnTrans->TYPE == VPN_TRANS_SUP_WAIT ) ) continue;

    VpnAction = VpnTrans->ACT;

    if ( (   VpnAction       != (vpnact_list *)0     ) &&
         ( ( VpnAction->TYPE == VPN_ACT_ASG_PORT   ) ||
           ( VpnAction->TYPE == VPN_ACT_ASG_SIGNAL ) ) )
    {
      RtlAtom = dupvexexpr( VpnAction->VEX_ATOM ); break;
    }
  }
/*
** Look for the first place IF
*/
  for ( VpnPlace  = VpnProc->PLACE;
        VpnPlace != (vpnplace_list *)0;
        VpnPlace  = VpnPlace->NEXT )
  {
    if ( VpnPlace->TYPE == VPN_PLACE_PROCESS ) break;
  }

  do
  {
    VpnArc   = GetVpnArc( VpnPlace->TRANS_OUT );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );
    VpnArc   = GetVpnArc( VpnTrans->PLACE_OUT );
    VpnPlace = GetVpnArcTargetPlace( VpnArc );
  }
  while ( VpnPlace->TYPE != VPN_PLACE_IF );

  VasyAssign = addrtlasg( RtlFigure, RtlAtom, RTL_ASG_CONDITIONAL );

  VasySynthesisConditionnalScanPlace( VpnPlace );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasySynthesisConditionnalVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|             VasySynthesisWithSelectVpnProc                  |
|                                                             |
\------------------------------------------------------------*/

static void VasySynthesisWithSelectVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list  *VpnTrans;
  vpntrans_list  *ElseTrans;
  vpnplace_list  *VpnPlace;
  vpnplace_list  *BeginPlace;
  vpnarc         *VpnArc;
  chain_list     *ScanChain;
  vpnact_list    *VpnAction;
  vexexpr        *AsgAtom;
  vexexpr        *RtlAtom;
  vexexpr        *AsgExpr;
  vexexpr        *ElseData;
  vexexpr        *VexData; 
  vexexpr        *VexCond; 
  char           *AtomName;
  rtlasg_list    *RtlAssign;
  long            MaxNode;
  long            NumberNode;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasySynthesisWithSelectVpnProc %s\n", VpnProc->NAME );
  }
/*
** Look for the place BEGIN_CASE
*/
  for ( BeginPlace  = VpnProc->PLACE;
        BeginPlace != (vpnplace_list *)0;
        BeginPlace  = BeginPlace->NEXT )
  {
    if ( BeginPlace->TYPE == VPN_PLACE_CASE ) break;
  }
/*
** Look for the transition OTHERS/ELSE
*/
  MaxNode    = 0;
  NumberNode = 0;
  ElseTrans  = (vpntrans_list *)0;

  for ( ScanChain  = BeginPlace->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnArc     = GetVpnArc( ScanChain );
    VpnTrans   = GetVpnArcTargetTrans( VpnArc );
    NumberNode = getvexexprnumnode( VpnTrans->VEX_GUARD );

    if ( NumberNode > MaxNode )
    {
      MaxNode   = NumberNode;
      ElseTrans = VpnTrans;
    }
  }
/*
** Create a conditionnal RTL assign
*/
  VpnArc   = GetVpnArc( ElseTrans->PLACE_OUT );
  VpnPlace = GetVpnArcTargetPlace( VpnArc );
  VpnArc   = GetVpnArc( VpnPlace->TRANS_OUT );
  VpnTrans = GetVpnArcTargetTrans( VpnArc );

  VpnAction = VpnTrans->ACT;

  AsgAtom   = VpnAction->VEX_ATOM;
  AsgExpr   = VpnAction->VEX_EXPR;
  ElseData  = AsgExpr;
  AtomName  = GetVexAtomValue( AsgAtom );
  
  RtlAtom   = dupvexexpr( AsgAtom );
  RtlAssign = addrtlasg( RtlFigure, RtlAtom, RTL_ASG_CONDITIONAL );

  for ( ScanChain  = BeginPlace->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    VpnArc   = GetVpnArc( ScanChain );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );

    if ( VpnTrans == ElseTrans ) continue;

    VexCond  = VpnTrans->VEX_GUARD;
    VpnArc   = GetVpnArc( VpnTrans->PLACE_OUT );
    VpnPlace = GetVpnArcTargetPlace( VpnArc );
    VpnArc   = GetVpnArc( VpnPlace->TRANS_OUT );
    VpnTrans = GetVpnArcTargetTrans( VpnArc );
    VexData  = VpnTrans->ACT->VEX_EXPR;

    VexCond = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexCond );
    VexData = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, VexData );

    addrtlasgbivex( RtlFigure, RtlAssign, VexCond, VexData, RTL_BIVEX_CONDITIONAL );
  }

  ElseData = VasySynthesisDupVexExpr( RtlFigure, (rtldecl_list *)0, ElseData );
  addrtlasgbivex( RtlFigure, RtlAssign, (vexexpr *)0, ElseData, RTL_BIVEX_CONDITIONAL );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasySynthesisWithSelectVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|             VasySynthesisCombinatorialVpnProc               |
|                                                             |
\------------------------------------------------------------*/

void VasySynthesisCombinatorialVpnProc( VpnFigure, RtlFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  rtlfig_list  *RtlFigure;
  vpnproc_list *VpnProc;
{
  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasySynthesisCombinatorialVpnProc %s\n", VpnProc->NAME );
  }

  VasySynthesisSimpleAssignVpnProc( VpnFigure, RtlFigure, VpnProc );

  if ( IsVpnProcWithSelect( VpnProc ) )
  {
    VasySynthesisWithSelectVpnProc( VpnFigure, RtlFigure, VpnProc );
  }
  else
  if ( IsVpnProcConditionnal( VpnProc ) )
  {
    VasySynthesisConditionnalVpnProc( VpnFigure, RtlFigure, VpnProc );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasySynthesisCombinatorialVpnProc %s\n", VpnProc->NAME );
  }
}
