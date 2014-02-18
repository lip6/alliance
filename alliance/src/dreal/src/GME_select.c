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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Select.c                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GME.h"
# include "GMT.h"
# include "GMV.h"

# include "GME_panel.h"
# include "GME_select.h"
# include "GME_message.h"
# include "GRD_select.h"
# include "GTB_dialog.h"

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

  static char DrealSelectBuffer [ DREAL_SELECT_BUFFER_SIZE  ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealAddSelectList                     |
|                                                             |
\------------------------------------------------------------*/

void DrealAddSelectList( Rectangle )
 
  rdsrec_list *Rectangle;
{
  char StaticLayer;

  rdsbegin();

  DrealSelectBuffer[0] = '\0';

  if ( IsRdsInstance( Rectangle ) )
  {
    sprintf( DrealSelectBuffer,
    "  INSTANCE %s", Rectangle->NAME );
  }
  else
  if ( IsRdsFigure( Rectangle ) )
  {
    sprintf( DrealSelectBuffer,
    "  ABUTMENT BOX : %s", Rectangle->NAME );
  }
  else
  {
    StaticLayer = RDS_STATIC_LAYER[ GetRdsLayer( Rectangle ) ];

    sprintf( DrealSelectBuffer, 
    "  RECTANGLE %s : %s",
      ( Rectangle->NAME != (char *)NULL ) ? Rectangle->NAME : "None",
      DREAL_RDS_LAYER_NAME_TABLE[ (int)StaticLayer ][0] );
  }

  DrealDisplaySelectList( DrealSelectBuffer );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealEditSelectRectangle                    |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectRectangle( Number )

  int Number;
{
  int          Counter;
  drealselect *Select;

  rdsbegin();

  Select = DrealHeadSelect;

  for ( Counter = 0; Counter < Number; Counter++ )
  {
    Select = Select->NEXT;
  }

  if ( IsDrealAccepted( Select->RECTANGLE ) )
  {
    DrealRejectRectangle( Select->RECTANGLE );
  }
  else
  {
    DrealAcceptRectangle( Select->RECTANGLE );
  }

  DrealDisplayRectangle( Select->RECTANGLE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealEditSelectAccept                       |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectAccept()
{
  rdsbegin();

  DrealPurgeSelect();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealEditSelectCancel                       |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectCancel()
{
  rdsbegin();

  DrealEditUnselectAll();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Select Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     DrealEditUnselectAll                    |
|                                                             |
\------------------------------------------------------------*/

void DrealEditUnselectAll()
{
  rdsrec_list *Rectangle;

  rdsbegin();

  if ( DrealHeadSelect != (drealselect *)NULL )
  {
    if ( DrealHeadSelect->NEXT == (drealselect *)NULL )
    {
      Rectangle = DrealHeadSelect->RECTANGLE;
      DrealDelSelect();
      DrealDisplayRectangle( Rectangle );
    }
    else
    {
      DrealDelSelect();
      DrealZoomRefresh();
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealEditSelectPoint                    |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectPoint( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  long         Offset;
  drealwin    *ScanWin;
  drealwinrec *ScanWinRec;
  rdsrec_list *Rec;
  drealselect *Select;
  char         ScanRec;
  char         Layer;
  char         StaticLayer;
  long         X;
  long         Y;

  rdsbegin();

  if ( DrealFigureRds != (rdsfig_list *)NULL ) 
  {
    LambdaX1 = LambdaX1 * RDS_PHYSICAL_GRID; 
    LambdaY1 = LambdaY1 * RDS_PHYSICAL_GRID; 

    X = ( LambdaX1 - DrealWindowXmin ) / DrealWindowSide;
    Y = ( LambdaY1 - DrealWindowYmin ) / DrealWindowSide;

    if ( X < 0             ) X = 0;
    if ( X > DrealWindowDx ) X = DrealWindowDx - 1;
    if ( Y < 0             ) Y = 0;
    if ( Y > DrealWindowDy ) Y = DrealWindowDy - 1;

    Offset = Y * DrealWindowDx + X;

    ScanWin = &DrealWindowTable[ Offset ];

    if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
      {
        StaticLayer = RDS_STATIC_LAYER[ (int)Layer ];

        if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ (int)StaticLayer ] != 1 ) continue; 

        for ( ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];
              ScanWinRec != (drealwinrec *)NULL;
              ScanWinRec  = ScanWinRec->NEXT )
        {
          for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
          {
            Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

            if ( ( Rec != (rdsrec_list *)NULL        ) &&
                 ( ! IsDrealDeleted( Rec )           ) &&
                 ( ! IsDrealSelected( Rec )          ) &&
                 ( Rec->X               <=  LambdaX1 ) &&
                 ( Rec->Y               <=  LambdaY1 ) &&
                 ( ( Rec->X + Rec->DX ) >=  LambdaX1 ) &&
                 ( ( Rec->Y + Rec->DY ) >=  LambdaY1 ) )
            {
              if ( IsRdsFigRec( Rec ) )
              {
                if ( Layer == RDS_ABOX )
                {
                  if ( (   Rec->X             == LambdaX1 ) ||
                       ( ( Rec->X + Rec->DX ) == LambdaX1 ) )
                  {
                    if ( ( LambdaY1 <     Rec->Y           ) ||
                         ( LambdaY1 > ( Rec->Y + Rec->DY ) ) )
        
                      continue;
                  }
                  else
                  if ( (   Rec->Y             == LambdaY1 ) ||
                       ( ( Rec->Y + Rec->DY ) == LambdaY1 ) )
                  {
                    if ( ( LambdaX1 <   Rec->X             ) ||
                         ( LambdaX1 > ( Rec->X + Rec->DX ) ) )
        
                      continue;
                  }
                  else continue;
                }

                DrealAddSelect( Rec );
              }
              else
              {
                if ( Layer == RDS_ABOX )
                {
                  DrealAddSelect( Rec );
                }
              }
            }
          }
        }
      }
    }

    for ( Select  = DrealHeadSelect;
          Select != (drealselect *)NULL;
          Select  = Select->NEXT )
    {
      DrealUnselectRectangle( Select->RECTANGLE );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealEditSelectWindow                  |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
{
  long         Offset;
  drealwin    *ScanWin;
  drealwinrec *ScanWinRec;
  rdsrec_list *Rec;
  drealselect *Select;
  char         ScanRec;
  char         Layer;
  char         StaticLayer;
  long         Xmin;
  long         Ymin;
  long         Xmax;
  long         Ymax;
  long         X;
  long         Y;
  long         Swap;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) &&
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 )
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 )
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }
  }

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    LambdaX1 = LambdaX1 * RDS_PHYSICAL_GRID; 
    LambdaX2 = LambdaX2 * RDS_PHYSICAL_GRID; 
    LambdaY1 = LambdaY1 * RDS_PHYSICAL_GRID; 
    LambdaY2 = LambdaY2 * RDS_PHYSICAL_GRID; 

    Xmin = ( LambdaX1 - DrealWindowXmin ) / DrealWindowSide;
    Xmax = ( LambdaX2 - DrealWindowXmin ) / DrealWindowSide;
    Ymin = ( LambdaY1 - DrealWindowYmin ) / DrealWindowSide;
    Ymax = ( LambdaY2 - DrealWindowYmin ) / DrealWindowSide;

    if ( Xmin < 0             ) Xmin = 0;
    if ( Xmin > DrealWindowDx ) Xmin = DrealWindowDx - 1;
    if ( Ymin < 0             ) Ymin = 0;
    if ( Ymin > DrealWindowDy ) Ymin = DrealWindowDy - 1;

    if ( Xmax < 0             ) Xmax = 0;
    if ( Xmax > DrealWindowDx ) Xmax = DrealWindowDx - 1;
    if ( Ymax < 0             ) Ymax = 0;
    if ( Ymax > DrealWindowDy ) Ymax = DrealWindowDy - 1;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
    {
      StaticLayer = RDS_STATIC_LAYER[ (int)Layer ];

      if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ (int)StaticLayer ] != 1 ) continue; 

      Y = Ymin;

      while ( Y <= Ymax )
      {
        X = Xmin;

        while ( X <= Xmax )
        {
          Offset = ( Y * DrealWindowDx ) + X;

          ScanWin = &DrealWindowTable[ Offset ];

          if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
          {
            for ( ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];
                  ScanWinRec != (drealwinrec *)NULL;
                  ScanWinRec  = ScanWinRec->NEXT )
            {
              for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
              {
                Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

                if ( ( Rec != (rdsrec_list *)NULL        ) &&
                     ( ! IsDrealDeleted( Rec )           ) &&
                     ( ! IsDrealAccepted( Rec )          ) &&
                     ( ! IsDrealSelected( Rec )          ) &&
                     ( Rec->X               >=  LambdaX1 ) &&
                     ( Rec->Y               >=  LambdaY1 ) &&
                     ( ( Rec->X + Rec->DX ) <=  LambdaX2 ) &&
                     ( ( Rec->Y + Rec->DY ) <=  LambdaY2 ) )
                {
                  if ( ( IsRdsFigRec( Rec   ) ) || 
                       ( IsRdsInstance( Rec ) ) )
                  {
                    DrealAddSelect( Rec );
                  }
                }
              }
            }
          }

          X = X + 1;
        }

        Y = Y + 1;
      }
    }

    for ( Select  = DrealHeadSelect;
          Select != (drealselect *)NULL;
          Select  = Select->NEXT )
    {
      DrealUnselectRectangle( Select->RECTANGLE );
    }
  }   

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealEditSelectFence                    |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectFence( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
{
  long         Offset;
  drealwin    *ScanWin;
  drealwinrec *ScanWinRec;
  rdsrec_list *Rec;
  drealselect *Select;
  char         ScanRec;
  char         Layer;
  char         StaticLayer;
  long         Xmin;
  long         Ymin;
  long         Xmax;
  long         Ymax;
  long         X;
  long         Y;
  long         Swap;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) &&
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 )
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 )
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }
  }

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    LambdaX1 = LambdaX1 * RDS_PHYSICAL_GRID; 
    LambdaX2 = LambdaX2 * RDS_PHYSICAL_GRID; 
    LambdaY1 = LambdaY1 * RDS_PHYSICAL_GRID; 
    LambdaY2 = LambdaY2 * RDS_PHYSICAL_GRID; 

    Xmin = ( LambdaX1 - DrealWindowXmin ) / DrealWindowSide;
    Xmax = ( LambdaX2 - DrealWindowXmin ) / DrealWindowSide;
    Ymin = ( LambdaY1 - DrealWindowYmin ) / DrealWindowSide;
    Ymax = ( LambdaY2 - DrealWindowYmin ) / DrealWindowSide;

    if ( Xmin < 0             ) Xmin = 0;
    if ( Xmin > DrealWindowDx ) Xmin = DrealWindowDx - 1;
    if ( Ymin < 0             ) Ymin = 0;
    if ( Ymin > DrealWindowDy ) Ymin = DrealWindowDy - 1;

    if ( Xmax < 0             ) Xmax = 0;
    if ( Xmax > DrealWindowDx ) Xmax = DrealWindowDx - 1;
    if ( Ymax < 0             ) Ymax = 0;
    if ( Ymax > DrealWindowDy ) Ymax = DrealWindowDy - 1;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
    {
      StaticLayer = RDS_STATIC_LAYER[ (int)Layer ];

      if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ (int)StaticLayer ] != 1 ) continue; 

      Y = Ymin;

      while ( Y <= Ymax )
      {
        X = Xmin;

        while ( X <= Xmax )
        {
          Offset = ( Y * DrealWindowDx ) + X;

          ScanWin = &DrealWindowTable[ Offset ];

          if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
          {
            for ( ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];
                  ScanWinRec != (drealwinrec *)NULL;
                  ScanWinRec  = ScanWinRec->NEXT )
            {
              for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
              {
                Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

                if ( ( Rec != (rdsrec_list *)NULL        ) &&
                     ( ! IsDrealDeleted( Rec )           ) &&
                     ( ! IsDrealAccepted( Rec )          ) &&
                     ( ! IsDrealSelected( Rec )          ) &&
                     ( ( Rec->X + Rec->DX ) >=  LambdaX1 ) &&
                     ( ( Rec->Y + Rec->DY ) >=  LambdaY1 ) &&
                     ( ( Rec->X           ) <=  LambdaX2 ) &&
                     ( ( Rec->Y           ) <=  LambdaY2 ) )
                {
                  if ( ( IsRdsFigRec( Rec   ) ) || 
                       ( IsRdsInstance( Rec ) ) )
                  {
                    DrealAddSelect( Rec );
                  }
                }
              }
            }
          }

          X = X + 1;
        }

        Y = Y + 1;
      }
    }

    for ( Select  = DrealHeadSelect;
          Select != (drealselect *)NULL;
          Select  = Select->NEXT )
    {
      DrealUnselectRectangle( Select->RECTANGLE );
    }
  }

  rdsend();
}


/*------------------------------------------------------------\
|                                                             |
|                  DrealEditSelectPointCopy                   |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectPointCopy( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  drealselect  *Select;
  drealselect  *DelSelect;
  drealselect **Previous;

  rdsbegin();

  DrealEditSelectPoint( LambdaX1, LambdaY1 );

  DrealDisplaySelectList( (char *)NULL );

  Previous = &DrealHeadSelect;
  Select   = DrealHeadSelect;

  while ( Select != (drealselect *)NULL )
  {
    if ( IsRdsFigure( Select->RECTANGLE   ) ||
         IsRdsInstance( Select->RECTANGLE ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      DrealFreeSelect( DelSelect );
    }
    else
    {
      DrealAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );
  }
  else
  {
    if ( DrealHeadSelect->NEXT != (drealselect *)NULL )
    {
      DrealEnterPanel( &DrealEditSelectPanel );
      DrealLimitedLoop( DrealEditSelectPanel.PANEL );
    }
    else
    {
      DrealAcceptRectangle( DrealHeadSelect->RECTANGLE );
      DrealDisplayRectangle( DrealHeadSelect->RECTANGLE );
    }

    if ( DrealHeadSelect != (drealselect *)NULL )
    {
      DrealContinueEditMode( DREAL_EDIT_COPY_POINT,
                             DrealPromptEditCopy, 1 );
    }
    else
    {
      DrealChangeEditMode( DREAL_SELECT_POINT_COPY,
                           DrealPromptSelectPointCopy );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealEditSelectWindowCopy                   |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectWindowCopy( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  drealselect  *Select;
  drealselect  *DelSelect;
  drealselect **Previous;

  rdsbegin();

  DrealEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &DrealHeadSelect;
  Select   = DrealHeadSelect;

  while ( Select != (drealselect *)NULL )
  {
    if ( IsRdsFigure( Select->RECTANGLE   ) ||
         IsRdsInstance( Select->RECTANGLE ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      DrealFreeSelect( DelSelect );
    }
    else
    {
      DrealAcceptRectangle( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );

    DrealChangeEditMode( DREAL_SELECT_WINDOW_COPY,
                         DrealPromptSelectWindowCopy );
  }
  else
  {
    if ( DrealHeadSelect->NEXT == (drealselect *)NULL )
    {
      DrealDisplayRectangle( DrealHeadSelect->RECTANGLE );
    }
    else
    {
      DrealZoomRefresh();
    }

    DrealContinueEditMode( DREAL_EDIT_COPY_WINDOW,
                           DrealPromptEditCopy, 0 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealEditSelectPointMove                   |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectPointMove( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  drealselect  *Select;

  rdsbegin();

  DrealEditSelectPoint( LambdaX1, LambdaY1 );

  DrealDisplaySelectList( (char *)NULL );

  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    DrealAddSelectList( Select->RECTANGLE );
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );
  }
  else
  {
    if ( DrealHeadSelect->NEXT != (drealselect *)NULL )
    {
      DrealEnterPanel( &DrealEditSelectPanel );
      DrealLimitedLoop( DrealEditSelectPanel.PANEL );
    }
    else
    {
      DrealAcceptRectangle( DrealHeadSelect->RECTANGLE );
      DrealDisplayRectangle( DrealHeadSelect->RECTANGLE );
    }

    if ( DrealHeadSelect != (drealselect *)NULL )
    {
      DrealContinueEditMode( DREAL_EDIT_MOVE_POINT,
                             DrealPromptEditMove, 1 );
    }
    else
    {
      DrealChangeEditMode( DREAL_SELECT_POINT_MOVE,
                           DrealPromptSelectPointMove );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealEditSelectWindowMove                   |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectWindowMove( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  drealselect  *Select;

  rdsbegin();

  DrealEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    DrealAcceptRectangle( Select->RECTANGLE );
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );

    DrealChangeEditMode( DREAL_SELECT_WINDOW_MOVE,
                         DrealPromptSelectWindowMove );
  }
  else
  {
    if ( DrealHeadSelect->NEXT == (drealselect *)NULL )
    {
      DrealDisplayRectangle( DrealHeadSelect->RECTANGLE );
    }
    else
    {
      DrealZoomRefresh();
    }

    DrealContinueEditMode( DREAL_EDIT_MOVE_WINDOW,
                           DrealPromptEditMove, 0 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealEditSelectPointDelete                 |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectPointDelete( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  drealselect  *Select;

  rdsbegin();

  DrealEditSelectPoint( LambdaX1, LambdaY1 );

  DrealDisplaySelectList( (char *)NULL );

  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    DrealAddSelectList( Select->RECTANGLE );
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );
  }
  else
  {
    if ( DrealHeadSelect->NEXT != (drealselect *)NULL )
    {
      DrealEnterPanel( &DrealEditSelectPanel );
      DrealLimitedLoop( DrealEditSelectPanel.PANEL );
    }
    else
    {
      DrealAcceptRectangle( DrealHeadSelect->RECTANGLE );
      DrealDisplayRectangle( DrealHeadSelect->RECTANGLE );
    }

    if ( DrealHeadSelect != (drealselect *)NULL ) 
    {
      DrealEditDelete();
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealEditSelectWindowDelete                 |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectWindowDelete( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  drealselect  *Select;

  rdsbegin();

  DrealEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    DrealAcceptRectangle( Select->RECTANGLE );
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );
  }
  else
  {
    if ( DrealHeadSelect->NEXT == (drealselect *)NULL )
    {
      DrealDisplayRectangle( DrealHeadSelect->RECTANGLE );
    }
    else
    {
      DrealZoomRefresh();
    }

    DrealEditDelete();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealEditSelectPointStretch                |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectPointStretch( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  drealselect  *Select;
  drealselect  *DelSelect;
  drealselect **Previous;

  rdsbegin();

  DrealEditSelectPoint( LambdaX1, LambdaY1 );

  DrealDisplaySelectList( (char *)NULL );

  Previous = &DrealHeadSelect;
  Select   = DrealHeadSelect;

  while ( Select != (drealselect *)NULL )
  {
    if ( IsRdsFigRec( Select->RECTANGLE ) )
    {
      DrealAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
    else
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      DrealFreeSelect( DelSelect );
    }
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );
  }
  else
  {
    if ( DrealHeadSelect->NEXT != (drealselect *)NULL )
    {
      DrealEnterPanel( &DrealEditSelectPanel );
      DrealLimitedLoop( DrealEditSelectPanel.PANEL );
    }
    else
    {
      DrealAcceptRectangle( DrealHeadSelect->RECTANGLE );
      DrealDisplayRectangle( DrealHeadSelect->RECTANGLE );
    }

    if ( DrealHeadSelect != (drealselect *)NULL )
    {
      DrealContinueEditMode( DREAL_EDIT_STRETCH_POINT,
                             DrealPromptEditStretch, 1 );
    }
    else
    {
      DrealChangeEditMode( DREAL_SELECT_POINT_STRETCH,
                           DrealPromptSelectPointStretch );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealEditSelectPointModify                 |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectPointModify( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  drealselect  *Select;
  drealselect  *DelSelect;
  drealselect **Previous;

  rdsbegin();

  DrealEditSelectPoint( LambdaX1, LambdaY1 );

  DrealDisplaySelectList( (char *)NULL );

  Previous = &DrealHeadSelect;
  Select   = DrealHeadSelect;

  while ( Select != (drealselect *)NULL )
  {
    if ( ! IsRdsFigure( Select->RECTANGLE   ) )
    {
      DrealAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
    else
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      DrealFreeSelect( DelSelect );
    }
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );
  }
  else
  {
    if ( DrealHeadSelect->NEXT != (drealselect *)NULL )
    {
      DrealEnterPanel( &DrealEditSelectPanel );
      DrealLimitedLoop( DrealEditSelectPanel.PANEL );
    }

    if ( DrealHeadSelect != (drealselect *)NULL )
    {
      DrealEditModify();
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealEditSelectWindowModify                 |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSelectWindowModify( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  drealselect  *Select;
  drealselect  *DelSelect;
  drealselect **Previous;

  rdsbegin();

  DrealEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &DrealHeadSelect;
  Select   = DrealHeadSelect;

  while ( Select != (drealselect *)NULL )
  {
    if ( ! IsRdsFigure( Select->RECTANGLE   ) )
    {
      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
    else
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      DrealFreeSelect( DelSelect );
    }
  }

  if ( DrealHeadSelect == (drealselect *)NULL  )
  {
    DrealWarningMessage( DrealMainWindow, "No element found !" );
  }
  else
  {
    DrealEditModify();
  }

  rdsend();
}
