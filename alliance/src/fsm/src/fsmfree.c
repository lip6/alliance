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
| File    :                  fsmfree.c                        |
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
# include "fsmfree.h"
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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Fsm Free Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Fsm Free Figure                        |
|                                                             |
\------------------------------------------------------------*/
 
void freefsmfig( Figure )

  fsmfig_list *Figure;
{
  autfreeblock( (char *)Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Free State                       |
|                                                             |
\------------------------------------------------------------*/

void freefsmstate( State )

  fsmstate_list *State;
{
  autfreeheap( (char *)State, sizeof( fsmstate_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Free Stack                       |
|                                                             |
\------------------------------------------------------------*/

void freefsmstack( Stack )

  fsmstack_list *Stack;
{
  autfreeheap( (char *)Stack, sizeof( fsmstack_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Free Trans                       |
|                                                             |
\------------------------------------------------------------*/

void freefsmtrans( Trans )

  fsmtrans_list *Trans;
{
  autfreeheap( (char *)Trans, sizeof( fsmtrans_list ));
}


/*------------------------------------------------------------\
|                                                             |
|                    Fsm Free Local Output                    |
|                                                             |
\------------------------------------------------------------*/

void freefsmlocout( Locout )

  fsmlocout_list *Locout;
{
  autfreeheap( (char *)Locout, sizeof( fsmlocout_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Free Output                      |
|                                                             |
\------------------------------------------------------------*/

void freefsmout( Output )

  fsmout_list *Output;
{
  autfreeblock( (char *)Output );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Free Input                       |
|                                                             |
\------------------------------------------------------------*/

void freefsmin( Input )

  fsmin_list *Input;
{
  autfreeblock( (char *)Input  );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Free Pragma                      |
|                                                             |
\------------------------------------------------------------*/

void freefsmpragma( Pragma )

  fsmpragma_list *Pragma;
{
  autfreeblock( (char *)Pragma  );
}

/*------------------------------------------------------------\
|                                                             |
|                        Fsm Free Port                        |
|                                                             |
\------------------------------------------------------------*/

void freefsmport( Port )

  fsmport_list *Port;
{
  autfreeblock( (char *)Port );
}
