/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                     Sch                           |
|                                                             |
| File    :                    main.c                         |
|                                                             |
| Date    :                   08.02.95                        |
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
# include "mlo.h"
# include "scl.h"

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

void SchUsage()
{
  fprintf( stderr, "\t\tschtest [Options] Input_name [Output_name]\n\n" );
  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n" );
  fprintf( stdout, "\t\t          -B Behavioral mode on\n" );
  fprintf( stdout, "\t\t          -v Vectorized mode on\n" );
  fprintf( stdout, "\t\t          -P to place and route\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}


/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

  int   argc;
  char *argv[];
{
  schfig_list *SchFigure;
  befig_list  *BeFigure;
  lofig_list  *LoFigure;
  char        *InputFileName;
  int          Number;
  int          Index;
  char         Option;

  int   FlagVerbose  = 0;
  int   FlagVector   = 0;
  int   FlagBeh      = 0;
  int   FlagPlace    = 0;
  
  mbkenv();
  autenv();
  schenv();

  InputFileName = (char *)0;

  if ( argc < 2 ) SchUsage();

  InputFileName = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        switch ( Option )
        {
          case 'V' : FlagVerbose  = 1;
          break;
          case 'v' : FlagVector   = 1;
          break;
          case 'P' : FlagPlace    = 1;
          break;
          case 'B' : FlagBeh      = 1;
          break;
          default  : SchUsage();
        }
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    SchUsage();
  }

  if ( InputFileName  == (char *)0 ) SchUsage();

  if ( FlagBeh )
  {
    BeFigure  = vhdlloadbefig( (befig_list *)0, InputFileName, 3 );
    SchFigure = befig2schfig( BeFigure );
  }
  else
  {
    LoFigure  = getlofig( InputFileName, 'A' );
    SchFigure = lofig2schfig( LoFigure, FlagVector );
  }

  if ( FlagPlace )
  {
    placerouteschfig( SchFigure );
  }

  if ( FlagVerbose )
  {
    viewschfig( SchFigure );
  }

  delschfig( SchFigure->NAME );

  return( 0 );
}


