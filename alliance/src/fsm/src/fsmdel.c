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
| File    :                   fsmdel.c                        |
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
# include "fsmdel.h"
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
|                       Fsm Del Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Fsm Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

int delfsmfig( Name )

   char *Name;
{
  fsmfig_list    *Figure;
  fsmfig_list   **Previous;
  fsmport_list   *Port;
  fsmport_list   *DelPort;
  fsmin_list     *Input;
  fsmin_list     *DelInput;
  fsmout_list    *Output;
  fsmout_list    *DelOutput;
  fsmstate_list  *State;
  fsmstate_list  *DelState;
  fsmtrans_list  *Trans;
  fsmtrans_list  *DelTrans;
  fsmstack_list  *Stack;
  fsmstack_list  *DelStack;
  fsmlocout_list *Locout;
  fsmlocout_list *DelLocout;

  Name     = namealloc( Name );
  Previous = &HEAD_FSMFIG;

  for ( Figure  = HEAD_FSMFIG;
        Figure != (fsmfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;

    Previous = &Figure->NEXT;
  }

  if ( Figure == (fsmfig_list *)0 ) return( 0 );

  *Previous = Figure->NEXT;

  Port = Figure->PORT;

  while( Port != (fsmport_list *)0 )
  {
    DelPort = Port;
    Port    = Port->NEXT;

    freefsmport( DelPort );
  }

  Input = Figure->IN;

  while( Input != (fsmin_list *)0 )
  {
    DelInput = Input;
    Input    = Input->NEXT;

    freefsmin( DelInput );
  }

  Output = Figure->OUT;

  while( Output != (fsmout_list *)0 )
  {
    DelOutput = Output;
    Output    = Output->NEXT;

    freefsmout( DelOutput );
  }

  State = Figure->STATE; 

  while ( State != (fsmstate_list *)0 )
  {
    DelState = State;
    State    = State->NEXT;

    freechain( DelState->STACK ); 
    freechain( DelState->FROM  ); 
    freechain( DelState->TO    ); 

    Locout = DelState->LOCOUT;
  
    while ( Locout != (fsmlocout_list *)0 )
    {
      DelLocout = Locout;
      Locout    = Locout->NEXT;

      freeablexpr( DelLocout->ABL    );
      freeablexpr( DelLocout->ABL_DC );

      freefsmlocout( DelLocout );
    }

    freefsmstate( DelState );
  }

  Trans = Figure->TRANS;

  while ( Trans != (fsmtrans_list *)0 )
  {
    DelTrans = Trans;
    Trans    = Trans->NEXT;

    freeablexpr( DelTrans->ABL );

    freefsmtrans( DelTrans );
  }

  Stack = Figure->STACK;

  while ( Stack != (fsmstack_list *)0 )
  {
    DelStack = Stack;
    Stack    = Stack->NEXT;

    freeablexpr( DelStack->ABL );

    freefsmstack( DelStack );
  }

  if ( Figure->PRAGMA != (ptype_list *)0 )
  {
    freeptype( Figure->PRAGMA );
  }

  destroyauthtable( Figure->HASH_PORT   );
  destroyauthtable( Figure->HASH_IN     );
  destroyauthtable( Figure->HASH_OUT    );
  destroyauthtable( Figure->HASH_STATE  );
  destroyauth2table( Figure->HASH_TRANS );

  freechain( Figure->MULTI );

  freefsmfig( Figure );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Del Trans                        |
|                                                             |
\------------------------------------------------------------*/

int delfsmtrans( Figure, Trans )

  fsmfig_list   *Figure;
  fsmtrans_list *Trans;
{
  fsmstate_list  *State;
  chain_list     *ScanChain;
  chain_list    **PrevChain;

  if ( Trans != searchfsmtrans( Figure, Trans->FROM, Trans->TO ) )
  {
    return( 0 );
  }

  *Trans->PREV = Trans->NEXT;

  if ( Trans->NEXT != (fsmtrans_list *)0 )
  {
    Trans->NEXT->PREV = Trans->PREV;
  }

  freeablexpr( Trans->ABL );

  State = Trans->FROM;

  if ( State != (fsmstate_list *)0 )
  {
    PrevChain = &State->FROM;

    for ( ScanChain  = State->FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == (void *)Trans ) break;

      PrevChain = &ScanChain->NEXT;
    }

    if ( ScanChain != (chain_list *)0 )
    {
      *PrevChain      = ScanChain->NEXT;
      ScanChain->NEXT = (chain_list *)0;
      freechain( ScanChain );
    }
  }

  State = Trans->TO;

  if ( State != (fsmstate_list *)0 )
  {
    PrevChain = &State->TO;

    for ( ScanChain  = State->TO;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == (void *)Trans ) break;

      PrevChain = &ScanChain->NEXT;
    }

    if ( ScanChain != (chain_list *)0 )
    {
      *PrevChain      = ScanChain->NEXT;
      ScanChain->NEXT = (chain_list *)0;
      freechain( ScanChain );
    }
  }
  
  delauth2elem( Figure->HASH_TRANS, (char *)Trans->FROM, (char *)Trans->TO );

  freefsmtrans( Trans );

  Figure->NUMBER_TRANS--;

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Del Stack                        |
|                                                             |
\------------------------------------------------------------*/

int delfsmstack( Figure, Stack )

  fsmfig_list   *Figure;
  fsmstack_list *Stack;
{
  fsmstate_list  *State;
  chain_list     *ScanChain;
  chain_list    **PrevChain;

  *Stack->PREV = Stack->NEXT;

  if ( Stack->NEXT != (fsmstack_list *)0 )
  {
    Stack->NEXT->PREV = Stack->PREV;
  }

  freeablexpr( Stack->ABL );

  State = Stack->CURRENT;

  if ( State != (fsmstate_list *)0 )
  {
    PrevChain = &State->STACK;

    for ( ScanChain  = State->STACK;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == (void *)Stack ) break;

      PrevChain = &ScanChain->NEXT;
    }

    if ( ScanChain != (chain_list *)0 )
    {
      *PrevChain      = ScanChain->NEXT;
      ScanChain->NEXT = (chain_list *)0;
      freechain( ScanChain );
    }
  }

  freefsmstack( Stack );

  Figure->NUMBER_STACK--;

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                    Fsm Del Local Output                     |
|                                                             |
\------------------------------------------------------------*/

int delfsmlocout( State, Locout )

  fsmstate_list  *State;
  fsmlocout_list *Locout;
{
  fsmlocout_list  *ScanLocout;
  fsmlocout_list **PrevLocout;

  PrevLocout = &State->LOCOUT;
  ScanLocout = State->LOCOUT;

  while ( ScanLocout != (fsmlocout_list *)0 )
  {
    if ( ScanLocout == Locout ) break;

    PrevLocout = &ScanLocout->NEXT;
    ScanLocout = ScanLocout->NEXT;
  }

  if ( ScanLocout == (fsmlocout_list *)0 ) return( 0 );

  *PrevLocout = Locout->NEXT;

  freeablexpr( Locout->ABL );
  freeablexpr( Locout->ABL_DC );

  freefsmlocout( Locout );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Del State                        |
|                                                             |
\------------------------------------------------------------*/

int delfsmstate( Figure, State )

  fsmfig_list   *Figure;
  fsmstate_list *State;
{
  fsmlocout_list *DelLocout;
  fsmlocout_list *ScanLocout;
  fsmstate_list  *TransState;
  fsmtrans_list  *ScanTrans;
  fsmstack_list  *ScanStack;
  chain_list     *ChainTrans;
  chain_list     *ScanChain;
  chain_list    **PrevChain;

  *State->PREV = State->NEXT;

  if ( State->NEXT != (fsmstate_list *)0 )
  {
    State->NEXT->PREV = State->PREV;
  }

  if ( State == Figure->FIRST_STATE )
  {
    Figure->FIRST_STATE = (fsmstate_list *)0;
  }

  if ( State == Figure->STAR_STATE )
  {
    Figure->STAR_STATE = (fsmstate_list *)0;
  }

  ScanLocout = State->LOCOUT;

  while ( ScanLocout != (fsmlocout_list *)0 )
  {
    DelLocout = ScanLocout;
    ScanLocout = ScanLocout->NEXT;

    freeablexpr( DelLocout->ABL );
    freeablexpr( DelLocout->ABL_DC );

    freefsmlocout( DelLocout );
  }

  for ( ChainTrans  = State->FROM;
        ChainTrans != (chain_list *)0;
        ChainTrans  = ChainTrans->NEXT )
  {
    ScanTrans  = (fsmtrans_list *)ChainTrans->DATA;
    TransState = ScanTrans->TO;

    PrevChain = &TransState->TO;
    ScanChain = TransState->TO;

    while ( ScanChain != (chain_list *)0 )
    {
      if ( ScanChain->DATA == (void *)ScanTrans ) break;

      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;
    }

    if ( ScanChain != (chain_list *)0 )
    {
      *PrevChain      = ScanChain->NEXT;
      ScanChain->NEXT = (chain_list *)0;
      freechain( ScanChain );
    }

    *ScanTrans->PREV = ScanTrans->NEXT;

    if ( ScanTrans->NEXT != (fsmtrans_list *)0 )
    {
      ScanTrans->NEXT->PREV = ScanTrans->PREV;
    }

    freeablexpr( ScanTrans->ABL );

    delauth2elem( Figure->HASH_TRANS, (char *)ScanTrans->FROM, (char *)ScanTrans->TO );

    Figure->NUMBER_TRANS--;

    freefsmtrans( ScanTrans );
  }

  for ( ChainTrans  = State->TO;
        ChainTrans != (chain_list *)0;
        ChainTrans  = ChainTrans->NEXT )
  {
    ScanTrans  = (fsmtrans_list *)ChainTrans->DATA;
    TransState = ScanTrans->FROM;

    PrevChain = &TransState->FROM;
    ScanChain = TransState->FROM;

    while ( ScanChain != (chain_list *)0 )
    {
      if ( ScanChain->DATA == (void *)ScanTrans ) break;

      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;
    }

    if ( ScanChain != (chain_list *)0 )
    {
      *PrevChain      = ScanChain->NEXT;
      ScanChain->NEXT = (chain_list *)0;
      freechain( ScanChain );
    }

    *ScanTrans->PREV = ScanTrans->NEXT;

    if ( ScanTrans->NEXT != (fsmtrans_list *)0 )
    {
      ScanTrans->NEXT->PREV = ScanTrans->PREV;
    }

    freeablexpr( ScanTrans->ABL );

    delauth2elem( Figure->HASH_TRANS, (char *)ScanTrans->FROM, (char *)ScanTrans->TO );

    Figure->NUMBER_TRANS--;

    freefsmtrans( ScanTrans );
  }

  freechain( State->TO   );
  freechain( State->FROM );

  for ( ScanChain  = State->STACK;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanStack = (fsmstack_list *)ScanChain->DATA;

    *ScanStack->PREV = ScanStack->NEXT;

    if ( ScanStack->NEXT != (fsmstack_list *)0 )
    {
      ScanStack->NEXT->PREV = ScanStack->PREV;
    }

    freeablexpr( ScanStack->ABL );
    freefsmstack( ScanStack ); 
  }

  freechain( State->STACK );
  
  delauthelem( Figure->HASH_STATE, State->NAME );

  freefsmstate( State );

  Figure->NUMBER_STATE--;

  return( 1 );
}
