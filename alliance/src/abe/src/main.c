/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| Tool    :                     Beh                           |
|                                                             |
| File    :                    main.c                         |
|                                                             |
| Date    :                   08.02.95                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

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
|                            Usage                            |
|                                                             |
\------------------------------------------------------------*/

void BehUsage()
{
  fprintf( stderr, "\t\tabetest [Options] Input_name [Output_name]\n\n"    );
  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n" );
  fprintf( stdout, "\t\tOptions : -E Erases auxialiry signals\n" );
  fprintf( stdout, "\t\tOptions : -D Displays beh figure\n" );
  fprintf( stdout, "\t\tOptions : -B Makes BDD nodes\n" );
  fprintf( stdout, "\t\tOptions : -S Saves beh figure    \n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             main                            |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

  int   argc;
  char *argv[];
{
  befig_list *BehFigure;
  char       *InputFileName;
  char       *OutputFileName;
  int         Number;
  int         Index;
  char        Option;

  int   FlagVerbose  = 0;
  int   FlagSave     = 0;
  int   FlagDisplay  = 0;
  int   FlagErase    = 0;
  int   FlagBdd      = 0;

  mbkenv();
  autenv();
  ablenv();
  bddenv();

  InputFileName  = (char *)0;
  OutputFileName = (char *)0;

  if ( argc < 2 ) BehUsage();

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        switch ( Option )
        {
          case 'E' : FlagErase = 1;
          break;
          case 'B' : FlagBdd = 1;
          break;
          case 'D' : FlagDisplay = 1;
          break;
          case 'V' : FlagVerbose = 1;
          break;
          case 'S' : FlagSave    = 1;
          break;
          default  : BehUsage();
        }
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    BehUsage();
  }

  if ( InputFileName  == (char *)0 ) BehUsage();
  if ( OutputFileName == (char *)0 ) OutputFileName = InputFileName;

  if ( FlagVerbose )
  {
    fprintf( stdout, "vhdlloadbefig %s\n", InputFileName );
  }

  BehFigure = vhdlloadbefig( (befig_list *)0, InputFileName, 3 );

  if ( FlagVerbose )
  {
    fprintf( stdout, "Figure %s loaded\n", BehFigure->NAME );
  }

/*\
  if ( FlagBdd )
  {
    BddSystem = createbddsystem( 100, 1000, 100, 5000000 );
    reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 100000, 100 );

    if ( FlagVerbose )
    {
      fprintf( stdout, "Makes BDD for %s\n", BehFigure->NAME );
    }

    BehFigure->BEDLY = (beaux_list *)0;
    beh_makbdd( BehFigure, ! FlagErase, 0 );

    testbddcircuit( (bddcircuit *)0 );
  
    destroybddcircuit( BehFigure->CIRCUI );
    destroybddsystem( BddSystem );
  }
  else
  if ( FlagErase )
  {
    if ( FlagVerbose )
    {
      fprintf( stdout, "Erases auxialiary signals in %s\n", BehFigure->NAME );
    }

    beh_delauxabl( BehFigure );
  }
\*/

  if ( FlagSave )
  {
    BehFigure->NAME = namealloc( OutputFileName );

    if ( FlagVerbose )
    {
      fprintf( stdout, "vhdlsavebefig %s\n", BehFigure->NAME );
    }
    vhdlsavebefig ( BehFigure,  0 );

    if ( FlagVerbose )
    {
      fprintf( stdout, "Figure %s saved\n", BehFigure->NAME );
    }
  }

  if ( FlagDisplay )
  {
    beh_viewbefig( BehFigure );
  }

  beh_frebefig( BehFigure );

  return( 0 );
}
