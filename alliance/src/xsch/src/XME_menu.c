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
# include "XME.h"
# include "XMX.h"

# include "XME_menu.h" 
# include "XME_dialog.h" 
# include "XME_edit.h" 
# include "XME_message.h" 

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

   XschMenuItem XschEditMenu[] =

         {
           {
             "Identify",
             'I',
             "Ctrl<Key>I",
             "Ctrl I",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditIdentify,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Connected",
             'C',
             "Ctrl<Key>C",
             "Ctrl C",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditConnected,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             "Search       ",
             'S',
             "Ctrl<Key>S",
             "Ctrl S",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackEditSearch,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XschMenuItem *)NULL
           }
           ,
           {
             NULL
           }
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackEditIdentify                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditIdentify( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschChangeEditMode( XSCH_EDIT_IDENTIFY, 
                      XschPromptEditIdentify );
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackEditConnected                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditConnected( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschChangeEditMode( XSCH_EDIT_CONNECTED,
                      XschPromptEditConnected );
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackEditSearch                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSearch( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XschEnterDialog( &XschSearchObjectDialog );

  autend();
}
