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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Panel.c                         |
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

  static char GraalPixmapDownArrow[] =

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

  static char GraalPixmapLeftArrow[] =

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

  static char GraalPixmapRightArrow[] =

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

  static char GraalPixmapUpArrow[] =

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

   static GraalPanelButtonItem GraalViewArrowsButton[] =

         {
           {
             "Left",
             NULL, GraalPixmapLeftArrow, 32, 32,
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
             NULL, GraalPixmapUpArrow, 32, 32,
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
             NULL, GraalPixmapDownArrow, 32, 32,
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
             NULL, GraalPixmapRightArrow, 32, 32,
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

   GraalPanelItem GraalViewArrowsPanel =

         {
           "Arrows",
           1,
           0,
           GRAAL_VIEW_ARROWS_X,
           GRAAL_VIEW_ARROWS_Y, 
           100,
           125,
           2,  
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           6,
           GraalViewArrowsButton
         };

   int GraalViewArrowsDefaultValues[ 5 ] =

         {
           GRAAL_VIEW_ARROWS_X, 
           GRAAL_VIEW_ARROWS_Y,
           100, 125 , 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Zoom                           |
|                                                             |
\------------------------------------------------------------*/

 GraalPanelButtonItem GraalViewZoomButton[] =

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

   GraalPanelItem GraalViewZoomPanel =

         {
           "Zoom",
           1,
           0,
           GRAAL_VIEW_ZOOM_X,
           GRAAL_VIEW_ZOOM_Y, 
           100,
           275,
           1,  
           11,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           11,
           GraalViewZoomButton
         };

   int GraalViewZoomDefaultValues[ 5 ] =

         {
           GRAAL_VIEW_ZOOM_X, 
           GRAAL_VIEW_ZOOM_Y,
           100, 275, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Layer                          |
|                                                             |
\------------------------------------------------------------*/

  static char *GraalViewLayerFillModeName[] =

  {
    "Outline",
    "Fill",
    "Pattern"
  };

  static char *GraalViewLayerInterfaceName[] =

  {
    "No interface",
    "Interface"
  };

  static char *GraalViewLayerConnectorIndex[] =

  {
    "No index",
    "Index"
  };

  static char *GraalViewLayerCursorType[] =

  {
    "Invert",
    "Xor"
  };

  static char *GraalViewLayerForceDisplay[] =

  {
    "Quick display",
    "Force display"
  };

  static char *GraalViewLayerBlackBoxString[] =

  {
    "No string box",
    "String box"
  };

     GraalPanelButtonItem GraalViewLayerButton[ 28 + (RDS_ALL_LAYER * 2) ] =

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
             (XtPointer)GRAAL_FIGURE_NAME,
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
             (XtPointer)GRAAL_FIGURE_NAME,
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
             (XtPointer)GRAAL_INSTANCE_NAME,
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
             (XtPointer)GRAAL_INSTANCE_NAME,
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
             (XtPointer)GRAAL_FIGURE_CONNECTOR_NAME,
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
             (XtPointer)GRAAL_FIGURE_CONNECTOR_NAME,
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
             (XtPointer)GRAAL_INSTANCE_CONNECTOR_NAME,
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
             (XtPointer)GRAAL_INSTANCE_CONNECTOR_NAME,
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
             (XtPointer)GRAAL_FIGURE_SEGMENT_NAME,
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
             (XtPointer)GRAAL_FIGURE_SEGMENT_NAME,
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
             (XtPointer)GRAAL_INSTANCE_SEGMENT_NAME,
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
             (XtPointer)GRAAL_INSTANCE_SEGMENT_NAME,
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
             (XtPointer)GRAAL_FIGURE_REFERENCE_NAME,
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
             (XtPointer)GRAAL_FIGURE_REFERENCE_NAME,
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
             (XtPointer)GRAAL_INSTANCE_REFERENCE_NAME,
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
             (XtPointer)GRAAL_INSTANCE_REFERENCE_NAME,
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
             "No index",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackLayerIndex,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Invert",
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
             "Quick display",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackLayerForceDisplay,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Scale",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             2, 0,
             2, 1,
             CallbackLayerScale,
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
             NULL,
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


     GraalPanelItem GraalViewLayerPanel =

         {
           "Layer",
           1,
           0,
           GRAAL_VIEW_LAYER_X,
           GRAAL_VIEW_LAYER_Y,
           180,
           575,
           4,
           24,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           ( RDS_ALL_LAYER         << 1 ) + 
           ( GRAAL_MAX_ACTIVE_NAME << 1 ) + 12,
           GraalViewLayerButton
         };

  int GraalViewLayerDefaultValues[ 5 ] =

         {
           GRAAL_VIEW_LAYER_X,
           GRAAL_VIEW_LAYER_Y,
           180, 575, 0
         };

   static char FirstEnterLayerPanel = 1;

/*------------------------------------------------------------\
|                                                             |
|                        Panel Grid                           |
|                                                             |
\------------------------------------------------------------*/

 GraalPanelButtonItem GraalViewGridButton[] =

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

     GraalPanelItem GraalViewGridPanel =

         {
           "Grid",
           1,
           0,
           GRAAL_VIEW_GRID_X,
           GRAAL_VIEW_GRID_Y, 
           90,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           4,
           GraalViewGridButton
         };

  int GraalViewGridDefaultValues[ 5 ] =

         {
           GRAAL_VIEW_GRID_X,
           GRAAL_VIEW_GRID_Y, 
           90, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalBuildPanelLayer                   |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildPanelLayer()
{
  GraalPanelButtonItem *Button;
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
    if ( ( GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED   ) )
    {
      for ( Visible = 0; Visible < 2; Visible++ )
      {
        Button = &GraalViewLayerButton[ Index + Visible ];

        Button->LABEL      = GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ];
        Button->FOREGROUND = GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][ 1 ];
        Button->BACKGROUND = GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][ 2 ];

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

  for ( Layer = 0; Layer < GRAAL_MAX_ACTIVE_NAME; Layer++ )
  {
    for ( Visible = 0; Visible < 2; Visible++ )
    {
      Button = &GraalViewLayerButton[ Index + Visible ];

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

  for ( Layer = 0; Layer < 5; Layer++ )
  {
    GraalViewLayerButton[ Index     ].Y = Y1;;
    GraalViewLayerButton[ Index + 1 ].Y = Y1;;

    Index = Index + 2;
    Y1    = Y1    + 1;
  }

  GraalViewLayerPanel.ROW = Y1;
  GraalViewLayerDefaultValues[ 3 ] = Y1 * 25;
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

  GraalZoomLeft();

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

  GraalZoomRight();

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

  GraalZoomUp();

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

  GraalZoomDown();

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

  GraalEnterDialog( &GraalViewMoveSetDialog );

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

  GraalExitPanel( &GraalViewArrowsPanel );

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

  GraalZoomRefresh();

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

  GraalZoomUndo();

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

  GraalZoomLess();

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

  GraalZoomMore();

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

  GraalEnterDialog( &GraalViewZoomSetDialog );

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

  GraalExitPanel( &GraalViewZoomPanel );

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

  GraalChangeEditMode( GRAAL_ZOOM_IN,
                       GraalPromptZoomIn );

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

  GraalChangeEditMode( GRAAL_ZOOM_CENTER,
                       GraalPromptZoomCenter );

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

  GraalEnterDialog( &GraalViewZoomGotoDialog );

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

  GraalChangeEditMode( GRAAL_ZOOM_PAN,
                       GraalPromptZoomPan );

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

  GraalZoomFit();

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

  GraalLayerAllVisible();

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

  GraalLayerAllInvisible();

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

  if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ (int)ClientData ] == 0 )
  {
    GraalSetLayerVisible( ClientData );
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

  if ( GRAAL_RDS_ACTIVE_NAME_TABLE[ (int)ClientData ] == 0 )
  {
    GraalSetNameVisible( ClientData );
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

  if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ (int)ClientData ] == 1 )
  { 
    GraalSetLayerInvisible( ClientData );
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

  if ( GRAAL_RDS_ACTIVE_NAME_TABLE[ (int)ClientData ] == 1 )
  {
    GraalSetNameInvisible( ClientData );
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

  GRAAL_FILL_MODE = ( GRAAL_FILL_MODE + 1 ) % 3;

  GraalChangeFillMode( GRAAL_FILL_MODE );

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

  GRAAL_INSTANCE_INTERFACE = ! GRAAL_INSTANCE_INTERFACE;
  GraalChangeInterface( GRAAL_INSTANCE_INTERFACE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLayerIndex                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerIndex( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GRAAL_CONNECTOR_INDEX = ! GRAAL_CONNECTOR_INDEX;
  GraalChangeIndex( GRAAL_CONNECTOR_INDEX );

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

  GRAAL_XOR_CURSOR = ! GRAAL_XOR_CURSOR;
  GraalChangeCursor( GRAAL_XOR_CURSOR );

  GraalChangeCursorContext();

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

  GRAAL_FORCE_DISPLAY = ! GRAAL_FORCE_DISPLAY;
  GraalChangeForceDisplay( GRAAL_FORCE_DISPLAY );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackLayerScale                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerScale( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  long NewScale;
  long Factor;

  rdsbegin();

  Factor   = RDS_LAMBDA / RDS_PHYSICAL_GRID;
  NewScale = GRAAL_SCALE + 1;

  while ( ( Factor % NewScale ) != 0 )
  {
    NewScale = NewScale + 1;

    if ( NewScale > Factor ) 
    {
      NewScale = 1; break;
    }
  }

  GraalLambdaGridX  = ( (float)GraalLambdaGridX  / (float)GRAAL_SCALE ) * (float)NewScale;
  GraalLambdaGridY  = ( (float)GraalLambdaGridY  / (float)GRAAL_SCALE ) * (float)NewScale;
  GraalLambdaGridDx = ( (float)GraalLambdaGridDx / (float)GRAAL_SCALE ) * (float)NewScale;
  GraalLambdaGridDy = ( (float)GraalLambdaGridDy / (float)GRAAL_SCALE ) * (float)NewScale;

  GRAAL_SCALE = NewScale;
  GraalChangeScale( GRAAL_SCALE );

  GraalLoadParameters();
  GraalComputeLambdaGrid();
  GraalComputeAndDisplayMap();
  GraalZoomRefresh();

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

  GRAAL_BLACK_BOX_STRING = ! GRAAL_BLACK_BOX_STRING;
  GraalChangeBlackBoxString( GRAAL_BLACK_BOX_STRING );

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

  GraalZoomRefresh();

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

  GraalExitPanel( &GraalViewLayerPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalChangeFillMode                    |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeFillMode( FillMode )

  char FillMode;
{
  int      Index;
  XmString Label;

  rdsbegin();

  Index = 2 + ( ( RDS_ALL_LAYER + GRAAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( GraalViewLayerFillModeName[ FillMode ] );

  XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
} 

/*------------------------------------------------------------\
|                                                             |
|                      GraalChangeInterface                   |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeInterface( Interface )

  char Interface;
{
  int      Index;
  XmString Label;

  rdsbegin();

  Index = 3 + ( ( RDS_ALL_LAYER + GRAAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( GraalViewLayerInterfaceName[ Interface ] );

  XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalChangeIndex                       |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeIndex( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  rdsbegin();

  Pos   = 4 + ( ( RDS_ALL_LAYER + GRAAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( GraalViewLayerConnectorIndex[ Index ] );

  XtVaSetValues( GraalViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalChangeCursor                      |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeCursor( Type )

  char Type;
{
  int      Pos;
  XmString Label;

  rdsbegin();

  Pos   = 5 + ( ( RDS_ALL_LAYER + GRAAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( GraalViewLayerCursorType[ Type ] );

  XtVaSetValues( GraalViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalChangeForceDisplay                |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeForceDisplay( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  rdsbegin();

  Pos   = 6 + ( ( RDS_ALL_LAYER + GRAAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( GraalViewLayerForceDisplay[ Index ] );

  XtVaSetValues( GraalViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalChangeScale                       |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeScale( Scale )

  long Scale;
{
  int      Pos;
  XmString Label;
  char     Buffer[ 64 ];

  rdsbegin();

  Pos   = 7 + ( ( RDS_ALL_LAYER + GRAAL_MAX_ACTIVE_NAME ) << 1 );

  sprintf( Buffer, "Scale %d", Scale );
  Label = XmStringCreateSimple( Buffer );

  XtVaSetValues( GraalViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalChangeBlackBoxString                  |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeBlackBoxString( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  rdsbegin();

  Pos   = 8 + ( ( RDS_ALL_LAYER + GRAAL_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( GraalViewLayerBlackBoxString[ Index ] );

  XtVaSetValues( GraalViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalSetLayerVisible                   |
|                                                             |
\------------------------------------------------------------*/

void GraalSetLayerVisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 2 + ( Layer << 1 );

  if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] != -1 )
  {
    GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = 1;
  
    XtVaSetValues( GraalViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   GRAAL_BLACK_COLOR                       ,
                   strlen( GRAAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );
  
    XtVaSetValues( GraalViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   GRAAL_BLACK_COLOR                       ,
                   strlen( GRAAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );
  
    Background = GraalViewLayerButton[ Index ].BACKGROUND;
    Foreground = GraalViewLayerButton[ Index ].FOREGROUND;
  
    XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );
  
    XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
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
|                       GraalSetNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void GraalSetNameVisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 2 + ( ( RDS_ALL_LAYER + Name ) << 1 );

  GRAAL_RDS_ACTIVE_NAME_TABLE[ Name ] = 1;

  XtVaSetValues( GraalViewLayerButton[ Index + 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNbackground                           ,
                 XmRString                               ,
                 GRAAL_BLACK_COLOR                       ,
                 strlen( GRAAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  XtVaSetValues( GraalViewLayerButton[ Index + 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNforeground                           ,
                 XmRString                               ,
                 GRAAL_BLACK_COLOR                       ,
                 strlen( GRAAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  Background = GraalViewLayerButton[ Index ].BACKGROUND;
  Foreground = GraalViewLayerButton[ Index ].FOREGROUND;

  XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
                 XtVaTypedArg            ,
                 XmNbackground           ,
                 XmRString               ,
                 Background              ,
                 strlen( Background ) + 1,
                 NULL
               );

  XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
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
|                    GraalSetLayerInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void GraalSetLayerInvisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 3 + ( Layer << 1 );

  if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] != -1 )
  {
    GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = 0;

    XtVaSetValues( GraalViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   GRAAL_BLACK_COLOR                       ,
                   strlen( GRAAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );

    XtVaSetValues( GraalViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   GRAAL_BLACK_COLOR                       ,
                   strlen( GRAAL_BLACK_COLOR ) + 1         ,
                   NULL
                 );

    Background = GraalViewLayerButton[ Index ].BACKGROUND;
    Foreground = GraalViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
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
|                     GraalSetNameInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void GraalSetNameInvisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  rdsbegin();

  Index = 3 + ( ( RDS_ALL_LAYER + Name ) << 1 );

  GRAAL_RDS_ACTIVE_NAME_TABLE[ Name ] = 0;

  XtVaSetValues( GraalViewLayerButton[ Index - 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNbackground                           ,
                 XmRString                               ,
                 GRAAL_BLACK_COLOR                       ,
                 strlen( GRAAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  XtVaSetValues( GraalViewLayerButton[ Index - 1 ].BUTTON,
                 XtVaTypedArg                            ,
                 XmNforeground                           ,
                 XmRString                               ,
                 GRAAL_BLACK_COLOR                       ,
                 strlen( GRAAL_BLACK_COLOR ) + 1         ,
                 NULL
               );

  Background = GraalViewLayerButton[ Index ].BACKGROUND;
  Foreground = GraalViewLayerButton[ Index ].FOREGROUND;

  XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
                 XtVaTypedArg                          ,
                 XmNbackground                         ,
                 XmRString                             ,
                 Background                            ,
                 strlen( Background ) + 1              ,
                 NULL
               );

  XtVaSetValues( GraalViewLayerButton[ Index ].BUTTON  ,
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
|                      GraalLayerAllVisible                   |
|                                                             |
\------------------------------------------------------------*/

void GraalLayerAllVisible()

{
  int   Layer;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    GraalSetLayerVisible( Layer );
  }

  for ( Layer = 0; Layer < GRAAL_MAX_ACTIVE_NAME; Layer++ )
  {
    GraalSetNameVisible( Layer );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalInitializeLayer                     |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeLayer()

{
  int   Index;

  rdsbegin();

  for ( Index = 0; Index < RDS_ALL_LAYER; Index++ )
  {
    if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ Index ] == 0 )
    {
      GraalSetLayerInvisible( Index );
    }
    else
    if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ Index ] == 1 )
    {
      GraalSetLayerVisible( Index );
    }
  }

  for ( Index = 0; Index < GRAAL_MAX_ACTIVE_NAME; Index++ )
  {
    if ( GRAAL_RDS_ACTIVE_NAME_TABLE[ Index ] == 0 )
    {
      GraalSetNameInvisible( Index );
    }
    else
    {
      GraalSetNameVisible( Index );
    }
  }

  GraalChangeFillMode( GRAAL_FILL_MODE           );
  GraalChangeInterface( GRAAL_INSTANCE_INTERFACE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalLayerAllInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void GraalLayerAllInvisible()

{
  int   Layer;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    GraalSetLayerInvisible( Layer );
  }

  for ( Layer = 0; Layer < GRAAL_MAX_ACTIVE_NAME; Layer++ )
  {
    GraalSetNameInvisible( Layer );
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

  GraalEnterDialog( &GraalViewGridXDialog );

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

  GraalEnterDialog( &GraalViewGridYDialog );

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

  GraalLambdaUserGrid = ! GraalLambdaUserGrid;

  if ( GraalLambdaUserGrid == GRAAL_FALSE )
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

  GraalZoomRefresh();

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

  GraalExitPanel( &GraalViewGridPanel );

  rdsend();
}
