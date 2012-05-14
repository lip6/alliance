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
| File    :                  Motif.c                          |
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

  XtAppContext  DrealApplication;
  Widget        DrealTopLevel;
  Widget        DrealMainWindow;
  Widget        DrealMainForm;
 
  static String DrealFallBacks [] =

         {
           "*.DrealMainWindow.iconName : Dreal",
           "*DrealMenuBar*fontList     : -*-helvetica-bold-r-*--14-*", 
           "*foreground                : black",
           "*background                : gray",
           "*fontList                  : -*-helvetica-medium-r-*--12-*",
           NULL
         };

  static char DrealTopLevelTitle[ 128 ] = "Dreal : ";

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    DrealChangeTopLevelTitle                 |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeTopLevelTitle( Title )

  char *Title;
{
  rdsbegin();

  if ( Title != (char *)NULL )
  {
    strcpy( DrealTopLevelTitle + 8, Title  );
  }
  else
  {
    DrealTopLevelTitle[ 8 ] = '\0';
  }

  XtVaSetValues( DrealTopLevel, XmNtitle, DrealTopLevelTitle, NULL );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealInitializeRessources                 |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeRessources( argc, argv )

     int  *argc;
     char *argv[];
{
  Arg Args[ 2 ];

  rdsbegin();

  DrealTopLevel = XtVaAppInitialize(
                                     &DrealApplication,
                                     "dreal.xrdb",
                                     NULL,
                                     0,
                                     argc,
                                     argv,
                                     DrealFallBacks,
                                     NULL 
                                   );

  DrealLoadTopLevelConfig();

  DrealInitializeColorMap();

  DrealMainWindow = XtVaCreateManagedWidget( "DrealMainWindow",
                                             xmMainWindowWidgetClass,
                                             DrealTopLevel,
                                             NULL);

  DrealMainForm = XtVaCreateManagedWidget( "DrealMainForm",
                                           xmFormWidgetClass,
                                           DrealMainWindow,
                                           XmNfractionBase,
                                           200,
                                           NULL );

  DrealInitializeMenu();
  DrealInitializeMessage();
  DrealInitializePanel(); 
  DrealInitializeGraphic();
  DrealInitializeEvent();
  DrealBuildPanelMap();
  DrealInitializeMapEvent();
  DrealInitializeColors();
  DrealInitializeGraphicContext();
  DrealInitializeDialog();
  DrealBuildPanelLibrary();
  DrealInitializeIcon();

  XtSetArg( Args[0], XmNallowShellResize, True ); 
  XtSetArg( Args[1], XmNdeleteResponse, XmDO_NOTHING );
  
  XtSetValues( DrealTopLevel, Args, 1 );
 
  XtRealizeWidget( DrealTopLevel );

  rdsend();
}
