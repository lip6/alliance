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
| Tool    :                   XVPN                            |
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
# include "vex.h"
# include "vpn.h"
# include "XSB.h"
# include "XVP.h"
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

   XvpnPanelItem XvpnViewMapPanel =

         {
           "Map",
           1,
           0,
           XVPN_VIEW_MAP_X,
           XVPN_VIEW_MAP_Y,
           190,
           200,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XvpnPanelButtonItem *)NULL 
         };

   int XvpnViewMapDefaultValues[ 5 ] =

         {
           XVPN_VIEW_MAP_X, 
           XVPN_VIEW_MAP_Y,
           190, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Graphic Map Window                     |
|                                                             |
\------------------------------------------------------------*/

  Widget     XvpnMapWindow;
  Widget     XvpnMapButtonCompute;
  Widget     XvpnMapButtonClose;

  Dimension  XvpnMapDx    = 0;
  Dimension  XvpnMapDy    = 0;

  static char XvpnFirstEnterMap = XVPN_TRUE;
  static char XvpnFirstExpose   = XVPN_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                           Unit Map                          |
|                                                             |
\------------------------------------------------------------*/

  float      XvpnUnitMapStep;

  long       XvpnUnitMapX;
  long       XvpnUnitMapY;
  long       XvpnUnitMapDx;
  long       XvpnUnitMapDy;

  long       XvpnBoundMapX; 
  long       XvpnBoundMapY; 
  long       XvpnBoundMapDx; 
  long       XvpnBoundMapDy; 

  long       XvpnPixelMapX;
  long       XvpnPixelMapY;

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

  static String XvpnMapEventTranslation =

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

  XvpnExitPanel( &XvpnViewMapPanel );

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

  XvpnComputeAndDisplayMap();

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
  Y = XvpnMapDy - Event->y;

  X = X + XvpnPixelMapX;
  Y = Y + XvpnPixelMapY;

  if ( X < 0 )
  {
    X = ((float)(X) / XvpnUnitMapStep) - 0.5 ;
  }
  else
  {   
    X = ((float)(X) / XvpnUnitMapStep) + 0.5 ;
  }

  if ( Y < 0 )
  {
    Y = ((float)(Y) / XvpnUnitMapStep) - 0.5 ;
  }
  else
  {   
    Y = ((float)(Y) / XvpnUnitMapStep) + 0.5 ;
  }

  switch ( MouseEvent )
  {
    case XVPN_B1UP   : 
    case XVPN_B2UP   :
    case XVPN_B3UP   :

     XvpnZoomCenter( X, Y );

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
  if ( ! XvpnFirstExpose )
  {
    autbegin();

    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR  );
    XvpnResizeMapWindow();
    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

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

  if ( XvpnFirstExpose )
  {
    XvpnFirstExpose = XVPN_FALSE;

    XvpnInitializeMapWindow();
  }

  XvpnRefreshMapWindow();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Initialize Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnsnitializeMapWindow                |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeMapWindow()

{
  autbegin();

  XtVaGetValues( XvpnMapWindow,
                 XmNwidth , &XvpnMapDx,
                 XmNheight, &XvpnMapDy,
                 NULL
               );
 
  XvpnInitializeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Graphic Window Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        XvpnRefreshMapWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XvpnRefreshMapWindow()
{
  autbegin();

  XvpnClearMapWindow();
  XvpnDisplayUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnClearMapWindow                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnClearMapWindow()

{
  XFillRectangle( XtDisplay( XvpnMapWindow ),
                  XtWindow( XvpnMapWindow ),
                  XvpnBackgroundGC,
                  0, 0, XvpnMapDx, XvpnMapDy );
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnResizeMapWindow                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnResizeMapWindow()

{
  autbegin();

  XtVaGetValues( XvpnMapWindow,
                 XmNwidth,  &XvpnMapDx,
                 XmNheight, &XvpnMapDy,
                 NULL
               );
 
  XvpnComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnInitializeMapEvent                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeMapEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XvpnMapWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XvpnMapEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackMapEvent";
  NewActions.proc   =  CallbackMapEvent;

  XtAppAddActions( XvpnApplication, &NewActions, 1 );

  XtAddCallback( XvpnMapWindow, 
                 XmNresizeCallback, 
                 CallbackMapResize, NULL );

  XtAddCallback( XvpnMapWindow, 
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
|                   XvpnInitializeUnitMap                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeUnitMap()

{
  autbegin();

  if ( ! XvpnComputeBound() )
  {
    XvpnBoundMapX  = XVPN_DEFAULT_MAP_X;
    XvpnBoundMapY  = XVPN_DEFAULT_MAP_Y;
    XvpnBoundMapDx = XVPN_DEFAULT_MAP_DX;
    XvpnBoundMapDy = XVPN_DEFAULT_MAP_DY;
  }
  else
  {
    XvpnBoundMapX  = XvpnBoundXmin / XVPN_UNIT;
    XvpnBoundMapY  = XvpnBoundYmin / XVPN_UNIT;
    XvpnBoundMapDx = XvpnBoundXmax / XVPN_UNIT;
    XvpnBoundMapDy = XvpnBoundYmax / XVPN_UNIT;
    XvpnBoundMapDx = XvpnBoundMapDx - XvpnBoundMapX;
    XvpnBoundMapDy = XvpnBoundMapDy - XvpnBoundMapY;
  }

  XvpnUnitMapX  = XvpnBoundMapX  - 2;
  XvpnUnitMapY  = XvpnBoundMapY  - 2;
  XvpnUnitMapDx = XvpnBoundMapDx + 4;
  XvpnUnitMapDy = XvpnBoundMapDy + 4;

  XvpnComputeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnComputeUnitMap                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnComputeUnitMap()

{
  long  Delta;
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XvpnMapDx) / (float)(XvpnUnitMapDx);
  StepY = (float)(XvpnMapDy) / (float)(XvpnUnitMapDy);

  if ( StepX < StepY )
  {
    XvpnUnitMapStep = StepX;
    Delta              = XvpnUnitMapDy; 
    XvpnUnitMapDy   = 1 + ( XvpnMapDy / StepX );
    Delta              = ( XvpnUnitMapDy - Delta ) >> 1;
    XvpnUnitMapY    = XvpnUnitMapY - Delta;
  }
  else
  {
    XvpnUnitMapStep = StepY;
    Delta              = XvpnUnitMapDx; 
    XvpnUnitMapDx   = 1 + ( XvpnMapDx / StepY );
    Delta              = ( XvpnUnitMapDx - Delta ) >> 1;
    XvpnUnitMapX    = XvpnUnitMapX - Delta;
  }

  XvpnPixelMapX = (float)(XvpnUnitMapX) * XvpnUnitMapStep;
  XvpnPixelMapY = (float)(XvpnUnitMapY) * XvpnUnitMapStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnDisplayUnitMap                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayUnitMap()
{
  long X1;
  long Y1;
  long X2;
  long Y2;

  autbegin();

  X1 = (float)( XvpnBoundMapX                   ) * XvpnUnitMapStep;
  Y1 = (float)( XvpnBoundMapY                   ) * XvpnUnitMapStep;
  X2 = (float)( XvpnBoundMapX + XvpnBoundMapDx ) * XvpnUnitMapStep;
  Y2 = (float)( XvpnBoundMapY + XvpnBoundMapDy ) * XvpnUnitMapStep;

  X1 = X1 - XvpnPixelMapX;
  X2 = X2 - XvpnPixelMapX;
  Y1 = Y1 - XvpnPixelMapY;
  Y2 = Y2 - XvpnPixelMapY;

  XDrawRectangle( XtDisplay( XvpnMapWindow ),
                  XtWindow( XvpnMapWindow ),
                  XvpnLargeTextGC,
                  X1, XvpnMapDy - Y2,
                  X2 - X1, Y2 - Y1 );

  X1 = XvpnUnitGridX + ( XvpnUnitGridDx >> 1 );
  Y1 = XvpnUnitGridY + ( XvpnUnitGridDy >> 1 );
  X1 = (float)( X1 ) * XvpnUnitMapStep;
  Y1 = (float)( Y1 ) * XvpnUnitMapStep;

  X1 = X1 - XvpnPixelMapX;
  Y1 = Y1 - XvpnPixelMapY;
  Y1 = XvpnMapDy - Y1;

  if ( ( X1 > 0          ) && 
       ( Y1 > 0          ) &&
       ( X1 < XvpnMapDx ) &&
       ( Y1 < XvpnMapDy ) )
  {
    XDrawLine( XtDisplay( XvpnMapWindow ),
               XtWindow( XvpnMapWindow ),
               XvpnLargeTextGC,
               X1 - 2, Y1,
               X1 + 2, Y1  );

    XDrawLine( XtDisplay( XvpnMapWindow ),
               XtWindow( XvpnMapWindow ),
               XvpnLargeTextGC,
               X1, Y1 - 2,
               X1, Y1 + 2  );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnBuildPanelMap                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnBuildPanelMap()
{
  Arg Args[3];

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse,          XmDESTROY      );
  XtSetArg( Args[2], XmNtitle         , XvpnViewMapPanel.TITLE );

  XvpnViewMapPanel.PANEL = 

    XmCreateFormDialog( XvpnMainWindow, "XvpnViewMapPanel", Args, 3);

  XtAddCallback( XvpnViewMapPanel.PANEL, XmNdestroyCallback,
                 XvpnDestroyDialogCallback, NULL  );

  XvpnViewMapPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XvpnViewMapPanelForm",
                             xmFormWidgetClass,
                             XvpnViewMapPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );
 
  XvpnViewMapPanel.FRAME = 

    XtVaCreateManagedWidget( "XvpnViewMapFrame",
                             xmFrameWidgetClass,
                             XvpnViewMapPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XvpnViewMapPanel.FORM = 

    XtVaCreateManagedWidget( "XvpnViewMapForm",
                             xmFormWidgetClass,
                             XvpnViewMapPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );

  XvpnMapButtonCompute = 

    XtVaCreateManagedWidget( "Compute",
                             xmPushButtonWidgetClass,
                             XvpnViewMapPanel.FORM, 
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
  XvpnMapButtonClose = 

    XtVaCreateManagedWidget( "Close",
                             xmPushButtonWidgetClass,
                             XvpnViewMapPanel.FORM, 
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

  XvpnMapWindow = 

    XtVaCreateManagedWidget( "XvpnMapWindow",
                             xmDrawingAreaWidgetClass,
                             XvpnViewMapPanel.FORM, 
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

  XtAddCallback( XvpnMapButtonCompute, 
                 XmNactivateCallback,
                 CallbackMapCompute, NULL );

  XtAddCallback( XvpnMapButtonClose, 
                 XmNactivateCallback,
                 CallbackCloseMap, NULL );

  XtVaSetValues( XvpnViewMapPanel.PANEL,
                 XmNheight, XvpnViewMapPanel.HEIGHT,
                 XmNwidth , XvpnViewMapPanel.WIDTH,
                 XmNx     , XvpnViewMapPanel.X,
                 XmNy     , XvpnViewMapPanel.Y,
                 NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnEnterMapPanel                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnEnterMapPanel()

{
  autbegin();

  XvpnFirstEnterMap = XVPN_FALSE;

  XvpnEnterPanel( &XvpnViewMapPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnComputeAndDisplayMap               |
|                                                             |
\------------------------------------------------------------*/

void XvpnComputeAndDisplayMap()

{

  if ( ! XvpnFirstEnterMap )
  {
    autbegin();

    XvpnInitializeUnitMap();
    XvpnClearMapWindow();
    XvpnDisplayUnitMap();

    autend();
  }
}
