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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  gmx101.h                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
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

# ifndef DREAL_GMX
# define DREAL_GMX

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

# define DREAL_BLACK_COLOR    "Black"
# define DREAL_WHITE_COLOR    "White"
# define DREAL_TEXT_COLOR     "White"
# define DREAL_ACCEPT_COLOR   "magenta"
# define DREAL_CURSOR_COLOR   "Black"

/*------------------------------------------------------------\
|                                                             |
|                       Dreal Message                         |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_MAX_MESSAGE    7

# define DREAL_MESSAGE_X      0 
# define DREAL_MESSAGE_Y      1
# define DREAL_MESSAGE_DX     2
# define DREAL_MESSAGE_DY     3
# define DREAL_MESSAGE_MODE   4
# define DREAL_MESSAGE_INFO   5
# define DREAL_MESSAGE_PROMPT 6

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
|                          Dreal Widget                       |
|                                                             |
\------------------------------------------------------------*/

  extern XtAppContext  DrealApplication;
  extern Widget        DrealTopLevel;
  extern Widget        DrealMainWindow;
  extern Widget        DrealMainForm;

  extern Widget        DrealMenuBar;

  extern Widget        DrealGraphicFrame;
  extern Widget        DrealGraphicWindow;
  extern Display      *DrealGraphicDisplay;

  extern Widget        DrealMessageMainPaned;
  extern Widget        DrealMessageMainForm;
  extern Widget        DrealMessageForm  [ DREAL_MAX_MESSAGE ];
  extern Widget        DrealMessageText  [ DREAL_MAX_MESSAGE ];
  extern Widget        DrealMessageLabel [ DREAL_MAX_MESSAGE ];
  extern Widget        DrealMessageFrame [ DREAL_MAX_MESSAGE ];

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Graphic Context                  |
|                                                             |
\------------------------------------------------------------*/

  extern GC DrealLayerFillGC   [ RDS_ALL_LAYER ];  
  extern GC DrealLayerDrawGC   [ RDS_ALL_LAYER ];  
  extern GC DrealLayerAcceptGC [ RDS_ALL_LAYER ];
  extern GC DrealLayerEquiGC   [ RDS_ALL_LAYER ];
  extern GC DrealAcceptDrawGC;
  extern GC DrealEquiDrawGC;
  extern GC DrealBackgroundGC;
  extern GC DrealGridGC;
  extern GC DrealXorGC;
  extern GC DrealSmallTextGC;
  extern GC DrealMediumTextGC;
  extern GC DrealLargeTextGC;

  extern XFontStruct *DrealSmallTextFont;
  extern XFontStruct *DrealLargeTextFont;
  extern XFontStruct *DrealMediumTextFont;

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Graphic Pixmap                   |
|                                                             |
\------------------------------------------------------------*/

  extern Pixmap    DrealGraphicPixmap;
  
/*------------------------------------------------------------\
|                                                             |
|                      Dreal Colormap                         |
|                                                             |
\------------------------------------------------------------*/

  extern Colormap  DrealColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Graphic Size                     |
|                                                             |
\------------------------------------------------------------*/

  extern Dimension DrealOldGraphicDx;
  extern Dimension DrealOldGraphicDy;
  extern Dimension DrealGraphicDx;
  extern Dimension DrealGraphicDy;

/*------------------------------------------------------------\
|                                                             |
|                       Dreal Lambda Grid                     |
|                                                             |
\------------------------------------------------------------*/

  extern float DrealLambdaGridStep;
  extern long  DrealLambdaGridX;
  extern long  DrealLambdaGridY;
  extern long  DrealLambdaGridDx;
  extern long  DrealLambdaGridDy;
  extern long  DrealPixelGridX;
  extern long  DrealPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                      User Lambda Grid                       |
|                                                             |
\------------------------------------------------------------*/

  extern long DrealLambdaUserGridDx;
  extern long DrealLambdaUserGridDy;
  extern char DrealLambdaUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                         Dreal Cursor                        |
|                                                             |
\------------------------------------------------------------*/

  extern long DrealLambdaCursorX;
  extern long DrealLambdaCursorY;

/*------------------------------------------------------------\
|                                                             |
|                     Dreal Change Title                      |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealChangeTopLevelTitle();

/*------------------------------------------------------------\
|                                                             |
|                         Dreal Event                         |
|                                                             |
\------------------------------------------------------------*/

  extern char DrealCountEventZoom;
  extern char DrealCountEventEdit;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   DrealInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealInitializeRessources();

/*------------------------------------------------------------\
|                                                             |
|                     Dreal Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealChangeEditMode();
  extern void DrealContinueEditMode();

/*------------------------------------------------------------\
|                                                             |
|                     DrealDisplayMessage                     |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealDisplayMessage();
  extern void DrealPromptEditMeasure();

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Graphic Window                   |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealClearGraphicWindow();
  extern void DrealResizeGraphicWindow();
  extern void DrealRefreshGraphicWindow();

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Lambda Grid                      |
|                                                             |
\------------------------------------------------------------*/

  extern void    DrealComputeLambdaGrid();
  extern void    DrealResizeLambdaGrid();

/*------------------------------------------------------------\
|                                                             |
|                     Dreal Display Figure                    |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealDisplayFigure();
  extern void DrealDisplayRectangle();

/*------------------------------------------------------------\
|                                                             |
|                        Dreal Cursor                         |
|                                                             |
\------------------------------------------------------------*/

  extern void DrealDisplayCursor();
  extern void DrealUndisplayCursor();

# endif 
