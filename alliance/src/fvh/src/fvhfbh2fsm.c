/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| Tool    :                     FVH                           |
|                                                             |
| File    :                  fvhfbh2fsm.c                     |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "fvh.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "fvherror.h"
# include "fvhfbh2fsm.h"

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
|                       Private Variables                     |
|                                                             |
\------------------------------------------------------------*/

  static char *FvhClockKeyword         = (char *)0;
  static char *FvhCurrentStateKeyword  = (char *)0;
  static char *FvhNextStateKeyword     = (char *)0;
  static char *FvhFirstStateKeyword    = (char *)0;
  static char *FvhReturnStateKeyword   = (char *)0;
  static char *FvhControlKeyword       = (char *)0;
  static char *FvhOthersKeyword        = (char *)0;

  static char *FvhStackControlKeyword[ FSM_MAX_CTRL ];

  static char *FvhClockName;
  static char *FvhCurrentStateName;
  static char *FvhNextStateName;
  static char *FvhFirstStateName;
  static char *FvhReturnStateName;
  static char *FvhControlName;
  static char *FvhStackControlName[ FSM_MAX_CTRL ];

  static fbtyp_list *FvhStateType;
  static fbtyp_list *FvhControlType;

  static chain_list *FvhDefaultStackName = (chain_list *)0;
  static long        FvhDefaultStackSize = 0;
  static long        FvhStackPushSize    = 0;
  static long        FvhStackPopSize     = 0;
  static char       *FvhStackHeadName    = (char *)0;

  static char  FvhTreatMainProcess  = 0;
  static char  FvhTreatStackControl = 0;
  static char  FvhTreatStateControl = 0;

  static char  FvhWhenCondition   = 0;
  static char  FvhStarStateLocout = 0;


/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       FvhInitializeKeyword                  |
|                                                             |
\------------------------------------------------------------*/

void FvhInitializeKeyword()
{
  if ( FvhClockKeyword == (char *)0 )
  {
    FvhClockKeyword        = namealloc( "clock"         );
    FvhCurrentStateKeyword = namealloc( "current_state" );
    FvhReturnStateKeyword  = namealloc( "return_state"  );
    FvhNextStateKeyword    = namealloc( "next_state"    );
    FvhFirstStateKeyword   = namealloc( "first_state"   );
    FvhControlKeyword      = namealloc( "control"       );
    FvhOthersKeyword       = namealloc( "others"        );

    FvhStackControlKeyword[ FSM_CTRL_NOP  ] = namealloc( "nop"  );
    FvhStackControlKeyword[ FSM_CTRL_PUSH ] = namealloc( "push" );
    FvhStackControlKeyword[ FSM_CTRL_POP  ] = namealloc( "pop"  );
  }

  FvhClockName        = FvhClockKeyword;
  FvhCurrentStateName = FvhCurrentStateKeyword;
  FvhReturnStateName  = FvhReturnStateKeyword;
  FvhNextStateName    = FvhNextStateKeyword;
  FvhFirstStateName   = FvhFirstStateKeyword;
  FvhControlName      = FvhControlKeyword;

  FvhStackControlName[ FSM_CTRL_NOP  ] = FvhStackControlKeyword[ FSM_CTRL_NOP  ];
  FvhStackControlName[ FSM_CTRL_PUSH ] = FvhStackControlKeyword[ FSM_CTRL_PUSH ];
  FvhStackControlName[ FSM_CTRL_POP  ] = FvhStackControlKeyword[ FSM_CTRL_POP  ];


  FvhStateType   = (fbtyp_list *)0;
  FvhControlType = (fbtyp_list *)0;

  FvhStarStateLocout = 0;
  FvhWhenCondition   = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhGetControl                      |
|                                                             |
\------------------------------------------------------------*/

int FvhFbhGetControl( Name )

  char *Name;
{
  int Index;

  for ( Index = 0; Index < FSM_MAX_CTRL; Index++ )
  {
    if ( Name == FvhStackControlName[ Index ] ) return( Index );
  }

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhGetStack                        |
|                                                             |
\------------------------------------------------------------*/

int FvhFbhGetStack( Name )

  char *Name;
{
  chain_list *ScanChain;
  int         Index;

  Index = 0;

  for ( ScanChain  = FvhDefaultStackName;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    if ( ScanChain->DATA == (void *)Name )
    {
      return( Index );
    }

    Index = Index + 1;
  }

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhTreatPragma                     |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatPragma( FbhFigure, FsmFigure )

  fbfig_list  *FbhFigure;
  fsmfig_list *FsmFigure;
{
  fbpgm_list *ScanPragma;

  for ( ScanPragma  = FbhFigure->BEPGM; 
        ScanPragma != (fbpgm_list *)0;
        ScanPragma  = ScanPragma->NEXT )
  {
    if ( ScanPragma->NAME != (char *)0 )
    {
      if ( ScanPragma->TYPE == FvhCurrentStateKeyword )
      {
        FvhCurrentStateName = ScanPragma->NAME;
      }
      else 
      if ( ScanPragma->TYPE == FvhNextStateKeyword )
      {
        FvhNextStateName = ScanPragma->NAME;
      }
      else 
      if ( ScanPragma->TYPE == FvhFirstStateKeyword )
      {
        FvhFirstStateName = ScanPragma->NAME;
      }
      else 
      if ( ScanPragma->TYPE == FvhReturnStateKeyword )
      {
        FvhReturnStateName = ScanPragma->NAME;
      }
      else 
      if ( ScanPragma->TYPE == FvhControlKeyword )
      {
        FvhControlName = ScanPragma->NAME;
      }
      else 
      if ( ScanPragma->TYPE == FvhClockKeyword )
      {
        FvhClockName = ScanPragma->NAME;
      }
      else 
      if ( ScanPragma->TYPE == FvhStackControlKeyword[ FSM_CTRL_NOP ] )
      {
        FvhStackControlName[ FSM_CTRL_NOP ] = ScanPragma->NAME;
      }
      else 
      if ( ScanPragma->TYPE == FvhStackControlKeyword[ FSM_CTRL_PUSH ] )
      {
        FvhStackControlName[ FSM_CTRL_PUSH ] = ScanPragma->NAME;
      }
      else
      if ( ScanPragma->TYPE == FvhStackControlKeyword[ FSM_CTRL_POP ] )
      {
        FvhStackControlName[ FSM_CTRL_POP ] = ScanPragma->NAME;
      }
    }

    FsmFigure->PRAGMA = addptype( FsmFigure->PRAGMA,
                                  (long)ScanPragma->TYPE,
                                  ScanPragma->NAME );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhTreatAux                        |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatAux( FbhFigure, FsmFigure )

  fbfig_list *FbhFigure;
  fsmfig_list    *FsmFigure;
{
  fbaux_list *ScanAux;
  fbout_list *ScanOut;

  int         StateType;
  int         Index;
  int         MixedFsmRtl;

  if ( ( FbhFigure->BEREG != (fbreg_list *)0 ) ||
       ( FbhFigure->BEBUS != (fbbus_list *)0 ) ||
       ( FbhFigure->BEBUX != (fbbux_list *)0 ) )
  {
    MixedFsmRtl = 1;
  }
  else
  {
    MixedFsmRtl = 0;

    for ( ScanOut  = FbhFigure->BEOUT;
          ScanOut != (fbout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      if ( ScanOut->ABL != (ablexpr *)0 )
      {
        MixedFsmRtl = 1; break;
      }
    }
  }

  StateType = 0;

  for ( ScanAux  = FbhFigure->BEAUX; 
        ScanAux != (fbaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    if ( ScanAux->NAME == FvhCurrentStateName )
    {
      StateType    = ScanAux->TYPE;
      FvhStateType = ( FbhFigure->BETYP + StateType - 1 );

      ClearFbhAssignByFsm( ScanAux );
    }
    else
    if ( ScanAux->NAME == FvhControlName )
    {
      FvhControlType = ( FbhFigure->BETYP + ScanAux->TYPE - 1 );

      if ( FvhControlType->SIZE != FSM_MAX_CTRL )
      {
        fvherror( FVH_ERROR_CONTROL_TYPE, FbhFigure->NAME, FvhControlType->USER );
      }

      ClearFbhAssignByFsm( ScanAux );
    }
    else
    if ( ScanAux->NAME == FvhNextStateName )
    {
      ClearFbhAssignByFsm( ScanAux );
    }
    else
    {
      MixedFsmRtl = 1;
    }
  }

  if ( ( FvhStateType       == (fbtyp_list *)0 ) ||
       ( FvhStateType->SIZE == 0                      ) )
  {
    fvherror( FVH_ERROR_NO_STATE, FbhFigure->NAME, 0 );
  }

  FsmFigure->STAR_STATE = addfsmstate( FsmFigure, "*" );
  SetFsmStarState( FsmFigure->STAR_STATE );

  for ( Index = 0; Index < FvhStateType->SIZE; Index++ )
  {
    if ( FvhStateType->VALUE[ Index ] == FvhFirstStateName )
    {
      FsmFigure->FIRST_STATE = addfsmstate( FsmFigure, FvhFirstStateName );
      SetFsmFirstState( FsmFigure->FIRST_STATE );
    }
    else
    {
      addfsmstate( FsmFigure, FvhStateType->VALUE[ Index ] );
    }
  }

  if ( FvhDefaultStackName != (chain_list *)0 )
  {
    freechain( FvhDefaultStackName );
    FvhDefaultStackName = (chain_list *)0;
  }
 
  if ( FvhControlType != (fbtyp_list *)0 )
  {
    FsmFigure->STACK_SIZE = 1;

    FvhDefaultStackSize = 0;
    FvhStackPushSize    = 0;
    FvhStackPopSize     = 0;
    FvhDefaultStackName = (chain_list *)0;

    for ( ScanAux = FbhFigure->BEAUX;
          ScanAux != (fbaux_list *)0;
          ScanAux  = ScanAux->NEXT )
    {
      if ( ScanAux->TYPE == StateType )
      {
        if ( ( ScanAux->NAME != FvhCurrentStateName ) &&
             ( ScanAux->NAME != FvhNextStateName    ) &&
             ( ScanAux->NAME != FvhReturnStateName  ) )
        {
          FvhDefaultStackSize++;

          FvhDefaultStackName = addchain( FvhDefaultStackName, (void *)ScanAux->NAME );
        }
      }
    }

    if ( FvhDefaultStackSize == 0 )
    {
      fvherror( FVH_ERROR_STACK_SIZE_ZERO, FsmFigure->NAME, 0 );
    }
  }

  if ( MixedFsmRtl )
  {
    SetFsmFigMixedRtl( FsmFigure );
    FsmFigure->FIGURE = (void *)FbhFigure;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhTreatPort                       |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatPort( FbhFigure, FsmFigure )

  fbfig_list *FbhFigure;
  fsmfig_list    *FsmFigure;
{
  fbpor_list  *ScanPort;
  fbaux_list  *ScanAux;
  fbaux_list  *FreeAux;
  fbaux_list  *ListAux;
  fbaux_list **PrevAux;
  fbout_list  *ScanOut;
  fbout_list  *FreeOut;
  fbout_list  *ListOut;
  fbout_list **PrevOut;
  fbreg_list  *ScanReg;
  fbbux_list  *ScanBux;
  fbbus_list  *ScanBus;
  char         ClockOk;
  char         Direction;

  ClockOk = 0;

  if ( ! IsFsmFigMixedRtl( FsmFigure ) )
  {
    for ( ScanPort  = FbhFigure->BEPOR; 
          ScanPort != (fbpor_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      if ( ScanPort->DIRECTION != 'O' )
      {
        if ( ScanPort->NAME == FvhClockName )
        {
          ClockOk = 1;
        }
        else
        {
          addfsmin( FsmFigure, ScanPort->NAME );
        }
      }
  
      if ( ScanPort->DIRECTION != 'I' )
      {
        addfsmout( FsmFigure, ScanPort->NAME );
      }
  
      addfsmport( FsmFigure, ScanPort->NAME, ScanPort->DIRECTION, ScanPort->TYPE );
    }
  }
  else
  {
    for ( ScanPort  = FbhFigure->BEPOR; 
          ScanPort != (fbpor_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      if ( ScanPort->DIRECTION != 'O' )
      {
        if ( ScanPort->NAME == FvhClockName )
        {
          ClockOk = 1;
        }
        else
        {
          addfsmin( FsmFigure, ScanPort->NAME );
        }
      }
  
      addfsmport( FsmFigure, ScanPort->NAME, ScanPort->DIRECTION, ScanPort->TYPE );
    }

    for ( ScanAux  = FbhFigure->BEAUX;
          ScanAux != (fbaux_list *)0;
          ScanAux  = ScanAux->NEXT )
    {
      if ( ( ScanAux->NAME == FvhNextStateName    ) ||
           ( ScanAux->NAME == FvhCurrentStateName ) ||
           ( ScanAux->NAME == FvhReturnStateName  ) ) continue;

      if ( ScanAux->NAME == FvhClockName )
      {
        ClockOk = 1;
      }
      else
      {
        addfsmin( FsmFigure, ScanAux->NAME );
      }

      if ( IsFbhAssignByFsm( ScanAux ) )
      {
        addfsmout( FsmFigure, ScanAux->NAME );
        Direction = 'B';
      }
      else
      {
        Direction = 'I';
      }

      addfsmport( FsmFigure, ScanAux->NAME, 'B', ScanAux->TYPE );
    }

    for ( ScanOut  = FbhFigure->BEOUT;
          ScanOut != (fbout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      if ( IsFbhAssignByFsm( ScanOut ) )
      {
        addfsmout( FsmFigure, ScanOut->NAME );
      }
    }

    for ( ScanReg  = FbhFigure->BEREG;
          ScanReg != (fbreg_list *)0;
          ScanReg  = ScanReg->NEXT )
    {
      addfsmin( FsmFigure, ScanReg->NAME );
      addfsmport( FsmFigure, ScanReg->NAME, 'I', 'B' );
    }

    for ( ScanBus  = FbhFigure->BEBUS;
          ScanBus != (fbbus_list *)0;
          ScanBus  = ScanBus->NEXT )
    {
      addfsmin( FsmFigure, ScanBus->NAME );
      addfsmport( FsmFigure, ScanBus->NAME, 'I', ScanBus->TYPE );
    }

    for ( ScanBux  = FbhFigure->BEBUX;
          ScanBux != (fbbux_list *)0;
          ScanBux  = ScanBux->NEXT )
    {
      addfsmin( FsmFigure, ScanBus->NAME );
      addfsmport( FsmFigure, ScanBus->NAME, 'I', ScanBus->TYPE );
    }

/*
** Clean Up the FbhFigure !
*/
    PrevAux = &FbhFigure->BEAUX;
    ListAux = (fbaux_list *)0;

    ScanAux = FbhFigure->BEAUX;

    while ( ScanAux != (fbaux_list *)0 )
    {
      if ( ( ScanAux->ABL == (ablexpr *)0  ) &&
           ( ! IsFbhAssignByFsm( ScanAux ) ) )
      {
        FreeAux  = ScanAux;
        ScanAux  = ScanAux->NEXT;
        *PrevAux = ScanAux;

        FreeAux->NEXT = ListAux;
        ListAux       = FreeAux;
      }
      else
      {
        PrevAux = &ScanAux->NEXT;
        ScanAux  = ScanAux->NEXT;
      }
    }

    fbh_frefbaux( ListAux );

    PrevOut = &FbhFigure->BEOUT;
    ListOut = (fbout_list *)0;

    ScanOut = FbhFigure->BEOUT;

    while ( ScanOut != (fbout_list *)0 )
    {
      if ( ScanOut->ABL == (ablexpr *)0 )
      {
        FreeOut  = ScanOut;
        ScanOut  = ScanOut->NEXT;
        *PrevOut = ScanOut;

        FreeOut->NEXT = ListOut;
        ListOut       = FreeOut;
      }
      else
      {
        PrevOut = &ScanOut->NEXT;
        ScanOut  = ScanOut->NEXT;
      }
    }

    fbh_frefbout( ListOut );
  }

  if ( ClockOk != 1 )
  {
    fvherror( FVH_ERROR_MISSING_CLOCK_PORT, FsmFigure->NAME, 0 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhTreatIf                         |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatIf( FbhFigure, FsmFigure,   ScanIfs,
                    StateFrom, StateTo,     Return,
                    Control,   Equation )

  fbfig_list     *FbhFigure;
  fsmfig_list    *FsmFigure;
  fbifs_list     *ScanIfs;
  fsmstate_list  *StateFrom;
  fsmstate_list  *StateTo;
  fsmstate_list  *Return;
  int             Control;
  ablexpr        *Equation;
{
  ablexpr        *NewEquation;

/*
**  IF ( Condition )
*/
  if ( isablnameinexpr( ScanIfs->CND, ABL_ATOM_NAME_DC ) )
  {
    fvherror( FVH_ERROR_ILLEGAL_IF_CONDITION, (char *)0, ScanIfs->USER );
  }

  NewEquation = dupablexpr( ScanIfs->CND );

  if ( Equation != (ablexpr    *)0 )
  {
    NewEquation = optimablbinexpr( ABL_AND, dupablexpr( Equation ), NewEquation );
  }
/*
**  THEN 
*/
  FvhFbhTreatInstruction( FbhFigure, FsmFigure,
                          ScanIfs->CNDTRUE,
                          StateFrom, StateTo, Return,
                          Control, NewEquation );
   
  delablexpr( NewEquation );

  NewEquation = optimablnotexpr( dupablexpr( ScanIfs->CND ) );

  if ( Equation != (ablexpr    *)0 )
  {
    NewEquation = optimablbinexpr( ABL_AND, dupablexpr( Equation ), NewEquation );
  }
/*
**  ELSE 
*/
  FvhFbhTreatInstruction( FbhFigure, FsmFigure,
                          ScanIfs->CNDFALSE, 
                          StateFrom, StateTo, Return, 
                          Control, NewEquation );

  delablexpr( NewEquation );
}

/*------------------------------------------------------------\
|                                                             |
|                         FvhFbhTreatAsg                      |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatAsg( FbhFigure, FsmFigure,   ScanAsg,
                     PStateFrom, PStateTo,   PReturn,
                     PControl,  Equation )

  fbfig_list      *FbhFigure;
  fsmfig_list     *FsmFigure;
  fbasg_list      *ScanAsg;
  fsmstate_list  **PStateFrom;
  fsmstate_list  **PStateTo;
  fsmstate_list  **PReturn;
  int             *PControl;
  ablexpr         *Equation;
{
  fbtyp_list      *ScanType;
  ablexpr         *NewEquation;
  ablexpr         *NewEquationDc;
  fsmout_list     *Output;
  int              Stack;

  ScanType = (fbtyp_list *)(FbhFigure->BETYP + ScanAsg->TYPE - 1);

  Output = searchfsmout( FsmFigure, ScanAsg->NAME );

  if ( Output != (fsmout_list *)0 )
  {
    if ( ! FvhTreatMainProcess )
    {
      fvherror( FVH_ERROR_IN_CLOCK_PROCESS, ScanAsg->NAME, ScanAsg->USER );
    }

    NewEquationDc = (ablexpr    *)0;
    NewEquation   = (ablexpr    *)0;
/*
**  O(i) <= Equation
*/
    if ( Equation != (ablexpr    *)0 )
    {
      if ( ! ABL_ATOM( ScanAsg->ABL ) )
      {
        if ( isablnameinexpr( ScanAsg->ABL, ABL_ATOM_NAME_DC ) )
        {
          fvherror( FVH_ERROR_ILLEGAL_OUT_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
        }

        NewEquation = optimablbinexpr( ABL_AND, 
                                        dupablexpr( ScanAsg->ABL ), 
                                        dupablexpr( Equation ) );
      }
      else
      {
        if ( ABL_ATOM_VALUE( ScanAsg->ABL ) == ABL_ATOM_NAME_ONE )
        {
          NewEquation = dupablexpr( Equation );
        }
        else
        if ( ABL_ATOM_VALUE( ScanAsg->ABL ) == ABL_ATOM_NAME_ZERO )
        {
          NewEquation = createablatomzero();
        }
        else
        if ( ABL_ATOM_VALUE( ScanAsg->ABL ) == ABL_ATOM_NAME_DC )
        {
          NewEquationDc = dupablexpr( Equation );
        }
        else
        {
          NewEquation = optimablbinexpr( ABL_AND, 
                                          dupablexpr( ScanAsg->ABL ),
                                          dupablexpr( Equation ) );
        }
      }
    }
    else
    {
      if ( *PStateFrom == (fsmstate_list *)0 )
      {
        fvherror( FVH_ERROR_ILLEGAL_OUT_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
      }

      if ( ! ABL_ATOM( ScanAsg->ABL ) )
      {
        if ( isablnameinexpr( ScanAsg->ABL, ABL_ATOM_NAME_DC ) )
        {
          fvherror( FVH_ERROR_ILLEGAL_OUT_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
        }

        NewEquation = dupablexpr( ScanAsg->ABL );
      }
      else
      {
        if ( ABL_ATOM_VALUE( ScanAsg->ABL ) == ABL_ATOM_NAME_ZERO )
        {
          NewEquation = createablatomzero();
        }
        else
        if ( ABL_ATOM_VALUE( ScanAsg->ABL ) == ABL_ATOM_NAME_DC )
        {
          NewEquationDc = createablatomone();
        }
        else
        {
          NewEquation = dupablexpr( ScanAsg->ABL );
        }
      }
    }

    if ( ( NewEquation   != (ablexpr    *)0 ) ||
         ( NewEquationDc != (ablexpr    *)0 ) )
    {
      if ( *PStateFrom == (fsmstate_list *)0 )
      {
        *PStateFrom = FsmFigure->STAR_STATE;
      }

      if ( *PStateFrom == FsmFigure->STAR_STATE )
      {
        FvhStarStateLocout = 1;
      }

      addfsmlocout( *PStateFrom, Output, NewEquation, NewEquationDc );
    }
    else
    {
      if ( ( *PStateFrom == (fsmstate_list *)0    ) ||
           ( *PStateFrom == FsmFigure->STAR_STATE ) )
      {
        FvhStarStateLocout = 1;
      }
    }
  }
  else
  if ( ScanType == FvhStateType )
  {
    if ( ABL_ATOM( ScanAsg->ABL ) )
    {
      if ( ScanAsg->NAME == FvhNextStateName )
      {
        if ( ! FvhTreatMainProcess )
        {
          fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhNextStateName, ScanAsg->USER );
        }

        *PStateTo = searchfsmstate( FsmFigure, ABL_ATOM_VALUE( ScanAsg->ABL ) );

        if ( *PStateTo == (fsmstate_list *)0 )
        {
          if ( FsmFigure->STACK_SIZE )
          {
            Stack = FvhFbhGetStack( ABL_ATOM_VALUE( ScanAsg->ABL ) );

            if ( Stack != -1 )
            {
/*
**  NEXT_STATE <= STACK(0)
*/
              if ( ( FvhStackHeadName != (char *)0                   ) &&
                   ( ABL_ATOM_VALUE( ScanAsg->ABL ) != FvhStackHeadName ) )
              {
                fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN, 
                          ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->USER );
              }

              *PStateTo        = FsmFigure->STAR_STATE;
              FvhStackHeadName = ABL_ATOM_VALUE( ScanAsg->ABL );
            }
            else
            {
              fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN,
                        ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->USER );
            }
          }
          else
          {
            fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
          }
        }
        else
        {
/*
**  NEXT_STATE <= E(i)
*/
          if ( Equation == (ablexpr    *)0 )
          {
            NewEquation = createablatomone();
          }
          else
          {
            NewEquation = dupablexpr( Equation );
          }

          if ( *PStateFrom == (fsmstate_list *)0 )
          {
            *PStateFrom = FsmFigure->STAR_STATE;
          }
        
          addfsmtrans( FsmFigure, *PStateFrom, *PStateTo, NewEquation );
        }
      }
      else
      if ( ScanAsg->NAME == FvhCurrentStateName )
      {
        if ( ( FvhTreatMainProcess  ) ||
             ( FvhTreatStateControl ) )
        {
          fvherror( FVH_ERROR_IN_MAIN_PROCESS, ScanAsg->NAME, ScanAsg->USER );
        }
/*
**  CURRENT_STATE <= NEXT_STATE
*/
        if ( ABL_ATOM_VALUE( ScanAsg->ABL ) != FvhNextStateName )
        {
          fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
        }
        else
        {
          if ( Equation == (ablexpr    *)0 )
          {
            fvherror( FVH_ERROR_NO_CONTROL_CONDITION, FsmFigure->NAME, ScanAsg->USER );
          }

          FsmFigure->CLOCK_ABL = dupablexpr( Equation );
          FvhTreatStateControl = 1;
        }
      }
      else
      if ( FsmFigure->STACK_SIZE )
      {
        if ( ScanAsg->NAME == FvhReturnStateName )
        {
          if ( ! FvhTreatMainProcess )
          {
            fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhReturnStateName, ScanAsg->USER );
          }
/*
**  RETURN_STATE <= E(i)
*/
          if ( ( *PReturn == (fsmstate_list *)0 ) &&
               (  FvhTreatMainProcess           ) )
          {
            *PReturn = searchfsmstate( FsmFigure, ABL_ATOM_VALUE( ScanAsg->ABL ) );

            if ( *PReturn == (fsmstate_list *)0 )
            {
              fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
            }
          }
          else
          {
            fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
          }
        }
        else
        {
          if ( FvhTreatMainProcess )
          {
            fvherror( FVH_ERROR_IN_MAIN_PROCESS, ScanAsg->NAME, ScanAsg->USER );
          }

          Stack = FvhFbhGetStack( ScanAsg->NAME );

          if ( Stack == -1 )
          {
            fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
          }

          if ( ABL_ATOM_VALUE( ScanAsg->ABL ) == FvhReturnStateName )
          {
/*
**  STACK(0) <= RETURN_STATE
*/
            if ( ( *PControl   != FSM_CTRL_PUSH          ) ||
                 ( ( FvhStackHeadName != (char *)0  ) &&
                   ( FvhStackHeadName != ScanAsg->NAME ) ) )
            {
              fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
            }

            FvhStackPushSize = FvhStackPushSize + 1;

            FvhStackHeadName = ScanAsg->NAME;
          }
          else
          {
            Stack = FvhFbhGetStack( ABL_ATOM_VALUE( ScanAsg->ABL ) );

            if ( Stack != -1 )
            {
              if ( ( FvhStackHeadName != (char *)0  ) &&
                   ( ScanAsg->NAME == FvhStackHeadName ) )
              {
/*
**  STACK(0) <= STACK(j)
*/
                if ( *PControl != FSM_CTRL_POP )
                {
                  fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN,
                            ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->USER );
                }
              }
/*
**  STACK(i) <= STACK(j)
*/
              if ( *PControl == FSM_CTRL_PUSH )
              {
                FvhStackPushSize = FvhStackPushSize + 1;
              }
              else
              if ( *PControl == FSM_CTRL_POP )
              {
                FvhStackPopSize = FvhStackPopSize + 1;
              }
              else
              {
                fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
              }
            }
            else
            {
              fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN,
                        ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->USER );
            }
          }
        }
      }
      else
      {
        fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->USER );
    }
  }
  else
  if ( ScanType == FvhControlType )
  {
    if ( ABL_ATOM( ScanAsg->ABL ) )
    {
      if ( ScanAsg->NAME == FvhControlName )
      {
/*
**  CTRL <= PUSH | POP | NOP
*/
        if ( *PControl == -1 )
        {
          if ( ! FvhTreatMainProcess )
          {
            fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhControlName, ScanAsg->USER );
          }

          *PControl = FvhFbhGetControl( ABL_ATOM_VALUE( ScanAsg->ABL ) );
        }
        else
        {
          fvherror( FVH_ERROR_ILLEGAL_ASSIGNATION, ScanAsg->NAME, ScanAsg->USER );
        }
      }
      else
      {
        fvherror( FVH_ERROR_ILLEGAL_ASSIGNATION, ScanAsg->NAME, ScanAsg->USER );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_CONTROL_ASSIGN, 
                ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->USER );
    }
  }
  else
  {
    fvherror( FVH_ERROR_ILLEGAL_ASSIGNATION, ScanAsg->NAME, ScanAsg->USER );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      FvhFbhTreatWhen                        |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatWhen( FbhFigure,  FsmFigure,   ScanCase,
                      PStateFrom, StateTo,     Return,
                      PControl,   Equation )

  fbfig_list  *FbhFigure;
  fsmfig_list     *FsmFigure;
  fbcas_list  *ScanCase;
  fsmstate_list  **PStateFrom;
  fsmstate_list   *StateTo;
  fsmstate_list   *Return;
  int             *PControl;
  ablexpr         *Equation;
{
  fbtyp_list *ScanType;
  int         Index;
  char       *ChoiceValue;

  if ( FvhTreatMainProcess )
  {
    if ( Equation != (ablexpr    *)0 )
    {
      FvhWhenCondition = 1;
    }
  }

/*
**  CASE  CURRENT_STATE | CTRL
*/
  ScanType = (fbtyp_list *)(FbhFigure->BETYP + ScanCase->TYPE - 1);

  if ( ScanType == FvhStateType )
  {
    if ( ( ABL_ATOM( ScanCase->ABL )                              ) &&
         ( ABL_ATOM_VALUE( ScanCase->ABL ) == FvhCurrentStateName ) )
    {
      if ( *PStateFrom == (fsmstate_list *)0 )
      {
        if ( ! FvhTreatMainProcess )
        {
          fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhCurrentStateName, ScanCase->USER );
        }
/*
**  WHEN E(i) =>
*/
        for ( Index = 0; Index < ScanCase->SIZE; Index++ )
        {
          ChoiceValue = ScanCase->CHOICE[ Index ].VALUE;

          if ( ChoiceValue == FvhOthersKeyword ) continue;

          *PStateFrom = searchfsmstate( FsmFigure, ChoiceValue );

          FvhFbhTreatInstruction( FbhFigure, FsmFigure, 
                                  ScanCase->CHOICE[ Index ].INSTRUCTION,
                                 *PStateFrom, StateTo, Return,
                                 *PControl, Equation );
        }
      }
      else
      {
        fvherror( FVH_ERROR_ILLEGAL_CASE, ScanType->NAME, ScanCase->USER );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_STATE, 
                ABL_ATOM_VALUE( ScanCase->ABL ), ScanCase->USER );
    }
  }
  else
  if ( ScanType == FvhControlType )
  {
    if ( ( ABL_ATOM( ScanCase->ABL )                         ) &&
         ( ABL_ATOM_VALUE( ScanCase->ABL ) == FvhControlName ) )
    {
      if ( *PControl == -1 )
      {
        if ( FvhTreatMainProcess )
        {
          fvherror( FVH_ERROR_IN_MAIN_PROCESS, FvhControlName, ScanCase->USER );
        }
/*
**  WHEN CTRL =>
*/
        for ( Index = 0; Index < ScanCase->SIZE; Index++ )
        {
          ChoiceValue = ScanCase->CHOICE[ Index ].VALUE;

          if ( ChoiceValue == FvhOthersKeyword ) continue;

          *PControl = FvhFbhGetControl( ChoiceValue );

          FvhTreatStackControl = 1;

          FvhFbhTreatInstruction( FbhFigure, FsmFigure,
                                  ScanCase->CHOICE[ Index ].INSTRUCTION,
                                 *PStateFrom, StateTo, Return, 
                                 *PControl, Equation );
        }
      }
      else
      {
        fvherror( FVH_ERROR_ILLEGAL_CASE, ScanType->NAME, ScanCase->USER );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_CONTROL, 
                ABL_ATOM_VALUE( ScanCase->ABL ), ScanCase->USER );
    }
  }
  else
  {
    fvherror( FVH_ERROR_ILLEGAL_CASE, ScanType->NAME, ScanCase->USER );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhTreatInstruction                |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatInstruction( FbhFigure, FsmFigure,   Instruct,
                             StateFrom, StateTo,     Return,
                             Control,   Equation )

  fbfig_list     *FbhFigure;
  fsmfig_list    *FsmFigure;
  ptype_list     *Instruct;
  fsmstate_list  *StateFrom;
  fsmstate_list  *StateTo;
  fsmstate_list  *Return;
  int             Control;
  ablexpr        *Equation;
{
  ptype_list     *ScanIns;
  ablexpr        *NewEquation;

  for ( ScanIns  = Instruct;
        ScanIns != (ptype_list *)0;
        ScanIns  = ScanIns->NEXT )
  {
    switch ( ScanIns->TYPE )
    {
      case FBH_BECAS : 

        FvhFbhTreatWhen( FbhFigure, FsmFigure, ScanIns->DATA,
                         &StateFrom, StateTo, Return,
                         &Control, Equation );

        StateFrom = (fsmstate_list *)0;

      break;

      case FBH_BEIFS : 

        FvhFbhTreatIf( FbhFigure, FsmFigure, ScanIns->DATA,
                       StateFrom, StateTo, Return,
                       Control, Equation );
      break;

      case FBH_BEASG : 

        FvhFbhTreatAsg( FbhFigure, FsmFigure, ScanIns->DATA,
                        &StateFrom, &StateTo, &Return,
                        &Control, Equation );
      break;

      default : 

        fvherror( FVH_ERROR_ILLEGAL_INSTRUCTION, 
                  (char *)( ScanIns->TYPE ), 0 );

      break;
    }
  }

  if ( ( FvhTreatMainProcess                 ) &&
       ( ( Control != -1                   ) ||
         ( Return  != (fsmstate_list *)0   ) ) )
  {
    switch ( Control )
    {
      case FSM_CTRL_NOP  :
      case FSM_CTRL_POP  :

        if ( Return != (fsmstate_list *)0 )
        {
          fvherror( FVH_ERROR_ILLEGAL_ASSIGNATION,
                    FvhReturnStateName, 0 );
        }

      break;

      case FSM_CTRL_PUSH :

        if ( Return == (fsmstate_list *)0 )
        {
          fvherror( FVH_ERROR_ILLEGAL_CONTROL_ASSIGN, 
                    FSM_CTRL_NAME[ Control ], 0 );
        }

      break;
    }

    if ( Equation == (ablexpr    *)0 )
    {
      NewEquation = createablatomone();
    }
    else
    {
      NewEquation = dupablexpr( Equation );
    }
 
    addfsmstack( FsmFigure, Control, StateFrom, Return, NewEquation );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhTreatProcess                    |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatProcess( FbhFigure, FsmFigure )

  fbfig_list  *FbhFigure;
  fsmfig_list *FsmFigure;
{
  fbpcs_list *ScanProc;
  chain_list *ScanSens;
  fbpcs_list *MainProc;
  fbpcs_list *ClockProc;
  ptype_list *ScanInst;
  fbifs_list *ScanIf;
  char        SensCurrent;

  MainProc = (fbpcs_list *)0;

  FvhStackHeadName     = (char *)0;
  FvhTreatStackControl = 0;
  FvhTreatStateControl = 0;
  FvhTreatMainProcess  = 0;
/*
** First Verify there are two VHDL process
*/
  ScanProc = FbhFigure->BEPCS;

  if ( ( ScanProc             == (fbpcs_list *)0 ) ||
       ( ScanProc->NEXT       == (fbpcs_list *)0 ) ||
       ( ScanProc->NEXT->NEXT != (fbpcs_list *)0 ) )
  {
    fvherror( FVH_ERROR_TWO_PROCESS, FsmFigure->NAME, 0 );
  }

  ClockProc = (fbpcs_list *)0;
/*
** Find the clock process 
*/
  for ( ScanProc  = FbhFigure->BEPCS;
        ScanProc != (fbpcs_list *)0;
        ScanProc  = ScanProc->NEXT )
  {
    ScanSens = ScanProc->SENSITIVITY;

    if ( ScanSens == (chain_list *)0 )
    {
      fvherror( FVH_ERROR_PROCESS_SENSITIVITY, ScanProc->LABEL, ScanProc->USER );
    }

    if ( ( ScanSens->NEXT == (chain_list *)0      ) &&
         ( ScanSens->DATA == (void *)FvhClockName ) )
    {
      FsmFigure->CLOCK = FvhClockName;

      ClockProc = ScanProc; break;
    }
  }

  if ( ClockProc == (fbpcs_list *)0 )
  {
    fvherror( FVH_ERROR_CLOCK_PROCESS, FsmFigure->NAME, 0 );
  }

  if ( FbhFigure->BEPCS == ClockProc )
  {
    MainProc = ClockProc->NEXT;
  }
  else
  {
    MainProc = FbhFigure->BEPCS;
  }
/*
** Verify the sensitivity list of the main process (to be done better)
*/
  SensCurrent = 0;

  for ( ScanSens  = MainProc->SENSITIVITY;
        ScanSens != (chain_list *)0;
        ScanSens  = ScanSens->NEXT )
  {
    if ( ScanSens->DATA == (void *)FvhCurrentStateName )
    {
      SensCurrent = 1;
    }
  }

  if ( ! SensCurrent )
  {
    fvherror( FVH_ERROR_MISSING_SIGNAL, FvhCurrentStateName, ScanProc->USER );
  }
/*
** Check the body of the clock process
*/
  ScanInst = ClockProc->INSTRUCTION;

  if ( ( ScanInst       == (ptype_list *)0 ) ||
       ( ScanInst->NEXT != (ptype_list *)0 ) ||
       ( ScanInst->TYPE != FBH_BEIFS       ) )
  {
    fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, ClockProc->LABEL, ClockProc->USER );
  }

  ScanIf = (fbifs_list *)ScanInst->DATA;

  if ( ( ScanIf->CNDTRUE  == (ptype_list *)0 ) ||
       ( ScanIf->CNDFALSE != (ptype_list *)0 ) )
  {
    fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, ClockProc->LABEL, ClockProc->USER );
  }
/*
** Check the body of the main process
*/
  ScanInst = MainProc->INSTRUCTION;

  if ( ScanInst == (ptype_list *)0 )
  {
    fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, MainProc->LABEL, MainProc->USER );
  }

  for ( ScanInst  = MainProc->INSTRUCTION;
        ScanInst != (ptype_list *)0;
        ScanInst  = ScanInst->NEXT )
  {
    if ( ( ScanInst->TYPE != FBH_BEIFS ) &&
         ( ScanInst->TYPE != FBH_BECAS ) )
    {
      fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, MainProc->LABEL, MainProc->USER );
    }
  }

  for ( ScanProc  = FbhFigure->BEPCS;
        ScanProc != (fbpcs_list *)0;
        ScanProc  = ScanProc->NEXT )
  {
    if ( ScanProc == MainProc )
    {
      FvhTreatMainProcess = 1;
    }

    FvhFbhTreatInstruction( FbhFigure, FsmFigure, ScanProc->INSTRUCTION,
                            (fsmstate_list *)0, (fsmstate_list *)0,
                            (fsmstate_list *)0, -1, (ablexpr    *)0 );
  }

  if ( ! FvhTreatStateControl )
  {
    fvherror( FVH_ERROR_MISSING_STATE_CONTROL, FsmFigure->NAME, 0 );
  }

  if ( FsmFigure->STACK_SIZE )
  {
    if ( ! FvhTreatStackControl )
    {
      fvherror( FVH_ERROR_MISSING_STACK_CONTROL, FsmFigure->NAME, 0 );
    }

    if ( ( FvhStackPushSize > FvhDefaultStackSize  ) ||
         ( FvhStackPopSize  > FvhDefaultStackSize  ) ||
         ( FvhStackPopSize != FvhStackPushSize - 1 ) )
    {
      fvherror( FVH_ERROR_BAD_STACK_CONTROL, FsmFigure->NAME, 0 );
    }

    FsmFigure->STACK_SIZE = FvhStackPushSize;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        FvhFbhPostTreat                      |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhPostTreat( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list *StarState;
  chain_list    *StarChain;
  fsmtrans_list *ScanTrans;
  ablexpr       *Equation;

  if ( ! FvhWhenCondition )
  {
    if ( FvhStarStateLocout )
    {
      fvherror( FVH_ERROR_ILLEGAL_DEFAULT_ASSIGN, (char *)0, 0 );
    }

    StarState = FsmFigure->STAR_STATE;
    Equation  = createabloper( ABL_OR );

    for ( StarChain  = StarState->FROM;
          StarChain != (chain_list *)0;
          StarChain  = StarChain->NEXT )
    {
      ScanTrans = (fsmtrans_list *)StarChain->DATA;
      addablhexpr( Equation, dupablexpr( ScanTrans->ABL ) );
    }

    if ( ABL_CDR( Equation ) == (ablexpr    *)0 )
    {
      delablexpr( Equation );

      return;
    }
    else
    if ( ABL_CDDR( Equation ) == (ablexpr    *)0 )
    {
      StarChain = ABL_CADR( Equation );
      ABL_CADR( Equation ) = (ablexpr    *)0;
      freeablexpr( Equation );
      Equation = StarChain;
    }

    Equation = optimablnotexpr( Equation );

    for ( ScanTrans  = FsmFigure->TRANS;
          ScanTrans != (fsmtrans_list *)0;
          ScanTrans  = ScanTrans->NEXT )
    {
      if ( IsFsmStarTrans( ScanTrans ) ) continue;

      ScanTrans->ABL = optimablbinexpr( ABL_AND, 
                                         ScanTrans->ABL,
                                         dupablexpr( Equation ) );
    }

    delablexpr( Equation );
  }
}
  
/*------------------------------------------------------------\
|                                                             |
|                          FvhFbh2Fsm                         |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *FvhFbh2Fsm( FbhFigure, FsmFigure )

  fbfig_list  *FbhFigure;
  fsmfig_list *FsmFigure;
{
  FvhInitializeKeyword();

  FsmFigure->NAME = FbhFigure->NAME;

  FvhFbhTreatPragma( FbhFigure, FsmFigure );
  FvhFbhTreatAux( FbhFigure, FsmFigure );
  FvhFbhTreatPort( FbhFigure, FsmFigure );
  FvhFbhTreatProcess( FbhFigure, FsmFigure );

  FvhFbhPostTreat( FsmFigure );

  return( FsmFigure );
}
