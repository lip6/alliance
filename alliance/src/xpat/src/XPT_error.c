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
| Tool    :                   XPAT                            |
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
# include <string.h>
# include <signal.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XSB.h" 
# include "XPT.h"
# include "XPT_error.h"

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

  static FILE   *XpatStreamErr;
  static FILE   *XpatStreamOut;
  static FILE   *XpatStreamAll;

  static char    XpatErrFileName[ 40 ];
  static char    XpatOutFileName[ 40 ];
  static char    XpatAllFileName[ 40 ];

  static char    XpatNormalMode = 1;

/*------------------------------------------------------------\
|                                                             |
|                      Xpat Error Message                    |
|                                                             |
\------------------------------------------------------------*/

  static char *XpatErrorBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                      Xpat Informations                     |
|                                                             |
\------------------------------------------------------------*/

  static char XpatInformationsBuffer[ XPAT_INFORMATIONS_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xpat Error Message Functions               |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XpatExitErrorMessage                     |
|                                                             |
\------------------------------------------------------------*/

void XpatExitErrorMessage( Error )

  int Error;
{
  autbegin();

  if ( XpatStreamErr != (FILE *)0 )
  {
    fclose( XpatStreamErr );
    unlink( XpatErrFileName );
  }

  if ( XpatStreamAll != (FILE *)0 )
  {
    fclose( XpatStreamAll );
    unlink( XpatAllFileName );
  }

  if ( ( XpatNormalMode             ) &&
       ( XpatStreamOut != (FILE *)0 ) )
  {
    fclose( XpatStreamOut );
    unlink( XpatOutFileName ); 
  }

  exit( Error );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatInitializeErrorMessage               |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeErrorMessage( Debug )

  char Debug;
{
  autbegin();

  XpatNormalMode = ! Debug;

  if ( XpatNormalMode )
  {
    sprintf( XpatOutFileName, "/tmp/%s_out_%d", PACKAGE, getpid() ); 
  }

  sprintf( XpatErrFileName, "/tmp/%s_err_%d", PACKAGE, getpid() );
  sprintf( XpatAllFileName, "/tmp/%s_all_%d", PACKAGE, getpid() );

  XpatStreamErr = freopen( XpatErrFileName, "w+", stderr);
  XpatStreamAll = fopen  ( XpatAllFileName, "w+"        );

  if ( XpatNormalMode )
  {
    XpatStreamOut = freopen( XpatOutFileName, "w+", stdout);
  }
  else
  {
    XpatStreamOut = XpatStreamErr;
  }

  if ( ( XpatStreamAll == (FILE *)NULL ) ||
       ( XpatStreamOut == (FILE *)NULL ) ||
       ( XpatStreamErr == (FILE *)NULL ) )
  {
    fprintf( stdout, "Grm: Unable to open trace window !\n");
    exit( 1 );
  }

  if ( XpatNormalMode )
  {
    unlink( XpatOutFileName ); 
  }

  unlink( XpatErrFileName );

  signal( SIGINT, XpatExitErrorMessage );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatFlushErrorMessage                 |
|                                                             |
\------------------------------------------------------------*/

void XpatFlushErrorMessage()
{
  int Data;

  autbegin();

  fflush( XpatStreamErr );
  fseek( XpatStreamErr, 0L, 0 );

  while( ( Data = fgetc( XpatStreamErr ) ) != EOF )
  {
    fputc( Data, XpatStreamAll );
  }

  fclose( XpatStreamErr );

  XpatStreamErr = freopen( XpatErrFileName, "w+", stderr );

  if ( XpatNormalMode )
  {
    fflush( XpatStreamOut );
    fseek( XpatStreamOut, 0L, 0 );

    while( ( Data = fgetc( XpatStreamOut ) ) != EOF )
    {
      fputc( Data, XpatStreamAll );
    } 
  
    fclose( XpatStreamOut );

    XpatStreamOut = freopen( XpatOutFileName, "w+", stdout );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatGetErrorMessage                    |
|                                                             |
\------------------------------------------------------------*/

char *XpatGetErrorMessage()

{
  char *Message;
  long  Length;
  long  Index;
  int   Data;

  autbegin();

  XpatFlushErrorMessage();

  fflush( XpatStreamAll );

  Length = ftell( XpatStreamAll );

  if ( XpatErrorBuffer != (char *)NULL )
  {
    autfreeblock( XpatErrorBuffer );
  }

  XpatErrorBuffer = autallocblock( Length + 1 );
  Index            = 0;

  while ( ( ( Data = fgetc( XpatStreamAll ) ) != EOF ) &&
          ( Index  < Length ) )
  {
    XpatErrorBuffer[ Index++ ] = (char)Data;
  }

  rewind( XpatStreamAll );

  XpatErrorBuffer[ Index ] = '\0';

  if ( Index == 0 )
  {
    Message = (char *)NULL;
  }
  else
  {
    Message = XpatErrorBuffer;
  }

  autend();
  return( Message );
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatGetInformations                   |
|                                                             |
\------------------------------------------------------------*/

char *XpatGetInformations()

{
  char *Scan;

  autbegin();

  XpatComputeBound();

  Scan = XpatInformationsBuffer;

  if ( XpatFigurePat != (patfig_list *)NULL )
  {
    sprintf( Scan, "  FIGURE : %s\n\n  PATTERNS : %d\n\n",
             XpatFigurePat->NAME,
             XpatFigurePat->SEQUENCE->PATNBR );
  }
  else
  {
    sprintf( Scan, "  FIGURE : No current figure !\n\n" );
  }

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  BOUNDING BOX : \n\n  XMIN : %d\n  YMIN : %d\n  XMAX : %d\n  YMAX : %d\n\n",
           XpatBoundXmin / XPAT_UNIT, XpatBoundYmin / XPAT_UNIT,
           XpatBoundXmax / XPAT_UNIT, XpatBoundYmax / XPAT_UNIT );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  XPAT PARAM : %s\n\n", XPAT_PARAM_NAME );

  autend();
  return( XpatInformationsBuffer );
}
