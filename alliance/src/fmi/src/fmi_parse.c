/*------------------------------------------------------------\
|                                                             |
| tool    :                     fmi                           |
|                                                             |
| file    :                  fmi_parse.c                      |
|                                                             |
| authors :                jacomme ludovic                    |
|                                                             |
| date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         include files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include FSM_H
# include FTL_H

# include "fmi_parse.h"

/*------------------------------------------------------------\
|                                                             |
|                           constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      private variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        parse fsm file                       |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list  *FmiParseFsm( FsmFilename )

  char *FsmFilename;
{
  fsmfig_list *FsmFigure;
  fsmstate_list  *ScanState;

  
  FsmFigure = getfsmfig( FsmFilename );

 /* on detache l'état star */
  
  ScanState = FsmFigure->STAR_STATE;

  if ( ScanState != (fsmstate_list *)0 )
  {
    *ScanState->PREV = ScanState->NEXT;

    if ( ScanState->NEXT != (fsmstate_list *)0 )
    {
      ScanState->NEXT->PREV = ScanState->PREV;
    }

    FsmFigure->NUMBER_STATE--;
  }

  ScanState = FsmFigure->STATE;
  
  simpfsmablexpr( FsmFigure );

  return( FsmFigure );
}
