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
| Tool    :                   XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
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

void XfsmSetPanelValues( Panel, Values )

  XfsmPanelItem *Panel;
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
    XfsmEnterPanel( Panel );
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

void XfsmWriteTopLevelValues()

{
  Dimension Values[5];

  autbegin();

  XtVaGetValues( XfsmTopLevel,
                 XmNx        , &Values[0],
                 XmNy        , &Values[1],
                 XmNwidth    , &Values[2],
                 XmNheight   , &Values[3],
                 NULL );

  Values[0] = Values[0] - XFSM_TOPLEVEL_TRANSLATE_X;
  Values[1] = Values[1] - XFSM_TOPLEVEL_TRANSLATE_Y;
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

void XfsmWritePanelValues( Panel )

  XfsmPanelItem *Panel;
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

    Values[0] = Values[0] - XFSM_PANEL_TRANSLATE_X;
    Values[1] = Values[1] - XFSM_PANEL_TRANSLATE_Y;
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

void XfsmReadPanelValues( Panel )

  XfsmPanelItem *Panel;
{
  int Values[5];

  autbegin();

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XfsmSetPanelValues( Panel, Values );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Read  Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

char XfsmReadTopLevelValues()

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

  XtVaSetValues( XfsmTopLevel,
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

void XfsmReadActiveLayers()

{
  short Layer;
  int   Value;

  autbegin();

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XFSM_ACTIVE_LAYER_TABLE[ Layer ] = Value;
  }

  for ( Layer = 0; Layer < XFSM_MAX_ACTIVE_NAME; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XFSM_ACTIVE_NAME_TABLE[ Layer ] = Value;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        WriteActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmWriteActiveLayers()

{
  char Layer;

  autbegin();

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XFSM_ACTIVE_LAYER_TABLE[ Layer ] );
  }

  for ( Layer = 0; Layer < XFSM_MAX_ACTIVE_NAME; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XFSM_ACTIVE_NAME_TABLE[ Layer ] );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmDefaultTopLevelValues                |
|                                                             |
\------------------------------------------------------------*/

void XfsmDefaultTopLevelValues()

{
  autbegin();

  XtVaSetValues( XfsmTopLevel,
                 XmNheight   , XFSM_TOPLEVEL_HEIGHT,
                 XmNwidth    , XFSM_TOPLEVEL_WIDTH,
                 XmNx        , XFSM_TOPLEVEL_X,
                 XmNy        , XFSM_TOPLEVEL_Y,
                 NULL
               );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmDefaultConfig()

{
  short Layer;

  autbegin();

  XfsmDefaultTopLevelValues();

  XfsmSetPanelValues( &XfsmViewArrowsPanel, 
                       XfsmViewArrowsDefaultValues );
  XfsmSetPanelValues( &XfsmViewZoomPanel, 
                       XfsmViewZoomDefaultValues );
  XfsmSetPanelValues( &XfsmViewGridPanel, 
                       XfsmViewGridDefaultValues );
  XfsmSetPanelValues( &XfsmViewLayerPanel, 
                       XfsmViewLayerDefaultValues );
  XfsmSetPanelValues( &XfsmViewMapPanel, 
                       XfsmViewMapDefaultValues );
  XfsmSetPanelValues( &XfsmEditSearchViewPanel, 
                       XfsmEditSearchViewDefaultValues );
  XfsmSetPanelValues( &XfsmEditSelectPanel, 
                       XfsmEditSelectDefaultValues );
  XfsmSetPanelValues( &XfsmEditIdentifyPanel, 
                       XfsmEditIdentifyDefaultValues );
  XfsmSetPanelValues( &XfsmToolsMessagePanel, 
                       XfsmToolsMessageDefaultValues );
  XfsmSetPanelValues( &XfsmSetupInformationsPanel,
                       XfsmSetupInformationsDefaultValues );
  XfsmSetPanelValues( &XfsmHelpPresentPanel,
                       XfsmHelpPresentDefaultValues );

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    XFSM_ACTIVE_LAYER_TABLE[ Layer ] = 1;
  }

  for ( Layer = 0; Layer < XFSM_MAX_ACTIVE_NAME; Layer++ )
  {
    XFSM_ACTIVE_NAME_TABLE[ Layer ] = 1;
  }

  XfsmInitializeLayer();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmLoadTopLevelConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmLoadTopLevelConfig()

{
  autbegin();

  FileConfig = fopen( XFSM_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    XfsmDefaultTopLevelValues();
  }
  else
  {
    if ( ! XfsmReadTopLevelValues() )
    {
      XfsmDefaultTopLevelValues();
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XfsmLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmLoadConfig( Message )

  char Message;
{
  autbegin();

  FileConfig = fopen( XFSM_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    if ( Message )
    {
      XfsmErrorMessage( XfsmMainWindow, 
                         "Unable to open config file !" );
    }
    else
    {
      XfsmDefaultConfig();
    }
  }
  else
  {
    if ( XfsmReadTopLevelValues() )
    {
      XfsmReadPanelValues( &XfsmViewArrowsPanel        );
      XfsmReadPanelValues( &XfsmViewZoomPanel          );
      XfsmReadPanelValues( &XfsmViewGridPanel          );
      XfsmReadPanelValues( &XfsmViewLayerPanel         );
      XfsmReadPanelValues( &XfsmViewMapPanel           );
      XfsmReadPanelValues( &XfsmEditSearchViewPanel    );
      XfsmReadPanelValues( &XfsmEditSelectPanel        );
      XfsmReadPanelValues( &XfsmEditIdentifyPanel      );
      XfsmReadPanelValues( &XfsmToolsMessagePanel      );
      XfsmReadPanelValues( &XfsmSetupInformationsPanel );
      XfsmReadPanelValues( &XfsmHelpPresentPanel       );

      XfsmReadActiveLayers();
      XfsmInitializeLayer();
    }
    else
    if ( Message )
    {
      XfsmErrorMessage( XfsmMainWindow,
                         "Bad version, unable to open config file !" );
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XfsmSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmSaveConfig()

{
  autbegin();

  FileConfig = fopen( XFSM_XMS_FILE_NAME, "w" );

  if ( FileConfig == (FILE *)NULL )
  {
    XfsmErrorMessage( XfsmMainWindow,
                       "Unable to open config file !" );
  }
  else
  {
    XfsmWriteTopLevelValues();

    XfsmWritePanelValues( &XfsmViewArrowsPanel        );
    XfsmWritePanelValues( &XfsmViewZoomPanel          );
    XfsmWritePanelValues( &XfsmViewGridPanel          );
    XfsmWritePanelValues( &XfsmViewLayerPanel         );
    XfsmWritePanelValues( &XfsmViewMapPanel           );
    XfsmWritePanelValues( &XfsmEditSearchViewPanel    );
    XfsmWritePanelValues( &XfsmEditSelectPanel        );
    XfsmWritePanelValues( &XfsmEditIdentifyPanel      );
    XfsmWritePanelValues( &XfsmToolsMessagePanel      );
    XfsmWritePanelValues( &XfsmSetupInformationsPanel );
    XfsmWritePanelValues( &XfsmHelpPresentPanel       );

    XfsmWriteActiveLayers();

    fclose( FileConfig );
  }

  autend();
}
