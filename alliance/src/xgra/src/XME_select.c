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
| Tool    :                    XGRA                           |
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
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XGR.h"
# include "XME.h"
# include "XMV.h"

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

  static char XgraSelectBuffer [ XGRA_SELECT_BUFFER_SIZE  ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XgraAddSelectList                   |
|                                                             |
\------------------------------------------------------------*/

void XgraAddSelectList( Obj )

  xgraobj_list *Obj;
{
  autnode_list *Node;
  autarc_list *Arc;

  autbegin();

  XgraSelectBuffer[0] = '\0';

  if ( IsXgraNode( Obj ) )
  {
    Node = (autnode_list *)( Obj->USER );

    sprintf( XgraSelectBuffer, "  NODE %s", Node->NAME );
  }
  else
  if ( IsXgraArc( Obj ) )
  {
    Arc = (autarc_list *)( Obj->USER );

    sprintf( XgraSelectBuffer, "  ARC %s -> %s",
             Arc->NODE_FROM->NAME, Arc->NODE_TO->NAME );
  }

  XgraDisplaySelectList( XgraSelectBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XgraEditSelectObject                       |
|                                                             |
\------------------------------------------------------------*/

void XgraEditSelectObject( Number )

  int Number;
{
  xgraselect_list *Select;
  int              Counter;

  autbegin();

  Select = XgraHeadSelect;

  for ( Counter = 0; Counter < Number; Counter++ )
  {
    Select = Select->NEXT;
  }

  if ( IsXgraAccept( Select->OBJECT ) )
  {
    XgraRejectObject( Select->OBJECT );
  }
  else
  {
    XgraAcceptObject( Select->OBJECT );
  }

  XgraDisplayObject( Select->OBJECT );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XgraEditSelectAccept                       |
|                                                             |
\------------------------------------------------------------*/

void XgraEditSelectAccept()
{
  autbegin();

  XgraPurgeSelect();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XgraEditSelectCancel                       |
|                                                             |
\------------------------------------------------------------*/

void XgraEditSelectCancel()
{
  autbegin();

  XgraEditUnselectAll();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Select Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XgraEditUnselectAll                    |
|                                                             |
\------------------------------------------------------------*/

void XgraEditUnselectAll()
{
  xgraobj_list *Obj;

  autbegin();

  if ( XgraHeadSelect != (xgraselect_list *)NULL )
  {
    if ( XgraHeadSelect->NEXT == (xgraselect_list *)NULL )
    {
      Obj = XgraHeadSelect->OBJECT;
      XgraDelSelect();
      XgraDisplayObject( Obj );
    }
    else
    {
      XgraDelSelect();
      XgraZoomRefresh();
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraEditObjectNearPoint                |
|                                                             |
\------------------------------------------------------------*/

char XgraEditObjectNearPoint( Obj, X1, Y1 )

  xgraobj_list *Obj;
  long          X1;
  long          Y1;
{
  short  Type;
  long   X1r;
  long   Y1r;
  long   X2r;
  long   Y2r;
  float  VectorX;
  float  VectorY;
  float  LineX;
  float  LineY;
  float  Norm;
  float  Distance;

  Type = GetXgraObjectType( Obj );

  if ( ( Obj->X           <= X1 ) &&
       ( Obj->Y           <= Y1 ) &&
       ( Obj->X + Obj->DX >= X1 ) &&
       ( Obj->Y + Obj->DY >= Y1 ) )
  {
    if ( ( Type == XGRA_OBJECT_LINE  ) ||
         ( Type == XGRA_OBJECT_ARROW ) )
    {
      if ( ( Obj->DX == 0 ) && 
           ( Obj->DY == 0 ) )
      {
        if ( ( Obj->X == X1 ) &&
             ( Obj->Y == Y1 ) )
        {
          return( XGRA_TRUE );
        }
      }
      else
      {
        if ( IsXgraLineLeft( Obj ) )
        {
          X1r = Obj->X + Obj->DX;
          X2r = Obj->X;
        }
        else
        {
          X1r = Obj->X;
          X2r = Obj->X + Obj->DX;
        }
    
        if ( IsXgraLineDown( Obj ) )
        {
          Y1r  = Obj->Y + Obj->DY;
          Y2r  = Obj->Y;
        }
        else
        {
          Y1r  = Obj->Y;
          Y2r  = Obj->Y + Obj->DY;
        }
  
        LineX   = (float)( X2r - X1r );
        LineY   = (float)( Y2r - Y1r );
        Norm    = sqrt( ( LineX * LineX ) + ( LineY * LineY ) );
        LineX  /= Norm;
        LineY  /= Norm;
  
        VectorX  = (float)( X2r - X1  );
        VectorY  = (float)( Y2r - Y1  );
  
        Distance = fabs( ( VectorX * LineY ) - ( VectorY * LineX ) );

        if ( Distance < (float) XGRA_UNIT )
        {
          return( XGRA_TRUE );
        }
      }
    }
    else
    {
      return( XGRA_TRUE );
    }
  }

  return( XGRA_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraEditSelectPoint                   |
|                                                             |
\------------------------------------------------------------*/

void XgraEditSelectPoint( X1, Y1 )

  long X1;
  long Y1;
{
  xgraobj_list    *Obj;
  xgraselect_list *Select;
  int              Layer;

  X1 = X1 * XGRA_UNIT;
  Y1 = Y1 * XGRA_UNIT;

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    if ( XGRA_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XgraFigure->OBJECT[ Layer ];
          Obj != (xgraobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XGRA_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ! IsXgraSelect( Obj ) )
      {
        if ( XgraEditObjectNearPoint( Obj, X1, Y1 ) )
        {
          XgraAddSelect( Obj );
        }
      }
    }
  }

  for ( Select  = XgraHeadSelect;
        Select != (xgraselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XgraUnselectObject( Select->OBJECT );
  }
}
