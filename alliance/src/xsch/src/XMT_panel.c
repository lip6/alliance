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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                   Panel.c                         |
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
# include <string.h>

# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>
# include <Xm/Text.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
# include "XMX.h"
# include "XTB.h"
# include "XMT.h"

# include "XMT_panel.h"
# include "XMT_message.h"
# include "XMT_dialog.h"
# include "XMT_tools.h"

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
|                       Panel Message                         |
|                                                             |
\------------------------------------------------------------*/

   static char *XschPanelMessageButtonName[] =

   {
     "Text",
     "Close"
   };

 XschPanelButtonItem XschToolsMessageButton[] =

         {
           {
             &XschPanelMessageButtonName[0],
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
             &XschPanelMessageButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             3, 9,
             2, 1,
             CallbackToolsCloseMessage,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XschPanelItem XschToolsMessagePanel =

         {
           "Message",
           1,
           0,
           XSCH_TOOLS_MESSAGE_X,
           XSCH_TOOLS_MESSAGE_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XschToolsMessageButton
         };

   int XschToolsMessageDefaultValues[ 5 ] =

         {
           XSCH_TOOLS_MESSAGE_X,
           XSCH_TOOLS_MESSAGE_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                    Hierarchy Panel                          |
|                                                             |
\------------------------------------------------------------*/

   static char *XschPanelHierarchyButtonName[] =

   {
     "Go up",
     "Go down",
     "Reload",
     "Close"
   };

 XschPanelButtonItem XschToolsHierarchyButton[] =

         {
           {
             &XschPanelHierarchyButtonName[0],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackToolsHierarchyUp,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XschPanelHierarchyButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackToolsHierarchyDown,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XschPanelHierarchyButtonName[2],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackToolsHierarchyReload,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             &XschPanelHierarchyButtonName[3],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0,  3,
             1,  1,
             CallbackToolsCloseHierarchy,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XschPanelItem XschToolsHierarchyPanel =

         {
           "Hierarchy",
           1,
           0,
           XSCH_TOOLS_HIERARCHY_X,
           XSCH_TOOLS_HIERARCHY_Y, 
           100,
           100,
           1,  
           4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XschToolsHierarchyButton
         };

   int XschToolsHierarchyDefaultValues[ 5 ] =
 
         {
           XSCH_TOOLS_HIERARCHY_X,
           XSCH_TOOLS_HIERARCHY_Y,
           100, 100, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Message                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsCloseMessage                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsCloseMessage( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschExitPanel( &XschToolsMessagePanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XschDisplayToolsMessage                   |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayToolsMessage()
{
  char *Message;

  autbegin();

  Message = XschGetErrorMessage();

  if ( Message != (char *)NULL )
  {
    XmTextSetString( XschToolsMessageButton[0].BUTTON, Message );

    XschEnterPanel( &XschToolsMessagePanel );
  }
  else
  {
    XmTextSetString( XschToolsMessageButton[0].BUTTON, ""      );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Hierarchy                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsHierarchyUp                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchyUp( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschToolsHierarchyUp();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsHierarchyDown                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchyDown( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschChangeEditMode( XSCH_TOOLS_HIERARCHY_DOWN,
                      XschPromptToolsHierarchyDown );
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsHierarchyReload                |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchyReload( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschToolsHierarchyReload();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackToolsCloseHierarchy                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsCloseHierarchy( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschToolsDelAllHierarchy();
  XschExitPanel( &XschToolsHierarchyPanel );

  autend();
}

