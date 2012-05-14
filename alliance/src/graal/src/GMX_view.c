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
| File    :                   View.c                          |
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
# include <signal.h>
# include <X11/X.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GSB.h"
# include "GRM.h"
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

    static long         GraalGraphicX1;
    static long         GraalGraphicX2;
    static long         GraalGraphicY1;
    static long         GraalGraphicY2;
    static XFontStruct *GraalTextFont;
    static GC           GraalTextGC;
    static GC           GraalDrawGC;
    static GC           GraalFillGC;
    static graalview   *GraalHeadView       = (graalview *)NULL;
    static graalview   *GraalHeadViewString = (graalview *)NULL;
    static char         GraalInterrupt = 0;
    static char         GraalCheckInt  = 0;

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
|                        GraalAllocView                       |
|                                                             |
\------------------------------------------------------------*/

graalview *GraalAllocView()
{
  return((graalview *)rdsalloc(sizeof(graalview), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         GraalFreeView                       |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeView( FreeView )

   graalview *FreeView;
{
  rdsfree((char *)FreeView, sizeof(graalview));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAddViewLater                     |
|                                                             |
\------------------------------------------------------------*/

void GraalAddViewLater( Rectangle )

   rdsrec_list *Rectangle;
{
  graalview *View;

  rdsbegin();

  View = GraalAllocView();

  View->RECTANGLE = Rectangle;
  View->NEXT      = GraalHeadView;
  GraalHeadView   = View;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAddViewString                    |
|                                                             |
\------------------------------------------------------------*/

void GraalAddViewString( Rectangle )

   rdsrec_list *Rectangle;
{
  graalview *View;

  rdsbegin();

  View = GraalAllocView();

  View->RECTANGLE     = Rectangle;
  View->NEXT          = GraalHeadViewString;
  GraalHeadViewString = View;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalDelView                          |
|                                                             |
\------------------------------------------------------------*/

void GraalDelView()
{
  graalview    *DelView;
  graalview    *View;

  rdsbegin();

  View          = GraalHeadView;
  GraalHeadView = (graalview *)NULL;

  while( View != (graalview *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    GraalFreeView( DelView );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalDelViewString                      |
|                                                             |
\------------------------------------------------------------*/

void GraalDelViewString()
{
  graalview    *DelView;
  graalview    *View;

  rdsbegin();

  View                = GraalHeadViewString;
  GraalHeadViewString = (graalview *)NULL;

  while( View != (graalview *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    GraalFreeView( DelView );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalDisplayOneRectangle                    |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayOneRectangle( Rec )

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

  X1r  = (float)( Rec->X ) * GraalLambdaGridStep;
  Y1r  = (float)( Rec->Y ) * GraalLambdaGridStep;
  X2r  = (float)( Rec->X + Rec->DX ) * GraalLambdaGridStep;
  Y2r  = (float)( Rec->Y + Rec->DY ) * GraalLambdaGridStep;

  X1r  = ( X1r / GRAAL_RDS_LAMBDA ) - GraalPixelGridX;
  X2r  = ( X2r / GRAAL_RDS_LAMBDA ) - GraalPixelGridX;
  Y1r  = ( Y1r / GRAAL_RDS_LAMBDA ) - GraalPixelGridY;
  Y2r  = ( Y2r / GRAAL_RDS_LAMBDA ) - GraalPixelGridY;

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
    if ( GRAAL_FILL_MODE != GRAAL_FILL_MODE_FILL )
    {
      if ( ( DeltaX <= 4 ) ||
           ( DeltaY <= 4 ) )
      {
        if ( ( RdsLayer != RDS_NWELL ) &&
             ( RdsLayer != RDS_PWELL ) )
        {
          Small = ! GRAAL_FORCE_DISPLAY;
        }
      }
    }

    if ( Tiny )
    {
      if ( ( DeltaX <= 1 ) &&
           ( DeltaY <= 1 ) )
      {
        XDrawPoint( GraalGraphicDisplay,
                    GraalGraphicPixmap,
                    GraalDrawGC,
                    X1r, GraalGraphicDy - Y1r );
      }
      else
      if ( DeltaX <= 1 )
      {
        Xcr = ( X1r + X2r ) >> 1;

        XDrawLine( GraalGraphicDisplay,
                   GraalGraphicPixmap,
                   GraalDrawGC,
                   Xcr, GraalGraphicDy - Y1r,
                   Xcr, GraalGraphicDy - Y2r );
      }
      else
      {
        Ycr = ( Y1r + Y2r ) >> 1;

        XDrawLine( GraalGraphicDisplay,
                   GraalGraphicPixmap,
                   GraalDrawGC,
                   X1r, GraalGraphicDy - Ycr,
                   X2r, GraalGraphicDy - Ycr );
      }
    }
    else
    {
      if ( ! Small )
      {
        if ( X1r < GraalGraphicX1 ) 
        { 
          X1r = GraalGraphicX1; DrawLine |= GRAAL_WEST_MASK;  
        }
      
        if ( X2r > GraalGraphicX2 ) 
        { 
          X2r = GraalGraphicX2; DrawLine |= GRAAL_EAST_MASK;  
        }
      
        if ( Y1r < GraalGraphicY1 ) 
        {
          Y1r = GraalGraphicY1; DrawLine |= GRAAL_SOUTH_MASK; 
        }
      
        if ( Y2r > GraalGraphicY2 ) 
        {
          Y2r = GraalGraphicY2; DrawLine |= GRAAL_NORTH_MASK; 
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
          XDrawRectangle( GraalGraphicDisplay,
                          GraalGraphicPixmap,
                          GraalDrawGC,
                          X1r, GraalGraphicDy - Y2r,
                          X2r - X1r,
                          Y2r - Y1r );
        }
        else
        {
          if ( ( DrawLine & GRAAL_WEST_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       GraalGraphicPixmap,
                       GraalDrawGC,
                       X1r, GraalGraphicDy - Y2r,
                       X1r, GraalGraphicDy - Y1r );
          }
    
          if ( ( DrawLine & GRAAL_EAST_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       GraalGraphicPixmap,
                       GraalDrawGC,
                       X2r, GraalGraphicDy - Y2r,
                       X2r, GraalGraphicDy - Y1r );
          }
    
          if ( ( DrawLine & GRAAL_SOUTH_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       GraalGraphicPixmap,
                       GraalDrawGC,
                       X1r, GraalGraphicDy - Y1r,
                       X2r, GraalGraphicDy - Y1r );
          }
    
          if ( ( DrawLine & GRAAL_NORTH_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       GraalGraphicPixmap,
                       GraalDrawGC,
                       X1r, GraalGraphicDy - Y2r,
                       X2r, GraalGraphicDy - Y2r );
          }
        }
   
        if ( ( ! Small                                    ) &&
             ( RdsLayer        != RDS_ABOX                ) &&
             ( GRAAL_FILL_MODE != GRAAL_FILL_MODE_OUTLINE ) )
        {
          if ( GRAAL_FILL_MODE == GRAAL_FILL_MODE_PATTERN )
          {
            XSetFillStyle( GraalGraphicDisplay, 
                           GraalFillGC, 
                           FillStippled );
          }
          else
          {
            XSetFillStyle( GraalGraphicDisplay, 
                           GraalFillGC, 
                           FillSolid );
          }
    
          XFillRectangle( GraalGraphicDisplay,
                          GraalGraphicPixmap,
                          GraalFillGC,
                          X1r, GraalGraphicDy - Y2r,
                          X2r - X1r,
                          Y2r - Y1r );
        }
      }
    }

    if ( ( DrawLine != 0     ) ||
         ( Rec->NAME == NULL ) )
    {
      ClearGraalDrawText( Rec ); 
    }
  }
  else
  {
    ClearGraalDrawText( Rec );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalDisplayOneString                       |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayOneString( Rec )

  rdsrec_list *Rec;
{
  char       *Name;
  phcon_list *PhCon;
  char        Buffer[ 512 ];
  long        X1r;
  long        X2r;
  long        Y1r;
  long        Y2r;
  long        WidthText;
  long        HeightText;

  rdsbegin();

  X1r  = (float)( Rec->X ) * GraalLambdaGridStep;
  Y1r  = (float)( Rec->Y ) * GraalLambdaGridStep;
  X2r  = (float)( Rec->X + Rec->DX ) * GraalLambdaGridStep;
  Y2r  = (float)( Rec->Y + Rec->DY ) * GraalLambdaGridStep;

  X1r  = ( X1r / GRAAL_RDS_LAMBDA ) - GraalPixelGridX;
  X2r  = ( X2r / GRAAL_RDS_LAMBDA ) - GraalPixelGridX;
  Y1r  = ( Y1r / GRAAL_RDS_LAMBDA ) - GraalPixelGridY;
  Y2r  = ( Y2r / GRAAL_RDS_LAMBDA ) - GraalPixelGridY;

  if ( ( GRAAL_CONNECTOR_INDEX ) &&
       ( IsRdsFigRec( Rec )    ) &&
       ( IsRdsConnector( Rec ) ) )
  {
    PhCon = (phcon_list *)GRAAL_MBK( Rec );
    sprintf( Buffer, "%s.%ld", PhCon->NAME, PhCon->INDEX );
    Name  = namealloc( Buffer );
  }
  else
  {
    Name = Rec->NAME;
  }

  HeightText = GraalTextFont->ascent;
  WidthText  = XTextWidth( GraalTextFont,
                           Name, strlen( Name ) );

  if ( GRAAL_BLACK_BOX_STRING )
  {
    XDrawImageString( GraalGraphicDisplay,
                      GraalGraphicPixmap,
                      GraalTextGC,
                      (( X1r + X2r - WidthText ) >> 1),
                      GraalGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                      Name, strlen( Name ) );
  }
  else
  {
    XDrawString( GraalGraphicDisplay,
                 GraalGraphicPixmap,
                 GraalTextGC,
                 (( X1r + X2r - WidthText ) >> 1),
                 GraalGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                 Name, strlen( Name ) );
  }

  ClearGraalDrawText( Rec );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalCheckInterrupt                      |
|                                                             |
\------------------------------------------------------------*/
 
char GraalCheckInterrupt()
{
  XEvent       Event;
  KeySym       Key;
  char         Text[ 2 ];
 
  rdsbegin();

  if ( GraalCheckInt )
  {
    if ( XCheckTypedEvent( GraalGraphicDisplay, KeyPress, &Event ) )
    {
      XLookupString( &Event.xkey, Text, 1, &Key, 0 );
 
      if ( Text[ 0 ] == '\003' )
      {
        GraalInterrupt = 1;
 
        XBell( GraalGraphicDisplay, 0 );
      }
    }
  }

  GraalCheckInt = 0;

  rdsend();

  return( GraalInterrupt );
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalInterruptDisplay                    |
|                                                             |
\------------------------------------------------------------*/

void GraalInterruptDisplay()
{
  rdsbegin();

  GraalCheckInt = 1;

  signal( SIGALRM, GraalInterruptDisplay );
  alarm( 1 );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalFlushEventDisplay                   |
|                                                             |
\------------------------------------------------------------*/

void GraalFlushEventDisplay()
{
  XEvent Event;
  
  rdsbegin();

  while ( XCheckTypedEvent( GraalGraphicDisplay, KeyPress, &Event ) );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalDisplayFigure                       |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayFigure( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   long GraphicX1;
   long GraphicY1;
   long GraphicX2;
   long GraphicY2;
{
  long         Offset;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  rdsrec_list *Rec;
  rdsins_list *Instance;
  char         ScanRec;
  char         Layer;
  char         StaticLayer;
  long         X1;
  long         Y1;
  long         X2;
  long         Y2;
  long         X1r;
  long         Y1r;
  long         X;
  long         Y;
  long         Xmin;
  long         Xmax;
  long         Ymin;
  long         Ymax;
  long         LambdaMin;
  char         DrawText;
  char         DrawTextFig;
  char         DrawTextFigCon;
  char         DrawTextFigSeg;
  char         DrawTextFigRef;
  char         DrawTextIns;
  char         DrawTextInsCon;
  char         DrawTextInsSeg;
  char         DrawTextInsRef;
  graalview   *View;

  rdsbegin();

  GraalTextGC    = GraalSmallTextGC;
  GraalTextFont  = GraalSmallTextFont;
  DrawTextFig    = 0;
  DrawTextFigCon = 0;
  DrawTextFigSeg = 0;
  DrawTextFigRef = 0;
  DrawTextIns    = 0;
  DrawTextInsCon = 0;
  DrawTextInsSeg = 0;
  DrawTextInsRef = 0;
  GraalGraphicX1 = GraphicX1;
  GraalGraphicX2 = GraphicX2;
  GraalGraphicY1 = GraphicY1;
  GraalGraphicY2 = GraphicY2;

  if ( ( GraalLambdaGridStep > GRAAL_LOWER_FIGURE_STEP ) ||
       ( GRAAL_FORCE_DISPLAY                           ) )
  {
    DrawTextFig = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_FIGURE_NAME ];
  }

  if ( ( GraalLambdaGridStep > GRAAL_LOWER_INSTANCE_STEP ) ||
       ( GRAAL_FORCE_DISPLAY                             ) )
  {
    DrawTextIns = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_INSTANCE_NAME ];
  }
 
  if ( ( GraalLambdaGridStep > GRAAL_LOWER_CONNECTOR_STEP ) ||
       ( GRAAL_FORCE_DISPLAY                              ) )
  {
    DrawTextInsCon = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_INSTANCE_CONNECTOR_NAME ];
    DrawTextFigCon = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_FIGURE_CONNECTOR_NAME ];
  }

  if ( ( GraalLambdaGridStep > GRAAL_LOWER_SEGMENT_STEP ) ||
       ( GRAAL_FORCE_DISPLAY                            ) )
  {
    DrawTextFigSeg = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_FIGURE_SEGMENT_NAME ];
    DrawTextInsSeg = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_INSTANCE_SEGMENT_NAME ];
  }

  if ( ( GraalLambdaGridStep > GRAAL_LOWER_REFERENCE_STEP ) ||
       ( GRAAL_FORCE_DISPLAY                              ) )
  {
    DrawTextFigRef = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_FIGURE_REFERENCE_NAME ];
    DrawTextInsRef = GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_INSTANCE_REFERENCE_NAME ];
  }

  if ( GraalLambdaGridStep > GRAAL_LOWER_CONNECTOR_STEP )
  {
    GraalTextGC    = GraalMediumTextGC;
    GraalTextFont  = GraalMediumTextFont;
  }

  if ( GraalLambdaGridStep > GRAAL_LOWER_REFERENCE_STEP )
  {
    GraalTextGC    = GraalLargeTextGC;
    GraalTextFont  = GraalLargeTextFont;
  }

  GraalClearGraphicWindow( GraphicX1, 
                           GraalGraphicDy - GraphicY2,
                           GraphicX2 - GraphicX1,
                           GraphicY2 - GraphicY1 );

  if ( GraalFigureMbk == (phfig_list *)NULL ) return;

  LambdaMin = (float)(4.0 / ( GraalLambdaGridStep ));

  X1 = GraphicX1 + GraalPixelGridX;
  X2 = GraphicX2 + GraalPixelGridX;
  Y1 = GraphicY1 + GraalPixelGridY;
  Y2 = GraphicY2 + GraalPixelGridY;

  X1 = ( X1 / GraalLambdaGridStep );
  Y1 = ( Y1 / GraalLambdaGridStep );
  X2 = ( X2 / GraalLambdaGridStep );
  Y2 = ( Y2 / GraalLambdaGridStep );

  if ( X2 >= 0 ) { X2 = X2 + 1; }
  if ( Y2 >= 0 ) { Y2 = Y2 + 1; }
  if ( X1 <= 0 ) { X1 = X1 - 1; }
  if ( Y1 <= 0 ) { Y1 = Y1 - 1; }

  X1 = X1 * GRAAL_RDS_LAMBDA; 
  X2 = X2 * GRAAL_RDS_LAMBDA; 
  Y1 = Y1 * GRAAL_RDS_LAMBDA; 
  Y2 = Y2 * GRAAL_RDS_LAMBDA; 

  Xmin = ( X1 - GraalWindowXmin ) / GraalWindowSide;
  Xmax = ( X2 - GraalWindowXmin ) / GraalWindowSide;
  Ymin = ( Y1 - GraalWindowYmin ) / GraalWindowSide;
  Ymax = ( Y2 - GraalWindowYmin ) / GraalWindowSide;

  if ( Xmin < 0              ) Xmin = 0;
  if ( Ymin < 0              ) Ymin = 0;
  if ( Xmin >= GraalWindowDx ) Xmin = GraalWindowDx - 1;
  if ( Ymin >= GraalWindowDy ) Ymin = GraalWindowDy - 1;

  if ( Xmax < 0              ) Xmax = 0;
  if ( Ymax < 0              ) Ymax = 0;
  if ( Xmax >= GraalWindowDx ) Xmax = GraalWindowDx - 1;
  if ( Ymax >= GraalWindowDy ) Ymax = GraalWindowDy - 1;

  GraalInterrupt = 0;
  GraalCheckInt  = 0;

  signal( SIGALRM, GraalInterruptDisplay );
  alarm( 1 );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
  {
    if ( GraalCheckInterrupt() ) break;

    StaticLayer = RDS_STATIC_LAYER[ Layer ];

    if ( GRAAL_RDS_ACTIVE_LAYER_TABLE[ StaticLayer ] != 1 ) continue;

    Y = Ymin;

    while ( Y <= Ymax )
    {
      if ( GraalCheckInterrupt() ) break;

      X = Xmin;

      while ( X <= Xmax )
      {
        if ( GraalCheckInterrupt() ) break;

        Offset = Y * GraalWindowDx + X;

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

              if ( ( Rec != (rdsrec_list *)NULL  ) &&
                   ( ! IsGraalDeleted( Rec )     ) &&
                   ( ! IsGraalInvisible( Rec )   ) &&
                   ( Rec->X               <=  X2 ) &&
                   ( Rec->Y               <=  Y2 ) &&
                   ( ( Rec->X + Rec->DX ) >=  X1 ) &&
                   ( ( Rec->Y + Rec->DY ) >=  Y1 ) )
              {
 
                if ( ( ! GRAAL_INSTANCE_INTERFACE ) && 
                     ( ! IsRdsFigRec( Rec )       ) &&
                     ( ! IsRdsInstance( Rec )     ) ) continue;

                if ( ( Rec->DX < LambdaMin ) &&
                     ( Rec->DY < LambdaMin ) )
                {
                  if ( ! GRAAL_FORCE_DISPLAY ) continue;
                }

                if ( ! IsGraalOneWindow( Rec ) )
                {
                  X1r = ( Rec->X - GraalWindowXmin ) / GraalWindowSide;
                  Y1r = ( Rec->Y - GraalWindowYmin ) / GraalWindowSide;

                  if ( X1r < 0              ) X1r = 0;
                  if ( Y1r < 0              ) Y1r = 0;
                  if ( X1r >= GraalWindowDx ) X1r = GraalWindowDx - 1;
                  if ( Y1r >= GraalWindowDy ) Y1r = GraalWindowDy - 1;

                  if ( X1r < Xmin ) X1r = Xmin;
                  if ( Y1r < Ymin ) Y1r = Ymin;

                  if ( ( X1r != X ) || ( Y1r != Y ) )
                  {
                    if ( ! GRAAL_FORCE_DISPLAY ) continue;
                  }
                }

                if ( ! IsRdsFigRec( Rec ) )
                {
                  Instance = (rdsins_list *)GRAAL_PREVIOUS( Rec );

                  if ( IsGraalDeleted( Instance->LAYERTAB[ RDS_ABOX ] ) ) continue;

                  if ( IsRdsConnector( Rec ) )
                  {
                    DrawText = DrawTextInsCon;
                  }
                  else
                  if ( ( IsRdsReference( Rec ) ) ||
                       ( IsRdsVia( Rec       ) ) )
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
                  if ( ( IsRdsReference( Rec ) ) ||
                       ( IsRdsVia( Rec       ) ) )
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

                if ( DrawText ) SetGraalDrawText( Rec );

                if ( ( IsGraalAccepted( Rec ) != 0 ) || 
                     ( IsGraalTreated( Rec )  != 0 ) ||
                     ( IsGraalDruc( Rec )     != 0 ) )
                {
                  GraalAddViewLater( Rec );
                }
                else
                {
                  GraalDrawGC = GraalLayerDrawGC[ StaticLayer ];
                  GraalFillGC = GraalLayerFillGC[ StaticLayer ];

                  GraalDisplayOneRectangle( Rec );

                  if ( IsGraalDrawText( Rec ) )
                  {
                    GraalAddViewString( Rec );
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

    for ( View  = GraalHeadView;
          View != (graalview *)NULL;
          View  = View->NEXT )
    {
      Rec = View->RECTANGLE;

      if ( IsGraalAccepted( Rec ) != 0 )
      {
        GraalDrawGC = GraalAcceptDrawGC;
        GraalFillGC = GraalLayerAcceptGC[ StaticLayer ];
      }
      else
      if ( IsGraalTreated( Rec ) != 0 )
      {
        GraalDrawGC = GraalEquiDrawGC;
        GraalFillGC = GraalLayerEquiGC[ StaticLayer ];
      }
      else
      {
        GraalDrawGC = GraalDrucDrawGC;
        GraalFillGC = GraalLayerDrucGC[ StaticLayer ];
      }

      GraalDisplayOneRectangle( Rec );
    }

    GraalDelView();
  }

  for ( View  = GraalHeadViewString;
        View != (graalview *)NULL;
        View  = View->NEXT )
  {
    Rec = View->RECTANGLE;

    GraalDisplayOneString( Rec );
  }

  GraalDelViewString();

  alarm( 0 );
  GraalFlushEventDisplay();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalDisplayRectangle                    |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayRectangle( Rectangle )

   rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;
  long         Xmin;
  long         Ymin;
  long         Xmax;
  long         Ymax;

  rdsbegin();

  Xmin = Rectangle->X;
  Ymin = Rectangle->Y;
  Xmax = Xmin + Rectangle->DX;
  Ymax = Ymin + Rectangle->DY;

  ScanRec = (rdsrec_list *)(Rectangle->USER);

  while ( ScanRec != Rectangle )
  {
    if ( Xmin > ScanRec->X )
    {
      Xmin = ScanRec->X;
    }

    if ( Xmax < ( ScanRec->X + ScanRec->DX ) )
    {
      Xmax = ScanRec->X + ScanRec->DX;
    }

    if ( Ymin > ScanRec->Y )
    { 
      Ymin = ScanRec->Y;
    }

    if ( Ymax < ( ScanRec->Y + ScanRec->DY ) )
    { 
      Ymax = ScanRec->Y + ScanRec->DY;
    }

    ScanRec = (rdsrec_list *)(ScanRec->USER);
  }

  Xmin = (float)( Xmin ) * GraalLambdaGridStep;
  Ymin = (float)( Ymin ) * GraalLambdaGridStep;
  Xmax = (float)( Xmax ) * GraalLambdaGridStep;
  Ymax = (float)( Ymax ) * GraalLambdaGridStep;

  Xmin = ( Xmin / GRAAL_RDS_LAMBDA ) - GraalPixelGridX - 1;
  Xmax = ( Xmax / GRAAL_RDS_LAMBDA ) - GraalPixelGridX + 1;
  Ymin = ( Ymin / GRAAL_RDS_LAMBDA ) - GraalPixelGridY - 1;
  Ymax = ( Ymax / GRAAL_RDS_LAMBDA ) - GraalPixelGridY + 1;

  if ( Xmin < 0 ) Xmin = 0;
  if ( Ymin < 0 ) Ymin = 0;

  if ( Xmax > GraalGraphicDx ) Xmax = GraalGraphicDx;
  if ( Ymax > GraalGraphicDy ) Ymax = GraalGraphicDy;

  if ( ( Xmax > 0              ) &&
       ( Ymax > 0              ) &&
       ( Xmin < GraalGraphicDx ) &&
       ( Ymin < GraalGraphicDy ) )
  {
    GraalDisplayFigure( Xmin, Ymin , Xmax, Ymax );

    GraalRefreshGraphicWindow( Xmin, GraalGraphicDy - Ymax, 
                               Xmax - Xmin, Ymax - Ymin );
  }

  rdsend();
}
