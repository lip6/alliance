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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XVPN                           |
|                                                             |
| File    :                    XMV.h                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XVPN_XMV
# define XVPN_XMV 

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

  extern short XvpnFirstViewLayer;

/*------------------------------------------------------------\
|                                                             |
|                        Panel And Menu                       |
|                                                             |
\------------------------------------------------------------*/

  extern XvpnMenuItem  XvpnViewMenu[];
  extern XvpnPanelItem XvpnViewArrowsPanel;
  extern XvpnPanelItem XvpnViewZoomPanel;
  extern XvpnPanelItem XvpnViewGridPanel;
  extern XvpnPanelItem XvpnViewLayerPanel;
  extern XvpnPanelItem XvpnViewMapPanel;
 
  extern int  XvpnPercentZoom;
  extern int  XvpnPercentMoveX;
  extern int  XvpnPercentMoveY;

  extern int  XvpnViewArrowsDefaultValues[5];
  extern int  XvpnViewZoomDefaultValues[5];
  extern int  XvpnViewLayerDefaultValues[5];
  extern int  XvpnViewGridDefaultValues[5];
  extern int  XvpnViewMapDefaultValues[5];
  
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

  extern void XvpnComputeAndDisplayMap();
  extern void XvpnInitializeZoom();
  extern void XvpnBuildViewDialog();
  extern void XvpnBuildPanelMap();
  extern void XvpnInitializeMapEvent();

  extern void XvpnZoomCenter();
  extern void XvpnZoomFit();
  extern void XvpnZoomIn();
  extern void XvpnZoomPan();
  extern void XvpnZoomUp();
  extern void XvpnZoomDown();
  extern void XvpnZoomLeft();
  extern void XvpnZoomRight();
  extern void XvpnZoomMore();
  extern void XvpnZoomLess();
  extern void XvpnZoomFit();
  extern void XvpnZoomRefresh();
  extern void XvpnZoomUndo();

  extern void XvpnPromptZoomIn();
  extern void XvpnPromptZoomPan();
  extern void XvpnPromptZoomCenter();

  extern void XvpnInitializeLayer();

# endif
