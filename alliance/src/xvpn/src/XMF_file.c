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
| Tool    :                    XVPN                           |
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
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "XSB.h"
# include "XVP.h"
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

  static xvpnslide_list  *XVPN_HEAD_SLIDE    = (xvpnslide_list *)0;
  static xvpnslide_list  *XVPN_LAST_SLIDE    = (xvpnslide_list *)0;
  static xvpnslide_list  *XVPN_CURRENT_SLIDE = (xvpnslide_list *)0;

  char XvpnFileFilter[ 10 ]    = "*.";
  char XvpnFileExtention[ 10 ] = ".";

  char XvpnFileBuffer[ 128 ];
  char XvpnDirectoryBuffer[ 512 ];

  char *XVPN_WORK_LIB = (char *)NULL;

  char  XvpnFileDescription[ 128 ];
  char *XvpnDescriptionBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XvpnLoadDescription                 |
|                                                             |
\------------------------------------------------------------*/

void XvpnLoadDescription( FigureName )

  char *FigureName;
{
  FILE *FileDesc;
  long  Length;
  long  Index;
  long  Line;
  int   Data;

  if ( XvpnDescriptionBuffer != (char *)NULL )
  {
    autfreeblock( XvpnDescriptionBuffer );
    XvpnDescriptionBuffer = (char *)NULL;
  }

  FileDesc = mbkfopen( FigureName, "vhdl", "r" );

  if ( FileDesc != (FILE *)NULL )
  {
    Line   = 1;
    Length = 1;

    while ( ( Data = fgetc( FileDesc ) ) != EOF )
    {
      Length++;
      if ( (char)Data == '\n' ) Line++;
    }

    Length += ( Line * 7 );

    XvpnDescriptionBuffer = (char *)autallocblock( Length );

    fseek( FileDesc, 0L, 0 );

    strcpy( XvpnDescriptionBuffer, "1      " );
    Index = 7;
    Line  = 1;

    while ( ( ( Data = fgetc( FileDesc ) ) != EOF ) &&
            ( Index  < Length ) )
    {
      XvpnDescriptionBuffer[ Index++ ] = (char)Data;

      if ( (char)Data == '\n' )
      {
        Line++;
        sprintf( &XvpnDescriptionBuffer[ Index ], "%-6ld ", Line );
        Index += 7;
      }
    }

    XvpnDescriptionBuffer[ Index ] = '\0';

    fclose( FileDesc );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void XvpnFileOpen( FileName )

  char *FileName;
{
  int    Index;
  char  *SWAP_WORK_LIB = WORK_LIB;

  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR  );
  XvpnUndisplayCursor();

  XvpnDelConnect();
  XvpnDelFigure();

  XvpnRecomputeBound = XVPN_TRUE;

  if ( XVPN_WORK_LIB == (char *)NULL )
  {
    XVPN_WORK_LIB = WORK_LIB;
  }

  if ( XvpnFileFilter[2] == '\0' )
  {
    strcat( XvpnFileFilter, VPN_IN    );
    strcat( XvpnFileExtention, VPN_IN );
  }

  for ( Index = strlen( FileName ); Index >= 0; Index-- )
  {
    if ( FileName[ Index ] == '/' ) break;
  }

  if ( Index >= 0 )
  {
    strcpy( XvpnDirectoryBuffer, FileName );
    strcpy( XvpnFileBuffer, FileName + Index + 1);

    XvpnDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = XvpnDirectoryBuffer;
  }
  else
  {
    strcpy( XvpnFileBuffer, FileName );

    WORK_LIB = XVPN_WORK_LIB;
  }

  Index = strlen( XvpnFileBuffer ) - strlen( XvpnFileExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( XvpnFileBuffer + Index, XvpnFileExtention ) )
    {
      XvpnFileBuffer[ Index ] = '\0';
    }
  }

  XvpnLoadFigure( XvpnFileBuffer );

  if ( XvpnFigure != (xvpnfig_list *)NULL )
  {
    XvpnChangeTopLevelTitle( XvpnFileBuffer );

    XvpnLoadDescription( XvpnFileBuffer );

    XvpnDisplayEditDescription( XvpnDescriptionBuffer );
  }
  else
  {
    XvpnChangeTopLevelTitle( (char *)NULL );

    XvpnDisplayEditDescription( (char *)NULL );
  }

  XvpnDisplayToolsMessage();

  XvpnInitializeZoom();
  XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );

  XvpnRefreshGraphicWindow( 0, 0,
                             XvpnGraphicDx, XvpnGraphicDy );

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );
  XvpnDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnFileAddSlide                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnFileAddSlide( FileName )

  char *FileName;
{
  xvpnslide_list *Slide;

  Slide = (xvpnslide_list *)autallocheap( sizeof( xvpnslide_list ) );

  if ( XVPN_HEAD_SLIDE == (xvpnslide_list *)0 )
  {
    XVPN_HEAD_SLIDE    = Slide;
    XVPN_LAST_SLIDE    = Slide;
  }
  else
  {
    XVPN_LAST_SLIDE->NEXT = Slide;
    Slide->PREV           = XVPN_LAST_SLIDE;

    XVPN_LAST_SLIDE = Slide;
  }

  Slide->NAME = mbkstrdup( FileName );
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnFileNextSlide                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnFileNextSlide()
{
  if ( XVPN_HEAD_SLIDE == (xvpnslide_list *)0 ) return;

  if ( ( XVPN_CURRENT_SLIDE       == (xvpnslide_list *)0 ) ||
       ( XVPN_CURRENT_SLIDE->NEXT == (xvpnslide_list *)0 ) )
  {
    XVPN_CURRENT_SLIDE = XVPN_HEAD_SLIDE;
  }
  else
  {
    XVPN_CURRENT_SLIDE = XVPN_CURRENT_SLIDE->NEXT;
  }

  XvpnFileOpen( XVPN_CURRENT_SLIDE->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnFilePrevSlide                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnFilePrevSlide()
{
  if ( XVPN_HEAD_SLIDE == (xvpnslide_list *)0 ) return;

  if ( ( XVPN_CURRENT_SLIDE       == (xvpnslide_list *)0 ) ||
       ( XVPN_CURRENT_SLIDE->PREV == (xvpnslide_list *)0 ) )
  {
    XVPN_CURRENT_SLIDE = XVPN_LAST_SLIDE;
  }
  else
  {
    XVPN_CURRENT_SLIDE = XVPN_CURRENT_SLIDE->PREV;
  }

  XvpnFileOpen( XVPN_CURRENT_SLIDE->NAME );
}
