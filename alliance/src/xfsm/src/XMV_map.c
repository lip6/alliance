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
| Tool    :                   XFSM                            |
|                                                             |
| File    :                   Map.c                           |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/PushB.h>
# include <Xm/DrawingA.h>
 
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
# include "XTB.h"
# include "XMX.h"
# include "XMV.h"

# include "XMV_map.h"

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

   XfsmPanelItem XfsmViewMapPanel =

         {
           "Map",
           1,
           0,
           XFSM_VIEW_MAP_X,
           XFSM_VIEW_MAP_Y,
           190,
           200,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XfsmPanelButtonItem *)NULL 
         };

   int XfsmViewMapDefaultValues[ 5 ] =

         {
           XFSM_VIEW_MAP_X, 
           XFSM_VIEW_MAP_Y,
           190, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Graphic Map Window                     |
|                                                             |
\------------------------------------------------------------*/

  Widget     XfsmMapWindow;
  Widget     XfsmMapButtonCompute;
  Widget     XfsmMapButtonClose;

  Dimension  XfsmMapDx    = 0;
  Dimension  XfsmMapDy    = 0;

  static char XfsmFirstEnterMap = XFSM_TRUE;
  static char XfsmFirstExpose   = XFSM_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                           Unit Map                          |
|                                                             |
\------------------------------------------------------------*/

  float      XfsmUnitMapStep;

  long       XfsmUnitMapX;
  long       XfsmUnitMapY;
  long       XfsmUnitMapDx;
  long       XfsmUnitMapDy;

  long       XfsmBoundMapX; 
  long       XfsmBoundMapY; 
  long       XfsmBoundMapDx; 
  long       XfsmBoundMapDy; 

  long       XfsmPixelMapX;
  long       XfsmPixelMapY;

/*------------------------------------------------------------\
|                                                             |
|                            Expose                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Translation                        |
|                                                             |
\------------------------------------------------------------*/

  static String XfsmMapEventTranslation =

    "<Btn1Up>:      CallbackMapEvent( 0 )\n\
     <Btn2Up>:      CallbackMapEvent( 1 )\n\
     <Btn3Up>:      CallbackMapEvent( 2 )";

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Event Callback                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        CallbackCloseMap                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackCloseMap( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XfsmExitPanel( &XfsmViewMapPanel );

  autend();
} 

/*------------------------------------------------------------\
|                                                             |
|                        CallbackMapCompute                   |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackMapCompute( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XfsmComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackMapEvent                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackMapEvent ( MyWidget, Event, Args, Argc )

     Widget        MyWidget;
     XButtonEvent *Event;
     String       *Args;
     int          *Argc;
{
  Display  *EventDisplay;
  char      MouseEvent;
  long      X;
  long      Y;

  autbegin();

  EventDisplay = Event->display;
  MouseEvent   = Args[ 0 ][ 0 ] - '0';

  X = Event->x;
  Y = XfsmMapDy - Event->y;

  X = X + XfsmPixelMapX;
  Y = Y + XfsmPixelMapY;

  if ( X < 0 )
  {
    X = ((float)(X) / XfsmUnitMapStep) - 0.5 ;
  }
  else
  {   
    X = ((float)(X) / XfsmUnitMapStep) + 0.5 ;
  }

  if ( Y < 0 )
  {
    Y = ((float)(Y) / XfsmUnitMapStep) - 0.5 ;
  }
  else
  {   
    Y = ((float)(Y) / XfsmUnitMapStep) + 0.5 ;
  }

  switch ( MouseEvent )
  {
    case XFSM_B1UP   : 
    case XFSM_B2UP   :
    case XFSM_B3UP   :

     XfsmZoomCenter( X, Y );

     break;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackMapResize                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackMapResize( MyWidget, ClientData, CallData )

     Widget                       MyWidget;
     XtPointer                    ClientData;
     XmDrawingAreaCallbackStruct *CallData;
{
  if ( ! XfsmFirstExpose )
  {
    autbegin();

    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR  );
    XfsmResizeMapWindow();
    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

    autend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackMapExpose                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackMapExpose( MyWidget, ClientData, CallData )

     Widget                       MyWidget;
     XtPointer                    ClientData;
     XmDrawingAreaCallbackStruct *CallData;
{
  XExposeEvent *ExposeEvent;

  autbegin();

  ExposeEvent = (XExposeEvent *)CallData->event;

  if ( XfsmFirstExpose )
  {
    XfsmFirstExpose = XFSM_FALSE;

    XfsmInitializeMapWindow();
  }

  XfsmRefreshMapWindow();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Initialize Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XfsmsnitializeMapWindow                |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeMapWindow()

{
  autbegin();

  XtVaGetValues( XfsmMapWindow,
                 XmNwidth , &XfsmMapDx,
                 XmNheight, &XfsmMapDy,
                 NULL
               );
 
  XfsmInitializeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Graphic Window Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        XfsmRefreshMapWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XfsmRefreshMapWindow()
{
  autbegin();

  XfsmClearMapWindow();
  XfsmDisplayUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmClearMapWindow                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmClearMapWindow()

{
  XFillRectangle( XtDisplay( XfsmMapWindow ),
                  XtWindow( XfsmMapWindow ),
                  XfsmBackgroundGC,
                  0, 0, XfsmMapDx, XfsmMapDy );
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmResizeMapWindow                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmResizeMapWindow()

{
  autbegin();

  XtVaGetValues( XfsmMapWindow,
                 XmNwidth,  &XfsmMapDx,
                 XmNheight, &XfsmMapDy,
                 NULL
               );
 
  XfsmComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmInitializeMapEvent                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeMapEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XfsmMapWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XfsmMapEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackMapEvent";
  NewActions.proc   =  CallbackMapEvent;

  XtAppAddActions( XfsmApplication, &NewActions, 1 );

  XtAddCallback( XfsmMapWindow, 
                 XmNresizeCallback, 
                 CallbackMapResize, NULL );

  XtAddCallback( XfsmMapWindow, 
                 XmNexposeCallback, 
                 CallbackMapExpose, NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Unit Map Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   XfsmInitializeUnitMap                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeUnitMap()

{
  autbegin();

  if ( ! XfsmComputeBound() )
  {
    XfsmBoundMapX  = XFSM_DEFAULT_MAP_X;
    XfsmBoundMapY  = XFSM_DEFAULT_MAP_Y;
    XfsmBoundMapDx = XFSM_DEFAULT_MAP_DX;
    XfsmBoundMapDy = XFSM_DEFAULT_MAP_DY;
  }
  else
  {
    XfsmBoundMapX  = XfsmBoundXmin / XFSM_UNIT;
    XfsmBoundMapY  = XfsmBoundYmin / XFSM_UNIT;
    XfsmBoundMapDx = XfsmBoundXmax / XFSM_UNIT;
    XfsmBoundMapDy = XfsmBoundYmax / XFSM_UNIT;
    XfsmBoundMapDx = XfsmBoundMapDx - XfsmBoundMapX;
    XfsmBoundMapDy = XfsmBoundMapDy - XfsmBoundMapY;
  }

  XfsmUnitMapX  = XfsmBoundMapX  - 2;
  XfsmUnitMapY  = XfsmBoundMapY  - 2;
  XfsmUnitMapDx = XfsmBoundMapDx + 4;
  XfsmUnitMapDy = XfsmBoundMapDy + 4;

  XfsmComputeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmComputeUnitMap                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmComputeUnitMap()

{
  long  Delta;
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XfsmMapDx) / (float)(XfsmUnitMapDx);
  StepY = (float)(XfsmMapDy) / (float)(XfsmUnitMapDy);

  if ( StepX < StepY )
  {
    XfsmUnitMapStep = StepX;
    Delta              = XfsmUnitMapDy; 
    XfsmUnitMapDy   = 1 + ( XfsmMapDy / StepX );
    Delta              = ( XfsmUnitMapDy - Delta ) >> 1;
    XfsmUnitMapY    = XfsmUnitMapY - Delta;
  }
  else
  {
    XfsmUnitMapStep = StepY;
    Delta              = XfsmUnitMapDx; 
    XfsmUnitMapDx   = 1 + ( XfsmMapDx / StepY );
    Delta              = ( XfsmUnitMapDx - Delta ) >> 1;
    XfsmUnitMapX    = XfsmUnitMapX - Delta;
  }

  XfsmPixelMapX = (float)(XfsmUnitMapX) * XfsmUnitMapStep;
  XfsmPixelMapY = (float)(XfsmUnitMapY) * XfsmUnitMapStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmDisplayUnitMap                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayUnitMap()
{
  long X1;
  long Y1;
  long X2;
  long Y2;

  autbegin();

  X1 = (float)( XfsmBoundMapX                   ) * XfsmUnitMapStep;
  Y1 = (float)( XfsmBoundMapY                   ) * XfsmUnitMapStep;
  X2 = (float)( XfsmBoundMapX + XfsmBoundMapDx ) * XfsmUnitMapStep;
  Y2 = (float)( XfsmBoundMapY + XfsmBoundMapDy ) * XfsmUnitMapStep;

  X1 = X1 - XfsmPixelMapX;
  X2 = X2 - XfsmPixelMapX;
  Y1 = Y1 - XfsmPixelMapY;
  Y2 = Y2 - XfsmPixelMapY;

  XDrawRectangle( XtDisplay( XfsmMapWindow ),
                  XtWindow( XfsmMapWindow ),
                  XfsmLargeTextGC,
                  X1, XfsmMapDy - Y2,
                  X2 - X1, Y2 - Y1 );

  X1 = XfsmUnitGridX + ( XfsmUnitGridDx >> 1 );
  Y1 = XfsmUnitGridY + ( XfsmUnitGridDy >> 1 );
  X1 = (float)( X1 ) * XfsmUnitMapStep;
  Y1 = (float)( Y1 ) * XfsmUnitMapStep;

  X1 = X1 - XfsmPixelMapX;
  Y1 = Y1 - XfsmPixelMapY;
  Y1 = XfsmMapDy - Y1;

  if ( ( X1 > 0          ) && 
       ( Y1 > 0          ) &&
       ( X1 < XfsmMapDx ) &&
       ( Y1 < XfsmMapDy ) )
  {
    XDrawLine( XtDisplay( XfsmMapWindow ),
               XtWindow( XfsmMapWindow ),
               XfsmLargeTextGC,
               X1 - 2, Y1,
               X1 + 2, Y1  );

    XDrawLine( XtDisplay( XfsmMapWindow ),
               XtWindow( XfsmMapWindow ),
               XfsmLargeTextGC,
               X1, Y1 - 2,
               X1, Y1 + 2  );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmBuildPanelMap                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmBuildPanelMap()
{
  Arg Args[3];

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , XfsmViewMapPanel.TITLE );

  XfsmViewMapPanel.PANEL = 

    XmCreateFormDialog( XfsmMainWindow, "XfsmViewMapPanel", Args, 3);

  XtAddCallback( XfsmViewMapPanel.PANEL, XmNdestroyCallback,
                 XfsmDestroyDialogCallback, NULL  );

  XfsmViewMapPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XfsmViewMapPanelForm",
                             xmFormWidgetClass,
                             XfsmViewMapPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );
 
  XfsmViewMapPanel.FRAME = 

    XtVaCreateManagedWidget( "XfsmViewMapFrame",
                             xmFrameWidgetClass,
                             XfsmViewMapPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XfsmViewMapPanel.FORM = 

    XtVaCreateManagedWidget( "XfsmViewMapForm",
                             xmFormWidgetClass,
                             XfsmViewMapPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );

  XfsmMapButtonCompute = 

    XtVaCreateManagedWidget( "Compute",
                             xmPushButtonWidgetClass,
                             XfsmViewMapPanel.FORM, 
                             XmNleftAttachment   , XmATTACH_POSITION,
                             XmNleftPosition     , 1,
                             XmNrightAttachment  , XmATTACH_POSITION,
                             XmNrightPosition    , 44,
                             XmNtopAttachment    , XmATTACH_POSITION,
                             XmNtopPosition      , 89,
                             XmNbottomAttachment , XmATTACH_POSITION,
                             XmNbottomPosition   , 99,
                             NULL
                           );
  XfsmMapButtonClose = 

    XtVaCreateManagedWidget( "Close",
                             xmPushButtonWidgetClass,
                             XfsmViewMapPanel.FORM, 
                             XmNleftAttachment   , XmATTACH_POSITION,
                             XmNleftPosition     , 45,
                             XmNrightAttachment  , XmATTACH_POSITION,
                             XmNrightPosition    , 99,
                             XmNtopAttachment    , XmATTACH_POSITION,
                             XmNtopPosition      , 89,
                             XmNbottomAttachment , XmATTACH_POSITION,
                             XmNbottomPosition   , 99,
                             NULL
                           );

  XfsmMapWindow = 

    XtVaCreateManagedWidget( "XfsmMapWindow",
                             xmDrawingAreaWidgetClass,
                             XfsmViewMapPanel.FORM, 
                             XmNleftAttachment   , XmATTACH_POSITION,
                             XmNleftPosition     , 1,
                             XmNrightAttachment  , XmATTACH_POSITION,
                             XmNrightPosition    , 99,
                             XmNtopAttachment    , XmATTACH_POSITION,
                             XmNtopPosition      , 1,
                             XmNbottomAttachment , XmATTACH_POSITION,
                             XmNbottomPosition   , 88,
                             XmNbackground       , 1,
                             XmNforeground       , 0,
                             NULL
                           );

  XtAddCallback( XfsmMapButtonCompute, 
                 XmNactivateCallback,
                 CallbackMapCompute, NULL );

  XtAddCallback( XfsmMapButtonClose, 
                 XmNactivateCallback,
                 CallbackCloseMap, NULL );

  XtVaSetValues( XfsmViewMapPanel.PANEL,
                 XmNheight, XfsmViewMapPanel.HEIGHT,
                 XmNwidth , XfsmViewMapPanel.WIDTH,
                 XmNx     , XfsmViewMapPanel.X,
                 XmNy     , XfsmViewMapPanel.Y,
                 NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmEnterMapPanel                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmEnterMapPanel()

{
  autbegin();

  XfsmFirstEnterMap = XFSM_FALSE;

  XfsmEnterPanel( &XfsmViewMapPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmComputeAndDisplayMap               |
|                                                             |
\------------------------------------------------------------*/

void XfsmComputeAndDisplayMap()

{

  if ( ! XfsmFirstEnterMap )
  {
    autbegin();

    XfsmInitializeUnitMap();
    XfsmClearMapWindow();
    XfsmDisplayUnitMap();

    autend();
  }
}
