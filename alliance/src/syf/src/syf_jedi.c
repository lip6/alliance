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
| Tool    :                     SYF                           |
|                                                             |
| File    :                  Syf_jedi.c                       |
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"

# include "syf_fsm.h"
# include "syf_tools.h"
# include "syf_error.h"
# include "syf_must.h"
# include "syf_jedi.h"

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
|                     SyfFsmJediEncode                        |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmJediEncode( FsmFigure )

   fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  fsmstate_list  *ScanState;
  syfstate       *ScanSyfState;
  syfcode        *CodeArray;
  long           *EdgeWeightArray;

  short          *DoneArray;
  long           *SortArray;
  long           *ValueArray;

  long           StateCounter;
  long           BestState;
  long           SortState;
  long           DoneState;

  long           SumWeightMax;
  long           SumWeight;
  long           SumCostMin;
  long           SumCost;

  long           LineState;
  long           LineBest;


  long           SortIndex;
  long           StateIndex;
  long           ValueIndex;

  long           EdgeWeight;
  long           Distance;

  long           BestCode;
  long           StateCode;
  long           CodeValue;

  long           NumberState;
  long           NumberBit;
  long           CodeMax;
  long           ValueMax;

  SyfInfo     = FSM_SYF_INFO( FsmFigure );
  CodeMax     = SyfInfo->NUMBER_CODE;
  ValueMax    = CodeMax;
  CodeArray   = SyfInfo->CODE_ARRAY;
  NumberState = FsmFigure->NUMBER_STATE;
  NumberBit   = SyfInfo->NUMBER_BIT;

  SyfFsmGetMustangEdge( FsmFigure );

  autfreeblock( SyfInfo->MUSTANG_TRANS );
  autfreeblock( SyfInfo->MUSTANG_OUT   );

  if ( SyfInfo->MUSTANG_CTRL != (long *)0 )
  {
    autfreeblock( SyfInfo->MUSTANG_CTRL );
  }

  EdgeWeightArray  = SyfInfo->MUSTANG_EDGE;

  ValueArray = (long  *)autallocblock( sizeof(long)  * CodeMax     );
  DoneArray  = (short *)autallocblock( sizeof(short) * NumberState );
  SortArray  = (long  *)autallocblock( sizeof(long)  * NumberState );

/*
**  Initialize possible code values
*/
  for ( ValueIndex = 0; ValueIndex < ValueMax; ValueIndex++ )
  {
    ValueArray[ ValueIndex ] = ValueIndex;
  }

/*
**  Assign code pointer for states
*/
  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = FSM_SYF_STATE( ScanState );

    ScanSyfState->CODE = &CodeArray[ ScanSyfState->INDEX ];
  }

  for ( StateCounter = 0; StateCounter < NumberState; StateCounter++ )
  {
/*
**  Look for a "best" state
*/
    LineState    =  0;
    SumWeightMax = -1;

    for ( StateIndex = 0; StateIndex < NumberState; StateIndex++ )
    {
      if ( ! DoneArray[ StateIndex ] )
      {
        SumWeight = 0;

        if ( StateCounter == 0 )
        {
          for ( SortState = 0; SortState < NumberState; SortState++ )
          {
            SumWeight += EdgeWeightArray[ LineState + SortState ];
          }
        }
        else
        {
          for ( SortIndex = 0; SortIndex < StateCounter; SortIndex++ )
          {
            SortState  = SortArray[ SortIndex ];
            SumWeight += EdgeWeightArray[ LineState + SortState ];
          }
        }

        if ( SumWeight > SumWeightMax )
        {
          BestState    = StateIndex;
          LineBest     = LineState;
          SumWeightMax = SumWeight;
        }
      }

      LineState = LineState + NumberState;
    }
/*
**  Get unused code for the "best" state
*/
    SortArray[ StateCounter ] = BestState;
    DoneArray[ BestState    ] = 1;

    if ( StateCounter == 0 )
    {
      BestCode = 0;
    }
    else
    {
      SumCostMin = -1;

      for ( ValueIndex = 0; ValueIndex < ValueMax; ValueIndex++ )
      {
        SumCost   = 0;
        CodeValue = ValueArray[ ValueIndex ];

        for ( SortIndex = 0; SortIndex < StateCounter; SortIndex++ )
        {
          DoneState  = SortArray[ SortIndex ];
          EdgeWeight = EdgeWeightArray[ LineBest + DoneState ];
         
          if ( EdgeWeight != 0 )
          {
            Distance = SyfGetHamingDistance( CodeArray[ DoneState ].VALUE, 
                                             CodeValue, NumberBit );

            SumCost +=  Distance * EdgeWeight;
          }
        }

        if ( ( SumCost < SumCostMin ) ||
             ( SumCostMin == -1     ) )
        {
          SumCostMin = SumCost;
          BestCode   = ValueIndex;
        }
      }
    }

    ValueMax  = ValueMax - 1;
    StateCode = ValueArray[ BestCode ];

    if ( ValueMax != BestCode )
    {
      ValueArray[ BestCode ] = ValueArray[ ValueMax ];
      ValueArray[ ValueMax ] = StateCode;
    }

    CodeArray[ BestState ].USED  = 1;
    CodeArray[ BestState ].VALUE = StateCode;
  }

  for ( ValueIndex = 0; ValueIndex < ValueMax; ValueIndex++ )
  {
    CodeArray[ NumberState + ValueIndex ].VALUE = ValueArray[ ValueIndex ];
  }

  autfreeblock( SortArray  );
  autfreeblock( DoneArray  );
  autfreeblock( ValueArray );

  autfreeblock( SyfInfo->MUSTANG_EDGE );

  SyfInfo->MUSTANG_TRANS = (long *)0;
  SyfInfo->MUSTANG_OUT   = (long *)0;
  SyfInfo->MUSTANG_CTRL  = (long *)0;
  SyfInfo->MUSTANG_EDGE  = (long *)0;
}
