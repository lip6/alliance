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
| File    :                 kis_parse.c                       |
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
# include FTL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "kis_parse.h"

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
|                          Privates                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        SyfKissParse                         |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *SyfKissParse( Name, FlagScan )

  char *Name;
  char  FlagScan;
{
  fsmfig_list    *FsmFigure;
  syfinfo        *SyfInfo;
  fsmstate_list  *ScanState;
  fsmout_list    *ScanOut;

  FsmFigure = Syfaddfsmfig( Name );
  SyfInfo   = FSM_SYF_INFO( FsmFigure );

  SyfInfo->SCAN_PATH = FlagScan;

  SyfInfo->CURRENT_STATE = namealloc( "current_state" );
  SyfInfo->RETURN_STATE  = namealloc( "return_state"  );
  SyfInfo->NEXT_STATE    = namealloc( "next_state"    );
  SyfInfo->NUMBER_CTRL   = FSM_MAX_CTRL;

  loadfsmfig( FsmFigure, Name );

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

  if ( FlagScan )
  {
    SyfInfo->SCAN_IN   = namealloc( "scan_in"   );
    SyfInfo->SCAN_OUT  = namealloc( "scan_out"  );
    SyfInfo->SCAN_TEST = namealloc( "scan_test" );

    addfsmport( FsmFigure, SyfInfo->SCAN_IN  , FSM_DIR_IN , FSM_TYPE_BIT );
    addfsmport( FsmFigure, SyfInfo->SCAN_TEST, FSM_DIR_IN , FSM_TYPE_BIT );
    addfsmport( FsmFigure, SyfInfo->SCAN_OUT , FSM_DIR_OUT, FSM_TYPE_BIT );

    addfsmin( FsmFigure , SyfInfo->SCAN_TEST );
    addfsmin( FsmFigure , SyfInfo->SCAN_IN   );
    addfsmout( FsmFigure, SyfInfo->SCAN_OUT  );
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    Syfaddfsmsyfstate( FsmFigure, ScanState );
  }

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    Syfaddfsmsyfout( ScanOut );
  }

  return( FsmFigure );
}
