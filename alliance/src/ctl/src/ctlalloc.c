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
| Tool    :                     Ctl                           |
|                                                             |
| File    :                   ctlalloc.c                      |
|                                                             |
| Date    :                   04.18.02                        |
|                                                             |
| Ctlhor  :               Jacomme Ludovic                     |
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
# include "ctlalloc.h"
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
|                       Ctl Alloc Figure                      |
|                                                             |
\------------------------------------------------------------*/

ctlfig_list *allocctlfig()
{
  return( (ctlfig_list *)autallocblock( sizeof( ctlfig_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Ctl Alloc Process                     |
|                                                             |
\------------------------------------------------------------*/

ctlform_list *allocctlform()
{
  return( (ctlform_list *)autallocheap( sizeof( ctlform_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Ctl Alloc Symbol                      |
|                                                             |
\------------------------------------------------------------*/

ctlsym *allocctlsym( Width )

  unsigned int Width;
{
  return( (ctlsym *)autallocblock( sizeof( ctlsym ) * Width ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Ctl Alloc Declaration                 |
|                                                             |
\------------------------------------------------------------*/

ctldecl_list *allocctldecl()
{
  return( (ctldecl_list *)autallocheap( sizeof( ctldecl_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Ctl Alloc Line                        |
|                                                             |
\------------------------------------------------------------*/

ctlline_list *allocctlline()
{
  return( (ctlline_list *)autallocheap( sizeof( ctlline_list ) ) );
}

