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
| Tool    :                   XPAT                            |
|                                                             |
| File    :                   Panel.c                         |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
# include "XMX.h"
# include "XTB.h"
# include "XME.h"

# include "XME_panel.h"
# include "XME_message.h"
# include "XME_dialog.h"
# include "XME_edit.h"
# include "XME_search.h"

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
|                        Search View Panel                    |
|                                                             |
\------------------------------------------------------------*/

   static char *XpatEditSearchViewButtonName[] =

   {
     "Continue",
     "Abort"
   };

   static XpatPanelButtonItem XpatEditSearchViewButton[] =

         {
           {
             &XpatEditSearchViewButtonName[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackEditSearchViewContinue,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XpatEditSearchViewButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackEditSearchViewAbort,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XpatPanelItem XpatEditSearchViewPanel  =

         {
           "View search",
           1,
           0,
           XPAT_EDIT_VIEW_SEARCH_X,
           XPAT_EDIT_VIEW_SEARCH_Y,
           100,
           50,
           1,
           2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XpatEditSearchViewButton
         };

  int XpatEditSearchViewDefaultValues[ 5 ] =

         {
           XPAT_EDIT_VIEW_SEARCH_X,
           XPAT_EDIT_VIEW_SEARCH_Y,
           100, 50, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                       Panel Identify                        |
|                                                             |
\------------------------------------------------------------*/

   static char *XpatPanelIdentifyButtonName[] =

   {
     "Text",
     "Close"
   };

 XpatPanelButtonItem XpatEditIdentifyButton[] =

         {
           {
             &XpatPanelIdentifyButtonName[0],
             "Nothing", NULL, 0, 0,
             NULL,
             NULL,
              0, 0,
              8, 9,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XpatPanelIdentifyButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             3, 9,
             2, 1,
             CallbackEditCloseIdentify,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XpatPanelItem XpatEditIdentifyPanel =

         {
           "Identify",
           1,
           0,
           XPAT_EDIT_IDENTIFY_X,
           XPAT_EDIT_IDENTIFY_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XpatEditIdentifyButton
         };

   int XpatEditIdentifyDefaultValues[ 5 ] =

         {
           XPAT_EDIT_IDENTIFY_X,
           XPAT_EDIT_IDENTIFY_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Select Panel                         |
|                                                             |
\------------------------------------------------------------*/

   static char *XpatEditSelectButtonName[] =

   {
     "Select",
     "Accept",
     "Cancel"
   };

   XpatPanelButtonItem XpatEditSelectButton[] =

         {
           {
             &XpatEditSelectButtonName[0],
             "List",
             "List", 0, 0,
             NULL,
             NULL,
             0, 0,
             8, 9,
             CallbackEditSelectList,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             &XpatEditSelectButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 9,
             2, 1,
             CallbackEditSelectAccept,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XpatEditSelectButtonName[2],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             5, 9,
             2, 1,
             CallbackEditSelectCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XpatPanelItem XpatEditSelectPanel =

         {
           "Select",
           1,
           0,
           XPAT_EDIT_SELECT_X,
           XPAT_EDIT_SELECT_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XpatEditSelectButton
         };

  int XpatEditSelectDefaultValues[ 5 ] =

         {
           XPAT_EDIT_SELECT_X,
           XPAT_EDIT_SELECT_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                    Callback For View Search                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  CallbackEditSearchViewContinue             |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSearchViewContinue( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XpatExitDialog();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackEditSearchViewAbort                |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSearchViewAbort( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XpatSearchAbort = XPAT_TRUE;

  XpatExitDialog();

  autbegin();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Identify                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackEditCloseIdentify               |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditCloseIdentify( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XpatExitPanel( &XpatEditIdentifyPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatDisplayEditIdentify                  |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayEditIdentify( Message )

  char *Message;
{
  autbegin();

  XmTextSetString( XpatEditIdentifyButton[0].BUTTON, Message );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Select                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSelectList                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSelectList(  MyWidget, ClientData, CallData )

     Widget                MyWidget;
     caddr_t               ClientData;
     XmListCallbackStruct *CallData;
{
  autbegin();

  if ( ClientData == (caddr_t)NULL )
  {
    CallbackEditSelectAccept( MyWidget, (caddr_t)NULL, (caddr_t)NULL );
  }
  else
  {
    XpatEditSelectObject( CallData->item_position - 1 );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSelectAccept                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSelectAccept(  MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XpatExitPanel( &XpatEditSelectPanel );

  XpatExitDialog();

  XpatEditSelectAccept();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSelectCancel                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSelectCancel(  MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XpatExitPanel( &XpatEditSelectPanel );

  XpatExitDialog();

  XpatEditSelectCancel();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XpatDisplaySelectList                     |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplaySelectList( Message )

  char *Message;
{
  XmString     Label;

  autbegin();

  if ( Message == (char *)NULL )
  {
    XmListDeleteAllItems( XpatEditSelectButton[ 0 ].BUTTON );
  }
  else
  {
    Label = XmStringCreateSimple( Message );
    XmListAddItem( XpatEditSelectButton[ 0 ].BUTTON , Label , 0 );
    XmStringFree( Label );
  }

  autend();
}
