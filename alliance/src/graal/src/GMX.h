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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  gmx101.h                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
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

# ifndef GRAAL_GMX
# define GRAAL_GMX

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

# define GRAAL_BLACK_COLOR    "Black"
# define GRAAL_WHITE_COLOR    "White"
# define GRAAL_TEXT_COLOR     "White"
# define GRAAL_ACCEPT_COLOR   "magenta"
# define GRAAL_EQUI_COLOR     "White"
# define GRAAL_DRUC_COLOR     "Gray"
# define GRAAL_CURSOR_COLOR   "Black"

/*------------------------------------------------------------\
|                                                             |
|                       Graal Message                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_MAX_MESSAGE    7

# define GRAAL_MESSAGE_X      0 
# define GRAAL_MESSAGE_Y      1
# define GRAAL_MESSAGE_DX     2
# define GRAAL_MESSAGE_DY     3
# define GRAAL_MESSAGE_MODE   4
# define GRAAL_MESSAGE_INFO   5
# define GRAAL_MESSAGE_PROMPT 6

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
|                          Graal Widget                       |
|                                                             |
\------------------------------------------------------------*/

  extern XtAppContext  GraalApplication;
  extern Widget        GraalTopLevel;
  extern Widget        GraalMainWindow;
  extern Widget        GraalMainForm;

  extern Widget        GraalMenuBar;

  extern Widget        GraalGraphicFrame;
  extern Widget        GraalGraphicWindow;
  extern Display      *GraalGraphicDisplay;

  extern Widget        GraalMessageMainPaned;
  extern Widget        GraalMessageMainForm;
  extern Widget        GraalMessageForm  [ GRAAL_MAX_MESSAGE ];
  extern Widget        GraalMessageText  [ GRAAL_MAX_MESSAGE ];
  extern Widget        GraalMessageLabel [ GRAAL_MAX_MESSAGE ];
  extern Widget        GraalMessageFrame [ GRAAL_MAX_MESSAGE ];

/*------------------------------------------------------------\
|                                                             |
|                      Graal Graphic Context                  |
|                                                             |
\------------------------------------------------------------*/

  extern GC GraalLayerFillGC   [ RDS_ALL_LAYER ];  
  extern GC GraalLayerDrawGC   [ RDS_ALL_LAYER ];  
  extern GC GraalLayerAcceptGC [ RDS_ALL_LAYER ];
  extern GC GraalLayerEquiGC   [ RDS_ALL_LAYER ];
  extern GC GraalLayerDrucGC   [ RDS_ALL_LAYER ];
  extern GC GraalAcceptDrawGC;
  extern GC GraalEquiDrawGC;
  extern GC GraalDrucDrawGC;
  extern GC GraalBackgroundGC;
  extern GC GraalGridGC;
  extern GC GraalXorGC;
  extern GC GraalSmallTextGC;
  extern GC GraalMediumTextGC;
  extern GC GraalLargeTextGC;

  extern XFontStruct *GraalSmallTextFont;
  extern XFontStruct *GraalLargeTextFont;
  extern XFontStruct *GraalMediumTextFont;

/*------------------------------------------------------------\
|                                                             |
|                      Graal Graphic Pixmap                   |
|                                                             |
\------------------------------------------------------------*/

  extern Pixmap    GraalGraphicPixmap;

/*------------------------------------------------------------\
|                                                             |
|                      Graal Colormap                         |
|                                                             |
\------------------------------------------------------------*/

  extern Colormap  GraalColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Graal Graphic Size                     |
|                                                             |
\------------------------------------------------------------*/

  extern Dimension GraalOldGraphicDx;
  extern Dimension GraalOldGraphicDy;
  extern Dimension GraalGraphicDx;
  extern Dimension GraalGraphicDy;

/*------------------------------------------------------------\
|                                                             |
|                       Graal Lambda Grid                     |
|                                                             |
\------------------------------------------------------------*/

  extern float GraalLambdaGridStep;
  extern long  GraalLambdaGridX;
  extern long  GraalLambdaGridY;
  extern long  GraalLambdaGridDx;
  extern long  GraalLambdaGridDy;
  extern long  GraalPixelGridX;
  extern long  GraalPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                      User Lambda Grid                       |
|                                                             |
\------------------------------------------------------------*/

  extern long GraalLambdaUserGridDx;
  extern long GraalLambdaUserGridDy;
  extern char GraalLambdaUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Cursor                        |
|                                                             |
\------------------------------------------------------------*/

  extern long GraalLambdaCursorX;
  extern long GraalLambdaCursorY;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Event                         |
|                                                             |
\------------------------------------------------------------*/

  extern char GraalCountEventZoom;
  extern char GraalCountEventEdit;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   GraalInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalInitializeRessources();

/*------------------------------------------------------------\
|                                                             |
|                     Graal Change Title                      |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalChangeTopLevelTitle();

/*------------------------------------------------------------\
|                                                             |
|                     Graal Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalChangeEditMode();
  extern void GraalContinueEditMode();

/*------------------------------------------------------------\
|                                                             |
|                     GraalDisplayMessage                     |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalDisplayMessage();
  extern void GraalPromptEditMeasure();

/*------------------------------------------------------------\
|                                                             |
|                      Graal Graphic Window                   |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalClearGraphicWindow();
  extern void GraalResizeGraphicWindow();
  extern void GraalRefreshGraphicWindow();

/*------------------------------------------------------------\
|                                                             |
|                      Graal Lambda Grid                      |
|                                                             |
\------------------------------------------------------------*/

  extern void    GraalComputeLambdaGrid();
  extern void    GraalResizeLambdaGrid();

/*------------------------------------------------------------\
|                                                             |
|                     Graal Display Figure                    |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalDisplayFigure();
  extern void GraalDisplayRectangle();

/*------------------------------------------------------------\
|                                                             |
|                        Graal Cursor                         |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalDisplayCursor();
  extern void GraalUndisplayCursor();

# endif 
