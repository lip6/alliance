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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Setup.c                          |
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
# include <Xm/PushBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GMF.h"
# include "GME.h"
# include "GMC.h"
# include "GMV.h"
# include "GMT.h"
# include "GMS.h"
# include "GMH.h"

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

void GraalSetPanelValues( Panel, Values )

  GraalPanelItem *Panel;
  int            *Values;
{
  rdsbegin();

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
    GraalEnterPanel( Panel );
  }
  else
  {
    XtUnmanageChild( Panel->PANEL );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Write TopLevel Values                  |
|                                                             |
\------------------------------------------------------------*/

void GraalWriteTopLevelValues()

{
  Dimension Values[5];

  rdsbegin();

  XtVaGetValues( GraalTopLevel,
                 XmNx        , &Values[0],
                 XmNy        , &Values[1],
                 XmNwidth    , &Values[2],
                 XmNheight   , &Values[3],
                 NULL );

  Values[0] = Values[0] - GRAAL_TOPLEVEL_TRANSLATE_X;
  Values[1] = Values[1] - GRAAL_TOPLEVEL_TRANSLATE_Y;
  Values[4] = 1;
  
  fprintf( FileConfig, "VERSION: %s\n", VERSION );
  fprintf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
           Values[0], Values[1], Values[2], Values[3], Values[4] );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Write Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

void GraalWritePanelValues( Panel )

  GraalPanelItem *Panel;
{
  Dimension Values[5];

  rdsbegin();

  if ( Panel->COMPUTE == 0 )
  {
    XtVaGetValues( Panel->PANEL,
                   XmNx        , &Values[0],
                   XmNy        , &Values[1],
                   XmNwidth    , &Values[2],
                   XmNheight   , &Values[3],
                   NULL );

    Values[0] = Values[0] - GRAAL_PANEL_TRANSLATE_X;
    Values[1] = Values[1] - GRAAL_PANEL_TRANSLATE_Y;
  }
  else
  {
    Values[0] = Panel->X;
    Values[1] = Panel->Y;
    Values[2] = Panel->WIDTH;
    Values[3] = Panel->HEIGHT;
  }

  Values[2] /= Panel->COLUMN;
  Values[3] /= Panel->ROW;

  Values[4] = Panel->MANAGED;

  fprintf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
           Values[0], Values[1], Values[2], Values[3], Values[4] );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Read Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

void GraalReadPanelValues( Panel )

  GraalPanelItem *Panel;
{
  int Values[5];

  rdsbegin();

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  Values[2] *= Panel->COLUMN;
  Values[3] *= Panel->ROW;

  GraalSetPanelValues( Panel, Values );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Read  Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

char GraalReadTopLevelValues()

{
  int  Values[5];
  char Version[64];

  rdsbegin();

  fscanf( FileConfig, "VERSION: %s\n", Version );

  if ( strcmp( Version, VERSION ) )
  {
    rdsend();
    return( 0 );
  }

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XtVaSetValues( GraalTopLevel,
                 XmNx        , Values[0],
                 XmNy        , Values[1],
                 XmNwidth    , Values[2],
                 XmNheight   , Values[3],
                 NULL );
  rdsend();
  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                         ReadActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void GraalReadActiveLayers()

{
  char Layer;
  int  Value;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    if ( ( GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != (char *)NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED           ) )
    {
      GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = Value;
    }
    else
    {
      GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = -1;
    }
  }

  for ( Layer = 0; Layer < GRAAL_MAX_ACTIVE_NAME; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    GRAAL_RDS_ACTIVE_NAME_TABLE[ Layer ] = Value;
  }

  fscanf( FileConfig, "FILLMODE: %d\n", &Value );
  
  GRAAL_FILL_MODE = Value;

  fscanf( FileConfig, "INTERFACE: %d\n", &Value );

  GRAAL_INSTANCE_INTERFACE = Value;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        WriteActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void GraalWriteActiveLayers()

{
  char Layer;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n", 
             ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] != 0 ) );
  }

  for ( Layer = 0; Layer < GRAAL_MAX_ACTIVE_NAME; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n", 
             GRAAL_RDS_ACTIVE_NAME_TABLE[ Layer ] );
  }

  fprintf( FileConfig, "FILLMODE: %d\n", 
           GRAAL_FILL_MODE );
  
  fprintf( FileConfig, "INTERFACE: %d\n", 
           GRAAL_INSTANCE_INTERFACE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalDefaultTopLevelValues                |
|                                                             |
\------------------------------------------------------------*/

void GraalDefaultTopLevelValues()

{
  rdsbegin();

  XtVaSetValues( GraalTopLevel,
                 XmNheight   , GRAAL_TOPLEVEL_HEIGHT,
                 XmNwidth    , GRAAL_TOPLEVEL_WIDTH,
                 XmNx        , GRAAL_TOPLEVEL_X,
                 XmNy        , GRAAL_TOPLEVEL_Y,
                 NULL
               );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void GraalDefaultConfig()

{
  char      Layer;

  rdsbegin();

  GraalDefaultTopLevelValues();

  GraalSetPanelValues( &GraalViewArrowsPanel, 
                        GraalViewArrowsDefaultValues );
  GraalSetPanelValues( &GraalViewZoomPanel, 
                        GraalViewZoomDefaultValues );
  GraalSetPanelValues( &GraalViewLayerPanel, 
                        GraalViewLayerDefaultValues );
  GraalSetPanelValues( &GraalViewGridPanel, 
                        GraalViewGridDefaultValues );
  GraalSetPanelValues( &GraalViewMapPanel, 
                        GraalViewMapDefaultValues );
  GraalSetPanelValues( &GraalEditGlobalPanel, 
                        GraalEditGlobalDefaultValues );
  GraalSetPanelValues( &GraalWindowGlobalPanel, 
                        GraalWindowGlobalDefaultValues );
  GraalSetPanelValues( &GraalEditSelectPanel, 
                        GraalEditSelectDefaultValues );
  GraalSetPanelValues( &GraalEditIdentifyPanel, 
                        GraalEditIdentifyDefaultValues );
  GraalSetPanelValues( &GraalEditSearchPanel, 
                        GraalEditSearchDefaultValues );
  GraalSetPanelValues( &GraalEditSearchViewPanel, 
                        GraalEditSearchViewDefaultValues );
  GraalSetPanelValues( &GraalModifySegmentPanel,
                        GraalModifySegmentDefaultValues );
  GraalSetPanelValues( &GraalModifyViaPanel,
                        GraalModifyViaDefaultValues );
  GraalSetPanelValues( &GraalModifyBigViaPanel,
                        GraalModifyBigViaDefaultValues );
  GraalSetPanelValues( &GraalModifyTransistorPanel,
                        GraalModifyTransistorDefaultValues );
  GraalSetPanelValues( &GraalModifyConnectorPanel,
                        GraalModifyConnectorDefaultValues );
  GraalSetPanelValues( &GraalModifyReferencePanel,
                        GraalModifyReferenceDefaultValues );
  GraalSetPanelValues( &GraalModifyInstancePanel,
                        GraalModifyInstanceDefaultValues );
  GraalSetPanelValues( &GraalCreateSegmentPanel,
                        GraalCreateSegmentDefaultValues );
  GraalSetPanelValues( &GraalCreateViaPanel,
                        GraalCreateViaDefaultValues );
  GraalSetPanelValues( &GraalCreateBigViaPanel,
                        GraalCreateBigViaDefaultValues );
  GraalSetPanelValues( &GraalCreateTransistorPanel,
                        GraalCreateTransistorDefaultValues );
  GraalSetPanelValues( &GraalCreateConnectorPanel,
                        GraalCreateConnectorDefaultValues );
  GraalSetPanelValues( &GraalCreateReferencePanel,
                        GraalCreateReferenceDefaultValues );
  GraalSetPanelValues( &GraalCreateInstancePanel,
                        GraalCreateInstanceDefaultValues );
  GraalSetPanelValues( &GraalToolsGlobalPanel,
                        GraalToolsGlobalDefaultValues );
  GraalSetPanelValues( &GraalToolsMessagePanel,
                        GraalToolsMessageDefaultValues );
  GraalSetPanelValues( &GraalToolsDrucPanel,
                        GraalToolsDrucDefaultValues );
  GraalSetPanelValues( &GraalToolsHierarchyPanel,
                        GraalToolsHierarchyDefaultValues );
  GraalSetPanelValues( &GraalSetupInformationsPanel,
                        GraalSetupInformationsDefaultValues );
  GraalSetPanelValues( &GraalHelpPresentPanel,
                        GraalHelpPresentDefaultValues );

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    if ( ( GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != (char *)NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED           ) )
    {
      GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = 1;
    }
    else
    {
      GRAAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = -1;
    }
  }

  for ( Layer = 0; Layer < GRAAL_MAX_ACTIVE_NAME; Layer++ )
  {
    GRAAL_RDS_ACTIVE_NAME_TABLE[ Layer ] = 1;
  }

  GRAAL_FILL_MODE = GRAAL_FILL_MODE_PATTERN;
  GRAAL_INSTANCE_INTERFACE = GRAAL_TRUE;

  GraalInitializeLayer();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalLoadTopLevelConfig                   |
|                                                             |
\------------------------------------------------------------*/

void GraalLoadTopLevelConfig()

{
  rdsbegin();

  FileConfig = fopen( GRAAL_GMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    GraalDefaultTopLevelValues();
  }
  else
  {
    if ( ! GraalReadTopLevelValues() )
    {
      GraalDefaultTopLevelValues();
    }

    fclose( FileConfig );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                          GraalLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void GraalLoadConfig( Message )

  char Message;
{
  rdsbegin();

  FileConfig = fopen( GRAAL_GMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    if ( Message )
    {
      GraalErrorMessage( GraalMainWindow, 
                         "Unable to open config file !" );
    }
    else
    {
      GraalDefaultConfig();
    }
  }
  else
  {
    if ( GraalReadTopLevelValues() )
    {
      GraalReadPanelValues( &GraalViewArrowsPanel        );
      GraalReadPanelValues( &GraalViewZoomPanel          );
      GraalReadPanelValues( &GraalViewLayerPanel         );
      GraalReadPanelValues( &GraalViewGridPanel          );
      GraalReadPanelValues( &GraalViewMapPanel           );
      GraalReadPanelValues( &GraalEditGlobalPanel        );
      GraalReadPanelValues( &GraalWindowGlobalPanel      );
      GraalReadPanelValues( &GraalEditSelectPanel        );
      GraalReadPanelValues( &GraalEditIdentifyPanel      );
      GraalReadPanelValues( &GraalEditSearchPanel        );
      GraalReadPanelValues( &GraalEditSearchViewPanel    );
      GraalReadPanelValues( &GraalModifySegmentPanel     );
      GraalReadPanelValues( &GraalModifyViaPanel         );
      GraalReadPanelValues( &GraalModifyBigViaPanel      );
      GraalReadPanelValues( &GraalModifyTransistorPanel  );
      GraalReadPanelValues( &GraalModifyConnectorPanel   );
      GraalReadPanelValues( &GraalModifyReferencePanel   );
      GraalReadPanelValues( &GraalModifyInstancePanel    );
      GraalReadPanelValues( &GraalCreateSegmentPanel     );
      GraalReadPanelValues( &GraalCreateViaPanel         );
      GraalReadPanelValues( &GraalCreateBigViaPanel      );
      GraalReadPanelValues( &GraalCreateTransistorPanel  );
      GraalReadPanelValues( &GraalCreateConnectorPanel   );
      GraalReadPanelValues( &GraalCreateReferencePanel   );
      GraalReadPanelValues( &GraalCreateInstancePanel    );
      GraalReadPanelValues( &GraalToolsGlobalPanel       );
      GraalReadPanelValues( &GraalToolsMessagePanel      );
      GraalReadPanelValues( &GraalToolsDrucPanel         );
      GraalReadPanelValues( &GraalToolsHierarchyPanel    );
      GraalReadPanelValues( &GraalSetupInformationsPanel );
      GraalReadPanelValues( &GraalHelpPresentPanel       );

      GraalReadActiveLayers();
      GraalInitializeLayer();
    }
    else
    if ( Message )
    {
      GraalErrorMessage( GraalMainWindow,
                         "Bad version, unable to open config file !" );
    }
    else
    {
      GraalDefaultConfig();
    }

    fclose( FileConfig );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                          GraalSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void GraalSaveConfig()

{
  rdsbegin();

  FileConfig = fopen( GRAAL_GMS_FILE_NAME, "w" );

  if ( FileConfig == (FILE *)NULL )
  {
    GraalErrorMessage( GraalMainWindow,
                       "Unable to open config file !" );
  }
  else
  {
    GraalWriteTopLevelValues();

    GraalWritePanelValues( &GraalViewArrowsPanel        );
    GraalWritePanelValues( &GraalViewZoomPanel          );
    GraalWritePanelValues( &GraalViewLayerPanel         );
    GraalWritePanelValues( &GraalViewGridPanel          );
    GraalWritePanelValues( &GraalViewMapPanel           );
    GraalWritePanelValues( &GraalEditGlobalPanel        );
    GraalWritePanelValues( &GraalWindowGlobalPanel      );
    GraalWritePanelValues( &GraalEditSelectPanel        );
    GraalWritePanelValues( &GraalEditIdentifyPanel      );
    GraalWritePanelValues( &GraalEditSearchPanel        );
    GraalWritePanelValues( &GraalEditSearchViewPanel    );
    GraalWritePanelValues( &GraalModifySegmentPanel     );
    GraalWritePanelValues( &GraalModifyViaPanel         );
    GraalWritePanelValues( &GraalModifyBigViaPanel      );
    GraalWritePanelValues( &GraalModifyTransistorPanel  );
    GraalWritePanelValues( &GraalModifyConnectorPanel   );
    GraalWritePanelValues( &GraalModifyReferencePanel   );
    GraalWritePanelValues( &GraalModifyInstancePanel    );
    GraalWritePanelValues( &GraalCreateSegmentPanel     );
    GraalWritePanelValues( &GraalCreateViaPanel         );
    GraalWritePanelValues( &GraalCreateBigViaPanel      );
    GraalWritePanelValues( &GraalCreateTransistorPanel  );
    GraalWritePanelValues( &GraalCreateConnectorPanel   );
    GraalWritePanelValues( &GraalCreateReferencePanel   );
    GraalWritePanelValues( &GraalCreateInstancePanel    );
    GraalWritePanelValues( &GraalToolsGlobalPanel       );
    GraalWritePanelValues( &GraalToolsMessagePanel      );
    GraalWritePanelValues( &GraalToolsDrucPanel         );
    GraalWritePanelValues( &GraalToolsHierarchyPanel    );
    GraalWritePanelValues( &GraalSetupInformationsPanel );
    GraalWritePanelValues( &GraalHelpPresentPanel       );

    GraalWriteActiveLayers();

    fclose( FileConfig );
  }

  rdsend();
}
