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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                    XMV.h                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XSCH_XMV
# define XSCH_XMV 

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

  extern char XschFirstViewLayer;

/*------------------------------------------------------------\
|                                                             |
|                        Panel And Menu                       |
|                                                             |
\------------------------------------------------------------*/

  extern XschMenuItem  XschViewMenu[];
  extern XschPanelItem XschViewArrowsPanel;
  extern XschPanelItem XschViewZoomPanel;
  extern XschPanelItem XschViewGridPanel;
  extern XschPanelItem XschViewLayerPanel;
  extern XschPanelItem XschViewMapPanel;
 
  extern int  XschPercentZoom;
  extern int  XschPercentMoveX;
  extern int  XschPercentMoveY;

  extern int  XschViewArrowsDefaultValues[5];
  extern int  XschViewZoomDefaultValues[5];
  extern int  XschViewLayerDefaultValues[5];
  extern int  XschViewGridDefaultValues[5];
  extern int  XschViewMapDefaultValues[5];
  
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

  extern void XschComputeAndDisplayMap();
  extern void XschInitializeZoom();
  extern void XschBuildViewDialog();
  extern void XschBuildPanelMap();
  extern void XschBuildPanelLayer();
  extern void XschInitializeMapEvent();

  extern void XschZoomCenter();
  extern void XschZoomRefresh();
  extern void XschZoomIn();
  extern void XschZoomPan();

  extern void XschPromptZoomIn();
  extern void XschPromptZoomPan();
  extern void XschPromptZoomCenter();

  extern void XschInitializeLayer();

# endif
