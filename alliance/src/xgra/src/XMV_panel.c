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
| File    :                   Panel.c                         |
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

# include <stdio.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMV.h"

# include "XMV_panel.h"
# include "XMV_dialog.h"
# include "XMV_view.h"
# include "XMV_message.h"

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

  static char XgraPixmapDownArrow[] =

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

  static char XgraPixmapLeftArrow[] =

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

  static char XgraPixmapRightArrow[] =

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

  static char XgraPixmapUpArrow[] =

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

   static char *XgraPanelArrowsButtonName[] = 

   {
     "Left",  
     "Up",
     "Down",
     "Right",
     "Move Set",
     "Close"
   };

   static XgraPanelButtonItem XgraViewArrowsButton[] =

         {
           {
             &XgraPanelArrowsButtonName[0],
             NULL, XgraPixmapLeftArrow, 32, 32,
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
             &XgraPanelArrowsButtonName[1],
             NULL, XgraPixmapUpArrow, 32, 32,
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
             &XgraPanelArrowsButtonName[2],
             NULL, XgraPixmapDownArrow, 32, 32,
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
             &XgraPanelArrowsButtonName[3],
             NULL, XgraPixmapRightArrow, 32, 32,
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
             &XgraPanelArrowsButtonName[4],
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
             &XgraPanelArrowsButtonName[5],
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
           {
             NULL
           }
         };

   XgraPanelItem XgraViewArrowsPanel =

         {
           "Arrows",
           1,
           0,
           XGRA_VIEW_ARROWS_X,
           XGRA_VIEW_ARROWS_Y, 
           100,
           125,
           2,  
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XgraViewArrowsButton
         };

   int XgraViewArrowsDefaultValues[ 5 ] =

         {
           XGRA_VIEW_ARROWS_X, 
           XGRA_VIEW_ARROWS_Y,
           100, 125 , 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Zoom                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XgraPanelZoomButtonName[] =

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


 XgraPanelButtonItem XgraViewZoomButton[] =

         {
           {
             &XgraPanelZoomButtonName[0],
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
             &XgraPanelZoomButtonName[1],
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
             &XgraPanelZoomButtonName[2],
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
             &XgraPanelZoomButtonName[3],
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
             &XgraPanelZoomButtonName[4],
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
             &XgraPanelZoomButtonName[5],
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
             &XgraPanelZoomButtonName[6],
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
             &XgraPanelZoomButtonName[7],
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
             &XgraPanelZoomButtonName[8],
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
             &XgraPanelZoomButtonName[9],
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
             &XgraPanelZoomButtonName[10],
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
           {
             NULL
           }
         };

   XgraPanelItem XgraViewZoomPanel =

         {
           "Zoom",
           1,
           0,
           XGRA_VIEW_ZOOM_X,
           XGRA_VIEW_ZOOM_Y, 
           100,
           275,
           1,  
           11,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XgraViewZoomButton
         };

   int XgraViewZoomDefaultValues[ 5 ] =

         {
           XGRA_VIEW_ZOOM_X, 
           XGRA_VIEW_ZOOM_Y,
           100, 275, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Grid                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XgraPanelGridButtonName[] =

   {
     "Grid X",
     "Grid Y",
     "Grid Off",
     "Close"
   };


 XgraPanelButtonItem XgraViewGridButton[] =

         {
           {
             &XgraPanelGridButtonName[0],
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
             &XgraPanelGridButtonName[1],
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
             &XgraPanelGridButtonName[2],
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
             &XgraPanelGridButtonName[3],
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
           {
             NULL
           }
         };

     XgraPanelItem XgraViewGridPanel =

         {
           "Grid",
           1,
           0,
           XGRA_VIEW_GRID_X,
           XGRA_VIEW_GRID_Y, 
           100,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XgraViewGridButton
         };

  int XgraViewGridDefaultValues[ 5 ] =

         {
           XGRA_VIEW_GRID_X,
           XGRA_VIEW_GRID_Y, 
           100, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Layer                          |
|                                                             |
\------------------------------------------------------------*/

 static char *XgraViewLayerButtonBackground = "Black";
 static char *XgraViewLayerButtonForeground = "White";

 static char *XgraPanelLayerButtonName[] =

  {
    "All visible",
    "All invisible",
    "Node Text",
    "Arc Text",
    "Apply",
    "Close"
  };

 XgraPanelButtonItem XgraViewLayerButton[] =

         {
           {
             &XgraPanelLayerButtonName[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackLayerAllVisible,
             (XtPointer)NULL,
             (Widget)NULL
           }
           , 
           {
             &XgraPanelLayerButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 0,
             1, 1,
             CallbackLayerAllInvisible,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 0 ][ 2 ],
             0, 1,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 0 ][ 2 ],
             1, 1,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 1 ][ 2 ],
             0, 2,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 1 ][ 2 ],
             1, 2,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 2 ][ 2 ],
             0, 3,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 2 ][ 2 ],
             1, 3,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 3 ][ 2 ],
             0, 4,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 3 ][ 2 ],
             1, 4,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 4 ][ 2 ],
             0, 5,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 4 ][ 2 ],
             1, 5,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 5 ][ 2 ],
             0, 6,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 5 ][ 2 ],
             1, 6,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 6 ][ 2 ],
             0, 7,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 6 ][ 2 ],
             1, 7,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 7 ][ 2 ],
             0, 8,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 7 ][ 2 ],
             1, 8,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 8 ][ 2 ],
             0, 9,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 8 ][ 2 ],
             1, 9,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 9 ][ 2 ],
             0, 10,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XGRA_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XGRA_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XGRA_LAYER_NAME_TABLE[ 9 ][ 2 ],
             1, 10,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XgraPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XgraViewLayerButtonForeground,
             &XgraViewLayerButtonBackground,
             0, 11,
             1, 1,
             CallbackNameVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XgraPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XgraViewLayerButtonForeground,
             &XgraViewLayerButtonBackground,
             1, 11,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XgraPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XgraViewLayerButtonForeground,
             &XgraViewLayerButtonBackground,
             0, 12,
             1, 1,
             CallbackNameVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XgraPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XgraViewLayerButtonForeground,
             &XgraViewLayerButtonBackground,
             1, 12,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XgraPanelLayerButtonName[4],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 13,
             1, 1,
             CallbackLayerApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XgraPanelLayerButtonName[5],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 13,
             1, 1,
             CallbackCloseLayer,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL
           }
         };

     XgraPanelItem XgraViewLayerPanel =

         {
           "Layer",
           1,
           0,
           XGRA_VIEW_LAYER_X,
           XGRA_VIEW_LAYER_Y, 
           200,
           350,
           2,  
           14,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XgraViewLayerButton
         };

  int XgraViewLayerDefaultValues[ 5 ] =

         {
           XGRA_VIEW_LAYER_X,
           XGRA_VIEW_LAYER_Y, 
           200, 350, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
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

  XgraZoomLeft();

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

  XgraZoomRight();

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

  XgraZoomUp();

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

  XgraZoomDown();

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

  XgraEnterDialog( &XgraViewMoveSetDialog );

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

  XgraExitPanel( &XgraViewArrowsPanel );

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

  XgraZoomRefresh();

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

  XgraZoomUndo();

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

  XgraZoomLess();

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

  XgraZoomMore();

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

  XgraEnterDialog( &XgraViewZoomSetDialog );

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

  XgraExitPanel( &XgraViewZoomPanel );

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

  XgraChangeEditMode( XGRA_ZOOM_IN,
                       XgraPromptZoomIn );

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

  XgraChangeEditMode( XGRA_ZOOM_CENTER,
                       XgraPromptZoomCenter );

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

  XgraEnterDialog( &XgraViewZoomGotoDialog );

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

  XgraChangeEditMode( XGRA_ZOOM_PAN,
                       XgraPromptZoomPan );

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

  XgraZoomFit();

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

  XgraEnterDialog( &XgraViewGridXDialog );

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

  XgraEnterDialog( &XgraViewGridYDialog );

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

  XgraUnitUserGrid = ! XgraUnitUserGrid;

  if ( XgraUnitUserGrid == XGRA_FALSE )
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

  XgraZoomRefresh();

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

  XgraExitPanel( &XgraViewGridPanel );

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

  XgraLayerAllVisible();

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

  XgraLayerAllInvisible();

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

  if ( XGRA_ACTIVE_LAYER_TABLE[ (long)ClientData ] == 0 )
  {
    XgraSetLayerVisible( (long)ClientData );
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

  if ( XGRA_ACTIVE_LAYER_TABLE[ (long)ClientData ] )
  {
    XgraSetLayerInvisible( (long)ClientData );
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

  if ( XGRA_ACTIVE_NAME_TABLE[ (long)ClientData ] == 0 )
  {
    XgraSetNameVisible( ClientData );
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

  if ( XGRA_ACTIVE_NAME_TABLE[ (long)ClientData ] )
  {
    XgraSetNameInvisible( ClientData );
  }

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

  XgraZoomRefresh();

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

  XgraExitPanel( &XgraViewLayerPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraSetLayerVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XgraSetLayerVisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = ( Layer << 1 ) + 2;

  if ( XgraViewLayerButton[ Index ].BUTTON != NULL )
  {
    XGRA_ACTIVE_LAYER_TABLE[ Layer ] = 1;

    XtVaSetValues( XgraViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME                   ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME                   ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XgraViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XgraViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
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
|                        XgraSetNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XgraSetNameVisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 2 + ( ( XGRA_MAX_LAYER + Name ) << 1 );

  if ( XgraViewLayerButton[ Index ].BUTTON != NULL )
  {
    XGRA_ACTIVE_NAME_TABLE[ Name ] = 1;

    XtVaSetValues( XgraViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME              ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME                   ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XgraViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XgraViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
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
|                     XgraSetLayerInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XgraSetLayerInvisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( Layer << 1 );

  if ( XgraViewLayerButton[ Index ].BUTTON != NULL )
  {
    XGRA_ACTIVE_LAYER_TABLE[ Layer ] = 0;

    XtVaSetValues( XgraViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME                   ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME                   ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XgraViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XgraViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
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
|                      XgraSetNameInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XgraSetNameInvisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( ( XGRA_MAX_LAYER + Name ) << 1 );

  if ( XgraViewLayerButton[ Index ].BUTTON != NULL )
  {
    XGRA_ACTIVE_NAME_TABLE[ Name ] = 0;

    XtVaSetValues( XgraViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME                   ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XGRA_BLACK_COLOR_NAME                   ,
                   strlen( XGRA_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XgraViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XgraViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XgraViewLayerButton[ Index ].BUTTON  ,
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
|                       XgraLayerAllVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XgraLayerAllVisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XGRA_MAX_LAYER; Index++ )
  {
    XgraSetLayerVisible( Index );
  }

  for ( Index = 0; Index < XGRA_MAX_ACTIVE_NAME; Index++ )
  {
    XgraSetNameVisible( Index );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraInitializeLayer                     |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeLayer()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XGRA_MAX_LAYER; Index++ )
  {
    if ( XGRA_ACTIVE_LAYER_TABLE[ Index ] == 0 )
    {
      XgraSetLayerInvisible( Index );
    }
    else
    {
      XgraSetLayerVisible( Index );
    }
  }

  for ( Index = 0; Index < XGRA_MAX_ACTIVE_NAME; Index++ )
  {
    if ( XGRA_ACTIVE_NAME_TABLE[ Index ] == 0 )
    {
      XgraSetNameInvisible( Index );
    }
    else
    {
      XgraSetNameVisible( Index );
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraLayerAllInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XgraLayerAllInvisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XGRA_MAX_LAYER; Index++ )
  {
    XgraSetLayerInvisible( Index );
  }

  for ( Index = 0; Index < XGRA_MAX_ACTIVE_NAME; Index++ )
  {
    XgraSetNameInvisible( Index );
  }

  autend();
}
