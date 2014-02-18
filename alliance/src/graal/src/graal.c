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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  graal.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

# ident "@(#)GRAAL ver 1.10 January 1995"

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>
# include <Xm/Xm.h>
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"
# include "rtl.h"
# include "GSB.h"
# include "GRM.h"
# include "GMX.h"
# include "GRM_window.h"
# include "GRM_error.h"

# include "graal.h"

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
|                           present                           |
|                                                             |
\------------------------------------------------------------*/

void GraalPresent()
{
  alliancebanner_with_authors( "Graal", VERSION,
                  "Graphic layout editor", "1999",
                  ALLIANCE_VERSION,
                  "Ludovic Jacomme and Frederic Venot" );
}

/*------------------------------------------------------------\
|                                                             |
|                            main                             |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

    int   argc;
    char *argv[];
{
  char *FileName   = (char *)NULL;
  char  WindowInfo = 0;
  char  Debug      = 0;
  int   Index;
 
  GraalPresent();

  mbkenv();
  rdsenv();

  for ( Index = 1;
        Index < argc;
        Index++ )
  {
    if ( ! strcmp( argv[ Index ], "-l" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index    = Index + 1;
        FileName = argv[ Index ];
      }
    }
    else
    if ( ! strcmp( argv[ Index ], "-w" ) )
    {
      WindowInfo = 1;
    }
    else 
    if ( ! strcmp( argv[ Index ], "-debug" ) )
    {
      Debug = 1;
    }
    else
    if ( ! strcmp( argv[ Index ], "-scale" ) )
    {
      Index++;

      if ( Index < argc )
      {
        GRAAL_SCALE = atol( argv[ Index ] );
        if ( GRAAL_SCALE <= 0 ) GRAAL_SCALE = 1;
      }
    }
    else
    if ( ! strcmp( argv[ Index ], "-xor" ) )
    {
      GRAAL_XOR_CURSOR = GRAAL_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-force" ) )
    {
      GRAAL_FORCE_DISPLAY = GRAAL_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-install" ) )
    {
      GRAAL_SWITCH_COLOR_MAP = GRAAL_TRUE;
    }
  }

  if ( Debug ) rdsdebug();

  rdsbegin();

  Graalenv();
  loadrdsparam();
  GraalLoadParameters();

  GraalInitializeRessources( &argc, argv );
  GraalInitializeWindow();

  if ( WindowInfo )
  {
    fprintf( stdout, "\nGraal window\n" );
    fprintf( stdout, "\n  XMIN : %ld"  , GraalWindowXmin / GRAAL_RDS_LAMBDA );
    fprintf( stdout, "\n  XMAX : %ld"  , GraalWindowXmax / GRAAL_RDS_LAMBDA );
    fprintf( stdout, "\n  YMIN : %ld"  , GraalWindowYmin / GRAAL_RDS_LAMBDA );
    fprintf( stdout, "\n  YMAX : %ld"  , GraalWindowYmax / GRAAL_RDS_LAMBDA );
    fprintf( stdout, "\n  SIDE : %ld"  , GraalWindowSide / GRAAL_RDS_LAMBDA );
    fprintf( stdout, "\n  DX   : %ld"  , GraalWindowDx                );
    fprintf( stdout, "\n  DY   : %ld"  , GraalWindowDy                );
    fprintf( stdout, "\n  SIZE : %ld\n", GraalWindowSize              );
  }

  GraalInitializeCutCxTable();
  GraalInitializeErrorMessage( Debug );

  if ( FileName != (char *)NULL )
  {
    GraalFigureMbk = Graalgetphfig( FileName, 'A' );
  }

  XtAppMainLoop( GraalApplication ); 

  rdsend();

  return( 0 );
}
