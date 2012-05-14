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

# ifndef XPAT_XMV
# define XPAT_XMV 

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

  extern char XpatFirstViewLayer;

/*------------------------------------------------------------\
|                                                             |
|                        Panel And Menu                       |
|                                                             |
\------------------------------------------------------------*/

  extern XpatMenuItem  XpatViewMenu[];
  extern XpatPanelItem XpatViewArrowsPanel;
  extern XpatPanelItem XpatViewZoomPanel;
  extern XpatPanelItem XpatViewGridPanel;
  extern XpatPanelItem XpatViewLayerPanel;
  extern XpatPanelItem XpatViewMapPanel;
 
  extern int  XpatPercentZoom;
  extern int  XpatPercentMoveX;
  extern int  XpatPercentMoveY;

  extern int  XpatViewArrowsDefaultValues[5];
  extern int  XpatViewZoomDefaultValues[5];
  extern int  XpatViewLayerDefaultValues[5];
  extern int  XpatViewGridDefaultValues[5];
  extern int  XpatViewMapDefaultValues[5];
  
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

  extern void XpatComputeAndDisplayMap();
  extern void XpatInitializeZoom();
  extern void XpatBuildViewDialog();
  extern void XpatBuildPanelMap();
  extern void XpatInitializeMapEvent();

  extern void XpatZoomCenter();
  extern void XpatZoomRefresh();
  extern void XpatZoomIn();
  extern void XpatZoomPan();

  extern void XpatReScale();

  extern void XpatPromptZoomIn();
  extern void XpatPromptZoomPan();
  extern void XpatPromptZoomCenter();

# endif
