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
| File    :                   fvhdrive.c                      |
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
# include <time.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "fvh.h"

# include "fvherror.h"
# include "fvhdrive.h"

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

  static FILE *VhdlFile;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        VhdlWriteHeader                      |
|                                                             |
\------------------------------------------------------------*/

static void VhdlWriteHeader( FsmFigure )

  fsmfig_list *FsmFigure;
{
  time_t VhdlTime;

  time( &VhdlTime );
  fprintf( VhdlFile, "-- VHDL FSM : `%s`\n", FsmFigure->NAME );
  fprintf( VhdlFile, "-- DATE : %s\n\n", ctime( &VhdlTime ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        VhdlWriteEntity                      |
|                                                             |
\------------------------------------------------------------*/

static void VhdlWriteEntity( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmport_list *Port;
  fsmport_list *ScanPort;
  char         *Name;
  char         *Vector;
  char         *NextVector;
  char         *Direction;
  char         *Type;
  char         *Bus;
  long          Index;
  long          NextIndex;
  long          Left;
  long          Right;
  long          Way;

  Name = vhdlablname( FsmFigure->NAME );

  fprintf( VhdlFile, "ENTITY %s IS\n", Name );

  if ( IsFsmFigMixedRtl( FsmFigure ) )
  {
    vhdlsavefbport( (fbfig_list *)FsmFigure->FIGURE, 0 );
  }
  else
  {
    fprintf( VhdlFile, "PORT\n(\n" );
  
    Port = FsmFigure->PORT; 
  
    while ( Port != (fsmport_list *)0 )
    {
      Vector = vhdlablvector( Port->NAME, &Index );
  
      if ( Port->DIR == FSM_DIR_IN  ) Direction = "IN";
      else
      if ( Port->DIR == FSM_DIR_OUT ) Direction = "OUT";
      else                            Direction = "INOUT";
  
      if ( Vector != (char *)0 )
      {
        Left = Right = Index;
        Way  = -1;
  
        for ( ScanPort  = Port->NEXT;
              ScanPort != (fsmport_list *)0;
              ScanPort  = ScanPort->NEXT )
        {
          NextVector = vhdlablvector( ScanPort->NAME, &NextIndex );
  
          if ( NextVector != Vector ) break;
  
          if ( Way == -1 )
          {
            if ( NextIndex < Left ) Way = 0;   /* downto */
            else                    Way = 1;   /* to     */
          }
  
          Right = NextIndex;
        }
  
        if ( Port->TYPE == FSM_TYPE_BIT_VEC ) Type = "BIT_VECTOR";
        else
        if ( Port->TYPE == FSM_TYPE_WOR_VEC ) Type = "WOR_VECTOR";
        else                                  Type = "MUX_VECTOR";
  
        if ( Port->TYPE != FSM_TYPE_BIT_VEC )  Bus = "BUS";
        else                                   Bus = "";
  
        fprintf( VhdlFile, "  %s : %s %s(%ld %s %ld) %s", 
                 vhdlablname( Vector ), Direction, Type, Left,
                 ( Way ) ? "TO" : "DOWNTO", Right, Bus );
  
        Port = ScanPort;
      }
      else
      {
        if ( Port->TYPE == FSM_TYPE_BIT     ) Type = "BIT";
        else
        if ( Port->TYPE == FSM_TYPE_WOR_BIT ) Type = "WOR_BIT BUS";
        else                                  Type = "MUX_BIT BUS";
  
        fprintf( VhdlFile, "  %s : %s %s",
                 vhdlablname( Port->NAME ), Direction, Type );
  
        Port = Port->NEXT;
      }
  
      if ( Port != (fsmport_list *)0 )
      {
        fprintf( VhdlFile, ";\n" );
      }
    }
  
    fprintf( VhdlFile, "\n);\n" );
  }
  
  fprintf( VhdlFile, "END %s;\n\n", Name );
}

/*------------------------------------------------------------\
|                                                             |
|                      VhdlWriteStack                         |
|                                                             |
\------------------------------------------------------------*/

static void VhdlWriteStack( ScanState )

  fsmstate_list *ScanState;
{
  fsmstack_list *ScanStack;
  chain_list    *ScanChain;

  for ( ScanChain  = ScanState->STACK;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanStack = (fsmstack_list *)ScanChain->DATA;
  
    if ( IsFsmStarStack( ScanStack ) ) continue;
  
    if ( ScanStack->ABL != (ablexpr    *)0 )
    {
      fprintf( VhdlFile, "      IF ( " );
      viewablexprfile( VhdlFile, ScanStack->ABL, ABL_VIEW_VHDL );
      fprintf( VhdlFile, " )\n      THEN CONTROL <= %s;\n", 
               FSM_CTRL_NAME[ (long)ScanStack->CTRL ] );
  
      if ( ScanStack->CTRL == FSM_CTRL_PUSH )
      {
        fprintf( VhdlFile, "           RETURN_STATE <= %s;\n", 
                 vhdlablname( ScanStack->RETURN->NAME ) );
      }
      else
      if ( ScanStack->CTRL == FSM_CTRL_POP )
      {
        fprintf( VhdlFile, "           NEXT_STATE <= STACK_0;\n" );
      }
          
      fprintf( VhdlFile, "      END IF;\n" );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VhdlWriteLocout                        |
|                                                             |
\------------------------------------------------------------*/

static void VhdlWriteLocout( ScanState )

  fsmstate_list *ScanState;
{
  fsmlocout_list *ScanLocout;

  for ( ScanLocout  = ScanState->LOCOUT;
        ScanLocout != (fsmlocout_list *)0;
        ScanLocout  = ScanLocout->NEXT )
  {
    if ( ScanLocout->ABL != (ablexpr    *)0 )
    {
      fprintf( VhdlFile, "      %s <= ", vhdlablname( ScanLocout->OUT->NAME ) );
      viewablexprfile( VhdlFile, ScanLocout->ABL, ABL_VIEW_VHDL );
      fprintf( VhdlFile, ";\n" );
    }

    if ( ScanLocout->ABL_DC != (ablexpr    *)0 )
    {
      if ( ( ABL_ATOM( ScanLocout->ABL_DC )                            ) &&
           ( ABL_ATOM_VALUE( ScanLocout->ABL_DC ) == ABL_ATOM_NAME_ONE ) )
      {
        fprintf( VhdlFile, "      %s <= 'd';\n", 
                 vhdlablname( ScanLocout->OUT->NAME ) );
      }
      else
      {
        fprintf( VhdlFile, "      IF ( " );
        viewablexprfile( VhdlFile, ScanLocout->ABL_DC, ABL_VIEW_VHDL );
        fprintf( VhdlFile, " )\n" );
        fprintf( VhdlFile, "      THEN %s <= 'd';\n",
                 vhdlablname( ScanLocout->OUT->NAME ) );
        fprintf( VhdlFile, "      END IF;\n" );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VhdlWriteArchitecture                  |
|                                                             |
\------------------------------------------------------------*/

static void VhdlWriteArchitecture( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;
  fsmtrans_list  *NextTrans;
  fsmin_list     *ScanIn;
  chain_list     *ScanChain;
  chain_list     *NextChain;
  char           *Vector;
  char           *LastVector;
  long            NumberState;
  long            Counter;
  long            Index;
  int             FirstOne;

  FsmFigure->STATE = (fsmstate_list *)reverse( (chain_list *)FsmFigure->STATE );

  fprintf( VhdlFile, "ARCHITECTURE FSM OF %s IS\n\n", 
           vhdlablname( FsmFigure->NAME ) );

  fprintf( VhdlFile, "TYPE STATE_TYPE IS\n(  " );

  if ( FsmFigure->STAR_STATE != (fsmstate_list *)0 )
  {
    NumberState = FsmFigure->NUMBER_STATE - 1;
  }
  else
  {
    NumberState = FsmFigure->NUMBER_STATE;
  }

  if ( FsmFigure->FIRST_STATE != (fsmstate_list *)0 )
  {
    fprintf( VhdlFile, "\n  %s", vhdlablname( FsmFigure->FIRST_STATE->NAME ) );

    if ( NumberState > 1 )
    {
      fprintf( VhdlFile, "," );
    }

    Counter = 1;
  }
  else
  {
    Counter = 0;
  }

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    if ( ( IsFsmFirstState( ScanState ) ) ||
         ( IsFsmStarState( ScanState  ) ) ) continue;

    Counter = Counter + 1;

    if ( ! ( Counter & 0x7 ) )
    {
      fprintf( VhdlFile, "\n  %s", vhdlablname( ScanState->NAME ) );
    }
    else
    {
      fprintf( VhdlFile, "%s", vhdlablname( ScanState->NAME ) );
    }

    if ( Counter < NumberState )
    {
      fprintf( VhdlFile, "," );
    }
  }

  if ( Counter & 0x7 )
  {
    fprintf( VhdlFile, "\n" );
  }

  fprintf( VhdlFile, ");\n\n" );

  if ( FsmFigure->STACK != (fsmstack_list *)0 )
  {
    fprintf( VhdlFile, "TYPE CONTROL_TYPE IS\n(\n" );

    for ( Index = 0; Index < FSM_MAX_CTRL; Index++ )
    {
      if ( Index != 0 )
      {
        fprintf( VhdlFile, ", " );
      }

      fprintf( VhdlFile, "%s", FSM_CTRL_NAME[ Index ] );
    }

    fprintf( VhdlFile, "\n);\n\n" );
    fprintf( VhdlFile, "SIGNAL CONTROL : CONTROL_TYPE;\n\n" );
    fprintf( VhdlFile, "SIGNAL RETURN_STATE : STATE_TYPE;\n" );
    fprintf( VhdlFile, "SIGNAL STACK_0" );

    for ( Index = 1; Index < FsmFigure->STACK_SIZE; Index++ )
    { 
      fprintf( VhdlFile, ", STACK_%ld", Index );

      if ( ! ( Index & 0x7 ) )
      {
        fprintf( VhdlFile, "\n" );
      }
    }

    fprintf( VhdlFile, " : STATE_TYPE;\n" );
  }

  fprintf( VhdlFile, "SIGNAL CURRENT_STATE, NEXT_STATE: STATE_TYPE;\n\n" );

  if ( FsmFigure->FIRST_STATE != (fsmstate_list *)0 )
  {
    fprintf( VhdlFile, "--PRAGMA FIRST_STATE %s\n\n", 
            vhdlablname( FsmFigure->FIRST_STATE->NAME ) );
  }

  if ( FsmFigure->CLOCK != (char *)0 )
  {
    fprintf( VhdlFile, "--PRAGMA CLOCK %s\n\n", 
            vhdlablname( FsmFigure->CLOCK ) );
  }

  if ( IsFsmFigMixedRtl( FsmFigure ) )
  {
    vhdlsavefbdecl( (fbfig_list *)FsmFigure->FIGURE, 0 );
  }

  fprintf( VhdlFile, "BEGIN\n" );

  fprintf( VhdlFile, "  PROCESS( CURRENT_STATE" );

  LastVector = (char *)0;

  for ( ScanIn  = FsmFigure->IN;
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    Vector = vhdlablvector( ScanIn->NAME, &Index );

    if ( Vector == (char *)0 )
    {
      fprintf( VhdlFile, ", %s", vhdlablname( ScanIn->NAME ) );
      LastVector = (char *)0;
    }
    else
    if ( Vector != LastVector )
    {
      fprintf( VhdlFile, ", %s", vhdlablname( Vector ) );
      LastVector = Vector;
    }
  }

  fprintf( VhdlFile, ")\n  BEGIN\n" );

  Counter = 0;

  if ( FsmFigure->STAR_STATE != (fsmstate_list *)0 )
  {
    for ( ScanChain  = FsmFigure->STAR_STATE->FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanTrans = (fsmtrans_list *)ScanChain->DATA;

      if ( ScanTrans->ABL != (ablexpr    *)0 )
      {
        if ( Counter > 0 ) fprintf( VhdlFile, "  ELSE\n" );
        fprintf( VhdlFile, "  IF (( " );
        viewablexprfile( VhdlFile, ScanTrans->ABL, ABL_VIEW_VHDL );
        fprintf( VhdlFile, ") = '1')\n  THEN NEXT_STATE <= %s;\n", 
                 vhdlablname( ScanTrans->TO->NAME ) );

        VhdlWriteLocout( FsmFigure->STAR_STATE );

        Counter = Counter + 1;
      }
    }

    while ( Counter > 1 )
    {
      fprintf( VhdlFile, "  END IF;\n" );
      Counter = Counter - 1;
    }
/*
**  Treat STACK in STAR_STATE, seems to be crazy
*/
  }

  if ( Counter > 0 ) fprintf( VhdlFile, "  ELSE\n" );
  fprintf( VhdlFile, "  CASE CURRENT_STATE IS\n" );

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    if ( IsFsmStarState( ScanState ) ) continue;

    fprintf( VhdlFile, "    WHEN %s =>\n", vhdlablname( ScanState->NAME ) );

/*
**  Treat TRANS
*/
    FirstOne = 1;

    for ( ScanChain  = ScanState->FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanTrans = (fsmtrans_list *)ScanChain->DATA;

      if ( IsFsmStarTrans( ScanTrans ) ) continue;

      for ( NextChain  = ScanChain->NEXT;
            NextChain != (chain_list *)0;
            NextChain  = NextChain->NEXT )
      {
        NextTrans = (fsmtrans_list *)ScanChain->DATA;
        if ( ! IsFsmStarTrans( NextTrans ) ) break;
      }

      if ( ScanTrans->ABL != (ablexpr    *)0 )
      {
        if ( FirstOne )
        {
          fprintf( VhdlFile, "      IF (( " );
          viewablexprfile( VhdlFile, ScanTrans->ABL, ABL_VIEW_VHDL );
          fprintf( VhdlFile, ") = '1')\n      THEN" );
          FirstOne = 0;
        }
        else
        if ( NextChain != (chain_list *)0 )
        {
          fprintf( VhdlFile, "      ELSIF (( " );
          viewablexprfile( VhdlFile, ScanTrans->ABL, ABL_VIEW_VHDL );
          fprintf( VhdlFile, ") = '1')\n      THEN" );
        }
        else
        {
          fprintf( VhdlFile, "      ELSE" );
        }

        fprintf( VhdlFile, " NEXT_STATE <= %s;\n", vhdlablname( ScanTrans->TO->NAME ) );

        if ( NextChain == (chain_list *)0 )
        {
          fprintf( VhdlFile, "      END IF;\n" );
        }
      }
    }

    VhdlWriteStack( ScanState );
    VhdlWriteLocout( ScanState );
  }

  fprintf( VhdlFile, "  END CASE;\n" );
  if ( Counter > 0 ) fprintf( VhdlFile, "  END IF;\n" );

  fprintf( VhdlFile, "  END PROCESS;\n\n" );

  fprintf( VhdlFile, "  PROCESS( %s )\n", FsmFigure->CLOCK );
  fprintf( VhdlFile, "  BEGIN\n" );

  fprintf( VhdlFile, "    IF ( " );
  viewablexprfile( VhdlFile, FsmFigure->CLOCK_ABL, ABL_VIEW_VHDL );
  fprintf( VhdlFile, " )\n    THEN CURRENT_STATE <= NEXT_STATE;\n" );

  if ( FsmFigure->STACK != (fsmstack_list *)0 )
  {
    fprintf( VhdlFile, "              CASE CONTROL IS\n" );

    for ( Index = 0; Index < FSM_MAX_CTRL; Index++ )
    {
      fprintf( VhdlFile, "     WHEN %s =>\n",
               FSM_CTRL_NAME[ Index ] );

      if ( Index == FSM_CTRL_POP )
      {
        for ( Counter = 1; Counter < FsmFigure->STACK_SIZE; Counter++ )
        {
          fprintf( VhdlFile, "       STACK_%ld <= STACK_%ld;\n",
                   Counter - 1, Counter );
        }

        if ( Counter == 1 )
        {
          fprintf( VhdlFile, "       NULL;\n" );
        }
      }
      else
      if ( Index == FSM_CTRL_PUSH )
      {
        fprintf( VhdlFile, "        STACK_0 <= RETURN_STATE;\n" );

        for ( Counter = 1; Counter < FsmFigure->STACK_SIZE; Counter++ )
        {
          fprintf( VhdlFile, "        STACK_%ld <= STACK_%ld;\n",
                   Counter, Counter - 1 );
        }
      }
      else
      {
        fprintf( VhdlFile, "        NULL;\n" );
      }
    }

    fprintf( VhdlFile, "     END CASE;\n" );
  }

  fprintf( VhdlFile, "    END IF;\n" );

  fprintf( VhdlFile, "  END PROCESS;\n" );

  if ( IsFsmFigMixedRtl( FsmFigure ) )
  {
    vhdlsavefbarchi( (fbfig_list *)FsmFigure->FIGURE, 0 );
  }

  fprintf( VhdlFile, "END;\n"  );

  FsmFigure->STATE = (fsmstate_list *)reverse( (chain_list *)FsmFigure->STATE );
}

/*------------------------------------------------------------\
|                                                             |
|                        vhdlsavefsmfig                       |
|                                                             |
\------------------------------------------------------------*/

void vhdlsavefsmfig( FsmFigure )

  fsmfig_list *FsmFigure;
{
  VhdlFile     = mbkfopen( FsmFigure->NAME, "fsm", "w" );
  FblDriveFile = VhdlFile;

  if ( VhdlFile == (FILE *)0 )
  {
    fvherror( FVH_ERROR_OPEN_FILE, FsmFigure->NAME, 0 );
  }

  if ( IsFsmFigMulti( FsmFigure ) )
  {
    fvherror( FVH_ERROR_NOT_YET_IMPLEMENTED, FsmFigure->NAME, 0 );
  }

  VhdlWriteHeader( FsmFigure );
  VhdlWriteEntity( FsmFigure );
  VhdlWriteArchitecture( FsmFigure );

  fclose( VhdlFile );
}
