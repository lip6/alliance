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
| Tool    :                   GRAAL                           |
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
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GME.h"
# include "GMT.h"

# include "GME_select.h"
# include "GME_message.h"

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

  static char GraalSelectBuffer [ GRAAL_SELECT_BUFFER_SIZE  ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalAddSelectList                     |
|                                                             |
\------------------------------------------------------------*/

void GraalAddSelectList( Rectangle )
 
  rdsrec_list *Rectangle;
{
  void *Pointer;
  int   MbkLayer;
  int   Orient;

  rdsbegin();

  GraalSelectBuffer[0] = '\0';
  Pointer = GRAAL_MBK( Rectangle );

  if ( IsRdsSegment( Rectangle ) )
  {
    MbkLayer = ((phseg_list *)Pointer)->LAYER;

    if ( GET_LYNX_TRANSISTOR_TYPE( MbkLayer ) != RDS_LYNX_TRANSISTOR_EMPTY )
    {
      sprintf( GraalSelectBuffer, 
      "  TRANSISTOR : %s",
      GRAAL_TRANSISTOR_NAME_TABLE[ ((phseg_list *)Pointer)->LAYER ][0] );
    }
    else
    {
      sprintf( GraalSelectBuffer, 
      "  SEGMENT %s : %s",
        ( Rectangle->NAME != (char *)NULL ) ? Rectangle->NAME : "None",
        GRAAL_SEGMENT_NAME_TABLE[ ((phseg_list *)Pointer)->LAYER ][0] );
    }
  }
  else
  if ( IsRdsConnector( Rectangle ) )
  {
    switch( ((phcon_list *)Pointer)->ORIENT )
    {
      case NORTH : Orient = GRAAL_NORTH;
      break;
      case SOUTH : Orient = GRAAL_SOUTH;
      break;
      case EAST  : Orient = GRAAL_EAST;
      break;
      default    : Orient = GRAAL_WEST;
    }

    sprintf( GraalSelectBuffer,
    "  CONNECTOR %s : %s %s",
    ( Rectangle->NAME != (char *)NULL ) ? Rectangle->NAME : "None",
    GRAAL_CONNECTOR_NAME_TABLE[ ((phcon_list *)Pointer)->LAYER ][0],
    GRAAL_ORIENT_NAME_TABLE[ Orient ][0] );
  }
  else
  if ( IsRdsVia( Rectangle ) )
  {
    if ( ( ((phvia_list *)Pointer)->DX == 0 ) &&
         ( ((phvia_list *)Pointer)->DY == 0 ) )
    {
      sprintf( GraalSelectBuffer,
      "  VIA : %s",
      GRAAL_VIA_NAME_TABLE[ ((phvia_list *)Pointer)->TYPE ][0] );
    }
    else
    {
      sprintf( GraalSelectBuffer,
      "  BIGVIA : %s",
      GRAAL_BIGVIA_NAME_TABLE[ ((phvia_list *)Pointer)->TYPE ][0] );
    }
  }
  else
  if ( IsRdsReference( Rectangle ) )
  {
    sprintf( GraalSelectBuffer,
    "  REFERENCE %s : %s",
    ( Rectangle->NAME != (char *)NULL ) ? Rectangle->NAME : "None",
    ( ! strcmp( ((phref_list *)Pointer)->FIGNAME, "ref_ref" ) ) ? "Ref_Ref" : "Ref_Con" );
  }
  else
  if ( IsRdsInstance( Rectangle ) )
  {
    sprintf( GraalSelectBuffer,
    "  INSTANCE %s : %s",
    ((phins_list *)Pointer)->INSNAME,
    ((phins_list *)Pointer)->FIGNAME );
  }
  if ( IsRdsFigure( Rectangle ) )
  {
    sprintf( GraalSelectBuffer,
    "  ABUTMENT BOX : %s",
    ((phfig_list *)Pointer)->NAME );
  }

  GraalDisplaySelectList( GraalSelectBuffer );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEditSelectRectangle                    |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectRectangle( Number )

  int Number;
{
  int          Counter;
  graalselect *Select;

  rdsbegin();

  Select = GraalHeadSelect;

  for ( Counter = 0; Counter < Number; Counter++ )
  {
    Select = Select->NEXT;
  }

  if ( IsGraalAccepted( Select->RECTANGLE ) )
  {
    GraalRejectRectangle( Select->RECTANGLE );
  }
  else
  {
    GraalAcceptRectangle( Select->RECTANGLE );
  }

  GraalDisplayRectangle( Select->RECTANGLE );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEditSelectAccept                       |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectAccept()
{
  rdsbegin();

  GraalPurgeSelect();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEditSelectCancel                       |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectCancel()
{
  rdsbegin();

  GraalEditUnselectAll();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Select Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     GraalEditUnselectAll                    |
|                                                             |
\------------------------------------------------------------*/

void GraalEditUnselectAll()
{
  rdsrec_list *Rectangle;

  rdsbegin();

  if ( GraalHeadSelect != (graalselect *)NULL )
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      Rectangle = GraalHeadSelect->RECTANGLE;
      GraalDelSelect();
      GraalDisplayRectangle( Rectangle );
    }
    else
    {
      GraalDelSelect();
      GraalZoomRefresh();
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalEditSelectPoint                    |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectPoint( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  long         Offset;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  rdsrec_list *Rec;
  rdsrec_list *CheckRec;
  graalselect *Select;
  char         ScanRec;
  char         Layer;
  char         StaticLayer;
  long         X;
  long         Y;

  rdsbegin();

  if ( GraalFigureMbk != (phfig_list *)NULL ) 
  {
    LambdaX1 = LambdaX1 * GRAAL_RDS_LAMBDA; 
    LambdaY1 = LambdaY1 * GRAAL_RDS_LAMBDA; 

    X = ( LambdaX1 - GraalWindowXmin ) / GraalWindowSide;
    Y = ( LambdaY1 - GraalWindowYmin ) / GraalWindowSide;

    if ( X < 0             ) X = 0;
    if ( X > GraalWindowDx ) X = GraalWindowDx - 1;
    if ( Y < 0             ) Y = 0;
    if ( Y > GraalWindowDy ) Y = GraalWindowDy - 1;

    Offset = Y * GraalWindowDx + X;

    ScanWin = &GraalWindowTable[ Offset ];

    if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
      {
        StaticLayer = RDS_STATIC_LAYER[ Layer ];

        if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 ) continue; 

        for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
              ScanWinRec != (graalwinrec *)NULL;
              ScanWinRec  = ScanWinRec->NEXT )
        {
          for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
          {
            Rec = ScanWinRec->RECTAB[ ScanRec ];

            if ( ( Rec != (rdsrec_list *)NULL        ) &&
                 ( ! IsGraalDeleted( Rec )           ) &&
                 ( ! IsGraalCut( Rec )               ) &&
                 ( ! IsGraalSelected( Rec )          ) &&
                 ( ! IsGraalDruc( Rec )              ) &&
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

                CheckRec = (rdsrec_list *)Rec->USER;

                while ( Rec != CheckRec )
                {
                  StaticLayer = RDS_STATIC_LAYER[ GetRdsLayer( CheckRec ) ];

                  if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 )

                    break;
                  
                  CheckRec = (rdsrec_list *)CheckRec->USER;
                }

                if ( Rec == CheckRec )
                {
                  GraalAddSelect( Rec );
                }
              }
              else
              {
                if ( Layer == RDS_ABOX )
                {
                  GraalAddSelect( Rec );
                }
              }
            }
          }
        }
      }
    }

    for ( Select  = GraalHeadSelect;
          Select != (graalselect *)NULL;
          Select  = Select->NEXT )
    {
      GraalUnselectRectangle( Select->RECTANGLE );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalEditSelectWindow                  |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
{
  long         Offset;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  rdsrec_list *Rec;
  rdsrec_list *CheckRec;
  graalselect *Select;
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

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    LambdaX1 = LambdaX1 * GRAAL_RDS_LAMBDA; 
    LambdaX2 = LambdaX2 * GRAAL_RDS_LAMBDA; 
    LambdaY1 = LambdaY1 * GRAAL_RDS_LAMBDA; 
    LambdaY2 = LambdaY2 * GRAAL_RDS_LAMBDA; 

    Xmin = ( LambdaX1 - GraalWindowXmin ) / GraalWindowSide;
    Xmax = ( LambdaX2 - GraalWindowXmin ) / GraalWindowSide;
    Ymin = ( LambdaY1 - GraalWindowYmin ) / GraalWindowSide;
    Ymax = ( LambdaY2 - GraalWindowYmin ) / GraalWindowSide;

    if ( Xmin < 0             ) Xmin = 0;
    if ( Xmin > GraalWindowDx ) Xmin = GraalWindowDx - 1;
    if ( Ymin < 0             ) Ymin = 0;
    if ( Ymin > GraalWindowDy ) Ymin = GraalWindowDy - 1;

    if ( Xmax < 0             ) Xmax = 0;
    if ( Xmax > GraalWindowDx ) Xmax = GraalWindowDx - 1;
    if ( Ymax < 0             ) Ymax = 0;
    if ( Ymax > GraalWindowDy ) Ymax = GraalWindowDy - 1;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
    {
      StaticLayer = RDS_STATIC_LAYER[ Layer ];

      if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 ) continue; 

      Y = Ymin;

      while ( Y <= Ymax )
      {
        X = Xmin;

        while ( X <= Xmax )
        {
          Offset = ( Y * GraalWindowDx ) + X;

          ScanWin = &GraalWindowTable[ Offset ];

          if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
          {
            for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                  ScanWinRec != (graalwinrec *)NULL;
                  ScanWinRec  = ScanWinRec->NEXT )
            {
              for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
              {
                Rec = ScanWinRec->RECTAB[ ScanRec ];

                if ( ( Rec != (rdsrec_list *)NULL        ) &&
                     ( ! IsGraalDeleted( Rec )           ) &&
                     ( ! IsGraalCut( Rec )               ) &&
                     ( ! IsGraalAccepted( Rec )          ) &&
                     ( ! IsGraalSelected( Rec )          ) &&
                     ( ! IsGraalDruc( Rec )              ) &&
                     ( Rec->X               >=  LambdaX1 ) &&
                     ( Rec->Y               >=  LambdaY1 ) &&
                     ( ( Rec->X + Rec->DX ) <=  LambdaX2 ) &&
                     ( ( Rec->Y + Rec->DY ) <=  LambdaY2 ) )
                {
                  if ( ( IsRdsFigRec( Rec   ) ) || 
                       ( IsRdsInstance( Rec ) ) )
                  {
                    CheckRec = (rdsrec_list *)Rec->USER;

                    while ( Rec != CheckRec )
                    {
                      StaticLayer = RDS_STATIC_LAYER[ GetRdsLayer( CheckRec ) ];

                      if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 )

                       break;

                      CheckRec = (rdsrec_list *)CheckRec->USER;
                    }

                    if ( Rec == CheckRec ) GraalAddSelect( Rec );
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

    for ( Select  = GraalHeadSelect;
          Select != (graalselect *)NULL;
          Select  = Select->NEXT )
    {
      GraalUnselectRectangle( Select->RECTANGLE );
    }
  }   

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalEditSelectFence                    |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectFence( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
{
  long         Offset;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  rdsrec_list *Rec;
  rdsrec_list *CheckRec;
  graalselect *Select;
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

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    LambdaX1 = LambdaX1 * GRAAL_RDS_LAMBDA; 
    LambdaX2 = LambdaX2 * GRAAL_RDS_LAMBDA; 
    LambdaY1 = LambdaY1 * GRAAL_RDS_LAMBDA; 
    LambdaY2 = LambdaY2 * GRAAL_RDS_LAMBDA; 

    Xmin = ( LambdaX1 - GraalWindowXmin ) / GraalWindowSide;
    Xmax = ( LambdaX2 - GraalWindowXmin ) / GraalWindowSide;
    Ymin = ( LambdaY1 - GraalWindowYmin ) / GraalWindowSide;
    Ymax = ( LambdaY2 - GraalWindowYmin ) / GraalWindowSide;

    if ( Xmin < 0             ) Xmin = 0;
    if ( Xmin > GraalWindowDx ) Xmin = GraalWindowDx - 1;
    if ( Ymin < 0             ) Ymin = 0;
    if ( Ymin > GraalWindowDy ) Ymin = GraalWindowDy - 1;

    if ( Xmax < 0             ) Xmax = 0;
    if ( Xmax > GraalWindowDx ) Xmax = GraalWindowDx - 1;
    if ( Ymax < 0             ) Ymax = 0;
    if ( Ymax > GraalWindowDy ) Ymax = GraalWindowDy - 1;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
    {
      StaticLayer = RDS_STATIC_LAYER[ Layer ];

      if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 ) continue; 

      Y = Ymin;

      while ( Y <= Ymax )
      {
        X = Xmin;

        while ( X <= Xmax )
        {
          Offset = ( Y * GraalWindowDx ) + X;

          ScanWin = &GraalWindowTable[ Offset ];

          if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
          {
            for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                  ScanWinRec != (graalwinrec *)NULL;
                  ScanWinRec  = ScanWinRec->NEXT )
            {
              for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
              {
                Rec = ScanWinRec->RECTAB[ ScanRec ];

                if ( ( Rec != (rdsrec_list *)NULL        ) &&
                     ( ! IsGraalDeleted( Rec )           ) &&
                     ( ! IsGraalCut( Rec )               ) &&
                     ( ! IsGraalAccepted( Rec )          ) &&
                     ( ! IsGraalSelected( Rec )          ) &&
                     ( ! IsGraalDruc( Rec )              ) &&
                     ( ( Rec->X + Rec->DX ) >=  LambdaX1 ) &&
                     ( ( Rec->Y + Rec->DY ) >=  LambdaY1 ) &&
                     ( ( Rec->X           ) <=  LambdaX2 ) &&
                     ( ( Rec->Y           ) <=  LambdaY2 ) )
                {
                  if ( ( IsRdsFigRec( Rec   ) ) || 
                       ( IsRdsInstance( Rec ) ) )
                  {
                    CheckRec = (rdsrec_list *)Rec->USER;

                    while ( Rec != CheckRec )
                    {
                      StaticLayer = RDS_STATIC_LAYER[ GetRdsLayer( CheckRec ) ];

                      if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 )

                        break;

                      CheckRec = (rdsrec_list *)CheckRec->USER;
                    }

                    if ( Rec == CheckRec ) GraalAddSelect( Rec );
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

    for ( Select  = GraalHeadSelect;
          Select != (graalselect *)NULL;
          Select  = Select->NEXT )
    {
      GraalUnselectRectangle( Select->RECTANGLE );
    }
  }

  rdsend();
}


/*------------------------------------------------------------\
|                                                             |
|                  GraalEditSelectPointCopy                   |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectPointCopy( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectPoint( LambdaX1, LambdaY1 );

  GraalDisplaySelectList( (char *)NULL );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( IsRdsFigure( Select->RECTANGLE   ) ||
         IsRdsInstance( Select->RECTANGLE ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT != (graalselect *)NULL )
    {
      GraalEnterPanel( &GraalEditSelectPanel );
      GraalLimitedLoop( GraalEditSelectPanel.PANEL );
    }
    else
    {
      GraalAcceptRectangle( GraalHeadSelect->RECTANGLE );
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }

    if ( GraalHeadSelect != (graalselect *)NULL )
    {
      GraalContinueEditMode( GRAAL_EDIT_COPY_POINT,
                             GraalPromptEditCopy, 1 );
    }
    else
    {
      GraalChangeEditMode( GRAAL_SELECT_POINT_COPY,
                           GraalPromptSelectPointCopy );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEditSelectWindowCopy                   |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectWindowCopy( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( IsRdsFigure( Select->RECTANGLE   ) ||
         IsRdsInstance( Select->RECTANGLE ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAcceptRectangle( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_COPY,
                         GraalPromptSelectWindowCopy );
  }
  else
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }
    else
    {
      GraalZoomRefresh();
    }

    GraalContinueEditMode( GRAAL_EDIT_COPY_WINDOW,
                           GraalPromptEditCopy, 0 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalEditSelectPointMove                   |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectPointMove( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  graalselect  *Select;

  rdsbegin();

  GraalEditSelectPoint( LambdaX1, LambdaY1 );

  GraalDisplaySelectList( (char *)NULL );

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    GraalAddSelectList( Select->RECTANGLE );
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT != (graalselect *)NULL )
    {
      GraalEnterPanel( &GraalEditSelectPanel );
      GraalLimitedLoop( GraalEditSelectPanel.PANEL );
    }
    else
    {
      GraalAcceptRectangle( GraalHeadSelect->RECTANGLE );
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }

    if ( GraalHeadSelect != (graalselect *)NULL )
    {
      GraalContinueEditMode( GRAAL_EDIT_MOVE_POINT,
                             GraalPromptEditMove, 1 );
    }
    else
    {
      GraalChangeEditMode( GRAAL_SELECT_POINT_MOVE,
                           GraalPromptSelectPointMove );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEditSelectWindowMove                   |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectWindowMove( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;

  rdsbegin();

  GraalEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    GraalAcceptRectangle( Select->RECTANGLE );
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_MOVE,
                         GraalPromptSelectWindowMove );
  }
  else
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }
    else
    {
      GraalZoomRefresh();
    }

    GraalContinueEditMode( GRAAL_EDIT_MOVE_WINDOW,
                           GraalPromptEditMove, 0 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalEditSelectPointDelete                 |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectPointDelete( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  graalselect  *Select;

  rdsbegin();

  GraalEditSelectPoint( LambdaX1, LambdaY1 );

  GraalDisplaySelectList( (char *)NULL );

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    GraalAddSelectList( Select->RECTANGLE );
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT != (graalselect *)NULL )
    {
      GraalEnterPanel( &GraalEditSelectPanel );
      GraalLimitedLoop( GraalEditSelectPanel.PANEL );
    }
    else
    {
      GraalAcceptRectangle( GraalHeadSelect->RECTANGLE );
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }

    if ( GraalHeadSelect != (graalselect *)NULL ) 
    {
      GraalEditDelete();
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEditSelectWindowDelete                 |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectWindowDelete( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;

  rdsbegin();

  GraalEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    GraalAcceptRectangle( Select->RECTANGLE );
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }
    else
    {
      GraalZoomRefresh();
    }

    GraalEditDelete();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalEditSelectWindowStretch               |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectWindowStretch( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;
  void         *Pointer;

  rdsbegin();

  GraalEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    Pointer = GRAAL_MBK( Select->RECTANGLE );

    if ( ( IsRdsSegment( Select->RECTANGLE )        ) ||
         ( IsRdsFigure( Select->RECTANGLE )         ) ||
         ( ( IsRdsVia( Select->RECTANGLE )        ) &&
           ( ( ((phvia_list *)Pointer)->DX != 0 ) ||
             ( ((phvia_list *)Pointer)->DY != 0 ) ) ) )
    {
      GraalAcceptRectangle( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
    else
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );

    GraalChangeEditMode( GRAAL_SELECT_WINDOW_STRETCH,
                         GraalPromptSelectWindowStretch );
  }
  else
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }
    else
    {
      GraalZoomRefresh();
    }

    GraalContinueEditMode( GRAAL_EDIT_STRETCH_WINDOW,
                           GraalPromptEditStretch, 0 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalEditSelectPointStretch                |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectPointStretch( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;
  void         *Pointer;

  rdsbegin();

  GraalEditSelectPoint( LambdaX1, LambdaY1 );

  GraalDisplaySelectList( (char *)NULL );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    Pointer = GRAAL_MBK( Select->RECTANGLE );

    if ( ( IsRdsSegment( Select->RECTANGLE )        ) ||
         ( IsRdsFigure( Select->RECTANGLE )         ) ||
         ( ( IsRdsVia( Select->RECTANGLE )        ) &&
           ( ( ((phvia_list *)Pointer)->DX != 0 ) ||
             ( ((phvia_list *)Pointer)->DY != 0 ) ) ) )
    {
      GraalAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
    else
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT != (graalselect *)NULL )
    {
      GraalEnterPanel( &GraalEditSelectPanel );
      GraalLimitedLoop( GraalEditSelectPanel.PANEL );
    }
    else
    {
      GraalAcceptRectangle( GraalHeadSelect->RECTANGLE );
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }

    if ( GraalHeadSelect != (graalselect *)NULL )
    {
      GraalContinueEditMode( GRAAL_EDIT_STRETCH_POINT,
                             GraalPromptEditStretch, 1 );
    }
    else
    {
      GraalChangeEditMode( GRAAL_SELECT_POINT_STRETCH,
                           GraalPromptSelectPointStretch );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalEditSelectPointModify                 |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectPointModify( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectPoint( LambdaX1, LambdaY1 );

  GraalDisplaySelectList( (char *)NULL );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( ! IsRdsFigure( Select->RECTANGLE   ) )
    {
      GraalAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
    else
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT != (graalselect *)NULL )
    {
      GraalEnterPanel( &GraalEditSelectPanel );
      GraalLimitedLoop( GraalEditSelectPanel.PANEL );
    }

    if ( GraalHeadSelect != (graalselect *)NULL )
    {
      GraalEditModify();
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalEditSelectWindowModify                 |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSelectWindowModify( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectWindow( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
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

      GraalFreeSelect( DelSelect );
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    GraalEditModify();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalToolsSelectEqui                       |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsSelectEqui( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;
  void         *Pointer;
  char          Delete;
  char          MbkLayer;
  char          ViaType;

  rdsbegin();

  GraalEditSelectPoint( LambdaX1, LambdaY1 );

  GraalDisplaySelectList( (char *)NULL );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    Delete = 0;

    if ( IsRdsSegment( Select->RECTANGLE ) )
    {
      Pointer = GRAAL_MBK( Select->RECTANGLE );

      MbkLayer = ((phseg_list *)Pointer)->LAYER;

      if ( GET_LYNX_TRANSISTOR_TYPE( MbkLayer ) != RDS_LYNX_TRANSISTOR_EMPTY )

        Delete = 1;
    }
    else
    if ( IsRdsVia( Select->RECTANGLE ) )
    {
      Pointer = GRAAL_MBK( Select->RECTANGLE );
      ViaType = ((phvia_list *)Pointer)->TYPE;

      for ( MbkLayer = 0; MbkLayer < MBK_MAX_LAYER; MbkLayer++ )
      {
        if ( GET_LYNX_TRANSISTOR_CX( MbkLayer ) == ViaType )
        {   
          Delete = 1; break;
        }
      }
    }
    else
    if ( ( IsRdsInstance( Select->RECTANGLE ) ) ||
         ( IsRdsFigure( Select->RECTANGLE )   ) )

       Delete = 1;

    if ( Delete )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT != (graalselect *)NULL )
    {
      GraalEnterPanel( &GraalEditSelectPanel );
      GraalLimitedLoop( GraalEditSelectPanel.PANEL );
    }
    else
    {
      GraalAcceptRectangle( GraalHeadSelect->RECTANGLE );
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }

    if ( GraalHeadSelect != (graalselect *)NULL )
    {
      GraalToolsEqui();
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalToolsSelectFlatten                     |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsSelectFlatten( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectFence( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( ( ! IsRdsInstance( Select->RECTANGLE ) ) ||
         (  IsGraalFlatten( Select->RECTANGLE ) ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAcceptRectangle( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No instance found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }
    else
    {
      GraalZoomRefresh();
    }

    GraalToolsFlatten();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalToolsSelectUnflatten                   |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsSelectUnflatten( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectFence( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( ( ! IsRdsInstance( Select->RECTANGLE  ) ) ||
         ( ! IsGraalFlatten( Select->RECTANGLE ) ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAcceptRectangle( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No instance found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }
    else
    {
      GraalZoomRefresh();
    }

    GraalToolsUnflatten();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalToolsSelectRealFlatten                 |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsSelectRealFlatten( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectFence( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( ( ! IsRdsInstance( Select->RECTANGLE ) ) ||
         (  IsGraalFlatten( Select->RECTANGLE ) ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAcceptRectangle( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No instance found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT == (graalselect *)NULL )
    {
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }
    else
    {
      GraalZoomRefresh();
    }

    GraalToolsRealFlatten();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalToolsSelectHierarchyDown              |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsSelectHierarchyDown( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;

  rdsbegin();

  GraalEditSelectPoint( LambdaX1, LambdaY1 );

  GraalDisplaySelectList( (char *)NULL );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( ! IsRdsInstance( Select->RECTANGLE ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAddSelectList( Select->RECTANGLE );

      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No instance found !" );
  }
  else
  {
    if ( GraalHeadSelect->NEXT != (graalselect *)NULL )
    {
      GraalEnterPanel( &GraalEditSelectPanel );
      GraalLimitedLoop( GraalEditSelectPanel.PANEL );
    }
    else
    {
      GraalAcceptRectangle( GraalHeadSelect->RECTANGLE );
      GraalDisplayRectangle( GraalHeadSelect->RECTANGLE );
    }

    if ( GraalHeadSelect != (graalselect *)NULL )
    {
      GraalToolsHierarchyDown();
    }
  }

  rdsend();
}
