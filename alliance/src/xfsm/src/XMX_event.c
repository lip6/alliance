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
| Tool    :                   XFSM                            |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
# include "XMX.h"
# include "XTB.h"
# include "XME.h"
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

  static String XfsmEventTranslation =

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
     <Key>P:        CallbackEvent( 16 )\n";


/*------------------------------------------------------------\
|                                                             |
|                         Event Count                         |
|                                                             |
\------------------------------------------------------------*/

  static char XfsmMaxEventEditTable[] =

  {
    2,  /* XFSM_EDIT_MEASURE   */
    1,  /* XFSM_EDIT_IDENTIFY  */
    1   /* XFSM_EDIT_CONNECTED */
  };

  static char XfsmInputEventEditTable[] =

  {
    XFSM_INPUT_HALF_BOX, /* XFSM_EDIT_MEASURE   */
    XFSM_INPUT_POINT,    /* XFSM_EDIT_IDENTIFY  */
    XFSM_INPUT_POINT     /* XFSM_EDIT_CONNECTED */
  };

  static char XfsmMaxEventZoomTable[] =

  {
    1,  /* XFSM_ZOOM_CENTER            */
    2,  /* XFSM_ZOOM_IN                */
    2   /* XFSM_ZOOM_PAN               */
  };

  static char XfsmInputEventZoomTable[] =

  {
    XFSM_INPUT_POINT,  /* XFSM_ZOOM_CENTER            */
    XFSM_INPUT_BOX,    /* XFSM_ZOOM_IN                */
    XFSM_INPUT_LINE    /* XFSM_ZOOM_PAN               */
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

  static long     XfsmUnitEventZoomX[ 2 ] = { 0, 0 };
  static long     XfsmUnitEventZoomY[ 2 ] = { 0, 0 };
  static char     XfsmMaxEventZoom          = 0;
         char     XfsmCountEventZoom        = 0;

  static long     XfsmUnitEventEditX[ 2 ] = { 0, 0 };
  static long     XfsmUnitEventEditY[ 2 ] = { 0, 0 };
  static char     XfsmMaxEventEdit          = 2;
         char     XfsmCountEventEdit        = 0;

  static char     XfsmFirstExpose           = XFSM_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                          Input                              |
|                                                             |
\------------------------------------------------------------*/

 static char XfsmInputMode     = XFSM_INPUT_HALF_BOX;
 static char XfsmSaveInputMode = XFSM_INPUT_HALF_BOX;
 static char XfsmEditMode      = XFSM_EDIT_MEASURE;
 static char XfsmSaveEditMode  = XFSM_EDIT_MEASURE;

/*------------------------------------------------------------\
|                                                             |
|                          Edit Message                       |
|                                                             |
\------------------------------------------------------------*/

  static void (*XfsmPromptMessage)()     = XfsmPromptEditMeasure;
  static void (*XfsmSavePromptMessage)() = XfsmPromptEditMeasure;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XfsmChangeEditMode                     |
|                                                             |
\------------------------------------------------------------*/
 
void XfsmChangeEditMode( EditMode , PromptMessage )
 
   char  EditMode;
   void (*PromptMessage)();
{
  autbegin();

  if ( EditMode & XFSM_ZOOM_MARK )
  {
    EditMode = EditMode & ~XFSM_ZOOM_MARK;

    XfsmCountEventZoom = 0;
    XfsmMaxEventZoom   = XfsmMaxEventZoomTable[ EditMode ];

    if ( ! ( XfsmEditMode & XFSM_ZOOM_MARK ) )
    {
      XfsmSaveEditMode      = XfsmEditMode;
      XfsmSaveInputMode     = XfsmInputMode;
      XfsmSavePromptMessage = XfsmPromptMessage;
    }

    XfsmInputMode     = XfsmInputEventZoomTable[ EditMode ];
    XfsmPromptMessage = PromptMessage;
    XfsmEditMode      = EditMode | XFSM_ZOOM_MARK;

    XfsmChangeCursorType( NULL, NULL, 0, XfsmInputMode );
  }  
  else
  {
    if ( XfsmEditMode & XFSM_ZOOM_MARK )
    {
      XfsmSaveEditMode      = EditMode;
      XfsmSaveInputMode     = XfsmInputEventEditTable[ EditMode ];
      XfsmSavePromptMessage = PromptMessage;
      XfsmCountEventEdit    = 0;
      XfsmMaxEventEdit      = XfsmMaxEventEditTable[ EditMode ];
    }
    else
    {
      if ( XfsmEditMode != EditMode )
      {
        XfsmCountEventEdit = 0;
        XfsmMaxEventEdit   = XfsmMaxEventEditTable[ EditMode ];
        XfsmEditMode       = EditMode;
        XfsmInputMode      = XfsmInputEventEditTable[ EditMode ];
        XfsmPromptMessage  = PromptMessage;

        XfsmChangeCursorType( NULL, NULL, 0, XfsmInputMode );
      }
    }

    XfsmEditUnselectAll();
  }

  XfsmPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XfsmContinueEditMode                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmContinueEditMode( EditMode , PromptMessage, CountEvent )

   char  EditMode;
   void (*PromptMessage)();
   char  CountEvent;
{
  autbegin();

  if ( XfsmEditMode & XFSM_ZOOM_MARK )
  {
    XfsmSaveEditMode      = EditMode;
    XfsmSaveInputMode     = XfsmInputEventEditTable[ EditMode ];
    XfsmSavePromptMessage = PromptMessage;
    XfsmCountEventEdit    = CountEvent;
    XfsmMaxEventEdit      = XfsmMaxEventEditTable[ EditMode ];
  }
  else
  {
    XfsmCountEventEdit = CountEvent;
    XfsmMaxEventEdit   = XfsmMaxEventEditTable[ EditMode ];
    XfsmEditMode       = EditMode;
    XfsmInputMode      = XfsmInputEventEditTable[ EditMode ];
    XfsmPromptMessage  = PromptMessage;

    XfsmChangeCursorType( XfsmUnitEventEditX,
                           XfsmUnitEventEditY,
                           XfsmCountEventEdit,
                           XfsmInputMode );

  }

  XfsmPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmTreatEventZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmTreatEventZoom()

{
  autbegin();

  XfsmChangeCursorType( XfsmUnitEventEditX,
                         XfsmUnitEventEditY,
                         XfsmCountEventEdit,
                         XfsmSaveInputMode );

  switch ( XfsmEditMode )
  {
    case XFSM_ZOOM_CENTER :
                              
      XfsmZoomCenter( XfsmUnitEventZoomX[ 0 ], 
                       XfsmUnitEventZoomY[ 0 ] );
      break;

    case XFSM_ZOOM_IN     :

      XfsmZoomIn( XfsmUnitEventZoomX[ 0 ],
                   XfsmUnitEventZoomY[ 0 ],
                   XfsmUnitEventZoomX[ 1 ],
                   XfsmUnitEventZoomY[ 1 ] );
      break;

    case XFSM_ZOOM_PAN    :

      XfsmZoomPan( XfsmUnitEventZoomX[ 0 ],
                    XfsmUnitEventZoomY[ 0 ],
                    XfsmUnitEventZoomX[ 1 ],
                    XfsmUnitEventZoomY[ 1 ] );
      break;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmTreatEventEdit                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmTreatEventEdit()

{ 
  long DeltaX;
  long DeltaY;
  long X1;
  long X2;
  long Y1;
  long Y2;

  autbegin();

  X1 = XfsmUnitEventEditX[0];
  X2 = XfsmUnitEventEditX[1];
  Y1 = XfsmUnitEventEditY[0];
  Y2 = XfsmUnitEventEditY[1];

  if ( ( XfsmInputMode == XFSM_INPUT_LSTRING ) || 
       ( XfsmInputMode == XFSM_INPUT_ORTHO   ) ||
       ( XfsmInputMode == XFSM_INPUT_SORTHO  ) )
  {
    DeltaX = X1 - X2;
    DeltaY = Y1 - Y2;

    if ( DeltaX < 0 ) DeltaX = - DeltaX;
    if ( DeltaY < 0 ) DeltaY = - DeltaY;

    if ( DeltaX > DeltaY )
    {
      Y2 = Y1;

      XfsmUnitEventEditX[0] = X2;
      XfsmUnitEventEditY[0] = Y2;
    }  
    else
    {   
      X2 = X1;

      XfsmUnitEventEditX[0] = X2;
      XfsmUnitEventEditY[0] = Y2;
    }
  }

  if ( XfsmInputMode == XFSM_INPUT_LSTRING )
  {   
    XfsmChangeCursorType( XfsmUnitEventEditX,
                           XfsmUnitEventEditY,
                           1, XfsmInputMode );
 
    XfsmCountEventEdit = 1;
  }
  else
  {
    XfsmChangeCursorType( NULL, NULL, 0,
                           XfsmInputMode );

    XfsmCountEventEdit = 0;
  }

  switch ( XfsmEditMode )
  {
    case XFSM_EDIT_MEASURE :

    break;

    case XFSM_EDIT_IDENTIFY :

     XfsmEditIdentify( X1, Y1 );

    break;

    case XFSM_EDIT_CONNECTED :

     XfsmEditConnected( X1, Y1 );

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

  if ( MouseEvent == XFSM_LEAVE )
  {
    XfsmUndisplayCursor();
    XfsmCursorInside = XFSM_FALSE;
  }
  else
  {
    OldUnitCursorX = XfsmUnitCursorX;
    OldUnitCursorY = XfsmUnitCursorY;

    XfsmComputeCursor( Event->x, Event->y );

    switch ( MouseEvent )
    {
      case XFSM_B1DN   : 

        if ( XfsmEditMode & XFSM_ZOOM_MARK )
        {
          if ( XfsmCountEventZoom < XfsmMaxEventZoom )
          {
            XfsmUnitEventZoomX[ XfsmCountEventZoom ] = XfsmUnitCursorX;
            XfsmUnitEventZoomY[ XfsmCountEventZoom ] = XfsmUnitCursorY;

            XfsmCountEventZoom = XfsmCountEventZoom + 1;

            XfsmPointCursor();
          
            if ( XfsmCountEventZoom == XfsmMaxEventZoom )
            {
              XfsmChangeCursorType( NULL, NULL, 0, XfsmInputMode );

              XfsmTreatEventZoom();

              XfsmInputMode      = XfsmSaveInputMode;
              XfsmEditMode       = XfsmSaveEditMode;
              XfsmPromptMessage  = XfsmSavePromptMessage;
              XfsmSaveEditMode   = XFSM_EDIT_MEASURE;
              XfsmSaveInputMode  = XFSM_INPUT_HALF_BOX;
              XfsmCountEventZoom = 0;
              XfsmMaxEventZoom   = 0;

              XfsmChangeCursorType( XfsmUnitEventEditX,
                                    XfsmUnitEventEditY,
                                    XfsmCountEventEdit,
                                    XfsmInputMode );

              XfsmComputeCursor( Event->x, Event->y );
              XfsmDisplayCursor();
            }
          }
        }
        else
        {
          if ( XfsmCountEventEdit < XfsmMaxEventEdit )
          {
            XfsmUnitEventEditX[ XfsmCountEventEdit ] = XfsmUnitCursorX;
            XfsmUnitEventEditY[ XfsmCountEventEdit ] = XfsmUnitCursorY;

            XfsmCountEventEdit = XfsmCountEventEdit + 1;

            XfsmPointCursor();

            if ( XfsmCountEventEdit == XfsmMaxEventEdit )
            {
              XfsmTreatEventEdit();

              XfsmDisplayCursor();
            }
          }
        }

        XfsmPromptMessage();

        break;

      case XFSM_B3DN   :

        if ( XfsmEditMode & XFSM_ZOOM_MARK )
        {
          if ( XfsmCountEventZoom != 0 )
          {
            XfsmCountEventZoom = XfsmCountEventZoom - 1;

            XfsmResetCursor();
          }
          else
          { 
            XfsmChangeCursorType( XfsmUnitEventEditX,
                                  XfsmUnitEventEditY,
                                  XfsmCountEventEdit,
                                  XfsmSaveInputMode );

            XfsmInputMode          = XfsmSaveInputMode;
            XfsmEditMode           = XfsmSaveEditMode;
            XfsmPromptMessage      = XfsmSavePromptMessage;
            XfsmSaveEditMode       = XFSM_EDIT_MEASURE;
            XfsmSaveInputMode      = XFSM_INPUT_HALF_BOX;
            XfsmSavePromptMessage  = XfsmPromptEditMeasure;
            XfsmCountEventZoom     = 0;
            XfsmMaxEventZoom       = 0;
          }
        }
        else
        {
          if ( XfsmCountEventEdit != 0 )
          {
            XfsmCountEventEdit = XfsmCountEventEdit - 1;

            XfsmResetCursor();
          }
          else
          {
            XfsmEditUnselectAll();

            XfsmChangeCursorType( NULL, NULL, 0,
                                   XFSM_INPUT_HALF_BOX );

            XfsmInputMode      = XFSM_INPUT_HALF_BOX;
            XfsmEditMode       = XFSM_EDIT_MEASURE;
            XfsmPromptMessage  = XfsmPromptEditMeasure;
            XfsmCountEventEdit = 0;
            XfsmMaxEventEdit   = 2;
          }
        }

        XfsmPromptMessage();

        break; 

      case XFSM_MOTION :

        break;

      case XFSM_ENTER  : XfsmCursorInside = XFSM_TRUE;
                          XfsmDisplayCursor();
                          break;

      case XFSM_KEY_UP    : XfsmZoomUp();

        break;

      case XFSM_KEY_DN : XfsmZoomDown();

        break;

      case XFSM_KEY_LEFT : XfsmZoomLeft();

        break;

      case XFSM_KEY_RIGHT : XfsmZoomRight();

        break;

      case XFSM_KEY_MORE : XfsmZoomMore();

        break;

      case XFSM_KEY_LESS : XfsmZoomLess();

        break;

      case XFSM_KEY_FIT : XfsmZoomFit();

        break;

      case XFSM_KEY_REFRESH : XfsmZoomRefresh();

        break;

      case XFSM_KEY_UNZOOM : XfsmZoomUndo();

        break;

      case XFSM_KEY_ZOOM_IN : 

         XfsmChangeEditMode( XFSM_ZOOM_IN, XfsmPromptZoomIn );

        break;

      case XFSM_KEY_CENTER :

         XfsmChangeEditMode( XFSM_ZOOM_CENTER, XfsmPromptZoomCenter );

        break;

      case XFSM_KEY_PAN :

         XfsmChangeEditMode( XFSM_ZOOM_PAN, XfsmPromptZoomPan );

        break;
    }

    if ( ( OldUnitCursorX != XfsmUnitCursorX ) ||
         ( OldUnitCursorY != XfsmUnitCursorY ) )
    {
      XfsmDisplayCoordinates(); 
      XfsmDisplayCursor();
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
  if ( XfsmFirstExpose == XFSM_FALSE )
  {
    autbegin();

    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );
    XfsmUndisplayCursor();

    XfsmResizeGraphicWindow();
    XfsmRefreshGraphicWindow( 0, 0,
                               XfsmGraphicDx, XfsmGraphicDy );
    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );
    XfsmDisplayCursor();

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

  if ( XfsmFirstExpose == XFSM_TRUE )
  {
    XfsmFirstExpose = XFSM_FALSE;

    XfsmInitializeGraphicWindow();
    XfsmUndisplayCursor();

    XfsmLoadConfig( XFSM_FALSE );

    if ( XfsmFirstFileName != (char *)NULL )
    {
      XfsmDisplayCursor();
      XfsmFileOpen( XfsmFirstFileName );

      XfsmFirstFileName = (char *)NULL;
    }
    else
    {
      XfsmChangeTopLevelTitle( (char *)NULL );

      XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
      XfsmDisplayCursor();
    }

    /* XfsmEnterPresentPanel(); */
  }
  else
  {
    XfsmUndisplayCursor();
    XfsmRefreshGraphicWindow( ExposeEvent->x, ExposeEvent->y,
                               ExposeEvent->width, ExposeEvent->height );
    XfsmDisplayCursor();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmInitializeEvent                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XfsmGraphicWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XfsmEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackEvent";
  NewActions.proc   =  CallbackEvent;

  XtAppAddActions( XfsmApplication, &NewActions, 1 );

  XtAddCallback( XfsmGraphicWindow, 
                 XmNresizeCallback, 
                 CallbackResize, NULL );

  XtAddCallback( XfsmGraphicWindow, 
                 XmNexposeCallback, 
                 CallbackExpose, NULL );

  autend();
}
