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
| Tool    :                   XGRA                            |
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
# include "XSB.h"
# include "XGR.h"
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

   XgraPanelItem XgraViewMapPanel =

         {
           "Map",
           1,
           0,
           XGRA_VIEW_MAP_X,
           XGRA_VIEW_MAP_Y,
           190,
           200,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XgraPanelButtonItem *)NULL 
         };

   int XgraViewMapDefaultValues[ 5 ] =

         {
           XGRA_VIEW_MAP_X, 
           XGRA_VIEW_MAP_Y,
           190, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Graphic Map Window                     |
|                                                             |
\------------------------------------------------------------*/

  Widget     XgraMapWindow;
  Widget     XgraMapButtonCompute;
  Widget     XgraMapButtonClose;

  Dimension  XgraMapDx    = 0;
  Dimension  XgraMapDy    = 0;

  static char XgraFirstEnterMap = XGRA_TRUE;
  static char XgraFirstExpose   = XGRA_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                           Unit Map                          |
|                                                             |
\------------------------------------------------------------*/

  float      XgraUnitMapStep;

  long       XgraUnitMapX;
  long       XgraUnitMapY;
  long       XgraUnitMapDx;
  long       XgraUnitMapDy;

  long       XgraBoundMapX; 
  long       XgraBoundMapY; 
  long       XgraBoundMapDx; 
  long       XgraBoundMapDy; 

  long       XgraPixelMapX;
  long       XgraPixelMapY;

/*------------------------------------------------------------\
|                                                             |
|                            Expose                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Arclation                        |
|                                                             |
\------------------------------------------------------------*/

  static String XgraMapEventArclation =

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

  XgraExitPanel( &XgraViewMapPanel );

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

  XgraComputeAndDisplayMap();

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
  Y = XgraMapDy - Event->y;

  X = X + XgraPixelMapX;
  Y = Y + XgraPixelMapY;

  if ( X < 0 )
  {
    X = ((float)(X) / XgraUnitMapStep) - 0.5 ;
  }
  else
  {   
    X = ((float)(X) / XgraUnitMapStep) + 0.5 ;
  }

  if ( Y < 0 )
  {
    Y = ((float)(Y) / XgraUnitMapStep) - 0.5 ;
  }
  else
  {   
    Y = ((float)(Y) / XgraUnitMapStep) + 0.5 ;
  }

  switch ( MouseEvent )
  {
    case XGRA_B1UP   : 
    case XGRA_B2UP   :
    case XGRA_B3UP   :

     XgraZoomCenter( X, Y );

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
  if ( ! XgraFirstExpose )
  {
    autbegin();

    XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR  );
    XgraResizeMapWindow();
    XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

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

  if ( XgraFirstExpose )
  {
    XgraFirstExpose = XGRA_FALSE;

    XgraInitializeMapWindow();
  }

  XgraRefreshMapWindow();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Initialize Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XgrasnitializeMapWindow                |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeMapWindow()

{
  autbegin();

  XtVaGetValues( XgraMapWindow,
                 XmNwidth , &XgraMapDx,
                 XmNheight, &XgraMapDy,
                 NULL
               );
 
  XgraInitializeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Graphic Window Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        XgraRefreshMapWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XgraRefreshMapWindow()
{
  autbegin();

  XgraClearMapWindow();
  XgraDisplayUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraClearMapWindow                   |
|                                                             |
\------------------------------------------------------------*/

void XgraClearMapWindow()

{
  XFillRectangle( XtDisplay( XgraMapWindow ),
                  XtWindow( XgraMapWindow ),
                  XgraBackgroundGC,
                  0, 0, XgraMapDx, XgraMapDy );
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraResizeMapWindow                  |
|                                                             |
\------------------------------------------------------------*/

void XgraResizeMapWindow()

{
  autbegin();

  XtVaGetValues( XgraMapWindow,
                 XmNwidth,  &XgraMapDx,
                 XmNheight, &XgraMapDy,
                 NULL
               );
 
  XgraComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraInitializeMapEvent                   |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeMapEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XgraMapWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XgraMapEventArclation ),
                 NULL
               );

  NewActions.string = "CallbackMapEvent";
  NewActions.proc   =  CallbackMapEvent;

  XtAppAddActions( XgraApplication, &NewActions, 1 );

  XtAddCallback( XgraMapWindow, 
                 XmNresizeCallback, 
                 CallbackMapResize, NULL );

  XtAddCallback( XgraMapWindow, 
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
|                   XgraInitializeUnitMap                     |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeUnitMap()

{
  autbegin();

  if ( ! XgraComputeBound() )
  {
    XgraBoundMapX  = XGRA_DEFAULT_MAP_X;
    XgraBoundMapY  = XGRA_DEFAULT_MAP_Y;
    XgraBoundMapDx = XGRA_DEFAULT_MAP_DX;
    XgraBoundMapDy = XGRA_DEFAULT_MAP_DY;
  }
  else
  {
    XgraBoundMapX  = XgraBoundXmin / XGRA_UNIT;
    XgraBoundMapY  = XgraBoundYmin / XGRA_UNIT;
    XgraBoundMapDx = XgraBoundXmax / XGRA_UNIT;
    XgraBoundMapDy = XgraBoundYmax / XGRA_UNIT;
    XgraBoundMapDx = XgraBoundMapDx - XgraBoundMapX;
    XgraBoundMapDy = XgraBoundMapDy - XgraBoundMapY;
  }

  XgraUnitMapX  = XgraBoundMapX  - 2;
  XgraUnitMapY  = XgraBoundMapY  - 2;
  XgraUnitMapDx = XgraBoundMapDx + 4;
  XgraUnitMapDy = XgraBoundMapDy + 4;

  XgraComputeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraComputeUnitMap                       |
|                                                             |
\------------------------------------------------------------*/

void XgraComputeUnitMap()

{
  long  Delta;
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XgraMapDx) / (float)(XgraUnitMapDx);
  StepY = (float)(XgraMapDy) / (float)(XgraUnitMapDy);

  if ( StepX < StepY )
  {
    XgraUnitMapStep = StepX;
    Delta              = XgraUnitMapDy; 
    XgraUnitMapDy   = 1 + ( XgraMapDy / StepX );
    Delta              = ( XgraUnitMapDy - Delta ) >> 1;
    XgraUnitMapY    = XgraUnitMapY - Delta;
  }
  else
  {
    XgraUnitMapStep = StepY;
    Delta              = XgraUnitMapDx; 
    XgraUnitMapDx   = 1 + ( XgraMapDx / StepY );
    Delta              = ( XgraUnitMapDx - Delta ) >> 1;
    XgraUnitMapX    = XgraUnitMapX - Delta;
  }

  XgraPixelMapX = (float)(XgraUnitMapX) * XgraUnitMapStep;
  XgraPixelMapY = (float)(XgraUnitMapY) * XgraUnitMapStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraDisplayUnitMap                  |
|                                                             |
\------------------------------------------------------------*/

void XgraDisplayUnitMap()
{
  long X1;
  long Y1;
  long X2;
  long Y2;

  autbegin();

  X1 = (float)( XgraBoundMapX                   ) * XgraUnitMapStep;
  Y1 = (float)( XgraBoundMapY                   ) * XgraUnitMapStep;
  X2 = (float)( XgraBoundMapX + XgraBoundMapDx ) * XgraUnitMapStep;
  Y2 = (float)( XgraBoundMapY + XgraBoundMapDy ) * XgraUnitMapStep;

  X1 = X1 - XgraPixelMapX;
  X2 = X2 - XgraPixelMapX;
  Y1 = Y1 - XgraPixelMapY;
  Y2 = Y2 - XgraPixelMapY;

  XDrawRectangle( XtDisplay( XgraMapWindow ),
                  XtWindow( XgraMapWindow ),
                  XgraLargeTextGC,
                  X1, XgraMapDy - Y2,
                  X2 - X1, Y2 - Y1 );

  X1 = XgraUnitGridX + ( XgraUnitGridDx >> 1 );
  Y1 = XgraUnitGridY + ( XgraUnitGridDy >> 1 );
  X1 = (float)( X1 ) * XgraUnitMapStep;
  Y1 = (float)( Y1 ) * XgraUnitMapStep;

  X1 = X1 - XgraPixelMapX;
  Y1 = Y1 - XgraPixelMapY;
  Y1 = XgraMapDy - Y1;

  if ( ( X1 > 0          ) && 
       ( Y1 > 0          ) &&
       ( X1 < XgraMapDx ) &&
       ( Y1 < XgraMapDy ) )
  {
    XDrawLine( XtDisplay( XgraMapWindow ),
               XtWindow( XgraMapWindow ),
               XgraLargeTextGC,
               X1 - 2, Y1,
               X1 + 2, Y1  );

    XDrawLine( XtDisplay( XgraMapWindow ),
               XtWindow( XgraMapWindow ),
               XgraLargeTextGC,
               X1, Y1 - 2,
               X1, Y1 + 2  );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraBuildPanelMap                      |
|                                                             |
\------------------------------------------------------------*/

void XgraBuildPanelMap()
{
  Arg Args[3];

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse,          XmDESTROY      );
  XtSetArg( Args[2], XmNtitle         , XgraViewMapPanel.TITLE );

  XgraViewMapPanel.PANEL = 

    XmCreateFormDialog( XgraMainWindow, "XgraViewMapPanel", Args, 3);

  XtAddCallback( XgraViewMapPanel.PANEL, XmNdestroyCallback,
                 XgraDestroyDialogCallback, NULL  );

  XgraViewMapPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XgraViewMapPanelForm",
                             xmFormWidgetClass,
                             XgraViewMapPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );
 
  XgraViewMapPanel.FRAME = 

    XtVaCreateManagedWidget( "XgraViewMapFrame",
                             xmFrameWidgetClass,
                             XgraViewMapPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XgraViewMapPanel.FORM = 

    XtVaCreateManagedWidget( "XgraViewMapForm",
                             xmFormWidgetClass,
                             XgraViewMapPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );

  XgraMapButtonCompute = 

    XtVaCreateManagedWidget( "Compute",
                             xmPushButtonWidgetClass,
                             XgraViewMapPanel.FORM, 
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
  XgraMapButtonClose = 

    XtVaCreateManagedWidget( "Close",
                             xmPushButtonWidgetClass,
                             XgraViewMapPanel.FORM, 
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

  XgraMapWindow = 

    XtVaCreateManagedWidget( "XgraMapWindow",
                             xmDrawingAreaWidgetClass,
                             XgraViewMapPanel.FORM, 
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

  XtAddCallback( XgraMapButtonCompute, 
                 XmNactivateCallback,
                 CallbackMapCompute, NULL );

  XtAddCallback( XgraMapButtonClose, 
                 XmNactivateCallback,
                 CallbackCloseMap, NULL );

  XtVaSetValues( XgraViewMapPanel.PANEL,
                 XmNheight, XgraViewMapPanel.HEIGHT,
                 XmNwidth , XgraViewMapPanel.WIDTH,
                 XmNx     , XgraViewMapPanel.X,
                 XmNy     , XgraViewMapPanel.Y,
                 NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraEnterMapPanel                      |
|                                                             |
\------------------------------------------------------------*/

void XgraEnterMapPanel()

{
  autbegin();

  XgraFirstEnterMap = XGRA_FALSE;

  XgraEnterPanel( &XgraViewMapPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraComputeAndDisplayMap               |
|                                                             |
\------------------------------------------------------------*/

void XgraComputeAndDisplayMap()

{

  if ( ! XgraFirstEnterMap )
  {
    autbegin();

    XgraInitializeUnitMap();
    XgraClearMapWindow();
    XgraDisplayUnitMap();

    autend();
  }
}
