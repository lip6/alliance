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
| Tool    :                   XFSM                            |
|                                                             |
| File    :                  xfsm.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ident "@(#)XFSM ver 1.01 January 1995"

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
# include "fsm.h"
# include "ftl.h"
# include "XSB.h"
# include "XFS.h"
# include "XMX.h"

# include "xfsm.h"

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

void XfsmPresent()
{
  alliancebanner_with_authors( "Xfsm", VERSION,
                  "Graphic FSM Viewer", "1995",
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
  char  Debug = 0;
  int   Index;
 
  XfsmPresent();

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  fsmenv();

  for ( Index = 1;
        Index < argc;
        Index++ )
  {
    if ( ! strcmp( argv[ Index ], "-l" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index             = Index + 1;
        XfsmFirstFileName = argv[ Index ];
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
      XFSM_XOR_CURSOR = XFSM_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-force" ) )
    {
      XFSM_FORCE_DISPLAY = XFSM_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-install" ) )
    {
      XFSM_SWITCH_COLOR_MAP = XFSM_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-I" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index  = Index + 1;
        FSM_IN = namealloc( argv[ Index ] );
      }
    }
  }

  if ( Debug ) autdebug();

  autbegin();

  Xfsmgetenv();
  XfsmLoadParameters();

  XfsmInitializeRessources( &argc, argv );
  XfsmInitializeErrorMessage( Debug );

  XtAppMainLoop( XfsmApplication ); 

  autend();

  return( 0 );
}
