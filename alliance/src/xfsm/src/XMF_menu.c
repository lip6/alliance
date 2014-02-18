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
| Tool    :                    XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"
# include "XSB.h"
# include "XFS.h"
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

  static int  XfsmFirstFileOpen = 1;

/*------------------------------------------------------------\
|                                                             |
|                            Menu                             |
|                                                             |
\------------------------------------------------------------*/

   XfsmMenuItem XfsmFileMenu[] =

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
             (XfsmMenuItem *)NULL
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
             (XfsmMenuItem *)NULL
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

  if ( XfsmFirstFileOpen )
  {
    XfsmFirstFileOpen = 0;

    if ( XfsmFileFilter[2] == '\0' )
    {
      strcat( XfsmFileFilter, FSM_IN    );
      strcat( XfsmFileExtention, FSM_IN );
    }

    Filter = XmStringCreateSimple( XfsmFileFilter ); 

    XtVaSetValues( XfsmFileOpenDialog.WIDGET, 
                   XmNpattern, Filter, NULL);

    XmStringFree( Filter );
  }

  XfsmEnterDialog( &XfsmFileOpenDialog );

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

  XfsmEnterDialog( &XfsmFileQuitDialog );

  autend();
}
