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
| Tool    :                    XFSM                           |
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

# ifndef XFSM_XMX
# define XFSM_XMX

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

# define XFSM_BLACK_COLOR_NAME         "Black"

/*------------------------------------------------------------\
|                                                             |
|                        Xfsm Message                         |
|                                                             |
\------------------------------------------------------------*/

# define XFSM_MAX_MESSAGE    7

# define XFSM_MESSAGE_X      0 
# define XFSM_MESSAGE_Y      1
# define XFSM_MESSAGE_DX     2
# define XFSM_MESSAGE_DY     3
# define XFSM_MESSAGE_MODE   4
# define XFSM_MESSAGE_INFO   5
# define XFSM_MESSAGE_PROMPT 6

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
|                          Xfsm  Widget                       |
|                                                             |
\------------------------------------------------------------*/

  extern XtAppContext  XfsmApplication;
  extern Widget        XfsmTopLevel;
  extern Widget        XfsmMainWindow;
  extern Widget        XfsmMainForm;

  extern Widget        XfsmMenuBar;

  extern Widget        XfsmGraphicFrame;
  extern Widget        XfsmGraphicWindow;
  extern Display      *XfsmGraphicDisplay;

  extern Widget        XfsmMessageMainPaned;
  extern Widget        XfsmMessageMainForm;
  extern Widget        XfsmMessageForm  [ XFSM_MAX_MESSAGE ];
  extern Widget        XfsmMessageText  [ XFSM_MAX_MESSAGE ];
  extern Widget        XfsmMessageLabel [ XFSM_MAX_MESSAGE ];
  extern Widget        XfsmMessageFrame [ XFSM_MAX_MESSAGE ];

/*------------------------------------------------------------\
|                                                             |
|                       Xfsm Graphic Context                  |
|                                                             |
\------------------------------------------------------------*/

  extern GC XfsmBackgroundGC;
  extern GC XfsmGridGC;
  extern GC XfsmXorGC;
  extern GC XfsmAcceptDrawGC;
  extern GC XfsmConnectDrawGC;
  extern GC XfsmLayerDrawGC[ XFSM_MAX_LAYER ];
  extern GC XfsmSmallTextGC;
  extern GC XfsmMediumTextGC;
  extern GC XfsmLargeTextGC;

  extern XFontStruct *XfsmSmallTextFont;
  extern XFontStruct *XfsmLargeTextFont;
  extern XFontStruct *XfsmMediumTextFont;

/*------------------------------------------------------------\
|                                                             |
|                       Xfsm Graphic Pixmap                   |
|                                                             |
\------------------------------------------------------------*/

  extern Pixmap    XfsmGraphicPixmap;

/*------------------------------------------------------------\
|                                                             |
|                       Xfsm Graphic Size                     |
|                                                             |
\------------------------------------------------------------*/

  extern Dimension XfsmOldGraphicDx;
  extern Dimension XfsmOldGraphicDy;
  extern Dimension XfsmGraphicDx;
  extern Dimension XfsmGraphicDy;

/*------------------------------------------------------------\
|                                                             |
|                          Xfsm Unit Grid                     |
|                                                             |
\------------------------------------------------------------*/

  extern float XfsmUnitGridStep;
  extern long  XfsmUnitGridX;
  extern long  XfsmUnitGridY;
  extern long  XfsmUnitGridDx;
  extern long  XfsmUnitGridDy;
  extern long  XfsmPixelGridX;
  extern long  XfsmPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  extern long XfsmUnitUserGridDx;
  extern long XfsmUnitUserGridDy;
  extern char XfsmUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                          Xfsm Cursor                        |
|                                                             |
\------------------------------------------------------------*/

  extern long XfsmUnitCursorX;
  extern long XfsmUnitCursorY;

/*------------------------------------------------------------\
|                                                             |
|                          Xfsm Event                         |
|                                                             |
\------------------------------------------------------------*/

  extern char XfsmCountEventZoom;
  extern char XfsmCountEventEdit;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XfsmInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmInitializeRessources();

/*------------------------------------------------------------\
|                                                             |
|                      Xfsm Change Title                      |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmChangeTopLevelTitle();

/*------------------------------------------------------------\
|                                                             |
|                      Xfsm Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmChangeEditMode();
  extern void XfsmContinueEditMode();

/*------------------------------------------------------------\
|                                                             |
|                      XfsmDisplayMessage                     |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmDisplayMessage();
  extern void XfsmPromptEditMeasure();

/*------------------------------------------------------------\
|                                                             |
|                       Xfsm Graphic Window                   |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmClearGraphicWindow();
  extern void XfsmResizeGraphicWindow();
  extern void XfsmRefreshGraphicWindow();

/*------------------------------------------------------------\
|                                                             |
|                         Xfsm Unit Grid                      |
|                                                             |
\------------------------------------------------------------*/

  extern void    XfsmComputeUnitGrid();
  extern void    XfsmResizeUnitGrid();

/*------------------------------------------------------------\
|                                                             |
|                      Xfsm Display Figure                    |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmDisplayFigure();
  extern void XfsmDisplayObject();

/*------------------------------------------------------------\
|                                                             |
|                         Xfsm Cursor                         |
|                                                             |
\------------------------------------------------------------*/

  extern void XfsmDisplayCursor();
  extern void XfsmUndisplayCursor();

# endif 
