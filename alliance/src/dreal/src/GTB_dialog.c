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
| File    :                  dialog.c                         |
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

# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/MessageB.h>
# include <Xm/SelectioB.h>
# include <Xm/SeparatoG.h>
# include "mut.h"
# include "rds.h"
# include "GTB.h"
# include "GTB_dialog.h"

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
  
  static DrealLockLoop = 0;

  static DrealDialogItem DrealWarningDialog =

  {
    "Warning",
    DREAL_DIALOG_WARNING,
    (Widget)NULL,
    (void *)NULL,
    (XtPointer)NULL,
    (void *)NULL,
    (XtPointer)NULL
  };

  static DrealDialogItem DrealErrorDialog =

  {
    "Error",
    DREAL_DIALOG_ERROR,
    (Widget)NULL,
    (void *)NULL,
    (XtPointer)NULL,
    (void *)NULL,
    (XtPointer)NULL
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 DrealDestroyDialogCallback                  |
|                                                             |
\------------------------------------------------------------*/

void DrealDestroyDialogCallback( MyWidget, ClientData, CallData )

    Widget  MyWidget;
    caddr_t ClientData;
    caddr_t CallData;
{
  DrealLockLoop = 0;
  DrealExitErrorMessage( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealExitDialogCallback                   |
|                                                             |
\------------------------------------------------------------*/

void DrealExitDialogCallback()

{
  rdsbegin();

  DrealExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealBuildDialog                       |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildDialog( Father, Dialog )

   Widget           Father;
   DrealDialogItem *Dialog;
{
  Arg      Args[3];
  XmString Label;

  rdsbegin();
 
  Label = XmStringCreateLtoR( Dialog->LABEL, 
                              XmSTRING_DEFAULT_CHARSET );
 
  if ( Dialog->TYPE < DREAL_DIALOG_FILE )
  {
    XtSetArg( Args[0], XmNmessageString    , Label);
  }
  else
  {
    XtSetArg( Args[0], XmNselectionLabelString, Label);
  }
 
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING );
  XtSetArg( Args[2], XmNtitle, " "                   );
 
  switch ( Dialog->TYPE )
  {
    case DREAL_DIALOG_MESSAGE :
 
       Dialog->WIDGET = 
 
          XmCreateMessageDialog( Father, Dialog->LABEL, Args, 3);
 
       break;
 
    case DREAL_DIALOG_ERROR :
 
       Dialog->WIDGET = 
 
          XmCreateErrorDialog( Father, Dialog->LABEL, Args, 3);
 
       break;
  
    case DREAL_DIALOG_INFO :
 
       Dialog->WIDGET = 
 
          XmCreateInformationDialog( Father, Dialog->LABEL, Args, 3);
 
       break;
 
    case DREAL_DIALOG_QUESTION :
 
       Dialog->WIDGET = 
 
          XmCreateQuestionDialog( Father, Dialog->LABEL, Args, 3);
 
       break;
 
    case DREAL_DIALOG_WARNING :
 
       Dialog->WIDGET = 
 
          XmCreateWarningDialog( Father, Dialog->LABEL, Args, 3);
 
       break;
 
    case DREAL_DIALOG_WORKING :
 
       Dialog->WIDGET = 
 
          XmCreateWorkingDialog( Father, Dialog->LABEL, Args, 3);
 
       break;
 
    case DREAL_DIALOG_FILE    :
 
       Dialog->WIDGET = 
 
          XmCreateFileSelectionDialog( Father, Dialog->LABEL, Args, 3);
 
       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_LABEL ) );
 
       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_TEXT  ) );
       break;
  
    case DREAL_DIALOG_PROMPT  :
 
       Dialog->WIDGET = 
 
          XmCreatePromptDialog( Father, Dialog->LABEL, Args, 3);
 
       XtUnmanageChild( XmSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_HELP_BUTTON ) );
  }
 
  XmStringFree( Label );
 
  XtAddCallback (Dialog->WIDGET, XmNdestroyCallback,
                DrealDestroyDialogCallback, NULL );
 
  if ( Dialog->TYPE < DREAL_DIALOG_FILE )
  {
    XtUnmanageChild( XmMessageBoxGetChild( Dialog->WIDGET, 
                     XmDIALOG_HELP_BUTTON ) );
  }
 
  if ( ( Dialog->CALLBACK_OK     == NULL ) &&
       ( Dialog->CALLBACK_CANCEL == NULL ) )
  {
    XtAddCallback( Dialog->WIDGET, XmNokCallback, 
                   DrealExitDialogCallback, NULL );
  }
  else
  {
    if ( Dialog->CALLBACK_OK == NULL )
    {
      XtUnmanageChild( XmMessageBoxGetChild( Dialog->WIDGET, 
                       XmDIALOG_OK_BUTTON ) );
    }
    else
    {
      XtAddCallback( Dialog->WIDGET, XmNokCallback, 
                     Dialog->CALLBACK_OK, 
                     Dialog->CALLDATA_OK );
    }
  }
 
  if ( Dialog->CALLBACK_CANCEL == NULL )
  {
    XtUnmanageChild( XmMessageBoxGetChild( Dialog->WIDGET, 
                     XmDIALOG_CANCEL_BUTTON ) );
  }
  else
  {
    XtAddCallback( Dialog->WIDGET, XmNcancelCallback, 
                   Dialog->CALLBACK_CANCEL, 
                   Dialog->CALLDATA_CANCEL );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealLimitedLoop                       |
|                                                             |
\------------------------------------------------------------*/

void DrealLimitedLoop( MyWidget )

  Widget MyWidget;
{
  Widget        DialogShell, WShell;
  XEvent        Event;
  XAnyEvent    *AnyEvent;
  XtAppContext  Context;

  rdsbegin();

  Context = XtWidgetToApplicationContext( MyWidget );

  DrealLockLoop = 1;

  for ( WShell = MyWidget; 
        ! XtIsShell( WShell );
        WShell = XtParent( WShell ) );

  while( DrealLockLoop == 1 )
  {
    XtAppNextEvent( Context, &Event );

    AnyEvent = (XAnyEvent *)( &Event );

    for ( DialogShell = XtWindowToWidget( AnyEvent->display, AnyEvent->window ); 
         ((DialogShell != NULL ) && (!XtIsShell( DialogShell )));
         DialogShell = XtParent(DialogShell));

    switch ( AnyEvent->type )
    {
      case ButtonPress   :
      case ButtonRelease :

        if (WShell == DialogShell)
        {
          XtDispatchEventToWidget( 
                XtWindowToWidget( AnyEvent->display, AnyEvent->window ), 
			  &Event );
        }
        break;

      case KeyPress      :
      case KeyRelease    :

        if ( WShell != DialogShell )
        {
          AnyEvent->window = XtWindow( WShell );
        }
 
      default :

        XtDispatchEvent( &Event );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealEnterDialog                        |
|                                                             |
\------------------------------------------------------------*/

void DrealEnterDialog( Dialog )
 
   DrealDialogItem *Dialog;
{
  rdsbegin();

  XtManageChild ( Dialog->WIDGET );

  DrealLimitedLoop( Dialog->WIDGET );

  XtUnmanageChild( Dialog->WIDGET );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealReEnterDialog                      |
|                                                             |
\------------------------------------------------------------*/

void DrealReEnterDialog()
{
  rdsbegin();

  DrealLockLoop = 1;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealExitDialog                        |
|                                                             |
\------------------------------------------------------------*/

void DrealExitDialog()

{
  rdsbegin();

  DrealLockLoop = 0;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealWarningMessage                      |
|                                                             |
\------------------------------------------------------------*/

 void DrealWarningMessage( Father, Subject )
 
    Widget  Father;
    char   *Subject;
{
  XmString Label;

  rdsbegin();
 
  if ( DrealWarningDialog.WIDGET == (Widget)NULL )
  {
    DrealBuildDialog( Father, &DrealWarningDialog );
  }
 
  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );
 
  XtVaSetValues( DrealWarningDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );
 
  XmStringFree( Label );
 
  DrealEnterDialog( &DrealWarningDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealErrorMessage                      |
|                                                             |
\------------------------------------------------------------*/

void DrealErrorMessage( Father, Subject )

   Widget   Father;
   char    *Subject;
{
  XmString Label;

  rdsbegin();
 
  if ( DrealErrorDialog.WIDGET == (Widget)NULL )
  {
    DrealBuildDialog( Father, &DrealErrorDialog );
  }
 
  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );
 
  XtVaSetValues( DrealErrorDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );
 
  XmStringFree( Label );
 
  DrealEnterDialog( &DrealErrorDialog );

  rdsend();
}
