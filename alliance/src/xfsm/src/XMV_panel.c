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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
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

  static char XfsmPixmapDownArrow[] =

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

  static char XfsmPixmapLeftArrow[] =

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

  static char XfsmPixmapRightArrow[] =

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

  static char XfsmPixmapUpArrow[] =

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

   static char *XfsmPanelArrowsButtonName[] = 

   {
     "Left",  
     "Up",
     "Down",
     "Right",
     "Move Set",
     "Close"
   };

   static XfsmPanelButtonItem XfsmViewArrowsButton[] =

         {
           {
             &XfsmPanelArrowsButtonName[0],
             NULL, XfsmPixmapLeftArrow, 32, 32,
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
             &XfsmPanelArrowsButtonName[1],
             NULL, XfsmPixmapUpArrow, 32, 32,
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
             &XfsmPanelArrowsButtonName[2],
             NULL, XfsmPixmapDownArrow, 32, 32,
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
             &XfsmPanelArrowsButtonName[3],
             NULL, XfsmPixmapRightArrow, 32, 32,
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
             &XfsmPanelArrowsButtonName[4],
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
             &XfsmPanelArrowsButtonName[5],
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

   XfsmPanelItem XfsmViewArrowsPanel =

         {
           "Arrows",
           1,
           0,
           XFSM_VIEW_ARROWS_X,
           XFSM_VIEW_ARROWS_Y, 
           100,
           125,
           2,  
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmViewArrowsButton
         };

   int XfsmViewArrowsDefaultValues[ 5 ] =

         {
           XFSM_VIEW_ARROWS_X, 
           XFSM_VIEW_ARROWS_Y,
           100, 125 , 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Zoom                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XfsmPanelZoomButtonName[] =

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


 XfsmPanelButtonItem XfsmViewZoomButton[] =

         {
           {
             &XfsmPanelZoomButtonName[0],
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
             &XfsmPanelZoomButtonName[1],
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
             &XfsmPanelZoomButtonName[2],
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
             &XfsmPanelZoomButtonName[3],
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
             &XfsmPanelZoomButtonName[4],
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
             &XfsmPanelZoomButtonName[5],
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
             &XfsmPanelZoomButtonName[6],
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
             &XfsmPanelZoomButtonName[7],
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
             &XfsmPanelZoomButtonName[8],
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
             &XfsmPanelZoomButtonName[9],
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
             &XfsmPanelZoomButtonName[10],
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

   XfsmPanelItem XfsmViewZoomPanel =

         {
           "Zoom",
           1,
           0,
           XFSM_VIEW_ZOOM_X,
           XFSM_VIEW_ZOOM_Y, 
           100,
           275,
           1,  
           11,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmViewZoomButton
         };

   int XfsmViewZoomDefaultValues[ 5 ] =

         {
           XFSM_VIEW_ZOOM_X, 
           XFSM_VIEW_ZOOM_Y,
           100, 275, 1
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Grid                           |
|                                                             |
\------------------------------------------------------------*/

 static char *XfsmPanelGridButtonName[] =

   {
     "Grid X",
     "Grid Y",
     "Grid Off",
     "Close"
   };


 XfsmPanelButtonItem XfsmViewGridButton[] =

         {
           {
             &XfsmPanelGridButtonName[0],
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
             &XfsmPanelGridButtonName[1],
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
             &XfsmPanelGridButtonName[2],
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
             &XfsmPanelGridButtonName[3],
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

     XfsmPanelItem XfsmViewGridPanel =

         {
           "Grid",
           1,
           0,
           XFSM_VIEW_GRID_X,
           XFSM_VIEW_GRID_Y, 
           100,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmViewGridButton
         };

  int XfsmViewGridDefaultValues[ 5 ] =

         {
           XFSM_VIEW_GRID_X,
           XFSM_VIEW_GRID_Y, 
           100, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Panel Layer                          |
|                                                             |
\------------------------------------------------------------*/

 static char *XfsmViewLayerButtonBackground = "Black";
 static char *XfsmViewLayerButtonForeground = "White";

 static char *XfsmPanelLayerButtonName[] =

  {
    "All visible",
    "All invisible",
    "State Text",
    "Trans Text",
    "Apply",
    "Close"
  };

  static char *XfsmViewLayerCursorType[] =

  {
    "Invert",
    "Xor"
  };

  static char *XfsmViewLayerForceDisplay[] =

  {
    "Quick display",
    "Force display"
  };
  
 XfsmPanelButtonItem XfsmViewLayerButton[] =

         {
           {
             &XfsmPanelLayerButtonName[0],
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
             &XfsmPanelLayerButtonName[1],
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
             &XFSM_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 0 ][ 2 ],
             0, 1,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 0 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 0 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 0 ][ 2 ],
             1, 1,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 1 ][ 2 ],
             0, 2,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 1 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 1 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 1 ][ 2 ],
             1, 2,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 2 ][ 2 ],
             0, 3,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 2 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 2 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 2 ][ 2 ],
             1, 3,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)2,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 3 ][ 2 ],
             0, 4,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 3 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 3 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 3 ][ 2 ],
             1, 4,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)3,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 4 ][ 2 ],
             0, 5,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 4 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 4 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 4 ][ 2 ],
             1, 5,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)4,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 5 ][ 2 ],
             0, 6,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 5 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 5 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 5 ][ 2 ],
             1, 6,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)5,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 6 ][ 2 ],
             0, 7,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 6 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 6 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 6 ][ 2 ],
             1, 7,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)6,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 7 ][ 2 ],
             0, 8,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 7 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 7 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 7 ][ 2 ],
             1, 8,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)7,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 8 ][ 2 ],
             0, 9,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 8 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 8 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 8 ][ 2 ],
             1, 9,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)8,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 9 ][ 2 ],
             0, 10,
             1, 1,
             CallbackLayerVisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XFSM_LAYER_NAME_TABLE[ 9 ][ 0 ],
             NULL, NULL, 0, 0,
             &XFSM_LAYER_NAME_TABLE[ 9 ][ 1 ],
             &XFSM_LAYER_NAME_TABLE[ 9 ][ 2 ],
             1, 10,
             1, 1,
             CallbackLayerInvisible,
             (XtPointer)9,
             (Widget)NULL
           }
           ,
           {
             &XfsmPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XfsmViewLayerButtonForeground,
             &XfsmViewLayerButtonBackground,
             0, 11,
             1, 1,
             CallbackNameVisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XfsmPanelLayerButtonName[2],
             NULL, NULL, 0, 0,
             &XfsmViewLayerButtonForeground,
             &XfsmViewLayerButtonBackground,
             1, 11,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             &XfsmPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XfsmViewLayerButtonForeground,
             &XfsmViewLayerButtonBackground,
             0, 12,
             1, 1,
             CallbackNameVisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XfsmPanelLayerButtonName[3],
             NULL, NULL, 0, 0,
             &XfsmViewLayerButtonForeground,
             &XfsmViewLayerButtonBackground,
             1, 12,
             1, 1,
             CallbackNameInvisible,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XfsmViewLayerCursorType[0],
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
             &XfsmViewLayerForceDisplay[0],
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
             &XfsmPanelLayerButtonName[4],
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
             &XfsmPanelLayerButtonName[5],
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
           NULL
         };

     XfsmPanelItem XfsmViewLayerPanel =

         {
           "Layer",
           1,
           0,
           XFSM_VIEW_LAYER_X,
           XFSM_VIEW_LAYER_Y, 
           200,
           375,
           2,  
           15,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmViewLayerButton
         };

  int XfsmViewLayerDefaultValues[ 5 ] =

         {
           XFSM_VIEW_LAYER_X,
           XFSM_VIEW_LAYER_Y, 
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

  XfsmZoomLeft();

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

  XfsmZoomRight();

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

  XfsmZoomUp();

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

  XfsmZoomDown();

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

  XfsmEnterDialog( &XfsmViewMoveSetDialog );

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

  XfsmExitPanel( &XfsmViewArrowsPanel );

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

  XfsmZoomRefresh();

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

  XfsmZoomUndo();

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

  XfsmZoomLess();

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

  XfsmZoomMore();

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

  XfsmEnterDialog( &XfsmViewZoomSetDialog );

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

  XfsmExitPanel( &XfsmViewZoomPanel );

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

  XfsmChangeEditMode( XFSM_ZOOM_IN,
                       XfsmPromptZoomIn );

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

  XfsmChangeEditMode( XFSM_ZOOM_CENTER,
                       XfsmPromptZoomCenter );

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

  XfsmEnterDialog( &XfsmViewZoomGotoDialog );

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

  XfsmChangeEditMode( XFSM_ZOOM_PAN,
                       XfsmPromptZoomPan );

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

  XfsmZoomFit();

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

  XfsmEnterDialog( &XfsmViewGridXDialog );

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

  XfsmEnterDialog( &XfsmViewGridYDialog );

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

  XfsmUnitUserGrid = ! XfsmUnitUserGrid;

  if ( XfsmUnitUserGrid == XFSM_FALSE )
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

  XfsmZoomRefresh();

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

  XfsmExitPanel( &XfsmViewGridPanel );

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

  XfsmLayerAllVisible();

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

  XfsmLayerAllInvisible();

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

  if ( XFSM_ACTIVE_LAYER_TABLE[ (long)ClientData ] == 0 )
  {
    XfsmSetLayerVisible( (long)ClientData );
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

  if ( XFSM_ACTIVE_LAYER_TABLE[ (long)ClientData ] )
  {
    XfsmSetLayerInvisible( (long)ClientData );
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

  if ( XFSM_ACTIVE_NAME_TABLE[ (long)ClientData ] == 0 )
  {
    XfsmSetNameVisible( ClientData );
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

  if ( XFSM_ACTIVE_NAME_TABLE[ (long)ClientData ] )
  {
    XfsmSetNameInvisible( ClientData );
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

  XFSM_XOR_CURSOR = ! XFSM_XOR_CURSOR;
  XfsmChangeCursor( XFSM_XOR_CURSOR );

  XfsmChangeCursorContext();

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

  XFSM_FORCE_DISPLAY = ! XFSM_FORCE_DISPLAY;
  XfsmChangeForceDisplay( XFSM_FORCE_DISPLAY );

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                      XfsmChangeCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmChangeCursor( Type )

  char Type;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 2 + ( ( XFSM_MAX_LAYER + XFSM_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XfsmViewLayerCursorType[ Type ] );

  XtVaSetValues( XfsmViewLayerButton[ Pos ].BUTTON,
                 XmNlabelString, Label,
                 XmNresizable  , False,
                 NULL );

  XmStringFree( Label );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmChangeForceDisplay                |
|                                                             |
\------------------------------------------------------------*/

void XfsmChangeForceDisplay( Index )

  char Index;
{
  int      Pos;
  XmString Label;

  autbegin();

  Pos   = 3 + ( ( XFSM_MAX_LAYER + XFSM_MAX_ACTIVE_NAME ) << 1 );
  Label = XmStringCreateSimple( XfsmViewLayerForceDisplay[ Index ] );

  XtVaSetValues( XfsmViewLayerButton[ Pos ].BUTTON,
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

  XfsmZoomRefresh();

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

  XfsmExitPanel( &XfsmViewLayerPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmSetLayerVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmSetLayerVisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = ( Layer << 1 ) + 2;

  if ( XfsmViewLayerButton[ Index ].BUTTON != NULL )
  {
    XFSM_ACTIVE_LAYER_TABLE[ Layer ] = 1;

    XtVaSetValues( XfsmViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME                   ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME                   ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XfsmViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XfsmViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
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
|                        XfsmSetNameVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmSetNameVisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 2 + ( ( XFSM_MAX_LAYER + Name ) << 1 );

  if ( XfsmViewLayerButton[ Index ].BUTTON != NULL )
  {
    XFSM_ACTIVE_NAME_TABLE[ Name ] = 1;

    XtVaSetValues( XfsmViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME              ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index + 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME                   ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XfsmViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XfsmViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg            ,
                   XmNbackground           ,
                   XmRString               ,
                   Background              ,
                   strlen( Background ) + 1,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
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
|                     XfsmSetLayerInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmSetLayerInvisible( Layer )

  int Layer;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( Layer << 1 );

  if ( XfsmViewLayerButton[ Index ].BUTTON != NULL )
  {
    XFSM_ACTIVE_LAYER_TABLE[ Layer ] = 0;

    XtVaSetValues( XfsmViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME                   ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME                   ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XfsmViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XfsmViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
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
|                      XfsmSetNameInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmSetNameInvisible( Name )

  int Name;
{
  int   Index;
  char *Background;
  char *Foreground;

  autbegin();

  Index = 3 + ( ( XFSM_MAX_LAYER + Name ) << 1 );

  if ( XfsmViewLayerButton[ Index ].BUTTON != NULL )
  {
    XFSM_ACTIVE_NAME_TABLE[ Name ] = 0;

    XtVaSetValues( XfsmViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNbackground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME                   ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index - 1 ].BUTTON,
                   XtVaTypedArg                            ,
                   XmNforeground                           ,
                   XmRString                               ,
                   XFSM_BLACK_COLOR_NAME                   ,
                   strlen( XFSM_BLACK_COLOR_NAME ) + 1     ,
                   NULL
                 );

    Background = *XfsmViewLayerButton[ Index ].BACKGROUND;
    Foreground = *XfsmViewLayerButton[ Index ].FOREGROUND;

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
                   XtVaTypedArg                          ,
                   XmNbackground                         ,
                   XmRString                             ,
                   Background                            ,
                   strlen( Background ) + 1              ,
                   NULL
                 );

    XtVaSetValues( XfsmViewLayerButton[ Index ].BUTTON  ,
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
|                       XfsmLayerAllVisible                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmLayerAllVisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XFSM_MAX_LAYER; Index++ )
  {
    XfsmSetLayerVisible( Index );
  }

  for ( Index = 0; Index < XFSM_MAX_ACTIVE_NAME; Index++ )
  {
    XfsmSetNameVisible( Index );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmInitializeLayer                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeLayer()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XFSM_MAX_LAYER; Index++ )
  {
    if ( XFSM_ACTIVE_LAYER_TABLE[ Index ] == 0 )
    {
      XfsmSetLayerInvisible( Index );
    }
    else
    {
      XfsmSetLayerVisible( Index );
    }
  }

  for ( Index = 0; Index < XFSM_MAX_ACTIVE_NAME; Index++ )
  {
    if ( XFSM_ACTIVE_NAME_TABLE[ Index ] == 0 )
    {
      XfsmSetNameInvisible( Index );
    }
    else
    {
      XfsmSetNameVisible( Index );
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmLayerAllInvisible                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmLayerAllInvisible()

{
  int   Index;

  autbegin();

  for ( Index = 0; Index < XFSM_MAX_LAYER; Index++ )
  {
    XfsmSetLayerInvisible( Index );
  }

  for ( Index = 0; Index < XFSM_MAX_ACTIVE_NAME; Index++ )
  {
    XfsmSetNameInvisible( Index );
  }

  autend();
}
