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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                  Error.c                          |
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
# include <signal.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h" 
# include "XSC.h"
# include "XSC_error.h"

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

  static FILE   *XschStreamErr;
  static FILE   *XschStreamOut;
  static FILE   *XschStreamAll;

  static char    XschErrFileName[ 40 ];
  static char    XschOutFileName[ 40 ];
  static char    XschAllFileName[ 40 ];

  static char    XschNormalMode = 1;

/*------------------------------------------------------------\
|                                                             |
|                      Xsch Error Message                     |
|                                                             |
\------------------------------------------------------------*/

  static char *XschErrorBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                      Xsch Informations                      |
|                                                             |
\------------------------------------------------------------*/

  static char XschInformationsBuffer[ XSCH_INFORMATIONS_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xsch Error Message Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XschExitErrorMessage                     |
|                                                             |
\------------------------------------------------------------*/

void XschExitErrorMessage( Error )

  int Error;
{
  autbegin();

  if ( XschStreamErr != (FILE *)0 )
  {
    fclose( XschStreamErr );
    unlink( XschErrFileName );
  }

  if ( XschStreamAll != (FILE *)0 )
  {
    fclose( XschStreamAll );
    unlink( XschAllFileName );
  }

  if ( ( XschNormalMode             ) &&
       ( XschStreamOut != (FILE *)0 ) )
  {
    fclose( XschStreamOut );
    unlink( XschOutFileName ); 
  }

  autexit( Error );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschInitializeErrorMessage               |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeErrorMessage( Debug )

  char Debug;
{
  autbegin();

  XschNormalMode = ! Debug;

  if ( XschNormalMode )
  {
    sprintf( XschOutFileName, "/tmp/%s_out_%d", PACKAGE, getpid() ); 
  }

  sprintf( XschErrFileName, "/tmp/%s_err_%d", PACKAGE, getpid() );
  sprintf( XschAllFileName, "/tmp/%s_all_%d", PACKAGE, getpid() );

  XschStreamErr = freopen( XschErrFileName, "w+", stderr);
  XschStreamAll = fopen  ( XschAllFileName, "w+"        );

  if ( XschNormalMode )
  {
    XschStreamOut = freopen( XschOutFileName, "w+", stdout);
  }
  else
  {
    XschStreamOut = XschStreamErr;
  }

  if ( ( XschStreamAll == (FILE *)NULL ) ||
       ( XschStreamOut == (FILE *)NULL ) ||
       ( XschStreamErr == (FILE *)NULL ) )
  {
    fprintf( stdout, "Grm: Unable to open trace window !\n");
    autexit( 1 );
  }

  if ( XschNormalMode )
  {
    unlink( XschOutFileName ); 
  }

  unlink( XschErrFileName );

  signal( SIGINT, XschExitErrorMessage );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschFlushErrorMessage                 |
|                                                             |
\------------------------------------------------------------*/

void XschFlushErrorMessage()
{
  int Data;

  autbegin();

  fflush( XschStreamErr );
  fseek( XschStreamErr, 0L, 0 );

  while( ( Data = fgetc( XschStreamErr ) ) != EOF )
  {
    fputc( Data, XschStreamAll );
  }

  fclose( XschStreamErr );

  XschStreamErr = freopen( XschErrFileName, "w+", stderr );

  if ( XschNormalMode )
  {
    fflush( XschStreamOut );
    fseek( XschStreamOut, 0L, 0 );

    while( ( Data = fgetc( XschStreamOut ) ) != EOF )
    {
      fputc( Data, XschStreamAll );
    } 
  
    fclose( XschStreamOut );

    XschStreamOut = freopen( XschOutFileName, "w+", stdout );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschGetErrorMessage                    |
|                                                             |
\------------------------------------------------------------*/

char *XschGetErrorMessage()

{
  char *Message;
  long  Length;
  long  Index;
  int   Data;

  autbegin();

  XschFlushErrorMessage();

  fflush( XschStreamAll );

  Length = ftell( XschStreamAll );

  if ( XschErrorBuffer != (char *)NULL )
  {
    autfreeblock( XschErrorBuffer );
  }

  XschErrorBuffer = autallocblock( Length + 1 );
  Index            = 0;

  while ( ( ( Data = fgetc( XschStreamAll ) ) != EOF ) &&
          ( Index  < Length ) )
  {
    XschErrorBuffer[ Index++ ] = (char)Data;
  }

  rewind( XschStreamAll );

  XschErrorBuffer[ Index ] = '\0';

  if ( Index == 0 )
  {
    Message = (char *)NULL;
  }
  else
  {
    Message = XschErrorBuffer;
  }

  autend();
  return( Message );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschGetInformations                   |
|                                                             |
\------------------------------------------------------------*/

char *XschGetInformations()

{
  char *Scan;

  autbegin();

  XschComputeBound();

  Scan = XschInformationsBuffer;

  if ( XschFigureSch != (schfig_list *)NULL )
  {
    sprintf( Scan, "  FIGURE : %s\n\n",
             XschFigureSch->NAME );
  }
  else
  {
    sprintf( Scan, "  FIGURE : No current figure !\n\n" );
  }

  Scan = Scan + strlen( Scan );

  sprintf( Scan, 
           "  BOUNDING BOX : \n\n  XMIN : %ld\n  YMIN : %ld\n  XMAX : %ld\n  YMAX : %ld\n\n",
           XschBoundXmin / XSCH_UNIT, XschBoundYmin / XSCH_UNIT,
           XschBoundXmax / XSCH_UNIT, XschBoundYmax / XSCH_UNIT );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  XSCH PARAM : %s\n\n", XSCH_PARAM_NAME );

  autend();
  return( XschInformationsBuffer );
}
