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
| File    :                  Bound.c                          |
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
# include <stdlib.h>
# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "XSB.h"
# include "XVP.h"
# include "XVP_bound.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  long         XvpnBoundXmin   = 0;
  long         XvpnBoundYmin   = 0;
  long         XvpnBoundXmax   = 0;
  long         XvpnBoundYmax   = 0;

  char         XvpnRecomputeBound = XVPN_TRUE; 

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XvpnComputeBound                       |
|                                                             |
\------------------------------------------------------------*/

char XvpnComputeBound()

{
  xvpnobj_list *Obj;
  short         Layer;
  short         FirstBound;

  if ( XvpnFigure == (xvpnfig_list *)NULL )
  {
    return( XVPN_FALSE );
  }

  if ( XvpnRecomputeBound == XVPN_FALSE )
  {
    return( XVPN_TRUE );
  }

  autbegin();

  FirstBound = 1;

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    for ( Obj  = XvpnFigure->OBJECT[ Layer ];
          Obj != (xvpnobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( FirstBound )
      {
        XvpnBoundXmin   = Obj->X;
        XvpnBoundYmin   = Obj->Y;
        XvpnBoundXmax   = Obj->X + Obj->DX;
        XvpnBoundYmax   = Obj->Y + Obj->DY;
        FirstBound      = 0;
      }
      else
      {
        if ( Obj->X < XvpnBoundXmin )
        {
          XvpnBoundXmin = Obj->X;
        }

        if ( Obj->Y < XvpnBoundYmin )
        {
          XvpnBoundYmin = Obj->Y;
        }

        if ( ( Obj->X + Obj->DX ) > XvpnBoundXmax )
        {
          XvpnBoundXmax = Obj->X + Obj->DX;
        }

        if ( ( Obj->Y + Obj->DY ) > XvpnBoundYmax )
        {
          XvpnBoundYmax = Obj->Y + Obj->DY;
        }
      }
    }
  }

  XvpnBoundXmin = XvpnBoundXmin - ( XVPN_UNIT << 1 );
  XvpnBoundYmin = XvpnBoundYmin - ( XVPN_UNIT << 1 );
  XvpnBoundXmax = XvpnBoundXmax + ( XVPN_UNIT << 1 );
  XvpnBoundYmax = XvpnBoundYmax + ( XVPN_UNIT << 1 );

  XvpnRecomputeBound = XVPN_FALSE;

  autend();
  return( XVPN_TRUE );
}
