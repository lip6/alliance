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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                   Color.h                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef XSCH_COLOR
# define XSCH_COLOR

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

# define XSCH_MAX_COLOR              ( 5 + XSCH_MAX_LAYER )

# define XSCH_LARGE_TEXT_FONT_NAME   "-*-fixed-medium-*-normal--15-*-*-*-*-90-iso10646-1"
# define XSCH_MEDIUM_TEXT_FONT_NAME  "-*-fixed-medium-*-normal--13-*-*-*-*-70-iso10646-1"
# define XSCH_SMALL_TEXT_FONT_NAME   "-*-fixed-medium-*-normal--8-*-*-*-*-50-iso10646-1"

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

  extern void XschInitializeGraphicContext();
  extern void XschInitializeColors();
  extern void XschChangeCursorContext();
  extern void XschInitializeColorMap();

# endif 
