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
| File    :                  fsmsearch.c                      |
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
# include "fsmsearch.h"
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
|                      Fsm Search Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Fsm Search Figure                    |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *searchfsmfig( Name )

   char *Name;
{
  fsmfig_list *Figure;

  Name = namealloc( Name );

  for ( Figure  = HEAD_FSMFIG;
        Figure != (fsmfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;
  }

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Search State                     |
|                                                             |
\------------------------------------------------------------*/

fsmstate_list *searchfsmstate( Figure, Name )

   fsmfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_STATE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (fsmstate_list *)( Element->VALUE ) );
  }

  return( (fsmstate_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Search Output                    |
|                                                             |
\------------------------------------------------------------*/

fsmout_list *searchfsmout( Figure, Name )

   fsmfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_OUT, Name );

  if ( Element != (authelem *)0 )
  {
    return( (fsmout_list *)( Element->VALUE ) );
  }

  return( (fsmout_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Search Input                     |
|                                                             |
\------------------------------------------------------------*/

fsmin_list *searchfsmin( Figure, Name )

   fsmfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_IN, Name );

  if ( Element != (authelem *)0 )
  {
    return( (fsmin_list *)( Element->VALUE ) );
  }

  return( (fsmin_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                         Fsm Search Port                     |
|                                                             |
\------------------------------------------------------------*/

fsmport_list *searchfsmport( Figure, Name )

   fsmfig_list   *Figure;
   char          *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_PORT, Name );

  if ( Element != (authelem *)0 )
  {
    return( (fsmport_list *)( Element->VALUE ) );
  }

  return( (fsmport_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm Search Transition                   |
|                                                             |
\------------------------------------------------------------*/

fsmtrans_list *searchfsmtrans( Figure, StateFrom, StateTo )

   fsmfig_list   *Figure;
   fsmstate_list *StateFrom;
   fsmstate_list *StateTo;
{
  auth2elem  *Element;

  Element = searchauth2elem( Figure->HASH_TRANS, (char *)StateFrom, (char *)StateTo );

  if ( Element != (auth2elem *)0 )
  {
    return( (fsmtrans_list *)( Element->VALUE ) );
  }

  return( (fsmtrans_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm Search Stack                        |
|                                                             |
\------------------------------------------------------------*/

fsmstack_list *searchfsmstack( Control, Current, Return )

   char           Control;
   fsmstate_list *Current;
   fsmstate_list *Return;
{
  fsmstack_list *Stack;
  chain_list    *ScanChain;

  for ( ScanChain  = Current->STACK;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Stack = (fsmstack_list *)( ScanChain->DATA );

    if ( ( Stack->CTRL   == Control ) &&
         ( Stack->RETURN == Return  ) ) return( Stack );
  }

  return( (fsmstack_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm Search Local output                 |
|                                                             |
\------------------------------------------------------------*/

fsmlocout_list *searchfsmlocout( State, Out )

   fsmstate_list *State;
   fsmout_list   *Out;
{
  fsmlocout_list *Locout;

  for ( Locout  = State->LOCOUT;
        Locout != (fsmlocout_list *)0;
        Locout  = Locout->NEXT )
  {
    if ( Locout->OUT == Out ) break;
  }

  return( Locout );
}
