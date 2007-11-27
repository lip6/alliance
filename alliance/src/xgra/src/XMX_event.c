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
| File    :                   Event.c                         |
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
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <Xm/Xm.h>

 
# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XMX.h"
# include "XTB.h"
# include "XME.h"
# include "XMV.h"
# include "XMF.h"
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
|                          Arclation                        |
|                                                             |
\------------------------------------------------------------*/

  static String XgraEventArclation =

    "<Btn1Down>:    CallbackEvent(  0 )\n\
     <Btn3Down>:    CallbackEvent(  1 )\n\
     <Motion>:      CallbackEvent(  2 )\n\
     <EnterWindow>: CallbackEvent(  3 )\n\
     <LeaveWindow>: CallbackEvent(  4 )\n\
     <Key>osfUp:    CallbackEvent(  5 )\n\
     <Key>osfDown:  CallbackEvent(  6 )\n\
     <Key>osfLeft:  CallbackEvent(  7 )\n\
     <Key>osfRight: CallbackEvent(  8 )\n\
     <Key>Z:        CallbackEvent(  9 )\n\
     <Key>M:        CallbackEvent( 10 )\n\
     <Key>F:        CallbackEvent( 11 )\n\
     <Key>R:        CallbackEvent( 12 )\n\
     <Key>U:        CallbackEvent( 13 )\n\
     <Key>I:        CallbackEvent( 14 )\n\
     <Key>C:        CallbackEvent( 15 )\n\
     <Key>P:        CallbackEvent( 16 )\n";


/*------------------------------------------------------------\
|                                                             |
|                         Event Count                         |
|                                                             |
\------------------------------------------------------------*/

  static char XgraMaxEventEditTable[] =

  {
    2,  /* XGRA_EDIT_MEASURE   */
    1,  /* XGRA_EDIT_IDENTIFY  */
    1   /* XGRA_EDIT_CONNECTED */
  };

  static char XgraInputEventEditTable[] =

  {
    XGRA_INPUT_HALF_BOX, /* XGRA_EDIT_MEASURE   */
    XGRA_INPUT_POINT,    /* XGRA_EDIT_IDENTIFY  */
    XGRA_INPUT_POINT     /* XGRA_EDIT_CONNECTED */
  };

  static char XgraMaxEventZoomTable[] =

  {
    1,  /* XGRA_ZOOM_CENTER            */
    2,  /* XGRA_ZOOM_IN                */
    2   /* XGRA_ZOOM_PAN               */
  };

  static char XgraInputEventZoomTable[] =

  {
    XGRA_INPUT_POINT,  /* XGRA_ZOOM_CENTER            */
    XGRA_INPUT_BOX,    /* XGRA_ZOOM_IN                */
    XGRA_INPUT_LINE    /* XGRA_ZOOM_PAN               */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Arclation                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                      Expose Edit Zoom                       |
|                                                             |
\------------------------------------------------------------*/

  static long     XgraUnitEventZoomX[ 2 ] = { 0, 0 };
  static long     XgraUnitEventZoomY[ 2 ] = { 0, 0 };
  static char     XgraMaxEventZoom          = 0;
         char     XgraCountEventZoom        = 0;

  static long     XgraUnitEventEditX[ 2 ] = { 0, 0 };
  static long     XgraUnitEventEditY[ 2 ] = { 0, 0 };
  static char     XgraMaxEventEdit          = 2;
         char     XgraCountEventEdit        = 0;

  static char     XgraFirstExpose           = XGRA_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                          Input                              |
|                                                             |
\------------------------------------------------------------*/

 static char XgraInputMode     = XGRA_INPUT_HALF_BOX;
 static char XgraSaveInputMode = XGRA_INPUT_HALF_BOX;
 static char XgraEditMode      = XGRA_EDIT_MEASURE;
 static char XgraSaveEditMode  = XGRA_EDIT_MEASURE;

/*------------------------------------------------------------\
|                                                             |
|                          Edit Message                       |
|                                                             |
\------------------------------------------------------------*/

  static void (*XgraPromptMessage)()     = XgraPromptEditMeasure;
  static void (*XgraSavePromptMessage)() = XgraPromptEditMeasure;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XgraChangeEditMode                     |
|                                                             |
\------------------------------------------------------------*/
 
void XgraChangeEditMode( EditMode , PromptMessage )
 
   char  EditMode;
   void (*PromptMessage)();
{
  autbegin();

  if ( EditMode & XGRA_ZOOM_MARK )
  {
    EditMode = EditMode & ~XGRA_ZOOM_MARK;

    XgraCountEventZoom = 0;
    XgraMaxEventZoom   = XgraMaxEventZoomTable[ EditMode ];

    if ( ! ( XgraEditMode & XGRA_ZOOM_MARK ) )
    {
      XgraSaveEditMode      = XgraEditMode;
      XgraSaveInputMode     = XgraInputMode;
      XgraSavePromptMessage = XgraPromptMessage;
    }

    XgraInputMode     = XgraInputEventZoomTable[ EditMode ];
    XgraPromptMessage = PromptMessage;
    XgraEditMode      = EditMode | XGRA_ZOOM_MARK;

    XgraChangeCursorType( NULL, NULL, 0, XgraInputMode );
  }  
  else
  {
    if ( XgraEditMode & XGRA_ZOOM_MARK )
    {
      XgraSaveEditMode      = EditMode;
      XgraSaveInputMode     = XgraInputEventEditTable[ EditMode ];
      XgraSavePromptMessage = PromptMessage;
      XgraCountEventEdit    = 0;
      XgraMaxEventEdit      = XgraMaxEventEditTable[ EditMode ];
    }
    else
    {
      if ( XgraEditMode != EditMode )
      {
        XgraCountEventEdit = 0;
        XgraMaxEventEdit   = XgraMaxEventEditTable[ EditMode ];
        XgraEditMode       = EditMode;
        XgraInputMode      = XgraInputEventEditTable[ EditMode ];
        XgraPromptMessage  = PromptMessage;

        XgraChangeCursorType( NULL, NULL, 0, XgraInputMode );
      }
    }

    XgraEditUnselectAll();
  }

  XgraPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XgraContinueEditMode                     |
|                                                             |
\------------------------------------------------------------*/

void XgraContinueEditMode( EditMode , PromptMessage, CountEvent )

   char  EditMode;
   void (*PromptMessage)();
   char  CountEvent;
{
  autbegin();

  if ( XgraEditMode & XGRA_ZOOM_MARK )
  {
    XgraSaveEditMode      = EditMode;
    XgraSaveInputMode     = XgraInputEventEditTable[ EditMode ];
    XgraSavePromptMessage = PromptMessage;
    XgraCountEventEdit    = CountEvent;
    XgraMaxEventEdit      = XgraMaxEventEditTable[ EditMode ];
  }
  else
  {
    XgraCountEventEdit = CountEvent;
    XgraMaxEventEdit   = XgraMaxEventEditTable[ EditMode ];
    XgraEditMode       = EditMode;
    XgraInputMode      = XgraInputEventEditTable[ EditMode ];
    XgraPromptMessage  = PromptMessage;

    XgraChangeCursorType( XgraUnitEventEditX,
                           XgraUnitEventEditY,
                           XgraCountEventEdit,
                           XgraInputMode );

  }

  XgraPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraTreatEventZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XgraTreatEventZoom()

{
  autbegin();

  XgraChangeCursorType( XgraUnitEventEditX,
                         XgraUnitEventEditY,
                         XgraCountEventEdit,
                         XgraSaveInputMode );

  switch ( XgraEditMode )
  {
    case XGRA_ZOOM_CENTER :
                              
      XgraZoomCenter( XgraUnitEventZoomX[ 0 ], 
                       XgraUnitEventZoomY[ 0 ] );
      break;

    case XGRA_ZOOM_IN     :

      XgraZoomIn( XgraUnitEventZoomX[ 0 ],
                   XgraUnitEventZoomY[ 0 ],
                   XgraUnitEventZoomX[ 1 ],
                   XgraUnitEventZoomY[ 1 ] );
      break;

    case XGRA_ZOOM_PAN    :

      XgraZoomPan( XgraUnitEventZoomX[ 0 ],
                    XgraUnitEventZoomY[ 0 ],
                    XgraUnitEventZoomX[ 1 ],
                    XgraUnitEventZoomY[ 1 ] );
      break;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraTreatEventEdit                    |
|                                                             |
\------------------------------------------------------------*/

void XgraTreatEventEdit()

{ 
  long DeltaX;
  long DeltaY;
  long X1;
  long X2;
  long Y1;
  long Y2;

  autbegin();

  X1 = XgraUnitEventEditX[0];
  X2 = XgraUnitEventEditX[1];
  Y1 = XgraUnitEventEditY[0];
  Y2 = XgraUnitEventEditY[1];

  if ( ( XgraInputMode == XGRA_INPUT_LSTRING ) || 
       ( XgraInputMode == XGRA_INPUT_ORTHO   ) ||
       ( XgraInputMode == XGRA_INPUT_SORTHO  ) )
  {
    DeltaX = X1 - X2;
    DeltaY = Y1 - Y2;

    if ( DeltaX < 0 ) DeltaX = - DeltaX;
    if ( DeltaY < 0 ) DeltaY = - DeltaY;

    if ( DeltaX > DeltaY )
    {
      Y2 = Y1;

      XgraUnitEventEditX[0] = X2;
      XgraUnitEventEditY[0] = Y2;
    }  
    else
    {   
      X2 = X1;

      XgraUnitEventEditX[0] = X2;
      XgraUnitEventEditY[0] = Y2;
    }
  }

  if ( XgraInputMode == XGRA_INPUT_LSTRING )
  {   
    XgraChangeCursorType( XgraUnitEventEditX,
                           XgraUnitEventEditY,
                           1, XgraInputMode );
 
    XgraCountEventEdit = 1;
  }
  else
  {
    XgraChangeCursorType( NULL, NULL, 0,
                           XgraInputMode );

    XgraCountEventEdit = 0;
  }

  switch ( XgraEditMode )
  {
    case XGRA_EDIT_MEASURE :

    break;

    case XGRA_EDIT_IDENTIFY :

     XgraEditIdentify( X1, Y1 );

    break;

    case XGRA_EDIT_CONNECTED :

     XgraEditConnected( X1, Y1 );

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
  char      MouseEvent;
  Position  OldUnitCursorX;
  Position  OldUnitCursorY;

  autbegin();

  EventDisplay = Event->display;

  MouseEvent = atoi( Args[ 0 ] );

  if ( MouseEvent == XGRA_LEAVE )
  {
    XgraUndisplayCursor();
    XgraCursorInside = XGRA_FALSE;
  }
  else
  {
    OldUnitCursorX = XgraUnitCursorX;
    OldUnitCursorY = XgraUnitCursorY;

    XgraComputeCursor( Event->x, Event->y );

    switch ( MouseEvent )
    {
      case XGRA_B1DN   : 

        if ( XgraEditMode & XGRA_ZOOM_MARK )
        {
          if ( XgraCountEventZoom < XgraMaxEventZoom )
          {
            XgraUnitEventZoomX[ XgraCountEventZoom ] = XgraUnitCursorX;
            XgraUnitEventZoomY[ XgraCountEventZoom ] = XgraUnitCursorY;

            XgraCountEventZoom = XgraCountEventZoom + 1;

            XgraPointCursor();
          
            if ( XgraCountEventZoom == XgraMaxEventZoom )
            {
              XgraChangeCursorType( NULL, NULL, 0, XgraInputMode );

              XgraTreatEventZoom();

              XgraInputMode      = XgraSaveInputMode;
              XgraEditMode       = XgraSaveEditMode;
              XgraPromptMessage  = XgraSavePromptMessage;
              XgraSaveEditMode   = XGRA_EDIT_MEASURE;
              XgraSaveInputMode  = XGRA_INPUT_HALF_BOX;
              XgraCountEventZoom = 0;
              XgraMaxEventZoom   = 0;

              XgraChangeCursorType( XgraUnitEventEditX,
                                    XgraUnitEventEditY,
                                    XgraCountEventEdit,
                                    XgraInputMode );

              XgraComputeCursor( Event->x, Event->y );
              XgraDisplayCursor();
            }
          }
        }
        else
        {
          if ( XgraCountEventEdit < XgraMaxEventEdit )
          {
            XgraUnitEventEditX[ XgraCountEventEdit ] = XgraUnitCursorX;
            XgraUnitEventEditY[ XgraCountEventEdit ] = XgraUnitCursorY;

            XgraCountEventEdit = XgraCountEventEdit + 1;

            XgraPointCursor();

            if ( XgraCountEventEdit == XgraMaxEventEdit )
            {
              XgraTreatEventEdit();

              XgraDisplayCursor();
            }
          }
        }

        XgraPromptMessage();

        break;

      case XGRA_B3DN   :

        if ( XgraEditMode & XGRA_ZOOM_MARK )
        {
          if ( XgraCountEventZoom != 0 )
          {
            XgraCountEventZoom = XgraCountEventZoom - 1;

            XgraResetCursor();
          }
          else
          { 
            XgraChangeCursorType( XgraUnitEventEditX,
                                  XgraUnitEventEditY,
                                  XgraCountEventEdit,
                                  XgraSaveInputMode );

            XgraInputMode          = XgraSaveInputMode;
            XgraEditMode           = XgraSaveEditMode;
            XgraPromptMessage      = XgraSavePromptMessage;
            XgraSaveEditMode       = XGRA_EDIT_MEASURE;
            XgraSaveInputMode      = XGRA_INPUT_HALF_BOX;
            XgraSavePromptMessage  = XgraPromptEditMeasure;
            XgraCountEventZoom     = 0;
            XgraMaxEventZoom       = 0;
          }
        }
        else
        {
          if ( XgraCountEventEdit != 0 )
          {
            XgraCountEventEdit = XgraCountEventEdit - 1;

            XgraResetCursor();
          }
          else
          {
            XgraEditUnselectAll();

            XgraChangeCursorType( NULL, NULL, 0,
                                   XGRA_INPUT_HALF_BOX );

            XgraInputMode      = XGRA_INPUT_HALF_BOX;
            XgraEditMode       = XGRA_EDIT_MEASURE;
            XgraPromptMessage  = XgraPromptEditMeasure;
            XgraCountEventEdit = 0;
            XgraMaxEventEdit   = 2;
          }
        }

        XgraPromptMessage();

        break; 

      case XGRA_MOTION :

        break;

      case XGRA_ENTER  : XgraCursorInside = XGRA_TRUE;
                          XgraDisplayCursor();
                          break;

      case XGRA_KEY_UP    : XgraZoomUp();

        break;

      case XGRA_KEY_DN : XgraZoomDown();

        break;

      case XGRA_KEY_LEFT : XgraZoomLeft();

        break;

      case XGRA_KEY_RIGHT : XgraZoomRight();

        break;

      case XGRA_KEY_MORE : XgraZoomMore();

        break;

      case XGRA_KEY_LESS : XgraZoomLess();

        break;

      case XGRA_KEY_FIT : XgraZoomFit();

        break;

      case XGRA_KEY_REFRESH : XgraZoomRefresh();

        break;

      case XGRA_KEY_UNZOOM : XgraZoomUndo();

        break;

      case XGRA_KEY_ZOOM_IN : 

         XgraChangeEditMode( XGRA_ZOOM_IN, XgraPromptZoomIn );

        break;

      case XGRA_KEY_CENTER :

         XgraChangeEditMode( XGRA_ZOOM_CENTER, XgraPromptZoomCenter );

        break;

      case XGRA_KEY_PAN :

         XgraChangeEditMode( XGRA_ZOOM_PAN, XgraPromptZoomPan );

        break;
    }

    if ( ( OldUnitCursorX != XgraUnitCursorX ) ||
         ( OldUnitCursorY != XgraUnitCursorY ) )
    {
      XgraDisplayCoordinates(); 
      XgraDisplayCursor();
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
  if ( XgraFirstExpose == XGRA_FALSE )
  {
    autbegin();

    XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );
    XgraUndisplayCursor();

    XgraResizeGraphicWindow();
    XgraRefreshGraphicWindow( 0, 0,
                               XgraGraphicDx, XgraGraphicDy );
    XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );
    XgraDisplayCursor();

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

  if ( XgraFirstExpose == XGRA_TRUE )
  {
    XgraFirstExpose = XGRA_FALSE;

    XgraInitializeGraphicWindow();
    XgraUndisplayCursor();

    XgraLoadConfig( XGRA_FALSE );

    if ( XgraFirstFileName != (char *)NULL )
    {
      XgraDisplayCursor();
      XgraFileOpen( XgraFirstFileName );

      XgraFirstFileName = (char *)NULL;
    }
    else
    {
      XgraChangeTopLevelTitle( (char *)NULL );

      XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );
      XgraDisplayCursor();
    }

    XgraEnterPresentPanel();
  }
  else
  {
    XgraUndisplayCursor();
    XgraRefreshGraphicWindow( ExposeEvent->x, ExposeEvent->y,
                               ExposeEvent->width, ExposeEvent->height );
    XgraDisplayCursor();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraInitializeEvent                    |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XgraGraphicWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XgraEventArclation ),
                 NULL
               );

  NewActions.string = "CallbackEvent";
  NewActions.proc   =  CallbackEvent;

  XtAppAddActions( XgraApplication, &NewActions, 1 );

  XtAddCallback( XgraGraphicWindow, 
                 XmNresizeCallback, 
                 CallbackResize, NULL );

  XtAddCallback( XgraGraphicWindow, 
                 XmNexposeCallback, 
                 CallbackExpose, NULL );

  autend();
}
