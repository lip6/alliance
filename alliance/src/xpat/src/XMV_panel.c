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
# include "pat.h"
# include "XSB.h"
# include "XTB.h"
# include "XPT.h"
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

  static char XpatPixmapDownArrow[] =

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

  static char XpatPixmapLeftArrow[] =

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

  static char XpatPixmapRightArrow[] =

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

  static char XpatPixmapUpArrow[] =

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

   static char *XpatPanelArrowsButtonName[] = 

   {
     "Left",  
     "Up",
     "Down",
     "Right",
     "Move Set",
     "Close"
   };

   static XpatPanelButtonItem XpatViewArrowsButton[] =

         {
           {
             &XpatPanelArrowsButtonName[0],
             NULL, XpatPixmapLeftArrow, 32, 32,
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
             &XpatPanelArrowsButtonName[1],
             NULL, XpatPixmapUpArrow, 32, 32,
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
             &XpatPanelArrowsButtonName[2],
             NULL, XpatPixmapDownArrow, 32, 32,
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
             &XpatPanelArrowsButtonName[3],
             NULL, XpatPixmapRightArrow, 32, 32,
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
             &XpatPanelArrowsButtonName[4],
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
             &XpatPanelArrowsButtonName[5],
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

   XpatPanelItem XpatViewArrowsPanel =

         {
           "Arrows",
           1,
           0,
           XPAT_VIEW_ARROWS_X,
           XPAT_VIEW_ARROWS_Y, 
           100,
           125,
           2,  
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XpatViewArrowsButton
         };

   int XpatViewArrowsDefaultValues[ 5 ] =

         {
           XPAT_VIEW_ARROWS_X, 
           XPAT_VIEW_ARROWS_Y,
           100, 125 , 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Zoom                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XpatPanelZoomButtonName[] =

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


 XpatPanelButtonItem XpatViewZoomButton[] =

         {
           {
             &XpatPanelZoomButtonName[0],
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
             &XpatPanelZoomButtonName[1],
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
             &XpatPanelZoomButtonName[2],
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
             &XpatPanelZoomButtonName[3],
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
             &XpatPanelZoomButtonName[4],
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
             &XpatPanelZoomButtonName[5],
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
             &XpatPanelZoomButtonName[6],
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
             &XpatPanelZoomButtonName[7],
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
             &XpatPanelZoomButtonName[8],
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
             &XpatPanelZoomButtonName[9],
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
             &XpatPanelZoomButtonName[10],
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

   XpatPanelItem XpatViewZoomPanel =

         {
           "Zoom",
           1,
           0,
           XPAT_VIEW_ZOOM_X,
           XPAT_VIEW_ZOOM_Y, 
           100,
           275,
           1,  
           11,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XpatViewZoomButton
         };

   int XpatViewZoomDefaultValues[ 5 ] =

         {
           XPAT_VIEW_ZOOM_X, 
           XPAT_VIEW_ZOOM_Y,
           100, 275, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Grid                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XpatPanelGridButtonName[] =

   {
     "Grid X",
     "Grid Y",
     "Grid Off",
     "Close"
   };


 XpatPanelButtonItem XpatViewGridButton[] =

         {
           {
             &XpatPanelGridButtonName[0],
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
             &XpatPanelGridButtonName[1],
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
             &XpatPanelGridButtonName[2],
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
             &XpatPanelGridButtonName[3],
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

     XpatPanelItem XpatViewGridPanel =

         {
           "Grid",
           1,
           0,
           XPAT_VIEW_GRID_X,
           XPAT_VIEW_GRID_Y, 
           100,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XpatViewGridButton
         };

  int XpatViewGridDefaultValues[ 5 ] =

         {
           XPAT_VIEW_GRID_X,
           XPAT_VIEW_GRID_Y, 
           100, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Layer                          |
|                                                             |
\------------------------------------------------------------*/

 static char *XpatViewLayerButtonBackground = "Black";
 static char *XpatViewLayerButtonForeground = "White";

 static char *XpatPanelLayerButtonName[] =

  {
    "All visible",
    "All invisible",
    "Vector Text",
    "IO Text",
    "Apply",
    "Close"
  };

  static char *XpatViewLayerCursorType[] =

  {
    "Invert",
    "Xor"
  };

  static char *XpatViewLayerForceDisplay[] =

  {
    "Quick display",
    "Force display"
  };

  static char *XpatViewLayerScale[] =
  {
    "Scale +",
    "Scale -"
  };

 XpatPanelButtonItem XpatViewLayerButton[] =

         {
           {
             &XpatPanelLayerButtonName[0],
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
             &XpatPanelLayerButtonName[1],
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
             &XPAT_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 0 ][ 2 ],
             0, 1,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 0 ][ 2 ],
             1, 1,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 1 ][ 2 ],
             0, 2,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 1 ][ 2 ],
             1, 2,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 2 ][ 2 ],
             0, 3,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 2 ][ 2 ],
             1, 3,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 3 ][ 2 ],
             0, 4,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 3 ][ 2 ],
             1, 4,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 4 ][ 2 ],
             0, 5,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 4 ][ 2 ],
             1, 5,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 5 ][ 2 ],
             0, 6,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 5 ][ 2 ],
             1, 6,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 6 ][ 2 ],
             0, 7,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 6 ][ 2 ],
             1, 7,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 7 ][ 2 ],
             0, 8,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 7 ][ 2 ],
             1, 8,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 8 ][ 2 ],
             0, 9,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 8 ][ 2 ],
             1, 9,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 9 ][ 2 ],
             0, 10,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XPAT_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XPAT_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XPAT_LAYER_NAME_TABLE[ 9 ][ 2 ],
             1, 10,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XpatPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XpatViewLayerButtonForeground,
             &XpatViewLayerButtonBackground,
             0, 11,
             1, 1,
             CallbackNameVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XpatPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XpatViewLayerButtonForeground,
             &XpatViewLayerButtonBackground,
             1, 11,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XpatPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XpatViewLayerButtonForeground,
             &XpatViewLayerButtonBackground,
             0, 12,
             1, 1,
             CallbackNameVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XpatPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XpatViewLayerButtonForeground,
             &XpatViewLayerButtonBackground,
             1, 12,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XpatViewLayerCursorType[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 13,
             1, 1,
             CallbackLayerCursor,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XpatViewLayerForceDisplay[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 13,
             1, 1,
             CallbackLayerForceDisplay,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XpatViewLayerScale[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 14,
             1, 1,
             CallbackLayerScale,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XpatViewLayerScale[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 14,
             1, 1,
             CallbackLayerScale,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XpatPanelLayerButtonName[4],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 15,
             1, 1,
             CallbackLayerApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XpatPanelLayerButtonName[5],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 15,
             1, 1,
             CallbackCloseLayer,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

     XpatPanelItem XpatViewLayerPanel =

         {
           "Layer",
           1,
           0,
           XPAT_VIEW_LAYER_X,
           XPAT_VIEW_LAYER_Y, 
           200,
           400,
           2,  
           16,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XpatViewLayerButton
         };

  int XpatViewLayerDefaultValues[ 5 ] =

         {
           XPAT_VIEW_LAYER_X,
           XPAT_VIEW_LAYER_Y, 
           200, 400, 0
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

  XpatZoomLeft();

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

  XpatZoomRight();

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

  XpatZoomUp();

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

  XpatZoomDown();

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

  XpatEnterDialog( &XpatViewMoveSetDialog );

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

  XpatExitPanel( &XpatViewArrowsPanel );

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

  XpatZoomRefresh();

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

  XpatZoomUndo();

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

  XpatZoomLess();

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

  XpatZoomMore();

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

  XpatEnterDialog( &XpatViewZoomSetDialog );

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

  XpatExitPanel( &XpatViewZoomPanel );

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

  XpatChangeEditMode( XPAT_ZOOM_IN,
                       XpatPromptZoomIn );

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

  XpatChangeEditMode( XPAT_ZOOM_CENTER,
                       XpatPromptZoomCenter );

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

  XpatEnterDialog( &XpatViewZoomGotoDialog );

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

  XpatChangeEditMode( XPAT_ZOOM_PAN,
                       XpatPromptZoomPan );

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

  XpatZoomFit();

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

  XpatEnterDialog( &XpatViewGridXDialog );

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

  XpatEnterDialog( &XpatViewGridYDialog );

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

  XpatUnitUserGrid = ! XpatUnitUserGrid;

  if ( XpatUnitUserGrid == XPAT_FALSE )
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

  XpatZoomRefresh();

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

  XpatExitPanel( &XpatViewGridPanel );

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

  XpatLayerAllVisible();

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

  XpatLayerAllInvisible();

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

  if ( XPAT_ACTIVE_LAYER_TABLE[ (int)ClientData ] == 0 )
  {
    XpatSetLayerVisible( (int)ClientData );
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

  if ( XPAT_ACTIVE_LAYER_TABLE[ (int)ClientData ] )
  {
    XpatSetLayerInvisible( (int)ClientData );
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

  if ( XPAT_ACTIVE_NAME_TABLE[ (int)ClientData ] == 0 )
  {
    XpatSetNameVisible( ClientData );
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

  if ( XPAT_ACTIVE_NAME_TABLE[ (int)ClientData ] )
  {
    XpatSetNameInvisible( ClientData );
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

  XPAT_XOR_CURSOR = ! XPAT_XOR_CURSOR;
  XpatChangeCursor( XPAT_XOR_CURSOR );

  XpatChangeCursorContext();

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

  XPAT_FORCE_DISPLAY = ! XPAT_FORCE_DISPLAY;
  XpatChangeForceDisplay( XPAT_FORCE_DISPLAY );

  autend();
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
  autbegin();

  XpatReScale( ClientData );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatChangeCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XpatChangeCursor( Type )

  char Type;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 2 + ( ( XPAT_MAX_LAYER + XPAT_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XpatViewLayerCursorType[ Type ] );

  XtVaSetValues( XpatViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatChangeForceDisplay                |
|                                                             |
\------------------------------------------------------------*/

void XpatChangeForceDisplay( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 3 + ( ( XPAT_MAX_LAYER + XPAT_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XpatViewLayerForceDisplay[ Index ] );

  XtVaSetValues( XpatViewLayerButton[ Pos ].BUTTON,
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

  XpatZoomRefresh();

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

  XpatExitPanel( &XpatViewLayerPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatSetLayerVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XpatSetLayerVisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = ( Layer << 1 ) + 2;

  if ( XpatViewLayerButton[ Index ].BUTTON != NULL )
  {
    XPAT_ACTIVE_LAYER_TABLE[ Layer ] = 1;

    XtVaSetValues( XpatViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME                   ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME                   ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XpatViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XpatViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
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
|                        XpatSetNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XpatSetNameVisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 2 + ( ( XPAT_MAX_LAYER + Name ) << 1 );

  if ( XpatViewLayerButton[ Index ].BUTTON != NULL )
  {
    XPAT_ACTIVE_NAME_TABLE[ Name ] = 1;

    XtVaSetValues( XpatViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME              ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME                   ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XpatViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XpatViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
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
|                     XpatSetLayerInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XpatSetLayerInvisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( Layer << 1 );

  if ( XpatViewLayerButton[ Index ].BUTTON != NULL )
  {
    XPAT_ACTIVE_LAYER_TABLE[ Layer ] = 0;

    XtVaSetValues( XpatViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME                   ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME                   ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XpatViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XpatViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
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
|                      XpatSetNameInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XpatSetNameInvisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( ( XPAT_MAX_LAYER + Name ) << 1 );

  if ( XpatViewLayerButton[ Index ].BUTTON != NULL )
  {
    XPAT_ACTIVE_NAME_TABLE[ Name ] = 0;

    XtVaSetValues( XpatViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME                   ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XPAT_BLACK_COLOR_NAME                   ,
                   strlen( XPAT_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XpatViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XpatViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XpatViewLayerButton[ Index ].BUTTON  ,
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
|                       XpatLayerAllVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XpatLayerAllVisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XPAT_MAX_LAYER; Index++ )
  {
    XpatSetLayerVisible( Index );
  }

  for ( Index = 0; Index < XPAT_MAX_ACTIVE_NAME; Index++ )
  {
    XpatSetNameVisible( Index );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatInitializeLayer                     |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeLayer()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XPAT_MAX_LAYER; Index++ )
  {
    if ( XPAT_ACTIVE_LAYER_TABLE[ Index ] == 0 )
    {
      XpatSetLayerInvisible( Index );
    }
    else
    {
      XpatSetLayerVisible( Index );
    }
  }

  for ( Index = 0; Index < XPAT_MAX_ACTIVE_NAME; Index++ )
  {
    if ( XPAT_ACTIVE_NAME_TABLE[ Index ] == 0 )
    {
      XpatSetNameInvisible( Index );
    }
    else
    {
      XpatSetNameVisible( Index );
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatLayerAllInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XpatLayerAllInvisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XPAT_MAX_LAYER; Index++ )
  {
    XpatSetLayerInvisible( Index );
  }

  for ( Index = 0; Index < XPAT_MAX_ACTIVE_NAME; Index++ )
  {
    XpatSetNameInvisible( Index );
  }

  autend();
}
