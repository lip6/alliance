/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Rtn                           |
|                                                             |
| File    :                  rtnfree.c                        |
|                                                             |
| Date    :                   04.07.96                        |
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
# include "vex.h"
# include "rtn.h"

# include <stdio.h>
# include <string.h>
# include "rtnfree.h"
# include "rtnalloc.h"
# include "rtnerror.h"

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
|                      Rtl Free Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Rtl Free Figure                        |
|                                                             |
\------------------------------------------------------------*/
 
void freertlfig( Figure )

  rtlfig_list *Figure;
{
  autfreeblock( (char *)Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Assign                      |
|                                                             |
\------------------------------------------------------------*/

void freertlasg( Asg )

  rtlasg_list *Asg;
{
  autfreeheap( (char *)Asg, sizeof( rtlasg_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Register                    |
|                                                             |
\------------------------------------------------------------*/

void freertlbivex( BiVex )

  rtlbivex_list *BiVex;
{
  autfreeheap( (char *)BiVex, sizeof( rtlbivex_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Declaration                 |
|                                                             |
\------------------------------------------------------------*/

void freertldecl( Decl )

  rtldecl_list *Decl;
{
  autfreeheap( (char *)Decl, sizeof( rtldecl_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Fsm Assign                  |
|                                                             |
\------------------------------------------------------------*/

void freertlfsmasg( FsmAsg )

  rtlfsmasg_list *FsmAsg;
{
  autfreeheap( (char *)FsmAsg, sizeof( rtlfsmasg_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Fsm State                   |
|                                                             |
\------------------------------------------------------------*/

void freertlfsmstate( fsmstate )

  rtlfsmstate_list *fsmstate;
{
  autfreeheap( (char *)fsmstate, sizeof( rtlfsmstate_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Fsm Trans                   |
|                                                             |
\------------------------------------------------------------*/

void freertlfsmtrans( fsmtrans )

  rtlfsmtrans_list *fsmtrans;
{
  autfreeheap( (char *)fsmtrans, sizeof( rtlfsmtrans_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                      Rtl Free Fsm                           |
|                                                             |
\------------------------------------------------------------*/
 
void freertlfsm( Fsm )

  rtlfsm_list *Fsm;
{
  autfreeblock( (char *)Fsm );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Symbol                      |
|                                                             |
\------------------------------------------------------------*/

void freertlsym( Sym )

  rtlsym *Sym;
{
  autfreeblock( (char *)Sym );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Free Model                       |
|                                                             |
\------------------------------------------------------------*/

void freertlmod( Model )

  rtlmod_list *Model;
{
  autfreeheap( (char *)Model, sizeof( rtlmod_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl Free Instance                       |
|                                                             |
\------------------------------------------------------------*/

void freertlins( Instance )

  rtlins_list *Instance;
{
  autfreeheap( (char *)Instance, sizeof( rtlins_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                         Rtl Free Port                       |
|                                                             |
\------------------------------------------------------------*/

void freertlport( Port )

  rtlport_list *Port;
{
  autfreeheap( (char *)Port, sizeof( rtlport_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                         Rtl Free Map                        |
|                                                             |
\------------------------------------------------------------*/

void freertlmap( Map )

  rtlmap_list *Map;
{
  autfreeheap( (char *)Map, sizeof( rtlmap_list ));
}

/*------------------------------------------------------------\
|                                                             |
|                         Rtl Free Generic                    |
|                                                             |
\------------------------------------------------------------*/

void freertlgen( Generic )

  rtlgen_list *Generic;
{
  autfreeheap( (char *)Generic, sizeof( rtlgen_list ));
}
