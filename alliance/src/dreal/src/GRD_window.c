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
| File    :                 Window.c                          |
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
# include <stdlib.h>
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GSB.h"
# include "GRD.h"
# include "GRD_window.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  long         DrealWindowXmin  = 0;
  long         DrealWindowYmin  = 0;
  long         DrealWindowXmax  = 0;
  long         DrealWindowYmax  = 0;
  long         DrealWindowDx    = 0;
  long         DrealWindowDy    = 0;
  long         DrealWindowSide  = 0;
  long         DrealWindowSize  = 0;
  drealwin    *DrealWindowTable = (drealwin *)NULL;

  long         DrealBoundXmin   = 0;
  long         DrealBoundYmin   = 0;
  long         DrealBoundXmax   = 0;
  long         DrealBoundYmax   = 0;

  char         DrealRecomputeBound = DREAL_TRUE;

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
|                       DrealAllocWinLayer                    |
|                                                             |
\------------------------------------------------------------*/

drealwin *DrealAllocWinLayer( DrealWin )

  drealwin *DrealWin;
{
  if ( DrealWin->LAYERTAB == (drealwinrec **)NULL )
  {
    DrealWin->LAYERTAB = 

     (drealwinrec **)rdsallocblock( sizeof(drealwinrec *) * RDS_MAX_LAYER );
  }

  return DrealWin;
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAllocWinRec                      |
|                                                             |
\------------------------------------------------------------*/

drealwinrec *DrealAllocWinRec()
{
  return((drealwinrec *)rdsallocheap( sizeof(drealwinrec) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAllocRecWin                      |
|                                                             |
\------------------------------------------------------------*/

drealrecwin *DrealAllocRecWin()
{
  return((drealrecwin *)rdsallocheap( sizeof(drealrecwin) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        DrealFreeWinLayer                    |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeWinLayer( FreeWin )

   drealwin *FreeWin;
{
  if ( FreeWin->LAYERTAB != (drealwinrec **)NULL )
  {
    rdsfreeblock( (char *)FreeWin->LAYERTAB );
    FreeWin->LAYERTAB = (drealwinrec **)NULL;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealFreeWinRec                      |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeWinRec( FreeWinRec )

   drealwinrec *FreeWinRec;
{
  rdsfreeheap( (char *)FreeWinRec, sizeof(drealwinrec) );
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealFreeRecWin                      |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeRecWin( FreeRecWin )

   drealrecwin *FreeRecWin;
{
  rdsfreeheap((char *)FreeRecWin, sizeof(drealrecwin) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Init Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Drealgetenv                        |
|                                                             |
\------------------------------------------------------------*/

long Drealgetenv( Name )
 
  char *Name;
{
  char *String;

  rdsbegin();

  String = mbkgetenv( Name );

  if ( String == (char *)NULL )
  {
    rdsend();
    return( 0 );
  }

  rdsend();
  return( atoi( String ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealInitializeWindow                  |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeWindow()
{
  rdsbegin();

  if ( DrealWindowTable == (drealwin *)NULL )
  {
    DrealWindowDx   = Drealgetenv( DREAL_WINDOW_DX   );
    DrealWindowDy   = Drealgetenv( DREAL_WINDOW_DY   );
    DrealWindowXmin = Drealgetenv( DREAL_WINDOW_XMIN );
    DrealWindowYmin = Drealgetenv( DREAL_WINDOW_YMIN );
    DrealWindowSide = Drealgetenv( DREAL_WINDOW_SIDE );

    if ( ( DrealWindowDx   <= 0 ) ||
         ( DrealWindowDy   <= 0 ) ||
         ( DrealWindowSide <= 0 ) )
    {
      DrealWindowDx   = DREAL_DEFAULT_WINDOW_DX;
      DrealWindowDy   = DREAL_DEFAULT_WINDOW_DY;
      DrealWindowSize = DREAL_DEFAULT_WINDOW_DX * DREAL_DEFAULT_WINDOW_DY;
      DrealWindowXmin = DREAL_DEFAULT_WINDOW_XMIN * RDS_PHYSICAL_GRID;
      DrealWindowYmin = DREAL_DEFAULT_WINDOW_YMIN * RDS_PHYSICAL_GRID;
      DrealWindowXmax = DREAL_DEFAULT_WINDOW_XMAX * RDS_PHYSICAL_GRID;
      DrealWindowYmax = DREAL_DEFAULT_WINDOW_YMAX * RDS_PHYSICAL_GRID;
      DrealWindowSide = DREAL_DEFAULT_WINDOW_SIDE * RDS_PHYSICAL_GRID;
    }
    else
    {
      DrealWindowSize = DrealWindowDx * DrealWindowDy;
      DrealWindowSide = DrealWindowSide * RDS_PHYSICAL_GRID;
      DrealWindowXmin = DrealWindowXmin * RDS_PHYSICAL_GRID;
      DrealWindowYmin = DrealWindowYmin * RDS_PHYSICAL_GRID;
      DrealWindowXmax = DrealWindowXmin + ( DrealWindowDx * DrealWindowSide );
      DrealWindowYmax = DrealWindowYmin + ( DrealWindowDy * DrealWindowSide );
    }

    DrealWindowTable = 

      (drealwin *)rdsallocblock( sizeof(drealwin) * DrealWindowSize );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Purge Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       DrealEraseWindow                      |
|                                                             |
\------------------------------------------------------------*/

void DrealEraseWindow()

{
  long         Offset;
  drealwin    *ScanWin;
  drealwinrec *ScanWinRec;
  drealwinrec *DelWinRec;
  char         Layer;

  rdsbegin();

  for ( Offset = 0; Offset < DrealWindowSize; Offset++ ) 
  {
    ScanWin = &DrealWindowTable[ Offset ];

    if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];

        while ( ScanWinRec != (drealwinrec *)NULL )
        {
          DelWinRec   = ScanWinRec;
          ScanWinRec  = ScanWinRec->NEXT;

          DrealFreeWinRec( DelWinRec );
        }
      }

      DrealFreeWinLayer( ScanWin );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealInsertRectangle                     |
|                                                             |
\------------------------------------------------------------*/

void DrealInsertRectangle( Rectangle )

   rdsrec_list *Rectangle;
{
  drealwin       *LinkWin;
  drealrecwin    *RecWin;
  drealwinrec    *WinRec;
  char            Layer;
  char            Index;
  long            Offset;
 
  long            X1;
  long            X2;
  long            Y1;
  long            Y2;
  long            FirstX;

  rdsbegin();

  Y1 = ( Rectangle->Y - 1             - DrealWindowYmin ) / DrealWindowSide;
  Y2 = ( Rectangle->Y + Rectangle->DY - DrealWindowYmin ) / DrealWindowSide;
  X1 = ( Rectangle->X - 1             - DrealWindowXmin ) / DrealWindowSide;
  X2 = ( Rectangle->X + Rectangle->DX - DrealWindowXmin ) / DrealWindowSide; 

  if ( X1 < 0              ) X1 = 0;
  if ( X2 < 0              ) X2 = 0;
  if ( X1 >= DrealWindowDx ) X1 = DrealWindowDx - 1;
  if ( X2 >= DrealWindowDx ) X2 = DrealWindowDx - 1;

  if ( Y1 < 0              ) Y1 = 0;
  if ( Y2 < 0              ) Y2 = 0;
  if ( Y1 >= DrealWindowDy ) Y1 = DrealWindowDy - 1;
  if ( Y2 >= DrealWindowDy ) Y2 = DrealWindowDy - 1;

  FirstX = X1;

  while ( Y1 <= Y2 )
  {
    X1 = FirstX;

    while ( X1 <= X2 )
    {
      Offset = Y1 * DrealWindowDx + X1;

      LinkWin = &DrealWindowTable[ Offset ];

      if ( LinkWin->LAYERTAB == (drealwinrec **)NULL )
      {
        DrealAllocWinLayer( LinkWin );
      }

      if ( DREAL_WINDOW( Rectangle ) == (drealrecwin *)NULL )
      {
        DREAL_WINDOW( Rectangle ) = (drealrecwin *)LinkWin;

        SetDrealOneWindow( Rectangle );
      }
      else
      {   
        if ( IsDrealOneWindow( Rectangle ) )
        {
          ClearDrealOneWindow( Rectangle );

          RecWin                    = DrealAllocRecWin();
          RecWin->WINDOW            = (drealwin *)DREAL_WINDOW( Rectangle );
          DREAL_WINDOW( Rectangle ) = RecWin;
        }

        RecWin                    = DrealAllocRecWin();
        RecWin->WINDOW            = LinkWin;
        RecWin->NEXT              = DREAL_WINDOW( Rectangle );
        DREAL_WINDOW( Rectangle ) = RecWin;
      }

      Layer = GetRdsLayer( Rectangle );
 
      WinRec = LinkWin->LAYERTAB[ (int)Layer ];
 
      if ( WinRec == (drealwinrec *)NULL )
      {
        WinRec                          = DrealAllocWinRec();
        LinkWin->LAYERTAB[ (int)Layer ] = WinRec;
        WinRec->RECTAB[ 0 ]             = Rectangle;
      }
      else
      {   
        for ( Index = 0; Index < DREAL_MAX_REC; Index++ )
        {
          if ( WinRec->RECTAB[ (int)Index ] == (rdsrec_list *)NULL ) break;
        }

        if ( Index == DREAL_MAX_REC )
        {
          WinRec                          = DrealAllocWinRec();
          WinRec->NEXT                    = LinkWin->LAYERTAB[ (int)Layer ];
          LinkWin->LAYERTAB[ (int)Layer ] = WinRec;
          WinRec->RECTAB[ 0 ]             = Rectangle;
        }
        else
        {
          WinRec->RECTAB[ (int)Index ] = Rectangle;
        }
      }
        
      X1 = X1 + 1;
    }
 
    Y1  = Y1 + 1;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealEraseRectangle                     |
|                                                             |
\------------------------------------------------------------*/

void DrealEraseRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  drealrecwin  StaticRecWin;
  drealrecwin *ScanRecWin;
  drealrecwin *DelRecWin;
  drealwinrec *ScanWinRec; 
  drealwinrec *FirstWinRec;
  drealwin    *ScanWin;

  char         Layer;
  char         Index;
  char         Found;

  rdsbegin();

  if ( IsDrealOneWindow( Rectangle ) )
  {
    StaticRecWin.NEXT   = (drealrecwin *)NULL;
    StaticRecWin.WINDOW = (drealwin *)DREAL_WINDOW( Rectangle );
    ScanRecWin          = &StaticRecWin;
  }
  else
  {
    ScanRecWin = DREAL_WINDOW( Rectangle );
  }

  Layer = GetRdsLayer( Rectangle );

  while ( ScanRecWin != (drealrecwin *)NULL )
  {
    DelRecWin  = ScanRecWin;
    ScanRecWin = ScanRecWin->NEXT;

    ScanWin = DelRecWin->WINDOW;
    Found   = DREAL_MAX_REC;

    FirstWinRec = ScanWin->LAYERTAB[ (int)Layer ];

    for ( ScanWinRec  = FirstWinRec;
          ScanWinRec != (drealwinrec *)NULL;
          ScanWinRec  = ScanWinRec->NEXT )
    {
      for ( Index = 0; Index < DREAL_MAX_REC; Index++ )
      {
        if ( ScanWinRec->RECTAB[ (int)Index ] == Rectangle )
        {
          Found = Index; break;
        }
      }

      if ( Found != DREAL_MAX_REC ) break;
    }

    if ( ScanWinRec == FirstWinRec )
    {
      ScanWinRec->RECTAB[ (int)Found ] = (rdsrec_list *)NULL;
    }
    else
    {
      for ( Index = 0; Index < DREAL_MAX_REC; Index++ )
      {
        if ( FirstWinRec->RECTAB[ (int)Index ] != (rdsrec_list *)NULL ) break;
      }

      ScanWinRec->RECTAB[ (int)Found ]  = FirstWinRec->RECTAB[ (int)Index ];
      FirstWinRec->RECTAB[ (int)Index ] = (rdsrec_list *)NULL;
    }

    for ( Index = 0; Index < DREAL_MAX_REC; Index++ )
    {
      if ( FirstWinRec->RECTAB[ (int)Index ] != (rdsrec_list *)NULL ) break;
    }

    if ( Index == DREAL_MAX_REC )
    {
      ScanWin->LAYERTAB[ (int)Layer ] = FirstWinRec->NEXT;

      DrealFreeWinRec( FirstWinRec );
    }
  
    if ( DelRecWin != &StaticRecWin )
    {
      DrealFreeRecWin( DelRecWin );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealEraseRecWin                        |
|                                                             |
\------------------------------------------------------------*/
 
void DrealEraseRecWin( Rectangle )

  rdsrec_list *Rectangle;
{ 
  drealrecwin *ScanRecWin;
  drealrecwin *DelRecWin;

  rdsbegin();

  if ( ! IsDrealOneWindow( Rectangle ) )
  { 
    ScanRecWin = DREAL_WINDOW( Rectangle );

    do
    {
      DelRecWin  = ScanRecWin;
      ScanRecWin = ScanRecWin->NEXT;

      DrealFreeRecWin( DelRecWin );
    }
    while ( ScanRecWin != (drealrecwin *)NULL );
  } 

  DREAL_WINDOW( Rectangle ) = (drealrecwin *)NULL;

  rdsend();
} 

/*------------------------------------------------------------\
|                                                             |
|                     DrealViewWindow                         |
|                                                             |
\------------------------------------------------------------*/

# ifdef DEBUG

void DrealViewWindow()
{
  drealwin       *ScanWin;
  drealwinrec    *ScanWinRec;
  rdsrec_list    *Rectangle;
  long            X;
  long            Y;
  long            Xmin;
  long            Ymin;
  long            Xmax;
  long            Ymax;
  long            Counter;
  long            MaskCounter;
  char            Layer;
  char            ScanRec;

  rdsbegin();
 
  fprintf( stdout, "\n\t--> Window\n" );
  fprintf( stdout, "\n\tXMIN : %d"  , DrealWindowXmin );
  fprintf( stdout, "\n\tXMAX : %d"  , DrealWindowXmax );
  fprintf( stdout, "\n\tYMIN : %d"  , DrealWindowYmin );
  fprintf( stdout, "\n\tYMAX : %d"  , DrealWindowYmax );
  fprintf( stdout, "\n\tDX   : %d"  , DrealWindowDx   );
  fprintf( stdout, "\n\tDY   : %d"  , DrealWindowDy   );
  fprintf( stdout, "\n\tSIDE : %d"  , DrealWindowSide );
  fprintf( stdout, "\n\tSIZE : %d\n", DrealWindowSize );
 
  fprintf( stdout, "\n\t--> Dump all windows\n" );
 
  Ymin = DrealWindowYmin;
  Ymax = Ymin + DrealWindowSide;
  Y    = 0;
 
  MaskCounter = 0;

  while ( Y < DrealWindowDy )
  {
    Xmin = DrealWindowXmin;
    Xmax = Xmin + DrealWindowSide ;
    X    = 0;
 
    ScanWin = DrealWindowTable[ Y * DrealWindowDx ];

    while ( X < DrealWindowDx )
    {
      fprintf( stdout, "\n\t--> Window[ %d, %d ]\n", X, Y );

      fprintf( stdout, "\n\tXMIN : %d"  , Xmin );
      fprintf( stdout, "\n\tYMIN : %d"  , Ymin );
      fprintf( stdout, "\n\tXMAX : %d"  , Xmax );
      fprintf( stdout, "\n\tYMAX : %d\n", Ymax );

      if ( ScanWin != (drealwin *)NULL )
      {
        Counter = 0;

        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                ScanWinRec != (drealwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
            {
              Rectangle = ScanWinRec->RECTAB[ ScanRec ];

              if ( Rectangle != (rdsrec_list *)NULL )
              {
                if ( ( Rectangle->X > Xmax                 ) ||
                     ( Rectangle->Y > Ymax                 ) ||
                     ( Rectangle->X + Rectangle->DX < Xmin ) ||
                     ( Rectangle->Y + Rectangle->DY < Ymin ) )
                {
                  viewrdsrec( Rectangle );
                }
                else
                {   
                  Counter = Counter + 1;
                }
              }  
            }  
          }
        }

        MaskCounter = MaskCounter + Counter;

        fprintf( stdout, "\n\t<-- Total rectangles %d", Counter );
      }

      Xmin = Xmax;
      Xmax = Xmax + DrealWindowSide;
      X    = X + 1;

      ScanWin = DrealWindowTable[ Y * DrealWindowDx + X ];
    }

    Ymin = Ymax;
    Ymax = Ymax + DrealWindowSide;
    Y    = Y + 1;
  }

  fprintf( stdout, "\n<-- Total Rectangles %d\n", MaskCounter );

  rdsend();
}

# endif

/*------------------------------------------------------------\
|                                                             |
|                     DrealComputeBound                       |
|                                                             |
\------------------------------------------------------------*/

char DrealComputeBound()

{
  drealwin       *ScanWin;
  drealwinrec    *ScanWinRec;
  rdsrec_list    *Rec;
  long            Offset;
  long            X;
  long            Y;
  char            Layer;
  char            FirstBound;
  char            ScanRec;

  if ( DrealFigureRds == (rdsfig_list *)NULL )
  {
    return( DREAL_FALSE );
  }

  if ( DrealRecomputeBound == DREAL_FALSE )
  {
    return( DREAL_TRUE );
  }

  rdsbegin();

  DrealBoundXmin = 0;
  DrealBoundXmax = 0;
  DrealBoundYmin = 0;
  DrealBoundYmax = 0;
  
  Y = 0;

  FirstBound = 1;
 
  while ( ( Y < DrealWindowDy ) && 
          ( FirstBound == 1   ) )
  {
    Offset = Y * DrealWindowDx;
    X      = 0;

    while ( X < DrealWindowDx )
    {
      ScanWin = &DrealWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];
                ScanWinRec != (drealwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsDrealDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound = 0;
                  DrealBoundYmin = Rec->Y;
                }
                else
                if ( Rec->Y < DrealBoundYmin ) 
                {
                  DrealBoundYmin = Rec->Y;
                }
              }
            }
          }
        }
      }

      X      = X + 1;
      Offset = Offset + 1;
    }

    Y = Y + 1;
  }

  if ( FirstBound == 1 )
  {
    rdsend();
    return( DREAL_FALSE );
  }

  FirstBound = 1;

  Y = DrealWindowDy - 1;

  while ( ( Y >= 0            ) && 
          ( FirstBound == 1   ) )
  {
    Offset = Y * DrealWindowDx;
    X      = 0;

    while ( X < DrealWindowDx )
    {
      ScanWin = &DrealWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];
                ScanWinRec != (drealwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsDrealDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound     = 0;
                  DrealBoundYmax = Rec->Y + Rec->DY;
                }
                else
                if ( ( Rec->Y + Rec->DY ) > DrealBoundYmax ) 
                {
                  DrealBoundYmax = Rec->Y + Rec->DY;
                }
              }
            }
          }
        }
      }

      X      = X + 1;
      Offset = Offset + 1;
    }

    Y = Y - 1;
  }

  FirstBound = 1;

  X = DrealWindowDx - 1;

  while ( ( X >= 0            ) && 
          ( FirstBound == 1   ) )
  {
    Offset = X;
    Y      = 0;

    while ( Y < DrealWindowDy )
    {
      ScanWin = &DrealWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];
                ScanWinRec != (drealwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsDrealDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound     = 0;
                  DrealBoundXmax = Rec->X + Rec->DX;
                }
                else
                if ( ( Rec->X + Rec->DX ) > DrealBoundXmax ) 
                {
                  DrealBoundXmax = Rec->X + Rec->DX;
                }
              }
            }
          }
        }
      }

      Y      = Y + 1;
      Offset = Offset + DrealWindowDx;
    }

    X = X - 1;
  }

  X = 0;

  FirstBound = 1;
 
  while ( ( X < DrealWindowDx ) && 
          ( FirstBound == 1   ) )
  {
    Offset = X;
    Y      = 0;

    while ( Y < DrealWindowDy )
    {
      ScanWin = &DrealWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (drealwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ (int)Layer ];
                ScanWinRec != (drealwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < DREAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsDrealDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound = 0;
                  DrealBoundXmin = Rec->X;
                }
                else
                if ( Rec->X < DrealBoundXmin ) 
                {
                  DrealBoundXmin = Rec->X;
                }
              }
            }
          }
        }
      }

      Y      = Y + 1;
      Offset = Offset + DrealWindowDx;
    }

    X = X + 1;
  }

  DrealRecomputeBound = DREAL_FALSE;

  rdsend();
  return( DREAL_TRUE );
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealCheckWindow                       |
|                                                             |
\------------------------------------------------------------*/

void DrealCheckWindow( CheckXmin, CheckYmin, CheckXmax, CheckYmax  )

  long  CheckXmin;
  long  CheckYmin;
  long  CheckXmax;
  long  CheckYmax;
{
  long  Border;
  long  SideX;
  long  SideY;
  char  Compute;

  rdsbegin();

  Compute = 0;
  Border  = DREAL_WINDOW_BORDER * RDS_PHYSICAL_GRID;

  if ( ( CheckXmin - Border ) < DrealWindowXmin )
  {
    DrealWindowXmin = CheckXmin - Border;
    Compute         = 1;
  }

  if ( ( CheckYmin - Border ) < DrealWindowYmin )
  {
    DrealWindowYmin = CheckYmin - Border;
    Compute         = 1;
  }

  if ( ( CheckXmax + Border ) > DrealWindowXmax )
  {
    DrealWindowXmax = CheckXmax + Border;
    Compute         = 1;
  }

  if ( ( CheckYmax + Border ) > DrealWindowYmax )
  {
    DrealWindowYmax = CheckYmax + Border;
    Compute         = 1;
  }

  if ( Compute )
  {
    DrealEraseWindow();
    rdsfreeblock( DrealWindowTable );

    DrealWindowDx = DREAL_DEFAULT_WINDOW_DX;
    DrealWindowDy = DREAL_DEFAULT_WINDOW_DY;

    SideX = 1 + ( DrealWindowXmax - DrealWindowXmin ) / DrealWindowDx;
    SideY = 1 + ( DrealWindowYmax - DrealWindowYmin ) / DrealWindowDy;

    if ( SideX < SideY )
    {
      DrealWindowSide = SideY;
    }
    else
    {
      DrealWindowSide = SideX;
    }

    DrealWindowDx = 1 + ( DrealWindowXmax - DrealWindowXmin ) / DrealWindowSide;
    DrealWindowDy = 1 + ( DrealWindowYmax - DrealWindowYmin ) / DrealWindowSide;

    DrealWindowXmax = DrealWindowXmin + ( DrealWindowDx * DrealWindowSide );
    DrealWindowYmax = DrealWindowYmin + ( DrealWindowDy * DrealWindowSide );
    DrealWindowSize = DrealWindowDx * DrealWindowDy;

    DrealWindowTable =

      (drealwin *)rdsallocblock( sizeof(drealwin) * DrealWindowSize );
  }

  rdsend();
}
