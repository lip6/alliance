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
| Tool    :                     Ctl                           |
|                                                             |
| File    :                  ctlsearch.c                      |
|                                                             |
| Date    :                   04.18.02                        |
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
# include "ctl.h"

# include <stdio.h>
# include "ctlsearch.h"
# include "ctlerror.h"

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
|                      Ctl Search Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Ctl Search Figure                    |
|                                                             |
\------------------------------------------------------------*/

ctlfig_list *searchctlfig( Name )

   char *Name;
{
  ctlfig_list *Figure;

  Name = namealloc( Name );

  for ( Figure  = HEAD_CTLFIG;
        Figure != (ctlfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;
  }

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                  Local Ctl Search Symbol                    |
|                                                             |
\------------------------------------------------------------*/

ctlsym *searchctlsym( Figure, Name, Index, Type )

  ctlfig_list    *Figure;
  char           *Name;
  short           Index;
  unsigned char   Type;
{
  authtable      *HashTable;
  authelem       *Element;
  vexexpr        *Atom;
  ctldecl_list   *Decl;
  short           Bound;

  if ( Type >= CTL_MAX_DECLAR_TYPE )
  {
    ctlerror( CTL_UNKNOWN_DECLAR_TYPE, Type, Name );
  }

  Name      = namealloc( Name );
  HashTable = Figure->HASH_DECLAR[ Type ];
  Element   = searchauthelem( HashTable, Name );

  if ( Element != (authelem *)0 )
  {
    Decl  = (ctldecl_list *)( Element->VALUE );
    Atom  = Decl->VEX_ATOM;

    if ( ! IsVexNodeAtom( Atom ) )
    {
      Atom = getvexarrayatom( Atom );
    }

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

  return( (ctlsym *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Search Declaration               |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *searchctldecl( Figure, Name, Type )

  ctlfig_list    *Figure;
  char           *Name;
  unsigned char   Type;
{
  authtable *HashTable;
  authelem  *Element;

  HashTable = Figure->HASH_DECLAR[ Type ];

  Name    = namealloc( Name );
  Element = searchauthelem( HashTable, Name );

  if ( Element != (authelem *)0 )
  {
    return( (ctldecl_list *)( Element->VALUE ) );
  }

  return( (ctldecl_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Search Form                      |
|                                                             |
\------------------------------------------------------------*/

ctlform_list *searchctlform( Figure, Name )

  ctlfig_list *Figure;
  char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_FORM, Name );

  if ( Element != (authelem *)0 )
  {
    return( (ctlform_list *)( Element->VALUE ) );
  }

  return( (ctlform_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Ctl Search Type                      |
|                                                             |
\------------------------------------------------------------*/

ctltype_list *searchctltype( Figure, Name )

  ctlfig_list *Figure;
  char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_TYPE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (ctltype_list *)( Element->VALUE ) );
  }

  return( (ctltype_list *)0 );
}

