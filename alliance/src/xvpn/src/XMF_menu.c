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
| Tool    :                    XVPN                           |
|                                                             |
| File    :                   Menu.c                          |
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
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "XSB.h"
# include "XVP.h"
# include "XMX.h"
# include "XTB.h"
# include "XMF.h"

# include "XMF_menu.h"
# include "XMF_dialog.h"
# include "XMF_file.h"

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

  static XvpnFirstFileOpen = 1;

/*------------------------------------------------------------\
|                                                             |
|                            Menu                             |
|                                                             |
\------------------------------------------------------------*/

   XvpnMenuItem XvpnFileMenu[] =

         {
           {
             "Open",
             'O',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileOpen,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XvpnMenuItem *)NULL
           }
           ,
           {
             "Quit      ",
             'Q',
             "Ctrl<Key>Q",
             "Ctrl Q",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileQuit,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (XvpnMenuItem *)NULL
           }
           ,
           { NULL }
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileOpen( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Filter;

  autbegin();

  if ( XvpnFirstFileOpen )
  {
    XvpnFirstFileOpen = 0;

    if ( XvpnFileFilter[2] == '\0' )
    {
      strcat( XvpnFileFilter, VPN_IN    );
      strcat( XvpnFileExtention, VPN_IN );
    }

    Filter = XmStringCreateSimple( XvpnFileFilter ); 

    XtVaSetValues( XvpnFileOpenDialog.WIDGET, 
                   XmNpattern, Filter, NULL);

    XmStringFree( Filter );
  }

  XvpnEnterDialog( &XvpnFileOpenDialog );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileQuit                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileQuit( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  autbegin();

  XvpnEnterDialog( &XvpnFileQuitDialog );

  autend();
}
