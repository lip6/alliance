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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Event.c                         |
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
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <Xm/Xm.h>

 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GMV.h"
# include "GMS.h"
# include "GMH.h"
# include "GMX_motif.h"
# include "GMX_grid.h"
# include "GMX_cursor.h"
# include "GMX_event.h"
# include "GMX_message.h"

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

  static String DrealEventTranslation =

    "<Btn1Down>:      CallbackEvent(  0 )\n\
     <Btn3Down>:      CallbackEvent(  1 )\n\
     <Motion>:        CallbackEvent(  2 )\n\
     <EnterWindow>:   CallbackEvent(  3 )\n\
     <LeaveWindow>:   CallbackEvent(  4 )\n\
     <Key>osfUp:      CallbackEvent(  5 )\n\
     <Key>osfDown:    CallbackEvent(  6 )\n\
     <Btn5Down>:      CallbackEvent(  6 )\n\
     <Key>osfLeft:    CallbackEvent(  7 )\n\
     <Key>osfRight:   CallbackEvent(  8 )\n\
     <Key>Z:          CallbackEvent(  9 )\n\
     <Key>M:          CallbackEvent( 10 )\n\
     <Key>F:          CallbackEvent( 11 )\n\
     <Key>R:          CallbackEvent( 12 )\n\
     <Key>U:          CallbackEvent( 13 )\n\
     <Key>I:          CallbackEvent( 14 )\n\
     <Key>C:          CallbackEvent( 15 )\n\
     <Key>P:          CallbackEvent( 16 )\n\
     Shift<Key>Tab:   CallbackEvent( 17 )\n\
     <Key>Tab:        CallbackEvent( 18 )";

/*------------------------------------------------------------\
|                                                             |
|                         Event Count                         |
|                                                             |
\------------------------------------------------------------*/

  static char DrealMaxEventEditTable[] =

  {
    2,  /* DREAL_EDIT_MEASURE           */
    2,  /* DREAL_CREATE_RECTANGLE       */
    2,  /* DREAL_CREATE_ABUTMENTBOX     */
    1,  /* DREAL_SELECT_POINT_IDENTIFY  */
    1,  /* DREAL_SELECT_POINT_COPY      */
    1,  /* DREAL_SELECT_POINT_MOVE      */
    1,  /* DREAL_SELECT_POINT_DELETE    */
    1,  /* DREAL_SELECT_POINT_STRETCH   */
    1,  /* DREAL_SELECT_POINT_MODIFY    */
    2,  /* DREAL_SELECT_WINDOW_IDENTIFY */
    2,  /* DREAL_SELECT_WINDOW_COPY     */
    2,  /* DREAL_SELECT_WINDOW_MOVE     */
    2,  /* DREAL_SELECT_WINDOW_DELETE   */
    2,  /* DREAL_SELECT_WINDOW_MODIFY   */
    2,  /* DREAL_EDIT_COPY_POINT        */
    2,  /* DREAL_EDIT_COPY_WINDOW       */
    2,  /* DREAL_EDIT_MOVE_POINT        */
    2,  /* DREAL_EDIT_MOVE_WINDOW       */
    2   /* DREAL_EDIT_STRETCH_POINT     */
  };

  static char DrealInputEventEditTable[] =

  {
    DREAL_INPUT_HALF_BOX, /* DREAL_EDIT_MEASURE           */
    DREAL_INPUT_BOX,      /* DREAL_CREATE_RECTANGLE       */
    DREAL_INPUT_BOX,      /* DREAL_CREATE_ABUTMENTBOX     */
    DREAL_INPUT_POINT,    /* DREAL_SELECT_POINT_IDENTIFY  */
    DREAL_INPUT_POINT,    /* DREAL_SELECT_POINT_COPY      */
    DREAL_INPUT_POINT,    /* DREAL_SELECT_POINT_MOVE      */
    DREAL_INPUT_POINT,    /* DREAL_SELECT_POINT_DELETE    */
    DREAL_INPUT_POINT,    /* DREAL_SELECT_POINT_STRETCH   */
    DREAL_INPUT_POINT,    /* DREAL_SELECT_POINT_MODIFY    */
    DREAL_INPUT_BOX,      /* DREAL_SELECT_WINDOW_IDENTIFY */
    DREAL_INPUT_BOX,      /* DREAL_SELECT_WINDOW_COPY     */
    DREAL_INPUT_BOX,      /* DREAL_SELECT_WINDOW_MOVE     */
    DREAL_INPUT_BOX,      /* DREAL_SELECT_WINDOW_DELETE   */
    DREAL_INPUT_BOX,      /* DREAL_SELECT_WINDOW_MODIFY   */
    DREAL_INPUT_LINE,     /* DREAL_EDIT_COPY_POINT        */
    DREAL_INPUT_LINE,     /* DREAL_EDIT_COPY_WINDOW       */
    DREAL_INPUT_LINE,     /* DREAL_EDIT_MOVE_POINT        */
    DREAL_INPUT_LINE,     /* DREAL_EDIT_MOVE_WINDOW       */
    DREAL_INPUT_ORTHO     /* DREAL_EDIT_STRETCH_POINT     */
  };

  static char DrealMaxEventZoomTable[] =

  {
    1,  /* DREAL_ZOOM_CENTER            */
    2,  /* DREAL_ZOOM_IN                */
    2   /* DREAL_ZOOM_PAN               */
  };

  static char DrealInputEventZoomTable[] =

  {
    DREAL_INPUT_POINT,  /* DREAL_ZOOM_CENTER            */
    DREAL_INPUT_BOX,    /* DREAL_ZOOM_IN                */
    DREAL_INPUT_LINE    /* DREAL_ZOOM_PAN               */
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

  static long     DrealLambdaEventZoomX[ 2 ] = { 0, 0 };
  static long     DrealLambdaEventZoomY[ 2 ] = { 0, 0 };
  static char     DrealMaxEventZoom          = 0;
         char     DrealCountEventZoom        = 0;

  static long     DrealLambdaEventEditX[ 2 ] = { 0, 0 };
  static long     DrealLambdaEventEditY[ 2 ] = { 0, 0 };
  static char     DrealMaxEventEdit          = 2;
         char     DrealCountEventEdit        = 0;

  static char     DrealFirstExpose           = DREAL_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                          Input                              |
|                                                             |
\------------------------------------------------------------*/

 static char DrealInputMode     = DREAL_INPUT_HALF_BOX;
 static char DrealSaveInputMode = DREAL_INPUT_HALF_BOX;
 static char DrealEditMode      = DREAL_EDIT_MEASURE;
 static char DrealSaveEditMode  = DREAL_EDIT_MEASURE;

/*------------------------------------------------------------\
|                                                             |
|                          Edit Message                       |
|                                                             |
\------------------------------------------------------------*/

  static void (*DrealPromptMessage)()     = DrealPromptEditMeasure;
  static void (*DrealSavePromptMessage)() = DrealPromptEditMeasure;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     DrealChangeEditMode                     |
|                                                             |
\------------------------------------------------------------*/
 
void DrealChangeEditMode( EditMode , PromptMessage )
 
   char  EditMode;
   void (*PromptMessage)();
{
  rdsbegin();

  if ( EditMode & DREAL_ZOOM_MARK )
  {
    EditMode = EditMode & ~DREAL_ZOOM_MARK;

    DrealCountEventZoom = 0;
    DrealMaxEventZoom   = DrealMaxEventZoomTable[ EditMode ];

    if ( ! ( DrealEditMode & DREAL_ZOOM_MARK ) )
    {
      DrealSaveEditMode      = DrealEditMode;
      DrealSaveInputMode     = DrealInputMode;
      DrealSavePromptMessage = DrealPromptMessage;
    }

    DrealInputMode     = DrealInputEventZoomTable[ EditMode ];
    DrealPromptMessage = PromptMessage;
    DrealEditMode      = EditMode | DREAL_ZOOM_MARK;

    DrealChangeCursorType( NULL, NULL, 0, DrealInputMode );
  }  
  else
  {
    if ( DrealEditMode & DREAL_ZOOM_MARK )
    {
      DrealSaveEditMode      = EditMode;
      DrealSaveInputMode     = DrealInputEventEditTable[ EditMode ];
      DrealSavePromptMessage = PromptMessage;
      DrealCountEventEdit    = 0;
      DrealMaxEventEdit      = DrealMaxEventEditTable[ EditMode ];
    }
    else
    {
      if ( DrealEditMode != EditMode )
      {
        DrealCountEventEdit = 0;
        DrealMaxEventEdit   = DrealMaxEventEditTable[ EditMode ];
        DrealEditMode       = EditMode;
        DrealInputMode      = DrealInputEventEditTable[ EditMode ];
        DrealPromptMessage  = PromptMessage;

        DrealChangeCursorType( NULL, NULL, 0, DrealInputMode );
      }
    }

    DrealEditUnselectAll();
  }

  DrealPromptMessage();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealContinueEditMode                     |
|                                                             |
\------------------------------------------------------------*/

void DrealContinueEditMode( EditMode , PromptMessage, CountEvent )

   char  EditMode;
   void (*PromptMessage)();
   char  CountEvent;
{
  rdsbegin();

  if ( DrealEditMode & DREAL_ZOOM_MARK )
  {
    DrealSaveEditMode      = EditMode;
    DrealSaveInputMode     = DrealInputEventEditTable[ EditMode ];
    DrealSavePromptMessage = PromptMessage;
    DrealCountEventEdit    = CountEvent;
    DrealMaxEventEdit      = DrealMaxEventEditTable[ EditMode ];
  }
  else
  {
    DrealCountEventEdit = CountEvent;
    DrealMaxEventEdit   = DrealMaxEventEditTable[ EditMode ];
    DrealEditMode       = EditMode;
    DrealInputMode      = DrealInputEventEditTable[ EditMode ];
    DrealPromptMessage  = PromptMessage;

    DrealChangeCursorType( DrealLambdaEventEditX,
                           DrealLambdaEventEditY,
                           DrealCountEventEdit,
                           DrealInputMode );

  }

  DrealPromptMessage();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealTreatEventZoom                    |
|                                                             |
\------------------------------------------------------------*/

void DrealTreatEventZoom()

{
  rdsbegin();

  switch ( DrealEditMode )
  {
    case DREAL_ZOOM_CENTER :
                              
      DrealZoomCenter( DrealLambdaEventZoomX[ 0 ], 
                       DrealLambdaEventZoomY[ 0 ] );
      break;

    case DREAL_ZOOM_IN     :

      DrealZoomIn( DrealLambdaEventZoomX[ 0 ],
                   DrealLambdaEventZoomY[ 0 ],
                   DrealLambdaEventZoomX[ 1 ],
                   DrealLambdaEventZoomY[ 1 ] );
      break;

    case DREAL_ZOOM_PAN    :

      DrealZoomPan( DrealLambdaEventZoomX[ 0 ],
                    DrealLambdaEventZoomY[ 0 ],
                    DrealLambdaEventZoomX[ 1 ],
                    DrealLambdaEventZoomY[ 1 ] );
      break;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealTreatEventEdit                    |
|                                                             |
\------------------------------------------------------------*/

void DrealTreatEventEdit()

{ 
  long DeltaX;
  long DeltaY;
  long X1;
  long X2;
  long Y1;
  long Y2;

  rdsend();

  X1 = DrealLambdaEventEditX[0];
  X2 = DrealLambdaEventEditX[1];
  Y1 = DrealLambdaEventEditY[0];
  Y2 = DrealLambdaEventEditY[1];

  if ( ( DrealInputMode == DREAL_INPUT_LSTRING ) || 
       ( DrealInputMode == DREAL_INPUT_ORTHO   ) ||
       ( DrealInputMode == DREAL_INPUT_SORTHO  ) )
  {
    DeltaX = X1 - X2;
    DeltaY = Y1 - Y2;

    if ( DeltaX < 0 ) DeltaX = - DeltaX;
    if ( DeltaY < 0 ) DeltaY = - DeltaY;

    if ( DeltaX > DeltaY )
    {
      Y2 = Y1;

      DrealLambdaEventEditX[0] = X2;
      DrealLambdaEventEditY[0] = Y2;
    }  
    else
    {   
      X2 = X1;

      DrealLambdaEventEditX[0] = X2;
      DrealLambdaEventEditY[0] = Y2;
    }
  }

  if ( DrealInputMode == DREAL_INPUT_LSTRING )
  {   
    DrealChangeCursorType( DrealLambdaEventEditX,
                           DrealLambdaEventEditY,
                           1, DrealInputMode );
 
    DrealCountEventEdit = 1;
  }
  else
  {
    DrealChangeCursorType( NULL, NULL, 0,
                           DrealInputMode );

    DrealCountEventEdit = 0;
  }

  switch ( DrealEditMode )
  {
    case DREAL_EDIT_MEASURE           :

      break;

    case DREAL_CREATE_RECTANGLE       :

      DrealCreateRectangleRds( X1, Y1, X2, Y2 );

      break;

    case DREAL_CREATE_ABUTMENTBOX  :

     DrealCreateAbutmentBoxRds( X1, Y1, X2, Y2 );

    break;

    case DREAL_SELECT_POINT_IDENTIFY :
 
      DrealEditSelectPoint( X1, Y1 );
      DrealEditIdentify();

    break;

    case DREAL_SELECT_WINDOW_IDENTIFY :

      DrealEditSelectFence( X1, Y1, X2, Y2 );
      DrealEditIdentify();

    break;

    case DREAL_SELECT_POINT_COPY :

      DrealEditSelectPointCopy( X1, Y1 );

    break;

    case DREAL_SELECT_WINDOW_COPY :

      DrealEditSelectWindowCopy( X1, Y1, X2, Y2 );

    break;

    case DREAL_EDIT_COPY_POINT :

      DrealEditCopy( X1, Y1, X2, Y2, 0 );

    break;

    case DREAL_EDIT_COPY_WINDOW :
 
      DrealEditCopy( X1, Y1, X2, Y2, 1 );

    break;

    case DREAL_SELECT_POINT_MOVE :

      DrealEditSelectPointMove( X1, Y1 );

    break;

    case DREAL_SELECT_WINDOW_MOVE :

      DrealEditSelectWindowMove( X1, Y1, X2, Y2 );

    break;

    case DREAL_EDIT_MOVE_POINT :

      DrealEditMove( X1, Y1, X2, Y2, 0 );

    break;

    case DREAL_EDIT_MOVE_WINDOW :

      DrealEditMove( X1, Y1, X2, Y2, 1 );

    break;

    case DREAL_SELECT_POINT_DELETE :

      DrealEditSelectPointDelete( X1, Y1 );

    break;

    case DREAL_SELECT_WINDOW_DELETE :

      DrealEditSelectWindowDelete( X1, Y1, X2, Y2 );

    break;

    case DREAL_SELECT_POINT_STRETCH :

      DrealEditSelectPointStretch( X1, Y1 );

    break;

    case DREAL_EDIT_STRETCH_POINT :

      DrealEditStretch( X1, Y1, X2, Y2 );

    break;

    case DREAL_SELECT_POINT_MODIFY :

      DrealEditSelectPointModify( X1, Y1 );

    break;

    case DREAL_SELECT_WINDOW_MODIFY :

      DrealEditSelectWindowModify( X1, Y1, X2, Y2 );

    break;
  }

  rdsend();
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
  int       FlagUp;
  Position  OldLambdaCursorX;
  Position  OldLambdaCursorY;

  rdsbegin();

  EventDisplay = Event->display;

  MouseEvent = atoi( Args[ 0 ] );

  if ( MouseEvent == DREAL_LEAVE )
  {
    DrealUndisplayCursor();
    DrealCursorInside = DREAL_FALSE;
  }
  else
  {
    OldLambdaCursorX = DrealLambdaCursorX;
    OldLambdaCursorY = DrealLambdaCursorY;

    DrealComputeCursor( Event->x, Event->y );

    switch ( MouseEvent )
    {
      case DREAL_B1DN   : 

        if ( DrealEditMode & DREAL_ZOOM_MARK )
        {
          if ( DrealCountEventZoom < DrealMaxEventZoom )
          {
            DrealLambdaEventZoomX[ DrealCountEventZoom ] = DrealLambdaCursorX;
            DrealLambdaEventZoomY[ DrealCountEventZoom ] = DrealLambdaCursorY;

            DrealCountEventZoom = DrealCountEventZoom + 1;

            DrealPointCursor();
          
            if ( DrealCountEventZoom == DrealMaxEventZoom )
            {
              DrealChangeCursorType( NULL, NULL, 0, DrealInputMode );

              DrealTreatEventZoom();

              DrealInputMode      = DrealSaveInputMode;
              DrealEditMode       = DrealSaveEditMode;
              DrealPromptMessage  = DrealSavePromptMessage;
              DrealSaveEditMode   = DREAL_EDIT_MEASURE;
              DrealSaveInputMode  = DREAL_INPUT_HALF_BOX;
              DrealCountEventZoom = 0;
              DrealMaxEventZoom   = 0;

              DrealChangeCursorType( DrealLambdaEventEditX,
                                     DrealLambdaEventEditY,
                                     DrealCountEventEdit,
                                     DrealInputMode );

              DrealComputeCursor( Event->x, Event->y );
              DrealDisplayCursor();
            }
          }
        }
        else
        {
          if ( DrealCountEventEdit < DrealMaxEventEdit )
          {
            DrealLambdaEventEditX[ DrealCountEventEdit ] = DrealLambdaCursorX;
            DrealLambdaEventEditY[ DrealCountEventEdit ] = DrealLambdaCursorY;

            DrealCountEventEdit = DrealCountEventEdit + 1;

            DrealPointCursor();

            if ( DrealCountEventEdit == DrealMaxEventEdit )
            {
              DrealTreatEventEdit();

              DrealDisplayCursor();
            }
          }
        }

        DrealPromptMessage();

        break;

      case DREAL_B3DN   :

        if ( DrealEditMode & DREAL_ZOOM_MARK )
        {
          if ( DrealCountEventZoom != 0 )
          {
            DrealCountEventZoom = DrealCountEventZoom - 1;

            DrealResetCursor();
          }
          else
          { 
            DrealChangeCursorType( DrealLambdaEventEditX,
                                   DrealLambdaEventEditY,
                                   DrealCountEventEdit,
                                   DrealSaveInputMode );

            DrealInputMode          = DrealSaveInputMode;
            DrealEditMode           = DrealSaveEditMode;
            DrealPromptMessage      = DrealSavePromptMessage;
            DrealSaveEditMode       = DREAL_EDIT_MEASURE;
            DrealSaveInputMode      = DREAL_INPUT_HALF_BOX;
            DrealSavePromptMessage  = DrealPromptEditMeasure;
            DrealCountEventZoom     = 0;
            DrealMaxEventZoom       = 0;
          }
        }
        else
        {
          if ( DrealCountEventEdit != 0 )
          {
            DrealCountEventEdit = DrealCountEventEdit - 1;

            DrealResetCursor();
          }
          else
          {
            DrealEditUnselectAll();

            DrealChangeCursorType( NULL, NULL, 0,
                                   DREAL_INPUT_HALF_BOX );

            DrealInputMode      = DREAL_INPUT_HALF_BOX;
            DrealEditMode       = DREAL_EDIT_MEASURE;
            DrealPromptMessage  = DrealPromptEditMeasure;
            DrealCountEventEdit = 0;
            DrealMaxEventEdit   = 2;
          }
        }

        DrealPromptMessage();

        break; 

      case DREAL_MOTION :

        break;

      case DREAL_ENTER  : 

        DrealCursorInside = DREAL_TRUE;
        DrealDisplayCursor();

        break;

      case DREAL_KEY_UP    : DrealZoomUp();

        break;

      case DREAL_KEY_DN : DrealZoomDown();

        break;

      case DREAL_KEY_LEFT : DrealZoomLeft();

        break;

      case DREAL_KEY_RIGHT : DrealZoomRight();

        break;

      case DREAL_KEY_MORE : DrealZoomMore();

        break;

      case DREAL_KEY_LESS : DrealZoomLess();

        break;

      case DREAL_KEY_FIT : DrealZoomFit();

        break;

      case DREAL_KEY_REFRESH : DrealZoomRefresh();

        break;

      case DREAL_KEY_UNZOOM : DrealZoomUndo();

        break;

      case DREAL_KEY_ZOOM_IN :

         DrealChangeEditMode( DREAL_ZOOM_IN, DrealPromptZoomIn );

        break;

      case DREAL_KEY_CENTER :

         DrealChangeEditMode( DREAL_ZOOM_CENTER, DrealPromptZoomCenter );

        break;

      case DREAL_KEY_PAN :

         DrealChangeEditMode( DREAL_ZOOM_PAN, DrealPromptZoomPan );

        break;

      case DREAL_KEY_UP_LAYER :
      case DREAL_KEY_DN_LAYER :

        if ( ! ( DrealEditMode & DREAL_ZOOM_MARK ) )
        {
          FlagUp = ( MouseEvent == DREAL_KEY_UP_LAYER );

          switch ( DrealEditMode )
          {
            case DREAL_CREATE_RECTANGLE : DrealChangeRectangleLayer( FlagUp );
            break;
          }

          DrealPromptMessage();
        }

        break;
    }

    if ( MouseEvent >= DREAL_KEY_UP )
    {
      DrealComputeCursor( Event->x, Event->y );
      DrealDisplayCursor();
    }

    if ( ( OldLambdaCursorX != DrealLambdaCursorX ) ||
         ( OldLambdaCursorY != DrealLambdaCursorY ) )
    {
      DrealDisplayCoordinates(); 
      DrealDisplayCursor();
    }
  }

  rdsend();
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
  rdsbegin();

  if ( DrealFirstExpose == DREAL_FALSE )
  {
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );
    DrealUndisplayCursor();

    DrealResizeGraphicWindow();
    DrealRefreshGraphicWindow( 0, 0,
                               DrealGraphicDx, DrealGraphicDy );
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );
    DrealDisplayCursor();
  }

  rdsend();
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

  rdsbegin();

  ExposeEvent = (XExposeEvent *)CallData->event;

  if ( DrealFirstExpose == DREAL_TRUE )
  {
    DrealFirstExpose = DREAL_FALSE;

    DrealInitializeGraphicWindow();
    DrealUndisplayCursor();

    DrealLoadConfig( DREAL_FALSE );

    if ( DrealFigureRds != (rdsfig_list *)NULL )
    {
      DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

      DrealChangeTopLevelTitle( DrealFigureRds->NAME );
      DrealInitializeZoom();
      DrealDisplayToolsMessage();
      DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );

      DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );
    }
    else
    {
      DrealChangeTopLevelTitle( (char *)NULL );
    }

    DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );
    DrealDisplayCursor();

    /* LUDO: Fed up to Click On IT ...
    DrealEnterPresentPanel();
    */
  }
  else
  {
    DrealUndisplayCursor();
    DrealRefreshGraphicWindow( ExposeEvent->x, ExposeEvent->y,
                               ExposeEvent->width, ExposeEvent->height );
    DrealDisplayCursor();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealInitializeEvent                    |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeEvent()

{
  XtActionsRec NewActions;

  rdsbegin();

  XtVaSetValues( DrealGraphicWindow,
                 XmNtranslations,
                 XtParseTranslationTable( DrealEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackEvent";
  NewActions.proc   =  CallbackEvent;

  XtAppAddActions( DrealApplication, &NewActions, 1 );

  XtAddCallback( DrealGraphicWindow, 
                 XmNresizeCallback, 
                 CallbackResize, NULL );

  XtAddCallback( DrealGraphicWindow, 
                 XmNexposeCallback, 
                 CallbackExpose, NULL );

  rdsend();
}
