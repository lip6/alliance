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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Motif.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
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
# include <Xm/Form.h>
# include <Xm/Frame.h>
# include <Xm/MainW.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GMS.h"

# include "GMX_motif.h"
# include "GMX_graphic.h" 
# include "GMX_event.h" 
# include "GMX_message.h" 
# include "GMX_menu.h" 
# include "GMX_color.h" 
# include "GMX_panel.h"
# include "GMX_dialog.h"

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

  XtAppContext  GraalApplication;
  Widget        GraalTopLevel;
  Widget        GraalMainWindow;
  Widget        GraalMainForm;
 
  static String GraalFallBacks [] =

         {
           "*.GraalMainWindow.iconName : Graal",
           "*GraalMenuBar*fontList     : -*-helvetica-bold-r-*--14-*", 
           "*foreground                : black",
           "*background                : gray",
           "*fontList                  : -*-helvetica-medium-r-*--12-*",
           "*pathMode                  : XmPATH_MODE_RELATIVE",
           NULL
         };

  static char GraalTopLevelTitle[ 128 ] = "Graal : ";

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    GraalChangeTopLevelTitle                 |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeTopLevelTitle( Title )

  char *Title;
{
  rdsbegin();

  if ( Title != (char *)NULL )
  {
    strcpy( GraalTopLevelTitle + 8, Title  );
  }
  else
  {
    GraalTopLevelTitle[ 8 ] = '\0';
  }

  XtVaSetValues( GraalTopLevel, XmNtitle, GraalTopLevelTitle, NULL );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeRessources( argc, argv )

     int  *argc;
     char *argv[];
{
  Arg Args[ 2 ];

  rdsbegin();

  GraalTopLevel = XtVaAppInitialize(
                                     &GraalApplication,
                                     "graal.xrdb",
                                     NULL,
                                     0,
                                     argc,
                                     argv,
                                     GraalFallBacks,
                                     NULL 
                                   );
  GraalLoadTopLevelConfig();

  GraalInitializeColorMap();

  GraalMainWindow = XtVaCreateManagedWidget( "GraalMainWindow",
                                             xmMainWindowWidgetClass,
                                             GraalTopLevel,
                                             NULL);

  GraalMainForm = XtVaCreateManagedWidget( "GraalMainForm",
                                           xmFormWidgetClass,
                                           GraalMainWindow,
                                           XmNfractionBase,
                                           200,
                                           NULL );
  GraalInitializeMenu();
  GraalInitializeMessage();
  GraalInitializePanel(); 
  GraalInitializeGraphic();
  GraalInitializeEvent();
  GraalBuildPanelMap();
  GraalInitializeMapEvent();
  GraalInitializeColors();
  GraalInitializeGraphicContext();
  GraalInitializeDialog();
  GraalBuildPanelLibrary();
  GraalInitializeIcon();

  XtSetArg( Args[0], XmNallowShellResize, True ); 
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING );

  XtSetValues( GraalTopLevel, Args, 2 );

  XtRealizeWidget( GraalTopLevel );

  rdsend();
}
