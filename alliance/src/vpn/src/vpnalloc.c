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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                   vpnalloc.c                      |
|                                                             |
| Date    :                   04.07.96                        |
|                                                             |
| Vpnhor  :               Jacomme Ludovic                     |
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
|                       Vpn Alloc Figure                      |
|                                                             |
\------------------------------------------------------------*/

vpnfig_list *allocvpnfig()
{
  return( (vpnfig_list *)autallocblock( sizeof( vpnfig_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Process                     |
|                                                             |
\------------------------------------------------------------*/

vpnproc_list *allocvpnproc()
{
  return( (vpnproc_list *)autallocheap( sizeof( vpnproc_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Function                    |
|                                                             |
\------------------------------------------------------------*/

vpnfunc_list *allocvpnfunc()
{
  return( (vpnfunc_list *)autallocheap( sizeof( vpnfunc_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Trans                       |
|                                                             |
\------------------------------------------------------------*/

vpntrans_list *allocvpntrans()
{
  return( (vpntrans_list *)autallocheap( sizeof( vpntrans_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Place                       |
|                                                             |
\------------------------------------------------------------*/

vpnplace_list *allocvpnplace()
{
  return( (vpnplace_list *)autallocheap( sizeof( vpnplace_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Arc                         |
|                                                             |
\------------------------------------------------------------*/

vpnarc *allocvpnarc()
{
  return( (vpnarc *)autallocheap( sizeof( vpnarc ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Symbol                      |
|                                                             |
\------------------------------------------------------------*/

vpnsym *allocvpnsym( Width )

  unsigned int Width;
{
  return( (vpnsym *)autallocblock( sizeof( vpnsym ) * Width ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Declaration                 |
|                                                             |
\------------------------------------------------------------*/

vpndecl_list *allocvpndecl()
{
  return( (vpndecl_list *)autallocheap( sizeof( vpndecl_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Action                      |
|                                                             |
\------------------------------------------------------------*/

vpnact_list *allocvpnact()
{
  return( (vpnact_list *)autallocheap( sizeof( vpnact_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Attribute                   |
|                                                             |
\------------------------------------------------------------*/

vpnattr_list *allocvpnattr()
{
  return( (vpnattr_list *)autallocheap( sizeof( vpnattr_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Line                        |
|                                                             |
\------------------------------------------------------------*/

vpnline_list *allocvpnline()
{
  return( (vpnline_list *)autallocheap( sizeof( vpnline_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Model                       |
|                                                             |
\------------------------------------------------------------*/

vpnmod_list *allocvpnmod()
{
  return( (vpnmod_list *)autallocheap( sizeof( vpnmod_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Instance                    |
|                                                             |
\------------------------------------------------------------*/

vpnins_list *allocvpnins()
{
  return( (vpnins_list *)autallocheap( sizeof( vpnins_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Vpn Alloc Port                        |
|                                                             |
\------------------------------------------------------------*/

vpnport_list *allocvpnport()
{
  return( (vpnport_list *)autallocheap( sizeof( vpnport_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Alloc Map                        |
|                                                             |
\------------------------------------------------------------*/

vpnmap_list *allocvpnmap()
{
  return( (vpnmap_list *)autallocheap( sizeof( vpnmap_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Alloc Generic                    |
|                                                             |
\------------------------------------------------------------*/

vpngen_list *allocvpngen()
{
  return( (vpngen_list *)autallocheap( sizeof( vpngen_list ) ) );
}
