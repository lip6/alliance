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
| Tool    :                   XPAT                            |
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
# include "pat.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XPT.h"
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

  char  XpatSearchAbort;
  int   XpatSearchNumber;
  char *XpatSearchString;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XpatEditInitializeSearch                 |
|                                                             |
\------------------------------------------------------------*/

void XpatEditInitializeSearch( Name )

  char *Name;
{
  char *Star;

  autbegin();

  Star = strrchr( Name, '*' );

  if ( Star != (char *)NULL )
  {
    XpatSearchNumber = (int)(Star - Name);
  }
  else
  {
    XpatSearchNumber = 0;
  }

  XpatSearchString = Name;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatEditSearchCompare                    |
|                                                             |
\------------------------------------------------------------*/

int XpatEditSearchCompare( Name )

  char *Name;
{
  if ( Name != (char *)NULL )
  {
    if ( XpatSearchNumber )
    {
      return( ! strncmp( XpatSearchString, Name, XpatSearchNumber ) );
    }
    else
    {
      return( Name == XpatSearchString );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatEditSearchObject                  |
|                                                             |
\------------------------------------------------------------*/

void XpatEditSearchObject( ObjectName )

  char *ObjectName;
{
  xpatobj_list    *Obj;
  xpatselect_list *Search;
  short            Layer;
  long             X1;
  long             Y1;

  autbegin();

  if ( XpatHeadConnect != (xpatselect_list *)NULL )
  {
    XpatDelConnect();
    XpatZoomRefresh();
  }

  XpatEditInitializeSearch( ObjectName );

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    if ( XPAT_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XpatFigure->OBJECT[ Layer ];
          Obj != (xpatobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( ( ! IsXpatSelect( Obj ) ) &&
           ( ( IsXpatVector( Obj ) ) ||
             ( IsXpatIO    ( Obj ) ) ) )
      {
        if ( XpatEditSearchCompare( Obj->NAME ) )
        {
          XpatAddSelect( Obj );
        }
      }
    }
  }

  for ( Search  = XpatHeadSelect;
        Search != (xpatselect_list *)NULL;
        Search  = Search->NEXT )
  {
    XpatUnselectObject( Search->OBJECT );
  }

  if ( XpatHeadSelect == (xpatselect_list *)NULL )
  {
    XpatWarningMessage( XpatMainWindow, "No object found !" );
  }
  else
  {
    XpatSearchAbort = XPAT_FALSE;

    for ( Search  = XpatHeadSelect;
          Search != (xpatselect_list *)NULL;
          Search  = Search->NEXT )
    {
      Obj = Search->OBJECT;

      XpatAddConnect( Obj );

      if ( XpatSearchAbort ) continue;

      X1 = ( Obj->X + ( Obj->DX >> 1 ) ) / XPAT_UNIT;
      Y1 = ( Obj->Y + ( Obj->DY >> 1 ) ) / XPAT_UNIT;

      XpatZoomCenter( X1, Y1 );

      if ( Search->NEXT != (xpatselect_list *)NULL )
      {
        XpatEnterPanel( &XpatEditSearchViewPanel );
        XpatLimitedLoop( XpatEditSearchViewPanel.PANEL );
      }
    }

    XpatDelSelect();

    XpatExitPanel( &XpatEditSearchViewPanel );
  }

  autend();
}
