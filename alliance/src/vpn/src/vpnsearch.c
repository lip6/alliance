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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                  vpnsearch.c                      |
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
# include "vpn.h"

# include <stdio.h>
# include "vpnsearch.h"
# include "vpnerror.h"

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
|                      Vpn Search Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Figure                    |
|                                                             |
\------------------------------------------------------------*/

vpnfig_list *searchvpnfig( Name )

   char *Name;
{
  vpnfig_list *Figure;

  Name = namealloc( Name );

  for ( Figure  = HEAD_VPNFIG;
        Figure != (vpnfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;
  }

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Process                   |
|                                                             |
\------------------------------------------------------------*/

vpnproc_list *searchvpnproc( Figure, Name )

   vpnfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_PROCESS, Name );

  if ( Element != (authelem *)0 )
  {
    return( (vpnproc_list *)( Element->VALUE ) );
  }

  return( (vpnproc_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Function                  |
|                                                             |
\------------------------------------------------------------*/

vpnfunc_list *searchvpnfunc( Figure, Name )

   vpnfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_FUNCTION, Name );

  if ( Element != (authelem *)0 )
  {
    return( (vpnfunc_list *)( Element->VALUE ) );
  }

  return( (vpnfunc_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Model                     |
|                                                             |
\------------------------------------------------------------*/

vpnmod_list *searchvpnmod( Figure, Name )

   vpnfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_MODEL, Name );

  if ( Element != (authelem *)0 )
  {
    return( (vpnmod_list *)( Element->VALUE ) );
  }

  return( (vpnmod_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Instance                  |
|                                                             |
\------------------------------------------------------------*/

vpnins_list *searchvpnins( Figure, Name )

   vpnfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_INSTANCE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (vpnins_list *)( Element->VALUE ) );
  }

  return( (vpnins_list *)0 );
}


/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Place                     |
|                                                             |
\------------------------------------------------------------*/

vpnplace_list *searchvpnplace( Figure, Name )

   vpnfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_PLACE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (vpnplace_list *)( Element->VALUE ) );
  }

  return( (vpnplace_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Trans                     |
|                                                             |
\------------------------------------------------------------*/

vpntrans_list *searchvpntrans( Figure, Name )

   vpnfig_list *Figure;
   char        *Name;
{
  authelem  *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Figure->HASH_TRANS, Name );

  if ( Element != (authelem *)0 )
  {
    return( (vpntrans_list *)( Element->VALUE ) );
  }

  return( (vpntrans_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                  Local Vpn Search Symbol                    |
|                                                             |
\------------------------------------------------------------*/

vpnsym *searchvpnsym( Figure, Name, Index, Type )

  vpnfig_list    *Figure;
  char           *Name;
  short           Index;
  unsigned char   Type;
{
  authtable      *HashTable;
  authelem       *Element;
  vexexpr        *Atom;
  vpndecl_list   *Decl;
  short           Bound;

  if ( Type >= VPN_MAX_DECLAR_TYPE )
  {
    vpnerror( VPN_UNKNOWN_DECLAR_TYPE, Type, Name );
  }

  Name      = namealloc( Name );
  HashTable = Figure->HASH_DECLAR[ Type ];
  Element   = searchauthelem( HashTable, Name );

  if ( Element != (authelem *)0 )
  {
    Decl  = (vpndecl_list *)( Element->VALUE );
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

  return( (vpnsym *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Arc                       |
|                                                             |
\------------------------------------------------------------*/

vpnarc *searchvpnarc( Figure, Source, Target )

   vpnfig_list   *Figure;
   void          *Source;
   void          *Target;
{
  auth2elem  *Element;

  Element = searchauth2elem( Figure->HASH_ARC, Source, Target );

  if ( Element != (auth2elem *)0 )
  {
    return( (vpnarc *)( Element->VALUE ) );
  }

  return( (vpnarc *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Search Declaration               |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *searchvpndecl( Figure, Name, Type )

  vpnfig_list    *Figure;
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
    return( (vpndecl_list *)( Element->VALUE ) );
  }

  return( (vpndecl_list *)0 );
}
