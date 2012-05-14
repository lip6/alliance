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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                    XVPN                           |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XVP.h"
# include "XME.h"
# include "XMV.h"

# include "XME_select.h"
# include "XME_panel.h"

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
|                         Select Buffer                       |
|                                                             |
\------------------------------------------------------------*/

  static char XvpnSelectBuffer [ XVPN_SELECT_BUFFER_SIZE  ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XvpnAddSelectList                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnAddSelectList( Obj )

  xvpnobj_list *Obj;
{
  autbegin();

  XvpnSelectBuffer[0] = '\0';

/*\
  if ( IsXvpnPlace( Obj ) )
  {
    Place = (vpnplace_list *)( Obj->USER );

    sprintf( XvpnSelectBuffer, "  PLACE %s", Place->NAME );
  }
  else
  if ( IsXvpnTrans( Obj ) )
  {
    Trans = (vpntrans_list *)( Obj->USER );

    sprintf( XvpnSelectBuffer, "  TRANSITION %s", Trans->NAME );
  }
  else
  if ( IsXvpnArcPlace( Obj ) )
  {
    Tra   = (vpnarc_list   *)( Obj->USER );
    Trans = (vpntrans_list *)( Tra->TARGET );
    Place = (vpnplace_list *)( VPN_ARC_PLUS( Tra )->USER );

    sprintf( XvpnSelectBuffer, "  ARC FROM PLACE %s TO TRANSITION %s",
             Place->NAME, Trans->NAME );
  }
  else
  if ( IsXvpnArcTrans( Obj ) )
  {
    Pla   = (vpnarc_list   *)( Obj->USER );
    Place = (vpnplace_list *)( Pla->TARGET );
    Trans = (vpntrans_list *)( VPN_ARC_PLUS( Pla )->USER );

    sprintf( XvpnSelectBuffer, "  ARC FROM TRANSITION %s TO PLACE %s",
             Trans->NAME, Place->NAME );
  }
\*/

  XvpnDisplaySelectList( XvpnSelectBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XvpnEditSelectObject                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditSelectObject( Number )

  int Number;
{
  xvpnselect_list *Select;
  int              Counter;

  autbegin();

  Select = XvpnHeadSelect;

  for ( Counter = 0; Counter < Number; Counter++ )
  {
    Select = Select->NEXT;
  }

  if ( IsXvpnAccept( Select->OBJECT ) )
  {
    XvpnRejectObject( Select->OBJECT );
  }
  else
  {
    XvpnAcceptObject( Select->OBJECT );
  }

  XvpnDisplayObject( Select->OBJECT );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XvpnEditSelectAccept                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditSelectAccept()
{
  autbegin();

  XvpnPurgeSelect();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XvpnEditSelectCancel                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditSelectCancel()
{
  autbegin();

  XvpnEditUnselectAll();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Select Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnEditUnselectAll                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditUnselectAll()
{
  xvpnobj_list *Obj;

  autbegin();

  if ( XvpnHeadSelect != (xvpnselect_list *)NULL )
  {
    if ( XvpnHeadSelect->NEXT == (xvpnselect_list *)NULL )
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

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnEditObjectNearPoint                |
|                                                             |
\------------------------------------------------------------*/

char XvpnEditObjectNearPoint( Obj, X1, Y1 )

  xvpnobj_list *Obj;
  long          X1;
  long          Y1;
{
  short  Type;
  long   X1r;
  long   Y1r;
  long   X2r;
  long   Y2r;
  float  VectorX;
  float  VectorY;
  float  LineX;
  float  LineY;
  float  Norm;
  float  Distance;

  Type = GetXvpnObjectType( Obj );

  if ( ( Obj->X           <= X1 ) &&
       ( Obj->Y           <= Y1 ) &&
       ( Obj->X + Obj->DX >= X1 ) &&
       ( Obj->Y + Obj->DY >= Y1 ) )
  {
    if ( ( Type == XVPN_OBJECT_LINE  ) ||
         ( Type == XVPN_OBJECT_ARROW ) )
    {
      if ( ( Obj->DX == 0 ) && 
           ( Obj->DY == 0 ) )
      {
        if ( ( Obj->X == X1 ) &&
             ( Obj->Y == Y1 ) )
        {
          return( XVPN_TRUE );
        }
      }
      else
      {
        if ( IsXvpnLineLeft( Obj ) )
        {
          X1r = Obj->X + Obj->DX;
          X2r = Obj->X;
        }
        else
        {
          X1r = Obj->X;
          X2r = Obj->X + Obj->DX;
        }
    
        if ( IsXvpnLineDown( Obj ) )
        {
          Y1r  = Obj->Y + Obj->DY;
          Y2r  = Obj->Y;
        }
        else
        {
          Y1r  = Obj->Y;
          Y2r  = Obj->Y + Obj->DY;
        }
  
        LineX   = (float)( X2r - X1r );
        LineY   = (float)( Y2r - Y1r );
        Norm    = sqrt( ( LineX * LineX ) + ( LineY * LineY ) );
        LineX  /= Norm;
        LineY  /= Norm;
  
        VectorX  = (float)( X2r - X1  );
        VectorY  = (float)( Y2r - Y1  );
  
        Distance = fabs( ( VectorX * LineY ) - ( VectorY * LineX ) );

        if ( Distance < (float) XVPN_UNIT )
        {
          return( XVPN_TRUE );
        }
      }
    }
    else
    {
      return( XVPN_TRUE );
    }
  }

  return( XVPN_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XvpnEditSelectPoint                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditSelectPoint( X1, Y1 )

  long X1;
  long Y1;
{
  xvpnobj_list    *Obj;
  xvpnselect_list *Select;
  int              Layer;

  if ( XvpnFigure == (xvpnfig_list *)0 ) return;

  X1 = X1 * XVPN_UNIT;
  Y1 = Y1 * XVPN_UNIT;

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    if ( XVPN_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XvpnFigure->OBJECT[ Layer ];
          Obj != (xvpnobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XVPN_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ! IsXvpnSelect( Obj ) )
      {
        if ( XvpnEditObjectNearPoint( Obj, X1, Y1 ) )
        {
          XvpnAddSelect( Obj );
        }
      }
    }
  }

  for ( Select  = XvpnHeadSelect;
        Select != (xvpnselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XvpnUnselectObject( Select->OBJECT );
  }
}
