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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Map.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include "GRD.h"
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

   DrealPanelItem DrealViewMapPanel =

         {
           "Map",
           1,
           0,
           DREAL_VIEW_MAP_X,
           DREAL_VIEW_MAP_Y,
           190,
           200,
           1,
           1,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           0,
           (DrealPanelButtonItem *)NULL 
         };

   int DrealViewMapDefaultValues[ 5 ] =

         {
           DREAL_VIEW_MAP_X, 
           DREAL_VIEW_MAP_Y,
           190, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Graphic Map Window                     |
|                                                             |
\------------------------------------------------------------*/

  Widget     DrealMapWindow;
  Widget     DrealMapButtonCompute;
  Widget     DrealMapButtonClose;

  Dimension  DrealMapDx    = 0;
  Dimension  DrealMapDy    = 0;

  static char DrealFirstEnterMap = DREAL_TRUE;
  static char DrealFirstExpose   = DREAL_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                         Lambda Map                          |
|                                                             |
\------------------------------------------------------------*/

  float      DrealLambdaMapStep;

  long       DrealLambdaMapX;
  long       DrealLambdaMapY;
  long       DrealLambdaMapDx;
  long       DrealLambdaMapDy;

  long       DrealBoundMapX; 
  long       DrealBoundMapY; 
  long       DrealBoundMapDx; 
  long       DrealBoundMapDy; 

  long       DrealPixelMapX;
  long       DrealPixelMapY;

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

  static String DrealMapEventTranslation =

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

  DrealExitPanel( &DrealViewMapPanel );

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

  DrealComputeAndDisplayMap();

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
  char      MouseEvent;
  long      X;
  long      Y;

  rdsbegin();

  MouseEvent = Args[ 0 ][ 0 ] - '0';

  X = Event->x;
  Y = DrealMapDy - Event->y;

  X = X + DrealPixelMapX;
  Y = Y + DrealPixelMapY;

  if ( X < 0 )
  {
    X = ((float)(X) / DrealLambdaMapStep) - 0.5 ;
  }
  else
  {   
    X = ((float)(X) / DrealLambdaMapStep) + 0.5 ;
  }

  if ( Y < 0 )
  {
    Y = ((float)(Y) / DrealLambdaMapStep) - 0.5 ;
  }
  else
  {   
    Y = ((float)(Y) / DrealLambdaMapStep) + 0.5 ;
  }

  switch ( MouseEvent )
  {
    case DREAL_B1UP   : 
    case DREAL_B2UP   :
    case DREAL_B3UP   :

     DrealZoomCenter( X, Y );

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
  rdsbegin();

  if ( ! DrealFirstExpose )
  {
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR  );
    DrealResizeMapWindow();
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );
  }

  rdsend();
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
  rdsbegin();

  if ( DrealFirstExpose )
  {
    DrealFirstExpose = DREAL_FALSE;

    DrealInitializeMapWindow();
  }

  DrealRefreshMapWindow();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Initialize Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     DrealsnitializeMapWindow                |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeMapWindow()

{
  rdsbegin();

  XtVaGetValues( DrealMapWindow,
                 XmNwidth , &DrealMapDx,
                 XmNheight, &DrealMapDy,
                 NULL
               );
 
  DrealInitializeLambdaMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Graphic Window Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       DrealRefreshMapWindow                 |
|                                                             |
\------------------------------------------------------------*/

void DrealRefreshMapWindow()
{
  rdsbegin();

  DrealClearMapWindow();
  DrealDisplayLambdaMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealClearMapWindow                   |
|                                                             |
\------------------------------------------------------------*/

void DrealClearMapWindow()

{
  XFillRectangle( XtDisplay( DrealMapWindow ),
                  XtWindow( DrealMapWindow ),
                  DrealBackgroundGC,
                  0, 0, DrealMapDx, DrealMapDy );
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealResizeMapWindow                  |
|                                                             |
\------------------------------------------------------------*/

void DrealResizeMapWindow()

{
  XtVaGetValues( DrealMapWindow,
                 XmNwidth,  &DrealMapDx,
                 XmNheight, &DrealMapDy,
                 NULL
               );
 
  DrealComputeAndDisplayMap();
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealInitializeMapEvent                   |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeMapEvent()

{
  XtActionsRec NewActions;

  XtVaSetValues( DrealMapWindow,
                 XmNtranslations,
                 XtParseTranslationTable( DrealMapEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackMapEvent";
  NewActions.proc   =  CallbackMapEvent;

  XtAppAddActions( DrealApplication, &NewActions, 1 );

  XtAddCallback( DrealMapWindow, 
                 XmNresizeCallback, 
                 CallbackMapResize, NULL );

  XtAddCallback( DrealMapWindow, 
                 XmNexposeCallback, 
                 CallbackMapExpose, NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                     Lambda Map Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                DrealInitializeLambdaMap                     |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeLambdaMap()

{
  if ( ! DrealComputeBound() )
  {
    DrealBoundMapX  = DREAL_DEFAULT_MAP_X;
    DrealBoundMapY  = DREAL_DEFAULT_MAP_Y;
    DrealBoundMapDx = DREAL_DEFAULT_MAP_DX;
    DrealBoundMapDy = DREAL_DEFAULT_MAP_DY;
  }
  else
  {
    DrealBoundMapX  = DrealBoundXmin / RDS_PHYSICAL_GRID;
    DrealBoundMapY  = DrealBoundYmin / RDS_PHYSICAL_GRID;
    DrealBoundMapDx = DrealBoundXmax / RDS_PHYSICAL_GRID;
    DrealBoundMapDy = DrealBoundYmax / RDS_PHYSICAL_GRID;
    DrealBoundMapDx = DrealBoundMapDx - DrealBoundMapX;
    DrealBoundMapDy = DrealBoundMapDy - DrealBoundMapY;
  }

  DrealLambdaMapX  = DrealBoundMapX  - 2;
  DrealLambdaMapY  = DrealBoundMapY  - 2;
  DrealLambdaMapDx = DrealBoundMapDx + 4;
  DrealLambdaMapDy = DrealBoundMapDy + 4;

  DrealComputeLambdaMap();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealComputeLambdaMap                       |
|                                                             |
\------------------------------------------------------------*/

void DrealComputeLambdaMap()

{
  long  Delta;
  float StepX;
  float StepY;

  StepX = (float)(DrealMapDx) / (float)(DrealLambdaMapDx);
  StepY = (float)(DrealMapDy) / (float)(DrealLambdaMapDy);

  if ( StepX < StepY )
  {
    DrealLambdaMapStep = StepX;
    Delta              = DrealLambdaMapDy; 
    DrealLambdaMapDy   = 1 + ( DrealMapDy / StepX );
    Delta              = ( DrealLambdaMapDy - Delta ) >> 1;
    DrealLambdaMapY    = DrealLambdaMapY - Delta;
  }
  else
  {
    DrealLambdaMapStep = StepY;
    Delta              = DrealLambdaMapDx; 
    DrealLambdaMapDx   = 1 + ( DrealMapDx / StepY );
    Delta              = ( DrealLambdaMapDx - Delta ) >> 1;
    DrealLambdaMapX    = DrealLambdaMapX - Delta;
  }

  DrealPixelMapX = (float)(DrealLambdaMapX) * DrealLambdaMapStep;
  DrealPixelMapY = (float)(DrealLambdaMapY) * DrealLambdaMapStep;
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealDisplayLambdaMap                  |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayLambdaMap()
{
  long X1;
  long Y1;
  long X2;
  long Y2;

  X1 = (float)( DrealBoundMapX                   ) * DrealLambdaMapStep;
  Y1 = (float)( DrealBoundMapY                   ) * DrealLambdaMapStep;
  X2 = (float)( DrealBoundMapX + DrealBoundMapDx ) * DrealLambdaMapStep;
  Y2 = (float)( DrealBoundMapY + DrealBoundMapDy ) * DrealLambdaMapStep;

  X1 = X1 - DrealPixelMapX;
  X2 = X2 - DrealPixelMapX;
  Y1 = Y1 - DrealPixelMapY;
  Y2 = Y2 - DrealPixelMapY;

  XDrawRectangle( XtDisplay( DrealMapWindow ),
                  XtWindow( DrealMapWindow ),
                  DrealLargeTextGC,
                  X1, DrealMapDy - Y2,
                  X2 - X1, Y2 - Y1 );

  X1 = DrealLambdaGridX + ( DrealLambdaGridDx >> 1 );
  Y1 = DrealLambdaGridY + ( DrealLambdaGridDy >> 1 );
  X1 = (float)( X1 ) * DrealLambdaMapStep;
  Y1 = (float)( Y1 ) * DrealLambdaMapStep;

  X1 = X1 - DrealPixelMapX;
  Y1 = Y1 - DrealPixelMapY;
  Y1 = DrealMapDy - Y1;

  if ( ( X1 > 0          ) && 
       ( Y1 > 0          ) &&
       ( X1 < DrealMapDx ) &&
       ( Y1 < DrealMapDy ) )
  {
    XDrawLine( XtDisplay( DrealMapWindow ),
               XtWindow( DrealMapWindow ),
               DrealLargeTextGC,
               X1 - 2, Y1,
               X1 + 2, Y1  );

    XDrawLine( XtDisplay( DrealMapWindow ),
               XtWindow( DrealMapWindow ),
               DrealLargeTextGC,
               X1, Y1 - 2,
               X1, Y1 + 2  );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealBuildPanelMap                     |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildPanelMap()
{
  Arg Args[3];

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , DrealViewMapPanel.TITLE );

  DrealViewMapPanel.PANEL = 

    XmCreateFormDialog( DrealMainWindow, "DrealViewMapPanel", Args, 3);

  XtAddCallback( DrealViewMapPanel.PANEL, XmNdestroyCallback,
                 DrealDestroyDialogCallback, NULL  );

  DrealViewMapPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "DrealViewMapPanelForm",
                             xmFormWidgetClass,
                             DrealViewMapPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );
 
  DrealViewMapPanel.FRAME = 

    XtVaCreateManagedWidget( "DrealViewMapFrame",
                             xmFrameWidgetClass,
                             DrealViewMapPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  DrealViewMapPanel.FORM = 

    XtVaCreateManagedWidget( "DrealViewMapForm",
                             xmFormWidgetClass,
                             DrealViewMapPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );

  DrealMapButtonCompute =

    XtVaCreateManagedWidget( "Compute",
                             xmPushButtonWidgetClass,
                             DrealViewMapPanel.FORM,
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
  DrealMapButtonClose =

    XtVaCreateManagedWidget( "Close",
                             xmPushButtonWidgetClass,
                             DrealViewMapPanel.FORM,
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

  DrealMapWindow = 

    XtVaCreateManagedWidget( "DrealMapWindow",
                             xmDrawingAreaWidgetClass,
                             DrealViewMapPanel.FORM, 
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

  XtAddCallback( DrealMapButtonCompute,
                 XmNactivateCallback,
                 CallbackMapCompute, NULL );

  XtAddCallback( DrealMapButtonClose, 
                 XmNactivateCallback,
                 CallbackCloseMap, NULL );

  XtVaSetValues( DrealViewMapPanel.PANEL,
                 XmNheight, DrealViewMapPanel.HEIGHT,
                 XmNwidth , DrealViewMapPanel.WIDTH,
                 XmNx     , DrealViewMapPanel.X,
                 XmNy     , DrealViewMapPanel.Y,
                 NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealEnterMapPanel                      |
|                                                             |
\------------------------------------------------------------*/

void DrealEnterMapPanel()

{
  DrealFirstEnterMap = DREAL_FALSE;

  DrealEnterPanel( &DrealViewMapPanel );
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealComputeAndDisplayMap               |
|                                                             |
\------------------------------------------------------------*/

void DrealComputeAndDisplayMap()

{
  if ( ! DrealFirstEnterMap )
  {
    DrealInitializeLambdaMap();
    DrealClearMapWindow();
    DrealDisplayLambdaMap();
  }
}
