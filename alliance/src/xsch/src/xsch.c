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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                  xsch.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>
# include <Xm/Xm.h>
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
# include "XMX.h"

# include "xsch.h"

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

void XschPresent()
{
  alliancebanner_with_contrib( "Xsch", VERSION,
                  "Graphical Schematic Viewer", "2000",
                  ALLIANCE_VERSION, "Ludovic Jacomme",
                  "B.Miramond, F.Petrot, S.Picault" );
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
  int  Debug   = 0;
  
  int  Index;
 
  XschPresent();

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  schenv();

  for ( Index = 1;
        Index < argc;
        Index++ )
  {
    if ( ! strcmp( argv[ Index ], "-l" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index             = Index + 1;
        XschFirstFileName = argv[ Index ];
      }
    }
    else
    if ( ! strcmp( argv[ Index ], "-debug" ) )
    {
      Debug = 1;
    }
    else
    if ( ! strcmp( argv[ Index ], "-xor" ) )
    {
      XSCH_XOR_CURSOR = XSCH_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-force" ) )
    {
      XSCH_FORCE_DISPLAY = XSCH_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-install" ) )
    {
      XSCH_SWITCH_COLOR_MAP = XSCH_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-v" ) )
    {
      XSCH_VECTORIZED_MODE = XSCH_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-I" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index  = Index + 1;
        SCH_IN = namealloc( argv[ Index ] );

        if ( strlen( SCH_IN ) <= 3 )
        {
          strcpy( IN_LO, SCH_IN );
        }
      }
    }
    else
    if ( ! strcmp( argv[ Index ], "-slide" ) )
    {
      for ( Index++; Index < argc; Index++ )
      {
        XschFileAddSlide( argv[ Index ] );
      }
    }
  }

  if ( Debug ) autdebug();

  autbegin();

  Xschgetenv();
  XschLoadParameters();

  XschInitializeRessources( &argc, argv );
  XschInitializeErrorMessage( Debug );

  XtAppMainLoop( XschApplication ); 

  autend();

  return( 0 );
}
