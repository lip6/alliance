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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VPN                           |
|                                                             |
| File    :                   vpnsimp.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.01.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"

# include "vpnerror.h"
# include "vpnsimp.h"

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
|                       Private Variables                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        simpvpnact                           |
|                                                             |
\------------------------------------------------------------*/

void simpvpnact( VpnAct )

  vpnact_list *VpnAct;
{
  VpnAct->VEX_ATOM = simpvexexpr( VpnAct->VEX_ATOM );
  VpnAct->VEX_EXPR = simpvexexpr( VpnAct->VEX_EXPR );
}

/*------------------------------------------------------------\
|                                                             |
|                        simpvpntrans                         |
|                                                             |
\------------------------------------------------------------*/

void simpvpntrans( VpnTrans )

  vpntrans_list *VpnTrans;
{
  vpnact_list *VpnAct;

  VpnTrans->VEX_GUARD = simpvexexpr( VpnTrans->VEX_GUARD );

  for ( VpnAct  = VpnTrans->ACT;
        VpnAct != (vpnact_list *)0;
        VpnAct  = VpnAct->NEXT )
  {
    simpvpnact( VpnAct );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        simpvpnfunc                          |
|                                                             |
\------------------------------------------------------------*/

void simpvpnfunc( VpnFunc )

  vpnfunc_list *VpnFunc;
{
  vpntrans_list *VpnTrans;

  for ( VpnTrans  = VpnFunc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    simpvpntrans( VpnTrans );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        simpvpnproc                          |
|                                                             |
\------------------------------------------------------------*/

void simpvpnproc( VpnProc )

  vpnproc_list *VpnProc;
{
  vpntrans_list *VpnTrans;

  for ( VpnTrans  = VpnProc->TRANS;
        VpnTrans != (vpntrans_list *)0;
        VpnTrans  = VpnTrans->NEXT )
  {
    simpvpntrans( VpnTrans );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        simpvpnfig                           |
|                                                             |
\------------------------------------------------------------*/

void simpvpnfig( VpnFigure )

  vpnfig_list *VpnFigure;
{
  vpnproc_list *VpnProc;
  vpnfunc_list *VpnFunc;

  for ( VpnProc  = VpnFigure->PROCESS;
        VpnProc != (vpnproc_list *)0;
        VpnProc  = VpnProc->NEXT )
  {
    simpvpnproc( VpnProc );
  }

  for ( VpnFunc  = VpnFigure->FUNCTION;
        VpnFunc != (vpnfunc_list *)0;
        VpnFunc  = VpnFunc->NEXT )
  {
    simpvpnfunc( VpnFunc );
  }
}
