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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Edit.c                          |
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

# include "GME_edit.h"
# include "GME_panel.h"
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
|                        Identify Buffer                      |
|                                                             |
\------------------------------------------------------------*/

  static char  DrealIdentifyMessage[ DREAL_IDENTIFY_MESSAGE_SIZE ];
  static char  DrealIdentifyBuffer [ DREAL_IDENTIFY_BUFFER_SIZE  ];
  static char *DrealScanIdentify;
  static long  DrealIdentifyLength;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealEditTreatUndo                     |
|                                                             |
\------------------------------------------------------------*/

void DrealEditTreatUndo( HeadUndo )

  drealundo *HeadUndo;
{
  rdsrec_list  *Rec;
  drealundorec *UndoRec;
  char          OneElement;

  if ( HeadUndo->UNDO->NEXT != (drealundorec *)NULL )
  { 
    OneElement = DREAL_FALSE;
  }
  else
  {
    OneElement = (DrealHeadSelect == (drealselect *)NULL);
  }

  DrealDelSelect();
  DrealRecomputeBound = DREAL_TRUE;

  for ( UndoRec  = HeadUndo->UNDO;
        UndoRec != (drealundorec *)NULL;
        UndoRec  = UndoRec->NEXT )
  {
    Rec = UndoRec->RECTANGLE;

    if ( IsRdsFigure( Rec ) )
    {
      if ( IsDrealDeleted( Rec ) )
      {
        ClearDrealDeleted( Rec );

        DrealFigureXAB1 = Rec->X;
        DrealFigureYAB1 = Rec->Y;
        DrealFigureXAB2 = ( Rec->X + Rec->DX );
        DrealFigureYAB2 = ( Rec->Y + Rec->DY );
      }
      else
      {
        DrealFigureXAB1 = 0;
        DrealFigureXAB2 = 0;
        DrealFigureYAB1 = 0;
        DrealFigureYAB2 = 0;

        DrealDeleteRectangle( Rec );
      }

      OneElement = DREAL_FALSE;
    }
    else
    {
      if ( IsDrealDeleted( Rec ) )
      {
        DrealUndeleteRectangle( Rec );
        if ( OneElement ) DrealDisplayRectangle( Rec );
      }
      else
      {   
        DrealDeleteRectangle( Rec );
        if ( OneElement ) DrealDisplayRectangle( Rec );
      }
    }
  }

  HeadUndo->UNDO = (drealundorec *)reverse( (chain_list *)HeadUndo->UNDO );

  if ( ! OneElement )
  {
    DrealZoomRefresh();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealEditRedo                          |
|                                                             |
\------------------------------------------------------------*/

void DrealEditRedo()

{
  drealundo *NewUndo;

  rdsbegin();

  if ( DrealHeadRedo != (drealundo *)NULL )
  {
    DrealEditTreatUndo( DrealHeadRedo );

    NewUndo       = DrealHeadRedo;
    DrealHeadRedo = DrealHeadRedo->NEXT;
    NewUndo->NEXT = DrealHeadUndo;
    DrealHeadUndo = NewUndo;
  }
  else
  {
    DrealWarningMessage( DrealMainWindow, "Nothing to Redo !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealEditUndo                          |
|                                                             |
\------------------------------------------------------------*/

void DrealEditUndo()

{
  drealundo *NewRedo;

  rdsbegin();

  if ( DrealHeadUndo != (drealundo *)NULL )
  {
    DrealEditTreatUndo( DrealHeadUndo );

    NewRedo       = DrealHeadUndo;
    DrealHeadUndo = DrealHeadUndo->NEXT;
    NewRedo->NEXT = DrealHeadRedo;
    DrealHeadRedo = NewRedo;
  }
  else
  {
    DrealWarningMessage( DrealMainWindow, "Nothing to undo !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         DrealAddIdentify                    |
|                                                             |
\------------------------------------------------------------*/

char DrealAddIdentify( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsins_list *Instance;
  char         StaticLayer;
  long         Length;

  rdsbegin();

  DrealIdentifyBuffer[0] = '\0';

  if ( IsRdsInstance( Rectangle ) )
  {
    Instance = (rdsins_list *)DREAL_PREVIOUS( Rectangle );

    sprintf( DrealIdentifyBuffer, 
    "INSTANCE :\n\n  NAME : %s\n  MODEL : %s\n  TRANSF : %s\n  X : %.3f\n  Y : %.3f\n\n",
    Instance->INSNAME,
    Instance->FIGNAME,
    RDS_TRANSF_NAME[ Instance->TRANSF ],
    Instance->X * DREAL_RDS_UNIT_TO_MICRON,
    Instance->Y * DREAL_RDS_UNIT_TO_MICRON );
  }
  else
  if ( IsRdsFigure( Rectangle ) )
  {
    sprintf( DrealIdentifyBuffer,
    "ABUTMENT BOX :\n\n  NAME : %s\n  X  : %.3f\n  Y  : %.3f\n  DX : %.3f\n  DY : %.3f\n\n",
    Rectangle->NAME,
    Rectangle->X  * DREAL_RDS_UNIT_TO_MICRON,
    Rectangle->Y  * DREAL_RDS_UNIT_TO_MICRON,
    Rectangle->DX * DREAL_RDS_UNIT_TO_MICRON,
    Rectangle->DY * DREAL_RDS_UNIT_TO_MICRON );
  }
  else
  {
    StaticLayer = RDS_STATIC_LAYER[ GetRdsLayer( Rectangle ) ];

    sprintf( DrealIdentifyBuffer, 
      "RECTANGLE :\n\n  NAME : %s\n  LAYER : %s\n  X : %.3f\n  Y : %.3f\n  DX : %.3f\n  DY : %.3f\n\n",
        ( Rectangle->NAME != (char *)NULL ) ? Rectangle->NAME : "None",
        DREAL_RDS_LAYER_NAME_TABLE[ StaticLayer ][0],
        Rectangle->X  * DREAL_RDS_UNIT_TO_MICRON,
        Rectangle->Y  * DREAL_RDS_UNIT_TO_MICRON,
        Rectangle->DX * DREAL_RDS_UNIT_TO_MICRON,
        Rectangle->DY * DREAL_RDS_UNIT_TO_MICRON );
  }

  Length = strlen( DrealIdentifyBuffer );

  if ( ( DrealIdentifyLength + Length ) < ( DREAL_IDENTIFY_MESSAGE_SIZE - 1 ) )
  {
    strcpy( DrealScanIdentify, DrealIdentifyBuffer );

    DrealScanIdentify   += Length; 
    DrealIdentifyLength += Length;

    rdsend();
    return( DREAL_TRUE );
  }

  strcpy( DrealScanIdentify, "#" );
 
  rdsend();
  return( DREAL_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealEditIdentify                       |
|                                                             |
\------------------------------------------------------------*/

void DrealEditIdentify()
{
  drealselect *Select;

  rdsbegin();

  strcpy( DrealIdentifyMessage, "No element found !" );
  DrealScanIdentify   = DrealIdentifyMessage;
  DrealIdentifyLength = 0;

  for ( Select  = DrealHeadSelect; 
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    if ( ! DrealAddIdentify( Select->RECTANGLE ) ) break;
  }

  DrealDelSelect();

  DrealDisplayIdentifyMessage( DrealIdentifyMessage );
  DrealEnterPanel( &DrealEditIdentifyPanel       );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealEditCopy                        |
|                                                             |
\------------------------------------------------------------*/

void DrealEditCopy( LambdaX1, LambdaY1, LambdaX2, LambdaY2, Mode )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
   char Mode; 
{
  drealselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  char         FirstUndo;
  long         DeltaX;
  long         DeltaY;
  long         X;
  long         Y;
  long         DX;
  long         DY;

  rdsbegin();

  DeltaX = ( LambdaX2 - LambdaX1 ) * RDS_PHYSICAL_GRID;
  DeltaY = ( LambdaY2 - LambdaY1 ) * RDS_PHYSICAL_GRID;

  FirstUndo = 1;

  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( ! IsRdsFigure( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        DrealAddUndo(); 

        FirstUndo = 0; 
      }

      X  = Rectangle->X + DeltaX;
      Y  = Rectangle->Y + DeltaY;
      DX = Rectangle->DX;
      DY = Rectangle->DY;

      NewRec = DrealAddRectangle( Rectangle->NAME, 
                                  GetRdsLayer( Rectangle ),
                                  X, Y, DX, DY );

      DrealAddUndoRec( NewRec );
    }
  }

  DrealDelSelect();
  DrealZoomRefresh();

  if ( Mode == 0 )
  {
    DrealChangeEditMode( DREAL_SELECT_POINT_COPY,
                         DrealPromptSelectPointCopy );
  }
  else
  {
    DrealChangeEditMode( DREAL_SELECT_WINDOW_COPY,
                         DrealPromptSelectWindowCopy );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealEditMove                        |
|                                                             |
\------------------------------------------------------------*/

void DrealEditMove( LambdaX1, LambdaY1, LambdaX2, LambdaY2, Mode )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
   char Mode;
{
  drealselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  char         FirstUndo;
  long         DeltaX;
  long         DeltaY;
  long         X;
  long         Y;
  long         DX;
  long         DY;

  rdsbegin();

  DeltaX = ( LambdaX2 - LambdaX1 ) * RDS_PHYSICAL_GRID;
  DeltaY = ( LambdaY2 - LambdaY1 ) * RDS_PHYSICAL_GRID;

  FirstUndo = 1;

  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( ! IsDrealAccepted( Rectangle ) ) continue;

    if ( IsRdsFigRec( Rectangle ) )
    {
      if ( FirstUndo )
      {
        DrealAddUndo();

        FirstUndo = 0;
      }

      DrealDeleteRectangle( Rectangle );
      DrealAddUndoRec( Rectangle );

      X  = Rectangle->X + DeltaX;
      Y  = Rectangle->Y + DeltaY;
      DX = Rectangle->DX;
      DY = Rectangle->DY;

      if ( IsRdsFigure( Rectangle ) )
      {
        NewRec = DrealAddAbox( X, Y, DX, DY );
      }
      else
      {
        NewRec = DrealAddRectangle( Rectangle->NAME,
                                    GetRdsLayer( Rectangle ),
                                    X, Y, DX, DY );
      }

      DrealAddUndoRec( NewRec );
    }
  }

  DrealDelSelect();
  DrealZoomRefresh();

  if ( Mode == 0 )
  {
    DrealChangeEditMode( DREAL_SELECT_POINT_MOVE,
                         DrealPromptSelectPointMove );
  }
  else
  {
    DrealChangeEditMode( DREAL_SELECT_WINDOW_MOVE,
                         DrealPromptSelectWindowMove );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealEditDelete                      |
|                                                             |
\------------------------------------------------------------*/

void DrealEditDelete()
{
  drealselect *Select;
  rdsrec_list *Rectangle;
  char         FirstUndo;

  rdsbegin();

  FirstUndo = 1;

  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( ! IsDrealAccepted( Rectangle ) ) continue;

    if ( IsRdsFigRec( Rectangle ) ) 
    {
      if ( FirstUndo ) 
      {
        DrealAddUndo();
        FirstUndo = 0;
      }

      if ( IsRdsFigure( Rectangle ) )
      {
        DrealFigureXAB1 = 0;
        DrealFigureXAB2 = 0;
        DrealFigureYAB1 = 0;
        DrealFigureYAB2 = 0;
      }

      DrealDeleteRectangle( Rectangle );
      DrealAddUndoRec( Rectangle );
    }
  }

  DrealDelSelect();
  DrealZoomRefresh();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealEditStretch                      |
|                                                             |
\------------------------------------------------------------*/

void DrealEditStretch( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
{
  drealselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  char         Orient;
  char         FirstUndo;
  long         Delta;
  long         X;
  long         Y;
  long         DX;
  long         DY;

  rdsbegin();

  Orient = 0;

  if ( LambdaX2 != LambdaX1 )
  {
    Delta  = LambdaX2 - LambdaX1;
    Orient = ( Delta > 0 ) ? DREAL_EAST : DREAL_WEST;
  }
  else
  {
    Delta  = LambdaY2 - LambdaY1;
    Orient = ( Delta > 0 ) ? DREAL_NORTH : DREAL_SOUTH;
  }

  Delta    = Delta    * RDS_PHYSICAL_GRID;
  LambdaX1 = LambdaX1 * RDS_PHYSICAL_GRID;
  LambdaY1 = LambdaY1 * RDS_PHYSICAL_GRID;
 
  if ( Delta != 0 )
  {
    FirstUndo = 1;

    for ( Select  = DrealHeadSelect;
          Select != (drealselect *)NULL;
          Select  = Select->NEXT )
    {
      Rectangle = Select->RECTANGLE;

      if ( IsRdsFigRec( Rectangle ) )
      {
        X  = Rectangle->X;
        Y  = Rectangle->Y;
        DX = Rectangle->DX;
        DY = Rectangle->DY;

        switch ( Orient )
        {
          case DREAL_NORTH :
          case DREAL_SOUTH :

            if ( LambdaY1 >= (Y + (DY >> 1)) )
            {
              DY += Delta;
            }
            else
            {
              Y  += Delta; DY -= Delta;
            }
          break;

          case DREAL_WEST :  
          case DREAL_EAST :
          
            if ( LambdaX1 <= (X + (DX >> 1)) )
            {
              X  += Delta; DX -= Delta;
            }
            else
            {
              DX += Delta;
            }
          break;
        }

        if ( ( DX > 0 ) &&
             ( DY > 0 ) )
        {
          if ( FirstUndo )
          {
            DrealAddUndo();

            FirstUndo = 0;
          }

          DrealDeleteRectangle( Rectangle );
          DrealAddUndoRec( Rectangle );

          if ( IsRdsFigure( Rectangle ) )
          {
            NewRec = DrealAddAbox( X, Y, DX, DY );
          }
          else
          {
            NewRec = DrealAddRectangle( Rectangle->NAME,
                                        GetRdsLayer( Rectangle ),
                                        X, Y, DX, DY );
          }

          DrealAddUndoRec( NewRec );
        }
      }
    }
  } 

  DrealDelSelect();
  DrealZoomRefresh();

  DrealChangeEditMode( DREAL_SELECT_POINT_STRETCH,
                       DrealPromptSelectPointStretch );

  rdsend();
}
