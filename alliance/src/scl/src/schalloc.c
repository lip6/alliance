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
| Tool    :                     Sch                           |
|                                                             |
| File    :                  schalloc.c                       |
|                                                             |
| Date    :                   04.03.98                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/


# include <stdio.h>
# include <string.h>
# include "schalloc.h"
# include "scherror.h"

# include "mut.h"
# include "mlo.h"
# include "aut.h"
# include "scl.h"


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

  schfig_list *HEAD_SCHFIG = (schfig_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Sch Alloc Figure                     |
|                                                             |
\------------------------------------------------------------*/
 
schfig_list *allocschfig()
{
  return( (schfig_list *)(autallocblock( sizeof( schfig_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Alloc Box                        |
|                                                             |
\------------------------------------------------------------*/

schbox_list *allocschbox()
{
  return( (schbox_list *)(autallocheap( sizeof( schbox_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Alloc Connector                  |
|                                                             |
\------------------------------------------------------------*/

schcon_list *allocschcon()
{
  return( (schcon_list *)(autallocheap( sizeof( schcon_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Alloc Net                        |
|                                                             |
\------------------------------------------------------------*/

schnet_list *allocschnet()
{
  return( (schnet_list *)(autallocheap( sizeof( schnet_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Alloc Wire                       |
|                                                             |
\------------------------------------------------------------*/

schwir_list *allocschwir()
{
  return( (schwir_list *)(autallocheap( sizeof( schwir_list ) ) ) );
}

