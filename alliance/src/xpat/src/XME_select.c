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
| Tool    :                    XPAT                           |
|                                                             |
| File    :                   Edit.c                          |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XPT.h"
# include "XME.h"

# include "XME_select.h"

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
/*------------------------------------------------------------\
|                                                             |
|                         Select Buffer                       |
|                                                             |
\------------------------------------------------------------*/

  static char XpatSelectBuffer [ XPAT_SELECT_BUFFER_SIZE  ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XpatAddSelectList                   |
|                                                             |
\------------------------------------------------------------*/

void XpatAddSelectList( Obj )

  xpatobj_list *Obj;
{
  autbegin();

  XpatSelectBuffer[0] = '\0';

  XpatDisplaySelectList( XpatSelectBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XpatEditSelectObject                       |
|                                                             |
\------------------------------------------------------------*/

void XpatEditSelectObject( Number )

  int Number;
{
  xpatselect_list *Select;
  int              Counter;

  autbegin();

  Select = XpatHeadSelect;

  for ( Counter = 0; Counter < Number; Counter++ )
  {
    Select = Select->NEXT;
  }

  if ( IsXpatAccept( Select->OBJECT ) )
  {
    XpatRejectObject( Select->OBJECT );
  }
  else
  {
    XpatAcceptObject( Select->OBJECT );
  }

  XpatDisplayObject( Select->OBJECT );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XpatEditSelectAccept                       |
|                                                             |
\------------------------------------------------------------*/

void XpatEditSelectAccept()
{
  autbegin();

  XpatPurgeSelect();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XpatEditSelectCancel                       |
|                                                             |
\------------------------------------------------------------*/

void XpatEditSelectCancel()
{
  autbegin();

  XpatEditUnselectAll();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Select Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XpatEditUnselectAll                    |
|                                                             |
\------------------------------------------------------------*/

void XpatEditUnselectAll()
{
  xpatobj_list *Obj;

  autbegin();

  if ( XpatHeadSelect != (xpatselect_list *)NULL )
  {
    if ( XpatHeadSelect->NEXT == (xpatselect_list *)NULL )
    {
      Obj = XpatHeadSelect->OBJECT;
      XpatDelSelect();
      XpatDisplayObject( Obj );
    }
    else
    {
      XpatDelSelect();
      XpatZoomRefresh();
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatEditObjectNearPoint                |
|                                                             |
\------------------------------------------------------------*/

char XpatEditObjectNearPoint( Obj, X1, Y1 )

  xpatobj_list *Obj;
  long          X1;
  long          Y1;
{
  short  Type;
  long   X1r;
  long   Y1r;
  long   X2r;
  long   Y2r;
  long   Delta;
  float  VectorX;
  float  VectorY;
  float  LineX;
  float  LineY;
  float  Norm;
  float  Distance;
  float  MinDistance;

  Type = GetXpatObjectType( Obj );

  if ( ( Type == XPAT_OBJECT_LINE  ) ||
       ( Type == XPAT_OBJECT_ARROW ) )
  {
    if ( IsXpatLineLeft( Obj ) )
    {
      X1r = Obj->X + Obj->DX;
      X2r = Obj->X;
    }
    else
    {
      X1r = Obj->X;
      X2r = Obj->X + Obj->DX;
    }
    
    if ( IsXpatLineDown( Obj ) )
    {
      Y1r  = Obj->Y + Obj->DY;
      Y2r  = Obj->Y;
    }
    else
    {
      Y1r  = Obj->Y;
      Y2r  = Obj->Y + Obj->DY;
    }
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
    Y1r = Obj->Y;
    Y2r = Obj->Y + Obj->DY;
  }
  
  if ( Obj->DY == 0 )
  {
    if ( ( X1 < X1r ) ||
         ( X1 > X2r ) )
    {
      return( XPAT_FALSE );
    }

    Delta = Y1 - Y1r;
    if ( Delta < 0 ) Delta = - Delta;

    if ( Delta < (XPAT_PATTERN_STEP_Y >> 1) * XPAT_UNIT )
    {
      return( XPAT_TRUE );
    }

    return( XPAT_FALSE );
  }
  else
  if ( Obj->DX == 0 )
  {
    if ( ( Y1 < Y1r ) ||
         ( Y1 > Y2r ) )
    {
      return( XPAT_FALSE );
    }

    Delta = X1 - X1r;
    if ( Delta < 0 ) Delta = - Delta;

    if ( Delta < (XPAT_PATTERN_STEP_Y >> 1) * XPAT_UNIT )
    {
      return( XPAT_TRUE );
    }

    return( XPAT_FALSE );
  }
  else
  if ( ( X1r > X1 ) ||
       ( Y1r > Y1 ) ||
       ( X2r < X1 ) ||
       ( Y2r < Y1 ) )
  {
    return( XPAT_FALSE );
  }

  if ( ( Type == XPAT_OBJECT_LINE  ) ||
       ( Type == XPAT_OBJECT_ARROW ) )
  {
    if ( ( Obj->DX == 0 ) && 
         ( Obj->DY == 0 ) )
    {
      if ( ( Obj->X == X1 ) &&
           ( Obj->Y == Y1 ) )
      {
        return( XPAT_TRUE );
      }
    }
    else
    {
      LineX   = (float)( X2r - X1r );
      LineY   = (float)( Y2r - Y1r );
      Norm    = sqrt( ( LineX * LineX ) + ( LineY * LineY ) );
      LineX  /= Norm;
      LineY  /= Norm;

      VectorX  = (float)( X2r - X1  );
      VectorY  = (float)( Y2r - Y1  );

      Distance = fabs( ( VectorX * LineY ) - ( VectorY * LineX ) );
      Distance = sqrt( Distance );

      if ( Distance < (float)( (XPAT_PATTERN_STEP_Y >> 1) * XPAT_UNIT ) )
      {
        return( XPAT_TRUE );
      }
    }
  }
  else
  {
    return( XPAT_TRUE );
  }

  return( XPAT_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatEditSelectPoint                   |
|                                                             |
\------------------------------------------------------------*/

void XpatEditSelectPoint( X1, Y1 )

  long X1;
  long Y1;
{
  xpatobj_list    *Obj;
  xpatselect_list *Select;
  int              Layer;

  if ( XpatFigure == (xpatfig_list *)0 ) return;

  X1 = X1 * XPAT_UNIT;
  Y1 = Y1 * XPAT_UNIT;

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    if ( XPAT_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XpatFigure->OBJECT[ Layer ];
          Obj != (xpatobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( IsXpatDelete( Obj ) ) continue;

      if ( XPAT_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ! IsXpatSelect( Obj ) )
      {
        if ( XpatEditObjectNearPoint( Obj, X1, Y1 ) )
        {
          XpatAddSelect( Obj );
        }
      }
    }
  }

  for ( Select  = XpatHeadSelect;
        Select != (xpatselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XpatUnselectObject( Select->OBJECT );
  }
}
