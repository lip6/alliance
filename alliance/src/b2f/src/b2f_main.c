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
| Tool    :                     B2F                           |
|                                                             |
| File    :                  b2f_main.c                       |
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
# include "abe.h"
# include "abv.h"
# include "abt.h"
# include "ftl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "b2f_beh2fsm.h"
# include "b2f_error.h"
# include "b2f_main.h"

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

   befig_list  *B2fBehFigure = (befig_list     *)0;
   fsmfig_list *B2fFsmFigure = (fsmfig_list    *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          B2f Usage                          |
|                                                             |
\------------------------------------------------------------*/

void B2fUsage()
{
  fprintf( stderr, "\t\tb2f [Options] Input_name Output_name\n\n" );

  fprintf( stdout, "\t\tOptions :\n" );
  fprintf( stdout, "\t\t -V             Sets Verbose mode on\n" );
  fprintf( stdout, "\t\t -I bit_string  Initial value of the state register\n" );
  fprintf( stdout, "\t\t -O o0,...,on   Initial value (one index list)\n" );
  fprintf( stdout, "\t\t -Z z0,...,zn   Initial value (zero index list)\n" );
  fprintf( stdout, "\t\t -R reset_cond  Reset condition\n" );
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
  char       *InputFileName;
  char       *OutputFileName;
  int         Number;
  int         Index;
  int         Skip;
  char        Option;

  int    FlagVerbose   = 0;
  int    FlagInitial   = 0;
  char  *StringInitial = (char *)0;

  alliancebanner_with_authors(
      "B2F", VERSION, "Translator Behavior To FSM Format", 
      "1999", ALLIANCE_VERSION, "Ludovic Jacomme" );
  mbkenv();
  autenv();
  ablenv();
  bddenv();
  fsmenv();

  if ( argc < 2 ) B2fUsage();

  InputFileName  = (char *)0;
  OutputFileName = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      Skip = 0;

      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        switch ( Option )
        {
          case 'V' : FlagVerbose = 1;
          break;
          case 'I' :
          case 'Z' :
          case 'O' :
          case 'R' : Number = Number + 1;

                     if ( ( Number >= argc ) ||
                          ( FlagInitial    ) ) B2fUsage();

                     FlagInitial   = Option;
                     StringInitial = argv[ Number ];
                     Skip          = 1;
          break;
          default  : B2fUsage();
        }

        if ( Skip ) break;
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    B2fUsage();
  }

  if ( ( InputFileName  == (char *)0 ) ||
       ( OutputFileName == (char *)0 ) ) B2fUsage();

  fprintf( stdout, "\n\n\t--> Translate figure %s\n", InputFileName );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Run BEH Compiler\n" );
    fprintf( stdout, "\t--> Compile file %s\n", InputFileName );
  }

  B2fBehFigure = vhdlloadbefig( (befig_list *)0, InputFileName, 2 );
  B2fFsmFigure = B2fBeh2Fsm( B2fBehFigure, FlagInitial, StringInitial );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Drive FSM file %s\n", OutputFileName );
  }

  B2fFsmFigure->NAME = namealloc( OutputFileName );
  savefsmfig( B2fFsmFigure );

  fprintf( stdout, "\t<-- done\n\n" );

  return( 0 );
}
