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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                 Dialog.c                          |
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

# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
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

  XschDialogItem XschViewZoomSetDialog =

  {
    "Zoom set",
    XSCH_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomSetCancel,
    (XtPointer)NULL
  };

  XschDialogItem XschViewZoomGotoDialog =

  {
    "Goto X,Y",
    XSCH_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomGotoOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomGotoCancel,
    (XtPointer)NULL
  };

  XschDialogItem XschViewMoveSetDialog =
 
  {
    "Move set dx,dy",
    XSCH_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewMoveSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewMoveSetCancel,
    (XtPointer)NULL
  };

  XschDialogItem XschViewGridXDialog =

  {
    "Grid X",
    XSCH_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewGridXOk,
    (XtPointer)NULL,
    (void *)CallbackViewGridXCancel,
    (XtPointer)NULL
  };

  XschDialogItem XschViewGridYDialog =

  {
    "Grid Y",
    XSCH_DIALOG_PROMPT,
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

void XschBuildViewDialog()

{
  autbegin();

  XschBuildDialog( XschMainWindow, &XschViewZoomSetDialog  );
  XschBuildDialog( XschMainWindow, &XschViewZoomGotoDialog );
  XschBuildDialog( XschMainWindow, &XschViewMoveSetDialog  );
  XschBuildDialog( XschMainWindow, &XschViewGridXDialog    );
  XschBuildDialog( XschMainWindow, &XschViewGridYDialog    );

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

  XtUnmanageChild( XschViewZoomSetDialog.WIDGET );

  XschExitDialog();

  if ( ZoomSet != (char *)NULL )
  {
    ZoomPercent = atoi( ZoomSet );

    if ( ( ZoomPercent < XSCH_PERCENT_ZOOM_MIN ) ||
         ( ZoomPercent > XSCH_PERCENT_ZOOM_MAX ) )
    {
      XschErrorMessage( XschMainWindow, "The value must be between 5% and 95% !" );
    }
    else
    {
      XschPercentZoom = ZoomPercent;
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

  XtUnmanageChild( XschViewZoomSetDialog.WIDGET );

  XschExitDialog();

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

  XtUnmanageChild( XschViewZoomGotoDialog.WIDGET );

  XschExitDialog();

  if ( ZoomGoto != (char *)NULL )
  {
    for ( Index = 0; ZoomGoto[ Index ]; Index++ )
    {
      if ( ZoomGoto[ Index ] == ',' ) 
      {
        ZoomGoto[ Index ] = '\0';

        if ( Index == 0 ) 
        {
          GotoX = XschUnitGridX + ( XschUnitGridDx >> 1 );
        }
        else
        {
          GotoX = atoi( ZoomGoto );
        }

        if ( ZoomGoto[ Index + 1 ] == '\0' )
        {
          GotoY = XschUnitGridY + ( XschUnitGridDy >> 1 );
        }
        else
        {
          GotoY = atoi( ZoomGoto + Index + 1 );
        }

        ZoomGoto = (char *)NULL;

        XschZoomCenter( GotoX, GotoY );

        break;
      }
    }

    if ( ZoomGoto != (char *)NULL )
    {
      XschErrorMessage( XschMainWindow, "A coordinate must be specified  !" );
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

  XtUnmanageChild( XschViewZoomGotoDialog.WIDGET );

  XschExitDialog();

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

  XtUnmanageChild( XschViewMoveSetDialog.WIDGET );

  XschExitDialog();

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

    if ( ( MovePercentX < XSCH_PERCENT_MOVE_MIN ) ||
         ( MovePercentX > XSCH_PERCENT_MOVE_MAX ) ||
         ( MovePercentY < XSCH_PERCENT_MOVE_MIN ) ||
         ( MovePercentY > XSCH_PERCENT_MOVE_MAX ) )
    {
      XschErrorMessage( XschMainWindow, "The values must be between 5% and 95% !" );
    }
    else
    {   
      XschPercentMoveX = MovePercentX;
      XschPercentMoveY = MovePercentY;
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

  XtUnmanageChild( XschViewMoveSetDialog.WIDGET );
 
  XschExitDialog();

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

  XtUnmanageChild( XschViewGridXDialog.WIDGET );

  XschExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XschErrorMessage( XschMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XschUnitUserGridDx = GridPercent;
      XschZoomRefresh();
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

  XtUnmanageChild( XschViewGridXDialog.WIDGET );
 
  XschExitDialog();

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

  XtUnmanageChild( XschViewGridYDialog.WIDGET );

  XschExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XschErrorMessage( XschMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XschUnitUserGridDy = GridPercent;
      XschZoomRefresh();
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

  XtUnmanageChild( XschViewGridYDialog.WIDGET );
 
  XschExitDialog();

  autend();
}
