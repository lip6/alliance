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
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XFS.h"
# include "XMS.h"

# include "XMS_panel.h"

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
|                            Panel                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Panel Informations                       |
|                                                             |
\------------------------------------------------------------*/

   static char *XfsmPanelInformationsButtonName[] =

   {
     "Text",
     "Close"
   };

 XfsmPanelButtonItem XfsmSetupInformationsButton[] =

         {
           {
             &XfsmPanelInformationsButtonName[0],
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
             &XfsmPanelInformationsButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             3, 9,
             2, 1,
             XfsmExitDialogCallback,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           NULL
         };

   XfsmPanelItem XfsmSetupInformationsPanel =

         {
           "Informations",
           1,
           0,
           XFSM_SETUP_INFORMATIONS_X,
           XFSM_SETUP_INFORMATIONS_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XfsmSetupInformationsButton
         };

   int XfsmSetupInformationsDefaultValues[ 5 ] =

         {
           XFSM_SETUP_INFORMATIONS_X,
           XFSM_SETUP_INFORMATIONS_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                Callback For Informations                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   XfsmDisplayInformations                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayInformations()

{
  char *Message;

  autbegin();

  Message = XfsmGetInformations();

  XmTextSetString( XfsmSetupInformationsButton[0].BUTTON, Message );

  XfsmEnterPanel( &XfsmSetupInformationsPanel );

  XfsmLimitedLoop( XfsmSetupInformationsPanel.PANEL );

  XfsmExitPanel( &XfsmSetupInformationsPanel );

  autend();
}
