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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Rtn                           |
|                                                             |
| File    :                  rtnsearch.c                      |
|                                                             |
| Date    :                   04.07.96                        |
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
# include "vex.h"
# include "rtn.h"

# include <stdio.h>
# include "rtnsearch.h"
# include "rtnerror.h"

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
|                      Rtl Search Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Rtl Search Figure                    |
|                                                             |
\------------------------------------------------------------*/

rtlfig_list *searchrtlfig( Name )

   char *Name;
{
  rtlfig_list *Figure;

  Name = namealloc( Name );

  for ( Figure  = HEAD_RTLFIG;
        Figure != (rtlfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;
  }

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                      Rtl Search Fsm State                   |
|                                                             |
\------------------------------------------------------------*/

rtlfsmstate_list *searchrtlfsmstate( Figure, Fsm, Name )

  rtlfig_list  *Figure;
  rtlfsm_list  *Fsm;
  char         *Name;
{
  authelem *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Fsm->HASH_STATE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (rtlfsmstate_list *)Element->VALUE );
  }

  return( (rtlfsmstate_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Rtl Search Declar                      |
|                                                             |
\------------------------------------------------------------*/

rtldecl_list *searchrtldecl( Figure, Name )

  rtlfig_list  *Figure;
  char         *Name;
{
  authelem *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_DECLAR, Name );

  if ( Element != (authelem *)0 )
  {
    return( (rtldecl_list *)Element->VALUE );
  }

  return( (rtldecl_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Search Symbol                    |
|                                                             |
\------------------------------------------------------------*/

rtlsym *searchrtlsym( Figure, Name, Index )

  rtlfig_list    *Figure;
  char           *Name;
  short           Index;
{
  authtable      *HashTable;
  authelem       *Element;
  vexexpr        *Atom;
  rtldecl_list   *Decl;
  short           Bound;

  Name      = namealloc( Name );
  HashTable = Figure->HASH_DECLAR;
  Element   = searchauthelem( HashTable, Name );

  if ( Element != (authelem *)0 )
  {
    Decl  = (rtldecl_list *)( Element->VALUE );
    Atom  = Decl->VEX_ATOM;
    Bound = Atom->WIDTH - 1;

    if ( Index == -1 )
    {
      if ( Bound == 0 ) return( Decl->DECL_SYM );
    }
    else
    {
      if ( IsVexAtomDown( Atom ) )
      {
        if ( ( Index <= Atom->LEFT  ) &&
             ( Index >= Atom->RIGHT ) )
        {
          Index -= Atom->RIGHT;

          return( &Decl->DECL_SYM[ Bound - Index ] );
        }
      }
      else
      {
        if ( ( Index >= Atom->LEFT  ) &&
             ( Index <= Atom->RIGHT ) )
        {
          Index -= Atom->LEFT;

          return( &Decl->DECL_SYM[ Index ] );
        }
      }
    }
  }

  return( (rtlsym *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Search Model                     |
|                                                             |
\------------------------------------------------------------*/

rtlmod_list *searchrtlmod( Figure, Name )

   rtlfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_MODEL, Name );

  if ( Element != (authelem *)0 )
  {
    return( (rtlmod_list *)( Element->VALUE ) );
  }

  return( (rtlmod_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Search Model Port                |
|                                                             |
\------------------------------------------------------------*/

rtlport_list *searchrtlmodport( Figure, Model, Name  )

   rtlfig_list *Figure;
   rtlmod_list *Model;
   char        *Name;
{
  rtlport_list *RtlPort;
  char         *Value;


  Name = namealloc( Name );

  for ( RtlPort  = Model->PORT;
        RtlPort != (rtlport_list *)0;
        RtlPort  = RtlPort->NEXT )
  {
    Value = getvexarrayname( RtlPort->VEX_ATOM );

    if ( Value == Name ) break;
  }

  return( RtlPort );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Search Instance                  |
|                                                             |
\------------------------------------------------------------*/

rtlins_list *searchrtlins( Figure, Name )

   rtlfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_INSTANCE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (rtlins_list *)( Element->VALUE ) );
  }

  return( (rtlins_list *)0 );
}

