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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                 Dialog.c                          |
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
# include <stdlib.h>
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

  GraalDialogItem GraalViewZoomSetDialog =

  {
    "Zoom set",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomSetCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalViewZoomGotoDialog =

  {
    "Goto X,Y",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewZoomGotoOk,
    (XtPointer)NULL,
    (void *)CallbackViewZoomGotoCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalViewMoveSetDialog =
 
  {
    "Move set dx,dy",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewMoveSetOk,
    (XtPointer)NULL,
    (void *)CallbackViewMoveSetCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalViewGridXDialog =

  {
    "Grid X",
    GRAAL_DIALOG_PROMPT,
    (Widget)NULL,
    (void *)CallbackViewGridXOk,
    (XtPointer)NULL,
    (void *)CallbackViewGridXCancel,
    (XtPointer)NULL
  };

  GraalDialogItem GraalViewGridYDialog =

  {
    "Grid Y",
    GRAAL_DIALOG_PROMPT,
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

void GraalBuildViewDialog()

{
  rdsbegin();

  GraalBuildDialog( GraalMainWindow, &GraalViewZoomSetDialog  );
  GraalBuildDialog( GraalMainWindow, &GraalViewZoomGotoDialog );
  GraalBuildDialog( GraalMainWindow, &GraalViewMoveSetDialog  );
  GraalBuildDialog( GraalMainWindow, &GraalViewGridXDialog    );
  GraalBuildDialog( GraalMainWindow, &GraalViewGridYDialog    );

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

  XtUnmanageChild( GraalViewZoomSetDialog.WIDGET );

  GraalExitDialog();

  if ( ZoomSet != (char *)NULL )
  {
    ZoomPercent = atoi( ZoomSet );

    if ( ( ZoomPercent < GRAAL_PERCENT_ZOOM_MIN ) ||
         ( ZoomPercent > GRAAL_PERCENT_ZOOM_MAX ) )
    {
      GraalErrorMessage( GraalMainWindow, "The value must be between 5% and 95% !" );
    }
    else
    {
      GraalPercentZoom = ZoomPercent;
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

  XtUnmanageChild( GraalViewZoomSetDialog.WIDGET );

  GraalExitDialog();

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

  XtUnmanageChild( GraalViewZoomGotoDialog.WIDGET );

  GraalExitDialog();

  if ( ZoomGoto != (char *)NULL )
  {
    for ( Index = 0; ZoomGoto[ Index ]; Index++ )
    {
      if ( ZoomGoto[ Index ] == ',' ) 
      {
        ZoomGoto[ Index ] = '\0';

        if ( Index == 0 ) 
        {
          GotoX = GraalLambdaGridX + ( GraalLambdaGridDx >> 1 );
        }
        else
        {
          GotoX = (float)atof( ZoomGoto ) * (float)GRAAL_SCALE;
        }

        if ( ZoomGoto[ Index + 1 ] == '\0' )
        {
          GotoY = GraalLambdaGridY + ( GraalLambdaGridDy >> 1 );
        }
        else
        {
          GotoY = (float)atof( ZoomGoto + Index + 1 ) * (float)GRAAL_SCALE;
        }

        ZoomGoto = (char *)NULL;

        GraalZoomCenter( GotoX, GotoY );

        break;
      }
    }

    if ( ZoomGoto != (char *)NULL )
    {
      GraalErrorMessage( GraalMainWindow, "A coordinate must be specified  !" );
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

  XtUnmanageChild( GraalViewZoomGotoDialog.WIDGET );

  GraalExitDialog();

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
  int   MovePercentX;
  int   MovePercentY;
  int   Index;

  rdsbegin();

  XmStringGetLtoR(  CallData->value,
                    XmSTRING_DEFAULT_CHARSET,
                   &MoveSet );

  XtUnmanageChild( GraalViewMoveSetDialog.WIDGET );

  GraalExitDialog();

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

    if ( ( MovePercentX < GRAAL_PERCENT_MOVE_MIN ) ||
         ( MovePercentX > GRAAL_PERCENT_MOVE_MAX ) ||
         ( MovePercentY < GRAAL_PERCENT_MOVE_MIN ) ||
         ( MovePercentY > GRAAL_PERCENT_MOVE_MAX ) )
    {
      GraalErrorMessage( GraalMainWindow, "The values must be between 5% and 95% !" );
    }
    else
    {   
      GraalPercentMoveX = MovePercentX;
      GraalPercentMoveY = MovePercentY;
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

  XtUnmanageChild( GraalViewMoveSetDialog.WIDGET );
 
  GraalExitDialog();

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

  XtUnmanageChild( GraalViewGridXDialog.WIDGET );

  GraalExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 1 )
    {
      GraalErrorMessage( GraalMainWindow, "The value must be greater than zero !" );
    }
    else
    {   
      GraalLambdaUserGridDx = GridPercent;
      GraalZoomRefresh();
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

  XtUnmanageChild( GraalViewGridXDialog.WIDGET );
 
  GraalExitDialog();

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

  XtUnmanageChild( GraalViewGridYDialog.WIDGET );

  GraalExitDialog();

  if ( GridSet != (char *)NULL )
  {
    GridPercent = atoi( GridSet );

    if ( GridPercent < 1 )
    {
      GraalErrorMessage( GraalMainWindow, "The value must be greater than zero !" );
    }
    else
    {   
      GraalLambdaUserGridDy = GridPercent;
      GraalZoomRefresh();
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

  XtUnmanageChild( GraalViewGridYDialog.WIDGET );
 
  GraalExitDialog();

  rdsend();
}
