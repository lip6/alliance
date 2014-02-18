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
| Tool    :                   XFSM                            |
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
# include <Xm/Text.h>
# include <Xm/List.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
# include "XMX.h"
# include "XTB.h"
# include "XME.h"

# include "XME_panel.h"
# include "XME_message.h"
# include "XME_dialog.h"
# include "XME_edit.h"
# include "XME_select.h"
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

   static char *XfsmEditSearchViewButtonName[] =

   {
     "Continue",
     "Abort"
   };

   static XfsmPanelButtonItem XfsmEditSearchViewButton[] =

         {
           {
             &XfsmEditSearchViewButtonName[0],
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
             &XfsmEditSearchViewButtonName[1],
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
           {
             NULL
           }
         };

   XfsmPanelItem XfsmEditSearchViewPanel  =

         {
           "View search",
           1,
           0,
           XFSM_EDIT_VIEW_SEARCH_X,
           XFSM_EDIT_VIEW_SEARCH_Y,
           100,
           50,
           1,
           2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmEditSearchViewButton
         };

  int XfsmEditSearchViewDefaultValues[ 5 ] =

         {
           XFSM_EDIT_VIEW_SEARCH_X,
           XFSM_EDIT_VIEW_SEARCH_Y,
           100, 50, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                       Panel Identify                        |
|                                                             |
\------------------------------------------------------------*/

   static char *XfsmPanelIdentifyButtonName[] =

   {
     "Text",
     "Close"
   };

 XfsmPanelButtonItem XfsmEditIdentifyButton[] =

         {
           {
             &XfsmPanelIdentifyButtonName[0],
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
             &XfsmPanelIdentifyButtonName[1],
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
           {
             NULL
           }
         };

   XfsmPanelItem XfsmEditIdentifyPanel =

         {
           "Identify",
           1,
           0,
           XFSM_EDIT_IDENTIFY_X,
           XFSM_EDIT_IDENTIFY_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmEditIdentifyButton
         };

   int XfsmEditIdentifyDefaultValues[ 5 ] =

         {
           XFSM_EDIT_IDENTIFY_X,
           XFSM_EDIT_IDENTIFY_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Select Panel                         |
|                                                             |
\------------------------------------------------------------*/

   static char *XfsmEditSelectButtonName[] =

   {
     "Select",
     "Accept",
     "Cancel"
   };

   XfsmPanelButtonItem XfsmEditSelectButton[] =

         {
           {
             &XfsmEditSelectButtonName[0],
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
             &XfsmEditSelectButtonName[1],
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
             &XfsmEditSelectButtonName[2],
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
           {
             NULL
           }
         };

   XfsmPanelItem XfsmEditSelectPanel =

         {
           "Select",
           1,
           0,
           XFSM_EDIT_SELECT_X,
           XFSM_EDIT_SELECT_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmEditSelectButton
         };

  int XfsmEditSelectDefaultValues[ 5 ] =

         {
           XFSM_EDIT_SELECT_X,
           XFSM_EDIT_SELECT_Y,
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

  XfsmExitDialog();

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

  XfsmSearchAbort = XFSM_TRUE;

  XfsmExitDialog();

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

  XfsmExitPanel( &XfsmEditIdentifyPanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmDisplayEditIdentify                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayEditIdentify( Message )

  char *Message;
{
  autbegin();

  XmTextSetString( XfsmEditIdentifyButton[0].BUTTON, Message );

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
    XfsmEditSelectObject( CallData->item_position - 1 );
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

  XfsmExitPanel( &XfsmEditSelectPanel );

  XfsmExitDialog();

  XfsmEditSelectAccept();

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

  XfsmExitPanel( &XfsmEditSelectPanel );

  XfsmExitDialog();

  XfsmEditSelectCancel();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XfsmDisplaySelectList                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplaySelectList( Message )

  char *Message;
{
  XmString     Label;

  autbegin();

  if ( Message == (char *)NULL )
  {
    XmListDeleteAllItems( XfsmEditSelectButton[ 0 ].BUTTON );
  }
  else
  {
    Label = XmStringCreateSimple( Message );
    XmListAddItem( XfsmEditSelectButton[ 0 ].BUTTON , Label , 0 );
    XmStringFree( Label );
  }

  autend();
}
