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
| Tool    :                   XFSM                           |
|                                                             |
| File    :                 Dialog.c                          |
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
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMV.h"

# include "XMV_dialog.h"
# include "XMV_view.h"

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
|                         Dialog Box                          |
|                                                             |
\------------------------------------------------------------*/

  XfsmDialogItem XfsmViewZoomSetDialog =

  {
    "Zoom set",
    XFSM_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomSetCancel,
    (XtPointer)NULL
  };

  XfsmDialogItem XfsmViewZoomGotoDialog =

  {
    "Goto X,Y",
    XFSM_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomGotoOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomGotoCancel,
    (XtPointer)NULL
  };

  XfsmDialogItem XfsmViewMoveSetDialog =
 
  {
    "Move set dx,dy",
    XFSM_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewMoveSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewMoveSetCancel,
    (XtPointer)NULL
  };

  XfsmDialogItem XfsmViewGridXDialog =

  {
    "Grid X",
    XFSM_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewGridXOk,
    (XtPointer)NULL,
    (void *)CallbackViewGridXCancel,
    (XtPointer)NULL
  };

  XfsmDialogItem XfsmViewGridYDialog =

  {
    "Grid Y",
    XFSM_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewGridYOk,
    (XtPointer)NULL,
    (void *)CallbackViewGridYCancel,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Build Dialog Box                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmBuildViewDialog()

{
  autbegin();

  XfsmBuildDialog( XfsmMainWindow, &XfsmViewZoomSetDialog  );
  XfsmBuildDialog( XfsmMainWindow, &XfsmViewZoomGotoDialog );
  XfsmBuildDialog( XfsmMainWindow, &XfsmViewMoveSetDialog  );
  XfsmBuildDialog( XfsmMainWindow, &XfsmViewGridXDialog    );
  XfsmBuildDialog( XfsmMainWindow, &XfsmViewGridYDialog    );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Dialog                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      CallbackViewZoomSetOk                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewZoomSetOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *ZoomSet;
  int   ZoomPercent;

  autbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &ZoomSet );

  XtUnmanageChild( XfsmViewZoomSetDialog.WIDGET );

  XfsmExitDialog();

  if ( ZoomSet != (char *)NULL )
  {
    ZoomPercent = atoi( ZoomSet );

    if ( ( ZoomPercent < XFSM_PERCENT_ZOOM_MIN ) ||
         ( ZoomPercent > XFSM_PERCENT_ZOOM_MAX ) )
    {
      XfsmErrorMessage( XfsmMainWindow, "The value must be between 5% and 95% !" );
    }
    else
    {
      XfsmPercentZoom = ZoomPercent;
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackViewZoomSetCancel                |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewZoomSetCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  autbegin();

  XtUnmanageChild( XfsmViewZoomSetDialog.WIDGET );

  XfsmExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackViewZoomGotoOk                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewZoomGotoOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *ZoomGoto;
  int   Index;
  long  GotoX;
  long  GotoY;

  autbegin();

  XmStringGetLtoR( CallData->value,
                   XmSTRING_DEFAULT_CHARSET,
                   &ZoomGoto );

  XtUnmanageChild( XfsmViewZoomGotoDialog.WIDGET );

  XfsmExitDialog();

  if ( ZoomGoto != (char *)NULL )
  {
    for ( Index = 0; ZoomGoto[ Index ]; Index++ )
    {
      if ( ZoomGoto[ Index ] == ',' ) 
      {
        ZoomGoto[ Index ] = '\0';

        if ( Index == 0 ) 
        {
          GotoX = XfsmUnitGridX + ( XfsmUnitGridDx >> 1 );
        }
        else
        {
          GotoX = atoi( ZoomGoto );
        }

        if ( ZoomGoto[ Index + 1 ] == '\0' )
        {
          GotoY = XfsmUnitGridY + ( XfsmUnitGridDy >> 1 );
        }
        else
        {
          GotoY = atoi( ZoomGoto + Index + 1 );
        }

        ZoomGoto = (char *)NULL;

        XfsmZoomCenter( GotoX, GotoY );

        break;
      }
    }

    if ( ZoomGoto != (char *)NULL )
    {
      XfsmErrorMessage( XfsmMainWindow, "A coordinate must be specified  !" );
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackViewZoomGotoCancel               |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewZoomGotoCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  autbegin();

  XtUnmanageChild( XfsmViewZoomGotoDialog.WIDGET );

  XfsmExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackViewMoveSetOk                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewMoveSetOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *MoveSet;
  int   MovePercentX;
  int   MovePercentY;
  int   Index;

  autbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &MoveSet );

  XtUnmanageChild( XfsmViewMoveSetDialog.WIDGET );

  XfsmExitDialog();

  if ( MoveSet != (char *)NULL )
  {
    for ( Index = 0; MoveSet[ Index ]; Index++ )
    {
      if ( MoveSet[ Index ] == ',' )
      {
        MoveSet[ Index ] = '\0';

        MovePercentX = atoi( MoveSet );
        MovePercentY = atoi( MoveSet + Index + 1 );
        MoveSet = (char *)NULL;

        break;
      }
    }

    if ( MoveSet != (char *)NULL )
    {
      MovePercentY = MovePercentX = atoi( MoveSet );
    }

    if ( ( MovePercentX < XFSM_PERCENT_MOVE_MIN ) ||
         ( MovePercentX > XFSM_PERCENT_MOVE_MAX ) ||
         ( MovePercentY < XFSM_PERCENT_MOVE_MIN ) ||
         ( MovePercentY > XFSM_PERCENT_MOVE_MAX ) )
    {
      XfsmErrorMessage( XfsmMainWindow, "The values must be between 5% and 95% !" );
    }
    else
    {   
      XfsmPercentMoveX = MovePercentX;
      XfsmPercentMoveY = MovePercentY;
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackViewMoveSetCancel                |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewMoveSetCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  autbegin();

  XtUnmanageChild( XfsmViewMoveSetDialog.WIDGET );
 
  XfsmExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackViewGridXOk                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewGridXOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *GridSet;
  int   GridPercent;

  autbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &GridSet );

  XtUnmanageChild( XfsmViewGridXDialog.WIDGET );

  XfsmExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XfsmErrorMessage( XfsmMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XfsmUnitUserGridDx = GridPercent;
      XfsmZoomRefresh();
    }
  }  

  autend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    CallbackViewGridXCancel                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewGridXCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  autbegin();

  XtUnmanageChild( XfsmViewGridXDialog.WIDGET );
 
  XfsmExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      CallbackViewGridYOk                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewGridYOk( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  char *GridSet;
  int   GridPercent;

  autbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &GridSet );

  XtUnmanageChild( XfsmViewGridYDialog.WIDGET );

  XfsmExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XfsmErrorMessage( XfsmMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XfsmUnitUserGridDy = GridPercent;
      XfsmZoomRefresh();
    }
  }

  autend();
}  

/*------------------------------------------------------------\
|                                                             |
|                    CallbackViewGridYCancel                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackViewGridYCancel( MyWidget, ClientData, CallData )

    Widget                        MyWidget;
    caddr_t                       ClientData;
    XmSelectionBoxCallbackStruct *CallData;
{
  autbegin();

  XtUnmanageChild( XfsmViewGridYDialog.WIDGET );
 
  XfsmExitDialog();

  autend();
}
