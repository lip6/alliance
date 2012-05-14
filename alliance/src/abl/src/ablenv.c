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
| Tool    :                     Abl                           |
|                                                             |
| File    :                   ablenv.c                        |
|                                                             |
| Date    :                   03.12.96                        |
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

# include <stdio.h>
# include "ablis.h"
# include "ablenv.h"
# include "ablerror.h"

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

  char *ABL_OPERATOR_NAME[ ABL_MAX_OPERATOR ];

  char *ABL_OPERATOR_UPPER_NAME[ ABL_MAX_OPERATOR ] =
  {
    "OR",     /*  ABL_OR      */
    "AND",    /*  ABL_AND     */
    "XOR",    /*  ABL_XOR     */
    "NOT",    /*  ABL_NOT     */
    "NOR",    /*  ABL_NOR     */
    "NAND",   /*  ABL_NAND    */
    "NXOR",   /*  ABL_NXOR    */
    "STABLE", /*  ABL_STABLE  */
    "AF",     /*  ABL_AF      */
    "AG",     /*  ABL_AG      */
    "AX",     /*  ABL_AX      */
    "AU",     /*  ABL_AU      */
    "EF",     /*  ABL_EF      */
    "EG",     /*  ABL_EG      */
    "EX",     /*  ABL_EX      */
    "EU"      /*  ABL_EU      */
  };

  char *ABL_ATOM_NAME_ONE;
  char *ABL_ATOM_NAME_ZERO;
  char *ABL_ATOM_NAME_DC;
  char *ABL_ATOM_NAME_TRISTATE;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Abl Env                           |
|                                                             |
\------------------------------------------------------------*/

void ablenv()
{
  int Index;

  ABL_ATOM_NAME_ZERO     = namealloc( "'0'" );
  ABL_ATOM_NAME_ONE      = namealloc( "'1'" );
  ABL_ATOM_NAME_DC       = namealloc( "'d'" );
  ABL_ATOM_NAME_TRISTATE = namealloc( "'z'" );

  for ( Index = 0; Index < ABL_MAX_OPERATOR; Index++ )
  {
    ABL_OPERATOR_NAME[ Index ] = namealloc( ABL_OPERATOR_UPPER_NAME[ Index ] );
  }
}
