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
| Tool    :                   XGRA                           |
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
# include "XTB.h"
# include "XSB.h"
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
  
  static int XgraLockLoop = 0;

  static XgraDialogItem XgraWarningDialog =

  {
    "Warning",
    XGRA_DIALOG_WARNING,
    (Widget)NULL,
    (void *)NULL,
    (XtPointer)NULL,
    (void *)NULL,
    (XtPointer)NULL
  };

  static XgraDialogItem XgraErrorDialog =

  {
    "Error",
    XGRA_DIALOG_ERROR,
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
|                 XgraDestroyDialogCallback                  |
|                                                             |
\------------------------------------------------------------*/

void XgraDestroyDialogCallback( MyWidget, ClientData, CallData )

    Widget  MyWidget;
    caddr_t ClientData;
    caddr_t CallData;
{
  XgraLockLoop = 0;

  exit(1);
}

/*------------------------------------------------------------\
|                                                             |
|                   XgraExitDialogCallback                   |
|                                                             |
\------------------------------------------------------------*/

void XgraExitDialogCallback()

{
  autbegin();

  XgraExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraBuildDialog                       |
|                                                             |
\------------------------------------------------------------*/

void XgraBuildDialog( Father, Dialog )

   Widget           Father;
   XgraDialogItem *Dialog;
{
  Arg      Args[3];
  XmString Label;

  autbegin();

  Label = XmStringCreateLtoR( Dialog->LABEL, 
                              XmSTRING_DEFAULT_CHARSET );

  if ( Dialog->TYPE < XGRA_DIALOG_FILE )
  {
    XtSetArg( Args[0], XmNmessageString    , Label);
  }
  else
  {
    XtSetArg( Args[0], XmNselectionLabelString, Label);
  }

  XtSetArg( Args[1], XmNdeleteResponse, XmDESTROY );
  XtSetArg( Args[2], XmNtitle, " "                );

  switch ( Dialog->TYPE )
  {
    case XGRA_DIALOG_MESSAGE :

       Dialog->WIDGET = 

          XmCreateMessageDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XGRA_DIALOG_ERROR :

       Dialog->WIDGET = 

          XmCreateErrorDialog( Father, Dialog->LABEL, Args, 3);

       break;
  
    case XGRA_DIALOG_INFO :

       Dialog->WIDGET = 

          XmCreateInformationDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XGRA_DIALOG_QUESTION :

       Dialog->WIDGET = 

          XmCreateQuestionDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XGRA_DIALOG_WARNING :

       Dialog->WIDGET = 

          XmCreateWarningDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XGRA_DIALOG_WORKING :

       Dialog->WIDGET = 

          XmCreateWorkingDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XGRA_DIALOG_FILE    :

       Dialog->WIDGET = 

          XmCreateFileSelectionDialog( Father, Dialog->LABEL, Args, 3);

       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_LABEL ) );

       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_TEXT  ) );

       break;
  
    case XGRA_DIALOG_PROMPT  :

       Dialog->WIDGET = 

          XmCreatePromptDialog( Father, Dialog->LABEL, Args, 3);

       XtUnmanageChild( XmSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_HELP_BUTTON ) );
  }

  XmStringFree( Label );

  XtAddCallback (Dialog->WIDGET, XmNdestroyCallback,
                XgraDestroyDialogCallback, NULL );

  if ( Dialog->TYPE < XGRA_DIALOG_FILE )
  {
    XtUnmanageChild( XmMessageBoxGetChild( Dialog->WIDGET, 
                     XmDIALOG_HELP_BUTTON ) );
  }

  if ( ( Dialog->CALLBACK_OK     == NULL ) &&
       ( Dialog->CALLBACK_CANCEL == NULL ) )
  {
    XtAddCallback( Dialog->WIDGET, XmNokCallback, 
                   XgraExitDialogCallback, NULL );
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
|                      XgraLimitedLoop                       |
|                                                             |
\------------------------------------------------------------*/

void XgraLimitedLoop( MyWidget )

  Widget MyWidget;
{
  Widget        DialogShell, WShell;
  XEvent        Event;
  XAnyEvent    *AnyEvent;
  XtAppContext  Context;

  autbegin();

  Context = XtWidgetToApplicationContext( MyWidget );

  XgraLockLoop = 1;

  for ( WShell = MyWidget; 
        ! XtIsShell( WShell );
        WShell = XtParent( WShell ) );

  while( XgraLockLoop == 1 )
  {
    XtAppNextEvent( Context, &Event );

    AnyEvent = (XAnyEvent *)( &Event );

    for ( DialogShell = XtWindowToWidget( AnyEvent->display, AnyEvent->window ); 
         ((DialogShell != NULL ) && (!XtIsShell( DialogShell )));
         DialogShell = XtParent(DialogShell));

    switch ( AnyEvent->type )
    {
      case KeyRelease    :
      case ButtonRelease : 
 
        if (WShell == DialogShell) XtDispatchEvent(&Event);

        break;
 
      case KeyPress      :
      case ButtonPress   :

        if (WShell != DialogShell)
        {
          XBell( AnyEvent->display, 0 );
          XtPopup( WShell, XtGrabNone ); 
        }
        else
        {
          XtDispatchEvent( &Event );
        }
        break;
 
      default :

        XtDispatchEvent( &Event );
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraEnterDialog                        |
|                                                             |
\------------------------------------------------------------*/

void XgraEnterDialog( Dialog )
 
   XgraDialogItem *Dialog;
{
  autbegin();

  XtManageChild ( Dialog->WIDGET );

  XgraLimitedLoop( Dialog->WIDGET );

  XtUnmanageChild( Dialog->WIDGET );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraReEnterDialog                      |
|                                                             |
\------------------------------------------------------------*/

void XgraReEnterDialog()
{
  XgraLockLoop = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraExitDialog                        |
|                                                             |
\------------------------------------------------------------*/

void XgraExitDialog()

{
  XgraLockLoop = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraWarningMessage                      |
|                                                             |
\------------------------------------------------------------*/

void XgraWarningMessage( Father, Subject )
 
    Widget  Father;
    char   *Subject;
{
  XmString Label;

  autbegin();

  if ( XgraWarningDialog.WIDGET == (Widget)NULL )
  {
    XgraBuildDialog( Father, &XgraWarningDialog );
  }

  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );

  XtVaSetValues( XgraWarningDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );

  XmStringFree( Label );

  XgraEnterDialog( &XgraWarningDialog );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraErrorMessage                      |
|                                                             |
\------------------------------------------------------------*/

void XgraErrorMessage( Father, Subject )

   Widget   Father;
   char    *Subject;
{
  XmString Label;

  autbegin();

  if ( XgraErrorDialog.WIDGET == (Widget)NULL )
  {
    XgraBuildDialog( Father, &XgraErrorDialog );
  }

  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );

  XtVaSetValues( XgraErrorDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );

  XmStringFree( Label );

  XgraEnterDialog( &XgraErrorDialog );

  autend();
}
