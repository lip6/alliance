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
| Tool    :                   XFSM                            |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
# include "XFS_bound.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  long         XfsmBoundXmin   = 0;
  long         XfsmBoundYmin   = 0;
  long         XfsmBoundXmax   = 0;
  long         XfsmBoundYmax   = 0;

  char         XfsmRecomputeBound = XFSM_TRUE; 

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     XfsmComputeBound                       |
|                                                             |
\------------------------------------------------------------*/

char XfsmComputeBound()

{
  xfsmobj_list *Obj;
  short         Layer;
  short         FirstBound;

  if ( XfsmFigure == (xfsmfig_list *)NULL )
  {
    return( XFSM_FALSE );
  }

  if ( XfsmRecomputeBound == XFSM_FALSE )
  {
    return( XFSM_TRUE );
  }

  autbegin();

  FirstBound = 1;

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    for ( Obj  = XfsmFigure->OBJECT[ Layer ];
          Obj != (xfsmobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( FirstBound )
      {
        XfsmBoundXmin   = Obj->X;
        XfsmBoundYmin   = Obj->Y;
        XfsmBoundXmax   = Obj->X + Obj->DX;
        XfsmBoundYmax   = Obj->Y + Obj->DY;
        FirstBound      = 0;
      }
      else
      {
        if ( Obj->X < XfsmBoundXmin )
        {
          XfsmBoundXmin = Obj->X;
        }

        if ( Obj->Y < XfsmBoundYmin )
        {
          XfsmBoundYmin = Obj->Y;
        }

        if ( ( Obj->X + Obj->DX ) > XfsmBoundXmax )
        {
          XfsmBoundXmax = Obj->X + Obj->DX;
        }

        if ( ( Obj->Y + Obj->DY ) > XfsmBoundYmax )
        {
          XfsmBoundYmax = Obj->Y + Obj->DY;
        }
      }
    }
  }

  XfsmBoundXmin = XfsmBoundXmin - ( XFSM_UNIT << 1 );
  XfsmBoundYmin = XfsmBoundYmin - ( XFSM_UNIT << 1 );
  XfsmBoundXmax = XfsmBoundXmax + ( XFSM_UNIT << 1 );
  XfsmBoundYmax = XfsmBoundYmax + ( XFSM_UNIT << 1 );

  XfsmRecomputeBound = XFSM_FALSE;

  autend();
  return( XFSM_TRUE );
}
