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
| File    :                  GMV101.h                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_GMV
# define GRAAL_GMV 

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

  extern char GraalFirstViewLayer;

/*------------------------------------------------------------\
|                                                             |
|                        Panel And Menu                       |
|                                                             |
\------------------------------------------------------------*/

  extern GraalMenuItem  GraalViewMenu[];
  extern GraalPanelItem GraalViewArrowsPanel;
  extern GraalPanelItem GraalViewZoomPanel;
  extern GraalPanelItem GraalViewGridPanel;
  extern GraalPanelItem GraalViewLayerPanel;
  extern GraalPanelItem GraalViewMapPanel;
 
  extern int  GraalPercentZoom;
  extern int  GraalPercentMoveX;
  extern int  GraalPercentMoveY;

  extern int  GraalViewArrowsDefaultValues[5];
  extern int  GraalViewZoomDefaultValues[5];
  extern int  GraalViewLayerDefaultValues[5];
  extern int  GraalViewGridDefaultValues[5];
  extern int  GraalViewMapDefaultValues[5];
  
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Values                            |
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

  extern void GraalComputeAndDisplayMap();
  extern void GraalInitializeZoom();
  extern void GraalBuildViewDialog();
  extern void GraalBuildPanelMap();
  extern void GraalBuildPanelLayer();
  extern void GraalInitializeMapEvent();

  extern void GraalZoomCenter();
  extern void GraalZoomRefresh();
  extern void GraalZoomIn();
  extern void GraalZoomPan();

  extern void GraalZoomUndo();

  extern void GraalZoomLeft();
  extern void GraalZoomRight();
  extern void GraalZoomUp();
  extern void GraalZoomDown();

  extern void GraalZoomMore();
  extern void GraalZoomLess();

  extern void GraalZoomFit();

  extern void GraalPromptZoomIn();
  extern void GraalPromptZoomPan();
  extern void GraalPromptZoomCenter();

# endif
