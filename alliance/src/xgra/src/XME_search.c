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
| Tool    :                   XGRA                            |
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
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XGR.h"
# include "XME.h"
# include "XMV.h"

# include "XME_search.h"
# include "XTB_dialog.h"

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

  char  XgraSearchAbort;
  int   XgraSearchNumber;
  char *XgraSearchString;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XgraEditInitializeSearch                 |
|                                                             |
\------------------------------------------------------------*/

void XgraEditInitializeSearch( Name )

  char *Name;
{
  char *Star;

  autbegin();

  Star = strrchr( Name, '*' );

  if ( Star != (char *)NULL )
  {
    XgraSearchNumber = (int)(Star - Name);
  }
  else
  {
    XgraSearchNumber = 0;
  }

  XgraSearchString = Name;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraEditSearchCompare                    |
|                                                             |
\------------------------------------------------------------*/

int XgraEditSearchCompare( Name )

  char *Name;
{
  if ( Name != (char *)NULL )
  {
    if ( XgraSearchNumber )
    {
      return( ! strncmp( XgraSearchString, Name, XgraSearchNumber ) );
    }
    else
    {
      return( Name == XgraSearchString );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraEditSearchObject                  |
|                                                             |
\------------------------------------------------------------*/

void XgraEditSearchObject( ObjectName )

  char *ObjectName;
{
  xgraobj_list    *Obj;
  xgraselect_list *Search;
  short            Layer;
  long             X1;
  long             Y1;

  autbegin();

  if ( XgraHeadConnect != (xgraselect_list *)NULL )
  {
    XgraDelConnect();
    XgraZoomRefresh();
  }

  XgraEditInitializeSearch( ObjectName );

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    if ( XGRA_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XgraFigure->TEXT[ Layer ];
          Obj != (xgraobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( ! IsXgraSelect( Obj ) )
      {
        if ( XgraEditSearchCompare( Obj->NAME ) )
        {
          XgraAddSelect( Obj );
        }
      }
    }
  }

  for ( Search  = XgraHeadSelect;
        Search != (xgraselect_list *)NULL;
        Search  = Search->NEXT )
  {
    XgraUnselectObject( Search->OBJECT );
  }

  if ( XgraHeadSelect == (xgraselect_list *)NULL )
  {
    XgraWarningMessage( XgraMainWindow, "No object found !" );
  }
  else
  {
    XgraSearchAbort = XGRA_FALSE;

    for ( Search  = XgraHeadSelect;
          Search != (xgraselect_list *)NULL;
          Search  = Search->NEXT )
    {
      Obj = Search->OBJECT;

      XgraAddConnect( Obj );

      if ( XgraSearchAbort ) continue;

      X1 = ( Obj->X + ( Obj->DX >> 1 ) ) / XGRA_UNIT;
      Y1 = ( Obj->Y + ( Obj->DY >> 1 ) ) / XGRA_UNIT;

      XgraZoomCenter( X1, Y1 );

      if ( Search->NEXT != (xgraselect_list *)NULL )
      {
        XgraEnterPanel( &XgraEditSearchViewPanel );
        XgraLimitedLoop( XgraEditSearchViewPanel.PANEL );
      }
    }

    XgraDelSelect();

    XgraExitPanel( &XgraEditSearchViewPanel );
  }

  autend();
}
