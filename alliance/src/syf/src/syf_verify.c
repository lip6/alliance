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
| File    :                 syf_verify.c                      |
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
# include "syf_env.h"
# include "syf_bdd.h"
# include "syf_tools.h"
# include "syf_verify.h"
# include "syf_error.h"

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
|                      SyfFsmVerifyMooreExpr                  |
|                                                             |
\------------------------------------------------------------*/

int SyfFsmVerifyMooreExpr( FsmFigure, Equation )

  fsmfig_list *FsmFigure;
  chain_list  *Equation;
{
  char *Value;

  if ( Equation == (chain_list *)0 )
  {
    return( 0 );
  }

  if ( ABL_ATOM( Equation ) )
  {
    Value = ABL_ATOM_VALUE( Equation );

    if ( ( Value != ABL_ATOM_NAME_ZERO ) &&
         ( Value != ABL_ATOM_NAME_ONE  ) &&
         ( Value != ABL_ATOM_NAME_DC   ) )
    {
      if ( searchfsmin( FsmFigure, Value ) )
      {
        return( 1 );
      }
    }
  }
  else
  {
    while ( ( Equation = ABL_CDR( Equation ) ) != (ablexpr *)0 )
    {
      if ( SyfFsmVerifyMooreExpr( FsmFigure, ABL_CAR( Equation ) ) )
      {
        return( 1 );
      }
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfVerifyExclusivity                  |
|                                                             |
\------------------------------------------------------------*/

static void SyfVerifyExclusivity( ScanState, StarState )

  fsmstate_list *ScanState;
  fsmstate_list *StarState;
{
  chain_list    *ScanChain;
  chain_list    *ScanNext;
  fsmtrans_list *ScanTrans;
  fsmtrans_list *NextTrans;
  fsmstack_list *ScanStack;
  fsmstack_list *NextStack;
  bddnode       *TransCond;
  bddnode       *NextCond;
  bddnode       *ProductCond;
  char           Counter;
  char           NextCounter;

  Counter   = 0;
  ScanChain = ScanState->FROM;

  while ( Counter < 4 )
  {
    while ( ScanChain != (chain_list *)0 )
    {
      if ( Counter & 0x1 )
      {
        ScanStack = (fsmstack_list *)ScanChain->DATA;
        TransCond = ScanStack->BDD;

        if ( ScanStack->CTRL != FSM_CTRL_POP )
        {
          ScanChain = ScanChain->NEXT; continue;
        }
      }
      else
      {
        ScanTrans = (fsmtrans_list *)ScanChain->DATA;
        TransCond = ScanTrans->BDD;
      }

      NextCounter = Counter;
      ScanNext    = ScanChain->NEXT;

      while ( NextCounter < 4 )
      {
        while ( ScanNext != (chain_list *)0 )
        {
          if ( NextCounter & 0x1 )
          {
            NextStack = (fsmstack_list *)ScanNext->DATA;
            NextCond  = NextStack->BDD;

            if ( NextStack->CTRL != FSM_CTRL_POP )
            {
              ScanNext = ScanNext->NEXT; continue;
            }
          }
          else
          {
            NextTrans = (fsmtrans_list *)ScanNext->DATA;
            NextCond  = NextTrans->BDD;
          }

          ProductCond = applybddnode( (bddsystem *)0, ABL_AND, NextCond, TransCond );

          if ( ProductCond != BddLocalSystem->ZERO )
          {
            SyfWarning( SYF_WARNING_INTER_TRANS, ScanState->NAME, NULL );
          }

          ScanNext = ScanNext->NEXT;
        }

        NextCounter = NextCounter + 1;

        if ( ( NextCounter > 1                 ) &&
             ( StarState == (fsmstate_list *)0 ) )  break;

        if ( NextCounter == 1 ) ScanNext = ScanState->STACK;
        else
        if ( NextCounter == 2 ) ScanNext = StarState->FROM;
        else                    ScanNext = StarState->STACK;
      }

      ScanChain = ScanChain->NEXT;
    }

    Counter = Counter + 1;

    
    if ( ( Counter > 1                     ) &&
         ( StarState == (fsmstate_list *)0 ) )  break;

    if ( Counter == 1 ) ScanChain = ScanState->STACK;
    else
    if ( Counter == 2 ) ScanChain = StarState->FROM;
    else                ScanChain = StarState->STACK;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfVerifyCompletly                    |
|                                                             |
\------------------------------------------------------------*/

static void SyfVerifyCompletly( ScanState, SumStar )

  fsmstate_list *ScanState;
  bddnode       *SumStar;
{
  chain_list    *ScanChain;
  fsmtrans_list *ScanTrans;
  fsmstack_list *ScanStack;
  bddnode       *TransCond;
  bddnode       *SumCond;
  char           Stack;

  SumCond   = SumStar;
  Stack     = 0;
  ScanChain = ScanState->FROM;

  while ( Stack < 2 )
  {
    while ( ScanChain != (chain_list *)0 )
    {
      if ( ! Stack )
      {
        ScanTrans = (fsmtrans_list *)ScanChain->DATA;
        TransCond = ScanTrans->BDD;
      }
      else
      {
        ScanStack = (fsmstack_list *)ScanChain->DATA;
        TransCond = ScanStack->BDD;

        if ( ScanStack->CTRL != FSM_CTRL_POP )
        {
          ScanChain = ScanChain->NEXT;

          continue;
        }
      }

      SumCond   = applybddnode( (bddsystem *)0,  ABL_OR, SumCond, TransCond );
      ScanChain = ScanChain->NEXT;
    }

    ScanChain = ScanState->STACK;
    Stack     = Stack + 1;
  }

  if ( SumCond != BddLocalSystem->ONE )
  {
    SyfWarning( SYF_WARNING_SUM_TRANS, ScanState->NAME, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfVerifyTransistion                  |
|                                                             |
\------------------------------------------------------------*/

static void SyfVerifyTransition( FsmFigure )

  fsmfig_list *FsmFigure;
{
  bddsystem     *BddSystem;
  bddcircuit    *BddCircuit;
  fsmstate_list *ScanState;
  fsmstate_list *StarState;
  chain_list    *ScanChain;
  fsmtrans_list *StarTrans;
  fsmstack_list *StarStack;
  bddnode       *TransCond;
  bddnode       *SumStar;
  char           Stack;

  if ( FsmFigure->CIRCUIT == (bddcircuit *)0 )
  {
    BddSystem  = createbddsystem( SYF_BDD_VAR_NODE, SYF_BDD_OPER_NODE, 1000, SYF_BDD_MAX_NODE );
    reorderbddsystemdynamic( BddSystem, SYF_BDD_REORDER_FUNC,
                             SYF_BDD_REORDER_NODE, SYF_BDD_REORDER_RATIO );
    BddCircuit = SyfFsmMakeBddCircuit( BddSystem, FsmFigure );
    reorderbddsystemsimple( BddSystem );
  }
  else
  {
    BddCircuit = FsmFigure->CIRCUIT;
    BddSystem  = BddCircuit->BDD_SYSTEM;

    setbddlocalcircuit( BddCircuit );
    setbddlocalsystem( BddSystem );
  }
/*
** Compute Sum Star Transitions
*/
  SumStar   = BddLocalSystem->ZERO;
  StarState = FsmFigure->STAR_STATE;

  if ( StarState != (fsmstate_list *)0 )
  {
    Stack     = 0;
    ScanChain = StarState->FROM;
  
    while ( Stack < 2 )
    {
      while ( ScanChain != (chain_list *)0 )
      {
        if ( ! Stack )
        {
          StarTrans = (fsmtrans_list *)ScanChain->DATA;
          TransCond = StarTrans->BDD;
        }
        else
        {
          StarStack = (fsmstack_list *)ScanChain->DATA;
          TransCond = StarStack->BDD;
  
          if ( StarStack->CTRL != FSM_CTRL_POP )
          {
            ScanChain = ScanChain->NEXT; continue;
          }
        }
  
        decbddrefext( SumStar );
        SumStar   = applybddnode( (bddsystem *)0, ABL_OR, SumStar, TransCond );
        ScanChain = ScanChain->NEXT;
      }
  
      ScanChain = StarState->STACK;
      Stack     = Stack + 1;
    }
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    SyfVerifyCompletly( ScanState, SumStar );
    SyfVerifyExclusivity( ScanState, StarState );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        SyfFsmVerify                         |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmVerify( FsmFigure, FlagVerify )

  fsmfig_list *FsmFigure;
  int          FlagVerify;
{
  syfinfo        *SyfInfo;
  fsmstate_list  *ScanState;
  fsmstack_list  *ScanStack;
  fsmlocout_list *ScanLocout;
  chain_list     *ScanChain;
  char            TypeMoore;
  int             NumberPush;
  int             NumberPop;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  if ( SyfInfo->STACK )
  {
    NumberPush = 0;
    NumberPop  = 0;

    for ( ScanStack  = FsmFigure->STACK;
          ScanStack != (fsmstack_list *)0;
          ScanStack  = ScanStack->NEXT )
    {
      if ( ScanStack->CTRL == FSM_CTRL_PUSH ) NumberPush++;
      else
      if ( ScanStack->CTRL == FSM_CTRL_POP  ) NumberPop++;
    }

    if ( ( NumberPush == 0 ) &&
         ( NumberPop  == 0 ) )
    {
      FsmFigure->STACK_SIZE = 0;

      while ( FsmFigure->STACK != (fsmstack_list *)0 )
      {
        delfsmstack( FsmFigure, FsmFigure->STACK );
      }

      SyfWarning( SYF_WARNING_REMOVE_UNUSED_STACK, FsmFigure->NAME, NULL );
    }
    else
    if ( NumberPop == 0 )
    {
      SyfError( SYF_ERROR_BAD_STACK_NO_POP, FsmFigure->NAME );
    }
    else
    if ( NumberPush == 0 )
    {
      SyfError( SYF_ERROR_BAD_STACK_NO_PUSH, FsmFigure->NAME );
    }
  }

  TypeMoore = 1;

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    if ( IsFsmStarState( ScanState ) ) continue;

    if ( ScanState->TO == (chain_list *)0 )
    {
/*
**  No transition to E(i)
*/
      if ( SyfInfo->STACK )
      {
        for ( ScanStack  = FsmFigure->STACK;
              ScanStack != (fsmstack_list *)0;
              ScanStack  = ScanStack->NEXT )
        {
          if ( ( ScanStack->CTRL   == FSM_CTRL_PUSH ) &&
               ( ScanStack->RETURN == ScanState     ) )
          {
            break;
          }
        }
/*
**  No stack transition to E(i)
*/
        if ( ScanStack == (fsmstack_list *)0 )
        {
          SyfWarning( SYF_WARNING_NO_TRANS_TO, ScanState->NAME, NULL );
        }
      }
      else
      {
        SyfWarning( SYF_WARNING_NO_TRANS_TO, ScanState->NAME, NULL );
      }
    }

    if ( ScanState->FROM == (chain_list *)0 )
    {
/*
**  No transition from E(i)
*/
      if ( SyfInfo->STACK )
      {
        for ( ScanChain  = ScanState->STACK;
              ScanChain != (chain_list *)0;
              ScanChain  = ScanChain->NEXT )
        {
          ScanStack = (fsmstack_list *)(ScanChain->DATA);

          if ( ScanStack->CTRL == FSM_CTRL_POP )
          {
            break;
          }
        }
/*
**  No stack transition from E(i)
*/
        if ( ScanChain == (chain_list *)0 )
        {
          SyfWarning( SYF_WARNING_NO_TRANS_FROM, ScanState->NAME, NULL );
        }
      }
      else
      {
        SyfWarning( SYF_WARNING_NO_TRANS_FROM, ScanState->NAME, NULL );
      }
    }

    if ( TypeMoore )
    {
      for ( ScanLocout  = ScanState->LOCOUT;
            ScanLocout != (fsmlocout_list *)0;
            ScanLocout  = ScanLocout->NEXT )
      {
        if ( SyfFsmVerifyMooreExpr( FsmFigure, ScanLocout->ABL ) )
        {
          TypeMoore = 0;
        }
      }
    }
  }

  if ( TypeMoore )
  {
    SyfInfo->FSM_TYPE = SYF_FSM_TYPE_MOORE;
  }
  else
  {
    SyfInfo->FSM_TYPE = SYF_FSM_TYPE_MEALY;
  }

  if ( FlagVerify )
  {
    SyfVerifyTransition( FsmFigure );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     SyfFsmOutputPolarity                    |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmOutputPolarity( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmstate_list  *StarState;
  chain_list     *ScanChain;
  fsmtrans_list  *ScanTrans;
  fsmlocout_list *ScanLocout;
  fsmout_list    *ScanOut;
  syfout         *SyfOut;
  bddsystem      *BddSystem;
  bddcircuit     *BddCircuit;
  bddnode        *BddOne;
  bddnode        *BddZero;
  bddnode        *BddNode;

  if ( FsmFigure->CIRCUIT == (bddcircuit *)0 )
  {
    BddSystem = createbddsystem( SYF_BDD_VAR_NODE, SYF_BDD_OPER_NODE, 1000, SYF_BDD_MAX_NODE );
    reorderbddsystemdynamic( BddSystem, SYF_BDD_REORDER_FUNC,
                             SYF_BDD_REORDER_NODE, SYF_BDD_REORDER_RATIO );
    BddCircuit = SyfFsmMakeBddCircuit( BddSystem, FsmFigure );
    reorderbddsystemsimple( BddSystem );
  }
  else
  {
    BddCircuit = FsmFigure->CIRCUIT;
    BddSystem  = BddCircuit->BDD_SYSTEM;

    setbddlocalcircuit( BddCircuit );
    setbddlocalsystem( BddSystem );
  }

  StarState = FsmFigure->STAR_STATE;
  BddOne    = BddSystem->ONE;
  BddZero   = BddSystem->ZERO;

  if ( StarState != (fsmstate_list *)0 )
  {
    BddOne = BddSystem->ZERO;

    for ( ScanChain  = StarState->FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanTrans = (fsmtrans_list *)ScanChain->DATA;
      BddOne    = applybddnode( (bddsystem *)0, ABL_OR, 
                                decbddrefext( BddOne ), ScanTrans->BDD );
    }

    if ( StarState->FROM == (chain_list *)0 )
    {
      StarState = (fsmstate_list *)0;
    }

    BddOne = applybddnodenot( (bddsystem *)0, decbddrefext( BddOne ) );
  }

  if ( StarState != (fsmstate_list *)0 )
  {
    ScanState = StarState;
    ScanState->NEXT = FsmFigure->STATE;
  }
  else
  {
    ScanState = FsmFigure->STATE;
  }

  while ( ScanState != (fsmstate_list *)0 )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      if ( ScanLocout->ABL == (ablexpr *)0 ) continue;

      if ( ScanLocout->BDD == (bddnode *)0 )
      {
        ScanLocout->BDD = addbddcircuitabl( BddCircuit, ScanLocout->ABL );
      }

      ScanOut = ScanLocout->OUT;
      SyfOut  = FSM_SYF_OUT( ScanOut );

      SyfOut->NUMBER_ASSIGN++;

      if ( ScanState != StarState )
      {
        if ( ScanLocout->BDD == BddOne  ) SyfOut->NUMBER_ONE++;
        else
        if ( ScanLocout->BDD == BddZero ) SyfOut->NUMBER_ZERO++;
      }
    }

    ScanState = ScanState->NEXT;
  }

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    SyfOut = FSM_SYF_OUT( ScanOut );

    if ( ( SyfOut->NUMBER_ASSIGN < FsmFigure->NUMBER_STATE ) &&
         ( SyfOut->NUMBER_ASSIGN != 0                      ) )
    {
      SyfWarning( SYF_WARNING_OUTPUT_NOT_ASSIGNED, ScanOut->NAME, SyfOut->NUMBER_ASSIGN );
    }

    if ( SyfOut->NUMBER_ONE > SyfOut->NUMBER_ZERO )
    {
      SyfOut->POLARITY = 1;
      SyfWarning( SYF_WARNING_MODIFY_POLARITY, ScanOut->NAME, NULL );
    }
  }

  if ( StarState != (fsmstate_list *)0 ) ScanState = StarState;
  else                                   ScanState = FsmFigure->STATE;

  while ( ScanState != (fsmstate_list *)0 )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      ScanOut = ScanLocout->OUT;
      SyfOut  = FSM_SYF_OUT( ScanOut );

      if ( SyfOut->POLARITY )
      {
        if ( ScanState == StarState )
        {
          BddNode = ScanLocout->BDD;
          BddNode = applybddnode( (bddsystem *)0, ABL_NOR, BddNode, BddOne );
        }
        else
        {
          BddNode = ScanLocout->BDD;
          BddNode = applybddnodenot( (bddsystem *)0, BddNode );
          BddNode = applybddnode( (bddsystem *)0, ABL_AND, decbddrefext( BddNode ), BddOne );
        }

        freeablexpr( ScanLocout->ABL );
        ScanLocout->ABL = convertbddcircuitabl( (bddcircuit *)0, BddNode );

        decbddrefext( ScanLocout->BDD );
        ScanLocout->BDD = BddNode;

        freeablexpr( ScanLocout->ABL_DC );
        ScanLocout->ABL_DC = (ablexpr *)0;
      }
    }

    ScanState = ScanState->NEXT;
  }

  decbddrefext( BddOne  );
  decbddrefext( BddZero );

  if ( StarState != (fsmstate_list *)0 )
  {
    StarState->NEXT = (fsmstate_list *)0;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        SyfFsmSimplify                       |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmSimplify( FsmFigure, FlagSimplify )

  fsmfig_list *FsmFigure;
  int          FlagSimplify;
{
  bddsystem      *BddSystem;
  bddcircuit     *BddCircuit;
  syfinfo        *SyfInfo;
  fsmstate_list  *ScanState;
  fsmstack_list  *ScanStack;
  fsmstack_list  *DelStack;
  fsmlocout_list *ScanLocout;
  fsmlocout_list *DelLocout;
  fsmtrans_list  *ScanTrans;
  fsmtrans_list  *DelTrans;

  if ( FlagSimplify )
  {
    if ( FsmFigure->CIRCUIT == (bddcircuit *)0 )
    {
      BddSystem  = createbddsystem( SYF_BDD_VAR_NODE, SYF_BDD_OPER_NODE, 1000, SYF_BDD_MAX_NODE );
      reorderbddsystemdynamic( BddSystem, SYF_BDD_REORDER_FUNC,
                               SYF_BDD_REORDER_NODE, SYF_BDD_REORDER_RATIO );
      BddCircuit = SyfFsmMakeBddCircuit( BddSystem, FsmFigure );
      reorderbddsystemsimple( BddSystem );
    }

    convertfsmbddnodeabl( FsmFigure );
  }

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  if ( SyfInfo->STACK )
  {
    ScanStack = FsmFigure->STACK;

    while ( ScanStack != (fsmstack_list *)0 )
    {
      ScanStack->ABL = simpablexpr( ScanStack->ABL );

      if ( ( ABL_ATOM( ScanStack->ABL )                             ) &&
           ( ABL_ATOM_VALUE( ScanStack->ABL ) == ABL_ATOM_NAME_ZERO ) )
      {
        DelStack  = ScanStack;
        ScanStack = ScanStack->NEXT;

        SyfWarning( SYF_WARNING_REMOVE_STACK, DelStack->CURRENT->NAME, NULL );

        delfsmstack( FsmFigure, DelStack );
      }
      else
      {
        ScanStack = ScanStack->NEXT;
      }
    }
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanLocout = ScanState->LOCOUT;

    while ( ScanLocout != (fsmlocout_list *)0 )
    {
      if ( ScanLocout->ABL != (chain_list *)0 )
      {
        ScanLocout->ABL = simpablexpr( ScanLocout->ABL );

        if ( ( ABL_ATOM( ScanLocout->ABL )                             ) &&
             ( ABL_ATOM_VALUE( ScanLocout->ABL ) == ABL_ATOM_NAME_ZERO ) )
        {
          freeablexpr( ScanLocout->ABL );
          ScanLocout->ABL = (chain_list *)0;
        }
      }

      if ( ScanLocout->ABL_DC != (chain_list *)0 )
      {
        ScanLocout->ABL_DC = simpablexpr( ScanLocout->ABL_DC );

        if ( ( ABL_ATOM( ScanLocout->ABL_DC )                             ) &&
             ( ABL_ATOM_VALUE( ScanLocout->ABL_DC ) == ABL_ATOM_NAME_ZERO ) )
        {
          freeablexpr( ScanLocout->ABL_DC );
          ScanLocout->ABL_DC = (chain_list *)0;
        }
      }

      if ( ( ScanLocout->ABL    == (chain_list *)0 ) &&
           ( ScanLocout->ABL_DC == (chain_list *)0 ) )
      {
        DelLocout  = ScanLocout;
        ScanLocout = ScanLocout->NEXT;

        delfsmlocout( ScanState, DelLocout );
      }
      else
      {
        ScanLocout = ScanLocout->NEXT;
      }
    }
  }

  ScanTrans  = FsmFigure->TRANS;

  while ( ScanTrans != (fsmtrans_list *)0 )
  {
    ScanTrans->ABL = simpablexpr( ScanTrans->ABL );

    if ( ( ABL_ATOM( ScanTrans->ABL )                             ) &&
         ( ABL_ATOM_VALUE( ScanTrans->ABL ) == ABL_ATOM_NAME_ZERO ) )
    {
      DelTrans  = ScanTrans;
      ScanTrans = ScanTrans->NEXT;

      SyfWarning( SYF_WARNING_REMOVE_TRANS,
                  DelTrans->FROM->NAME, DelTrans->TO->NAME );

      delfsmtrans( FsmFigure, DelTrans );
    }
    else
    {
      ScanTrans = ScanTrans->NEXT;
    }
  }

  if ( FsmFigure->CLOCK_ABL != (chain_list *)0 )
  {
    FsmFigure->CLOCK_ABL = simpablexpr( FsmFigure->CLOCK_ABL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        SyfFsmTreatReset                     |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmTreatReset( FsmFigure )

  fsmfig_list *FsmFigure;
{
  bddsystem      *BddSystem;
  bddcircuit     *BddCircuit;
  bddassoc       *BddAssoc;
  bddnode        *SumStar;
  bddnode        *TransCond;
  bddnode        *BddNode;
  bddvar          BddVar;
  fsmstate_list  *StarState;
  fsmtrans_list  *ScanTrans;
  chain_list     *ScanChain;
  chain_list     *Support;

  StarState = FsmFigure->STAR_STATE;

  if ( ( StarState       == (fsmstate_list *)0 ) ||
       ( StarState->FROM == (chain_list    *)0 ) ) return;

  if ( FsmFigure->CIRCUIT == (bddcircuit *)0 )
  {
    BddSystem  = createbddsystem( SYF_BDD_VAR_NODE, SYF_BDD_OPER_NODE, 1000, SYF_BDD_MAX_NODE );
    reorderbddsystemdynamic( BddSystem, SYF_BDD_REORDER_FUNC,
                             SYF_BDD_REORDER_NODE, SYF_BDD_REORDER_RATIO );
    BddCircuit = SyfFsmMakeBddCircuit( BddSystem, FsmFigure );
    reorderbddsystemsimple( BddSystem );
  }

/*
** Compute Sum Star Transitions
*/
  SumStar = BddLocalSystem->ZERO;

  for ( ScanChain  = StarState->FROM;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanTrans = (fsmtrans_list *)ScanChain->DATA;
    TransCond = ScanTrans->BDD;
  
    decbddrefext( SumStar );
    SumStar   = applybddnode( (bddsystem *)0, ABL_OR, SumStar, TransCond );
  }

  decbddrefext( SumStar );
  SumStar = applybddnodenot( (bddsystem *)0, SumStar );

  BddAssoc = addbddassoc( (bddsystem *)0 );
  Support  = getbddnodesupportchain( (bddsystem *)0, SumStar );
    
  for ( ScanChain  = Support;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    BddNode = (bddnode *)ScanChain->DATA;
    BddVar  = getbddvarbyindex( (bddsystem *)0, BddNode->INDEX );
    addbddnodeassoc( (bddsystem *)0, BddAssoc, BddVar, BddNode );
  }

  freechain( Support);

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    if ( IsFsmStarTrans( ScanTrans ) ) continue;

    TransCond = ScanTrans->BDD;
    TransCond = existbddnodeassoc( (bddsystem *)0, TransCond, BddAssoc );
/*
** Verify the correctness of the solution
*/
    BddNode = applybddnode( (bddsystem *)0, ABL_AND, TransCond, SumStar );
    decbddrefext( BddNode );

    if ( BddNode != ScanTrans->BDD )
    {
      decbddrefext( TransCond );
      TransCond = ScanTrans->BDD;
      TransCond = cofactorbddnode( (bddsystem *)0, TransCond, SumStar );
    }

    decbddrefext( ScanTrans->BDD );
    ScanTrans->BDD = TransCond;
  }

  delbddassoc( (bddsystem *)0, BddAssoc );

  convertfsmbddnodeabl( FsmFigure );

  if ( FsmFigure->CIRCUIT != (bddcircuit *)0 )
  {
    BddCircuit = FsmFigure->CIRCUIT;
    BddSystem  = BddCircuit->BDD_SYSTEM;

    destroybddsystem( BddSystem );
    destroybddcircuit( BddCircuit );

    FsmFigure->CIRCUIT = (bddcircuit *)0;
  }
}
