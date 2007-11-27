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
| Tool    :                   XGRA                           |
|                                                             |
| File    :                  Setup.c                          |
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
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
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

void XgraSetPanelValues( Panel, Values )

  XgraPanelItem *Panel;
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
    XgraEnterPanel( Panel );
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

void XgraWriteTopLevelValues()

{
  Dimension Values[5];

  autbegin();

  XtVaGetValues( XgraTopLevel,
                 XmNx        , &Values[0],
                 XmNy        , &Values[1],
                 XmNwidth    , &Values[2],
                 XmNheight   , &Values[3],
                 NULL );

  Values[0] = Values[0] - XGRA_TOPLEVEL_ARCLATE_X;
  Values[1] = Values[1] - XGRA_TOPLEVEL_ARCLATE_Y;
  Values[4] = 1;
  
  fprintf( FileConfig, "VERSION: %s\n", ALLIANCE_VERSION );
  fprintf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
           Values[0], Values[1], Values[2], Values[3], Values[4] );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Write Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

void XgraWritePanelValues( Panel )

  XgraPanelItem *Panel;
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

    Values[0] = Values[0] - XGRA_PANEL_ARCLATE_X;
    Values[1] = Values[1] - XGRA_PANEL_ARCLATE_Y;
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

void XgraReadPanelValues( Panel )

  XgraPanelItem *Panel;
{
  int Values[5];

  autbegin();

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XgraSetPanelValues( Panel, Values );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Read  Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

char XgraReadTopLevelValues()

{
  int  Values[5];
  char Version[64];

  autbegin();

  fscanf( FileConfig, "VERSION: %s\n", Version );

  if ( strcmp( Version, ALLIANCE_VERSION ) )
  {
    autend();
    return( 0 );
  }

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XtVaSetValues( XgraTopLevel,
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

void XgraReadActiveLayers()

{
  short Layer;
  int   Value;

  autbegin();

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XGRA_ACTIVE_LAYER_TABLE[ Layer ] = Value;
  }

  for ( Layer = 0; Layer < XGRA_MAX_ACTIVE_NAME; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XGRA_ACTIVE_NAME_TABLE[ Layer ] = Value;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        WriteActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void XgraWriteActiveLayers()

{
  char Layer;

  autbegin();

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XGRA_ACTIVE_LAYER_TABLE[ Layer ] );
  }

  for ( Layer = 0; Layer < XGRA_MAX_ACTIVE_NAME; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XGRA_ACTIVE_NAME_TABLE[ Layer ] );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraDefaultTopLevelValues                |
|                                                             |
\------------------------------------------------------------*/

void XgraDefaultTopLevelValues()

{
  autbegin();

  XtVaSetValues( XgraTopLevel,
                 XmNheight   , XGRA_TOPLEVEL_HEIGHT,
                 XmNwidth    , XGRA_TOPLEVEL_WIDTH,
                 XmNx        , XGRA_TOPLEVEL_X,
                 XmNy        , XGRA_TOPLEVEL_Y,
                 NULL
               );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XgraDefaultConfig()

{
  short Layer;

  autbegin();

  XgraDefaultTopLevelValues();

  XgraSetPanelValues( &XgraViewArrowsPanel, 
                       XgraViewArrowsDefaultValues );
  XgraSetPanelValues( &XgraViewZoomPanel, 
                       XgraViewZoomDefaultValues );
  XgraSetPanelValues( &XgraViewGridPanel, 
                       XgraViewGridDefaultValues );
  XgraSetPanelValues( &XgraViewLayerPanel, 
                       XgraViewLayerDefaultValues );
  XgraSetPanelValues( &XgraViewMapPanel, 
                       XgraViewMapDefaultValues );
  XgraSetPanelValues( &XgraEditSearchViewPanel, 
                       XgraEditSearchViewDefaultValues );
  XgraSetPanelValues( &XgraEditSelectPanel, 
                       XgraEditSelectDefaultValues );
  XgraSetPanelValues( &XgraEditIdentifyPanel, 
                       XgraEditIdentifyDefaultValues );
  XgraSetPanelValues( &XgraToolsMessagePanel, 
                       XgraToolsMessageDefaultValues );
  XgraSetPanelValues( &XgraSetupInformationsPanel,
                       XgraSetupInformationsDefaultValues );
  XgraSetPanelValues( &XgraHelpPresentPanel,
                       XgraHelpPresentDefaultValues );

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    XGRA_ACTIVE_LAYER_TABLE[ Layer ] = 1;
  }

  for ( Layer = 0; Layer < XGRA_MAX_ACTIVE_NAME; Layer++ )
  {
    XGRA_ACTIVE_NAME_TABLE[ Layer ] = 1;
  }

  XgraInitializeLayer();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraLoadTopLevelConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XgraLoadTopLevelConfig()

{
  autbegin();

  FileConfig = fopen( XGRA_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    XgraDefaultTopLevelValues();
  }
  else
  {
    if ( ! XgraReadTopLevelValues() )
    {
      XgraDefaultTopLevelValues();
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XgraLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XgraLoadConfig( Message )

  char Message;
{
  autbegin();

  FileConfig = fopen( XGRA_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    if ( Message )
    {
      XgraErrorMessage( XgraMainWindow, 
                         "Unable to open config file !" );
    }
    else
    {
      XgraDefaultConfig();
    }
  }
  else
  {
    if ( XgraReadTopLevelValues() )
    {
      XgraReadPanelValues( &XgraViewArrowsPanel        );
      XgraReadPanelValues( &XgraViewZoomPanel          );
      XgraReadPanelValues( &XgraViewGridPanel          );
      XgraReadPanelValues( &XgraViewLayerPanel         );
      XgraReadPanelValues( &XgraViewMapPanel           );
      XgraReadPanelValues( &XgraEditSearchViewPanel    );
      XgraReadPanelValues( &XgraEditSelectPanel        );
      XgraReadPanelValues( &XgraEditIdentifyPanel      );
      XgraReadPanelValues( &XgraToolsMessagePanel      );
      XgraReadPanelValues( &XgraSetupInformationsPanel );
      XgraReadPanelValues( &XgraHelpPresentPanel       );

      XgraReadActiveLayers();
      XgraInitializeLayer();
    }
    else
    if ( Message )
    {
      XgraErrorMessage( XgraMainWindow,
                         "Bad version, unable to open config file !" );
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XgraSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XgraSaveConfig()

{
  autbegin();

  FileConfig = fopen( XGRA_XMS_FILE_NAME, "w" );

  if ( FileConfig == (FILE *)NULL )
  {
    XgraErrorMessage( XgraMainWindow,
                       "Unable to open config file !" );
  }
  else
  {
    XgraWriteTopLevelValues();

    XgraWritePanelValues( &XgraViewArrowsPanel        );
    XgraWritePanelValues( &XgraViewZoomPanel          );
    XgraWritePanelValues( &XgraViewGridPanel          );
    XgraWritePanelValues( &XgraViewLayerPanel         );
    XgraWritePanelValues( &XgraViewMapPanel           );
    XgraWritePanelValues( &XgraEditSearchViewPanel    );
    XgraWritePanelValues( &XgraEditSelectPanel        );
    XgraWritePanelValues( &XgraEditIdentifyPanel      );
    XgraWritePanelValues( &XgraToolsMessagePanel      );
    XgraWritePanelValues( &XgraSetupInformationsPanel );
    XgraWritePanelValues( &XgraHelpPresentPanel       );

    XgraWriteActiveLayers();

    fclose( FileConfig );
  }

  autend();
}
