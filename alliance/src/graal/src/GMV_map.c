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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Map.c                           |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/PushB.h>
# include <Xm/DrawingA.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GRM.h"
# include "GTB.h"
# include "GMX.h"
# include "GMV.h"

# include "GMV_map.h"

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

   GraalPanelItem GraalViewMapPanel =

         {
           "Map",
           1,
           0,
           GRAAL_VIEW_MAP_X,
           GRAAL_VIEW_MAP_Y,
           190,
           200,
           1,
           1,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           0,
           (GraalPanelButtonItem *)NULL 
         };

   int GraalViewMapDefaultValues[ 5 ] =

         {
           GRAAL_VIEW_MAP_X, 
           GRAAL_VIEW_MAP_Y,
           190, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Graphic Map Window                     |
|                                                             |
\------------------------------------------------------------*/

  Widget     GraalMapWindow;
  Widget     GraalMapButtonCompute;
  Widget     GraalMapButtonClose;

  Dimension  GraalMapDx    = 0;
  Dimension  GraalMapDy    = 0;

  static char GraalFirstEnterMap = GRAAL_TRUE;
  static char GraalFirstExpose   = GRAAL_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                         Lambda Map                          |
|                                                             |
\------------------------------------------------------------*/

  float      GraalLambdaMapStep;

  long       GraalLambdaMapX;
  long       GraalLambdaMapY;
  long       GraalLambdaMapDx;
  long       GraalLambdaMapDy;

  long       GraalBoundMapX; 
  long       GraalBoundMapY; 
  long       GraalBoundMapDx; 
  long       GraalBoundMapDy; 

  long       GraalPixelMapX;
  long       GraalPixelMapY;

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

  static String GraalMapEventTranslation =

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
  rdsbegin();

  GraalExitPanel( &GraalViewMapPanel );

  rdsend();
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
  rdsbegin();

  GraalComputeAndDisplayMap();

  rdsend();
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

  rdsbegin();

  EventDisplay = Event->display;
  MouseEvent   = Args[ 0 ][ 0 ] - '0';

  X = Event->x;
  Y = GraalMapDy - Event->y;

  X = X + GraalPixelMapX;
  Y = Y + GraalPixelMapY;

  if ( X < 0 )
  {
    X = ((float)(X) / GraalLambdaMapStep) - 0.5 ;
  }
  else
  {   
    X = ((float)(X) / GraalLambdaMapStep) + 0.5 ;
  }

  if ( Y < 0 )
  {
    Y = ((float)(Y) / GraalLambdaMapStep) - 0.5 ;
  }
  else
  {   
    Y = ((float)(Y) / GraalLambdaMapStep) + 0.5 ;
  }

  switch ( MouseEvent )
  {
    case GRAAL_B1UP   : 
    case GRAAL_B2UP   :
    case GRAAL_B3UP   :

     GraalZoomCenter( X, Y );

     break;
  }

  rdsend();
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
  if ( ! GraalFirstExpose )
  {
    rdsbegin();

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );
    GraalResizeMapWindow();
    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

    rdsend();
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

  rdsbegin();

  ExposeEvent = (XExposeEvent *)CallData->event;

  if ( GraalFirstExpose )
  {
    GraalFirstExpose = GRAAL_FALSE;

    GraalInitializeMapWindow();
  }

  GraalRefreshMapWindow();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Initialize Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     GraalsnitializeMapWindow                |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeMapWindow()

{
  rdsbegin();

  XtVaGetValues( GraalMapWindow,
                 XmNwidth , &GraalMapDx,
                 XmNheight, &GraalMapDy,
                 NULL
               );
 
  GraalInitializeLambdaMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Graphic Window Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       GraalRefreshMapWindow                 |
|                                                             |
\------------------------------------------------------------*/

void GraalRefreshMapWindow()
{
  rdsbegin();

  GraalClearMapWindow();
  GraalDisplayLambdaMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalClearMapWindow                   |
|                                                             |
\------------------------------------------------------------*/

void GraalClearMapWindow()

{
  XFillRectangle( XtDisplay( GraalMapWindow ),
                  XtWindow( GraalMapWindow ),
                  GraalBackgroundGC,
                  0, 0, GraalMapDx, GraalMapDy );
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalResizeMapWindow                  |
|                                                             |
\------------------------------------------------------------*/

void GraalResizeMapWindow()

{
  rdsbegin();

  XtVaGetValues( GraalMapWindow,
                 XmNwidth,  &GraalMapDx,
                 XmNheight, &GraalMapDy,
                 NULL
               );
 
  GraalComputeAndDisplayMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalInitializeMapEvent                   |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeMapEvent()

{
  XtActionsRec NewActions;

  rdsbegin();

  XtVaSetValues( GraalMapWindow,
                 XmNtranslations,
                 XtParseTranslationTable( GraalMapEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackMapEvent";
  NewActions.proc   =  CallbackMapEvent;

  XtAppAddActions( GraalApplication, &NewActions, 1 );

  XtAddCallback( GraalMapWindow, 
                 XmNresizeCallback, 
                 CallbackMapResize, NULL );

  XtAddCallback( GraalMapWindow, 
                 XmNexposeCallback, 
                 CallbackMapExpose, NULL );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Lambda Map Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                GraalInitializeLambdaMap                     |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeLambdaMap()

{
  rdsbegin();

  if ( ! GraalComputeBound() )
  {
    GraalBoundMapX  = GRAAL_DEFAULT_MAP_X;
    GraalBoundMapY  = GRAAL_DEFAULT_MAP_Y;
    GraalBoundMapDx = GRAAL_DEFAULT_MAP_DX;
    GraalBoundMapDy = GRAAL_DEFAULT_MAP_DY;
  }
  else
  {
    GraalBoundMapX  = GraalBoundXmin / GRAAL_RDS_LAMBDA;
    GraalBoundMapY  = GraalBoundYmin / GRAAL_RDS_LAMBDA;
    GraalBoundMapDx = GraalBoundXmax / GRAAL_RDS_LAMBDA;
    GraalBoundMapDy = GraalBoundYmax / GRAAL_RDS_LAMBDA;
    GraalBoundMapDx = GraalBoundMapDx - GraalBoundMapX;
    GraalBoundMapDy = GraalBoundMapDy - GraalBoundMapY;
  }

  GraalLambdaMapX  = GraalBoundMapX  - 2;
  GraalLambdaMapY  = GraalBoundMapY  - 2;
  GraalLambdaMapDx = GraalBoundMapDx + 4;
  GraalLambdaMapDy = GraalBoundMapDy + 4;

  GraalComputeLambdaMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalComputeLambdaMap                       |
|                                                             |
\------------------------------------------------------------*/

void GraalComputeLambdaMap()

{
  long  Delta;
  float StepX;
  float StepY;

  rdsbegin();

  StepX = (float)(GraalMapDx) / (float)(GraalLambdaMapDx);
  StepY = (float)(GraalMapDy) / (float)(GraalLambdaMapDy);

  if ( StepX < StepY )
  {
    GraalLambdaMapStep = StepX;
    Delta              = GraalLambdaMapDy; 
    GraalLambdaMapDy   = 1 + ( GraalMapDy / StepX );
    Delta              = ( GraalLambdaMapDy - Delta ) >> 1;
    GraalLambdaMapY    = GraalLambdaMapY - Delta;
  }
  else
  {
    GraalLambdaMapStep = StepY;
    Delta              = GraalLambdaMapDx; 
    GraalLambdaMapDx   = 1 + ( GraalMapDx / StepY );
    Delta              = ( GraalLambdaMapDx - Delta ) >> 1;
    GraalLambdaMapX    = GraalLambdaMapX - Delta;
  }

  GraalPixelMapX = (float)(GraalLambdaMapX) * GraalLambdaMapStep;
  GraalPixelMapY = (float)(GraalLambdaMapY) * GraalLambdaMapStep;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalDisplayLambdaMap                  |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayLambdaMap()
{
  long X1;
  long Y1;
  long X2;
  long Y2;

  rdsbegin();

  X1 = (float)( GraalBoundMapX                   ) * GraalLambdaMapStep;
  Y1 = (float)( GraalBoundMapY                   ) * GraalLambdaMapStep;
  X2 = (float)( GraalBoundMapX + GraalBoundMapDx ) * GraalLambdaMapStep;
  Y2 = (float)( GraalBoundMapY + GraalBoundMapDy ) * GraalLambdaMapStep;

  X1 = X1 - GraalPixelMapX;
  X2 = X2 - GraalPixelMapX;
  Y1 = Y1 - GraalPixelMapY;
  Y2 = Y2 - GraalPixelMapY;

  XDrawRectangle( XtDisplay( GraalMapWindow ),
                  XtWindow( GraalMapWindow ),
                  GraalLargeTextGC,
                  X1, GraalMapDy - Y2,
                  X2 - X1, Y2 - Y1 );

  X1 = GraalLambdaGridX + ( GraalLambdaGridDx >> 1 );
  Y1 = GraalLambdaGridY + ( GraalLambdaGridDy >> 1 );
  X1 = (float)( X1 ) * GraalLambdaMapStep;
  Y1 = (float)( Y1 ) * GraalLambdaMapStep;

  X1 = X1 - GraalPixelMapX;
  Y1 = Y1 - GraalPixelMapY;
  Y1 = GraalMapDy - Y1;

  if ( ( X1 > 0          ) && 
       ( Y1 > 0          ) &&
       ( X1 < GraalMapDx ) &&
       ( Y1 < GraalMapDy ) )
  {
    XDrawLine( XtDisplay( GraalMapWindow ),
               XtWindow( GraalMapWindow ),
               GraalLargeTextGC,
               X1 - 2, Y1,
               X1 + 2, Y1  );

    XDrawLine( XtDisplay( GraalMapWindow ),
               XtWindow( GraalMapWindow ),
               GraalLargeTextGC,
               X1, Y1 - 2,
               X1, Y1 + 2  );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalBuildPanelMap                     |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildPanelMap()
{
  Arg Args[3];

  rdsbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , GraalViewMapPanel.TITLE );

  GraalViewMapPanel.PANEL = 

    XmCreateFormDialog( GraalMainWindow, "GraalViewMapPanel", Args, 3);

  XtAddCallback( GraalViewMapPanel.PANEL, XmNdestroyCallback,
                 GraalDestroyDialogCallback, NULL  );

  GraalViewMapPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "GraalViewMapPanelForm",
                             xmFormWidgetClass,
                             GraalViewMapPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );
 
  GraalViewMapPanel.FRAME = 

    XtVaCreateManagedWidget( "GraalViewMapFrame",
                             xmFrameWidgetClass,
                             GraalViewMapPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  GraalViewMapPanel.FORM = 

    XtVaCreateManagedWidget( "GraalViewMapForm",
                             xmFormWidgetClass,
                             GraalViewMapPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );

  GraalMapButtonCompute = 

    XtVaCreateManagedWidget( "Compute",
                             xmPushButtonWidgetClass,
                             GraalViewMapPanel.FORM, 
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
  GraalMapButtonClose = 

    XtVaCreateManagedWidget( "Close",
                             xmPushButtonWidgetClass,
                             GraalViewMapPanel.FORM, 
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

  GraalMapWindow = 

    XtVaCreateManagedWidget( "GraalMapWindow",
                             xmDrawingAreaWidgetClass,
                             GraalViewMapPanel.FORM, 
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

  XtAddCallback( GraalMapButtonCompute, 
                 XmNactivateCallback,
                 CallbackMapCompute, NULL );

  XtAddCallback( GraalMapButtonClose, 
                 XmNactivateCallback,
                 CallbackCloseMap, NULL );

  XtVaSetValues( GraalViewMapPanel.PANEL,
                 XmNheight, GraalViewMapPanel.HEIGHT,
                 XmNwidth , GraalViewMapPanel.WIDTH,
                 XmNx     , GraalViewMapPanel.X,
                 XmNy     , GraalViewMapPanel.Y,
                 NULL );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalEnterMapPanel                      |
|                                                             |
\------------------------------------------------------------*/

void GraalEnterMapPanel()

{
  rdsbegin();

  GraalFirstEnterMap = GRAAL_FALSE;

  GraalEnterPanel( &GraalViewMapPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalComputeAndDisplayMap               |
|                                                             |
\------------------------------------------------------------*/

void GraalComputeAndDisplayMap()

{

  if ( ! GraalFirstEnterMap )
  {
    rdsbegin();

    GraalInitializeLambdaMap();
    GraalClearMapWindow();
    GraalDisplayLambdaMap();

    rdsend();
  }
}
