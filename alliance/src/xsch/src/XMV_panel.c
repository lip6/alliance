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
| File    :                   Panel.c                         |
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

# include <stdio.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMV.h"

# include "XMV_panel.h"
# include "XMV_dialog.h"
# include "XMV_view.h"
# include "XMV_message.h"
# include "XMX_color.h"

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

  static char XschPixmapDownArrow[] =

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

  static char XschPixmapLeftArrow[] =

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

  static char XschPixmapRightArrow[] =

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

  static char XschPixmapUpArrow[] =

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

   static char *XschPanelArrowsButtonName[] = 

   {
     "Left",  
     "Up",
     "Down",
     "Right",
     "Move Set",
     "Close"
   };

   static XschPanelButtonItem XschViewArrowsButton[] =

         {
           {
             &XschPanelArrowsButtonName[0],
             NULL, XschPixmapLeftArrow, 32, 32,
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
             &XschPanelArrowsButtonName[1],
             NULL, XschPixmapUpArrow, 32, 32,
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
             &XschPanelArrowsButtonName[2],
             NULL, XschPixmapDownArrow, 32, 32,
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
             &XschPanelArrowsButtonName[3],
             NULL, XschPixmapRightArrow, 32, 32,
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
             &XschPanelArrowsButtonName[4],
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
             &XschPanelArrowsButtonName[5],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             2, 1,
             CallbackCloseArrows,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XschPanelItem XschViewArrowsPanel =

         {
           "Arrows",
           1,
           0,
           XSCH_VIEW_ARROWS_X,
           XSCH_VIEW_ARROWS_Y, 
           100,
           125,
           2,  
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XschViewArrowsButton
         };

   int XschViewArrowsDefaultValues[ 5 ] =

         {
           XSCH_VIEW_ARROWS_X, 
           XSCH_VIEW_ARROWS_Y,
           100, 125 , 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Zoom                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XschPanelZoomButtonName[] =

   {
     "Refresh",
     "UnZoom",
     "Zoom",
     "Mooz",
     "Zoom Set",
     "Zoom In",
     "Center",
     "Goto",
     "Pan",
     "Fit",
     "Close"
   };


 XschPanelButtonItem XschViewZoomButton[] =

         {
           {
             &XschPanelZoomButtonName[0],
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
             &XschPanelZoomButtonName[1],
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
             &XschPanelZoomButtonName[2],
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
             &XschPanelZoomButtonName[3],
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
             &XschPanelZoomButtonName[4],
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
             &XschPanelZoomButtonName[5],
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
             &XschPanelZoomButtonName[6],
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
             &XschPanelZoomButtonName[7],
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
             &XschPanelZoomButtonName[8],
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
             &XschPanelZoomButtonName[9],
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
             &XschPanelZoomButtonName[10],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 10,
             1, 1,
             CallbackCloseZoom,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XschPanelItem XschViewZoomPanel =

         {
           "Zoom",
           1,
           0,
           XSCH_VIEW_ZOOM_X,
           XSCH_VIEW_ZOOM_Y, 
           100,
           275,
           1,  
           11,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XschViewZoomButton
         };

   int XschViewZoomDefaultValues[ 5 ] =

         {
           XSCH_VIEW_ZOOM_X, 
           XSCH_VIEW_ZOOM_Y,
           100, 275, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Grid                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XschPanelGridButtonName[] =

   {
     "Grid X",
     "Grid Y",
     "Grid Off",
     "Close"
   };


 XschPanelButtonItem XschViewGridButton[] =

         {
           {
             &XschPanelGridButtonName[0],
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
             &XschPanelGridButtonName[1],
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
             &XschPanelGridButtonName[2],
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
             &XschPanelGridButtonName[3],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackCloseGrid,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           NULL
         };

     XschPanelItem XschViewGridPanel =

         {
           "Grid",
           1,
           0,
           XSCH_VIEW_GRID_X,
           XSCH_VIEW_GRID_Y, 
           100,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XschViewGridButton
         };

  int XschViewGridDefaultValues[ 5 ] =

         {
           XSCH_VIEW_GRID_X,
           XSCH_VIEW_GRID_Y, 
           100, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Layer                          |
|                                                             |
\------------------------------------------------------------*/

 static char *XschViewLayerButtonBackground = "Black";
 static char *XschViewLayerButtonForeground = "White";

 static char *XschPanelLayerButtonName[] =

  {
    "All visible",
    "All invisible",
    "I/O",
    "Gate",
    "I/O gate",
    "Net",
    "Apply",
    "Close"
  };

  static char *XschViewLayerCursorType[] =

  {
    "Invert",
    "Xor"
  };

  static char *XschViewLayerForceDisplay[] =

  {
    "Quick display",
    "Force display"
  };

  XschPanelButtonItem XschViewLayerButton[] =

         {
           {
             &XschPanelLayerButtonName[0],
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
             &XschPanelLayerButtonName[1],
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
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
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[4],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[4],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[5],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameVisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[5],
             NULL, NULL, 0, 0,
             &XschViewLayerButtonForeground,
             &XschViewLayerButtonBackground,
             0, 0,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XschViewLayerCursorType[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 8,
             2, 1,
             CallbackLayerCursor,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XschViewLayerForceDisplay[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             2, 8,
             2, 1,
             CallbackLayerForceDisplay,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[6],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 9,
             2, 1,
             CallbackLayerApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XschPanelLayerButtonName[7],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             2, 9,
             2, 1,
             CallbackCloseLayer,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

     XschPanelItem XschViewLayerPanel =

         {
           "Layer",
           1,
           0,
           XSCH_VIEW_LAYER_X,
           XSCH_VIEW_LAYER_Y, 
           300,
           500,
           4,  
           45,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XschViewLayerButton
         };

  int XschViewLayerDefaultValues[ 5 ] =

         {
           XSCH_VIEW_LAYER_X,
           XSCH_VIEW_LAYER_Y, 
           300, 500, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       XschBuildPanelLayer                   |
|                                                             |
\------------------------------------------------------------*/

void XschBuildPanelLayer()
{
  XschPanelButtonItem  *Button;
  unsigned char         Visible;
  long                  Layer;
  unsigned char         Index;
  unsigned char         X1;
  unsigned char         Y1;

  X1    = 0;
  Y1    = 1;
  Index = 2;

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    for ( Visible = 0; Visible < 2; Visible++ )
    {
      Button = &XschViewLayerButton[ Index + Visible ];

      Button->LABEL      = &XSCH_LAYER_NAME_TABLE[ Layer ][ 0 ];
      Button->FOREGROUND = &XSCH_LAYER_NAME_TABLE[ Layer ][ 1 ];
      Button->BACKGROUND = &XSCH_LAYER_NAME_TABLE[ Layer ][ 2 ];

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

    Index = Index + 2;
  }

  if ( X1 > 0 )
  {
    X1 = 0;
    Y1 = Y1 + 1;
  }

  for ( Layer = 0; Layer < 4; Layer++ )
  {
    for ( Visible = 0; Visible < 2; Visible++ )
    {
      Button = &XschViewLayerButton[ Index + Visible ];

      Button->X = X1;
      Button->Y = Y1;

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

    Index = Index + 2;
  }

  if ( X1 > 0 )
  {
    X1 = 0;
    Y1 = Y1 + 1;
  }

  for ( Layer = 0; Layer < 3; Layer++ )
  {
    XschViewLayerButton[ Index     ].Y = Y1;;
    XschViewLayerButton[ Index + 1 ].Y = Y1;;

    Index = Index + 2;
    Y1    = Y1    + 1;
  }

  XschViewLayerPanel.ROW = Y1 - 1;
  XschViewLayerDefaultValues[ 3 ] = ( Y1 - 1 ) * 25;
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
  autbegin();

  XschZoomLeft();

  autend();
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
  autbegin();

  XschZoomRight();

  autend();
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
  autbegin();

  XschZoomUp();

  autend();
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
  autbegin();

  XschZoomDown();

  autend();
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
  autbegin();

  XschEnterDialog( &XschViewMoveSetDialog );

  autend();
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
  autbegin();

  XschExitPanel( &XschViewArrowsPanel );

  autend();
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
  autbegin();

  XschZoomRefresh();

  autend();
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
  autbegin();

  XschZoomUndo();

  autend();
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
  autbegin();

  XschZoomLess();

  autend();
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
  autbegin();

  XschZoomMore();

  autend();
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
  autbegin();

  XschEnterDialog( &XschViewZoomSetDialog );

  autend();
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
  autbegin();

  XschExitPanel( &XschViewZoomPanel );

  autend();
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
  autbegin();

  XschChangeEditMode( XSCH_ZOOM_IN,
                       XschPromptZoomIn );

  autend();
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
  autbegin();

  XschChangeEditMode( XSCH_ZOOM_CENTER,
                       XschPromptZoomCenter );

  autend();
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
  autbegin();

  XschEnterDialog( &XschViewZoomGotoDialog );

  autend();
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
  autbegin();

  XschChangeEditMode( XSCH_ZOOM_PAN,
                       XschPromptZoomPan );

  autend();
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
  autbegin();

  XschZoomFit();

  autend();
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
  autbegin();

  XschEnterDialog( &XschViewGridXDialog );

  autend();
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
  autbegin();

  XschEnterDialog( &XschViewGridYDialog );

  autend();
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

  autbegin();

  XschUnitUserGrid = ! XschUnitUserGrid;

  if ( XschUnitUserGrid == XSCH_FALSE )
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

  XschZoomRefresh();

  autend();
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
  autbegin();

  XschExitPanel( &XschViewGridPanel );

  autend();
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
  autbegin();

  XschLayerAllVisible();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackLayerAllInvisible                |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackLayerAllInvisible( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschLayerAllInvisible();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackLayerVisible                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerVisible( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  if ( XSCH_ACTIVE_LAYER_TABLE[ (int)ClientData ] == 0 )
  {
    XschSetLayerVisible( (int)ClientData );
  }

  autend();
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
  autbegin();

  if ( XSCH_ACTIVE_LAYER_TABLE[ (int)ClientData ] )
  {
    XschSetLayerInvisible( (int)ClientData );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackNameVisible                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackNameVisible( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  if ( XSCH_ACTIVE_NAME_TABLE[ (int)ClientData ] == 0 )
  {
    XschSetNameVisible( ClientData );
  }

  autend();
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
  autbegin();

  if ( XSCH_ACTIVE_NAME_TABLE[ (int)ClientData ] )
  {
    XschSetNameInvisible( ClientData );
  }

  autend();
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
  autbegin();

  XSCH_XOR_CURSOR = ! XSCH_XOR_CURSOR;
  XschChangeCursor( XSCH_XOR_CURSOR );

  XschChangeCursorContext();

  autend();
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
  autbegin();

  XSCH_FORCE_DISPLAY = ! XSCH_FORCE_DISPLAY;
  XschChangeForceDisplay( XSCH_FORCE_DISPLAY );

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XschChangeCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XschChangeCursor( Type )

  char Type;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 2 + ( ( XSCH_MAX_LAYER + XSCH_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XschViewLayerCursorType[ Type ] );

  XtVaSetValues( XschViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschChangeForceDisplay                |
|                                                             |
\------------------------------------------------------------*/

void XschChangeForceDisplay( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 3 + ( ( XSCH_MAX_LAYER + XSCH_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XschViewLayerForceDisplay[ Index ] );

  XtVaSetValues( XschViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      CallbackLayerApply                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackLayerApply( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschZoomRefresh();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackCloseLayer                     |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackCloseLayer( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschExitPanel( &XschViewLayerPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschSetLayerVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XschSetLayerVisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = ( Layer << 1 ) + 2;

  if ( XschViewLayerButton[ Index ].BUTTON != NULL )
  {
    XSCH_ACTIVE_LAYER_TABLE[ Layer ] = 1;

    XtVaSetValues( XschViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME                   ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME                   ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XschViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XschViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNforeground           ,
                   XmRString               ,
                   Foreground              ,
                   strlen( Foreground ) + 1,
                   NULL
                 );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschSetNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XschSetNameVisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 2 + ( ( XSCH_MAX_LAYER + Name ) << 1 );

  if ( XschViewLayerButton[ Index ].BUTTON != NULL )
  {
    XSCH_ACTIVE_NAME_TABLE[ Name ] = 1;

    XtVaSetValues( XschViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME              ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME                   ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XschViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XschViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNforeground           ,
                   XmRString               ,
                   Foreground              ,
                   strlen( Foreground ) + 1,
                   NULL
                 );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschSetLayerInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XschSetLayerInvisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( Layer << 1 );

  if ( XschViewLayerButton[ Index ].BUTTON != NULL )
  {
    XSCH_ACTIVE_LAYER_TABLE[ Layer ] = 0;

    XtVaSetValues( XschViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME                   ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME                   ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XschViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XschViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNforeground                         ,
                   XmRString                             ,
                   Foreground                            ,
                   strlen( Foreground ) + 1              ,
                   NULL
                 );
  } 

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschSetNameInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XschSetNameInvisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( ( XSCH_MAX_LAYER + Name ) << 1 );

  if ( XschViewLayerButton[ Index ].BUTTON != NULL )
  {
    XSCH_ACTIVE_NAME_TABLE[ Name ] = 0;

    XtVaSetValues( XschViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME                   ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XSCH_BLACK_COLOR_NAME                   ,
                   strlen( XSCH_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XschViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XschViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XschViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNforeground                         ,
                   XmRString                             ,
                   Foreground                            ,
                   strlen( Foreground ) + 1              ,
                   NULL
                 );
  } 

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschLayerAllVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XschLayerAllVisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XSCH_MAX_LAYER; Index++ )
  {
    XschSetLayerVisible( Index );
  }

  for ( Index = 0; Index < XSCH_MAX_ACTIVE_NAME; Index++ )
  {
    XschSetNameVisible( Index );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschInitializeLayer                     |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeLayer()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XSCH_MAX_LAYER; Index++ )
  {
    if ( XSCH_ACTIVE_LAYER_TABLE[ Index ] == 0 )
    {
      XschSetLayerInvisible( Index );
    }
    else
    {
      XschSetLayerVisible( Index );
    }
  }

  for ( Index = 0; Index < XSCH_MAX_ACTIVE_NAME; Index++ )
  {
    if ( XSCH_ACTIVE_NAME_TABLE[ Index ] == 0 )
    {
      XschSetNameInvisible( Index );
    }
    else
    {
      XschSetNameVisible( Index );
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschLayerAllInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XschLayerAllInvisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XSCH_MAX_LAYER; Index++ )
  {
    XschSetLayerInvisible( Index );
  }

  for ( Index = 0; Index < XSCH_MAX_ACTIVE_NAME; Index++ )
  {
    XschSetNameInvisible( Index );
  }

  autend();
}
