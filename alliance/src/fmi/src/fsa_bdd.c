/*------------------------------------------------------------\
|                                                             |
| Tool    :                     FSA                           |
|                                                             |
| File    :                   fsa_bdd.c                       |
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

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include FSM_H
# include FTL_H
# include PAT_H
# include PPT_H

# include "fsa_error.h"
# include "fsa_bdd.h"

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
|                     Fsa Fsm Make Bdd Circuit                |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *FsaFsmMakeBddCircuit( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmin_list  *ScanIn;
  fsmout_list *ScanOut;
  bddsystem   *BddSystem;
  bddcircuit  *BddCircuit;

  BddSystem  = createbddsystem( 50, 10000, 1000, 900000 );

  reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 100000, 50 );

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

  makefsmbddnode( FsmFigure );
  reorderbddsystemwindow( BddSystem );

  return( BddCircuit );
}

/*------------------------------------------------------------\
|                                                             |
|                   Fsa Fsm Convert Bdd Circuit               |
|                                                             |
\------------------------------------------------------------*/

void FsaFsmConvertBddCircuit( FsmFigure )

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
