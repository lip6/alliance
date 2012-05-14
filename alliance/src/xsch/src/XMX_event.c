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
| File    :                   Event.c                         |
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
# include <stdlib.h>
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <Xm/Xm.h>

 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
# include "XMX.h"
# include "XTB.h"
# include "XME.h"
# include "XMF.h"
# include "XMV.h"
# include "XMS.h"
# include "XMH.h"
# include "XMX_motif.h"
# include "XMX_grid.h"
# include "XMX_cursor.h"
# include "XMX_event.h"
# include "XMX_message.h"

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
|                          Translation                        |
|                                                             |
\------------------------------------------------------------*/

  static String XschEventTranslation =

    "<Btn1Down>:    CallbackEvent(  0 )\n\
     <Btn3Down>:    CallbackEvent(  1 )\n\
     <Motion>:      CallbackEvent(  2 )\n\
     <EnterWindow>: CallbackEvent(  3 )\n\
     <LeaveWindow>: CallbackEvent(  4 )\n\
     <Key>osfUp:    CallbackEvent(  5 )\n\
     <Btn4Down>:    CallbackEvent(  5 )\n\
     <Key>osfDown:  CallbackEvent(  6 )\n\
     <Btn5Down>:    CallbackEvent(  6 )\n\
     <Key>osfLeft:  CallbackEvent(  7 )\n\
     <Key>osfRight: CallbackEvent(  8 )\n\
     <Key>Z:        CallbackEvent(  9 )\n\
     <Key>M:        CallbackEvent( 10 )\n\
     <Key>F:        CallbackEvent( 11 )\n\
     <Key>R:        CallbackEvent( 12 )\n\
     <Key>U:        CallbackEvent( 13 )\n\
     <Key>I:        CallbackEvent( 14 )\n\
     <Key>C:        CallbackEvent( 15 )\n\
     <Key>P:        CallbackEvent( 16 )\n\
     <Key>+:        CallbackEvent( 17 )\n\
     <Key>-:        CallbackEvent( 18 )\n";


/*------------------------------------------------------------\
|                                                             |
|                         Event Count                         |
|                                                             |
\------------------------------------------------------------*/

  static int  XschMaxEventEditTable[] =

  {
    2,  /* XSCH_EDIT_MEASURE        */
    1,  /* XSCH_EDIT_IDENTIFY       */
    1,  /* XSCH_EDIT_CONNECTED      */
    1   /* XSCH_TOOLS_HIERARCH_DOWN */
  };

  static int  XschInputEventEditTable[] =

  {
    XSCH_INPUT_HALF_BOX, /* XSCH_EDIT_MEASURE        */
    XSCH_INPUT_POINT,    /* XSCH_EDIT_IDENTIFY       */
    XSCH_INPUT_POINT,    /* XSCH_EDIT_CONNECTED      */
    XSCH_INPUT_POINT     /* XSCH_TOOLS_HIERARCH_DOWN */
  };

  static int  XschMaxEventZoomTable[] =

  {
    1,  /* XSCH_ZOOM_CENTER            */
    2,  /* XSCH_ZOOM_IN                */
    2   /* XSCH_ZOOM_PAN               */
  };

  static int  XschInputEventZoomTable[] =

  {
    XSCH_INPUT_POINT,  /* XSCH_ZOOM_CENTER            */
    XSCH_INPUT_BOX,    /* XSCH_ZOOM_IN                */
    XSCH_INPUT_LINE    /* XSCH_ZOOM_PAN               */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Translation                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                      Expose Edit Zoom                       |
|                                                             |
\------------------------------------------------------------*/

  static long     XschUnitEventZoomX[ 2 ] = { 0, 0 };
  static long     XschUnitEventZoomY[ 2 ] = { 0, 0 };
  static int      XschMaxEventZoom          = 0;
         int      XschCountEventZoom        = 0;

  static long     XschUnitEventEditX[ 2 ] = { 0, 0 };
  static long     XschUnitEventEditY[ 2 ] = { 0, 0 };
  static int      XschMaxEventEdit          = 2;
         int      XschCountEventEdit        = 0;

  static int      XschFirstExpose           = XSCH_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                          Input                              |
|                                                             |
\------------------------------------------------------------*/

 static int  XschInputMode     = XSCH_INPUT_HALF_BOX;
 static int  XschSaveInputMode = XSCH_INPUT_HALF_BOX;
 static int  XschEditMode      = XSCH_EDIT_MEASURE;
 static int  XschSaveEditMode  = XSCH_EDIT_MEASURE;

/*------------------------------------------------------------\
|                                                             |
|                          Edit Message                       |
|                                                             |
\------------------------------------------------------------*/

  static void (*XschPromptMessage)()     = XschPromptEditMeasure;
  static void (*XschSavePromptMessage)() = XschPromptEditMeasure;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XschChangeEditMode                     |
|                                                             |
\------------------------------------------------------------*/
 
void XschChangeEditMode( EditMode , PromptMessage )
 
   int   EditMode;
   void (*PromptMessage)();
{
  autbegin();

  if ( EditMode & XSCH_ZOOM_MARK )
  {
    EditMode = EditMode & ~XSCH_ZOOM_MARK;

    XschCountEventZoom = 0;
    XschMaxEventZoom   = XschMaxEventZoomTable[ EditMode ];

    if ( ! ( XschEditMode & XSCH_ZOOM_MARK ) )
    {
      XschSaveEditMode      = XschEditMode;
      XschSaveInputMode     = XschInputMode;
      XschSavePromptMessage = XschPromptMessage;
    }

    XschInputMode     = XschInputEventZoomTable[ EditMode ];
    XschPromptMessage = PromptMessage;
    XschEditMode      = EditMode | XSCH_ZOOM_MARK;

    XschChangeCursorType( NULL, NULL, 0, XschInputMode );
  }  
  else
  {
    if ( XschEditMode & XSCH_ZOOM_MARK )
    {
      XschSaveEditMode      = EditMode;
      XschSaveInputMode     = XschInputEventEditTable[ EditMode ];
      XschSavePromptMessage = PromptMessage;
      XschCountEventEdit    = 0;
      XschMaxEventEdit      = XschMaxEventEditTable[ EditMode ];
    }
    else
    {
      if ( XschEditMode != EditMode )
      {
        XschCountEventEdit = 0;
        XschMaxEventEdit   = XschMaxEventEditTable[ EditMode ];
        XschEditMode       = EditMode;
        XschInputMode      = XschInputEventEditTable[ EditMode ];
        XschPromptMessage  = PromptMessage;

        XschChangeCursorType( NULL, NULL, 0, XschInputMode );
      }
    }

    XschEditUnselectAll();
  }

  XschPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XschContinueEditMode                     |
|                                                             |
\------------------------------------------------------------*/

void XschContinueEditMode( EditMode , PromptMessage, CountEvent )

   int   EditMode;
   void (*PromptMessage)();
   int   CountEvent;
{
  autbegin();

  if ( XschEditMode & XSCH_ZOOM_MARK )
  {
    XschSaveEditMode      = EditMode;
    XschSaveInputMode     = XschInputEventEditTable[ EditMode ];
    XschSavePromptMessage = PromptMessage;
    XschCountEventEdit    = CountEvent;
    XschMaxEventEdit      = XschMaxEventEditTable[ EditMode ];
  }
  else
  {
    XschCountEventEdit = CountEvent;
    XschMaxEventEdit   = XschMaxEventEditTable[ EditMode ];
    XschEditMode       = EditMode;
    XschInputMode      = XschInputEventEditTable[ EditMode ];
    XschPromptMessage  = PromptMessage;

    XschChangeCursorType( XschUnitEventEditX,
                           XschUnitEventEditY,
                           XschCountEventEdit,
                           XschInputMode );

  }

  XschPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschTreatEventZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XschTreatEventZoom()

{
  autbegin();

  XschChangeCursorType( XschUnitEventEditX,
                         XschUnitEventEditY,
                         XschCountEventEdit,
                         XschSaveInputMode );

  switch ( XschEditMode )
  {
    case XSCH_ZOOM_CENTER :
                              
      XschZoomCenter( XschUnitEventZoomX[ 0 ], 
                       XschUnitEventZoomY[ 0 ] );
      break;

    case XSCH_ZOOM_IN     :

      XschZoomIn( XschUnitEventZoomX[ 0 ],
                   XschUnitEventZoomY[ 0 ],
                   XschUnitEventZoomX[ 1 ],
                   XschUnitEventZoomY[ 1 ] );
      break;

    case XSCH_ZOOM_PAN    :

      XschZoomPan( XschUnitEventZoomX[ 0 ],
                    XschUnitEventZoomY[ 0 ],
                    XschUnitEventZoomX[ 1 ],
                    XschUnitEventZoomY[ 1 ] );
      break;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschTreatEventEdit                    |
|                                                             |
\------------------------------------------------------------*/

void XschTreatEventEdit()

{ 
  long DeltaX;
  long DeltaY;
  long X1;
  long X2;
  long Y1;
  long Y2;

  autbegin();

  X1 = XschUnitEventEditX[0];
  X2 = XschUnitEventEditX[1];
  Y1 = XschUnitEventEditY[0];
  Y2 = XschUnitEventEditY[1];

  if ( ( XschInputMode == XSCH_INPUT_LSTRING ) || 
       ( XschInputMode == XSCH_INPUT_ORTHO   ) ||
       ( XschInputMode == XSCH_INPUT_SORTHO  ) )
  {
    DeltaX = X1 - X2;
    DeltaY = Y1 - Y2;

    if ( DeltaX < 0 ) DeltaX = - DeltaX;
    if ( DeltaY < 0 ) DeltaY = - DeltaY;

    if ( DeltaX > DeltaY )
    {
      Y2 = Y1;

      XschUnitEventEditX[0] = X2;
      XschUnitEventEditY[0] = Y2;
    }  
    else
    {   
      X2 = X1;

      XschUnitEventEditX[0] = X2;
      XschUnitEventEditY[0] = Y2;
    }
  }

  if ( XschInputMode == XSCH_INPUT_LSTRING )
  {   
    XschChangeCursorType( XschUnitEventEditX,
                           XschUnitEventEditY,
                           1, XschInputMode );
 
    XschCountEventEdit = 1;
  }
  else
  {
    XschChangeCursorType( NULL, NULL, 0,
                           XschInputMode );

    XschCountEventEdit = 0;
  }

  switch ( XschEditMode )
  {
    case XSCH_EDIT_MEASURE :

    break;

    case XSCH_EDIT_IDENTIFY :

     XschEditIdentify( X1, Y1 );

    break;

    case XSCH_EDIT_CONNECTED :

     XschEditConnected( X1, Y1 );

    break;

    case XSCH_TOOLS_HIERARCHY_DOWN :

      XschToolsSelectHierarchyDown( X1, Y1 );

    break;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackEvent                         |
|                                                             |
\------------------------------------------------------------*/

void CallbackEvent ( MyWidget, Event, Args, Argc )

     Widget        MyWidget;
     XButtonEvent *Event;
     String       *Args;
     int          *Argc;
{
  Display  *EventDisplay;
  int       MouseEvent;
  Position  OldUnitCursorX;
  Position  OldUnitCursorY;

  autbegin();

  EventDisplay = Event->display;

  MouseEvent = atoi( Args[ 0 ] );

  if ( MouseEvent == XSCH_LEAVE )
  {
    XschUndisplayCursor();
    XschCursorInside = XSCH_FALSE;
  }
  else
  {
    OldUnitCursorX = XschUnitCursorX;
    OldUnitCursorY = XschUnitCursorY;

    XschComputeCursor( Event->x, Event->y );

    switch ( MouseEvent )
    {
      case XSCH_B1DN   : 

        if ( XschEditMode & XSCH_ZOOM_MARK )
        {
          if ( XschCountEventZoom < XschMaxEventZoom )
          {
            XschUnitEventZoomX[ XschCountEventZoom ] = XschUnitCursorX;
            XschUnitEventZoomY[ XschCountEventZoom ] = XschUnitCursorY;

            XschCountEventZoom = XschCountEventZoom + 1;

            XschPointCursor();
          
            if ( XschCountEventZoom == XschMaxEventZoom )
            {
              XschChangeCursorType( NULL, NULL, 0, XschInputMode );

              XschTreatEventZoom();

              XschInputMode      = XschSaveInputMode;
              XschEditMode       = XschSaveEditMode;
              XschPromptMessage  = XschSavePromptMessage;
              XschSaveEditMode   = XSCH_EDIT_MEASURE;
              XschSaveInputMode  = XSCH_INPUT_HALF_BOX;
              XschCountEventZoom = 0;
              XschMaxEventZoom   = 0;

              XschChangeCursorType( XschUnitEventEditX,
                                    XschUnitEventEditY,
                                    XschCountEventEdit,
                                    XschInputMode );

              XschComputeCursor( Event->x, Event->y );
              XschDisplayCursor();
            }
          }
        }
        else
        {
          if ( XschCountEventEdit < XschMaxEventEdit )
          {
            XschUnitEventEditX[ XschCountEventEdit ] = XschUnitCursorX;
            XschUnitEventEditY[ XschCountEventEdit ] = XschUnitCursorY;

            XschCountEventEdit = XschCountEventEdit + 1;

            XschPointCursor();

            if ( XschCountEventEdit == XschMaxEventEdit )
            {
              XschTreatEventEdit();

              XschDisplayCursor();
            }
          }
        }

        XschPromptMessage();

        break;

      case XSCH_B3DN   :

        if ( XschEditMode & XSCH_ZOOM_MARK )
        {
          if ( XschCountEventZoom != 0 )
          {
            XschCountEventZoom = XschCountEventZoom - 1;

            XschResetCursor();
          }
          else
          { 
            XschChangeCursorType( XschUnitEventEditX,
                                  XschUnitEventEditY,
                                  XschCountEventEdit,
                                  XschSaveInputMode );

            XschInputMode          = XschSaveInputMode;
            XschEditMode           = XschSaveEditMode;
            XschPromptMessage      = XschSavePromptMessage;
            XschSaveEditMode       = XSCH_EDIT_MEASURE;
            XschSaveInputMode      = XSCH_INPUT_HALF_BOX;
            XschSavePromptMessage  = XschPromptEditMeasure;
            XschCountEventZoom     = 0;
            XschMaxEventZoom       = 0;
          }
        }
        else
        {
          if ( XschCountEventEdit != 0 )
          {
            XschCountEventEdit = XschCountEventEdit - 1;

            XschResetCursor();
          }
          else
          {
            XschEditUnselectAll();

            XschChangeCursorType( NULL, NULL, 0,
                                   XSCH_INPUT_HALF_BOX );

            XschInputMode      = XSCH_INPUT_HALF_BOX;
            XschEditMode       = XSCH_EDIT_MEASURE;
            XschPromptMessage  = XschPromptEditMeasure;
            XschCountEventEdit = 0;
            XschMaxEventEdit   = 2;
          }
        }

        XschPromptMessage();

        break; 

      case XSCH_MOTION :

        break;

      case XSCH_ENTER  : XschCursorInside = XSCH_TRUE;
                          XschDisplayCursor();
                          break;

      case XSCH_KEY_UP    : XschZoomUp();

        break;

      case XSCH_KEY_DN : XschZoomDown();

        break;

      case XSCH_KEY_LEFT : XschZoomLeft();

        break;

      case XSCH_KEY_RIGHT : XschZoomRight();

        break;

      case XSCH_KEY_MORE : XschZoomMore();

        break;

      case XSCH_KEY_LESS : XschZoomLess();

        break;

      case XSCH_KEY_FIT : XschZoomFit();

        break;

      case XSCH_KEY_REFRESH : XschZoomRefresh();

        break;

      case XSCH_KEY_UNZOOM : XschZoomUndo();

        break;

      case XSCH_KEY_ZOOM_IN : 

         XschChangeEditMode( XSCH_ZOOM_IN, XschPromptZoomIn );

        break;

      case XSCH_KEY_CENTER :

         XschChangeEditMode( XSCH_ZOOM_CENTER, XschPromptZoomCenter );

        break;

      case XSCH_KEY_PAN :

         XschChangeEditMode( XSCH_ZOOM_PAN, XschPromptZoomPan );

        break;

      case XSCH_KEY_NEXT :

         XschFileNextSlide();

        break;

      case XSCH_KEY_PREV :

         XschFilePrevSlide();

        break;
    }

    if ( ( OldUnitCursorX != XschUnitCursorX ) ||
         ( OldUnitCursorY != XschUnitCursorY ) )
    {
      XschDisplayCoordinates(); 
      XschDisplayCursor();
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackResize                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackResize( MyWidget, ClientData, CallData )

     Widget                       MyWidget;
     XtPointer                    ClientData;
     XmDrawingAreaCallbackStruct *CallData;
{
  if ( XschFirstExpose == XSCH_FALSE )
  {
    autbegin();

    XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );
    XschUndisplayCursor();

    XschResizeGraphicWindow();
    XschRefreshGraphicWindow( 0, 0,
                               XschGraphicDx, XschGraphicDy );
    XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );
    XschDisplayCursor();

    autend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       CallbackExpose                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackExpose( MyWidget, ClientData, CallData )

     Widget                       MyWidget;
     XtPointer                    ClientData;
     XmDrawingAreaCallbackStruct *CallData;
{
  XExposeEvent *ExposeEvent;

  ExposeEvent = (XExposeEvent *)CallData->event;

  autbegin();

  if ( XschFirstExpose == XSCH_TRUE )
  {
    XschFirstExpose = XSCH_FALSE;

    XschInitializeGraphicWindow();
    XschUndisplayCursor();

    XschLoadConfig( XSCH_FALSE );

    if ( XschFirstFileName != (char *)NULL )
    {
      XschDisplayCursor();
      XschFileOpen( XschFirstFileName, 1 );

      XschFirstFileName = (char *)NULL;
    }
    else
    {
      XschChangeTopLevelTitle( (char *)NULL );

      XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );
      XschDisplayCursor();
    }

    /* XschEnterPresentPanel(); */
  }
  else
  {
    XschUndisplayCursor();
    XschRefreshGraphicWindow( ExposeEvent->x, ExposeEvent->y,
                               ExposeEvent->width, ExposeEvent->height );
    XschDisplayCursor();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschInitializeEvent                    |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XschGraphicWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XschEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackEvent";
  NewActions.proc   =  CallbackEvent;

  XtAppAddActions( XschApplication, &NewActions, 1 );

  XtAddCallback( XschGraphicWindow, 
                 XmNresizeCallback, 
                 CallbackResize, NULL );

  XtAddCallback( XschGraphicWindow, 
                 XmNexposeCallback, 
                 CallbackExpose, NULL );

  autend();
}
