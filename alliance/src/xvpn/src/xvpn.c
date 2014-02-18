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
| Tool    :                   XVPN                            |
|                                                             |
| File    :                  xvpn.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ident "@(#)XVPN ver 1.01 September 1996"

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
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "XSB.h"
# include "XVP.h"
# include "XMX.h"
# include "XTB.h"
# include "XMF.h"

# include "xvpn.h"

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

void XvpnPresent()
{
  alliancebanner_with_authors( "Xvpn", VERSION,
                  "Graphic Petri Nets Viewer", "1996",
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
 
  XvpnPresent();

  mbkenv();
  autenv();
  vexenv();
  vpnenv();
  Xvpngetenv();

  for ( Index = 1;
        Index < argc;
        Index++ )
  {
    if ( ! strcmp( argv[ Index ], "-l" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index             = Index + 1;
        XvpnFirstFileName = argv[ Index ];
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
      XVPN_XOR_CURSOR = XVPN_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-force" ) )
    {
      XVPN_FORCE_DISPLAY = XVPN_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-install" ) )
    {
      XVPN_SWITCH_COLOR_MAP = XVPN_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-I" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index  = Index + 1;
        VPN_IN = namealloc( argv[ Index ] );
      }
    }
    else
    if ( ! strcmp( argv[ Index ], "-elaborate" ) )
    {
      XvpnElaborate = 1;
    }
    else
    if ( ! strcmp( argv[ Index ], "-bdd" ) )
    {
      XvpnBdd = 1;
    }
    else
    if ( ! strcmp( argv[ Index ], "-bound" ) )
    {
      XvpnBound = 1;
    }
    else
    if ( ! strcmp( argv[ Index ], "-simp" ) )
    {
      XvpnSimp = 1;
    }
    else
    if ( ! strcmp( argv[ Index ], "-noreduce" ) )
    {
      XvpnReduce = 0;
    }
    else
    if ( ! strcmp( argv[ Index ], "-reduce_after" ) )
    {
      XvpnReduceAfter = 1;
    }
    else
    if ( ! strcmp( argv[ Index ], "-slide" ) )
    {
      for ( Index++; Index < argc; Index++ )
      {
        XvpnFileAddSlide( argv[ Index ] );
      }
    }
  }

  if ( Debug ) autdebug();

  autbegin();

  XvpnLoadParameters();

  XvpnInitializeRessources( &argc, argv );
  XvpnInitializeErrorMessage( Debug );

  XtAppMainLoop( XvpnApplication ); 

  autend();

  return( 0 );
}
