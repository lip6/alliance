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
| Tool    :                    XSCH                           |
|                                                             |
| File    :                   XMX.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
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

# ifndef XSCH_XMX
# define XSCH_XMX

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

# define XSCH_BLACK_COLOR_NAME         "Black"

/*------------------------------------------------------------\
|                                                             |
|                        Xsch Message                         |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_MAX_MESSAGE    7

# define XSCH_MESSAGE_X      0 
# define XSCH_MESSAGE_Y      1
# define XSCH_MESSAGE_DX     2
# define XSCH_MESSAGE_DY     3
# define XSCH_MESSAGE_MODE   4
# define XSCH_MESSAGE_INFO   5
# define XSCH_MESSAGE_PROMPT 6

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
|                          Xsch  Widget                       |
|                                                             |
\------------------------------------------------------------*/

  extern XtAppContext  XschApplication;
  extern Widget        XschTopLevel;
  extern Widget        XschMainWindow;
  extern Widget        XschMainForm;

  extern Widget        XschMenuBar;

  extern Widget        XschGraphicFrame;
  extern Widget        XschGraphicWindow;
  extern Display      *XschGraphicDisplay;

  extern Widget        XschMessageMainPaned;
  extern Widget        XschMessageMainForm;
  extern Widget        XschMessageForm  [ XSCH_MAX_MESSAGE ];
  extern Widget        XschMessageText  [ XSCH_MAX_MESSAGE ];
  extern Widget        XschMessageLabel [ XSCH_MAX_MESSAGE ];
  extern Widget        XschMessageFrame [ XSCH_MAX_MESSAGE ];

/*------------------------------------------------------------\
|                                                             |
|                       Xsch Graphic Context                  |
|                                                             |
\------------------------------------------------------------*/

  extern GC XschBackgroundGC;
  extern GC XschGridGC;
  extern GC XschXorGC;
  extern GC XschAcceptDrawGC;
  extern GC XschConnectDrawGC;
  extern GC XschLayerDrawGC[ XSCH_MAX_LAYER ];
  extern GC XschSmallTextGC;
  extern GC XschMediumTextGC;
  extern GC XschLargeTextGC;

  extern XFontStruct *XschSmallTextFont;
  extern XFontStruct *XschLargeTextFont;
  extern XFontStruct *XschMediumTextFont;

/*------------------------------------------------------------\
|                                                             |
|                       Xsch Graphic Pixmap                   |
|                                                             |
\------------------------------------------------------------*/

  extern Pixmap    XschGraphicPixmap;

/*------------------------------------------------------------\
|                                                             |
|                       Xsch Graphic Size                     |
|                                                             |
\------------------------------------------------------------*/

  extern Dimension XschOldGraphicDx;
  extern Dimension XschOldGraphicDy;
  extern Dimension XschGraphicDx;
  extern Dimension XschGraphicDy;

/*------------------------------------------------------------\
|                                                             |
|                          Xsch Unit Grid                     |
|                                                             |
\------------------------------------------------------------*/

  extern float XschUnitGridStep;
  extern long  XschUnitGridX;
  extern long  XschUnitGridY;
  extern long  XschUnitGridDx;
  extern long  XschUnitGridDy;
  extern long  XschPixelGridX;
  extern long  XschPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  extern long XschUnitUserGridDx;
  extern long XschUnitUserGridDy;
  extern int  XschUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                          Xsch Cursor                        |
|                                                             |
\------------------------------------------------------------*/

  extern long XschUnitCursorX;
  extern long XschUnitCursorY;

/*------------------------------------------------------------\
|                                                             |
|                          Xsch Event                         |
|                                                             |
\------------------------------------------------------------*/

  extern int  XschCountEventZoom;
  extern int  XschCountEventEdit;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XschInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

  extern void XschInitializeRessources();

/*------------------------------------------------------------\
|                                                             |
|                      Xsch Change Title                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XschChangeTopLevelTitle();

/*------------------------------------------------------------\
|                                                             |
|                      Xsch Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XschChangeEditMode();
  extern void XschContinueEditMode();

/*------------------------------------------------------------\
|                                                             |
|                      XschDisplayMessage                     |
|                                                             |
\------------------------------------------------------------*/

  extern void XschDisplayMessage();
  extern void XschPromptEditMeasure();

/*------------------------------------------------------------\
|                                                             |
|                       Xsch Graphic Window                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XschClearGraphicWindow();
  extern void XschResizeGraphicWindow();
  extern void XschRefreshGraphicWindow();

/*------------------------------------------------------------\
|                                                             |
|                         Xsch Unit Grid                      |
|                                                             |
\------------------------------------------------------------*/

  extern void    XschComputeUnitGrid();
  extern void    XschResizeUnitGrid();

/*------------------------------------------------------------\
|                                                             |
|                      Xsch Display Figure                    |
|                                                             |
\------------------------------------------------------------*/

  extern void XschDisplayFigure();
  extern void XschDisplayObject();

/*------------------------------------------------------------\
|                                                             |
|                         Xsch Cursor                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XschDisplayCursor();
  extern void XschUndisplayCursor();

# endif 
