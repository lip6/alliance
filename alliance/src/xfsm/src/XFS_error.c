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
| Tool    :                   XFSM                            |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h" 
# include "XFS.h"
# include "XFS_error.h"

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

  static FILE   *XfsmStreamErr;
  static FILE   *XfsmStreamOut;
  static FILE   *XfsmStreamAll;

  static char    XfsmErrFileName[ 40 ];
  static char    XfsmOutFileName[ 40 ];
  static char    XfsmAllFileName[ 40 ];

  static char    XfsmNormalMode = 1;

/*------------------------------------------------------------\
|                                                             |
|                      Xfsm Error Message                     |
|                                                             |
\------------------------------------------------------------*/

  static char *XfsmErrorBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                      Xfsm Informations                      |
|                                                             |
\------------------------------------------------------------*/

  static char XfsmInformationsBuffer[ XFSM_INFORMATIONS_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xfsm Error Message Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XfsmExitErrorMessage                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmExitErrorMessage( Error )

  int Error;
{
  autbegin();

  if ( XfsmStreamErr != (FILE *)0 )
  {
    fclose( XfsmStreamErr );
    unlink( XfsmErrFileName );
  }

  if ( XfsmStreamAll != (FILE *)0 )
  {
    fclose( XfsmStreamAll );
    unlink( XfsmAllFileName );
  }

  if ( ( XfsmNormalMode             ) &&
       ( XfsmStreamOut != (FILE *)0 ) )
  {
    fclose( XfsmStreamOut );
    unlink( XfsmOutFileName ); 
  }

  exit( Error );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmInitializeErrorMessage               |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeErrorMessage( Debug )

  char Debug;
{
  autbegin();

  XfsmNormalMode = ! Debug;

  if ( XfsmNormalMode )
  {
    sprintf( XfsmOutFileName, "/tmp/%s_out_%d", PACKAGE, getpid() ); 
  }

  sprintf( XfsmErrFileName, "/tmp/%s_err_%d", PACKAGE, getpid() );
  sprintf( XfsmAllFileName, "/tmp/%s_all_%d", PACKAGE, getpid() );

  XfsmStreamErr = freopen( XfsmErrFileName, "w+", stderr);
  XfsmStreamAll = fopen  ( XfsmAllFileName, "w+"        );

  if ( XfsmNormalMode )
  {
    XfsmStreamOut = freopen( XfsmOutFileName, "w+", stdout);
  }
  else
  {
    XfsmStreamOut = XfsmStreamErr;
  }

  if ( ( XfsmStreamAll == (FILE *)NULL ) ||
       ( XfsmStreamOut == (FILE *)NULL ) ||
       ( XfsmStreamErr == (FILE *)NULL ) )
  {
    fprintf( stdout, "Grm: Unable to open trace window !\n");
    exit( 1 );
  }

  if ( XfsmNormalMode )
  {
    unlink( XfsmOutFileName ); 
  }

  unlink( XfsmErrFileName );

  signal( SIGINT, XfsmExitErrorMessage );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmFlushErrorMessage                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmFlushErrorMessage()
{
  int Data;

  autbegin();

  fflush( XfsmStreamErr );
  fseek( XfsmStreamErr, 0L, 0 );

  while( ( Data = fgetc( XfsmStreamErr ) ) != EOF )
  {
    fputc( Data, XfsmStreamAll );
  }

  fclose( XfsmStreamErr );

  XfsmStreamErr = freopen( XfsmErrFileName, "w+", stderr );

  if ( XfsmNormalMode )
  {
    fflush( XfsmStreamOut );
    fseek( XfsmStreamOut, 0L, 0 );

    while( ( Data = fgetc( XfsmStreamOut ) ) != EOF )
    {
      fputc( Data, XfsmStreamAll );
    } 
  
    fclose( XfsmStreamOut );

    XfsmStreamOut = freopen( XfsmOutFileName, "w+", stdout );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmGetErrorMessage                    |
|                                                             |
\------------------------------------------------------------*/

char *XfsmGetErrorMessage()

{
  char *Message;
  long  Length;
  long  Index;
  int   Data;

  autbegin();

  XfsmFlushErrorMessage();

  fflush( XfsmStreamAll );

  Length = ftell( XfsmStreamAll );

  if ( XfsmErrorBuffer != (char *)NULL )
  {
    autfreeblock( XfsmErrorBuffer );
  }

  XfsmErrorBuffer = autallocblock( Length + 1 );
  Index            = 0;

  while ( ( ( Data = fgetc( XfsmStreamAll ) ) != EOF ) &&
          ( Index  < Length ) )
  {
    XfsmErrorBuffer[ Index++ ] = (char)Data;
  }

  rewind( XfsmStreamAll );

  XfsmErrorBuffer[ Index ] = '\0';

  if ( Index == 0 )
  {
    Message = (char *)NULL;
  }
  else
  {
    Message = XfsmErrorBuffer;
  }

  autend();
  return( Message );
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmGetInformations                   |
|                                                             |
\------------------------------------------------------------*/

char *XfsmGetInformations()

{
  char *Scan;

  autbegin();

  XfsmComputeBound();

  Scan = XfsmInformationsBuffer;

  if ( XfsmFigureFsm != (fsmfig_list *)NULL )
  {
    sprintf( Scan, "  FIGURE : %s\n\n  STATES : %d\n  TRANSITIONS : %d\n  INPUTS : %d\n  OUTPUTS : %d\n\n", XfsmFigureFsm->NAME, 
        XfsmFigureFsm->NUMBER_STATE,
        XfsmFigureFsm->NUMBER_TRANS,
        XfsmFigureFsm->NUMBER_IN,
        XfsmFigureFsm->NUMBER_OUT );
  }
  else
  {
    sprintf( Scan, "  FIGURE : No current figure !\n\n" );
  }

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  BOUNDING BOX : \n\n  XMIN : %d\n  YMIN : %d\n  XMAX : %d\n  YMAX : %d\n\n",
           XfsmBoundXmin / XFSM_UNIT, XfsmBoundYmin / XFSM_UNIT,
           XfsmBoundXmax / XFSM_UNIT, XfsmBoundYmax / XFSM_UNIT );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  XFSM PARAM : %s\n\n", XFSM_PARAM_NAME );

  autend();
  return( XfsmInformationsBuffer );
}
