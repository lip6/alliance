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
| Tool    :                     Fsm                           |
|                                                             |
| File    :                  fsmview.c                        |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
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
# include "fsmview.h"
# include "fsmerror.h"

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
|                       Fsm View Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Fsm View Input                       |
|                                                             |
\------------------------------------------------------------*/

void viewfsmin( Input )

  fsmin_list *Input ;
{
  fprintf( stdout, "\n--> Input" );
  fprintf( stdout, "\n\t\tNAME : %s", Input->NAME );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Input->FLAGS );
  fprintf( stdout, "\n\t\tUSER : %lx", (long)Input->USER );
  fprintf( stdout, "\n<-- Input" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm View Output                      |
|                                                             |
\------------------------------------------------------------*/

void viewfsmout( Output )

  fsmout_list *Output ;
{
  fprintf( stdout, "\n--> Output" );
  fprintf( stdout, "\n\t\tNAME : %s", Output->NAME );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Output->FLAGS );
  fprintf( stdout, "\n\t\tUSER : %lx", (long)Output->USER );
  fprintf( stdout, "\n<-- Output" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm View Port                        |
|                                                             |
\------------------------------------------------------------*/

void viewfsmport( Port )

  fsmport_list *Port ;
{
  fprintf( stdout, "\n--> Port" );
  fprintf( stdout, "\n\t\tNAME : %s", Port->NAME );
  fprintf( stdout, "\n\t\tDIR : %c", Port->DIR );
  if ( Port->TYPE < FSM_MAX_TYPE )
    fprintf( stdout, "\n\t\tTYPE : %s", FSM_TYPE_NAME[ Port->TYPE ] );
  else
    fprintf( stdout, "\n\t\tTYPE : %d", (int)Port->TYPE );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Port->FLAGS );
  fprintf( stdout, "\n\t\tUSER : %lx", (long)Port->USER );
  fprintf( stdout, "\n<-- Port" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm View Local Output                   |
|                                                             |
\------------------------------------------------------------*/

void viewfsmlocout( Locout )

  fsmlocout_list *Locout;
{
  fprintf( stdout, "\n--> Local output" );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Locout->FLAGS );
  fprintf( stdout, "\n\t\tUSER : %lx", (long)Locout->USER );
  fprintf( stdout, "\n\t\tOUT : " );

  if ( Locout->OUT != (fsmout_list *)0 )
  {
    fprintf( stdout, "%s", Locout->OUT->NAME );
  }

  fprintf( stdout, "\n\t\tABL : " );

  if ( Locout->ABL != (ablexpr *)0 )
  {
    viewablexpr( Locout->ABL, ABL_VIEW_VHDL );
  }

  fprintf( stdout, "\n\t\tABL_DC : " );

  if ( Locout->ABL_DC != (ablexpr *)0 )
  {
    viewablexpr( Locout->ABL_DC, ABL_VIEW_VHDL );
  }

  fprintf( stdout, "\n<-- Local output" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm View Transition                     |
|                                                             |
\------------------------------------------------------------*/

void viewfsmtrans( Trans )

  fsmtrans_list *Trans;
{
  fprintf( stdout, "\n--> Transition" );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Trans->FLAGS );
  fprintf( stdout, "\n\t\tUSER : %lx", (long)Trans->USER );

  fprintf( stdout, "\n\t\tFROM : " );

  if ( Trans->FROM != (fsmstate_list *)0 )
  {
    fprintf( stdout, "%s", Trans->FROM->NAME );
  }

  fprintf( stdout, "\n\t\tTO : " );

  if ( Trans->TO != (fsmstate_list *)0 )
  {
    fprintf( stdout, "%s", Trans->TO->NAME );
  }

  fprintf( stdout, "\n\t\tABL : " );

  if ( Trans->ABL != (ablexpr *)0 )
  {
    viewablexpr( Trans->ABL, ABL_VIEW_VHDL );
  }

  fprintf( stdout, "\n<-- Transition" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm View Stack                          |
|                                                             |
\------------------------------------------------------------*/

void viewfsmstack( Stack )

  fsmstack_list *Stack;
{
  fprintf( stdout, "\n--> Stack" );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Stack->FLAGS );
  fprintf( stdout, "\n\t\tUSER : %lx", (long)Stack->USER );

  if ( Stack->CTRL < FSM_MAX_CTRL )
  {
    fprintf( stdout, "\n\t\tCTRL : %s", FSM_CTRL_NAME[ (long)Stack->CTRL ] );
  }
  else
  {
    fprintf( stdout, "\n\t\tCTRL : user %d ", Stack->CTRL );
  }

  fprintf( stdout, "\n\t\tCURRENT : " );

  if ( Stack->CURRENT != (fsmstate_list *)0 )
  {
    fprintf( stdout, "%s", Stack->CURRENT->NAME );
  }

  fprintf( stdout, "\n\t\tRETURN : " );

  if ( Stack->RETURN != (fsmstate_list *)0 )
  {
    fprintf( stdout, "%s", Stack->RETURN->NAME );
  }

  fprintf( stdout, "\n\t\tABL : " );

  if ( Stack->ABL != (ablexpr *)0 )
  {
    viewablexpr( Stack->ABL, ABL_VIEW_VHDL );
  }

  fprintf( stdout, "\n<-- Stack" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm View State                       |
|                                                             |
\------------------------------------------------------------*/

void viewfsmstate( State )

  fsmstate_list *State;
{
  chain_list     *ScanChain;
  fsmlocout_list *Locout;

  fprintf( stdout, "\n--> State" );
  fprintf( stdout, "\n\t\tNAME : %s", State->NAME );
  fprintf( stdout, "\n\t\tFLAGS : %lx", State->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)State->USER );
  fprintf( stdout, "\n\t\tFROM : " );

  for ( ScanChain  = State->FROM;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewfsmtrans( (fsmtrans_list *)(ScanChain->DATA) );
  }

  fprintf( stdout, "\n\t\tTO : " );

  for ( ScanChain  = State->TO;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewfsmtrans( (fsmtrans_list *)(ScanChain->DATA) );
  }

  fprintf( stdout, "\n\t\tSTACK : " );

  for ( ScanChain  = State->STACK;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewfsmstack( (fsmstack_list *)(ScanChain->DATA) );
  }

  fprintf( stdout, "\n\t\tLOCOUT : " );

  for ( Locout  = State->LOCOUT;
        Locout != (fsmlocout_list *)0;
        Locout  = Locout->NEXT )
  {
    viewfsmlocout( Locout );
  }

  fprintf( stdout, "\n<-- State" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm View Figure                      |
|                                                             |
\------------------------------------------------------------*/

void viewfsmfig( Figure )

  fsmfig_list *Figure;
{
  fsmstate_list *State;
  fsmtrans_list *Trans;
  fsmstack_list *Stack;
  fsmin_list    *Input;
  fsmout_list   *Output;
  fsmport_list  *Port;

  fprintf( stdout, "\n--> Figure" );
  fprintf( stdout, "\n\tNAME : %s", Figure->NAME  );

  fprintf( stdout, "\n\tFLAGS : %lx", Figure->FLAGS );
  fprintf( stdout, "\n\tUSER  : %lx", (long)Figure->USER );
  fprintf( stdout, "\n\tNUMBER_IN : %ld", Figure->NUMBER_IN );
  fprintf( stdout, "\n\tNUMBER_OUT : %ld", Figure->NUMBER_OUT );
  fprintf( stdout, "\n\tNUMBER_STATE : %ld", Figure->NUMBER_STATE );
  fprintf( stdout, "\n\tNUMBER_TRANS : %ld", Figure->NUMBER_TRANS );
  fprintf( stdout, "\n\tNUMBER_STACK : %ld", Figure->NUMBER_STACK );
  fprintf( stdout, "\n\tSTACK_SIZE   : %ld", Figure->STACK_SIZE   );
  fprintf( stdout, "\n\tCLOCK        : %s", Figure->CLOCK ? Figure->CLOCK : "NULL" );
  fprintf( stdout, "\n\tCLOCK_ABL    : " );

  if ( Figure->CLOCK_ABL != (ablexpr *)0 )
  {
    viewablexpr( Figure->CLOCK_ABL, ABL_VIEW_VHDL );
  }

  fprintf( stdout, "\n\tPORT : " );

  for ( Port  = Figure->PORT;
        Port != (fsmport_list *)0;
        Port  = Port->NEXT )
  {
    viewfsmport( Port );
  }

  fprintf( stdout, "\n\tIN : " );

  for ( Input  = Figure->IN;
        Input != (fsmin_list *)0;
        Input  = Input->NEXT )
  {
    viewfsmin( Input );
  }

  fprintf( stdout, "\n\tOUT : " );

  for ( Output  = Figure->OUT;
        Output != (fsmout_list *)0;
        Output  = Output->NEXT )
  {
    viewfsmout( Output );
  }


  if ( Figure->FIRST_STATE != (fsmstate_list *)0 )
  {
    fprintf( stdout, "\n\tFIRST_STATE : " );
    viewfsmstate( Figure->FIRST_STATE );
  }

  if ( Figure->STAR_STATE != (fsmstate_list *)0 )
  {
    fprintf( stdout, "\n\tSTAR_STATE : " );
    viewfsmstate( Figure->STAR_STATE );
  }

  fprintf( stdout, "\n\tSTATE : " );

  for ( State  = Figure->STATE;
        State != (fsmstate_list *)0;
        State  = State->NEXT )
  {
    viewfsmstate( State );
  }

  fprintf( stdout, "\n\tTRANS : " );

  for ( Trans  = Figure->TRANS;
        Trans != (fsmtrans_list *)0;
        Trans  = Trans->NEXT )
  {
    viewfsmtrans( Trans );
  }

  fprintf( stdout, "\n\tSTACK : " );

  for ( Stack  = Figure->STACK;
        Stack != (fsmstack_list *)0;
        Stack  = Stack->NEXT )
  {
    viewfsmstack( Stack );
  }

  fprintf( stdout, "\n<-- Figure\n" );
}
