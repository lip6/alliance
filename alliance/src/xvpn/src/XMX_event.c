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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XVPN                            |
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
# include <stdlib.h>
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
# include "XMX_graphic.h"

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

  static String XvpnEventTranslation =

    "<Btn1Down>:      CallbackEvent( 0 )\n\
     <Btn3Down>:      CallbackEvent( 1 )\n\
     <Motion>:        CallbackEvent( 2 )\n\
     <EnterWindow>:   CallbackEvent( 3 )\n\
     <LeaveWindow>:   CallbackEvent( 4 )\n\
     <Key>osfUp:      CallbackEvent( 5 )\n\
     <Btn4Down>:      CallbackEvent( 5 )\n\
     <Key>osfDown:    CallbackEvent( 6 )\n\
     <Btn5Down>:      CallbackEvent( 6 )\n\
     <Key>osfLeft:    CallbackEvent( 7 )\n\
     <Key>osfRight:   CallbackEvent( 8 )\n\
     <Key>Z:          CallbackEvent( 9 )\n\
     <Key>M:          CallbackEvent( 10 )\n\
     <Key>F:          CallbackEvent( 11 )\n\
     <Key>R:          CallbackEvent( 12 )\n\
     <Key>U:          CallbackEvent( 13 )\n\
     <Key>I:          CallbackEvent( 14 )\n\
     <Key>C:          CallbackEvent( 15 )\n\
     <Key>P:          CallbackEvent( 16 )\n\
     <Key>+:          CallbackEvent( 17 )\n\
     <Key>-:          CallbackEvent( 18 )\n";

/*------------------------------------------------------------\
|                                                             |
|                         Event Count                         |
|                                                             |
\------------------------------------------------------------*/

  static short XvpnMaxEventEditTable[] =

  {
    2,  /* XVPN_EDIT_MEASURE   */
    1,  /* XVPN_EDIT_IDENTIFY  */
    1   /* XVPN_EDIT_CONNECTED */
  };

  static short XvpnInputEventEditTable[] =

  {
    XVPN_INPUT_HALF_BOX, /* XVPN_EDIT_MEASURE   */
    XVPN_INPUT_POINT,    /* XVPN_EDIT_IDENTIFY  */
    XVPN_INPUT_POINT     /* XVPN_EDIT_CONNECTED */
  };

  static short XvpnMaxEventZoomTable[] =

  {
    1,  /* XVPN_ZOOM_CENTER            */
    2,  /* XVPN_ZOOM_IN                */
    2   /* XVPN_ZOOM_PAN               */
  };

  static short XvpnInputEventZoomTable[] =

  {
    XVPN_INPUT_POINT,  /* XVPN_ZOOM_CENTER            */
    XVPN_INPUT_BOX,    /* XVPN_ZOOM_IN                */
    XVPN_INPUT_LINE    /* XVPN_ZOOM_PAN               */
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

  static long     XvpnUnitEventZoomX[ 2 ] = { 0, 0 };
  static long     XvpnUnitEventZoomY[ 2 ] = { 0, 0 };
  static short    XvpnMaxEventZoom          = 0;
         short    XvpnCountEventZoom        = 0;

  static long     XvpnUnitEventEditX[ 2 ] = { 0, 0 };
  static long     XvpnUnitEventEditY[ 2 ] = { 0, 0 };
  static short    XvpnMaxEventEdit          = 1;
         short    XvpnCountEventEdit        = 0;

  static short    XvpnFirstExpose           = XVPN_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                          Input                              |
|                                                             |
\------------------------------------------------------------*/

 static short XvpnInputMode     = XVPN_INPUT_POINT;
 static short XvpnSaveInputMode = XVPN_INPUT_POINT;
 static short XvpnEditMode      = XVPN_EDIT_IDENTIFY;
 static short XvpnSaveEditMode  = XVPN_EDIT_IDENTIFY;

/*------------------------------------------------------------\
|                                                             |
|                          Edit Message                       |
|                                                             |
\------------------------------------------------------------*/

  static void (*XvpnPromptMessage)()     = XvpnPromptEditIdentify;
  static void (*XvpnSavePromptMessage)() = XvpnPromptEditIdentify;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnChangeEditMode                     |
|                                                             |
\------------------------------------------------------------*/
 
void XvpnChangeEditMode( EditMode , PromptMessage )
 
   short EditMode;
   void (*PromptMessage)();
{
  autbegin();

  if ( EditMode & XVPN_ZOOM_MARK )
  {
    EditMode = EditMode & ~XVPN_ZOOM_MARK;

    XvpnCountEventZoom = 0;
    XvpnMaxEventZoom   = XvpnMaxEventZoomTable[ EditMode ];

    if ( ! ( XvpnEditMode & XVPN_ZOOM_MARK ) )
    {
      XvpnSaveEditMode      = XvpnEditMode;
      XvpnSaveInputMode     = XvpnInputMode;
      XvpnSavePromptMessage = XvpnPromptMessage;
    }

    XvpnInputMode     = XvpnInputEventZoomTable[ EditMode ];
    XvpnPromptMessage = PromptMessage;
    XvpnEditMode      = EditMode | XVPN_ZOOM_MARK;

    XvpnChangeCursorType( NULL, NULL, 0, XvpnInputMode );
  }  
  else
  {
    if ( XvpnEditMode & XVPN_ZOOM_MARK )
    {
      XvpnSaveEditMode      = EditMode;
      XvpnSaveInputMode     = XvpnInputEventEditTable[ EditMode ];
      XvpnSavePromptMessage = PromptMessage;
      XvpnCountEventEdit    = 0;
      XvpnMaxEventEdit      = XvpnMaxEventEditTable[ EditMode ];
    }
    else
    {
      if ( XvpnEditMode != EditMode )
      {
        XvpnCountEventEdit = 0;
        XvpnMaxEventEdit   = XvpnMaxEventEditTable[ EditMode ];
        XvpnEditMode       = EditMode;
        XvpnInputMode      = XvpnInputEventEditTable[ EditMode ];
        XvpnPromptMessage  = PromptMessage;

        XvpnChangeCursorType( NULL, NULL, 0, XvpnInputMode );
      }
    }

    XvpnEditUnselectAll();
  }

  XvpnPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XvpnContinueEditMode                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnContinueEditMode( EditMode , PromptMessage, CountEvent )

   short EditMode;
   void (*PromptMessage)();
   short CountEvent;
{
  autbegin();

  if ( XvpnEditMode & XVPN_ZOOM_MARK )
  {
    XvpnSaveEditMode      = EditMode;
    XvpnSaveInputMode     = XvpnInputEventEditTable[ EditMode ];
    XvpnSavePromptMessage = PromptMessage;
    XvpnCountEventEdit    = CountEvent;
    XvpnMaxEventEdit      = XvpnMaxEventEditTable[ EditMode ];
  }
  else
  {
    XvpnCountEventEdit = CountEvent;
    XvpnMaxEventEdit   = XvpnMaxEventEditTable[ EditMode ];
    XvpnEditMode       = EditMode;
    XvpnInputMode      = XvpnInputEventEditTable[ EditMode ];
    XvpnPromptMessage  = PromptMessage;

    XvpnChangeCursorType( XvpnUnitEventEditX,
                           XvpnUnitEventEditY,
                           XvpnCountEventEdit,
                           XvpnInputMode );

  }

  XvpnPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnTreatEventZoom                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnTreatEventZoom()

{
  autbegin();

  XvpnChangeCursorType( XvpnUnitEventEditX,
                         XvpnUnitEventEditY,
                         XvpnCountEventEdit,
                         XvpnSaveInputMode );

  switch ( XvpnEditMode )
  {
    case XVPN_ZOOM_CENTER :
                              
      XvpnZoomCenter( XvpnUnitEventZoomX[ 0 ], 
                       XvpnUnitEventZoomY[ 0 ] );
      break;

    case XVPN_ZOOM_IN     :

      XvpnZoomIn( XvpnUnitEventZoomX[ 0 ],
                   XvpnUnitEventZoomY[ 0 ],
                   XvpnUnitEventZoomX[ 1 ],
                   XvpnUnitEventZoomY[ 1 ] );
      break;

    case XVPN_ZOOM_PAN    :

      XvpnZoomPan( XvpnUnitEventZoomX[ 0 ],
                    XvpnUnitEventZoomY[ 0 ],
                    XvpnUnitEventZoomX[ 1 ],
                    XvpnUnitEventZoomY[ 1 ] );
      break;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnTreatEventEdit                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnTreatEventEdit()

{ 
  long DeltaX;
  long DeltaY;
  long X1;
  long X2;
  long Y1;
  long Y2;

  autbegin();

  X1 = XvpnUnitEventEditX[0];
  X2 = XvpnUnitEventEditX[1];
  Y1 = XvpnUnitEventEditY[0];
  Y2 = XvpnUnitEventEditY[1];

  if ( ( XvpnInputMode == XVPN_INPUT_LSTRING ) || 
       ( XvpnInputMode == XVPN_INPUT_ORTHO   ) ||
       ( XvpnInputMode == XVPN_INPUT_SORTHO  ) )
  {
    DeltaX = X1 - X2;
    DeltaY = Y1 - Y2;

    if ( DeltaX < 0 ) DeltaX = - DeltaX;
    if ( DeltaY < 0 ) DeltaY = - DeltaY;

    if ( DeltaX > DeltaY )
    {
      Y2 = Y1;

      XvpnUnitEventEditX[0] = X2;
      XvpnUnitEventEditY[0] = Y2;
    }  
    else
    {   
      X2 = X1;

      XvpnUnitEventEditX[0] = X2;
      XvpnUnitEventEditY[0] = Y2;
    }
  }

  if ( XvpnInputMode == XVPN_INPUT_LSTRING )
  {   
    XvpnChangeCursorType( XvpnUnitEventEditX,
                           XvpnUnitEventEditY,
                           1, XvpnInputMode );
 
    XvpnCountEventEdit = 1;
  }
  else
  {
    XvpnChangeCursorType( NULL, NULL, 0,
                           XvpnInputMode );

    XvpnCountEventEdit = 0;
  }

  switch ( XvpnEditMode )
  {
    case XVPN_EDIT_MEASURE :

    break;

    case XVPN_EDIT_IDENTIFY :

     XvpnEditIdentify( X1, Y1 );

    break;

    case XVPN_EDIT_CONNECTED :

     XvpnEditConnected( X1, Y1 );

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
  short     MouseEvent;
  Position  OldUnitCursorX;
  Position  OldUnitCursorY;

  autbegin();

  EventDisplay = Event->display;

  MouseEvent = atoi( Args[ 0 ] );

  if ( MouseEvent == XVPN_LEAVE )
  {
    XvpnUndisplayCursor();
    XvpnCursorInside = XVPN_FALSE;
  }
  else
  {
    OldUnitCursorX = XvpnUnitCursorX;
    OldUnitCursorY = XvpnUnitCursorY;

    XvpnComputeCursor( Event->x, Event->y );

    switch ( MouseEvent )
    {
      case XVPN_B1DN   : 

        if ( XvpnEditMode & XVPN_ZOOM_MARK )
        {
          if ( XvpnCountEventZoom < XvpnMaxEventZoom )
          {
            XvpnUnitEventZoomX[ XvpnCountEventZoom ] = XvpnUnitCursorX;
            XvpnUnitEventZoomY[ XvpnCountEventZoom ] = XvpnUnitCursorY;

            XvpnCountEventZoom = XvpnCountEventZoom + 1;

            XvpnPointCursor();
          
            if ( XvpnCountEventZoom == XvpnMaxEventZoom )
            {
              XvpnTreatEventZoom();

              XvpnInputMode      = XvpnSaveInputMode;
              XvpnEditMode       = XvpnSaveEditMode;
              XvpnPromptMessage  = XvpnSavePromptMessage;
              XvpnSaveEditMode   = XVPN_EDIT_IDENTIFY;
              XvpnSaveInputMode  = XVPN_INPUT_POINT;
              XvpnCountEventZoom = 0;
              XvpnMaxEventZoom   = 0;

              XvpnComputeCursor( Event->x, Event->y );
              XvpnDisplayCursor();
            }
          }
        }
        else
        {
          if ( XvpnCountEventEdit < XvpnMaxEventEdit )
          {
            XvpnUnitEventEditX[ XvpnCountEventEdit ] = XvpnUnitCursorX;
            XvpnUnitEventEditY[ XvpnCountEventEdit ] = XvpnUnitCursorY;

            XvpnCountEventEdit = XvpnCountEventEdit + 1;

            XvpnPointCursor();

            if ( XvpnCountEventEdit == XvpnMaxEventEdit )
            {
              XvpnTreatEventEdit();

              XvpnDisplayCursor();
            }
          }
        }

        XvpnPromptMessage();

        break;

      case XVPN_B3DN   :

        if ( XvpnEditMode & XVPN_ZOOM_MARK )
        {
          if ( XvpnCountEventZoom != 0 )
          {
            XvpnCountEventZoom = XvpnCountEventZoom - 1;

            XvpnResetCursor();
          }
          else
          { 
            XvpnChangeCursorType( XvpnUnitEventEditX,
                                  XvpnUnitEventEditY,
                                  XvpnCountEventEdit,
                                  XvpnSaveInputMode );

            XvpnInputMode          = XvpnSaveInputMode;
            XvpnEditMode           = XvpnSaveEditMode;
            XvpnPromptMessage      = XvpnSavePromptMessage;
            XvpnSaveEditMode       = XVPN_EDIT_IDENTIFY;
            XvpnSaveInputMode      = XVPN_INPUT_POINT;
            XvpnSavePromptMessage  = XvpnPromptEditIdentify;
            XvpnCountEventZoom     = 0;
            XvpnMaxEventZoom       = 0;
          }
        }
        else
        {
          if ( XvpnCountEventEdit != 0 )
          {
            XvpnCountEventEdit = XvpnCountEventEdit - 1;

            XvpnResetCursor();
          }
          else
          {
            XvpnEditUnselectAll();

            XvpnChangeCursorType( NULL, NULL, 0,
                                  XVPN_INPUT_POINT );

            XvpnInputMode      = XVPN_INPUT_POINT;
            XvpnEditMode       = XVPN_EDIT_IDENTIFY;
            XvpnPromptMessage  = XvpnPromptEditIdentify;
            XvpnCountEventEdit = 0;
            XvpnMaxEventEdit   = 1;
          }
        }

        XvpnPromptMessage();

        break; 

      case XVPN_MOTION :

        break;

      case XVPN_ENTER  : XvpnCursorInside = XVPN_TRUE;
                         XvpnDisplayCursor();
        break;

      case XVPN_KEY_UP    : XvpnZoomUp();

        break;

      case XVPN_KEY_DN : XvpnZoomDown();

        break;

      case XVPN_KEY_LEFT : XvpnZoomLeft();

        break;

      case XVPN_KEY_RIGHT : XvpnZoomRight();

        break;

      case XVPN_KEY_MORE : XvpnZoomMore();

        break;

      case XVPN_KEY_LESS : XvpnZoomLess();

        break;

      case XVPN_KEY_FIT : XvpnZoomFit();

        break;

      case XVPN_KEY_REFRESH : XvpnZoomRefresh();

        break;

      case XVPN_KEY_UNZOOM : XvpnZoomUndo();

        break;

      case XVPN_KEY_ZOOM_IN :

         XvpnChangeEditMode( XVPN_ZOOM_IN, XvpnPromptZoomIn );

        break;

      case XVPN_KEY_CENTER :

         XvpnChangeEditMode( XVPN_ZOOM_CENTER, XvpnPromptZoomCenter );

        break;

      case XVPN_KEY_PAN :

         XvpnChangeEditMode( XVPN_ZOOM_PAN, XvpnPromptZoomPan );

        break;

      case XVPN_KEY_NEXT :

        XvpnFileNextSlide();

        break;

      case XVPN_KEY_PREV :

        XvpnFilePrevSlide();

        break;
    }

    if ( ( OldUnitCursorX != XvpnUnitCursorX ) ||
         ( OldUnitCursorY != XvpnUnitCursorY ) )
    {
      XvpnDisplayCoordinates(); 
      XvpnDisplayCursor();
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
  if ( XvpnFirstExpose == XVPN_FALSE )
  {
    autbegin();

    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );
    XvpnUndisplayCursor();

    XvpnResizeGraphicWindow();
    XvpnRefreshGraphicWindow( 0, 0,
                               XvpnGraphicDx, XvpnGraphicDy );
    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );
    XvpnDisplayCursor();

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

  if ( XvpnFirstExpose == XVPN_TRUE )
  {
    XvpnFirstExpose = XVPN_FALSE;

    XvpnInitializeGraphicWindow();
    XvpnUndisplayCursor();

    XvpnLoadConfig( XVPN_FALSE );

    if ( XvpnFirstFileName != (char *)NULL )
    {
      XvpnDisplayCursor();
      XvpnFileOpen( XvpnFirstFileName );

      XvpnFirstFileName = (char *)NULL;
    }
    else
    {
      XvpnChangeTopLevelTitle( (char *)NULL );

      XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
      XvpnDisplayCursor();
    }

    /* XvpnEnterPresentPanel(); */
  }
  else
  {
    XvpnUndisplayCursor();
    XvpnRefreshGraphicWindow( ExposeEvent->x, ExposeEvent->y,
                               ExposeEvent->width, ExposeEvent->height );
    XvpnDisplayCursor();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnInitializeEvent                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XvpnGraphicWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XvpnEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackEvent";
  NewActions.proc   =  CallbackEvent;

  XtAppAddActions( XvpnApplication, &NewActions, 1 );

  XtAddCallback( XvpnGraphicWindow, 
                 XmNresizeCallback, 
                 CallbackResize, NULL );

  XtAddCallback( XvpnGraphicWindow, 
                 XmNexposeCallback, 
                 CallbackExpose, NULL );

  autend();
}
