/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                  Search.c                         |
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
# include "XME.h"
# include "XMV.h"

# include "XME_search.h"

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

  char  XvpnSearchAbort;
  int   XvpnSearchNumber;
  char *XvpnSearchString;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XvpnEditInitializeSearch                 |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditInitializeSearch( Name )

  char *Name;
{
  char *Star;

  autbegin();

  Star = strrchr( Name, '*' );

  if ( Star != (char *)NULL )
  {
    XvpnSearchNumber = (int)(Star - Name);
  }
  else
  {
    XvpnSearchNumber = 0;
  }

  XvpnSearchString = Name;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnEditSearchCompare                    |
|                                                             |
\------------------------------------------------------------*/

int XvpnEditSearchCompare( Name )

  char *Name;
{
  if ( Name != (char *)NULL )
  {
    if ( XvpnSearchNumber )
    {
      return( ! strncmp( XvpnSearchString, Name, XvpnSearchNumber ) );
    }
    else
    {
      return( Name == XvpnSearchString );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XvpnEditSearchObject                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnEditSearchObject( ObjectName )

  char *ObjectName;
{
  xvpnobj_list    *Obj;
  xvpnselect_list *Search;
  short            Layer;
  long             X1;
  long             Y1;

  autbegin();

  if ( XvpnHeadConnect != (xvpnselect_list *)NULL )
  {
    XvpnDelConnect();
    XvpnZoomRefresh();
  }

  XvpnEditInitializeSearch( ObjectName );

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    if ( XVPN_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XvpnFigure->TEXT[ Layer ];
          Obj != (xvpnobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( ! IsXvpnSelect( Obj ) )
      {
        if ( XvpnEditSearchCompare( Obj->NAME ) )
        {
          XvpnAddSelect( Obj );
        }
      }
    }
  }

  for ( Search  = XvpnHeadSelect;
        Search != (xvpnselect_list *)NULL;
        Search  = Search->NEXT )
  {
    XvpnUnselectObject( Search->OBJECT );
  }

  if ( XvpnHeadSelect == (xvpnselect_list *)NULL )
  {
    XvpnWarningMessage( XvpnMainWindow, "No object found !" );
  }
  else
  {
    XvpnSearchAbort = XVPN_FALSE;

    for ( Search  = XvpnHeadSelect;
          Search != (xvpnselect_list *)NULL;
          Search  = Search->NEXT )
    {
      Obj = Search->OBJECT;

      XvpnAddConnect( Obj );

      if ( XvpnSearchAbort ) continue;

      X1 = ( Obj->X + ( Obj->DX >> 1 ) ) / XVPN_UNIT;
      Y1 = ( Obj->Y + ( Obj->DY >> 1 ) ) / XVPN_UNIT;

      XvpnZoomCenter( X1, Y1 );

      if ( Search->NEXT != (xvpnselect_list *)NULL )
      {
        XvpnEnterPanel( &XvpnEditSearchViewPanel );
        XvpnLimitedLoop( XvpnEditSearchViewPanel.PANEL );
      }
    }

    XvpnDelSelect();

    XvpnExitPanel( &XvpnEditSearchViewPanel );
  }

  autend();
}
