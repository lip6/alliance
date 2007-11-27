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
| Tool    :                    XGRA                           |
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

# ifndef XGRA_XMX
# define XGRA_XMX

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

# define XGRA_BLACK_COLOR_NAME         "Black"

/*------------------------------------------------------------\
|                                                             |
|                        Xgra Message                         |
|                                                             |
\------------------------------------------------------------*/

# define XGRA_MAX_MESSAGE    7

# define XGRA_MESSAGE_X      0 
# define XGRA_MESSAGE_Y      1
# define XGRA_MESSAGE_DX     2
# define XGRA_MESSAGE_DY     3
# define XGRA_MESSAGE_MODE   4
# define XGRA_MESSAGE_INFO   5
# define XGRA_MESSAGE_PROMPT 6

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
|                          Xgra  Widget                       |
|                                                             |
\------------------------------------------------------------*/

  extern XtAppContext  XgraApplication;
  extern Widget        XgraTopLevel;
  extern Widget        XgraMainWindow;
  extern Widget        XgraMainForm;

  extern Widget        XgraMenuBar;

  extern Widget        XgraGraphicFrame;
  extern Widget        XgraGraphicWindow;
  extern Display      *XgraGraphicDisplay;

  extern Widget        XgraMessageMainPaned;
  extern Widget        XgraMessageMainForm;
  extern Widget        XgraMessageForm  [ XGRA_MAX_MESSAGE ];
  extern Widget        XgraMessageText  [ XGRA_MAX_MESSAGE ];
  extern Widget        XgraMessageLabel [ XGRA_MAX_MESSAGE ];
  extern Widget        XgraMessageFrame [ XGRA_MAX_MESSAGE ];

/*------------------------------------------------------------\
|                                                             |
|                       Xgra Graphic Context                  |
|                                                             |
\------------------------------------------------------------*/

  extern GC XgraBackgroundGC;
  extern GC XgraGridGC;
  extern GC XgraXorGC;
  extern GC XgraAcceptDrawGC;
  extern GC XgraConnectDrawGC;
  extern GC XgraLayerDrawGC[ XGRA_MAX_LAYER ];
  extern GC XgraSmallTextGC;
  extern GC XgraMediumTextGC;
  extern GC XgraLargeTextGC;

  extern XFontStruct *XgraSmallTextFont;
  extern XFontStruct *XgraLargeTextFont;
  extern XFontStruct *XgraMediumTextFont;

/*------------------------------------------------------------\
|                                                             |
|                       Xgra Graphic Pixmap                   |
|                                                             |
\------------------------------------------------------------*/

  extern Pixmap    XgraGraphicPixmap;

/*------------------------------------------------------------\
|                                                             |
|                       Xgra Graphic Size                     |
|                                                             |
\------------------------------------------------------------*/

  extern Dimension XgraOldGraphicDx;
  extern Dimension XgraOldGraphicDy;
  extern Dimension XgraGraphicDx;
  extern Dimension XgraGraphicDy;

/*------------------------------------------------------------\
|                                                             |
|                          Xgra Unit Grid                     |
|                                                             |
\------------------------------------------------------------*/

  extern float XgraUnitGridStep;
  extern long  XgraUnitGridX;
  extern long  XgraUnitGridY;
  extern long  XgraUnitGridDx;
  extern long  XgraUnitGridDy;
  extern long  XgraPixelGridX;
  extern long  XgraPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  extern long XgraUnitUserGridDx;
  extern long XgraUnitUserGridDy;
  extern char XgraUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                          Xgra Cursor                        |
|                                                             |
\------------------------------------------------------------*/

  extern long XgraUnitCursorX;
  extern long XgraUnitCursorY;

/*------------------------------------------------------------\
|                                                             |
|                          Xgra Event                         |
|                                                             |
\------------------------------------------------------------*/

  extern char XgraCountEventZoom;
  extern char XgraCountEventEdit;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XgraInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraInitializeRessources();

/*------------------------------------------------------------\
|                                                             |
|                      Xgra Change Title                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraChangeTopLevelTitle();

/*------------------------------------------------------------\
|                                                             |
|                      Xgra Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraChangeEditMode();
  extern void XgraContinueEditMode();

/*------------------------------------------------------------\
|                                                             |
|                      XgraDisplayMessage                     |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraDisplayMessage();
  extern void XgraPromptEditMeasure();

/*------------------------------------------------------------\
|                                                             |
|                       Xgra Graphic Window                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraClearGraphicWindow();
  extern void XgraResizeGraphicWindow();
  extern void XgraRefreshGraphicWindow();

/*------------------------------------------------------------\
|                                                             |
|                         Xgra Unit Grid                      |
|                                                             |
\------------------------------------------------------------*/

  extern void    XgraComputeUnitGrid();
  extern void    XgraResizeUnitGrid();

/*------------------------------------------------------------\
|                                                             |
|                      Xgra Display Figure                    |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraDisplayFigure();
  extern void XgraDisplayObject();

/*------------------------------------------------------------\
|                                                             |
|                         Xgra Cursor                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XgraDisplayCursor();
  extern void XgraUndisplayCursor();

# endif 
