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
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_main.c                     |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   05.13.02                        |
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
# include "btr.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"
# include "abe.h"
# include "fsm.h"
# include "ftl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mocha_debug.h"
# include "mocha_shared.h"
# include "mocha_ctl.h"
# include "mocha_fsm.h"
# include "mocha_beh.h"
# include "mocha_bdd.h"
# include "mocha_check.h"
# include "mocha_main.h"

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

   mochafig_list *MochaFigure    = (mochafig_list  *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Mocha Usage                        |
|                                                             |
\------------------------------------------------------------*/

void MochaUsage()
{
  fprintf( stderr, "\t\tmocha [Options] fsm_filename ctl_filename\n\n" );

  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n"       );
  fprintf( stdout, "\t\t          -D Sets Debug mode on\n"         );
  fprintf( stdout, "\t\t          -B Sets VBE mode on\n"           );
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
  char  *DescFileName;
  char  *CtlFileName;
  int    Number;
  int    Index;
  char   Option;

  int    FlagVerbose = 0;
  int    FlagDebug   = 0;
  int    FlagFsm     = 1;

  alliancebanner_with_authors( "MoChA",
      VERSION, "Model Checker Ancestor", "2002",
      ALLIANCE_VERSION, "Ludovic Jacomme" );

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  fsmenv();
  vexenv();
  ctlenv();

  if ( argc < 2 ) MochaUsage();

  DescFileName = (char *)0;
  CtlFileName  = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        switch ( Option )
        {
          case 'V' : FlagVerbose = 1;
          break;
          case 'B' : FlagFsm = 0;
          break;
          case 'D' : FlagDebug = 1;
          break;
          default  : MochaUsage();
        }

        if ( Option == 'r' ) break;
      }
    }
    else
    if ( DescFileName == (char *)0 ) DescFileName = argv[ Number ];
    else
    if ( CtlFileName  == (char *)0 ) CtlFileName  = argv[ Number ];
    else
    MochaUsage();
  }

  if ( ( DescFileName == (char *)0 ) ||
       ( CtlFileName  == (char *)0 ) ) MochaUsage();

  MochaFigure = MochaAddFigure( DescFileName );

  if ( FlagFsm )
  {
    if ( FlagVerbose )
    {
      fprintf( stdout, "\t--> Running FSM Compiler\n" );
      fprintf( stdout, "\t    Compiling file %s\n", DescFileName );
    }

    MochaCompileFsm( MochaFigure, DescFileName, FlagVerbose );
  }
  else
  {
    if ( FlagVerbose )
    {
      fprintf( stdout, "\t--> Running BEH Compiler\n" );
      fprintf( stdout, "\t    Compiling file %s\n", DescFileName );
    }

    MochaCompileBeh( MochaFigure, DescFileName, FlagVerbose );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Verifying BEH figure\n" );
  }

  MochaPostTreatVerifyBeh( MochaFigure, FlagVerbose );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Running CTL Compiler\n" );
    fprintf( stdout, "\t    Compiling file %s\n", CtlFileName );
  }

  MochaCompileCtl( MochaFigure, CtlFileName, FlagVerbose );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Building BDDs\n" );
  }

  MochaBuildBdd( MochaFigure, FlagVerbose );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Model checking of \"%s\"\n", MochaFigure->NAME );
  }

  if ( MochaCheckModel( MochaFigure, FlagVerbose ) )
  {
    /* TO BE DONE */

    return( 1 );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t<-- done\n" );
  }

  return( 0 );
}
