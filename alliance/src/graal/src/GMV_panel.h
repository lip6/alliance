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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Panel.h                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  25/02/93                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_VIEW_PANEL
# define GRAAL_VIEW_PANEL

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
 
# define GRAAL_VIEW_ARROWS_X     990
# define GRAAL_VIEW_ARROWS_Y      80
# define GRAAL_VIEW_ZOOM_X       990
# define GRAAL_VIEW_ZOOM_Y       230
# define GRAAL_VIEW_LAYER_X      430
# define GRAAL_VIEW_LAYER_Y       90
# define GRAAL_VIEW_GRID_X       470
# define GRAAL_VIEW_GRID_Y       330

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
|                       Callback View Panel                   |
|                                                             |
\------------------------------------------------------------*/

  extern void CallbackLeftArrow();
  extern void CallbackRightArrow();
  extern void CallbackUpArrow();
  extern void CallbackDownArrow();
  extern void CallbackMoveSet();
  extern void CallbackCloseArrows();

  extern void CallbackZoomRefresh();
  extern void CallbackZoomPrevious();
  extern void CallbackZoomLess();
  extern void CallbackZoomMore();
  extern void CallbackZoomSet();
  extern void CallbackZoomIn();
  extern void CallbackZoomFit();
  extern void CallbackZoomCenter();
  extern void CallbackZoomGoto();
  extern void CallbackZoomPan();
  extern void CallbackCloseZoom();

  extern void CallbackLayerAllVisible();
  extern void CallbackLayerAllInvisible();
  extern void CallbackNameVisible();
  extern void CallbackNameInvisible();
  extern void CallbackLayerVisible();
  extern void CallbackLayerInvisible();
  extern void CallbackLayerFillMode();
  extern void CallbackLayerInterface();
  extern void CallbackLayerIndex();
  extern void CallbackLayerCursor();
  extern void CallbackLayerForceDisplay();
  extern void CallbackLayerScale();
  extern void CallbackLayerBlackBoxString();
  extern void CallbackLayerApply();
  extern void CallbackCloseLayer();

  extern void GraalSetLayerVisible();
  extern void GraalSetLayerInvisible();
  extern void GraalSetNameVisible();
  extern void GraalSetNameInvisible();
  extern void GraalLayerAllVisible();
  extern void GraalLayerAllInvisible();
  extern void GraalChangeFillMode();
  extern void GraalChangeInterface();
  extern void GraalChangeIndex();
  extern void GraalChangeCursor();
  extern void GraalChangeForceDisplay();
  extern void GraalChangeScale();
  extern void GraalChangeBlackBoxString();

  extern void CallbackSetGridX();
  extern void CallbackSetGridY();
  extern void CallbackGridOnOff();
  extern void CallbackCloseGrid();

  extern void GraalInitializeLayer();

# endif 
