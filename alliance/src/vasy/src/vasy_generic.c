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
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_generic.c                   |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.02.00                        |
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
# include "vbh.h"

# include "vasy_generic.h"
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

  static authtable  *VasyHashGeneric  = (authtable *)0;
  static authtable  *VasyHashConstant = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyGenericSubstVexExprConst             |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenericSubstVexExprConst( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanOper;
  char       *AtomValue;
  vexexpr    *VexConst;
  vexexpr    *VexOperand;
  authelem   *Element;
  long        Oper;
  /* ICI BIG BUG !!
  short       Signed;
  */

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      AtomValue = GetVexAtomValue( VexExpr );
      Element   = searchauthelem( VasyHashGeneric, AtomValue );

      if ( Element != (authelem *)0 )
      {
        VexConst = (vexexpr *)Element->VALUE;
      }
      else
      {
        Element = searchauthelem( VasyHashConstant, AtomValue );

        if ( Element != (authelem *)0 )
        {
          VexConst = (vexexpr *)Element->VALUE;
        }
      }

      if ( Element != (authelem *)0 )
      {
        if ( ( IsVexNodeAtom( VexConst )    ) &&
             ( IsVexAtomLiteral( VexConst ) ) )
        {
          /* ICI BIG BUG !!!
          if ( IsVexNodeSigned( VexExpr ) ) Signed = 1;
          else                              Signed = 0;
          */

          VexExpr->VALUE = VexConst->VALUE;
          VexExpr->TYPE  = VexConst->TYPE;
          VexExpr->WIDTH = VexConst->WIDTH;
          VexExpr->LEFT  = VexConst->LEFT; 
          VexExpr->RIGHT = VexConst->RIGHT;

          /* ICI BIG BUG !!
          if ( Signed ) SetVexNodeSigned( VexExpr );
          else          ClearVexNodeSigned( VexExpr );
          */
        }
      }
    }
  }
  else
  {
    if ( IsVexNodeOper( VexExpr ) ) Oper = GetVexOperValue( VexExpr );
    else                            Oper = -1;

    for ( ScanOper  = VexExpr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VexOperand = GetVexOperand( ScanOper );

      if ( ( Oper == VEX_INDEX              ) ||
           ( Oper == VEX_TO                 ) ||
           ( Oper == VEX_DOWNTO             ) )
      {
        if ( ( ScanOper != VexExpr->OPERAND   ) ||
             ( ! IsVexNodeAtom( VexOperand  ) ) )
        {
          VasyGenericSubstVexExprConst( VexOperand );
        }
      }
      else
      {
        VasyGenericSubstVexExprConst( VexOperand );
      }
    }
  }
}
/*------------------------------------------------------------\
|                                                             |
|                    VasyGenericSubstVexExpr                  |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenericSubstVexExpr( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanOper;
  char       *AtomValue;
  vexexpr    *VexConst;
  vexexpr    *VexOperand;
  authelem   *Element;
  long        Oper;
  /* ICI BIG BUG !!
  short       Signed;
  */

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      AtomValue = GetVexAtomValue( VexExpr );
      Element   = searchauthelem( VasyHashGeneric, AtomValue );

      if ( Element != (authelem *)0 )
      {
        VexConst = (vexexpr *)Element->VALUE;

        if ( ( IsVexNodeAtom( VexConst )    ) &&
             ( IsVexAtomLiteral( VexConst ) ) )
        {
          /* ICI BIG BUG !!
          if ( IsVexNodeSigned( VexExpr ) ) Signed = 1;
          else                              Signed = 0;
          */

          VexExpr->VALUE = VexConst->VALUE;
          VexExpr->TYPE  = VexConst->TYPE;
          VexExpr->WIDTH = VexConst->WIDTH;
          VexExpr->LEFT  = VexConst->LEFT; 
          VexExpr->RIGHT = VexConst->RIGHT;

          /* ICI BIG BUG !!
          if ( Signed ) SetVexNodeSigned( VexExpr );
          else          ClearVexNodeSigned( VexExpr );
          */
        }
      }
    }
  }
  else
  {
    if ( IsVexNodeOper( VexExpr ) ) Oper = GetVexOperValue( VexExpr );
    else                            Oper = -1;

    for ( ScanOper  = VexExpr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VexOperand = GetVexOperand( ScanOper );

      if ( ( ( Oper == VEX_INDEX              ) ||
             ( Oper == VEX_TO                 ) ||
             ( Oper == VEX_DOWNTO             ) ) &&
           ( ( ScanOper != VexExpr->OPERAND   ) ||
             ( ! IsVexNodeAtom( VexOperand  ) ) ) )
      {
        VasyGenericSubstVexExprConst( VexOperand );
      }
      else
      {
        VasyGenericSubstVexExpr( VexOperand );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyGenericSubstSimpConst              |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyGenericSubstSimpVexConst( VexExpr )

  vexexpr *VexExpr;
{
  VasyGenericSubstVexExprConst( VexExpr );
  VexExpr = simpvexexpr( VexExpr );

  return( VexExpr );
}


/*------------------------------------------------------------\
|                                                             |
|                      VasyGenericSubstSimpVex                |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyGenericSubstSimpVex( VexExpr )

  vexexpr *VexExpr;
{
  VasyGenericSubstVexExpr( VexExpr );
  VexExpr = simpvexexpr( VexExpr );

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyGenericSubstSimpTarget             |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyGenericSubstSimpTarget( VexTarget )

  vexexpr *VexTarget;
{
  if ( ! IsVexNodeAtom( VexTarget ) )
  {
    VasyGenericSubstVexExprConst( VexTarget );
    VexTarget = simpvexexpr( VexTarget );
  }

  return( VexTarget );
}

/*------------------------------------------------------------\
|                                                             |
|                        VasyGenericVbhInstance               |
|                                                             |
\------------------------------------------------------------*/

void VasyGenericVbhInstance( VbhFigure )

  vbfig_list *VbhFigure;
{
  vbins_list *VbhInstance;
  vbmap_list *VbhGenMap;
  vbmap_list *VbhPortMap;
  char       *Name;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyGenericVbhInstance %s\n", VbhFigure->NAME );
  }

  for ( VbhInstance  = VbhFigure->BEINS;
        VbhInstance != (vbins_list *)0;
        VbhInstance  = VbhInstance->NEXT )
  {
    for ( VbhGenMap  = VbhInstance->GEN_MAP;
          VbhGenMap != (vbmap_list *)0;
          VbhGenMap  = VbhGenMap->NEXT )
    {
      if ( IsVexNodeAtom( VbhGenMap->ACTUAL ) )
      {
        Name = GetVexAtomValue( VbhGenMap->FORMAL );
        addauthelem( VasyHashGeneric, Name, (long)VbhGenMap->ACTUAL );
      }
    }
  
    for ( VbhPortMap  = VbhInstance->PORT_MAP;
          VbhPortMap != (vbmap_list *)0;
          VbhPortMap  = VbhPortMap->NEXT )
    {
      if ( VbhPortMap->FORMAL != (vexexpr *)0 )
      {
        VbhPortMap->FORMAL = VasyGenericSubstSimpTarget( VbhPortMap->FORMAL );
      }
  
      if ( VbhPortMap->ACTUAL != (vexexpr *)0 )
      {
        VbhPortMap->ACTUAL = VasyGenericSubstSimpVex( VbhPortMap->ACTUAL );
      }
    }
  
    for ( VbhGenMap  = VbhInstance->GEN_MAP;
          VbhGenMap != (vbmap_list *)0;
          VbhGenMap  = VbhGenMap->NEXT )
    {
      if ( IsVexNodeAtom( VbhGenMap->ACTUAL ) )
      {
        Name = GetVexAtomValue( VbhGenMap->ACTUAL );
        delauthelem( VasyHashGeneric, Name );
      }
    }

    vbh_frevbmap( VbhInstance->GEN_MAP );
    VbhInstance->GEN_MAP = (vbmap_list *)0;
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyGenericVbhInstance %s\n\n", VbhFigure->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        VasyGenericVbhFig                    |
|                                                             |
\------------------------------------------------------------*/

void VasyGenericVbhFig( VbhFigure )

 vbfig_list *VbhFigure;
{
  vbgen_list  *VbhGeneric;
  vbcst_list  *VbhConstant;
  char        *Name;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyGenericVbhFig %s\n", VbhFigure->NAME );
  }

  VasyHashGeneric  = createauthtable( 100 );
  VasyHashConstant = createauthtable( 100 );

  VasyGenericVbhInstance( VbhFigure );

  for ( VbhConstant  = VbhFigure->BECST;
        VbhConstant != (vbcst_list *)0;
        VbhConstant  = VbhConstant->NEXT )
  {
    VbhConstant->TARGET = VasyGenericSubstSimpTarget( VbhConstant->TARGET );
    Name = getvexatomname( VbhConstant->TARGET );
    VbhConstant->VEX    = VasyGenericSubstSimpVex( VbhConstant->VEX );

    addauthelem( VasyHashConstant, Name, (long)VbhConstant->VEX );
  }

  for ( VbhGeneric  = VbhFigure->BEGEN;
        VbhGeneric != (vbgen_list *)0;
        VbhGeneric  = VbhGeneric->NEXT )
  {
    VbhGeneric->TARGET = VasyGenericSubstSimpTarget( VbhGeneric->TARGET );

    if ( VbhGeneric->VEX != (vexexpr *)0 )
    {
      VbhGeneric->VEX = VasyGenericSubstSimpVex( VbhGeneric->VEX );

      if ( IsVexNodeAtom( VbhGeneric->TARGET ) )
      {
        Name = GetVexAtomValue( VbhGeneric->TARGET );
        addauthelem( VasyHashGeneric, Name, (long)VbhGeneric->VEX );
      }
    }
  }

  for ( VbhConstant  = VbhFigure->BECST;
        VbhConstant != (vbcst_list *)0;
        VbhConstant  = VbhConstant->NEXT )
  {
    VbhConstant->TARGET = VasyGenericSubstSimpTarget( VbhConstant->TARGET );
    Name = getvexatomname( VbhConstant->TARGET );
    VbhConstant->VEX    = VasyGenericSubstSimpVex( VbhConstant->VEX );

    addauthelem( VasyHashConstant, Name, (long)VbhConstant->VEX );
  }

  vbh_forallvexvbfig( VbhFigure, VasyGenericSubstSimpTarget,
                                 VasyGenericSubstSimpVexConst );

  destroyauthtable( VasyHashGeneric );
  destroyauthtable( VasyHashConstant );

  if ( IsVasyDebugLevel1() )
  {
    vbh_viewvbfig( VbhFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyGenericVbhFig %s\n\n", VbhFigure->NAME );
  }
}
