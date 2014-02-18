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
| File    :                  vasy_array.c                     |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   02.02.04                        |
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

# include "vasy_array.h"
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

  static ptype_list **VasyPrevInst       = (ptype_list **)0;
  static vbpcs_list  *VasyProcess        = (vbpcs_list  *)0;
  static long         VasyNumberVar      = 0;
  static vbtyp_list  *VasyTypeNatural    = (vbtyp_list  *)0;

  static char VasyBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   VasyDynFuncAddVariable                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyDynFuncAddVariable( BaseType, Width )

  vbtyp_list *BaseType;
  int         Width;
{
  vexexpr *VexAtom;
  char    *Name;

  sprintf( VasyBuffer, "dynfunc_var_%ld", VasyNumberVar++ );
  Name = namealloc( VasyBuffer ); 

  if ( Width == 0 )
  {
    VexAtom = createvexatomvec( Name, BaseType->LEFT, BaseType->RIGHT );
  }
  else
  if ( Width > 0 )
  {
    VexAtom = createvexatomvec( Name, Width - 1, 0 );
  }
  else
  {
    VexAtom = createvexatombit( Name );
  }

  VasyProcess->VARIABLE = 
    vbh_addvbvar( VasyProcess->VARIABLE, VexAtom, (vexexpr *)0, BaseType, VasyProcess->LINE );

  SetVbhProcSequential( VasyProcess );

  return( dupvexexpr( VexAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyExpandDynFuncVexFunc                  |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyExpandDynFuncVexFunc( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr    *VexAtom;
  vexexpr    *Operand;
  vexexpr    *VexVar;
  vexexpr    *VexData;
  vexexpr    *VexCond;
  vexexpr    *VexValue;
  char       *ChoiceValue;
  vbcho_list *VbhChoice;
  vbcas_list *VbhCase;
  ptype_list *ScanPType;
  char       *FuncName;
//int         FuncId;
  int         DynFuncSize;
  int         SizeValue;
  int         IntValue;
  int         Index;

  FuncName = GetVexFuncValue( VexExpr );
/*FuncId =*/ getvexstdfuncid( FuncName );

  VexAtom = GetVexOperand( VexExpr->OPERAND       );
  Operand = GetVexOperand( VexExpr->OPERAND->NEXT );

  SizeValue = Operand->WIDTH;
  /*
  ** Too big !!
  */
  if ( SizeValue > 8 ) return( VexExpr );

  DynFuncSize = 1L << SizeValue;
  VexVar      = VasyDynFuncAddVariable( VasyTypeNatural, VexAtom->WIDTH );

  if ( ! IsVexNodeAtom( Operand ) )
  {
    VexCond = VasyDynFuncAddVariable( VasyTypeNatural, SizeValue );
  }
  else
  {
    VexCond = Operand;
  }

  ScanPType = vbh_addvbcas( *VasyPrevInst, dupvexexpr( VexCond ), 
                             VasyTypeNatural, VasyProcess->LINE );
  *VasyPrevInst = ScanPType;
  VbhCase       = (vbcas_list *)ScanPType->DATA;

  if ( ! IsVexNodeAtom( Operand ) )
  {
    ScanPType = vbh_addvbvar( *VasyPrevInst, VexCond, dupvexexpr( Operand ),
                               VasyTypeNatural, VasyProcess->LINE );
    *VasyPrevInst = ScanPType;
  }

  VbhChoice = (vbcho_list *)autallocblock( sizeof( vbcho_list ) * DynFuncSize );
  VbhCase->SIZE   = DynFuncSize;
  VbhCase->CHOICE = VbhChoice;

  IntValue = 0;

  for ( Index = 0; Index < DynFuncSize; Index++ )
  {
    VexValue    = createvexatomlong( IntValue, SizeValue, 0 );
    ChoiceValue = GetVexAtomValue( VexValue );

    VexData = createvexfunc( FuncName, VexAtom->WIDTH );
    addvexqexpr( VexData, dupvexexpr( VexAtom ) );
    addvexqexpr( VexData, VexValue );

    ScanPType = vbh_addvbvar( (ptype_list *)0, 
          dupvexexpr( VexVar ), VexData, VasyTypeNatural, VasyProcess->LINE );

    VbhChoice[ Index ].INSTRUCTION = ScanPType;
    VbhChoice[ Index ].SIZE        = SizeValue;
    VbhChoice[ Index ].VALUES      = addchain( NULL, (void *)ChoiceValue );

    IntValue++;
  }

  freevexexpr( VexExpr );

  return( VexVar );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDynFuncScanVex                        |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyDynFuncScanVex( VexExpr )

  vexexpr *VexExpr;
{
  vexexpr    *Operand;
  vexexpr    *VexInt;
  chain_list *ScanChain;
  char       *FuncName;
  long        Length;
  int         FuncId;

  if ( IsVexNodeAtom( VexExpr ) )
  {
    return( VexExpr );
  }

  for ( ScanChain  = VexExpr->OPERAND;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Operand = GetVexOperand( ScanChain );
    Operand = VasyDynFuncScanVex( Operand );
    SetVexOperand( ScanChain, Operand );
  }

  if ( IsVexNodeFunc( VexExpr ) )
  {
    FuncName = GetVexFuncValue( VexExpr );
    FuncId   = getvexstdfuncid( FuncName );

    if ( FuncId != -1 )
    {
      switch ( FuncId )
      {
        case VEX_STD_TO_UNSIGNED           :
        case VEX_STD_TO_SIGNED             :
        case VEX_STD_CONV_UNSIGNED         :
        case VEX_STD_CONV_SIGNED           :
        case VEX_STD_CONV_STD_LOGIC_VECTOR :
        case VEX_STD_EXT                   :
        case VEX_STD_SXT                   :
        case VEX_STD_RESIZE                :
        case VEX_STD_SHL                   :
        case VEX_STD_SHIFT_LEFT            :
        case VEX_STD_SHR                   :
        case VEX_STD_SHIFT_RIGHT           :


          Operand  = GetVexOperand( VexExpr->OPERAND       );
          VexInt   = GetVexOperand( VexExpr->OPERAND->NEXT );

          if ( evalvexatomlong( VexInt, &Length ) )
          {
            VexExpr = VasyExpandDynFuncVexFunc( VexExpr );
          }

        break;
      }
    }
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDynFuncScanVexCond                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyDynFuncScanVexCond( PrevInst, VexExpr )

  ptype_list **PrevInst;
  vexexpr     *VexExpr;
{
  if ( VexExpr != (vexexpr *)0 )
  {
    VasyPrevInst = PrevInst;
    VexExpr = VasyDynFuncScanVex( VexExpr );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDynFuncScanAsg                        |
|                                                             |
\------------------------------------------------------------*/

static void VasyDynFuncScanAsg( PrevInst, CurInst, Variable )

  ptype_list **PrevInst;
  ptype_list  *CurInst;
  int          Variable;
{
  vbasg_list *ScanAsg;
  vbvar_list *ScanVar;

  VasyPrevInst = PrevInst;

  if ( Variable )
  {
    ScanVar      = (vbvar_list *)CurInst->DATA;
    ScanVar->VEX = VasyDynFuncScanVex( ScanVar->VEX );
  }
  else
  {
    ScanAsg      = (vbasg_list *)CurInst->DATA;
    ScanAsg->VEX = VasyDynFuncScanVex( ScanAsg->VEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDynFuncScanInstruction                |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *VasyDynFuncScanInstruction( Instruction )

  ptype_list *Instruction;
{
  ptype_list  *ScanPType;
  ptype_list **PrevPType;
  vbifs_list  *ScanIfs;
  vbcas_list  *ScanCase;
  vbcho_list  *ScanChoice;
  vblop_list  *ScanLoop;
  vbfor_list  *ScanFor;
  vbwhi_list  *ScanWhile;
  vbwas_list  *ScanWait;
  vbnxt_list  *ScanNext;
  vbext_list  *ScanExit;
  int          Offset;

  ScanPType = Instruction;
  PrevPType = &Instruction;

  while ( ScanPType != (ptype_list *)0 )
  {
    switch ( ScanPType->TYPE )
    {
      case VBH_BEIFS : 

        ScanIfs = (vbifs_list *)ScanPType->DATA;
        ScanIfs->CND      = VasyDynFuncScanVexCond( PrevPType, ScanIfs->CND );
        ScanIfs->CNDTRUE  = VasyDynFuncScanInstruction( ScanIfs->CNDTRUE  );
        ScanIfs->CNDFALSE = VasyDynFuncScanInstruction( ScanIfs->CNDFALSE );

      break;

      case VBH_BECAS : 

        ScanCase = (vbcas_list *)ScanPType->DATA;
        ScanCase->VEX = VasyDynFuncScanVexCond( PrevPType, ScanCase->VEX );

        for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
        {
          ScanChoice = &ScanCase->CHOICE[ Offset ];
          ScanChoice->INSTRUCTION = VasyDynFuncScanInstruction( ScanChoice->INSTRUCTION );
        }

      break;

      case VBH_BEFOR : 

        ScanFor = (vbfor_list *)ScanPType->DATA;
        ScanFor->INSTRUCTION = VasyDynFuncScanInstruction( ScanFor->INSTRUCTION );

        continue;

      break;

      case VBH_BEWHI : 

        ScanWhile = (vbwhi_list *)ScanPType->DATA;
        ScanWhile->CND         = VasyDynFuncScanVexCond( PrevPType, ScanWhile->CND );
        ScanWhile->INSTRUCTION = VasyDynFuncScanInstruction( ScanWhile->INSTRUCTION );

      break;

      case VBH_BELOP : 

        ScanLoop = (vblop_list *)ScanPType->DATA;
        ScanLoop->INSTRUCTION = VasyDynFuncScanInstruction( ScanLoop->INSTRUCTION );

      break;

      case VBH_BEASG : 

        VasyDynFuncScanAsg( PrevPType, ScanPType, 0 );

      break;

      case VBH_BEVAR :

        VasyDynFuncScanAsg( PrevPType, ScanPType, 1 );

      break;

      case VBH_BEWAS :

        ScanWait = (vbwas_list *)ScanPType->DATA;
        ScanWait->CND = VasyDynFuncScanVexCond( PrevPType, ScanWait->CND );

      break;

      case VBH_BENXT :

        ScanNext = (vbnxt_list *)ScanPType->DATA;
        ScanNext->CND = VasyDynFuncScanVexCond( PrevPType, ScanNext->CND );

      break;

      case VBH_BEEXT :

        ScanExit = (vbext_list *)ScanPType->DATA;
        ScanExit->CND = VasyDynFuncScanVexCond( PrevPType, ScanExit->CND );

      break;

      case VBH_BEAGR :

        VasyError( VASY_NOT_YET_IMPLEMENTED_ERROR, "aggregate assignation" );

      break;
    }

    PrevPType = &ScanPType->NEXT;
    ScanPType = ScanPType->NEXT;
  }

  return( Instruction );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyExpandDynFuncScanProcess          |
|                                                             |
\------------------------------------------------------------*/

static void VasyDynFuncScanProcess( BePcs )

  vbpcs_list *BePcs;
{
  VasyProcess = BePcs;

  BePcs->INSTRUCTION = VasyDynFuncScanInstruction( BePcs->INSTRUCTION );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyExpandDynFuncVbhFig               |
|                                                             |
\------------------------------------------------------------*/

void VasyExpandDynFuncVbhFig( VbhFigure )

  vbfig_list *VbhFigure;
{
  vbpcs_list *BePcs;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyExpandDynFuncVbhFig %s\n", VbhFigure->NAME );
  }

  VasyTypeNatural = vbh_getvbtypbyindex( VbhFigure, VBH_TYPE_NATURAL );

  for ( BePcs  = VbhFigure->BEPCS;
        BePcs != (vbpcs_list *)0;
        BePcs  = BePcs->NEXT )
  {
    VasyDynFuncScanProcess( BePcs );
  }

  if ( IsVasyDebugLevel1() )
  {
    vbh_viewvbfig( VbhFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyExpandDynFuncVbhFig %s\n\n", VbhFigure->NAME );
  }
}
