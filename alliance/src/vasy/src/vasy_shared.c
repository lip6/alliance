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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                    VASY                           |
|                                                             |
| File    :                 vasy_shared.c                     |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
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
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_error.h"
# include "vasy_shared.h"

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

  char *VASY_SYM_TYPE[ VASY_MAX_SYM_TYPE ] =
  {
    "UNKNOWN",
    "UNUSED",
    "CONSTANT",
    "COMBINATORIAL",
    "TRISTATE",
    "REGISTER"
  };

  char *VasyPowerVdd = (char *)0;
  char *VasyPowerVss = (char *)0;

  int   VasyFlagDebug    = 0;
  int   VasyFlagOption   = 0;
  int   VasyFlagDrive    = 2;
  int   VasyFlagVerbose  = 0;
  int   VasyFlagHier     = 0;
  int   VasyFlagOver     = 0;
  int   VasyFlagStdLogic = 0;
  int   VasyFlagInitial  = 0;
  int   VasyFlagPower    = 0;
  int   VasyFlagCLA      = 0;
  int   VasyFlagEqual    = 0;
  int   VasyFlagLax      = 0;
  int   VasyFlagBoom     = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      VasyAddVpnProcInfo                     |
|                                                             |
\------------------------------------------------------------*/

vasyprocinfo *VasyAddVpnProcInfo( VpnProc )

   vpnproc_list *VpnProc;
{
  vasyprocinfo *ProcInfo;

  ProcInfo = (vasyprocinfo *)autallocheap( sizeof( vasyprocinfo ) );

  SetVasyVpnProcInfo( VpnProc, ProcInfo );

  return( ProcInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyDelVpnProcInfo                     |
|                                                             |
\------------------------------------------------------------*/

void VasyDelVpnProcInfo( VpnProc )

   vpnproc_list *VpnProc;
{
  vasyprocinfo   *ProcInfo;
  vasybivex_list *ProcBiVex;
  vasybivex_list *DelProcBiVex;

  ProcInfo = GetVasyVpnProcInfo( VpnProc );

  ProcBiVex = ProcInfo->BIVEX;

  while ( ProcBiVex != (vasybivex_list *)0 )
  {
    freevexexpr( ProcBiVex->VEX_COND );
    freevexexpr( ProcBiVex->VEX_DATA );

    DelProcBiVex = ProcBiVex;
    ProcBiVex    = ProcBiVex->NEXT;

    autfreeheap( DelProcBiVex, sizeof( vasybivex_list ) );
  }

  autfreeheap( ProcInfo, sizeof( vasyprocinfo ) );

  SetVasyVpnProcInfo( VpnProc, (vasyprocinfo *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyAddVpnActInfo                      |
|                                                             |
\------------------------------------------------------------*/

vasyactinfo *VasyAddVpnActInfo( VpnAction )

   vpnact_list *VpnAction;
{
  vasyactinfo   *ActInfo;
  vexexpr       *AsgAtom;
  chain_list   **Support;
  chain_list   **Literal;
  chain_list   **Event;
  vasysyminfo    *SymInfo;
  unsigned char *Flags;
  short          Width;

  AsgAtom = VpnAction->VEX_ATOM;
  Width   = AsgAtom->WIDTH;

  ActInfo = (vasyactinfo    *)autallocheap( sizeof( vasyactinfo ) );

  Support = (chain_list    **)autallocblock( sizeof( chain_list *) * Width );
  Literal = (chain_list    **)autallocblock( sizeof( chain_list *) * Width );
  Event   = (chain_list    **)autallocblock( sizeof( chain_list *) * Width );
  Flags   = (unsigned char  *)autallocblock( sizeof( char        ) * Width );
  SymInfo = (vasysyminfo    *)autallocblock( sizeof( vasysyminfo ) * Width );

  ActInfo->SUPPORT  = Support;
  ActInfo->LITERAL  = Literal;
  ActInfo->EVENT    = Event;
  ActInfo->FLAGS    = Flags;
  ActInfo->SYM_INFO = SymInfo;

  SetVasyVpnActInfo( VpnAction, ActInfo );

  return( ActInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyDelVpnActInfo                      |
|                                                             |
\------------------------------------------------------------*/

void VasyDelVpnActInfo( VpnAction )

   vpnact_list *VpnAction;
{
  vasyactinfo     *ActInfo;
  vexexpr        *AsgAtom;
  chain_list    **Support;
  chain_list    **Literal;
  chain_list    **Event;
  unsigned char  *Flags;
  vasysyminfo     *SymInfo;
  vasybivex_list *SymBiVex;
  vasybivex_list *DelSymBiVex;
  int             Index;

  ActInfo = GetVasyVpnActInfo( VpnAction );

  AsgAtom = VpnAction->VEX_ATOM;
  Support = ActInfo->SUPPORT;
  Literal = ActInfo->LITERAL;
  Event   = ActInfo->EVENT;
  Flags   = ActInfo->FLAGS;
  SymInfo = ActInfo->SYM_INFO;

  for ( Index = 0; Index < AsgAtom->WIDTH; Index++ )
  {
    freechain( Support[ Index ] );
    freechain( Literal[ Index ] );

    SymBiVex = SymInfo[ Index ].BIVEX;

    while ( SymBiVex != (vasybivex_list *)0 )
    {
      freevexexpr( SymBiVex->VEX_COND );
      freevexexpr( SymBiVex->VEX_DATA );

      DelSymBiVex = SymBiVex;
      SymBiVex    = SymBiVex->NEXT;

      autfreeheap( DelSymBiVex, sizeof( vasybivex_list ) );
    }

    freevexexpr( SymInfo[ Index ].VEX_DATA );
  }

  autfreeblock( Support );
  autfreeblock( Literal );
  autfreeblock( Event   );
  autfreeblock( Flags   );
  autfreeblock( SymInfo );

  autfreeheap( ActInfo, sizeof( vasyactinfo ) );

  SetVasyVpnActInfo( VpnAction, (vasyactinfo *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyAddVpnWaitInfo                       |
|                                                             |
\------------------------------------------------------------*/

vasywaitinfo *VasyAddVpnWaitInfo( VpnTrans )

   vpntrans_list *VpnTrans;
{
  vasywaitinfo  *WaitInfo;

  WaitInfo = (vasywaitinfo *)autallocheap( sizeof( vasywaitinfo ) );
  SetVasyVpnWaitInfo( VpnTrans, WaitInfo );

  return( WaitInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyDelVpnWaitInfo                      |
|                                                             |
\------------------------------------------------------------*/

void VasyDelVpnWaitInfo( VpnTrans )

   vpntrans_list *VpnTrans;
{
  vasywaitinfo  *WaitInfo;

  WaitInfo = GetVasyVpnWaitInfo( VpnTrans );
  autfreeheap( WaitInfo, sizeof( vasywaitinfo ) );

  SetVasyVpnWaitInfo( VpnTrans, (vasywaitinfo *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyAddVpnTransInfo                      |
|                                                             |
\------------------------------------------------------------*/

vasytransinfo *VasyAddVpnTransInfo( VpnTrans )

   vpntrans_list *VpnTrans;
{
  vasytransinfo  *TransInfo;

  TransInfo = (vasytransinfo *)autallocheap( sizeof( vasytransinfo ) );

  TransInfo->HASH_ASSIGN  = createauthtable( 100 );
  TransInfo->HASH_BITVEC  = createauthtable( 100 );
  TransInfo->HASH_SUPPORT = createauth2table( 100 );
  TransInfo->HASH_LITERAL = createauth2table( 100 );
  TransInfo->HASH_EVENT   = createauth2table( 100 );

  SetVasyVpnTransInfo( VpnTrans, TransInfo );

  return( TransInfo );
}

/*------------------------------------------------------------\
|                                                             |
|                     VasyDelVpnTransInfo                     |
|                                                             |
\------------------------------------------------------------*/

void VasyDelVpnTransInfo( VpnTrans )

   vpntrans_list *VpnTrans;
{
  vasytransinfo  *TransInfo;

  TransInfo = GetVasyVpnTransInfo( VpnTrans );

  destroyauthtable( TransInfo->HASH_ASSIGN  );
  destroyauthtable( TransInfo->HASH_BITVEC  );
  destroyauth2table( TransInfo->HASH_SUPPORT );
  destroyauth2table( TransInfo->HASH_LITERAL );
  destroyauth2table( TransInfo->HASH_EVENT   );

  autfreeheap( TransInfo, sizeof( vasytransinfo ) );

  SetVasyVpnTransInfo( VpnTrans, (vasytransinfo *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyAddVpnSymBiVex                       |
|                                                             |
\------------------------------------------------------------*/

vasybivex_list *VasyAddVpnSymBiVex( SymInfo, VexCond, VexData, Type )

  vasysyminfo *SymInfo;
  vexexpr    *VexCond;
  vexexpr    *VexData;
  int         Type;
{
  vasybivex_list *SymBiVex;

  SymBiVex = (vasybivex_list *)autallocheap( sizeof( vasybivex_list ) );

  SymBiVex->VEX_COND = VexCond;
  SymBiVex->VEX_DATA = VexData;
  SymBiVex->TYPE     = Type;

  SymBiVex->NEXT = SymInfo->BIVEX;
  SymInfo->BIVEX = SymBiVex;

  return( SymBiVex );
}

/*------------------------------------------------------------\
|                                                             |
|                    VasyAddVpnProcBiVex                      |
|                                                             |
\------------------------------------------------------------*/

vasybivex_list *VasyAddVpnProcBiVex( ProcInfo, VexCond, VexData, Type )

  vasyprocinfo *ProcInfo;
  vexexpr     *VexCond;
  vexexpr     *VexData;
  int          Type;
{
  vasybivex_list *SymBiVex;

  SymBiVex = (vasybivex_list *)autallocheap( sizeof( vasybivex_list ) );

  SymBiVex->VEX_COND = VexCond;
  SymBiVex->VEX_DATA = VexData;
  SymBiVex->TYPE     = Type;

  SymBiVex->NEXT  = ProcInfo->BIVEX;
  ProcInfo->BIVEX = SymBiVex;

  return( SymBiVex );
}
