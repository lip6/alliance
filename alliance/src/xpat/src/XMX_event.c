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
| Tool    :                   XPAT                            |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
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

  static String XpatEventTranslation =

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

  static char XpatMaxEventEditTable[] =

  {
    2,  /* XPAT_EDIT_MEASURE    */
    1,  /* XPAT_EDIT_IDENTIFY   */
    1,  /* XPAT_EDIT_CONNECTED  */
    1,  /* XPAT_EDIT_ADD_CURSOR */
    1   /* XPAT_EDIT_DEL_CURSOR */
  };

  static char XpatInputEventEditTable[] =

  {
    XPAT_INPUT_HALF_BOX, /* XPAT_EDIT_MEASURE    */
    XPAT_INPUT_POINT,    /* XPAT_EDIT_IDENTIFY   */
    XPAT_INPUT_POINT,    /* XPAT_EDIT_CONNECTED  */
    XPAT_INPUT_POINT,    /* XPAT_EDIT_ADD_CURSOR */
    XPAT_INPUT_POINT     /* XPAT_EDIT_DEL_CURSOR */
  };

  static char XpatMaxEventZoomTable[] =

  {
    1,  /* XPAT_ZOOM_CENTER            */
    2,  /* XPAT_ZOOM_IN                */
    2   /* XPAT_ZOOM_PAN               */
  };

  static char XpatInputEventZoomTable[] =

  {
    XPAT_INPUT_POINT,  /* XPAT_ZOOM_CENTER            */
    XPAT_INPUT_BOX,    /* XPAT_ZOOM_IN                */
    XPAT_INPUT_LINE    /* XPAT_ZOOM_PAN               */
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

  static long     XpatUnitEventZoomX[ 2 ] = { 0, 0 };
  static long     XpatUnitEventZoomY[ 2 ] = { 0, 0 };
  static char     XpatMaxEventZoom          = 0;
         char     XpatCountEventZoom        = 0;

  static long     XpatUnitEventEditX[ 2 ] = { 0, 0 };
  static long     XpatUnitEventEditY[ 2 ] = { 0, 0 };
  static char     XpatMaxEventEdit          = 2;
         char     XpatCountEventEdit        = 0;

  static char     XpatFirstExpose           = XPAT_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                          Input                              |
|                                                             |
\------------------------------------------------------------*/

 static char XpatInputMode     = XPAT_INPUT_HALF_BOX;
 static char XpatSaveInputMode = XPAT_INPUT_HALF_BOX;
 static char XpatEditMode      = XPAT_EDIT_MEASURE;
 static char XpatSaveEditMode  = XPAT_EDIT_MEASURE;

/*------------------------------------------------------------\
|                                                             |
|                          Edit Message                       |
|                                                             |
\------------------------------------------------------------*/

  static void (*XpatPromptMessage)()     = XpatPromptEditMeasure;
  static void (*XpatSavePromptMessage)() = XpatPromptEditMeasure;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XpatChangeEditMode                     |
|                                                             |
\------------------------------------------------------------*/
 
void XpatChangeEditMode( EditMode , PromptMessage )
 
   char  EditMode;
   void (*PromptMessage)();
{
  autbegin();

  if ( EditMode & XPAT_ZOOM_MARK )
  {
    EditMode = EditMode & ~XPAT_ZOOM_MARK;

    XpatCountEventZoom = 0;
    XpatMaxEventZoom   = XpatMaxEventZoomTable[ EditMode ];

    if ( ! ( XpatEditMode & XPAT_ZOOM_MARK ) )
    {
      XpatSaveEditMode      = XpatEditMode;
      XpatSaveInputMode     = XpatInputMode;
      XpatSavePromptMessage = XpatPromptMessage;
    }

    XpatInputMode     = XpatInputEventZoomTable[ EditMode ];
    XpatPromptMessage = PromptMessage;
    XpatEditMode      = EditMode | XPAT_ZOOM_MARK;

    XpatChangeCursorType( NULL, NULL, 0, XpatInputMode );
  }  
  else
  {
    if ( XpatEditMode & XPAT_ZOOM_MARK )
    {
      XpatSaveEditMode      = EditMode;
      XpatSaveInputMode     = XpatInputEventEditTable[ EditMode ];
      XpatSavePromptMessage = PromptMessage;
      XpatCountEventEdit    = 0;
      XpatMaxEventEdit      = XpatMaxEventEditTable[ EditMode ];
    }
    else
    {
      if ( XpatEditMode != EditMode )
      {
        XpatCountEventEdit = 0;
        XpatMaxEventEdit   = XpatMaxEventEditTable[ EditMode ];
        XpatEditMode       = EditMode;
        XpatInputMode      = XpatInputEventEditTable[ EditMode ];
        XpatPromptMessage  = PromptMessage;

        XpatChangeCursorType( NULL, NULL, 0, XpatInputMode );
      }
    }

    XpatEditUnselectAll();
  }

  XpatPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XpatContinueEditMode                     |
|                                                             |
\------------------------------------------------------------*/

void XpatContinueEditMode( EditMode , PromptMessage, CountEvent )

   char  EditMode;
   void (*PromptMessage)();
   char  CountEvent;
{
  autbegin();

  if ( XpatEditMode & XPAT_ZOOM_MARK )
  {
    XpatSaveEditMode      = EditMode;
    XpatSaveInputMode     = XpatInputEventEditTable[ EditMode ];
    XpatSavePromptMessage = PromptMessage;
    XpatCountEventEdit    = CountEvent;
    XpatMaxEventEdit      = XpatMaxEventEditTable[ EditMode ];
  }
  else
  {
    XpatCountEventEdit = CountEvent;
    XpatMaxEventEdit   = XpatMaxEventEditTable[ EditMode ];
    XpatEditMode       = EditMode;
    XpatInputMode      = XpatInputEventEditTable[ EditMode ];
    XpatPromptMessage  = PromptMessage;

    XpatChangeCursorType( XpatUnitEventEditX,
                           XpatUnitEventEditY,
                           XpatCountEventEdit,
                           XpatInputMode );

  }

  XpatPromptMessage();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatTreatEventZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XpatTreatEventZoom()

{
  autbegin();

  XpatChangeCursorType( XpatUnitEventEditX,
                         XpatUnitEventEditY,
                         XpatCountEventEdit,
                         XpatSaveInputMode );

  switch ( XpatEditMode )
  {
    case XPAT_ZOOM_CENTER :
                              
      XpatZoomCenter( XpatUnitEventZoomX[ 0 ], 
                       XpatUnitEventZoomY[ 0 ] );
      break;

    case XPAT_ZOOM_IN     :

      XpatZoomIn( XpatUnitEventZoomX[ 0 ],
                   XpatUnitEventZoomY[ 0 ],
                   XpatUnitEventZoomX[ 1 ],
                   XpatUnitEventZoomY[ 1 ] );
      break;

    case XPAT_ZOOM_PAN    :

      XpatZoomPan( XpatUnitEventZoomX[ 0 ],
                    XpatUnitEventZoomY[ 0 ],
                    XpatUnitEventZoomX[ 1 ],
                    XpatUnitEventZoomY[ 1 ] );
      break;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatTreatEventEdit                    |
|                                                             |
\------------------------------------------------------------*/

void XpatTreatEventEdit()

{ 
  long DeltaX;
  long DeltaY;
  long X1;
  long X2;
  long Y1;
  long Y2;

  autbegin();

  X1 = XpatUnitEventEditX[0];
  X2 = XpatUnitEventEditX[1];
  Y1 = XpatUnitEventEditY[0];
  Y2 = XpatUnitEventEditY[1];

  if ( ( XpatInputMode == XPAT_INPUT_LSTRING ) || 
       ( XpatInputMode == XPAT_INPUT_ORTHO   ) ||
       ( XpatInputMode == XPAT_INPUT_SORTHO  ) )
  {
    DeltaX = X1 - X2;
    DeltaY = Y1 - Y2;

    if ( DeltaX < 0 ) DeltaX = - DeltaX;
    if ( DeltaY < 0 ) DeltaY = - DeltaY;

    if ( DeltaX > DeltaY )
    {
      Y2 = Y1;

      XpatUnitEventEditX[0] = X2;
      XpatUnitEventEditY[0] = Y2;
    }  
    else
    {   
      X2 = X1;

      XpatUnitEventEditX[0] = X2;
      XpatUnitEventEditY[0] = Y2;
    }
  }

  if ( XpatInputMode == XPAT_INPUT_LSTRING )
  {   
    XpatChangeCursorType( XpatUnitEventEditX,
                           XpatUnitEventEditY,
                           1, XpatInputMode );
 
    XpatCountEventEdit = 1;
  }
  else
  {
    XpatChangeCursorType( NULL, NULL, 0,
                           XpatInputMode );

    XpatCountEventEdit = 0;
  }

  switch ( XpatEditMode )
  {
    case XPAT_EDIT_MEASURE :

    break;

    case XPAT_EDIT_IDENTIFY :

     XpatEditIdentify( X1, Y1 );

    break;

    case XPAT_EDIT_CONNECTED :

     XpatEditConnected( X1, Y1 );

    break;

    case XPAT_EDIT_ADD_CURSOR :

     XpatEditAddCursor( X1, Y1 );

    break;

    case XPAT_EDIT_DEL_CURSOR :

     XpatEditDelCursor( X1, Y1 );

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

  if ( MouseEvent == XPAT_LEAVE )
  {
    XpatUndisplayCursor();
    XpatCursorInside = XPAT_FALSE;
  }
  else
  {
    OldUnitCursorX = XpatUnitCursorX;
    OldUnitCursorY = XpatUnitCursorY;

    XpatComputeCursor( Event->x, Event->y );

    switch ( MouseEvent )
    {
      case XPAT_B1DN   : 

        if ( XpatEditMode & XPAT_ZOOM_MARK )
        {
          if ( XpatCountEventZoom < XpatMaxEventZoom )
          {
            XpatUnitEventZoomX[ XpatCountEventZoom ] = XpatUnitCursorX;
            XpatUnitEventZoomY[ XpatCountEventZoom ] = XpatUnitCursorY;

            XpatCountEventZoom = XpatCountEventZoom + 1;

            XpatPointCursor();
          
            if ( XpatCountEventZoom == XpatMaxEventZoom )
            {
              XpatChangeCursorType( NULL, NULL, 0, XpatInputMode );

              XpatTreatEventZoom();

              XpatInputMode      = XpatSaveInputMode;
              XpatEditMode       = XpatSaveEditMode;
              XpatPromptMessage  = XpatSavePromptMessage;
              XpatSaveEditMode   = XPAT_EDIT_MEASURE;
              XpatSaveInputMode  = XPAT_INPUT_HALF_BOX;
              XpatCountEventZoom = 0;
              XpatMaxEventZoom   = 0;

              XpatChangeCursorType( XpatUnitEventEditX,
                                    XpatUnitEventEditY,
                                    XpatCountEventEdit,
                                    XpatInputMode );

              XpatComputeCursor( Event->x, Event->y );
              XpatDisplayCursor();
            }
          }
        }
        else
        {
          if ( XpatCountEventEdit < XpatMaxEventEdit )
          {
            XpatUnitEventEditX[ XpatCountEventEdit ] = XpatUnitCursorX;
            XpatUnitEventEditY[ XpatCountEventEdit ] = XpatUnitCursorY;

            XpatCountEventEdit = XpatCountEventEdit + 1;

            XpatPointCursor();

            if ( XpatCountEventEdit == XpatMaxEventEdit )
            {
              XpatTreatEventEdit();

              XpatDisplayCursor();
            }
          }
        }

        XpatPromptMessage();

        break;

      case XPAT_B3DN   :

        if ( XpatEditMode & XPAT_ZOOM_MARK )
        {
          if ( XpatCountEventZoom != 0 )
          {
            XpatCountEventZoom = XpatCountEventZoom - 1;

            XpatResetCursor();
          }
          else
          { 
            XpatChangeCursorType( XpatUnitEventEditX,
                                  XpatUnitEventEditY,
                                  XpatCountEventEdit,
                                  XpatSaveInputMode );

            XpatInputMode          = XpatSaveInputMode;
            XpatEditMode           = XpatSaveEditMode;
            XpatPromptMessage      = XpatSavePromptMessage;
            XpatSaveEditMode       = XPAT_EDIT_MEASURE;
            XpatSaveInputMode      = XPAT_INPUT_HALF_BOX;
            XpatSavePromptMessage  = XpatPromptEditMeasure;
            XpatCountEventZoom     = 0;
            XpatMaxEventZoom       = 0;
          }
        }
        else
        {
          if ( XpatCountEventEdit != 0 )
          {
            XpatCountEventEdit = XpatCountEventEdit - 1;

            XpatResetCursor();
          }
          else
          {
            XpatEditUnselectAll();

            XpatChangeCursorType( NULL, NULL, 0,
                                   XPAT_INPUT_HALF_BOX );

            XpatInputMode      = XPAT_INPUT_HALF_BOX;
            XpatEditMode       = XPAT_EDIT_MEASURE;
            XpatPromptMessage  = XpatPromptEditMeasure;
            XpatCountEventEdit = 0;
            XpatMaxEventEdit   = 2;
          }
        }

        XpatPromptMessage();

        break; 

      case XPAT_MOTION :

        break;

      case XPAT_ENTER  : XpatCursorInside = XPAT_TRUE;
                          XpatDisplayCursor();
                          break;

      case XPAT_KEY_UP    : XpatZoomUp();

        break;

      case XPAT_KEY_DN : XpatZoomDown();

        break;

      case XPAT_KEY_LEFT : XpatZoomLeft();

        break;

      case XPAT_KEY_RIGHT : XpatZoomRight();

        break;

      case XPAT_KEY_MORE : XpatZoomMore();

        break;

      case XPAT_KEY_LESS : XpatZoomLess();

        break;

      case XPAT_KEY_FIT : XpatZoomFit();

        break;

      case XPAT_KEY_REFRESH : XpatZoomRefresh();

        break;

      case XPAT_KEY_UNZOOM : XpatZoomUndo();

        break;

      case XPAT_KEY_ZOOM_IN : 

         XpatChangeEditMode( XPAT_ZOOM_IN, XpatPromptZoomIn );

        break;

      case XPAT_KEY_CENTER :

         XpatChangeEditMode( XPAT_ZOOM_CENTER, XpatPromptZoomCenter );

        break;

      case XPAT_KEY_PAN :

         XpatChangeEditMode( XPAT_ZOOM_PAN, XpatPromptZoomPan );

        break;

      case XPAT_RESCALE_MORE :

          XpatReScale( 0 );

        break;

      case XPAT_RESCALE_LESS :

          XpatReScale( 1 );

        break;
    }

    if ( ( OldUnitCursorX != XpatUnitCursorX ) ||
         ( OldUnitCursorY != XpatUnitCursorY ) )
    {
      XpatDisplayCoordinates(); 
      XpatDisplayCursor();
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
  if ( XpatFirstExpose == XPAT_FALSE )
  {
    autbegin();

    XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );
    XpatUndisplayCursor();

    XpatResizeGraphicWindow();
    XpatRefreshGraphicWindow( 0, 0,
                               XpatGraphicDx, XpatGraphicDy );
    XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );
    XpatDisplayCursor();

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

  if ( XpatFirstExpose == XPAT_TRUE )
  {
    XpatFirstExpose = XPAT_FALSE;

    XpatInitializeGraphicWindow();
    XpatUndisplayCursor();

    XpatLoadConfig( XPAT_FALSE );

    if ( XpatFirstFileName != (char *)NULL )
    {
      XpatDisplayCursor();
      XpatFileOpen( XpatFirstFileName );

      XpatFirstFileName = (char *)NULL;
    }
    else
    {
      XpatChangeTopLevelTitle( (char *)NULL );

      XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );
      XpatDisplayCursor();
    }

    XpatEnterPresentPanel();
  }
  else
  {
    XpatUndisplayCursor();
    XpatRefreshGraphicWindow( ExposeEvent->x, ExposeEvent->y,
                               ExposeEvent->width, ExposeEvent->height );
    XpatDisplayCursor();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatInitializeEvent                    |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeEvent()

{
  XtActionsRec NewActions;

  autbegin();

  XtVaSetValues( XpatGraphicWindow,
                 XmNtranslations,
                 XtParseTranslationTable( XpatEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackEvent";
  NewActions.proc   =  CallbackEvent;

  XtAppAddActions( XpatApplication, &NewActions, 1 );

  XtAddCallback( XpatGraphicWindow, 
                 XmNresizeCallback, 
                 CallbackResize, NULL );

  XtAddCallback( XpatGraphicWindow, 
                 XmNexposeCallback, 
                 CallbackExpose, NULL );

  autend();
}
