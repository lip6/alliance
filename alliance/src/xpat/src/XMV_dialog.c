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
| Tool    :                   XPAT                           |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
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

  XpatDialogItem XpatViewZoomSetDialog =

  {
    "Zoom set",
    XPAT_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomSetCancel,
    (XtPointer)NULL
  };

  XpatDialogItem XpatViewZoomGotoDialog =

  {
    "Goto X,Y",
    XPAT_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomGotoOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomGotoCancel,
    (XtPointer)NULL
  };

  XpatDialogItem XpatViewMoveSetDialog =
 
  {
    "Move set dx,dy",
    XPAT_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewMoveSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewMoveSetCancel,
    (XtPointer)NULL
  };

  XpatDialogItem XpatViewGridXDialog =

  {
    "Grid X",
    XPAT_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewGridXOk,
    (XtPointer)NULL,
    (void *)CallbackViewGridXCancel,
    (XtPointer)NULL
  };

  XpatDialogItem XpatViewGridYDialog =

  {
    "Grid Y",
    XPAT_DIALOG_PROMPT,
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

void XpatBuildViewDialog()

{
  autbegin();

  XpatBuildDialog( XpatMainWindow, &XpatViewZoomSetDialog  );
  XpatBuildDialog( XpatMainWindow, &XpatViewZoomGotoDialog );
  XpatBuildDialog( XpatMainWindow, &XpatViewMoveSetDialog  );
  XpatBuildDialog( XpatMainWindow, &XpatViewGridXDialog    );
  XpatBuildDialog( XpatMainWindow, &XpatViewGridYDialog    );

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

  XtUnmanageChild( XpatViewZoomSetDialog.WIDGET );

  XpatExitDialog();

  if ( ZoomSet != (char *)NULL )
  {
    ZoomPercent = atoi( ZoomSet );

    if ( ( ZoomPercent < XPAT_PERCENT_ZOOM_MIN ) ||
         ( ZoomPercent > XPAT_PERCENT_ZOOM_MAX ) )
    {
      XpatErrorMessage( XpatMainWindow, "The value must be between 5% and 95% !" );
    }
    else
    {
      XpatPercentZoom = ZoomPercent;
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

  XtUnmanageChild( XpatViewZoomSetDialog.WIDGET );

  XpatExitDialog();

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

  XtUnmanageChild( XpatViewZoomGotoDialog.WIDGET );

  XpatExitDialog();

  if ( ZoomGoto != (char *)NULL )
  {
    for ( Index = 0; ZoomGoto[ Index ]; Index++ )
    {
      if ( ZoomGoto[ Index ] == ',' ) 
      {
        ZoomGoto[ Index ] = '\0';

        if ( Index == 0 ) 
        {
          GotoX = XpatUnitGridX + ( XpatUnitGridDx >> 1 );
        }
        else
        {
          GotoX = atoi( ZoomGoto );
        }

        if ( ZoomGoto[ Index + 1 ] == '\0' )
        {
          GotoY = XpatUnitGridY + ( XpatUnitGridDy >> 1 );
        }
        else
        {
          GotoY = atoi( ZoomGoto + Index + 1 );
          GotoY = GotoY * XPAT_PATTERN_STEP_Y;
        }

        ZoomGoto = (char *)NULL;

        XpatZoomCenter( GotoX, GotoY );

        break;
      }
    }

    if ( ZoomGoto != (char *)NULL )
    {
      XpatErrorMessage( XpatMainWindow, "A coordinate must be specified  !" );
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

  XtUnmanageChild( XpatViewZoomGotoDialog.WIDGET );

  XpatExitDialog();

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

  XtUnmanageChild( XpatViewMoveSetDialog.WIDGET );

  XpatExitDialog();

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

    if ( ( MovePercentX < XPAT_PERCENT_MOVE_MIN ) ||
         ( MovePercentX > XPAT_PERCENT_MOVE_MAX ) ||
         ( MovePercentY < XPAT_PERCENT_MOVE_MIN ) ||
         ( MovePercentY > XPAT_PERCENT_MOVE_MAX ) )
    {
      XpatErrorMessage( XpatMainWindow, "The values must be between 5% and 95% !" );
    }
    else
    {   
      XpatPercentMoveX = MovePercentX;
      XpatPercentMoveY = MovePercentY;
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

  XtUnmanageChild( XpatViewMoveSetDialog.WIDGET );
 
  XpatExitDialog();

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

  XtUnmanageChild( XpatViewGridXDialog.WIDGET );

  XpatExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XpatErrorMessage( XpatMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XpatUnitUserGridDx = GridPercent;
      XpatZoomRefresh();
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

  XtUnmanageChild( XpatViewGridXDialog.WIDGET );
 
  XpatExitDialog();

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

  XtUnmanageChild( XpatViewGridYDialog.WIDGET );

  XpatExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      XpatErrorMessage( XpatMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      XpatUnitUserGridDy = GridPercent;
      XpatZoomRefresh();
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

  XtUnmanageChild( XpatViewGridYDialog.WIDGET );
 
  XpatExitDialog();

  autend();
}
