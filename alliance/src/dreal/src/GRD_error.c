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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Error.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include "GRD.h"
# include "GRD_error.h"

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

  static FILE   *DrealStreamErr;
  static FILE   *DrealStreamOut;
  static FILE   *DrealStreamAll;

  static char    DrealErrFileName[ 40 ];
  static char    DrealOutFileName[ 40 ];
  static char    DrealAllFileName[ 40 ];

  static char    DrealNormalMode = 1;

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Error Message                    |
|                                                             |
\------------------------------------------------------------*/

  static char *DrealErrorBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Informations                     |
|                                                             |
\------------------------------------------------------------*/

  static char DrealInformationsBuffer[ DREAL_INFORMATIONS_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Dreal Error Message Functions               |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    DrealExitCloseStream                     |
|                                                             |
\------------------------------------------------------------*/

void DrealExitCloseStream()
{
  rdsbegin();

  if ( DrealStreamErr != (FILE *)0 )
  {
    fclose( DrealStreamErr );
    unlink( DrealErrFileName );
  }

  if ( DrealStreamAll != (FILE *)0 )
  {
    fclose( DrealStreamAll );
    unlink( DrealAllFileName );
  }

  if ( ( DrealNormalMode             ) &&
       ( DrealStreamOut != (FILE *)0 ) )
  {
    fclose( DrealStreamOut );
    unlink( DrealOutFileName ); 
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealExitFatalError                      |
|                                                             |
\------------------------------------------------------------*/

void DrealExitFatalError()
{
  rdsbegin();

  DrealExitCloseStream();

  if ( DrealHeadUndo != (drealundo *)NULL )
  {
    DrealFigureRds->NAME = namealloc( "dreal_dump" );
    DrealSaveFigure();
  }

  exit( 2 );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealExitErrorMessage                     |
|                                                             |
\------------------------------------------------------------*/

void DrealExitErrorMessage( Error )

  int Error;
{
  rdsbegin();

  DrealExitCloseStream();

  if ( DrealHeadUndo != (drealundo *)NULL )
  {
    CallbackFileSaveAs( NULL, NULL, NULL );
  }

  exit( Error );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealInitializeErrorMessage              |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeErrorMessage( Debug )

  char Debug;
{
  rdsbegin();

  DrealNormalMode = ! Debug;

  if ( DrealNormalMode )
  {
    sprintf( DrealOutFileName, "/tmp/%s_out_%d", PACKAGE, getpid() ); 
  }

  sprintf( DrealErrFileName, "/tmp/%s_err_%d", PACKAGE, getpid() );
  sprintf( DrealAllFileName, "/tmp/%s_all_%d", PACKAGE, getpid() );

  DrealStreamErr = freopen( DrealErrFileName, "w+", stderr);
  DrealStreamAll = fopen  ( DrealAllFileName, "w+"        );

  if ( DrealNormalMode )
  {
    DrealStreamOut = freopen( DrealOutFileName, "w+", stdout);
  }
  else
  {
    DrealStreamOut = DrealStreamErr;
  }

  if ( ( DrealStreamAll == (FILE *)NULL ) ||
       ( DrealStreamOut == (FILE *)NULL ) ||
       ( DrealStreamErr == (FILE *)NULL ) )
  {
    fprintf( stdout, "Grm: Unable to open trace window !\n");
    exit( 1 );
  }

  if ( DrealNormalMode )
  {
    unlink( DrealOutFileName ); 
  }

  unlink( DrealErrFileName );

  signal( SIGINT, DrealExitErrorMessage );

  if ( ! Debug )
  {
    signal( SIGSEGV, DrealExitFatalError );
    signal( SIGUSR1, DrealExitFatalError );
    signal( SIGUSR2, DrealExitFatalError );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealFlushErrorMessage                 |
|                                                             |
\------------------------------------------------------------*/

void DrealFlushErrorMessage()
{
  int Data;

  rdsbegin();

  fflush( DrealStreamErr );
  fseek( DrealStreamErr, 0L, 0 );

  while( ( Data = fgetc( DrealStreamErr ) ) != EOF )
  {
    fputc( Data, DrealStreamAll );
  }

  fclose( DrealStreamErr );

  DrealStreamErr = freopen( DrealErrFileName, "w+", stderr );

  if ( DrealNormalMode )
  {
    fflush( DrealStreamOut );
    fseek( DrealStreamOut, 0L, 0 );

    while( ( Data = fgetc( DrealStreamOut ) ) != EOF )
    {
      fputc( Data, DrealStreamAll );
    } 

    fclose( DrealStreamOut );

    DrealStreamOut = freopen( DrealOutFileName, "w+", stdout );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealGetErrorMessage                   |
|                                                             |
\------------------------------------------------------------*/

char *DrealGetErrorMessage()
{
  char *Message;
  long  Length;
  long  Index;
  int   Data;

  rdsbegin();

  DrealFlushErrorMessage();

  fflush( DrealStreamAll );

  Length = ftell( DrealStreamAll );

  if ( DrealErrorBuffer != (char *)NULL )
  {
    rdsfreeblock( DrealErrorBuffer );
  }

  DrealErrorBuffer = rdsallocblock( Length + 1 );
  Index            = 0;

  while ( ( ( Data = fgetc( DrealStreamAll ) ) != EOF ) &&
          ( Index  < Length ) )
  {
    DrealErrorBuffer[ Index++ ] = (char)Data;
  }

  rewind( DrealStreamAll );

  DrealErrorBuffer[ Index ] = '\0';

  if ( Index == 0 )
  {
    Message = (char *)NULL;
  }
  else
  {
    Message = DrealErrorBuffer;
  }

  rdsend();
  return( Message );
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealGetInformations                   |
|                                                             |
\------------------------------------------------------------*/

char *DrealGetInformations()

{
  char *Scan;

  rdsbegin();

  DrealComputeBound();

  Scan = DrealInformationsBuffer;

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    sprintf( Scan, "  FIGURE : %s\n\n  XAB1 : %.3f\n  YAB1 : %.3f\n  XAB2 : %.3f\n  YAB2 : %.3f\n\n",
             DrealFigureRds->NAME,
             DrealFigureXAB1 * DREAL_RDS_UNIT_TO_MICRON,
             DrealFigureYAB1 * DREAL_RDS_UNIT_TO_MICRON,
             DrealFigureXAB2 * DREAL_RDS_UNIT_TO_MICRON,
             DrealFigureYAB2 * DREAL_RDS_UNIT_TO_MICRON  );
  }
  else
  {
    sprintf( Scan, "  FIGURE : No current figure !\n\n" );
  }

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  BOUNDING BOX : \n\n  XMIN : %.3f\n  YMIN : %.3f\n  XMAX : %.3f\n  YMAX : %.3f\n\n",
           (float)( DrealBoundXmin ) * DREAL_RDS_UNIT_TO_MICRON,
           (float)( DrealBoundYmin ) * DREAL_RDS_UNIT_TO_MICRON,
           (float)( DrealBoundXmax ) * DREAL_RDS_UNIT_TO_MICRON,
           (float)( DrealBoundYmax ) * DREAL_RDS_UNIT_TO_MICRON );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  WINDOWS : \n\n  XMIN : %.3f\n  YMIN : %.3f\n  XMAX : %.3f\n  YMAX : %.3f\n  DX : %d\n  DY : %d\n  SIDE : %.3f\n\n",
           (float)( DrealWindowXmin ) * DREAL_RDS_UNIT_TO_MICRON,
           (float)( DrealWindowYmin ) * DREAL_RDS_UNIT_TO_MICRON,
           (float)( DrealWindowXmax ) * DREAL_RDS_UNIT_TO_MICRON,
           (float)( DrealWindowYmax ) * DREAL_RDS_UNIT_TO_MICRON,
           DrealWindowDx,
           DrealWindowDy,
           (float)( DrealWindowSide ) * DREAL_RDS_UNIT_TO_MICRON );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  RDS TECHNOLOGY : %s\n\n", RDS_TECHNO_NAME );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  DREAL TECHNOLOGY : %s\n\n", DREAL_TECHNO_NAME );


  rdsend();
  return( DrealInformationsBuffer );
}
