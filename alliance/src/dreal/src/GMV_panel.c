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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Panel.c                         |
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

# include <stdio.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GTB.h"
# include "GMX.h"
# include "GMV.h"

# include "GMV_panel.h"
# include "GMV_dialog.h"
# include "GMV_view.h"
# include "GMV_message.h"
# include "GMX_color.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
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
|                        Panel Pixmap                         |
|                                                             |
\------------------------------------------------------------*/

  static char DrealPixmapDownArrow[] =

  { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xfe, 0xff, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00,
    0x00, 0x02, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00,
    0x00, 0x02, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00,
    0xf0, 0x03, 0x80, 0x1f, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10,
    0x10, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x08,
    0x40, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x02, 0x80, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x08, 0x20, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x40, 0x04, 0x00,
    0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  static char DrealPixmapLeftArrow[] =

  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x0f, 0x00, 0x00, 0x40, 0x08, 0x00, 0x00, 0x20, 0x08, 0x00,
    0x00, 0x10, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x04, 0xf8, 0x1f,
    0x00, 0x02, 0x00, 0x10, 0x00, 0x01, 0x00, 0x10, 0x80, 0x00, 0x00, 0x10,
    0x40, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10,
    0x08, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x10,
    0x40, 0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x10,
    0x00, 0x02, 0x00, 0x10, 0x00, 0x04, 0xf8, 0x1f, 0x00, 0x08, 0x08, 0x00,
    0x00, 0x10, 0x08, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x40, 0x08, 0x00,
    0x00, 0x80, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  static char DrealPixmapRightArrow[] =

  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xf0, 0x01, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x10, 0x04, 0x00,
    0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x10, 0x00, 0xf8, 0x1f, 0x20, 0x00,
    0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x80, 0x00, 0x08, 0x00, 0x00, 0x01,
    0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x08,
    0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x04,
    0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x01, 0x08, 0x00, 0x80, 0x00,
    0x08, 0x00, 0x40, 0x00, 0xf8, 0x1f, 0x20, 0x00, 0x00, 0x10, 0x10, 0x00,
    0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 0x02, 0x00,
    0x00, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  static char DrealPixmapUpArrow[] =

  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x20, 0x02, 0x00,
    0x00, 0x10, 0x04, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x04, 0x10, 0x00,
    0x00, 0x02, 0x20, 0x00, 0x00, 0x01, 0x40, 0x00, 0x80, 0x00, 0x80, 0x00,
    0x40, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x04,
    0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x08,
    0x08, 0x00, 0x00, 0x08, 0xf8, 0x01, 0xc0, 0x0f, 0x00, 0x01, 0x40, 0x00,
    0x00, 0x01, 0x40, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x01, 0x40, 0x00,
    0x00, 0x01, 0x40, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x01, 0x40, 0x00,
    0x00, 0x01, 0x40, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Arrows                         |
|                                                             |
\------------------------------------------------------------*/

   static DrealPanelButtonItem DrealViewArrowsButton[] =

         {
           {
             "Left",
             NULL, DrealPixmapLeftArrow, 32, 32,
             NULL,
             NULL,
             0, 0,
             1, 2,
             CallbackLeftArrow,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Up",
             NULL, DrealPixmapUpArrow, 32, 32,
             NULL,
             NULL,
             1, 0,
             1, 2,
             CallbackUpArrow,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Down",
             NULL, DrealPixmapDownArrow, 32, 32,
             NULL,
             NULL,
             0, 2,
             1, 2,
             CallbackDownArrow,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Right",
             NULL, DrealPixmapRightArrow, 32, 32,
             NULL,
             NULL,
             1, 2,
             1, 2,
             CallbackRightArrow,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Move Set",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 4,
             2, 1,
             CallbackMoveSet,
             (XtPointer)NULL,
             (Widget)NULL
           } 
           , 
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             2, 1,
             CallbackCloseArrows,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   DrealPanelItem DrealViewArrowsPanel =

         {
           "Arrows",
           1,
           1,
           DREAL_VIEW_ARROWS_X,
           DREAL_VIEW_ARROWS_Y, 
           90,
           125,
           2,  
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           6,
           DrealViewArrowsButton
         };

   int DrealViewArrowsDefaultValues[ 5 ] =

         {
           DREAL_VIEW_ARROWS_X, 
           DREAL_VIEW_ARROWS_Y,
           90, 125, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Zoom                           |
|                                                             |
\------------------------------------------------------------*/

 DrealPanelButtonItem DrealViewZoomButton[] =

         {
           {
             "Refresh",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackZoomRefresh,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "UnZoom",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackZoomPrevious,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Zoom",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackZoomMore,
             (XtPointer)NULL,
             (Widget)NULL
           } 
           ,
           {
             "Mooz",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackZoomLess,
             (XtPointer)NULL,
             (Widget)NULL
           } 
           ,
           {
             "Zoom Set",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 4,
             1, 1,
             CallbackZoomSet,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Zoom In",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             1, 1,
             CallbackZoomIn,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Center",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 6,
             1, 1,
             CallbackZoomCenter,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Goto",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 7,
             1, 1,
             CallbackZoomGoto,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Pan",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 8,
             1, 1,
             CallbackZoomPan,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Fit",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 9,
             1, 1,
             CallbackZoomFit,
             (XtPointer)NULL,
             (Widget)NULL
           } 
           , 
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 10,
             1, 1,
             CallbackCloseZoom,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   DrealPanelItem DrealViewZoomPanel =

         {
           "Zoom",
           1,
           1,
           DREAL_VIEW_ZOOM_X,
           DREAL_VIEW_ZOOM_Y, 
           90,
           275,
           1,  
           11,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           11,
           DrealViewZoomButton
         };

   int DrealViewZoomDefaultValues[ 5 ] =

         {
           DREAL_VIEW_ZOOM_X, 
           DREAL_VIEW_ZOOM_Y,
           90, 275, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Layer                          |
|                                                             |
\------------------------------------------------------------*/

  static char *DrealViewLayerFillModeName[] =

  {
    "Outline",
    "Fill",
    "Pattern"
  };

  static char *DrealViewLayerInterfaceName[] =

  {
    "No interface",
    "Interface"
  };

  static char *DrealViewLayerCursorType[] =

  {
    "Invert",
    "Xor"
  };

  static char *DrealViewLayerForceDisplay[] =

  {
    "Quick display",
    "Force display"
  };

  static char *DrealViewLayerBlackBoxString[] =

  {
    "No string box",
    "String box"
  };

     DrealPanelButtonItem DrealViewLayerButton[ ( RDS_ALL_LAYER << 1 ) + 
                                                ( DREAL_MAX_ACTIVE_NAME << 1 ) + 10 ] =
         {
           {
             "All visible",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackLayerAllVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "All invisible",
             NULL, NULL, 0, 0, 
             NULL, 
             NULL, 
             2, 0, 
             2, 1, 
             CallbackLayerAllInvisible, 
             (XtPointer)NULL, 
             (Widget)NULL 
           } 
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,

           {
             NULL,
             NULL, NULL, 0, 0, 
             NULL,
             NULL,
             0, 0, 
             1, 1, 
             CallbackLayerVisible, 
             (XtPointer)NULL,
             (Widget)NULL 
           }
           , 
           {
             NULL,
             NULL, NULL, 0, 0, 
             NULL,
             NULL,
             0, 0, 
             1, 1, 
             CallbackLayerInvisible, 
             (XtPointer)NULL,
             (Widget)NULL 
           } 
           , 
           {
             "Fig",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_FIGURE_NAME,
             (Widget)NULL
           }
           ,
           {
             "Fig",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_FIGURE_NAME,
             (Widget)NULL
           }
           ,
           {
             "Inst",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_INSTANCE_NAME,
             (Widget)NULL
           }
           ,
           {
             "Inst",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_INSTANCE_NAME,
             (Widget)NULL
           }
           ,
           {
             "FCon",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_FIGURE_CONNECTOR_NAME,
             (Widget)NULL
           }
           ,
           {
             "FCon",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_FIGURE_CONNECTOR_NAME,
             (Widget)NULL
           }
           ,
           {
             "ICon",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_INSTANCE_CONNECTOR_NAME,
             (Widget)NULL
           }
           ,
           {
             "ICon",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_INSTANCE_CONNECTOR_NAME,
             (Widget)NULL
           }
           ,
           {
             "FSeg",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_FIGURE_SEGMENT_NAME,
             (Widget)NULL
           }
           ,
           {
             "FSeg",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_FIGURE_SEGMENT_NAME,
             (Widget)NULL
           }
           ,
           {
             "ISeg",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_INSTANCE_SEGMENT_NAME,
             (Widget)NULL
           }
           ,
           {
             "ISeg",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_INSTANCE_SEGMENT_NAME,
             (Widget)NULL
           }
           ,
           {
             "FRef",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_FIGURE_REFERENCE_NAME,
             (Widget)NULL
           }
           ,
           {
             "FRef",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_FIGURE_REFERENCE_NAME,
             (Widget)NULL
           }
           ,
           {
             "IRef",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)DREAL_INSTANCE_REFERENCE_NAME,
             (Widget)NULL
           }
           ,
           {
             "IRef",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)DREAL_INSTANCE_REFERENCE_NAME,
             (Widget)NULL
           }
           ,
           {
             "Pattern",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackLayerFillMode,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Interface",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             2, 0,
             2, 1,
             CallbackLayerInterface,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Invert",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackLayerCursor,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Quick display",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             2, 0,
             2, 1,
             CallbackLayerForceDisplay,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "No string box",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackLayerBlackBoxString,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             2, 0,
             2, 1,
             CallbackLayerCursor,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Apply",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackLayerApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             2, 0,
             2, 1,
             CallbackCloseLayer,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };


     DrealPanelItem DrealViewLayerPanel =

         {
           "Layer",
           1,
           0,
           DREAL_VIEW_LAYER_X,
           DREAL_VIEW_LAYER_Y,
           180,
           550,
           4,
           23,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           ( RDS_ALL_LAYER         << 1 ) + 
           ( DREAL_MAX_ACTIVE_NAME << 1 ) + 10,
           DrealViewLayerButton
         };

  int DrealViewLayerDefaultValues[ 5 ] =

         {
           DREAL_VIEW_LAYER_X,
           DREAL_VIEW_LAYER_Y,
           180, 550, 0
         };

   static char FirstEnterLayerPanel = 1;

/*------------------------------------------------------------\
|                                                             |
|                        Panel Grid                           |
|                                                             |
\------------------------------------------------------------*/

 DrealPanelButtonItem DrealViewGridButton[] =

         {
           {
             "Grid X",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackSetGridX,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             "Grid Y",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackSetGridY,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           {
             "Grid Off",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackGridOnOff,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackCloseGrid,
             (XtPointer)NULL,
             (Widget)NULL   
           }
         };

     DrealPanelItem DrealViewGridPanel =

         {
           "Grid",
           1,
           0,
           DREAL_VIEW_GRID_X,
           DREAL_VIEW_GRID_Y, 
           90,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           4,
           DrealViewGridButton
         };

  int DrealViewGridDefaultValues[ 5 ] =

         {
           DREAL_VIEW_GRID_X,
           DREAL_VIEW_GRID_Y, 
           90, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealBuildPanelLayer                   |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildPanelLayer()
{
  DrealPanelButtonItem *Button;
  unsigned char         Visible;
  long                  Layer;
  unsigned char         Index;
  unsigned char         X1;
  unsigned char         Y1;

  X1    = 0;
  Y1    = 1;
  Index = 2;

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    if ( ( DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED   ) )
    {
      for ( Visible = 0; Visible < 2; Visible++ )
      {
        Button = &DrealViewLayerButton[ Index + Visible ];

        Button->LABEL      = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ];
        Button->FOREGROUND = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 1 ];
        Button->BACKGROUND = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 2 ];

        Button->X        = X1;
        Button->Y        = Y1;
        Button->CALLDATA = (XtPointer)Layer;

        if ( X1 == 3 ) 
        {
          X1 = 0; Y1 = Y1 + 1;
        }
        else
        {
          X1 = X1 + 1;
        }
      }
    }

    Index = Index + 2;
  }

  if ( X1 > 0 )
  {
    X1 = 0;
    Y1 = Y1 + 1;
  }

  for ( Layer = 0; Layer < DREAL_MAX_ACTIVE_NAME; Layer++ )
  {
    for ( Visible = 0; Visible < 2; Visible++ )
    {
      Button = &DrealViewLayerButton[ Index + Visible ];

      Button->X = X1;
      Button->Y = Y1;

      Button->FOREGROUND = "White";
      Button->BACKGROUND = "Black";
      

      if ( X1 == 3 ) 
      {
        X1 = 0; Y1 = Y1 + 1;
      }
      else
      {
        X1 = X1 + 1;
      }
    }

    Index = Index + 2;
  }

  if ( X1 > 0 ) Y1 = Y1 + 1;

  for ( Layer = 0; Layer < 4; Layer++ )
  {
    DrealViewLayerButton[ Index     ].Y = Y1;;
    DrealViewLayerButton[ Index + 1 ].Y = Y1;;

    Index = Index + 2;
    Y1    = Y1    + 1;
  }

  DrealViewLayerPanel.ROW = Y1;
  DrealViewLayerDefaultValues[ 3 ] = Y1 * 25;
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Arrows                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      CallbackLeftArrow                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackLeftArrow( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomLeft();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackRightArrow                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackRightArrow( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomRight();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackUpArrow                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackUpArrow( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomUp();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackDownArrow                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackDownArrow( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomDown();

  rdsend();
}


/*------------------------------------------------------------\
|                                                             |
|                       CallbackMoveSet                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackMoveSet( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealViewMoveSetDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackCloseArrows                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackCloseArrows( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealViewArrowsPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Zoom Callback                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       CallbackZoomRefresh                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomRefresh( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomRefresh();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackZoomPrevious                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomPrevious( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomUndo();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackZoomLess                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomLess( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomLess();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackZoomMore                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomMore( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomMore();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackZoomSet                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomSet( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealViewZoomSetDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackCloseZoom                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackCloseZoom( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealViewZoomPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackZoomIn                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomIn( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealChangeEditMode( DREAL_ZOOM_IN,
                       DrealPromptZoomIn );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackZoomCenter                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomCenter( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealChangeEditMode( DREAL_ZOOM_CENTER,
                       DrealPromptZoomCenter );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackZoomGoto                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomGoto( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealViewZoomGotoDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackZoomPan                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomPan( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealChangeEditMode( DREAL_ZOOM_PAN,
                       DrealPromptZoomPan );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        CallbackZoomFit                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackZoomFit( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomFit();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Layer                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackLayerAllVisible                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerAllVisible( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealLayerAllVisible();

  rdsend();
}

/*------------------------------------------------------------\ 
|                                                             | 
|                  CallbackLayerAllInvisible                  | 
|                                                             | 
\------------------------------------------------------------*/ 
 
void CallbackLayerAllInvisible( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  DrealLayerAllInvisible();

  rdsend();
}

/*------------------------------------------------------------\ 
|                                                             | 
|                      CallbackLayerVisible                   | 
|                                                             | 
\------------------------------------------------------------*/ 
 
void CallbackLayerVisible( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ (long)ClientData ] == 0 )
  {
    DrealSetLayerVisible( (long)ClientData );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackNameVisible( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( DREAL_RDS_ACTIVE_NAME_TABLE[ (long)ClientData ] == 0 )
  {
    DrealSetNameVisible( (long)ClientData );
  }

  rdsend();
}

/*------------------------------------------------------------\ 
|                                                             | 
|                     CallbackLayerInvisible                  | 
|                                                             | 
\------------------------------------------------------------*/ 
 
void CallbackLayerInvisible( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ (long)ClientData ] == 1 )
  { 
    DrealSetLayerInvisible( (long)ClientData );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackNameInvisible                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackNameInvisible( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( DREAL_RDS_ACTIVE_NAME_TABLE[ (long)ClientData ] == 1 )
  {
    DrealSetNameInvisible( (long)ClientData );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLayerFillMode                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerFillMode( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DREAL_FILL_MODE = ( DREAL_FILL_MODE + 1 ) % 3;

  DrealChangeFillMode( DREAL_FILL_MODE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLayerInterface                |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerInterface( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DREAL_INSTANCE_INTERFACE = ! DREAL_INSTANCE_INTERFACE;
  DrealChangeInterface( DREAL_INSTANCE_INTERFACE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLayerCursor                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerCursor( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DREAL_XOR_CURSOR = ! DREAL_XOR_CURSOR;
  DrealChangeCursor( DREAL_XOR_CURSOR );

  DrealChangeCursorContext();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLayerForceDisplay             |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerForceDisplay( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DREAL_FORCE_DISPLAY = ! DREAL_FORCE_DISPLAY;
  DrealChangeForceDisplay( DREAL_FORCE_DISPLAY );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackLayerBlackBoxString               |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerBlackBoxString( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DREAL_BLACK_BOX_STRING = ! DREAL_BLACK_BOX_STRING;
  DrealChangeBlackBoxString( DREAL_BLACK_BOX_STRING );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLayerApply                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerApply( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealZoomRefresh();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackCloseLayer                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackCloseLayer( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealViewLayerPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealChangeFillMode                    |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeFillMode( FillMode )

  char FillMode;
{
  int      Index;
  XmString Label;

  rdsbegin();

  Index = 2 + ( ( RDS_ALL_LAYER + DREAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( DrealViewLayerFillModeName[ FillMode ] );

  XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealChangeInterface                   |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeInterface( Interface )

  char Interface;
{
  int      Index;
  XmString Label;

  rdsbegin();

  Index = 3 + ( ( RDS_ALL_LAYER + DREAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( DrealViewLayerInterfaceName[ Interface ] );

  XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealChangeCurosr                      |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeCursor( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  rdsbegin();

  Pos   = 4 + ( ( RDS_ALL_LAYER + DREAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( DrealViewLayerCursorType[ Index ] );

  XtVaSetValues( DrealViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealChangeForceDisplay                |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeForceDisplay( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  rdsbegin();

  Pos   = 5 + ( ( RDS_ALL_LAYER + DREAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( DrealViewLayerForceDisplay[ Index ] );

  XtVaSetValues( DrealViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealChangeBlackBoxString                  |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeBlackBoxString( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  rdsbegin();

  Pos   = 6 + ( ( RDS_ALL_LAYER + DREAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( DrealViewLayerBlackBoxString[ Index ] );

  XtVaSetValues( DrealViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealSetLayerVisible                   |
|                                                             |
\------------------------------------------------------------*/

void DrealSetLayerVisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 2 + ( Layer << 1 );

  if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] != -1 )
  {
    DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = 1;

    XtVaSetValues( DrealViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   DREAL_BLACK_COLOR                       ,
                   strlen( DREAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );

    XtVaSetValues( DrealViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   DREAL_BLACK_COLOR                       ,
                   strlen( DREAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );

    Background = DrealViewLayerButton[ Index ].BACKGROUND;
    Foreground = DrealViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNforeground           ,
                   XmRString               ,
                   Foreground              ,
                   strlen( Foreground ) + 1,
                   NULL
                 );
  }

  rdsend();
} 

/*------------------------------------------------------------\
|                                                             |
|                       DrealSetNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void DrealSetNameVisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 2 + ( ( RDS_ALL_LAYER + Name ) << 1 );

  DREAL_RDS_ACTIVE_NAME_TABLE[ Name ] = 1;

  XtVaSetValues( DrealViewLayerButton[ Index + 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNbackground                           ,
                 XmRString                               ,
                 DREAL_BLACK_COLOR                       ,
                 strlen( DREAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  XtVaSetValues( DrealViewLayerButton[ Index + 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNforeground                           ,
                 XmRString                               ,
                 DREAL_BLACK_COLOR                       ,
                 strlen( DREAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  Background = DrealViewLayerButton[ Index ].BACKGROUND;
  Foreground = DrealViewLayerButton[ Index ].FOREGROUND;

  XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                 XtVaTypedArg            ,
                 XmNbackground           ,
                 XmRString               ,
                 Background              ,
                 strlen( Background ) + 1,
                 NULL
               );

  XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                 XtVaTypedArg            ,
                 XmNforeground           ,
                 XmRString               ,
                 Foreground              ,
                 strlen( Foreground ) + 1,
                 NULL
               );
  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealSetLayerInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void DrealSetLayerInvisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 3 + ( Layer << 1 );

  if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] != -1 )
  {
    DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = 0;

    XtVaSetValues( DrealViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   DREAL_BLACK_COLOR                       ,
                   strlen( DREAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );

    XtVaSetValues( DrealViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   DREAL_BLACK_COLOR                       ,
                   strlen( DREAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );

    Background = DrealViewLayerButton[ Index ].BACKGROUND;
    Foreground = DrealViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNforeground                         ,
                   XmRString                             ,
                   Foreground                            ,
                   strlen( Foreground ) + 1              ,
                   NULL
                 );
  } 

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealSetNameInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void DrealSetNameInvisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 3 + ( ( RDS_ALL_LAYER + Name ) << 1 );

  DREAL_RDS_ACTIVE_NAME_TABLE[ Name ] = 0;

  XtVaSetValues( DrealViewLayerButton[ Index - 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNbackground                           ,
                 XmRString                               ,
                 DREAL_BLACK_COLOR                       ,
                 strlen( DREAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  XtVaSetValues( DrealViewLayerButton[ Index - 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNforeground                           ,
                 XmRString                               ,
                 DREAL_BLACK_COLOR                       ,
                 strlen( DREAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  Background = DrealViewLayerButton[ Index ].BACKGROUND;
  Foreground = DrealViewLayerButton[ Index ].FOREGROUND;

  XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                 XtVaTypedArg                          ,
                 XmNbackground                         ,
                 XmRString                             ,
                 Background                            ,
                 strlen( Background ) + 1              ,
                 NULL
               );

  XtVaSetValues( DrealViewLayerButton[ Index ].BUTTON  ,
                 XtVaTypedArg                          ,
                 XmNforeground                         ,
                 XmRString                             ,
                 Foreground                            ,
                 strlen( Foreground ) + 1              ,
                 NULL
               );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealLayerAllVisible                   |
|                                                             |
\------------------------------------------------------------*/

void DrealLayerAllVisible()

{
  int   Layer;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    DrealSetLayerVisible( Layer );
  }

  for ( Layer = 0; Layer < DREAL_MAX_ACTIVE_NAME; Layer++ )
  {
    DrealSetNameVisible( Layer );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealInitializeLayer                     |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeLayer()

{
  int   Index;

  rdsbegin();

  for ( Index = 0; Index < RDS_ALL_LAYER; Index++ )
  {
    if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ Index ] == 0 )
    {
      DrealSetLayerInvisible( Index );
    }
    else
    if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ Index ] == 1 )
    {
      DrealSetLayerVisible( Index );
    }
  }

  for ( Index = 0; Index < DREAL_MAX_ACTIVE_NAME; Index++ )
  {
    if ( DREAL_RDS_ACTIVE_NAME_TABLE[ Index ] == 0 )
    {
      DrealSetNameInvisible( Index );
    }
    else
    {
      DrealSetNameVisible( Index );
    }
  }

  DrealChangeFillMode( DREAL_FILL_MODE );
  DrealChangeInterface( DREAL_INSTANCE_INTERFACE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealLayerAllInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void DrealLayerAllInvisible()

{
  int   Layer;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    DrealSetLayerInvisible( Layer );
  }

  for ( Layer = 0; Layer < DREAL_MAX_ACTIVE_NAME; Layer++ )
  {
    DrealSetNameInvisible( Layer );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Grid                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      CallbackSetGridX                       |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSetGridX( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealViewGridXDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackSetGridY                       |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackSetGridY( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealViewGridYDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackGridOnOff                      |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackGridOnOff( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Label;

  rdsbegin();

  DrealLambdaUserGrid = ! DrealLambdaUserGrid;

  if ( DrealLambdaUserGrid == DREAL_FALSE )
  {  
    Label = XmStringCreateSimple( "Grid Off" );
  }  
  else
  {   
    Label = XmStringCreateSimple( "Grid On" );
  }  

  XtVaSetValues( MyWidget, XmNlabelString, Label,
                           XmNresizable  , False,
                           NULL );
  XmStringFree( Label );

  DrealZoomRefresh();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackCloseGrid                      |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackCloseGrid( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealViewGridPanel );

  rdsend();
}
