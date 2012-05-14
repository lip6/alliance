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
| Tool    :                     FSP                           |
|                                                             |
| File    :                  fsp_comp.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "fsp_debug.h"
# include "fsp_comp.h"

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

  static char FspBuffer[ 1024 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         FspRenameState                      |
|                                                             |
\------------------------------------------------------------*/

static void FspRenameState( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list *ScanState;

  ScanState = FsmFigure->STATE;

  while ( ScanState != (fsmstate_list *)0 )
  {
    sprintf( FspBuffer, "%s.%s", FsmFigure->NAME, ScanState->NAME );
    ScanState->NAME = namealloc( FspBuffer );

    ScanState = ScanState->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         FspCompile                          |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *FspCompile( FileName, FileFormat ) 

  char *FileName;
  char *FileFormat;
{
  fsmfig_list *FsmFigure;
  char        *SaveFormat;

  SaveFormat = FSM_IN;
  FSM_IN     = namealloc( FileFormat );
  FsmFigure  = getfsmfig( FileName );

  if ( IsFsmFigMulti( FsmFigure ) )
  {
    fprintf( stderr, "\t\tMulti FSM not supported !\n" );
    autexit( 1 );
  }

  FSM_IN     = SaveFormat;

  sprintf( FspBuffer, "%s.%s", FileName, FileFormat );
  FsmFigure->NAME = namealloc( FspBuffer );

  FspRenameState( FsmFigure );

  return( FsmFigure );
}
