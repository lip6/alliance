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
| File    :                   View.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include <signal.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GSB.h"
# include "GRD.h"
# include "GMX.h"

# include "GMX_view.h" 

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

    static long         DrealGraphicX1;
    static long         DrealGraphicX2;
    static long         DrealGraphicY1;
    static long         DrealGraphicY2;
    static XFontStruct *DrealTextFont;
    static GC           DrealTextGC;
    static GC           DrealDrawGC;
    static GC           DrealFillGC;
    static drealview   *DrealHeadView       = (drealview *)NULL;
    static drealview   *DrealHeadViewString = (drealview *)NULL;
    static char         DrealInterrupt = 0;
    static char         DrealCheckInt  = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        DrealAllocView                       |
|                                                             |
\------------------------------------------------------------*/

 drealview *DrealAllocView()
 {
   return((drealview *)rdsallocheap( sizeof(drealview) ) );
 }

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrealFreeView                       |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeView( FreeView )

   drealview *FreeView;
{
  rdsfreeheap((char *)FreeView, sizeof(drealview));
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAddViewLater                     |
|                                                             |
\------------------------------------------------------------*/

void DrealAddViewLater( Rectangle )

   rdsrec_list *Rectangle;
{
  drealview *View;

  rdsbegin();

  View = DrealAllocView();

  View->RECTANGLE = Rectangle;
  View->NEXT      = DrealHeadView;
  DrealHeadView   = View;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAddViewString                    |
|                                                             |
\------------------------------------------------------------*/

void DrealAddViewString( Rectangle )

   rdsrec_list *Rectangle;
{
  drealview *View;

  rdsbegin();

  View = DrealAllocView();

  View->RECTANGLE     = Rectangle;
  View->NEXT          = DrealHeadViewString;
  DrealHeadViewString = View;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealDelView                          |
|                                                             |
\------------------------------------------------------------*/

void DrealDelView()
{
  drealview    *DelView;
  drealview    *View;

  rdsbegin();

  View          = DrealHeadView;
  DrealHeadView = (drealview *)NULL;

  while( View != (drealview *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    DrealFreeView( DelView );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealDelViewString                      |
|                                                             |
\------------------------------------------------------------*/

void DrealDelViewString()
{
  drealview    *DelView;
  drealview    *View;

  rdsbegin();

  View                = DrealHeadViewString;
  DrealHeadViewString = (drealview *)NULL;

  while( View != (drealview *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    DrealFreeView( DelView );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealDisplayOneRectangle                    |
|                                                             |
\------------------------------------------------------------*/


void DrealDisplayOneRectangle( Rec )

  rdsrec_list *Rec;
{
  long X1r;
  long X2r;
  long Y1r;
  long Y2r;
  long DeltaX;
  long DeltaY;
  long Xcr;
  long Ycr;
  char DrawLine;
  char Tiny;
  char Small;
  char RdsLayer;

  rdsbegin();

  RdsLayer = GetRdsLayer( Rec );

  X1r  = (float)( Rec->X ) * DrealLambdaGridStep;
  Y1r  = (float)( Rec->Y ) * DrealLambdaGridStep;
  X2r  = (float)( Rec->X + Rec->DX ) * DrealLambdaGridStep;
  Y2r  = (float)( Rec->Y + Rec->DY ) * DrealLambdaGridStep;

  X1r  = ( X1r / RDS_PHYSICAL_GRID ) - DrealPixelGridX;
  X2r  = ( X2r / RDS_PHYSICAL_GRID ) - DrealPixelGridX;
  Y1r  = ( Y1r / RDS_PHYSICAL_GRID ) - DrealPixelGridY;
  Y2r  = ( Y2r / RDS_PHYSICAL_GRID ) - DrealPixelGridY;

  if ( ( X1r <= X2r ) &&
       ( Y1r <= Y2r ) )
  {
    DrawLine = 0;
    Tiny     = 0;
    Small    = 0;

    DeltaX = X2r - X1r;
    DeltaY = Y2r - Y1r;

    if ( ( DeltaX <= 1 ) ||
         ( DeltaY <= 1 ) ) Tiny = 1;
    else
    if ( DREAL_FILL_MODE != DREAL_FILL_MODE_FILL )
    {
      if ( ( DeltaX <= 4 ) ||
           ( DeltaY <= 4 ) )
      {
        Small = ! DREAL_FORCE_DISPLAY;
      }
    }

    if ( Tiny )
    {
      if ( ( DeltaX <= 1 ) &&
           ( DeltaY <= 1 ) )
      {
        XDrawPoint( DrealGraphicDisplay,
                    DrealGraphicPixmap,
                    DrealDrawGC,
                    X1r, DrealGraphicDy - Y1r );
      }
      else
      if ( DeltaX <= 1 )
      {
        Xcr = ( X1r + X2r ) >> 1;

        XDrawLine( DrealGraphicDisplay,
                   DrealGraphicPixmap,
                   DrealDrawGC,
                   Xcr, DrealGraphicDy - Y1r,
                   Xcr, DrealGraphicDy - Y2r );
      }
      else
      {
        Ycr = ( Y1r + Y2r ) >> 1;

        XDrawLine( DrealGraphicDisplay,
                   DrealGraphicPixmap,
                   DrealDrawGC,
                   X1r, DrealGraphicDy - Ycr,
                   X2r, DrealGraphicDy - Ycr );
      }
    }
    else
    {
      if ( ! Small )
      {
        if ( X1r < DrealGraphicX1 ) 
        { 
          X1r = DrealGraphicX1; DrawLine |= DREAL_WEST_MASK;  
        }
      
        if ( X2r > DrealGraphicX2 ) 
        { 
          X2r = DrealGraphicX2; DrawLine |= DREAL_EAST_MASK;  
        }
      
        if ( Y1r < DrealGraphicY1 ) 
        {
          Y1r = DrealGraphicY1; DrawLine |= DREAL_SOUTH_MASK; 
        }
      
        if ( Y2r > DrealGraphicY2 ) 
        {
          Y2r = DrealGraphicY2; DrawLine |= DREAL_NORTH_MASK; 
        }
      }

      if ( ( X1r <= X2r ) &&
           ( Y1r <= Y2r ) )
      {
        if ( ( X1r == X2r ) &&
             ( Y1r == Y2r ) ) 
        {
          X2r++;
          Y2r++;
        }
    
        if ( DrawLine == 0 )
        {
          XDrawRectangle( DrealGraphicDisplay,
                          DrealGraphicPixmap,
                          DrealDrawGC,
                          X1r, DrealGraphicDy - Y2r,
                          X2r - X1r,
                          Y2r - Y1r );
        }
        else
        {
          if ( ( DrawLine & DREAL_WEST_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       DrealGraphicPixmap,
                       DrealDrawGC,
                       X1r, DrealGraphicDy - Y2r,
                       X1r, DrealGraphicDy - Y1r );
          }
    
          if ( ( DrawLine & DREAL_EAST_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       DrealGraphicPixmap,
                       DrealDrawGC,
                       X2r, DrealGraphicDy - Y2r,
                       X2r, DrealGraphicDy - Y1r );
          }
    
          if ( ( DrawLine & DREAL_SOUTH_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       DrealGraphicPixmap,
                       DrealDrawGC,
                       X1r, DrealGraphicDy - Y1r,
                       X2r, DrealGraphicDy - Y1r );
          }
    
          if ( ( DrawLine & DREAL_NORTH_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       DrealGraphicPixmap,
                       DrealDrawGC,
                       X1r, DrealGraphicDy - Y2r,
                       X2r, DrealGraphicDy - Y2r );
          }
        }
   
        if ( ( ! Small                                    ) &&
             ( RdsLayer        != RDS_ABOX                ) &&
             ( DREAL_FILL_MODE != DREAL_FILL_MODE_OUTLINE ) )
        {
          if ( DREAL_FILL_MODE == DREAL_FILL_MODE_PATTERN )
          {
            XSetFillStyle( DrealGraphicDisplay, 
                           DrealFillGC, 
                           FillStippled );
          }
          else
          {
            XSetFillStyle( DrealGraphicDisplay, 
                           DrealFillGC, 
                           FillSolid );
          }
    
          XFillRectangle( DrealGraphicDisplay,
                          DrealGraphicPixmap,
                          DrealFillGC,
                          X1r, DrealGraphicDy - Y2r,
                          X2r - X1r,
                          Y2r - Y1r );
        }
      }
    }

    if ( ( DrawLine != 0     ) ||
         ( Rec->NAME == NULL ) )
    {
      ClearDrealDrawText( Rec ); 
    }
  }
  else
  {
    ClearDrealDrawText( Rec );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealDisplayOneString                       |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayOneString( Rec )

  rdsrec_list *Rec;
{
  long X1r;
  long X2r;
  long Y1r;
  long Y2r;
  long WidthText;
  long HeightText;

  rdsbegin();

  X1r  = (float)( Rec->X ) * DrealLambdaGridStep;
  Y1r  = (float)( Rec->Y ) * DrealLambdaGridStep;
  X2r  = (float)( Rec->X + Rec->DX ) * DrealLambdaGridStep;
  Y2r  = (float)( Rec->Y + Rec->DY ) * DrealLambdaGridStep;

  X1r  = ( X1r / RDS_PHYSICAL_GRID ) - DrealPixelGridX;
  X2r  = ( X2r / RDS_PHYSICAL_GRID ) - DrealPixelGridX;
  Y1r  = ( Y1r / RDS_PHYSICAL_GRID ) - DrealPixelGridY;
  Y2r  = ( Y2r / RDS_PHYSICAL_GRID ) - DrealPixelGridY;

  HeightText = DrealTextFont->ascent;
  WidthText  = XTextWidth( DrealTextFont,
                           Rec->NAME, strlen( Rec->NAME ) );

  WidthText = XTextWidth( DrealTextFont,
                          Rec->NAME, strlen( Rec->NAME ) );

  if ( DREAL_BLACK_BOX_STRING )
  {
    XDrawImageString( DrealGraphicDisplay,
                      DrealGraphicPixmap,
                      DrealTextGC,
                      (( X1r + X2r - WidthText ) >> 1),
                      DrealGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                      Rec->NAME, strlen( Rec->NAME ) );
  }
  else
  {
    XDrawString( DrealGraphicDisplay,
                 DrealGraphicPixmap,
                 DrealTextGC,
                 (( X1r + X2r - WidthText ) >> 1),
                 DrealGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                 Rec->NAME, strlen( Rec->NAME ) );
  }

  ClearDrealDrawText( Rec );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealCheckInterrupt                      |
|                                                             |
\------------------------------------------------------------*/
 
char DrealCheckInterrupt()
{
  XEvent       Event;
  KeySym       Key;
  char         Text[ 2 ];
 
  rdsbegin();
 
  if ( DrealCheckInt )
  {
    if ( XCheckTypedEvent( DrealGraphicDisplay, KeyPress, &Event ) )
    {
      XLookupString( &Event.xkey, Text, 1, &Key, 0 );
 
      if ( Text[ 0 ] == '\003' )
      {
        DrealInterrupt = 1;
 
        XBell( DrealGraphicDisplay, 0 );
      }
    }
  }

  DrealCheckInt = 0;
 
  rdsend();
 
  return( DrealInterrupt );
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealInterruptDisplay                    |
|                                                             |
\------------------------------------------------------------*/
 
void DrealInterruptDisplay()
{
  rdsbegin();
 
  DrealCheckInt = 1;
 
  signal( SIGALRM, DrealInterruptDisplay );
  alarm( 1 );
 
  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealFlushEventDisplay                   |
|                                                             |
\------------------------------------------------------------*/

void DrealFlushEventDisplay()
{
  XEvent       Event;

  rdsbegin();

  while ( XCheckTypedEvent( DrealGraphicDisplay, KeyPress, &Event ) );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealDisplayFigure                       |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayFigure( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   long GraphicX1;
   long GraphicY1;
   long GraphicX2;
   long GraphicY2;
{
  long         Offset;
  drealwin    *ScanWin;
  drealwinrec *ScanWinRec;
  rdsrec_list *Rec;
  char         ScanRec;
  char         Layer;
  char         StaticLayer;
  char         DrawLine;
  long         Xmin;
  long         Ymin;
  long         Xmax;
  long         Ymax;
  long         X1;
  long         Y1;
  long         X2;
  long         Y2;
  long         X;
  long         Y;
  long         LambdaGridX;
  long         LambdaGridY;
  long         X1r;
  long         Y1r;
  long         X2r;
  long         Y2r;
  long         LambdaMin; 
  char         DrawText;
  char         DrawTextFig;
  char         DrawTextFigSeg;
  char         DrawTextFigCon;
  char         DrawTextFigRef;
  char         DrawTextIns;
  char         DrawTextInsSeg;
  char         DrawTextInsCon;
  char         DrawTextInsRef;
  drealview   *View;

  rdsbegin();

  DrealTextGC    = DrealSmallTextGC;
  DrealTextFont  = DrealSmallTextFont;
  DrawTextFig    = 0;
  DrawTextFigCon = 0;
  DrawTextFigSeg = 0;
  DrawTextFigRef = 0;
  DrawTextIns    = 0;
  DrawTextInsCon = 0;
  DrawTextInsSeg = 0;
  DrawTextInsRef = 0;
  DrealGraphicX1 = GraphicX1;
  DrealGraphicX2 = GraphicX2;
  DrealGraphicY1 = GraphicY1;
  DrealGraphicY2 = GraphicY2;

  if ( ( DrealLambdaGridStep > DREAL_LOWER_FIGURE_STEP ) ||
       ( DREAL_FORCE_DISPLAY                           ) )
  {
    DrawTextFig = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_FIGURE_NAME ];
  }

  if ( ( DrealLambdaGridStep > DREAL_LOWER_INSTANCE_STEP ) ||
       ( DREAL_FORCE_DISPLAY                             ) )
  {
    DrawTextIns = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_INSTANCE_NAME ];
  }
 
  if ( ( DrealLambdaGridStep > DREAL_LOWER_CONNECTOR_STEP ) ||
       ( DREAL_FORCE_DISPLAY                              ) )
  {
    DrawTextInsCon = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_INSTANCE_CONNECTOR_NAME ];
    DrawTextFigCon = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_FIGURE_CONNECTOR_NAME ];
  }

  if ( ( DrealLambdaGridStep > DREAL_LOWER_SEGMENT_STEP ) ||
       ( DREAL_FORCE_DISPLAY                            ) )
  {
    DrawTextFigSeg = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_FIGURE_SEGMENT_NAME ];
    DrawTextInsSeg = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_INSTANCE_SEGMENT_NAME ];
  }

  if ( ( DrealLambdaGridStep > DREAL_LOWER_REFERENCE_STEP ) ||
       ( DREAL_FORCE_DISPLAY                              ) )
  {
    DrawTextFigRef = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_FIGURE_REFERENCE_NAME ];
    DrawTextInsRef = DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_INSTANCE_REFERENCE_NAME ];
  }

  if ( DrealLambdaGridStep > DREAL_LOWER_CONNECTOR_STEP )
  {
    DrealTextGC    = DrealMediumTextGC;
    DrealTextFont  = DrealMediumTextFont;
  }

  if ( DrealLambdaGridStep > DREAL_LOWER_REFERENCE_STEP )
  {
    DrealTextGC    = DrealLargeTextGC;
    DrealTextFont  = DrealLargeTextFont;
  }

  DrealClearGraphicWindow( GraphicX1, 
                           DrealGraphicDy - GraphicY2,
                           GraphicX2 - GraphicX1,
                           GraphicY2 - GraphicY1 );

  if ( DrealFigureRds == (rdsfig_list *)NULL )
  {
    rdsend();
    return;
  }

  LambdaMin = (float)(4.0 / ( DrealLambdaGridStep ));

  X1 = GraphicX1 + DrealPixelGridX;
  X2 = GraphicX2 + DrealPixelGridX;
  Y1 = GraphicY1 + DrealPixelGridY;
  Y2 = GraphicY2 + DrealPixelGridY;

  X1 = ( X1 / DrealLambdaGridStep );
  Y1 = ( Y1 / DrealLambdaGridStep );
  X2 = ( X2 / DrealLambdaGridStep );
  Y2 = ( Y2 / DrealLambdaGridStep );

  if ( X2 >= 0 ) { X2 = X2 + 1; }
  if ( Y2 >= 0 ) { Y2 = Y2 + 1; }
  if ( X1 <= 0 ) { X1 = X1 - 1; }
  if ( Y1 <= 0 ) { Y1 = Y1 - 1; }

  X1 = X1 * RDS_PHYSICAL_GRID; 
  X2 = X2 * RDS_PHYSICAL_GRID; 
  Y1 = Y1 * RDS_PHYSICAL_GRID; 
  Y2 = Y2 * RDS_PHYSICAL_GRID; 

  LambdaGridX = DrealLambdaGridX * RDS_PHYSICAL_GRID;
  LambdaGridY = DrealLambdaGridY * RDS_PHYSICAL_GRID;

  Xmin = ( X1 - DrealWindowXmin ) / DrealWindowSide;
  Xmax = ( X2 - DrealWindowXmin ) / DrealWindowSide;
  Ymin = ( Y1 - DrealWindowYmin ) / DrealWindowSide;
  Ymax = ( Y2 - DrealWindowYmin ) / DrealWindowSide;

  if ( Xmin < 0              ) Xmin = 0;
  if ( Ymin < 0              ) Ymin = 0;
  if ( Xmin >= DrealWindowDx ) Xmin = DrealWindowDx - 1;
  if ( Ymin >= DrealWindowDy ) Ymin = DrealWindowDy - 1;

  if ( Xmax < 0              ) Xmax = 0;
  if ( Ymax < 0              ) Ymax = 0;
  if ( Xmax >= DrealWindowDx ) Xmax = DrealWindowDx - 1;
  if ( Ymax >= DrealWindowDy ) Ymax = DrealWindowDy - 1;

  DrealInterrupt = 0;
  DrealCheckInt  = 0;

  signal( SIGALRM, DrealInterruptDisplay );
  alarm( 1 );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
  {
    if ( DrealCheckInterrupt() ) break;

    StaticLayer = RDS_STATIC_LAYER[ Layer ];

    if ( DREAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 ) continue; 

    Y = Ymin;

    while ( Y <= Ymax )
    {
      if ( DrealCheckInterrupt() ) break;

      X = Xmin;

      while ( X <= Xmax )
      {
        if ( DrealCheckInterrupt() ) break;

        Offset = Y * DrealWindowDx + X;

        ScanWin = &DrealWindowTable[ Offset ];

        if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                ScanWinRec != (drealwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL  ) &&
                   ( ! IsDrealDeleted( Rec )     ) &&
                   ( Rec->X               <=  X2 ) &&
                   ( Rec->Y               <=  Y2 ) &&
                   ( ( Rec->X + Rec->DX ) >=  X1 ) &&
                   ( ( Rec->Y + Rec->DY ) >=  Y1 ) )
              {
                if ( ( ! DREAL_INSTANCE_INTERFACE ) &&
                     ( ! IsRdsFigRec( Rec )       ) &&
                     ( ! IsRdsInstance( Rec )     ) ) continue;

                if ( ( Rec->DX < LambdaMin ) &&
                     ( Rec->DY < LambdaMin ) )
                {
                  if ( ! DREAL_FORCE_DISPLAY ) continue;
                }

                if ( ! IsDrealOneWindow( Rec ) )
                {
                  X1r = ( Rec->X - DrealWindowXmin ) / DrealWindowSide;
                  Y1r = ( Rec->Y - DrealWindowYmin ) / DrealWindowSide;

                  if ( X1r < 0              ) X1r = 0;
                  if ( Y1r < 0              ) Y1r = 0;
                  if ( X1r >= DrealWindowDx ) X1r = DrealWindowDx - 1;
                  if ( Y1r >= DrealWindowDy ) Y1r = DrealWindowDy - 1;

                  if ( X1r < Xmin ) X1r = Xmin;
                  if ( Y1r < Ymin ) Y1r = Ymin;

                  if ( ( X1r != X ) || ( Y1r != Y ) )
                  {
                    if ( ! DREAL_FORCE_DISPLAY ) continue;
                  }
                }

                if ( ! IsRdsFigRec( Rec ) )
                {
                  if ( IsRdsConnector( Rec ) )
                  {
                    DrawText = DrawTextInsCon;
                  }
                  else
                  if ( IsRdsReference( Rec ) )
                  {
                    DrawText = DrawTextInsRef;
                  }
                  else
                  if ( IsRdsSegment( Rec ) )
                  {
                    DrawText = DrawTextInsSeg;
                  }
                  else
                  {
                    DrawText = DrawTextIns;
                  }
                }
                else
                {
                  if ( IsRdsConnector( Rec ) )
                  {
                    DrawText = DrawTextFigCon;
                  }
                  else
                  if ( IsRdsReference( Rec ) )
                  {
                    DrawText = DrawTextFigRef;
                  }
                  else
                  if ( IsRdsSegment( Rec ) )
                  {
                    DrawText = DrawTextFigSeg;
                  }
                  else
                  {
                    DrawText = DrawTextFig;
                  }
                }

                if ( DrawText ) SetDrealDrawText( Rec );

                if ( IsDrealAccepted( Rec ) != 0 )
                {
                  DrealAddViewLater( Rec );
                }
                else
                {
                  DrealDrawGC = DrealLayerDrawGC[ StaticLayer ];
                  DrealFillGC = DrealLayerFillGC[ StaticLayer ];

                  DrealDisplayOneRectangle( Rec );

                  if ( IsDrealDrawText( Rec ) )
                  {
                    DrealAddViewString( Rec );
                  }
                }
              }
            }
          }
        }

        X = X + 1;
      }

      Y = Y + 1;
    }

    for ( View  = DrealHeadView;
          View != (drealview *)NULL;
          View  = View->NEXT )
    {
      Rec = View->RECTANGLE;

      if ( IsDrealAccepted( Rec ) != 0 )
      {
        DrealDrawGC = DrealAcceptDrawGC;
        DrealFillGC = DrealLayerAcceptGC[ StaticLayer ];
      }

      DrealDisplayOneRectangle( Rec );
    }

    DrealDelView();
  }

  for ( View  = DrealHeadViewString;
        View != (drealview *)NULL;
        View  = View->NEXT )
  {
    Rec = View->RECTANGLE;

    DrealDisplayOneString( Rec );
  }

  DrealDelViewString();

  alarm( 0 );
  DrealFlushEventDisplay();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealDisplayRectangle                    |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayRectangle( Rectangle )

   rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;
  long         Xmin;
  long         Ymin;
  long         Xmax;
  long         Ymax;
  char         Layer;

  rdsbegin();

  Xmin = Rectangle->X;
  Ymin = Rectangle->Y;
  Xmax = Xmin + Rectangle->DX;
  Ymax = Ymin + Rectangle->DY;

  Xmin = (float)( Xmin ) * DrealLambdaGridStep;
  Ymin = (float)( Ymin ) * DrealLambdaGridStep;
  Xmax = (float)( Xmax ) * DrealLambdaGridStep;
  Ymax = (float)( Ymax ) * DrealLambdaGridStep;

  Xmin = ( Xmin / RDS_PHYSICAL_GRID ) - DrealPixelGridX - 1;
  Xmax = ( Xmax / RDS_PHYSICAL_GRID ) - DrealPixelGridX + 1;
  Ymin = ( Ymin / RDS_PHYSICAL_GRID ) - DrealPixelGridY - 1;
  Ymax = ( Ymax / RDS_PHYSICAL_GRID ) - DrealPixelGridY + 1;

  if ( Xmin < 0 ) Xmin = 0;
  if ( Ymin < 0 ) Ymin = 0;

  if ( Xmax > DrealGraphicDx ) Xmax = DrealGraphicDx;
  if ( Ymax > DrealGraphicDy ) Ymax = DrealGraphicDy;

  if ( ( Xmax > 0              ) &&
       ( Ymax > 0              ) &&
       ( Xmin < DrealGraphicDx ) &&
       ( Ymin < DrealGraphicDy ) )
  {
    DrealDisplayFigure( Xmin, Ymin , Xmax, Ymax );

    DrealRefreshGraphicWindow( Xmin, DrealGraphicDy - Ymax, 
                               Xmax - Xmin, Ymax - Ymin );
  }

  rdsend();
}
