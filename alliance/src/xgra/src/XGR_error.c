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
| Tool    :                   XGRA                            |
|                                                             |
| File    :                  Error.c                          |
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
# include <signal.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XGR.h"
# include "XGR_error.h"

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

  static FILE   *XgraStreamErr;
  static FILE   *XgraStreamOut;
  static FILE   *XgraStreamAll;

  static char    XgraErrFileName[ 40 ];
  static char    XgraOutFileName[ 40 ];
  static char    XgraAllFileName[ 40 ];

  static char    XgraNormalMode = 1;

/*------------------------------------------------------------\
|                                                             |
|                      Xgra Error Message                    |
|                                                             |
\------------------------------------------------------------*/

  static char *XgraErrorBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                      Xgra Informations                     |
|                                                             |
\------------------------------------------------------------*/

  static char XgraInformationsBuffer[ XGRA_INFORMATIONS_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xgra Error Message Functions               |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   XgraInitializeErrorMessage               |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeErrorMessage( Debug )

  char Debug;
{
  autbegin();

  XgraNormalMode = ! Debug;

  if ( XgraNormalMode )
  {
    sprintf( XgraOutFileName, "/usr/tmp/out_%d",getpid() ); 
  }

  sprintf( XgraErrFileName, "/usr/tmp/err_%d",getpid() );
  sprintf( XgraAllFileName, "/usr/tmp/all_%d",getpid() );

  XgraStreamErr = freopen( XgraErrFileName, "w+", stderr);
  XgraStreamAll = fopen  ( XgraAllFileName, "w+"        );

  if ( XgraNormalMode )
  {
    XgraStreamOut = freopen( XgraOutFileName, "w+", stdout);
  }
  else
  {
    XgraStreamOut = XgraStreamErr;
  }

  if ( ( XgraStreamAll == (FILE *)NULL ) ||
       ( XgraStreamOut == (FILE *)NULL ) ||
       ( XgraStreamErr == (FILE *)NULL ) )
  {
    fprintf( stdout, "Grm: Unable to open trace window !\n");
    exit( 1 );
  }

  if ( XgraNormalMode )
  {
    unlink( XgraOutFileName ); 
  }

  unlink( XgraErrFileName );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraFlushErrorMessage                 |
|                                                             |
\------------------------------------------------------------*/

void XgraFlushErrorMessage()
{
  int Data;

  autbegin();

  fflush( XgraStreamErr );
  fseek( XgraStreamErr, 0L, 0 );

  while( ( Data = fgetc( XgraStreamErr ) ) != EOF )
  {
    fputc( Data, XgraStreamAll );
  }

  fclose( XgraStreamErr );

  XgraStreamErr = freopen( XgraErrFileName, "w+", stderr );

  if ( XgraNormalMode )
  {
    fflush( XgraStreamOut );
    fseek( XgraStreamOut, 0L, 0 );

    while( ( Data = fgetc( XgraStreamOut ) ) != EOF )
    {
      fputc( Data, XgraStreamAll );
    } 
  
    fclose( XgraStreamOut );

    XgraStreamOut = freopen( XgraOutFileName, "w+", stdout );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraGetErrorMessage                    |
|                                                             |
\------------------------------------------------------------*/

char *XgraGetErrorMessage()

{
  char *Message;
  long  Length;
  long  Index;
  int   Data;

  autbegin();

  XgraFlushErrorMessage();

  fflush( XgraStreamAll );

  Length = ftell( XgraStreamAll );

  if ( XgraErrorBuffer != (char *)NULL )
  {
    autfreeblock( XgraErrorBuffer );
  }

  XgraErrorBuffer = autallocblock( Length + 1 );
  Index            = 0;

  while ( ( ( Data = fgetc( XgraStreamAll ) ) != EOF ) &&
          ( Index  < Length ) )
  {
    XgraErrorBuffer[ Index++ ] = (char)Data;
  }

  rewind( XgraStreamAll );

  XgraErrorBuffer[ Index ] = '\0';

  if ( Index == 0 )
  {
    Message = (char *)NULL;
  }
  else
  {
    Message = XgraErrorBuffer;
  }

  autend();
  return( Message );
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraGetInformations                    |
|                                                             |
\------------------------------------------------------------*/

char *XgraGetInformations()

{
  char *Scan;

  autbegin();

  XgraComputeBound();

  Scan = XgraInformationsBuffer;

  if ( XgraFigureGra != (autgraph *)NULL )
  {
    sprintf( Scan, "  FIGURE : %s\n\n  NODES : %d\n  ARCS : %d\n\n",
        XgraFigure->NAME,
        XgraFigureGra->NUMBER_NODE,
        XgraFigureGra->NUMBER_ARC );
  }
  else
  {
    sprintf( Scan, "  FIGURE : No current figure !\n\n" );
  }

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  BOUNDING BOX : \n\n  XMIN : %d\n  YMIN : %d\n  XMAX : %d\n  YMAX : %d\n\n",
           XgraBoundXmin / XGRA_UNIT, XgraBoundYmin / XGRA_UNIT,
           XgraBoundXmax / XGRA_UNIT, XgraBoundYmax / XGRA_UNIT );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  XGRA PARAM : %s\n\n", XGRA_PARAM_NAME );

  autend();
  return( XgraInformationsBuffer );
}
