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
| Tool    :                   XFSM                            |
|                                                             |
| File    :                   Fsm.c                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"
# include "XSB.h" 
# include "XFS.h"
# include "XFS_fsm.h"

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

  fsmfig_list  *XfsmFigureFsm = (fsmfig_list  *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                       For Set Long Jump                     |
|                                                             |
\------------------------------------------------------------*/

  static jmp_buf XfsmJumpBuffer;
  static void    (*OldExitHandler)() = NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xfsm Exit Handler Functions                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XfsmExitHandler                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmExitHandler()
{
  XfsmFlushErrorMessage();

  longjmp( XfsmJumpBuffer, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmActiveExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmActiveExitHandler()
{
  MBK_EXIT_FUNCTION = XfsmExitHandler;
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmResetExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmResetExitHandler()
{
  MBK_EXIT_FUNCTION = NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                          Get Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Xfsmgetfsmfig                      |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *Xfsmgetfsmfig( FileName )

  char *FileName;
{
  fsmfig_list *FigureFsm;

  autbegin();

  FigureFsm = (fsmfig_list *)NULL;

  XfsmActiveExitHandler();

  if ( setjmp( XfsmJumpBuffer ) == 0 )
  {
    FigureFsm = getfsmfig( FileName );

    XfsmResetExitHandler();

    autend();
    return( FigureFsm );
  }

  XfsmResetExitHandler();

  if ( FigureFsm != (fsmfig_list *)NULL )
  {
    delfsmfig( FigureFsm->NAME );
  }

  autend();
  return( (fsmfig_list *)NULL );
}
