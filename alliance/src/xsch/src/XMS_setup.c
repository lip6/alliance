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
| File    :                  Setup.c                          |
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
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XMF.h"
# include "XME.h"
# include "XMV.h"
# include "XMT.h"
# include "XMS.h"
# include "XMH.h"

# include "XMS_setup.h"

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
  
  static FILE *FileConfig;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Set Panel New Values                   |
|                                                             |
\------------------------------------------------------------*/

void XschSetPanelValues( Panel, Values )

  XschPanelItem *Panel;
  int            *Values;
{
  autbegin();

  Panel->COMPUTE = 1;
  Panel->X       = Values[0];
  Panel->Y       = Values[1];
  Panel->WIDTH   = Values[2];
  Panel->HEIGHT  = Values[3];
  Panel->MANAGED = Values[4];

  XtVaSetValues( Panel->PANEL,
                 XmNx        , Values[0],
                 XmNy        , Values[1],
                 XmNwidth    , Values[2],
                 XmNheight   , Values[3],
                 NULL );

  if ( Panel->MANAGED )
  {
    XschEnterPanel( Panel );
  }
  else
  {
    XtUnmanageChild( Panel->PANEL );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Write TopLevel Values                  |
|                                                             |
\------------------------------------------------------------*/

void XschWriteTopLevelValues()

{
  Dimension Values[5];

  autbegin();

  XtVaGetValues( XschTopLevel,
                 XmNx        , &Values[0],
                 XmNy        , &Values[1],
                 XmNwidth    , &Values[2],
                 XmNheight   , &Values[3],
                 NULL );

  Values[0] = Values[0] - XSCH_TOPLEVEL_TRANSLATE_X;
  Values[1] = Values[1] - XSCH_TOPLEVEL_TRANSLATE_Y;
  Values[4] = 1;
  
  fprintf( FileConfig, "VERSION: %s\n", VERSION );
  fprintf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
           Values[0], Values[1], Values[2], Values[3], Values[4] );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Write Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

void XschWritePanelValues( Panel )

  XschPanelItem *Panel;
{
  Dimension Values[5];

  autbegin();

  if ( Panel->COMPUTE == 0 )
  {
    XtVaGetValues( Panel->PANEL,
                   XmNx        , &Values[0],
                   XmNy        , &Values[1],
                   XmNwidth    , &Values[2],
                   XmNheight   , &Values[3],
                   NULL );

    Values[0] = Values[0] - XSCH_PANEL_TRANSLATE_X;
    Values[1] = Values[1] - XSCH_PANEL_TRANSLATE_Y;
  }
  else
  {
    Values[0] = Panel->X;
    Values[1] = Panel->Y;
    Values[2] = Panel->WIDTH;
    Values[3] = Panel->HEIGHT;
  }

  Values[4] = Panel->MANAGED;

  fprintf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
           Values[0], Values[1], Values[2], Values[3], Values[4] );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Read Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

void XschReadPanelValues( Panel )

  XschPanelItem *Panel;
{
  int Values[5];

  autbegin();

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XschSetPanelValues( Panel, Values );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Read  Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

char XschReadTopLevelValues()

{
  int  Values[5];
  char Version[64];

  autbegin();

  fscanf( FileConfig, "VERSION: %s\n", Version );

  if ( strcmp( Version, VERSION ) )
  {
    autend();
    return( 0 );
  }

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XtVaSetValues( XschTopLevel,
                 XmNx        , Values[0],
                 XmNy        , Values[1],
                 XmNwidth    , Values[2],
                 XmNheight   , Values[3],
                 NULL );
  autend();
  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                         ReadActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void XschReadActiveLayers()

{
  short Layer;
  int   Value;

  autbegin();

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XSCH_ACTIVE_LAYER_TABLE[ Layer ] = Value;
  }

  for ( Layer = 0; Layer < XSCH_MAX_ACTIVE_NAME; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XSCH_ACTIVE_NAME_TABLE[ Layer ] = Value;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        WriteActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void XschWriteActiveLayers()

{
  int  Layer;

  autbegin();

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XSCH_ACTIVE_LAYER_TABLE[ Layer ] );
  }

  for ( Layer = 0; Layer < XSCH_MAX_ACTIVE_NAME; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XSCH_ACTIVE_NAME_TABLE[ Layer ] );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschDefaultTopLevelValues                |
|                                                             |
\------------------------------------------------------------*/

void XschDefaultTopLevelValues()

{
  autbegin();

  XtVaSetValues( XschTopLevel,
                 XmNheight   , XSCH_TOPLEVEL_HEIGHT,
                 XmNwidth    , XSCH_TOPLEVEL_WIDTH,
                 XmNx        , XSCH_TOPLEVEL_X,
                 XmNy        , XSCH_TOPLEVEL_Y,
                 NULL
               );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XschDefaultConfig()

{
  short Layer;

  autbegin();

  XschDefaultTopLevelValues();

  XschSetPanelValues( &XschViewArrowsPanel, 
                       XschViewArrowsDefaultValues );
  XschSetPanelValues( &XschViewZoomPanel, 
                       XschViewZoomDefaultValues );
  XschSetPanelValues( &XschViewGridPanel, 
                       XschViewGridDefaultValues );
  XschSetPanelValues( &XschViewLayerPanel, 
                       XschViewLayerDefaultValues );
  XschSetPanelValues( &XschViewMapPanel, 
                       XschViewMapDefaultValues );
  XschSetPanelValues( &XschEditSearchViewPanel, 
                       XschEditSearchViewDefaultValues );
  XschSetPanelValues( &XschEditSelectPanel, 
                       XschEditSelectDefaultValues );
  XschSetPanelValues( &XschEditIdentifyPanel, 
                       XschEditIdentifyDefaultValues );
  XschSetPanelValues( &XschToolsMessagePanel, 
                       XschToolsMessageDefaultValues );
  XschSetPanelValues( &XschToolsHierarchyPanel, 
                       XschToolsHierarchyDefaultValues );
  XschSetPanelValues( &XschSetupInformationsPanel,
                       XschSetupInformationsDefaultValues );
  XschSetPanelValues( &XschHelpPresentPanel,
                       XschHelpPresentDefaultValues );

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    XSCH_ACTIVE_LAYER_TABLE[ Layer ] = 1;
  }

  XSCH_ACTIVE_LAYER_TABLE[ XSCH_CONSTRUCTION_LAYER ] = 0;

  for ( Layer = 0; Layer < XSCH_MAX_ACTIVE_NAME; Layer++ )
  {
    XSCH_ACTIVE_NAME_TABLE[ Layer ] = 1;
  }

  XschInitializeLayer();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschLoadTopLevelConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XschLoadTopLevelConfig()

{
  autbegin();

  FileConfig = fopen( XSCH_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    XschDefaultTopLevelValues();
  }
  else
  {
    if ( ! XschReadTopLevelValues() )
    {
      XschDefaultTopLevelValues();
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XschLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XschLoadConfig( Message )

  char Message;
{
  autbegin();

  FileConfig = fopen( XSCH_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    if ( Message )
    {
      XschErrorMessage( XschMainWindow, 
                         "Unable to open config file !" );
    }
    else
    {
      XschDefaultConfig();
    }
  }
  else
  {
    if ( XschReadTopLevelValues() )
    {
      XschReadPanelValues( &XschViewArrowsPanel        );
      XschReadPanelValues( &XschViewZoomPanel          );
      XschReadPanelValues( &XschViewGridPanel          );
      XschReadPanelValues( &XschViewLayerPanel         );
      XschReadPanelValues( &XschViewMapPanel           );
      XschReadPanelValues( &XschEditSearchViewPanel    );
      XschReadPanelValues( &XschEditSelectPanel        );
      XschReadPanelValues( &XschEditIdentifyPanel      );
      XschReadPanelValues( &XschToolsMessagePanel      );
      XschReadPanelValues( &XschToolsHierarchyPanel    );
      XschReadPanelValues( &XschSetupInformationsPanel );
      XschReadPanelValues( &XschHelpPresentPanel       );

      XschReadActiveLayers();
      XschInitializeLayer();
    }
    else
    if ( Message )
    {
      XschErrorMessage( XschMainWindow,
                         "Bad version, unable to open config file !" );
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XschSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XschSaveConfig()

{
  autbegin();

  FileConfig = fopen( XSCH_XMS_FILE_NAME, "w" );

  if ( FileConfig == (FILE *)NULL )
  {
    XschErrorMessage( XschMainWindow,
                       "Unable to open config file !" );
  }
  else
  {
    XschWriteTopLevelValues();

    XschWritePanelValues( &XschViewArrowsPanel        );
    XschWritePanelValues( &XschViewZoomPanel          );
    XschWritePanelValues( &XschViewGridPanel          );
    XschWritePanelValues( &XschViewLayerPanel         );
    XschWritePanelValues( &XschViewMapPanel           );
    XschWritePanelValues( &XschEditSearchViewPanel    );
    XschWritePanelValues( &XschEditSelectPanel        );
    XschWritePanelValues( &XschEditIdentifyPanel      );
    XschWritePanelValues( &XschToolsMessagePanel      );
    XschWritePanelValues( &XschToolsHierarchyPanel    );
    XschWritePanelValues( &XschSetupInformationsPanel );
    XschWritePanelValues( &XschHelpPresentPanel       );

    XschWriteActiveLayers();

    fclose( FileConfig );
  }

  autend();
}
