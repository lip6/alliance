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
| Tool    :                     RDS                           |
|                                                             |
| File    :                   FlatRds.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   01.09.93                        |
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
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rtl.h"
# include "rfm.h"
# include "rpr.h"

/*------------------------------------------------------------\
|                                                             |
|                            Banner                           |
|                                                             |
\------------------------------------------------------------*/

void Banner()
{
  alliancebanner_with_authors( "FlatRds", "1.02", 
                  "Flatten RDS", "1994", 
                  ALLIANCE_VERSION, "L.Jacomme, F.Petrot, F.Wajsburt" );
}

/*------------------------------------------------------------\
|                                                             |
|                            Usage                            |
|                                                             |
\------------------------------------------------------------*/

void Usage()
{
  fprintf( stderr, "\n\tUsage : flatrds physical_figure instance output_name" );
  fprintf( stderr, "\n\t        flatten the instance in the physical_figure" );
  fprintf( stderr, "\n\tUsage : flatrds -r physical_figure output_name" );
  fprintf( stderr, "\n\t        flatten physical_figure to the catalog" );
  fprintf( stderr, "\n\tUsage : flatrds -t physical_figure output_name" );
  fprintf( stderr, "\n\t        flatten physical_figure to the transistor\n\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                             Main                            |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

   int   argc;
   char *argv[];
{
  rdsfig_list *Figure;

  Banner();

  mbkenv();
  rdsenv();
  loadrdsparam();

  if ( argc < 4 ) Usage();

  if ( ! strcmp( "-r", argv[1] ) ) 
  {
     Figure = getrdsfig( argv[2], 'A', 0 );

     fprintf( stdout, "\n   --> Flatten figure %s to the catalog\n",
              Figure->NAME );

     rflattenrdsfig( Figure, RDS_YES, RDS_YES );
  }
  else
  if ( ! strcmp( "-t", argv[1] ) )
  {
     Figure = getrdsfig( argv[2], 'A', 0 );

     fprintf( stdout, "\n   --> Flatten figure %s to the transistor\n",
              Figure->NAME );

     rflattenrdsfig( Figure, RDS_YES, RDS_NO );
  }
  else
  {
    Figure = getrdsfig( argv[1], 'A', 0 );
   
    fprintf( stdout, "\n   --> Flatten the instance %s in the figure %s\n", 
             argv[2], Figure->NAME );

    flattenrdsfig( Figure, argv[2], RDS_YES );
  }

  Figure->NAME = namealloc( argv[3] );

  fprintf( stdout, "   --> Saving figure %s \n\n", Figure->NAME );

  saverdsfig( Figure );

  return( 0 );
}
