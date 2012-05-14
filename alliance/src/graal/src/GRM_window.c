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
| File    :                 Window.c                          |
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
# include <stdlib.h>
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GSB.h"
# include "GRM.h"
# include "GRM_window.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  long         GraalWindowXmin  = 0;
  long         GraalWindowYmin  = 0;
  long         GraalWindowXmax  = 0;
  long         GraalWindowYmax  = 0;
  long         GraalWindowDx    = 0;
  long         GraalWindowDy    = 0;
  long         GraalWindowSide  = 0;
  long         GraalWindowSize  = 0;
  graalwin    *GraalWindowTable = (graalwin *)NULL;

  long         GraalBoundXmin   = 0;
  long         GraalBoundYmin   = 0;
  long         GraalBoundXmax   = 0;
  long         GraalBoundYmax   = 0;

  char         GraalRecomputeBound = GRAAL_TRUE; 

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
|                       GraalAllocWinLayer                    |
|                                                             |
\------------------------------------------------------------*/

graalwin *GraalAllocWinLayer( GraalWin )

  graalwin *GraalWin;
{
  if ( GraalWin->LAYERTAB == (graalwinrec **)NULL )
  {
    GraalWin->LAYERTAB =

     (graalwinrec **)rdsallocblock( sizeof(graalwinrec *) * RDS_MAX_LAYER );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAllocWinRec                      |
|                                                             |
\------------------------------------------------------------*/

graalwinrec *GraalAllocWinRec()
{
  return((graalwinrec *)rdsalloc(sizeof(graalwinrec), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAllocRecWin                      |
|                                                             |
\------------------------------------------------------------*/

graalrecwin *GraalAllocRecWin()
{
  return((graalrecwin *)rdsalloc(sizeof(graalrecwin), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        GraalFreeWinLayer                    |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeWinLayer( FreeWin )

   graalwin *FreeWin;
{
  if ( FreeWin->LAYERTAB != (graalwinrec **)NULL )
  {
    rdsfreeblock( (char *)FreeWin->LAYERTAB );
    FreeWin->LAYERTAB = (graalwinrec **)NULL;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalFreeWinRec                      |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeWinRec( FreeWinRec )

   graalwinrec *FreeWinRec;
{
  rdsfree((char *)FreeWinRec, sizeof(graalwinrec));
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalFreeRecWin                      |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeRecWin( FreeRecWin )

   graalrecwin *FreeRecWin;
{
  rdsfree((char *)FreeRecWin, sizeof(graalrecwin));
}

/*------------------------------------------------------------\
|                                                             |
|                        Init Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Graalgetenv                        |
|                                                             |
\------------------------------------------------------------*/

long Graalgetenv( Name )
 
  char *Name;
{
  char *String;

  String = mbkgetenv( Name );

  if ( String == (char *)NULL )
  {
    return( 0 );
  }

  return( atoi( String ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalInitializeWindow                  |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeWindow()
{
  rdsbegin();

  if ( GraalWindowTable == (graalwin *)NULL )
  {
    GraalWindowDx   = Graalgetenv( GRAAL_WINDOW_DX   );
    GraalWindowDy   = Graalgetenv( GRAAL_WINDOW_DY   );
    GraalWindowXmin = Graalgetenv( GRAAL_WINDOW_XMIN );
    GraalWindowYmin = Graalgetenv( GRAAL_WINDOW_YMIN );
    GraalWindowSide = Graalgetenv( GRAAL_WINDOW_SIDE );

    if ( ( GraalWindowDx   <= 0 ) ||
         ( GraalWindowDy   <= 0 ) ||
         ( GraalWindowSide <= 0 ) )
    {
      GraalWindowDx   = GRAAL_DEFAULT_WINDOW_DX;
      GraalWindowDy   = GRAAL_DEFAULT_WINDOW_DY;
      GraalWindowSize = GRAAL_DEFAULT_WINDOW_DX * GRAAL_DEFAULT_WINDOW_DY;
      GraalWindowXmin = GRAAL_DEFAULT_WINDOW_XMIN * GRAAL_RDS_LAMBDA;
      GraalWindowYmin = GRAAL_DEFAULT_WINDOW_YMIN * GRAAL_RDS_LAMBDA;
      GraalWindowXmax = GRAAL_DEFAULT_WINDOW_XMAX * GRAAL_RDS_LAMBDA;
      GraalWindowYmax = GRAAL_DEFAULT_WINDOW_YMAX * GRAAL_RDS_LAMBDA;
      GraalWindowSide = GRAAL_DEFAULT_WINDOW_SIDE * GRAAL_RDS_LAMBDA;
    }
    else
    {
      GraalWindowSize = GraalWindowDx * GraalWindowDy;
      GraalWindowSide = GraalWindowSide * GRAAL_RDS_LAMBDA;
      GraalWindowXmin = GraalWindowXmin * GRAAL_RDS_LAMBDA;
      GraalWindowYmin = GraalWindowYmin * GRAAL_RDS_LAMBDA;
      GraalWindowXmax = GraalWindowXmin + ( GraalWindowDx * GraalWindowSide );
      GraalWindowYmax = GraalWindowYmin + ( GraalWindowDy * GraalWindowSide );
    }

    GraalWindowTable = 

      (graalwin *)rdsallocblock( sizeof(graalwin) * GraalWindowSize );
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
|                       GraalEraseWindow                      |
|                                                             |
\------------------------------------------------------------*/

void GraalEraseWindow()

{
  long         Offset;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  graalwinrec *DelWinRec;
  char         Layer;

  rdsbegin();

  for ( Offset = 0; Offset < GraalWindowSize; Offset++ ) 
  {
    ScanWin = &GraalWindowTable[ Offset ];

    if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        ScanWinRec  = ScanWin->LAYERTAB[ Layer ];

        while ( ScanWinRec != (graalwinrec *)NULL )
        {
          DelWinRec   = ScanWinRec;
          ScanWinRec  = ScanWinRec->NEXT;

          GraalFreeWinRec( DelWinRec );
        }
      }

      GraalFreeWinLayer( ScanWin );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalInsertRectangle                     |
|                                                             |
\------------------------------------------------------------*/

void GraalInsertRectangle( Rectangle )

   rdsrec_list *Rectangle;
{
  graalwin       *LinkWin;
  graalrecwin    *RecWin;
  graalwinrec    *WinRec;
  char            Layer;
  char            Index;
  long            Offset;
 
  long            X1;
  long            X2;
  long            Y1;
  long            Y2;
  long            FirstX;

  rdsbegin();

  Y1 = ( Rectangle->Y - 1             - GraalWindowYmin ) / GraalWindowSide;
  Y2 = ( Rectangle->Y + Rectangle->DY - GraalWindowYmin ) / GraalWindowSide;
  X1 = ( Rectangle->X - 1             - GraalWindowXmin ) / GraalWindowSide;
  X2 = ( Rectangle->X + Rectangle->DX - GraalWindowXmin ) / GraalWindowSide; 

  if ( X1 < 0              ) X1 = 0;
  if ( X2 < 0              ) X2 = 0;
  if ( X1 >= GraalWindowDx ) X1 = GraalWindowDx - 1;
  if ( X2 >= GraalWindowDx ) X2 = GraalWindowDx - 1;

  if ( Y1 < 0              ) Y1 = 0;
  if ( Y2 < 0              ) Y2 = 0;
  if ( Y1 >= GraalWindowDy ) Y1 = GraalWindowDy - 1;
  if ( Y2 >= GraalWindowDy ) Y2 = GraalWindowDy - 1;

  FirstX = X1;

  while ( Y1 <= Y2 )
  {
    X1 = FirstX;

    while ( X1 <= X2 )
    {
      Offset = Y1 * GraalWindowDx + X1;

      LinkWin = &GraalWindowTable[ Offset ];

      if ( LinkWin->LAYERTAB == (graalwinrec **)NULL )
      {
        GraalAllocWinLayer( LinkWin );
      }

      if ( GRAAL_WINDOW( Rectangle ) == (graalrecwin *)NULL )
      {
        GRAAL_WINDOW( Rectangle ) = (graalrecwin *)LinkWin;

        SetGraalOneWindow( Rectangle );
      }
      else
      {   
        if ( IsGraalOneWindow( Rectangle ) )
        {
          ClearGraalOneWindow( Rectangle );

          RecWin                    = GraalAllocRecWin();
          RecWin->WINDOW            = (graalwin *)GRAAL_WINDOW( Rectangle );
          GRAAL_WINDOW( Rectangle ) = RecWin;
        }

        RecWin                    = GraalAllocRecWin();
        RecWin->WINDOW            = LinkWin;
        RecWin->NEXT              = GRAAL_WINDOW( Rectangle );
        GRAAL_WINDOW( Rectangle ) = RecWin;
      }

      Layer = GetRdsLayer( Rectangle );
 
      WinRec = LinkWin->LAYERTAB[ Layer ];
 
      if ( WinRec == (graalwinrec *)NULL )
      {
        WinRec                     = GraalAllocWinRec();
        LinkWin->LAYERTAB[ Layer ] = WinRec;
        WinRec->RECTAB[ 0 ]        = Rectangle;
      }
      else
      {   
        for ( Index = 0; Index < GRAAL_MAX_REC; Index++ )
        {
          if ( WinRec->RECTAB[ Index ] == (rdsrec_list *)NULL ) break;
        }

        if ( Index == GRAAL_MAX_REC )
        {
          WinRec                     = GraalAllocWinRec();
          WinRec->NEXT               = LinkWin->LAYERTAB[ Layer ];
          LinkWin->LAYERTAB[ Layer ] = WinRec;
          WinRec->RECTAB[ 0 ]        = Rectangle;
        }
        else
        {
          WinRec->RECTAB[ Index ] = Rectangle;
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
|                     GraalEraseRectangle                     |
|                                                             |
\------------------------------------------------------------*/

void GraalEraseRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  graalrecwin  StaticRecWin;
  graalrecwin *ScanRecWin;
  graalrecwin *DelRecWin;
  graalwinrec *ScanWinRec; 
  graalwinrec *FirstWinRec;
  graalwin    *ScanWin;

  char         Layer;
  char         Index;
  char         Found;

  rdsbegin();

  if ( IsGraalOneWindow( Rectangle ) )
  {
    StaticRecWin.NEXT   = (graalrecwin *)NULL;
    StaticRecWin.WINDOW = (graalwin *)GRAAL_WINDOW( Rectangle );
    ScanRecWin          = &StaticRecWin;
  }
  else
  {
    ScanRecWin = GRAAL_WINDOW( Rectangle );
  }

  Layer = GetRdsLayer( Rectangle );

  while ( ScanRecWin != (graalrecwin *)NULL )
  {
    DelRecWin  = ScanRecWin;
    ScanRecWin = ScanRecWin->NEXT;

    ScanWin = DelRecWin->WINDOW;
    Found   = GRAAL_MAX_REC;

    FirstWinRec = ScanWin->LAYERTAB[ Layer ];

    for ( ScanWinRec  = FirstWinRec;
          ScanWinRec != (graalwinrec *)NULL;
          ScanWinRec  = ScanWinRec->NEXT )
    {
      for ( Index = 0; Index < GRAAL_MAX_REC; Index++ )
      {
        if ( ScanWinRec->RECTAB[ Index ] == Rectangle )
        {
          Found = Index; break;
        }
      }

      if ( Found != GRAAL_MAX_REC ) break;
    }

    if ( ScanWinRec == FirstWinRec )
    {
      ScanWinRec->RECTAB[ Found ] = (rdsrec_list *)NULL;
    }
    else
    {
      for ( Index = 0; Index < GRAAL_MAX_REC; Index++ )
      {
        if ( FirstWinRec->RECTAB[ Index ] != (rdsrec_list *)NULL ) break;
      }

      ScanWinRec->RECTAB[ Found ]  = FirstWinRec->RECTAB[ Index ];
      FirstWinRec->RECTAB[ Index ] = (rdsrec_list *)NULL;
    }

    for ( Index = 0; Index < GRAAL_MAX_REC; Index++ )
    {
      if ( FirstWinRec->RECTAB[ Index ] != (rdsrec_list *)NULL ) break;
    }

    if ( Index == GRAAL_MAX_REC )
    {
      ScanWin->LAYERTAB[ Layer ] = FirstWinRec->NEXT;

      GraalFreeWinRec( FirstWinRec );
    }
  
    if ( DelRecWin != &StaticRecWin )
    {
      GraalFreeRecWin( DelRecWin );
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalEraseRecWin                        |
|                                                             |
\------------------------------------------------------------*/
 
void GraalEraseRecWin( Rectangle )

  rdsrec_list *Rectangle;
{ 
  graalrecwin *ScanRecWin;
  graalrecwin *DelRecWin;

  rdsbegin();

  if ( ! IsGraalOneWindow( Rectangle ) )
  { 
    ScanRecWin = GRAAL_WINDOW( Rectangle );

    do
    {
      DelRecWin  = ScanRecWin;
      ScanRecWin = ScanRecWin->NEXT;

      GraalFreeRecWin( DelRecWin );
    }
    while ( ScanRecWin != (graalrecwin *)NULL );
  } 

  GRAAL_WINDOW( Rectangle ) = (graalrecwin *)NULL;

  rdsend();
} 

/*------------------------------------------------------------\
|                                                             |
|                     GraalViewWindow                         |
|                                                             |
\------------------------------------------------------------*/

# ifdef DEBUG

void GraalViewWindow()
{
  graalwin       *ScanWin;
  graalwinrec    *ScanWinRec;
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
  fprintf( stdout, "\n\tXMIN : %d"  , GraalWindowXmin );
  fprintf( stdout, "\n\tXMAX : %d"  , GraalWindowXmax );
  fprintf( stdout, "\n\tYMIN : %d"  , GraalWindowYmin );
  fprintf( stdout, "\n\tYMAX : %d"  , GraalWindowYmax );
  fprintf( stdout, "\n\tDX   : %d"  , GraalWindowDx   );
  fprintf( stdout, "\n\tDY   : %d"  , GraalWindowDy   );
  fprintf( stdout, "\n\tSIDE : %d"  , GraalWindowSide );
  fprintf( stdout, "\n\tSIZE : %d\n", GraalWindowSize );
 
  fprintf( stdout, "\n\t--> Dump all windows\n" );
 
  Ymin = GraalWindowYmin;
  Ymax = Ymin + GraalWindowSide;
  Y    = 0;
 
  MaskCounter = 0;

  while ( Y < GraalWindowDy )
  {
    Xmin = GraalWindowXmin;
    Xmax = Xmin + GraalWindowSide ;
    X    = 0;
 
    ScanWin = GraalWindowTable[ Y * GraalWindowDx ];

    while ( X < GraalWindowDx )
    {
      if ( ScanWin != (graalwin *)NULL )
      {
        fprintf( stdout, "\n\t--> Window[ %d, %d ]\n", X, Y );

        fprintf( stdout, "\n\tXMIN : %d"  , Xmin );
        fprintf( stdout, "\n\tYMIN : %d"  , Ymin );
        fprintf( stdout, "\n\tXMAX : %d"  , Xmax );
        fprintf( stdout, "\n\tYMAX : %d\n", Ymax );

        Counter = 0;

        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                ScanWinRec != (graalwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
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
      Xmax = Xmax + GraalWindowSide;
      X    = X + 1;

      ScanWin = GraalWindowTable[ Y * GraalWindowDx + X ];
    }

    Ymin = Ymax;
    Ymax = Ymax + GraalWindowSide;
    Y    = Y + 1;
  }

  fprintf( stdout, "\n<-- Total Rectangles %d\n", MaskCounter );

  rdsend();
}

# endif 

/*------------------------------------------------------------\
|                                                             |
|                     GraalComputeBound                       |
|                                                             |
\------------------------------------------------------------*/

char GraalComputeBound()

{
  graalwin       *ScanWin;
  graalwinrec    *ScanWinRec;
  rdsrec_list    *Rec;
  long            Offset;
  long            X;
  long            Y;
  char            Layer;
  char            FirstBound;
  char            ScanRec;

  if ( GraalFigureMbk == (phfig_list *)NULL )
  {
    return( GRAAL_FALSE );
  }

  if ( GraalRecomputeBound == GRAAL_FALSE )
  {
    return( GRAAL_TRUE );
  }

  rdsbegin();

  GraalBoundXmin = 0;
  GraalBoundXmax = 0;
  GraalBoundYmin = 0;
  GraalBoundYmax = 0;
  
  Y = 0;

  FirstBound = 1;
 
  while ( ( Y < GraalWindowDy ) && 
          ( FirstBound == 1   ) )
  {
    Offset = Y * GraalWindowDx;
    X      = 0;

    while ( X < GraalWindowDx )
    {
      ScanWin = &GraalWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                ScanWinRec != (graalwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsGraalDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound = 0;
                  GraalBoundYmin = Rec->Y;
                }
                else
                if ( Rec->Y < GraalBoundYmin ) 
                {
                  GraalBoundYmin = Rec->Y;
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
    return( GRAAL_FALSE );
  }

  FirstBound = 1;

  Y = GraalWindowDy - 1;

  while ( ( Y >= 0            ) && 
          ( FirstBound == 1   ) )
  {
    Offset = Y * GraalWindowDx;
    X      = 0;

    while ( X < GraalWindowDx )
    {
      ScanWin = &GraalWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                ScanWinRec != (graalwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsGraalDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound     = 0;
                  GraalBoundYmax = Rec->Y + Rec->DY;
                }
                else
                if ( ( Rec->Y + Rec->DY ) > GraalBoundYmax ) 
                {
                  GraalBoundYmax = Rec->Y + Rec->DY;
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

  X = GraalWindowDx - 1;

  while ( ( X >= 0            ) && 
          ( FirstBound == 1   ) )
  {
    Offset = X;
    Y      = 0;

    while ( Y < GraalWindowDy )
    {
      ScanWin = &GraalWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                ScanWinRec != (graalwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsGraalDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound     = 0;
                  GraalBoundXmax = Rec->X + Rec->DX;
                }
                else
                if ( ( Rec->X + Rec->DX ) > GraalBoundXmax ) 
                {
                  GraalBoundXmax = Rec->X + Rec->DX;
                }
              }
            }
          }
        }
      }

      Y      = Y + 1;
      Offset = Offset + GraalWindowDx;
    }

    X = X - 1;
  }

  X = 0;

  FirstBound = 1;
 
  while ( ( X < GraalWindowDx ) && 
          ( FirstBound == 1   ) )
  {
    Offset = X;
    Y      = 0;

    while ( Y < GraalWindowDy )
    {
      ScanWin = &GraalWindowTable[ Offset ];

      if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
      {
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                ScanWinRec != (graalwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL ) &&
                   ( ! IsGraalDeleted( Rec )    ) )
              {
                if ( FirstBound == 1 )
                {
                  FirstBound = 0;
                  GraalBoundXmin = Rec->X;
                }
                else
                if ( Rec->X < GraalBoundXmin ) 
                {
                  GraalBoundXmin = Rec->X;
                }
              }
            }
          }
        }
      }

      Y      = Y + 1;
      Offset = Offset + GraalWindowDx;
    }

    X = X + 1;
  }

  GraalRecomputeBound = GRAAL_FALSE;

  rdsend();
  return( GRAAL_TRUE );
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalCheckWindow                       |
|                                                             |
\------------------------------------------------------------*/

void GraalCheckWindow( CheckXmin, CheckYmin, CheckXmax, CheckYmax  )

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
  Border  = GRAAL_WINDOW_BORDER * GRAAL_RDS_LAMBDA;

  if ( ( CheckXmin - Border ) < GraalWindowXmin )
  {
    GraalWindowXmin = CheckXmin - Border;
    Compute         = 1;
  }

  if ( ( CheckYmin - Border ) < GraalWindowYmin )
  {
    GraalWindowYmin = CheckYmin - Border;
    Compute         = 1;
  }

  if ( ( CheckXmax + Border ) > GraalWindowXmax )
  {
    GraalWindowXmax = CheckXmax + Border;
    Compute         = 1;
  }

  if ( ( CheckYmax + Border ) > GraalWindowYmax )
  {
    GraalWindowYmax = CheckYmax + Border;
    Compute         = 1;
  }

  if ( Compute )
  {
    GraalEraseWindow();
    rdsfreeblock( GraalWindowTable );

    GraalWindowDx = GRAAL_DEFAULT_WINDOW_DX;
    GraalWindowDy = GRAAL_DEFAULT_WINDOW_DY;

    SideX = 1 + ( GraalWindowXmax - GraalWindowXmin ) / GraalWindowDx;
    SideY = 1 + ( GraalWindowYmax - GraalWindowYmin ) / GraalWindowDy;

    if ( SideX < SideY )
    {
      GraalWindowSide = SideY;
    }
    else
    {
      GraalWindowSide = SideX;
    }

    GraalWindowDx = 1 + ( GraalWindowXmax - GraalWindowXmin ) / GraalWindowSide;
    GraalWindowDy = 1 + ( GraalWindowYmax - GraalWindowYmin ) / GraalWindowSide;

    GraalWindowXmax = GraalWindowXmin + ( GraalWindowDx * GraalWindowSide );
    GraalWindowYmax = GraalWindowYmin + ( GraalWindowDy * GraalWindowSide );
    GraalWindowSize = GraalWindowDx * GraalWindowDy;

    GraalWindowTable =

      (graalwin *)rdsallocblock( sizeof(graalwin) * GraalWindowSize );
  }

  rdsend();
}
