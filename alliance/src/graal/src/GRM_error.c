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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Error.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
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
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "rtl.h"
# include "GSB.h" 
# include "GRM.h"
# include "GRM_error.h"

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

  static FILE   *GraalStreamErr;
  static FILE   *GraalStreamOut;
  static FILE   *GraalStreamAll;

  static char    GraalErrFileName[ 40 ];
  static char    GraalOutFileName[ 40 ];
  static char    GraalAllFileName[ 40 ];

  static char    GraalNormalMode = 1;

/*------------------------------------------------------------\
|                                                             |
|                      Graal Error Message                    |
|                                                             |
\------------------------------------------------------------*/

  static char *GraalErrorBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                      Graal Informations                     |
|                                                             |
\------------------------------------------------------------*/

  static char GraalInformationsBuffer[ GRAAL_INFORMATIONS_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Graal Error Message Functions               |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    GraalExitCloseStream                     |
|                                                             |
\------------------------------------------------------------*/

void GraalExitCloseStream()
{
  rdsbegin();

  if ( GraalStreamErr != (FILE *)0 )
  {
    fclose( GraalStreamErr );
    unlink( GraalErrFileName );
  }

  if ( GraalStreamAll != (FILE *)0 )
  {
    fclose( GraalStreamAll );
    unlink( GraalAllFileName );
  }

  if ( ( GraalNormalMode             ) &&
       ( GraalStreamOut != (FILE *)0 ) )
  {
    fclose( GraalStreamOut );
    unlink( GraalOutFileName ); 
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalExitFatalError                      |
|                                                             |
\------------------------------------------------------------*/

void GraalExitFatalError()
{
  rdsbegin();

  GraalExitCloseStream();

  if ( GraalHeadUndo != (graalundo *)NULL )
  {
    GraalFigureMbk->NAME = namealloc( "graal_dump" );
    GraalSaveFigure();
  }

  exit( 2 );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalExitErrorMessage                     |
|                                                             |
\------------------------------------------------------------*/

void GraalExitErrorMessage( Error )

  int Error;
{
  rdsbegin();

  GraalExitCloseStream();

  if ( GraalHeadUndo != (graalundo *)NULL )
  {
    CallbackFileSaveAs( NULL, NULL, NULL );
  }

  exit( Error );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalInitializeErrorMessage               |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeErrorMessage( Debug )

  char Debug;
{
  rdsbegin();

  GraalNormalMode = ! Debug;

  if ( GraalNormalMode )
  {
    sprintf( GraalOutFileName, "/tmp/%s_out_%d", GRAAL_TOOL_NAME, getpid() ); 
  }

  sprintf( GraalErrFileName, "/tmp/%s_err_%d", GRAAL_TOOL_NAME, getpid() );
  sprintf( GraalAllFileName, "/tmp/%s_all_%d", GRAAL_TOOL_NAME, getpid() );

  GraalStreamErr = freopen( GraalErrFileName, "w+", stderr);
  GraalStreamAll = fopen  ( GraalAllFileName, "w+"        );

  if ( GraalNormalMode )
  {
    GraalStreamOut = freopen( GraalOutFileName, "w+", stdout);
  }
  else
  {
    GraalStreamOut = GraalStreamErr;
  }

  if ( ( GraalStreamAll == (FILE *)NULL ) ||
       ( GraalStreamOut == (FILE *)NULL ) ||
       ( GraalStreamErr == (FILE *)NULL ) )
  {
    fprintf( stdout, "Grm: Unable to open trace window !\n");
    exit( 1 );
  }

  if ( GraalNormalMode )
  {
    unlink( GraalOutFileName ); 
  }

  unlink( GraalErrFileName );

  signal( SIGINT, GraalExitErrorMessage );

  if ( ! Debug )
  {
    signal( SIGSEGV, GraalExitFatalError );
    signal( SIGUSR1, GraalExitFatalError );
    signal( SIGUSR2, GraalExitFatalError );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalFlushErrorMessage                 |
|                                                             |
\------------------------------------------------------------*/

void GraalFlushErrorMessage()
{
  int Data;

  rdsbegin();

  fflush( GraalStreamErr );
  fseek( GraalStreamErr, 0L, 0 );

  while( ( Data = fgetc( GraalStreamErr ) ) != EOF )
  {
    fputc( Data, GraalStreamAll );
  }

  fclose( GraalStreamErr );

  GraalStreamErr = freopen( GraalErrFileName, "w+", stderr );

  if ( GraalNormalMode )
  {
    fflush( GraalStreamOut );
    fseek( GraalStreamOut, 0L, 0 );

    while( ( Data = fgetc( GraalStreamOut ) ) != EOF )
    {
      fputc( Data, GraalStreamAll );
    } 
  
    fclose( GraalStreamOut );

    GraalStreamOut = freopen( GraalOutFileName, "w+", stdout );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalGetErrorMessage                   |
|                                                             |
\------------------------------------------------------------*/

char *GraalGetErrorMessage()

{
  char *Message;
  long  Length;
  long  Index;
  int   Data;

  rdsbegin();

  GraalFlushErrorMessage();

  fflush( GraalStreamAll );

  Length = ftell( GraalStreamAll );

  if ( GraalErrorBuffer != (char *)NULL )
  {
    rdsfreeblock( GraalErrorBuffer );
  }

  GraalErrorBuffer = rdsallocblock( Length + 1 );
  Index            = 0;

  while ( ( ( Data = fgetc( GraalStreamAll ) ) != EOF ) &&
          ( Index  < Length ) )
  {
    GraalErrorBuffer[ Index++ ] = (char)Data;
  }

  rewind( GraalStreamAll );

  GraalErrorBuffer[ Index ] = '\0';

  if ( Index == 0 )
  {
    Message = (char *)NULL;
  }
  else
  {
    Message = GraalErrorBuffer;
  }

  rdsend();
  return( Message );
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalGetInformations                   |
|                                                             |
\------------------------------------------------------------*/

char *GraalGetInformations()

{
  char *Scan;

  rdsbegin();

  GraalComputeBound();

  Scan = GraalInformationsBuffer;

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    sprintf( Scan, "  FIGURE : %s\n\n  XAB1 : %d\n  YAB1 : %d\n  XAB2 : %d\n  YAB2 : %d\n\n",
             GraalFigureMbk->NAME,
             GraalFigureMbk->XAB1  / GRAAL_SCALE_X,
             GraalFigureMbk->YAB1  / GRAAL_SCALE_X,
             GraalFigureMbk->XAB2  / GRAAL_SCALE_X,
             GraalFigureMbk->YAB2  / GRAAL_SCALE_X  );
  }
  else
  {
    sprintf( Scan, "  FIGURE : No current figure !\n\n" );
  }

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  BOUNDING BOX : \n\n  XMIN : %d\n  YMIN : %d\n  XMAX : %d\n  YMAX : %d\n\n",
           GraalBoundXmin / GRAAL_RDS_LAMBDA, GraalBoundYmin / GRAAL_RDS_LAMBDA,
           GraalBoundXmax / GRAAL_RDS_LAMBDA, GraalBoundYmax / GRAAL_RDS_LAMBDA );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  WINDOWS : \n\n  XMIN : %d\n  YMIN : %d\n  XMAX : %d\n  YMAX : %d\n  DX : %d\n  DY : %d\n  SIDE : %d\n\n",
           GraalWindowXmin / GRAAL_RDS_LAMBDA, GraalWindowYmin / GRAAL_RDS_LAMBDA,
           GraalWindowXmax / GRAAL_RDS_LAMBDA, GraalWindowYmax / GRAAL_RDS_LAMBDA,
           GraalWindowDx               , GraalWindowDy,
           GraalWindowSide / GRAAL_RDS_LAMBDA );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  RDS TECHNOLOGY : %s\n\n", RDS_TECHNO_NAME );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  GRAAL TECHNOLOGY : %s\n\n", GRAAL_TECHNO_NAME );

  rdsend();
  return( GraalInformationsBuffer );
}
