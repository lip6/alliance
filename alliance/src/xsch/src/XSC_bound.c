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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                  Bound.c                          |
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
# include <stdlib.h>
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h"
# include "XSC.h"
# include "XSC_bound.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  long         XschBoundXmin   = 0;
  long         XschBoundYmin   = 0;
  long         XschBoundXmax   = 0;
  long         XschBoundYmax   = 0;

  char         XschRecomputeBound = XSCH_TRUE; 

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XschComputeBound                       |
|                                                             |
\------------------------------------------------------------*/

char XschComputeBound()

{
  xschobj_list *Obj;
  short         Layer;
  short         FirstBound;

  if ( XschFigure == (xschfig_list *)NULL )
  {
    return( XSCH_FALSE );
  }

  if ( XschRecomputeBound == XSCH_FALSE )
  {
    return( XSCH_TRUE );
  }

  autbegin();

  FirstBound = 1;

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    for ( Obj  = XschFigure->OBJECT[ Layer ];
          Obj != (xschobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( FirstBound )
      {
        XschBoundXmin   = Obj->X;
        XschBoundYmin   = Obj->Y;
        XschBoundXmax   = Obj->X + Obj->DX;
        XschBoundYmax   = Obj->Y + Obj->DY;
        FirstBound      = 0;
      }
      else
      {
        if ( Obj->X < XschBoundXmin )
        {
          XschBoundXmin = Obj->X;
        }

        if ( Obj->Y < XschBoundYmin )
        {
          XschBoundYmin = Obj->Y;
        }

        if ( ( Obj->X + Obj->DX ) > XschBoundXmax )
        {
          XschBoundXmax = Obj->X + Obj->DX;
        }

        if ( ( Obj->Y + Obj->DY ) > XschBoundYmax )
        {
          XschBoundYmax = Obj->Y + Obj->DY;
        }
      }
    }
  }

  XschBoundXmin = XschBoundXmin - ( XSCH_UNIT << 1 );
  XschBoundYmin = XschBoundYmin - ( XSCH_UNIT << 1 );
  XschBoundXmax = XschBoundXmax + ( XSCH_UNIT << 1 );
  XschBoundYmax = XschBoundYmax + ( XSCH_UNIT << 1 );

  XschRecomputeBound = XSCH_FALSE;

  autend();
  return( XSCH_TRUE );
}
