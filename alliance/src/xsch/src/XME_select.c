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
| Tool    :                    XSCH                           |
|                                                             |
| File    :                   Edit.c                          |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

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

# include "XME_select.h"
# include "XME_panel.h"

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

  static char XschSelectBuffer [ XSCH_SELECT_BUFFER_SIZE  ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XschAddSelectList                   |
|                                                             |
\------------------------------------------------------------*/

void XschAddSelectList( Obj )

  xschobj_list *Obj;
{
  loins_list  *LoIns;
  losig_list  *LoSig;
  schbox_list *SchBox;
  schnet_list *SchNet;
  schcon_list *SchCon;
  autbegin();

  XschSelectBuffer[0] = '\0';

  if ( IsXschSchBox( Obj ) )
  {
    SchBox = (schbox_list *)Obj->USER;

    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_LOINS )
    {
      LoIns = (loins_list *)SchBox->SOURCE;
      sprintf( XschSelectBuffer, "  Instance %s (%s)", LoIns->INSNAME, LoIns->FIGNAME );
    }
    else
    {
      sprintf( XschSelectBuffer, "  Box %s", SchBox->NAME );
    }
  }
  else
  if ( IsXschSchCon( Obj ) )
  {
    SchCon = (schcon_list *) Obj->USER;
    sprintf( XschSelectBuffer, "  Connector %s", SchCon->NAME );
  }
  else
  if ( IsXschSchNet( Obj ) )
  {
    SchNet = (schnet_list *)Obj->USER;

    if ( SchNet->SOURCE_TYPE == SCH_SOURCE_LOSIG )
    {
      LoSig = (losig_list *)SchNet->SOURCE;

      if ( LoSig->NAMECHAIN == (chain_list *)0 )
      {
        sprintf( XschSelectBuffer, "  Signal %ld", LoSig->INDEX );
      }
      else
      {
        sprintf( XschSelectBuffer, "  Signal %s", (char *)LoSig->NAMECHAIN->DATA );
      }
    }
    else
    {
      strcpy( XschSelectBuffer, "  Net" );
    }
  }

  XschDisplaySelectList( XschSelectBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XschEditSelectObject                       |
|                                                             |
\------------------------------------------------------------*/

void XschEditSelectObject( Number )

  int Number;
{
  xschselect_list *Select;
  int              Counter;

  autbegin();

  Select = XschHeadSelect;

  for ( Counter = 0; Counter < Number; Counter++ )
  {
    Select = Select->NEXT;
  }

  if ( IsXschAccept( Select->OBJECT ) )
  {
    XschRejectObject( Select->OBJECT );
  }
  else
  {
    XschAcceptObject( Select->OBJECT );
  }

  XschDisplayObject( Select->OBJECT );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XschEditSelectAccept                       |
|                                                             |
\------------------------------------------------------------*/

void XschEditSelectAccept()
{
  autbegin();

  XschPurgeSelect();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                  XschEditSelectCancel                       |
|                                                             |
\------------------------------------------------------------*/

void XschEditSelectCancel()
{
  autbegin();

  XschEditUnselectAll();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Select Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XschEditUnselectAll                    |
|                                                             |
\------------------------------------------------------------*/

void XschEditUnselectAll()
{
  xschobj_list *Obj;

  autbegin();

  if ( XschHeadSelect != (xschselect_list *)NULL )
  {
    if ( XschHeadSelect->NEXT == (xschselect_list *)NULL )
    {
      Obj = XschHeadSelect->OBJECT;
      XschDelSelect();
      XschDisplayObject( Obj );
    }
    else
    {
      XschDelSelect();
      XschZoomRefresh();
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatEditObjectNearPoint                |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_SNAP_TOLERANCE (float)(XSCH_UNIT * 8.0)

char XschEditObjectNearPoint( Obj, X1, Y1 )

  xschobj_list *Obj;
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

  Type = GetXschObjectType( Obj );

  if ( ( Type == XSCH_OBJECT_LINE  ) ||
       ( Type == XSCH_OBJECT_ARROW ) )
  {
    if ( IsXschLineLeft( Obj ) )
    {
      X1r = Obj->X + Obj->DX;
      X2r = Obj->X;
    }
    else
    {
      X1r = Obj->X;
      X2r = Obj->X + Obj->DX;
    }
    
    if ( IsXschLineDown( Obj ) )
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
    if ( X1r < X2r )
    {
      if ( ( X1 < X1r ) ||
           ( X1 > X2r ) ) return ( XSCH_FALSE );
    }
    else
    {
      if ( ( X1 < X2r ) ||
           ( X1 > X1r ) ) return ( XSCH_FALSE );
    }

    Delta = Y1 - Y1r;
    if ( Delta < 0 ) Delta = - Delta;

    if ( ( (float)Delta * XschUnitGridStep ) < XSCH_SNAP_TOLERANCE )
    {
      return( XSCH_TRUE );
    }

    return( XSCH_FALSE );
  }
  else
  if ( Obj->DX == 0 )
  {
    if ( Y1r < Y2r )
    {
      if ( ( Y1 < Y1r ) ||
           ( Y1 > Y2r ) ) return ( XSCH_FALSE );
    }
    else
    {
      if ( ( Y1 < Y2r ) ||
           ( Y1 > Y1r ) ) return ( XSCH_FALSE );
    }

    Delta = X1 - X1r;
    if ( Delta < 0 ) Delta = - Delta;

    if ( ( (float)Delta * XschUnitGridStep ) < XSCH_SNAP_TOLERANCE )
    {
      return( XSCH_TRUE );
    }

    return( XSCH_FALSE );
  }
  else
  if ( ( X1r > X1 ) ||
       ( Y1r > Y1 ) ||
       ( X2r < X1 ) ||
       ( Y2r < Y1 ) )
  {
    return( XSCH_FALSE );
  }

  if ( ( Type == XSCH_OBJECT_LINE  ) ||
       ( Type == XSCH_OBJECT_ARROW ) )
  {
    if ( ( Obj->DX == 0 ) && 
         ( Obj->DY == 0 ) )
    {
      if ( ( Obj->X == X1 ) &&
           ( Obj->Y == Y1 ) )
      {
        return( XSCH_TRUE );
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

      if ( ( Distance * XschUnitGridStep ) < XSCH_SNAP_TOLERANCE )
      {
        return( XSCH_TRUE );
      }
    }
  }
  else
  {
    return( XSCH_TRUE );
  }

  return( XSCH_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschEditSelectPoint                   |
|                                                             |
\------------------------------------------------------------*/

void XschEditSelectPoint( X1, Y1 )

  long X1;
  long Y1;
{
  xschobj_list    *Obj;
  xschselect_list *Select;
  int              Layer;

  X1 = X1 * XSCH_UNIT;
  Y1 = Y1 * XSCH_UNIT;

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    if ( XSCH_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XschFigure->OBJECT[ Layer ];
          Obj != (xschobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XSCH_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ! IsXschSelect( Obj ) )
      {
        if ( XschEditObjectNearPoint( Obj, X1, Y1 ) )
        {
          XschAddSelect( Obj );
        }
      }
    }
  }

  for ( Select  = XschHeadSelect;
        Select != (xschselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XschUnselectObject( Select->OBJECT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  XschToolsSelectHierarchyDown              |
|                                                             |
\------------------------------------------------------------*/

void XschToolsSelectHierarchyDown( X1, Y1 )

   long X1;
   long Y1;
{
  xschselect_list  *Select;
  xschselect_list  *DelSelect;
  xschselect_list **Previous;

  autbegin();

  XschEditSelectPoint( X1, Y1 );

  XschDisplaySelectList( (char *)NULL );

  Previous = &XschHeadSelect;
  Select   = XschHeadSelect;

  while ( Select != (xschselect_list *)NULL )
  {
    if ( ! IsXschSchBox( Select->OBJECT ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      XschFreeSelect( DelSelect );
    }
    else
    {
      XschAddSelectList( Select->OBJECT );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( XschHeadSelect == (xschselect_list *)NULL  )
  {
    XschWarningMessage( XschMainWindow, "No instance found !" );
  }
  else
  {
    if ( XschHeadSelect->NEXT != (xschselect_list *)NULL )
    {
      XschEnterPanel( &XschEditSelectPanel );
      XschLimitedLoop( XschEditSelectPanel.PANEL );
    }
    else
    {
      XschAcceptObject( XschHeadSelect->OBJECT );
      XschDisplayObject( XschHeadSelect->OBJECT );
    }

    if ( XschHeadSelect != (xschselect_list *)NULL )
    {
      XschToolsHierarchyDown();
    }
  }

  autend();
}
