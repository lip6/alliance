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
| Tool    :                   XPAT                            |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
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

   XpatPanelItem XpatViewMapPanel =

         {
           "Map",
           1,
           0,
           XPAT_VIEW_MAP_X,
           XPAT_VIEW_MAP_Y,
           190,
           200,
           0,
           0,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (XpatPanelButtonItem *)NULL 
         };

   int XpatViewMapDefaultValues[ 5 ] =

         {
           XPAT_VIEW_MAP_X, 
           XPAT_VIEW_MAP_Y,
           190, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Graphic Map Window                     |
|                                                             |
\------------------------------------------------------------*/

  Widget     XpatMapWindow;
  Widget     XpatMapButtonCompute;
  Widget     XpatMapButtonClose;

  Dimension  XpatMapDx    = 0;
  Dimension  XpatMapDy    = 0;

  static char XpatFirstEnterMap = XPAT_TRUE;
  static char XpatFirstExpose   = XPAT_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                           Unit Map                          |
|                                                             |
\------------------------------------------------------------*/

  float      XpatUnitMapStep;

  long       XpatUnitMapX;
  long       XpatUnitMapY;
  long       XpatUnitMapDx;
  long       XpatUnitMapDy;

  long       XpatBoundMapX; 
  long       XpatBoundMapY; 
  long       XpatBoundMapDx; 
  long       XpatBoundMapDy; 

  long       XpatPixelMapX;
  long       XpatPixelMapY;

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

  static String XpatMapEventTranslation =

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

  XpatExitPanel( &XpatViewMapPanel );

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

  XpatComputeAndDisplayMap();

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
  char      MouseEvent;
  long      X;
  long      Y;

  autbegin();

  MouseEvent   = Args[ 0 ][ 0 ] - '0';

  X = Event->x;
  Y = XpatMapDy - Event->y;

  X = X + XpatPixelMapX;
  Y = Y + XpatPixelMapY;

  if ( X < 0 )
  {
    X = ((float)(X) / XpatUnitMapStep) - 0.5 ;
  }
  else
  {   
    X = ((float)(X) / XpatUnitMapStep) + 0.5 ;
  }

  if ( Y < 0 )
  {
    Y = ((float)(Y) / XpatUnitMapStep) - 0.5 ;
  }
  else
  {   
    Y = ((float)(Y) / XpatUnitMapStep) + 0.5 ;
  }

  switch ( MouseEvent )
  {
    case XPAT_B1UP   : 
    case XPAT_B2UP   :
    case XPAT_B3UP   :

     XpatZoomCenter( X, Y );

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
  if ( ! XpatFirstExpose )
  {
    autbegin();

    XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR  );
    XpatResizeMapWindow();
    XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

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
  autbegin();

  if ( XpatFirstExpose )
  {
    XpatFirstExpose = XPAT_FALSE;

    XpatInitializeMapWindow();
  }

  XpatRefreshMapWindow();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Initialize Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XpatsnitializeMapWindow                |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeMapWindow()

{
  autbegin();

  XtVaGetValues( XpatMapWindow,
                 XmNwidth , &XpatMapDx,
                 XmNheight, &XpatMapDy,
                 NULL
               );
 
  XpatInitializeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Graphic Window Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        XpatRefreshMapWindow                 |
|                                                             |
\------------------------------------------------------------*/

void XpatRefreshMapWindow()
{
  autbegin();

  XpatClearMapWindow();
  XpatDisplayUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatClearMapWindow                   |
|                                                             |
\------------------------------------------------------------*/

void XpatClearMapWindow()

{
  XFillRectangle( XtDisplay( XpatMapWindow ),
                  XtWindow( XpatMapWindow ),
                  XpatBackgroundGC,
                  0, 0, XpatMapDx, XpatMapDy );
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatResizeMapWindow                  |
|                                                             |
\------------------------------------------------------------*/

void XpatResizeMapWindow()

{
  autbegin();

  XtVaGetValues( XpatMapWindow,
                 XmNwidth,  &XpatMapDx,
                 XmNheight, &XpatMapDy,
                 NULL
               );
 
  XpatComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatInitializeMapEvent                   |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeMapEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XpatMapWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XpatMapEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackMapEvent";
  NewActions.proc   =  CallbackMapEvent;

  XtAppAddActions( XpatApplication, &NewActions, 1 );

  XtAddCallback( XpatMapWindow, 
                 XmNresizeCallback, 
                 CallbackMapResize, NULL );

  XtAddCallback( XpatMapWindow, 
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
|                   XpatInitializeUnitMap                     |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeUnitMap()

{
  autbegin();

  if ( ! XpatComputeBound() )
  {
    XpatBoundMapX  = XPAT_DEFAULT_MAP_X;
    XpatBoundMapY  = XPAT_DEFAULT_MAP_Y;
    XpatBoundMapDx = XPAT_DEFAULT_MAP_DX;
    XpatBoundMapDy = XPAT_DEFAULT_MAP_DY;
  }
  else
  {
    XpatBoundMapX  = XpatBoundXmin / XPAT_UNIT;
    XpatBoundMapY  = XpatBoundYmin / XPAT_UNIT;
    XpatBoundMapDx = XpatBoundXmax / XPAT_UNIT;
    XpatBoundMapDy = XpatBoundYmax / XPAT_UNIT;
    XpatBoundMapDx = XpatBoundMapDx - XpatBoundMapX;
    XpatBoundMapDy = XpatBoundMapDy - XpatBoundMapY;
  }

  XpatUnitMapX  = XpatBoundMapX  - 2;
  XpatUnitMapY  = XpatBoundMapY  - 2;
  XpatUnitMapDx = XpatBoundMapDx + 4;
  XpatUnitMapDy = XpatBoundMapDy + 4;

  XpatComputeUnitMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatComputeUnitMap                       |
|                                                             |
\------------------------------------------------------------*/

void XpatComputeUnitMap()

{
  long  Delta;
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XpatMapDx) / (float)(XpatUnitMapDx);
  StepY = (float)(XpatMapDy) / (float)(XpatUnitMapDy);

  if ( StepX < StepY )
  {
    XpatUnitMapStep = StepX;
    Delta              = XpatUnitMapDy; 
    XpatUnitMapDy   = 1 + ( XpatMapDy / StepX );
    Delta              = ( XpatUnitMapDy - Delta ) >> 1;
    XpatUnitMapY    = XpatUnitMapY - Delta;
  }
  else
  {
    XpatUnitMapStep = StepY;
    Delta              = XpatUnitMapDx; 
    XpatUnitMapDx   = 1 + ( XpatMapDx / StepY );
    Delta              = ( XpatUnitMapDx - Delta ) >> 1;
    XpatUnitMapX    = XpatUnitMapX - Delta;
  }

  XpatPixelMapX = (float)(XpatUnitMapX) * XpatUnitMapStep;
  XpatPixelMapY = (float)(XpatUnitMapY) * XpatUnitMapStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatDisplayUnitMap                  |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayUnitMap()
{
  long X1;
  long Y1;
  long X2;
  long Y2;

  autbegin();

  X1 = (float)( XpatBoundMapX                   ) * XpatUnitMapStep;
  Y1 = (float)( XpatBoundMapY                   ) * XpatUnitMapStep;
  X2 = (float)( XpatBoundMapX + XpatBoundMapDx ) * XpatUnitMapStep;
  Y2 = (float)( XpatBoundMapY + XpatBoundMapDy ) * XpatUnitMapStep;

  X1 = X1 - XpatPixelMapX;
  X2 = X2 - XpatPixelMapX;
  Y1 = Y1 - XpatPixelMapY;
  Y2 = Y2 - XpatPixelMapY;

  XDrawRectangle( XtDisplay( XpatMapWindow ),
                  XtWindow( XpatMapWindow ),
                  XpatLargeTextGC,
                  X1, XpatMapDy - Y2,
                  X2 - X1, Y2 - Y1 );

  X1 = XpatUnitGridX + ( XpatUnitGridDx >> 1 );
  Y1 = XpatUnitGridY + ( XpatUnitGridDy >> 1 );
  X1 = (float)( X1 ) * XpatUnitMapStep;
  Y1 = (float)( Y1 ) * XpatUnitMapStep;

  X1 = X1 - XpatPixelMapX;
  Y1 = Y1 - XpatPixelMapY;
  Y1 = XpatMapDy - Y1;

  if ( ( X1 > 0          ) && 
       ( Y1 > 0          ) &&
       ( X1 < XpatMapDx ) &&
       ( Y1 < XpatMapDy ) )
  {
    XDrawLine( XtDisplay( XpatMapWindow ),
               XtWindow( XpatMapWindow ),
               XpatLargeTextGC,
               X1 - 2, Y1,
               X1 + 2, Y1  );

    XDrawLine( XtDisplay( XpatMapWindow ),
               XtWindow( XpatMapWindow ),
               XpatLargeTextGC,
               X1, Y1 - 2,
               X1, Y1 + 2  );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatBuildPanelMap                      |
|                                                             |
\------------------------------------------------------------*/

void XpatBuildPanelMap()
{
  Arg Args[3];

  autbegin();

  XtSetArg( Args[0], XmNshadowType    , XmSHADOW_ETCHED_IN      );
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING            );
  XtSetArg( Args[2], XmNtitle         , XpatViewMapPanel.TITLE );

  XpatViewMapPanel.PANEL = 

    XmCreateFormDialog( XpatMainWindow, "XpatViewMapPanel", Args, 3);

  XtAddCallback( XpatViewMapPanel.PANEL, XmNdestroyCallback,
                 XpatDestroyDialogCallback, NULL  );

  XpatViewMapPanel.PANEL_FORM = 

    XtVaCreateManagedWidget( "XpatViewMapPanelForm",
                             xmFormWidgetClass,
                             XpatViewMapPanel.PANEL,
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );
 
  XpatViewMapPanel.FRAME = 

    XtVaCreateManagedWidget( "XpatViewMapFrame",
                             xmFrameWidgetClass,
                             XpatViewMapPanel.PANEL_FORM, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             NULL
                           );
 
  XpatViewMapPanel.FORM = 

    XtVaCreateManagedWidget( "XpatViewMapForm",
                             xmFormWidgetClass,
                             XpatViewMapPanel.FRAME, 
                             XmNtopAttachment    , XmATTACH_FORM,
                             XmNbottomAttachment , XmATTACH_FORM,
                             XmNleftAttachment   , XmATTACH_FORM,
                             XmNrightAttachment  , XmATTACH_FORM,
                             XmNfractionBase     , 100,
                             NULL
                           );

  XpatMapButtonCompute = 

    XtVaCreateManagedWidget( "Compute",
                             xmPushButtonWidgetClass,
                             XpatViewMapPanel.FORM, 
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
  XpatMapButtonClose = 

    XtVaCreateManagedWidget( "Close",
                             xmPushButtonWidgetClass,
                             XpatViewMapPanel.FORM, 
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

  XpatMapWindow = 

    XtVaCreateManagedWidget( "XpatMapWindow",
                             xmDrawingAreaWidgetClass,
                             XpatViewMapPanel.FORM, 
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

  XtAddCallback( XpatMapButtonCompute, 
                 XmNactivateCallback,
                 CallbackMapCompute, NULL );

  XtAddCallback( XpatMapButtonClose, 
                 XmNactivateCallback,
                 CallbackCloseMap, NULL );

  XtVaSetValues( XpatViewMapPanel.PANEL,
                 XmNheight, XpatViewMapPanel.HEIGHT,
                 XmNwidth , XpatViewMapPanel.WIDTH,
                 XmNx     , XpatViewMapPanel.X,
                 XmNy     , XpatViewMapPanel.Y,
                 NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatEnterMapPanel                      |
|                                                             |
\------------------------------------------------------------*/

void XpatEnterMapPanel()

{
  autbegin();

  XpatFirstEnterMap = XPAT_FALSE;

  XpatEnterPanel( &XpatViewMapPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatComputeAndDisplayMap               |
|                                                             |
\------------------------------------------------------------*/

void XpatComputeAndDisplayMap()

{

  if ( ! XpatFirstEnterMap )
  {
    autbegin();

    XpatInitializeUnitMap();
    XpatClearMapWindow();
    XpatDisplayUnitMap();

    autend();
  }
}
