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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                  vpnview.c                        |
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
# include "vpn.h"

# include <stdio.h>
# include "vpnview.h"
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
|                       Vpn View Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Vpn View Line                        |
|                                                             |
\------------------------------------------------------------*/

void viewvpnline( Line )

  vpnline_list *Line;
{
  char *File;

  fprintf( stdout, "\n--> Line" );
  fprintf( stdout, "\n\t\tLINE  : " );

  File = (char *)0;

  while ( Line != (vpnline_list *)0 )
  {
    if ( Line->NAME != File ) 
    {
      File = Line->NAME;
      fprintf( stdout, "  %s [%ld]", Line->NAME, Line->LINE );
    }
    else
    {
      fprintf( stdout, "  [%ld]", Line->LINE );
    }

    Line = Line->NEXT;
  }

  fprintf( stdout, "\n<-- Line" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn View Attribute                   |
|                                                             |
\------------------------------------------------------------*/

void viewvpnattr( Attr )

  vpnattr_list *Attr;
{
  fprintf( stdout, "\n--> Attribute" );
  fprintf( stdout, "\n\t\tNAME  : %s" , Attr->NAME   );
  fprintf( stdout, "\n\t\tENTITY: %s" , Attr->ENTITY );

  if ( Attr->TYPE >= VPN_MAX_ATTR_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE  : %d", (int)Attr->TYPE );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE  : %s", VPN_ATTRIBUTE_TYPE[ Attr->TYPE ] );
  }

  fprintf( stdout, "\n\t\tVALUE : %s" , Attr->VALUE  );
  fprintf( stdout, "\n\t\tFLAGS : %lx", Attr->FLAGS  );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Attr->USER );
  fprintf( stdout, "\n<-- Symbol" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn View Port                        |
|                                                             |
\------------------------------------------------------------*/

void viewvpnport( Port )

  vpnport_list *Port;
{
  fprintf( stdout, "\n--> Port" );

  viewvpnline( Port->LINE );

  fprintf( stdout, "\n\t\tVEX_ATOM : " );
  viewvexexprbound( Port->VEX_ATOM );

  if ( Port->DIR >= VPN_MAX_DIR_TYPE )
  {
    fprintf( stdout, "\n\t\tDIR   : %d", (int)Port->DIR  );
  }
  else
  {
    fprintf( stdout, "\n\t\tDIR   : %s", VPN_DIR_TYPE[ Port->DIR ] );
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
|                        Vpn View Map                         |
|                                                             |
\------------------------------------------------------------*/

void viewvpnmap( Map )

  vpnmap_list *Map;
{
  fprintf( stdout, "\n--> Map" );

  viewvpnline( Map->LINE );

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

void viewvpngen( Generic )

  vpngen_list *Generic;
{
  fprintf( stdout, "\n--> Generic" );

  viewvpnline( Generic->LINE );

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
|                        Vpn View Symbol                      |
|                                                             |
\------------------------------------------------------------*/

void viewvpnsym( Symbol )

  vpnsym *Symbol;
{
  fprintf( stdout, "\n--> Symbol" );
  fprintf( stdout, "\n\t\tNAME  : %s", Symbol->NAME  );

  fprintf( stdout, "\n\t\tDECL  : " );
  viewvexexprbound( Symbol->DECL->VEX_ATOM );

  fprintf( stdout, "\n\t\tINDEX : %d", (int)Symbol->INDEX );
  fprintf( stdout, "\n\t\tINIT  : '%c'", VEX_LITERAL_BY_ID[ Symbol->INIT  ] );
  fprintf( stdout, "\n\t\tEFFEC : '%c'", VEX_LITERAL_BY_ID[ Symbol->EFFEC ] );
  fprintf( stdout, "\n\t\tDRIVE : '%c'", VEX_LITERAL_BY_ID[ Symbol->DRIVE ] );
  fprintf( stdout, "\n\t\tEVENT : %d"  , Symbol->EVENT );

  fprintf( stdout, "\n\t\tFLAGS : %lx", Symbol->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Symbol->USER );
  fprintf( stdout, "\n<-- Symbol" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn View Declaration                 |
|                                                             |
\------------------------------------------------------------*/

void viewvpndecl( Decl )

  vpndecl_list *Decl;
{
  vpnsym *DeclSym;
  int     Scan;
  int     Width;
  
  fprintf( stdout, "\n--> Declaration" );

  viewvpnline( Decl->LINE );

  if ( *Decl->PREV != Decl )
  {
    fprintf( stdout, "\n\t\tPREV     : ERROR" );
  }

  fprintf( stdout, "\n\t\tVEX_ATOM : " );
  viewvexexprbound( Decl->VEX_ATOM );

  fprintf( stdout, "\n\t\tVEX_INIT : " );
  viewvexexprbound( Decl->VEX_INIT );

  fprintf( stdout, "\n\t\tDECL_SYM :\n" );

  DeclSym = Decl->DECL_SYM;
  Width   = Decl->VEX_ATOM->WIDTH;

  for ( Scan = 0; Scan < Width; Scan++ )
  {
    viewvpnsym( &DeclSym[ Scan ] );
  }

  if ( Decl->DIR >= VPN_MAX_DIR_TYPE )
  {
    fprintf( stdout, "\n\t\tDIR   : %d", (int)Decl->DIR  );
  }
  else
  {
    fprintf( stdout, "\n\t\tDIR   : %s", VPN_DIR_TYPE[ Decl->DIR ] );
  }

  if ( Decl->TYPE >= VPN_MAX_DECLAR_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE  : %d", Decl->TYPE  );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE  : %s", VPN_DECLAR_TYPE[ Decl->TYPE ] );
  }

  if ( Decl->BASE >= VEX_MAX_TYPE )
  {
    fprintf( stdout, "\n\t\tBASE  : %d", Decl->BASE );
  }
  else
  {
    fprintf( stdout, "\n\t\tBASE  : %s", VEX_TYPE_NAME[ Decl->BASE ] );
  }

  if ( Decl->KIND >= VPN_MAX_KIND_TYPE )
  {
    fprintf( stdout, "\n\t\tKIND  : %d", Decl->KIND );
  }
  else
  {
    fprintf( stdout, "\n\t\tKIND  : %s", VPN_KIND_TYPE[ Decl->KIND ] );
  }

  if ( Decl->CLASS >= VPN_MAX_CLASS_TYPE )
  {
    fprintf( stdout, "\n\t\tCLASS : %d", Decl->CLASS );
  }
  else
  {
    fprintf( stdout, "\n\t\tCLASS : %s", VPN_CLASS_TYPE[ Decl->CLASS ] );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Decl->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Decl->USER );
  fprintf( stdout, "\n<-- Declaration" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn View Action                         |
|                                                             |
\------------------------------------------------------------*/

void viewvpnact( Action )

  vpnact_list *Action;
{
  fprintf( stdout, "\n--> Action" );

  viewvpnline( Action->LINE );

  if ( *Action->PREV != Action )
  {
    fprintf( stdout, "\n\t\tPREV     : ERROR" );
  }

  fprintf( stdout, "\n\t\tVEX_ATOM : " );

  viewvexexprbound( Action->VEX_ATOM );

  fprintf( stdout, "\n\t\tVEX_EXPR : \n" );

  viewvexexprbound( Action->VEX_EXPR );

  if ( Action->TYPE >= VPN_MAX_ACT_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE     : %d", Action->TYPE  );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE     : %s", VPN_ACT_TYPE[ Action->TYPE ] );
  }

  fprintf( stdout, "\n\t\tFLAGS    : %lx", Action->FLAGS );
  fprintf( stdout, "\n\t\tUSER     : %lx", (long)Action->USER );
  fprintf( stdout, "\n<-- Action" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn View Transition                     |
|                                                             |
\------------------------------------------------------------*/

void viewvpntrans( Trans )

  vpntrans_list *Trans;
{
  chain_list    *ScanChain;
  vpnact_list   *ScanAsg;
  vpnplace_list *ScanPlace;
  vpnarc        *ScanArc;

  fprintf( stdout, "\n--> Transition" );

  if ( *Trans->PREV != Trans )
  {
    fprintf( stdout, "\n\t\tPREV       : ERROR" );
  }

  fprintf( stdout, "\n\t\tNAME       : %s", Trans->NAME );

  if ( Trans->FATHER < VPN_MAX_FATHER_TYPE )
  {
    fprintf( stdout, "\n\t\tFATHER     : %s", VPN_FATHER_TYPE[ Trans->FATHER ] );
  }
  else
  {
    fprintf( stdout, "\n\t\tFATHER     : %d", Trans->FATHER );
  }

  if ( Trans->TYPE >= VPN_MAX_TRANS_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE       : %d", Trans->TYPE );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE       : %s", VPN_TRANS_TYPE[ Trans->TYPE ] );
  }

  for ( ScanChain  = Trans->WAIT_SYM;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    fprintf( stdout, "\n\t\tWAIT_SYM   : %s\n", ((vpnsym *)ScanChain->DATA)->NAME );
  }

  fprintf( stdout, "\n\t\tACT        :\n" );

  for ( ScanAsg  = Trans->ACT;
        ScanAsg != (vpnact_list *)0;
        ScanAsg  = ScanAsg->NEXT )
  {
    viewvpnact( ScanAsg );
  }

  fprintf( stdout, "\n\t\tVEX_GUARD  : " );

  viewvexexprbound( Trans->VEX_GUARD );

  fprintf( stdout, "\n" );

  fprintf( stdout, "\n\t\tNUMBER_IN  : %d", Trans->NUMBER_IN );

  for ( ScanChain  = Trans->PLACE_IN;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanArc   = (vpnarc        *)ScanChain->DATA;
    ScanPlace = (vpnplace_list *)ScanArc->SOURCE;
    fprintf( stdout, "\n\t\tPLACE_IN   : %s", ScanPlace->NAME );
  }

  fprintf( stdout, "\n\t\tNUMBER_OUT : %d", Trans->NUMBER_OUT );

  for ( ScanChain  = Trans->PLACE_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanArc   = (vpnarc        *)ScanChain->DATA;
    ScanPlace = (vpnplace_list *)ScanArc->TARGET;
    fprintf( stdout, "\n\t\tPLACE_OUT  : %s", ScanPlace->NAME );
  }

  fprintf( stdout, "\n\t\tFLAGS      : %lx", Trans->FLAGS );
  fprintf( stdout, "\n\t\tUSER       : %lx", (long)Trans->USER );

  fprintf( stdout, "\n<-- Transition" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn View Place                          |
|                                                             |
\------------------------------------------------------------*/

void viewvpnplace( Place )

  vpnplace_list *Place;
{
  chain_list    *ScanChain;
  vpntrans_list *ScanTrans;
  vpnarc        *ScanArc;

  fprintf( stdout, "\n--> Place " );

  viewvpnline( Place->LINE );

  if ( *Place->PREV != Place )
  {
    fprintf( stdout, "\n\t\tPREV       : ERROR" );
  }

  fprintf( stdout, "\n\t\tNAME       : %s", Place->NAME );

  if ( Place->FATHER < VPN_MAX_FATHER_TYPE )
  {
    fprintf( stdout, "\n\t\tFATHER     : %s", VPN_FATHER_TYPE[ Place->FATHER ] );
  }
  else
  {
    fprintf( stdout, "\n\t\tFATHER     : %d", Place->FATHER );
  }

  if ( Place->TYPE >= VPN_MAX_PLACE_TYPE )
  {
    fprintf( stdout, "\n\t\tTYPE       : %d", Place->TYPE );
  }
  else
  {
    fprintf( stdout, "\n\t\tTYPE       : %s", VPN_PLACE_TYPE[ Place->TYPE ] );
  }

  if ( Place->LINK != (vpnplace_list *)0 )
  {
    fprintf( stdout, "\n\t\tLINK       : %s", Place->LINK->NAME );
  }

  fprintf( stdout, "\n\t\tTOKEN      : %d", Place->TOKEN );
  fprintf( stdout, "\n\t\tNUMBER_IN  : %d", Place->NUMBER_IN );

  for ( ScanChain  = Place->TRANS_IN;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanArc   = (vpnarc        *)ScanChain->DATA;
    ScanTrans = (vpntrans_list *)ScanArc->SOURCE;
    fprintf( stdout, "\n\t\tTRANS_IN   : %s", ScanTrans->NAME );
  }

  fprintf( stdout, "\n\t\tNUMBER_OUT : %d", Place->NUMBER_OUT );

  for ( ScanChain  = Place->TRANS_OUT;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanArc   = (vpnarc        *)ScanChain->DATA;
    ScanTrans = (vpntrans_list *)ScanArc->TARGET;
    fprintf( stdout, "\n\t\tTRANS_OUT  : %s", ScanTrans->NAME );
  }

  fprintf( stdout, "\n\t\tFLAGS      : %lx", Place->FLAGS );
  fprintf( stdout, "\n\t\tUSER       : %lx", (long)Place->USER );
  fprintf( stdout, "\n<-- Place" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn View Function                       |
|                                                             |
\------------------------------------------------------------*/

void viewvpnfunc( Function )

  vpnfunc_list *Function;
{
  vpnplace_list *ScanPlace;
  vpntrans_list *ScanTrans;
  chain_list    *ScanChain;

  fprintf( stdout, "\n--> Function" );

  viewvpnline( Function->LINE );

  fprintf( stdout, "\n\t\tNAME  : %s", Function->NAME );
  fprintf( stdout, "\n\t\tTYPE  : %x", (int)Function->TYPE );

  if ( Function->VEX_RET != (vexexpr *)0 )
  {
    fprintf( stdout, "\n\t\tVEX_RET :\n" );
    viewvexexprbound( Function->VEX_RET );
  }

  fprintf( stdout, "\n\t\tARG_DECL :\n" );

  for ( ScanChain  = Function->ARG_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewvpndecl( (vpndecl_list *)ScanChain->DATA );
  }

  fprintf( stdout, "\n\t\tVAR_DECL  :\n" );

  for ( ScanChain  = Function->VAR_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewvpndecl( (vpndecl_list *)ScanChain->DATA );
  }

  fprintf( stdout, "\n\t\tDEF_DECL  :\n" );

  for ( ScanChain  = Function->DEF_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewvpndecl( (vpndecl_list *)ScanChain->DATA );
  }

  if ( Function->ELABO != (vpntrans_list *)0 )
  {
    fprintf( stdout, "\n\t\tELABO : %s", Function->ELABO->NAME );
  }
  
  if ( Function->FIRST != (vpntrans_list *)0 )
  {
    fprintf( stdout, "\n\t\tFIRST : %s", Function->FIRST->NAME );
  }
  
  fprintf( stdout, "\n\t\tPLACE :" );

  for ( ScanPlace  = Function->PLACE;
        ScanPlace != (vpnplace_list *)0;
        ScanPlace  = ScanPlace->NEXT )
  {
    viewvpnplace( ScanPlace );
  }

  fprintf( stdout, "\n\t\tTRANS :" );

  for ( ScanTrans  = Function->TRANS;
        ScanTrans != (vpntrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    viewvpntrans( ScanTrans );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Function->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Function->USER );
  fprintf( stdout, "\n<-- Function" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn View Process                        |
|                                                             |
\------------------------------------------------------------*/

void viewvpnproc( Process )

  vpnproc_list *Process;
{
  vpnplace_list *ScanPlace;
  vpntrans_list *ScanTrans;
  chain_list    *ScanChain;

  fprintf( stdout, "\n--> Process" );

  viewvpnline( Process->LINE );

  fprintf( stdout, "\n\t\tNAME  : %s", Process->NAME );
  fprintf( stdout, "\n\t\tTYPE  : %x", (int)Process->TYPE );

  fprintf( stdout, "\n\t\tVAR_DECL  :\n" );

  for ( ScanChain  = Process->VAR_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewvpndecl( (vpndecl_list *)ScanChain->DATA );
  }

  fprintf( stdout, "\n\t\tDEF_DECL  :\n" );

  for ( ScanChain  = Process->DEF_DECL;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    viewvpndecl( (vpndecl_list *)ScanChain->DATA );
  }

  if ( Process->ELABO != (vpntrans_list *)0 )
  {
    fprintf( stdout, "\n\t\tELABO : %s", Process->ELABO->NAME );
  }
  
  if ( Process->FIRST != (vpntrans_list *)0 )
  {
    fprintf( stdout, "\n\t\tFIRST : %s", Process->FIRST->NAME );
  }
  
  fprintf( stdout, "\n\t\tPLACE :" );

  for ( ScanPlace  = Process->PLACE;
        ScanPlace != (vpnplace_list *)0;
        ScanPlace  = ScanPlace->NEXT )
  {
    viewvpnplace( ScanPlace );
  }

  fprintf( stdout, "\n\t\tTRANS :" );

  for ( ScanTrans  = Process->TRANS;
        ScanTrans != (vpntrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    viewvpntrans( ScanTrans );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Process->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Process->USER );
  fprintf( stdout, "\n<-- Process" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn View Model                          |
|                                                             |
\------------------------------------------------------------*/

void viewvpnmod( Model )

  vpnmod_list *Model;
{
  vpnport_list *ScanPort;
  vpngen_list  *ScanGen;

  fprintf( stdout, "\n--> Model" );

  viewvpnline( Model->LINE );

  fprintf( stdout, "\n\t\tNAME  : %s", Model->NAME );

  fprintf( stdout, "\n\t\tPORT  :" );

  for ( ScanPort  = Model->PORT;
        ScanPort != (vpnport_list *)0;
        ScanPort  = ScanPort->NEXT )
  {
    viewvpnport( ScanPort );
  }

  fprintf( stdout, "\n\t\tGENERIC:" );

  for ( ScanGen  = Model->GENERIC;
        ScanGen != (vpngen_list *)0;
        ScanGen  = ScanGen->NEXT )
  {
    viewvpngen( ScanGen );
  }


  fprintf( stdout, "\n\t\tFLAGS : %lx", Model->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Model->USER );
  fprintf( stdout, "\n<-- Model" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Vpn View Instance                       |
|                                                             |
\------------------------------------------------------------*/

void viewvpnins( Instance )

  vpnins_list *Instance;
{
  vpnmap_list *ScanMap;
  vpngen_list *ScanGeneric;

  fprintf( stdout, "\n--> Instance" );

  viewvpnline( Instance->LINE );

  fprintf( stdout, "\n\t\tNAME    : %s", Instance->NAME );
  fprintf( stdout, "\n\t\tMODEL   : %s", Instance->MODEL->NAME );

  fprintf( stdout, "\n\t\tMAP     :" );

  for ( ScanMap  = Instance->MAP;
        ScanMap != (vpnmap_list *)0;
        ScanMap  = ScanMap->NEXT )
  {
    viewvpnmap( ScanMap );
  }

  fprintf( stdout, "\n\t\tGENERIC :" );

  for ( ScanGeneric  = Instance->GENERIC;
        ScanGeneric != (vpngen_list *)0;
        ScanGeneric  = ScanGeneric->NEXT )
  {
    viewvpngen( ScanGeneric );
  }

  fprintf( stdout, "\n\t\tFLAGS : %lx", Instance->FLAGS );
  fprintf( stdout, "\n\t\tUSER  : %lx", (long)Instance->USER );
  fprintf( stdout, "\n<-- Model" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Vpn View Figure                      |
|                                                             |
\------------------------------------------------------------*/

void viewvpnfig( Figure )

  vpnfig_list *Figure;
{
  vpnmod_list   *Model;
  vpnins_list   *Instance;
  vpnproc_list  *Process;
  vpnfunc_list  *Function;
  vpndecl_list  *Decl;
  vpnattr_list  *Attr;
  int            Type;

  fprintf( stdout, "\n--> Figure" );
  fprintf( stdout, "\n\tNAME            : %s", Figure->NAME  );

  for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
  {
    fprintf( stdout, "\n\tDECLARATION %s : ", VPN_DECLAR_TYPE[ Type ]  );

    for ( Decl  = Figure->DECLAR[ Type ];
          Decl != (vpndecl_list *)0;
          Decl  = Decl->NEXT )
    {
      viewvpndecl( Decl );
    }
  }

  fprintf( stdout, "\n\tATTRIBUTE :" );

  for ( Attr  = Figure->ATTRIBUTE;
        Attr != (vpnattr_list *)0;
        Attr  = Attr->NEXT )
  {
    viewvpnattr( Attr );
  }

  fprintf( stdout, "\n\tMODEL     :" );

  for ( Model  = Figure->MODEL;
        Model != (vpnmod_list *)0;
        Model  = Model->NEXT )
  {
    viewvpnmod( Model );
  }

  fprintf( stdout, "\n\tINSTANCE  :" );

  for ( Instance  = Figure->INSTANCE;
        Instance != (vpnins_list *)0;
        Instance  = Instance->NEXT )
  {
    viewvpnins( Instance );
  }

  fprintf( stdout, "\n\tPROCESS : " );

  for ( Process  = Figure->PROCESS;
        Process != (vpnproc_list *)0;
        Process  = Process->NEXT )
  {
    viewvpnproc( Process );
  }

  fprintf( stdout, "\n\tFUNCTION : " );

  for ( Function  = Figure->FUNCTION;
        Function != (vpnfunc_list *)0;
        Function  = Function->NEXT )
  {
    viewvpnfunc( Function );
  }

  fprintf( stdout, "\n\tFLAGS : %lx", Figure->FLAGS );
  fprintf( stdout, "\n\tUSER  : %lx", (long)Figure->USER );
  fprintf( stdout, "\n<-- Figure\n" );
}
