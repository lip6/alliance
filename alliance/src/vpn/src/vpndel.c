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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                   vpndel.c                        |
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
# include "vpn.h"

# include <stdio.h>
# include "vpndel.h"
# include "vpnerror.h"

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
|                      Vpn Del Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del All Line                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallvpnline( ScanLine )

  vpnline_list *ScanLine;
{
  vpnline_list  *Delete;

  while ( ScanLine != (vpnline_list *)0 )
  {
    Delete   = ScanLine;
    ScanLine = ScanLine->NEXT;

    freevpnline( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del All Port                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallvpnport( ScanPort )

  vpnport_list *ScanPort;
{
  vpnport_list *Delete;

  while ( ScanPort != (vpnport_list *)0 )
  {
    Delete   = ScanPort;
    ScanPort = ScanPort->NEXT;

    loc_delallvpnline( Delete->LINE );
    freevexexpr( Delete->VEX_ATOM );

    freevpnport( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del All Map                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallvpnmap( ScanMap )

  vpnmap_list *ScanMap;
{
  vpnmap_list *Delete;

  while ( ScanMap != (vpnmap_list *)0 )
  {
    Delete  = ScanMap;
    ScanMap = ScanMap->NEXT;

    loc_delallvpnline( Delete->LINE );
    freevexexpr( Delete->VEX_FORMAL );
    freevexexpr( Delete->VEX_ACTUAL );

    freevpnmap( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del All Generic                  |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallvpngen( ScanGeneric )

  vpngen_list *ScanGeneric;
{
  vpngen_list *Delete;

  while ( ScanGeneric != (vpngen_list *)0 )
  {
    Delete      = ScanGeneric;
    ScanGeneric = ScanGeneric->NEXT;

    loc_delallvpnline( Delete->LINE );
    freevexexpr( Delete->VEX_ATOM );
    freevexexpr( Delete->VEX_EXPR );

    freevpngen( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del All Action                   |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallvpnact( ScanAct )

  vpnact_list *ScanAct;
{
  vpnact_list  *Delete;

  while ( ScanAct != (vpnact_list *)0 )
  {
    Delete  = ScanAct;
    ScanAct = ScanAct->NEXT;

    loc_delallvpnline( Delete->LINE );
    freevexexpr( Delete->VEX_EXPR );
    freevexexpr( Delete->VEX_ATOM );

    freevpnact( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del All Place                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallvpnplace( ScanPlace )

  vpnplace_list *ScanPlace;
{
  vpnplace_list *Delete;
  vpnarc        *ScanArc;
  chain_list    *ScanChain;

  while ( ScanPlace != (vpnplace_list *)0 )
  {
    Delete    = ScanPlace;
    ScanPlace = ScanPlace->NEXT;

    for ( ScanChain  = Delete->TRANS_IN;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (vpnarc *)ScanChain->DATA;
      freevpnarc( ScanArc );
    }

    for ( ScanChain  = Delete->TRANS_OUT;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (vpnarc *)ScanChain->DATA;
      freevpnarc( ScanArc );
    }

    freechain( Delete->TRANS_IN  );
    freechain( Delete->TRANS_OUT );

    loc_delallvpnline( Delete->LINE );

    freevpnplace( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del All Trans                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallvpntrans( ScanTrans )

  vpntrans_list *ScanTrans;
{
  vpntrans_list *Delete;

  while ( ScanTrans != (vpntrans_list *)0 )
  {
    Delete    = ScanTrans;
    ScanTrans = ScanTrans->NEXT;

    freechain( Delete->PLACE_IN  );
    freechain( Delete->PLACE_OUT );
    freechain( Delete->WAIT_SYM  );

    loc_delallvpnact( Delete->ACT );

    freevexexpr( Delete->VEX_GUARD );

    freevpntrans( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Declar                       |
|                                                             |
\------------------------------------------------------------*/

static void loc_delvpndecl( Figure, Declar )

  vpnfig_list  *Figure;
  vpndecl_list *Declar;
{
  char *Name;

  *Declar->PREV = Declar->NEXT;

  if ( Declar->NEXT != (vpndecl_list *)0 ) 
  { 
    Declar->NEXT->PREV = Declar->PREV;
  }

  Name = Declar->DECL_SYM->NAME;
  delauthelem( Figure->HASH_DECLAR[ Declar->TYPE   ], Name ); 
  delauthelem( Figure->HASH_DECLAR[ VPN_DECLAR_ALL ], Name );

  freevpnsym( Declar->DECL_SYM );
  freevexexpr( Declar->VEX_ATOM );
  freevexexpr( Declar->VEX_INIT );
  freevpndecl( Declar );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Function                     |
|                                                             |
\------------------------------------------------------------*/

int delvpnfunc( Figure, Function )

  vpnfig_list  *Figure;
  vpnfunc_list *Function;
{
  vpnplace_list *DelPlace;
  vpnplace_list *ScanPlace;
  vpntrans_list *DelTrans;
  vpntrans_list *ScanTrans;
  vpndecl_list  *ScanDecl;
  vpnarc        *ScanArc;
  chain_list    *ScanChain;

  if ( Function != searchvpnfunc( Figure, Function->NAME ) )
  {
    return( 0 );
  }

  *Function->PREV = Function->NEXT;

  if ( Function->NEXT != (vpnfunc_list *)0 )
  {
    Function->NEXT->PREV = Function->PREV;
  }

  ScanPlace = Function->PLACE;

  while ( ScanPlace != (vpnplace_list *)0 )
  {
    DelPlace  = ScanPlace;
    ScanPlace = ScanPlace->NEXT;

    for ( ScanChain  = DelPlace->TRANS_IN;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (vpnarc *)ScanChain->DATA;
      delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
      freevpnarc( ScanArc );
    }

    for ( ScanChain  = DelPlace->TRANS_OUT;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (vpnarc *)ScanChain->DATA;
      delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
      freevpnarc( ScanArc );
    }

    freechain( DelPlace->TRANS_IN  );
    freechain( DelPlace->TRANS_OUT );

    loc_delallvpnline( DelPlace->LINE );

    delauthelem( Figure->HASH_PLACE, DelPlace->NAME );
    freevpnplace( DelPlace );
  }

  ScanTrans = Function->TRANS;

  while ( ScanTrans != (vpntrans_list *)0 )
  {
    DelTrans  = ScanTrans;
    ScanTrans = ScanTrans->NEXT;

    freechain( DelTrans->PLACE_IN  );
    freechain( DelTrans->PLACE_OUT );
    freechain( DelTrans->WAIT_SYM  );

    loc_delallvpnact( DelTrans->ACT );

    freevexexpr( DelTrans->VEX_GUARD );

    delauthelem( Figure->HASH_TRANS, DelTrans->NAME );
    freevpntrans( DelTrans );
  }

  loc_delallvpnline( Function->LINE );

  for ( ScanChain  = Function->ARG_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanDecl = (vpndecl_list *)ScanChain->DATA ;
    loc_delvpndecl( Figure, ScanDecl );
  }

  for ( ScanChain  = Function->VAR_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanDecl = (vpndecl_list *)ScanChain->DATA ;
    loc_delvpndecl( Figure, ScanDecl );
  }

  for ( ScanChain  = Function->DEF_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanDecl = (vpndecl_list *)ScanChain->DATA ;
    loc_delvpndecl( Figure, ScanDecl );
  }

  freechain( Function->VAR_DECL  );
  freechain( Function->DEF_DECL  );
  freechain( Function->ARG_DECL );

  freevexexpr( Function->VEX_RET );

  delauthelem( Figure->HASH_FUNCTION, Function->NAME );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Process                      |
|                                                             |
\------------------------------------------------------------*/

int delvpnproc( Figure, Process )

  vpnfig_list  *Figure;
  vpnproc_list *Process;
{
  vpnplace_list *DelPlace;
  vpnplace_list *ScanPlace;
  vpntrans_list *DelTrans;
  vpntrans_list *ScanTrans;
  vpnarc        *ScanArc;
  vpndecl_list  *ScanDecl;
  chain_list    *ScanChain;

  if ( Process != searchvpnproc( Figure, Process->NAME ) )
  {
    return( 0 );
  }

  *Process->PREV = Process->NEXT;

  if ( Process->NEXT != (vpnproc_list *)0 )
  {
    Process->NEXT->PREV = Process->PREV;
  }

  ScanPlace = Process->PLACE;

  while ( ScanPlace != (vpnplace_list *)0 )
  {
    DelPlace  = ScanPlace;
    ScanPlace = ScanPlace->NEXT;

    for ( ScanChain  = DelPlace->TRANS_IN;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (vpnarc *)ScanChain->DATA;
      delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
      freevpnarc( ScanArc );
    }

    for ( ScanChain  = DelPlace->TRANS_OUT;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (vpnarc *)ScanChain->DATA;
      delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
      freevpnarc( ScanArc );
    }

    freechain( DelPlace->TRANS_IN  );
    freechain( DelPlace->TRANS_OUT );

    loc_delallvpnline( DelPlace->LINE );

    delauthelem( Figure->HASH_PLACE, DelPlace->NAME );
    freevpnplace( DelPlace );
  }

  ScanTrans = Process->TRANS;

  while ( ScanTrans != (vpntrans_list *)0 )
  {
    DelTrans  = ScanTrans;
    ScanTrans = ScanTrans->NEXT;

    freechain( DelTrans->PLACE_IN  );
    freechain( DelTrans->PLACE_OUT );
    freechain( DelTrans->WAIT_SYM  );

    loc_delallvpnact( DelTrans->ACT );

    freevexexpr( DelTrans->VEX_GUARD );

    delauthelem( Figure->HASH_TRANS, DelTrans->NAME );
    freevpntrans( DelTrans );
  }

  loc_delallvpnline( Process->LINE );

  for ( ScanChain  = Process->VAR_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanDecl = (vpndecl_list *)ScanChain->DATA ;
    loc_delvpndecl( Figure, ScanDecl );
  }

  for ( ScanChain  = Process->DEF_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanDecl = (vpndecl_list *)ScanChain->DATA ;
    loc_delvpndecl( Figure, ScanDecl );
  }

  freechain( Process->VAR_DECL  );
  freechain( Process->DEF_DECL  );

  delauthelem( Figure->HASH_PROCESS, Process->NAME );

  freevpnproc( Process );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

int delvpnfig( Name )

   char *Name;
{
  vpnfig_list   *Figure;
  vpnfig_list  **Previous;
  vpnproc_list  *ScanProc;
  vpnmod_list   *ScanModel;
  vpnins_list   *ScanInstance;
  vpnattr_list  *ScanAttr;
  vpndecl_list  *ScanDecl;
  vpnfunc_list  *ScanFunc;
  void          *Delete;
  int            Type;

  Name     = namealloc( Name );
  Previous = &HEAD_VPNFIG;

  for ( Figure  = HEAD_VPNFIG;
        Figure != (vpnfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;

    Previous = &Figure->NEXT;
  }

  if ( Figure == (vpnfig_list *)0 ) return( 0 );

  *Previous = Figure->NEXT;

  destroyauthtable( Figure->HASH_PROCESS  );
  destroyauthtable( Figure->HASH_FUNCTION );
  destroyauthtable( Figure->HASH_MODEL    );
  destroyauthtable( Figure->HASH_INSTANCE );
  destroyauthtable( Figure->HASH_TRANS    );
  destroyauthtable( Figure->HASH_PLACE    );
  destroyauth2table( Figure->HASH_ARC     );

  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
  {
    destroyauthtable( Figure->HASH_DECLAR[ Type ] );
  }

  ScanModel = Figure->MODEL;

  while ( ScanModel != (vpnmod_list *)0 )
  {
    loc_delallvpnline( ScanModel->LINE );
    loc_delallvpnport( ScanModel->PORT );
    loc_delallvpngen( ScanModel->GENERIC );

    Delete   = (void *)( ScanModel );
    ScanModel = ScanModel->NEXT;

    freevpnmod( Delete );
  }

  ScanInstance = Figure->INSTANCE;

  while ( ScanInstance != (vpnins_list *)0 )
  {
    loc_delallvpnline( ScanInstance->LINE );
    loc_delallvpnmap( ScanInstance->MAP  );
    loc_delallvpngen( ScanInstance->GENERIC );

    Delete       = (void *)( ScanInstance );
    ScanInstance = ScanInstance->NEXT;

    freevpnins( Delete );
  }

  ScanAttr = Figure->ATTRIBUTE;

  while ( ScanAttr != (vpnattr_list *)0 )
  {
    loc_delallvpnline( ScanAttr->LINE );

    Delete   = (void *)( ScanAttr );
    ScanAttr = ScanAttr->NEXT;
  
    freevpnattr( Delete );
  }

  ScanProc = Figure->PROCESS;

  while ( ScanProc != (vpnproc_list *)0 )
  {
    loc_delallvpnplace( ScanProc->PLACE );
    loc_delallvpntrans( ScanProc->TRANS );
    loc_delallvpnline( ScanProc->LINE );

    freechain( ScanProc->VAR_DECL  );
    freechain( ScanProc->DEF_DECL  );

    Delete   = (void *)( ScanProc );
    ScanProc = ScanProc->NEXT;
  
    freevpnproc( Delete );
  }

  ScanFunc = Figure->FUNCTION;

  while ( ScanFunc != (vpnfunc_list *)0 )
  {
    loc_delallvpnplace( ScanFunc->PLACE );
    loc_delallvpntrans( ScanFunc->TRANS );
    loc_delallvpnline( ScanFunc->LINE );

    freevexexpr( ScanFunc->VEX_RET );

    freechain( ScanFunc->VAR_DECL  );
    freechain( ScanFunc->DEF_DECL  );
    freechain( ScanFunc->ARG_DECL  );

    Delete   = (void *)( ScanFunc );
    ScanFunc = ScanFunc->NEXT;
  
    freevpnfunc( Delete );
  }

  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
  {
    ScanDecl = Figure->DECLAR[ Type ];

    while ( ScanDecl != (vpndecl_list *)0 )
    {
      loc_delallvpnline( ScanDecl->LINE );

      freevexexpr( ScanDecl->VEX_ATOM );
      freevexexpr( ScanDecl->VEX_INIT );
      freevpnsym( ScanDecl->DECL_SYM );

      Delete    = (void *)( ScanDecl );
      ScanDecl = ScanDecl->NEXT;

      freevpndecl( Delete );
    }
  }

  freechain( Figure->VAR_DECL );
  freechain( Figure->DEF_DECL );

  freevpnfig( Figure );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Place                        |
|                                                             |
\------------------------------------------------------------*/

int delvpnplace( Figure, Place )

   vpnfig_list   *Figure;
   vpnplace_list *Place;
{
  vpntrans_list *Trans;
  vpnarc        *ScanArc;
  chain_list    *ChainTrans;
  chain_list    *ChainPlace;
  chain_list   **PrevChain;

  if ( Place != searchvpnplace( Figure, Place->NAME ) )
  {
    return( 0 );
  }

  *Place->PREV = Place->NEXT;

  if ( Place->NEXT != (vpnplace_list *)0 )
  {
    Place->NEXT->PREV = Place->PREV;
  }

  for ( ChainTrans  = Place->TRANS_IN;
        ChainTrans != (chain_list *)0;
        ChainTrans  = ChainTrans->NEXT )
  {
    ScanArc = (vpnarc        *)ChainTrans->DATA;
    Trans   = (vpntrans_list *)ScanArc->SOURCE;

    ChainPlace = Trans->PLACE_OUT;
    PrevChain  = &Trans->PLACE_OUT;

    while ( ChainPlace != (chain_list *)0 )
    {
      if ( ChainPlace->DATA == (void *)ScanArc ) break;

      PrevChain  = &ChainPlace->NEXT;
      ChainPlace = ChainPlace->NEXT;
    }

    if ( ChainPlace != (chain_list *)0 )
    {
      *PrevChain = ChainPlace->NEXT;

      ChainPlace->NEXT = (chain_list *)0;
      freechain( ChainPlace );

      Trans->NUMBER_OUT--;
    }
    else
    {
      vpnerror( VPN_DEL_ARC_TRANS_ERROR, Trans->NAME, 0 );
    }

    delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
    freevpnarc( ScanArc );
  }

  for ( ChainTrans  = Place->TRANS_OUT;
        ChainTrans != (chain_list *)0;
        ChainTrans  = ChainTrans->NEXT )
  {
    ScanArc = (vpnarc        *)ChainTrans->DATA;
    Trans   = (vpntrans_list *)ScanArc->TARGET;

    ChainPlace = Trans->PLACE_IN;
    PrevChain  = &Trans->PLACE_IN;

    while ( ChainPlace != (chain_list *)0 )
    {
      if ( ChainPlace->DATA == (void *)ScanArc ) break;

      PrevChain  = &ChainPlace->NEXT;
      ChainPlace = ChainPlace->NEXT;
    }

    if ( ChainPlace != (chain_list *)0 )
    {
      *PrevChain = ChainPlace->NEXT;

      ChainPlace->NEXT = (chain_list *)0;
      freechain( ChainPlace );

      Trans->NUMBER_IN--;
    }
    else
    {
      vpnerror( VPN_DEL_ARC_PLACE_ERROR, Place->NAME, 0 );
    }

    delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
    freevpnarc( ScanArc );
  }

  loc_delallvpnline( Place->LINE );

  freechain( Place->TRANS_IN  );
  freechain( Place->TRANS_OUT );

  delauthelem( Figure->HASH_PLACE, Place->NAME );

  freevpnplace( Place );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Trans                        |
|                                                             |
\------------------------------------------------------------*/

int delvpntrans( Figure, Trans )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
{
  vpnplace_list *Place;
  vpnarc        *ScanArc;
  vpnact_list   *ScanAct;
  vpnact_list   *DelAct;
  chain_list    *ChainPlace;
  chain_list    *ChainTrans;
  chain_list   **PrevChain;

  if ( Trans != searchvpntrans( Figure, Trans->NAME ) )
  {
    return( 0 );
  }

  *Trans->PREV = Trans->NEXT;

  if ( Trans->NEXT != (vpntrans_list *)0 )
  {
    Trans->NEXT->PREV = Trans->PREV;
  }

  for ( ChainPlace  = Trans->PLACE_IN;
        ChainPlace != (chain_list *)0;
        ChainPlace  = ChainPlace->NEXT )
  {
    ScanArc = (vpnarc        *)ChainPlace->DATA;
    Place   = (vpnplace_list *)ScanArc->SOURCE;

    ChainTrans = Place->TRANS_OUT;
    PrevChain  = &Place->TRANS_OUT;

    while ( ChainTrans != (chain_list *)0 )
    {
      if ( ChainTrans->DATA == (void *)ScanArc ) break;

      PrevChain  = &ChainTrans->NEXT;
      ChainTrans = ChainTrans->NEXT;
    }

    if ( ChainTrans != (chain_list *)0 )
    {
      *PrevChain = ChainTrans->NEXT;

      ChainTrans->NEXT = (chain_list *)0;
      freechain( ChainTrans );

      Place->NUMBER_OUT--;
    }
    else
    {
      vpnerror( VPN_DEL_ARC_PLACE_ERROR, Place->NAME, 0 );
    }

    delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
    freevpnarc( ScanArc );
  }

  for ( ChainPlace  = Trans->PLACE_OUT;
        ChainPlace != (chain_list *)0;
        ChainPlace  = ChainPlace->NEXT )
  {
    ScanArc = (vpnarc        *)ChainPlace->DATA;
    Place   = (vpnplace_list *)ScanArc->TARGET;

    ChainTrans = Place->TRANS_IN;
    PrevChain  = &Place->TRANS_IN;

    while ( ChainTrans != (chain_list *)0 )
    {
      if ( ChainTrans->DATA == (void *)ScanArc ) break;

      PrevChain  = &ChainTrans->NEXT;
      ChainTrans = ChainTrans->NEXT;
    }

    if ( ChainTrans != (chain_list *)0 )
    {
      *PrevChain = ChainTrans->NEXT;

      ChainTrans->NEXT = (chain_list *)0;
      freechain( ChainTrans );

      Place->NUMBER_IN--;
    }
    else
    {
      vpnerror( VPN_DEL_ARC_TRANS_ERROR, Trans->NAME, 0 );
    }


    delauth2elem( Figure->HASH_ARC, ScanArc->SOURCE, ScanArc->TARGET );
    freevpnarc( ScanArc );
  }

  freechain( Trans->PLACE_IN  );
  freechain( Trans->PLACE_OUT );

  delauthelem( Figure->HASH_TRANS, Trans->NAME );

  ScanAct = Trans->ACT;

  while ( ScanAct != (vpnact_list *)0 )
  {
    loc_delallvpnline( ScanAct->LINE );

    freevexexpr( ScanAct->VEX_EXPR );
    freevexexpr( ScanAct->VEX_ATOM );

    DelAct  = ScanAct;
    ScanAct = ScanAct->NEXT;

    freevpnact( DelAct );
  }

  freevpntrans( Trans );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Arc                          |
|                                                             |
\------------------------------------------------------------*/

int delvpnarc( Figure, Arc )

   vpnfig_list *Figure;
   vpnarc      *Arc;
{
  vpnplace_list *VpnPlace;
  vpntrans_list *VpnTrans;
  chain_list    *ScanChain;
  chain_list   **PrevChain;
  chain_list   **PrevChainTrans;
  chain_list   **PrevChainPlace;

  if ( Arc != searchvpnarc( Figure, Arc->SOURCE, Arc->TARGET ) )
  {
    return( 0 );
  }

  if ( Arc->TYPE == VPN_ARC_PLACE_TRANS )
  {
    VpnPlace = (vpnplace_list *)( Arc->SOURCE );
    VpnTrans = (vpntrans_list *)( Arc->TARGET );

    PrevChainTrans = &VpnPlace->TRANS_OUT;
    PrevChainPlace = &VpnTrans->PLACE_IN;

    VpnPlace->NUMBER_OUT--;
    VpnTrans->NUMBER_IN--;
  }
  else
  {
    VpnPlace = (vpnplace_list *)( Arc->TARGET );
    VpnTrans = (vpntrans_list *)( Arc->SOURCE );

    PrevChainTrans = &VpnPlace->TRANS_IN;
    PrevChainPlace = &VpnTrans->PLACE_OUT;

    VpnPlace->NUMBER_IN--;
    VpnTrans->NUMBER_OUT--;
  }

  PrevChain = PrevChainPlace;
  ScanChain = *PrevChain;

  while ( ScanChain != (chain_list *)0 )
  {
    if ( ScanChain->DATA == (void *)Arc ) break;

    PrevChain = &ScanChain->NEXT;
    ScanChain = ScanChain->NEXT;
  }

  if ( ScanChain != (chain_list *)0 )
  {
    *PrevChain = ScanChain->NEXT;
    ScanChain->NEXT = (chain_list *)0;
    freechain( ScanChain );
  }

  PrevChain = PrevChainTrans;
  ScanChain = *PrevChain;

  while ( ScanChain != (chain_list *)0 )
  {
    if ( ScanChain->DATA == (void *)Arc ) break;

    PrevChain = &ScanChain->NEXT;
    ScanChain = ScanChain->NEXT;
  }

  if ( ScanChain != (chain_list *)0 )
  {
    *PrevChain = ScanChain->NEXT;
    ScanChain->NEXT = (chain_list *)0;
    freechain( ScanChain );
  }

  delauth2elem( Figure->HASH_ARC, Arc->SOURCE, Arc->TARGET );
  freevpnarc( Arc );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Action                       |
|                                                             |
\------------------------------------------------------------*/

int delvpnact( Figure, Trans, Action )

   vpnfig_list   *Figure;
   vpntrans_list *Trans;
   vpnact_list   *Action;
{
  vpnact_list *NextAction;

  NextAction    = Action->NEXT;
  *Action->PREV = NextAction;

  if ( NextAction != (vpnact_list *)0 )
  {
    NextAction->PREV = Action->PREV;
  }
  else
  {
    Trans->LAST_ACT = Action->PREV;
  }

  loc_delallvpnline( Action->LINE );

  freevexexpr( Action->VEX_ATOM );
  freevexexpr( Action->VEX_EXPR );

  freevpnact( Action );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn Del Line                         |
|                                                             |
\------------------------------------------------------------*/

int delvpnline( Figure, PrevLine, Line )

  vpnfig_list   *Figure;
  vpnline_list **PrevLine;
  vpnline_list  *Line;
{
  vpnline_list *ScanLine;

  for ( ScanLine  = *PrevLine;
        ScanLine != (vpnline_list *)0;
        ScanLine  = Line->NEXT )
  {
    if ( ScanLine == Line )
    {
      *PrevLine = Line->NEXT;
      freevpnline( Line );

      return( 1 );
    }

    PrevLine = &ScanLine->NEXT;
  }

  return( 0 );
}
