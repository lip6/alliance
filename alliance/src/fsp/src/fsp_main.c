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
| Tool    :                     FSP                           |
|                                                             |
| File    :                  fsp_main.c                       |
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
# include "fsp_debug.h"
# include "fsp_comp.h"
# include "fsp_proof.h"
# include "fsp_main.h"

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

   fsmfig_list *FspFsmFigure1 = (fsmfig_list    *)0;
   fsmfig_list *FspFsmFigure2 = (fsmfig_list    *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Fsp Usage                          |
|                                                             |
\------------------------------------------------------------*/

void FspUsage()
{
  fprintf( stderr, "\t\tfsp [Options] format1 format2 filename1 filename2\n\n" );

  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n"       );
  fprintf( stdout, "\t\t          -D Sets Debug mode on\n"         );
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
  char  *InputFileName1;
  char  *InputFileName2;
  char  *InputFormat1;
  char  *InputFormat2;
  int    Number;
  int    Index;
  char   Option;

  int    FlagVerbose = 0;
  int    FlagDebug   = 0;

  alliancebanner_with_authors( "FSP", VERSION, "FSM formal Proof", "1999",
      ALLIANCE_VERSION, "Ludovic Jacomme" );

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  fsmenv();

  if ( argc < 4 ) FspUsage();

  InputFileName1 = (char *)0;
  InputFileName2 = (char *)0;
  InputFormat1   = (char *)0;
  InputFormat2   = (char *)0;

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
          case 'D' : FlagDebug = 1;
          break;
          default  : FspUsage();
        }

        if ( Option == 'r' ) break;
      }
    }
    else
    if ( InputFormat1   == (char *)0 ) InputFormat1   = argv[ Number ];
    else
    if ( InputFormat2   == (char *)0 ) InputFormat2   = argv[ Number ];
    else
    if ( InputFileName1 == (char *)0 ) InputFileName1 = argv[ Number ];
    else
    if ( InputFileName2 == (char *)0 ) InputFileName2 = argv[ Number ];
    else
    FspUsage();
  }

  if ( ( InputFormat1   == (char *)0 ) ||
       ( InputFormat2   == (char *)0 ) ||
       ( InputFileName1 == (char *)0 ) ||
       ( InputFileName2 == (char *)0 ) ) FspUsage();

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Run FSM Compiler\n" );
    fprintf( stdout, "\t--> Compile file %s\n", InputFileName1 );
  }

  FspFsmFigure1 = FspCompile( InputFileName1, InputFormat1 );

  if ( ( IsFsmFigMulti( FspFsmFigure1    ) ) ||
       ( IsFsmFigMixedRtl( FspFsmFigure1 ) ) )
  {
    fprintf( stderr, "\tMulti FSM or Mixed RTL FSM not supported !\n" );
    autexit( 1 );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Run FSM Compiler\n" );
    fprintf( stdout, "\t--> Compile file %s\n", InputFileName2 );
  }

  FspFsmFigure2 = FspCompile( InputFileName2, InputFormat2 );

  if ( ( IsFsmFigMulti( FspFsmFigure2    ) ) ||
       ( IsFsmFigMixedRtl( FspFsmFigure2 ) ) )
  {
    fprintf( stderr, "\tMulti FSM or Mixed RTL FSM not supported !\n" );
    autexit( 1 );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t--> Formal proof between \"%s\" and \"%s\"\n",
             FspFsmFigure1->NAME, FspFsmFigure2->NAME );
  }

  if ( FspFormalProof( FspFsmFigure1, FspFsmFigure2, FlagDebug ) )
  {
    fprintf( stdout, "\n\t==> \"%s\" and \"%s\" are identicals\n\n", 
             FspFsmFigure1->NAME, FspFsmFigure2->NAME );
  }
  else
  {
    fprintf( stdout, "\n\t==> \"%s\" and \"%s\" are not identicals\n\n", 
             FspFsmFigure1->NAME, FspFsmFigure2->NAME );

    exit( 1 );
  }

  return( 0 );
}
