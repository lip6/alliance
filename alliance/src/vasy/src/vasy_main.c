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
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_main.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   25.08.97                        |
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

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "vex.h"
# include "bdd.h"
# include "vbh.h"
# include "vbl.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"

# include "vasy_error.h"
# include "vasy_debug.h"
# include "vasy_shared.h"
# include "vasy_parse.h"
# include "vasy_preanal.h"
# include "vasy_func.h"
# include "vasy_elabo.h"
# include "vasy_reduce.h"
# include "vasy_analys.h"
# include "vasy_simprtl.h"
# include "vasy_drvalc.h"
# include "vasy_drvsyn.h"
# include "vasy_drvvlog.h"
# include "vasy_drvrtl.h"
# include "vasy_main.h"

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
|                         Vasy Usage                          |
|                                                             |
\------------------------------------------------------------*/

void VasyUsage()
{
  fprintf( stderr, "  vasy [Options] Input_name Output_name\n\n" );

  fprintf( stdout, "  Options : -D [Level] [File] Sets Debug mode on\n"   );
  fprintf( stdout, "            -V                Sets Verbose mode on\n" );
  fprintf( stdout, "            -I Extention      Input file format\n" );
  fprintf( stdout, "            -H                Sets Hierarchical mode on\n" );
  fprintf( stdout, "            -P File           Specifies package list file\n" );
  fprintf( stdout, "            -v                Verilog output\n"  );
  fprintf( stdout, "            -a                Alliance output\n" );
  fprintf( stdout, "            -s                Standard VHDL output\n" );
  fprintf( stdout, "            -r                Rtl output\n" );
  fprintf( stdout, "            -o                Overwrites existing files\n"  );
  fprintf( stdout, "            -i                Drives initials values\n"  );
  fprintf( stdout, "            -p                Adds power supply connectors\n" );
  fprintf( stdout, "            -S                Uses Std_logic instead of Bit\n" );
  fprintf( stdout, "            -C Number_bit     Uses CLA adder (with -a only)\n" );
  fprintf( stdout, "            -E Number_bit     Expands equal operator (with -a only)\n" );
  fprintf( stdout, "            -L                Drives a .lax file (with -a only)\n" );
  fprintf( stdout, "            -B                Drives a .boom file (with -a only)\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        VasyMainTreatModel                   |
|                                                             |
\------------------------------------------------------------*/

vbfig_list *VasyMainTreatModel( InputFileName, OutputFileName, GenMap, Deep )

  char       *InputFileName;
  char       *OutputFileName;
  vbmap_list *GenMap;
  int         Deep;
{
  vbfig_list  *VbhFigure;
  vpnfig_list *VpnFigure;
  rtlfig_list *RtlFigure;
  int          Index;
  char         Tab[ 32 ];

  if ( Deep > 30 ) Deep = 30;

  Tab[ 0 ] = '\t';

  for ( Index = 1; Index < Deep; Index++ )
  {
    Tab[ Index ] = ' ';
  }

  Tab[ Index ] = '\0';

  fprintf( stdout, "%s--> Run VHDL Compiler\n", Tab );
  fprintf( stdout, "%s--> Compile file %s\n", Tab, InputFileName );

  VbhFigure = VasyParseVbhFig( InputFileName, GenMap, Deep );

  if ( VasyFlagVerbose )
  {
    fprintf( stdout, "%s--> Translate to VPN\n", Tab );
  }

  VpnFigure = VasyParseVbh2VpnFig( VbhFigure );

  if ( VasyFlagVerbose )
  {
    fprintf( stdout, "%s--> Preliminary Analysis\n", Tab );
  }

  VasyPreAnalysisVpnFig( VpnFigure );

  if ( VasyFlagVerbose )
  {
    fprintf( stdout, "%s--> In-Line function\n", Tab );
  }

  VasyFuncVpnFig( VpnFigure );

  if ( VasyFlagVerbose )
  {
    fprintf( stdout, "%s--> Elaboration\n", Tab );
  }

  VasyElaborateVpnFig( VpnFigure );

  if ( VasyFlagVerbose )
  {
    fprintf( stdout, "%s--> Petri Net Reduction\n", Tab );
  }

  VasyReduceVpnFig( VpnFigure );

  if ( VasyFlagVerbose )
  {
    fprintf( stdout, "%s--> Analysis and Synthesis\n", Tab );
  }

  RtlFigure = VasyAnalysisVpnFig( VpnFigure );

  if ( VasyFlagVerbose )
  {
    fprintf( stdout, "%s--> Simplify RTL figure\n", Tab );
  }

  VasySimplifyRtlFig( RtlFigure );

  RtlFigure->NAME = namealloc( OutputFileName );

  if ( VasyFlagDrive == 0 )
  {
    fprintf( stdout, "%s--> Drive Verilog file %s\n", Tab, OutputFileName );

    VasyDriveVerilogRtlFig( RtlFigure, OutputFileName );
  }
  else
  if ( VasyFlagDrive == 1 )
  {
    fprintf( stdout, "%s--> Drive Alliance file %s\n", Tab, OutputFileName );

    VasyDriveAllianceRtlFig( RtlFigure, OutputFileName );
  }
  else
  if ( VasyFlagDrive == 2 )
  {
    fprintf( stdout, "%s--> Drive Standard VHDL file %s\n", Tab, OutputFileName );

    VasyDriveSynopsysRtlFig( RtlFigure, OutputFileName, VasyFlagStdLogic, VasyFlagInitial, VasyFlagOver );
  }
  else
  {
    fprintf( stdout, "%s--> Drive Rtl file %s\n", Tab, OutputFileName );

    VasyDriveRtlRtlFig( RtlFigure, OutputFileName );
  }

  return( VbhFigure );
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
  char  *InputFileName;
  char  *OutputFileName;
  char  *PackageFileName;
  char  *InputFormat;
  char   Option;
  int    Number;
  int    Index;


  alliancebanner_with_contrib(
      "VASY", VERSION, "VHDL Analyzer for SYnthesis",
      "2000", ALLIANCE_VERSION, "Ludovic Jacomme", "Frederic Petrot" );

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  vexenv();
  vpnenv();

  if ( argc < 2 ) VasyUsage();

  InputFileName   = (char *)0;
  OutputFileName  = (char *)0;
  PackageFileName = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        if ( VasyFlagDebug )
        {
          if ( ! VasyDebugAddItem( &argv[ Number ][ Index ], VasyFlagDebug ) )
          {
            VasyUsage();
          }

          break;
        }
        else
        if ( VasyFlagOption )
        {
          if ( ! VasyDebugSetOption( &argv[ Number ][ Index ] ) )
          {
            VasyUsage();
          }

          break;
        }
        else
        {
          if ( Option == 'I' )
          {
            Number = Number + 1;

            if ( Number < argc )
            {
              if ( argv[ Number ][ 0 ] == '.' )
              {
                InputFormat = &argv[ Number ][ 1 ];
              }
              else
              {
                InputFormat = argv[ Number ];
              }

              VPN_IN = namealloc( InputFormat );
            }
            else VasyUsage();

            break;
          }
          else
          if ( Option == 'P' )
          {
            Number = Number + 1;

            if ( Number < argc )
            {
              PackageFileName = argv[ Number ];
            }
            else VasyUsage();

            break;
          }
          else
          if ( Option == 'C' )
          {
            Number = Number + 1;

            if ( Number < argc )
            {
              VasyFlagCLA = atoi( argv[ Number ] );
            }
            else VasyUsage();

            break;
          }
          else
          if ( Option == 'E' )
          {
            Number = Number + 1;

            if ( Number < argc )
            {
              VasyFlagEqual = atoi( argv[ Number ] );
            }
            else VasyUsage();

            break;
          }
          
          switch ( Option )
          {
            case 'V' : VasyFlagVerbose = 1;
            break;
            case 'L' : VasyFlagLax = 1;
            break;
            case 'B' : VasyFlagBoom = 1;
            break;
            case 'H' : VasyFlagHier = 1;
            break;
            case 'S' : VasyFlagStdLogic = 1;
            break;
            case 'p' : VasyFlagPower = 1;
            break;
            case 'i' : VasyFlagInitial = 1;
            break;
            case 'o' : VasyFlagOver = 1;
            break;
            case 'v' : VasyFlagDrive   = 0;
            break;
            case 'a' : VasyFlagDrive   = 1;
            break;
            case 's' : VasyFlagDrive   = 2;
            break;
            case 'r' : VasyFlagDrive   = 3;
            break;
            case 'D' :

              Index  = Index + 1;
              Option = argv[ Number ][ Index ];

              if ( Option == '0' ) VasyFlagDebug = VASY_DEBUG_LEVEL0;
              else
              if ( Option == '1' ) VasyFlagDebug = VASY_DEBUG_LEVEL1;
              else
              if ( Option == '2' ) VasyFlagDebug = VASY_DEBUG_LEVEL2;
              else
              VasyUsage();
            break;

            case 'O' :
              VasyFlagOption = 1;
            break;

            default  : VasyUsage();
          }
        }
      }

      VasyFlagDebug  = 0;
      VasyFlagOption = 0;
    }
    else
    if ( InputFileName  == (char *)0 ) InputFileName  = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    VasyUsage();
  }

  if ( InputFileName == (char *)0 ) VasyUsage();

  InputFileName = autbasename( InputFileName, VPN_IN );

  if ( OutputFileName == (char *)0 )
  {
    OutputFileName = InputFileName;
  }

  if ( IsVasyDebugStatistics() ) VasyDebugStartChrono(0);
  
  if ( PackageFileName != (char *)0 )
  {
    VasyParsePackage( PackageFileName );
  }

  VasyMainTreatModel( InputFileName, OutputFileName, (vbmap_list *)0, 1 );

  vbh_frevbfig( VBL_HEADFIG );

  if ( IsVasyDebugStatistics() )
  {
    VasyPrintf( stdout, "--> Total time : %ld sec\n", VasyDebugReadChrono(0) );
  }

  fprintf( stdout, "\n" );


  return( 0 );
}
