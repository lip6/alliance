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
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_shared.c                   |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   07.08.02                        |
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
# include "btr.h"
# include "fsm.h"
# include "ftl.h"
# include "vex.h"
# include "ctl.h"
# include "ctp.h"
# include "abe.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mocha_debug.h"
# include "mocha_shared.h"

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

  mochafig_list *HEAD_MOCHA_FIG = (mochafig_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         MochaAddState                       |
|                                                             |
\------------------------------------------------------------*/

mochastate_list *MochaAddState( MochaFigure, MochaFsm, Name, Code )

  mochafig_list *MochaFigure;
  mochafsm_list *MochaFsm;
  char          *Name;
  long           Code;
{
  mochastate_list *MochaState;

  MochaState = (mochastate_list *)autallocblock( sizeof( mochastate_list ) );

  MochaState->NAME     = namealloc( Name );
  MochaState->CODE     = Code;
  MochaState->NEXT     = MochaFsm->STATE;
  MochaFsm->STATE      = MochaState;

  addauthelem( MochaFsm->HASH_STATE, Name, (long)MochaState );

  return( MochaState );
}

/*------------------------------------------------------------\
|                                                             |
|                         MochaAddFsm                         |
|                                                             |
\------------------------------------------------------------*/

mochafsm_list *MochaAddFsm( MochaFigure, Name )

  mochafig_list *MochaFigure;
  char          *Name;
{
  mochafsm_list *MochaFsm;

  MochaFsm = (mochafsm_list *)autallocblock( sizeof( mochafsm_list ) );

  MochaFsm->NAME       = namealloc( Name );
  MochaFsm->HASH_STATE = createauthtable( 100 );
  MochaFsm->NEXT       = MochaFigure->FSM;
  MochaFigure->FSM     = MochaFsm;

  return( MochaFsm );
}

/*------------------------------------------------------------\
|                                                             |
|                         MochaAddFigure                      |
|                                                             |
\------------------------------------------------------------*/
 
mochafig_list *MochaAddFigure( Name )
 
  char *Name;
{
  mochafig_list *MochaFigure;
 
  MochaFigure = (mochafig_list *)autallocblock( sizeof( mochafig_list ) );

  MochaFigure->NAME = namealloc( Name );
  MochaFigure->NEXT = HEAD_MOCHA_FIG;
  HEAD_MOCHA_FIG    = MochaFigure;
 
  return( MochaFigure );
}
