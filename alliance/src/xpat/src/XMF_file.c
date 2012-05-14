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
| Tool    :                    XPAT                           |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
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

  char XpatFileFilter[ 10 ]    = "*.";
  char XpatFileExtention[ 10 ] = ".";

  char XpatFileBuffer[ 128 ];
  char XpatDirectoryBuffer[ 512 ];

  char *XPAT_WORK_LIB = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XpatFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void XpatFileOpen( FileName )

  char *FileName;
{
  int    Index;
  char  *SWAP_WORK_LIB = WORK_LIB;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR  );
  XpatUndisplayCursor();

  XpatDelConnect();
  XpatDelFigure();

  XpatRecomputeBound = XPAT_TRUE;

  if ( XPAT_WORK_LIB == (char *)NULL )
  {
    XPAT_WORK_LIB = WORK_LIB;
  }

  if ( XpatFileFilter[2] == '\0' )
  {
    strcat( XpatFileFilter, "pat"    );
    strcat( XpatFileExtention, "pat" );
  }

  for ( Index = strlen( FileName ); Index >= 0; Index-- )
  {
    if ( FileName[ Index ] == '/' ) break;
  }

  if ( Index >= 0 )
  {
    strcpy( XpatDirectoryBuffer, FileName );
    strcpy( XpatFileBuffer, FileName + Index + 1);

    XpatDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = XpatDirectoryBuffer;
  }
  else
  {
    strcpy( XpatFileBuffer, FileName );

    WORK_LIB = XPAT_WORK_LIB;
  }

  Index = strlen( XpatFileBuffer ) - strlen( XpatFileExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( XpatFileBuffer + Index, XpatFileExtention ) )
    {
      XpatFileBuffer[ Index ] = '\0';
    }
  }

  XpatLoadFigure( XpatFileBuffer );

  if ( XpatFigure != (xpatfig_list *)NULL )
  {
    XpatChangeTopLevelTitle( XpatFileBuffer );
  }
  else
  {
    XpatChangeTopLevelTitle( (char *)NULL );
  }

  XpatDisplayToolsMessage();

  XpatInitializeZoom();
  XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );

  XpatRefreshGraphicWindow( 0, 0,
                             XpatGraphicDx, XpatGraphicDy );

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );
  XpatDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  autend();
}
