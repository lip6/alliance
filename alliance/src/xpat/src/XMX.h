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
| Tool    :                    XPAT                           |
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

# ifndef XPAT_XMX
# define XPAT_XMX

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

# define XPAT_BLACK_COLOR_NAME         "Black"

/*------------------------------------------------------------\
|                                                             |
|                        Xpat Message                         |
|                                                             |
\------------------------------------------------------------*/

# define XPAT_MAX_MESSAGE    6

# define XPAT_MESSAGE_X      0 
# define XPAT_MESSAGE_Y      1
# define XPAT_MESSAGE_DX     2
# define XPAT_MESSAGE_MODE   3
# define XPAT_MESSAGE_INFO   4
# define XPAT_MESSAGE_PROMPT 5

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
|                          Xpat  Widget                       |
|                                                             |
\------------------------------------------------------------*/

  extern XtAppContext  XpatApplication;
  extern Widget        XpatTopLevel;
  extern Widget        XpatMainWindow;
  extern Widget        XpatMainForm;

  extern Widget        XpatMenuBar;

  extern Widget        XpatGraphicFrame;
  extern Widget        XpatGraphicWindow;
  extern Display      *XpatGraphicDisplay;

  extern Widget        XpatMessageMainPaned;
  extern Widget        XpatMessageMainForm;
  extern Widget        XpatMessageForm  [ XPAT_MAX_MESSAGE ];
  extern Widget        XpatMessageText  [ XPAT_MAX_MESSAGE ];
  extern Widget        XpatMessageLabel [ XPAT_MAX_MESSAGE ];
  extern Widget        XpatMessageFrame [ XPAT_MAX_MESSAGE ];

/*------------------------------------------------------------\
|                                                             |
|                       Xpat Graphic Context                  |
|                                                             |
\------------------------------------------------------------*/

  extern GC XpatBackgroundGC;
  extern GC XpatGridGC;
  extern GC XpatXorGC;
  extern GC XpatAcceptDrawGC;
  extern GC XpatConnectDrawGC;
  extern GC XpatLayerDrawGC[ XPAT_MAX_LAYER ];
  extern GC XpatSmallTextGC;
  extern GC XpatMediumTextGC;
  extern GC XpatLargeTextGC;

  extern XFontStruct *XpatSmallTextFont;
  extern XFontStruct *XpatLargeTextFont;
  extern XFontStruct *XpatMediumTextFont;

/*------------------------------------------------------------\
|                                                             |
|                       Xpat Graphic Pixmap                   |
|                                                             |
\------------------------------------------------------------*/

  extern Pixmap    XpatGraphicPixmap;

/*------------------------------------------------------------\
|                                                             |
|                       Xpat Graphic Size                     |
|                                                             |
\------------------------------------------------------------*/

  extern Dimension XpatOldGraphicDx;
  extern Dimension XpatOldGraphicDy;
  extern Dimension XpatGraphicDx;
  extern Dimension XpatGraphicDy;

/*------------------------------------------------------------\
|                                                             |
|                          Xpat Unit Grid                     |
|                                                             |
\------------------------------------------------------------*/

  extern float XpatUnitGridStep;
  extern long  XpatUnitGridX;
  extern long  XpatUnitGridY;
  extern long  XpatUnitGridDx;
  extern long  XpatUnitGridDy;
  extern long  XpatPixelGridX;
  extern long  XpatPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  extern long XpatUnitUserGridDx;
  extern long XpatUnitUserGridDy;
  extern char XpatUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                          Xpat Cursor                        |
|                                                             |
\------------------------------------------------------------*/

  extern long XpatUnitCursorX;
  extern long XpatUnitCursorY;

/*------------------------------------------------------------\
|                                                             |
|                          Xpat Event                         |
|                                                             |
\------------------------------------------------------------*/

  extern char XpatCountEventZoom;
  extern char XpatCountEventEdit;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XpatInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatInitializeRessources();

/*------------------------------------------------------------\
|                                                             |
|                      Xpat Change Title                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatChangeTopLevelTitle();

/*------------------------------------------------------------\
|                                                             |
|                      Xpat Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatChangeEditMode();
  extern void XpatContinueEditMode();

/*------------------------------------------------------------\
|                                                             |
|                      XpatDisplayMessage                     |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatDisplayMessage();
  extern void XpatPromptEditMeasure();

/*------------------------------------------------------------\
|                                                             |
|                       Xpat Graphic Window                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatClearGraphicWindow();
  extern void XpatResizeGraphicWindow();
  extern void XpatRefreshGraphicWindow();

/*------------------------------------------------------------\
|                                                             |
|                         Xpat Unit Grid                      |
|                                                             |
\------------------------------------------------------------*/

  extern void    XpatComputeUnitGrid();
  extern void    XpatResizeUnitGrid();

/*------------------------------------------------------------\
|                                                             |
|                      Xpat Display Figure                    |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatDisplayFigure();
  extern void XpatDisplayObject();

/*------------------------------------------------------------\
|                                                             |
|                         Xpat Cursor                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XpatDisplayCursor();
  extern void XpatUndisplayCursor();

# endif 
