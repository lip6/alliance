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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                   Map.c                           |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/PushB.h>
# include <Xm/DrawingA.h>
 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
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

   XschPanelItem XschViewMapPanel =

         {
           "Map",
           1,
           0,
           XSCH_VIEW_MAP_X,
           XSCH_VIEW_MAP_Y,
           190,
           200,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XschPanelButtonItem *)NULL 
         };

   int XschViewMapDefaultValues[ 5 ] =

         {
           XSCH_VIEW_MAP_X, 
           XSCH_VIEW_MAP_Y,
           190, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Graphic Map Window                     |
|                                                             |
\------------------------------------------------------------*/

  Widget     XschMapWindow;
  Widget     XschMapButtonCompute;
  Widget     XschMapButtonClose;

  Dimension  XschMapDx    = 0;
  Dimension  XschMapDy    = 0;

  static char XschFirstEnterMap = XSCH_TRUE;
  static char XschFirstExpose   = XSCH_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                           Unit Map                          |
|                                                             |
\------------------------------------------------------------*/

  float      XschUnitMapStep;

  long       XschUnitMapX;
  long       XschUnitMapY;
  long       XschUnitMapDx;
  long       XschUnitMapDy;

  long       XschBoundMapX; 
  long       XschBoundMapY; 
  long       XschBoundMapDx; 
  long       XschBoundMapDy; 

  long       XschPixelMapX;
  long       XschPixelMapY;

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

  static String XschMapEventTranslation =

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

  XschExitPanel( &XschViewMapPanel );

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

  XschComputeAndDisplayMap();

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
  Y = XschMapDy - Event->y;

  X = X + XschPixelMapX;
  Y = Y + XschPixelMapY;

  if ( X < 0 )
  {
    X = ((float)(X) / XschUnitMapStep) - 0.5 ;
  }
  else
  {   
    X = ((float)(X) / XschUnitMapStep) + 0.5 ;
  }

  if ( Y < 0 )
  {
    Y = ((float)(Y) / XschUnitMapStep) - 0.5 ;
  }
  else
  {   
    Y = ((float)(Y) / XschUnitMapStep) + 0.5 ;
  }

  switch ( MouseEvent )
  {
    case XSCH_B1UP   : 
    case XSCH_B2UP   :
    case XSCH_B3UP   :

     XschZoomCenter( X, Y );

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
  if ( ! XschFirstExpose )
  {
    autbegin();

    XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR  );
    XschResizeMapWindow();
    XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

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

  if ( XschFirstExpose )
  {
    XschFirstExpose = XSCH_FALSE;

    XschInitializeMapWindow();
  }

  XschRefreshMapWindow();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Initialize Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XschsnitializeMapWindow                |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeMapWindow()

{
  autbegin();

  XtVaGetValues( XschMapWindow,
                 XmNwidth , &XschMapDx,
                 XmNheight, &XschMapDy,
                 NULL
               );
 
  XschInitializeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Graphic Window Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        XschRefreshMapWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XschRefreshMapWindow()
{
  autbegin();

  XschClearMapWindow();
  XschDisplayUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschClearMapWindow                   |
|                                                             |
\------------------------------------------------------------*/

void XschClearMapWindow()

{
  XFillRectangle( XtDisplay( XschMapWindow ),
                  XtWindow( XschMapWindow ),
                  XschBackgroundGC,
                  0, 0, XschMapDx, XschMapDy );
}

/*------------------------------------------------------------\
|                                                             |
|                        XschResizeMapWindow                  |
|                                                             |
\------------------------------------------------------------*/

void XschResizeMapWindow()

{
  autbegin();

  XtVaGetValues( XschMapWindow,
                 XmNwidth,  &XschMapDx,
                 XmNheight, &XschMapDy,
                 NULL
               );
 
  XschComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschInitializeMapEvent                   |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeMapEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XschMapWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XschMapEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackMapEvent";
  NewActions.proc   =  CallbackMapEvent;

  XtAppAddActions( XschApplication, &NewActions, 1 );

  XtAddCallback( XschMapWindow, 
                 XmNresizeCallback, 
                 CallbackMapResize, NULL );

  XtAddCallback( XschMapWindow, 
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
|                   XschInitializeUnitMap                     |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeUnitMap()

{
  autbegin();

  if ( ! XschComputeBound() )
  {
    XschBoundMapX  = XSCH_DEFAULT_MAP_X;
    XschBoundMapY  = XSCH_DEFAULT_MAP_Y;
    XschBoundMapDx = XSCH_DEFAULT_MAP_DX;
    XschBoundMapDy = XSCH_DEFAULT_MAP_DY;
  }
  else
  {
    XschBoundMapX  = XschBoundXmin / XSCH_UNIT;
    XschBoundMapY  = XschBoundYmin / XSCH_UNIT;
    XschBoundMapDx = XschBoundXmax / XSCH_UNIT;
    XschBoundMapDy = XschBoundYmax / XSCH_UNIT;
    XschBoundMapDx = XschBoundMapDx - XschBoundMapX;
    XschBoundMapDy = XschBoundMapDy - XschBoundMapY;
  }

  XschUnitMapX  = XschBoundMapX  - 2;
  XschUnitMapY  = XschBoundMapY  - 2;
  XschUnitMapDx = XschBoundMapDx + 4;
  XschUnitMapDy = XschBoundMapDy + 4;

  XschComputeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschComputeUnitMap                       |
|                                                             |
\------------------------------------------------------------*/

void XschComputeUnitMap()

{
  long  Delta;
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XschMapDx) / (float)(XschUnitMapDx);
  StepY = (float)(XschMapDy) / (float)(XschUnitMapDy);

  if ( StepX < StepY )
  {
    XschUnitMapStep = StepX;
    Delta              = XschUnitMapDy; 
    XschUnitMapDy   = 1 + ( XschMapDy / StepX );
    Delta              = ( XschUnitMapDy - Delta ) >> 1;
    XschUnitMapY    = XschUnitMapY - Delta;
  }
  else
  {
    XschUnitMapStep = StepY;
    Delta              = XschUnitMapDx; 
    XschUnitMapDx   = 1 + ( XschMapDx / StepY );
    Delta              = ( XschUnitMapDx - Delta ) >> 1;
    XschUnitMapX    = XschUnitMapX - Delta;
  }

  XschPixelMapX = (float)(XschUnitMapX) * XschUnitMapStep;
  XschPixelMapY = (float)(XschUnitMapY) * XschUnitMapStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschDisplayUnitMap                  |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayUnitMap()
{
  long X1;
  long Y1;
  long X2;
  long Y2;

  autbegin();

  X1 = (float)( XschBoundMapX                   ) * XschUnitMapStep;
  Y1 = (float)( XschBoundMapY                   ) * XschUnitMapStep;
  X2 = (float)( XschBoundMapX + XschBoundMapDx ) * XschUnitMapStep;
  Y2 = (float)( XschBoundMapY + XschBoundMapDy ) * XschUnitMapStep;

  X1 = X1 - XschPixelMapX;
  X2 = X2 - XschPixelMapX;
  Y1 = Y1 - XschPixelMapY;
  Y2 = Y2 - XschPixelMapY;

  XDrawRectangle( XtDisplay( XschMapWindow ),
                  XtWindow( XschMapWindow ),
                  XschLargeTextGC,
                  X1, XschMapDy - Y2,
                  X2 - X1, Y2 - Y1 );

  X1 = XschUnitGridX + ( XschUnitGridDx >> 1 );
  Y1 = XschUnitGridY + ( XschUnitGridDy >> 1 );
  X1 = (float)( X1 ) * XschUnitMapStep;
  Y1 = (float)( Y1 ) * XschUnitMapStep;

  X1 = X1 - XschPixelMapX;
  Y1 = Y1 - XschPixelMapY;
  Y1 = XschMapDy - Y1;

  if ( ( X1 > 0          ) && 
       ( Y1 > 0          ) &&
       ( X1 < XschMapDx ) &&
       ( Y1 < XschMapDy ) )
  {
    XDrawLine( XtDisplay( XschMapWindow ),
               XtWindow( XschMapWindow ),
               XschLargeTextGC,
               X1 - 2, Y1,
               X1 + 2, Y1  );

    XDrawLine( XtDisplay( XschMapWindow ),
               XtWindow( XschMapWindow ),
               XschLargeTextGC,
               X1, Y1 - 2,
               X1, Y1 + 2  );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschBuildPanelMap                      |
|                                                             |
\------------------------------------------------------------*/

void XschBuildPanelMap()
{
  Arg Args[3];

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , XschViewMapPanel.TITLE );

  XschViewMapPanel.PANEL = 

    XmCreateFormDialog( XschMainWindow, "XschViewMapPanel", Args, 3);

  XtAddCallback( XschViewMapPanel.PANEL, XmNdestroyCallback,
                 XschDestroyDialogCallback, NULL  );

  XschViewMapPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XschViewMapPanelForm",
                             xmFormWidgetClass,
                             XschViewMapPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );
 
  XschViewMapPanel.FRAME = 

    XtVaCreateManagedWidget( "XschViewMapFrame",
                             xmFrameWidgetClass,
                             XschViewMapPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XschViewMapPanel.FORM = 

    XtVaCreateManagedWidget( "XschViewMapForm",
                             xmFormWidgetClass,
                             XschViewMapPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );

  XschMapButtonCompute = 

    XtVaCreateManagedWidget( "Compute",
                             xmPushButtonWidgetClass,
                             XschViewMapPanel.FORM, 
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
  XschMapButtonClose = 

    XtVaCreateManagedWidget( "Close",
                             xmPushButtonWidgetClass,
                             XschViewMapPanel.FORM, 
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

  XschMapWindow = 

    XtVaCreateManagedWidget( "XschMapWindow",
                             xmDrawingAreaWidgetClass,
                             XschViewMapPanel.FORM, 
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

  XtAddCallback( XschMapButtonCompute, 
                 XmNactivateCallback,
                 CallbackMapCompute, NULL );

  XtAddCallback( XschMapButtonClose, 
                 XmNactivateCallback,
                 CallbackCloseMap, NULL );

  XtVaSetValues( XschViewMapPanel.PANEL,
                 XmNheight, XschViewMapPanel.HEIGHT,
                 XmNwidth , XschViewMapPanel.WIDTH,
                 XmNx     , XschViewMapPanel.X,
                 XmNy     , XschViewMapPanel.Y,
                 NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschEnterMapPanel                      |
|                                                             |
\------------------------------------------------------------*/

void XschEnterMapPanel()

{
  autbegin();

  XschFirstEnterMap = XSCH_FALSE;

  XschEnterPanel( &XschViewMapPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschComputeAndDisplayMap               |
|                                                             |
\------------------------------------------------------------*/

void XschComputeAndDisplayMap()

{

  if ( ! XschFirstEnterMap )
  {
    autbegin();

    XschInitializeUnitMap();
    XschClearMapWindow();
    XschDisplayUnitMap();

    autend();
  }
}
