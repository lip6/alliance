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
| Tool    :                   XPAT                           |
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
# include "pat.h"
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

void XpatSetPanelValues( Panel, Values )

  XpatPanelItem *Panel;
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
    XpatEnterPanel( Panel );
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

void XpatWriteTopLevelValues()

{
  Dimension Values[5];

  autbegin();

  XtVaGetValues( XpatTopLevel,
                 XmNx        , &Values[0],
                 XmNy        , &Values[1],
                 XmNwidth    , &Values[2],
                 XmNheight   , &Values[3],
                 NULL );

  Values[0] = Values[0] - XPAT_TOPLEVEL_TRANSLATE_X;
  Values[1] = Values[1] - XPAT_TOPLEVEL_TRANSLATE_Y;
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

void XpatWritePanelValues( Panel )

  XpatPanelItem *Panel;
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

    Values[0] = Values[0] - XPAT_PANEL_TRANSLATE_X;
    Values[1] = Values[1] - XPAT_PANEL_TRANSLATE_Y;
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

void XpatReadPanelValues( Panel )

  XpatPanelItem *Panel;
{
  int Values[5];

  autbegin();

  fscanf( FileConfig, "X: %d, Y: %d, WIDTH: %d, HEIGHT: %d, MANAGED: %d\n",
          &Values[0], &Values[1], &Values[2], &Values[3], &Values[4] );

  XpatSetPanelValues( Panel, Values );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Read  Panel Values                    |
|                                                             |
\------------------------------------------------------------*/

char XpatReadTopLevelValues()

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

  XtVaSetValues( XpatTopLevel,
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

void XpatReadActiveLayers()

{
  short Layer;
  int   Value;

  autbegin();

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XPAT_ACTIVE_LAYER_TABLE[ Layer ] = Value;
  }

  for ( Layer = 0; Layer < XPAT_MAX_ACTIVE_NAME; Layer++ )
  {
    fscanf( FileConfig, "ACTIVE: %d\n", &Value );

    XPAT_ACTIVE_NAME_TABLE[ Layer ] = Value;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        WriteActiveLayers                    |
|                                                             |
\------------------------------------------------------------*/

void XpatWriteActiveLayers()

{
  char Layer;

  autbegin();

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XPAT_ACTIVE_LAYER_TABLE[ Layer ] );
  }

  for ( Layer = 0; Layer < XPAT_MAX_ACTIVE_NAME; Layer++ )
  {
    fprintf( FileConfig, "ACTIVE: %d\n",
             XPAT_ACTIVE_NAME_TABLE[ Layer ] );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatDefaultTopLevelValues                |
|                                                             |
\------------------------------------------------------------*/

void XpatDefaultTopLevelValues()

{
  autbegin();

  XtVaSetValues( XpatTopLevel,
                 XmNheight   , XPAT_TOPLEVEL_HEIGHT,
                 XmNwidth    , XPAT_TOPLEVEL_WIDTH,
                 XmNx        , XPAT_TOPLEVEL_X,
                 XmNy        , XPAT_TOPLEVEL_Y,
                 NULL
               );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatDefaultConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XpatDefaultConfig()

{
  short Layer;

  autbegin();

  XpatDefaultTopLevelValues();

  XpatSetPanelValues( &XpatViewArrowsPanel, 
                       XpatViewArrowsDefaultValues );
  XpatSetPanelValues( &XpatViewZoomPanel, 
                       XpatViewZoomDefaultValues );
  XpatSetPanelValues( &XpatViewGridPanel, 
                       XpatViewGridDefaultValues );
  XpatSetPanelValues( &XpatViewLayerPanel, 
                       XpatViewLayerDefaultValues );
  XpatSetPanelValues( &XpatViewMapPanel, 
                       XpatViewMapDefaultValues );
  XpatSetPanelValues( &XpatEditSearchViewPanel, 
                       XpatEditSearchViewDefaultValues );
  XpatSetPanelValues( &XpatEditSelectPanel, 
                       XpatEditSelectDefaultValues );
  XpatSetPanelValues( &XpatEditIdentifyPanel, 
                       XpatEditIdentifyDefaultValues );
  XpatSetPanelValues( &XpatToolsMessagePanel, 
                       XpatToolsMessageDefaultValues );
  XpatSetPanelValues( &XpatSetupInformationsPanel,
                       XpatSetupInformationsDefaultValues );
  XpatSetPanelValues( &XpatHelpPresentPanel,
                       XpatHelpPresentDefaultValues );

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    XPAT_ACTIVE_LAYER_TABLE[ Layer ] = 1;
  }

  for ( Layer = 0; Layer < XPAT_MAX_ACTIVE_NAME; Layer++ )
  {
    XPAT_ACTIVE_NAME_TABLE[ Layer ] = 1;
  }

  XpatInitializeLayer();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatLoadTopLevelConfig                   |
|                                                             |
\------------------------------------------------------------*/

void XpatLoadTopLevelConfig()

{
  autbegin();

  FileConfig = fopen( XPAT_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    XpatDefaultTopLevelValues();
  }
  else
  {
    if ( ! XpatReadTopLevelValues() )
    {
      XpatDefaultTopLevelValues();
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XpatLoadConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XpatLoadConfig( Message )

  char Message;
{
  autbegin();

  FileConfig = fopen( XPAT_XMS_FILE_NAME, "r" );

  if ( FileConfig == (FILE *)NULL )
  {
    if ( Message )
    {
      XpatErrorMessage( XpatMainWindow, 
                         "Unable to open config file !" );
    }
    else
    {
      XpatDefaultConfig();
    }
  }
  else
  {
    if ( XpatReadTopLevelValues() )
    {
      XpatReadPanelValues( &XpatViewArrowsPanel        );
      XpatReadPanelValues( &XpatViewZoomPanel          );
      XpatReadPanelValues( &XpatViewGridPanel          );
      XpatReadPanelValues( &XpatViewLayerPanel         );
      XpatReadPanelValues( &XpatViewMapPanel           );
      XpatReadPanelValues( &XpatEditSearchViewPanel    );
      XpatReadPanelValues( &XpatEditSelectPanel        );
      XpatReadPanelValues( &XpatEditIdentifyPanel      );
      XpatReadPanelValues( &XpatToolsMessagePanel      );
      XpatReadPanelValues( &XpatSetupInformationsPanel );
      XpatReadPanelValues( &XpatHelpPresentPanel       );

      XpatReadActiveLayers();
      XpatInitializeLayer();
    }
    else
    if ( Message )
    {
      XpatErrorMessage( XpatMainWindow,
                         "Bad version, unable to open config file !" );
    }

    fclose( FileConfig );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                           XpatSaveConfig                    |
|                                                             |
\------------------------------------------------------------*/

void XpatSaveConfig()

{
  autbegin();

  FileConfig = fopen( XPAT_XMS_FILE_NAME, "w" );

  if ( FileConfig == (FILE *)NULL )
  {
    XpatErrorMessage( XpatMainWindow,
                       "Unable to open config file !" );
  }
  else
  {
    XpatWriteTopLevelValues();

    XpatWritePanelValues( &XpatViewArrowsPanel        );
    XpatWritePanelValues( &XpatViewZoomPanel          );
    XpatWritePanelValues( &XpatViewGridPanel          );
    XpatWritePanelValues( &XpatViewLayerPanel         );
    XpatWritePanelValues( &XpatViewMapPanel           );
    XpatWritePanelValues( &XpatEditSearchViewPanel    );
    XpatWritePanelValues( &XpatEditSelectPanel        );
    XpatWritePanelValues( &XpatEditIdentifyPanel      );
    XpatWritePanelValues( &XpatToolsMessagePanel      );
    XpatWritePanelValues( &XpatSetupInformationsPanel );
    XpatWritePanelValues( &XpatHelpPresentPanel       );

    XpatWriteActiveLayers();

    fclose( FileConfig );
  }

  autend();
}
