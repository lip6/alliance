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
| Tool    :                     FSP                           |
|                                                             |
| File    :                   fsp_proof.c                     |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
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
# include "fsp_debug.h"
# include "fsp_proof.h"

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

  static fspstate_list *FspHeadState = (fspstate_list *)0;
  static int            FspFlagDebug = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         FspAddState                         |
|                                                             |
\------------------------------------------------------------*/

fspstate_list *FspAddState( State1, State2 )

  fsmstate_list *State1;
  fsmstate_list *State2;
{
  fspstate_list *FspState;

  FspState = (fspstate_list *)autallocheap( sizeof( fspstate_list ) );

  FspState->STATE1 = State1;
  FspState->STATE2 = State2;

  FspState->NEXT = FspHeadState;
  FspHeadState   = FspState;

  return( FspState );
}

/*------------------------------------------------------------\
|                                                             |
|                         FspDelState                         |
|                                                             |
\------------------------------------------------------------*/

void FspDelState( FspState )

  fspstate_list *FspState;
{
  fspstate_list **FspPrevious;
  fspstate_list  *FspScan;

  FspPrevious = &FspHeadState;

  for ( FspScan  = FspHeadState;
        FspScan != (fspstate_list *)0;
        FspScan  = FspScan->NEXT )
  {
    if ( FspScan == FspState )
    {
      *FspPrevious = FspState->NEXT;
      autfreeheap( FspState, sizeof( fspstate_list ) );

      break;
    }

    FspPrevious = &FspScan->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         FspFreeState                        |
|                                                             |
\------------------------------------------------------------*/

void FspFreeState()
{
  while ( FspHeadState != (fspstate_list *)0 )
  {
    FspDelState( FspHeadState );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       FspExplodeStarState                   |
|                                                             |
\------------------------------------------------------------*/

void FspExplodeStarState( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmstate_list  *StarState;
  chain_list     *StarChain;
  fsmtrans_list  *StarTrans;
  fsmlocout_list *StarLocout;

  StarState = FsmFigure->STAR_STATE;

  if ( StarState != (fsmstate_list *)0 )
  {
    for ( StarChain  = StarState->FROM;
          StarChain != (chain_list *)0;
          StarChain  = StarChain->NEXT )
    {
      StarTrans = (fsmtrans_list *)StarChain->DATA;

      for ( ScanState  = FsmFigure->STATE;
            ScanState != (fsmstate_list *)0;
            ScanState  = ScanState->NEXT )
      {
        addfsmtrans( FsmFigure, ScanState, StarTrans->TO,
                       StarTrans->ABL );
      }
    }

    for ( StarLocout  = StarState->LOCOUT;
          StarLocout != (fsmlocout_list *)0;
          StarLocout  = StarLocout->NEXT )
    {
      for ( ScanState  = FsmFigure->STATE;
            ScanState != (fsmstate_list *)0;
            ScanState  = ScanState->NEXT )
      {
        addfsmlocout( ScanState, StarLocout->OUT,
                        StarLocout->ABL, StarLocout->ABL_DC );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         FspProduct                          |
|                                                             |
\------------------------------------------------------------*/

int FspProduct( FsmFigure1, FsmFigure2 )

  fsmfig_list *FsmFigure1;
  fsmfig_list *FsmFigure2;
{
  auth2table    *HashTable;
  auth2elem     *Element;
  fsmstate_list *State1;
  fsmstate_list *State2;
  fsmstate_list *StateSucc1;
  fsmstate_list *StateSucc2;
  fsmtrans_list *ScanTrans1;
  fsmtrans_list *ScanTrans2;
  chain_list    *ScanChain1;
  chain_list    *ScanChain2;
  bddnode       *StateCond1;
  bddnode       *StateCond2;
  bddnode       *BddNode;
  long           MaxState;
  int            Error;

  if ( FsmFigure1->NUMBER_STATE > FsmFigure2->NUMBER_STATE )
  {
    MaxState = FsmFigure1->NUMBER_STATE;
  }
  else
  {
    MaxState = FsmFigure2->NUMBER_STATE;
  }
  
  HashTable    = createauth2table( MaxState << 1 );
  FspHeadState = (fspstate_list *)0;
  Error        = 0;

  FspAddState( FsmFigure1->FIRST_STATE,  FsmFigure2->FIRST_STATE );
  State1 = FspHeadState->STATE1;
  State2 = FspHeadState->STATE2;

  if ( State1->USER != State2->USER )
  {
    Error = 1;

    FspPrintf( stdout, "Error output conflicts between state '%s' and state '%s'\n",
               State1->NAME, State2->NAME );

    if ( FspFlagDebug )
    {
      addbddcircuitout( (bddcircuit *)0, "conflict1", State1->USER );
      addbddcircuitout( (bddcircuit *)0, "conflict2", State2->USER );

      testbddcircuit( (bddcircuit *)0 );
    }
  }

  Element = addauth2elem( HashTable, (char *)State1, (char *)State2, 0 );

  while ( ( FspHeadState != (fspstate_list *)0 ) &&
          ( Error        == 0                  ) )
  {
    State1 = FspHeadState->STATE1;
    State2 = FspHeadState->STATE2;

    if ( ( State1->TO == (chain_list *)0 ) &&
         ( State2->TO != (chain_list *)0 ) )
    {
      Error = 1;
    }

    FspDelState( FspHeadState );

    for ( ScanChain1  = State1->FROM;
          ScanChain1 != (chain_list *)0;
          ScanChain1  = ScanChain1->NEXT )
    {
      ScanTrans1 = (fsmtrans_list *)ScanChain1->DATA; 
      StateSucc1 = ScanTrans1->TO;
      StateCond1 = ScanTrans1->BDD;

      for ( ScanChain2  = State2->FROM;
            ScanChain2 != (chain_list *)0;
            ScanChain2  = ScanChain2->NEXT )
      {
        ScanTrans2 = (fsmtrans_list *)ScanChain2->DATA;
        StateSucc2 = ScanTrans2->TO;
        StateCond2 = ScanTrans2->BDD;

        BddNode = applybddnode( (bddsystem *)0, ABL_AND, StateCond1, StateCond2 );

        if ( BddNode != BddLocalSystem->ZERO )
        {
          Element = searchauth2elem( HashTable, (char *)StateSucc1, (char *)StateSucc2 );

          if ( Element == (auth2elem *)0 )
          {
            if ( StateSucc1->USER != StateSucc2->USER )
            {
              FspPrintf( stdout, "Output conflicts between \"%s\" and \"%s\"\n", 
                         StateSucc1->NAME, StateSucc2->NAME );

              if ( FspFlagDebug )
              {
                addbddcircuitout( (bddcircuit *)0, "conflict1", StateSucc1->USER );
                addbddcircuitout( (bddcircuit *)0, "conflict2", StateSucc2->USER );

                testbddcircuit( (bddcircuit *)0 );
              }

              Error = 1;
            }

            FspAddState( StateSucc1, StateSucc2 );
            Element = addauth2elem( HashTable, (char *)StateSucc1, (char *)StateSucc2, 0 );
          }
        }
      }
    }
  }

  destroyauth2table( HashTable );
  FspFreeState();

  return( Error );
}

/*------------------------------------------------------------\
|                                                             |
|                       FspVerifyInputOutput                  |
|                                                             |
\------------------------------------------------------------*/

int FspVerifyInputOutput( FsmFigure1, FsmFigure2 )

  fsmfig_list *FsmFigure1;
  fsmfig_list *FsmFigure2;
{
  fsmin_list  *ScanIn;
  fsmout_list *ScanOut;
  authtable   *HashTable;

  if ( ( FsmFigure1->NUMBER_IN  != FsmFigure2->NUMBER_IN  ) ||
       ( FsmFigure2->NUMBER_OUT != FsmFigure2->NUMBER_OUT ) )
  {
    return( 0 );
  }

  HashTable = createauthtable( FsmFigure1->NUMBER_IN << 1 );

  for ( ScanIn  = FsmFigure1->IN;
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    addauthelem( HashTable, ScanIn->NAME, 0 );
  }

  for ( ScanIn = FsmFigure2->IN;
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    addauthelem( HashTable, ScanIn->NAME, 0 );
  }

  if ( ( HashTable->NUMBER_ELEM == FsmFigure1->NUMBER_IN ) &&
       ( HashTable->NUMBER_ELEM == FsmFigure2->NUMBER_IN ) )
  {
    destroyauthtable( HashTable );
    HashTable = createauthtable( FsmFigure1->NUMBER_OUT << 1 );

    for ( ScanOut  = FsmFigure1->OUT;
          ScanOut != (fsmout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      addauthelem( HashTable, ScanOut->NAME, 0 );
    }

    for ( ScanOut  = FsmFigure2->OUT;
          ScanOut != (fsmout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      addauthelem( HashTable, ScanOut->NAME, 0 );
    }

    if ( ( HashTable->NUMBER_ELEM == FsmFigure1->NUMBER_OUT ) &&
         ( HashTable->NUMBER_ELEM == FsmFigure2->NUMBER_OUT ) )
    {
      destroyauthtable( HashTable );

      return( 1 );
    }
  }

  destroyauthtable( HashTable );
  return( 0 );
}
 
/*------------------------------------------------------------\
|                                                             |
|                       FspMakeBddNode                        |
|                                                             |
\------------------------------------------------------------*/

void FspMakeBddNode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmlocout_list *ScanLocout;
  fsmtrans_list  *ScanTrans;
  bddnode        *BddNode;
  bddnode        *BddNodeOut;
  bddnode        *BddNodeLocOn;
  bddnode        *BddNodeLocOff;
  bddnode        *BddNodeLocDc;
  bddnode        *BddNodeLocout;

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    ScanTrans->BDD = addbddcircuitabl( (bddcircuit *)0, ScanTrans->ABL );
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    BddNode = BddLocalSystem->ONE;

    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      BddNodeOut = searchbddcircuitin( (bddcircuit *)0, ScanLocout->OUT->NAME );

      if ( ScanLocout->ABL != (chain_list *)0 )
      {
        BddNodeLocOn = addbddcircuitabl( (bddcircuit *)0, ScanLocout->ABL );
      }
      else
      {
        BddNodeLocOn = BddLocalSystem->ZERO;
      }

      if ( ScanLocout->ABL_DC != (chain_list *)0 )
      {
        BddNodeLocDc  = addbddcircuitabl( (bddcircuit *)0, ScanLocout->ABL_DC );
        BddNodeLocOff = applybddnode( (bddsystem *)0, ABL_OR, 
                                      BddNodeLocDc  , BddNodeOut );
      }
      else
      {
        BddNodeLocDc  = BddLocalSystem->ZERO;
        BddNodeLocOff = BddNodeOut;
      }

      BddNodeLocOff = applybddnode( (bddsystem *)0, ABL_NOR, 
                                    decbddrefext( BddNodeLocOff ),
                                    BddNodeLocOn );

      BddNodeLocOn  = applybddnode( (bddsystem *)0, ABL_AND, 
                                    decbddrefext( BddNodeLocOn ),
                                    BddNodeOut );

      BddNodeLocout = applybddnode( (bddsystem *)0, ABL_OR , 
                                    decbddrefext( BddNodeLocOn  ),
                                    decbddrefext( BddNodeLocOff ) );

      BddNodeLocout = applybddnode( (bddsystem *)0, ABL_OR, 
                                    decbddrefext( BddNodeLocout ), 
                                    decbddrefext( BddNodeLocDc  ) );

      BddNode = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddNode       ),
                              decbddrefext( BddNodeLocout ) );
    }

    BddNode = addbddcircuitout( (bddcircuit *)0, ScanState->NAME, BddNode );
    ScanState->USER = (void *)BddNode;
  }

# ifdef DEBUG
  testbddcircuit( (bddcircuit *)0 );
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                       FspMakeBddCircuit                     |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *FspMakeBddCircuit( FsmFigure1, FsmFigure2, BddSystem )

  fsmfig_list *FsmFigure1;
  fsmfig_list *FsmFigure2;
  bddsystem   *BddSystem;
{
  fsmin_list  *ScanIn;
  fsmout_list *ScanOut;
  bddcircuit  *BddCircuit;

  BddCircuit = createbddcircuit( FsmFigure1->NAME,
                                 FsmFigure1->NUMBER_IN + FsmFigure1->NUMBER_OUT,
                                 FsmFigure1->NUMBER_STATE, BddSystem );

  for ( ScanIn  = FsmFigure1->IN;
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanIn->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  for ( ScanOut  = FsmFigure1->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanOut->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  FsmFigure1->CIRCUIT = BddCircuit;
  FsmFigure2->CIRCUIT = BddCircuit;

  FspMakeBddNode( FsmFigure1 );
  FspMakeBddNode( FsmFigure2 );

  return( BddCircuit );
}

/*------------------------------------------------------------\
|                                                             |
|                        FspFormalProof                       |
|                                                             |
\------------------------------------------------------------*/

int FspFormalProof( FsmFigure1, FsmFigure2, FlagDebug )

  fsmfig_list *FsmFigure1;
  fsmfig_list *FsmFigure2;
  int          FlagDebug;
{
  bddsystem  *BddSystem;
  bddcircuit *BddCircuit;
  int         Error;

  FspFlagDebug = FlagDebug;

  if ( FsmFigure1->STACK != (fsmstack_list *)0 )
  {
    FspPrintf( stdout, "Error unable to treat FSM with stack !\n" ); 
    autexit( 1 );
  }

  if ( FsmFigure2->STACK != (fsmstack_list *)0 )
  {
    FspPrintf( stdout, "Error unable to treat FSM with stack !\n" ); 
    autexit( 1 );
  }

  if ( FsmFigure1->FIRST_STATE == (fsmstate_list *)0 )
  {
    FspPrintf( stdout, "Error missing first state in %s !\n", FsmFigure1->NAME ); 
    autexit( 1 );
  }

  if ( FsmFigure2->FIRST_STATE == (fsmstate_list *)0 )
  {
    FspPrintf( stdout, "Error missing first state in %s !\n", FsmFigure2->NAME ); 
    autexit( 1 );
  }

  if ( ! FspVerifyInputOutput( FsmFigure1, FsmFigure2 ) )
  {
    FspPrintf( stdout, "Error port mismatch between %s and %s\n\n",
               FsmFigure1->NAME, FsmFigure2->NAME ); 
    autexit( 1 );
  }

  FspExplodeStarState( FsmFigure1 );
  FspExplodeStarState( FsmFigure2 );

  BddSystem  = createbddsystem( 100, 1000, 1000, 900000 );
  SetBddSystemNoWarning( BddSystem );
  reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 100000, 50 );
  BddCircuit = FspMakeBddCircuit( FsmFigure1, FsmFigure2, BddSystem );

  Error = FspProduct( FsmFigure1, FsmFigure2 );

  destroybddsystem( BddSystem );
  destroybddcircuit( BddCircuit );

  return( ! Error );
}
