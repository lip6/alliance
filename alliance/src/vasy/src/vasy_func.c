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
| Tool    :                    VASY                           |
|                                                             |
| File    :                 vasy_func.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
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
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_func.h"
# include "vasy_redact.h"
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

  static vpnline_list *VasyVpnLine = (vpnline_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

# if 0
/*------------------------------------------------------------\
|                                                             |
|                      VasyFuncAddVpnDefine                   |
|                                                             |
\------------------------------------------------------------*/

static vpndecl_list *VasyFuncAddVpnDefine( VpnFigure, VpnProc, Width, DefBase )

  vpnfig_list    *VpnFigure;
  vpnproc_list   *VpnProc;
  unsigned short  Width;
  unsigned char   DefBase;
{
  vpndecl_list *DefDeclar;
  vexexpr      *DefAtom;
  char         *FatherName;
  char          Buffer[ 128 ];
  unsigned char NewBase;
  long          Number;

  FatherName = VpnProc->NAME;
  Number     = GetVpnNumDeclDef( VpnFigure );

  sprintf( Buffer, "%s_std_%ld", FatherName, Number );

  NewBase = getvextypescalar( DefBase );

  if ( ( isvextypevector( DefBase ) ) &&
       ( ( Width   >  1           ) ||
         ( NewBase == DefBase     ) ) )
  {
    DefAtom = createvexatomvec( Buffer, Width - 1, 0 );
  }
  else
  {
    DefAtom = createvexatombit( Buffer );
    DefBase = NewBase;
  }

  DefDeclar = addvpndeclprocdef( VpnFigure, VpnProc, DefAtom, DefBase );

  return( DefDeclar );
}
# endif

/*------------------------------------------------------------\
|                                                             |
|                       VasyFuncSubstVexStdFunc               |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyFuncSubstVexStdFunc( Expr )

  vexexpr *Expr;
{
  vexexpr    *Operand;
  vexexpr    *VexInt;
  chain_list *ScanChain;
  long        Length;
  int         FuncId;
  char       *FuncName;

  if ( ( IsVexNodeOper( Expr ) ) ||
       ( IsVexNodeFunc( Expr ) ) )
  {
    for ( ScanChain  = Expr->OPERAND;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Operand = GetVexOperand( ScanChain );
      Operand = VasyFuncSubstVexStdFunc( Operand );
      SetVexOperand( ScanChain, Operand );
    }

    if ( IsVexNodeFunc( Expr ) )
    {
      FuncName = GetVexFuncValue( Expr );

      FuncId   = getvexstdfuncid( FuncName );

      if ( FuncId != -1 )
      {
        switch ( FuncId )
        {
          case VEX_STD_TO_BIT                :
          case VEX_STD_TO_BITVECTOR          :
          case VEX_STD_TO_STDULOGIC          :
          case VEX_STD_TO_STDLOGICVECTOR     :
          case VEX_STD_TO_STDULOGICVECTOR    :
          case VEX_STD_TO_X01                :
          case VEX_STD_TO_X01Z               :
          case VEX_STD_TO_UX01               :
          case VEX_STD_CONV_INTEGER          :
          case VEX_STD_RISING_EDGE           :
          case VEX_STD_FALLING_EDGE          :
          case VEX_STD_STD_LOGIC_VECTOR      :
          case VEX_STD_STD_ULOGIC_VECTOR     :
          case VEX_STD_SIGNED                :
          case VEX_STD_UNSIGNED              :
          case VEX_STD_TO_INTEGER            :

            Operand = GetVexOperand( Expr->OPERAND );
            SetVexOperand( Expr->OPERAND, (vexexpr *)0 );
            freevexexpr( Expr );

            if ( FuncId == VEX_STD_FALLING_EDGE )
            {
              Expr    = createvexunaryexpr( VEX_EVENT, 1, dupvexexpr( Operand ) );
              Operand = createvexunaryexpr( VEX_NOT  , 1, Operand               );
              Expr    = createvexbinexpr(   VEX_AND  , 1, Expr, Operand         );
            }
            else
            if ( FuncId == VEX_STD_RISING_EDGE )
            {
              Expr = createvexunaryexpr( VEX_EVENT, 1, dupvexexpr( Operand ) );
              Expr = createvexbinexpr(   VEX_AND  , 1, Expr, Operand         );
            }
            else
            {
              Expr = Operand;

              if ( FuncId == VEX_STD_SIGNED )
              {
                SetVexNodeSigned( Expr );
              }
              else
              if ( FuncId == VEX_STD_UNSIGNED )
              {
                ClearVexNodeSigned( Expr );
              }
            }

          break;

          case VEX_STD_ABS :

            SetVexNodeOper( Expr );
            SetVexOperValue( Expr, VEX_ABS );

          break;

          case VEX_STD_TO_UNSIGNED           :
          case VEX_STD_TO_SIGNED             :
          case VEX_STD_CONV_UNSIGNED         :
          case VEX_STD_CONV_SIGNED           :
          case VEX_STD_CONV_STD_LOGIC_VECTOR :
          case VEX_STD_EXT                   :
          case VEX_STD_SXT                   :
          case VEX_STD_RESIZE                :

            Operand  = GetVexOperand( Expr->OPERAND       );
            VexInt   = GetVexOperand( Expr->OPERAND->NEXT );

            if ( evalvexatomlong( VexInt, &Length ) )
            {
              VasyErrorLine( VASY_ERROR_ILLEGAL_USE_OF,
                             VasyVpnLine, GetVexFuncValue( Expr ) );
            }

            if ( ( Operand->WIDTH != Length    ) &&
                 ( IsVexNodeAtom( Operand )    ) )
            {
              if ( FuncId == VEX_STD_SXT ) SetVexNodeSigned( Operand );

              Operand = extendvexatomsign( Operand, Length );
            }

            if ( Operand->WIDTH == Length )
            {
              SetVexOperand( Expr->OPERAND, (vexexpr *)0 );
              freevexexpr( Expr );

              Expr = Operand;
            }
            else
            {
              VasyErrorLine( VASY_ERROR_ILLEGAL_USE_OF,
                             VasyVpnLine, GetVexFuncValue( Expr ) );
            }


            if ( ( FuncId == VEX_STD_TO_SIGNED   ) ||
                 ( FuncId == VEX_STD_CONV_SIGNED ) )
            {
              SetVexNodeSigned( Expr );
            }
            else
            if ( FuncId != VEX_STD_RESIZE )
            {
              ClearVexNodeSigned( Expr );
            }

          break;

          case VEX_STD_SHL         :
          case VEX_STD_SHIFT_LEFT  :
          case VEX_STD_SHR         :
          case VEX_STD_SHIFT_RIGHT :

            Operand  = GetVexOperand( Expr->OPERAND       );
            VexInt   = GetVexOperand( Expr->OPERAND->NEXT );

            if ( evalvexatomlong( VexInt, &Length ) )
            {
              VasyErrorLine( VASY_ERROR_ILLEGAL_USE_OF,
                             VasyVpnLine, GetVexFuncValue( Expr ) );
            }

            if ( IsVexNodeAtom( Operand ) )
            {
              if ( ( FuncId == VEX_STD_SHL        ) ||
                   ( FuncId == VEX_STD_SHIFT_LEFT ) )
              {
                Operand = shiftvexatomleft( Operand, Length );
              }
              else
              {
                Operand = shiftvexatomright( Operand, Length );
              }

              SetVexOperand( Expr->OPERAND, (vexexpr *)0 );
              freevexexpr( Expr );

              Expr = Operand;
            }
            else
            {
              VasyErrorLine( VASY_ERROR_ILLEGAL_USE_OF,
                             VasyVpnLine, GetVexFuncValue( Expr ) );
            }

          break;

          default : 
          
            VasyErrorLine( VASY_NOT_YET_IMPLEMENTED_ERROR, VasyVpnLine, "standard function" );

          /* VEX_STD_IS_X
             VEX_STD_ROTATE_LEFT
             VEX_STD_ROTATE_RIGHT
          */
        }
      }
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyFuncVpnProc                       |
|                                                             |
\------------------------------------------------------------*/

static void VasyFuncVpnProc( VpnFigure, VpnProc )

  vpnfig_list  *VpnFigure;
  vpnproc_list *VpnProc;
{
  vpntrans_list *VpnTrans;
  vpnact_list   *VpnAction;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyFuncVpnProc %s\n", VpnProc->NAME );
  }

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    for ( VpnAction  = VpnTrans->ACT;
          VpnAction != (vpnact_list *)0;
          VpnAction  = VpnAction->NEXT )
    {
      VasyVpnLine = VpnAction->LINE;
      VpnAction->VEX_EXPR = VasyFuncSubstVexStdFunc( VpnAction->VEX_EXPR );
    }
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyFuncVpnProc %s\n", VpnProc->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyFuncVpnInst                       |
|                                                             |
\------------------------------------------------------------*/

static void VasyFuncVpnInst( VpnFigure, VpnInst )

  vpnfig_list *VpnFigure;
  vpnins_list *VpnInst;
{
  vpnmap_list *VpnMap;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  --> VasyFuncVpnInst %s\n", VpnInst->NAME );
  }

  for ( VpnMap  = VpnInst->MAP;
        VpnMap != (vpnmap_list *)0;
        VpnMap  = VpnMap->NEXT )
  {
    VasyVpnLine = VpnMap->LINE;
    VpnMap->VEX_ACTUAL = VasyFuncSubstVexStdFunc( VpnMap->VEX_ACTUAL );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "  <-- VasyFuncVpnInst %s\n", VpnInst->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyFuncVpnFig                        |
|                                                             |
\------------------------------------------------------------*/

void VasyFuncVpnFig( VpnFigure )

 vpnfig_list *VpnFigure;
{
  vpnproc_list *VpnProc;
  vpnins_list  *VpnInst;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyFuncVpnFig %s\n", VpnFigure->NAME );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    VasyFuncVpnProc( VpnFigure, VpnProc );
  }

  for ( VpnInst  = VpnFigure->INSTANCE;
        VpnInst != (vpnins_list *)0;
        VpnInst  = VpnInst->NEXT )
  {
    VasyFuncVpnInst( VpnFigure, VpnInst );
  }

  if ( IsVasyDebugLevel1() )
  {
    VasyDebugSaveVpnFig( VpnFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyFuncVpnFig %s\n\n", VpnFigure->NAME );
  }
}
