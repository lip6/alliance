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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  GMV101.h                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef DREAL_GMV
# define DREAL_GMV 

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

  extern char DrealFirstViewLayer;

/*------------------------------------------------------------\
|                                                             |
|                        Panel And Menu                       |
|                                                             |
\------------------------------------------------------------*/

  extern DrealMenuItem  DrealViewMenu[];
  extern DrealPanelItem DrealViewArrowsPanel;
  extern DrealPanelItem DrealViewZoomPanel;
  extern DrealPanelItem DrealViewGridPanel;
  extern DrealPanelItem DrealViewLayerPanel;
  extern DrealPanelItem DrealViewMapPanel;
 
  extern int  DrealPercentZoom;
  extern int  DrealPercentMove;

  extern int  DrealViewArrowsDefaultValues[5];
  extern int  DrealViewZoomDefaultValues[5];
  extern int  DrealViewLayerDefaultValues[5];
  extern int  DrealViewGridDefaultValues[5];
  extern int  DrealViewMapDefaultValues[5];
  
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Callback                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Zoom                             |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealComputeAndDisplayMap();
  extern void DrealInitializeZoom();
  extern void DrealBuildViewDialog();
  extern void DrealBuildPanelLayer();
  extern void DrealBuildPanelMap();
  extern void DrealInitializeMapEvent();


  extern void DrealZoomUndo();
  extern void DrealZoomLeft();
  extern void DrealZoomRight();
  extern void DrealZoomUp();
  extern void DrealZoomDown();
  extern void DrealZoomMore();
  extern void DrealZoomLess();
  extern void DrealZoomFit();
  extern void DrealZoomCenter();
  extern void DrealZoomRefresh();
  extern void DrealZoomIn();
  extern void DrealZoomPan();

  extern void DrealPromptZoomIn();
  extern void DrealPromptZoomPan();
  extern void DrealPromptZoomCenter();

# endif
