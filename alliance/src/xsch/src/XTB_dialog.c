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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                  dialog.c                         |
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

# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/MessageB.h>
# include <Xm/SelectioB.h>
# include <Xm/SeparatoG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XTB.h"
# include "XSB.h"

# include "XTB_dialog.h"
# include "XSC_error.h"

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
  
  static XschLockLoop = 0;

  static XschDialogItem XschWarningDialog =

  {
    "Warning",
    XSCH_DIALOG_WARNING,
    (Widget)NULL,
    (void *)NULL,
    (XtPointer)NULL,
    (void *)NULL,
    (XtPointer)NULL
  };

  static XschDialogItem XschErrorDialog =

  {
    "Error",
    XSCH_DIALOG_ERROR,
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
|                 XschDestroyDialogCallback                  |
|                                                             |
\------------------------------------------------------------*/

void XschDestroyDialogCallback( MyWidget, ClientData, CallData )

    Widget  MyWidget;
    caddr_t ClientData;
    caddr_t CallData;
{
  XschLockLoop = 0;
  XschExitErrorMessage( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                   XschExitDialogCallback                   |
|                                                             |
\------------------------------------------------------------*/

void XschExitDialogCallback()

{
  autbegin();

  XschExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschBuildDialog                       |
|                                                             |
\------------------------------------------------------------*/

void XschBuildDialog( Father, Dialog )

   Widget           Father;
   XschDialogItem *Dialog;
{
  Arg      Args[3];
  XmString Label;

  autbegin();

  Label = XmStringCreateLtoR( Dialog->LABEL, 
                              XmSTRING_DEFAULT_CHARSET );

  if ( Dialog->TYPE < XSCH_DIALOG_FILE )
  {
    XtSetArg( Args[0], XmNmessageString    , Label);
  }
  else
  {
    XtSetArg( Args[0], XmNselectionLabelString, Label);
  }

  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING );
  XtSetArg( Args[2], XmNtitle, " "                );

  switch ( Dialog->TYPE )
  {
    case XSCH_DIALOG_MESSAGE :

       Dialog->WIDGET = 

          XmCreateMessageDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XSCH_DIALOG_ERROR :

       Dialog->WIDGET = 

          XmCreateErrorDialog( Father, Dialog->LABEL, Args, 3);

       break;
  
    case XSCH_DIALOG_INFO :

       Dialog->WIDGET = 

          XmCreateInformationDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XSCH_DIALOG_QUESTION :

       Dialog->WIDGET = 

          XmCreateQuestionDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XSCH_DIALOG_WARNING :

       Dialog->WIDGET = 

          XmCreateWarningDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XSCH_DIALOG_WORKING :

       Dialog->WIDGET = 

          XmCreateWorkingDialog( Father, Dialog->LABEL, Args, 3);

       break;

    case XSCH_DIALOG_FILE    :

       Dialog->WIDGET = 

          XmCreateFileSelectionDialog( Father, Dialog->LABEL, Args, 3);

       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_LABEL ) );

       XtUnmanageChild( XmFileSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_FILTER_TEXT  ) );

       break;
  
    case XSCH_DIALOG_PROMPT  :

       Dialog->WIDGET = 

          XmCreatePromptDialog( Father, Dialog->LABEL, Args, 3);

       XtUnmanageChild( XmSelectionBoxGetChild( Dialog->WIDGET, 
                        XmDIALOG_HELP_BUTTON ) );
  }

  XmStringFree( Label );

  XtAddCallback (Dialog->WIDGET, XmNdestroyCallback,
                XschDestroyDialogCallback, NULL );

  if ( Dialog->TYPE < XSCH_DIALOG_FILE )
  {
    XtUnmanageChild( XmMessageBoxGetChild( Dialog->WIDGET, 
                     XmDIALOG_HELP_BUTTON ) );
  }

  if ( ( Dialog->CALLBACK_OK     == NULL ) &&
       ( Dialog->CALLBACK_CANCEL == NULL ) )
  {
    XtAddCallback( Dialog->WIDGET, XmNokCallback, 
                   XschExitDialogCallback, NULL );
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
|                       XschLimitedLoop                       |
|                                                             |
\------------------------------------------------------------*/

void XschLimitedLoop( MyWidget )

  Widget MyWidget;
{
  Widget        DialogShell, WShell;
  XEvent        Event;
  XAnyEvent    *AnyEvent;
  XtAppContext  Context;

  autbegin();

  Context = XtWidgetToApplicationContext( MyWidget );

  XschLockLoop = 1;

  for ( WShell = MyWidget; 
        ! XtIsShell( WShell );
        WShell = XtParent( WShell ) );

  while( XschLockLoop == 1 )
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
          XtDispatchEvent( &Event );
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
|                     XschEnterDialog                        |
|                                                             |
\------------------------------------------------------------*/

void XschEnterDialog( Dialog )
 
   XschDialogItem *Dialog;
{
  autbegin();

  XtManageChild ( Dialog->WIDGET );

  XschLimitedLoop( Dialog->WIDGET );

  XtUnmanageChild( Dialog->WIDGET );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschReEnterDialog                      |
|                                                             |
\------------------------------------------------------------*/

void XschReEnterDialog()
{
  XschLockLoop = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                      XschExitDialog                        |
|                                                             |
\------------------------------------------------------------*/

void XschExitDialog()

{
  XschLockLoop = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                    XschWarningMessage                      |
|                                                             |
\------------------------------------------------------------*/

void XschWarningMessage( Father, Subject )
 
    Widget  Father;
    char   *Subject;
{
  XmString Label;

  autbegin();

  if ( XschWarningDialog.WIDGET == (Widget)NULL )
  {
    XschBuildDialog( Father, &XschWarningDialog );
  }

  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );

  XtVaSetValues( XschWarningDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );

  XmStringFree( Label );

  XschEnterDialog( &XschWarningDialog );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschErrorMessage                      |
|                                                             |
\------------------------------------------------------------*/

void XschErrorMessage( Father, Subject )

   Widget   Father;
   char    *Subject;
{
  XmString Label;

  autbegin();

  if ( XschErrorDialog.WIDGET == (Widget)NULL )
  {
    XschBuildDialog( Father, &XschErrorDialog );
  }

  Label = XmStringCreateLtoR( Subject,
                              XmSTRING_DEFAULT_CHARSET );

  XtVaSetValues( XschErrorDialog.WIDGET,
                 XmNmessageString,
                 Label, NULL );

  XmStringFree( Label );

  XschEnterDialog( &XschErrorDialog );

  autend();
}
