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
| Tool    :                     SYF                           |
|                                                             |
| File    :                 fbh_parse.c                       |
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

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "fbh_parse.h"

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

  static char *SyfClockKeyword         = (char *)0;
  static char *SyfCurrentStateKeyword  = (char *)0;
  static char *SyfNextStateKeyword     = (char *)0;
  static char *SyfReturnStateKeyword   = (char *)0;
  static char *SyfControlKeyword       = (char *)0;
  static char *SyfToolsKeyword         = (char *)0;
  static char *SyfScanInKeyword        = (char *)0;
  static char *SyfScanOutKeyword       = (char *)0;
  static char *SyfScanTestKeyword      = (char *)0;

  static char *SyfStackControlKeyword[ FSM_MAX_CTRL ];

  static char *SyfClockName;
  static char *SyfCurrentStateName;
  static char *SyfNextStateName;
  static char *SyfReturnStateName;
  static char *SyfControlName;
  static char *SyfScanInName;
  static char *SyfScanOutName;
  static char *SyfScanTestName;
  static char *SyfStackControlName[ FSM_MAX_CTRL ];

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

void SyfInitializeKeyword()
{
  if ( SyfClockKeyword == (char *)0 )
  {
    SyfClockKeyword        = namealloc( "clock"         );
    SyfCurrentStateKeyword = namealloc( "current_state" );
    SyfReturnStateKeyword  = namealloc( "return_state"  );
    SyfNextStateKeyword    = namealloc( "next_state"    );
    SyfControlKeyword      = namealloc( "control"       );
    SyfScanInKeyword       = namealloc( "scan_in"       );
    SyfScanOutKeyword      = namealloc( "scan_out"      );
    SyfScanTestKeyword     = namealloc( "scan_test"     );
    SyfToolsKeyword        = namealloc( "syf"           );

    SyfStackControlKeyword[ FSM_CTRL_NOP  ] = namealloc( "nop"  );
    SyfStackControlKeyword[ FSM_CTRL_PUSH ] = namealloc( "push" );
    SyfStackControlKeyword[ FSM_CTRL_POP  ] = namealloc( "pop"  );
  }

  SyfClockName        = SyfClockKeyword;
  SyfCurrentStateName = SyfCurrentStateKeyword;
  SyfReturnStateName  = SyfReturnStateKeyword;
  SyfNextStateName    = SyfNextStateKeyword;
  SyfControlName      = SyfControlKeyword;
  SyfScanInName       = SyfScanInKeyword;
  SyfScanOutName      = SyfScanOutKeyword;
  SyfScanTestName     = SyfScanTestKeyword;

  SyfStackControlName[ FSM_CTRL_NOP  ] = SyfStackControlKeyword[ FSM_CTRL_NOP  ];
  SyfStackControlName[ FSM_CTRL_PUSH ] = SyfStackControlKeyword[ FSM_CTRL_PUSH ];
  SyfStackControlName[ FSM_CTRL_POP  ] = SyfStackControlKeyword[ FSM_CTRL_POP  ];
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfFbhTreatPort                       |
|                                                             |
\------------------------------------------------------------*/

void SyfFbhTreatPort( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmin_list     *ScanIn;
  fsmout_list    *ScanOut;
  syfinfo        *SyfInfo;
  char            ScanOk;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  ScanOk  = ( SyfInfo->SCAN_PATH ) ? 0x00 : 0x07;

  for ( ScanIn  = FsmFigure->IN; 
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    if ( ScanIn->NAME == SyfScanInName )
    {
      ScanOk |= 0x01;
    }
    else
    if ( ScanIn->NAME == SyfScanTestName )
    {
      ScanOk |= 0x02;
    }
  }

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    if ( ScanOut->NAME == SyfScanOutName )
    {
      ScanOk |= 0x04;
    }

    Syfaddfsmsyfout( ScanOut );
  }

  if ( ScanOk != 0x07 )
  {
    SyfError( SYF_ERROR_MISSING_SCAN_PORT, FsmFigure->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfFbhTreatPragma                     |
|                                                             |
\------------------------------------------------------------*/

void SyfFbhTreatPragma( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmpragma_list *ScanPragma;
  syfinfo        *SyfInfo;
  char           *PragmaType;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  for ( ScanPragma  = FsmFigure->PRAGMA;
        ScanPragma != (fsmpragma_list *)0;
        ScanPragma  = ScanPragma->NEXT )
  {
    PragmaType = ScanPragma->TYPE;

    if ( PragmaType == SyfCurrentStateKeyword )
    {
      SyfCurrentStateName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfNextStateKeyword )
    {
      SyfNextStateName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfReturnStateKeyword )
    {
      SyfReturnStateName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfControlKeyword )
    {
      SyfControlName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfClockKeyword )
    {
      SyfClockName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfStackControlKeyword[ FSM_CTRL_NOP ] )
    {
      SyfStackControlName[ FSM_CTRL_NOP ] = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfStackControlKeyword[ FSM_CTRL_PUSH ] )
    {
      SyfStackControlName[ FSM_CTRL_PUSH ] = ScanPragma->NAME;
    }
    else
    if ( PragmaType == SyfStackControlKeyword[ FSM_CTRL_POP ] )
    {
      SyfStackControlName[ FSM_CTRL_POP ] = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfScanInKeyword )
    {
      SyfScanInName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfScanOutKeyword )
    {
      SyfScanOutName = ScanPragma->NAME;
    }
    else 
    if ( PragmaType == SyfScanTestKeyword )
    {
      SyfScanTestName = ScanPragma->NAME;
    }
  }

  SyfInfo->SCAN_IN       = SyfScanInName;
  SyfInfo->SCAN_OUT      = SyfScanOutName;
  SyfInfo->SCAN_TEST     = SyfScanTestName;
  SyfInfo->CURRENT_STATE = SyfCurrentStateName;
  SyfInfo->NEXT_STATE    = SyfNextStateName;
  SyfInfo->RETURN_STATE  = SyfReturnStateName;
  SyfInfo->NUMBER_CTRL   = FSM_MAX_CTRL;
}

/*------------------------------------------------------------\
|                                                             |
|                        SyfFbhParse                          |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *SyfFbhParse( Name, FlagScan )

  char *Name;
  char  FlagScan;
{
  fsmfig_list    *FsmFigure;
  fsmfig_list    *ScanFigure;
  chain_list     *ScanChain;
  syfinfo        *SyfInfo;
  fsmstate_list  *ScanState;

  SyfInitializeKeyword();

  FsmFigure = Syfaddfsmfig( Name );

  loadfsmfig( FsmFigure, Name );

  if ( IsFsmFigMulti( FsmFigure ) )
  {
    for ( ScanChain  = FsmFigure->MULTI;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanFigure = (fsmfig_list *)ScanChain->DATA; 
      SyfInfo = Syfaddfsmsyfinfo( ScanFigure );
    }
  }
  else
  {
    FsmFigure->MULTI = addchain( (chain_list *)0, FsmFigure );
  }

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure = (fsmfig_list *)ScanChain->DATA;
    SyfInfo    = FSM_SYF_INFO( ScanFigure );

    SyfInfo->SCAN_PATH = FlagScan;
  
    ScanState = ScanFigure->STAR_STATE;
  
    if ( ScanState != (fsmstate_list *)0 )
    {
      *ScanState->PREV = ScanState->NEXT;
  
      if ( ScanState->NEXT != (fsmstate_list *)0 )
      {
        ScanState->NEXT->PREV = ScanState->PREV;
      }
  
      ScanFigure->NUMBER_STATE--;
    }
  
    SyfInfo->STACK = ( ScanFigure->STACK_SIZE != 0 );
  
    SyfFbhTreatPragma( ScanFigure );
    SyfFbhTreatPort( ScanFigure );
  
    for ( ScanState  = ScanFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      Syfaddfsmsyfstate( ScanFigure, ScanState );
    }
  }

  return( FsmFigure );
}
