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
# include "XSB.h"
# include "XGR.h"
# include "XGR_bound.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  long         XgraBoundXmin   = 0;
  long         XgraBoundYmin   = 0;
  long         XgraBoundXmax   = 0;
  long         XgraBoundYmax   = 0;

  char         XgraRecomputeBound = XGRA_TRUE; 

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XgraComputeBound                       |
|                                                             |
\------------------------------------------------------------*/

char XgraComputeBound()

{
  xgraobj_list *Obj;
  short         Layer;
  short         FirstBound;

  if ( XgraFigure == (xautgraph *)NULL )
  {
    return( XGRA_FALSE );
  }

  if ( XgraRecomputeBound == XGRA_FALSE )
  {
    return( XGRA_TRUE );
  }

  autbegin();

  FirstBound = 1;

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    for ( Obj  = XgraFigure->OBJECT[ Layer ];
          Obj != (xgraobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( FirstBound )
      {
        XgraBoundXmin   = Obj->X;
        XgraBoundYmin   = Obj->Y;
        XgraBoundXmax   = Obj->X + Obj->DX;
        XgraBoundYmax   = Obj->Y + Obj->DY;
        FirstBound      = 0;
      }
      else
      {
        if ( Obj->X < XgraBoundXmin )
        {
          XgraBoundXmin = Obj->X;
        }

        if ( Obj->Y < XgraBoundYmin )
        {
          XgraBoundYmin = Obj->Y;
        }

        if ( ( Obj->X + Obj->DX ) > XgraBoundXmax )
        {
          XgraBoundXmax = Obj->X + Obj->DX;
        }

        if ( ( Obj->Y + Obj->DY ) > XgraBoundYmax )
        {
          XgraBoundYmax = Obj->Y + Obj->DY;
        }
      }
    }
  }

  XgraBoundXmin = XgraBoundXmin - ( XGRA_UNIT << 1 );
  XgraBoundYmin = XgraBoundYmin - ( XGRA_UNIT << 1 );
  XgraBoundXmax = XgraBoundXmax + ( XGRA_UNIT << 1 );
  XgraBoundYmax = XgraBoundYmax + ( XGRA_UNIT << 1 );

  XgraRecomputeBound = XGRA_FALSE;

  autend();
  return( XGRA_TRUE );
}
