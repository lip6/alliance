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
| Tool    :                     SCAN                          |
|                                                             |
| File    :                  scan_main.c                      |
|                                                             |
| Author  :                                                   |
|                                                             |
| Date    :                                                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "mlu.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "scan_main.h"
# include "scan_param.h"
# include "scan_path.h" 
# include "scan_insert.h"

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Scan Usage                          |
|                                                             |
\------------------------------------------------------------*/

void ScanUsage()
{
  fprintf( stdout, "\t\tscapin [Options] Input_name Path_name Output_name\n\n" );

  fprintf( stdout, "\t\tOptions  : -V        Sets Verbose mode on\n" );
  fprintf( stdout, "\t\t           -R        Sets multiplexor-register mode on\n" );
  fprintf( stdout, "\t\t           -B        Sets output buffer mode on\n" );
  fprintf( stdout, "\t\t           -P file   Parameter file\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                            Main                             |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

   int   argc;
   char *argv[];
{
  lofig_list *LoFigure;
  scanparam  *ScanParam;
  pathparam  *PathParam; 
  char       *InputFileName;
  char       *OutputFileName;
  char       *ParamFileName;
  char       *PathFileName; 
  char       *String;
  char        Buffer[ 1024 ];
  int         Number;
  int         Index;
  char        Option;

  int   FlagVerbose = 0;
  int   FlagDebug   = 0;
  int   FlagRegMux  = 0;
  int   FlagBuffer  = 0;

  alliancebanner_with_contrib( 
      "Scapin", VERSION, "SCAn Path INsertion", "2000", ALLIANCE_VERSION,
      "Ludovic Jacomme", "Ilhem Kazi Tani" );

  mbkenv();
  autenv();

  if ( argc < 3 ) ScanUsage();

  OutputFileName = (char *)0;
  InputFileName  = (char *)0;
  ParamFileName  = (char *)0;
  PathFileName   = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        if ( Option == 'P' )
        {
          Number = Number + 1;

          if ( Number < argc )
          {
            ParamFileName = argv[ Number ];

            break;
          }  
          else  ScanUsage();
        }

        switch ( Option )
        {
          case 'V' : FlagVerbose = 1;
          break;
          case 'D' : FlagDebug = 1;
          break;
          case 'R' : FlagRegMux = 1;
          break;
          case 'B' : FlagBuffer = 1;
          break;
          default  : ScanUsage();
        }
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    if ( PathFileName == (char *)0 ) PathFileName = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    ScanUsage();
  }

  if ( ( InputFileName  == (char *)0 ) ||
       ( PathFileName   == (char *)0 ) ||
       ( OutputFileName == (char *)0 ) ) ScanUsage();

  if ( ParamFileName == (char *)0 )
  {
    ParamFileName = mbkgetenv( "SCAPIN_PARAM_NAME" );

    if ( ParamFileName == (char *)NULL )
    {
      String = mbkgetenv( "ALLIANCE_TOP" );
      if ( String == (char *)NULL ) String = ALLIANCE_TOP;

      sprintf( Buffer, "%s/%s", String, SCAPIN_DEFAULT_PARAM_NAME );
      ParamFileName = mbkstrdup( Buffer );
    }
  }

  fprintf( stdout, "\t--> Parse parameter file %s\n", ParamFileName );

  ScanParam = ScanParseParamFile( ParamFileName, FlagDebug );

  fprintf( stdout, "\t--> Parse path file %s\n", PathFileName );

  PathParam = ScanParsePathParamFile( PathFileName, FlagDebug );

  fprintf( stdout, "\t--> Compile Structural file %s\n", InputFileName );

  LoFigure = getlofig( InputFileName, 'A' );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Insert Scan Path\n" );
  }

  ScanInsertScanPath( LoFigure, ScanParam, PathParam, FlagVerbose,
                      FlagDebug, FlagRegMux, FlagBuffer );

  LoFigure->NAME = namealloc( OutputFileName );

  fprintf( stdout, "\t--> Save Structural file %s\n\n", OutputFileName );

  savelofig( LoFigure );

  return( 0 );
}
