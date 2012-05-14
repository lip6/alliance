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
| Tool    :                     Rtl                           |
|                                                             |
| File    :                   rtndel.c                        |
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
# include "rtndel.h"
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
|                        Rtl Del All Port                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallrtlport( ScanPort )

  rtlport_list *ScanPort;
{
  rtlport_list *Delete;

  while ( ScanPort != (rtlport_list *)0 )
  {
    Delete   = ScanPort;
    ScanPort = ScanPort->NEXT;

    freevexexpr( Delete->VEX_ATOM );

    freertlport( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Del All Map                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallrtlmap( ScanMap )

  rtlmap_list *ScanMap;
{
  rtlmap_list *Delete;

  while ( ScanMap != (rtlmap_list *)0 )
  {
    Delete  = ScanMap;
    ScanMap = ScanMap->NEXT;

    freevexexpr( Delete->VEX_FORMAL );
    freevexexpr( Delete->VEX_ACTUAL );

    freertlmap( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Del All Generic                  |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallrtlgen( ScanGeneric )

  rtlgen_list *ScanGeneric;
{
  rtlgen_list *Delete;

  while ( ScanGeneric != (rtlgen_list *)0 )
  {
    Delete      = ScanGeneric;
    ScanGeneric = ScanGeneric->NEXT;

    freevexexpr( Delete->VEX_ATOM );
    freevexexpr( Delete->VEX_EXPR );

    freertlgen( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Rtl Del Functions                      |
|                                                             |
\------------------------------------------------------------*/

static void loc_delallbivex( ScanBiVex )

  rtlbivex_list *ScanBiVex;
{
  rtlbivex_list *Delete;

  while ( ScanBiVex != (rtlbivex_list *)0 )
  {
    freevexexpr( ScanBiVex->VEX_COND );
    freevexexpr( ScanBiVex->VEX_DATA );

    Delete    = ScanBiVex;
    ScanBiVex = ScanBiVex->NEXT;

    freertlbivex( Delete );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Del Assign                       |
|                                                             |
\------------------------------------------------------------*/

int delrtlasg( Figure, Assign )

  rtlfig_list *Figure;
  rtlasg_list *Assign;
{
  *Assign->PREV = Assign->NEXT;

  if ( Assign->NEXT != (rtlasg_list *)0 )
  {
    Assign->NEXT->PREV = Assign->PREV;
  }

  freevexexpr( Assign->VEX_ATOM   );
  freevexexpr( Assign->VEX_DATA   );

  loc_delallbivex( Assign->BIVEX );

  freertlasg( Assign );

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

int delrtlfig( Name )

  char *Name;
{
  rtlfig_list      *Figure;
  rtlfig_list     **Previous;
  rtldecl_list     *ScanDecl;
  rtlasg_list      *ScanAsg;
  rtlfsm_list      *ScanFsm;
  rtlfsmstate_list *ScanState;
  rtlfsmtrans_list *ScanTrans;
  rtlfsmasg_list   *ScanFsmAsg;
  rtlmod_list      *ScanModel;
  rtlins_list      *ScanInstance;
  void             *Delete;
  int               Type;

  Name     = namealloc( Name );
  Previous = &HEAD_RTLFIG;

  for ( Figure  = HEAD_RTLFIG;
        Figure != (rtlfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;

    Previous = &Figure->NEXT;
  }

  if ( Figure == (rtlfig_list *)0 ) return( 0 );

  *Previous = Figure->NEXT;

  for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
  {
    ScanDecl = Figure->DECLAR[ Type ];

    while ( ScanDecl != (rtldecl_list *)0 )
    {
      freevexexpr( ScanDecl->VEX_ATOM );
      freevexexpr( ScanDecl->VEX_INIT );
      freertlsym( ScanDecl->DECL_SYM );

      Delete    = (void *)( ScanDecl );
      ScanDecl = ScanDecl->NEXT;

      freertldecl( Delete );
    }
  }

  destroyauthtable( Figure->HASH_DECLAR   );
  destroyauthtable( Figure->HASH_MODEL    );
  destroyauthtable( Figure->HASH_INSTANCE );

  ScanModel = Figure->MODEL;

  while ( ScanModel != (rtlmod_list *)0 )
  {
    loc_delallrtlport( ScanModel->PORT );
    loc_delallrtlgen( ScanModel->GENERIC );

    Delete   = (void *)( ScanModel );
    ScanModel = ScanModel->NEXT;

    freertlmod( Delete );
  }

  ScanInstance = Figure->INSTANCE;

  while ( ScanInstance != (rtlins_list *)0 )
  {
    loc_delallrtlmap( ScanInstance->MAP  );
    loc_delallrtlgen( ScanInstance->GENERIC );

    Delete       = (void *)( ScanInstance );
    ScanInstance = ScanInstance->NEXT;

    freertlins( Delete );
  }

  ScanAsg = Figure->ASSIGN;

  while ( ScanAsg != (rtlasg_list *)0 )
  {
    freevexexpr( ScanAsg->VEX_ATOM   );
    freevexexpr( ScanAsg->VEX_DATA   );

    loc_delallbivex( ScanAsg->BIVEX );

    Delete   = (void *)( ScanAsg );
    ScanAsg = ScanAsg->NEXT;
  
    freertlasg( Delete );
  }

  ScanFsm = Figure->FSM;

  while ( ScanFsm != (rtlfsm_list *)0 )
  {
    ScanState = ScanFsm->STATE; 

    while ( ScanState != (rtlfsmstate_list *)0 )
    {
      freechain( ScanState->FROM );
      freechain( ScanState->TO   );

      ScanFsmAsg = ScanState->ASSIGN;

      while ( ScanFsmAsg != (rtlfsmasg_list *)0 )
      {
        freevexexpr( ScanFsmAsg->VEX_ATOM );
        freevexexpr( ScanFsmAsg->VEX_DATA );
  
        Delete     = (void *)ScanFsmAsg;
        ScanFsmAsg = ScanFsmAsg->NEXT;

        freertlfsmasg( Delete );
      }
   
      Delete    = (void *)ScanState;
      ScanState = ScanState->NEXT;

      freertlfsmstate( Delete );
    }

    ScanTrans = ScanFsm->TRANS; 

    while ( ScanTrans != (rtlfsmtrans_list *)0 )
    {
      freevexexpr( ScanTrans->VEX_COND );

      ScanFsmAsg = ScanTrans->ASSIGN;

      while ( ScanFsmAsg != (rtlfsmasg_list *)0 )
      {
        freevexexpr( ScanFsmAsg->VEX_ATOM );
        freevexexpr( ScanFsmAsg->VEX_DATA );
  
        Delete     = (void *)ScanFsmAsg;
        ScanFsmAsg = ScanFsmAsg->NEXT;

        freertlfsmasg( Delete );
      }

      Delete    = (void *)ScanTrans;
      ScanTrans = ScanTrans->NEXT;

      freertlfsmtrans( Delete );
    }

    loc_delallbivex( ScanFsm->BIVEX );

    destroyauthtable( ScanFsm->HASH_STATE );

    Delete   = (void *)( ScanFsm );
    ScanFsm = ScanFsm->NEXT;

    freertlfsm( Delete );
  }

  freertlfig( Figure );

  return( 1 );
}
