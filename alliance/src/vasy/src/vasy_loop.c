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
| File    :                  vasy_loop.c                      |
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
# include "vbh.h"

# include "vasy_loop.h"
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

  static char  *VasyForIndexName    = (char *)0;
  static long   VasyForIndexValue   = 0;
  static short  VasyForIndexSigned  = 0;
  static char  *VasyForLabel        = (char *)0;
  static char  *VasyForLabelNext    = (char *)0;
  static char  *VasyForLabelExit    = (char *)0;
  static short  VasyForFoundExit    = 0;
  static short  VasyForFoundNext    = 0;
  static long   VasyForEnterNewLoop = 0;

  static authtable *VasyHashLabel    = (authtable *)0;
  static authtable *VasyHashConstant = (authtable *)0;
  static authtable *VasyHashGeneric  = (authtable *)0;

  static char       VasyBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    VasyUnrollSubstConst                     |
|                                                             |
\------------------------------------------------------------*/

static void VasyUnrollSubstConst( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanOper;
  vbcst_list *VbhConst;
  vbgen_list *VbhGen;
  char       *AtomValue;
  vexexpr    *VexConst;
  authelem   *Element;

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
        VexExpr->VALUE = VexConst->VALUE;
        VexExpr->TYPE  = VexConst->TYPE;
        VexExpr->WIDTH = VexConst->WIDTH;
        VexExpr->LEFT  = VexConst->LEFT; 
        VexExpr->RIGHT = VexConst->RIGHT;
      }
    }
  }
  else
  {
    for ( ScanOper  = VexExpr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VasyUnrollSubstConst( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyUnrollForAddLabel                   |
|                                                             |
\------------------------------------------------------------*/

static char *VasyUnrollForAddLabel( Label )

  char *Label;
{
  char *NewLabel;

  if ( Label != (char *)0 )
  {
    sprintf( VasyBuffer, "unroll_%s_%ld", Label, VasyForIndexValue );
    NewLabel = namealloc( VasyBuffer );
    addauthelem( VasyHashLabel, Label, (long)NewLabel );
  }
  else
  {
    NewLabel = (char *)0;
  }

  return( NewLabel );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyUnrollForDelLabel                    |
|                                                             |
\------------------------------------------------------------*/

static void VasyUnrollForDelLabel( Label )

  char *Label;
{
  if ( Label != (char *)0 )
  {
    delauthelem( VasyHashLabel, Label );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyUnrollForRenameLabel               |
|                                                             |
\------------------------------------------------------------*/

static char *VasyUnrollForRenameLabel( Label )

  char *Label;
{
  authelem *Element;

  if ( Label != (char *)0 )
  {
    Element = searchauthelem( VasyHashLabel, Label );
    if ( Element != (authelem *)0 )
    {
      Label = (char *)Element->VALUE;
    }
  }

  return( Label );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyUnrollVexExpr                      |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyUnrollVexExpr( VexExpr )

  vexexpr *VexExpr;
{
  chain_list *ScanOper;
  vexexpr    *Operand;
  char       *Name;
  int         Width;

  if ( IsVexNodeAtom( VexExpr ) )
  {
    if ( ! IsVexAtomLiteral( VexExpr ) )
    {
      Name = GetVexAtomValue( VexExpr );

      if ( Name == VasyForIndexName )
      {
        Width = VexExpr->WIDTH;
        freevexexpr( VexExpr );

        VexExpr = createvexatomlong( VasyForIndexValue, Width, VasyForIndexSigned );
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
      Operand = VasyUnrollVexExpr( Operand );
      SetVexOperand( ScanOper, Operand );
    }
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyUnrollIsVexCondTrueFalse             |
|                                                             |
\------------------------------------------------------------*/

static int VasyUnrollIsVexCondTrueFalse( VexExpr )

  vexexpr *VexExpr;
{
  char *AtomValue;

  if ( VexExpr == (vexexpr *)0 ) return( 1 );

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
|                      VasyUnrollForBodyVexExpr               |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VasyUnrollForBodyVexExpr( VexExpr )

  vexexpr *VexExpr;
{
  if ( VexExpr != (vexexpr *)0 )
  {
    VexExpr = dupvexexpr( VexExpr );
    VexExpr = VasyUnrollVexExpr( VexExpr ); 
    VexExpr = simpvexexpr( VexExpr );
  }

  return( VexExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyUnrollForBody                      |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *VasyUnrollForBody( Instruction )

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
  int          TrueFalse;

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

        NewAsg->TARGET = VasyUnrollForBodyVexExpr( ScanAsg->TARGET );
        NewAsg->VEX    = VasyUnrollForBodyVexExpr( ScanAsg->VEX    );
        NewAsg->TYPE   = ScanAsg->TYPE;
        NewAsg->LINE   = ScanAsg->LINE;

      break;

      case VBH_BEVAR :

        ScanVar = (vbvar_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbvar_list ) );
        NewVar  = (vbvar_list *)Pointer;

        NewVar->TARGET = VasyUnrollForBodyVexExpr( ScanVar->TARGET );
        NewVar->VEX    = VasyUnrollForBodyVexExpr( ScanVar->VEX    );
        NewVar->TYPE   = ScanVar->TYPE;
        NewVar->LINE   = ScanVar->LINE;

      break;

      case VBH_BENXT :

        ScanNext = (vbnxt_list *)Instruction->DATA;
/*
** Special treatments for NEXT instruction
*/
        if ( ( ScanNext->LABEL == VasyForLabel      ) ||
             ( ( ScanNext->LABEL     == (char *)0 ) &&
               ( VasyForEnterNewLoop == 0         ) ) )
        {
          InstType = VBH_BEEXT;

          ScanExit = (vbext_list *)Instruction->DATA;
          Pointer  = autallocheap( sizeof( vbext_list ) );
          NewExit  = (vbext_list *)Pointer;

          NewExit->LINE  = ScanNext->LINE;
          NewExit->CND   = VasyUnrollForBodyVexExpr( ScanNext->CND );

          TrueFalse = VasyUnrollIsVexCondTrueFalse( NewExit->CND );

          if ( TrueFalse == 0 )
          {
            freevexexpr( NewExit->CND );
            autfreeheap( Pointer, sizeof( vbext_list ) );
            Pointer = (void *)0;
          }
          else
          {
            if ( TrueFalse == 1 )
            {
              freevexexpr( NewExit->CND );
              NewExit->CND = (vexexpr *)0;
            }

            NewExit->LABEL = VasyForLabelNext;
          }

          VasyForFoundNext = 1;
        }
        else
        {
          Pointer  = autallocheap( sizeof( vbnxt_list ) );
          NewNext  = (vbnxt_list *)Pointer;

          NewNext->LINE = ScanNext->LINE;
          NewNext->CND  = VasyUnrollForBodyVexExpr( ScanNext->CND );

          TrueFalse = VasyUnrollIsVexCondTrueFalse( NewNext->CND );

          if ( TrueFalse == 0 )
          {
            freevexexpr( NewNext->CND );
            autfreeheap( Pointer, sizeof( vbnxt_list ) );
            Pointer = (void *)0;
          }
          else
          {
            if ( TrueFalse == 1 )
            {
              freevexexpr( NewNext->CND );
              NewNext->CND = (vexexpr *)0;
            }

            NewNext->LABEL = VasyUnrollForRenameLabel( ScanNext->LABEL );
          }

        }

      break;

      case VBH_BEEXT :
/*
** Special treatments for EXIT instruction
*/
        ScanExit = (vbext_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vbext_list ) );
        NewExit  = (vbext_list *)Pointer;

        NewExit->LINE  = ScanExit->LINE;
        NewExit->CND   = VasyUnrollForBodyVexExpr( ScanExit->CND );

        if ( ( ScanExit->LABEL == VasyForLabel      ) ||
             ( ( ScanExit->LABEL     == (char *)0 ) &&
               ( VasyForEnterNewLoop == 0         ) ) )
        {
          VasyForFoundExit = 1;
          NewExit->LABEL = VasyForLabelExit;
        }
        else
        {
          NewExit->LABEL = VasyUnrollForRenameLabel( ScanExit->LABEL );
        }

        TrueFalse = VasyUnrollIsVexCondTrueFalse( NewExit->CND );

        if ( TrueFalse == 0 )
        {
          freevexexpr( NewExit->CND );
          autfreeheap( Pointer, sizeof( vbext_list ) );
          Pointer = (void *)0;
        }
        else
        if ( TrueFalse == 1 )
        {
          freevexexpr( NewExit->CND );
          NewExit->CND = (vexexpr *)0;
        }

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

        NewWait->CND       = VasyUnrollForBodyVexExpr( ScanWait->CND );
        NewWait->TIME_UNIT = ScanWait->TIME_UNIT;
        NewWait->TIMEOUT   = ScanWait->TIMEOUT;
        NewWait->LINE      = ScanWait->LINE;

      break;

      case VBH_BERET :

        ScanReturn = (vbret_list *)Instruction->DATA;
        Pointer    = autallocheap( sizeof( vbret_list ) );
        NewReturn  = (vbret_list *)Pointer;

        NewReturn->RET  = VasyUnrollForBodyVexExpr( ScanReturn->RET );
        NewReturn->LINE = ScanReturn->LINE;

      break;

      case VBH_BECAL :

        ScanCall = (vbcal_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vbcal_list ) );
        NewCall  = (vbcal_list *)Pointer;

        NewCall->CALL = VasyUnrollForBodyVexExpr( ScanCall->CALL );
        NewCall->LINE = ScanCall->LINE;

      break;

      case VBH_BEAGR :

        ScanAgr = (vbagr_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbagr_list ) );
        NewAgr  = (vbagr_list *)Pointer;

        NewAgr->ASSIGN = VasyUnrollForBody( ScanAgr->ASSIGN );
        NewAgr->LINE   = ScanAgr->LINE;

      break;

      case VBH_BEIFS : 

        ScanIfs = (vbifs_list *)Instruction->DATA;
        Pointer = autallocheap( sizeof( vbifs_list ) );
        NewIfs  = (vbifs_list *)Pointer;

        NewIfs->CND      = VasyUnrollForBodyVexExpr( ScanIfs->CND );
        NewIfs->CNDTRUE  = VasyUnrollForBody( ScanIfs->CNDTRUE  );
        NewIfs->CNDFALSE = VasyUnrollForBody( ScanIfs->CNDFALSE );
        NewIfs->LINE     = ScanIfs->LINE;

      break;

      case VBH_BECAS : 

        ScanCase = (vbcas_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vbcas_list ) );
        NewCase  = (vbcas_list *)Pointer;

        NewCase->VEX  = VasyUnrollForBodyVexExpr( ScanCase->VEX );
        NewCase->TYPE = ScanCase->TYPE;
        NewCase->LINE = ScanCase->LINE;

        NewCase->SIZE   = ScanCase->SIZE;
        NewCase->CHOICE = (vbcho_list *)autallocblock( ScanCase->SIZE * sizeof( vbcho_list ) );

        for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
        {
          ScanChoice = &ScanCase->CHOICE[ Offset ];
          NewChoice  = &NewCase->CHOICE[ Offset ];

          NewChoice->INSTRUCTION = VasyUnrollForBody( ScanChoice->INSTRUCTION );

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

        VasyUnrollForAddLabel( ScanFor->LABEL );

        VasyForEnterNewLoop++;
        NewFor->INSTRUCTION = VasyUnrollForBody( ScanFor->INSTRUCTION );
        VasyForEnterNewLoop--;

        NewFor->LABEL    = VasyUnrollForRenameLabel( ScanFor->LABEL );
        NewFor->VARIABLE = VasyUnrollForBodyVexExpr( ScanFor->VARIABLE );
        NewFor->LEFT     = VasyUnrollForBodyVexExpr( ScanFor->LEFT     );
        NewFor->RIGHT    = VasyUnrollForBodyVexExpr( ScanFor->RIGHT    );
        NewFor->UP       = ScanFor->UP;
        NewFor->LINE     = ScanFor->LINE;

        VasyUnrollForDelLabel( ScanFor->LABEL );

      break;

      case VBH_BEWHI : 

        ScanWhile = (vbwhi_list *)Instruction->DATA;
        Pointer   = autallocheap( sizeof( vbwhi_list ) );
        NewWhile  = (vbwhi_list *)Pointer;

        VasyUnrollForAddLabel( ScanWhile->LABEL );

        VasyForEnterNewLoop++;
        NewWhile->INSTRUCTION = VasyUnrollForBody( ScanWhile->INSTRUCTION ); 
        VasyForEnterNewLoop--;

        NewWhile->LABEL = VasyUnrollForRenameLabel( ScanWhile->LABEL );
        NewWhile->CND   = VasyUnrollForBodyVexExpr( ScanWhile->CND );
        NewWhile->LINE  = ScanWhile->LINE;

        VasyUnrollForDelLabel( ScanWhile->LABEL );

      break;

      case VBH_BELOP : 

        ScanLoop = (vblop_list *)Instruction->DATA;
        Pointer  = autallocheap( sizeof( vblop_list ) );
        NewLoop  = (vblop_list *)Pointer;

        VasyUnrollForAddLabel( ScanLoop->LABEL );

        VasyForEnterNewLoop++;
        NewLoop->INSTRUCTION = VasyUnrollForBody( ScanLoop->INSTRUCTION );
        VasyForEnterNewLoop--;

        NewLoop->LABEL = VasyUnrollForRenameLabel( ScanLoop->LABEL );
        NewLoop->LINE  = ScanLoop->LINE;

        VasyUnrollForDelLabel( ScanLoop->LABEL );

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
|                   VasyUnrollForInstruction                  |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *VasyUnrollForInstruction( Instruction )

  ptype_list *Instruction;
{
  ptype_list  *NewBody;
  ptype_list **PrevPType;
  ptype_list **PrevPType2;
  ptype_list  *ScanPType;
  vblop_list  *NewLoop;
  ptype_list  *NewExit;
  vbfor_list  *ScanFor;
  vexexpr     *VexLeft;
  vexexpr     *VexRight;
  vexexpr     *VexAtom;
  ptype_list  *HeadBody;
  long         Left;
  long         Right;

  ScanFor  = (vbfor_list *)Instruction->DATA;
  VexAtom  = ScanFor->VARIABLE;

  VexLeft  = ScanFor->LEFT;
  VasyUnrollSubstConst( VexLeft  );
  VexLeft  = simpvexexpr( VexLeft );
  ScanFor->LEFT = VexLeft;

  VexRight = ScanFor->RIGHT;
  VasyUnrollSubstConst( VexRight );
  VexRight = simpvexexpr( VexRight );
  ScanFor->RIGHT = VexRight;

  VasyForIndexName = GetVexAtomValue( VexAtom );  

  if ( ( evalvexatomlong( VexLeft, &Left   ) ) ||
       ( evalvexatomlong( VexRight, &Right ) ) )
  {
    viewvexexprln( VexLeft ); viewvexexprln( VexRight );
    VasyError( VASY_ERROR_IN_UNROLL_LOOP, (char *)0 );
  }

  if ( ( Left  < 0 ) ||
       ( Right < 0 ) ) VasyForIndexSigned = 1;
  else                 VasyForIndexSigned = 0;

  VasyForIndexValue = Left;
  VasyForFoundNext  = 0;
  VasyForFoundExit  = 0;
  VasyForLabel      = ScanFor->LABEL;

  PrevPType = &HeadBody;
  HeadBody  = (ptype_list *)0;

  sprintf( VasyBuffer, "%s_exit", ScanFor->LABEL );
  VasyForLabelExit = namealloc( VasyBuffer );

  while ( ( ( VasyForIndexValue <= Right ) && (   ScanFor->UP ) ) ||
          ( ( VasyForIndexValue >= Right ) && ( ! ScanFor->UP ) ) )
  {
    sprintf( VasyBuffer, "%s_next_%ld", ScanFor->LABEL, VasyForIndexValue );
    VasyForLabelNext = namealloc( VasyBuffer );

    NewBody = VasyUnrollForBody( ScanFor->INSTRUCTION );

    if ( VasyForFoundNext )
    {
      ScanPType = vbh_addvblop( (ptype_list *)0, VasyForLabelNext, ScanFor->LINE );
      NewLoop   = (vblop_list *)ScanPType->DATA;
      NewLoop->INSTRUCTION = NewBody;

      *PrevPType = ScanPType;
      PrevPType  = &ScanPType->NEXT;

      PrevPType2 = &NewBody;

      for ( ScanPType  = NewBody;
            ScanPType != (ptype_list *)0;
            ScanPType  = ScanPType->NEXT )
      {
        PrevPType2 = &ScanPType->NEXT;
      }
      
      NewExit = addptype( (ptype_list *)0, VBH_BEEXT, 
                          vbh_addvbext( VasyForLabelNext, (vexexpr *)0, ScanFor->LINE ) );
      *PrevPType2 = NewExit;
    }
    else
    {
      *PrevPType = NewBody;

      for ( ScanPType  = NewBody;
            ScanPType != (ptype_list *)0;
            ScanPType  = ScanPType->NEXT )
      {
        PrevPType = &ScanPType->NEXT;
      }
    }

    if ( ScanFor->UP ) VasyForIndexValue++;
    else               VasyForIndexValue--;
  }

  if ( VasyForFoundExit )
  {
    NewExit = addptype( (ptype_list *)0, VBH_BEEXT, 
                        vbh_addvbext( VasyForLabelExit, (vexexpr *)0, ScanFor->LINE ) );
    *PrevPType = NewExit;

    ScanPType = vbh_addvblop( (ptype_list *)0, VasyForLabelExit, ScanFor->LINE );
    NewLoop   = (vblop_list *)ScanPType->DATA;
    NewLoop->INSTRUCTION = HeadBody;

    HeadBody  = ScanPType;
    PrevPType = &ScanPType->NEXT;
  }

  if ( IsVasyDebugLevel2() )
  {
    vbh_viewvbinst( HeadBody );
  }

  *PrevPType = Instruction->NEXT;

  Instruction->NEXT = (ptype_list *)0;
  vbh_frevbinst( Instruction );

  return( HeadBody );
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyUnrollScanInstruction                 |
|                                                             |
\------------------------------------------------------------*/

static ptype_list *VasyUnrollScanInstruction( Instruction )

  ptype_list *Instruction;
{
  ptype_list  *ScanPType;
  ptype_list **PrevPType;
  vbifs_list  *ScanIfs;
  vbcas_list  *ScanCase;
  vbcho_list  *ScanChoice;
  vblop_list  *ScanLoop;
  vbwhi_list  *ScanWhile;
  int          Offset;

  ScanPType = Instruction;
  PrevPType = &Instruction;

  while ( ScanPType != (ptype_list *)0 )
  {
    switch ( ScanPType->TYPE )
    {
      case VBH_BEIFS : 

        ScanIfs = (vbifs_list *)ScanPType->DATA;
        ScanIfs->CNDTRUE  = VasyUnrollScanInstruction( ScanIfs->CNDTRUE  );
        ScanIfs->CNDFALSE = VasyUnrollScanInstruction( ScanIfs->CNDFALSE );

      break;

      case VBH_BECAS : 

        ScanCase = (vbcas_list *)ScanPType->DATA;
        for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
        {
          ScanChoice = &ScanCase->CHOICE[ Offset ];
          ScanChoice->INSTRUCTION = VasyUnrollScanInstruction( ScanChoice->INSTRUCTION );
        }

      break;

      case VBH_BEFOR : 

        ScanPType = VasyUnrollForInstruction( ScanPType );
        *PrevPType = ScanPType;

        continue;

      break;

      case VBH_BEWHI : 

        ScanWhile = (vbwhi_list *)ScanPType->DATA;
        ScanWhile->INSTRUCTION = VasyUnrollScanInstruction( ScanWhile->INSTRUCTION );

      break;

      case VBH_BELOP : 

        ScanLoop = (vblop_list *)ScanPType->DATA;
        ScanLoop->INSTRUCTION = VasyUnrollScanInstruction( ScanLoop->INSTRUCTION );

      break;
    }

    PrevPType = &ScanPType->NEXT;
    ScanPType = ScanPType->NEXT;
  }

  return( Instruction );
}

/*------------------------------------------------------------\
|                                                             |
|                       VasyUnrollLoopVbhFig                  |
|                                                             |
\------------------------------------------------------------*/

void VasyUnrollLoopVbhFig( VbhFigure )

 vbfig_list *VbhFigure;
{
  vbpcs_list *VbhProcess;
  vbgen_list *VbhGeneric;
  vbcst_list *VbhConstant;
  char       *Name;

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyUnrollLoopVbhFig %s\n", VbhFigure->NAME );
  }

  VasyHashLabel    = createauthtable( 100 );
  VasyHashConstant = createauthtable( 100 );
  VasyHashGeneric  = createauthtable( 100 );

  for ( VbhGeneric  = VbhFigure->BEGEN;
        VbhGeneric != (vbgen_list *)0;
        VbhGeneric  = VbhGeneric->NEXT )
  {
    Name = getvexatomname( VbhGeneric->TARGET );
    addauthelem( VasyHashGeneric, Name, (long)VbhGeneric );
  }

  for ( VbhConstant  = VbhFigure->BECST;
        VbhConstant != (vbcst_list *)0;
        VbhConstant  = VbhConstant->NEXT )
  {
    Name = getvexatomname( VbhConstant->TARGET );
    addauthelem( VasyHashConstant, Name, (long)VbhConstant );
  }

  for ( VbhProcess  = VbhFigure->BEPCS;
        VbhProcess != (vbpcs_list *)0;
        VbhProcess  = VbhProcess->NEXT )
  {
    if ( ! IsVbhProcSequential( VbhProcess ) ) continue;

    VbhProcess->INSTRUCTION = VasyUnrollScanInstruction( VbhProcess->INSTRUCTION );
  }

  destroyauthtable( VasyHashLabel ); 
  destroyauthtable( VasyHashConstant );
  destroyauthtable( VasyHashGeneric );

  if ( IsVasyDebugLevel1() )
  {
    vbh_viewvbfig( VbhFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyUnrollLoopVbhFig %s\n\n", VbhFigure->NAME );
  }
}
