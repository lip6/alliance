/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| Tool    :                     Fsm                           |
|                                                             |
| File    :                  fsmalloc.c                       |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
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

# include <stdio.h>
# include <string.h>
# include "fsmalloc.h"
# include "fsmerror.h"

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

  fsmfig_list *HEAD_FSMFIG = (fsmfig_list *)0;

  char *FSM_CTRL_NAME [ FSM_MAX_CTRL ] =
  {
    "NOP", "PUSH", "POP"
  };

  char *FSM_TYPE_NAME [ FSM_MAX_TYPE ] =
  {
    "NONE",
    "SEVERITY",
    "BOOLEAN",
    "BIT",
    "INTEGER",
    "MUX_BIT",
    "WOR_BIT",
    "REG_BIT",
    "NATURAL",
    "BIT_VEC",
    "MUX_VEC",
    "WOR_VEC",
    "REG_VEC"
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Fsm Alloc Figure                     |
|                                                             |
\------------------------------------------------------------*/
 
fsmfig_list *allocfsmfig()
{
  return( (fsmfig_list *)(autallocblock( sizeof( fsmfig_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Fsm Alloc State                     |
|                                                             |
\------------------------------------------------------------*/

fsmstate_list *allocfsmstate()
{
  return( (fsmstate_list *)(autallocheap( sizeof( fsmstate_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Fsm Alloc Stack                     |
|                                                             |
\------------------------------------------------------------*/

fsmstack_list *allocfsmstack()
{
  return( (fsmstack_list *)(autallocheap( sizeof( fsmstack_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Fsm Alloc Transition                   |
|                                                             |
\------------------------------------------------------------*/

fsmtrans_list *allocfsmtrans()
{
  return( (fsmtrans_list *)(autallocheap( sizeof( fsmtrans_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Fsm Alloc Local Output                  |
|                                                             |
\------------------------------------------------------------*/

fsmlocout_list *allocfsmlocout()
{
  return( (fsmlocout_list *)(autallocheap( sizeof( fsmlocout_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Alloc Pragma                     |
|                                                             |
\------------------------------------------------------------*/

fsmpragma_list *allocfsmpragma()
{
  return( (fsmpragma_list *)(autallocblock( sizeof( fsmpragma_list ) ) ) );
}


/*------------------------------------------------------------\
|                                                             |
|                        Fsm Alloc Input                      |
|                                                             |
\------------------------------------------------------------*/

fsmin_list *allocfsmin()
{
  return( (fsmin_list *)(autallocblock( sizeof( fsmin_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Alloc Output                     |
|                                                             |
\------------------------------------------------------------*/

fsmout_list *allocfsmout()
{
  return( (fsmout_list *)(autallocblock( sizeof( fsmout_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Alloc Port                       |
|                                                             |
\------------------------------------------------------------*/

fsmport_list *allocfsmport()
{
  return( (fsmport_list *)(autallocblock( sizeof( fsmport_list ) ) ) );
}
