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
| Tool    :                   XVPN                            |
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
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "XSB.h"
# include "XVP.h"
# include "XTB.h"
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

   static char *XvpnPanelInformationsButtonName[] =

   {
     "Text",
     "Close"
   };

 XvpnPanelButtonItem XvpnSetupInformationsButton[] =

         {
           {
             &XvpnPanelInformationsButtonName[0],
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
             &XvpnPanelInformationsButtonName[1],
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             3, 9,
             2, 1,
             XvpnExitDialogCallback,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           { NULL }
         };

   XvpnPanelItem XvpnSetupInformationsPanel =

         {
           "Informations",
           1,
           0,
           XVPN_SETUP_INFORMATIONS_X,
           XVPN_SETUP_INFORMATIONS_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           XvpnSetupInformationsButton
         };

   int XvpnSetupInformationsDefaultValues[ 5 ] =

         {
           XVPN_SETUP_INFORMATIONS_X,
           XVPN_SETUP_INFORMATIONS_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                Callback For Informations                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   XvpnDisplayInformations                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayInformations()

{
  char *Message;

  autbegin();

  Message = XvpnGetInformations();

  XmTextSetString( XvpnSetupInformationsButton[0].BUTTON, Message );

  XvpnEnterPanel( &XvpnSetupInformationsPanel );

  XvpnLimitedLoop( XvpnSetupInformationsPanel.PANEL );

  XvpnExitPanel( &XvpnSetupInformationsPanel );

  autend();
}
