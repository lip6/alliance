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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                  Motif.c                          |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/MainW.h>
 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
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
# include "XMX_icon.h"
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

  XtAppContext  XschApplication;
  Widget        XschTopLevel;
  Widget        XschMainWindow;
  Widget        XschMainForm;
 
  static String XschFallBacks [] =

         {
           "*.XschMainWindow.iconName : Xsch",
           "*XschMenuBar*fontList     : -*-helvetica-bold-r-*--14-*", 
           "*foreground                : black",
           "*background                : gray",
           "*fontList                  : -*-helvetica-medium-r-*--12-*",
           NULL
         };

  static char XschTopLevelTitle[ 128 ] = "Xsch : ";

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XschChangeTopLevelTitle                 |
|                                                             |
\------------------------------------------------------------*/

void XschChangeTopLevelTitle( Title )

  char *Title;
{
  autbegin();

  if ( Title != (char *)NULL )
  {
    strcpy( XschTopLevelTitle + 7, Title  );
  }
  else
  {
    XschTopLevelTitle[ 7 ] = '\0';
  }

  XtVaSetValues( XschTopLevel, XmNtitle, XschTopLevelTitle, NULL );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeRessources( argc, argv )

     int  *argc;
     char *argv[];
{
  Arg Args[ 2 ];

  autbegin();

  XschTopLevel = XtVaAppInitialize(
                                     &XschApplication,
                                     "xsch.xrdb",
                                     NULL,
                                     0,
                                     argc,
                                     argv,
                                     XschFallBacks,
                                     NULL 
                                   );

  XschLoadTopLevelConfig();

  XschInitializeColorMap();

  XschMainWindow = XtVaCreateManagedWidget( "XschMainWindow",
                                             xmMainWindowWidgetClass,
                                             XschTopLevel,
                                             NULL);

  XschMainForm = XtVaCreateManagedWidget( "XschMainForm",
                                           xmFormWidgetClass,
                                           XschMainWindow,
                                           XmNfractionBase,
                                           200,
                                           NULL );

  XschInitializeIcon();
  XschInitializeMenu();
  XschInitializeMessage();
  XschInitializePanel(); 
  XschInitializeGraphic();
  XschInitializeEvent();
  XschBuildPanelMap();
  XschInitializeMapEvent();
  XschInitializeColors();
  XschInitializeGraphicContext();
  XschInitializeDialog();
  XschBuildPanelLibrary();

  XtSetArg( Args[0], XmNallowShellResize, True ); 
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING );
  XtSetValues( XschTopLevel, Args, 2 );
 
  XtRealizeWidget( XschTopLevel );

  autend();
}
