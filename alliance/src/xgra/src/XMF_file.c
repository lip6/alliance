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
| Tool    :                    XGRA                           |
|                                                             |
| File    :                   File.c                          |
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
# include <stdlib.h>
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XGR.h"
# include "XMX.h"
# include "XTB.h"
# include "XMF.h"
# include "XME.h"
# include "XMT.h"
# include "XMV.h"

# include "XMF_file.h"

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

  char XgraFileFilter[ 10 ]    = "*.";
  char XgraFileExtention[ 10 ] = ".";

  char XgraFileBuffer[ 128 ];
  char XgraDirectoryBuffer[ 512 ];

  char *XGRA_WORK_LIB = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XgraFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void XgraFileOpen( FileName )

  char *FileName;
{
  int    Index;
  char  *SWAP_WORK_LIB = WORK_LIB;

  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR  );
  XgraUndisplayCursor();

  XgraDelConnect();
  XgraDelFigure();

  XgraRecomputeBound = XGRA_TRUE;

  if ( XGRA_WORK_LIB == (char *)NULL )
  {
    XGRA_WORK_LIB = WORK_LIB;
  }

  if ( XgraFileFilter[2] == '\0' )
  {
    strcat( XgraFileFilter, "gr"    );
    strcat( XgraFileExtention, "gr" );
  }

  for ( Index = strlen( FileName ); Index >= 0; Index-- )
  {
    if ( FileName[ Index ] == '/' ) break;
  }

  if ( Index >= 0 )
  {
    strcpy( XgraDirectoryBuffer, FileName );
    strcpy( XgraFileBuffer, FileName + Index + 1);

    XgraDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = XgraDirectoryBuffer;
  }
  else
  {
    strcpy( XgraFileBuffer, FileName );

    WORK_LIB = XGRA_WORK_LIB;
  }

  Index = strlen( XgraFileBuffer ) - strlen( XgraFileExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( XgraFileBuffer + Index, XgraFileExtention ) )
    {
      XgraFileBuffer[ Index ] = '\0';
    }
  }

  XgraLoadFigure( XgraFileBuffer );

  if ( XgraFigure != (xautgraph *)NULL )
  {
    XgraChangeTopLevelTitle( XgraFileBuffer );
  }
  else
  {
    XgraChangeTopLevelTitle( (char *)NULL );
  }

  XgraDisplayToolsMessage();

  XgraInitializeZoom();
  XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );

  XgraRefreshGraphicWindow( 0, 0,
                             XgraGraphicDx, XgraGraphicDy );

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );
  XgraDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  autend();
}
