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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                  vpnfree.c                        |
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
# include <string.h>
# include "vpnfree.h"
# include "vpnalloc.h"
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
|                     Vpn Free Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Vpn Free Figure                        |
|                                                             |
\------------------------------------------------------------*/
 
void freevpnfig( Figure )

  vpnfig_list *Figure;
{
  autfreeblock( (char *)Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Process                     |
|                                                             |
\------------------------------------------------------------*/

void freevpnproc( Process )

  vpnproc_list *Process;
{
  autfreeheap( (char *)Process, sizeof( vpnproc_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Function                    |
|                                                             |
\------------------------------------------------------------*/

void freevpnfunc( Function )

  vpnfunc_list *Function;
{
  autfreeheap( (char *)Function, sizeof( vpnfunc_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Place                       |
|                                                             |
\------------------------------------------------------------*/

void freevpnplace( Place )

  vpnplace_list *Place;
{
  autfreeheap( (char *)Place, sizeof( vpnplace_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Arc                         |
|                                                             |
\------------------------------------------------------------*/

void freevpnarc( Arc )

  vpnarc *Arc;
{
  autfreeheap( (char *)Arc, sizeof( vpnarc ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Action                      |
|                                                             |
\------------------------------------------------------------*/

void freevpnact( Act )

  vpnact_list *Act;
{
  autfreeheap( (char *)Act, sizeof( vpnact_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Symbol                      |
|                                                             |
\------------------------------------------------------------*/

void freevpnsym( Sym )

  vpnsym *Sym;
{
  autfreeblock( (char *)Sym );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Declaration                 |
|                                                             |
\------------------------------------------------------------*/

void freevpndecl( Decl )

  vpndecl_list *Decl;
{
  autfreeheap( (char *)Decl, sizeof( vpndecl_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Trans                       |
|                                                             |
\------------------------------------------------------------*/

void freevpntrans( Trans )

  vpntrans_list *Trans;
{
  autfreeheap( (char *)Trans, sizeof( vpntrans_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Attr                        |
|                                                             |
\------------------------------------------------------------*/

void freevpnattr( Attr )

  vpnattr_list *Attr;
{
  autfreeheap( (char *)Attr, sizeof( vpnattr_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Line                        |
|                                                             |
\------------------------------------------------------------*/

void freevpnline( Line )

  vpnline_list *Line;
{
  autfreeheap( (char *)Line, sizeof( vpnline_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Free Model                       |
|                                                             |
\------------------------------------------------------------*/

void freevpnmod( Model )

  vpnmod_list *Model;
{
  autfreeheap( (char *)Model, sizeof( vpnmod_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn Free Instance                       |
|                                                             |
\------------------------------------------------------------*/

void freevpnins( Instance )

  vpnins_list *Instance;
{
  autfreeheap( (char *)Instance, sizeof( vpnins_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Free Port                       |
|                                                             |
\------------------------------------------------------------*/

void freevpnport( Port )

  vpnport_list *Port;
{
  autfreeheap( (char *)Port, sizeof( vpnport_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Free Map                        |
|                                                             |
\------------------------------------------------------------*/

void freevpnmap( Map )

  vpnmap_list *Map;
{
  autfreeheap( (char *)Map, sizeof( vpnmap_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Free Generic                    |
|                                                             |
\------------------------------------------------------------*/

void freevpngen( Generic )

  vpngen_list *Generic;
{
  autfreeheap( (char *)Generic, sizeof( vpngen_list ));
}
