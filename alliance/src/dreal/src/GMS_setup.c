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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Setup.c                          |
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
# include <Xm/PushBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GMF.h"
# include "GMV.h"
# include "GME.h"
# include "GMC.h"
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

void DrealSetPanelValues( Panel, Values )

  DrealPanelItem *Panel;
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
    DrealEnterPanel( Panel );
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

void DrealWriteTopLevelValues()

{
  Dimension Values[5];

  rdsbegin();

  XtVaGetValues( DrealTopLevel,
                 XmNx        , &Values[0],
                 XmNy        , &Values[1],
                 XmNwidth    , &Values[2],
                 XmNheight   , &Values[3],
                 NULL );

  Values[0] = Values[0] - DREAL_TOPLEVEL_TRANSLATE_X;
  Values[1] = Values[1] - DREAL_TOPLEVEL_TRANSLATE_Y;
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

void DrealWritePanelValues( Panel )

  DrealPanelItem *Panel;
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

    Values[0] = Values[0] - DREAL_PANEL_TRANSLATE_X;
    Values[1] = Values[1] - DREAL_PANEL_TRANSLATE_Y;
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

void DrealReadPanelValues( Panel )

  DrealPanelItem *Panel;
{
  int Values[5];

  rdsbegin();

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  Values[2] *= Panel->COLUMN;
  Values[3] *= Panel->ROW;

  DrealSetPanelValues( Panel, Values );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Read  Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

char DrealReadTopLevelValues()
{
  int Values[5];
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

  XtVaSetValues( DrealTopLevel,
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

void DrealReadActiveLayers()

{
  char Layer;
  int  Value;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    if ( ( DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != (char *)NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED           ) )
    {
      DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = Value;
    }
    else
    {
      DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = -1;
    }
  }

  for ( Layer = 0; Layer < DREAL_MAX_ACTIVE_NAME; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    DREAL_RDS_ACTIVE_NAME_TABLE[ Layer ] = Value;
  }

  fscanf( FileConfig, "FILLMODE: %d\n", &Value );

  DREAL_FILL_MODE = Value;

  fscanf( FileConfig, "INTERFACE: %d\n", &Value );

  DREAL_INSTANCE_INTERFACE = Value;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        WriteActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void DrealWriteActiveLayers()

{
  char Layer;
  int  Value;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             ( DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] != 0 ) );
  }

  for ( Layer = 0; Layer < DREAL_MAX_ACTIVE_NAME; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n", 
             DREAL_RDS_ACTIVE_NAME_TABLE[ Layer ] );
  }

  fprintf( FileConfig, "FILLMODE: %d\n",
           DREAL_FILL_MODE );

  fprintf( FileConfig, "INTERFACE: %d\n",
           DREAL_INSTANCE_INTERFACE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealDefaultTopLevelValues                |
|                                                             |
\------------------------------------------------------------*/

void DrealDefaultTopLevelValues()

{
  rdsbegin();

  XtVaSetValues( DrealTopLevel,
                 XmNheight   , DREAL_TOPLEVEL_HEIGHT,
                 XmNwidth    , DREAL_TOPLEVEL_WIDTH,
                 XmNx        , DREAL_TOPLEVEL_X,
                 XmNy        , DREAL_TOPLEVEL_Y,
                 NULL
               );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void DrealDefaultConfig()

{
  char      Layer;

  rdsbegin();

  DrealDefaultTopLevelValues();

  DrealSetPanelValues( &DrealViewArrowsPanel, 
                        DrealViewArrowsDefaultValues );
  DrealSetPanelValues( &DrealViewZoomPanel, 
                        DrealViewZoomDefaultValues );
  DrealSetPanelValues( &DrealViewLayerPanel, 
                        DrealViewLayerDefaultValues );
  DrealSetPanelValues( &DrealViewGridPanel, 
                        DrealViewGridDefaultValues );
  DrealSetPanelValues( &DrealViewMapPanel, 
                        DrealViewMapDefaultValues );
  DrealSetPanelValues( &DrealCreateRectanglePanel, 
                        DrealCreateRectangleDefaultValues );
  DrealSetPanelValues( &DrealModifyRectanglePanel, 
                        DrealModifyRectangleDefaultValues );
  DrealSetPanelValues( &DrealToolsMessagePanel,
                        DrealToolsMessageDefaultValues );
  DrealSetPanelValues( &DrealHelpPresentPanel,
                        DrealHelpPresentDefaultValues );

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    if ( ( DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != (char *)NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED           ) )
    {
      DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = 1;
    }
    else
    {
      DREAL_RDS_ACTIVE_LAYER_TABLE[ Layer ] = -1;
    }
  }

  for ( Layer = 0; Layer < DREAL_MAX_ACTIVE_NAME; Layer++ )
  {
    DREAL_RDS_ACTIVE_NAME_TABLE[ Layer ] = 1;
  }

  DREAL_FILL_MODE = DREAL_FILL_MODE_PATTERN;
  DREAL_INSTANCE_INTERFACE = DREAL_TRUE;

  DrealInitializeLayer();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealLoadTopLevelConfig                   |
|                                                             |
\------------------------------------------------------------*/

void DrealLoadTopLevelConfig()

{
  rdsbegin();

  FileConfig = fopen( DREAL_GMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    DrealDefaultTopLevelValues();
  }
  else
  {
    if ( ! DrealReadTopLevelValues() )
    {
      DrealDefaultTopLevelValues();
    }

    fclose( FileConfig );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                          DrealLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void DrealLoadConfig( Message )

  char Message;
{
  rdsbegin();

  FileConfig = fopen( DREAL_GMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    if ( Message )
    {
      DrealErrorMessage( DrealMainWindow, 
                         "Unable to open config file !" );
    }
    else
    {
      DrealDefaultConfig();
    }
  }
  else
  {
    if ( DrealReadTopLevelValues() )
    {
      DrealReadPanelValues( &DrealViewArrowsPanel       );
      DrealReadPanelValues( &DrealViewZoomPanel         );
      DrealReadPanelValues( &DrealViewLayerPanel        );
      DrealReadPanelValues( &DrealViewGridPanel         );
      DrealReadPanelValues( &DrealViewMapPanel          );
      DrealReadPanelValues( &DrealCreateRectanglePanel  );
      DrealReadPanelValues( &DrealModifyRectanglePanel  );
      DrealReadPanelValues( &DrealToolsMessagePanel     );
      DrealReadPanelValues( &DrealHelpPresentPanel      );

      DrealReadActiveLayers();
      DrealInitializeLayer();
    }
    else
    if ( Message )
    {
      DrealErrorMessage( DrealMainWindow,
                         "Bad version, unable to open config file !" );
    }
    else
    {
      DrealDefaultConfig();
    }

    fclose( FileConfig );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                          DrealSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void DrealSaveConfig()

{
  rdsbegin();

  FileConfig = fopen( DREAL_GMS_FILE_NAME, "w" );

  if ( FileConfig == (FILE *)NULL )
  {
    DrealErrorMessage( DrealMainWindow,
                       "Unable to open config file !" );
  }
  else
  {
    DrealWriteTopLevelValues();

    DrealWritePanelValues( &DrealViewArrowsPanel       );
    DrealWritePanelValues( &DrealViewZoomPanel         );
    DrealWritePanelValues( &DrealViewLayerPanel        );
    DrealWritePanelValues( &DrealViewGridPanel         );
    DrealWritePanelValues( &DrealViewMapPanel          );
    DrealWritePanelValues( &DrealCreateRectanglePanel  );
    DrealWritePanelValues( &DrealModifyRectanglePanel  );
    DrealWritePanelValues( &DrealToolsMessagePanel     );
    DrealWritePanelValues( &DrealHelpPresentPanel      );

    DrealWriteActiveLayers();

    fclose( FileConfig );
  }

  rdsend();
}
