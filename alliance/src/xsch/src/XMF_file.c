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
| Tool    :                    XSCH                           |
|                                                             |
| File    :                   File.c                          |
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
# include <stdlib.h>
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
# include "XMX.h"
# include "XTB.h"
# include "XMF.h"
# include "XMV.h"
# include "XMT.h"

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

  static xschslide_list  *XSCH_HEAD_SLIDE    = (xschslide_list *)0;
  static xschslide_list  *XSCH_LAST_SLIDE    = (xschslide_list *)0;
  static xschslide_list  *XSCH_CURRENT_SLIDE = (xschslide_list *)0;

  char XschFileFilter[ 10 ]    = "*.";
  char XschFileExtention[ 10 ] = ".";

  char XschFileBuffer[ 128 ];
  char XschDirectoryBuffer[ 512 ];

  char *XSCH_WORK_LIB = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XschFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void XschFileOpen( FileName, DisplayFigure )

  char *FileName;
  int   DisplayFigure;
{
  int    Index;
  char  *SWAP_WORK_LIB = WORK_LIB;

  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR  );
  XschUndisplayCursor();

  XschDelConnect();
  XschDelFigure();

  XschRecomputeBound = XSCH_TRUE;

  if ( XSCH_WORK_LIB == (char *)NULL )
  {
    XSCH_WORK_LIB = WORK_LIB;
  }

  if ( XschFileFilter[2] == '\0' )
  {
    strcat( XschFileFilter, XSCH_IN_SCHEM );
    strcat( XschFileExtention, XSCH_IN_SCHEM );
  }

  for ( Index = strlen( FileName ); Index >= 0; Index-- )
  {
    if ( FileName[ Index ] == '/' ) break;
  }

  if ( Index >= 0 )
  {
    strcpy( XschDirectoryBuffer, FileName );
    strcpy( XschFileBuffer, FileName + Index + 1);

    XschDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = XschDirectoryBuffer;
  }
  else
  {
    strcpy( XschFileBuffer, FileName );

    WORK_LIB = XSCH_WORK_LIB;
  }

  Index = strlen( XschFileBuffer ) - strlen( XschFileExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( XschFileBuffer + Index, XschFileExtention ) )
    {
      XschFileBuffer[ Index ] = '\0';
    }
  }

  XschLoadFigure( XschFileBuffer );

  if ( XschFigure != (xschfig_list *)NULL )
  {
    XschChangeTopLevelTitle( XschFileBuffer );
  }
  else
  {
    XschChangeTopLevelTitle( (char *)NULL );
  }

  XschDisplayToolsMessage();

  XschInitializeZoom();

  if ( DisplayFigure )
  {
    XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );

    XschRefreshGraphicWindow( 0, 0,
                             XschGraphicDx, XschGraphicDy );
  }

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );
  XschDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschFileAddSlide                    |
|                                                             |
\------------------------------------------------------------*/

void XschFileAddSlide( FileName )

  char *FileName;
{
  xschslide_list *Slide;

  Slide = (xschslide_list *)autallocheap( sizeof( xschslide_list ) );

  if ( XSCH_HEAD_SLIDE == (xschslide_list *)0 )
  {
    XSCH_HEAD_SLIDE    = Slide;
    XSCH_LAST_SLIDE    = Slide;
  }
  else
  {
    XSCH_LAST_SLIDE->NEXT = Slide;
    Slide->PREV           = XSCH_LAST_SLIDE;

    XSCH_LAST_SLIDE = Slide;
  }

  Slide->NAME = mbkstrdup( FileName );
}

/*------------------------------------------------------------\
|                                                             |
|                         XschFileNextSlide                   |
|                                                             |
\------------------------------------------------------------*/

void XschFileNextSlide()
{
  if ( XSCH_HEAD_SLIDE == (xschslide_list *)0 ) return;

  if ( ( XSCH_CURRENT_SLIDE       == (xschslide_list *)0 ) ||
       ( XSCH_CURRENT_SLIDE->NEXT == (xschslide_list *)0 ) )
  {
    XSCH_CURRENT_SLIDE = XSCH_HEAD_SLIDE;
  }
  else
  {
    XSCH_CURRENT_SLIDE = XSCH_CURRENT_SLIDE->NEXT;
  }

  XschFileOpen( XSCH_CURRENT_SLIDE->NAME, 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                         XschFilePrevSlide                   |
|                                                             |
\------------------------------------------------------------*/

void XschFilePrevSlide()
{
  if ( XSCH_HEAD_SLIDE == (xschslide_list *)0 ) return;

  if ( ( XSCH_CURRENT_SLIDE       == (xschslide_list *)0 ) ||
       ( XSCH_CURRENT_SLIDE->PREV == (xschslide_list *)0 ) )
  {
    XSCH_CURRENT_SLIDE = XSCH_LAST_SLIDE;
  }
  else
  {
    XSCH_CURRENT_SLIDE = XSCH_CURRENT_SLIDE->PREV;
  }

  XschFileOpen( XSCH_CURRENT_SLIDE->NAME, 1 );
}
