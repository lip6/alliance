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

# include <stdio.h>
# include <memory.h>
# include <stdlib.h>
# include <string.h>

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include FSM_H

# include "syf_env.h"
# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_tools.h"
# include "syf_must.h"

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
|                     SyfFsmGetMustangCtrl                    |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmGetMustangCtrl( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  fsmstack_list  *ScanStack;
  long           *CtrlWeightArray;
  long            NumberCtrl;
  long            NumberState;
  long            StateIndex;
  long            LineCtrl;
  long            Control;

  SyfInfo     = FSM_SYF_INFO( FsmFigure );
  NumberState = FsmFigure->NUMBER_STATE;
  NumberCtrl  = FSM_MAX_CTRL - 1;

  if ( ! SyfInfo->STACK ) return;

  if ( SyfInfo->MUSTANG_CTRL == (long*)0 )
  {
    CtrlWeightArray =

     (long*)autallocblock( sizeof(long) * NumberState * NumberCtrl );

    SyfInfo->MUSTANG_CTRL = CtrlWeightArray;
  }
  else
  {
    CtrlWeightArray = SyfInfo->MUSTANG_CTRL;

    memset( CtrlWeightArray, 0, sizeof(long) * NumberState * NumberCtrl );
  }

  for ( ScanStack  = FsmFigure->STACK;
        ScanStack != (fsmstack_list *)0;
        ScanStack  = ScanStack->NEXT )
  {
    if ( IsFsmStarStack( ScanStack ) ) continue;

    Control = ScanStack->CTRL;

    if ( Control != FSM_CTRL_NOP )
    {
      LineCtrl   = ( Control - 1 ) * NumberState;
      StateIndex = FSM_SYF_STATE( ScanStack->CURRENT )->INDEX;

      if ( SYF_MUSTANG_JEDI_ATOM )
      {
        CtrlWeightArray[ LineCtrl + StateIndex ] += 

          getablexprnumatom( ScanStack->ABL );
      }
      else
      {
        CtrlWeightArray[ LineCtrl + StateIndex ]++;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfFsmGetMustangOut                    |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmGetMustangOut( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  long           *OutWeightArray;
  fsmout_list    *ScanOut;
  fsmstate_list  *ScanState;
  fsmlocout_list *ScanLocout;
  long            NumberState;
  long            NumberOut;
  long            IndexState;
  long            IndexOut;

  SyfInfo        = FSM_SYF_INFO( FsmFigure );
  NumberState    = FsmFigure->NUMBER_STATE;
  NumberOut      = FsmFigure->NUMBER_OUT;
  OutWeightArray = SyfInfo->MUSTANG_OUT;

  if ( OutWeightArray == (long *)0 )
  {
    OutWeightArray = 

     (long *)autallocblock( sizeof(long) * NumberState * NumberOut );

    SyfInfo->MUSTANG_OUT = OutWeightArray;

    IndexOut = 0;

    for ( ScanOut  = FsmFigure->OUT;
          ScanOut != (fsmout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      FSM_SYF_OUT( ScanOut )->INDEX = IndexOut++;
    }
  }
  else
  {
    memset( OutWeightArray, 0, sizeof(long) * NumberState * NumberOut );
  }

  IndexState  = 0;

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      IndexOut = FSM_SYF_OUT( ScanLocout->OUT )->INDEX;

      if ( SYF_MUSTANG_JEDI_ATOM )
      {
        if ( ScanLocout->ABL != (chain_list *)0 )
        {
          OutWeightArray[ ( IndexOut * NumberState ) + IndexState  ] +=

            getablexprnumatom( ScanLocout->ABL );
        }
      }
      else
      {
        OutWeightArray[ ( IndexOut * NumberState ) + IndexState  ]++;
      }
    }

    FSM_SYF_STATE( ScanState )->INDEX = IndexState++;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     SyfFsmGetMustangTrans                   |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmGetMustangTrans( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  long           *TransWeightArray;
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;
  long            NumberState;
  long            Index;
  long            IndexTo;
  long            IndexFrom;
  long            IndexState;

  SyfInfo          = FSM_SYF_INFO( FsmFigure );
  NumberState      = FsmFigure->NUMBER_STATE;
  TransWeightArray = SyfInfo->MUSTANG_TRANS;

  if ( TransWeightArray == (long *)0 )
  {
    TransWeightArray = 

     (long *)autallocblock( sizeof(long) * NumberState * NumberState );
  
    SyfInfo->MUSTANG_TRANS = TransWeightArray;
  
    IndexState = 0;

    for ( ScanState  = FsmFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      FSM_SYF_STATE( ScanState )->INDEX = IndexState++;
    }
  }
  else
  {
    memset( TransWeightArray, 0, sizeof(long) * NumberState * NumberState );
  }

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    if ( IsFsmStarTrans( ScanTrans ) ) continue;

    IndexTo   = FSM_SYF_STATE( ScanTrans->TO   )->INDEX;
    IndexFrom = FSM_SYF_STATE( ScanTrans->FROM )->INDEX;
    Index     = ( IndexTo * NumberState ) + IndexFrom;
    
    if ( SYF_MUSTANG_JEDI_ATOM )
    {
      TransWeightArray[ Index ] += getablexprnumatom( ScanTrans->ABL );
    }
    else
    {
      TransWeightArray[ Index ]++;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfFsmGetMustangEdge                   |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmGetMustangEdge( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  long            NumberState;
  long            NumberOut;
  long            NumberBit;
  long            NumberCtrl;
  long           *EdgeWeightArray;
  long           *OutWeightArray;
  long           *TransWeightArray;
  long           *CtrlWeightArray;
  long            StateIKWeight;
  long            StateJKWeight;
  long            CtrlIKWeight;
  long            CtrlJKWeight;
  long            OutIKWeight;
  long            OutJKWeight;
  long            LineStateI;
  long            LineStateJ;
  long            LineCtrlK;
  long            LineOutK;
  long            StateIJ;
  long            StateI;
  long            StateJ;
  long            StateK;
  long            OutK;
  long            CtrlK;

  SyfInfo         = FSM_SYF_INFO( FsmFigure );
  NumberState     = FsmFigure->NUMBER_STATE;
  NumberOut       = FsmFigure->NUMBER_OUT;
  NumberBit       = SyfInfo->NUMBER_BIT;
  NumberCtrl      = FSM_MAX_CTRL - 1;
  EdgeWeightArray = SyfInfo->MUSTANG_EDGE;

  if ( SyfInfo->MUSTANG_OUT == (long *)0 )
  {
    SyfFsmGetMustangOut( FsmFigure );
  }

  if ( SyfInfo->MUSTANG_TRANS == (long *)0 )
  {
    SyfFsmGetMustangTrans( FsmFigure );
  }

  if ( SyfInfo->MUSTANG_CTRL == (long *)0 )
  {
    SyfFsmGetMustangCtrl( FsmFigure );
  }

  if ( EdgeWeightArray == (long *)0 )
  {
    EdgeWeightArray = 

     (long *)autallocblock( sizeof(long) * NumberState * NumberState );

    SyfInfo->MUSTANG_EDGE = EdgeWeightArray;
  }
  else
  {
    memset( EdgeWeightArray, 0, sizeof(long) * NumberState * NumberState );
  }

  EdgeWeightArray   = SyfInfo->MUSTANG_EDGE;
  OutWeightArray    = SyfInfo->MUSTANG_OUT;
  CtrlWeightArray   = SyfInfo->MUSTANG_CTRL;
  TransWeightArray  = SyfInfo->MUSTANG_TRANS;

  LineStateI = 0;

  for ( StateI = 0; StateI < NumberState; StateI++ )
  {
    LineStateJ = 0;

    for ( StateJ = 0; StateJ < NumberState; StateJ++ )
    {
      StateIJ = LineStateI + StateJ;

      if ( StateI != StateJ )
      {
        for ( StateK = 0; StateK < NumberState; StateK++ )
        {
          StateIKWeight = TransWeightArray[ LineStateI + StateK ];
          StateJKWeight = TransWeightArray[ LineStateJ + StateK ];

          if ( ( StateIKWeight != 0 ) &&
               ( StateJKWeight != 0 ) )
          {
            EdgeWeightArray[ StateIJ ] += StateIKWeight * StateJKWeight;
          }
        }

        EdgeWeightArray[ StateIJ ] *= NumberBit;

        if ( CtrlWeightArray != (long *)0 )
        {
          LineCtrlK = 0;

          for ( CtrlK = 0; CtrlK < NumberCtrl; CtrlK++ )
          {
            CtrlIKWeight = CtrlWeightArray[ LineCtrlK + StateI ];
            CtrlJKWeight = CtrlWeightArray[ LineCtrlK + StateJ ];

            if ( ( CtrlIKWeight != 0 ) &&
                 ( CtrlJKWeight != 0 ) )
            {
              EdgeWeightArray[ StateIJ ] += CtrlIKWeight * CtrlJKWeight;
            }

            LineCtrlK = LineCtrlK + NumberState;
          }
        }

        LineOutK = 0;

        for ( OutK = 0; OutK < NumberOut; OutK++ )
        {
          OutIKWeight = OutWeightArray[ LineOutK + StateI ];
          OutJKWeight = OutWeightArray[ LineOutK + StateJ ];

          if ( ( OutIKWeight != 0 ) &&
               ( OutJKWeight != 0 ) )
          {
            EdgeWeightArray[ StateIJ ] += OutIKWeight * OutJKWeight;
          }

          LineOutK = LineOutK + NumberState;
        }
      }

      LineStateJ = LineStateJ + NumberState;
    }

    LineStateI = LineStateI + NumberState;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    SyfFsmGetMustangCost                     |
|                                                             |
\------------------------------------------------------------*/

long SyfFsmGetMustangCost( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  fsmstate_list  *ScanState;
  syfstate       *ScanSyfState;
  long            NumberState;
  long            NumberBit;
  long           *EdgeWeightArray;
  long           *CodeArray;
  long            StateI;
  long            StateJ;
  long            LineStateI;
  long            Distance;
  long            Weight;
  char            FreeEdge;
  long            EdgeCost;

  SyfInfo     = FSM_SYF_INFO( FsmFigure );
  NumberState = FsmFigure->NUMBER_STATE;
  NumberBit   = SyfInfo->NUMBER_BIT;

  if ( SyfInfo->CODE_ARRAY == (syfcode *)0 )
  {
    return( 0 );
  }

  if ( SyfInfo->MUSTANG_EDGE == (long *)0 )
  {
    SyfFsmGetMustangEdge( FsmFigure );

    autfreeblock( SyfInfo->MUSTANG_TRANS );
    autfreeblock( SyfInfo->MUSTANG_OUT   );

    if ( SyfInfo->MUSTANG_CTRL != (long *)0 )
    {
      autfreeblock( SyfInfo->MUSTANG_CTRL );
    }

    SyfInfo->MUSTANG_TRANS = (long *)0;
    SyfInfo->MUSTANG_OUT   = (long *)0;
    SyfInfo->MUSTANG_CTRL  = (long *)0;

    FreeEdge = 1;
  }
  else
  {
    FreeEdge = 0;
  }

  EdgeWeightArray = SyfInfo->MUSTANG_EDGE;

  CodeArray = (long *)autallocblock( sizeof(long) * NumberState );

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    ScanSyfState = FSM_SYF_STATE( ScanState );
    
    CodeArray[ ScanSyfState->INDEX ] = ScanSyfState->CODE->VALUE;
  }

  EdgeCost   = 0;
  LineStateI = 0;

  for ( StateI = 0; StateI < NumberState; StateI++ )
  {
    for ( StateJ = 0; StateJ < NumberState; StateJ++ )
    {
      if ( StateJ != StateI )
      {
        Weight = EdgeWeightArray[ StateJ + LineStateI ];

        if ( Weight != 0 )
        {
          Distance = 

           SyfGetHamingDistance( CodeArray[ StateI ], CodeArray[ StateJ ], NumberBit );

          EdgeCost = EdgeCost + ( Distance * Weight );
        }
      }
    }

    LineStateI = LineStateI + NumberState;
  }

  autfreeblock( CodeArray );

  if ( FreeEdge )
  {
    autfreeblock( SyfInfo->MUSTANG_EDGE );

    SyfInfo->MUSTANG_EDGE = (long *)0;
  }

  return( EdgeCost );
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfFsmGetSortArray                    |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmGetSortArray( ValueArray, IndexArray, ArraySize )

  long *ValueArray;
  long *IndexArray;
  long  ArraySize;
{
  long Node;
  long NodeIndex;
  long LastNode;
  long Father;
  long FatherIndex;
  long LeftChild; 
  long LeftIndex;
  long RightChild;
  long RightIndex;
  long Child;
  long ChildIndex;

  for ( LastNode = 0; LastNode < ArraySize; LastNode++ )
  {
    IndexArray[ LastNode ] = LastNode;

    NodeIndex = LastNode;
    Node      = LastNode;

    do
    {
      Father = HEAP_FATHER( Node );

      if ( Father != -1 )
      {
        FatherIndex = IndexArray[ Father ];

        if ( ValueArray[ FatherIndex ] > ValueArray[ NodeIndex ] )
        {
          IndexArray[ Father ] = NodeIndex;
          IndexArray[ Node   ] = FatherIndex;

          Node = Father;
        }
      }
    }
    while ( Node == Father );
  }

  for ( LastNode = ArraySize - 1; LastNode >= 0; LastNode-- )
  {
    Node = 0;

    NodeIndex              = IndexArray[ LastNode ];
    IndexArray[ LastNode ] = IndexArray[ 0 ];
    IndexArray[ 0 ]        = NodeIndex;

    do
    {
      LeftChild  = HEAP_LEFT_CHILD( Node );
      RightChild = HEAP_RIGHT_CHILD( Node );

      if ( LeftChild < LastNode )
      {
        LeftIndex  = IndexArray[ LeftChild  ];
        RightIndex = IndexArray[ RightChild ];

        if ( ( RightChild               < LastNode                ) &&
             ( ValueArray[ RightIndex ] < ValueArray[ LeftIndex ] ) )
        {
          Child      = RightChild;
          ChildIndex = RightIndex;
        }
        else
        {
          Child      = LeftChild;
          ChildIndex = LeftIndex;
        }

        if ( ValueArray[ NodeIndex ] > ValueArray[ ChildIndex ] )
        {
          IndexArray[ Child ] = NodeIndex;
          IndexArray[ Node ]  = ChildIndex;

          Node = Child;
        }
        else 
        {
          LeftChild = LastNode;
        }
      }
    }
    while ( LeftChild < LastNode );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      SyfFsmMustangEncode                    |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmMustangEncode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  syfinfo        *SyfInfo;
  fsmstate_list  *ScanState;
  syfstate       *ScanSyfState;
  syfcode        *CodeArray;
  long           *EdgeArray;

  short          *DoneArray;
  long           *SortArray;
  long           *ValueArray;

  long           NearCounter;
  long           NearCounterMax;
  long           StateCounter;

  long           NearState;
  long           BestState;
  long           SortState;

  long           SumWeightMax;
  long           SumWeight;
  long           EdgeWeight;

  long           SortIndex;
  long           StateIndex;
  long           ValueIndex;
  long           BestIndex;

  long           LineState;
  long           LineSort;

  long           StateCode;
  long           NearCode;

  long           BestDistance;
  long           Distance;

  long           BitSet;
  long           BestBitSet;

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

  SyfFsmGetMustangEdge( FsmFigure);

  autfreeblock( SyfInfo->MUSTANG_TRANS );
  autfreeblock( SyfInfo->MUSTANG_OUT   );

  if ( SyfInfo->MUSTANG_CTRL != (long *)0 )
  {
    autfreeblock( SyfInfo->MUSTANG_CTRL );
  }

  EdgeArray  = SyfInfo->MUSTANG_EDGE;

  ValueArray = (long  *)autallocblock( sizeof(long)  * CodeMax     );
  DoneArray  = (short *)autallocblock( sizeof(short) * NumberState );
  SortArray  = (long  *)autallocblock( sizeof(long)  * NumberState 
                                                     * NumberState );
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

/*
**  Sort states index from edge weight values
*/
  LineState = 0;

  for ( StateIndex = 0; StateIndex < NumberState; StateIndex++ )
  {
    SyfFsmGetSortArray( EdgeArray + LineState, 
                        SortArray + LineState, NumberState );

    LineState = LineState + NumberState;
  }

  StateCounter = 0;

  while ( StateCounter < NumberState )
  {
/*
**  Look for a "best" state, and a maximum of NumberBit "near" states
*/
    SumWeightMax   = -1;
    BestState      = -1;
    LineSort       = 0;
    NearCounterMax = 0;

    for ( StateIndex = 0; StateIndex < NumberState; StateIndex++ )
    {
      if ( ! DoneArray[ StateIndex ] )
      {
        SumWeight   = 0;
        NearCounter = 0;

        for ( SortIndex = 0; SortIndex < NumberState; SortIndex++ )
        {
          SortState = SortArray[ LineSort + SortIndex ];

          if ( ! DoneArray[ SortState ] )
          {
            EdgeWeight = EdgeArray[ LineSort + SortState ];

            if ( EdgeWeight == 0 ) break;

            SumWeight = SumWeight + EdgeWeight;

            NearCounter++;

            if ( NearCounter >= NumberBit ) break;
          }
        }
   
        if ( SumWeight > SumWeightMax )
        {
          BestState      = StateIndex;
          LineState      = LineSort;
          SumWeightMax   = SumWeight;
          NearCounterMax = NearCounter;
        }
      }

      LineSort = LineSort + NumberState;
    }
/*
**  Get unused code for the "best" state
*/
    DoneArray[ BestState ] = 1;

    if ( ! CodeArray[ BestState ].USED )
    {
      ValueMax  = ValueMax - 1;
      StateCode = ValueArray[ 0 ];

      if ( ValueMax != 0 )
      {
        ValueArray[ 0 ]        = ValueArray[ ValueMax ];
        ValueArray[ ValueMax ] = StateCode;
      }

      CodeArray[ BestState ].USED  = 1;
      CodeArray[ BestState ].VALUE = StateCode;

      StateCounter++;
    }
    else
    {
      StateCode = CodeArray[ BestState ].VALUE;
    }
/*
**  Get nearest code for the "near" states
*/
    NearCounter = 0;
    LineSort    = LineState;

    for ( SortIndex = 0; SortIndex < NumberState; SortIndex++ )
    {
      NearState = SortArray[ LineSort + SortIndex ];

      if ( DoneArray[ NearState ] ) continue;

      if ( ! CodeArray[ NearState ].USED )
      {
        BestIndex    = 0;
        BestDistance = NumberBit;
        BestBitSet   = NumberBit;
/*
**  Search the nearest code in the possible code array
*/
        for ( ValueIndex = 0; ValueIndex < ValueMax; ValueIndex++ )
        {
          Distance =  

           SyfGetHamingDistance( StateCode, ValueArray[ ValueIndex ], NumberBit );

          BitSet = 

           SyfGetNumberBitSet( ValueArray[ ValueIndex ], NumberBit );

          if ( BestDistance > Distance )
          {
            BestDistance = Distance;
            BestBitSet   = BitSet;
            BestIndex    = ValueIndex;
          }
          else
          if ( ( BestDistance == Distance  ) &&
               ( BitSet       < BestBitSet ) )
          {
            BestBitSet = BitSet;
            BestIndex  = ValueIndex;
          }
        }

        ValueMax = ValueMax - 1;
        NearCode = ValueArray[ BestIndex ];

        if ( BestIndex != ValueMax )
        {
          ValueArray[ BestIndex ] = ValueArray[ ValueMax  ];
          ValueArray[ ValueMax  ] = NearCode;
        }

        CodeArray[ NearState ].USED  = 1;
        CodeArray[ NearState ].VALUE = NearCode;

        StateCounter++;
      }

      NearCounter++;

      if ( NearCounter >= NearCounterMax ) break;
    }
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
