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
| File    :                 Dialog.c                          |
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
# include <math.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GMV.h"

# include "GMV_dialog.h"
# include "GMV_view.h"

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

  DrealDialogItem DrealViewZoomSetDialog =

  {
    "Zoom set",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomSetCancel,
    (XtPointer)NULL
  };

  DrealDialogItem DrealViewZoomGotoDialog =

  {
    "Goto X,Y",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomGotoOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomGotoCancel,
    (XtPointer)NULL
  };

  DrealDialogItem DrealViewMoveSetDialog =
 
  {
    "Move set",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewMoveSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewMoveSetCancel,
    (XtPointer)NULL
  };

  DrealDialogItem DrealViewGridXDialog =

  {
    "Grid X",
    DREAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewGridXOk,
    (XtPointer)NULL,
    (void *)CallbackViewGridXCancel,
    (XtPointer)NULL
  };

  DrealDialogItem DrealViewGridYDialog =

  {
    "Grid Y",
    DREAL_DIALOG_PROMPT,
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

void DrealBuildViewDialog()

{
  rdsbegin();

  DrealBuildDialog( DrealMainWindow, &DrealViewZoomSetDialog  );
  DrealBuildDialog( DrealMainWindow, &DrealViewZoomGotoDialog );
  DrealBuildDialog( DrealMainWindow, &DrealViewMoveSetDialog  );
  DrealBuildDialog( DrealMainWindow, &DrealViewGridXDialog    );
  DrealBuildDialog( DrealMainWindow, &DrealViewGridYDialog    );

  rdsend();
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

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &ZoomSet );

  XtUnmanageChild( DrealViewZoomSetDialog.WIDGET );

  DrealExitDialog();

  if ( ZoomSet != (char *)NULL )
  {
    ZoomPercent = atoi( ZoomSet );

    if ( ( ZoomPercent < DREAL_PERCENT_ZOOM_MIN ) ||
         ( ZoomPercent > DREAL_PERCENT_ZOOM_MAX ) )
    {
      DrealErrorMessage( DrealMainWindow, "The value must be between 5% and 95% !" );
    }
    else
    {
      DrealPercentZoom = ZoomPercent;
    }
  }

  rdsend();
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
  rdsbegin();

  XtUnmanageChild( DrealViewZoomSetDialog.WIDGET );

  DrealExitDialog();

  rdsend();
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

  rdsbegin();

  XmStringGetLtoR( CallData->value,
                   XmSTRING_DEFAULT_CHARSET,
                   &ZoomGoto );

  XtUnmanageChild( DrealViewZoomGotoDialog.WIDGET );

  DrealExitDialog();

  if ( ZoomGoto != (char *)NULL )
  {
    for ( Index = 0; ZoomGoto[ Index ]; Index++ )
    {
      if ( ZoomGoto[ Index ] == ',' ) 
      {
        ZoomGoto[ Index ] = '\0';

        if ( Index == 0 )
        {
          GotoX = DrealLambdaGridX + ( DrealLambdaGridDx >> 1 );
        }
        else
        {
          GotoX = atof( ZoomGoto ) / DREAL_PHYSICAL_GRID_TO_MICRON;
        }

        if ( ZoomGoto[ Index + 1 ] == '\0' )
        {
          GotoY = DrealLambdaGridY + ( DrealLambdaGridDy >> 1 );
        }
        else
        {
          GotoY = atof( ZoomGoto + Index + 1 ) / DREAL_PHYSICAL_GRID_TO_MICRON;
        }

        ZoomGoto = (char *)NULL;

        DrealZoomCenter( GotoX, GotoY );

        break;
      }
    }

    if ( ZoomGoto != (char *)NULL )
    {
      DrealErrorMessage( DrealMainWindow, "A coordinate must be specified  !" );
    }
  }

  rdsend();
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
  rdsbegin();

  XtUnmanageChild( DrealViewZoomGotoDialog.WIDGET );

  DrealExitDialog();

  rdsend();
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
  int   MovePercent;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &MoveSet );

  XtUnmanageChild( DrealViewMoveSetDialog.WIDGET );

  DrealExitDialog();

  if ( MoveSet != (char *)NULL )
  {
    MovePercent = atoi( MoveSet );

    if ( ( MovePercent < DREAL_PERCENT_MOVE_MIN ) ||
         ( MovePercent > DREAL_PERCENT_MOVE_MAX ) )
    {
      DrealErrorMessage( DrealMainWindow, "The value must be between 5% and 95% !" );
    }
    else
    {   
      DrealPercentMove = MovePercent;
    }
  }

  rdsend();
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
  rdsbegin();

  XtUnmanageChild( DrealViewMoveSetDialog.WIDGET );
 
  DrealExitDialog();

  rdsend();
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

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &GridSet );

  XtUnmanageChild( DrealViewGridXDialog.WIDGET );

  DrealExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      DrealErrorMessage( DrealMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      DrealLambdaUserGridDx = GridPercent;
      DrealZoomRefresh();
    }
  }  

  rdsend();
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
  rdsbegin();

  XtUnmanageChild( DrealViewGridXDialog.WIDGET );
 
  DrealExitDialog();

  rdsend();
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

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &GridSet );

  XtUnmanageChild( DrealViewGridYDialog.WIDGET );

  DrealExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 2 )
    {
      DrealErrorMessage( DrealMainWindow, "The value must be greater than one !" );
    }
    else
    {   
      DrealLambdaUserGridDy = GridPercent;
      DrealZoomRefresh();
    }
  }

  rdsend();
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
  rdsbegin();

  XtUnmanageChild( DrealViewGridYDialog.WIDGET );
 
  DrealExitDialog();

  rdsend();
}
