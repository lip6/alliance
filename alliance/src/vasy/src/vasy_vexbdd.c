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
| File    :                  vasy_vexbdd.c                    |
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
# include "vasy_vexbdd.h"

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

  static authtable   *VasyHashSym2Abl   = (authtable   *)0;
  static authtable   *VasyHashAbl2Sym   = (authtable   *)0;
  static authtable   *VasyHashAbl2Del   = (authtable   *)0;
  static authtable   *VasyHashAssign    = (authtable   *)0;
  static authtable   *VasyHashBitVec = (authtable   *)0;
  static bddsystem   *VasyBddSystem     = (bddsystem   *)0;
  static bddcircuit  *VasyBddCircuit    = (bddcircuit  *)0;
  static vpnfig_list *VasyFigure        = (vpnfig_list *)0;

  static vpnline_list *VasyVpnLine      = (vpnline_list *)0;

  static char VasyOperVex2Abl[ VEX_MAX_OPERATOR ] =
  {
    -1      ,  /* VEX_CONCAT    */
    ABL_NOT ,  /* VEX_NOT       */
    -1      ,  /* VEX_NEG       */
    -1      ,  /* VEX_EVENT     */
    ABL_OR  ,  /* VEX_OR        */
    ABL_AND ,  /* VEX_AND       */
    ABL_XOR ,  /* VEX_XOR       */
    ABL_NOR ,  /* VEX_NOR       */
    ABL_NAND,  /* VEX_NAND      */
    ABL_NXOR,  /* VEX_NXOR      */
    ABL_NXOR,  /* VEX_EQ        */
    ABL_XOR ,  /* VEX_NE        */
    -1      ,  /* VEX_LT        */
    -1      ,  /* VEX_LE        */
    -1      ,  /* VEX_GT        */
    -1      ,  /* VEX_GE        */
    -1      ,  /* VEX_ADD       */
    -1      ,  /* VEX_SUB       */
    -1      ,  /* VEX_MUL       */
    -1      ,  /* VEX_DIV       */
    -1      ,  /* VEX_EXP       */
    -1      ,  /* VEX_MOD       */
    -1      ,  /* VEX_REM       */
    -1      ,  /* VEX_TO        */
    -1      ,  /* VEX_DOWNTO    */
    -1      ,  /* VEX_INDEX     */
    -1      ,  /* VEX_LEFT      */
    -1      ,  /* VEX_RIGHT     */
    -1      ,  /* VEX_LOW       */
    -1      ,  /* VEX_HIGH      */
    -1      ,  /* VEX_LENGTH    */
    -1      ,  /* VEX_RANGE     */
    -1      ,  /* VEX_REV_RANGE */
    -1      ,  /* VEX_DRIVER    */
    ABL_AND ,  /* VEX_IFT       */
    -1      ,  /* VEX_ARRAY     */
    -1      ,  /* VEX_INDEX_N   */
    -1      ,  /* VEX_OTHERS    */ 
    -1      ,  /* VEX_NUM_BIT   */
    -1      ,  /* VEX_ABS       */
    -1      ,  /* AF            */
    -1      ,  /* AG            */
    -1      ,  /* AX            */
    -1      ,  /* AU            */
    -1      ,  /* EF            */
    -1      ,  /* EG            */
    -1      ,  /* EX            */
    -1      ,  /* EU            */
    -1      ,  /* EQUIV         */
    -1         /* IMPLY         */ 
  };

  static char VasyOperAbl2Vex[ ABL_MAX_OPERATOR ] =
  {
    VEX_OR  ,  /* ABL_OR      */
    VEX_AND ,  /* ABL_AND     */
    VEX_XOR ,  /* ABL_XOR     */
    VEX_NOT ,  /* ABL_NOT     */
    VEX_NOR ,  /* ABL_NOR     */
    VEX_NAND,  /* ABL_NAND    */
    VEX_NXOR,  /* ABL_NXOR    */
    -1      ,  /* ABL_STABLE  */
    -1      ,  /* ABL_AF      */
    -1      ,  /* ABL_AG      */
    -1      ,  /* ABL_AX      */
    -1      ,  /* ABL_AU      */
    -1      ,  /* ABL_EF      */
    -1      ,  /* ABL_EG      */
    -1      ,  /* ABL_EX      */
    -1         /* ABL_EU      */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddGetAblName                    |
|                                                             |
\------------------------------------------------------------*/

static char *VasyVexBddGetAblName( AsgSymbol )

  vpnsym *AsgSymbol;
{
  authelem *Element;
  char     *AblName;
  char      Buffer[ 128 ];

  Element = searchauthelem( VasyHashSym2Abl, (char*)AsgSymbol );

  if ( Element == (authelem *)0 )
  {
    if ( AsgSymbol->INDEX == -1 )
    {
      AblName  = AsgSymbol->NAME;
    }
    else
    {
      sprintf( Buffer, "%s %d", AsgSymbol->NAME, AsgSymbol->INDEX );
      AblName = namealloc( Buffer );
    }

    addauthelem( VasyHashSym2Abl, (char*)AsgSymbol, (long)AblName   );
    addauthelem( VasyHashAbl2Sym, (char*)AblName  , (long)AsgSymbol );
  }
  else
  {
    AblName = (char *)Element->VALUE;
  }

  return( AblName );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddGetAblDelName                 |
|                                                             |
\------------------------------------------------------------*/

static char *VasyVexBddGetAblDelName( AblName )

  char *AblName;
{
  authelem *Element;
  char     *DelName;
  char      Buffer[ 128 ];

  if ( VasyHashAbl2Del == (authtable *)0 )
  {
    VasyHashAbl2Del = createauthtable( 100 );
  }

  Element = searchauthelem( VasyHashAbl2Del, AblName );

  if ( Element == (authelem *)0 )
  {
    strcpy( Buffer    , "del."  );
    strcpy( Buffer + 4, AblName );

    DelName = namealloc( Buffer );

    addauthelem( VasyHashAbl2Del, AblName, (long)DelName );
  }
  else
  {
    DelName = (char *)Element->VALUE;
  }

  return( DelName );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddGetVpnSymbol                  |
|                                                             |
\------------------------------------------------------------*/

static vpnsym *VasyVexBddGetVpnSymbol( AblName )

  char *AblName;
{
  authelem *Element;

  if ( VasyHashAbl2Sym == (authtable *)0 )
  {
    return( (vpnsym *)0 );
  }

  Element = searchauthelem( VasyHashAbl2Sym, AblName );

  if ( Element != (authelem *)0 )
  {
    return( (vpnsym *)Element->VALUE );
  }

  return( (vpnsym *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddVex2BddSize                   |
|                                                             |
\------------------------------------------------------------*/

static long VasyVexBddVex2BddSize( Expr, Position )

  vexexpr *Expr;
  short    Position;
{
  vpnact_list    *VpnAction;
  vpndecl_list   *VpnDeclar;
  vpnsym         *VpnSymbol;
  char           *AtomName;
  chain_list     *ScanChain;
  authelem       *Element;
  vexexpr        *VexAtom;
  vexexpr        *VexExpr;
  vexexpr        *Operand;
  char           *NodeName;
  bddnode        *BddNode;
  long            Oper;
  int             ScanPos;
  int             PosFrom;
  int             Index;
  int             PosTo;
  long            Size;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "VasyVexBddVex2BddSize %d->\n", Position );
    viewvexexprboundln( Expr );
  }

  Size = 1;

  if ( IsVexNodeAtom( Expr ) )
  {
    AtomName = GetVexAtomValue( Expr );

    if ( Position >= Expr->WIDTH ) ScanPos = Expr->WIDTH - 1;
    else                           ScanPos = Position;

    if ( ! IsVexAtomLiteral( Expr ) )
    {
/*
**  Expression is an atom => check if this atom should be substituted !
*/
      VpnDeclar = searchvpndeclall( VasyFigure, AtomName );
      Index     = getvexvectorindex( Expr, ScanPos );
      VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

      NodeName = VasyVexBddGetAblName( VpnSymbol );
      BddNode  = searchbddcircuitin( VasyBddCircuit, NodeName );

      if ( BddNode == (bddnode *)0 )
      {
/*
**  Should substitute this atom by its bdd node
*/
        VexExpr = (vexexpr *)0;
        VexAtom = (vexexpr *)0;

        if ( VpnDeclar->TYPE == VPN_DECLAR_DEFINE   )
        {
          if ( ! isvextypedivisible( VpnDeclar->BASE ) )
          {
            Element = searchauthelem( VasyHashAssign, AtomName );
          }
          else
          {
            Element = searchauthelem( VasyHashBitVec, (char*)VpnSymbol );
          }

          if ( Element != (authelem *)0 )
          {
            VpnAction = (vpnact_list *)Element->VALUE;

            VexAtom   = VpnAction->VEX_ATOM;
            VexExpr   = VpnAction->VEX_EXPR;
          }
        }
        else
        if ( VpnDeclar->TYPE == VPN_DECLAR_CONSTANT )
        {
          VexAtom = VpnDeclar->VEX_ATOM;
          VexExpr = VpnDeclar->VEX_INIT;
        }

        if ( VexAtom != (vexexpr *)0 )
        {
          ScanPos = getvexvectorpos( VexAtom, Index );
          Size    = VasyVexBddVex2BddSize( VexExpr, ScanPos );
        }
      }
    }
    else
    {
      Size = 0;
    }
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( Oper == VEX_CONCAT )
    {
/*
**  Special case for concatenation
*/
      PosFrom = 0;
      Operand = (vexexpr *)0;

      for ( ScanChain  = Expr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand  = GetVexOperand( ScanChain );
        PosTo    = PosFrom + Operand->WIDTH - 1;

        if ( ( Position >= PosFrom ) &&
             ( Position <= PosTo   ) ) break;

        PosFrom = PosTo + 1;
        PosTo   = PosFrom;
      }

      ScanPos = Position - PosFrom;
      Size    = VasyVexBddVex2BddSize( Operand, ScanPos );
    }
    else
    if ( Oper != VEX_EVENT )
    {
/*
**  Apply VasyVexBddVex2BddSize for all operands
*/
      for ( ScanChain  = Expr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand = GetVexOperand( ScanChain );
        Size   += VasyVexBddVex2BddSize( Operand, Position );
      }
    }
  }

  return( Size );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddVex2Bdd                       |
|                                                             |
\------------------------------------------------------------*/

static bddnode *VasyVexBddVex2Bdd( Expr, Position )

  vexexpr *Expr;
  short    Position;
{
  vpnact_list    *VpnAction;
  vpndecl_list   *VpnDeclar;
  vpnsym         *VpnSymbol;
  char           *AtomName;
  chain_list     *ScanChain;
  authelem       *Element;
  vexexpr        *VexAtom;
  vexexpr        *VexExpr;
  vexexpr        *Operand;
  char           *NodeName;
  bddnode        *BddNode;
  bddnode        *BddLast;
  long            Oper;
  long            AblOper;
  int             ScanPos;
  int             PosFrom;
  int             Index;
  int             PosTo;
  int             Negative;
  int             FoundEq;

  if ( IsVasyDebugLevel2() )
  {
    VasyPrintf( stdout, "VasyVexBddVex2Bdd %d->\n", Position );
    viewvexexprboundln( Expr );
  }

  BddNode = (bddnode *)0;
  FoundEq = 0;

  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue(  Expr );

    if ( ( Oper == VEX_EQ ) ||
         ( Oper == VEX_NE ) )
    {
/*
**  Special case for equal / not equal
*/
      Expr    = simpvexexpreq( dupvexexpr( Expr ) );
      Oper    = GetVexOperValue( Expr );
      FoundEq = 1;
    }
  }

  if ( IsVexNodeAtom( Expr ) )
  {
    AtomName = GetVexAtomValue( Expr );

    if ( Position >= Expr->WIDTH ) ScanPos = Expr->WIDTH - 1;
    else                           ScanPos = Position;

    if ( IsVexAtomLiteral( Expr ) )
    {
/*
**  Expression is a literal => return a terminal bdd node
*/
      Index = getvexliteralid( AtomName[ ScanPos + 1 ] );

      if ( Index == VEX_ZERO_ID ) BddNode = VasyBddSystem->ZERO;
      else
      if ( Index == VEX_ONE_ID  ) BddNode = VasyBddSystem->ONE;
      else
      if ( Index == VEX_DC_ID   ) BddNode = VasyBddSystem->ZERO;
      else
      {
        NodeName = VEX_ATOM_BY_ID[ Index ];
        BddNode  = searchbddcircuitin( VasyBddCircuit, NodeName );
      }
    }
    else
    {
/*
**  Expression is an atom => check if this atom should be substituted !
*/
      VpnDeclar = searchvpndeclall( VasyFigure, AtomName );
      Index     = getvexvectorindex( Expr, ScanPos );
      VpnSymbol = getvpnsymdecl( VpnDeclar, Index );

      NodeName = VasyVexBddGetAblName( VpnSymbol );
      BddNode  = searchbddcircuitin( VasyBddCircuit, NodeName );

      if ( BddNode == (bddnode *)0 )
      {
/*
**  Should substitute this atom by its bdd node
*/
        BddNode = searchbddcircuitout( VasyBddCircuit, NodeName );

        if ( BddNode == (bddnode *)0 )
        {
          VexExpr = (vexexpr *)0;
          VexAtom = (vexexpr *)0;

          if ( VpnDeclar->TYPE == VPN_DECLAR_DEFINE   )
          {
            if ( ! isvextypedivisible( VpnDeclar->BASE ) )
            {
              Element = searchauthelem( VasyHashAssign, AtomName );
            }
            else
            {
              Element = searchauthelem( VasyHashBitVec, (char*)VpnSymbol );
            }

            if ( Element != (authelem *)0 )
            {
              VpnAction = (vpnact_list *)Element->VALUE;

              VexAtom   = VpnAction->VEX_ATOM;
              VexExpr   = VpnAction->VEX_EXPR;
            }
          }
          else
          if ( VpnDeclar->TYPE == VPN_DECLAR_CONSTANT )
          {
            VexAtom = VpnDeclar->VEX_ATOM;
            VexExpr = VpnDeclar->VEX_INIT;
          }

          if ( ( VexAtom != (vexexpr *)0           ) &&
               ( ! isvexarithoperinexpr( VexExpr ) ) )
          {
            ScanPos   = getvexvectorpos( VexAtom, Index );
            BddNode   = VasyVexBddVex2Bdd( VexExpr, ScanPos );

            addbddcircuitout( VasyBddCircuit, NodeName, BddNode );
          }
          else
          {
            BddNode = addbddcircuitin( VasyBddCircuit, NodeName, 0, BDD_IN_MODE_LAST );
          }
        }
        else
        {
          incbddrefext( BddNode );
        }
      }
    }
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    Oper = GetVexOperValue( Expr );

    if ( Oper == VEX_CONCAT )
    {
/*
**  Special case for concatenation
*/
      PosFrom = 0;
      Operand = (vexexpr *)0;

      for ( ScanChain  = Expr->OPERAND;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand  = GetVexOperand( ScanChain );
        PosTo    = PosFrom + Operand->WIDTH - 1;

        if ( ( Position >= PosFrom ) &&
             ( Position <= PosTo   ) ) break;

        PosFrom = PosTo + 1;
        PosTo   = PosFrom;
      }

      ScanPos = Position - PosFrom;
      BddNode = VasyVexBddVex2Bdd( Operand, ScanPos );
    }
    else
    if ( Oper == VEX_EVENT )
    {
/*
**  Special case for event attribute => Add an input S'stable
*/
      Operand = GetVexOperand( Expr->OPERAND );

      if ( ! IsVexNodeAtom( Operand ) )
      {
        VasyErrorLine( VASY_ERROR_IN_VEX2BDD, VasyVpnLine, VEX_OPER_NAME[ Oper ] );
      }

      AtomName  = GetVexAtomValue( Operand );
      VpnDeclar = searchvpndeclall( VasyFigure, AtomName );
      Index     = getvexvectorindex( Expr, Position );
      VpnSymbol = getvpnsymdecl( VpnDeclar, Index );
      NodeName  = VasyVexBddGetAblName( VpnSymbol );

      NodeName = getbddstablename( NodeName );
      BddNode  = addbddcircuitin( VasyBddCircuit, NodeName, 0, BDD_IN_MODE_LAST );
      BddNode  = applybddnodenot( VasyBddSystem, BddNode );
    }
    else
    {
/*
**  Apply VasyVexBddVex2Bdd for all operands
*/
      if ( isvexnegativeoper( Oper ) )
      {
        Negative = 1;
        Oper     = getvexnotoper( Oper );
      }
      else
      {
        Negative = 0;
      }

      AblOper = VasyOperVex2Abl[ Oper ];

      if ( AblOper == -1 )
      {
        VasyErrorLine( VASY_ERROR_IN_VEX2BDD, VasyVpnLine, VEX_OPER_NAME[ Oper ] );
      }

      Operand = GetVexOperand( Expr->OPERAND );
      BddNode = VasyVexBddVex2Bdd( Operand, Position );

      for ( ScanChain  = Expr->OPERAND->NEXT;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Operand = GetVexOperand( ScanChain );
        BddLast = VasyVexBddVex2Bdd( Operand, Position );

        BddNode = applybddnode( VasyBddSystem, AblOper,
                                decbddrefext( BddLast ),
                                decbddrefext( BddNode ) );
      }

      if ( Negative )
      {
        BddNode = applybddnodenot( VasyBddSystem,
                                   decbddrefext( BddNode ) );
      }
    }
  }

  if ( FoundEq ) freevexexpr( Expr );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddSupportVpnSymbol              |
|                                                             |
\------------------------------------------------------------*/

chain_list *VasyVexBddSupportVpnSymbol( VpnFigure, VpnProc, BddNode )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  bddnode      *BddNode;
{
  vasyprocinfo  *ProcInfo;
  chain_list   *Support;
  chain_list  **PrevChain;
  chain_list   *ScanChain;
  chain_list   *DelChain;
  char        **NameIn;
  bddindex     *IndexIn;
  char         *AblName;
  char         *Name;
  vpnsym       *VpnSymbol;
  bddindex      Index;

  VasyFigure = VpnFigure;

  ProcInfo       = GetVasyVpnProcInfo( VpnProc );
  VasyBddCircuit  = ProcInfo->BDD_CIRCUIT;
  VasyBddSystem   = ProcInfo->BDD_SYSTEM;
  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  Support = getbddnodesupportchain( VasyBddSystem, BddNode );
  IndexIn = VasyBddCircuit->INDEX_IN;
  NameIn  = VasyBddCircuit->NAME_IN;

  ScanChain = Support;
  PrevChain = &Support;

  while ( ScanChain != (chain_list *)0 )
  {
    BddNode = (bddnode *)ScanChain->DATA;

    Index   = BddNode->INDEX - BDD_INDEX_MIN;
    Index   = IndexIn[ Index ];
    AblName = NameIn[ Index ]; 

    Name = isbddstablename( AblName );
    if ( Name != (char *)0 ) AblName = Name;

    VpnSymbol = VasyVexBddGetVpnSymbol( AblName );

    if ( VpnSymbol != (vpnsym *)0 )
    {
      ScanChain->DATA = (void *)VpnSymbol;

      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;
    }
    else
    {
      *PrevChain = ScanChain->NEXT;
      DelChain   = ScanChain;
      ScanChain  = ScanChain->NEXT;

      DelChain->NEXT = (chain_list *)0;
      freechain( DelChain );
    }
  }

  return( Support );
}
  
/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddConvertVex2BddSize            |
|                                                             |
\------------------------------------------------------------*/

long VasyVexBddConvertVex2BddSize( VpnFigure, VpnProc, VpnTrans, Expr, Position )

  vpnfig_list     *VpnFigure;
  vpnproc_list    *VpnProc;
  vpntrans_list   *VpnTrans;
  vexexpr         *Expr;
  short            Position;
{
  vasyprocinfo  *ProcInfo;
  vasytransinfo *TransInfo;
  long           Size;

  VasyFigure = VpnFigure;

  ProcInfo       = GetVasyVpnProcInfo( VpnProc );
  VasyBddCircuit  = ProcInfo->BDD_CIRCUIT;
  VasyBddSystem   = ProcInfo->BDD_SYSTEM;
  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  TransInfo     = GetVasyVpnTransInfo( VpnTrans );
  VasyHashAssign = TransInfo->HASH_ASSIGN;
  VasyHashBitVec = TransInfo->HASH_BITVEC;

  Size = VasyVexBddVex2BddSize( Expr, Position );

  return( Size );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddConvertVex2Bdd                |
|                                                             |
\------------------------------------------------------------*/

bddnode *VasyVexBddConvertVex2Bdd( VpnFigure, VpnProc, VpnTrans, Expr, Position )

  vpnfig_list     *VpnFigure;
  vpnproc_list    *VpnProc;
  vpntrans_list   *VpnTrans;
  vexexpr         *Expr;
  short            Position;
{
  vasyprocinfo  *ProcInfo;
  vasytransinfo *TransInfo;
  bddnode      *BddNode;

  VasyFigure = VpnFigure;

  ProcInfo       = GetVasyVpnProcInfo( VpnProc );
  VasyBddCircuit  = ProcInfo->BDD_CIRCUIT;
  VasyBddSystem   = ProcInfo->BDD_SYSTEM;
  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  TransInfo     = GetVasyVpnTransInfo( VpnTrans );
  VasyHashAssign = TransInfo->HASH_ASSIGN;
  VasyHashBitVec = TransInfo->HASH_BITVEC;

  VasyVpnLine = VpnProc->LINE;
  BddNode = VasyVexBddVex2Bdd( Expr, Position );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyVexBddAbl2Vex                     |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyVexBddAbl2Vex( AblExpr )

  ablexpr *AblExpr;
{
  vexexpr *VexExpr;
  vexexpr *VexOperand;
  vpnsym  *VpnSymbol;
  char    *AtomValue;
  long     AblOper;
  long     VexOper;
  int      Index;

  if ( ABL_ATOM( AblExpr ) )
  {
    AtomValue = ABL_ATOM_VALUE( AblExpr );

    if ( AtomValue[ 0 ] == '\'' )
    {
      Index   = getvexliteralid( AtomValue[ 1 ] );
      VexExpr = createvexatombit( VEX_ATOM_BY_ID[ Index ] );
    }
    else
    {
      VpnSymbol = VasyVexBddGetVpnSymbol( AtomValue );

      if ( VpnSymbol->INDEX == -1 )
      {
        VexExpr = createvexatombit( VpnSymbol->NAME );
      }
      else
      {
        VexExpr = createvexatomvec( VpnSymbol->NAME, VpnSymbol->INDEX, VpnSymbol->INDEX );
      }
    }
  }
  else
  {
    AblOper = ABL_OPER( AblExpr );

    if ( AblOper == ABL_STABLE )
    {
      VexOperand = VasyVexBddAbl2Vex( ABL_CADR( AblExpr ) );
      VexExpr = createvexunaryexpr( VEX_EVENT, 1, VexOperand );
      VexExpr = createvexunaryexpr( VEX_NOT  , 1, VexExpr    );
    }
    else
    if ( AblOper == ABL_NOT )
    {
      VexOperand = VasyVexBddAbl2Vex( ABL_CADR( AblExpr ) );
      VexExpr    = optimvexnotexpr( VexOperand );
    }
    else
    {
      VexOper = VasyOperAbl2Vex[ AblOper ];

      if ( VexOper == -1 )
      {
        VasyError( VASY_ERROR_IN_ABL2VEX, ABL_OPERATOR_NAME[ AblOper ] );
      }

      VexExpr = createvexoper( VexOper, 1 );

      while ( ( AblExpr = ABL_CDR( AblExpr ) ) != (ablexpr *)0 )
      {
        VexOperand = VasyVexBddAbl2Vex( ABL_CAR( AblExpr ) );
        addvexhexpr( VexExpr, VexOperand );
      }
    }
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyVexBddConvertBdd2Vex                |
|                                                             |
\------------------------------------------------------------*/

vexexpr *VasyVexBddConvertBdd2Vex( VpnFigure, VpnProc, BddNode )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  bddnode      *BddNode;
{
  vasyprocinfo *ProcInfo;
  bddcircuit  *BddCircuit;
  ablexpr     *AblExpr;
  vexexpr     *VexExpr;

  ProcInfo   = GetVasyVpnProcInfo( VpnProc );

  BddCircuit = ProcInfo->BDD_CIRCUIT;
  AblExpr = convertbddcircuitabl( BddCircuit, BddNode );

  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  VexExpr = VasyVexBddAbl2Vex( AblExpr );
  freeablexpr( AblExpr );

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                VasyVexBddSearchBddCircuitInput              |
|                                                             |
\------------------------------------------------------------*/

bddnode *VasyVexBddSearchBddCircuitInput( VpnFigure, VpnProc, VpnSymbol )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnsym       *VpnSymbol;
{
  vasyprocinfo *ProcInfo;
  bddcircuit  *BddCircuit;
  bddnode     *BddNode;
  char        *AblName;

  ProcInfo   = GetVasyVpnProcInfo( VpnProc );
  BddCircuit = ProcInfo->BDD_CIRCUIT;

  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  AblName = VasyVexBddGetAblName( VpnSymbol );
  BddNode = searchbddcircuitin( BddCircuit, AblName );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                VasyVexBddSearchBddCircuitStable             |
|                                                             |
\------------------------------------------------------------*/

bddnode *VasyVexBddSearchBddCircuitStable( VpnFigure, VpnProc, VpnSymbol )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnsym       *VpnSymbol;
{
  vasyprocinfo *ProcInfo;
  bddcircuit  *BddCircuit;
  bddnode     *BddNode;
  char        *AblName;

  ProcInfo   = GetVasyVpnProcInfo( VpnProc );
  BddCircuit = ProcInfo->BDD_CIRCUIT;

  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  AblName = VasyVexBddGetAblName( VpnSymbol );
  AblName = getbddstablename( AblName );

  BddNode = searchbddcircuitin( BddCircuit, AblName );

  if ( BddNode == (bddnode *)0 )
  {
    BddNode = addbddcircuitin( BddCircuit, AblName, 0, BDD_IN_MODE_LAST );
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                VasyVexBddAddBddCircuitInput                 |
|                                                             |
\------------------------------------------------------------*/

bddnode *VasyVexBddAddBddCircuitInput( VpnFigure, VpnProc, VpnSymbol )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnsym       *VpnSymbol;
{
  vasyprocinfo *ProcInfo;
  bddcircuit  *BddCircuit;
  bddnode     *BddNode;
  char        *AblName;
  char        *DelName;

  ProcInfo   = GetVasyVpnProcInfo( VpnProc );
  BddCircuit = ProcInfo->BDD_CIRCUIT;

  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  AblName = VasyVexBddGetAblName( VpnSymbol );
  DelName = VasyVexBddGetAblDelName( AblName );

  BddNode = searchbddcircuitin( BddCircuit, DelName );

  if ( BddNode == (bddnode *)0 )
  {
    BddNode = addbddcircuitin( BddCircuit, AblName, 0, BDD_IN_MODE_LAST );
  }
  else
  {
    BddNode = renamebddcircuitin( BddCircuit, DelName, AblName );
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                VasyVexBddDelBddCircuitInput                 |
|                                                             |
\------------------------------------------------------------*/

bddnode *VasyVexBddDelBddCircuitInput( VpnFigure, VpnProc, VpnSymbol )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnsym       *VpnSymbol;
{
  vasyprocinfo *ProcInfo;
  bddcircuit  *BddCircuit;
  bddnode     *BddNode;
  char        *AblName;
  char        *DelName;

  ProcInfo   = GetVasyVpnProcInfo( VpnProc );
  BddCircuit = ProcInfo->BDD_CIRCUIT;

  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  AblName = VasyVexBddGetAblName( VpnSymbol );
  DelName = VasyVexBddGetAblDelName( AblName );

  BddNode = renamebddcircuitin( BddCircuit, AblName, DelName );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                VasyVexBddDelBddCircuitOutput                |
|                                                             |
\------------------------------------------------------------*/

int VasyVexBddDelBddCircuitOutput( VpnFigure, VpnProc, VpnSymbol )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
  vpnsym       *VpnSymbol;
{
  vasyprocinfo *ProcInfo;
  bddcircuit  *BddCircuit;
  char        *AblName;

  ProcInfo   = GetVasyVpnProcInfo( VpnProc );
  BddCircuit = ProcInfo->BDD_CIRCUIT;

  VasyHashSym2Abl = ProcInfo->HASH_SYM2ABL;
  VasyHashAbl2Sym = ProcInfo->HASH_ABL2SYM;

  AblName = VasyVexBddGetAblName( VpnSymbol );
  return( delbddcircuitout( BddCircuit, AblName ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyVexBddCreateBddCircuit                |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *VasyVexBddCreateBddCircuit( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vasyprocinfo *ProcInfo;
  bddsystem   *BddSystem;
  bddcircuit  *BddCircuit;
  bddnode    **BddLiteral;
  bddnode     *BddNode;
  char        *Name;
  int          Literal;

  ProcInfo = GetVasyVpnProcInfo( VpnProc );

  BddSystem  = createbddsystem( 100, 1000, 100, 500000 );
  BddCircuit = createbddcircuit( VpnProc->NAME, 100, 100, BddSystem );
  BddLiteral = (bddnode **)autallocblock( sizeof( bddnode *) * VEX_MAX_ID );

  reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 100000, 100 );

  for ( Literal = 0; Literal < VEX_MAX_ID; Literal++ )
  {
    if ( Literal == VEX_ONE_ID )
    {
      BddNode = BddSystem->ONE;
    }
    else
    if ( Literal == VEX_ZERO_ID )
    {
      BddNode = BddSystem->ZERO;
    }
    if ( Literal == VEX_DC_ID )
    {
      BddNode = BddSystem->ZERO;
    }
    else
    {
      Name    = VEX_ATOM_BY_ID[ Literal ];
      BddNode = addbddcircuitin( BddCircuit, Name, 0, BDD_IN_MODE_LAST );
    }

    BddLiteral[ Literal ] = BddNode;
  }

  ProcInfo->BDD_SYSTEM   = BddSystem;
  ProcInfo->BDD_CIRCUIT  = BddCircuit;
  ProcInfo->BDD_LITERAL  = BddLiteral;
  ProcInfo->HASH_ABL2SYM = createauthtable( 100 );
  ProcInfo->HASH_SYM2ABL = createauthtable( 100 );

  return( BddCircuit );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyVexBddDestroyBddCircuit               |
|                                                             |
\------------------------------------------------------------*/

void VasyVexBddDestroyBddCircuit( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vasyprocinfo *ProcInfo;

  ProcInfo = GetVasyVpnProcInfo( VpnProc );

# if 0
  if ( IsVasyDebugLevel1() )
  {
    VasyPrintf( stdout, "  --> VasyVexBddDestroyBddCircuit\n" );
    testbddcircuit( ProcInfo->BDD_CIRCUIT );
  }
# endif

  destroybddsystem( ProcInfo->BDD_SYSTEM );
  destroybddcircuit( ProcInfo->BDD_CIRCUIT );

  destroyauthtable( ProcInfo->HASH_ABL2SYM );
  destroyauthtable( ProcInfo->HASH_SYM2ABL );

  ProcInfo->BDD_SYSTEM   = (bddsystem  *)0;
  ProcInfo->BDD_CIRCUIT  = (bddcircuit *)0;
  ProcInfo->HASH_ABL2SYM = (authtable *)0;
  ProcInfo->HASH_SYM2ABL = (authtable *)0;
}
