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
| Tool    :                    XVPN                           |
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

void XvpnSetPanelValues( Panel, Values )

  XvpnPanelItem *Panel;
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
    XvpnEnterPanel( Panel );
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

void XvpnWriteTopLevelValues()

{
  Dimension Values[5];

  autbegin();

  XtVaGetValues( XvpnTopLevel,
                 XmNx        , &Values[0],
                 XmNy        , &Values[1],
                 XmNwidth    , &Values[2],
                 XmNheight   , &Values[3],
                 NULL );

  Values[0] = Values[0] - XVPN_TOPLEVEL_TRANSLATE_X;
  Values[1] = Values[1] - XVPN_TOPLEVEL_TRANSLATE_Y;
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

void XvpnWritePanelValues( Panel )

  XvpnPanelItem *Panel;
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

    Values[0] = Values[0] - XVPN_PANEL_TRANSLATE_X;
    Values[1] = Values[1] - XVPN_PANEL_TRANSLATE_Y;
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

void XvpnReadPanelValues( Panel )

  XvpnPanelItem *Panel;
{
  int Values[5];

  autbegin();

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XvpnSetPanelValues( Panel, Values );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Read  Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

short XvpnReadTopLevelValues()

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

  XtVaSetValues( XvpnTopLevel,
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

void XvpnReadActiveLayers()

{
  short Layer;
  int   Value;

  autbegin();

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XVPN_ACTIVE_LAYER_TABLE[ Layer ] = Value;
  }

  for ( Layer = 0; Layer < XVPN_MAX_ACTIVE_NAME; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XVPN_ACTIVE_NAME_TABLE[ Layer ] = Value;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        WriteActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnWriteActiveLayers()

{
  short Layer;

  autbegin();

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XVPN_ACTIVE_LAYER_TABLE[ Layer ] );
  }

  for ( Layer = 0; Layer < XVPN_MAX_ACTIVE_NAME; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XVPN_ACTIVE_NAME_TABLE[ Layer ] );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnDefaultTopLevelValues                |
|                                                             |
\------------------------------------------------------------*/

void XvpnDefaultTopLevelValues()

{
  autbegin();

  XtVaSetValues( XvpnTopLevel,
                 XmNheight   , XVPN_TOPLEVEL_HEIGHT,
                 XmNwidth    , XVPN_TOPLEVEL_WIDTH,
                 XmNx        , XVPN_TOPLEVEL_X,
                 XmNy        , XVPN_TOPLEVEL_Y,
                 NULL
               );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnDefaultConfig()

{
  short Layer;

  autbegin();

  XvpnDefaultTopLevelValues();

  XvpnSetPanelValues( &XvpnViewArrowsPanel, 
                       XvpnViewArrowsDefaultValues );
  XvpnSetPanelValues( &XvpnViewZoomPanel, 
                       XvpnViewZoomDefaultValues );
  XvpnSetPanelValues( &XvpnViewGridPanel, 
                       XvpnViewGridDefaultValues );
  XvpnSetPanelValues( &XvpnViewLayerPanel, 
                       XvpnViewLayerDefaultValues );
  XvpnSetPanelValues( &XvpnViewMapPanel, 
                       XvpnViewMapDefaultValues );
  XvpnSetPanelValues( &XvpnEditSearchViewPanel, 
                       XvpnEditSearchViewDefaultValues );
  XvpnSetPanelValues( &XvpnEditSelectPanel, 
                       XvpnEditSelectDefaultValues );
  XvpnSetPanelValues( &XvpnEditIdentifyPanel, 
                       XvpnEditIdentifyDefaultValues );
  XvpnSetPanelValues( &XvpnEditDescriptionPanel, 
                       XvpnEditDescriptionDefaultValues );
  XvpnSetPanelValues( &XvpnToolsMessagePanel, 
                       XvpnToolsMessageDefaultValues );
  XvpnSetPanelValues( &XvpnSetupInformationsPanel,
                       XvpnSetupInformationsDefaultValues );
  XvpnSetPanelValues( &XvpnHelpPresentPanel,
                       XvpnHelpPresentDefaultValues );

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    XVPN_ACTIVE_LAYER_TABLE[ Layer ] = 1;
  }

  for ( Layer = 0; Layer < XVPN_MAX_ACTIVE_NAME; Layer++ )
  {
    XVPN_ACTIVE_NAME_TABLE[ Layer ] = 1;
  }

  XvpnInitializeLayer();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnLoadTopLevelConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnLoadTopLevelConfig()

{
  autbegin();

  FileConfig = fopen( XVPN_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    XvpnDefaultTopLevelValues();
  }
  else
  {
    if ( ! XvpnReadTopLevelValues() )
    {
      XvpnDefaultTopLevelValues();
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XvpnLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnLoadConfig( Message )

  short Message;
{
  autbegin();

  FileConfig = fopen( XVPN_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    if ( Message )
    {
      XvpnErrorMessage( XvpnMainWindow, 
                         "Unable to open config file !" );
    }
    else
    {
      XvpnDefaultConfig();
    }
  }
  else
  {
    if ( XvpnReadTopLevelValues() )
    {
      XvpnReadPanelValues( &XvpnViewArrowsPanel        );
      XvpnReadPanelValues( &XvpnViewZoomPanel          );
      XvpnReadPanelValues( &XvpnViewGridPanel          );
      XvpnReadPanelValues( &XvpnViewLayerPanel         );
      XvpnReadPanelValues( &XvpnViewMapPanel           );
      XvpnReadPanelValues( &XvpnEditSearchViewPanel    );
      XvpnReadPanelValues( &XvpnEditSelectPanel        );
      XvpnReadPanelValues( &XvpnEditIdentifyPanel      );
      XvpnReadPanelValues( &XvpnEditDescriptionPanel   );
      XvpnReadPanelValues( &XvpnToolsMessagePanel      );
      XvpnReadPanelValues( &XvpnSetupInformationsPanel );
      XvpnReadPanelValues( &XvpnHelpPresentPanel       );

      XvpnReadActiveLayers();
      XvpnInitializeLayer();
    }
    else
    if ( Message )
    {
      XvpnErrorMessage( XvpnMainWindow,
                         "Bad version, unable to open config file !" );
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XvpnSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnSaveConfig()

{
  autbegin();

  FileConfig = fopen( XVPN_XMS_FILE_NAME, "w" );

  if ( FileConfig == (FILE *)NULL )
  {
    XvpnErrorMessage( XvpnMainWindow,
                       "Unable to open config file !" );
  }
  else
  {
    XvpnWriteTopLevelValues();

    XvpnWritePanelValues( &XvpnViewArrowsPanel        );
    XvpnWritePanelValues( &XvpnViewZoomPanel          );
    XvpnWritePanelValues( &XvpnViewGridPanel          );
    XvpnWritePanelValues( &XvpnViewLayerPanel         );
    XvpnWritePanelValues( &XvpnViewMapPanel           );
    XvpnWritePanelValues( &XvpnEditSearchViewPanel    );
    XvpnWritePanelValues( &XvpnEditSelectPanel        );
    XvpnWritePanelValues( &XvpnEditIdentifyPanel      );
    XvpnWritePanelValues( &XvpnEditDescriptionPanel   );
    XvpnWritePanelValues( &XvpnToolsMessagePanel      );
    XvpnWritePanelValues( &XvpnSetupInformationsPanel );
    XvpnWritePanelValues( &XvpnHelpPresentPanel       );

    XvpnWriteActiveLayers();

    fclose( FileConfig );
  }

  autend();
}
