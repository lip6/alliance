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
| Tool    :                     FSM                           |
|                                                             |
| File    :                  fksparse.c                       |
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"

# include "fkserror.h"
# include "fksparse.h"

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

  static long  KissLineNumber = 0;
  static char  KissBuffer[ KISS_BUFFER_SIZE ];
  static char *KissScanBuffer;
  static FILE *KissFile;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        KissSpipBlank                        |
|                                                             |
\------------------------------------------------------------*/

static void KissSkipBlank( End )

  int End;
{
  int Index;

  do
  {
    if ( (  KissScanBuffer == (char *)0 ) ||
         ( *KissScanBuffer == '\0'      ) )
    {
      KissScanBuffer = fgets( KissBuffer, KISS_BUFFER_SIZE, KissFile );

      KissLineNumber++;

      if ( KissScanBuffer == (char *)0 )
      {
        if ( End ) return;

        fkserror( KISS_ERROR_END, KissLineNumber );
      }

      for ( Index = 0; KissScanBuffer[ Index ] != '\0'; Index++ )
      {
        KissScanBuffer[ Index ] = tolower( KissScanBuffer[ Index ] );
      }
    }

    while ( ( *KissScanBuffer != '\0'    ) &&
            ( isspace( *KissScanBuffer ) ) )
    {
      KissScanBuffer++;
    }
  }
  while ( *KissScanBuffer == '\0' );
}

/*------------------------------------------------------------\
|                                                             |
|                          KissGetValue                       |
|                                                             |
\------------------------------------------------------------*/

static int KissGetValue()
{
  char *Scan;
  int   Value;

  Value = 0;

  for ( Scan = KissScanBuffer; *Scan != '\0'; Scan++ )
  {
    if ( isspace( *Scan ) ) break;
  }

  if ( Scan != KissScanBuffer )
  {
    *Scan = '\0'; 
    Value = atoi( KissScanBuffer );
    *Scan = ' ';

    KissScanBuffer = Scan;
  }

  if ( Value <= 0 )
  {
    fkserror( KISS_ERROR_VALUE, KissLineNumber );
  }

  return( Value );
}

/*------------------------------------------------------------\
|                                                             |
|                          KissGetName                        |
|                                                             |
\------------------------------------------------------------*/

static char *KissGetName()
{
  char *Scan;
  char *Name;

  Name = (char *)0;

  for ( Scan = KissScanBuffer; *Scan != '\0'; Scan++ )
  {
    if ( isspace( *Scan ) ) break;
  }

  if ( Scan != KissScanBuffer )
  {
    *Scan = '\0';
    Name  = namealloc( KissScanBuffer );
    *Scan = ' ';

    KissScanBuffer = Scan;
  }

  if ( Name == (char *)0 )
  {
    fkserror( KISS_ERROR_NAME, KissLineNumber );
  }

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                        kissloadfsmfig                       |
|                                                             |
\------------------------------------------------------------*/

void kissloadfsmfig( FsmFigure, FigureName )

  fsmfig_list *FsmFigure;
  char        *FigureName;
{
  fsmstate_list *StateFrom;
  fsmstate_list *StateTo          = NULL;
  fsmstate_list *StarState;
  fsmout_list   *Output;
  fsmin_list    *Input;
  ablexpr       *Atom;
  ablexpr       *Equation;
  ablexpr       *Transition;
  char          *StateName;
  char          *StateVoidName;
  char          *StarStateName;
  char          *PortName;
  int            NumberIn        = 0;
  int            NumberOut       = 0;
  int            NumberState     = 0;
  char           Instruction;
  char           DeclarationType;
  int            Index;
  int            Value;
  char           BufferName[ 30 ];

  KissFile = mbkfopen( FigureName, "kiss2", "r" );

  if ( KissFile == (FILE *)0 )
  {
    fkserror( KISS_ERROR_OPEN_FILE, FigureName );
  }

  KissLineNumber = 0;
  Instruction    = 0;
  KissScanBuffer = (char       *)0;
  Atom           = (ablexpr    *)0;

  StateVoidName = namealloc( "void" );
  StarStateName = namealloc( "*"    );

  StarState = addfsmstate( FsmFigure, StarStateName );
  FsmFigure->STAR_STATE = StarState;
  SetFsmStarState( StarState );
/*
**  clock AND NOT clock'STABLE )
*/
  FsmFigure->CLOCK = namealloc( "clock" );

  Equation = createabloper( ABL_STABLE );
  addablhexpr( Equation, createablatom( FsmFigure->CLOCK ) );
  Equation = optimablnotexpr( Equation );
  Equation = optimablbinexpr( ABL_AND, Equation,
                               createablatom( FsmFigure->CLOCK ) );

  FsmFigure->CLOCK_ABL = Equation;

  addfsmport( FsmFigure, FsmFigure->CLOCK, FSM_DIR_IN, FSM_TYPE_BIT );

  KissSkipBlank( 0 );

  while ( KissScanBuffer != (char *)0  )
  {
    if ( Instruction == 0 )
    {
/*
**  Declaration Part
*/
      if ( KissScanBuffer[ 0 ] == '.' )
      {
        DeclarationType = KissScanBuffer[ 1 ];
        KissScanBuffer  = KissScanBuffer + 2;

        if ( ( DeclarationType == KISS_KEYWORD_IN    ) ||
             ( DeclarationType == KISS_KEYWORD_OUT   ) ||
             ( DeclarationType == KISS_KEYWORD_STATE ) ||
             ( DeclarationType == KISS_KEYWORD_R     ) ||
             ( DeclarationType == KISS_KEYWORD_P     ) )
        {
          KissSkipBlank( 0 );

          if ( DeclarationType != KISS_KEYWORD_R )
          {
            Value = KissGetValue();
          }
          else
          {
            StateName  = KissGetName();
            FsmFigure->FIRST_STATE = addfsmstate( FsmFigure, StateName );
            SetFsmFirstState( FsmFigure->FIRST_STATE );
          }

          if ( DeclarationType == KISS_KEYWORD_IN )
          {
            NumberIn = Value;
/*
** Initialize input
*/
            for ( Index = 0; Index < NumberIn; Index++ )
            {
              sprintf( BufferName, "I%d", Index );
              PortName = namealloc( BufferName );
              addfsmin( FsmFigure, PortName );
              addfsmport( FsmFigure, PortName, FSM_DIR_IN, FSM_TYPE_BIT );
            }

            FsmFigure->IN = 
               (fsmin_list *)reverse( (chain_list *)FsmFigure->IN );
          }
          else
          if ( DeclarationType == KISS_KEYWORD_OUT )
          {
            NumberOut = Value;

            for ( Index = 0; Index < NumberOut; Index++ )
            {
/*
** Initialize output
*/
              sprintf( BufferName, "O%d", Index );
              PortName = namealloc( BufferName );
              addfsmout( FsmFigure, PortName );
              addfsmport( FsmFigure, PortName, FSM_DIR_OUT, FSM_TYPE_BIT );

              FsmFigure->OUT = 
                 (fsmout_list *)reverse( (chain_list *)FsmFigure->OUT );
            }
          }
          else
          if ( DeclarationType == KISS_KEYWORD_STATE )
          {
            NumberState = Value;
/*
** Initialize state
*/
          }
        }
        else
        {
          KissScanBuffer = (char *)0;
        }
      }
      else
      {
        if ( NumberState == 0 )
        {
          fkserror( KISS_ERROR_STATE, FsmFigure->NAME );
        }

        if ( ( NumberIn  == 0 ) ||
             ( NumberOut == 0 ) )
        {
          fkserror( KISS_ERROR_PORT, FsmFigure->NAME );
        }

        Instruction = 1;
      }
    }

    if ( Instruction == 1 )
    {
/*
**  Instruction Part
*/
      if ( ( KissScanBuffer[0] == '.' ) &&
           ( KissScanBuffer[1] == 'e' ) )  break;
/*
**  Input list 
*/
      Input    = FsmFigure->IN;
      Equation = createabloper( ABL_AND );

      for ( Index = 0; Index < NumberIn; Index++ )
      {
        if ( *KissScanBuffer == '0' )
        {
          addablhexpr( Equation, optimablnotexpr( createablatom( Input->NAME ) ) );
        }
        else
        if ( *KissScanBuffer == '1' )
        {
          addablhexpr( Equation, createablatom( Input->NAME ) );
        }
        else
        if ( *KissScanBuffer != '-' )
        {
          fkserror( KISS_ERROR_PARSE, KissLineNumber );
        }

        KissScanBuffer++;
        Input = Input->NEXT;
      }

      if ( ABL_CDR( Equation ) == (ablexpr    *)0 )
      {
        delablexpr( Equation );
        Transition = createablatomone();
      }
      else
      if ( ABL_CDDR( Equation ) == (ablexpr    *)0 )
      {
        Transition = dupablexpr( ABL_CADR( Equation ) );
        delablexpr( Equation );
      }
      else
      {
        Transition = Equation;
      }

      KissSkipBlank();
/*
**  State From
*/
      StateName = KissGetName();

      if ( StateName == StarStateName ) 
      {
        StateFrom = StarState;
      }
      else
      {
        StateFrom = searchfsmstate( FsmFigure, StateName );
      }

      if ( StateFrom == (fsmstate_list *)0 )
      {
        if ( FsmFigure->NUMBER_STATE > NumberState )
        {
          fkserror( KISS_ERROR_NAME, KissLineNumber );
        }

        StateFrom = searchfsmstate( FsmFigure, StateName );

        if ( StateFrom == (fsmstate_list *)0 )
        {
          StateFrom = addfsmstate( FsmFigure, StateName );
        }
      }

      KissSkipBlank();
/*
**  State To
*/
      StateName = KissGetName();

      if ( StateName != StateVoidName )
      {
        if ( StateName == StarStateName ) 
        {
          StateTo = StarState;
        }
        else
        {
          StateTo = searchfsmstate( FsmFigure, StateName );
        }

        if ( StateTo == (fsmstate_list *)0 )
        {
          if ( FsmFigure->NUMBER_STATE > NumberState )
          {
            fkserror( KISS_ERROR_NAME, KissLineNumber );
          }

          StateTo = searchfsmstate( FsmFigure, StateName );

          if ( StateTo == (fsmstate_list *)0 )
          {
            StateTo = addfsmstate( FsmFigure, StateName );
          }
        }
      }

      if ( StateTo != (fsmstate_list *)0 )
      {
        Equation = dupablexpr( Transition );
        addfsmtrans( FsmFigure, StateFrom, StateTo, Equation );
      }

      KissSkipBlank();
/*
**  Output
*/
      Output = FsmFigure->OUT;

      for ( Index = 0; Index < NumberOut; Index++ )
      {
        if ( *KissScanBuffer == '1' )
        {
          if ( Transition != (ablexpr *)0 )
          {
            Equation = dupablexpr( Transition );
            addfsmlocout( StateFrom, Output, Equation, (ablexpr    *)0 );
          }
        }
        else
        if ( *KissScanBuffer == '-' )
        {
          if ( Transition != (ablexpr *)0 )
          {
            Equation = dupablexpr( Transition );
            addfsmlocout( StateFrom, Output, (ablexpr    *)0, Equation );
          }
        }
        else
        if ( *KissScanBuffer == '0' )
        {
          Atom = createablatomzero();
          addfsmlocout( StateFrom, Output, Atom, (ablexpr    *)0 );
        }
        else
        {
          fkserror( KISS_ERROR_PARSE, KissLineNumber );
        }

        KissScanBuffer++;
        Output = Output->NEXT;
      }

      if ( Transition != (ablexpr *)0 )
      {
        delablexpr( Transition );
      }
    }

    KissSkipBlank( 1 );
  }

  FsmFigure->PORT = (fsmport_list *)reverse( (chain_list *)FsmFigure->PORT );

  fclose( KissFile );
}
