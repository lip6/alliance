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
| File    :                   Syf_asp.c                       |
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
# include <stdlib.h>
# include <memory.h>
# include <string.h>
# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_must.h"
# include "syf_asp.h"

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
|                      SyfFsmGetAspState                      |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmGetAspState( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  long           *StateWeightArray;
  fsmstate_list  *ScanState;
  syfstate       *ScanSyfState;
  long            NumberState;
  long            IndexState;

  SyfInfo     = FSM_SYF_INFO( FsmFigure );
  NumberState = FsmFigure->NUMBER_STATE;

  if ( SyfInfo->ASP_STATE == (long *)0 )
  {
    StateWeightArray =

     (long *)autallocblock( sizeof( long ) * NumberState );

    SyfInfo->ASP_STATE = StateWeightArray;

    IndexState = 0;

    for ( ScanState  = FsmFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      ScanSyfState = FSM_SYF_STATE( ScanState );

      ScanSyfState->INDEX = IndexState;

      StateWeightArray[ IndexState++ ] += getablexprnumatom( ScanSyfState->ABL_TRANS );
    }
  }
  else
  {
    StateWeightArray = SyfInfo->ASP_STATE;

    memset( StateWeightArray, 0, sizeof(long) * NumberState );

    for ( ScanState  = FsmFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      ScanSyfState = FSM_SYF_STATE( ScanState );

      StateWeightArray[ ScanSyfState->INDEX ] += getablexprnumatom( ScanSyfState->ABL_TRANS );
    }
  }

# ifdef SYF_DEBUG

  SyfFsmViewAspState( FsmFigure );

# endif
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfFsmAspEncode                       |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmAspEncode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo       *SyfInfo;
  fsmstate_list *ScanState;
  syfstate      *ScanSyfState;

  syfcode       *CodeArray;
  long          *TransWeightArray;
  long          *OutWeightArray;
  long          *CtrlWeightArray;
  long          *StateWeightArray;

  long          *WeightArray;
  long          *SumWeightArray;
  long          *SortGroupArray;
  long          *SortStateArray;
  short         *SelectArray;
  long          *ValueArray;

  long           NumberState;
  long           NumberOut;
  long           NumberBit;
  long           NumberGroup;
  long           NumberCtrl;

  long           CodeMax;
  long           CodeValue;
  long           CodeIndex;

  long           BitSetMax;
  long           BitSetMask;
  long           BitSetCounter;
  long           RemainBit;
  long           RemainBitMax;

  long           GroupIndex;
  long           GroupIndexCtrl;
  long           SortGroupIndex;
  long           LineGroup;

  long           StateIndex;
  long           SortStateIndex;
  long           SumWeight;
  long           TransExist;

  SyfInfo     = FSM_SYF_INFO( FsmFigure );
  CodeMax     = SyfInfo->NUMBER_CODE;
  CodeArray   = SyfInfo->CODE_ARRAY;
  NumberBit   = SyfInfo->NUMBER_BIT;
  NumberState = FsmFigure->NUMBER_STATE;
  NumberOut   = FsmFigure->NUMBER_OUT;
  NumberGroup = NumberState + NumberOut;
  NumberCtrl  = FSM_MAX_CTRL - 1;

  SyfFsmGetAspState( FsmFigure );
  SyfFsmGetMustangTrans( FsmFigure );
  SyfFsmGetMustangOut( FsmFigure );
  SyfFsmGetMustangCtrl( FsmFigure );

  TransWeightArray = SyfInfo->MUSTANG_TRANS;
  OutWeightArray   = SyfInfo->MUSTANG_OUT;
  CtrlWeightArray  = SyfInfo->MUSTANG_CTRL;
  StateWeightArray = SyfInfo->ASP_STATE;

  if ( CtrlWeightArray != (long  *)0 )
  {
    NumberGroup = NumberGroup + NumberCtrl;
  }

  SumWeightArray = (long  *)autallocblock( sizeof(long)  * NumberGroup );
  SortGroupArray = (long  *)autallocblock( sizeof(long)  * NumberGroup );
  SortStateArray = (long  *)autallocblock( sizeof(long)  * NumberState );
  SelectArray    = (short *)autallocblock( sizeof(short) * CodeMax     );
  ValueArray     = (long  *)autallocblock( sizeof(long)  * CodeMax     );

/*
**  Initialize possible code values
*/

  for ( CodeIndex = 0; CodeIndex < CodeMax; CodeIndex++ )
  {
    CodeArray[ CodeIndex ].VALUE = CodeIndex;
  }

/*
**  Compute weight sum for all groups 
*/

# ifdef SYF_DEBUG

  fprintf( stdout, "\nCompute weight sum for groups\n" );

# endif

  WeightArray    = TransWeightArray;
  LineGroup      = 0;
  GroupIndexCtrl = ( NumberState + NumberOut );

  for ( GroupIndex = 0; GroupIndex < NumberGroup; GroupIndex++ )
  {
    SumWeight = 0;

    if ( GroupIndex == NumberState ) 
    {
      LineGroup   = 0;
      WeightArray = OutWeightArray;
    }
    else
    if ( GroupIndex == GroupIndexCtrl )
    {
      LineGroup   = 0;
      WeightArray = CtrlWeightArray;
    }

    for ( StateIndex = 0; StateIndex < NumberState; StateIndex++ )
    {
      TransExist = WeightArray[ StateIndex + LineGroup ];

      if ( TransExist )
      {
        SumWeight += StateWeightArray[ StateIndex ];
      }
    }

    SumWeightArray[ GroupIndex ] = SumWeight;

# ifdef SYF_DEBUG

  fprintf( stdout, "Sum %c%-3d: %d\n", 
           ( GroupIndex < NumberState    ) ? 'E' :
           ( GroupIndex < GroupIndexCtrl ) ? 'O' : 'C',
           GroupIndex, SumWeight );
# endif

    LineGroup += NumberState;
  }

/*
**  Sort sum weight array index
*/

  SyfFsmGetSortArray( SumWeightArray, SortGroupArray, NumberGroup );

/*
**  Sort state weight array index
*/

  SyfFsmGetSortArray( StateWeightArray, SortStateArray, NumberState );

/*
**  Encode states bit by bit
*/

  BitSetMax = 1 << ( NumberBit - 1 );

  for ( RemainBit = NumberBit; RemainBit > 0; RemainBit-- )
  {
    RemainBitMax   = 1 << ( RemainBit - 1 );
    BitSetMask     = 1 << ( NumberBit - RemainBit );
    BitSetCounter  = 0;

    memset( SelectArray, 0, sizeof(short) * CodeMax );

    for ( GroupIndex = 0; GroupIndex < NumberGroup; GroupIndex++ )
    {
      SortGroupIndex = SortGroupArray[ GroupIndex ];

      if ( SortGroupIndex < NumberState )
      {
        WeightArray = TransWeightArray;
        LineGroup   = SortGroupIndex * NumberState;
      }
      else
      if ( SortGroupIndex < GroupIndexCtrl )
      {
        WeightArray = OutWeightArray;
        LineGroup   = ( SortGroupIndex - NumberState ) * NumberState;
      }
      else
      {
        WeightArray = CtrlWeightArray;
        LineGroup   = ( SortGroupIndex - GroupIndexCtrl ) * NumberState;
      }

      for ( StateIndex = 0; StateIndex < NumberState; StateIndex++ )
      {
        SortStateIndex = SortStateArray[ StateIndex ];

        TransExist = WeightArray[ LineGroup + SortStateIndex ];

        if ( TransExist )
        {
          CodeValue = ValueArray[ SortStateIndex ];
/*
**  If the "BitSetMask" bit isn't currently set on this code and
**  if less than "RemainBitMax" same code have been already selected 
**  then affect to this state code value the bit "BitSetMask"
*/
          if ( ( ! ( CodeValue & BitSetMask              ) ) &&
               ( SelectArray[ CodeValue ] < RemainBitMax ) )
          {
            ValueArray[ SortStateIndex ] |= BitSetMask;
            SelectArray[ CodeValue ]++;

            BitSetCounter = BitSetCounter + 1;

            if ( BitSetCounter >= BitSetMax ) break;
          }
        }
      }

      if ( BitSetCounter >= BitSetMax ) break;
    }
  }

  BitSetMask = ( 1 << NumberBit ) - 1;

/*
**  Assign code pointer's for states
*/
  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = FSM_SYF_STATE( ScanState );
    StateIndex   = ScanSyfState->INDEX;
    CodeValue    = ValueArray[ StateIndex ] ^ BitSetMask;

    CodeArray[ CodeValue ].USED = 1;
    ScanSyfState->CODE = &CodeArray[ CodeValue ];
  }

  autfreeblock( SumWeightArray );
  autfreeblock( SortGroupArray );
  autfreeblock( SortStateArray );
  autfreeblock( ValueArray     );
  autfreeblock( SelectArray    );

  autfreeblock( SyfInfo->MUSTANG_TRANS );
  autfreeblock( SyfInfo->MUSTANG_OUT   );
  autfreeblock( SyfInfo->ASP_STATE     );

  if ( SyfInfo->MUSTANG_CTRL != (long *)0 )
  {
    autfreeblock( SyfInfo->MUSTANG_CTRL );
  }

  SyfInfo->MUSTANG_TRANS = (long *)0;
  SyfInfo->MUSTANG_OUT   = (long *)0;
  SyfInfo->MUSTANG_CTRL  = (long *)0;
  SyfInfo->ASP_STATE     = (long *)0;
}

# ifdef SYF_DEBUG

/*------------------------------------------------------------\
|                                                             |
|                        View Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Syf View Asp State                     |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmViewAspState( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo *SyfInfo;
  long    *StateWeightArray;
  long     NumberState;
  long     IndexLine;

  SyfInfo          = FSM_SYF_INFO( FsmFigure );
  NumberState      = FsmFigure->NUMBER_STATE;
  StateWeightArray = SyfInfo->ASP_STATE;

  fprintf( stdout, "\n--> Asp state weight array" );

  if ( StateWeightArray != (long *)0 )
  {
    fprintf( stdout, "\n    " );

    for ( IndexLine = 0; IndexLine < NumberState; IndexLine++ )
    {
      fprintf( stdout, "\nE%-3d %-3d", IndexLine, StateWeightArray[ IndexLine ] );
    }
  }

  fprintf( stdout, "\n<-- Asp state weight array" );
}

# endif
