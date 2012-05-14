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
| Tool    :                   XFSM                            |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XFS.h"
# include "XME.h"

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

  char  XfsmSearchAbort;
  int   XfsmSearchNumber;
  char *XfsmSearchString;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XfsmEditInitializeSearch                 |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditInitializeSearch( Name )

  char *Name;
{
  char *Star;

  autbegin();

  Star = strrchr( Name, '*' );

  if ( Star != (char *)NULL )
  {
    XfsmSearchNumber = (int)(Star - Name);
  }
  else
  {
    XfsmSearchNumber = 0;
  }

  XfsmSearchString = Name;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmEditSearchCompare                    |
|                                                             |
\------------------------------------------------------------*/

int XfsmEditSearchCompare( Name )

  char *Name;
{
  if ( Name != (char *)NULL )
  {
    if ( XfsmSearchNumber )
    {
      return( ! strncmp( XfsmSearchString, Name, XfsmSearchNumber ) );
    }
    else
    {
      return( Name == XfsmSearchString );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmEditSearchObject                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditSearchObject( ObjectName )

  char *ObjectName;
{
  xfsmobj_list    *Obj;
  xfsmselect_list *Search;
  short            Layer;
  long             X1;
  long             Y1;

  autbegin();

  if ( XfsmHeadConnect != (xfsmselect_list *)NULL )
  {
    XfsmDelConnect();
    XfsmZoomRefresh();
  }

  XfsmEditInitializeSearch( ObjectName );

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    if ( XFSM_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XfsmFigure->TEXT[ Layer ];
          Obj != (xfsmobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( ! IsXfsmSelect( Obj ) )
      {
        if ( XfsmEditSearchCompare( Obj->NAME ) )
        {
          XfsmAddSelect( Obj );
        }
      }
    }
  }

  for ( Search  = XfsmHeadSelect;
        Search != (xfsmselect_list *)NULL;
        Search  = Search->NEXT )
  {
    XfsmUnselectObject( Search->OBJECT );
  }

  if ( XfsmHeadSelect == (xfsmselect_list *)NULL )
  {
    XfsmWarningMessage( XfsmMainWindow, "No object found !" );
  }
  else
  {
    XfsmSearchAbort = XFSM_FALSE;

    for ( Search  = XfsmHeadSelect;
          Search != (xfsmselect_list *)NULL;
          Search  = Search->NEXT )
    {
      Obj = Search->OBJECT;

      XfsmAddConnect( Obj );

      if ( XfsmSearchAbort ) continue;

      X1 = ( Obj->X + ( Obj->DX >> 1 ) ) / XFSM_UNIT;
      Y1 = ( Obj->Y + ( Obj->DY >> 1 ) ) / XFSM_UNIT;

      XfsmZoomCenter( X1, Y1 );

      if ( Search->NEXT != (xfsmselect_list *)NULL )
      {
        XfsmEnterPanel( &XfsmEditSearchViewPanel );
        XfsmLimitedLoop( XfsmEditSearchViewPanel.PANEL );
      }
    }

    XfsmDelSelect();

    XfsmExitPanel( &XfsmEditSearchViewPanel );
  }

  autend();
}
