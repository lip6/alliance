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
| Tool    :                   XVPN                           |
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
# include <stdlib.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
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

  XvpnDialogItem XvpnViewZoomSetDialog =

  {
    "Zoom set",
    XVPN_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomSetCancel,
    (XtPointer)NULL
  };

  XvpnDialogItem XvpnViewZoomGotoDialog =

  {
    "Goto X,Y",
    XVPN_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomGotoOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomGotoCancel,
    (XtPointer)NULL
  };

  XvpnDialogItem XvpnViewMoveSetDialog =
 
  {
    "Move set dx,dy",
    XVPN_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewMoveSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewMoveSetCancel,
    (XtPointer)NULL
  };

  XvpnDialogItem XvpnViewGridXDialog =

  {
    "Grid X",
    XVPN_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewGridXOk,
    (XtPointer)NULL,
    (void *)CallbackViewGridXCancel,
    (XtPointer)NULL
  };

  XvpnDialogItem XvpnViewGridYDialog =

  {
    "Grid Y",
    XVPN_DIALOG_PROMPT,
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

void XvpnBuildViewDialog()

{
  autbegin();

  XvpnBuildDialog( XvpnMainWindow, &XvpnViewZoomSetDialog  );
  XvpnBuildDialog( XvpnMainWindow, &XvpnViewZoomGotoDialog );
  XvpnBuildDialog( XvpnMainWindow, &XvpnViewMoveSetDialog  );
  XvpnBuildDialog( XvpnMainWindow, &XvpnViewGridXDialog    );
  XvpnBuildDialog( XvpnMainWindow, &XvpnViewGridYDialog    );

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

  XtUnmanageChild( XvpnViewZoomSetDialog.WIDGET );

  XvpnExitDialog();

  if ( ZoomSet != (char *)NULL )
  {
    ZoomPercent = atoi( ZoomSet );

    if ( ( ZoomPercent < XVPN_PERCENT_ZOOM_MIN ) ||
         ( ZoomPercent > XVPN_PERCENT_ZOOM_MAX ) )
    {
      XvpnErrorMessage( XvpnMainWindow, "The value must be between 5% and 95% !" );
    }
    else
    {
      XvpnPercentZoom = ZoomPercent;
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

  XtUnmanageChild( XvpnViewZoomSetDialog.WIDGET );

  XvpnExitDialog();

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

  XtUnmanageChild( XvpnViewZoomGotoDialog.WIDGET );

  XvpnExitDialog();

  if ( ZoomGoto != (char *)NULL )
  {
    for ( Index = 0; ZoomGoto[ Index ]; Index++ )
    {
      if ( ZoomGoto[ Index ] == ',' ) 
      {
        ZoomGoto[ Index ] = '\0';

        if ( Index == 0 ) 
        {
          GotoX = XvpnUnitGridX + ( XvpnUnitGridDx >> 1 );
        }
        else
        {
          GotoX = atoi( ZoomGoto );
        }

        if ( ZoomGoto[ Index + 1 ] == '\0' )
        {
          GotoY = XvpnUnitGridY + ( XvpnUnitGridDy >> 1 );
        }
        else
        {
          GotoY = atoi( ZoomGoto + Index + 1 );
        }

        ZoomGoto = (char *)NULL;

        XvpnZoomCenter( GotoX, GotoY );

        break;
      }
    }

    if ( ZoomGoto != (char *)NULL )
    {
      XvpnErrorMessage( XvpnMainWindow, "A coordinate must be specified  !" );
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

  XtUnmanageChild( XvpnViewZoomGotoDialog.WIDGET );

  XvpnExitDialog();

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

  XtUnmanageChild( XvpnViewMoveSetDialog.WIDGET );

  XvpnExitDialog();

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

    if ( ( MovePercentX < XVPN_PERCENT_MOVE_MIN ) ||
         ( MovePercentX > XVPN_PERCENT_MOVE_MAX ) ||
         ( MovePercentY < XVPN_PERCENT_MOVE_MIN ) ||
         ( MovePercentY > XVPN_PERCENT_MOVE_MAX ) )
    {
      XvpnErrorMessage( XvpnMainWindow, "The values must be between 5% and 95% !" );
    }
    else
    {   
      XvpnPercentMoveX = MovePercentX;
      XvpnPercentMoveY = MovePercentY;
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

  XtUnmanageChild( XvpnViewMoveSetDialog.WIDGET );
 
  XvpnExitDialog();

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

  XtUnmanageChild( XvpnViewGridXDialog.WIDGET );

  XvpnExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XvpnErrorMessage( XvpnMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XvpnUnitUserGridDx = GridPercent;
      XvpnZoomRefresh();
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

  XtUnmanageChild( XvpnViewGridXDialog.WIDGET );
 
  XvpnExitDialog();

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

  XtUnmanageChild( XvpnViewGridYDialog.WIDGET );

  XvpnExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XvpnErrorMessage( XvpnMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XvpnUnitUserGridDy = GridPercent;
      XvpnZoomRefresh();
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

  XtUnmanageChild( XvpnViewGridYDialog.WIDGET );
 
  XvpnExitDialog();

  autend();
}
