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
| Tool    :                   XFSM                           |
|                                                             |
| File    :                  Motif.c                          |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/MainW.h>
 
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMS.h"

# include "XMX_motif.h"
# include "XMX_graphic.h" 
# include "XMX_event.h" 
# include "XMX_message.h" 
# include "XMX_menu.h" 
# include "XMX_color.h" 
# include "XMX_panel.h"
# include "XMX_dialog.h"

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

  XtAppContext  XfsmApplication;
  Widget        XfsmTopLevel;
  Widget        XfsmMainWindow;
  Widget        XfsmMainForm;
 
  static String XfsmFallBacks [] =

         {
           "*.XfsmMainWindow.iconName : Xfsm",
           "*XfsmMenuBar*fontList     : -*-helvetica-bold-r-*--14-*", 
           "*foreground                : black",
           "*background                : gray",
           "*fontList                  : -*-helvetica-medium-r-*--12-*",
           NULL
         };

  static char XfsmTopLevelTitle[ 128 ] = "Xfsm : ";

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XfsmChangeTopLevelTitle                 |
|                                                             |
\------------------------------------------------------------*/

void XfsmChangeTopLevelTitle( Title )

  char *Title;
{
  autbegin();

  if ( Title != (char *)NULL )
  {
    strcpy( XfsmTopLevelTitle + 7, Title  );
  }
  else
  {
    XfsmTopLevelTitle[ 7 ] = '\0';
  }

  XtVaSetValues( XfsmTopLevel, XmNtitle, XfsmTopLevelTitle, NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeRessources( argc, argv )

     int  *argc;
     char *argv[];
{
  Arg Args[ 2 ];

  autbegin();

  XfsmTopLevel = XtVaAppInitialize(
                                     &XfsmApplication,
                                     "xfsm.xrdb",
                                     NULL,
                                     0,
                                     argc,
                                     argv,
                                     XfsmFallBacks,
                                     NULL 
                                   );

  XfsmLoadTopLevelConfig();

  XfsmInitializeColorMap();

  XfsmMainWindow = XtVaCreateManagedWidget( "XfsmMainWindow",
                                             xmMainWindowWidgetClass,
                                             XfsmTopLevel,
                                             NULL);

  XfsmMainForm = XtVaCreateManagedWidget( "XfsmMainForm",
                                           xmFormWidgetClass,
                                           XfsmMainWindow,
                                           XmNfractionBase,
                                           200,
                                           NULL );

  XfsmInitializeIcon();
  XfsmInitializeMenu();
  XfsmInitializeMessage();
  XfsmInitializePanel(); 
  XfsmInitializeGraphic();
  XfsmInitializeEvent();
  XfsmBuildPanelMap();
  XfsmInitializeMapEvent();
  XfsmInitializeColors();
  XfsmInitializeGraphicContext();
  XfsmInitializeDialog();
  XfsmBuildPanelLibrary();

  XtSetArg( Args[0], XmNallowShellResize, True ); 
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING );

  XtSetValues( XfsmTopLevel, Args, 2 );
 
  XtRealizeWidget( XfsmTopLevel );

  autend();
}
