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
| Tool    :                     Fsm                           |
|                                                             |
| File    :                   fsmadd.c                        |
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
# include "fsmadd.h"
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
|                      Fsm Add Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Fsm Add Figure                       |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *addfsmfig( Name )

   char *Name;
{
  fsmfig_list *Figure;

  Figure = allocfsmfig();

  Figure->NAME = namealloc( Name );
  Figure->NEXT = HEAD_FSMFIG;
  HEAD_FSMFIG  = Figure;

  Figure->HASH_PORT  = createauthtable( 100 );
  Figure->HASH_IN    = createauthtable( 100 );
  Figure->HASH_OUT   = createauthtable( 100 );
  Figure->HASH_STATE = createauthtable( 100 );
  Figure->HASH_TRANS = createauth2table( 100 );

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Add State                        |
|                                                             |
\------------------------------------------------------------*/

fsmstate_list *addfsmstate( Figure, Name )

   fsmfig_list *Figure;
   char        *Name;
{
  fsmstate_list *State;

  Name  = namealloc( Name );
  State = searchfsmstate( Figure, Name );

  if ( State != (fsmstate_list *)0 )
  {
    fsmerror( FSM_STATE_EXIST_ERROR, Name );
  }

  State = allocfsmstate();

  State->NAME    = namealloc( Name  );
  State->NEXT    = Figure->STATE;
  State->PREV    = &Figure->STATE;

  if ( Figure->STATE != (fsmstate_list *)0 )
  {
    Figure->STATE->PREV = &State->NEXT;
  }

  Figure->STATE  = State;

  Figure->NUMBER_STATE++;

  addauthelem( Figure->HASH_STATE, Name, (long)State );

  return( State );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Add Stack                        |
|                                                             |
\------------------------------------------------------------*/

fsmstack_list *addfsmstack( Figure, Control, Current, Return, Condition )

   fsmfig_list   *Figure;
   char           Control;
   fsmstate_list *Current;
   fsmstate_list *Return;
   ablexpr       *Condition;
{
  fsmstack_list *Stack;

  Stack = searchfsmstack( Control, Current, Return );

  if ( Stack == (fsmstack_list *)0 )
  {
    Stack = allocfsmstack();
  
    Current->STACK = addchain( Current->STACK, (void *)Stack );

    if ( Current == Figure->STAR_STATE )
    {
      SetFsmStarStack( Stack );
    }
  
    Stack->ABL     = Condition;
    Stack->CTRL    = Control;
    Stack->CURRENT = Current;
    Stack->RETURN  = Return;
    Stack->NEXT    = Figure->STACK;
    Stack->PREV    = &Figure->STACK;

    if ( Figure->STACK != (fsmstack_list *)0 )
    {
      Figure->STACK->PREV = &Stack->NEXT;
    }

    Figure->STACK = Stack;
  
    Figure->NUMBER_STACK++;
  }
  else
  if ( Condition != (ablexpr *)0 )
  {
    if ( Stack->ABL != (ablexpr *)0 )
    {
      Stack->ABL =

       createablbinexpr( ABL_OR, Condition, Stack->ABL );
    }
    else
    {
      Stack->ABL = Condition;
    }
  }

  return( Stack );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Add Transition                   |
|                                                             |
\------------------------------------------------------------*/

fsmtrans_list *addfsmtrans( Figure, StateFrom, StateTo, Condition )

   fsmfig_list   *Figure;
   fsmstate_list *StateFrom;
   fsmstate_list *StateTo;
   ablexpr       *Condition;
{
  fsmtrans_list *Trans;

  Trans = searchfsmtrans( Figure, StateFrom, StateTo );

  if ( Trans == (fsmtrans_list *)0 )
  {
    Trans = allocfsmtrans();

    StateFrom->FROM = addchain( StateFrom->FROM, (void *)Trans );
    StateTo->TO     = addchain( StateTo->TO,     (void *)Trans );

    Trans->ABL    = Condition;
    Trans->FROM   = StateFrom;
    Trans->TO     = StateTo;
    Trans->NEXT   = Figure->TRANS;
    Trans->PREV   = &Figure->TRANS;

    if ( Figure->TRANS != (fsmtrans_list *)0 )
    {
      Figure->TRANS->PREV = &Trans->NEXT;
    }

    Figure->TRANS = Trans;

    if ( ( StateFrom == Figure->STAR_STATE ) ||
         ( StateTo   == Figure->STAR_STATE ) )
    {
      SetFsmStarTrans( Trans );
    }

    Figure->NUMBER_TRANS++;

    addauth2elem( Figure->HASH_TRANS, (char *)StateFrom, (char *)StateTo, (long)Trans );
  }
  else
  if ( Condition != (ablexpr *)0 )
  {
    if ( Trans->ABL != (ablexpr *)0 )
    {
      Trans->ABL = 

       createablbinexpr( ABL_OR, Condition, Trans->ABL );
    }
    else
    {
      Trans->ABL = Condition;
    }
  }

  return( Trans );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm Add Local Output                    |
|                                                             |
\------------------------------------------------------------*/

fsmlocout_list *addfsmlocout( State, Output, Equation, EquationDC )

   fsmstate_list *State;
   fsmout_list   *Output;
   ablexpr       *Equation;
   ablexpr       *EquationDC;
{
  fsmlocout_list *Locout;

  Locout = searchfsmlocout( State, Output );

  if ( Locout == (fsmlocout_list *)0 )
  {
    Locout = allocfsmlocout();

    Locout->OUT    = Output;
    Locout->ABL    = Equation;
    Locout->ABL_DC = EquationDC;
    Locout->NEXT   = State->LOCOUT;
    State->LOCOUT  = Locout;
  }
  else
  {
    if ( Equation != (ablexpr *)0 )
    {
      if ( Locout->ABL != (ablexpr *)0 )
      {
        Locout->ABL = 

          createablbinexpr( ABL_OR, Equation, Locout->ABL );
      }
      else
      {
        Locout->ABL = Equation;
      }
    }

    if ( EquationDC != (ablexpr *)0 )
    {
      if ( Locout->ABL_DC != (ablexpr *)0 )
      {
        Locout->ABL_DC = 

         createablbinexpr( ABL_OR, EquationDC, Locout->ABL_DC );
      }
      else
      {
        Locout->ABL_DC = EquationDC;
      }
    }
  }

  return( Locout );
}

/*------------------------------------------------------------\
|                                                             |
|                         Fsm Add Pragma                      |
|                                                             |
\------------------------------------------------------------*/

fsmpragma_list *addfsmpragma( Figure, Type, Name, Value )

   fsmfig_list   *Figure;
   char          *Type;
   char          *Name;
   char          *Value;
{
  fsmpragma_list *Pragma;

  Type  = namealloc( Type  );
  Name  = namealloc( Name  );
  Value = namealloc( Value );

  Pragma = allocfsmpragma();

  Pragma->TYPE    = namealloc( Type  );
  Pragma->NAME    = namealloc( Name  );
  Pragma->VALUE   = namealloc( Value );
  Pragma->NEXT    = Figure->PRAGMA;
  Figure->PRAGMA  = Pragma;

  return( Pragma );
}

/*------------------------------------------------------------\
|                                                             |
|                         Fsm Add Output                      |
|                                                             |
\------------------------------------------------------------*/

fsmout_list *addfsmout( Figure, Name )

   fsmfig_list   *Figure;
   char          *Name;
{
  fsmout_list *Output;

  Name   = namealloc( Name );
  Output = searchfsmout( Figure, Name );

  if ( Output != (fsmout_list *)0 )
  {
    fsmerror( FSM_OUT_EXIST_ERROR, Name );
  }

  Output = allocfsmout();

  Output->NAME = namealloc( Name );
  Output->NEXT = Figure->OUT;
  Figure->OUT  = Output;

  Figure->NUMBER_OUT++;

  addauthelem( Figure->HASH_OUT, Name, (long)Output );

  return( Output );
}

/*------------------------------------------------------------\
|                                                             |
|                         Fsm Add Input                       |
|                                                             |
\------------------------------------------------------------*/

fsmin_list *addfsmin( Figure, Name )

   fsmfig_list   *Figure;
   char          *Name;
{
  fsmin_list *Input;

  Name  = namealloc( Name );
  Input = searchfsmin( Figure, Name );

  if ( Input != (fsmin_list *)0 )
  {
    fsmerror( FSM_IN_EXIST_ERROR, Name );
  }

  Input = allocfsmin();

  Input->NAME = namealloc( Name );
  Input->NEXT = Figure->IN;
  Figure->IN  = Input;

  Figure->NUMBER_IN++;

  addauthelem( Figure->HASH_IN, Name, (long)Input );

  return( Input );
}

/*------------------------------------------------------------\
|                                                             |
|                         Fsm Add Port                        |
|                                                             |
\------------------------------------------------------------*/

fsmport_list *addfsmport( Figure, Name, Dir, Type )

   fsmfig_list   *Figure;
   char          *Name;
   char           Dir;
   char           Type;
{
  fsmport_list *Port;

  Name = namealloc( Name );
  Port = searchfsmport( Figure, Name );

  if ( Port != (fsmport_list *)0 )
  {
    fsmerror( FSM_PORT_EXIST_ERROR, Name );
  }

  Port = allocfsmport();

  Port->NAME   = Name;
  Port->DIR    = Dir;
  Port->TYPE   = Type;
  Port->NEXT   = Figure->PORT;
  Figure->PORT = Port;

  Figure->NUMBER_PORT++;

  addauthelem( Figure->HASH_PORT, Name, (long)Port );

  return( Port );
}
