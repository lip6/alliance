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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                  Search.c                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
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
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XSC.h"
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

  char  XschSearchAbort;
  int   XschSearchNumber;
  char *XschSearchString;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XschEditInitializeSearch                 |
|                                                             |
\------------------------------------------------------------*/

void XschEditInitializeSearch( Name )

  char *Name;
{
  char *Star;

  autbegin();

  Star = strrchr( Name, '*' );

  if ( Star != (char *)NULL )
  {
    XschSearchNumber = (int)(Star - Name);
  }
  else
  {
    XschSearchNumber = 0;
  }

  XschSearchString = Name;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschEditSearchCompare                    |
|                                                             |
\------------------------------------------------------------*/

int XschEditSearchCompare( Name )

  char *Name;
{
  if ( Name != (char *)NULL )
  {
    if ( XschSearchNumber )
    {
      return( ! strncmp( XschSearchString, Name, XschSearchNumber ) );
    }
    else
    {
      return( Name == XschSearchString );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschEditSearchObject                  |
|                                                             |
\------------------------------------------------------------*/

void XschEditSearchObject( ObjectName )

  char *ObjectName;
{
  xschobj_list    *Obj;
  xschselect_list *Search;
  short            Layer;
  long             X1;
  long             Y1;

  autbegin();

  if ( XschHeadConnect != (xschselect_list *)NULL )
  {
    XschDelConnect();
    XschZoomRefresh();
  }

  XschEditInitializeSearch( ObjectName );

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    if ( XSCH_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XschFigure->OBJECT[ Layer ];
          Obj != (xschobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( ( Obj->NAME != (char *)0 ) &&
           ( ! IsXschSelect( Obj )  ) )
      {
        if ( XschEditSearchCompare( Obj->NAME ) )
        {
          XschAddSelect( Obj );
        }
      }
    }
  }

  for ( Search  = XschHeadSelect;
        Search != (xschselect_list *)NULL;
        Search  = Search->NEXT )
  {
    XschUnselectObject( Search->OBJECT );
  }

  if ( XschHeadSelect == (xschselect_list *)NULL )
  {
    XschWarningMessage( XschMainWindow, "No object found !" );
  }
  else
  {
    XschSearchAbort = XSCH_FALSE;

    for ( Search  = XschHeadSelect;
          Search != (xschselect_list *)NULL;
          Search  = Search->NEXT )
    {
      Obj = Search->OBJECT;

      XschAddConnect( Obj );

      if ( XschSearchAbort ) continue;

      X1 = ( Obj->X + ( Obj->DX >> 1 ) ) / XSCH_UNIT;
      Y1 = ( Obj->Y + ( Obj->DY >> 1 ) ) / XSCH_UNIT;

      XschZoomCenter( X1, Y1 );

      if ( Search->NEXT != (xschselect_list *)NULL )
      {
        XschEnterPanel( &XschEditSearchViewPanel );
        XschLimitedLoop( XschEditSearchViewPanel.PANEL );
      }
    }

    XschDelSelect();

    XschExitPanel( &XschEditSearchViewPanel );
  }

  autend();
}
