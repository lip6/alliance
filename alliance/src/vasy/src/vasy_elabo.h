/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                     VASY                           |
|                                                             |
| File    :                 vasy_elabo.h                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef VASY_ELABO_H
# define VASY_ELABO_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Tag                             |
|                                                             |
\------------------------------------------------------------*/

# define VASY_ELABORATE_TAG_MASK      (long)0x01

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyElaborateTag( O )   ((O)->FLAGS |=  VASY_ELABORATE_TAG_MASK)
# define ClearVasyElaborateTag( O ) ((O)->FLAGS &= ~VASY_ELABORATE_TAG_MASK)
# define IsVasyElaborateTag( O )    ((O)->FLAGS &   VASY_ELABORATE_TAG_MASK)

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct vasyelab
  {
    char           *VALUE;
    unsigned short  WIDTH;
    unsigned short  SIGNED;

  } vasyelab;

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

  extern void VasyElaborateVpnFig();

# endif
