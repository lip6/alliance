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
| File    :                  Menu.c                           |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>
# include <Xm/RowColumn.h>

# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XTB.h"
# include "XMF.h"
# include "XME.h"
# include "XMV.h"
# include "XMT.h"
# include "XMS.h"
# include "XMH.h"
# include "XMX.h"

# include "XMX_menu.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  Widget XgraMenuBar;

  XgraMenuItem XgraMainMenu[] =

         {
           {
             "File",
             'F',
             NULL,
             NULL,
             &xmCascadeButtonGadgetClass,
             False,
             False,
             False,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             XgraFileMenu
           }
           ,
           {
             "Edit",
             'E',
             NULL,
             NULL,
             &xmCascadeButtonGadgetClass,
             False,
             False,
             False,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             XgraEditMenu
           }
           ,
           {
             "View",
             'V',
             NULL,
             NULL,
             &xmCascadeButtonGadgetClass,
             False,
             False,
             False,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             XgraViewMenu
           }
           ,
           {
             "Tools",
             'T',
             NULL,
             NULL,
             &xmCascadeButtonGadgetClass,
             False,
             False,
             False,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             XgraToolsMenu
           }
           ,
           {
             "Setup",
             'S',
             NULL,
             NULL,
             &xmCascadeButtonGadgetClass,
             False,
             False,
             False,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             XgraSetupMenu
           }
           ,
           {
             "Help",
             'H',
             NULL,
             NULL,
             &xmCascadeButtonGadgetClass,
             False,
             False,
             True,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             XgraHelpMenu
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
|                      XgraInitializeMenu                    |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeMenu()

{
  autbegin();

  XgraMenuBar = XmCreateMenuBar( XgraMainWindow,
                                 "XgraMenuBar", NULL, 0 );

  XgraBuildMenus( XgraMenuBar, XgraMainMenu );

  XtManageChild( XgraMenuBar );

  autend();
}

