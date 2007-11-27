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
| Tool    :                   XGRA                            |
|                                                             |
| File    :                   Gra.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XGR.h"
# include "XGR_graph.h"

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

  autgraph  *XgraFigureGra = (autgraph  *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                       For Set Long Jump                     |
|                                                             |
\------------------------------------------------------------*/

  static jmp_buf XgraJumpBuffer;
  static void    (*OldExitHandler)() = NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xgra Exit Handler Functions                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XgraExitHandler                        |
|                                                             |
\------------------------------------------------------------*/

void XgraExitHandler()
{
  XgraFlushErrorMessage();

  signal( SIGTERM, OldExitHandler );
  longjmp( XgraJumpBuffer, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraActiveExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XgraActiveExitHandler()
{
  OldExitHandler = signal( SIGTERM, XgraExitHandler );
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraResetExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XgraResetExitHandler()
{
  signal( SIGTERM, OldExitHandler );
}

/*------------------------------------------------------------\
|                                                             |
|                          Get Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Xgragetgrafig                      |
|                                                             |
\------------------------------------------------------------*/

autgraph *Xgragetgrafig( FileName )

  char *FileName;
{
  autgraph *FigureGra;

  autbegin();

  FigureGra = (autgraph *)NULL;

  XgraActiveExitHandler();

  if ( setjmp( XgraJumpBuffer ) == 0 )
  {
    FigureGra = loadautgraph( FileName );

    XgraResetExitHandler();

    autend();
    return( FigureGra );
  }

  if ( FigureGra != (autgraph *)NULL )
  {
    destroyautgraph( FigureGra );
  }

  autend();
  return( (autgraph *)NULL );
}
