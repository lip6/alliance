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
| File    :                  Syf_must.c                       |
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

# include <stdio.h>
# include <memory.h>
# include <stdlib.h>
# include <string.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_onehot.h"

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
|                      SyfFsmOneHotEncode                     |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmOneHotEncode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo       *SyfInfo;
  fsmstate_list *ScanState;
  syfcode       *CodeArray;
  long           CodeMax;
  long           Index;

  SyfInfo   = FSM_SYF_INFO( FsmFigure );
  CodeMax   = SyfInfo->NUMBER_CODE;
  CodeArray = SyfInfo->CODE_ARRAY;

  ScanState = FsmFigure->STATE;

  for ( Index = 0; Index < CodeMax; Index++ )
  {
    FSM_SYF_STATE( ScanState )->CODE = &CodeArray[ Index ];

    CodeArray[ Index ].VALUE = Index;
    CodeArray[ Index ].USED  = 1;

    ScanState = ScanState->NEXT;
  }
}
