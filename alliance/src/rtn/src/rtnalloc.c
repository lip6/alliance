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
| Tool    :                     Rtl                           |
|                                                             |
| File    :                   rtnalloc.c                      |
|                                                             |
| Date    :                   04.07.96                        |
|                                                             |
| Rtlhor  :               Jacomme Ludovic                     |
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
# include "rtnalloc.h"
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
|                       Rtl Alloc Figure                      |
|                                                             |
\------------------------------------------------------------*/

rtlfig_list *allocrtlfig()
{
  return( (rtlfig_list *)autallocblock( sizeof( rtlfig_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Declaration                 |
|                                                             |
\------------------------------------------------------------*/

rtldecl_list *allocrtldecl()
{
  return( (rtldecl_list *)autallocheap( sizeof( rtldecl_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Assign                      |
|                                                             |
\------------------------------------------------------------*/

rtlasg_list *allocrtlasg()
{
  return( (rtlasg_list *)autallocheap( sizeof( rtlasg_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Bi Vex                      |
|                                                             |
\------------------------------------------------------------*/

rtlbivex_list *allocrtlbivex()
{
  return( (rtlbivex_list *)autallocheap( sizeof( rtlbivex_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Fsm                         |
|                                                             |
\------------------------------------------------------------*/

rtlfsm_list *allocrtlfsm()
{
  return( (rtlfsm_list *)autallocblock( sizeof( rtlfsm_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Fsm Trans                   |
|                                                             |
\------------------------------------------------------------*/

rtlfsmtrans_list *allocrtlfsmtrans()
{
  return( (rtlfsmtrans_list *)autallocheap( sizeof( rtlfsmtrans_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Fsm State                   |
|                                                             |
\------------------------------------------------------------*/

rtlfsmstate_list *allocrtlfsmstate()
{
  return( (rtlfsmstate_list *)autallocheap( sizeof( rtlfsmstate_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Fsm Assign                  |
|                                                             |
\------------------------------------------------------------*/

rtlfsmasg_list *allocrtlfsmasg()
{
  return( (rtlfsmasg_list *)autallocheap( sizeof( rtlfsmasg_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Symbol                      |
|                                                             |
\------------------------------------------------------------*/

rtlsym *allocrtlsym( Width )

  unsigned int Width;
{
  return( (rtlsym *)autallocblock( sizeof( rtlsym ) * Width ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Model                       |
|                                                             |
\------------------------------------------------------------*/

rtlmod_list *allocrtlmod()
{
  return( (rtlmod_list *)autallocheap( sizeof( rtlmod_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Instance                    |
|                                                             |
\------------------------------------------------------------*/

rtlins_list *allocrtlins()
{
  return( (rtlins_list *)autallocheap( sizeof( rtlins_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Alloc Port                        |
|                                                             |
\------------------------------------------------------------*/

rtlport_list *allocrtlport()
{
  return( (rtlport_list *)autallocheap( sizeof( rtlport_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Alloc Map                        |
|                                                             |
\------------------------------------------------------------*/

rtlmap_list *allocrtlmap()
{
  return( (rtlmap_list *)autallocheap( sizeof( rtlmap_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Alloc Map                        |
|                                                             |
\------------------------------------------------------------*/

rtlgen_list *allocrtlgen()
{
  return( (rtlgen_list *)autallocheap( sizeof( rtlgen_list ) ) );
}
