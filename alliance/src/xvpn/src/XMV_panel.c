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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                    XVPN                           |
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

  static char XvpnPixmapDownArrow[] =

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

  static char XvpnPixmapLeftArrow[] =

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

  static char XvpnPixmapRightArrow[] =

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

  static char XvpnPixmapUpArrow[] =

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

   static char *XvpnPanelArrowsButtonName[] = 

   {
     "Left",  
     "Up",
     "Down",
     "Right",
     "Move Set",
     "Close"
   };

   static XvpnPanelButtonItem XvpnViewArrowsButton[] =

         {
           {
             &XvpnPanelArrowsButtonName[0],
             NULL, XvpnPixmapLeftArrow, 32, 32,
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
             &XvpnPanelArrowsButtonName[1],
             NULL, XvpnPixmapUpArrow, 32, 32,
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
             &XvpnPanelArrowsButtonName[2],
             NULL, XvpnPixmapDownArrow, 32, 32,
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
             &XvpnPanelArrowsButtonName[3],
             NULL, XvpnPixmapRightArrow, 32, 32,
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
             &XvpnPanelArrowsButtonName[4],
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
             &XvpnPanelArrowsButtonName[5],
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
           { NULL }
         };

   XvpnPanelItem XvpnViewArrowsPanel =

         {
           "Arrows",
           1,
           0,
           XVPN_VIEW_ARROWS_X,
           XVPN_VIEW_ARROWS_Y, 
           100,
           125,
           2,  
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XvpnViewArrowsButton
         };

   int XvpnViewArrowsDefaultValues[ 5 ] =

         {
           XVPN_VIEW_ARROWS_X, 
           XVPN_VIEW_ARROWS_Y,
           100, 125 , 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Zoom                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XvpnPanelZoomButtonName[] =

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


 XvpnPanelButtonItem XvpnViewZoomButton[] =

         {
           {
             &XvpnPanelZoomButtonName[0],
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
             &XvpnPanelZoomButtonName[1],
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
             &XvpnPanelZoomButtonName[2],
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
             &XvpnPanelZoomButtonName[3],
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
             &XvpnPanelZoomButtonName[4],
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
             &XvpnPanelZoomButtonName[5],
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
             &XvpnPanelZoomButtonName[6],
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
             &XvpnPanelZoomButtonName[7],
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
             &XvpnPanelZoomButtonName[8],
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
             &XvpnPanelZoomButtonName[9],
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
             &XvpnPanelZoomButtonName[10],
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
           { NULL }
         };

   XvpnPanelItem XvpnViewZoomPanel =

         {
           "Zoom",
           1,
           0,
           XVPN_VIEW_ZOOM_X,
           XVPN_VIEW_ZOOM_Y, 
           100,
           275,
           1,  
           11,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XvpnViewZoomButton
         };

   int XvpnViewZoomDefaultValues[ 5 ] =

         {
           XVPN_VIEW_ZOOM_X, 
           XVPN_VIEW_ZOOM_Y,
           100, 275, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Grid                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XvpnPanelGridButtonName[] =

   {
     "Grid X",
     "Grid Y",
     "Grid Off",
     "Close"
   };


 XvpnPanelButtonItem XvpnViewGridButton[] =

         {
           {
             &XvpnPanelGridButtonName[0],
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
             &XvpnPanelGridButtonName[1],
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
             &XvpnPanelGridButtonName[2],
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
             &XvpnPanelGridButtonName[3],
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
           { NULL }
         };

     XvpnPanelItem XvpnViewGridPanel =

         {
           "Grid",
           1,
           0,
           XVPN_VIEW_GRID_X,
           XVPN_VIEW_GRID_Y, 
           100,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XvpnViewGridButton
         };

  int XvpnViewGridDefaultValues[ 5 ] =

         {
           XVPN_VIEW_GRID_X,
           XVPN_VIEW_GRID_Y, 
           100, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Layer                          |
|                                                             |
\------------------------------------------------------------*/

 static char *XvpnViewLayerButtonBackground = "Black";
 static char *XvpnViewLayerButtonForeground = "White";

 static char *XvpnPanelLayerButtonName[] =

  {
    "All visible",
    "All invisible",
    "Place Text",
    "Trans Text",
    "Apply",
    "Close"
  };

  static char *XvpnViewLayerCursorType[] =

  {
    "Invert",
    "Xor"
  };

  static char *XvpnViewLayerForceDisplay[] =

  {
    "Quick display",
    "Force display"
  };

 XvpnPanelButtonItem XvpnViewLayerButton[] =

         {
           {
             &XvpnPanelLayerButtonName[0],
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
             &XvpnPanelLayerButtonName[1],
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
             &XVPN_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 0 ][ 2 ],
             0, 1,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 0 ][ 2 ],
             1, 1,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 1 ][ 2 ],
             0, 2,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 1 ][ 2 ],
             1, 2,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 2 ][ 2 ],
             0, 3,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 2 ][ 2 ],
             1, 3,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 3 ][ 2 ],
             0, 4,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 3 ][ 2 ],
             1, 4,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 4 ][ 2 ],
             0, 5,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 4 ][ 2 ],
             1, 5,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 5 ][ 2 ],
             0, 6,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 5 ][ 2 ],
             1, 6,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 6 ][ 2 ],
             0, 7,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 6 ][ 2 ],
             1, 7,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 7 ][ 2 ],
             0, 8,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 7 ][ 2 ],
             1, 8,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 8 ][ 2 ],
             0, 9,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 8 ][ 2 ],
             1, 9,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 9 ][ 2 ],
             0, 10,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XVPN_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XVPN_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XVPN_LAYER_NAME_TABLE[ 9 ][ 2 ],
             1, 10,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XvpnPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XvpnViewLayerButtonForeground,
             &XvpnViewLayerButtonBackground,
             0, 11,
             1, 1,
             CallbackNameVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XvpnPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XvpnViewLayerButtonForeground,
             &XvpnViewLayerButtonBackground,
             1, 11,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XvpnPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XvpnViewLayerButtonForeground,
             &XvpnViewLayerButtonBackground,
             0, 12,
             1, 1,
             CallbackNameVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XvpnPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XvpnViewLayerButtonForeground,
             &XvpnViewLayerButtonBackground,
             1, 12,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XvpnViewLayerCursorType[0],
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
             &XvpnViewLayerForceDisplay[0],
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
             &XvpnPanelLayerButtonName[4],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 14,
             1, 1,
             CallbackLayerApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XvpnPanelLayerButtonName[5],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 14,
             1, 1,
             CallbackCloseLayer,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           { NULL }
         };

     XvpnPanelItem XvpnViewLayerPanel =

         {
           "Layer",
           1,
           0,
           XVPN_VIEW_LAYER_X,
           XVPN_VIEW_LAYER_Y, 
           200,
           375,
           2,  
           15,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XvpnViewLayerButton
         };

  int XvpnViewLayerDefaultValues[ 5 ] =

         {
           XVPN_VIEW_LAYER_X,
           XVPN_VIEW_LAYER_Y, 
           200, 375, 0
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

  XvpnZoomLeft();

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

  XvpnZoomRight();

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

  XvpnZoomUp();

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

  XvpnZoomDown();

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

  XvpnEnterDialog( &XvpnViewMoveSetDialog );

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

  XvpnExitPanel( &XvpnViewArrowsPanel );

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

  XvpnZoomRefresh();

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

  XvpnZoomUndo();

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

  XvpnZoomLess();

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

  XvpnZoomMore();

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

  XvpnEnterDialog( &XvpnViewZoomSetDialog );

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

  XvpnExitPanel( &XvpnViewZoomPanel );

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

  XvpnChangeEditMode( XVPN_ZOOM_IN,
                       XvpnPromptZoomIn );

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

  XvpnChangeEditMode( XVPN_ZOOM_CENTER,
                       XvpnPromptZoomCenter );

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

  XvpnEnterDialog( &XvpnViewZoomGotoDialog );

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

  XvpnChangeEditMode( XVPN_ZOOM_PAN,
                       XvpnPromptZoomPan );

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

  XvpnZoomFit();

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

  XvpnEnterDialog( &XvpnViewGridXDialog );

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

  XvpnEnterDialog( &XvpnViewGridYDialog );

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

  XvpnUnitUserGrid = ! XvpnUnitUserGrid;

  if ( XvpnUnitUserGrid == XVPN_FALSE )
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

  XvpnZoomRefresh();

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

  XvpnExitPanel( &XvpnViewGridPanel );

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

  XvpnLayerAllVisible();

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

  XvpnLayerAllInvisible();

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

  if ( XVPN_ACTIVE_LAYER_TABLE[ (long)ClientData ] == 0 )
  {
    XvpnSetLayerVisible( (long)ClientData );
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

  if ( XVPN_ACTIVE_LAYER_TABLE[ (long)ClientData ] )
  {
    XvpnSetLayerInvisible( (long)ClientData );
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

  if ( XVPN_ACTIVE_NAME_TABLE[ (long)ClientData ] == 0 )
  {
    XvpnSetNameVisible( ClientData );
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

  if ( XVPN_ACTIVE_NAME_TABLE[ (long)ClientData ] )
  {
    XvpnSetNameInvisible( ClientData );
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

  XVPN_XOR_CURSOR = ! XVPN_XOR_CURSOR;
  XvpnChangeCursor( XVPN_XOR_CURSOR );

  XvpnChangeCursorContext();

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

  XVPN_FORCE_DISPLAY = ! XVPN_FORCE_DISPLAY;
  XvpnChangeForceDisplay( XVPN_FORCE_DISPLAY );

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XvpnChangeCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnChangeCursor( Type )

  int Type;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 2 + ( ( XVPN_MAX_LAYER + XVPN_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XvpnViewLayerCursorType[ Type ] );

  XtVaSetValues( XvpnViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnChangeForceDisplay                |
|                                                             |
\------------------------------------------------------------*/

void XvpnChangeForceDisplay( Index )

  int Index;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 3 + ( ( XVPN_MAX_LAYER + XVPN_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XvpnViewLayerForceDisplay[ Index ] );

  XtVaSetValues( XvpnViewLayerButton[ Pos ].BUTTON,
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

  XvpnZoomRefresh();

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

  XvpnExitPanel( &XvpnViewLayerPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XvpnSetLayerVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnSetLayerVisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = ( Layer << 1 ) + 2;

  if ( XvpnViewLayerButton[ Index ].BUTTON != NULL )
  {
    XVPN_ACTIVE_LAYER_TABLE[ Layer ] = 1;

    XtVaSetValues( XvpnViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XvpnViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XvpnViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
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
|                        XvpnSetNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnSetNameVisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 2 + ( ( XVPN_MAX_LAYER + Name ) << 1 );

  if ( XvpnViewLayerButton[ Index ].BUTTON != NULL )
  {
    XVPN_ACTIVE_NAME_TABLE[ Name ] = 1;

    XtVaSetValues( XvpnViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XvpnViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XvpnViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
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
|                     XvpnSetLayerInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnSetLayerInvisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( Layer << 1 );

  if ( XvpnViewLayerButton[ Index ].BUTTON != NULL )
  {
    XVPN_ACTIVE_LAYER_TABLE[ Layer ] = 0;

    XtVaSetValues( XvpnViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XvpnViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XvpnViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
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
|                      XvpnSetNameInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnSetNameInvisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( ( XVPN_MAX_LAYER + Name ) << 1 );

  if ( XvpnViewLayerButton[ Index ].BUTTON != NULL )
  {
    XVPN_ACTIVE_NAME_TABLE[ Name ] = 0;

    XtVaSetValues( XvpnViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XVPN_BLACK_COLOR_NAME                   ,
                   strlen( XVPN_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XvpnViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XvpnViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XvpnViewLayerButton[ Index ].BUTTON  ,
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
|                       XvpnLayerAllVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnLayerAllVisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XVPN_MAX_LAYER; Index++ )
  {
    XvpnSetLayerVisible( Index );
  }

  for ( Index = 0; Index < XVPN_MAX_ACTIVE_NAME; Index++ )
  {
    XvpnSetNameVisible( Index );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnInitializeLayer                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeLayer()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XVPN_MAX_LAYER; Index++ )
  {
    if ( XVPN_ACTIVE_LAYER_TABLE[ Index ] == 0 )
    {
      XvpnSetLayerInvisible( Index );
    }
    else
    {
      XvpnSetLayerVisible( Index );
    }
  }

  for ( Index = 0; Index < XVPN_MAX_ACTIVE_NAME; Index++ )
  {
    if ( XVPN_ACTIVE_NAME_TABLE[ Index ] == 0 )
    {
      XvpnSetNameInvisible( Index );
    }
    else
    {
      XvpnSetNameVisible( Index );
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnLayerAllInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnLayerAllInvisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XVPN_MAX_LAYER; Index++ )
  {
    XvpnSetLayerInvisible( Index );
  }

  for ( Index = 0; Index < XVPN_MAX_ACTIVE_NAME; Index++ )
  {
    XvpnSetNameInvisible( Index );
  }

  autend();
}
