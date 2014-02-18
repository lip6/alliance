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
| Tool    :                   XFSM                           |
|                                                             |
| File    :                  dialog.c                         |
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

# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/MessageB.h>
# include <Xm/SelectioB.h>
# include <Xm/SeparatoG.h>
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XTB.h"
# include "XSB.h"
# include "XFS.h"
# include "XTB_dialog.h"

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
  
  static int XfsmLockLoop = 0;

  static XfsmDialogItem XfsmWarningDialog =

  {
    "Warning",
    XFSM_DIALOG_WARNING,
    (Widget)NULL,
    (void *)NULL,
    (XtPointer)NULL,
    (void *)NULL,
    (XtPointer)NULL
  };

  static XfsmDialogItem XfsmErrorDialog =

  {
    "Error",
    XFSM_DIALOG_ERROR,
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
|                 XfsmDestroyDialogCallback                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmDestroyDialogCallback( MyWidget, ClientData, CallData )

    Widget  MyWidget;
    caddr_t ClientData;
    caddr_t CallData;
{
  XfsmLockLoop = 0;
  XfsmExitErrorMessage( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                   XfsmExitDialogCallback                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmExitDialogCallback()

{
  autbegin();

  XfsmExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmBuildDialog                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmBuildDialog( Father, Dialog )

   Widget           Father;
   XfsmDialogItem *Dialog;
{
  Arg      Args[3];
  XmString Label;

  autbegin();

  Label = XmStringCreateLtoR( Dialog->LABEL, 
                              XmSTRING_DEFAULT_CHARSET );

  if ( Dialog->TYPE < XFSM_DIALOG_FILE )
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
    case XFSM_DIALOG_MESSAGE :

       Dialog->WIDGET = 

          XmCreateMessageDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XFSM_DIALOG_ERROR :

       Dialog->WIDGET = 

          XmCreateErrorDialog( Father, Dialog->LABEL, Args, 3);

       break;
  
    case XFSM_DIALOG_INFO :

       Dialog->WIDGET = 

          XmCreateInformationDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XFSM_DIALOG_QUESTION :

       Dialog->WIDGET = 

          XmCreateQuestionDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XFSM_DIALOG_WARNING :

       Dialog->WIDGET = 

          XmCreateWarningDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XFSM_DIALOG_WORKING :

       Dialog->WIDGET = 

          XmCreateWorkingDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XFSM_DIALOG_FILE    :

       Dialog->WIDGET = 

          XmCreateFileSelectionDialog( Father, Dialog->LABEL, Args, 3);

       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_LABEL ) );

       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_TEXT  ) );

       break;
  
    case XFSM_DIALOG_PROMPT  :

       Dialog->WIDGET = 

          XmCreatePromptDialog( Father, Dialog->LABEL, Args, 3);

       XtUnmanageChild( XmSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_HELP_BUTTON ) );
  }

  XmStringFree( Label );

  XtAddCallback (Dialog->WIDGET, XmNdestroyCallback,
                XfsmDestroyDialogCallback, NULL );

  if ( Dialog->TYPE < XFSM_DIALOG_FILE )
  {
    XtUnmanageChild( XmMessageBoxGetChild( Dialog->WIDGET, 
                     XmDIALOG_HELP_BUTTON ) );
  }

  if ( ( Dialog->CALLBACK_OK     == NULL ) &&
       ( Dialog->CALLBACK_CANCEL == NULL ) )
  {
    XtAddCallback( Dialog->WIDGET, XmNokCallback, 
                   XfsmExitDialogCallback, NULL );
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

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmLimitedLoop                       |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                       XfsmLimitedLoop                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmLimitedLoop( MyWidget )

  Widget MyWidget;
{
  Widget        DialogShell, WShell;
  XEvent        Event;
  XAnyEvent    *AnyEvent;
  XtAppContext  Context;

  autbegin();

  Context = XtWidgetToApplicationContext( MyWidget );

  XfsmLockLoop = 1;

  for ( WShell = MyWidget; 
        ! XtIsShell( WShell );
        WShell = XtParent( WShell ) );

  while( XfsmLockLoop == 1 )
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
 
        if ( WShell == DialogShell )
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

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmEnterDialog                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmEnterDialog( Dialog )
 
   XfsmDialogItem *Dialog;
{
  autbegin();

  XtManageChild ( Dialog->WIDGET );

  XfsmLimitedLoop( Dialog->WIDGET );

  XtUnmanageChild( Dialog->WIDGET );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmReEnterDialog                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmReEnterDialog()
{
  XfsmLockLoop = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmExitDialog                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmExitDialog()

{
  XfsmLockLoop = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmWarningMessage                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmWarningMessage( Father, Subject )
 
    Widget  Father;
    char   *Subject;
{
  XmString Label;

  autbegin();

  if ( XfsmWarningDialog.WIDGET == (Widget)NULL )
  {
    XfsmBuildDialog( Father, &XfsmWarningDialog );
  }

  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );

  XtVaSetValues( XfsmWarningDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );

  XmStringFree( Label );

  XfsmEnterDialog( &XfsmWarningDialog );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmErrorMessage                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmErrorMessage( Father, Subject )

   Widget   Father;
   char    *Subject;
{
  XmString Label;

  autbegin();

  if ( XfsmErrorDialog.WIDGET == (Widget)NULL )
  {
    XfsmBuildDialog( Father, &XfsmErrorDialog );
  }

  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );

  XtVaSetValues( XfsmErrorDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );

  XmStringFree( Label );

  XfsmEnterDialog( &XfsmErrorDialog );

  autend();
}
