/*------------------------------------------------------------\
|                                                             |
| Tool    :                     FSA                           |
|                                                             |
| File    :                   fmi_bdd.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"

# include "fmi_bdd.h"

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Fmi Fsm Make Bdd Node                   |
|                                                             |
\------------------------------------------------------------*/

void FmiMakeFsmBddNode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmlocout_list *ScanLocout;
  fsmtrans_list  *ScanTrans;
  bddnode        *BddNode;
  bddnode        *BddNodeOut;
  bddnode        *BddNodeLocout;

  setbddlocalcircuit( FsmFigure->CIRCUIT );
  setbddlocalsystem( BddLocalCircuit->BDD_SYSTEM );

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
        BddNodeLocout = addbddcircuitabl( (bddcircuit *)0, ScanLocout->ABL );
      }
      else
      {
        BddNodeLocout = BddLocalSystem->ZERO;
      }

      BddNodeLocout = applybddnode( (bddsystem *)0, ABL_NXOR, 
                                    decbddrefext( BddNodeLocout ), 
                                    decbddrefext( BddNodeOut    ) );

      BddNode = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddNode       ),
                              decbddrefext( BddNodeLocout ) );
    }

    BddNode = addbddcircuitout( (bddcircuit *)0, ScanState->NAME, BddNode );
    ScanState->USER = (void *)BddNode;
  }

#if 0
  testbddcircuit( (bddcircuit *)0 );
#endif
}

/*------------------------------------------------------------\
|                                                             |
|                     Fmi Fsm Make Bdd Circuit                |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *FmiMakeFsmBddCircuit( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmin_list  *ScanIn;
  fsmout_list *ScanOut;
  bddsystem   *BddSystem;
  bddcircuit  *BddCircuit;

  BddSystem  = createbddsystem( 50, 10000, 1000, 900000 );
  reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 100000, 50 );
  SetBddSystemNoWarning( BddSystem );

  BddCircuit = createbddcircuit( FsmFigure->NAME,
                                 FsmFigure->NUMBER_IN + FsmFigure->NUMBER_OUT,
                                 FsmFigure->NUMBER_STATE, BddSystem );

  for ( ScanIn  = FsmFigure->IN;
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanIn->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanOut->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  FsmFigure->CIRCUIT = BddCircuit;

  FmiMakeFsmBddNode( FsmFigure );

  return( FsmFigure->CIRCUIT );
}

/*------------------------------------------------------------\
|                                                             |
|                   Fmi Fsm Convert Bdd Circuit               |
|                                                             |
\------------------------------------------------------------*/

void FmiConvertFsmBddCircuit( FsmFigure )

  fsmfig_list *FsmFigure;
{
  bddcircuit *BddCircuit;
  bddsystem  *BddSystem;

  BddCircuit = FsmFigure->CIRCUIT;
  BddSystem  = BddCircuit->BDD_SYSTEM;

  reorderbddsystemwindow( BddSystem );
  convertfsmbddnodeabl( FsmFigure );

  destroybddcircuit( BddCircuit );
  destroybddsystem( BddSystem );

  FsmFigure->CIRCUIT = (bddcircuit *)0;
}
