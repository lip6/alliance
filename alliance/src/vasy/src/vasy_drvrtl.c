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
| Tool    :                     VASY                           |
|                                                             |
| File    :                 vasy_drvrtl.c                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
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
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "rtn.h"
# include "rtd.h"

# include "vasy_debug.h"
# include "vasy_error.h"
# include "vasy_simprtl.h"
# include "vasy_shared.h"
# include "vasy_drvrtl.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveRtlCheck                         |
|                                                             |
\------------------------------------------------------------*/

static void VasyDriveRtlCheck( RtlFigure )

  rtlfig_list *RtlFigure;
{
}

/*------------------------------------------------------------\
|                                                             |
|                   VasyDriveRtlRtlFig                        |
|                                                             |
\------------------------------------------------------------*/

void VasyDriveRtlRtlFig( RtlFigure, FileName )

  rtlfig_list  *RtlFigure;
  char         *FileName;
{
  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "--> VasyDriveRtlRtlFig %s\n", FileName );
  }

  RtlFigure->NAME = namealloc( FileName );

  VasyDriveRtlCheck( RtlFigure );
  savertlfig( RtlFigure );

  if ( IsVasyDebugLevel1() )
  {
    viewrtlfig( RtlFigure );
  }

  if ( IsVasyDebugLevel0() )
  {
    VasyPrintf( stdout, "<-- VasyDriveRtlRtlFig %s\n\n", FileName );
  }
}
