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
| Tool    :                    XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"
# include "XSB.h"
# include "XFS.h"
# include "XMX.h"
# include "XTB.h"
# include "XMF.h"

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

  char XfsmFileFilter[ 10 ]    = "*.";
  char XfsmFileExtention[ 10 ] = ".";

  char XfsmFileBuffer[ 128 ];
  char XfsmDirectoryBuffer[ 512 ];

  char *XFSM_WORK_LIB = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XfsmFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmFileOpen( FileName )

  char *FileName;
{
  int    Index;
  char  *SWAP_WORK_LIB = WORK_LIB;

  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR  );
  XfsmUndisplayCursor();

  XfsmDelConnect();
  XfsmDelFigure();

  XfsmRecomputeBound = XFSM_TRUE;

  if ( XFSM_WORK_LIB == (char *)NULL )
  {
    XFSM_WORK_LIB = WORK_LIB;
  }

  if ( XfsmFileFilter[2] == '\0' )
  {
    strcat( XfsmFileFilter, FSM_IN    );
    strcat( XfsmFileExtention, FSM_IN );
  }

  for ( Index = strlen( FileName ); Index >= 0; Index-- )
  {
    if ( FileName[ Index ] == '/' ) break;
  }

  if ( Index >= 0 )
  {
    strcpy( XfsmDirectoryBuffer, FileName );
    strcpy( XfsmFileBuffer, FileName + Index + 1);

    XfsmDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = XfsmDirectoryBuffer;
  }
  else
  {
    strcpy( XfsmFileBuffer, FileName );

    WORK_LIB = XFSM_WORK_LIB;
  }

  Index = strlen( XfsmFileBuffer ) - strlen( XfsmFileExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( XfsmFileBuffer + Index, XfsmFileExtention ) )
    {
      XfsmFileBuffer[ Index ] = '\0';
    }
  }

  XfsmLoadFigure( XfsmFileBuffer );

  if ( XfsmFigure != (xfsmfig_list *)NULL )
  {
    XfsmChangeTopLevelTitle( XfsmFileBuffer );
  }
  else
  {
    XfsmChangeTopLevelTitle( (char *)NULL );
  }

  XfsmDisplayToolsMessage();

  XfsmInitializeZoom();
  XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );

  XfsmRefreshGraphicWindow( 0, 0,
                             XfsmGraphicDx, XfsmGraphicDy );

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );
  XfsmDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  autend();
}
