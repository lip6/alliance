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
| Tool    :                   XPAT                           |
|                                                             |
| File    :                   Map.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef XPAT_MAP
# define XPAT_MAP

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XPAT_B1UP   0
# define XPAT_B2UP   1
# define XPAT_B3UP   2

# define XPAT_VIEW_MAP_X        25
# define XPAT_VIEW_MAP_Y       520

# define XPAT_DEFAULT_MAP_X      0
# define XPAT_DEFAULT_MAP_Y      0
# define XPAT_DEFAULT_MAP_DX    40
# define XPAT_DEFAULT_MAP_DY    40

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
 
  extern Widget XpatMapPanel;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatComputeUnitMap();
  extern void XpatInitializeUnitMap();
  extern void XpatDisplayUnitMap();

  extern void XpatEnterMapPanel();
  extern void XpatClearMapWindow();
  extern void XpatResizeMapWindow();
  extern void XpatRefreshMapWindow();
  extern void XpatInitializeMapWindow();

  extern void CallbackCloseMap();
  extern void CallbackMapCompute();
  extern void CallbackMapResize();
  extern void CallbackMapExpose();
  extern void CallbackMapEvent();

# endif 
