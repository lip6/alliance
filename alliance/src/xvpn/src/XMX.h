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
| Tool    :                    XVPN                           |
|                                                             |
| File    :                   XMX.h                           |
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
/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XVPN_XMX
# define XVPN_XMX

/*------------------------------------------------------------\
|                                                             |
|                         Graphic Context                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Colors Define                      |
|                                                             |
\------------------------------------------------------------*/

# define XVPN_BLACK_COLOR_NAME  "Black"

/*------------------------------------------------------------\
|                                                             |
|                        Xvpn Message                         |
|                                                             |
\------------------------------------------------------------*/

# define XVPN_MAX_MESSAGE    7

# define XVPN_MESSAGE_X      0 
# define XVPN_MESSAGE_Y      1
# define XVPN_MESSAGE_DX     2
# define XVPN_MESSAGE_DY     3
# define XVPN_MESSAGE_MODE   4
# define XVPN_MESSAGE_INFO   5
# define XVPN_MESSAGE_PROMPT 6

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
|                          Xvpn  Widget                       |
|                                                             |
\------------------------------------------------------------*/

  extern XtAppContext  XvpnApplication;
  extern Widget        XvpnTopLevel;
  extern Widget        XvpnMainWindow;
  extern Widget        XvpnMainForm;

  extern Widget        XvpnMenuBar;

  extern Widget        XvpnGraphicFrame;
  extern Widget        XvpnGraphicWindow;
  extern Display      *XvpnGraphicDisplay;

  extern Widget        XvpnMessageMainPaned;
  extern Widget        XvpnMessageMainForm;
  extern Widget        XvpnMessageForm  [ XVPN_MAX_MESSAGE ];
  extern Widget        XvpnMessageText  [ XVPN_MAX_MESSAGE ];
  extern Widget        XvpnMessageLabel [ XVPN_MAX_MESSAGE ];
  extern Widget        XvpnMessageFrame [ XVPN_MAX_MESSAGE ];

/*------------------------------------------------------------\
|                                                             |
|                       Xvpn Graphic Context                  |
|                                                             |
\------------------------------------------------------------*/

  extern GC XvpnBackgroundGC;
  extern GC XvpnGridGC;
  extern GC XvpnXorGC;
  extern GC XvpnAcceptDrawGC;
  extern GC XvpnConnectDrawGC;
  extern GC XvpnLayerDrawGC[ XVPN_MAX_LAYER ];
  extern GC XvpnSmallTextGC;
  extern GC XvpnMediumTextGC;
  extern GC XvpnLargeTextGC;

  extern XFontStruct *XvpnSmallTextFont;
  extern XFontStruct *XvpnLargeTextFont;
  extern XFontStruct *XvpnMediumTextFont;

/*------------------------------------------------------------\
|                                                             |
|                       Xvpn Graphic Pixmap                   |
|                                                             |
\------------------------------------------------------------*/

  extern Pixmap    XvpnGraphicPixmap;

/*------------------------------------------------------------\
|                                                             |
|                       Xvpn Graphic Size                     |
|                                                             |
\------------------------------------------------------------*/

  extern Dimension XvpnOldGraphicDx;
  extern Dimension XvpnOldGraphicDy;
  extern Dimension XvpnGraphicDx;
  extern Dimension XvpnGraphicDy;

/*------------------------------------------------------------\
|                                                             |
|                          Xvpn Unit Grid                     |
|                                                             |
\------------------------------------------------------------*/

  extern float XvpnUnitGridStep;
  extern long  XvpnUnitGridX;
  extern long  XvpnUnitGridY;
  extern long  XvpnUnitGridDx;
  extern long  XvpnUnitGridDy;
  extern long  XvpnPixelGridX;
  extern long  XvpnPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  extern long  XvpnUnitUserGridDx;
  extern long  XvpnUnitUserGridDy;
  extern short XvpnUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                          Xvpn Cursor                        |
|                                                             |
\------------------------------------------------------------*/

  extern long XvpnUnitCursorX;
  extern long XvpnUnitCursorY;

/*------------------------------------------------------------\
|                                                             |
|                          Xvpn Event                         |
|                                                             |
\------------------------------------------------------------*/

  extern short XvpnCountEventZoom;
  extern short XvpnCountEventEdit;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XvpnInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnInitializeRessources();

/*------------------------------------------------------------\
|                                                             |
|                      Xvpn Change Title                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnChangeTopLevelTitle();

/*------------------------------------------------------------\
|                                                             |
|                      Xvpn Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnChangeEditMode();
  extern void XvpnContinueEditMode();

/*------------------------------------------------------------\
|                                                             |
|                      XvpnDisplayMessage                     |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnDisplayMessage();
  extern void XvpnPromptEditIdentify();

/*------------------------------------------------------------\
|                                                             |
|                       Xvpn Graphic Window                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnClearGraphicWindow();
  extern void XvpnResizeGraphicWindow();
  extern void XvpnRefreshGraphicWindow();

/*------------------------------------------------------------\
|                                                             |
|                         Xvpn Unit Grid                      |
|                                                             |
\------------------------------------------------------------*/

  extern void    XvpnComputeUnitGrid();
  extern void    XvpnResizeUnitGrid();
  extern void    XvpnInitializeUnitGrid();

/*------------------------------------------------------------\
|                                                             |
|                      Xvpn Display Figure                    |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnDisplayFigure();
  extern void XvpnDisplayObject();

/*------------------------------------------------------------\
|                                                             |
|                         Xvpn Cursor                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XvpnDisplayCursor();
  extern void XvpnUndisplayCursor();

# endif 
