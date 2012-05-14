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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                  boom_main.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "boom_shared.h"
# include "boom_parse.h"
# include "boom_check.h"
# include "boom_optim.h"
# include "boom_cost.h"
# include "boom_drive.h"
# include "boom_post.h"
# include "boom_error.h"
# include "boom_debug.h"
# include "boom_param.h"
# include "boom_main.h"

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
|                          Boom Usage                         |
|                                                             |
\------------------------------------------------------------*/

void BoomUsage()
{
  fprintf( stderr, "  boom [Options] [Algorithm] Input_file_name [Output_file_name]\n\n" );

  fprintf( stdout, "  Options    : -V     Sets verbose mode on\n" );
  fprintf( stdout, "               -T     Sets trace and verbose modes on\n" );
  fprintf( stdout, "               -O     Reverses initial Bdd variables order\n" );
  fprintf( stdout, "               -A     Keeps all auxiliary variables\n" );
  fprintf( stdout, "               -P     Uses a parameter file (Input_file_name.boom)\n" );
  fprintf( stdout, "               -L     Uses literal's number for surface estimation\n" );
  fprintf( stdout, "               -l num Optimization level [0-3] (default 0, low level)\n" );
  fprintf( stdout, "               -d num Delay optimization percent (default 0 %%)\n" );
  fprintf( stdout, "               -i num Iteration count\n" );
  fprintf( stdout, "               -a num Amplitude\n" );
  fprintf( stdout, "\n" );
  fprintf( stdout, "  Algorithms : -s     Simulated annealing (default)\n" );
  fprintf( stdout, "               -j     Just do it algorithm\n" );
  fprintf( stdout, "               -b     Burgun algorithm\n" );
  fprintf( stdout, "               -g     Gradient algorithm\n" );
  fprintf( stdout, "               -p     Procrastination algorithm\n" );
  fprintf( stdout, "               -w     Window bdd reorder\n" );
  fprintf( stdout, "               -t     Top bdd reorder\n" );
  fprintf( stdout, "               -m     Simple bdd reorder\n" );
  fprintf( stdout, "               -o     One pass (faster algorithm)\n" );
  fprintf( stdout, "               -r     Random bdd reorder\n" );
  fprintf( stdout, "               -n     No optimization algorithm\n" );
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
  befig_list    *BehFigure;
  boombefiginfo *FigInfo;
  char          *InputFileName;
  char          *OutputFileName;
  int            Number;
  int            Index;
  char           Option;

  int FlagParamFile     = 0;
  int FlagVerbose       = 0;
  int FlagTrace         = 0;
  int FlagInitBddOrder  = 0;
  int FlagKeepAux       = 0;
  int FlagUseLiteral    = 0;
  int OptimAlgorithm    = BOOM_OPTIM_ALGORITHM_ANNEAL;
  int OptimLevel        = 0;
  int OptimDelayPerCent = 0;
  int Iteration         = 0;
  int Amplitude         = 0;

  alliancebanner_with_authors( "BooM", VERSION, "BOOlean Minimization", 
                  "2000", ALLIANCE_VERSION, "Ludovic Jacomme" );
  mbkenv();
  autenv();
  ablenv();
  bddenv();

  if ( argc < 2 ) BoomUsage();

  InputFileName  = (char *)0;
  OutputFileName = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        if ( BoomFlagDebug )
        {
          if ( BoomDebugAddItem( &argv[ Number ][ Index ], BoomFlagDebug ) ) break;
        }

        if ( BoomFlagOption )
        {
          if ( BoomDebugSetOption( &argv[ Number ][ Index ] ) ) break;
        }
        
        if ( Option == 'i' )
        {
          Number = Number + 1;
          if ( Number >= argc ) BoomUsage();

          Iteration = atoi( argv[ Number ] );

          break;
        }
        else
        if ( Option == 'a' )
        {
          Number = Number + 1;
          if ( Number >= argc ) BoomUsage();

          Amplitude = atoi( argv[ Number ] );

          break;
        }
        else
        if ( Option == 'l' )
        {
          Number = Number + 1;
          if ( Number >= argc ) BoomUsage();

          OptimLevel = atoi( argv[ Number ] );

          if ( ( OptimLevel < 0  ) ||
               ( OptimLevel > 3 ) ) BoomUsage();

          break;
        }
        else
        if ( Option == 'd' )
        {
          Number = Number + 1;
          if ( Number >= argc ) BoomUsage();

          OptimDelayPerCent = atoi( argv[ Number ] );

          if ( ( OptimDelayPerCent < 0   ) ||
               ( OptimDelayPerCent > 100 ) ) BoomUsage();

          break;
        }

        switch ( Option )
        {
          case 'V' : FlagVerbose = 1;
          break;
          case 'T' : FlagTrace = 1;
          break;
          case 'O' : FlagInitBddOrder = 1;
          break;
          case 'P' : FlagParamFile = 1;
          break;
          case 'L' : FlagUseLiteral = 1;
          break;
          case 'A' : FlagKeepAux = 1;
          break;
          case 'j' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_SIMUL;
          break;
          case 'm' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_SIMPLE;
          break;
          case 't' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_TOP;
          break;
          case 'w' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_WINDOW;
          break;
          case 'o' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_ONE;
          break;
          case 'r' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_RANDOM;
          break;
          case 'p' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_PROCRAST;
          break;
          case 'b' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_BURGUN;
          break;
          case 'g' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_GRADIENT;
          break;
          case 'n' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_NOTHING;
          break;
          case 's' : OptimAlgorithm = BOOM_OPTIM_ALGORITHM_ANNEAL;
          break;
          case 'D' :

            Index  = Index + 1;
            Option = argv[ Number ][ Index ];

            if ( Option == '0' ) BoomFlagDebug = BOOM_DEBUG_LEVEL0;
            else
            if ( Option == '1' ) BoomFlagDebug = BOOM_DEBUG_LEVEL1;
            else
            if ( Option == '2' ) BoomFlagDebug = BOOM_DEBUG_LEVEL2;
            else
            BoomUsage();
          break;
          default  : BoomUsage();
        }

        if ( Option == 'r' ) break;
      }
    }
    else
    if ( InputFileName  == (char *)0 ) InputFileName  = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    BoomUsage();
  }

  if ( InputFileName  == (char *)0 ) BoomUsage();

  InputFileName = autbasename( InputFileName, "vbe" );

  if ( OutputFileName == (char *)0 )
  {
    OutputFileName = autallocblock( strlen( InputFileName ) + 5 );
    sprintf( OutputFileName, "%s_o", InputFileName );
  }
  else
  {
    OutputFileName = autbasename( OutputFileName, "vbe" );
  }

  fprintf( stdout, "\t--> Parse BEH file %s.vbe\n", InputFileName );

  BehFigure = BoomParseBehFigure( InputFileName );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\n\t--> Check figure %s\n", BehFigure->NAME );
  }

  BoomCheckBehFigure( BehFigure );

  if ( FlagParamFile )
  {
    fprintf( stdout, "\n\t--> Parse parameter file %s.boom\n", InputFileName );
    BoomParseParamFile( BehFigure, InputFileName );
  }

  FigInfo = GetBoomBehFigInfo( BehFigure );

  if ( FlagKeepAux      ) FigInfo->KEEP_AUX       = 1;
  if ( FlagInitBddOrder ) FigInfo->INIT_BDD_ORDER = 1;

  if ( FlagTrace ) FlagVerbose = 1;

  FigInfo->OPTIM_ALGORITHM     = OptimAlgorithm;
  FigInfo->OPTIM_LEVEL         = OptimLevel;
  FigInfo->OPTIM_DELAY_PERCENT = OptimDelayPerCent;
  FigInfo->ITERATION           = Iteration;
  FigInfo->AMPLITUDE           = Amplitude;
  FigInfo->VERBOSE             = FlagVerbose;
  FigInfo->TRACE               = FlagTrace;
  FigInfo->USE_LITERAL         = FlagUseLiteral;

  if ( FlagVerbose )
  {
    fprintf( stdout, "\n\t--> Optimization parameters\n" );
    fprintf( stdout, "\t    Algorithm : %s\n", BoomOptimAlgorithmName[ OptimAlgorithm ] );
    fprintf( stdout, "\t    Keep aux  : %s\n", FlagKeepAux ? "yes" : "no" );
    fprintf( stdout, "\t    Area      : %3d %%\n", 100 - OptimDelayPerCent );
    fprintf( stdout, "\t    Delay     : %3d %%\n", OptimDelayPerCent );
    fprintf( stdout, "\t    Level     : %3d\n", OptimLevel );
    fprintf( stdout, "\n" );
  }

  if ( FlagVerbose )
  {
    BoomBehGiveFullCost( BehFigure, 0 );
    BoomBehPrintCost( "Initial", BehFigure );
    fprintf( stdout, "\n" );
  }

  BoomBehOptimize( BehFigure );

  if ( FlagVerbose )
  {
    BoomBehGiveFullCost( BehFigure, 0 );
    BoomBehPrintCost( "Final", BehFigure );
    fprintf( stdout, "\n" );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Post treat figure %s\n\n", BehFigure->NAME );

    BoomPostTreatBehFigure( BehFigure, OutputFileName );
  }

  fprintf( stdout, "\t--> Drive BEH file %s\n\n", OutputFileName );

  BoomDriveBehFigure( BehFigure, OutputFileName );

  BoomBehFreeBehFig( BehFigure );

  return( 0 );
}
