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
| Tool    :                   XVPN                            |
|                                                             |
| File    :                   Edit.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <math.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XVP.h"
# include "XMV.h"
# include "XME.h"

# include "XME_edit.h"
# include "XME_panel.h"
# include "XME_select.h"
# include "XME_message.h"

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
|                        Identify Buffer                      |
|                                                             |
\------------------------------------------------------------*/

  static char  XvpnIdentifyMessage[ XVPN_IDENTIFY_MESSAGE_SIZE ];
  static char  XvpnIdentifyBuffer [ XVPN_IDENTIFY_BUFFER_SIZE  ];
  static char  XvpnIdentifyVex    [ XVPN_IDENTIFY_VEX_SIZE     ];
  static char  XvpnIdentifyAct    [ XVPN_IDENTIFY_ACT_SIZE     ];
  static char  XvpnIdentifySig    [ XVPN_IDENTIFY_SIG_SIZE     ];

  static char *XvpnScanIdentify;
  static long  XvpnIdentifyLength;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XvpnAddIdentify                     |
|                                                             |
\------------------------------------------------------------*/

short XvpnAddIdentify( Obj )

  xvpnobj_list *Obj;
{
  vpnplace_list *Place;
  vpntrans_list *Trans;
  vpnarc        *Tra;
  vpnarc        *Pla;
  vpnact_list   *Act;
  vpnsym        *Sig;
  vpnproc_list  *Proc;
  vpnfunc_list  *Func;
  vpndecl_list  *Decl;
  vpnline_list  *Line;
  chain_list    *ScanChain;
  char          *ScanString;
  char          *VexString;
  char          *FileName;
  long           Length;

  autbegin();

  XvpnIdentifyBuffer[0] = '\0';

  if ( IsXvpnPlace( Obj ) )
  {
    Place = (vpnplace_list *)( Obj->USER );

    ScanString  = XvpnIdentifyBuffer;
    strcpy( ScanString, "PLACE\n\n  LINE :" );
    ScanString += 15;

    FileName = (char *)0;

    for ( Line  = Place->LINE;
          Line != (vpnline_list *)0;
          Line  = Line->NEXT )
    {
      if ( FileName != Line->NAME )
      {
        FileName = Line->NAME;
        sprintf( ScanString, "  %s [%ld]", Line->NAME, Line->LINE );
      }
      else
      {
        sprintf( ScanString, "  [%ld]", Line->LINE );
      }

      ScanString += strlen( ScanString );
    }

    sprintf( ScanString, 
    "\n  NAME : %s\n  FATHER : %s\n  TYPE : %s\n  TOKEN : %d\n  NUMBER IN : %d\n  NUMBER OUT : %d\n  LINK : %s\n  FLAGS : %ld\n\n",
    Place->NAME,
    VPN_FATHER_TYPE[ Place->FATHER ],
    VPN_PLACE_TYPE[ Place->TYPE ],
    Place->TOKEN,
    Place->NUMBER_IN,
    Place->NUMBER_OUT,
    ( Place->LINK == (vpnplace_list *)0 ) ? "none" : Place->LINK->NAME,
    Place->FLAGS );
  }
  else
  if ( IsXvpnTrans( Obj ) )
  {
    Trans = (vpntrans_list *)( Obj->USER );

    ScanString  = XvpnIdentifyAct;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( Act  = Trans->ACT;
          Act != (vpnact_list *)0;
          Act  = Act->NEXT )
    {
      strcpy( ScanString, "LINE :" );
      ScanString += 6;

      for ( Line  = Act->LINE;
            Line != (vpnline_list *)0;
            Line  = Line->NEXT )
      {
        sprintf( ScanString, "  [%ld]", Line->LINE );
        ScanString += strlen( ScanString );
      }

/*\
      strcpy( ScanString, "\n  TYPE : " );
      ScanString += 10;

      VexString = VPN_ACT_TYPE[ Act->TYPE ];
      strcpy( ScanString, VexString );
      ScanString += strlen( VexString );
\*/
      strcpy( ScanString, "\n  > " );
      ScanString += 5;

      if ( Act->VEX_ATOM != (vexexpr *)0 )
      {
        VexString = XvpnVex2String( Act->VEX_ATOM );
        strcpy( ScanString, VexString );
        ScanString += strlen( VexString );

        if ( Act->TYPE == VPN_ACT_ASG_VARIABLE )
        {
          strcpy( ScanString, " := " );
        }
        else
        if ( Act->TYPE == VPN_ACT_ASG_DEFINE )
        {
          strcpy( ScanString, " #= " );
        }
        else
        {
          strcpy( ScanString, " <= " );
        }

        ScanString += 4;
      }

      if ( Act->VEX_EXPR != (vexexpr *)0 )
      {
        VexString = XvpnVex2String( Act->VEX_EXPR );
        strcpy( ScanString, VexString );
        ScanString += strlen( VexString );
      }

      strcpy( ScanString, "\n  " );
      ScanString += 3;
    }

    ScanString  = XvpnIdentifySig;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( ScanChain  = Trans->WAIT_SYM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Sig = (vpnsym *)( ScanChain->DATA );
      sprintf( ScanString, "%s %d", Sig->NAME, Sig->INDEX );
      ScanString += strlen( ScanString );

      strcpy( ScanString, "\n  " );
      ScanString += 3;
    }

    if ( Trans->VEX_GUARD != (vexexpr *)0 )
    {
      VexString = XvpnVex2String( Trans->VEX_GUARD );
      strcpy( XvpnIdentifyVex, VexString );
    }
    else
    {
      XvpnIdentifyVex[ 0 ] = '\0';
    }

    sprintf( XvpnIdentifyBuffer,
    "TRANSITION :\n\n  NAME : %s\n  FATHER : %s\n  TYPE : %s\n  NUMBER IN : %d\n  NUMBER OUT : %d\n  FLAGS : %ld\n  GUARD : %s\n  ACTION : \n%s\n  SYMBOLS\n%s\n\n",
    Trans->NAME,
    VPN_FATHER_TYPE[ Trans->FATHER ],
    VPN_TRANS_TYPE[ Trans->TYPE ],
    Trans->NUMBER_IN,
    Trans->NUMBER_OUT,
    Trans->FLAGS,
    XvpnIdentifyVex,
    XvpnIdentifyAct,
    XvpnIdentifySig );
  }
  else
  if ( IsXvpnArcPlace( Obj ) )
  {
    Tra   = (vpnarc        *)( Obj->USER );
    Trans = (vpntrans_list *)( Tra->TARGET );
    Place = (vpnplace_list *)( Tra->SOURCE );

    sprintf( XvpnIdentifyBuffer,
    "ARC : \n\n  FROM PLACE : %s\n  TO TRANSITION : %s\n\n",
    Place->NAME, Trans->NAME);
  }
  else
  if ( IsXvpnArcTrans( Obj ) )
  {
    Pla   = (vpnarc        *)( Obj->USER );
    Place = (vpnplace_list *)( Pla->TARGET );
    Trans = (vpntrans_list *)( Pla->SOURCE );

    sprintf( XvpnIdentifyBuffer,
    "ARC :\n\n  FROM TRANSITION : %s\n  TO PLACE : %s\n\n",
    Trans->NAME, Place->NAME );
  }
  else
  if ( IsXvpnProc( Obj ) )
  {
    Proc = (vpnproc_list *)( Obj->USER );
    ScanString  = XvpnIdentifySig;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( ScanChain  = Proc->VAR_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Decl = (vpndecl_list *)( ScanChain->DATA );

      strcpy( ScanString, "> LINE :" );
      ScanString += 8;

      for ( Line  = Decl->LINE;
            Line != (vpnline_list *)0;
            Line  = Line->NEXT )
      {
        sprintf( ScanString, "  [%ld]", Line->LINE );
        ScanString += strlen( ScanString );
      }

      strcpy( ScanString, "\n  " );
      ScanString += 3;

      VexString = XvpnVex2String( Decl->VEX_ATOM );
      strcpy( ScanString, VexString );
      ScanString += strlen( ScanString );

      strcpy( ScanString, "\n  " );
      ScanString += 3;
    }

    ScanString  = XvpnIdentifyVex;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( ScanChain  = Proc->DEF_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Decl = (vpndecl_list *)( ScanChain->DATA );

      strcpy( ScanString, "> LINE :" );
      ScanString += 8;

      for ( Line  = Decl->LINE;
            Line != (vpnline_list *)0;
            Line  = Line->NEXT )
      {
        sprintf( ScanString, "  [%ld]", Line->LINE );
        ScanString += strlen( ScanString );
      }

      strcpy( ScanString, "\n  " );
      ScanString += 3;

      VexString = XvpnVex2String( Decl->VEX_ATOM );
      strcpy( ScanString, VexString );
      ScanString += strlen( ScanString );

      strcpy( ScanString, "\n  " );
      ScanString += 3;
    }

    ScanString  = XvpnIdentifyBuffer;
    strcpy( ScanString, "PROCESS\n\n  LINE :" );
    ScanString += 17;

    for ( Line  = Proc->LINE;
          Line != (vpnline_list *)0;
          Line  = Line->NEXT )
    {
      sprintf( ScanString, "  [%ld]", Line->LINE );
      ScanString += strlen( ScanString );
    }

    sprintf( ScanString,
    "\n  NAME : %s\n  VARIABLE : \n%s\n  DEFINE : \n%s\n\n",
    Proc->NAME, XvpnIdentifySig, XvpnIdentifyVex );

    ScanString += strlen( ScanString );
    sprintf( ScanString, "\n  ELABO : %s\n  FIRST : %s\n\n\n",
        ( Proc->ELABO != (vpntrans_list *)0 ) ? Proc->ELABO->NAME : "None",
        ( Proc->FIRST != (vpntrans_list *)0 ) ? Proc->FIRST->NAME : "None" );
  }
  else
  if ( IsXvpnFunc( Obj ) )
  {
    Func = (vpnfunc_list *)( Obj->USER );

    ScanString  = XvpnIdentifySig;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( ScanChain  = Func->ARG_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Decl = (vpndecl_list *)( ScanChain->DATA );

      strcpy( ScanString, "> LINE :" );
      ScanString += 8;

      for ( Line  = Decl->LINE;
            Line != (vpnline_list *)0;
            Line  = Line->NEXT )
      {
        sprintf( ScanString, "  [%ld]", Line->LINE );
        ScanString += strlen( ScanString );
      }

      strcpy( ScanString, "\n  " );
      ScanString += 3;

      VexString = XvpnVex2String( Decl->VEX_ATOM );
      strcpy( ScanString, VexString );
      ScanString += strlen( ScanString );

      strcpy( ScanString, "\n  " );
      ScanString += 3;
    }

    ScanString  = XvpnIdentifyAct;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( ScanChain  = Func->VAR_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Decl = (vpndecl_list *)( ScanChain->DATA );

      strcpy( ScanString, "> LINE :" );
      ScanString += 8;

      for ( Line  = Decl->LINE;
            Line != (vpnline_list *)0;
            Line  = Line->NEXT )
      {
        sprintf( ScanString, "  [%ld]", Line->LINE );
        ScanString += strlen( ScanString );
      }

      strcpy( ScanString, "\n  " );
      ScanString += 3;

      VexString = XvpnVex2String( Decl->VEX_ATOM );
      strcpy( ScanString, VexString );
      ScanString += strlen( ScanString );

      strcpy( ScanString, "\n  " );
      ScanString += 3;
    }

    ScanString  = XvpnIdentifyVex;
    strcpy( ScanString, "  " );
    ScanString += 2;

    for ( ScanChain  = Func->DEF_DECL;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Decl = (vpndecl_list *)( ScanChain->DATA );

      strcpy( ScanString, "> LINE :" );
      ScanString += 8;

      for ( Line  = Decl->LINE;
            Line != (vpnline_list *)0;
            Line  = Line->NEXT )
      {
        sprintf( ScanString, "  [%ld]", Line->LINE );
        ScanString += strlen( ScanString );
      }

      strcpy( ScanString, "\n  " );
      ScanString += 3;

      VexString = XvpnVex2String( Decl->VEX_ATOM );
      strcpy( ScanString, VexString );
      ScanString += strlen( ScanString );

      strcpy( ScanString, "\n  " );
      ScanString += 3;
    }

    ScanString  = XvpnIdentifyBuffer;
    strcpy( ScanString, "FUNCTION\n\n  LINE :" );
    ScanString += 18;

    for ( Line  = Func->LINE;
          Line != (vpnline_list *)0;
          Line  = Line->NEXT )
    {
      sprintf( ScanString, "  [%ld]", Line->LINE );
      ScanString += strlen( ScanString );
    }

    sprintf( ScanString,
    "\n  NAME : %s\n  ARGUMENT : \n%s\n  VARIABLE : \n%s\n  DEFINE : \n%s\n\n",
    Func->NAME, XvpnIdentifySig, XvpnIdentifyAct, XvpnIdentifyVex );
  }

  Length = strlen( XvpnIdentifyBuffer );

  if ( ( XvpnIdentifyLength + Length ) < ( XVPN_IDENTIFY_MESSAGE_SIZE - 1 ) )
  {
    strcpy( XvpnScanIdentify, XvpnIdentifyBuffer );

    XvpnScanIdentify   += Length; 
    XvpnIdentifyLength += Length;

    autend();
    return( XVPN_TRUE );
  }

  strcpy( XvpnScanIdentify, "#" );
 
  autend();
  return( XVPN_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnEditIdentify                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditIdentify( X1, Y1 )

  long X1;
  long Y1;
{
  xvpnselect_list *Select;

  strcpy( XvpnIdentifyMessage, "No element found !" );

  XvpnScanIdentify   = XvpnIdentifyMessage;
  XvpnIdentifyLength = 0;

  XvpnEditSelectPoint( X1, Y1 );

  for ( Select  = XvpnHeadSelect;
        Select != (xvpnselect_list *)0;
        Select  = Select->NEXT )
  {
    if ( ! XvpnAddIdentify( Select->OBJECT ) ) break;
  }

  XvpnDelSelect();

  XvpnDisplayEditIdentify( XvpnIdentifyMessage );
  XvpnEnterPanel( &XvpnEditIdentifyPanel       );
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnEditConnected                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditConnected( X1, Y1 )

  long X1;
  long Y1;
{
  xvpnselect_list  *Select;
  xvpnobj_list     *Obj;

  if ( XvpnHeadConnect != (xvpnselect_list *)0 )
  {
    XvpnDelConnect();
    XvpnZoomRefresh();
  }

  XvpnEditSelectPoint( X1, Y1 );

  XvpnDisplaySelectList( (char *)0 );

  for ( Select  = XvpnHeadSelect;
        Select != (xvpnselect_list *)0;
        Select  = Select->NEXT )
  {
    XvpnAddSelectList( Select->OBJECT );
  }

  if ( XvpnHeadSelect == (xvpnselect_list *)0  )
  {
    XvpnWarningMessage( XvpnMainWindow, "No element found !" );
  }
  else
  {
    if ( XvpnHeadSelect->NEXT != (xvpnselect_list *)0 )
    {
      XvpnEnterPanel( &XvpnEditSelectPanel );
      XvpnLimitedLoop( XvpnEditSelectPanel.PANEL );
    }
    else
    {
      XvpnAcceptObject( XvpnHeadSelect->OBJECT );
      XvpnDisplayObject( XvpnHeadSelect->OBJECT );
    }
  }

  XvpnPurgeSelect();

  for ( Select  = XvpnHeadSelect;
        Select != (xvpnselect_list *)0;
        Select  = Select->NEXT )
  {
    XvpnAddConnect( Select->OBJECT );
  }

  if ( XvpnHeadSelect != (xvpnselect_list *)0 )
  {
    if ( XvpnHeadSelect->NEXT == (xvpnselect_list *)0 )
    {
      Obj = XvpnHeadSelect->OBJECT;
      XvpnDelSelect();
      XvpnDisplayObject( Obj );
    }
    else
    {
      XvpnDelSelect();
      XvpnZoomRefresh();
    }
  }
}
