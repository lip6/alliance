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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  dreal.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

# ident "@(#)DREAL ver 1.3 October 1994"

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
# include "GSB.h"
# include "GRD.h"
# include "GMX.h"

# include "dreal.h"

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

void DrealPresent()
{
  alliancebanner_with_authors( "Dreal", VERSION,
                  "Design  Real layout", "1999",
                  ALLIANCE_VERSION, "Ludovic Jacomme" );
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
 
  DrealPresent();

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
    if ( ! strcmp( argv[ Index ], "-xor" ) )
    {
      DREAL_XOR_CURSOR = DREAL_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-force" ) )
    {
      DREAL_FORCE_DISPLAY = DREAL_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-install" ) )
    {
      DREAL_SWITCH_COLOR_MAP = DREAL_TRUE;
    }
  }

  if ( Debug ) rdsdebug();

  rdsbegin();

  Drealenv();
  loadrdsparam();
  DrealLoadParameters();

  DrealInitializeRessources( &argc, argv );
  DrealInitializeWindow();

  if ( WindowInfo )
  {
    fprintf( stdout, "\nDreal window\n" );
    fprintf( stdout, "\n  XMIN : %d"  , DrealWindowXmin / RDS_PHYSICAL_GRID );
    fprintf( stdout, "\n  XMAX : %d"  , DrealWindowXmax / RDS_PHYSICAL_GRID );
    fprintf( stdout, "\n  YMIN : %d"  , DrealWindowYmin / RDS_PHYSICAL_GRID );
    fprintf( stdout, "\n  YMAX : %d"  , DrealWindowYmax / RDS_PHYSICAL_GRID );
    fprintf( stdout, "\n  SIDE : %d"  , DrealWindowSide / RDS_PHYSICAL_GRID );
    fprintf( stdout, "\n  DX   : %d"  , DrealWindowDx                );
    fprintf( stdout, "\n  DY   : %d"  , DrealWindowDy                );
    fprintf( stdout, "\n  SIZE : %d\n", DrealWindowSize              );
  }

  DrealInitializeErrorMessage( Debug );

  if ( FileName != (char *)NULL )
  {
    DrealAddFigure( FileName );
  }

  XtAppMainLoop( DrealApplication ); 

  rdsend();

  return( 0 );
}
