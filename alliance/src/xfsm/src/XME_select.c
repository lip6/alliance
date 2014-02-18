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
| Tool    :                    XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XFS.h"
# include "XME.h"

# include "XME_panel.h"
# include "XMV_view.h"
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

  static char XfsmSelectBuffer [ XFSM_SELECT_BUFFER_SIZE  ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XfsmAddSelectList                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmAddSelectList( Obj )

  xfsmobj_list *Obj;
{
  fsmstate_list *State;
  fsmtrans_list *Trans;

  autbegin();

  XfsmSelectBuffer[0] = '\0';

  if ( IsXfsmState( Obj ) )
  {
    State = (fsmstate_list *)( Obj->USER );

    sprintf( XfsmSelectBuffer, "  STATE %s", State->NAME );
  }
  else
  if ( IsXfsmTrans( Obj ) )
  {
    Trans = (fsmtrans_list *)( Obj->USER );

    sprintf( XfsmSelectBuffer, "  TRANSITION %s -> %s",
             Trans->FROM->NAME, Trans->TO->NAME );
  }

  XfsmDisplaySelectList( XfsmSelectBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XfsmEditSelectObject                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditSelectObject( Number )

  int Number;
{
  xfsmselect_list *Select;
  int              Counter;

  autbegin();

  Select = XfsmHeadSelect;

  for ( Counter = 0; Counter < Number; Counter++ )
  {
    Select = Select->NEXT;
  }

  if ( IsXfsmAccept( Select->OBJECT ) )
  {
    XfsmRejectObject( Select->OBJECT );
  }
  else
  {
    XfsmAcceptObject( Select->OBJECT );
  }

  XfsmDisplayObject( Select->OBJECT );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XfsmEditSelectAccept                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditSelectAccept()
{
  autbegin();

  XfsmPurgeSelect();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XfsmEditSelectCancel                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditSelectCancel()
{
  autbegin();

  XfsmEditUnselectAll();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Select Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XfsmEditUnselectAll                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditUnselectAll()
{
  xfsmobj_list *Obj;

  autbegin();

  if ( XfsmHeadSelect != (xfsmselect_list *)NULL )
  {
    if ( XfsmHeadSelect->NEXT == (xfsmselect_list *)NULL )
    {
      Obj = XfsmHeadSelect->OBJECT;
      XfsmDelSelect();
      XfsmDisplayObject( Obj );
    }
    else
    {
      XfsmDelSelect();
      XfsmZoomRefresh();
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmEditObjectNearPoint                |
|                                                             |
\------------------------------------------------------------*/

char XfsmEditObjectNearPoint( Obj, X1, Y1 )

  xfsmobj_list *Obj;
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

  Type = GetXfsmObjectType( Obj );

  if ( ( Obj->X           <= X1 ) &&
       ( Obj->Y           <= Y1 ) &&
       ( Obj->X + Obj->DX >= X1 ) &&
       ( Obj->Y + Obj->DY >= Y1 ) )
  {
    if ( ( Type == XFSM_OBJECT_LINE  ) ||
         ( Type == XFSM_OBJECT_ARROW ) )
    {
      if ( ( Obj->DX == 0 ) && 
           ( Obj->DY == 0 ) )
      {
        if ( ( Obj->X == X1 ) &&
             ( Obj->Y == Y1 ) )
        {
          return( XFSM_TRUE );
        }
      }
      else
      {
        if ( IsXfsmLineLeft( Obj ) )
        {
          X1r = Obj->X + Obj->DX;
          X2r = Obj->X;
        }
        else
        {
          X1r = Obj->X;
          X2r = Obj->X + Obj->DX;
        }
    
        if ( IsXfsmLineDown( Obj ) )
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

        if ( Distance < (float) XFSM_UNIT )
        {
          return( XFSM_TRUE );
        }
      }
    }
    else
    {
      return( XFSM_TRUE );
    }
  }

  return( XFSM_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmEditSelectPoint                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmEditSelectPoint( X1, Y1 )

  long X1;
  long Y1;
{
  xfsmobj_list    *Obj;
  xfsmselect_list *Select;
  int              Layer;

  X1 = X1 * XFSM_UNIT;
  Y1 = Y1 * XFSM_UNIT;

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    if ( XFSM_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XfsmFigure->OBJECT[ Layer ];
          Obj != (xfsmobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XFSM_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ! IsXfsmSelect( Obj ) )
      {
        if ( XfsmEditObjectNearPoint( Obj, X1, Y1 ) )
        {
          XfsmAddSelect( Obj );
        }
      }
    }
  }

  for ( Select  = XfsmHeadSelect;
        Select != (xfsmselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XfsmUnselectObject( Select->OBJECT );
  }
}
