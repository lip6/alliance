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
| File    :                  vasy_support.c                   |
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

# include "vasy_error.h"
# include "vasy_debug.h"
# include "vasy_shared.h"
# include "vasy_support.h"

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

  static authtable   *VasyHashBitVec    = (authtable   *)0;
  static authtable   *VasyHashAssign    = (authtable   *)0;
  static authtable   *VasyHashReadError = (authtable   *)0;
  static auth2table  *VasyHash2Support  = (auth2table  *)0;
  static auth2table  *VasyHash2Literal  = (auth2table  *)0;
  static auth2table  *VasyHash2Event    = (auth2table  *)0;

  static vpnfig_list *VasyFigure      = (vpnfig_list *)0;
  static vpnsym      *VasySymbolError = (vpnsym      *)0;
  static int          VasyArithmetic  = 0;

  static chain_list  **VasySupport = (chain_list   **)0;
  static chain_list  **VasyLiteral = (chain_list   **)0;
  static chain_list  **VasyEvent   = (chain_list   **)0;

  static short         VasyMaxPosition = 0;

  static authtable   *VasyHashSupportBit = (authtable  *)0;
  static chain_list  *VasySupportBit     = (chain_list *)0;
  static authtable   *VasyHashEventBit   = (authtable  *)0;
  static chain_list  *VasyEventBit       = (chain_list *)0;

  static vpnline_list *VasyVpnLine       = (vpnline_list *)0;

  static char VasyOperSupportMode[ VEX_MAX_OPERATOR ] =
  {
    VASY_SUPPORT_ERROR,    /* VEX_CONCAT    */
    VASY_SUPPORT_LOGIC,    /* VEX_NOT       */
    VASY_SUPPORT_ARITH,    /* VEX_NEG       */
    VASY_SUPPORT_LOGIC,    /* VEX_EVENT     */
    VASY_SUPPORT_LOGIC,    /* VEX_OR        */
    VASY_SUPPORT_LOGIC,    /* VEX_AND       */
    VASY_SUPPORT_LOGIC,    /* VEX_XOR       */
    VASY_SUPPORT_LOGIC,    /* VEX_NOR       */
    VASY_SUPPORT_LOGIC,    /* VEX_NAND      */
    VASY_SUPPORT_LOGIC,    /* VEX_NXOR      */
    VASY_SUPPORT_ARITH,    /* VEX_EQ        */
    VASY_SUPPORT_ARITH,    /* VEX_NE        */
    VASY_SUPPORT_ARITH,    /* VEX_LT        */
    VASY_SUPPORT_ARITH,    /* VEX_LE        */
    VASY_SUPPORT_ARITH,    /* VEX_GT        */
    VASY_SUPPORT_ARITH,    /* VEX_GE        */
    VASY_SUPPORT_ARITH,    /* VEX_ADD       */
    VASY_SUPPORT_ARITH,    /* VEX_SUB       */
    VASY_SUPPORT_ARITH,    /* VEX_MUL       */
    VASY_SUPPORT_ARITH,    /* VEX_DIV       */
    VASY_SUPPORT_ARITH,    /* VEX_EXP       */
    VASY_SUPPORT_ARITH,    /* VEX_MOD       */
    VASY_SUPPORT_ARITH,    /* VEX_REM       */
    VASY_SUPPORT_ERROR,    /* VEX_TO        */
    VASY_SUPPORT_ERROR,    /* VEX_DOWNTO    */
    VASY_SUPPORT_ERROR,    /* VEX_INDEX     */
    VASY_SUPPORT_ERROR,    /* VEX_LEFT      */
    VASY_SUPPORT_ERROR,    /* VEX_RIGHT     */
    VASY_SUPPORT_ERROR,    /* VEX_LOW       */
    VASY_SUPPORT_ERROR,    /* VEX_HIGH      */
    VASY_SUPPORT_ERROR,    /* VEX_LENGTH    */
    VASY_SUPPORT_ERROR,    /* VEX_RANGE     */
    VASY_SUPPORT_ERROR,    /* VEX_REV_RANGE */
    VASY_SUPPORT_ERROR,    /* VEX_DRIVER    */
    VASY_SUPPORT_LOGIC,    /* VEX_IFT       */
    VASY_SUPPORT_ERROR,    /* VEX_ARRAY     */
    VASY_SUPPORT_ERROR,    /* VEX_INDEX_N   */
    VASY_SUPPORT_ERROR,    /* VEX_OTHERS    */
    VASY_SUPPORT_ERROR,    /* VEX_NUM_BIT   */
    VASY_SUPPORT_ARITH     /* VEX_ABS       */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   VasySupportViewHashSymbol                 |
|                                                             |
\------------------------------------------------------------*/

/*
static void VasySupportViewHashSymbol( Element )

  authelem *Element;
{
  vpnsym *VpnSymbol;

  VpnSymbol = (vpnsym *)Element->KEY;
  fprintf( stdout, "Sym %s %d\n", VpnSymbol->NAME, VpnSymbol->INDEX );
}
*/

/*------------------------------------------------------------\
|                                                             |
|                   VasySupportViewHashName                   |
|                                                             |
\------------------------------------------------------------*/

/*
static void VasySupportViewHashName( Element )

  authelem *Element;
{
  fprintf( stdout, "%s\n", (char *)Element->KEY );
}
*/

/*------------------------------------------------------------\
|                                                             |
|                   VasySupportViewVpnAct                     |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportViewVpnAct( VpnTrans )

  vpntrans_list *VpnTrans;
{
  chain_list   **Support;
  chain_list   **Literal;
  chain_list   **Event;
  unsigned char *Flags;
  chain_list    *ScanChain;
  vpnact_list   *VpnAction;
  vexexpr       *AsgAtom;
  vexexpr       *AsgExpr;
  vpnsym        *AsgSymbol;
  char          *VexLiteral;
  char          *AtomName;
  int            AsgIndex;
  int            AsgPos;
  int            AsgWidth;
  int            AsgStep;

  VasyPrintf( stdout, "--> VasySupportViewVpnAct\n" );

  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    AsgAtom  = VpnAction->VEX_ATOM;
    AsgExpr  = VpnAction->VEX_EXPR;
    AtomName = GetVexAtomValue( AsgAtom );
    Support  = GetVasyVpnActInfoSupport( VpnAction );
    Literal  = GetVasyVpnActInfoLiteral( VpnAction );
    Event    = GetVasyVpnActInfoEvent( VpnAction );
    Flags    = GetVasyVpnActInfoFlags( VpnAction );
    
    viewvexexprbound( AsgAtom );
    fprintf( stdout, " <<< " );
    viewvexexprboundln( AsgExpr );

    if ( IsVexAtomDown( AsgAtom ) ) AsgStep = -1;
    else                            AsgStep =  1;

    AsgIndex = AsgAtom->LEFT;
    AsgWidth = AsgAtom->WIDTH;

    while ( AsgWidth != 0 )
    {
      AsgPos = getvexvectorpos( AsgAtom, AsgIndex );
      VasyPrintf( stdout, "%s[%d] %x > ", AtomName, AsgIndex, (int)Flags[ AsgPos ] );

      for ( ScanChain  = Support[ AsgPos ];
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        AsgSymbol = (vpnsym *)ScanChain->DATA;
        fprintf( stdout, "%s[%d] ", AsgSymbol->NAME, AsgSymbol->INDEX );
      }

      fprintf( stdout, "\n" );
      VasyPrintf( stdout, "%s[%d] ! ", AtomName, AsgIndex );

      for ( ScanChain  = Literal[ AsgPos ];
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        VexLiteral = (char *)ScanChain->DATA;
        fprintf( stdout, "%s ", VexLiteral );
      }

      fprintf( stdout, "\n" );
      VasyPrintf( stdout, "%s[%d] * ", AtomName, AsgIndex );

      for ( ScanChain  = Event[ AsgPos ];
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        AsgSymbol = (vpnsym *)ScanChain->DATA;
        fprintf( stdout, "%s[%d] ", AsgSymbol->NAME, AsgSymbol->INDEX );
      }

      fprintf( stdout, "\n" );

      AsgIndex += AsgStep;
      AsgWidth--;
    }
  }

  VasyPrintf( stdout, "<-- VasySupportViewVpnAct\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasySupportUsedVpnSymbol                    |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportUsedVpnSymbol( AsgSymbol )

  vpnsym *AsgSymbol;
{
  vpnact_list    *VpnAction;
  vpndecl_list   *AsgDeclar;
  vpnsym         *VpnSymbol;
  vexexpr        *AsgAtom;
  authelem       *Element;
  vasyactinfo     *ActInfo;
  chain_list    **Support;
  unsigned char  *Flags;
  chain_list     *ScanChain;
  int             AsgPos;

  AsgDeclar = AsgSymbol->DECL;

  if ( ! isvextypedivisible( AsgDeclar->BASE ) )
  {
    Element = searchauthelem( VasyHashAssign, AsgSymbol->NAME );
  }
  else
  {
    Element = searchauthelem( VasyHashBitVec, (char*)AsgSymbol );
  }

  if ( Element == (authelem *)0 ) return;

  VpnAction = (vpnact_list *)Element->VALUE;
  ActInfo   = GetVasyVpnActInfo( VpnAction );

  Support   = ActInfo->SUPPORT;
  Flags     = ActInfo->FLAGS;

  AsgAtom = VpnAction->VEX_ATOM;
  AsgPos  = getvexvectorpos( AsgAtom, AsgSymbol->INDEX );

  if ( ! IsVasyVpnActInfoUsed( Flags[ AsgPos ] ) )
  {
    SetVasyVpnActInfoUsed( Flags[ AsgPos ] );

    for ( ScanChain  = Support[ AsgPos ];
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      VpnSymbol = (vpnsym *)ScanChain->DATA;

      if ( VpnSymbol != AsgSymbol )
      {
        VasySupportUsedVpnSymbol( VpnSymbol );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasySupportUsedVpnTrans                     |
|                                                             |
\------------------------------------------------------------*/

void VasySupportUsedVpnTrans( VpnFigure, VpnProc, VpnTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
{
  vasytransinfo   *TransInfo;
  vpnact_list    *VpnAction;
  vasyactinfo     *VasyAction;
  vpndecl_list   *AsgDeclar;
  vpnsym         *VpnSymbol;
  vpnsym         *AsgSymbol;
  vexexpr        *AsgAtom;
  vexexpr        *VexGuard;
  chain_list     *SupportGuard;
  chain_list    **Support;
  chain_list     *ScanChain;
  char           *AtomName;
  unsigned char  *Flags;
  int             AsgIndex;
  int             AsgMin;
  int             AsgMax;
  int             AsgPos;

  TransInfo = GetVasyVpnTransInfo( VpnTrans );

  VasyHashAssign  = TransInfo->HASH_ASSIGN;
  VasyHashBitVec  = TransInfo->HASH_BITVEC;

  VexGuard = VpnTrans->VEX_GUARD;

  if ( VexGuard !=  (vexexpr *)0 )
  {
    SupportGuard = VasySupportVpnTransBitVex( VpnFigure, VpnProc, VpnTrans, VexGuard );

    for ( ScanChain  = SupportGuard;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      VpnSymbol = (vpnsym *)ScanChain->DATA;
      VasySupportUsedVpnSymbol( VpnSymbol );
    }

    freechain( SupportGuard );
  }

  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    if ( ( VpnAction->TYPE == VPN_ACT_ASG_SIGNAL ) ||
         ( VpnAction->TYPE == VPN_ACT_ASG_PORT   ) )
    {
      VasyAction = GetVasyVpnActInfo( VpnAction );
      Support   = VasyAction->SUPPORT;
      Flags     = VasyAction->FLAGS;

      AsgAtom   = VpnAction->VEX_ATOM;
      AtomName  = GetVexAtomValue( AsgAtom );
      AsgDeclar = searchvpndeclall( VpnFigure, AtomName );

      AsgMin = getvexvectormin( AsgAtom );
      AsgMax = getvexvectormax( AsgAtom );

      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
        AsgPos    = getvexvectorpos( AsgAtom, AsgIndex );

        SetVasyVpnActInfoUsed( Flags[ AsgPos ] );

        for ( ScanChain  = Support[ AsgPos ];
              ScanChain != (chain_list *)0;
              ScanChain  = ScanChain->NEXT )
        {
          VpnSymbol = (vpnsym *)ScanChain->DATA;

          if ( VpnSymbol != AsgSymbol )
          {
            VasySupportUsedVpnSymbol( VpnSymbol );
          }
        }
      }
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasySupportViewVpnAct( VpnTrans );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportEventVpnSymbol                  |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportEventVpnSymbol( PosSupp, Position, VpnSymbol )

  int          PosSupp;
  int          Position;
  vpnsym      *VpnSymbol;
{
  auth2elem *Element2;

  Position += PosSupp;
  Element2  = searchauth2elem( VasyHash2Event, (char*)&VasyEvent[ Position ], (char*)VpnSymbol );

  if ( Element2 == (auth2elem *)0 )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "+ " );
    }

    VasyEvent[ Position ] = addchain( VasyEvent[ Position ], VpnSymbol );
    addauth2elem( VasyHash2Event, (char*)&VasyEvent[ Position ], (char*)VpnSymbol, 0 );
  }
  else
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "* " );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "Event[ %d ] <= %s %d\n", 
               Position, VpnSymbol->NAME, VpnSymbol->INDEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportVpnSymbol                       |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportVpnSymbol( PosSupp, Position, VpnSymbol )

  int          PosSupp;
  int          Position;
  vpnsym      *VpnSymbol;
{
  auth2elem *Element2;

  Position += PosSupp;
  Element2  = searchauth2elem( VasyHash2Support, 
                               (char*)&VasySupport[ Position ], (char*)VpnSymbol );

  if ( Element2 == (auth2elem *)0 )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "+ " );
    }

    VasySupport[ Position ] = addchain( VasySupport[ Position ], VpnSymbol );
    addauth2elem( VasyHash2Support, (char*)&VasySupport[ Position ], (char*)VpnSymbol, 0 );
  }
  else
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "* " );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "Support[ %d ] <= %s %d\n", 
             Position, VpnSymbol->NAME, VpnSymbol->INDEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportVexLiteral                      |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportVexLiteral( PosSupp, Position, Literal )

  int   PosSupp;
  int   Position;
  char *Literal;
{
  auth2elem *Element2;

  Position += PosSupp;
  Element2  = searchauth2elem( VasyHash2Literal, 
                               (char*)&VasyLiteral[ Position ], (char*)Literal );

  if ( Element2 == (auth2elem *)0 )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "+ " );
    }

    VasyLiteral[ Position ] = addchain( VasyLiteral[ Position ], Literal );
    addauth2elem( VasyHash2Literal, (char*)&VasyLiteral[ Position ], (char*)Literal, 0 );
  }
  else
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "* " );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "Literal[ %d ] <= %s\n", Position, Literal );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    VasySupportVex                           |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportVex( PosSupp, PosFrom, PosTo, Expr, Mode )

  unsigned short   PosSupp;
  unsigned short   PosFrom;
  unsigned short   PosTo;
  vexexpr         *Expr;
  unsigned char    Mode;
{
  vpnact_list    *VpnAction;
  vpndecl_list   *VpnDeclar;
  vpnsym         *VpnSymbol;
  char           *VexLiteral;
  char           *AtomName;
  chain_list    **DupSupport;
  chain_list    **DupLiteral;
  chain_list    **DupEvent;
  chain_list     *ScanChain;
  authelem       *Element;
  vexexpr        *Operand;
  vexexpr        *Operand2;
  unsigned char   NewMode;
  long            Oper;
  int             LiteralId;
  int             Position;
  int             DupPosition;
  int             Index;

  if ( Expr == (vexexpr *)0 ) return;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "VasySupportVex %d %d %d Mode %d-> \n",
             PosSupp, PosFrom, PosTo, Mode );
    viewvexexprboundln( Expr );
  }

  Oper = -1;

  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( Oper == VEX_CONCAT )
    {
      for ( ScanChain  = Expr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand  = GetVexOperand( ScanChain );

        if ( Mode != VASY_SUPPORT_ARITH )
        {
          PosFrom  = 0;
          PosTo    = Operand->WIDTH - 1;
        }

        VasySupportVex( PosSupp, PosFrom, PosTo, Operand, Mode );

        if ( Mode != VASY_SUPPORT_ARITH ) PosSupp += Operand->WIDTH;
      }
    }
    else
    if ( Oper == VEX_EVENT )
    {
      Expr = GetVexOperand( Expr->OPERAND );
    }
    else
    {
      NewMode = VasyOperSupportMode[ Oper ];

      if ( NewMode == VASY_SUPPORT_ERROR )
      {
        VasyErrorLine( VASY_ERROR_IN_SUPPORT, VasyVpnLine, VEX_OPER_NAME[ Oper ] );
      }
      else
      if ( NewMode == VASY_SUPPORT_ARITH )
      {
        if ( ( Oper == VEX_EQ ) ||
             ( Oper == VEX_NE ) )
        {
          Operand  = GetVexOperand( Expr->OPERAND       );
          Operand2 = GetVexOperand( Expr->OPERAND->NEXT );

          if ( ( Operand->WIDTH  == 1 ) &&
               ( Operand2->WIDTH == 1 ) )
          {
            NewMode = VASY_SUPPORT_LOGIC;
          }
          else
          if ( Operand->WIDTH != Operand2->WIDTH )
          {
            VasyArithmetic = 1;
          }
        }
        else
        {
          VasyArithmetic = 1;
        }
      }

      if ( Mode != VASY_SUPPORT_ARITH ) Mode = NewMode;

      for ( ScanChain  = Expr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand = GetVexOperand( ScanChain );
        VasySupportVex( PosSupp, PosFrom, PosTo, Operand, Mode );
      }
    }
  }

  if ( IsVexNodeAtom( Expr ) )
  {
    AtomName = GetVexAtomValue( Expr );

    if ( IsVexAtomLiteral( Expr ) )
    {
      if ( Mode == VASY_SUPPORT_LOGIC )
      {
        for ( Position = PosFrom; Position <= PosTo; Position++ )
        {
          if ( Position >= Expr->WIDTH ) DupPosition = Expr->WIDTH - 1;
          else                           DupPosition = Position;

          LiteralId = getvexliteralid( AtomName[ DupPosition + 1 ] );

          if ( LiteralId != -1 )
          {
            VexLiteral = VEX_ATOM_BY_ID[ LiteralId ];
            VasySupportVexLiteral( PosSupp, Position, VexLiteral );
          }
        }
      }
      else
      {
        for ( Position = PosFrom; Position <= PosTo; Position++ )
        {
          for ( DupPosition = 0; DupPosition < Expr->WIDTH; DupPosition++ )
          {
            LiteralId = getvexliteralid( AtomName[ DupPosition + 1 ] );

            if ( LiteralId != -1 )
            {
              VexLiteral = VEX_ATOM_BY_ID[ LiteralId ];
              VasySupportVexLiteral( PosSupp, Position, VexLiteral );
            }
          }
        }
      }
      
      return;
    }

    VpnDeclar = searchvpndeclall( VasyFigure, AtomName );
/*
**  Checks the Atom's type
*/
    if ( VpnDeclar->TYPE == VPN_DECLAR_DEFINE )
    {
/*
**  Atom is a define, verify if it is read only (variable register) !
*/
      for ( Position = PosFrom; Position <= PosTo; Position++ )
      {
        if ( Position >= Expr->WIDTH ) DupPosition = Expr->WIDTH - 1;
        else                           DupPosition = Position;

        Index     = getvexvectorindex( Expr, DupPosition );
        VpnSymbol = getvpnsymdecl( VpnDeclar, Index );
        Element   = searchauthelem( VasyHashReadError, (char*)VpnSymbol );

        if ( Element != (authelem *)0 )
        {
          VasyPrintf( stdout, "Error !!! %s %d\n", VpnSymbol->NAME, VpnSymbol->INDEX );
          VasySymbolError = (vpnsym *)Element->VALUE;
        }
      }
/*
**  Atom is a define, duplicate its support list
*/
      VpnAction  = (vpnact_list *)0;
      DupSupport = (chain_list **)0;
      DupLiteral = (chain_list **)0;
      DupEvent   = (chain_list **)0;

      if ( ! isvextypedivisible( VpnDeclar->BASE ) )
      {
/*
**  Atom is a define and its base is not bit_vector
*/
        Element    = searchauthelem( VasyHashAssign, AtomName );
        VpnAction  = (vpnact_list *)Element->VALUE;

        DupSupport = GetVasyVpnActInfoSupport( VpnAction );
        DupLiteral = GetVasyVpnActInfoLiteral( VpnAction );
        DupEvent   = GetVasyVpnActInfoEvent( VpnAction );
      }

      for ( Position = PosFrom; Position <= PosTo; Position++ )
      {
        if ( Position >= Expr->WIDTH ) DupPosition = Expr->WIDTH - 1;
        else                           DupPosition = Position;

        Index     = getvexvectorindex( Expr, DupPosition );
        VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

        if ( isvextypedivisible( VpnDeclar->BASE ) )
        {
/*
**  Atom is a define and its base is bit_vector
*/
          Element    = searchauthelem( VasyHashBitVec, (char*)VpnSymbol );
          VpnAction  = (vpnact_list *)Element->VALUE;

          DupSupport = GetVasyVpnActInfoSupport( VpnAction );
          DupLiteral = GetVasyVpnActInfoLiteral( VpnAction );
          DupEvent   = GetVasyVpnActInfoEvent( VpnAction );
        }
/*
**  Duplicate support of Atom in support list
*/
        if ( Mode == VASY_SUPPORT_LOGIC )
        {
          DupPosition = getvexvectorpos( VpnAction->VEX_ATOM, Index );
        
          for ( ScanChain  = DupSupport[ DupPosition ];
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            VpnSymbol = (vpnsym *)ScanChain->DATA;
            VasySupportVpnSymbol( PosSupp, Position, VpnSymbol );
          }

          for ( ScanChain  = DupLiteral[ DupPosition ];
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            VexLiteral = (char *)ScanChain->DATA;
            VasySupportVexLiteral( PosSupp, Position, VexLiteral );
          }

          for ( ScanChain  = DupEvent[ DupPosition ];
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            VpnSymbol = (vpnsym *)ScanChain->DATA;
            VasySupportEventVpnSymbol( PosSupp, Position, VpnSymbol );
          }
        }
        else
        {
          for ( DupPosition = 0; DupPosition < Expr->WIDTH; DupPosition++ )
          {
            for ( ScanChain  = DupSupport[ DupPosition ];
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              VpnSymbol = (vpnsym *)ScanChain->DATA;
              VasySupportVpnSymbol( PosSupp, Position, VpnSymbol );
            }

            for ( ScanChain  = DupLiteral[ DupPosition ];
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              VexLiteral = (char *)ScanChain->DATA;
              VasySupportVexLiteral( PosSupp, Position, VexLiteral );
            }

            for ( ScanChain  = DupEvent[ DupPosition ];
                  ScanChain != (chain_list *)0;
                  ScanChain  = ScanChain->NEXT )
            {
              VpnSymbol = (vpnsym *)ScanChain->DATA;
              VasySupportEventVpnSymbol( PosSupp, Position, VpnSymbol );
            }
          }
        }
      }
    }

    if ( ( VpnDeclar->TYPE == VPN_DECLAR_PORT     ) ||
         ( VpnDeclar->TYPE == VPN_DECLAR_SIGNAL   ) ||
         ( VpnDeclar->TYPE == VPN_DECLAR_DEFINE   ) ||
         ( VpnDeclar->TYPE == VPN_DECLAR_VARIABLE ) )
    {
/*
**  Atom is a port, a signal a define or a variable, add it in the support list
*/
      if ( Mode == VASY_SUPPORT_LOGIC )
      {
        for ( Position = PosFrom; Position <= PosTo; Position++ )
        {
          if ( Position >= Expr->WIDTH ) DupPosition = Expr->WIDTH - 1;
          else                           DupPosition = Position;

          Index     = getvexvectorindex( Expr, DupPosition );
          VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

          VasySupportVpnSymbol( PosSupp, Position, VpnSymbol );

          if ( Oper == VEX_EVENT )
          {
            VasySupportEventVpnSymbol( PosSupp, Position, VpnSymbol );
          }
        }
      }
      else
      {
        for ( Position = PosFrom; Position <= PosTo; Position++ )
        {
          for ( DupPosition = 0; DupPosition < Expr->WIDTH; DupPosition++ )
          {
            Index     = getvexvectorindex( Expr, DupPosition );
            VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

            VasySupportVpnSymbol( PosSupp, Position, VpnSymbol );

            if ( Oper == VEX_EVENT )
            {
              VasySupportEventVpnSymbol( PosSupp, Position, VpnSymbol );
            }
          }
        }
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VasySupportVpnAct                      |
|                                                             |
\------------------------------------------------------------*/

static chain_list **VasySupportVpnAct( VpnFigure, VpnProc, VpnAction )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnact_list  *VpnAction;
{
  vasybivex_list *ScanBiVex;
  vasyactinfo    *ActInfo;
  vasysyminfo    *SymInfo;
  vexexpr        *AsgAtom;
  vexexpr        *AsgExpr;
  int             PosFrom;
  int             PosTo;
  int             AsgPos;
  int             Position;
  unsigned char  *Flags;

  ActInfo = GetVasyVpnActInfo( VpnAction );
  
  VasyVpnLine = VpnAction->LINE;

  AsgAtom = VpnAction->VEX_ATOM;
  AsgExpr = VpnAction->VEX_EXPR;

  VasyFigure      = VpnFigure;
  VasySymbolError = (vpnsym *)0;
  VasySupport     = GetVasyVpnActInfoSupport( VpnAction );
  VasyLiteral     = GetVasyVpnActInfoLiteral( VpnAction );
  VasyEvent       = GetVasyVpnActInfoEvent( VpnAction );
  VasyArithmetic  = 0;

  VasyMaxPosition = AsgAtom->WIDTH;

  PosFrom = 0;
  PosTo   = 0;

  if ( ActInfo->SPLIT )
  {
    for ( AsgPos = 0; AsgPos < AsgAtom->WIDTH; AsgPos++ )
    {
      SymInfo = &ActInfo->SYM_INFO[ AsgPos ];

      if ( SymInfo->TYPE != VASY_SYM_UNKNOWN )
      {
        if ( PosFrom < PosTo )
        {
          if ( IsVasyDebugLevel1() )
          {
            VasyPrintf( stdout, ">> Support Of [%d-%d] ", PosFrom, PosTo );
            viewvexexprbound( AsgAtom );
            fprintf( stdout, " : " );
            viewvexexprboundln( AsgExpr );
          }

          VasySupportVex( PosFrom, PosFrom, PosTo - 1, AsgExpr, VASY_SUPPORT_LOGIC );
        }

        if ( SymInfo->TYPE != VASY_SYM_UNUSED )
        {
          if ( IsVasyDebugLevel1() )
          {
            VasyPrintf( stdout, ">> Support Of [%d-%d] %d ", AsgPos, AsgPos, SymInfo->TYPE);
            viewvexexprbound( AsgAtom );
            fprintf( stdout, " : " );

            viewvexexprboundln( SymInfo->VEX_DATA   );

            for ( ScanBiVex  = SymInfo->BIVEX;
                  ScanBiVex != (vasybivex_list *)0;
                  ScanBiVex  = ScanBiVex->NEXT )
            {
              viewvexexprboundln( ScanBiVex->VEX_COND );
              viewvexexprboundln( ScanBiVex->VEX_DATA );
            }
          }

          VasySupportVex( AsgPos, 0, 0, SymInfo->VEX_DATA, VASY_SUPPORT_LOGIC );

          for ( ScanBiVex  = SymInfo->BIVEX;
                ScanBiVex != (vasybivex_list *)0;
                ScanBiVex  = ScanBiVex->NEXT )
          {
            VasySupportVex( AsgPos, 0, 0, ScanBiVex->VEX_COND, VASY_SUPPORT_LOGIC );
            VasySupportVex( AsgPos, 0, 0, ScanBiVex->VEX_DATA, VASY_SUPPORT_LOGIC );
          }
        }

        PosFrom = AsgPos + 1;
        PosTo   = PosFrom;
      }
      else
      {
        PosTo++;
      }
    }
  }
  else
  {
    PosTo = AsgAtom->WIDTH;
  }

  if ( PosFrom < PosTo )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, ">> Support Of [%d-%d] ", PosFrom, PosTo );
      viewvexexprbound( AsgAtom );
      fprintf( stdout, " : " );
      viewvexexprboundln( AsgExpr );
    }

    VasySupportVex( PosFrom, PosFrom, PosTo - 1, AsgExpr, VASY_SUPPORT_LOGIC );
  }

  if ( VasySymbolError != (vpnsym *)0 )
  {
    VasyPrintf( stdout, "Error !! READ VARIABLE %s %d\n", 
               VasySymbolError->NAME, VasySymbolError->INDEX );
  }

  if ( VasyArithmetic )
  {
    Flags = GetVasyVpnActInfoFlags( VpnAction );

    for ( Position = PosFrom; Position < PosTo; Position++ )
    {
      SetVasyVpnActInfoArith( Flags[ Position ] );
    }
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "<<\n" );
  }

  return( VasySupport );
}

/*------------------------------------------------------------\
|                                                             |
|                 VasySupportMergeVpnTrans                    |
|                                                             |
\------------------------------------------------------------*/

void VasySupportMergeVpnTrans( VpnFigure, VpnProc, DefTrans, VpnTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *DefTrans;
  vpntrans_list *VpnTrans;
{
  vasytransinfo  *TransInfo;
  vpnact_list   *VpnAction;
  vpnsym        *AsgSymbol;
  vpndecl_list  *AsgDeclar;
  vexexpr       *AsgAtom;
  char          *AtomName;
  int            AsgIndex;
  int            AsgMin;
  int            AsgMax;
  authtable     *HashAssign;
  authtable     *HashBitVec;

  TransInfo = GetVasyVpnTransInfo( VpnTrans );

  HashAssign = TransInfo->HASH_ASSIGN;
  HashBitVec = TransInfo->HASH_BITVEC;
/*
**  Initialize all actions and put them in assign hash table
*/
  for ( VpnAction  = DefTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    AsgAtom   = VpnAction->VEX_ATOM;
    AtomName  = GetVexAtomValue( AsgAtom );
    AsgDeclar = searchvpndeclall( VpnFigure, AtomName );
/*
**  Add assign to bit_vector symbol V in hash table
*/
    if ( isvextypedivisible( AsgDeclar->BASE ) )
    {
      AsgMin = getvexvectormin( AsgAtom );
      AsgMax = getvexvectormax( AsgAtom );

      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
        addauthelem( HashBitVec, (char*)AsgSymbol, (long)VpnAction );
      }
    }
/*
**  Add assign to symbol V in hash table
*/
    addauthelem( HashAssign, AtomName, (long)VpnAction );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasySupportCreateVpnTrans                   |
|                                                             |
\------------------------------------------------------------*/

void VasySupportCreateVpnTrans( VpnFigure, VpnProc, VpnTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
{
  vasytransinfo  *TransInfo;
  vpnact_list   *VpnAction;
  vpnsym        *AsgSymbol;
  vpndecl_list  *AsgDeclar;
  vexexpr       *AsgAtom;
  char          *AtomName;
  int            AsgIndex;
  int            AsgMin;
  int            AsgMax;
  authtable     *HashAssign;
  authtable     *HashBitVec;

  TransInfo = VasyAddVpnTransInfo( VpnTrans );

  HashAssign = TransInfo->HASH_ASSIGN;
  HashBitVec = TransInfo->HASH_BITVEC;
/*
**  Initialize all actions and put them in assign hash table
*/
  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    AsgAtom   = VpnAction->VEX_ATOM;
    AtomName  = GetVexAtomValue( AsgAtom );
    AsgDeclar = searchvpndeclall( VpnFigure, AtomName );
/*
**  Add Vasy Action to each Vpn Action
*/
    VasyAddVpnActInfo( VpnAction );
/*
**  Add assign to bit_vector symbol V in hash table
*/
    if ( isvextypedivisible( AsgDeclar->BASE ) )
    {
      AsgMin = getvexvectormin( AsgAtom );
      AsgMax = getvexvectormax( AsgAtom );

      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
        addauthelem( HashBitVec, (char*)AsgSymbol, (long)VpnAction );
      }
    }
/*
**  Add assign to symbol V in hash table
*/
    addauthelem( HashAssign, AtomName, (long)VpnAction );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasySupportResetVpnTrans                    |
|                                                             |
\------------------------------------------------------------*/

void VasySupportResetVpnTrans( VpnFigure, VpnProc, VpnTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
{
  vasytransinfo  *TransInfo;
  vpnact_list   *VpnAction;
  vexexpr       *AsgAtom;
  int            AsgIndex;
  chain_list   **Support;
  chain_list   **Event;
  chain_list   **Literal;
  unsigned char *Flags;

  TransInfo = GetVasyVpnTransInfo( VpnTrans );

  resetauth2table( TransInfo->HASH_SUPPORT );
  resetauth2table( TransInfo->HASH_LITERAL );
  resetauth2table( TransInfo->HASH_EVENT   );
/*
**  Initialize all actions and put them in assign hash table
*/
  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    AsgAtom = VpnAction->VEX_ATOM;

    Support  = GetVasyVpnActInfoSupport( VpnAction );
    Literal  = GetVasyVpnActInfoLiteral( VpnAction );
    Event    = GetVasyVpnActInfoEvent( VpnAction );
    Flags    = GetVasyVpnActInfoFlags( VpnAction );

    for ( AsgIndex = 0; AsgIndex < AsgAtom->WIDTH; AsgIndex++ )
    {
      freechain( Support[ AsgIndex ] );
      freechain( Event[ AsgIndex ] );
      freechain( Literal[ AsgIndex ] );

      Support[ AsgIndex ] = (chain_list *)0;
      Literal[ AsgIndex ] = (chain_list *)0;
      Event  [ AsgIndex ] = (chain_list *)0;
      Flags  [ AsgIndex ] = 0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 VasySupportDestroyVpnTrans                  |
|                                                             |
\------------------------------------------------------------*/

void VasySupportDestroyVpnTrans( VpnFigure, VpnProc, VpnTrans )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
{
  vpnact_list *VpnAction;

  VasyDelVpnTransInfo( VpnTrans );
/*
**  Del Vasy Action to each Vpn Action
*/
  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    VasyDelVpnActInfo( VpnAction );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportEventBitVpnSymbol               |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportEventBitVpnSymbol( VpnSymbol )

  vpnsym *VpnSymbol;
{
  authelem *Element;

  Element = searchauthelem( VasyHashEventBit, (char*)VpnSymbol );

  if ( Element == (authelem *)0 )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "+ " );
    }

    VasyEventBit = addchain( VasyEventBit, VpnSymbol );
    addauthelem( VasyHashEventBit, (char*)VpnSymbol, 0 );
  }
  else
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "* " );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "EventBit <= %s %d\n", VpnSymbol->NAME, VpnSymbol->INDEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportEventBitVex                     |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportEventBitVex( Expr )

  vexexpr *Expr;
{
  vpndecl_list   *VpnDeclar;
  vpnsym         *VpnSymbol;
  char           *AtomName;
  chain_list     *ScanChain;
  vexexpr        *Operand;
  long            Oper;
  int             Index;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "VasySupportEventBitVex -> \n" );
    viewvexexprboundln( Expr );
  }

  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( Oper == VEX_EVENT )
    {
      Expr = GetVexOperand( Expr->OPERAND );

      AtomName  = GetVexAtomValue( Expr );
      VpnDeclar = searchvpndeclall( VasyFigure, AtomName );

      Index     = getvexvectorindex( Expr, 0 );
      VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

      VasySupportEventBitVpnSymbol( VpnSymbol );
    }
    else
    {
      for ( ScanChain  = Expr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand = GetVexOperand( ScanChain );
        VasySupportEventBitVex( Operand );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|              VasySupportVpnTransEventBitVex                 |
|                                                             |
\------------------------------------------------------------*/

chain_list *VasySupportVpnTransEventBitVex( VpnFigure, VpnProc, VexBit )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vexexpr       *VexBit;
{
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasySupportVpnTransEventBitVex\n" );
  }

  VasyFigure = VpnFigure;

  if ( VasyHashEventBit == (authtable *)0 )
  {
    VasyHashEventBit = createauthtable( 50 );
  }

  VasyEventBit = (chain_list *)0;
  VasySupportEventBitVex( VexBit );

  resetauthtable( VasyHashEventBit );

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  <-- VasySupportVpnTransEventBitVex\n" );
  }

  return( VasyEventBit );
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportBitVpnSymbol                    |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportBitVpnSymbol( VpnSymbol )

  vpnsym *VpnSymbol;
{
  authelem *Element;

  Element = searchauthelem( VasyHashSupportBit, (char*)VpnSymbol );

  if ( Element == (authelem *)0 )
  {
    if ( IsVasyDebugLevel1() )
    {
      VasyPrintf( stdout, "+ " );
    }

    VasySupportBit = addchain( VasySupportBit, VpnSymbol );
    addauthelem( VasyHashSupportBit, (char*)VpnSymbol, 0 );
  }
  else
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "* " );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "SupportBit <= %s %d\n", VpnSymbol->NAME, VpnSymbol->INDEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportBitVex                          |
|                                                             |
\------------------------------------------------------------*/

static void VasySupportBitVex( Expr )

  vexexpr *Expr;
{
  vpnact_list    *VpnAction;
  vpndecl_list   *VpnDeclar;
  vpnsym         *VpnSymbol;
  char           *AtomName;
  chain_list    **Support;
  chain_list     *ScanChain;
  authelem       *Element;
  vexexpr        *Operand;
  long            Oper;
  int             Position;
  int             Index;
  unsigned char   Mode;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "VasySupportBitVex -> \n" );
    viewvexexprboundln( Expr );
  }

  if ( IsVexNodeAtom( Expr ) )
  {
    AtomName = GetVexAtomValue( Expr );

    if ( IsVexAtomLiteral( Expr ) ) return;

    VpnDeclar = searchvpndeclall( VasyFigure, AtomName );
/*
**  Checks the Atom's type
*/
    if ( VpnDeclar->TYPE == VPN_DECLAR_DEFINE )
    {
/*
**  Atom is a define (and width = 1), duplicate its support list
*/
      Index     = getvexvectorindex( Expr, 0 );
      VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

      if ( ! isvextypedivisible( VpnDeclar->BASE ) )
      {
/*
**  Atom is a define and its base is not bit_vector
*/
        Element   = searchauthelem( VasyHashAssign, AtomName );
        VpnAction = (vpnact_list *)Element->VALUE;
      }
      else
      {
/*
**  Atom is a define and its base is bit_vector
*/
        Element   = searchauthelem( VasyHashBitVec, (char*)VpnSymbol );
        VpnAction = (vpnact_list *)Element->VALUE;
      }

      Support   = GetVasyVpnActInfoSupport( VpnAction );
      Position  = getvexvectorpos( VpnAction->VEX_ATOM, Index );
/*
**  Duplicate support of Atom in support list
*/
      for ( ScanChain  = Support[ Position ];
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        VpnSymbol = (vpnsym *)ScanChain->DATA;
        VasySupportBitVpnSymbol( VpnSymbol );
      }
    }

    if ( ( VpnDeclar->TYPE == VPN_DECLAR_PORT     ) ||
         ( VpnDeclar->TYPE == VPN_DECLAR_SIGNAL   ) ||
         ( VpnDeclar->TYPE == VPN_DECLAR_DEFINE   ) ||
         ( VpnDeclar->TYPE == VPN_DECLAR_VARIABLE ) )
    {
/*
**  Atom is a port, a signal a define or a variable, add it in the support list
*/
      Index     = getvexvectorindex( Expr, 0 );
      VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

      VasySupportBitVpnSymbol( VpnSymbol );
    }
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );
    Mode = VasyOperSupportMode[ Oper ];

    if ( ( Oper        == VEX_EQ ) &&
         ( Expr->WIDTH == 1      ) )
    {
      Mode = VASY_SUPPORT_LOGIC;
    }

    if ( ( Mode == VASY_SUPPORT_ERROR ) ||
         ( Mode == VASY_SUPPORT_ARITH ) )
    {
      VasyErrorLine( VASY_ERROR_IN_SUPPORT, VasyVpnLine, VEX_OPER_NAME[ Oper ] );
    }

    for ( ScanChain  = Expr->OPERAND;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Operand = GetVexOperand( ScanChain );
      VasySupportBitVex( Operand );
    }
  }
}


/*------------------------------------------------------------\
|                                                             |
|                  VasySupportVpnTransBitVex                  |
|                                                             |
\------------------------------------------------------------*/

chain_list *VasySupportVpnTransBitVex( VpnFigure, VpnProc, VpnTrans, VexBit )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
  vexexpr       *VexBit;
{
  vasytransinfo  *TransInfo;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasySupportVpnTransBitVex\n" );
  }

  VasyFigure = VpnFigure;
  TransInfo = GetVasyVpnTransInfo( VpnTrans );

  VasyHashAssign  = TransInfo->HASH_ASSIGN;
  VasyHashBitVec  = TransInfo->HASH_BITVEC;

  if ( VasyHashSupportBit == (authtable *)0 )
  {
    VasyHashSupportBit = createauthtable( 100 );
  }

  VasySupportBit = (chain_list *)0;
  VasySupportBitVex( VexBit );

  resetauthtable( VasyHashSupportBit );

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  <-- VasySupportVpnTransBitVex\n" );
  }

  return( VasySupportBit );
}

/*------------------------------------------------------------\
|                                                             |
|                  VasySupportVpnTrans                        |
|                                                             |
\------------------------------------------------------------*/

void VasySupportVpnTrans( VpnFigure, VpnProc, VpnTrans, CheckVar )

  vpnfig_list   *VpnFigure;
  vpnproc_list  *VpnProc;
  vpntrans_list *VpnTrans;
  int            CheckVar;
{
  vasytransinfo  *TransInfo;
  vpnact_list   *VpnAction;
  vpnact_list   *VpnAction2;
  vpnact_list   *DefAction;
  vpnsym        *AsgSymbol;
  vpnsym        *DefSymbol;
  vpnsym        *AsgSymbol2;
  vpndecl_list  *AsgDeclar;
  vpndecl_list  *DefDeclar;
  vpndecl_list  *AsgDeclar2;
  vexexpr       *AsgAtom;
  vexexpr       *DefAtom;
  vexexpr       *AsgAtom2;
  char          *AtomName;
  char          *AtomName2;
  authelem      *Element;
  auth2elem     *Element2;
  chain_list   **Support;
  chain_list   **Support2;
  chain_list   **DefSupport;
  chain_list   **DefLiteral;
  chain_list    *ScanChain;
  int            AsgIndex;
  int            AsgIndex2;
  int            DefIndex;
  int            AsgMin;
  int            AsgMin2;
  int            AsgMax;
  int            AsgMax2;
  int            AsgPos;
  int            AsgPos2;
  int            DefPos;

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasySupportVpnTrans %s\n", VpnTrans->NAME );
  }

  TransInfo = GetVasyVpnTransInfo( VpnTrans );

  VasyHashAssign   = TransInfo->HASH_ASSIGN;
  VasyHashBitVec   = TransInfo->HASH_BITVEC;
  VasyHash2Support = TransInfo->HASH_SUPPORT;
  VasyHash2Literal = TransInfo->HASH_LITERAL;
  VasyHash2Event   = TransInfo->HASH_EVENT;

  if ( VasyHashReadError == (authtable *)0 )
  {
    VasyHashReadError = createauthtable( 100 );
  }
/*
**  For all assignment compute support
*/
  for ( VpnAction  = VpnTrans->ACT;
        VpnAction != (vpnact_list *)0;
        VpnAction  = VpnAction->NEXT )
  {
    VasySupportVpnAct( VpnFigure, VpnProc, VpnAction );

    if ( ( CheckVar                                ) &&
         ( VpnAction->TYPE == VPN_ACT_ASG_VARIABLE ) )
    {
/*
**  Check if a variable depends on itself
*/
      Support  = GetVasyVpnActInfoSupport( VpnAction );
      AsgAtom  = VpnAction->VEX_ATOM;
      DefAtom  = VpnAction->VEX_EXPR;

      AtomName  = GetVexAtomValue( AsgAtom );
      AsgDeclar = searchvpndeclall( VpnFigure, AtomName );

      AtomName  = GetVexAtomValue( DefAtom );
      DefDeclar = searchvpndeclall( VpnFigure, AtomName );

      AsgMin = getvexvectormin( AsgAtom );
      AsgMax = getvexvectormax( AsgAtom );

      for ( AsgIndex = AsgMin; AsgIndex <= AsgMax; AsgIndex++ )
      {
        AsgSymbol = getvpnsymdecl( AsgDeclar, AsgIndex );
        AsgPos    = getvexvectorpos( AsgAtom, AsgIndex );

        Element2  = searchauth2elem( VasyHash2Support, 
                                     (char*)&Support[ AsgPos ], (char*)AsgSymbol );

        if ( Element2 != (auth2elem *)0 )
        {
/*
**  The variable depends on itself, hash all define of its support (unreadable)
*/
          if ( IsVasyDebugLevel0() )
          {
            VasyPrintf( stdout, "!!! variable %s %d depends on itself !\n",
                       AsgSymbol->NAME, AsgSymbol->INDEX );
          }

          DefIndex   = getvexvectorindex( DefAtom, AsgPos );
          DefSymbol  = getvpnsymdecl( DefDeclar, DefIndex );
          DefPos     = getvexvectorpos( DefAtom, DefIndex );

          if ( isvextypedivisible( DefDeclar->BASE ) )
          {
            Element = searchauthelem( VasyHashBitVec, (char*)DefSymbol );
          }
          else
          {
            Element = searchauthelem( VasyHashAssign, DefSymbol->NAME );
          }

          DefAction  = (vpnact_list *)Element->VALUE;
          DefSupport = GetVasyVpnActInfoSupport( DefAction );
          DefLiteral = GetVasyVpnActInfoLiteral( DefAction );

          for ( ScanChain  = DefSupport[ DefPos ];
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            delauth2elem( VasyHash2Support, 
                          (char*)&DefSupport[ DefPos ], (char*)ScanChain->DATA );
          }

          for ( ScanChain  = DefLiteral[ DefPos ];
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            delauth2elem( VasyHash2Literal, 
                          (char*)&DefLiteral[ DefPos ], (char*)ScanChain->DATA );
          }

          freechain( DefSupport[ DefPos ] );
          DefSupport[ DefPos ] = (chain_list *)0;

          freechain( DefLiteral[ DefPos ] );
          DefLiteral[ DefPos ] = (chain_list *)0;

          addauth2elem( VasyHash2Support, (char*)&DefSupport[ DefPos ], (char*)AsgSymbol, 0 );
          DefSupport[ DefPos ] = addchain( DefSupport[ DefPos ], AsgSymbol );

          for ( ScanChain  = Support[ AsgPos ];
                ScanChain != (chain_list *)0;
                ScanChain  = ScanChain->NEXT )
          {
            AsgSymbol2 = (vpnsym *)ScanChain->DATA;

            if ( AsgSymbol2 == DefSymbol ) continue;

            AsgDeclar2 = AsgSymbol2->DECL;

            if ( AsgDeclar2->TYPE == VPN_DECLAR_DEFINE )
            {
              if ( isvextypedivisible( AsgDeclar2->BASE ) )
              {
                Element = searchauthelem( VasyHashBitVec, (char*)AsgSymbol2 );
              }
              else
              {
                Element = searchauthelem( VasyHashAssign, AsgSymbol2->NAME );
              }

              VpnAction2 = (vpnact_list *)Element->VALUE;
              Support2   = GetVasyVpnActInfoSupport( VpnAction2 );
              AsgAtom2   = VpnAction2->VEX_ATOM;

              if ( AsgSymbol2->INDEX == -1 ) AsgIndex2 = 0;
              else                           AsgIndex2 = AsgSymbol2->INDEX;

              AsgPos2  = getvexvectorpos( AsgAtom2, AsgIndex2 );
              Element2 = searchauth2elem( VasyHash2Support, 
                                          (char*)&Support2[ AsgPos2 ], (char*)AsgSymbol );

              if ( Element2 != (auth2elem *)0 )
              {
                if ( IsVasyDebugLevel1() )
                {
                   VasyPrintf( stdout, ">>> define %s %d depends on %s %d !\n", 
                     AsgSymbol2->NAME, AsgSymbol2->INDEX, AsgSymbol->NAME, AsgSymbol->INDEX );
                }

                addauthelem( VasyHashReadError, (char*)AsgSymbol2, (long)AsgSymbol );
              }
            }
          }
/*
**  Verify that no previous signal or variable assigment use this variable !
*/
          for ( VpnAction2  = VpnTrans->ACT;
                VpnAction2 != (vpnact_list *)0;
                VpnAction2  = VpnAction2->NEXT )
          {
            if ( VpnAction2 == VpnAction ) break;

            if ( VpnAction2->TYPE == VPN_ACT_ASG_DEFINE ) continue;

            Support2   = GetVasyVpnActInfoSupport( VpnAction2 );
            AsgAtom2   = VpnAction2->VEX_ATOM;
            AtomName2  = GetVexAtomValue( AsgAtom2 );
            AsgDeclar2 = searchvpndeclall( VpnFigure, AtomName2 );

            AsgMin2 = getvexvectormin( AsgAtom2 );
            AsgMax2 = getvexvectormax( AsgAtom2 );

            for ( AsgIndex2 = AsgMin2; AsgIndex2 <= AsgMax2; AsgIndex2++ )
            {
              AsgSymbol2 = getvpnsymdecl( AsgDeclar2, AsgIndex2 );
              AsgPos2    = getvexvectorpos( AsgAtom2, AsgIndex2 );
              Element2   = searchauth2elem( VasyHash2Support, 
                                            (char*)&Support2[ AsgPos2 ], (char*)AsgSymbol );

              if ( Element2 != (auth2elem *)0 )
              {
                if ( IsVasyDebugLevel1() )
                {
                   VasyPrintf( stdout, "### Error symbol %s %d !!!\n",
                               AsgSymbol2->NAME, AsgSymbol2->INDEX );
                }
              }
            }
          }
        }
      }
    }
  }

  resetauthtable( VasyHashReadError );

  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  <-- VasySupportVpnTrans %s\n", VpnTrans->NAME );
  }
}
