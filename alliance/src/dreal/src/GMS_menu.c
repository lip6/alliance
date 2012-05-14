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

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMS.h"

# include "GMS_menu.h"
# include "GMS_setup.h"

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

   DrealMenuItem DrealSetupMenu[] =

         {
           {
             "Default Config",
             'D',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackSetupDefaultConfig,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Load Config",
             'L',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackSetupLoadConfig,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Save Config",
             'S',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackSetupSaveConfig,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Informations",
             'I',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackSetupInformations,
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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  CallbackSetupLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackSetupLoadConfig( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealLoadConfig( DREAL_TRUE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackSetupSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackSetupSaveConfig( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealSaveConfig();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackSetupDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackSetupDefaultConfig( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealDefaultConfig();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackSetupInformations                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackSetupInformations( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealDisplayInformations();

  rdsend();
}
