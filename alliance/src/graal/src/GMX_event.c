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
| File    :                   Event.c                         |
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
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <Xm/Xm.h>

 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GMV.h"
# include "GMS.h"
# include "GME.h"
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

  static String GraalEventTranslation =

    "<Btn1Down>:      CallbackEvent(  0 )\n\
     <Btn3Down>:      CallbackEvent(  1 )\n\
     Shift<Btn4Down>: CallbackEvent(  7 )\n\
     Shift<Btn5Down>: CallbackEvent(  8 )\n\
     Ctrl<Btn4Down>:  CallbackEvent(  9 )\n\
     Ctrl<Btn5Down>:  CallbackEvent( 10 )\n\
     <Btn4Down>:      CallbackEvent(  5 )\n\
     <Btn5Down>:      CallbackEvent(  6 )\n\
     <Motion>:        CallbackEvent(  2 )\n\
     <EnterWindow>:   CallbackEvent(  3 )\n\
     <LeaveWindow>:   CallbackEvent(  4 )\n\
     <Key>osfUp:      CallbackEvent(  5 )\n\
     <Key>osfDown:    CallbackEvent(  6 )\n\
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

  static char GraalMaxEventEditTable[] =

  {
    2,  /* GRAAL_EDIT_MEASURE           */
    2,  /* GRAAL_CREATE_SEGMENT         */
    2,  /* GRAAL_CREATE_SEGMENT_WIRE    */
    1,  /* GRAAL_CREATE_VIA             */
    2,  /* GRAAL_CREATE_TRANSISTOR      */
    2,  /* GRAAL_CREATE_TRANSISTOR_WIRE */
    1,  /* GRAAL_CREATE_CONNECTOR       */
    1,  /* GRAAL_CREATE_INSTANCE        */
    1,  /* GRAAL_CREATE_REFERENCE       */
    2,  /* GRAAL_CREATE_ABUTMENTBOX     */
    1,  /* GRAAL_SELECT_POINT_IDENTIFY  */
    1,  /* GRAAL_SELECT_POINT_COPY      */
    1,  /* GRAAL_SELECT_POINT_MOVE      */
    1,  /* GRAAL_SELECT_POINT_DELETE    */
    1,  /* GRAAL_SELECT_POINT_STRETCH   */
    1,  /* GRAAL_SELECT_POINT_MODIFY    */
    2,  /* GRAAL_SELECT_WINDOW_IDENTIFY */
    2,  /* GRAAL_SELECT_WINDOW_COPY     */
    2,  /* GRAAL_SELECT_WINDOW_MOVE     */
    2,  /* GRAAL_SELECT_WINDOW_DELETE   */
    2,  /* GRAAL_SELECT_WINDOW_STRETCH  */
    2,  /* GRAAL_SELECT_WINDOW_MODIFY   */
    2,  /* GRAAL_EDIT_COPY_POINT        */
    2,  /* GRAAL_EDIT_COPY_WINDOW       */
    2,  /* GRAAL_EDIT_MOVE_POINT        */
    2,  /* GRAAL_EDIT_MOVE_WINDOW       */
    2,  /* GRAAL_EDIT_STRETCH_POINT     */
    2,  /* GRAAL_EDIT_STRETCH_WINDOW    */
    1,  /* GRAAL_TOOLS_EQUI             */
    2,  /* GRAAL_TOOLS_FLATTEN          */
    2,  /* GRAAL_TOOLS_UNFLATTEN        */
    2,  /* GRAAL_TOOLS_PEEK             */
    2,  /* GRAAL_TOOLS_DRUC             */
    2,  /* GRAAL_TOOLS_REAL_FLATTEN     */
    1,  /* GRAAL_TOOLS_HIERARCHY_DOWN   */
    1,  /* GRAAL_TOOLS_HIERARCHY_SHOW   */
    2   /* GRAAL_CREATE_BIGVIA          */
  };

  static char GraalInputEventEditTable[] =

  {
    GRAAL_INPUT_HALF_BOX, /* GRAAL_EDIT_MEASURE           */
    GRAAL_INPUT_ORTHO,    /* GRAAL_CREATE_SEGMENT         */
    GRAAL_INPUT_LSTRING,  /* GRAAL_CREATE_SEGMENT_WIRE    */
    GRAAL_INPUT_POINT,    /* GRAAL_CREATE_VIA             */
    GRAAL_INPUT_ORTHO,    /* GRAAL_CREATE_TRANSISTOR      */
    GRAAL_INPUT_LSTRING,  /* GRAAL_CREATE_TRANSISTOR_WIRE */
    GRAAL_INPUT_POINT,    /* GRAAL_CREATE_CONNECTOR       */
    GRAAL_INPUT_POINT,    /* GRAAL_CREATE_INSTANCE        */
    GRAAL_INPUT_POINT,    /* GRAAL_CREATE_REFERENCE       */
    GRAAL_INPUT_BOX,      /* GRAAL_CREATE_ABUTMENTBOX     */
    GRAAL_INPUT_POINT,    /* GRAAL_SELECT_POINT_IDENTIFY  */
    GRAAL_INPUT_POINT,    /* GRAAL_SELECT_POINT_COPY      */
    GRAAL_INPUT_POINT,    /* GRAAL_SELECT_POINT_MOVE      */ 
    GRAAL_INPUT_POINT,    /* GRAAL_SELECT_POINT_DELETE    */ 
    GRAAL_INPUT_POINT,    /* GRAAL_SELECT_POINT_STRETCH   */ 
    GRAAL_INPUT_POINT,    /* GRAAL_SELECT_POINT_MODIFY    */ 
    GRAAL_INPUT_BOX,      /* GRAAL_SELECT_WINDOW_IDENTIFY */
    GRAAL_INPUT_BOX,      /* GRAAL_SELECT_WINDOW_COPY     */ 
    GRAAL_INPUT_BOX,      /* GRAAL_SELECT_WINDOW_MOVE     */ 
    GRAAL_INPUT_BOX,      /* GRAAL_SELECT_WINDOW_DELETE   */ 
    GRAAL_INPUT_BOX,      /* GRAAL_SELECT_WINDOW_STRETCH  */
    GRAAL_INPUT_BOX,      /* GRAAL_SELECT_WINDOW_MODIFY   */ 
    GRAAL_INPUT_LINE,     /* GRAAL_EDIT_COPY_POINT        */ 
    GRAAL_INPUT_LINE,     /* GRAAL_EDIT_COPY_WINDOW       */ 
    GRAAL_INPUT_LINE,     /* GRAAL_EDIT_MOVE_POINT        */ 
    GRAAL_INPUT_LINE,     /* GRAAL_EDIT_MOVE_WINDOW       */ 
    GRAAL_INPUT_ORTHO,    /* GRAAL_EDIT_STRETCH_POINT     */ 
    GRAAL_INPUT_ORTHO,    /* GRAAL_EDIT_STRETCH_WINDOW    */ 
    GRAAL_INPUT_POINT,    /* GRAAL_TOOLS_EQUI             */
    GRAAL_INPUT_BOX,      /* GRAAL_TOOLS_FLATTEN          */
    GRAAL_INPUT_BOX,      /* GRAAL_TOOLS_UNFLATTEN        */
    GRAAL_INPUT_BOX,      /* GRAAL_TOOLS_PEEK             */
    GRAAL_INPUT_BOX,      /* GRAAL_TOOLS_DRUC             */
    GRAAL_INPUT_BOX,      /* GRAAL_TOOLS_REAL_FLATTEN     */
    GRAAL_INPUT_POINT,    /* GRAAL_TOOLS_HIERARCHY_DOWN   */
    GRAAL_INPUT_POINT,    /* GRAAL_TOOLS_HIERARCHY_SHOW   */
    GRAAL_INPUT_BOX       /* GRAAL_CREATE_BIGVIA          */
  };

  static char GraalMaxEventZoomTable[] =

  {
    1,  /* GRAAL_ZOOM_CENTER            */
    2,  /* GRAAL_ZOOM_IN                */
    2   /* GRAAL_ZOOM_PAN               */
  };

  static char GraalInputEventZoomTable[] =

  {
    GRAAL_INPUT_POINT,  /* GRAAL_ZOOM_CENTER            */
    GRAAL_INPUT_BOX,    /* GRAAL_ZOOM_IN                */
    GRAAL_INPUT_LINE    /* GRAAL_ZOOM_PAN               */
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

  static long     GraalLambdaEventZoomX[ 2 ] = { 0, 0 };
  static long     GraalLambdaEventZoomY[ 2 ] = { 0, 0 };
  static char     GraalMaxEventZoom          = 0;
         char     GraalCountEventZoom        = 0;

  static long     GraalLambdaEventEditX[ 2 ] = { 0, 0 };
  static long     GraalLambdaEventEditY[ 2 ] = { 0, 0 };
  static char     GraalMaxEventEdit          = 2;
         char     GraalCountEventEdit        = 0;

  static char     GraalFirstExpose           = GRAAL_TRUE;

/*------------------------------------------------------------\
|                                                             |
|                          Input                              |
|                                                             |
\------------------------------------------------------------*/

 static char GraalInputMode     = GRAAL_INPUT_HALF_BOX;
 static char GraalSaveInputMode = GRAAL_INPUT_HALF_BOX;
 static char GraalEditMode      = GRAAL_EDIT_MEASURE;
 static char GraalSaveEditMode  = GRAAL_EDIT_MEASURE;

/*------------------------------------------------------------\
|                                                             |
|                          Edit Message                       |
|                                                             |
\------------------------------------------------------------*/

  static void (*GraalPromptMessage)()     = GraalPromptEditMeasure;
  static void (*GraalSavePromptMessage)() = GraalPromptEditMeasure;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     GraalChangeEditMode                     |
|                                                             |
\------------------------------------------------------------*/
 
void GraalChangeEditMode( EditMode , PromptMessage )
 
   char  EditMode;
   void (*PromptMessage)();
{
  rdsbegin();

  if ( EditMode & GRAAL_ZOOM_MARK )
  {
    EditMode = EditMode & ~GRAAL_ZOOM_MARK;

    GraalCountEventZoom = 0;
    GraalMaxEventZoom   = GraalMaxEventZoomTable[ EditMode ];

    if ( ! ( GraalEditMode & GRAAL_ZOOM_MARK ) )
    {
      GraalSaveEditMode      = GraalEditMode;
      GraalSaveInputMode     = GraalInputMode;
      GraalSavePromptMessage = GraalPromptMessage;
    }

    GraalInputMode     = GraalInputEventZoomTable[ EditMode ];
    GraalPromptMessage = PromptMessage;
    GraalEditMode      = EditMode | GRAAL_ZOOM_MARK;

    GraalChangeCursorType( NULL, NULL, 0, GraalInputMode );
  }  
  else
  {
    if ( GraalEditMode & GRAAL_ZOOM_MARK )
    {
      GraalSaveEditMode      = EditMode;
      GraalSaveInputMode     = GraalInputEventEditTable[ EditMode ];
      GraalSavePromptMessage = PromptMessage;
      GraalCountEventEdit    = 0;
      GraalMaxEventEdit      = GraalMaxEventEditTable[ EditMode ];
    }
    else
    {
      if ( GraalEditMode != EditMode )
      {
        GraalCountEventEdit = 0;
        GraalMaxEventEdit   = GraalMaxEventEditTable[ EditMode ];
        GraalEditMode       = EditMode;
        GraalInputMode      = GraalInputEventEditTable[ EditMode ];
        GraalPromptMessage  = PromptMessage;

        GraalChangeCursorType( NULL, NULL, 0, GraalInputMode );
      }
    }

    GraalEditUnselectAll();
  }

  GraalPromptMessage();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalContinueEditMode                     |
|                                                             |
\------------------------------------------------------------*/

void GraalContinueEditMode( EditMode , PromptMessage, CountEvent )

   char  EditMode;
   void (*PromptMessage)();
   char  CountEvent;
{
  rdsbegin();

  if ( GraalEditMode & GRAAL_ZOOM_MARK )
  {
    GraalSaveEditMode      = EditMode;
    GraalSaveInputMode     = GraalInputEventEditTable[ EditMode ];
    GraalSavePromptMessage = PromptMessage;
    GraalCountEventEdit    = CountEvent;
    GraalMaxEventEdit      = GraalMaxEventEditTable[ EditMode ];
  }
  else
  {
    GraalCountEventEdit = CountEvent;
    GraalMaxEventEdit   = GraalMaxEventEditTable[ EditMode ];
    GraalEditMode       = EditMode;
    GraalInputMode      = GraalInputEventEditTable[ EditMode ];
    GraalPromptMessage  = PromptMessage;

    GraalChangeCursorType( GraalLambdaEventEditX,
                           GraalLambdaEventEditY,
                           GraalCountEventEdit,
                           GraalInputMode );

  }

  GraalPromptMessage();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalTreatEventZoom                    |
|                                                             |
\------------------------------------------------------------*/

void GraalTreatEventZoom()

{
  rdsbegin();

  switch ( GraalEditMode )
  {
    case GRAAL_ZOOM_CENTER :
                              
      GraalZoomCenter( GraalLambdaEventZoomX[ 0 ], 
                       GraalLambdaEventZoomY[ 0 ] );
      break;

    case GRAAL_ZOOM_IN     :

      GraalZoomIn( GraalLambdaEventZoomX[ 0 ],
                   GraalLambdaEventZoomY[ 0 ],
                   GraalLambdaEventZoomX[ 1 ],
                   GraalLambdaEventZoomY[ 1 ] );
      break;

    case GRAAL_ZOOM_PAN    :

      GraalZoomPan( GraalLambdaEventZoomX[ 0 ],
                    GraalLambdaEventZoomY[ 0 ],
                    GraalLambdaEventZoomX[ 1 ],
                    GraalLambdaEventZoomY[ 1 ] );
      break;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalTreatEventEdit                    |
|                                                             |
\------------------------------------------------------------*/

void GraalTreatEventEdit()

{ 
  long DeltaX;
  long DeltaY;
  long X1;
  long X2;
  long Y1;
  long Y2;

  rdsbegin();

  X1 = GraalLambdaEventEditX[0];
  X2 = GraalLambdaEventEditX[1];
  Y1 = GraalLambdaEventEditY[0];
  Y2 = GraalLambdaEventEditY[1];

  if ( ( GraalInputMode == GRAAL_INPUT_LSTRING ) || 
       ( GraalInputMode == GRAAL_INPUT_ORTHO   ) ||
       ( GraalInputMode == GRAAL_INPUT_SORTHO  ) )
  {
    DeltaX = X1 - X2;
    DeltaY = Y1 - Y2;

    if ( DeltaX < 0 ) DeltaX = - DeltaX;
    if ( DeltaY < 0 ) DeltaY = - DeltaY;

    if ( DeltaX > DeltaY )
    {
      Y2 = Y1;

      GraalLambdaEventEditX[0] = X2;
      GraalLambdaEventEditY[0] = Y2;
    }  
    else
    {   
      X2 = X1;

      GraalLambdaEventEditX[0] = X2;
      GraalLambdaEventEditY[0] = Y2;
    }
  }

  if ( GraalInputMode == GRAAL_INPUT_LSTRING )
  {   
    GraalChangeCursorType( GraalLambdaEventEditX,
                           GraalLambdaEventEditY,
                           1, GraalInputMode );
 
    GraalCountEventEdit = 1;
  }
  else
  {
    GraalChangeCursorType( NULL, NULL, 0,
                           GraalInputMode );

    GraalCountEventEdit = 0;
  }

  switch ( GraalEditMode )
  {
    case GRAAL_EDIT_MEASURE           :

      break;

    case GRAAL_CREATE_SEGMENT         :
    case GRAAL_CREATE_SEGMENT_WIRE    :

      GraalCreateSegmentMbk( X1, Y1, X2, Y2 );

      break;

    case GRAAL_CREATE_VIA             :

      GraalCreateViaMbk( X1, Y1 );

      break;

    case GRAAL_CREATE_BIGVIA          :

      GraalCreateBigViaMbk( X1, Y1, X2, Y2 );

      break;
 
    case GRAAL_CREATE_TRANSISTOR      :
    case GRAAL_CREATE_TRANSISTOR_WIRE :

      GraalCreateTransistorMbk( X1, Y1, X2, Y2 );

      break;

    case GRAAL_CREATE_CONNECTOR       :

      GraalCreateConnectorMbk( X1, Y1 );

      break;

    case GRAAL_CREATE_REFERENCE       :

      GraalCreateReferenceMbk( X1, Y1 );

      break;

    case GRAAL_CREATE_INSTANCE        :

      GraalCreateInstanceMbk( X1, Y1 );

      break;

    case GRAAL_CREATE_ABUTMENTBOX  :

     GraalCreateAbutmentBoxMbk( X1, Y1, X2, Y2 );

    break;

    case GRAAL_SELECT_POINT_IDENTIFY :
 
      GraalEditSelectPoint( X1, Y1 );
      GraalEditIdentify();

    break;

    case GRAAL_SELECT_WINDOW_IDENTIFY :

      GraalEditSelectFence( X1, Y1, X2, Y2 );
      GraalEditIdentify();

    break;

    case GRAAL_SELECT_POINT_COPY :

      GraalEditSelectPointCopy( X1, Y1 );

    break;

    case GRAAL_SELECT_WINDOW_COPY :

      GraalEditSelectWindowCopy( X1, Y1, X2, Y2 );

    break;

    case GRAAL_EDIT_COPY_POINT :

      GraalEditCopy( X1, Y1, X2, Y2, 0 );

    break;

    case GRAAL_EDIT_COPY_WINDOW :
 
      GraalEditCopy( X1, Y1, X2, Y2, 1 );

    break;

    case GRAAL_SELECT_POINT_MOVE :

      GraalEditSelectPointMove( X1, Y1 );

    break;

    case GRAAL_SELECT_WINDOW_MOVE :

      GraalEditSelectWindowMove( X1, Y1, X2, Y2 );

    break;

    case GRAAL_EDIT_MOVE_POINT :

      GraalEditMove( X1, Y1, X2, Y2, 0 );

    break;

    case GRAAL_EDIT_MOVE_WINDOW :

      GraalEditMove( X1, Y1, X2, Y2, 1 );

    break;

    case GRAAL_SELECT_POINT_DELETE :

      GraalEditSelectPointDelete( X1, Y1 );

    break;

    case GRAAL_SELECT_WINDOW_DELETE :

      GraalEditSelectWindowDelete( X1, Y1, X2, Y2 );

    break;

    case GRAAL_SELECT_POINT_STRETCH :

      GraalEditSelectPointStretch( X1, Y1 );

    break;

    case GRAAL_SELECT_WINDOW_STRETCH :

      GraalEditSelectWindowStretch( X1, Y1, X2, Y2 );

    break;

    case GRAAL_EDIT_STRETCH_POINT :

      GraalEditStretch( X1, Y1, X2, Y2, 0 );

    break;

    case GRAAL_EDIT_STRETCH_WINDOW :

      GraalEditStretch( X1, Y1, X2, Y2, 1 );

    break;

    case GRAAL_SELECT_POINT_MODIFY :

      GraalEditSelectPointModify( X1, Y1 );

    break;

    case GRAAL_SELECT_WINDOW_MODIFY :

      GraalEditSelectWindowModify( X1, Y1, X2, Y2 );

    break;

    case GRAAL_TOOLS_EQUI :

      GraalToolsSelectEqui( X1, Y1 );

    break;

    case GRAAL_TOOLS_PEEK :

       GraalToolsPeek( X1, Y1, X2, Y2 );

    break;

    case GRAAL_TOOLS_FLATTEN :

      GraalToolsSelectFlatten( X1, Y1, X2, Y2 );

    break;

    case GRAAL_TOOLS_UNFLATTEN :

      GraalToolsSelectUnflatten( X1, Y1, X2, Y2 );

    break;

    case GRAAL_TOOLS_DRUC :

      GraalToolsDruc( X1, Y1, X2, Y2 );

    break;

    case GRAAL_TOOLS_REAL_FLATTEN :

      GraalToolsSelectRealFlatten( X1, Y1, X2, Y2 );

    break;

    case GRAAL_TOOLS_HIERARCHY_DOWN :

      GraalToolsSelectHierarchyDown( X1, Y1 );

    break;

    case GRAAL_TOOLS_HIERARCHY_SHOW :

      GraalEditSelectPoint( X1, Y1 );
      GraalToolsHierarchyShow( X1, Y1 );

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

  if ( MouseEvent == GRAAL_LEAVE )
  {
    GraalUndisplayCursor();
    GraalCursorInside = GRAAL_FALSE;
  }
  else
  {
    OldLambdaCursorX = GraalLambdaCursorX;
    OldLambdaCursorY = GraalLambdaCursorY;

    GraalComputeCursor( Event->x, Event->y );

    switch ( MouseEvent )
    {
      case GRAAL_B1DN   : 

        if ( GraalEditMode & GRAAL_ZOOM_MARK )
        {
          if ( GraalCountEventZoom < GraalMaxEventZoom )
          {
            GraalLambdaEventZoomX[ GraalCountEventZoom ] = GraalLambdaCursorX;
            GraalLambdaEventZoomY[ GraalCountEventZoom ] = GraalLambdaCursorY;

            GraalCountEventZoom = GraalCountEventZoom + 1;

            GraalPointCursor();

            if ( GraalCountEventZoom == GraalMaxEventZoom )
            {
              GraalChangeCursorType( NULL, NULL, 0, GraalInputMode );

              GraalTreatEventZoom();

              GraalInputMode      = GraalSaveInputMode;
              GraalEditMode       = GraalSaveEditMode;
              GraalPromptMessage  = GraalSavePromptMessage;
              GraalSaveEditMode   = GRAAL_EDIT_MEASURE;
              GraalSaveInputMode  = GRAAL_INPUT_HALF_BOX;
              GraalCountEventZoom = 0;
              GraalMaxEventZoom   = 0;

              GraalChangeCursorType( GraalLambdaEventEditX,
                                     GraalLambdaEventEditY,
                                     GraalCountEventEdit,
                                     GraalInputMode );

              GraalComputeCursor( Event->x, Event->y );
              GraalDisplayCursor();
            }
          }
        }
        else
        {
          if ( GraalCountEventEdit < GraalMaxEventEdit )
          {
            GraalLambdaEventEditX[ GraalCountEventEdit ] = GraalLambdaCursorX;
            GraalLambdaEventEditY[ GraalCountEventEdit ] = GraalLambdaCursorY;

            GraalCountEventEdit = GraalCountEventEdit + 1;

            GraalPointCursor();

            if ( GraalCountEventEdit == GraalMaxEventEdit )
            {
              GraalTreatEventEdit();

              GraalDisplayCursor();
            }
          }
        }

        GraalPromptMessage();

        break;

      case GRAAL_B3DN   :

        if ( GraalEditMode & GRAAL_ZOOM_MARK )
        {
          if ( GraalCountEventZoom != 0 )
          {
            GraalCountEventZoom = GraalCountEventZoom - 1;

            GraalResetCursor();
          }
          else
          { 
            GraalChangeCursorType( GraalLambdaEventEditX,
                                   GraalLambdaEventEditY,
                                   GraalCountEventEdit,
                                   GraalSaveInputMode );

            GraalInputMode          = GraalSaveInputMode;
            GraalEditMode           = GraalSaveEditMode;
            GraalPromptMessage      = GraalSavePromptMessage;
            GraalSaveEditMode       = GRAAL_EDIT_MEASURE;
            GraalSaveInputMode      = GRAAL_INPUT_HALF_BOX;
            GraalSavePromptMessage  = GraalPromptEditMeasure;
            GraalCountEventZoom     = 0;
            GraalMaxEventZoom       = 0;
          }
        }
        else
        {
          if ( GraalCountEventEdit != 0 )
          {
            GraalCountEventEdit = GraalCountEventEdit - 1;

            GraalResetCursor();
          }
          else
          {
            GraalEditUnselectAll();

            GraalChangeCursorType( NULL, NULL, 0,
                                   GRAAL_INPUT_HALF_BOX );

            GraalInputMode      = GRAAL_INPUT_HALF_BOX;
            GraalEditMode       = GRAAL_EDIT_MEASURE;
            GraalPromptMessage  = GraalPromptEditMeasure;
            GraalCountEventEdit = 0;
            GraalMaxEventEdit   = 2;
          }
        }

        GraalPromptMessage();

        break; 

      case GRAAL_MOTION :

        break;

      case GRAAL_ENTER  : 

        GraalCursorInside = GRAAL_TRUE;
        GraalDisplayCursor();

        break;

      case GRAAL_KEY_UP    : GraalZoomUp();

        break;

      case GRAAL_KEY_DN : GraalZoomDown();

        break;

      case GRAAL_KEY_LEFT : GraalZoomLeft();

        break;

      case GRAAL_KEY_RIGHT : GraalZoomRight();

        break;

      case GRAAL_KEY_MORE : GraalZoomMore();

        break;

      case GRAAL_KEY_LESS : GraalZoomLess();

        break;

      case GRAAL_KEY_FIT : GraalZoomFit();

        break;

      case GRAAL_KEY_REFRESH : GraalZoomRefresh();

        break;

      case GRAAL_KEY_UNZOOM : GraalZoomUndo();

        break;

      case GRAAL_KEY_ZOOM_IN : 

         GraalChangeEditMode( GRAAL_ZOOM_IN, GraalPromptZoomIn );

        break;

      case GRAAL_KEY_CENTER :

         GraalChangeEditMode( GRAAL_ZOOM_CENTER, GraalPromptZoomCenter );

        break;

      case GRAAL_KEY_PAN :

         GraalChangeEditMode( GRAAL_ZOOM_PAN, GraalPromptZoomPan );

        break;

      case GRAAL_KEY_UP_LAYER :
      case GRAAL_KEY_DN_LAYER :

        if ( ! ( GraalEditMode & GRAAL_ZOOM_MARK ) )
        {
          FlagUp = ( MouseEvent == GRAAL_KEY_UP_LAYER );

          switch ( GraalEditMode )
          {
            case GRAAL_CREATE_SEGMENT         :
            case GRAAL_CREATE_SEGMENT_WIRE    : GraalChangeSegmentLayer( FlagUp );
            break;

            case GRAAL_CREATE_VIA             : GraalChangeViaType( FlagUp );
            break;

            case GRAAL_CREATE_BIGVIA          : GraalChangeBigViaType( FlagUp );
            break;

            case GRAAL_CREATE_TRANSISTOR      :
            case GRAAL_CREATE_TRANSISTOR_WIRE : GraalChangeTransistorType( FlagUp );
            break;

            case GRAAL_CREATE_CONNECTOR       : GraalChangeConnectorLayer( FlagUp );
            break;

            case GRAAL_CREATE_REFERENCE       : GraalChangeReferenceType( FlagUp );
            break;
          }

          GraalPromptMessage();
        }

        break;
    }

    if ( MouseEvent >= GRAAL_KEY_UP )
    {
      GraalComputeCursor( Event->x, Event->y );
      GraalDisplayCursor();
    }

    if ( ( OldLambdaCursorX != GraalLambdaCursorX ) ||
         ( OldLambdaCursorY != GraalLambdaCursorY ) )
    {
      GraalDisplayCoordinates(); 
      GraalDisplayCursor();
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
  if ( GraalFirstExpose == GRAAL_FALSE )
  {
    rdsbegin();

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );
    GraalUndisplayCursor();

    GraalResizeGraphicWindow();
    GraalRefreshGraphicWindow( 0, 0,
                               GraalGraphicDx, GraalGraphicDy );
    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );
    GraalDisplayCursor();

    rdsend();
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

  rdsbegin();

  if ( GraalFirstExpose == GRAAL_TRUE )
  {
    GraalFirstExpose = GRAAL_FALSE;

    GraalInitializeGraphicWindow();
    GraalUndisplayCursor();

    GraalLoadConfig( GRAAL_FALSE );

    if ( GraalFigureMbk != (phfig_list *)NULL )
    {
      GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

      GraalAddFigure();
      GraalChangeTopLevelTitle( GraalFigureMbk->NAME );
      GraalInitializeZoom();
      GraalDisplayToolsMessage();
      GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
      GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );
    }
    else
    {
      GraalChangeTopLevelTitle( (char *)NULL );
    }

    GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );
    GraalDisplayCursor();

    /* LUDO: Fed up to Click On IT ...
    GraalEnterPresentPanel();
    */
  }
  else
  {
    GraalUndisplayCursor();
    GraalRefreshGraphicWindow( ExposeEvent->x, ExposeEvent->y,
                               ExposeEvent->width, ExposeEvent->height );
    GraalDisplayCursor();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalInitializeEvent                    |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeEvent()

{
  XtActionsRec NewActions;

  rdsbegin();

  XtVaSetValues( GraalGraphicWindow,
                 XmNtranslations,
                 XtParseTranslationTable( GraalEventTranslation ),
                 NULL
               );

  NewActions.string = "CallbackEvent";
  NewActions.proc   =  CallbackEvent;

  XtAppAddActions( GraalApplication, &NewActions, 1 );

  XtAddCallback( GraalGraphicWindow, 
                 XmNresizeCallback, 
                 CallbackResize, NULL );

  XtAddCallback( GraalGraphicWindow, 
                 XmNexposeCallback, 
                 CallbackExpose, NULL );

  rdsend();
}
