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
| Tool    :                   XPAT                            |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
# include "XPT_bound.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  long         XpatBoundXmin   = 0;
  long         XpatBoundYmin   = 0;
  long         XpatBoundXmax   = 0;
  long         XpatBoundYmax   = 0;

  char         XpatRecomputeBound = XPAT_TRUE; 

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XpatComputeBound                        |
|                                                             |
\------------------------------------------------------------*/

char XpatComputeBound()

{
  xpatobj_list *Obj;
  short         Layer;
  short         FirstBound;

  if ( XpatFigure == (xpatfig_list *)NULL )
  {
    return( XPAT_FALSE );
  }

  if ( XpatRecomputeBound == XPAT_FALSE )
  {
    return( XPAT_TRUE );
  }

  autbegin();

  FirstBound = 1;

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    for ( Obj  = XpatFigure->OBJECT[ Layer ];
          Obj != (xpatobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( FirstBound )
      {
        XpatBoundXmin   = Obj->X;
        XpatBoundYmin   = Obj->Y;
        XpatBoundXmax   = Obj->X + Obj->DX;
        XpatBoundYmax   = Obj->Y + Obj->DY;
        FirstBound      = 0;
      }
      else
      {
        if ( Obj->X < XpatBoundXmin )
        {
          XpatBoundXmin = Obj->X;
        }

        if ( Obj->Y < XpatBoundYmin )
        {
          XpatBoundYmin = Obj->Y;
        }

        if ( ( Obj->X + Obj->DX ) > XpatBoundXmax )
        {
          XpatBoundXmax = Obj->X + Obj->DX;
        }

        if ( ( Obj->Y + Obj->DY ) > XpatBoundYmax )
        {
          XpatBoundYmax = Obj->Y + Obj->DY;
        }
      }
    }
  }

  XpatBoundXmin = XpatBoundXmin - ( XPAT_UNIT << 1 );
  XpatBoundYmin = XpatBoundYmin - ( XPAT_UNIT << 1 );
  XpatBoundXmax = XpatBoundXmax + ( XPAT_UNIT << 1 );
  XpatBoundYmax = XpatBoundYmax + ( XPAT_UNIT << 1 );

  XpatRecomputeBound = XPAT_FALSE;

  autend();
  return( XPAT_TRUE );
}
