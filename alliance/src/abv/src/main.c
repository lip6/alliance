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
# include <stdio.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "abe.h"
# include "abv.h"


/*------------------------------------------------------------\
|                                                             |
|                            Usage                            |
|                                                             |
\------------------------------------------------------------*/

void BvlUsage()
{
  fprintf( stderr, "\t\tabvtest [Options] Input_name\n\n" );
  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

  int    argc;
  char **argv;
{
  befig_list *Figure;
  char       *InputFileName;
  int         Number;
  int         Index;
  char        Option;
  long        Value;

  int FlagVerbose = 0;

  mbkenv();
  autenv();
  ablenv();

  InputFileName = (char *)0;

  if ( argc < 2 ) BvlUsage();

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
          case 'V' : FlagVerbose = 1;
          break;
          default  : BvlUsage();
        }
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    BvlUsage();
  }

  if ( InputFileName == (char *)0 ) BvlUsage();

  Figure = vhdlloadbefig( NULL, InputFileName, 0);

  beh_frebefig( Figure );
}
