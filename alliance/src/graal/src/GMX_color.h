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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Color.h                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_COLOR
# define GRAAL_COLOR

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Pattern                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_PATTERN_WIDTH    16
# define GRAAL_PATTERN_HEIGHT   16
# define GRAAL_PATTERN_SQUARE   32
# define GRAAL_MAX_PATTERN      38

# define GRAAL_MAX_COLOR        ( RDS_ALL_LAYER + 7 )

# define GRAAL_LARGE_TEXT_FONT_NAME   "-*-fixed-medium-*-normal--15-*-*-*-*-90-iso10646-1"
# define GRAAL_MEDIUM_TEXT_FONT_NAME  "-*-fixed-medium-*-normal--13-*-*-*-*-70-iso10646-1"
# define GRAAL_SMALL_TEXT_FONT_NAME   "-*-fixed-medium-*-normal--8-*-*-*-*-50-iso10646-1"

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

  extern void GraalInitializeGraphicContext();
  extern void GraalInitializeColors();
  extern void GraalChangeCursorContext();
  extern void GraalInitializeColorMap();

# endif 
