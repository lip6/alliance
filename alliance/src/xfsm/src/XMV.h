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
| Tool    :                   XFSM                           |
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

# ifndef XFSM_XMV
# define XFSM_XMV 

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

  extern char XfsmFirstViewLayer;

/*------------------------------------------------------------\
|                                                             |
|                        Panel And Menu                       |
|                                                             |
\------------------------------------------------------------*/

  extern XfsmMenuItem  XfsmViewMenu[];
  extern XfsmPanelItem XfsmViewArrowsPanel;
  extern XfsmPanelItem XfsmViewZoomPanel;
  extern XfsmPanelItem XfsmViewGridPanel;
  extern XfsmPanelItem XfsmViewLayerPanel;
  extern XfsmPanelItem XfsmViewMapPanel;
 
  extern int  XfsmPercentZoom;
  extern int  XfsmPercentMoveX;
  extern int  XfsmPercentMoveY;

  extern int  XfsmViewArrowsDefaultValues[5];
  extern int  XfsmViewZoomDefaultValues[5];
  extern int  XfsmViewLayerDefaultValues[5];
  extern int  XfsmViewGridDefaultValues[5];
  extern int  XfsmViewMapDefaultValues[5];
  
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

  extern void XfsmComputeAndDisplayMap();
  extern void XfsmInitializeZoom();
  extern void XfsmBuildViewDialog();
  extern void XfsmBuildPanelMap();
  extern void XfsmInitializeMapEvent();

  extern void XfsmZoomCenter();
  extern void XfsmZoomRefresh();
  extern void XfsmZoomIn();
  extern void XfsmZoomPan();

  extern void XfsmPromptZoomIn();
  extern void XfsmPromptZoomPan();
  extern void XfsmPromptZoomCenter();

# endif
