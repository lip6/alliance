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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Menu.c                          |
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

# include <stdio.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GTB.h"
# include "GRD.h"
# include "GMV.h"
# include "GMT.h"
# include "GMX.h"

# include "GMT_menu.h" 
# include "GMT_tools.h" 
# include "GMT_dialog.h"
# include "GMT_message.h" 

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

   DrealMenuItem DrealToolsMenu[] =

         {
           {
             "Save Image",
             'S',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsSavePixmap,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Flatten",
             'F',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsFlatten,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Message",
             'M',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackToolsMessage,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
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
  rdsbegin();

  DrealEnterPanel( &DrealToolsMessagePanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackToolsFlatten                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsFlatten( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealToolsFlatten();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackToolsSavePixmap                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackToolsSavePixmap( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    DrealEnterDialog( &DrealToolsSavePixmapDialog );
  }
  else
  {
    DrealErrorMessage( DrealMainWindow, "No current figure !" ); 
  }

  rdsend();
}
