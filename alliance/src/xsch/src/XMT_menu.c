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
| File    :                   Menu.c                          |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XTB.h"
# include "XSC.h"
# include "XMV.h"
# include "XMT.h"
# include "XMX.h"

# include "XMT_menu.h" 
# include "XMT_tools.h" 
# include "XMT_dialog.h"
# include "XMT_message.h" 

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

   XschMenuItem XschToolsMenu[] =

         {
           {
             "Hierarchy",
             'H',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsHierarchy,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Save Xfig file",
             'S',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsSaveXfig,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Message",
             'M',
             "Ctrl<Key>M",
             "Ctrl M",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsMessage,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           NULL
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsMessage                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsMessage( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschEnterPanel( &XschToolsMessagePanel );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackToolsHierarchy                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsHierarchy( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschEnterPanel( &XschToolsHierarchyPanel );

  XschChangeEditMode( XSCH_TOOLS_HIERARCHY_DOWN,
                      XschPromptToolsHierarchyDown );
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackToolsSaveXfig                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsSaveXfig( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  if ( XschFigure != (xschfig_list *)NULL )
  {
    XschEnterDialog( &XschToolsSaveXfigDialog );
  }
  else
  {
    XschErrorMessage( XschMainWindow, "No current figure !" ); 
  }

  autend();
}
