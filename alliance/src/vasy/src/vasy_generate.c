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
| File    :                  vasy_generate.c                  |
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

# include "vasy_generate.h"
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

  static authtable  *VasyHashConstant = (authtable *)0;
  static authtable  *VasyHashGeneric  = (authtable *)0;
  static authtable  *VasyHashForIndex = (authtable *)0;

  static char       *VasyGenerateLabel = (char *)0;

  static char        VasyBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyGenerateSubstAllConst                |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenerateSubstAllConst( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanOper;
  vbcst_list *VbhConst;
  vbgen_list *VbhGen;
  char       *AtomValue;
  vexexpr    *VexConst;
  authelem   *Element;
  /* ICI BIG BUG !!
  short       Signed;
  */

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      AtomValue = GetVexAtomValue( VexExpr );
      Element   = searchauthelem( VasyHashConstant, AtomValue );

      if ( Element == (authelem *)0 )
      {
        Element = searchauthelem( VasyHashGeneric, AtomValue );

        if ( Element != (authelem *)0 )
        {
          VbhGen = (vbgen_list *)Element->VALUE;

          VasyGenerateSubstAllConst( VbhGen->VEX );
          VbhGen->VEX = simpvexexpr( VbhGen->VEX );

          VexConst = VbhGen->VEX;
        }
      }
      else
      {
        VbhConst = (vbcst_list *)Element->VALUE;

        VasyGenerateSubstAllConst( VbhConst->VEX );
        VbhConst->VEX = simpvexexpr( VbhConst->VEX );

        VexConst = VbhConst->VEX;
      }

      if ( Element != (authelem *)0 )
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
  else
  {
    for ( ScanOper  = VexExpr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasyGenerateSubstAllConst( GetVexOperand( ScanOper ) );
    }
  }
}
/*------------------------------------------------------------\
|                                                             |
|                    VasyGenerateSubstConst                   |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenerateSubstConst( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanOper;
  vbcst_list *VbhConst;
  vbgen_list *VbhGen;
  char       *AtomValue;
  vexexpr    *VexConst;
  authelem   *Element;
  /* ICI BIG BUG !!
  short       Signed;
  */

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      AtomValue = GetVexAtomValue( VexExpr );
      Element   = searchauthelem( VasyHashConstant, AtomValue );

      if ( Element == (authelem *)0 )
      {
        Element = searchauthelem( VasyHashGeneric, AtomValue );

        if ( Element != (authelem *)0 )
        {
          VbhGen   = (vbgen_list *)Element->VALUE;
          VexConst = VbhGen->VEX;
        }
      }
      else
      {
        VbhConst = (vbcst_list *)Element->VALUE;
        VexConst = VbhConst->VEX;
      }

      if ( Element != (authelem *)0 )
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
  else
  {
    for ( ScanOper  = VexExpr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasyGenerateSubstConst( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyGenerateVexExpr                    |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyGenerateVexExpr( VexExpr )

  vexexpr *VexExpr;
{
  authelem   *Element;
  chain_list *ScanOper;
  vexexpr    *Operand;
  char       *Name;
  int         Width;

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      Name    = GetVexAtomValue( VexExpr );
      Element = searchauthelem( VasyHashForIndex, Name );

      if ( Element != (authelem *)0 )
      {
        Width = VexExpr->WIDTH;
        freevexexpr( VexExpr );

        VexExpr = createvexatomlong( Element->VALUE, Width, 2 );
      }
    }
  }
  else
  {
    for ( ScanOper  = VexExpr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      Operand = GetVexOperand( ScanOper );
      Operand = VasyGenerateVexExpr( Operand );
      SetVexOperand( ScanOper, Operand );
    }
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyGenerateForVexExpr                 |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyGenerateForVexExpr( VexExpr )

  vexexpr *VexExpr;
{
  if ( VexExpr != (vexexpr *)0 )
  {
    VexExpr = dupvexexpr( VexExpr );
    VexExpr = VasyGenerateVexExpr( VexExpr ); 
    VexExpr = simpvexexpr( VexExpr );
  }

  return( VexExpr );
}


/*------------------------------------------------------------\
|                                                             |
|                    VasyGenerateIsVexCondTrue                |
|                                                             |
\------------------------------------------------------------*/

static int VasyGenerateIsVexCondTrue( VexExpr )

  vexexpr *VexExpr;
{
  char *AtomValue;

  if ( ( IsVexNodeAtom( VexExpr )    ) &&
       ( IsVexAtomLiteral( VexExpr ) ) )
  {
    AtomValue = GetVexAtomValue( VexExpr );

    if ( AtomValue == VEX_ATOM_ONE  ) return( 1 );
    if ( AtomValue == VEX_ATOM_ZERO ) return( 0 );
  }

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyGenerateForRenameLabel              |
|                                                             |
\------------------------------------------------------------*/

static char *VasyGenerateForRenameLabel( Label )

  char *Label;
{
  sprintf( VasyBuffer, "%s_%s", VasyGenerateLabel, Label );
  Label = namealloc( VasyBuffer );

  return( Label );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyGenerateForDupInst                  |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *VasyGenerateForDupInst( Instruction )

  ptype_list *Instruction;
{
  vbasg_list  *ScanAsg;
  vbagr_list  *ScanAgr;
  vbvar_list  *ScanVar;
  vbwas_list  *ScanWait;
  vbnxt_list  *ScanNext;
  vbext_list  *ScanExit;
  vbret_list  *ScanReturn;
  vbcal_list  *ScanCall;
  vbifs_list  *ScanIfs;
  vbcas_list  *ScanCase;
  vbcho_list  *ScanChoice;
  vblop_list  *ScanLoop;
  vbfor_list  *ScanFor;
  vbwhi_list  *ScanWhile;
  vbasg_list  *NewAsg;
  vbagr_list  *NewAgr;
  vbvar_list  *NewVar;
  vbwas_list  *NewWait;
  vbnxt_list  *NewNext;
  vbext_list  *NewExit;
  vbret_list  *NewReturn;
  vbcal_list  *NewCall;
  vbifs_list  *NewIfs;
  vbcas_list  *NewCase;
  vbcho_list  *NewChoice;
  vblop_list  *NewLoop;
  vbfor_list  *NewFor;
  vbwhi_list  *NewWhile;
  ptype_list  *NewInstruction;
  chain_list  *ScanChain;
  chain_list  *ScanValue;
  void        *Pointer;
  int          InstType;
  int          Offset;

  NewInstruction = (ptype_list *)0;

  while ( Instruction != (ptype_list *)0 )
  {
    InstType = Instruction->TYPE;
    Pointer  = (void *)0;

    switch ( InstType )
    {
      case VBH_BEASG :

        ScanAsg = (vbasg_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbasg_list ) );
        NewAsg  = (vbasg_list *)Pointer;

        NewAsg->TARGET = VasyGenerateForVexExpr( ScanAsg->TARGET );
        NewAsg->VEX    = VasyGenerateForVexExpr( ScanAsg->VEX    );
        NewAsg->TYPE   = ScanAsg->TYPE;
        NewAsg->LINE   = ScanAsg->LINE;

      break;

      case VBH_BEVAR :

        ScanVar = (vbvar_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbvar_list ) );
        NewVar  = (vbvar_list *)Pointer;

        NewVar->TARGET = VasyGenerateForVexExpr( ScanVar->TARGET );
        NewVar->VEX    = VasyGenerateForVexExpr( ScanVar->VEX    );
        NewVar->TYPE   = ScanVar->TYPE;
        NewVar->LINE   = ScanVar->LINE;

      break;

      case VBH_BENXT :

        ScanNext = (vbnxt_list *)Instruction->DATA;

        Pointer  = autallocheap( sizeof( vbnxt_list ) );
        NewNext  = (vbnxt_list *)Pointer;

        NewNext->LINE  = ScanNext->LINE;
        NewNext->CND   = VasyGenerateForVexExpr( ScanNext->CND );
        NewNext->LABEL = VasyGenerateForRenameLabel( ScanNext->LABEL );

      break;

      case VBH_BEEXT :

        ScanExit = (vbext_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vbext_list ) );
        NewExit  = (vbext_list *)Pointer;

        NewExit->LINE  = ScanExit->LINE;
        NewExit->CND   = VasyGenerateForVexExpr( ScanExit->CND );
        NewExit->LABEL = VasyGenerateForRenameLabel( ScanExit->LABEL );

      break;

      case VBH_BEWAS :

        ScanWait = (vbwas_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vbwas_list ) );
        NewWait  = (vbwas_list *)Pointer;

        for ( ScanChain  = ScanWait->SEN;
              ScanChain != (chain_list *)0;
              ScanChain  = ScanChain->NEXT )
        {
          NewWait->SEN = addchain( NewWait->SEN, ScanChain->DATA );
        }

        NewWait->CND       = VasyGenerateForVexExpr( ScanWait->CND );
        NewWait->TIME_UNIT = ScanWait->TIME_UNIT;
        NewWait->TIMEOUT   = ScanWait->TIMEOUT;
        NewWait->LINE      = ScanWait->LINE;

      break;

      case VBH_BERET :

        ScanReturn = (vbret_list *)Instruction->DATA;
        Pointer    = autallocheap( sizeof( vbret_list ) );
        NewReturn  = (vbret_list *)Pointer;

        NewReturn->RET  = VasyGenerateForVexExpr( ScanReturn->RET );
        NewReturn->LINE = ScanReturn->LINE;

      break;

      case VBH_BECAL :

        ScanCall = (vbcal_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vbcal_list ) );
        NewCall  = (vbcal_list *)Pointer;

        NewCall->CALL = VasyGenerateForVexExpr( ScanCall->CALL );
        NewCall->LINE = ScanCall->LINE;

      break;

      case VBH_BEAGR :

        ScanAgr = (vbagr_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbagr_list ) );
        NewAgr  = (vbagr_list *)Pointer;

        NewAgr->ASSIGN = VasyGenerateForDupInst( ScanAgr->ASSIGN );
        NewAgr->LINE   = ScanAgr->LINE;

      break;

      case VBH_BEIFS : 

        ScanIfs = (vbifs_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbifs_list ) );
        NewIfs  = (vbifs_list *)Pointer;

        NewIfs->CND      = VasyGenerateForVexExpr( ScanIfs->CND );
        NewIfs->CNDTRUE  = VasyGenerateForDupInst( ScanIfs->CNDTRUE  );
        NewIfs->CNDFALSE = VasyGenerateForDupInst( ScanIfs->CNDFALSE );
        NewIfs->LINE     = ScanIfs->LINE;

      break;

      case VBH_BECAS : 

        ScanCase = (vbcas_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vbcas_list ) );
        NewCase  = (vbcas_list *)Pointer;

        NewCase->VEX  = VasyGenerateForVexExpr( ScanCase->VEX );
        NewCase->TYPE = ScanCase->TYPE;
        NewCase->LINE = ScanCase->LINE;

        NewCase->SIZE   = ScanCase->SIZE;
        NewCase->CHOICE = (vbcho_list *)autallocblock( ScanCase->SIZE * sizeof( vbcho_list ) );

        for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
        {
          ScanChoice = &ScanCase->CHOICE[ Offset ];
          NewChoice  = &NewCase->CHOICE[ Offset ];

          NewChoice->INSTRUCTION = VasyGenerateForDupInst( ScanChoice->INSTRUCTION );

          NewChoice->VALUES = (chain_list *)0;

          for ( ScanValue  = ScanChoice->VALUES;
                ScanValue != (chain_list *)0;
                ScanValue  = ScanValue->NEXT )
          {
            NewChoice->VALUES = addchain( NewChoice->VALUES, ScanValue->DATA );
          }

          NewChoice->SIZE        = ScanChoice->SIZE ;
          NewChoice->LINE        = ScanChoice->LINE;
        }

      break;

      case VBH_BEFOR :

        ScanFor = (vbfor_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbfor_list ) );
        NewFor  = (vbfor_list *)Pointer;

        NewFor->INSTRUCTION = VasyGenerateForDupInst( ScanFor->INSTRUCTION );
        NewFor->LABEL       = VasyGenerateForRenameLabel( ScanFor->LABEL );
        NewFor->VARIABLE    = VasyGenerateForVexExpr( ScanFor->VARIABLE );
        NewFor->LEFT        = VasyGenerateForVexExpr( ScanFor->LEFT     );
        NewFor->RIGHT       = VasyGenerateForVexExpr( ScanFor->RIGHT    );
        NewFor->UP          = ScanFor->UP;
        NewFor->LINE        = ScanFor->LINE;

      break;

      case VBH_BEWHI : 

        ScanWhile = (vbwhi_list *)Instruction->DATA;
        Pointer   = autallocheap( sizeof( vbwhi_list ) );
        NewWhile  = (vbwhi_list *)Pointer;

        NewWhile->INSTRUCTION = VasyGenerateForDupInst( ScanWhile->INSTRUCTION ); 
        NewWhile->LABEL       = VasyGenerateForRenameLabel( ScanWhile->LABEL );
        NewWhile->CND         = VasyGenerateForVexExpr( ScanWhile->CND );
        NewWhile->LINE        = ScanWhile->LINE;

      break;

      case VBH_BELOP : 

        ScanLoop = (vblop_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vblop_list ) );
        NewLoop  = (vblop_list *)Pointer;

        NewLoop->INSTRUCTION = VasyGenerateForDupInst( ScanLoop->INSTRUCTION );
        NewLoop->LABEL       = VasyGenerateForRenameLabel( ScanLoop->LABEL );
        NewLoop->LINE        = ScanLoop->LINE;

      break;
    }

    if ( Pointer != (void *)0 )
    {
      NewInstruction = addptype( NewInstruction, InstType, Pointer );
    }

    Instruction = Instruction->NEXT;
  }

  NewInstruction = (ptype_list *)reverse( (chain_list *)NewInstruction );

  return( NewInstruction );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyGenerateForDupInstance              |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenerateForDupInstance( VbhFigure, VbhGenerate )


  vbfig_list *VbhFigure;
  vbgnr_list *VbhGenerate;
{
  vbins_list *ScanInstance;
  vbmap_list *ScanMap;
  vbins_list *NewInstance;

  for ( ScanInstance  = VbhGenerate->BEINS;
        ScanInstance != (vbins_list *)0;
        ScanInstance  = ScanInstance->NEXT )
  {
    sprintf( VasyBuffer, "%s_%s", VasyGenerateLabel, ScanInstance->NAME );

    VbhFigure->BEINS = vbh_addvbins( VbhFigure->BEINS,
          VasyBuffer, ScanInstance->MODEL, ScanInstance->LINE );

    NewInstance = VbhFigure->BEINS;

    for ( ScanMap  = ScanInstance->PORT_MAP;
          ScanMap != (vbmap_list *)0;
          ScanMap  = ScanMap->NEXT )
    {
      NewInstance->PORT_MAP = vbh_addvbmap( NewInstance->PORT_MAP,
          VasyGenerateForVexExpr( ScanMap->FORMAL ),
          VasyGenerateForVexExpr( ScanMap->ACTUAL ), ScanMap->LINE );
    }

    for ( ScanMap  = ScanInstance->GEN_MAP;
          ScanMap != (vbmap_list *)0;
          ScanMap  = ScanMap->NEXT )
    {
      NewInstance->GEN_MAP = vbh_addvbmap( NewInstance->GEN_MAP,
          VasyGenerateForVexExpr( ScanMap->FORMAL ),
          VasyGenerateForVexExpr( ScanMap->ACTUAL ), ScanMap->LINE );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyGenerateForDupProcess               |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenerateForDupProcess( VbhFigure, VbhGenerate )


  vbfig_list *VbhFigure;
  vbgnr_list *VbhGenerate;
{
  vbpcs_list *ScanProcess;
  vbpcs_list *NewProcess;
  chain_list *NewSens;
  chain_list *ScanSens;

  for ( ScanProcess  = VbhGenerate->BEPCS;
        ScanProcess != (vbpcs_list *)0;
        ScanProcess  = ScanProcess->NEXT )
  {
    sprintf( VasyBuffer, "%s_%s", VasyGenerateLabel, ScanProcess->LABEL );

    VbhFigure->BEPCS = vbh_addvbpcs( VbhFigure->BEPCS,
          VasyBuffer, (chain_list *)0, (ptype_list *)0, ScanProcess->LINE );

    NewProcess       = VbhFigure->BEPCS;
    NewProcess->TYPE = ScanProcess->TYPE;
/*
**  Process sensitivity list
*/
    NewSens = (chain_list *)0;

    for ( ScanSens  = ScanProcess->SENSITIVITY;
          ScanSens != (chain_list *)0;
          ScanSens  = ScanSens->NEXT )
    {
      NewSens = addchain( NewSens, ScanSens->DATA );
    }

    NewProcess->SENSITIVITY = NewSens;
/*
**  Process variables
*/
    NewProcess->VARIABLE = VasyGenerateForDupInst( ScanProcess->VARIABLE );
/*
**  Process instruction
*/
    NewProcess->INSTRUCTION = VasyGenerateForDupInst( ScanProcess->INSTRUCTION );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyGenerateIf                         |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenerateFor();

static void VasyGenerateIf( VbhFigure, VbhGenerate )

  vbfig_list  *VbhFigure;
  vbgnr_list  *VbhGenerate;
{
  vbgnr_list *ScanGenerate;
  char       *OldLabel;
  vexexpr    *VexCond;
  int         TrueFalse;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyGenerateIf %s\n", VbhGenerate->LABEL );
  }
  
  VexCond = VbhGenerate->IF_COND;
  VasyGenerateSubstConst( VexCond );
  VexCond = simpvexexpr( VexCond );
  VbhGenerate->IF_COND = VexCond;

  VexCond   = VasyGenerateForVexExpr( VexCond );
  TrueFalse = VasyGenerateIsVexCondTrue( VexCond );

  if ( TrueFalse == -1 )
  {
    viewvexexprboundln( VexCond );
    VasyError( VASY_ERROR_ILLEGAL_USE_OF, "if generate condition" );
  }
  else
  if ( TrueFalse == 1 )
  {
    OldLabel = VasyGenerateLabel;
    sprintf( VasyBuffer, "%s_%s_if", OldLabel, VbhGenerate->LABEL );
    VasyGenerateLabel = namealloc( VasyBuffer );

    for ( ScanGenerate  = VbhGenerate->BEGNR;
          ScanGenerate != (vbgnr_list *)0;
          ScanGenerate  = ScanGenerate->NEXT )
    {
      if ( VbhGenerate->TYPE == VBH_GENERATE_FOR )
      {
        VasyGenerateFor( &VbhGenerate->BEPCS, &VbhGenerate->BEINS, ScanGenerate );
      }
      else
      {
        VasyGenerateIf( &VbhGenerate->BEPCS, &VbhGenerate->BEINS, ScanGenerate );
      }
    }

    VasyGenerateForDupProcess(  VbhFigure, VbhGenerate );
    VasyGenerateForDupInstance( VbhFigure, VbhGenerate );

    VasyGenerateLabel = OldLabel;
  }

  freevexexpr( VexCond );

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyGenerateIf %s\n", VbhGenerate->LABEL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyGenerateFor                         |
|                                                             |
\------------------------------------------------------------*/

static void VasyGenerateFor( VbhFigure, VbhGenerate )

  vbfig_list  *VbhFigure;
  vbgnr_list  *VbhGenerate;
{
  vbgnr_list *ScanGenerate;
  vexexpr    *VexLeft;
  vexexpr    *VexRight;
  vexexpr    *VexAtom;
  char       *OldLabel;
  char       *IndexName;
  long        IndexValue;
  long        Left;
  long        Right;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyGenerateFor %s\n", VbhGenerate->LABEL );
  }

  VexAtom  = VbhGenerate->FOR_VARIABLE;

  VexLeft  = VbhGenerate->FOR_LEFT;
  VasyGenerateSubstConst( VexLeft  );
  VexLeft  = simpvexexpr( VexLeft );
  VbhGenerate->FOR_LEFT = VexLeft;

  VexRight = VbhGenerate->FOR_RIGHT;
  VasyGenerateSubstConst( VexRight );
  VexRight = simpvexexpr( VexRight );
  VbhGenerate->FOR_RIGHT = VexRight;

  IndexName = GetVexAtomValue( VexAtom );  

  if ( ( evalvexatomlong( VexLeft, &Left   ) ) ||
       ( evalvexatomlong( VexRight, &Right ) ) )
  {
    VasyError( VASY_ERROR_IN_UNROLL_LOOP, (char *)0 );
  }

  OldLabel   = VasyGenerateLabel;
  IndexValue = Left;

  while ( ( ( IndexValue <= Right ) && (   VbhGenerate->FOR_UP ) ) ||
          ( ( IndexValue >= Right ) && ( ! VbhGenerate->FOR_UP ) ) )
  {
    sprintf( VasyBuffer, "%s_%s_%ld", OldLabel, VbhGenerate->LABEL, IndexValue );
    VasyGenerateLabel = namealloc( VasyBuffer );

    addauthelem( VasyHashForIndex, IndexName, IndexValue );

    if ( IsVasyDebugLevel0() )
    {
      fprintf( stdout, "%s = %ld\n", IndexName, IndexValue );
    }

    for ( ScanGenerate  = VbhGenerate->BEGNR;
          ScanGenerate != (vbgnr_list *)0;
          ScanGenerate  = ScanGenerate->NEXT )
    {
      if ( ScanGenerate->TYPE == VBH_GENERATE_FOR )
      {
        VasyGenerateFor( VbhFigure, ScanGenerate );
      }
      else
      {
        VasyGenerateIf( VbhFigure, ScanGenerate );
      }
    }

    VasyGenerateForDupInstance( VbhFigure, VbhGenerate );
    VasyGenerateForDupProcess(  VbhFigure, VbhGenerate );

    if ( VbhGenerate->FOR_UP ) IndexValue++;
    else                       IndexValue--;
  }

  delauthelem( VasyHashForIndex, IndexName ); 

  VasyGenerateLabel = OldLabel;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyGenerateFor %s\n", VbhGenerate->LABEL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyGenerateVbhFig                    |
|                                                             |
\------------------------------------------------------------*/

void VasyGenerateVbhFig( VbhFigure )

 vbfig_list *VbhFigure;
{
  vbgnr_list *VbhGenerate;
  vbgen_list *VbhGeneric;
  vbcst_list *VbhConstant;
  char       *Name;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyGenerateVbhFig %s\n", VbhFigure->NAME );
  }

  VasyHashConstant = createauthtable( 100 );
  VasyHashGeneric  = createauthtable( 100 );
  VasyHashForIndex = createauthtable( 100 );

  VasyGenerateLabel = VbhFigure->NAME;

  for ( VbhGeneric  = VbhFigure->BEGEN;
        VbhGeneric != (vbgen_list *)0;
        VbhGeneric  = VbhGeneric->NEXT )
  {
    Name = getvexatomname( VbhGeneric->TARGET );
    addauthelem( VasyHashGeneric, Name, (long)VbhGeneric );
    VasyGenerateSubstConst( VbhGeneric->VEX );
  }

  for ( VbhConstant  = VbhFigure->BECST;
        VbhConstant != (vbcst_list *)0;
        VbhConstant  = VbhConstant->NEXT )
  {
    Name = getvexatomname( VbhConstant->TARGET );
    addauthelem( VasyHashConstant, Name, (long)VbhConstant );
  }
/*
** Propagates all constants and generic values in their own declarations
*/
  for ( VbhGeneric  = VbhFigure->BEGEN;
        VbhGeneric != (vbgen_list *)0;
        VbhGeneric  = VbhGeneric->NEXT )
  {
    VasyGenerateSubstAllConst( VbhGeneric->VEX );
  }

  for ( VbhConstant  = VbhFigure->BECST;
        VbhConstant != (vbcst_list *)0;
        VbhConstant  = VbhConstant->NEXT )
  {
    VasyGenerateSubstAllConst( VbhConstant->VEX );
  }
/*
** Generate 
*/
  for ( VbhGenerate  = VbhFigure->BEGNR;
        VbhGenerate != (vbgnr_list *)0;
        VbhGenerate  = VbhGenerate->NEXT )
  {
    if ( VbhGenerate->TYPE == VBH_GENERATE_FOR )
    {
      VasyGenerateFor( VbhFigure, VbhGenerate );
    }
    else
    {
      VasyGenerateIf( VbhFigure, VbhGenerate );
    }
  }

  vbh_frevbgnr( VbhFigure->BEGNR );
  VbhFigure->BEGNR = (vbgnr_list *)0;

  destroyauthtable( VasyHashConstant );
  destroyauthtable( VasyHashGeneric  );
  destroyauthtable( VasyHashForIndex );

  if ( IsVasyDebugLevel1() )
  {
    vbh_viewvbfig( VbhFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyGenerateVbhFig %s\n\n", VbhFigure->NAME );
  }
}
