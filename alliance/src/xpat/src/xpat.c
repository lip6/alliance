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
| Tool    :                   XPAT                            |
|                                                             |
| File    :                  xpat.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ident "@(#)XPAT ver 1.01 January 1996"

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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
# include "XMX.h"

# include "xpat.h"

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

void XpatPresent()
{
  alliancebanner_with_contrib( "Xpat", VERSION,
                  "Graphic Pattern Viewer", "1999",
                  ALLIANCE_VERSION, "Ludovic Jacomme", "Julien Dunoyer" );
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
 
  XpatPresent();

  mbkenv();
  autenv();

  for ( Index = 1;
        Index < argc;
        Index++ )
  {
    if ( ! strcmp( argv[ Index ], "-l" ) )
    {
      if ( ( Index + 1 ) < argc )
      {
        Index             = Index + 1;
        XpatFirstFileName = argv[ Index ];
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
      XPAT_XOR_CURSOR = XPAT_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-force" ) )
    {
      XPAT_FORCE_DISPLAY = XPAT_TRUE;
    }
    else
    if ( ! strcmp( argv[ Index ], "-install" ) )
    {
      XPAT_SWITCH_COLOR_MAP = XPAT_TRUE;
    }
  }

  if ( Debug ) autdebug();

  autbegin();

  Xpatgetenv();
  XpatLoadParameters();

  XpatInitializeRessources( &argc, argv );
  XpatInitializeErrorMessage( Debug );

  XtAppMainLoop( XpatApplication ); 

  autend();

  return( 0 );
}
