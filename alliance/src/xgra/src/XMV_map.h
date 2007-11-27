/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XGRA                           |
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

# ifndef XGRA_MAP
# define XGRA_MAP

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XGRA_B1UP   0
# define XGRA_B2UP   1
# define XGRA_B3UP   2

# define XGRA_VIEW_MAP_X        25
# define XGRA_VIEW_MAP_Y       520

# define XGRA_DEFAULT_MAP_X      0
# define XGRA_DEFAULT_MAP_Y      0
# define XGRA_DEFAULT_MAP_DX    40
# define XGRA_DEFAULT_MAP_DY    40

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
 
  extern Widget XgraMapPanel;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraComputeUnitMap();
  extern void XgraInitializeUnitMap();
  extern void XgraDisplayUnitMap();

  extern void XgraEnterMapPanel();
  extern void XgraClearMapWindow();
  extern void XgraResizeMapWindow();
  extern void XgraRefreshMapWindow();
  extern void XgraInitializeMapWindow();

  extern void CallbackCloseMap();
  extern void CallbackMapCompute();
  extern void CallbackMapResize();
  extern void CallbackMapExpose();
  extern void CallbackMapEvent();

# endif 
