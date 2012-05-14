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
| File    :                  rtnview.c                        |
|                                                             |
| Date    :                   28.08.96                        |
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
# include "rtnview.h"
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
|                       Rtl View Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Symbol                      |
|                                                             |
\------------------------------------------------------------*/

void viewrtlsym( Symbol )

  rtlsym *Symbol;
{
  fprintf( stdout, "\n--> Symbol" );
  fprintf( stdout, "\n\t\tNAME  : %s", Symbol->NAME  );

  fprintf( stdout, "\n\t\tDECL  : " );
  viewvexexprbound( Symbol->DECL->VEX_ATOM );

  fprintf( stdout, "\n\t\tINDEX : %d", (int)Symbol->INDEX );
  fprintf( stdout, "\n\t\tINIT  : '%c'", Symbol->INIT  );
  fprintf( stdout, "\n\t\tEFFEC : '%c'", Symbol->EFFEC );
  fprintf( stdout, "\n\t\tDRIVE : '%c'", Symbol->DRIVE );
  fprintf( stdout, "\n\t\tEVENT : %d"  , Symbol->EVENT );

  fprintf( stdout, "\n\t\tFLAGS : %lx", Symbol->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Symbol->USER );
  fprintf( stdout, "\n<-- Symbol" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Declaration                 |
|                                                             |
\------------------------------------------------------------*/

void viewrtldecl( Decl )

  rtldecl_list *Decl;
{
  rtlsym *DeclSym;
  int     Scan;
  int     Width;
  
  fprintf( stdout, "\n--> Declaration" );

  fprintf( stdout, "\n\t\tVEX_ATOM : " );
  viewvexexprbound( Decl->VEX_ATOM );

  fprintf( stdout, "\n\t\tVEX_INIT : " );
  viewvexexprbound( Decl->VEX_INIT );

  fprintf( stdout, "\n\t\tDECL_SYM :\n" );

  DeclSym = Decl->DECL_SYM;
  Width   = Decl->VEX_ATOM->WIDTH;

  for ( Scan = 0; Scan < Width; Scan++ )
  {
    viewrtlsym( &DeclSym[ Scan ] );
  }

  if ( Decl->DIR >= RTL_MAX_DIR_TYPE )
  {
    fprintf( stdout, "\n\t\tDIR   : %d", (int)Decl->DIR  );
  }
  else
  {
    fprintf( stdout, "\n\t\tDIR   : %s", RTL_DIR_TYPE[ Decl->DIR ] );
  }

  if ( Decl->TYPE >= RTL_MAX_DECLAR_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE  : %d", Decl->TYPE  );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE  : %s", RTL_DECLAR_TYPE[ Decl->TYPE ] );
  }

  if ( Decl->BASE >= VEX_MAX_TYPE )
  {
    fprintf( stdout, "\n\t\tBASE  : %d", Decl->BASE );
  }
  else
  {
    fprintf( stdout, "\n\t\tBASE  : %s", VEX_TYPE_NAME[ Decl->BASE ] );
  }

  if ( Decl->KIND >= RTL_MAX_KIND_TYPE )
  {
    fprintf( stdout, "\n\t\tKIND  : %d", Decl->KIND );
  }
  else
  {
    fprintf( stdout, "\n\t\tKIND  : %s", RTL_KIND_TYPE[ Decl->KIND ] );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Decl->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Decl->USER );
  fprintf( stdout, "\n<-- Declaration" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl View BiVex                          |
|                                                             |
\------------------------------------------------------------*/

void viewrtlbivex( BiVex )

  rtlbivex_list *BiVex;
{
  fprintf( stdout, "\n--> BiVex" );

  fprintf( stdout, "\n\t\tVEX_COND : " );

  viewvexexprbound( BiVex->VEX_COND );

  fprintf( stdout, "\n\t\tVEX_DATA : \n" );

  viewvexexprbound( BiVex->VEX_DATA );

  if ( BiVex->TYPE >= RTL_MAX_BIVEX_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE     : %d", BiVex->TYPE  );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE     : %s", RTL_BIVEX_TYPE[ BiVex->TYPE ] );
  }

  fprintf( stdout, "\n\t\tFLAGS    : %lx", BiVex->FLAGS );
  fprintf( stdout, "\n\t\tUSER     : %lx", (long)BiVex->USER );
  fprintf( stdout, "\n<-- BiVex" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl View Assign                         |
|                                                             |
\------------------------------------------------------------*/

void viewrtlasg( Asg )

  rtlasg_list *Asg;
{
  rtlbivex_list *BiVex;

  fprintf( stdout, "\n--> Assign" );

  if ( Asg->TYPE >= RTL_MAX_ASG_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE     : %d", Asg->TYPE );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE     : %s", RTL_ASG_TYPE[ Asg->TYPE ] );
  }

  if ( Asg->REG_TYPE >= RTL_MAX_ASG_REGISTER_TYPE )
  {
    fprintf( stdout, "\n\t\tREG_TYPE : %d", Asg->REG_TYPE );
  }
  else
  {
    fprintf( stdout, "\n\t\tREG_TYPE : %s", RTL_ASG_REGISTER_TYPE[ Asg->REG_TYPE ] );
  }

  fprintf( stdout, "\n\t\tVEX_ATOM : " );

  viewvexexprbound( Asg->VEX_ATOM );

  fprintf( stdout, "\n\t\tVEX_DATA : \n" );

  viewvexexprbound( Asg->VEX_DATA );

  fprintf( stdout, "\n\t\tBIVEX    : \n" );

  for ( BiVex  = Asg->BIVEX;
        BiVex != (rtlbivex_list *)0;
        BiVex  = BiVex->NEXT )
  {
    viewrtlbivex( BiVex );
  }

  fprintf( stdout, "\n\t\tFLAGS    : %lx", Asg->FLAGS );
  fprintf( stdout, "\n\t\tUSER     : %lx", (long)Asg->USER );

  fprintf( stdout, "\n<-- Assign" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl View Fsm Assign                     |
|                                                             |
\------------------------------------------------------------*/

void viewrtlfsmasg( Asg )

  rtlfsmasg_list *Asg;
{
  fprintf( stdout, "\n--> Fsm Assign" );

  fprintf( stdout, "\n\t\tVEX_ATOM : " );

  viewvexexprbound( Asg->VEX_ATOM );

  fprintf( stdout, "\n\t\tVEX_DATA : \n" );

  viewvexexprbound( Asg->VEX_DATA );

  fprintf( stdout, "\n\t\tFLAGS    : %lx", Asg->FLAGS );
  fprintf( stdout, "\n\t\tUSER     : %lx", (long)Asg->USER );

  fprintf( stdout, "\n<-- Fsm Assign" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Fsm State                   |
|                                                             |
\------------------------------------------------------------*/

void viewrtlfsmstate( State )

  rtlfsmstate_list *State;
{
  rtlfsmasg_list *FsmAsg;

  fprintf( stdout, "\n--> State" );
  fprintf( stdout, "\n\tNAME     : %s", State->NAME  );

  for ( FsmAsg  = State->ASSIGN;
        FsmAsg != (rtlfsmasg_list *)0;
        FsmAsg  = FsmAsg->NEXT )
  {
    viewrtlfsmasg( FsmAsg );
  }

  fprintf( stdout, "\n\tFLAGS    : %lx", State->FLAGS );
  fprintf( stdout, "\n\tUSER     : %lx", (long)State->USER );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Fsm Trans                   |
|                                                             |
\------------------------------------------------------------*/

void viewrtlfsmtrans( Trans )

  rtlfsmtrans_list *Trans;
{
  rtlfsmasg_list *FsmAsg;

  fprintf( stdout, "\n--> Trans" );

  if ( Trans->FROM != (rtlfsmstate_list *)0 )
  {
    fprintf( stdout, "\n\tFROM     : %s", Trans->FROM->NAME );
  }

  if ( Trans->TO != (rtlfsmstate_list *)0 )
  {
    fprintf( stdout, "\n\tTO       : %s", Trans->TO->NAME );
  }

  fprintf( stdout, "\n\t\tVEX_COND : " );

  viewvexexprbound( Trans->VEX_COND );

  for ( FsmAsg  = Trans->ASSIGN;
        FsmAsg != (rtlfsmasg_list *)0;
        FsmAsg  = FsmAsg->NEXT )
  {
    viewrtlfsmasg( FsmAsg );
  }

  fprintf( stdout, "\n\tFLAGS    : %lx", Trans->FLAGS );
  fprintf( stdout, "\n\tUSER     : %lx", (long)Trans->USER );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Fsm                         |
|                                                             |
\------------------------------------------------------------*/

void viewrtlfsm( Fsm )

  rtlfsm_list *Fsm;
{
  rtlfsmtrans_list *FsmTrans;
  rtlfsmstate_list *FsmState;

  fprintf( stdout, "\n--> Fsm" );
  fprintf( stdout, "\n\tNAME            : %s", Fsm->NAME  );

  fprintf( stdout, "\n\tSTATE  : " );

  for ( FsmState  = Fsm->STATE;
        FsmState != (rtlfsmstate_list *)0;
        FsmState  = FsmState->NEXT )
  {
    viewrtlfsmstate( FsmState );
  }

  fprintf( stdout, "\n\tTRANS  : " );

  for ( FsmTrans  = Fsm->TRANS;
        FsmTrans != (rtlfsmtrans_list *)0;
        FsmTrans  = FsmTrans->NEXT )
  {
    viewrtlfsmtrans( FsmTrans );
  }

  fprintf( stdout, "\n\tFLAGS : %lx", Fsm->FLAGS );
  fprintf( stdout, "\n\tUSER  : %lx", (long)Fsm->USER );

  fprintf( stdout, "\n<-- Fsm" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Port                        |
|                                                             |
\------------------------------------------------------------*/

void viewrtlport( Port )

  rtlport_list *Port;
{
  fprintf( stdout, "\n--> Port" );

  fprintf( stdout, "\n\t\tVEX_ATOM : " );
  viewvexexprbound( Port->VEX_ATOM );

  if ( Port->DIR >= RTL_MAX_DIR_TYPE )
  {
    fprintf( stdout, "\n\t\tDIR   : %d", (int)Port->DIR  );
  }
  else
  {
    fprintf( stdout, "\n\t\tDIR   : %s", RTL_DIR_TYPE[ Port->DIR ] );
  }

  if ( Port->BASE >= VEX_MAX_TYPE )
  {
    fprintf( stdout, "\n\t\tBASE  : %d", Port->BASE );
  }
  else
  {
    fprintf( stdout, "\n\t\tBASE  : %s", VEX_TYPE_NAME[ Port->BASE ] );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Port->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Port->USER );
  fprintf( stdout, "\n<-- Port" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Map                         |
|                                                             |
\------------------------------------------------------------*/

void viewrtlmap( Map )

  rtlmap_list *Map;
{
  fprintf( stdout, "\n--> Map" );

  fprintf( stdout, "\n\t\tFORMAL   : " );

  viewvexexprbound( Map->VEX_FORMAL );

  fprintf( stdout, "\n\t\tACTUAL   : " );

  viewvexexprbound( Map->VEX_ACTUAL );

  fprintf( stdout, "\n\t\tFLAGS : %lx", Map->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Map->USER );
  fprintf( stdout, "\n<-- Map" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn View Generic                     |
|                                                             |
\------------------------------------------------------------*/

void viewrtlgen( Generic )

  rtlgen_list *Generic;
{
  fprintf( stdout, "\n--> Generic" );

  fprintf( stdout, "\n\t\tATOM  : " );

  viewvexexprbound( Generic->VEX_ATOM );

  fprintf( stdout, "\n\t\tEXPR  : " );

  viewvexexprbound( Generic->VEX_EXPR );

  if ( Generic->BASE >= VEX_MAX_TYPE )
  {
    fprintf( stdout, "\n\t\tBASE  : %d", Generic->BASE );
  }
  else
  {
    fprintf( stdout, "\n\t\tBASE  : %s", VEX_TYPE_NAME[ Generic->BASE ] );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Generic->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Generic->USER );
  fprintf( stdout, "\n<-- Generic" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl View Model                          |
|                                                             |
\------------------------------------------------------------*/

void viewrtlmod( Model )

  rtlmod_list *Model;
{
  rtlport_list *ScanPort;
  rtlgen_list  *ScanGeneric;

  fprintf( stdout, "\n--> Model" );

  fprintf( stdout, "\n\t\tNAME  : %s", Model->NAME );

  fprintf( stdout, "\n\t\tPORT  :" );

  for ( ScanPort  = Model->PORT;
        ScanPort != (rtlport_list *)0;
        ScanPort  = ScanPort->NEXT )
  {
    viewrtlport( ScanPort );
  }

  fprintf( stdout, "\n\t\tGENERIC:" );

  for ( ScanGeneric  = Model->GENERIC;
        ScanGeneric != (rtlgen_list *)0;
        ScanGeneric  = ScanGeneric->NEXT )
  {
    viewrtlgen( ScanGeneric );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Model->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Model->USER );
  fprintf( stdout, "\n<-- Model" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Rtl View Instance                       |
|                                                             |
\------------------------------------------------------------*/

void viewrtlins( Instance )

  rtlins_list *Instance;
{
  rtlmap_list *ScanMap;
  rtlgen_list *ScanGeneric;

  fprintf( stdout, "\n--> Instance" );

  fprintf( stdout, "\n\t\tNAME  : %s", Instance->NAME );
  fprintf( stdout, "\n\t\tMODEL : %s", Instance->MODEL->NAME );

  fprintf( stdout, "\n\t\tMAP   :" );

  for ( ScanMap  = Instance->MAP;
        ScanMap != (rtlmap_list *)0;
        ScanMap  = ScanMap->NEXT )
  {
    viewrtlmap( ScanMap );
  }

  fprintf( stdout, "\n\t\tGENERIC:" );

  for ( ScanGeneric  = Instance->GENERIC;
        ScanGeneric != (rtlgen_list *)0;
        ScanGeneric  = ScanGeneric->NEXT )
  {
    viewrtlgen( ScanGeneric );
  }


  fprintf( stdout, "\n\t\tFLAGS : %lx", Instance->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Instance->USER );
  fprintf( stdout, "\n<-- Model" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rtl View Figure                      |
|                                                             |
\------------------------------------------------------------*/

void viewrtlfig( Figure )

  rtlfig_list *Figure;
{
  rtlmod_list   *Model;
  rtlins_list   *Instance;
  rtldecl_list  *Decl;
  rtlasg_list   *Assign;
  rtlfsm_list   *Fsm;
  int            Type;

  fprintf( stdout, "\n--> Figure" );
  fprintf( stdout, "\n\tNAME            : %s", Figure->NAME  );

  for ( Type = 0; Type < RTL_MAX_DECLAR_TYPE; Type++ )
  {
    fprintf( stdout, "\n\tDECLARATION %s : ", RTL_DECLAR_TYPE[ Type ]  );

    for ( Decl  = Figure->DECLAR[ Type ];
          Decl != (rtldecl_list *)0;
          Decl  = Decl->NEXT )
    {
      viewrtldecl( Decl );
    }
  }

  fprintf( stdout, "\n\tMODEL     :" );

  for ( Model  = Figure->MODEL;
        Model != (rtlmod_list *)0;
        Model  = Model->NEXT )
  {
    viewrtlmod( Model );
  }

  fprintf( stdout, "\n\tINSTANCE  :" );

  for ( Instance  = Figure->INSTANCE;
        Instance != (rtlins_list *)0;
        Instance  = Instance->NEXT )
  {
    viewrtlins( Instance );
  }

  fprintf( stdout, "\n\tASSIGN : " );

  for ( Assign  = Figure->ASSIGN;
        Assign != (rtlasg_list *)0;
        Assign  = Assign->NEXT )
  {
    viewrtlasg( Assign );
  }

  fprintf( stdout, "\n\tFSM    : " );

  for ( Fsm  = Figure->FSM;
        Fsm != (rtlfsm_list *)0;
        Fsm  = Fsm->NEXT )
  {
    viewrtlfsm( Fsm );
  }

  fprintf( stdout, "\n\tFLAGS : %lx", Figure->FLAGS );
  fprintf( stdout, "\n\tUSER  : %lx", (long)Figure->USER );
  fprintf( stdout, "\n<-- Figure\n" );
}
