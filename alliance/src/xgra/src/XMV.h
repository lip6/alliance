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

# ifndef XGRA_XMV
# define XGRA_XMV 

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

  extern char XgraFirstViewLayer;

/*------------------------------------------------------------\
|                                                             |
|                        Panel And Menu                       |
|                                                             |
\------------------------------------------------------------*/

  extern XgraMenuItem  XgraViewMenu[];
  extern XgraPanelItem XgraViewArrowsPanel;
  extern XgraPanelItem XgraViewZoomPanel;
  extern XgraPanelItem XgraViewGridPanel;
  extern XgraPanelItem XgraViewLayerPanel;
  extern XgraPanelItem XgraViewMapPanel;
 
  extern int  XgraPercentZoom;
  extern int  XgraPercentMoveX;
  extern int  XgraPercentMoveY;

  extern int  XgraViewArrowsDefaultValues[5];
  extern int  XgraViewZoomDefaultValues[5];
  extern int  XgraViewLayerDefaultValues[5];
  extern int  XgraViewGridDefaultValues[5];
  extern int  XgraViewMapDefaultValues[5];
  
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

  extern void XgraComputeAndDisplayMap();
  extern void XgraInitializeZoom();
  extern void XgraBuildViewDialog();
  extern void XgraBuildPanelMap();
  extern void XgraInitializeMapEvent();

  extern void XgraZoomCenter();
  extern void XgraZoomRefresh();
  extern void XgraZoomIn();
  extern void XgraZoomPan();

  extern void XgraPromptZoomIn();
  extern void XgraPromptZoomPan();
  extern void XgraPromptZoomCenter();

# endif
