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
| Tool    :                     K2F                           |
|                                                             |
| File    :                  k2f_main.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "k2f_error.h"
# include "k2f_main.h"

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

   fsmfig_list *K2fFsmFigure = (fsmfig_list    *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          K2f Usage                          |
|                                                             |
\------------------------------------------------------------*/

void K2fUsage()
{
  fprintf( stderr, "\t\tk2f In_format Out_format Input_name [Output_name]\n\n" );
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
  char  *InputFormat;
  char  *OutputFormat;
  int    Number;

  alliancebanner_with_authors( "K2F", VERSION,
      "Translator Kiss <-> FSM Format", "1996", ALLIANCE_VERSION,
      "Ludovic Jacomme" );

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  fsmenv();

  if ( argc < 4 ) K2fUsage();

  InputFileName  = (char *)0;
  OutputFileName = (char *)0;
  InputFormat    = (char *)0;
  OutputFormat   = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( InputFormat    == (char *)0 ) InputFormat   = argv[ Number ];
    else
    if ( OutputFormat   == (char *)0 ) OutputFormat  = argv[ Number ];
    else
    if ( InputFileName  == (char *)0 ) InputFileName = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    K2fUsage();
  }

  if ( ( InputFileName  == (char *)0 ) ||
       ( InputFormat    == (char *)0 ) ||
       ( OutputFormat   == (char *)0 ) ) K2fUsage();

  if ( OutputFileName == (char *)0 ) OutputFileName = InputFileName;

  FSM_IN  = namealloc( InputFormat  );
  FSM_OUT = namealloc( OutputFormat );

  fprintf( stdout, "\n\n\t--> Translate figure %s\n", InputFileName );

  K2fFsmFigure = getfsmfig( InputFileName );

  K2fFsmFigure->NAME = namealloc( OutputFileName );
  savefsmfig( K2fFsmFigure );

  fprintf( stdout, "\t<-- done\n\n" );

  return( 0 );
}
