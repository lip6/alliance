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
| Tool    :                     VVH                           |
|                                                             |
| File    :                  vvh_vbh2vpn.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.11.94                        |
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
# include "vbh.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "vvh_error.h"
# include "vvh_vbh2vpn.h"

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

         vpnfig_list   *VvhVpnFigure     = (vpnfig_list   *)0;
  static vbfig_list    *VvhVbhFigure     = (vbfig_list    *)0;
  static vpntrans_list *VvhPrevTrans     = (vpntrans_list *)0;
  static vpnplace_list *VvhFirstPlace    = (vpnplace_list *)0;
  static vpnplace_list *VvhBeginLoop     = (vpnplace_list *)0;
  static vpnplace_list *VvhEndLoop       = (vpnplace_list *)0;
  static vpnproc_list  *VvhProcess       = (vpnproc_list  *)0;
  static vpnfunc_list  *VvhFunction      = (vpnfunc_list  *)0;
  static authtable     *VvhHashVarName   = (authtable     *)0;
  static authtable     *VvhHashEndLoop   = (authtable     *)0;
  static authtable     *VvhHashBeginLoop = (authtable     *)0;
  static char          *VvhTopName       = (char          *)0;
  static long           VvhCurrentLine   = 0;
  static long           VvhNumberProc    = 0;
  static long           VvhNumberFunc    = 0;
  static long           VvhNumberAsg     = 0;
  static long           VvhNumberIfs     = 0;
  static long           VvhNumberCase    = 0;
  static long           VvhNumberWait    = 0;
  static long           VvhNumberWhile   = 0;
  static long           VvhNumberLoop    = 0;
  static long           VvhNumberFor     = 0;
  static long           VvhNumberNext    = 0;
  static long           VvhNumberExit    = 0;
  static long           VvhNumberCall    = 0;
  static long           VvhNumberReturn  = 0;
  static char          *VvhCaseOthers    = (char *)0;

  static char           VvhName[ 512 ];

  static int            VvhReduced = 0;
  static int            VvhAggregate = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        VvhVpnGetBaseType                    |
|                                                             |
\------------------------------------------------------------*/

static unsigned char VvhVpnGetBaseType( VbhType )

  vbtyp_list *VbhType;
{
  unsigned char  Base;

  Base = 0;

  if ( VbhType->INDEX < VBH_MAX_TYPE )
  {
    Base = VbhType->INDEX;
  }
  else
  if ( VbhType->CLASS == 'S' )
  {
    Base = VvhVpnGetBaseType( VbhType->BASE );
  }
  else
  if ( VbhType->CLASS == 'E' )
  {
    Base = VEX_TYPE_ENUMERATE;
  }
  else
  if ( VbhType->CLASS == 'A' )
  {
    if ( VbhType->BASE->CLASS == 'A' )
    {
      Base = VEX_TYPE_ARRAY;
    }
    else
    {
      Base = VvhVpnGetBaseType( VbhType->BASE );
    }
  }
  else
  if ( VbhType->CLASS == 'U' )
  {
    Base = VEX_TYPE_ARRAY;
  }
  else
  {
    vvherror( VVH_ERROR_BAD_TYPE, VbhType->NAME, VvhVbhFigure->NAME );
  }

  return( Base );
}

/*------------------------------------------------------------\
|                                                             |
|                        VvhVpnAddDefine                      |
|                                                             |
\------------------------------------------------------------*/

static vpndecl_list *VvhVpnAddDefine( Width, DefBase )

  unsigned short Width;
  unsigned char  DefBase;
{
  vpndecl_list  *DefDeclar;
  vexexpr       *DefAtom;
  char          *FatherName;
  long           Number;

  Number = GetVpnNumDeclDef( VvhVpnFigure );

  if ( VvhProcess != (vpnproc_list *)0 ) FatherName = VvhProcess->NAME;
  else                                   FatherName = VvhFunction->NAME;

  sprintf( VvhName, "%s_def_%ld", FatherName, Number );

  if ( Width > 1 ) DefAtom = createvexatomvec( VvhName, Width - 1, 0 );
  else             DefAtom = createvexatombit( VvhName );

  if ( VvhProcess != (vpnproc_list *)0 )
  {
    DefDeclar = addvpndeclprocdef( VvhVpnFigure, VvhProcess, DefAtom, DefBase );
  }
  else
  {
    DefDeclar = addvpndeclfuncdef( VvhVpnFigure, VvhFunction, DefAtom, DefBase );
  }

  addvpnline( VvhVpnFigure, &DefDeclar->LINE, VvhCurrentLine );

  return( DefDeclar );
}

/*------------------------------------------------------------\
|                                                             |
|                        VvhVpnAddPlace                       |
|                                                             |
\------------------------------------------------------------*/

static vpnplace_list *VvhVpnAddPlace( Name )

  char *Name;
{
  vpnplace_list *VpnPlace;

  if ( VvhProcess != (vpnproc_list *)0 )
  {
    VpnPlace = addvpnprocplace( VvhVpnFigure, VvhProcess, Name );
  }
  else
  {
    VpnPlace = addvpnfuncplace( VvhVpnFigure, VvhFunction, Name );
  }

  addvpnline( VvhVpnFigure, &VpnPlace->LINE, VvhCurrentLine );

  return( VpnPlace );
}

/*------------------------------------------------------------\
|                                                             |
|                        VvhVpnAddTrans                       |
|                                                             |
\------------------------------------------------------------*/

static vpntrans_list *VvhVpnAddTrans( Name )

  char *Name;
{
  if ( VvhProcess != (vpnproc_list *)0 )
  {
    return( addvpnproctrans( VvhVpnFigure, VvhProcess, Name ) );
  }

  return( addvpnfunctrans( VvhVpnFigure, VvhFunction, Name ) );
}
 
/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatVarName                    |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatVarName( Expr )

  vexexpr *Expr;
{
  authelem *Element;

  Element = searchauthelem( VvhHashVarName, Expr->VALUE );

  if ( Element != (authelem *)0 )
  {
    Expr->VALUE = (void *)Element->VALUE;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhSubstituteVar                   |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhSubstituteVar( Expr )

  vexexpr *Expr;
{
  chain_list *ScanOper;

  if ( IsVexNodeAtom( Expr ) )
  {
    VvhVbhTreatVarName( Expr );
  }
  else
  {
    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      VvhVbhSubstituteVar( GetVexOperand( ScanOper ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhDuplicateExpr                   |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VvhVbhDuplicateExpr( Expr )

  vexexpr *Expr;
{
  vexexpr     *ExprDup;
  chain_list  *ScanOper;
  chain_list  *DupOper;
  chain_list **PrevOper;

  ExprDup = dupvexnode( Expr );

  if ( ! IsVexNodeAtom( Expr ) )
  {
    PrevOper = &ExprDup->OPERAND;

    for ( ScanOper  = Expr->OPERAND;
          ScanOper != (chain_list *)0;
          ScanOper  = ScanOper->NEXT )
    {
      Expr = VvhVbhDuplicateExpr( GetVexOperand( ScanOper ) );

      DupOper = addchain( (chain_list *)0, (void *)Expr );
      *PrevOper = DupOper;
      PrevOper  = &DupOper->NEXT;
    }
  }
  else
  {
    VvhVbhTreatVarName( ExprDup );
  }

  return( ExprDup );
}

/*------------------------------------------------------------\
|                                                             |
|                        VvhVpnGetVexDeclar                   |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VvhVpnGetVexDeclar( VexAtom, Base, VbhType )

  vexexpr       *VexAtom;
  unsigned char  Base;
  vbtyp_list    *VbhType;
{
  char       *Name;
  vexexpr    *VexDecl;
  vexexpr    *VexArray;
  short       Left;
  short       Right;

  if ( Base != VEX_TYPE_ARRAY )
  {
    VexDecl = VvhVbhDuplicateExpr( VexAtom );
  }
  else
  {
    VexArray = VvhVbhDuplicateExpr( VexAtom );
    Name     = GetVexAtomValue( VexArray );

    VexDecl  = createvexoper( VEX_ARRAY, VexArray->WIDTH );
    addvexhexpr( VexDecl, VexArray );

    VbhType = VbhType->BASE;

    while ( ( VbhType        != (vbtyp_list *)0 ) &&
            ( ( VbhType->INDEX >= VBH_MAX_TYPE     ) ||
              ( VbhType->INDEX == VBH_TYPE_INTEGER ) ||
              ( VbhType->INDEX == VBH_TYPE_NATURAL ) ) )
    {
      if ( VbhType->INDEX == VBH_TYPE_NATURAL )
      {
        Left = 30; Right = 0;
      }
      else
      if ( VbhType->INDEX == VBH_TYPE_INTEGER )
      {
        Left = 31; Right = 0;
      }
      else
      {
        Left  = VbhType->LEFT; Right = VbhType->RIGHT;
      }

      VexArray = createvexatomvec( Name, Left, Right );
      addvexqexpr( VexDecl, VexArray );

      VbhType = VbhType->BASE;
    }
  }

  return( VexDecl );
}


/*------------------------------------------------------------\
|                                                             |
|                     VvhVbhGetTargetName                     |
|                                                             |
\------------------------------------------------------------*/

static char *VvhVbhGetTargetName( Expr )

  vexexpr *Expr;
{
  if ( IsVexNodeAtom( Expr ) )
  {
    return( GetVexAtomValue( Expr ) );
  }
  else
  if ( IsVexNodeOper( Expr ) )
  {
    return( VvhVbhGetTargetName( GetVexOperand( Expr->OPERAND ) ) );
  }

  return( (char *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatInitial                    |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatInitial( VpnDeclar, Init )

  vpndecl_list *VpnDeclar;
  vexexpr      *Init;
{
  if ( Init != (vexexpr *)0 )
  {
    VpnDeclar->VEX_INIT = dupvexexpr( Init );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatDefine                     |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VvhVbhTreatDefine( Expr, Base )

  vexexpr       *Expr;
  unsigned char  Base;
{
  vpnplace_list *AssignPlace;
  vpntrans_list *AssignTrans;
  vpnact_list   *VpnAction;
  vpnarc        *VpnArc;
  vpndecl_list  *DefDeclar;
  vexexpr       *DefAtom;
  long           Number;
  int            NewOne;

  DefDeclar = VvhVpnAddDefine( Expr->WIDTH, Base );
  DefAtom   = dupvexnode( DefDeclar->VEX_ATOM );

  Number = VvhNumberAsg++;
  Expr   = VvhVbhDuplicateExpr( Expr );

  NewOne = 1;

  if ( VvhPrevTrans->TYPE == VPN_TRANS_ACT_EXEC )
  {
    VpnArc      = GetVpnArc( VvhPrevTrans->PLACE_IN );
    AssignPlace = GetVpnArcSourcePlace( VpnArc );

    if ( AssignPlace->TYPE == VPN_PLACE_ASSIGN ) NewOne = ! VvhReduced;
  }

  if ( NewOne )
  {
    sprintf( VvhName, "asg_%s.%s.%ld", VvhVbhGetTargetName( DefAtom ), VvhTopName, Number );
    AssignPlace = VvhVpnAddPlace( VvhName );
    AssignPlace->TYPE = VPN_PLACE_ASSIGN;
    AssignTrans = VvhVpnAddTrans( VvhName );
    AssignTrans->TYPE = VPN_TRANS_ACT_EXEC;

    VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, AssignPlace );
    VpnArc = addvpnarcplace( VvhVpnFigure, AssignPlace , AssignTrans );

    VvhPrevTrans = AssignTrans;
  }

  VpnAction = addvpnactasg( VvhVpnFigure, VvhPrevTrans, DefAtom, Expr );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );

  return( dupvexnode( DefAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatTest                       |
|                                                             |
\------------------------------------------------------------*/

static vexexpr *VvhVbhTreatTest( VpnTrans, VexCond )

  vpntrans_list *VpnTrans;
  vexexpr       *VexCond;
{
  vpndecl_list  *DefDeclar;
  vpnact_list   *VpnAction;
  vexexpr       *DefAtom;

  DefDeclar = VvhVpnAddDefine( VexCond->WIDTH, VEX_TYPE_BOOLEAN );
  DefAtom   = dupvexnode( DefDeclar->VEX_ATOM );
  VexCond   = VvhVbhDuplicateExpr( VexCond );

  VpnAction = addvpnactasg( VvhVpnFigure, VpnTrans, DefAtom, VexCond );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );

  return( dupvexnode( DefAtom ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatPort                       |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatPort()
{
  vbpor_list    *ScanPort;
  vpndecl_list  *VpnDeclar;
  vexexpr       *Expr;
  unsigned char  Base;
  unsigned char  Dir;
  unsigned char  Kind;

  for ( ScanPort  = VvhVbhFigure->BEPOR; 
        ScanPort != (vbpor_list *)0;
        ScanPort  = ScanPort->NEXT )
  {
    VvhCurrentLine = ScanPort->LINE;

    switch ( ScanPort->DIR )
    {
      case 'I' : Dir = VPN_DIR_IN; break;
      case 'O' : Dir = VPN_DIR_OUT; break;
      default  : Dir = VPN_DIR_INOUT; break;
    }

    switch ( ScanPort->KIND )
    {
      case VBH_KIND_BUS      : Kind = VPN_KIND_BUS;      break;
      case VBH_KIND_REGISTER : Kind = VPN_KIND_REGISTER; break;
      default                : Kind = VPN_KIND_NONE;     break;
    }

    Base = VvhVpnGetBaseType( ScanPort->TYPE );
    Expr = VvhVpnGetVexDeclar( ScanPort->TARGET, Base, ScanPort->TYPE );

    VpnDeclar = addvpndeclport( VvhVpnFigure, Expr, Base, Dir, Kind );
    addvpnline( VvhVpnFigure, &VpnDeclar->LINE, VvhCurrentLine );

    VvhVbhTreatInitial( VpnDeclar, ScanPort->VEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatConstant                   |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatConstant()
{
  vbcst_list    *ScanCst;
  vpndecl_list  *VpnDeclar;
  vexexpr       *Expr;
  unsigned char  Base;

  for ( ScanCst  = VvhVbhFigure->BECST;
        ScanCst != (vbcst_list *)0;
        ScanCst  = ScanCst->NEXT )
  {
    VvhCurrentLine = ScanCst->LINE;

    Base = VvhVpnGetBaseType( ScanCst->TYPE );
    Expr = VvhVpnGetVexDeclar( ScanCst->TARGET, Base, ScanCst->TYPE );

    VpnDeclar = addvpndeclcst( VvhVpnFigure, Expr, Base );
    addvpnline( VvhVpnFigure, &VpnDeclar->LINE, VvhCurrentLine );

    VvhVbhTreatInitial( VpnDeclar, ScanCst->VEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatGeneric                    |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatGeneric()
{
  vbgen_list    *ScanGen;
  vpndecl_list  *VpnDeclar;
  vexexpr       *Expr;
  unsigned char  Base;

  for ( ScanGen  = VvhVbhFigure->BEGEN;
        ScanGen != (vbgen_list *)0;
        ScanGen  = ScanGen->NEXT )
  {
    VvhCurrentLine = ScanGen->LINE;

    Base = VvhVpnGetBaseType( ScanGen->TYPE );
    Expr = VvhVpnGetVexDeclar( ScanGen->TARGET, Base, ScanGen->TYPE );

    VpnDeclar = addvpndeclgen( VvhVpnFigure, Expr, Base );
    addvpnline( VvhVpnFigure, &VpnDeclar->LINE, VvhCurrentLine );

    VvhVbhTreatInitial( VpnDeclar, ScanGen->VEX );
  }
}


/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatAuxiliary                  |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatAuxiliary()
{
  vbaux_list    *ScanAux;
  vpndecl_list  *VpnDeclar;
  vexexpr       *Expr;
  unsigned char  Base;
  unsigned char  Kind;

  for ( ScanAux  = VvhVbhFigure->BEAUX;
        ScanAux != (vbaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    VvhCurrentLine = ScanAux->LINE;

    switch ( ScanAux->KIND )
    {
      case VBH_KIND_BUS      : Kind = VPN_KIND_BUS;      break;
      case VBH_KIND_REGISTER : Kind = VPN_KIND_REGISTER; break;
      default                : Kind = VPN_KIND_NONE;     break;
    }

    Base = VvhVpnGetBaseType( ScanAux->TYPE );
    Expr = VvhVpnGetVexDeclar( ScanAux->TARGET, Base, ScanAux->TYPE );

    VpnDeclar = addvpndeclsig( VvhVpnFigure, Expr, Base, Kind );
    addvpnline( VvhVpnFigure, &VpnDeclar->LINE, VvhCurrentLine );

    VvhVbhTreatInitial( VpnDeclar, ScanAux->VEX );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatIfs                        |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatIfs( ScanIfs )

  vbifs_list  *ScanIfs;
{
  vpnplace_list *BeginPlace;
  vpnplace_list *EndPlace;
  vpntrans_list *TrueTrans;
  vpntrans_list *FalseTrans;
  vpntrans_list *EndTrans;
  vpnarc        *VpnArc;
  vexexpr       *VexCond;
  long           Number;

  VvhCurrentLine = ScanIfs->LINE;

  Number = VvhNumberIfs++;

  VexCond = ScanIfs->CND;

  if ( ( ! IsVexNodeAtom( VexCond ) ) || ( ! VvhReduced ) )
  {
    VexCond = VvhVbhTreatDefine( VexCond, VEX_TYPE_BOOLEAN );
  }
  else
  {
    VexCond = VvhVbhDuplicateExpr( VexCond );
  }

  sprintf( VvhName, "beg_if.%s.%ld", VvhTopName, Number );
  BeginPlace = VvhVpnAddPlace( VvhName );
  BeginPlace->TYPE = VPN_PLACE_IF;

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );

  sprintf( VvhName, "if_true.%s.%ld", VvhTopName, Number );
  TrueTrans = VvhVpnAddTrans( VvhName );
  TrueTrans->TYPE  = VPN_TRANS_GUARDED;
  TrueTrans->VEX_GUARD = dupvexexpr( VexCond );

  VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, TrueTrans );

  sprintf( VvhName, "if_false.%s.%ld", VvhTopName, Number );
  FalseTrans = VvhVpnAddTrans( VvhName );
  FalseTrans->TYPE  = VPN_TRANS_GUARDED;
  FalseTrans->VEX_GUARD = optimvexnotexpr( VexCond );

  VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, FalseTrans );

  VvhPrevTrans = TrueTrans;

  VvhVbhTreatInstruction( ScanIfs->CNDTRUE );

  TrueTrans    = VvhPrevTrans;
  VvhPrevTrans = FalseTrans;

  VvhVbhTreatInstruction( ScanIfs->CNDFALSE );

  FalseTrans = VvhPrevTrans;

  sprintf( VvhName, "end_if.%s.%ld", VvhTopName, Number );
  EndPlace = VvhVpnAddPlace( VvhName );
  EndPlace->TYPE = VPN_PLACE_END_IF;

  BeginPlace->LINK = EndPlace;

  if ( TrueTrans != (vpntrans_list *)0 )
  {
    VpnArc = addvpnarctrans( VvhVpnFigure, TrueTrans , EndPlace );
  }

  if ( FalseTrans != (vpntrans_list *)0 )
  {
    VpnArc = addvpnarctrans( VvhVpnFigure, FalseTrans, EndPlace );
  }

  EndTrans = VvhVpnAddTrans( VvhName );
  EndTrans->TYPE = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarcplace( VvhVpnFigure, EndPlace, EndTrans );

  VvhPrevTrans = EndTrans;
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatVar                        |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatVar( ScanVar )

  vbvar_list  *ScanVar;
{
  vpnplace_list *AssignPlace;
  vpntrans_list *AssignTrans;
  vpnact_list   *VpnAction;
  vpnarc        *VpnArc;
  vexexpr       *Expr;
  vexexpr       *Target;
  long           Number;
  int            NewOne;

  VvhCurrentLine = ScanVar->LINE;

  Number  = VvhNumberAsg++;
  Target  = VvhVbhDuplicateExpr( ScanVar->TARGET );
  Expr    = VvhVbhDuplicateExpr( ScanVar->VEX    );

  if ( ! VvhAggregate )
  {
    NewOne = 1;
  
    if ( VvhPrevTrans->TYPE == VPN_TRANS_ACT_EXEC )
    {
      VpnArc      = GetVpnArc( VvhPrevTrans->PLACE_IN );
      AssignPlace = GetVpnArcSourcePlace( VpnArc );
  
      if ( AssignPlace->TYPE == VPN_PLACE_ASSIGN ) NewOne = ! VvhReduced;
    }
  
    if ( NewOne )
    {
      sprintf( VvhName, "asg_%s.%s.%ld", VvhVbhGetTargetName( Target ), VvhTopName, Number );
      AssignPlace = VvhVpnAddPlace( VvhName );
      AssignPlace->TYPE = VPN_PLACE_ASSIGN;
      AssignTrans = VvhVpnAddTrans( VvhName );
      AssignTrans->TYPE = VPN_TRANS_ACT_EXEC;
  
      VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, AssignPlace );
      VpnArc = addvpnarcplace( VvhVpnFigure, AssignPlace , AssignTrans );
  
      VvhPrevTrans = AssignTrans;
    }
  }

  VpnAction = addvpnactasg( VvhVpnFigure, VvhPrevTrans, Target, Expr );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatAsg                        |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatAsg( ScanAsg )

  vbasg_list  *ScanAsg;
{
  vpnplace_list *AssignPlace;
  vpntrans_list *AssignTrans;
  vpnact_list   *VpnAction;
  vpnarc        *VpnArc;
  vexexpr       *Target;
  vexexpr       *Expr;
  long           Number;
  int            NewOne;

  VvhCurrentLine = ScanAsg->LINE;

  Number = VvhNumberAsg++;
  Target = VvhVbhDuplicateExpr( ScanAsg->TARGET );
  Expr   = VvhVbhDuplicateExpr( ScanAsg->VEX    );

  if ( ! VvhAggregate )
  {
    NewOne = 1;
  
    if ( VvhPrevTrans->TYPE == VPN_TRANS_ACT_EXEC )
    {
      VpnArc      = GetVpnArc( VvhPrevTrans->PLACE_IN );
      AssignPlace = GetVpnArcSourcePlace( VpnArc );
  
      if ( AssignPlace->TYPE == VPN_PLACE_ASSIGN ) NewOne = ! VvhReduced;
    }
  
    if ( NewOne )
    {
      sprintf( VvhName, "asg_%s.%s.%ld", VvhVbhGetTargetName( Target ), VvhTopName, Number );
      AssignPlace = VvhVpnAddPlace( VvhName );
      AssignPlace->TYPE = VPN_PLACE_ASSIGN;
      AssignTrans = VvhVpnAddTrans( VvhName );
      AssignTrans->TYPE = VPN_TRANS_ACT_EXEC;
  
      VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, AssignPlace );
      VpnArc = addvpnarcplace( VvhVpnFigure, AssignPlace , AssignTrans );
  
      VvhPrevTrans = AssignTrans;
    }
  }

  VpnAction = addvpnactasg( VvhVpnFigure, VvhPrevTrans, Target, Expr );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatAggregate                  |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatAggregate( ScanAgr )

  vbagr_list  *ScanAgr;
{
  vpnplace_list *AssignPlace;
  vpntrans_list *AssignTrans;
  vpnarc        *VpnArc;
  long           Number;

  Number = VvhNumberAsg++;

  sprintf( VvhName, "aggr.%s.%ld", VvhTopName, Number );
  AssignPlace = VvhVpnAddPlace( VvhName );
  AssignPlace->TYPE = VPN_PLACE_ASSIGN;
  AssignTrans = VvhVpnAddTrans( VvhName );
  AssignTrans->TYPE = VPN_TRANS_ACT_EXEC_CONC;

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, AssignPlace );
  VpnArc = addvpnarcplace( VvhVpnFigure, AssignPlace , AssignTrans );

  VvhPrevTrans = AssignTrans;

  VvhAggregate = 1;
  VvhVbhTreatInstruction( ScanAgr->ASSIGN );
  VvhAggregate = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatWaitSymbol                 |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatWaitSymbol( ScanChain, WaitTrans )

  chain_list    *ScanChain;
  vpntrans_list *WaitTrans;
{
  authelem     *Element;
  vexexpr      *VexAtom;
  vpndecl_list *VpnDeclar;
  vpnsym       *VpnSymbol;
  char         *Name;
  long          Index;
  long          Pos;

  while ( ScanChain != (chain_list *)0 )
  {
    Element = searchauthelem( VvhHashVarName, ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      ScanChain->DATA = (void *)Element->VALUE;
    }

    Name = getvexvectorname( ScanChain->DATA, &Index );

    if ( Name == (char *)0 )
    {
      Index = -1; Name = (char *)ScanChain->DATA;
    }
    else
    if ( Index == -1 ) /* Vector of dynamic size ? */
    {
      Index     = -2;
      VpnDeclar = searchvpndeclall( VvhVpnFigure, Name );
      VexAtom   = VpnDeclar->VEX_ATOM;

      for ( Pos  = 0; Pos < VexAtom->WIDTH; Pos++ )
      {
        VpnSymbol = getvpnsymdeclpos( VpnDeclar, Pos );
        WaitTrans->WAIT_SYM = addchain( WaitTrans->WAIT_SYM, VpnSymbol );
      }
    }

    if ( Index != -2 )
    {
      VpnSymbol = searchvpnsymall( VvhVpnFigure, Name, Index );
      WaitTrans->WAIT_SYM = addchain( WaitTrans->WAIT_SYM, VpnSymbol );
    }

    ScanChain = ScanChain->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatCase                       |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatCase( ScanCase )

  vbcas_list *ScanCase;
{
  vbcho_list    *ScanChoice;
  chain_list    *ScanValue;
  vpnplace_list *BeginPlace;
  vpnplace_list *EndPlace;
  vpntrans_list *PrevTrans;
  vpntrans_list *CaseTrans;
  vpntrans_list *EndTrans;
  vpnarc        *VpnArc;
  vexexpr       *VexCond;
  vexexpr       *VexOthers;
  vexexpr       *VexGuard;
  vexexpr       *VexSum;
  vexexpr       *VexBoolean;
  vexexpr       *VexAtom;
  char          *ChoiceValue;
  unsigned char  Base;
  long           Number;
  long           Index;
  long           IndexStep;
  long           Offset;
  long           Scan;
  long           MaxLine;

  VvhCurrentLine = ScanCase->LINE;

  Number = VvhNumberCase++;

  if ( VvhCaseOthers == (char *)0 )
  {
    VvhCaseOthers = namealloc( "others" );
  }

  Base = VvhVpnGetBaseType( ScanCase->TYPE );

  ScanChoice = &ScanCase->CHOICE[ ScanCase->SIZE - 1 ];

  if ( ScanChoice->VALUES->DATA == (void *)VvhCaseOthers )
  {
    VexOthers = createvexatombit( VEX_ATOM_ONE );
  }
  else
  {
    VexOthers = (vexexpr *)0;
  }

  VexCond = ScanCase->VEX;

  if ( ( ! IsVexNodeAtom( VexCond ) ) || ( ! VvhReduced ) )
  {
    VexCond = VvhVbhTreatDefine( VexCond, Base );
  }
  else
  {
    VexCond = VvhVbhDuplicateExpr( VexCond );
  }

  PrevTrans = VvhPrevTrans;

  if ( IsVexAtomDown( VexCond ) ) IndexStep = -1;
  else                            IndexStep =  1;

  sprintf( VvhName, "beg_case.%s.%ld", VvhTopName, Number );
  BeginPlace = VvhVpnAddPlace( VvhName );
  BeginPlace->TYPE = VPN_PLACE_CASE;

  sprintf( VvhName, "end_case.%s.%ld", VvhTopName, Number );
  EndPlace = VvhVpnAddPlace( VvhName );
  EndPlace->TYPE = VPN_PLACE_END_CASE;

  BeginPlace->LINK = EndPlace;
  MaxLine          = VvhCurrentLine;

  for ( Offset = 0; Offset < ScanCase->SIZE; Offset++ )
  {
    ScanChoice = &ScanCase->CHOICE[ Offset ];
    VvhCurrentLine = ScanChoice->LINE;

    sprintf( VvhName, "case%ld.%s.%ld", Offset, VvhTopName, Number );
    CaseTrans = VvhVpnAddTrans( VvhName );
    CaseTrans->TYPE = VPN_TRANS_GUARDED;

    if ( ( VvhReduced                                  ) &&
         ( isvextypedivisible( ScanCase->TYPE->INDEX ) ) )
    {
      if ( ScanChoice->VALUES->DATA == (void *)VvhCaseOthers )
      {
        VexGuard = VexOthers;
  
        if ( ( ! IsVexNodeAtom( VexCond ) ) )
        {
          VvhPrevTrans = PrevTrans;
  
          VexGuard   = VvhVbhTreatDefine( VexGuard, VEX_TYPE_BIT );
          PrevTrans  = VvhPrevTrans;
        }
      }
      else
      {
/*
** Test all values
*/
        VexSum = createvexoper( VEX_OR, 1 );

        for ( ScanValue  = ScanChoice->VALUES;
              ScanValue != (chain_list *)0;
              ScanValue  = ScanValue->NEXT )
        {
          ChoiceValue = (char *)ScanValue->DATA;

          VexGuard = createvexoper( VEX_AND, 1 );
          Index = VexCond->LEFT;
    
          for ( Scan = 0; Scan < VexCond->WIDTH; Scan++ )
          {
            if ( ChoiceValue[ Scan + 1 ] == '-' )
            {
              continue;
            }
    
            if ( IsVexAtomVector( VexCond ) )
            {
              VexAtom = createvexatomvec( GetVexAtomValue( VexCond ), Index, Index );
            }
            else
            {
              VexAtom = dupvexexpr( VexCond );
            }
    
            Index  += IndexStep;
    
            if ( ChoiceValue[ Scan + 1 ] == '0' )
            {
              VexAtom = createvexunaryexpr( VEX_NOT, 1, VexAtom );
            }
    
            addvexhexpr( VexGuard, VexAtom );
          }
    
          if ( VexGuard->OPERAND->NEXT == (chain_list *)0 )
          {
            VexAtom = GetVexOperand( VexGuard->OPERAND );
            GetVexOperand( VexGuard->OPERAND ) = (vexexpr *)0;
            freevexexpr( VexGuard );
            VexGuard = VexAtom;
          }

          addvexhexpr( VexSum, VexGuard );
        }

        VexGuard = VexSum;

        if ( VexGuard->OPERAND->NEXT == (chain_list *)0 )
        {
          VexAtom = GetVexOperand( VexGuard->OPERAND );
          GetVexOperand( VexGuard->OPERAND ) = (vexexpr *)0;
          freevexexpr( VexGuard );
          VexGuard = VexAtom;
        }
    
        if ( ! IsVexNodeAtom( VexCond ) )
        {
          VvhPrevTrans = PrevTrans;
          VexGuard     = VvhVbhTreatDefine( VexGuard, VEX_TYPE_BIT );
          PrevTrans    = VvhPrevTrans;
        }
    
        if ( VexOthers != (vexexpr *)0 )
        {
          VexAtom   = optimvexnotexpr( dupvexexpr( VexGuard ) );
          VexOthers = createvexbinexpr( VEX_AND, 1, VexOthers, VexAtom );
        }
      }

      VexBoolean = createvexbinexpr( VEX_EQ, 1, VexGuard,
                                     createvexatomlit( VEX_ATOM_ONE ) );
    }
    else
    {
      if ( ScanChoice->VALUES->DATA == VvhCaseOthers )
      {
        VexBoolean = VexOthers;
      }
      else
      {
        VexSum = createvexoper( VEX_OR, 1 );

        for ( ScanValue  = ScanChoice->VALUES;
              ScanValue != (chain_list *)0;
              ScanValue  = ScanValue->NEXT )
        {
          ChoiceValue = (char *)ScanValue->DATA;

          VexGuard   = createvexatomlit( ChoiceValue );
          VexGuard   = createvexbinexpr( VEX_EQ, 1, dupvexexpr( VexCond ), VexGuard );

          addvexhexpr( VexSum, VexGuard );
        }

        if ( VexGuard->OPERAND->NEXT == (chain_list *)0 )
        {
          VexAtom = GetVexOperand( VexGuard->OPERAND );
          GetVexOperand( VexGuard->OPERAND ) = (vexexpr *)0;
          freevexexpr( VexGuard );
          VexGuard = VexAtom;
        }

        VexBoolean = dupvexexpr( VexGuard );
        VexGuard   = optimvexnotexpr( VexGuard );

        if ( VexOthers != (vexexpr *)0 )
        {
          VexOthers = createvexbinexpr( VEX_AND, 1, VexOthers, VexGuard );
        }
      }
    }

    CaseTrans->VEX_GUARD = VexBoolean;

    VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, CaseTrans );

    VvhPrevTrans = CaseTrans;

    VvhVbhTreatInstruction( ScanChoice->INSTRUCTION );

    if ( VvhCurrentLine > MaxLine ) MaxLine = VvhCurrentLine;

    if ( VvhPrevTrans != (vpntrans_list *)0 )
    {
      VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, EndPlace );
    }
  }

  freevexexpr( VexCond );

  VvhCurrentLine = MaxLine;
  addvpnline( VvhVpnFigure, &EndPlace->LINE, VvhCurrentLine );

  sprintf( VvhName, "end_case.%s.%ld", VvhTopName, Number );
  EndTrans = VvhVpnAddTrans( VvhName );
  EndTrans->TYPE = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarctrans( VvhVpnFigure, PrevTrans, BeginPlace );
  VpnArc = addvpnarcplace( VvhVpnFigure, EndPlace, EndTrans );

  VvhPrevTrans = EndTrans;
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatWait                       |
|                                                             |
\------------------------------------------------------------*/

static int VvhVbhTreatWait( ScanWait )

  vbwas_list  *ScanWait;
{
  vpnplace_list *WaitPlace;
  vpnplace_list *UntilPlace;
  vpnplace_list *UntilTest;
  vpntrans_list *WaitTrans;
  vpntrans_list *UntilTrans;
  vpntrans_list *UntilTrue;
  vpntrans_list *UntilFalse;
  vpntrans_list *UntilEnd;
  vpnarc        *VpnArc;
  chain_list    *Support;
  vexexpr       *VexCond;
  long           Number;

  VvhCurrentLine = ScanWait->LINE;

  Number = VvhNumberWait++;

  sprintf( VvhName, "wait.%s.%ld", VvhTopName, Number );

  WaitPlace = VvhVpnAddPlace( VvhName );
  WaitPlace->TYPE = VPN_PLACE_WAIT;
  WaitTrans = VvhVpnAddTrans( VvhName );

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, WaitPlace );
  VpnArc = addvpnarcplace( VvhVpnFigure, WaitPlace   , WaitTrans );

  WaitTrans->TYPE = VPN_TRANS_INF_WAIT;
  VvhPrevTrans = WaitTrans;

  VexCond = ScanWait->CND;

  if ( ScanWait->SEN != (chain_list *)0 )
  {
    VvhVbhTreatWaitSymbol( ScanWait->SEN, WaitTrans );
  }
  else
  if ( VexCond != (vexexpr *)0 )
  {
    Support = getvexexprsupport( VexCond );

    if ( Support != (chain_list *)0 )
    {
      VvhVbhTreatWaitSymbol( Support, WaitTrans );
      freechain( Support );
    }
  }

  if ( VexCond != (vexexpr *)0 )
  {
    sprintf( VvhName, "beg_until.%s.%ld", VvhTopName, Number );
    UntilPlace = VvhVpnAddPlace( VvhName );
    UntilPlace->TYPE = VPN_PLACE_UNTIL;

    sprintf( VvhName, "until_test.%s.%ld", VvhTopName, Number );
    UntilTrans = VvhVpnAddTrans( VvhName );
    UntilTrans->TYPE = VPN_TRANS_ACT_EXEC;

    VexCond = VvhVbhTreatTest( UntilTrans, VexCond );

    VpnArc = addvpnarctrans( VvhVpnFigure, WaitTrans , UntilPlace );
    VpnArc = addvpnarcplace( VvhVpnFigure, UntilPlace, UntilTrans );

    UntilTest = VvhVpnAddPlace( VvhName );
    UntilTest->TYPE = VPN_PLACE_UNTIL_TEST;

    VpnArc = addvpnarctrans( VvhVpnFigure, UntilTrans, UntilTest  );

    sprintf( VvhName, "until_true.%s.%ld", VvhTopName, Number );
    UntilTrue = VvhVpnAddTrans( VvhName );
    UntilTrue->TYPE = VPN_TRANS_GUARDED;
    UntilTrue->VEX_GUARD = dupvexexpr( VexCond );

    sprintf( VvhName, "until_false.%s.%ld", VvhTopName, Number );
    UntilFalse = VvhVpnAddTrans( VvhName );
    UntilFalse->TYPE = VPN_TRANS_GUARDED;
    UntilFalse->VEX_GUARD = optimvexnotexpr( VexCond );

    VpnArc = addvpnarcplace( VvhVpnFigure, UntilTest , UntilTrue  );
    VpnArc = addvpnarcplace( VvhVpnFigure, UntilTest , UntilFalse );
    VpnArc = addvpnarctrans( VvhVpnFigure, UntilFalse, WaitPlace  );

    sprintf( VvhName, "end_until.%s.%ld", VvhTopName, Number );
    UntilPlace = VvhVpnAddPlace( VvhName );
    UntilPlace->TYPE = VPN_PLACE_END_UNTIL;

    UntilEnd = VvhVpnAddTrans( VvhName );
    UntilEnd->TYPE = VPN_TRANS_IMMEDIATE;

    VpnArc = addvpnarctrans( VvhVpnFigure, UntilTrue, UntilPlace );
    VpnArc = addvpnarcplace( VvhVpnFigure, UntilPlace, UntilEnd  );
    
    WaitPlace->LINK = UntilPlace;

    VvhPrevTrans = UntilEnd;
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatLoop                       |
|                                                             |
\------------------------------------------------------------*/

static int VvhVbhTreatLoop( ScanLoop )

  vblop_list  *ScanLoop;
{
  vpnplace_list *BeginPlace;
  vpnplace_list *EndPlace;
  vpnplace_list *OldBeginLoop;
  vpnplace_list *OldEndLoop;
  vpntrans_list *BeginTrans;
  vpntrans_list *EndTrans;
  vpnarc        *VpnArc;
  long           Number;

  VvhCurrentLine = ScanLoop->LINE;

  Number = VvhNumberLoop++;
  
  sprintf( VvhName, "beg_loop.%s.%s.%ld", ScanLoop->LABEL, VvhTopName, Number );
  BeginPlace = VvhVpnAddPlace( VvhName );
  BeginPlace->TYPE = VPN_PLACE_LOOP;

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );

  BeginTrans = VvhVpnAddTrans( VvhName );
  BeginTrans->TYPE  = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, BeginTrans );

  sprintf( VvhName, "end_loop.%s.%s.%ld", ScanLoop->LABEL, VvhTopName, Number );
  EndPlace = VvhVpnAddPlace( VvhName );
  EndPlace->TYPE = VPN_PLACE_END_LOOP;

  BeginPlace->LINK = EndPlace;

  EndTrans = VvhVpnAddTrans( VvhName );
  EndTrans->TYPE = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarcplace( VvhVpnFigure, EndPlace, EndTrans );

  addauthelem( VvhHashEndLoop  , ScanLoop->LABEL, (long)EndPlace   );
  addauthelem( VvhHashBeginLoop, ScanLoop->LABEL, (long)BeginPlace );

  OldBeginLoop = VvhBeginLoop;
  OldEndLoop   = VvhEndLoop;
  VvhBeginLoop = BeginPlace;
  VvhEndLoop   = EndPlace;

  VvhPrevTrans = BeginTrans;

  VvhVbhTreatInstruction( ScanLoop->INSTRUCTION );

  addvpnline( VvhVpnFigure, &EndPlace->LINE, VvhCurrentLine );

  if ( VvhPrevTrans != (vpntrans_list *)0 )
  {
    VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );
  }

  delauthelem( VvhHashEndLoop  , ScanLoop->LABEL );
  delauthelem( VvhHashBeginLoop, ScanLoop->LABEL );

  VvhBeginLoop = OldBeginLoop;
  VvhEndLoop   = OldEndLoop;

  if ( EndPlace->NUMBER_IN == 0 )
  {
    BeginPlace->LINK = (vpnplace_list *)0;

    delvpnplace( VvhVpnFigure, EndPlace );
    delvpntrans( VvhVpnFigure, EndTrans );

    VvhPrevTrans = (vpntrans_list *)0;

    return( 1 );
  }

  VvhPrevTrans = EndTrans;

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatWhile                      |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatWhile( ScanWhile )

  vbwhi_list  *ScanWhile;
{
  vpnplace_list *BeginPlace;
  vpnplace_list *TestPlace;
  vpnplace_list *EndPlace;
  vpnplace_list *OldBeginLoop;
  vpnplace_list *OldEndLoop;
  vpntrans_list *TestTrans;
  vpntrans_list *TrueTrans;
  vpntrans_list *FalseTrans;
  vpntrans_list *EndTrans;
  vpnarc        *VpnArc;
  vexexpr       *VexCond;
  long           Number;

  VvhCurrentLine = ScanWhile->LINE;

  Number = VvhNumberWhile++;
  
  sprintf( VvhName, "beg_while.%s.%s.%ld", ScanWhile->LABEL, VvhTopName, Number );
  BeginPlace = VvhVpnAddPlace( VvhName );
  BeginPlace->TYPE = VPN_PLACE_WHILE;

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );

  sprintf( VvhName, "end_while.%s.%s.%ld", ScanWhile->LABEL, VvhTopName, Number );
  EndPlace = VvhVpnAddPlace( VvhName );
  EndPlace->TYPE = VPN_PLACE_END_WHILE;

  BeginPlace->LINK = EndPlace;

  EndTrans = VvhVpnAddTrans( VvhName );
  EndTrans->TYPE = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarcplace( VvhVpnFigure, EndPlace, EndTrans );

  addauthelem( VvhHashEndLoop  , ScanWhile->LABEL, (long)EndPlace   );
  addauthelem( VvhHashBeginLoop, ScanWhile->LABEL, (long)BeginPlace );

  OldBeginLoop = VvhBeginLoop;
  OldEndLoop   = VvhEndLoop;
  VvhBeginLoop = BeginPlace;
  VvhEndLoop   = EndPlace;

  VexCond = ScanWhile->CND;

  sprintf( VvhName, "while_test.%s.%s.%ld", ScanWhile->LABEL, VvhTopName, Number );
  TestTrans = VvhVpnAddTrans( VvhName );
  TestTrans->TYPE = VPN_TRANS_ACT_EXEC;

  VexCond = VvhVbhTreatTest( TestTrans, VexCond );

  TestPlace = VvhVpnAddPlace( VvhName );
  TestPlace->TYPE = VPN_PLACE_WHILE_TEST;

  VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, TestTrans );
  VpnArc = addvpnarctrans( VvhVpnFigure, TestTrans,  TestPlace );

  sprintf( VvhName, "while_true.%s.%s.%ld", ScanWhile->LABEL, VvhTopName, Number );
  TrueTrans = VvhVpnAddTrans( VvhName );
  TrueTrans->TYPE  = VPN_TRANS_GUARDED;
  TrueTrans->VEX_GUARD = dupvexexpr( VexCond );

  sprintf( VvhName, "while_false.%s.%s.%ld", ScanWhile->LABEL, VvhTopName, Number );
  FalseTrans = VvhVpnAddTrans( VvhName );
  FalseTrans->TYPE  = VPN_TRANS_GUARDED;
  FalseTrans->VEX_GUARD = optimvexnotexpr( VexCond );

  VpnArc = addvpnarcplace( VvhVpnFigure, TestPlace, TrueTrans );
  VpnArc = addvpnarcplace( VvhVpnFigure, TestPlace, FalseTrans );

  VvhPrevTrans = TrueTrans;

  VvhVbhTreatInstruction( ScanWhile->INSTRUCTION );

  if ( VvhPrevTrans != (vpntrans_list *)0 )
  {
    VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );
  }

  VpnArc = addvpnarctrans( VvhVpnFigure, FalseTrans, EndPlace );

  addvpnline( VvhVpnFigure, &EndPlace->LINE, VvhCurrentLine );

  VvhPrevTrans = EndTrans;

  delauthelem( VvhHashEndLoop  , ScanWhile->LABEL );
  delauthelem( VvhHashBeginLoop, ScanWhile->LABEL );

  VvhBeginLoop = OldBeginLoop;
  VvhEndLoop   = OldEndLoop;
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatFor                        |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatFor( ScanFor )

  vbfor_list  *ScanFor;
{
  vpnplace_list *BeginPlace;
  vpnplace_list *LoopPlace;
  vpnplace_list *TestPlace;
  vpnplace_list *EndPlace;
  vpnplace_list *IncPlace;
  vpnplace_list *OldBeginLoop;
  vpnplace_list *OldEndLoop;
  vpntrans_list *TestTrans;
  vpntrans_list *InitTrans;
  vpntrans_list *TrueTrans;
  vpntrans_list *FalseTrans;
  vpntrans_list *EndTrans;
  vpntrans_list *IncTrans;
  vpnact_list   *VpnAction;
  vpnarc        *VpnArc;
  vexexpr       *VexLeft;
  vexexpr       *VexRight;
  vexexpr       *VexVar;
  vexexpr       *VexCond;
  vexexpr       *VexInc;
  vexexpr       *VexOne;
  long           Number;

  VvhCurrentLine = ScanFor->LINE;

  Number = VvhNumberFor++;
  
  sprintf( VvhName, "beg_for.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  BeginPlace = VvhVpnAddPlace( VvhName );
  BeginPlace->TYPE = VPN_PLACE_FOR;

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );

  sprintf( VvhName, "end_for.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  EndPlace = VvhVpnAddPlace( VvhName );
  EndPlace->TYPE = VPN_PLACE_END_FOR;

  BeginPlace->LINK = EndPlace;

  EndTrans = VvhVpnAddTrans( VvhName );
  EndTrans->TYPE = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarcplace( VvhVpnFigure, EndPlace, EndTrans );

  sprintf( VvhName, "for_init.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  InitTrans = VvhVpnAddTrans( VvhName );
  InitTrans->TYPE = VPN_TRANS_ACT_EXEC;

  VexLeft  = VvhVbhDuplicateExpr( ScanFor->LEFT     );
  VexRight = VvhVbhDuplicateExpr( ScanFor->RIGHT    );
  VexVar   = VvhVbhDuplicateExpr( ScanFor->VARIABLE );

  if ( ScanFor->UP )
  {
    VexCond = createvexbinexpr( VEX_LE, 1, dupvexexpr( VexVar ), VexRight );
  }
  else
  {
    VexCond = createvexbinexpr( VEX_GE, 1, dupvexexpr( VexVar ), VexRight );
  }

  VpnAction = addvpnactasg( VvhVpnFigure, InitTrans, dupvexexpr( VexVar ), VexLeft );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );

  sprintf( VvhName, "for_loop.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  LoopPlace = VvhVpnAddPlace( VvhName );
  LoopPlace->TYPE = VPN_PLACE_FOR_LOOP;

  VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, InitTrans );
  VpnArc = addvpnarctrans( VvhVpnFigure, InitTrans,  LoopPlace );

  sprintf( VvhName, "for_test.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  TestTrans = VvhVpnAddTrans( VvhName );
  TestTrans->TYPE = VPN_TRANS_ACT_EXEC;

  VexCond = VvhVbhTreatTest( TestTrans, VexCond );

  TestPlace = VvhVpnAddPlace( VvhName );
  TestPlace->TYPE = VPN_PLACE_FOR_TEST;

  VpnArc = addvpnarcplace( VvhVpnFigure, LoopPlace, TestTrans );
  VpnArc = addvpnarctrans( VvhVpnFigure, TestTrans, TestPlace );

  sprintf( VvhName, "for_true.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  TrueTrans = VvhVpnAddTrans( VvhName );
  TrueTrans->TYPE  = VPN_TRANS_GUARDED;
  TrueTrans->VEX_GUARD = dupvexexpr( VexCond );

  sprintf( VvhName, "for_false.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  FalseTrans = VvhVpnAddTrans( VvhName );
  FalseTrans->TYPE  = VPN_TRANS_GUARDED;
  FalseTrans->VEX_GUARD = optimvexnotexpr( VexCond );

  VpnArc = addvpnarcplace( VvhVpnFigure, TestPlace, TrueTrans );
  VpnArc = addvpnarcplace( VvhVpnFigure, TestPlace, FalseTrans );

  sprintf( VvhName, "for_inc.%s.%s.%ld", ScanFor->LABEL, VvhTopName, Number );
  IncPlace = VvhVpnAddPlace( VvhName );
  IncPlace->TYPE = VPN_PLACE_FOR_INC;

  LoopPlace->LINK = IncPlace;

  IncTrans = VvhVpnAddTrans( VvhName );
  IncTrans->TYPE = VPN_TRANS_ACT_EXEC;

  VexOne = createvexatomlit( VEX_ATOM_ONE );

  if ( ScanFor->UP )
  {
    VexInc = createvexbinexpr( VEX_ADD, VexVar->WIDTH, dupvexexpr( VexVar ), VexOne );
  }
  else
  {
    VexInc = createvexbinexpr( VEX_SUB, VexVar->WIDTH, dupvexexpr( VexVar ), VexOne );
  }

  VpnAction = addvpnactasg( VvhVpnFigure, IncTrans, VexVar, VexInc );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );

  VpnArc = addvpnarcplace( VvhVpnFigure, IncPlace , IncTrans  );
  VpnArc = addvpnarctrans( VvhVpnFigure, IncTrans , LoopPlace );

  OldBeginLoop = VvhBeginLoop;
  OldEndLoop   = VvhEndLoop;
  VvhBeginLoop = IncPlace;
  VvhEndLoop   = EndPlace;

  addauthelem( VvhHashEndLoop  , ScanFor->LABEL, (long)EndPlace );
  addauthelem( VvhHashBeginLoop, ScanFor->LABEL, (long)IncPlace );

  VvhPrevTrans = TrueTrans;

  VvhVbhTreatInstruction( ScanFor->INSTRUCTION );

  TrueTrans = VvhPrevTrans;

  if ( TrueTrans != (vpntrans_list *)0 )
  {
    VpnArc = addvpnarctrans( VvhVpnFigure, TrueTrans, IncPlace  );
  }

  VpnArc = addvpnarctrans( VvhVpnFigure, FalseTrans, EndPlace );

  addvpnline( VvhVpnFigure, &EndPlace->LINE, VvhCurrentLine );

  VvhPrevTrans = EndTrans;

  delauthelem( VvhHashEndLoop  , ScanFor->LABEL );
  delauthelem( VvhHashBeginLoop, ScanFor->LABEL );

  VvhBeginLoop = OldBeginLoop;
  VvhEndLoop   = OldEndLoop;
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatNext                       |
|                                                             |
\------------------------------------------------------------*/

static int VvhVbhTreatNext( ScanNext )

  vbnxt_list  *ScanNext;
{
  authelem      *Element;
  vpnplace_list *BeginPlace;
  vpnplace_list *EndPlace;
  vpntrans_list *EndTrans;
  vpnplace_list *LoopPlace;
  vpntrans_list *TrueTrans;
  vpntrans_list *FalseTrans;
  vpnarc        *VpnArc;
  vexexpr       *VexCond;
  long           Number;

  VvhCurrentLine = ScanNext->LINE;

  Number = VvhNumberNext++;

  if ( ScanNext->LABEL != (char *)0 )
  {
    Element = searchauthelem( VvhHashBeginLoop, ScanNext->LABEL );
    LoopPlace = (vpnplace_list *)Element->VALUE;
  }
  else
  {
    LoopPlace = VvhBeginLoop;
  }

  sprintf( VvhName, "next.%s.%ld", VvhTopName, Number );
  BeginPlace = VvhVpnAddPlace( VvhName );
  BeginPlace->TYPE = VPN_PLACE_NEXT;

  VexCond = ScanNext->CND;

  if ( VexCond != (vexexpr *)0 )
  {
    if ( ( ! IsVexNodeAtom( VexCond ) ) || ( ! VvhReduced ) )
    {
      VexCond = VvhVbhTreatDefine( VexCond, VEX_TYPE_BOOLEAN );
    }
    else
    {
      VexCond = VvhVbhDuplicateExpr( VexCond );
    }
  }

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );

  if ( VexCond != (vexexpr *)0 )
  {
    sprintf( VvhName, "end_next.%s.%ld", VvhTopName, Number );
    EndPlace = VvhVpnAddPlace( VvhName );
    EndPlace->TYPE = VPN_PLACE_END_NEXT;

    BeginPlace->LINK = EndPlace;

    EndTrans = VvhVpnAddTrans( VvhName );
    EndTrans->TYPE = VPN_TRANS_IMMEDIATE;

    VpnArc = addvpnarcplace( VvhVpnFigure, EndPlace, EndTrans );

    sprintf( VvhName, "next_true.%s.%ld", VvhTopName, Number );
    TrueTrans = VvhVpnAddTrans( VvhName );
    TrueTrans->TYPE  = VPN_TRANS_GUARDED;
    TrueTrans->VEX_GUARD = dupvexexpr( VexCond );

    VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, TrueTrans );

    sprintf( VvhName, "next_false.%s.%ld", VvhTopName, Number );
    FalseTrans = VvhVpnAddTrans( VvhName );
    FalseTrans->TYPE  = VPN_TRANS_GUARDED;
    FalseTrans->VEX_GUARD = optimvexnotexpr( VexCond );

    VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, FalseTrans );
    VpnArc = addvpnarctrans( VvhVpnFigure, FalseTrans, EndPlace  );
    VpnArc = addvpnarctrans( VvhVpnFigure, TrueTrans , LoopPlace );

    VvhPrevTrans = EndTrans;

    return( 0 );
  }

  TrueTrans = VvhVpnAddTrans( VvhName );
  TrueTrans->TYPE  = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, TrueTrans );
  VpnArc = addvpnarctrans( VvhVpnFigure, TrueTrans , LoopPlace );

  VvhPrevTrans = (vpntrans_list *)0;

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatExit                       |
|                                                             |
\------------------------------------------------------------*/

static int VvhVbhTreatExit( ScanExit )

  vbext_list  *ScanExit;
{
  authelem      *Element;
  vpnplace_list *BeginPlace;
  vpnplace_list *EndPlace;
  vpntrans_list *EndTrans;
  vpnplace_list *LoopPlace;
  vpntrans_list *TrueTrans;
  vpntrans_list *FalseTrans;
  vpnarc        *VpnArc;
  vexexpr       *VexCond;
  long           Number;

  VvhCurrentLine = ScanExit->LINE;

  Number = VvhNumberExit++;

  if ( ScanExit->LABEL != (char *)0 )
  {
    Element = searchauthelem( VvhHashEndLoop, ScanExit->LABEL );
    LoopPlace = (vpnplace_list *)Element->VALUE;
  }
  else
  {
    LoopPlace = VvhEndLoop;
  }

  sprintf( VvhName, "exit.%s.%ld", VvhTopName, Number );
  BeginPlace = VvhVpnAddPlace( VvhName );
  BeginPlace->TYPE = VPN_PLACE_EXIT;

  VexCond = ScanExit->CND;

  if ( VexCond != (vexexpr *)0 )
  {
    if ( ( ! IsVexNodeAtom( VexCond ) ) || ( ! VvhReduced ) )
    {
      VexCond = VvhVbhTreatDefine( VexCond, VEX_TYPE_BOOLEAN );
    }
    else
    {
      VexCond = VvhVbhDuplicateExpr( VexCond );
    }
  }

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );

  if ( VexCond != (vexexpr *)0 )
  {
    sprintf( VvhName, "end_exit.%s.%ld", VvhTopName, Number );
    EndPlace = VvhVpnAddPlace( VvhName );
    EndPlace->TYPE = VPN_PLACE_END_EXIT;

    BeginPlace->LINK = EndPlace;

    EndTrans = VvhVpnAddTrans( VvhName );
    EndTrans->TYPE = VPN_TRANS_IMMEDIATE;

    VpnArc = addvpnarcplace( VvhVpnFigure, EndPlace, EndTrans );

    sprintf( VvhName, "exit_true.%s.%ld", VvhTopName, Number );
    TrueTrans = VvhVpnAddTrans( VvhName );
    TrueTrans->TYPE  = VPN_TRANS_GUARDED;
    TrueTrans->VEX_GUARD = dupvexexpr( VexCond );
  
    VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, TrueTrans );
  
    sprintf( VvhName, "exit_false.%s.%ld", VvhTopName, Number );
    FalseTrans = VvhVpnAddTrans( VvhName );
    FalseTrans->TYPE  = VPN_TRANS_GUARDED;
    FalseTrans->VEX_GUARD = optimvexnotexpr( VexCond );
  
    VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, FalseTrans );
    VpnArc = addvpnarctrans( VvhVpnFigure, FalseTrans, EndPlace  );
    VpnArc = addvpnarctrans( VvhVpnFigure, TrueTrans , LoopPlace );
  
    VvhPrevTrans = EndTrans;

    return( 0 );
  }

  TrueTrans = VvhVpnAddTrans( VvhName );
  TrueTrans->TYPE  = VPN_TRANS_IMMEDIATE;

  VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, TrueTrans );
  VpnArc = addvpnarctrans( VvhVpnFigure, TrueTrans , LoopPlace );

  VvhPrevTrans = (vpntrans_list *)0;

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatCall                       |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatCall( ScanCall )

  vbcal_list  *ScanCall;
{
  vpnplace_list *CallPlace;
  vpntrans_list *CallTrans;
  vpnact_list   *VpnAction;
  vpnarc        *VpnArc;
  vexexpr       *Expr;
  long           Number;

  VvhCurrentLine = ScanCall->LINE;

  Number = VvhNumberCall++;
  Expr   = VvhVbhDuplicateExpr( ScanCall->CALL );

  sprintf( VvhName, "call_%s.%s.%ld", (char *)Expr->VALUE, VvhTopName, Number );
  CallPlace = VvhVpnAddPlace( VvhName );
  CallPlace->TYPE = VPN_PLACE_CALL;
  CallTrans = VvhVpnAddTrans( VvhName );
  CallTrans->TYPE = VPN_TRANS_ACT_EXEC;

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, CallPlace );
  VpnArc = addvpnarcplace( VvhVpnFigure, CallPlace , CallTrans );

  VvhPrevTrans = CallTrans;

  VpnAction = addvpnactcall( VvhVpnFigure, VvhPrevTrans, Expr );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatReturn                     |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatReturn( ScanReturn )

  vbret_list  *ScanReturn;
{
  vpnplace_list *ReturnPlace;
  vpntrans_list *ReturnTrans;
  vpnact_list   *VpnAction;
  vpnarc        *VpnArc;
  vexexpr       *Expr;
  long           Number;

  VvhCurrentLine = ScanReturn->LINE;

  Number = VvhNumberReturn++;

  if ( ScanReturn->RET != (vexexpr *)0 )
  {
    Expr = VvhVbhDuplicateExpr( ScanReturn->RET );
  }
  else
  {
    Expr = (vexexpr *)0;
  }

  sprintf( VvhName, "ret.%s.%ld", VvhTopName, Number );
  ReturnPlace = VvhVpnAddPlace( VvhName );
  ReturnPlace->TYPE = VPN_PLACE_RETURN;
  ReturnTrans = VvhVpnAddTrans( VvhName );
  ReturnTrans->TYPE = VPN_TRANS_ACT_EXEC;

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, ReturnPlace );
  VpnArc = addvpnarcplace( VvhVpnFigure, ReturnPlace , ReturnTrans );

  VvhPrevTrans = ReturnTrans;

  VpnAction = addvpnactreturn( VvhVpnFigure, VvhPrevTrans, Expr );
  addvpnline( VvhVpnFigure, &VpnAction->LINE, VvhCurrentLine );

  VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, VvhFirstPlace );

  VvhPrevTrans = (vpntrans_list *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatInstruction                |
|                                                             |
\------------------------------------------------------------*/

void VvhVbhTreatInstruction( Instruction )

  ptype_list  *Instruction;
{
  while ( Instruction != (ptype_list *)0 )
  {
    switch ( Instruction->TYPE )
    {
      case VBH_BEIFS : VvhVbhTreatIfs( Instruction->DATA );
      break;

      case VBH_BEASG : VvhVbhTreatAsg( Instruction->DATA );
      break;

      case VBH_BEVAR : VvhVbhTreatVar( Instruction->DATA );
      break;

      case VBH_BECAS : VvhVbhTreatCase( Instruction->DATA );
      break;

      case VBH_BEWAS : if ( VvhVbhTreatWait( Instruction->DATA ) ) return;
      break;

      case VBH_BEFOR : VvhVbhTreatFor( Instruction->DATA );
      break;

      case VBH_BEWHI : VvhVbhTreatWhile( Instruction->DATA );
      break;

      case VBH_BELOP : if ( VvhVbhTreatLoop( Instruction->DATA ) ) return;
      break;

      case VBH_BENXT : if ( VvhVbhTreatNext( Instruction->DATA ) ) return;
      break;

      case VBH_BEEXT : if ( VvhVbhTreatExit( Instruction->DATA ) ) return;
      break;

      case VBH_BECAL : VvhVbhTreatCall( Instruction->DATA );
      break;

      case VBH_BERET : VvhVbhTreatReturn( Instruction->DATA ); return;
      break;

      case VBH_BEAGR : VvhVbhTreatAggregate( Instruction->DATA );
      break;
    }

    Instruction = Instruction->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatProcess                    |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatProcess()
{
  vpnplace_list *BeginPlace;
  vpntrans_list *BeginTrans;
  vpnplace_list *WaitPlace;
  vpntrans_list *WaitTrans;
  vpndecl_list  *VpnDeclar;
  vpnarc        *VpnArc;
  ptype_list    *ScanPType;
  chain_list    *ScanChain;
  vbpcs_list    *ScanProc;
  vbvar_list    *ScanVar;
  char          *VarName;
  vexexpr       *Expr;
  unsigned char  Base;
  long           Number;

  VvhNumberProc = 0;

  for ( ScanProc  = VvhVbhFigure->BEPCS;
        ScanProc != (vbpcs_list *)0;
        ScanProc  = ScanProc->NEXT )
  {
    VvhCurrentLine = ScanProc->LINE;

    resetauthtable( VvhHashVarName   );
    resetauthtable( VvhHashEndLoop   );
    resetauthtable( VvhHashBeginLoop );

    VvhBeginLoop = (vpnplace_list *)0;
    VvhEndLoop   = (vpnplace_list *)0;

    if ( ( ScanProc->LABEL != (char *)0 ) &&
         ( ScanProc->LABEL[ 0 ] != '\0' ) )
    {
      sprintf( VvhName, "%s", ScanProc->LABEL );
    }
    else
    {
      sprintf( VvhName, "proc_%ld", VvhNumberProc );
    }

    VvhProcess = addvpnproc( VvhVpnFigure, VvhName );
    addvpnline( VvhVpnFigure, &VvhProcess->LINE, VvhCurrentLine );
    VvhProcess->TYPE = ScanProc->TYPE;

    VvhTopName = VvhProcess->NAME;

    sprintf( VvhName, "beg.%s", VvhTopName );
    BeginPlace = VvhVpnAddPlace( VvhName );
    BeginPlace->TYPE = VPN_PLACE_PROCESS;
    addvpnline( VvhVpnFigure, &BeginPlace->LINE, VvhCurrentLine );

    sprintf( VvhName, "beg.%s", VvhTopName );
    BeginTrans = VvhVpnAddTrans( VvhName );
    BeginTrans->TYPE  = VPN_TRANS_SUP_WAIT;

    VvhProcess->FIRST = BeginTrans;
    VvhProcess->ELABO = BeginTrans;
    BeginPlace->TOKEN = 1;

    VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, BeginTrans );

    for ( ScanPType  = ScanProc->VARIABLE;
          ScanPType != (ptype_list *)0;
          ScanPType  = ScanPType->NEXT )
    {
      ScanVar = (vbvar_list *)ScanPType->DATA;
      VvhCurrentLine = ScanVar->LINE;

      sprintf( VvhName, "%s_%s", VvhTopName, (char *)ScanVar->TARGET->VALUE );
      VarName = namealloc( VvhName );
      addauthelem( VvhHashVarName, ScanVar->TARGET->VALUE, (long)VarName );

      Base = VvhVpnGetBaseType( ScanVar->TYPE );
      Expr = VvhVpnGetVexDeclar( ScanVar->TARGET, Base, ScanVar->TYPE );

      VpnDeclar = addvpndeclprocvar( VvhVpnFigure, VvhProcess, Expr, Base );
      addvpnline( VvhVpnFigure, &VpnDeclar->LINE, VvhCurrentLine );

      VvhVbhTreatInitial( VpnDeclar, ScanVar->VEX );
    }

    VvhPrevTrans    = BeginTrans;
    VvhNumberAsg    = 0;
    VvhNumberIfs    = 0;
    VvhNumberCase   = 0;
    VvhNumberWait   = 0;
    VvhNumberWhile  = 0;
    VvhNumberLoop   = 0;
    VvhNumberFor    = 0;
    VvhNumberNext   = 0;
    VvhNumberExit   = 0;
    VvhNumberCall   = 0;
    VvhNumberReturn = 0;

    VvhVbhTreatInstruction( ScanProc->INSTRUCTION );

    ScanChain = ScanProc->SENSITIVITY;

    if ( ScanChain != (chain_list *)0 )
    {
      Number = VvhNumberWait++;

      sprintf( VvhName, "wait.%s.%ld", VvhTopName, Number );

      WaitPlace = VvhVpnAddPlace( VvhName );
      WaitPlace->TYPE = VPN_PLACE_WAIT;
      WaitTrans = VvhVpnAddTrans( VvhName );

      if ( VvhPrevTrans != (vpntrans_list *)0 )
      {
        VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, WaitPlace );
      }

      VpnArc = addvpnarcplace( VvhVpnFigure, WaitPlace, WaitTrans );

      WaitTrans->TYPE = VPN_TRANS_INF_WAIT;
      VvhPrevTrans = WaitTrans;

      VvhVbhTreatWaitSymbol( ScanChain, WaitTrans );
    }

    if ( VvhPrevTrans != (vpntrans_list *)0 )
    {
      VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );
    }

    VvhNumberProc++;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       VvhVbhTreatFunction                   |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatFunction()
{
  vpnplace_list *BeginPlace;
  vpntrans_list *BeginTrans;
  vpndecl_list  *VpnDeclar;
  vpnarc        *VpnArc;
  ptype_list    *ScanPType;
  vbfun_list    *ScanFunc;
  vbarg_list    *ScanArg;
  vbvar_list    *ScanVar;
  char          *VarName;
  char          *ArgName;
  vexexpr       *Expr;
  unsigned char  Base;
  unsigned char  Dir;
  unsigned char  Class;

  VvhNumberFunc = 0;

  VvhProcess = (vpnproc_list *)0;

  for ( ScanFunc  = VvhVbhFigure->BEFUN;
        ScanFunc != (vbfun_list *)0;
        ScanFunc  = ScanFunc->NEXT )
  {
    VvhCurrentLine = ScanFunc->LINE;

    resetauthtable( VvhHashVarName   );
    resetauthtable( VvhHashEndLoop   );
    resetauthtable( VvhHashBeginLoop );

    VvhBeginLoop = (vpnplace_list *)0;
    VvhEndLoop   = (vpnplace_list *)0;

    sprintf( VvhName, "%s", ScanFunc->NAME );
    VvhTopName = namealloc( VvhName );
    VvhFunction = addvpnfunc( VvhVpnFigure, ScanFunc->NAME );
    addvpnline( VvhVpnFigure, &VvhFunction->LINE, VvhCurrentLine );
    VvhFunction->TYPE = ScanFunc->TYPE;

    sprintf( VvhName, "beg.%s", VvhTopName );
    BeginPlace = VvhVpnAddPlace( VvhName );
    BeginPlace->TYPE = VPN_PLACE_FUNCTION;

    VvhFirstPlace = BeginPlace;

    sprintf( VvhName, "beg.%s", VvhTopName );
    BeginTrans = addvpnfunctrans( VvhVpnFigure, VvhFunction, VvhName );
    BeginTrans->TYPE  = VPN_TRANS_SUP_WAIT;

    VvhFunction->FIRST = BeginTrans;
    VvhFunction->ELABO = BeginTrans;
    BeginPlace->TOKEN = 1;

    VpnArc = addvpnarcplace( VvhVpnFigure, BeginPlace, BeginTrans );

    for ( ScanArg  = ScanFunc->ARGUMENT;
          ScanArg != (vbarg_list *)0;
          ScanArg  = ScanArg->NEXT )
    {
      sprintf( VvhName, "%s_%s", VvhFunction->NAME, (char *)ScanArg->TARGET->VALUE );
      ArgName = namealloc( VvhName );
      addauthelem( VvhHashVarName, ScanArg->TARGET->VALUE, (long)ArgName );

      switch ( ScanArg->DIR )
      {
        case 'I' : Dir = VPN_DIR_IN; break;
        case 'O' : Dir = VPN_DIR_OUT; break;
        default  : Dir = VPN_DIR_INOUT; break;
      }

      switch ( ScanArg->CLASS )
      {
        case VBH_CLASS_CONSTANT : Class = VPN_CLASS_CONSTANT; break;
        case VBH_CLASS_VARIABLE : Class = VPN_CLASS_VARIABLE; break;
        case VBH_CLASS_SIGNAL   : Class = VPN_CLASS_SIGNAL;   break;
        default                 : Class = VPN_CLASS_NONE;     break;
      }

      Base = VvhVpnGetBaseType( ScanArg->TYPE );
      Expr = VvhVpnGetVexDeclar( ScanArg->TARGET, Base, ScanArg->TYPE );

      VpnDeclar = addvpndeclfuncarg( VvhVpnFigure, VvhFunction, Expr, 
                                     Base, Dir, Class );
    }

    if ( ScanFunc->RETURN != (vbarg_list *)0 )
    {
      ScanArg = ScanFunc->RETURN;
      VvhFunction->VEX_RET = VvhVbhDuplicateExpr( ScanArg->TARGET );
    }

    for ( ScanPType  = ScanFunc->VARIABLE;
          ScanPType != (ptype_list *)0;
          ScanPType  = ScanPType->NEXT )
    {
      ScanVar = (vbvar_list *)ScanPType->DATA;

      sprintf( VvhName, "%s_%s", VvhFunction->NAME, (char *)ScanVar->TARGET->VALUE );
      VarName = namealloc( VvhName );
      addauthelem( VvhHashVarName, ScanVar->TARGET->VALUE, (long)VarName );

      Base = VvhVpnGetBaseType( ScanVar->TYPE );
      Expr = VvhVpnGetVexDeclar( ScanVar->TARGET, Base, ScanVar->TYPE );

      VpnDeclar = addvpndeclfuncvar( VvhVpnFigure, VvhFunction, Expr, Base );

      VvhVbhTreatInitial( VpnDeclar, ScanVar->VEX );
    }

    VvhPrevTrans    = BeginTrans;
    VvhNumberAsg    = 0;
    VvhNumberIfs    = 0;
    VvhNumberCase   = 0;
    VvhNumberWait   = 0;
    VvhNumberWhile  = 0;
    VvhNumberLoop   = 0;
    VvhNumberFor    = 0;
    VvhNumberNext   = 0;
    VvhNumberExit   = 0;
    VvhNumberCall   = 0;
    VvhNumberReturn = 0;

    VvhVbhTreatInstruction( ScanFunc->INSTRUCTION );

    if ( VvhPrevTrans != (vpntrans_list *)0 )
    {
      VpnArc = addvpnarctrans( VvhVpnFigure, VvhPrevTrans, BeginPlace );
    }

    VvhNumberFunc++;
  }

  VvhFunction = (vpnfunc_list *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                          VvhVbhTreatAttribute               |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatAttribute()
{
  vpnattr_list  *VpnAttr;
  vbatr_list    *ScanAttr;
  unsigned char  Type;

  for ( ScanAttr  = VvhVbhFigure->BEATR;
        ScanAttr != (vbatr_list *)0;
        ScanAttr  = ScanAttr->NEXT )
  {
    VvhCurrentLine = ScanAttr->LINE;

    switch ( ScanAttr->TYPE )
    {
      case VBH_ATTR_ENTITY         : Type = VPN_ATTR_ENTITY; break;
      case VBH_ATTR_ARCHITECTURE   : Type = VPN_ATTR_ARCHITECTURE; break;
      case VBH_ATTR_PACKAGE        : Type = VPN_ATTR_PACKAGE; break;
      case VBH_ATTR_FUNCTION       : Type = VPN_ATTR_FUNCTION; break;
      case VBH_ATTR_PROCEDURE      : Type = VPN_ATTR_PROCEDURE; break;
      case VBH_ATTR_SUBTYPE        : Type = VPN_ATTR_SUBTYPE; break;
      case VBH_ATTR_CONSTANT       : Type = VPN_ATTR_CONSTANT; break;
      case VBH_ATTR_VARIABLE       : Type = VPN_ATTR_VARIABLE; break;
      case VBH_ATTR_SIGNAL         : Type = VPN_ATTR_SIGNAL; break;
      case VBH_ATTR_LABEL          : Type = VPN_ATTR_LABEL; break;
      case VBH_ATTR_TYPE           : Type = VPN_ATTR_TYPE; break;
      case VBH_ATTR_CONFIGURATION  : Type = VPN_ATTR_CONFIGURATION; break;
      case VBH_ATTR_COMPONENT      : Type = VPN_ATTR_COMPONENT; break;
      default                      : Type = VPN_ATTR_NONE;     break;
    }

    VpnAttr = addvpnattr( VvhVpnFigure, ScanAttr->NAME,
                          ScanAttr->ENTITY, Type, ScanAttr->VALUE );

    addvpnline( VvhVpnFigure, &VpnAttr->LINE, VvhCurrentLine );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          VvhVbhTreatModel                   |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatModel()
{
  vpnmod_list   *VpnModel;
  vpnport_list  *VpnPort;
  vpngen_list   *VpnGen;
  vbmod_list    *ScanModel;
  vbpor_list    *ScanPort;
  vbgen_list    *ScanGen;
  vexexpr       *Expr;
  vexexpr       *Data;
  unsigned char  Base;
  unsigned char  Dir;

  for ( ScanModel  = VvhVbhFigure->BEMOD;
        ScanModel != (vbmod_list *)0;
        ScanModel  = ScanModel->NEXT )
  {
    VvhCurrentLine = ScanModel->LINE;

    VpnModel = addvpnmod( VvhVpnFigure, ScanModel->NAME );

    addvpnline( VvhVpnFigure, &VpnModel->LINE, VvhCurrentLine );

    for ( ScanPort  = ScanModel->BEPOR;
          ScanPort != (vbpor_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      VvhCurrentLine = ScanPort->LINE;

      switch ( ScanPort->DIR )
      {
        case 'I' : Dir = VPN_DIR_IN; break;
        case 'O' : Dir = VPN_DIR_OUT; break;
        default  : Dir = VPN_DIR_INOUT; break;
      }

      Base = VvhVpnGetBaseType( ScanPort->TYPE );
      Expr = VvhVpnGetVexDeclar( ScanPort->TARGET, Base, ScanPort->TYPE );

      VpnPort = addvpnmodport( VvhVpnFigure, VpnModel, Expr, Base, Dir );

      addvpnline( VvhVpnFigure, &VpnPort->LINE, VvhCurrentLine );
    }

    for ( ScanGen  = ScanModel->BEGEN;
          ScanGen != (vbgen_list *)0;
          ScanGen  = ScanGen->NEXT )
    {
      VvhCurrentLine = ScanGen->LINE;

      Base = VvhVpnGetBaseType( ScanGen->TYPE );
      Expr = VvhVpnGetVexDeclar( ScanGen->TARGET, Base, ScanGen->TYPE );
      Data = dupvexexpr( ScanGen->VEX );

      VpnGen = addvpnmodgen( VvhVpnFigure, VpnModel, Expr, Data, Base );

      addvpnline( VvhVpnFigure, &VpnGen->LINE, VvhCurrentLine );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          VvhVbhTreatInstance                |
|                                                             |
\------------------------------------------------------------*/

static void VvhVbhTreatInstance()
{
  vpnmod_list   *VpnModel;
  vpnins_list   *VpnInst;
  vpnmap_list   *VpnMap;
  vpngen_list   *VpnGen;
  vbins_list    *ScanInst;
  vbmap_list    *ScanMap;
  vexexpr       *VexFormal;
  vexexpr       *VexActual;

  for ( ScanInst  = VvhVbhFigure->BEINS;
        ScanInst != (vbins_list *)0;
        ScanInst  = ScanInst->NEXT )
  {
    VvhCurrentLine = ScanInst->LINE;

    VpnModel = searchvpnmod( VvhVpnFigure, ScanInst->MODEL );
    VpnInst  = addvpnins( VvhVpnFigure, ScanInst->NAME, VpnModel );

    addvpnline( VvhVpnFigure, &VpnInst->LINE, VvhCurrentLine );

    for ( ScanMap  = ScanInst->PORT_MAP;
          ScanMap != (vbmap_list *)0;
          ScanMap  = ScanMap->NEXT )
    {
      VvhCurrentLine = ScanMap->LINE;

      VexFormal = VvhVbhDuplicateExpr( ScanMap->FORMAL );
      VexActual = VvhVbhDuplicateExpr( ScanMap->ACTUAL );

      VpnMap = addvpninsmap( VvhVpnFigure, VpnInst, VexFormal, VexActual );

      addvpnline( VvhVpnFigure, &VpnMap->LINE, VvhCurrentLine );
    }

    for ( ScanMap  = ScanInst->GEN_MAP;
          ScanMap != (vbmap_list *)0;
          ScanMap  = ScanMap->NEXT )
    {
      VvhCurrentLine = ScanMap->LINE;

      VexFormal = VvhVbhDuplicateExpr( ScanMap->FORMAL );
      VexActual = VvhVbhDuplicateExpr( ScanMap->ACTUAL );

      VpnGen = addvpninsgen( VvhVpnFigure, VpnInst, VexFormal, VexActual );

      addvpnline( VvhVpnFigure, &VpnGen->LINE, VvhCurrentLine );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          VvhVbh2Vpn                         |
|                                                             |
\------------------------------------------------------------*/

vpnfig_list *VvhVbh2Vpn( VbhFigure, VpnFigure, Reduced )

  vbfig_list  *VbhFigure;
  vpnfig_list *VpnFigure;
  int          Reduced;
{
  VvhVbhFigure = VbhFigure;
  VvhVpnFigure = VpnFigure;
  VvhReduced   = Reduced;

  if ( VvhHashVarName == (authtable *)0 )
  {
    VvhHashVarName   = createauthtable( 100 );
    VvhHashEndLoop   = createauthtable( 100 );
    VvhHashBeginLoop = createauthtable( 100 );
  }

  if ( Reduced )
  {
    vbh_simpvbfig( VbhFigure );
  }

  VvhVbhTreatGeneric();
  VvhVbhTreatPort();
  VvhVbhTreatConstant();
  VvhVbhTreatAuxiliary();
  VvhVbhTreatProcess();
  VvhVbhTreatFunction();
  VvhVbhTreatAttribute();
  VvhVbhTreatModel();
  VvhVbhTreatInstance();

  return( VpnFigure );
}
