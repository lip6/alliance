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
| Tool    :                     SYF                           |
|                                                             |
| File    :                  syf_synth.c                      |
|                                                             |
| Authors :                  C. Sarwary                       |
|                     Modified by Jacomme Ludovic             |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include FSM_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_dc.h"
# include "syf_synth.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthReturn2Abl                     |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthReturn2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstack_list *ScanStack;
  syfstate      *ReturnSyfState;
  syfstate      *CurrentSyfState;
  syfinfo       *SyfInfo;
  ablexpr    *Equation;
  ablexpr    *Atom;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  if ( ! SyfInfo->STACK ) return;

  for ( ScanStack  = FsmFigure->STACK;
        ScanStack != (fsmstack_list *)0;
        ScanStack  = ScanStack->NEXT )
  {
    if ( ScanStack->CTRL == FSM_CTRL_PUSH )
    {
      ReturnSyfState  = FSM_SYF_STATE( ScanStack->RETURN  );

      if ( IsFsmStarStack( ScanStack ) )
      {
        Equation = dupablexpr( ScanStack->ABL );
      }
      else
      {
        CurrentSyfState = FSM_SYF_STATE( ScanStack->CURRENT );

        Atom = createablatom( CurrentSyfState->CURRENT_NAME );
        Equation = optimablbinexpr( ABL_AND, 
                                     dupablexpr( ScanStack->ABL ),
                                     Atom );
      }

      if ( ReturnSyfState->ABL_RETURN == (ablexpr *)0 )
      {
        ReturnSyfState->ABL_RETURN = createabloper( ABL_OR );
      }

      addablhexpr( ReturnSyfState->ABL_RETURN, Equation );
    }
  }

  for ( ScanStack  = FsmFigure->STACK;
        ScanStack != (fsmstack_list *)0;
        ScanStack  = ScanStack->NEXT )
  {
    if ( ScanStack->CTRL == FSM_CTRL_PUSH )
    {
      ReturnSyfState = FSM_SYF_STATE( ScanStack->RETURN );
      Equation       = ReturnSyfState->ABL_RETURN;

      if ( ( Equation             != (ablexpr *)0 ) &&
           ( ABL_CDDR( Equation ) == (ablexpr *)0 ) )
      {
        Atom = ABL_CADR( Equation );
        ABL_CADR( Equation ) = (ablexpr *)0;
        freeablexpr( Equation );
        ReturnSyfState->ABL_RETURN = Atom;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   SyfSynthFreeReturnAbl                     |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeReturnAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo       *SyfInfo;
  syfstate      *ReturnSyfState;
  fsmstack_list *ScanStack;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  if ( SyfInfo->STACK )
  {
    for ( ScanStack  = FsmFigure->STACK;
          ScanStack != (fsmstack_list *)0;
          ScanStack  = ScanStack->NEXT )
    {
      if ( ScanStack->CTRL == FSM_CTRL_PUSH )
      {
        ReturnSyfState = FSM_SYF_STATE( ScanStack->RETURN  );

        if ( ReturnSyfState->ABL_RETURN != (ablexpr *)0 )
        {
          delablexpr( ReturnSyfState->ABL_RETURN );
          ReturnSyfState->ABL_RETURN = (ablexpr *)0;
        }
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthStack2Abl                      |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthStack2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstack_list *ScanStack;
  syfstate      *ReturnSyfState;
  syfinfo       *SyfInfo;
  syfregstack   *StackArray;
  syfregstack   *StackNextArray;
  syfregstack   *StackPrevArray;
  syfregstate   *RegArray;
  syfctrl       *CtrlArray;
  long           Constant;
  long           StackMask;
  long           StackNewMask;
  long           StackCste;
  long           StackNewCste;
  long           ScanBit;
  long           BitMask;
  long           Index;
  ablexpr    *Stack;
  ablexpr    *Equation;
  ablexpr    *ScanPath;
  ablexpr    *Atom;
  char          *PrevStack;
  char           Encode;

  SyfInfo = FSM_SYF_INFO( FsmFigure );
  Encode  = SyfInfo->ENCODE;

  if ( ! SyfInfo->STACK ) return;

  RegArray   = SyfInfo->REG_ARRAY;
  CtrlArray  = SyfInfo->CTRL_ARRAY;
  StackArray = SyfInfo->STACK_ARRAY;
  StackCste  = -1;
  StackMask  = 0;

  if ( Encode != SYF_ENCODE_ONE_HOT )
  {
    for ( ScanStack  = FsmFigure->STACK;
          ScanStack != (fsmstack_list *)0;
          ScanStack  = ScanStack->NEXT )
    {
      if ( ScanStack->CTRL == FSM_CTRL_PUSH )
      {
        ReturnSyfState  = FSM_SYF_STATE( ScanStack->RETURN  );
        StackNewCste    = ReturnSyfState->CODE->VALUE;

        if ( StackCste == -1 )
        {
          StackCste = StackNewCste;
          StackMask = -1;
        }
        else
        {
          StackNewMask = ~(StackCste ^ StackNewCste);
          StackMask    = StackMask & StackNewMask;
        }
      }
    }

/*
**  Push state in Stack_0 equation
*/
    for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
    {
      StackArray[ ScanBit ].ABL = createabloper( ABL_OR );
    }

    for ( ScanStack  = FsmFigure->STACK;
          ScanStack != (fsmstack_list *)0;
          ScanStack  = ScanStack->NEXT )
    {
      if ( ScanStack->CTRL == FSM_CTRL_PUSH )
      {
        ReturnSyfState = FSM_SYF_STATE( ScanStack->RETURN );
        StackNewCste   = ReturnSyfState->CODE->VALUE;

        BitMask = 1;

        for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
        {
          if ( BitMask & StackNewCste )
          {
            Atom = createablatom( ReturnSyfState->RETURN_NAME );

            addablhexpr( StackArray[ ScanBit ].ABL, Atom );
          }

          BitMask = BitMask << 1;
        }
      }
    }

    for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
    {
      Equation = StackArray[ ScanBit ].ABL;

      if ( ABL_CDR( Equation ) == (ablexpr *)0 )
      {
        delablexpr( Equation );
        StackArray[ ScanBit ].ABL = createablatomzero();
      }
      else
      if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
      {
        Atom = ABL_CADR( Equation );
        ABL_CADR( Equation ) = (ablexpr *)0;
        freeablexpr( Equation );
        StackArray[ ScanBit ].ABL = Atom;
      }
    }
  }
  else
  {
/*
**  Push state in Stack_0 equation
*/
    for ( ScanStack  = FsmFigure->STACK;
          ScanStack != (fsmstack_list *)0;
          ScanStack  = ScanStack->NEXT )
    {
      if ( ScanStack->CTRL == FSM_CTRL_PUSH )
      {
        ReturnSyfState = FSM_SYF_STATE( ScanStack->RETURN );
        ScanBit        = ReturnSyfState->CODE->VALUE;

        StackArray[ ScanBit ].ABL = createablatom( ReturnSyfState->RETURN_NAME );
      }
    }

    for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
    {
      Equation = StackArray[ ScanBit ].ABL;

      if ( Equation == (ablexpr *)0 )
      {
        StackArray[ ScanBit ].ABL   = createablatomzero();
        StackArray[ ScanBit ].FLAGS = 1;
      }
    }
  }

  SyfInfo->STACK_MASK = StackMask;
  SyfInfo->STACK_CSTE = StackCste & StackMask;

  StackPrevArray = (syfregstack *)0;

  for ( Index = 0; Index < FsmFigure->STACK_SIZE; Index++ )
  {
    if ( Index + 1 < FsmFigure->STACK_SIZE )
    {
      StackNextArray = SyfInfo->STACK_ARRAY + ( SyfInfo->NUMBER_REG * ( Index + 1 ) );
    }

    BitMask  = 1;

    for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
    {
      if ( Encode == SYF_ENCODE_ONE_HOT )
      {
        Constant = SyfInfo->STACK_ARRAY[ ScanBit ].FLAGS;
      }
      else
      {
        Constant = ( BitMask & StackMask );
      }
/*
**  Special case for constants
*/
      if ( Constant )
      {
        StackArray[ ScanBit ].FLAGS = 1;

        if ( Encode != SYF_ENCODE_ONE_HOT )
        {
/*
**  Constant equation only for Stack_0
*/
          if ( Index == 0 )
          {
            delablexpr( StackArray[ ScanBit ].ABL );

            if ( StackCste & BitMask )
            {
              StackArray[ ScanBit ].ABL = createablatomone();
            }
            else
            {
              StackArray[ ScanBit ].ABL = createablatomzero();
            }
          }
        }
      }
      else
      {
/*
**  Generates Stack register equation
*/
        Stack = createabloper( ABL_OR );
        Atom  = createablatom( CtrlArray[ FSM_CTRL_NOP ].NAME );

        Equation = createablatom( StackArray[ ScanBit ].NAME_OUT );
        Equation = optimablbinexpr( ABL_AND, Equation, Atom );

        addablhexpr( Stack, Equation );

        Atom = createablatom( CtrlArray[ FSM_CTRL_PUSH ].NAME );

        if ( Index == 0 )
        {
          Equation = StackArray[ ScanBit ].ABL;
        }
        else
        {
          Equation = createablatom( StackPrevArray[ ScanBit ].NAME_OUT );
        }

        Equation = optimablbinexpr( ABL_AND, Equation, Atom );

        addablhexpr( Stack, Equation );

        if ( Index + 1 < FsmFigure->STACK_SIZE )
        {
          Atom = createablatom( CtrlArray[ FSM_CTRL_POP ].NAME );

          Equation = createablatom( StackNextArray[ ScanBit ].NAME_OUT );
          Equation = optimablbinexpr( ABL_AND, Equation, Atom );

          addablhexpr( Stack, Equation );
        }

        StackArray[ ScanBit ].ABL = Stack;
      }

      BitMask = BitMask << 1;
    }

    StackPrevArray = StackArray;
    StackArray     = StackNextArray;
  }

  if ( SyfInfo->SCAN_PATH )
  {
    StackArray = SyfInfo->STACK_ARRAY;
    PrevStack  = SyfInfo->SCAN_IN;

    for ( Index = 0; Index < SyfInfo->NUMBER_STACK; Index++ )
    {
      if ( ! StackArray[ Index ].FLAGS )
      {
        ScanPath = optimablbinexpr( ABL_AND,
                                  createablatom( PrevStack ),
                                  createablatom( SyfInfo->SCAN_TEST ) );

        Equation = optimablbinexpr( ABL_AND,
                                  StackArray[ Index ].ABL,
                                  optimablnotexpr( createablatom( SyfInfo->SCAN_TEST ) ) );

        StackArray[ Index ].ABL = optimablbinexpr( ABL_OR, Equation, ScanPath );

        PrevStack = StackArray[ Index ].NAME_OUT;
      }
    }

    SyfInfo->SCAN_STACK = PrevStack;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   SyfSynthFreeStackAbl                      |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeStackAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo       *SyfInfo;
  syfregstack   *StackArray;
  syfstate      *ReturnSyfState;
  fsmstack_list *ScanStack;
  long           Index;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  if ( SyfInfo->STACK )
  {
    StackArray = SyfInfo->STACK_ARRAY;

    for ( Index = 0; Index < SyfInfo->NUMBER_STACK; Index++ )
    {
      StackArray[ Index ].FLAGS = 0;

      if ( StackArray[ Index ].ABL != (ablexpr *)0 )
      {
        delablexpr( StackArray[ Index ].ABL );
        StackArray[ Index ].ABL = (ablexpr *)0;
      }
    }

    for ( ScanStack  = FsmFigure->STACK;
          ScanStack != (fsmstack_list *)0;
          ScanStack  = ScanStack->NEXT )
    {
      if ( ScanStack->CTRL == FSM_CTRL_PUSH )
      {
        ReturnSyfState = FSM_SYF_STATE( ScanStack->RETURN  );

        if ( ReturnSyfState->ABL_RETURN != (ablexpr *)0 )
        {
          delablexpr( ReturnSyfState->ABL_RETURN );
          ReturnSyfState->ABL_RETURN = (ablexpr *)0;
        }
      }
    }
  }
}
  
/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthCtrl2Abl                       |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthCtrl2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstack_list *ScanStack;
  syfstate      *ScanSyfState;
  syfinfo       *SyfInfo;
  syfctrl       *CtrlArray;
  ablexpr    *Equation;
  ablexpr    *CtrlNop;
  ablexpr    *Atom;
  long           Index;

  SyfInfo   = FSM_SYF_INFO( FsmFigure );
  CtrlArray = SyfInfo->CTRL_ARRAY;

  if ( ! SyfInfo->STACK ) return;

  for ( Index = 0; Index < SyfInfo->NUMBER_CTRL; Index++ )
  {
    if ( Index != FSM_CTRL_NOP )
    {
      CtrlArray[ Index ].ABL = createabloper( ABL_OR );
    }
  }

  for ( ScanStack  = FsmFigure->STACK;
        ScanStack != (fsmstack_list *)0;
        ScanStack  = ScanStack->NEXT )
  {
    if ( ScanStack->CTRL != FSM_CTRL_NOP )
    {
      if ( IsFsmStarStack( ScanStack ) )
      {
        Equation = dupablexpr( ScanStack->ABL );
      }
      else
      {
        if ( ScanStack->CTRL == FSM_CTRL_PUSH )
        {
          ScanSyfState = FSM_SYF_STATE( ScanStack->RETURN );
          Equation = createablatom( ScanSyfState->RETURN_NAME );
        }
        else
        {
          ScanSyfState = FSM_SYF_STATE( ScanStack->CURRENT );
          Atom = createablatom( ScanSyfState->CURRENT_NAME );

          Equation = optimablbinexpr( ABL_AND, 
                                       dupablexpr( ScanStack->ABL ),
                                       Atom );
        }
      }

      addablhexpr( CtrlArray[ ScanStack->CTRL ].ABL, Equation );
    }
  }

  CtrlNop = createabloper( ABL_OR );

  for ( Index = 0; Index < SyfInfo->NUMBER_CTRL; Index++ )
  {
    if ( Index != FSM_CTRL_NOP )
    {
      Equation = CtrlArray[ Index ].ABL;

      if ( ABL_CDR( Equation ) == (ablexpr *)0 )
      {
        Atom = createablatomzero();
        delablexpr( Equation );
        CtrlArray[ Index ].ABL = Atom;
      }
      else
      if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
      {
        Atom = ABL_CADR( Equation );
        ABL_CADR( Equation ) = (ablexpr *)0;
        freeablexpr( Equation );
        CtrlArray[ Index ].ABL = Atom;
      }
     
      Atom = createablatom( CtrlArray[ Index ].NAME );

      addablhexpr( CtrlNop, Atom );
    }
  }

  CtrlArray[ FSM_CTRL_NOP ].ABL = optimablnotexpr( CtrlNop );
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthFreeCtrlAbl                    |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeCtrlAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo       *SyfInfo;
  syfctrl       *CtrlArray;
  long           Index;

  SyfInfo   = FSM_SYF_INFO( FsmFigure );
  CtrlArray = SyfInfo->CTRL_ARRAY;

  if ( SyfInfo->STACK )
  {
    for ( Index = 0; Index < SyfInfo->NUMBER_CTRL; Index++ )
    {
      if ( CtrlArray[ Index ].ABL != (ablexpr *)0 )
      {
        delablexpr( CtrlArray[ Index ].ABL );
        CtrlArray[ Index ].ABL = (ablexpr *)0;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfSynthOut2Abl                       |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthOut2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  syfregout      *OutArray;
  fsmout_list    *ScanOutput;
  fsmout_list    *ScanOut;
  fsmstate_list  *ScanState;
  fsmlocout_list *ScanLocout;
  syfout         *ScanSyfOut;
  syfstate       *ScanSyfState;
  syfcode        *ScanSyfCode;
  ablexpr        *Equation;
  ablexpr        *ScanPath;
  ablexpr        *Atom;
  char           *PrevScan;
  long            Index;

  SyfInfo  = FSM_SYF_INFO( FsmFigure );
  OutArray = SyfInfo->OUT_ARRAY;
  ScanOut  = (fsmout_list *)0;

  for ( ScanOutput  = FsmFigure->OUT;
        ScanOutput != (fsmout_list *)0;
        ScanOutput  = ScanOutput->NEXT )
  {
    ScanSyfOut = FSM_SYF_OUT( ScanOutput );

    if ( SyfInfo->SCAN_OUT == ScanOutput->NAME )
    {
      ScanOut = ScanOutput;
    }

    ScanSyfOut->ABL = createabloper( ABL_OR );
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = FSM_SYF_STATE( ScanState );
    ScanSyfCode  = ScanSyfState->CODE;

    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      ScanSyfOut = FSM_SYF_OUT( ScanLocout->OUT );

      if ( SyfInfo->REG_OUT )
      {
        if ( SyfInfo->STACK )
        {
          Atom = createablatom( ScanSyfState->NEXT_IN_NAME );
        }
        else
        {
          Atom = createablatom( ScanSyfState->NEXT_NAME );
        }
      }
      else
      {
        Atom = createablatom( ScanSyfState->CURRENT_NAME );
      }

      if ( ScanLocout->ABL_DC != (ablexpr *)0 )
      {
        Equation = optimablbinexpr( ABL_AND, 
                                     dupablexpr( ScanLocout->ABL_DC ),
                                     createablatom( ABL_ATOM_NAME_DC ) );
      }

      if ( ScanLocout->ABL != (ablexpr *)0 )
      {
        if ( ScanLocout->ABL_DC != (ablexpr *)0 )
        {
          Equation = optimablbinexpr( ABL_OR, 
                                       dupablexpr( ScanLocout->ABL ),
                                       Equation );
        }
        else
        {
          Equation = dupablexpr( ScanLocout->ABL );
        }
      }

      Equation = optimablbinexpr( ABL_AND, Atom, Equation );

      addablhexpr( ScanSyfOut->ABL, Equation );
    }
  }

  ScanState = FsmFigure->STAR_STATE;

  if ( ScanState != (fsmstate_list *)0 )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      ScanSyfOut = FSM_SYF_OUT( ScanLocout->OUT );

      if ( ScanLocout->ABL_DC != (ablexpr *)0 )
      {
        Equation = optimablbinexpr( ABL_AND, 
                                     dupablexpr( ScanLocout->ABL_DC ),
                                     createablatom( ABL_ATOM_NAME_DC ) );
      }

      if ( ScanLocout->ABL != (ablexpr *)0 )
      {
        if ( ScanLocout->ABL_DC != (ablexpr *)0 )
        {
          Equation = optimablbinexpr( ABL_OR, 
                                       dupablexpr( ScanLocout->ABL ),
                                       Equation );
        }
        else
        {
          Equation = dupablexpr( ScanLocout->ABL );
        }
      }

      addablhexpr( ScanSyfOut->ABL, Equation );
    }
  }
 
  for ( ScanOutput  = FsmFigure->OUT;
        ScanOutput != (fsmout_list *)0;
        ScanOutput  = ScanOutput->NEXT )
  {
    ScanSyfOut = FSM_SYF_OUT( ScanOutput );

    Equation = ScanSyfOut->ABL;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Out(i) = OR Null -> Out(i) = 0
*/
      if ( ScanOutput != ScanOut )
      {
        Atom = createablatomzero();
      }
      else
      {
        Atom = createablatom( SyfInfo->SCAN_IN );
      }

      delablexpr( Equation );
      ScanSyfOut->ABL = Atom;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Out(i) = OR Locout(j) -> Out(i) = Locout(j)
*/
      Atom = ABL_CADR( Equation );
      ABL_CADR( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      ScanSyfOut->ABL = Atom;
    }
  }

  if ( SyfInfo->REG_OUT )
  {
    Index = 0;

    for ( ScanOutput  = FsmFigure->OUT;
          ScanOutput != (fsmout_list *)0;
          ScanOutput  = ScanOutput->NEXT )
    {
      if ( ScanOutput != ScanOut )
      {
        ScanSyfOut = FSM_SYF_OUT( ScanOutput );

        if ( ! ABL_ATOM( ScanSyfOut->ABL ) )
        {
          OutArray[ Index ].ABL = ScanSyfOut->ABL;
          ScanSyfOut->ABL = createablatom( OutArray[ Index ].NAME_OUT );
        }

        Index = Index + 1;
      }
    }
  }

  if ( SyfInfo->SCAN_PATH )
  {
    PrevScan = SyfInfo->REG_ARRAY[ SyfInfo->NUMBER_REG - 1 ].NAME_OUT;

    if ( SyfInfo->REG_OUT )
    {
      for ( Index = 0; Index < SyfInfo->NUMBER_OUT; Index++ )
      {
        if ( OutArray[ Index ].ABL == (ablexpr *)0 ) continue;
          
        ScanPath = optimablbinexpr( ABL_AND, 
                                     createablatom( PrevScan ),
                                     createablatom( SyfInfo->SCAN_TEST ) );

        Equation = optimablbinexpr( ABL_AND,
                                     OutArray[ Index ].ABL,
                                     optimablnotexpr( createablatom( SyfInfo->SCAN_TEST ) ) );

        OutArray[ Index ].ABL = optimablbinexpr( ABL_OR, Equation, ScanPath );

        PrevScan = OutArray[ Index ].NAME_OUT;
      }
    }

    ScanSyfOut = FSM_SYF_OUT( ScanOut );
    ScanSyfOut->ABL = createablatom( PrevScan );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    SyfSynthFreeOutAbl                       |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeOutAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  syfregout      *OutArray;
  fsmout_list    *ScanOutput;
  syfout         *ScanSyfOut;
  long            Index;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  for ( ScanOutput  = FsmFigure->OUT;
        ScanOutput != (fsmout_list *)0;
        ScanOutput  = ScanOutput->NEXT )
  {
    ScanSyfOut = FSM_SYF_OUT( ScanOutput );

    if ( ScanSyfOut->ABL != (ablexpr *)0 )
    {
      delablexpr( ScanSyfOut->ABL );
      ScanSyfOut->ABL = (ablexpr *)0;
    }
  }

  if ( SyfInfo->REG_OUT )
  {
    OutArray = SyfInfo->OUT_ARRAY;

    for ( Index = 0; Index < SyfInfo->NUMBER_OUT; Index++ )
    {
      if ( OutArray[ Index ].ABL != (ablexpr *)0 )
      {
        delablexpr( OutArray[ Index ].ABL );
        OutArray[ Index ].ABL = (ablexpr *)0;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfSynthReg2Abl                       |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthReg2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo       *SyfInfo;
  fsmstate_list *ScanState;
  fsmstate_list *StarState;
  fsmtrans_list *StarTrans;
  chain_list    *ScanChain;
  ablexpr       *Equation;
  ablexpr       *ScanPath;
  ablexpr       *Stack;
  ablexpr       *Atom;
  ablexpr       *AblReset;
  ablexpr       *AblSet;
  syfstate      *ScanSyfState;
  syfcode       *ScanCode;
  syfregstate   *RegArray;
  syfctrl       *CtrlArray;
  syfregstack   *StackArray;
  char          *PrevScan;
  long           Index;
  long           BitMask;
  long           ScanBit;
  char           Encode;

  SyfInfo    = FSM_SYF_INFO( FsmFigure );
  RegArray   = SyfInfo->REG_ARRAY;
  StackArray = SyfInfo->STACK_ARRAY;
  CtrlArray  = SyfInfo->CTRL_ARRAY;
  Encode     = SyfInfo->ENCODE;

  for ( Index = 0; Index < SyfInfo->NUMBER_REG; Index++ )
  {
    RegArray[ Index ].ABL       = createabloper( ABL_OR );
    RegArray[ Index ].ABL_SET   = createabloper( ABL_OR );
    RegArray[ Index ].ABL_RESET = createabloper( ABL_OR );
  }

/*
**  If Code[i] of E(j), Reg(i) <- Reg(i) OR E(j)
*/
  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = FSM_SYF_STATE( ScanState );
    ScanCode     = ScanSyfState->CODE;

    if ( Encode == SYF_ENCODE_ONE_HOT )
    {
      Atom = createablatom( ScanSyfState->NEXT_NAME );

      addablhexpr( RegArray[ ScanCode->VALUE ].ABL, Atom );
    }
    else
    {
      BitMask = 1;

      for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
      {
        if ( ScanCode->VALUE & BitMask )
        {
          Atom = createablatom( ScanSyfState->NEXT_NAME );

          addablhexpr( RegArray[ ScanBit ].ABL, Atom );
        }

        BitMask = BitMask << 1;
      }
    }
  }

  for ( Index = 0; Index < SyfInfo->NUMBER_REG; Index++ )
  {
    Equation = RegArray[ Index ].ABL;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Reg(i) = OR Null -> Reg(i) = 0
*/
      Atom = createablatomzero();
      delablexpr( Equation );
      RegArray[ Index ].ABL = Atom;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
/*
**  Reg(i) = OR E(j) -> Reg(i) = E(j)
*/
      Atom = ABL_CADR( Equation );
      ABL_CADR( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      RegArray[ Index ].ABL = Atom;
    }

    if ( SyfInfo->STACK )
    {
      Atom = createablatom( CtrlArray[ FSM_CTRL_POP ].NAME );

      Equation = createablatom( StackArray[ Index ].NAME_OUT );
      Stack = optimablbinexpr( ABL_AND, Equation, dupablexpr( Atom ) );

      Equation = optimablbinexpr( ABL_AND,
                                   RegArray[ Index ].ABL, 
                                   optimablnotexpr( Atom ) );

      RegArray[ Index ].ABL = optimablbinexpr( ABL_OR, Stack, Equation );
    }
  }
/*
** Compute the Set and Reset conditions
*/
  StarState = FsmFigure->STAR_STATE;

  if ( StarState != (fsmstate_list *)0 )
  {
    for ( ScanChain  = StarState->FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      StarTrans = (fsmtrans_list *)ScanChain->DATA;
      ScanState = StarTrans->TO;

      ScanSyfState = FSM_SYF_STATE( ScanState );
      ScanCode     = ScanSyfState->CODE;

      if ( Encode == SYF_ENCODE_ONE_HOT )
      {
        Atom = createablatom( ScanSyfState->NEXT_NAME );

        addablhexpr( RegArray[ ScanCode->VALUE ].ABL_SET, dupablexpr( StarTrans->ABL ) );

        for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
        {
          if ( ScanBit == ScanCode->VALUE ) continue;

          addablhexpr( RegArray[ ScanBit ].ABL_RESET, dupablexpr( StarTrans->ABL ) );
        }
      }
      else
      {
        BitMask = 1;

        for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
        {
          if ( ScanCode->VALUE & BitMask )
          {
            addablhexpr( RegArray[ ScanBit ].ABL_SET, dupablexpr( StarTrans->ABL ) );
          }
          else
          {
            addablhexpr( RegArray[ ScanBit ].ABL_RESET, dupablexpr( StarTrans->ABL ) );
          }

          BitMask = BitMask << 1;
        }
      }
    }
  }

/*
** Add Reset and Set conditions
*/
  for ( Index = 0; Index < SyfInfo->NUMBER_REG; Index++ )
  {
    Equation = RegArray[ Index ].ABL_SET;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
      freeablexpr( Equation );
      RegArray[ Index ].ABL_SET = (chain_list *)0;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
      Atom = ABL_CADR( Equation );
      ABL_CADR( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      RegArray[ Index ].ABL_SET = Atom;
    }

    Equation = RegArray[ Index ].ABL_RESET;

    if ( ABL_CDR( Equation ) == (ablexpr *)0 )
    {
      freeablexpr( Equation );
      RegArray[ Index ].ABL_RESET = (chain_list *)0;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
    {
      Atom = ABL_CADR( Equation );
      ABL_CADR( Equation ) = (ablexpr *)0;
      freeablexpr( Equation );
      RegArray[ Index ].ABL_RESET = Atom;
    }
  }

  for ( Index = 0; Index < SyfInfo->NUMBER_REG; Index++ )
  {
    AblSet   = RegArray[ Index ].ABL_SET;
    AblReset = RegArray[ Index ].ABL_RESET;

    RegArray[ Index ].ABL_SET   = (ablexpr *)0;
    RegArray[ Index ].ABL_RESET = (ablexpr *)0;

    Equation = createablatom( RegArray[ Index ].NAME_IN );

    if ( AblReset != (ablexpr *)0 )
    {
      AblReset = optimablnotexpr( AblReset );
      Equation = optimablbinexpr( ABL_AND, Equation, AblReset );
    }

    if ( AblSet != (ablexpr *)0 )
    {
      Equation = optimablbinexpr( ABL_OR, Equation, AblSet );
    }

    RegArray[ Index ].ABL_IN = Equation;
  }
/*
** Add a scan path
*/
  if ( SyfInfo->SCAN_PATH )
  {
    if ( SyfInfo->STACK )
    {
      PrevScan = SyfInfo->SCAN_STACK;
    }
    else
    {
      PrevScan = SyfInfo->SCAN_IN;
    }

    for ( Index = 0; Index < SyfInfo->NUMBER_REG; Index++ )
    {
      ScanPath = optimablbinexpr( ABL_AND, 
                                  createablatom( PrevScan ),
                                  createablatom( SyfInfo->SCAN_TEST ) );

      Equation = optimablbinexpr( ABL_AND,
                                  RegArray[ Index ].ABL_IN,
                                  optimablnotexpr( createablatom( SyfInfo->SCAN_TEST ) ) );

      RegArray[ Index ].ABL_IN = optimablbinexpr( ABL_OR, Equation, ScanPath );

      PrevScan = RegArray[ Index ].NAME_OUT;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   SyfSynthFreeRegAbl                        |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeRegAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo     *SyfInfo;
  syfregstate *RegArray;
  long         Index;

  SyfInfo  = FSM_SYF_INFO( FsmFigure );
  RegArray = SyfInfo->REG_ARRAY;

  for ( Index = 0; Index < SyfInfo->NUMBER_REG; Index++ )
  {
    if ( RegArray[ Index ].ABL != (ablexpr *)0 )
    {
      delablexpr( RegArray[ Index ].ABL );
      RegArray[ Index ].ABL = (ablexpr *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     SyfSynthCode2Abl                        |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthCode2Abl( FsmFigure, FlagDc )

  fsmfig_list *FsmFigure;
  int          FlagDc;
{
  syfinfo     *SyfInfo;
  syfcode     *CodeArray;
  syfregstate *RegArray;
  ablexpr  *Equation;
  ablexpr  *EquationOut;
  long         Index;
  long         BitMask;
  long         ScanBit;
  char         DecodeOut;
  char         Encode;

  SyfInfo   = FSM_SYF_INFO( FsmFigure );
  Encode    = SyfInfo->ENCODE;

  if ( ( Encode != SYF_ENCODE_ONE_HOT ) &&
       ( Encode != SYF_ENCODE_FRANCK  ) &&
       ( FlagDc                       ) )
  {
    SyfSynthCodeDc2Abl( FsmFigure ); return;
  }

  CodeArray = SyfInfo->CODE_ARRAY;
  RegArray  = SyfInfo->REG_ARRAY;
  DecodeOut = ( SyfInfo->REG_OUT ) && ( SyfInfo->STACK );

  for ( Index = 0; Index < SyfInfo->NUMBER_CODE; Index++ )
  {
    if ( Encode == SYF_ENCODE_ONE_HOT )
    {
      Equation = createablatom( RegArray[ Index ].NAME_OUT );

      if ( DecodeOut )
      {
        EquationOut = createablatom( RegArray[ Index ].NAME_IN );
      }
    }
    else
    if ( SyfInfo->NUMBER_BIT < 2 )
    {
      Equation = createablatom( RegArray[ 0 ].NAME_OUT );

      if ( DecodeOut )
      {
        EquationOut = createablatom( RegArray[ 0 ].NAME_IN );
      }

      if ( ! CodeArray[ Index ].VALUE )
      {
        Equation = optimablnotexpr( Equation );

        if ( DecodeOut )
        {
          EquationOut = optimablnotexpr( EquationOut );
        }
      }
    }
    else
    {
      Equation = createabloper( ABL_AND );
      BitMask  = 1;

      if ( DecodeOut )
      {
        EquationOut = createabloper( ABL_AND );
      }

      for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
      {
        if ( CodeArray[ Index ].VALUE & BitMask )
        {
          addablhexpr( Equation,  
                       createablatom( RegArray[ ScanBit ].NAME_OUT ) );

          if ( DecodeOut )
          {
            addablhexpr( EquationOut,
                         createablatom( RegArray[ ScanBit ].NAME_IN ) );
          }
        }
        else
        {
          addablhexpr( Equation,
                       optimablnotexpr( createablatom( RegArray[ ScanBit ].NAME_OUT ) ) );

          if ( DecodeOut )
          {
            addablhexpr( EquationOut,
                         optimablnotexpr( createablatom( RegArray[ ScanBit ].NAME_IN ) ) );

          }
        }

        BitMask = BitMask << 1;
      }
    }

    CodeArray[ Index ].ABL = Equation;

    if ( DecodeOut )
    {
      CodeArray[ Index ].ABL_OUT = EquationOut;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   SyfSynthFreeCodeAbl                       |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeCodeAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo *SyfInfo;
  syfcode *CodeArray;
  long     Index;

  SyfInfo   = FSM_SYF_INFO( FsmFigure );
  CodeArray = SyfInfo->CODE_ARRAY;

  for ( Index = 0; Index < SyfInfo->NUMBER_CODE; Index++ )
  {
    if ( CodeArray[ Index ].ABL != (ablexpr *)0 )
    {
      delablexpr( CodeArray[ Index ].ABL );
      CodeArray[ Index ].ABL = (ablexpr *)0;
    }

    if ( CodeArray[ Index ].ABL_OUT != (ablexpr *)0 )
    {
      delablexpr( CodeArray[ Index ].ABL_OUT );
      CodeArray[ Index ].ABL_OUT = (ablexpr *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthTrans2Abl                      |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthTrans2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo       *SyfInfo;
  fsmstate_list *ScanState;
  syfstate      *ScanSyfState;
  syfstate      *FromSyfState;
  ablexpr       *ScanChain;
  ablexpr       *Atom;
  fsmtrans_list *ScanTrans;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanChain    = ScanState->TO;
    ScanSyfState = FSM_SYF_STATE( ScanState );

    if ( ScanChain == (ablexpr *)0 )
    {
/*
**  No transition to E(i)
*/
      ScanSyfState->ABL_TRANS = createablatomzero();
    }
    else
    if ( ScanChain->NEXT == (ablexpr *)0 )
    {
/*
**  One transition to E(i)
*/
      ScanTrans    = (fsmtrans_list *)( ScanChain->DATA );

      if ( IsFsmStarTrans( ScanTrans ) )
      {
        ScanSyfState->ABL_TRANS = createablatomzero();
      }
      else
      {
        FromSyfState = FSM_SYF_STATE( ScanTrans->FROM );

        Atom = createablatom( FromSyfState->CURRENT_NAME );
      
        ScanSyfState->ABL_TRANS = 

           optimablbinexpr( ABL_AND, Atom, dupablexpr( ScanTrans->ABL ) );
      }
    }
    else
    {
/*
**  More than one transition to E(i)
*/
      ScanSyfState->ABL_TRANS = createabloper( ABL_OR );

      while ( ScanChain != (ablexpr *)0 )
      {
        ScanTrans = (fsmtrans_list *)( ScanChain->DATA );

        if ( IsFsmStarTrans( ScanTrans ) )
        {
          addablhexpr( ScanSyfState->ABL_TRANS, createablatomzero() );
        }
        else
        {
          FromSyfState = FSM_SYF_STATE( ScanTrans->FROM );

          Atom = createablatom( FromSyfState->CURRENT_NAME );
      
          addablhexpr( ScanSyfState->ABL_TRANS, 
                       optimablbinexpr( ABL_AND, 
                                         Atom, dupablexpr( ScanTrans->ABL ) ) );

        }

        ScanChain = ScanChain->NEXT;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthFreeTransAbl                   |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeTransAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list *ScanState;
  syfstate      *ScanSyfState;

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = FSM_SYF_STATE( ScanState );

    if ( ScanSyfState->ABL_TRANS != (ablexpr *)0 )
    {
      delablexpr( ScanSyfState->ABL_TRANS );
      ScanSyfState->ABL_TRANS = (ablexpr *)0;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfSynthFsm2Abl                       |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFsm2Abl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo *SyfInfo;
  
  SyfInfo = FSM_SYF_INFO( FsmFigure );

  SyfSynthTrans2Abl( FsmFigure );
  SyfSynthReturn2Abl( FsmFigure );
  SyfSynthOut2Abl( FsmFigure );
  SyfSynthCtrl2Abl( FsmFigure );
  SyfSynthStack2Abl( FsmFigure );
  SyfSynthReg2Abl( FsmFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthFreeFsmAbl                     |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFreeFsmAbl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  SyfSynthFreeTransAbl( FsmFigure );
  SyfSynthFreeReturnAbl( FsmFigure );
  SyfSynthFreeOutAbl( FsmFigure );
  SyfSynthFreeCtrlAbl( FsmFigure );
  SyfSynthFreeStackAbl( FsmFigure );
  SyfSynthFreeRegAbl( FsmFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfSynthFsmInit                        |
|                                                             |
\------------------------------------------------------------*/

void SyfSynthFsmInit( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo     *SyfInfo;
  char         Buffer[ 25 ];
  syfregstate *RegArray;
  long         RegMax;
  syfregout   *OutArray;
  long         OutMax;
  syfregstack *StackArray;
  long         StackMax;
  syfctrl     *CtrlArray;
  long         CtrlMax;
  long         Index;
  long         Stack;
  long         ScanBit;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  RegMax   = SyfInfo->NUMBER_BIT;
  RegArray = (syfregstate  *)autallocblock( sizeof( syfregstate ) * RegMax );

  for ( Index = 0; Index < RegMax; Index++ )
  {
    sprintf( Buffer, "%s %ld", SyfInfo->CURRENT_STATE, Index );
    RegArray[ Index ].NAME_OUT = namealloc( Buffer );
    sprintf( Buffer, "%s %ld", SyfInfo->NEXT_STATE, Index );
    RegArray[ Index ].NAME_IN = namealloc( Buffer );
    RegArray[ Index ].NAME_MASTER = RegArray[ Index ].NAME_OUT;
  }

  SyfInfo->REG_ARRAY  = RegArray;
  SyfInfo->NUMBER_REG = RegMax;

  if ( SyfInfo->REG_OUT )
  {
    if ( SyfInfo->SCAN_PATH )
    {
      OutMax = FsmFigure->NUMBER_OUT - 1;
    }
    else
    {
      OutMax = FsmFigure->NUMBER_OUT;
    }

    OutArray = (syfregout *)autallocblock( sizeof( syfregout ) * OutMax );

    for ( Index = 0; Index < OutMax; Index++ )
    {
      sprintf( Buffer, "regouts_%ld", Index );
      OutArray[ Index ].NAME_OUT = namealloc( Buffer );
      OutArray[ Index ].NAME_MASTER = OutArray[ Index ].NAME_OUT;
    }

    SyfInfo->OUT_ARRAY  = OutArray;
    SyfInfo->NUMBER_OUT = OutMax;
  }

  if ( SyfInfo->STACK )
  {
    CtrlMax    = FSM_MAX_CTRL;
    StackMax   = FsmFigure->STACK_SIZE * SyfInfo->NUMBER_REG;
    CtrlArray  = (syfctrl *)autallocblock( sizeof( syfctrl ) * CtrlMax );
    StackArray = (syfregstack *)autallocblock( sizeof( syfregstack ) * StackMax );

    for ( Index = 0; Index < CtrlMax; Index++ )
    {
      sprintf( Buffer, "ctrl_%s", FSM_CTRL_NAME[ Index ] );
      CtrlArray[ Index ].NAME = namealloc( Buffer );
    }

    SyfInfo->NUMBER_CTRL = CtrlMax;
    SyfInfo->CTRL_ARRAY  = CtrlArray;

    Index = 0;

    for ( Stack = 0; Stack < FsmFigure->STACK_SIZE; Stack++ )
    {
      for ( ScanBit = 0; ScanBit < SyfInfo->NUMBER_REG; ScanBit++ )
      {
        sprintf( Buffer, "stacks_%ld_%ld", Stack, ScanBit );
        StackArray[ Index ].NAME_OUT = namealloc( Buffer );
        StackArray[ Index ].NAME_MASTER = StackArray[ Index ].NAME_OUT;

        Index = Index + 1;
      }
    }

    SyfInfo->NUMBER_STACK = StackMax;
    SyfInfo->STACK_ARRAY  = StackArray;
  }
}
