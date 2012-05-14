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
| Tool    :                     FMI                           |
|                                                             |
| File    :                  fmi_main.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.11.94                        |
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
# include "fsm.h"
# include "ftl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "fmi_parse.h"
# include "fmi_bdd.h"
# include "fmi_optim.h"
# include "fmi_main.h"

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
|                          Fmi Usage                          |
|                                                             |
\------------------------------------------------------------*/

void FmiUsage()
{
  fprintf( stderr, "\t\tfmi [Options] filename1 filename2\n\n" );

  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n"       );
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
  char  *InputFileName;
  char  *OutputFileName;
  int    Number;

  fsmfig_list *FsmFigure;

  int    FlagVerbose = 0;

  alliancebanner_with_contrib( "FMI", VERSION, "FSM Minimization", "2000",
      ALLIANCE_VERSION, "Frédéric Pétrot, Ludovic Jacomme", "Jean-Marie Alexandre" );

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  fsmenv();

  if ( argc < 1 ) FmiUsage();

  InputFileName  = (char *)0;
  OutputFileName = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( ! strcmp( argv[ Number ], "-V" ) ) FlagVerbose = 1;
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    FmiUsage();
  }

  if ( InputFileName == (char *)0 ) FmiUsage();

  InputFileName = autbasename( InputFileName, FSM_IN );

  if ( OutputFileName == (char *)0 )
  {
    OutputFileName = autallocblock( strlen( InputFileName ) + 3 );
    sprintf( OutputFileName, "%sfmi", InputFileName );
  }
  else
  {
    OutputFileName = autbasename( OutputFileName, FSM_IN );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Run FSM Compiler\n" );
    fprintf( stdout, "\t--> Compile file %s\n", InputFileName );
  }

  FsmFigure = FmiParseFsm( InputFileName );

  if ( ( IsFsmFigMulti( FsmFigure    ) ) ||
       ( IsFsmFigMixedRtl( FsmFigure ) ) )
  {
    fprintf( stderr, "\tMulti FSM or Mixed RTL FSM not supported !\n" );
    autexit( 1 );
  }
  
  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Build Binay Decision Diagrams\n" );
  }

  FmiMakeFsmBddCircuit( FsmFigure );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Identify equivalent states\n" );
  }

  FsmFigure = FmiOptimizeFsm( FsmFigure );
 
  FsmFigure->NAME = namealloc( OutputFileName );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Save file %s\n", OutputFileName );
  }

  savefsmfig( FsmFigure );

  return( 0 );
}
