/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
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

/* # define DEBUG  */

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

  static fbpcsinfo   *FvhProcInfo    = (fbpcsinfo   *)0;
  static fvhfig_list *FvhHeadFigList = (fvhfig_list *)0;


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
|                       FvhFbhAddFigList                      |
|                                                             |
\------------------------------------------------------------*/

static fvhfig_list *FvhFbhAddFigList( Name ) 

  char *Name;
{
  fvhfig_list *ScanFigList;

  ScanFigList = (fvhfig_list *)autallocheap( sizeof( fvhfig_list ) );

  ScanFigList->NAME = Name;
  ScanFigList->NEXT = FvhHeadFigList;
  FvhHeadFigList    = ScanFigList;

  ScanFigList->CLOCK         = FvhClockKeyword;
  ScanFigList->CURRENT_STATE = FvhCurrentStateKeyword;
  ScanFigList->NEXT_STATE    = FvhNextStateKeyword;
  ScanFigList->FIRST_STATE   = FvhFirstStateKeyword;
  ScanFigList->RETURN_STATE  = FvhReturnStateKeyword;
  ScanFigList->CONTROL       = FvhControlKeyword;

  ScanFigList->STACK_CONTROL[ FSM_CTRL_NOP  ] = FvhStackControlKeyword[ FSM_CTRL_NOP  ];
  ScanFigList->STACK_CONTROL[ FSM_CTRL_PUSH ] = FvhStackControlKeyword[ FSM_CTRL_PUSH ];
  ScanFigList->STACK_CONTROL[ FSM_CTRL_POP  ] = FvhStackControlKeyword[ FSM_CTRL_POP  ];

  return( ScanFigList );
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhFreeFigList                     |
|                                                             |
\------------------------------------------------------------*/

static void FvhFbhFreeFigList()
{
  fvhfig_list *DelFigList;

  while ( FvhHeadFigList != (fvhfig_list *)0 )
  {
    DelFigList     = FvhHeadFigList;
    FvhHeadFigList = FvhHeadFigList->NEXT;

    freechain( DelFigList->DEFAULT_STACK_NAME );

    autfreeheap( DelFigList, sizeof( fvhfig_list ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       FvhFbhTreatPragma                     |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatPragma( FbhFigure, FsmFigure, NumberProc )

  fbfig_list  *FbhFigure;
  fsmfig_list *FsmFigure;
  int          NumberProc;
{
  fbpgm_list  *ScanPragma;
  fvhfig_list *ScanFigList;
  char        *Value;
  char        *NewValue;
  int          First;
  int          NumberValue;

  FvhHeadFigList = (fvhfig_list *)0;

  if ( NumberProc > 2 )
  { 
    SetFsmFigMulti( FsmFigure );

    NumberValue = 0; 

    for ( ScanPragma  = FbhFigure->BEPGM;
          ScanPragma != (fbpgm_list *)0;
          ScanPragma  = ScanPragma->NEXT )
    {
      Value = ScanPragma->VALUE;

      if ( Value == (void *)0 )
      {
        fvherror( FVH_ERROR_PRAGMA_SPECIFICATION, FbhFigure->NAME, 0 );
      }

      for ( ScanFigList  = FvhHeadFigList;
            ScanFigList != (fvhfig_list *)0;
            ScanFigList  = ScanFigList->NEXT )
      {
        if ( ScanFigList->NAME == Value ) break;
      }

      if ( ScanFigList == (fvhfig_list *)0 )
      {
        ScanFigList = FvhFbhAddFigList( Value );
        NumberValue++;
      }
    }

    if ( ( NumberValue * 2 ) != NumberProc )
    {
      fvherror( FVH_ERROR_PRAGMA_SPECIFICATION, FbhFigure->NAME, 0 );
    }
  }
  else
  {
    Value = (char *)0;
    First = 1;

    for ( ScanPragma  = FbhFigure->BEPGM;
          ScanPragma != (fbpgm_list *)0;
          ScanPragma  = ScanPragma->NEXT )
    {
      if ( First )
      {
        Value    = (char *)ScanPragma->VALUE;
        First    = 0;
      }

      NewValue = (char *)ScanPragma->VALUE;

      if ( Value != NewValue )
      {
        fvherror( FVH_ERROR_PRAGMA_SPECIFICATION, FbhFigure->NAME, 0 );
      }
    }

    if ( Value == (char *)0 )
    {
      Value = FbhFigure->NAME;
    }

    FvhFbhAddFigList( Value );

    for ( ScanPragma  = FbhFigure->BEPGM;
          ScanPragma != (fbpgm_list *)0;
          ScanPragma  = ScanPragma->NEXT )
    {
      ScanPragma->VALUE = (void *)Value;
    }
  }

  for ( ScanPragma  = FbhFigure->BEPGM; 
        ScanPragma != (fbpgm_list *)0;
        ScanPragma  = ScanPragma->NEXT )
  {
    Value = (char *)ScanPragma->VALUE;

    for ( ScanFigList  = FvhHeadFigList;
          ScanFigList != (fvhfig_list *)0;
          ScanFigList  = ScanFigList->NEXT )
    {
      if ( ScanFigList->NAME == Value ) break;
    }

    if ( ScanPragma->TYPE == FvhCurrentStateKeyword )
    {
      ScanFigList->CURRENT_STATE = ScanPragma->NAME;
    }
    else 
    if ( ScanPragma->TYPE == FvhNextStateKeyword )
    {
      ScanFigList->NEXT_STATE = ScanPragma->NAME;
    }
    else 
    if ( ScanPragma->TYPE == FvhFirstStateKeyword )
    {
      ScanFigList->FIRST_STATE = ScanPragma->NAME;
    }
    else 
    if ( ScanPragma->TYPE == FvhReturnStateKeyword )
    {
      ScanFigList->RETURN_STATE = ScanPragma->NAME;
    }
    else 
    if ( ScanPragma->TYPE == FvhControlKeyword )
    {
      ScanFigList->CONTROL = ScanPragma->NAME;
    }
    else 
    if ( ScanPragma->TYPE == FvhClockKeyword )
    {
      ScanFigList->CLOCK = ScanPragma->NAME;
    }
    else 
    if ( ScanPragma->TYPE == FvhStackControlKeyword[ FSM_CTRL_NOP ] )
    {
      ScanFigList->STACK_CONTROL[ FSM_CTRL_NOP ] = ScanPragma->NAME;
    }
    else 
    if ( ScanPragma->TYPE == FvhStackControlKeyword[ FSM_CTRL_PUSH ] )
    {
      ScanFigList->STACK_CONTROL[ FSM_CTRL_PUSH ] = ScanPragma->NAME;
    }
    else
    if ( ScanPragma->TYPE == FvhStackControlKeyword[ FSM_CTRL_POP ] )
    {
      ScanFigList->STACK_CONTROL[ FSM_CTRL_POP ] = ScanPragma->NAME;
    }

    addfsmpragma( FsmFigure, ScanPragma->TYPE, ScanPragma->NAME, ScanPragma->VALUE );
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
  authelem     *Element;
  fbpcs_list   *Process;
  fbpcsinfo    *ProcInfo;
  fsmfig_list  *Figure;
  fvhfig_list  *ScanFigList;
  fbaux_list   *ScanAux;
  fbout_list   *ScanOut;
  fbtyp_list   *Type;

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

  for ( ScanAux  = FbhFigure->BEAUX; 
        ScanAux != (fbaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    for ( ScanFigList  = FvhHeadFigList;
          ScanFigList != (fvhfig_list *)0;
          ScanFigList  = ScanFigList->NEXT )
    {
      if ( ScanAux->NAME == ScanFigList->CURRENT_STATE )
      {
        ScanFigList->STATE_TYPE = ( FbhFigure->BETYP + ScanAux->TYPE - 1 );
        ClearFbhAssignByFsm( ScanAux );

        break;
      }
      else
      if ( ScanAux->NAME == ScanFigList->CONTROL )
      {
        Type = ( FbhFigure->BETYP + ScanAux->TYPE - 1 );
        ScanFigList->CONTROL_TYPE = Type;

        if ( Type->SIZE != FSM_MAX_CTRL )
        {
          fvherror( FVH_ERROR_CONTROL_TYPE, FbhFigure->NAME, Type->LINE_NUM );
        }

        ClearFbhAssignByFsm( ScanAux );

        break;
      }
    }
  }

  for ( ScanAux  = FbhFigure->BEAUX; 
        ScanAux != (fbaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    for ( ScanFigList  = FvhHeadFigList;
          ScanFigList != (fvhfig_list *)0;
          ScanFigList  = ScanFigList->NEXT )
    {
      Type = ( FbhFigure->BETYP + ScanAux->TYPE - 1 );

      if ( ( Type == ScanFigList->CONTROL_TYPE ) ||
           ( Type == ScanFigList->STATE_TYPE   ) )
      {
        ClearFbhAssignByFsm( ScanAux );

        break;
      }
    }

    if ( ScanFigList == (fvhfig_list *)0 )
    {
      MixedFsmRtl = 1;
    }
  }

  for ( ScanFigList  = FvhHeadFigList;
        ScanFigList != (fvhfig_list *)0;
        ScanFigList  = ScanFigList->NEXT )
  {
    if ( ( ScanFigList->STATE_TYPE       == (fbtyp_list *)0 ) ||
         ( ScanFigList->STATE_TYPE->SIZE == 0               ) )
    {
      fvherror( FVH_ERROR_NO_STATE, FbhFigure->NAME, ScanFigList->NAME );
    }
  }

  if ( ! IsFsmFigMulti( FsmFigure ) )
  {
    FvhHeadFigList->FSM_FIGURE = FsmFigure;
  }

  for ( ScanFigList  = FvhHeadFigList;
        ScanFigList != (fvhfig_list *)0;
        ScanFigList  = ScanFigList->NEXT) 
  {
    Figure = ScanFigList->FSM_FIGURE;

    if ( Figure == (fsmfig_list *)0 )
    {
      Figure = addfsmfig( ScanFigList->NAME );
      ScanFigList->FSM_FIGURE = Figure;
      FsmFigure->MULTI = addchain( FsmFigure->MULTI, (void *)Figure );

      SetFsmFigMultiLeaf( Figure );
    }

    Figure->STAR_STATE = addfsmstate( Figure, "*" );
    SetFsmStarState( Figure->STAR_STATE );

    Type = ScanFigList->STATE_TYPE;

    for ( Index = 0; Index < Type->SIZE; Index++ )
    {
      if ( Type->VALUE[ Index ] == ScanFigList->FIRST_STATE )
      {
        Figure->FIRST_STATE = addfsmstate( Figure, ScanFigList->FIRST_STATE );
        SetFsmFirstState( Figure->FIRST_STATE );
      }
      else
      {
        addfsmstate( Figure, Type->VALUE[ Index ] );
      }
    }

    if ( ScanFigList->CONTROL_TYPE != (fbtyp_list *)0 )
    {
      Figure->STACK_SIZE = 1;

      FvhDefaultStackSize = 0;
      FvhStackPushSize    = 0;
      FvhStackPopSize     = 0;
      FvhDefaultStackName = (chain_list *)0;
  
      for ( ScanAux = FbhFigure->BEAUX;
            ScanAux != (fbaux_list *)0;
            ScanAux  = ScanAux->NEXT )
      {
        Type = ( FbhFigure->BETYP + ScanAux->TYPE - 1 );

        if ( Type == ScanFigList->STATE_TYPE )
        {
          if ( ( ScanAux->NAME != ScanFigList->CURRENT_STATE ) &&
               ( ScanAux->NAME != ScanFigList->NEXT_STATE    ) &&
               ( ScanAux->NAME != ScanFigList->RETURN_STATE  ) )
          {
            FvhDefaultStackSize++;
  
            FvhDefaultStackName = addchain( FvhDefaultStackName, (void *)ScanAux->NAME );
          }
        }
      }
  
      if ( FvhDefaultStackSize == 0 )
      {
        fvherror( FVH_ERROR_STACK_SIZE_ZERO, Figure->NAME, 0 );
      }

      ScanFigList->DEFAULT_STACK_SIZE = FvhDefaultStackSize;
      ScanFigList->STACK_PUSH_SIZE    = FvhStackPushSize;
      ScanFigList->STACK_POP_SIZE     = FvhStackPopSize;
      ScanFigList->DEFAULT_STACK_NAME = FvhDefaultStackName;
    }

    for ( Process  = FbhFigure->BEPCS;
          Process != (fbpcs_list *)0;
          Process  = Process->NEXT )
    {
      if ( Process->FLAG ) continue;

      ProcInfo = (fbpcsinfo *)Process->USER;
      Element  = searchauthelem( ProcInfo->HASH_READ, ScanFigList->CURRENT_STATE );

      if ( Element != (authelem *)0 )
      {
        Element = searchauthelem( ProcInfo->HASH_WRITE, ScanFigList->NEXT_STATE );

        if ( ( Element                   == (authelem *)0 ) ||
             ( ScanFigList->PROCESS_MAIN != (fbpcs_list *)0 ) )
        {
          fvherror( FVH_ERROR_BAD_PROCESS, FsmFigure->NAME, Process->LABEL );
        }

        ScanFigList->PROCESS_MAIN = Process;
        Process->FLAG = 1;
      }
      else
      {
        Element = searchauthelem( ProcInfo->HASH_WRITE, ScanFigList->CURRENT_STATE );

        if ( Element != (authelem *)0 )
        {
          Element = searchauthelem( ProcInfo->HASH_READ, ScanFigList->NEXT_STATE );

          if ( ( Element                     == (authelem *)0 ) ||
               ( ScanFigList->PROCESS_CLOCK != (fbpcs_list *)0 ) )
          {
            fvherror( FVH_ERROR_BAD_PROCESS, FsmFigure->NAME, Process->LABEL );
          }

          ScanFigList->PROCESS_CLOCK = Process;
          Process->FLAG = 1;
        }
      }
    }
  }

  for ( Process  = FbhFigure->BEPCS;
        Process != (fbpcs_list *)0;
        Process  = Process->NEXT )
  {
    if ( ! Process->FLAG )
    {
      fvherror( FVH_ERROR_BAD_PROCESS, FsmFigure->NAME, Process->LABEL );
    }

    Process->FLAG = 0;
  }

  if ( MixedFsmRtl )
  {
    SetFsmFigMixedRtl( FsmFigure );
    FsmFigure->FIGURE = (void *)FbhFigure;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     FvhFbhTreatMultiPort                    |
|                                                             |
\------------------------------------------------------------*/

void FvhFbhTreatMultiPort( FsmFigure, Process, ScanFigList )

  fsmfig_list *FsmFigure;
  fbpcs_list  *Process;
  fvhfig_list *ScanFigList;
{
  fsmfig_list  *Figure;
  fbpcsinfo    *ProcInfo;
  chain_list   *ScanChain;
  fsmport_list *FsmPort;
  char         *Name;

  Figure   = ScanFigList->FSM_FIGURE;
  ProcInfo = (fbpcsinfo *)Process->USER;

  for ( ScanChain  = ProcInfo->READ_LIST;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Name = (char *)ScanChain->DATA;

    if ( ( Name != ScanFigList->CLOCK         ) &&
         ( Name != ScanFigList->CURRENT_STATE ) &&
         ( Name != ScanFigList->NEXT_STATE    ) &&
         ( Name != ScanFigList->CONTROL       ) )
    {
      FsmPort = searchfsmport( FsmFigure, Name );

      if ( FsmPort != (fsmport_list *)0 )
      {
        addfsmin( Figure, Name );

        if ( searchfsmport( Figure, Name ) == (fsmport_list *)0 )
        {
          addfsmport( Figure, FsmPort->NAME, FsmPort->DIR, FsmPort->TYPE );
        }
      }
    }
  }

  for ( ScanChain  = ProcInfo->WRITE_LIST;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Name = (char *)ScanChain->DATA;

    if ( ( Name != ScanFigList->CURRENT_STATE ) &&
         ( Name != ScanFigList->NEXT_STATE    ) &&
         ( Name != ScanFigList->CONTROL       ) )
    {
      FsmPort = searchfsmport( FsmFigure, Name );

      if ( FsmPort != (fsmport_list *)0 )
      {
        addfsmout( Figure, Name );

        if ( searchfsmport( Figure, Name ) == (fsmport_list *)0 )
        {
          addfsmport( Figure, FsmPort->NAME, FsmPort->DIR, FsmPort->TYPE );
        }
      }
    }
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
  fvhfig_list *ScanFigList;
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
  char         IsClock;

  if ( ! IsFsmFigMixedRtl( FsmFigure ) )
  {
    for ( ScanPort  = FbhFigure->BEPOR; 
          ScanPort != (fbpor_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      if ( ScanPort->DIRECTION != 'O' )
      {
        IsClock = 0;

        for ( ScanFigList  = FvhHeadFigList;
              ScanFigList != (fvhfig_list *)0;
              ScanFigList  = ScanFigList->NEXT )
        {
          if ( ScanPort->NAME == ScanFigList->CLOCK )
          {
            ScanFigList->CLOCK_OK = 1; IsClock = 1;
          }
        }

        if ( ! IsClock )
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
        IsClock = 0;

        for ( ScanFigList  = FvhHeadFigList;
              ScanFigList != (fvhfig_list *)0;
              ScanFigList  = ScanFigList->NEXT )
        {
          if ( ScanPort->NAME == ScanFigList->CLOCK )
          {
            ScanFigList->CLOCK_OK = 1; IsClock = 1;
          }
        }

        if ( ! IsClock )
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
      for ( ScanFigList  = FvhHeadFigList;
            ScanFigList != (fvhfig_list *)0;
            ScanFigList  = ScanFigList->NEXT )
      {
        if ( ( ScanAux->NAME == ScanFigList->NEXT_STATE    ) ||
             ( ScanAux->NAME == ScanFigList->CURRENT_STATE ) ||
             ( ScanAux->NAME == ScanFigList->RETURN_STATE  ) ) break;
      }

      if ( ScanFigList != (fvhfig_list *)0 ) continue;

      IsClock = 0;

      for ( ScanFigList  = FvhHeadFigList;
            ScanFigList != (fvhfig_list *)0;
            ScanFigList  = ScanFigList->NEXT )
      {
        if ( ScanAux->NAME == ScanFigList->CLOCK )
        {
          ScanFigList->CLOCK_OK = 1; IsClock = 1;
        }
      }

      if ( ! IsClock )
      {
        addfsmin( FsmFigure, ScanAux->NAME );
      }

      if ( IsFbhAssignByFsm( ScanAux ) )
      {
        addfsmout( FsmFigure, ScanAux->NAME );
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

  for ( ScanFigList  = FvhHeadFigList;
        ScanFigList != (fvhfig_list *)0;
        ScanFigList  = ScanFigList->NEXT )
  {
    if ( ! ScanFigList->CLOCK_OK )
    {
      fvherror( FVH_ERROR_MISSING_CLOCK_PORT, FsmFigure->NAME, ScanFigList->NAME );
    }
  }

  if ( IsFsmFigMulti( FsmFigure ) )
  {
    for ( ScanFigList  = FvhHeadFigList;
          ScanFigList != (fvhfig_list *)0;
          ScanFigList  = ScanFigList->NEXT) 
    {
      FvhFbhTreatMultiPort( FsmFigure, ScanFigList->PROCESS_CLOCK, ScanFigList );
      FvhFbhTreatMultiPort( FsmFigure, ScanFigList->PROCESS_MAIN , ScanFigList );
    }
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
    fvherror( FVH_ERROR_ILLEGAL_IF_CONDITION, (char *)0, ScanIfs->LINE_NUM );
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
      fvherror( FVH_ERROR_IN_CLOCK_PROCESS, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
          fvherror( FVH_ERROR_ILLEGAL_OUT_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
        fvherror( FVH_ERROR_ILLEGAL_OUT_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
      }

      if ( ! ABL_ATOM( ScanAsg->ABL ) )
      {
        if ( isablnameinexpr( ScanAsg->ABL, ABL_ATOM_NAME_DC ) )
        {
          fvherror( FVH_ERROR_ILLEGAL_OUT_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
          fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhNextStateName, ScanAsg->LINE_NUM );
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
                          ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->LINE_NUM );
              }

              *PStateTo        = FsmFigure->STAR_STATE;
              FvhStackHeadName = ABL_ATOM_VALUE( ScanAsg->ABL );
            }
            else
            {
              fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN,
                        ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->LINE_NUM );
            }
          }
          else
          {
            fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
          fvherror( FVH_ERROR_IN_MAIN_PROCESS, ScanAsg->NAME, ScanAsg->LINE_NUM );
        }
/*
**  CURRENT_STATE <= NEXT_STATE
*/
        if ( ABL_ATOM_VALUE( ScanAsg->ABL ) != FvhNextStateName )
        {
          fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
        }
        else
        {
          if ( Equation == (ablexpr    *)0 )
          {
            fvherror( FVH_ERROR_NO_CONTROL_CONDITION, FsmFigure->NAME, ScanAsg->LINE_NUM );
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
            fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhReturnStateName, ScanAsg->LINE_NUM );
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
              fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
            }
          }
          else
          {
            fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
          }
        }
        else
        {
          if ( FvhTreatMainProcess )
          {
            fvherror( FVH_ERROR_IN_MAIN_PROCESS, ScanAsg->NAME, ScanAsg->LINE_NUM );
          }

          Stack = FvhFbhGetStack( ScanAsg->NAME );

          if ( Stack == -1 )
          {
            fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
              fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
                            ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->LINE_NUM );
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
                fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
              }
            }
            else
            {
              fvherror( FVH_ERROR_ILLEGAL_STACK_ASSIGN,
                        ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->LINE_NUM );
            }
          }
        }
      }
      else
      {
        fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_STATE_ASSIGN, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
            fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhControlName, ScanAsg->LINE_NUM );
          }

          *PControl = FvhFbhGetControl( ABL_ATOM_VALUE( ScanAsg->ABL ) );
        }
        else
        {
          fvherror( FVH_ERROR_ILLEGAL_ASSIGNATION, ScanAsg->NAME, ScanAsg->LINE_NUM );
        }
      }
      else
      {
        fvherror( FVH_ERROR_ILLEGAL_ASSIGNATION, ScanAsg->NAME, ScanAsg->LINE_NUM );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_CONTROL_ASSIGN, 
                ABL_ATOM_VALUE( ScanAsg->ABL ), ScanAsg->LINE_NUM );
    }
  }
  else
  {
    fvherror( FVH_ERROR_ILLEGAL_ASSIGNATION, ScanAsg->NAME, ScanAsg->LINE_NUM );
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
          fvherror( FVH_ERROR_IN_CLOCK_PROCESS, FvhCurrentStateName, ScanCase->LINE_NUM );
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
        fvherror( FVH_ERROR_ILLEGAL_CASE, ScanType->NAME, ScanCase->LINE_NUM );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_STATE, 
                ABL_ATOM_VALUE( ScanCase->ABL ), ScanCase->LINE_NUM );
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
          fvherror( FVH_ERROR_IN_MAIN_PROCESS, FvhControlName, ScanCase->LINE_NUM );
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
        fvherror( FVH_ERROR_ILLEGAL_CASE, ScanType->NAME, ScanCase->LINE_NUM );
      }
    }
    else
    {
      fvherror( FVH_ERROR_ILLEGAL_CONTROL, 
                ABL_ATOM_VALUE( ScanCase->ABL ), ScanCase->LINE_NUM );
    }
  }
  else
  {
    fvherror( FVH_ERROR_ILLEGAL_CASE, ScanType->NAME, ScanCase->LINE_NUM );
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
  fsmfig_list *Figure;
  fvhfig_list *ScanFigList;
  fbpcs_list  *ScanProc = NULL;
  chain_list  *ScanSens;
  fbpcs_list  *MainProc;
  fbpcs_list  *ClockProc;
  ptype_list  *ScanInst;
  fbifs_list  *ScanIf;
  char         SensCurrent;

  for ( ScanFigList  = FvhHeadFigList;
        ScanFigList != (fvhfig_list *)0;
        ScanFigList  = ScanFigList->NEXT )
  {
    Figure    = ScanFigList->FSM_FIGURE;
    MainProc  = ScanFigList->PROCESS_MAIN;
    ClockProc = ScanFigList->PROCESS_CLOCK;

    FvhStackHeadName     = (char *)0;
    FvhTreatStackControl = 0;
    FvhTreatStateControl = 0;
    FvhTreatMainProcess  = 0;

    ScanSens = ClockProc->SENSITIVITY;

    if ( ( ScanSens       != (chain_list *)0            ) &&
         ( ScanSens->NEXT == (chain_list *)0            ) &&
         ( ScanSens->DATA == (void *)ScanFigList->CLOCK ) )
    {
      Figure->CLOCK = ScanFigList->CLOCK;
    }
    else
    {
      fvherror( FVH_ERROR_PROCESS_SENSITIVITY, ClockProc->LABEL, ClockProc->LINE_NUM );
    }
/*
** Verify the sensitivity list of the main process (to be done better)
*/
    SensCurrent = 0;

    for ( ScanSens  = MainProc->SENSITIVITY;
          ScanSens != (chain_list *)0;
          ScanSens  = ScanSens->NEXT )
    {
      if ( ScanSens->DATA == (void *)ScanFigList->CURRENT_STATE )
      {
        SensCurrent = 1;
      }
    }

    if ( ! SensCurrent )
    {
      fvherror( FVH_ERROR_MISSING_SIGNAL,
                ScanFigList->CURRENT_STATE, ScanProc->LINE_NUM );
    }
/*
** Check the body of the clock process
*/
    ScanInst = ClockProc->INSTRUCTION;

    if ( ( ScanInst       == (ptype_list *)0 ) ||
         ( ScanInst->NEXT != (ptype_list *)0 ) ||
         ( ScanInst->TYPE != FBH_BEIFS       ) )
    {
      fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, ClockProc->LABEL, ClockProc->LINE_NUM );
    }

    ScanIf = (fbifs_list *)ScanInst->DATA;

    if ( ( ScanIf->CNDTRUE  == (ptype_list *)0 ) ||
         ( ScanIf->CNDFALSE != (ptype_list *)0 ) )
    {
      fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, ClockProc->LABEL, ClockProc->LINE_NUM );
    }
/*
** Check the body of the main process
*/
    ScanInst = MainProc->INSTRUCTION;

    if ( ScanInst == (ptype_list *)0 )
    {
      fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, MainProc->LABEL, MainProc->LINE_NUM );
    }

    for ( ScanInst  = MainProc->INSTRUCTION;
          ScanInst != (ptype_list *)0;
          ScanInst  = ScanInst->NEXT )
    {
      if ( ( ScanInst->TYPE != FBH_BEIFS ) &&
           ( ScanInst->TYPE != FBH_BECAS ) )
      {
        fvherror( FVH_ERROR_ILLEGAL_PROCESS_BODY, MainProc->LABEL, MainProc->LINE_NUM );
      }
    }

    FvhCurrentStateName = ScanFigList->CURRENT_STATE;
    FvhNextStateName    = ScanFigList->NEXT_STATE;
    FvhFirstStateName   = ScanFigList->FIRST_STATE;
    FvhReturnStateName  = ScanFigList->RETURN_STATE;
    FvhControlName      = ScanFigList->CONTROL;

    FvhStackControlName[ FSM_CTRL_NOP  ] = ScanFigList->STACK_CONTROL[ FSM_CTRL_NOP  ];
    FvhStackControlName[ FSM_CTRL_POP  ] = ScanFigList->STACK_CONTROL[ FSM_CTRL_POP  ];
    FvhStackControlName[ FSM_CTRL_PUSH ] = ScanFigList->STACK_CONTROL[ FSM_CTRL_PUSH ];

    FvhStateType   = ScanFigList->STATE_TYPE;
    FvhControlType = ScanFigList->CONTROL_TYPE;

    FvhStarStateLocout   = 0;
    FvhWhenCondition     = 0;
    FvhTreatStateControl = 0;
    FvhDefaultStackSize  = ScanFigList->DEFAULT_STACK_SIZE;
    FvhStackPushSize     = ScanFigList->STACK_PUSH_SIZE;
    FvhStackPopSize      = ScanFigList->STACK_POP_SIZE;
    FvhDefaultStackName  = ScanFigList->DEFAULT_STACK_NAME;

    FvhTreatMainProcess  = 1;

    FvhFbhTreatInstruction( FbhFigure, Figure, MainProc->INSTRUCTION,
                              (fsmstate_list *)0, (fsmstate_list *)0,
                              (fsmstate_list *)0, -1, (ablexpr    *)0 );

    FvhTreatMainProcess = 0;

    FvhFbhTreatInstruction( FbhFigure, Figure, ClockProc->INSTRUCTION,
                              (fsmstate_list *)0, (fsmstate_list *)0,
                              (fsmstate_list *)0, -1, (ablexpr    *)0 );

    if ( ! FvhTreatStateControl )
    {
      fvherror( FVH_ERROR_MISSING_STATE_CONTROL, Figure->NAME, 0 );
    }

    if ( Figure->STACK_SIZE )
    {
      if ( ! FvhTreatStackControl )
      {
        fvherror( FVH_ERROR_MISSING_STACK_CONTROL, Figure->NAME, 0 );
      }

      if ( ( FvhStackPushSize > FvhDefaultStackSize  ) ||
           ( FvhStackPopSize  > FvhDefaultStackSize  ) ||
           ( FvhStackPopSize != FvhStackPushSize - 1 ) )
      {
        fvherror( FVH_ERROR_BAD_STACK_CONTROL, Figure->NAME, 0 );
      }

      Figure->STACK_SIZE = FvhStackPushSize;
    }

    ScanFigList->WHEN_CONDITION    = FvhWhenCondition;
    ScanFigList->STAR_STATE_LOCOUT = FvhStarStateLocout;
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
  fvhfig_list   *ScanFigList;
  fsmfig_list   *Figure;
  fsmstate_list *StarState;
  chain_list    *StarChain;
  fsmtrans_list *ScanTrans;
  ablexpr       *Equation;

  for ( ScanFigList  = FvhHeadFigList;
        ScanFigList != (fvhfig_list *)0;
        ScanFigList  = ScanFigList->NEXT )
  {
    if ( IsFsmFigMulti( FsmFigure ) )
    {
      Figure = ScanFigList->FSM_FIGURE;

      addfsmpragma( Figure, FvhClockKeyword       , ScanFigList->CLOCK        , ScanFigList->NAME );
      addfsmpragma( Figure, FvhCurrentStateKeyword, ScanFigList->CURRENT_STATE, ScanFigList->NAME );
      addfsmpragma( Figure, FvhNextStateKeyword   , ScanFigList->NEXT_STATE   , ScanFigList->NAME );
      addfsmpragma( Figure, FvhFirstStateKeyword  , ScanFigList->FIRST_STATE  , ScanFigList->NAME );
      addfsmpragma( Figure, FvhControlKeyword     , ScanFigList->CONTROL      , ScanFigList->NAME );

      addfsmpragma( Figure, FvhStackControlName[ FSM_CTRL_NOP ],
                    ScanFigList->STACK_CONTROL[ FSM_CTRL_NOP ] , ScanFigList->NAME );
      addfsmpragma( Figure, FvhStackControlName[ FSM_CTRL_POP ],
                    ScanFigList->STACK_CONTROL[ FSM_CTRL_POP ] , ScanFigList->NAME );
      addfsmpragma( Figure, FvhStackControlName[ FSM_CTRL_PUSH ],
                    ScanFigList->STACK_CONTROL[ FSM_CTRL_PUSH ] , ScanFigList->NAME );
    }

    if ( ! ScanFigList->WHEN_CONDITION )
    {
      Figure = ScanFigList->FSM_FIGURE;

      if ( ScanFigList->STAR_STATE_LOCOUT )
      {
        fvherror( FVH_ERROR_ILLEGAL_DEFAULT_ASSIGN, (char *)0, 0 );
      }
  
      StarState = Figure->STAR_STATE;
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

        /* dangerous ! */
  
        continue;
      }
      else
      if ( ABL_CDDR( Equation ) == (ablexpr    *)0 )
      {
        StarChain = ABL_CADR( Equation );
        ABL_CADR_L( Equation ) = (ablexpr    *)0;
        freeablexpr( Equation );
        Equation = StarChain;
      }
  
      Equation = optimablnotexpr( Equation );
  
      for ( ScanTrans  = Figure->TRANS;
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
}

/*------------------------------------------------------------\
|                                                             |
|                          FvhFbhScanLeftExpr                 |
|                                                             |
\------------------------------------------------------------*/

static void FvhFbhScanLeftExpr( Target )

  char *Target;
{
  authelem *Element;

  Element = searchauthelem( FvhProcInfo->HASH_WRITE, Target );

  if ( Element == (authelem *)0 )
  {
    FvhProcInfo->WRITE_LIST = addchain( FvhProcInfo->WRITE_LIST, Target );
    addauthelem( FvhProcInfo->HASH_WRITE, Target, 0 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          FvhFbhScanRightExpr                |
|                                                             |
\------------------------------------------------------------*/

static void FvhFbhScanRightExpr( Expr )

  ablexpr *Expr;
{
  authelem *Element;
  char     *Value;

  if ( ABL_ATOM( Expr ) )
  {
    Value = ABL_ATOM_VALUE( Expr );

    if ( ( Value != ABL_ATOM_NAME_ONE  ) &&
         ( Value != ABL_ATOM_NAME_ZERO ) &&
         ( Value != ABL_ATOM_NAME_DC   ) )
    {
      Element = searchauthelem( FvhProcInfo->HASH_READ, Value );

      if ( Element == (authelem *)0 )
      {
        FvhProcInfo->READ_LIST = addchain( FvhProcInfo->READ_LIST, Value );
        addauthelem( FvhProcInfo->HASH_READ, Value, 0 );
      }
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      FvhFbhScanRightExpr( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          FvhFbhScanProcess                  |
|                                                             |
\------------------------------------------------------------*/

static int FvhFbhScanProcess( FbhFigure, FsmFigure )

  fbfig_list  *FbhFigure;
  fsmfig_list *FsmFigure;
{
  fbpcs_list *ScanProcess;
  fbpcsinfo  *ProcInfo;
  int         Number;

  Number = 0;

  for ( ScanProcess  = FbhFigure->BEPCS;
        ScanProcess != (fbpcs_list *)0;
        ScanProcess  = ScanProcess->NEXT ) 
  {
    ProcInfo = (fbpcsinfo *)autallocheap( sizeof( fbpcsinfo ) ); 
    ProcInfo->HASH_READ  = createauthtable( 50 );
    ProcInfo->HASH_WRITE = createauthtable( 50 );
    ScanProcess->USER = (void *)ProcInfo;

    FvhProcInfo = ProcInfo;

    FbhScanExprProcess( ScanProcess, FvhFbhScanLeftExpr, FvhFbhScanRightExpr );

# ifdef DEBUG
    {
      chain_list *ScanList;

      fprintf( stdout, "Process %s:\n", ScanProcess->LABEL );

      fprintf( stdout, "READ\n" );
      for ( ScanList  = ProcInfo->READ_LIST;
            ScanList != (chain_list *)0;
            ScanList  = ScanList->NEXT )
      {
        fprintf( stdout, "%s\n", (char *)ScanList->DATA );
      }

      fprintf( stdout, "WRITE\n" );
      for ( ScanList  = ProcInfo->WRITE_LIST;
            ScanList != (chain_list *)0;
            ScanList  = ScanList->NEXT )
      {
        fprintf( stdout, "%s\n", (char *)ScanList->DATA );
      }
    }
# endif

    Number++;
  }

  if ( Number & 1 )
  {
    fvherror( FVH_ERROR_TWO_PROCESS, FsmFigure->NAME, 0 );
  }

  return( Number );
}

/*------------------------------------------------------------\
|                                                             |
|                          FvhFbhCleanProcess                 |
|                                                             |
\------------------------------------------------------------*/

static void FvhFbhCleanProcess( FbhFigure )

  fbfig_list *FbhFigure;
{
  fbpcs_list *ScanProcess;
  fbpcsinfo  *ProcInfo;

  for ( ScanProcess  = FbhFigure->BEPCS;
        ScanProcess != (fbpcs_list *)0;
        ScanProcess  = ScanProcess->NEXT ) 
  {
    ProcInfo = (fbpcsinfo *)ScanProcess->USER;
    ScanProcess->USER = (void *)0;
    destroyauthtable( ProcInfo->HASH_READ  );
    destroyauthtable( ProcInfo->HASH_WRITE );

    freechain( ProcInfo->READ_LIST  );
    freechain( ProcInfo->WRITE_LIST );

    autfreeheap( ProcInfo, sizeof( fbpcsinfo ) );
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
  int Number;

  FvhInitializeKeyword();

  FsmFigure->NAME = FbhFigure->NAME;

  Number = FvhFbhScanProcess( FbhFigure, FsmFigure );

  FvhFbhTreatPragma( FbhFigure, FsmFigure, Number );

  FvhFbhTreatAux( FbhFigure, FsmFigure );
  FvhFbhTreatPort( FbhFigure, FsmFigure );
  FvhFbhTreatProcess( FbhFigure, FsmFigure );

  FvhFbhPostTreat( FsmFigure );

  FvhFbhCleanProcess( FbhFigure );
  FvhFbhFreeFigList();

# ifdef DEBUG

  {
    chain_list  *ScanChain;
    fsmfig_list *Figure;

    if ( IsFsmFigMulti( FsmFigure ) )
    {
      fprintf( stdout, "MULTI FSM !!!\n" );
      viewfsmfig( FsmFigure );

      for ( ScanChain  = FsmFigure->MULTI;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        Figure = (fsmfig_list *)ScanChain->DATA;

        fprintf( stdout, "--------------------------\n" );
        viewfsmfig( Figure );
        fprintf( stdout, "--------------------------\n" );
      }
    }
    else
    {
      viewfsmfig( FsmFigure );
    }
  }

# endif

  return( FsmFigure );
}
