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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  trans.c                          |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.07.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"

# include "mbkrds.h"
# include "interval.h"
# include "hole.h"
# include "diffusion.h"
# include "pattern.h"
# include "error.h"

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

  static lynxtrans_list *LYNX_HEAD_TRANSISTOR = (lynxtrans_list *)0;
  static char            LYNX_UNIFY_LAYER_TABLE[ RDS_ALL_LAYER ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      LynxPurgeFigure                        |
|                                                             |
\------------------------------------------------------------*/

static void LynxPurgeFigure( FigurePurge )

  rdsfig_list *FigurePurge;
{
  rdsrec_list *ScanRec;
  rdsrec_list *NextRec;
  int          Layer;

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    ScanRec = FigurePurge->LAYERTAB[ Layer ];
    FigurePurge->LAYERTAB[ Layer ] = (rdsrec_list *)0;

    while ( ScanRec != (rdsrec_list *)NULL )
    {
      NextRec = ScanRec->NEXT;

      freerdsrec( ScanRec, FigurePurge->SIZE );

      ScanRec = NextRec;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxUnifyFigure                        |
|                                                             |
\------------------------------------------------------------*/

static void LynxUnifyFigure( FigureUnify )

  rdsfig_list *FigureUnify;
{
  rdswindow   *WindowRds;
  rdsrec_list *HeadEqui;
  rdsrec_list *ScanEqui;

  WindowRds = buildrdswindow( FigureUnify );
  HeadEqui  = equirdsfig( FigureUnify, WindowRds );
  destroyrdswindow( FigureUnify, WindowRds );
  unifyrdsfig( FigureUnify, HeadEqui );

  while ( HeadEqui != (rdsrec_list *)0 )
  {
    ClearRdsEqui( HeadEqui );
    ClearRdsEndEqui( HeadEqui );

    ScanEqui = HeadEqui;
    HeadEqui = RDS_EQUI( HeadEqui );
    RDS_EQUI( ScanEqui ) = (rdsrec_list *)0;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       LynxInitializeLayer                   |
|                                                             |
\------------------------------------------------------------*/

static void LynxInitializeLayer()
{
  int RdsLayer;
  int MbkLayer;
  int GateLayer;
  int DiffLayer;
  long Index;

  for ( RdsLayer = 0; RdsLayer < RDS_MAX_LAYER; RdsLayer++ )
  {
    LYNX_UNIFY_LAYER_TABLE[ RdsLayer ] = 0;
  }

  for ( MbkLayer = 0; MbkLayer < MBK_MAX_LAYER; MbkLayer++ )
  {
    GateLayer   = GET_LYNX_TRANSISTOR_GATE_LAYER( MbkLayer );
    DiffLayer   = GET_LYNX_TRANSISTOR_DIFF_LAYER( MbkLayer );

    if ( ( GateLayer == RDS_LYNX_TRANSISTOR_EMPTY ) ||
         ( DiffLayer == RDS_LYNX_TRANSISTOR_EMPTY ) ) continue;

    Index    = 0;
    RdsLayer = GET_SEGMENT_LAYER( MbkLayer, Index );

    while ( RdsLayer != RDS_SEGMENT_EMPTY )
    {
      LYNX_UNIFY_LAYER_TABLE[ RdsLayer ] = 1;

      Index    = Index + 1;
      RdsLayer = GET_SEGMENT_LAYER( MbkLayer, Index );
    }
  }

  for ( RdsLayer = 0; RdsLayer < RDS_MAX_LAYER; RdsLayer++ )
  {
    Index     = 0;
    DiffLayer = GET_LYNX_DIFFUSION_LAYER( RdsLayer, Index );

    while ( DiffLayer != RDS_LYNX_DIFFUSION_EMPTY )
    {
      LYNX_UNIFY_LAYER_TABLE[ RdsLayer  ] = 1;
      LYNX_UNIFY_LAYER_TABLE[ DiffLayer ] = 1;

      Index     = Index + 1;
      DiffLayer = GET_LYNX_DIFFUSION_LAYER( RdsLayer, Index );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxInitializePattern                  |
|                                                             |
\------------------------------------------------------------*/

static void LynxInitializePattern()
{
  rdsfig_list    *FigurePattern;
  rdsrec_list    *FirstRec;
  rdsrec_list    *ScanRec;
  rdsrec_list    *OrientRec;
  rdsrec_list    *PrevRec;
  lynxtrans_list *Transistor;
  lynxpattern    *Pattern;
  phseg_list      Segment;
  char            BadLength;
  int             MbkLayer;
  int             GateLayer;
  int             DiffLayer;
  int             ActivLayer;
  int             RdsLayer;
  int             Orient;
  char            CheckOrient;
  long            Length;
  long            Width;
  long            GateX;
  long            GateY;

  FigurePattern = addrdsfig( "_pattern_", LYNX_MODE );

  Segment.NEXT = (phseg_list *)0;
  Segment.NAME = (char       *)0;
  Segment.X1   = 0;
  Segment.Y1   = 0;

  LYNX_HEAD_TRANSISTOR = (lynxtrans_list *)0;

  for ( MbkLayer = 0; MbkLayer < MBK_MAX_LAYER; MbkLayer++ )
  {
    GateLayer   = GET_LYNX_TRANSISTOR_GATE_LAYER( MbkLayer );
    DiffLayer   = GET_LYNX_TRANSISTOR_DIFF_LAYER( MbkLayer );
    ActivLayer  = GET_LYNX_TRANSISTOR_ACTIV_LAYER( MbkLayer );

    if ( ( GateLayer == RDS_LYNX_TRANSISTOR_EMPTY ) || 
         ( DiffLayer == RDS_LYNX_TRANSISTOR_EMPTY ) ) continue;

    Transistor = (lynxtrans_list *)rdsallocblock( sizeof( lynxtrans_list ) );

    Transistor->NEXT     = LYNX_HEAD_TRANSISTOR;
    LYNX_HEAD_TRANSISTOR = Transistor;

    Transistor->GATE_LAYER    = GateLayer;
    Transistor->DIFF_LAYER    = DiffLayer;
    Transistor->ACTIV_LAYER   = ActivLayer;
    Transistor->MBK_LAYER     = MbkLayer;
    Transistor->ORIENT        = ( LYNX_MAX_ORIENT >> 1 );
/*
** Compute transistor minimum symbolic length 
*/
    Segment.LAYER = MbkLayer;
    Segment.WIDTH = SCALE_X;
    Segment.X2    = 0;
    Segment.TYPE  = UP;

    for ( Length = 1; Length < LYNX_MAX_LENGTH; Length++ )
    {
      Segment.Y2 = Length * SCALE_X;
      ScanRec    = FirstRec = segmbkrds( FigurePattern, &Segment, 0 );
      BadLength  = 0;

      do
      {
        if ( ( ScanRec->DX <= 0 ) ||
             ( ScanRec->DY <= 0 ) ) 
        {
          BadLength = 1; break;
        }

        ScanRec = (rdsrec_list *)ScanRec->USER;
      }
      while ( ScanRec != FirstRec );

      if ( ! BadLength )  break;
    }

    if ( BadLength )
    {
      LynxError( LYNX_ERROR_MINIMUM_LENGTH, 0, 0 );
    }

    LynxPurgeFigure( FigurePattern );

    Pattern     = Transistor->PATTERN;
    CheckOrient = 1;

    for ( Width = 0; Width < LYNX_MAX_WIDTH; Width++ )
    {
      Segment.WIDTH = ( Width + 1 ) * SCALE_X;

      for ( Orient = 0; Orient < LYNX_MAX_ORIENT; Orient++ )
      {
        if ( ( Transistor->ORIENT == 1  ) &&
             ( ( Orient == LYNX_SOUTH ) ||
               ( Orient == LYNX_WEST  ) ) ) continue;

        switch ( Orient )
        {
          case LYNX_NORTH : Segment.X2   = 0;
                            Segment.Y2   = Length * SCALE_X;
                            Segment.TYPE = UP;
          break;

          case LYNX_SOUTH : Segment.X2   = 0;
                            Segment.Y2   = Length * SCALE_X;
                            Segment.TYPE = DOWN;
          break;

          case LYNX_EAST :  Segment.X2   = Length * SCALE_X;
                            Segment.Y2   = 0;
                            Segment.TYPE = RIGHT;
          break;

          case LYNX_WEST :  Segment.X2   = Length * SCALE_X;
                            Segment.Y2   = 0;
                            Segment.TYPE = LEFT;
          break;
        }

        ScanRec = segmbkrds( FigurePattern, &Segment, 0 );
         
        LynxUnifyFigure( FigurePattern );

        FirstRec = FigurePattern->LAYERTAB[ GateLayer ];

        if ( ( FirstRec       == (rdsrec_list *)0 ) ||
             ( FirstRec->NEXT != (rdsrec_list *)0 ) )
        {
          LynxError( LYNX_ERROR_TRANSISTOR_GATE, 0, 0 );
        }

        GateX = FirstRec->X;
        GateY = FirstRec->Y;

        if ( Orient == LYNX_NORTH )
        {
          Pattern[ Width ].WIDTH = FirstRec->DX;
          Transistor->LENGTH     = FirstRec->DY;
        }

        FirstRec->X = 0;
        FirstRec->Y = 0;

        PrevRec = FirstRec;

        for ( RdsLayer = 0; RdsLayer < RDS_MAX_LAYER; RdsLayer++ )
        {
          for ( ScanRec  = FigurePattern->LAYERTAB[ RdsLayer ];
                ScanRec != (rdsrec_list *)0;
                ScanRec  = ScanRec->NEXT )
          {
            if ( ScanRec != FirstRec )
            {
              LYNX_LINK( PrevRec ) = ScanRec;
              PrevRec              = ScanRec;

              ScanRec->X -= GateX;
              ScanRec->Y -= GateY;
            }
          }

          FigurePattern->LAYERTAB[ RdsLayer ] = (rdsrec_list *)0;
        }

        LYNX_LINK( PrevRec ) = FirstRec;
        Pattern[ Width ].REAL[ Orient ] = FirstRec;

        FirstRec = segmbkrds( FigurePattern, &Segment, 1 );
        ScanRec  = FirstRec;

        do
        {
          LYNX_LINK( ScanRec ) = (rdsrec_list *)ScanRec->USER;

          ScanRec->X -= GateX;
          ScanRec->Y -= GateY;

          ScanRec = (rdsrec_list *)ScanRec->USER;
        }
        while ( ScanRec != FirstRec );

        for ( RdsLayer = 0; RdsLayer < RDS_MAX_LAYER; RdsLayer++ )
        {
          FigurePattern->LAYERTAB[ RdsLayer ] = (rdsrec_list *)0;
        }

        Pattern[ Width ].SYMB[ Orient ] = FirstRec;

        if ( ( CheckOrient          ) &&
             ( Orient == LYNX_SOUTH ) )
        {
          CheckOrient = 0;

          FirstRec  = Pattern[ Width ].REAL[ LYNX_NORTH ];
          ScanRec   = FirstRec;
          OrientRec = Pattern[ Width ].REAL[ LYNX_SOUTH ];

          do
          {
            if ( ( ScanRec->X     != OrientRec->X     ) ||
                 ( ScanRec->Y     != OrientRec->Y     ) ||
                 ( ScanRec->DX    != OrientRec->DX    ) ||
                 ( ScanRec->DY    != OrientRec->DY    ) ||
                 ( ScanRec->FLAGS != OrientRec->FLAGS ) )
            {
              break;
            }

            ScanRec   = LYNX_LINK( ScanRec   );
            OrientRec = LYNX_LINK( OrientRec );
          }
          while ( ScanRec != FirstRec );

          if ( ScanRec == FirstRec )
          {
            Transistor->ORIENT = 1;
          }
        }
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxPatternMatching                    |
|                                                             |
\------------------------------------------------------------*/

long LynxPatternMatching( FigureRds )

   rdsfig_list *FigureRds;
{
  lynxtrans_list  *Transistor;
  lynxpattern     *Pattern;
  lynxinter_list  *NewInter;
  lynxinter_list  *ScanInter;
  lynxinter_list  *DelInter;
  lynxinter_list **PrevInter;
  lynxinter_list  *Interval;
  rdsrec_list     *GateRec;
  rdsrec_list     *ScanRec;
  rdsrec_list     *FirstRec;
  rdsrec_list     *InterRec;
  rdsrec_list     *NewRec;
  rdsrec_list     *FirstNewRec;
  rdsrec_list     *PrevNewRec;
  rdswindow       *WindowRds;
  rdswin_list     *WinList;
  rdswin_list     *ScanWin;
  rdswinrec_list  *ScanWinRec;
  int              ScanRecIndex;
  long             X1;
  long             X2;
  long             Y1;
  long             Y2;
  long             FirstX;
  long             FirstY;
  long             WX1;
  long             WY1;
  long             WX2;
  long             WY2;
  long             Min;
  long             Max;
  long             Width;
  long             DeltaGate;
  long             DeltaDn;
  long             DeltaUp;
  long             GateWidth;
  long             GateLength;
  long             GateLow;
  long             GateHigh;
  int              GateOrient;
  long             GateMin;
  long             GateMax;
  char             Orient;
  char             Vertical;
  char             GateVertical;
  int              GateLayer;
  int              DiffLayer;
  int              ActivLayer;
  int              MbkLayer;
  int              Layer;
  long             Number;

  WindowRds = buildrdswindow( FigureRds );
  Number    = 0;

  LynxDiffusionFigure( FigureRds, WindowRds );

/*
** For all kind of transistor
*/
  for ( Transistor  = LYNX_HEAD_TRANSISTOR;
        Transistor != (lynxtrans_list *)0;
        Transistor  = Transistor->NEXT )
  {
    Pattern     = Transistor->PATTERN;
    GateLayer   = Transistor->GATE_LAYER;
    MbkLayer    = Transistor->MBK_LAYER;
    DiffLayer   = Transistor->DIFF_LAYER;
    ActivLayer  = Transistor->ACTIV_LAYER;
/*
** For all possible gate
*/
    for ( GateRec  = FigureRds->LAYERTAB[ GateLayer ];
          GateRec != (rdsrec_list *)0;
          GateRec  = GateRec->NEXT )
    {
/*
** If Gate not in window continue
*/
      if ( GateRec->USER == (void *)0 ) continue;

      GateVertical = ( GateRec->DX < GateRec->DY );
      Vertical     = 0;
      Interval     = (lynxinter_list *)0;

      while ( ( Vertical < 2                    ) &&
              ( Interval == (lynxinter_list *)0 ) )
      {
        if ( GateVertical )
        {
          GateOrient   = LYNX_NORTH;
          GateWidth    = GateRec->DX;
          GateLength   = GateRec->DY;
          GateMin      = GateRec->Y;
          GateMax      = GateRec->DY + GateMin;
          GateLow      = GateRec->X;
          GateHigh     = GateRec->DX + GateLow;
        }
        else
        {
          GateOrient   = LYNX_EAST;
          GateWidth    = GateRec->DY;
          GateLength   = GateRec->DX;
          GateMin      = GateRec->X;
          GateMax      = GateRec->DX + GateMin;
          GateLow      = GateRec->Y;
          GateHigh     = GateRec->DY + GateLow;
        }
  
        DeltaGate = ( GateLength - Transistor->LENGTH );
  
        if ( DeltaGate < 0 ) break;
/*
** Search minimum width
*/
        for ( Width = 0; Width < LYNX_MAX_WIDTH; Width++ )
        {
          if ( Pattern[ Width ].WIDTH > GateWidth ) break;
        }

        Width = Width - 1;

        if ( Width < 0 ) break;
/*
** For all possible orient
*/
        for ( Orient = 0; Orient < Transistor->ORIENT; Orient++ )
        {
          GateOrient = GateOrient + Orient;
          Interval   = LynxUnionInterval( (lynxinter_list *)0, GateMin, GateMax );
  
          FirstRec = Pattern[ Width ].REAL[ GateOrient ];
          ScanRec  = LYNX_LINK( FirstRec );
  
          do
          {
            Layer = GetRdsLayer( ScanRec );
  
            X1 = ScanRec->X + GateRec->X;
            Y1 = ScanRec->Y + GateRec->Y;
  
            if ( GateVertical )
            {
              DeltaDn = ScanRec->Y;
              DeltaUp = FirstRec->DY - ( ScanRec->Y + ScanRec->DY );
  
              X2 = ScanRec->DX + X1;
              Y2 = ScanRec->DY + Y1 + DeltaGate;
            }
            else
            {
              DeltaDn = ScanRec->X;
              DeltaUp = FirstRec->DX - ( ScanRec->X + ScanRec->DX );
  
              X2 = ScanRec->DX + X1 + DeltaGate;
              Y2 = ScanRec->DY + Y1;
            }
/*
** Compute possible matching for each pattern's rectangle.
*/
            NewInter = (lynxinter_list *)0;
  
            WY1 = ( Y1 - 1 - WindowRds->YMIN ) / WindowRds->SIDE;
            WY2 = ( Y2     - WindowRds->YMIN ) / WindowRds->SIDE;
            WX1 = ( X1 - 1 - WindowRds->XMIN ) / WindowRds->SIDE;
            WX2 = ( X2     - WindowRds->XMIN ) / WindowRds->SIDE;
  
            if ( WX1 < 0              ) WX1 = 0;
            if ( WX2 < 0              ) WX2 = 0;
            if ( WX1 >= WindowRds->DX ) WX1 = WindowRds->DX - 1;
            if ( WX2 >= WindowRds->DX ) WX2 = WindowRds->DX - 1;
  
            if ( WY1 < 0              ) WY1 = 0;
            if ( WY2 < 0              ) WY2 = 0;
            if ( WY1 >= WindowRds->DY ) WY1 = WindowRds->DY - 1;
            if ( WY2 >= WindowRds->DY ) WY2 = WindowRds->DY - 1;
  
            FirstX = WX1;
            FirstY = WY1;
  
            while ( WY1 <= WY2 )
            {
              WinList = WindowRds->WINTAB + ( WY1 * WindowRds->DX );
    
              WX1 = FirstX;
    
              while ( WX1 <= WX2 )
              {
                ScanWin = WinList + WX1;
    
                if ( ScanWin->LAYERTAB != (rdswinrec_list **)NULL )
                {
                  for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                        ScanWinRec != (rdswinrec_list *)NULL;
                        ScanWinRec  = ScanWinRec->NEXT )
                  {
                    for ( ScanRecIndex = 0;
                          ScanRecIndex < RWI_MAX_REC;
                          ScanRecIndex++ )
                    {
                      InterRec = ScanWinRec->RECTAB[ ScanRecIndex ];
    
                      if ( InterRec != (rdsrec_list *)0 )
                      {
                        if ( GateVertical )
                        {
                          if ( ( ( InterRec->X                ) <= X1 ) &&
                               ( ( InterRec->X + InterRec->DX ) >= X2 ) )
                          {
                            Min = InterRec->Y;
                            Max = InterRec->Y + InterRec->DY;
    
                            NewInter = LynxUnionInterval( NewInter, Min, Max );
                          }
                        }
                        else
                        {
                          if ( ( ( InterRec->Y                ) <= Y1 ) &&
                               ( ( InterRec->Y + InterRec->DY ) >= Y2 ) )
                          {
                            Min = InterRec->X;
                            Max = InterRec->X + InterRec->DX;
    
                            NewInter = LynxUnionInterval( NewInter, Min, Max );
                          }
                        }
                      }
                    }
                  }
                }
    
                WX1 = WX1 + 1;
              }
    
              WY1 = WY1 + 1;
            }
/*
** No gate rectangle under activ rectangle
*/
            if ( Layer == ActivLayer )
            {
              WY1 = FirstY;

              while ( WY1 <= WY2 )
              {
                WinList = WindowRds->WINTAB + ( WY1 * WindowRds->DX );
    
                WX1 = FirstX;

                while ( WX1 <= WX2 )
                {
                  ScanWin = WinList + WX1;
    
                  if ( ScanWin->LAYERTAB != (rdswinrec_list **)NULL )
                  {
                    for ( ScanWinRec  = ScanWin->LAYERTAB[ GateLayer ];
                          ScanWinRec != (rdswinrec_list *)NULL;
                          ScanWinRec  = ScanWinRec->NEXT )
                    {
                      for ( ScanRecIndex = 0;
                            ScanRecIndex < RWI_MAX_REC;
                            ScanRecIndex++ )
                      {
                        InterRec = ScanWinRec->RECTAB[ ScanRecIndex ];

                        if ( InterRec != (rdsrec_list *)0 )
                        {
                          if ( GateVertical )
                          {
                            if ( ( ( ( InterRec->X                ) < GateLow  ) &&
                                   ( ( InterRec->X + InterRec->DX ) > X1       ) ) ||
                                 ( ( ( InterRec->X                ) < X2       ) &&
                                   ( ( InterRec->X + InterRec->DX ) > GateHigh ) ) )
                            {
                              Min = InterRec->Y;
                              Max = InterRec->Y + InterRec->DY;

                              NewInter = LynxSplitInterval( NewInter, Min, Max );
                            }
                          }
                          else
                          {
                            if ( ( ( ( InterRec->Y                ) < GateLow  ) &&
                                   ( ( InterRec->Y + InterRec->DY ) > Y1       ) ) ||
                                 ( ( ( InterRec->Y                ) < Y2       ) &&
                                   ( ( InterRec->Y + InterRec->DY ) > GateHigh ) ) )
                            {
                              Min = InterRec->X;
                              Max = InterRec->X + InterRec->DX;

                              NewInter = LynxSplitInterval( NewInter, Min, Max );
                            }
                          }
                        }

                        if ( NewInter == (lynxinter_list *)0 ) break;
                      }

                      if ( NewInter == (lynxinter_list *)0 ) break;
                    }
                  }
    
                  if ( NewInter == (lynxinter_list *)0 ) break;

                  WX1 = WX1 + 1;
                }

                if ( NewInter == (lynxinter_list *)0 ) break;
    
                WY1 = WY1 + 1;
              }
            }
  
            for ( ScanInter  = NewInter;
                  ScanInter != (lynxinter_list *)0;
                  ScanInter  = ScanInter->NEXT )
            {
              ScanInter->MIN -= DeltaDn;
              ScanInter->MAX += DeltaUp;
            }
  
            ScanInter = Interval;
            Interval  = LynxIntersectInterval( ScanInter, NewInter );
  
            LynxDelAllInterval( NewInter  );
            LynxDelAllInterval( ScanInter );
  
            ScanRec = LYNX_LINK( ScanRec );
          }
          while ( ( ScanRec  != FirstRec            ) &&
                  ( Interval != (lynxinter_list *)0 ) );
  
          if ( Interval != (lynxinter_list *)0 ) break;
        }
/*
** For all interval
*/
        PrevInter = &Interval;
        ScanInter =  Interval;

        while ( ScanInter != (lynxinter_list *)0 )
        {
          DeltaGate = ( ScanInter->MAX - ScanInter->MIN - Transistor->LENGTH );
 
          if ( DeltaGate < 0 )
          {
             *PrevInter = ScanInter->NEXT;
             DelInter   = ScanInter;
             ScanInter  = ScanInter->NEXT;

             DelInter->NEXT = (lynxinter_list *)0;
             LynxDelAllInterval( DelInter );

             continue;
          }

          if ( GateVertical )
          {
            X1 = GateRec->X;
            Y1 = ScanInter->MIN;
            X2 = GateRec->DX + X1;
            Y2 = ScanInter->MAX - ScanInter->MIN + Y1;
          }
          else
          {
            X1 = ScanInter->MIN;
            Y1 = GateRec->Y;
            X2 = ScanInter->MAX - ScanInter->MIN + X1;
            Y2 = GateRec->DY + Y1;
          }
/*
** Make hole diffusion under gate
*/
          if ( DiffLayer != RDS_LYNX_TRANSISTOR_EMPTY )
          {
            LynxHoleFigure( FigureRds, WindowRds, X1, Y1, X2, Y2, DiffLayer );
          }
/*
** Build symbolic transistor
*/
          FirstRec = Pattern[ Width ].SYMB[ GateOrient ];
          ScanRec  = FirstRec;
  
          FirstNewRec = (rdsrec_list *)0;
          PrevNewRec  = (rdsrec_list *)0;
  
          do
          {
            Layer = GetRdsLayer( ScanRec );
  
            if ( GateVertical )
            {
              X1 = ScanRec->X + GateRec->X;
              Y1 = ScanRec->Y + ScanInter->MIN;
              X2 = ScanRec->DX + X1;
              Y2 = ScanRec->DY + Y1 + DeltaGate;
            }
            else
            {
              X1 = ScanRec->X + ScanInter->MIN;
              Y1 = ScanRec->Y + GateRec->Y;
              X2 = ScanRec->DX + X1 + DeltaGate;
              Y2 = ScanRec->DY + Y1;
            }
  
            NewRec = addrdsfigrec( FigureRds, (char *)0, Layer,
                                   X1, Y1, X2 - X1, Y2 - Y1 );
  
            if ( FirstNewRec == (rdsrec_list *)0 )
            {
              FirstNewRec = NewRec;
              PrevNewRec  = NewRec;
  
              SetLynxFirstLink( FirstNewRec );
            }
            else
            {
              LYNX_LINK( PrevNewRec ) = NewRec;
              PrevNewRec              = NewRec;
            }
  
            SetLynxAttribute( NewRec, MbkLayer );
            SetLynxOrient( NewRec, GateOrient );
  
            SetRdsSegment( NewRec );
              
            if ( Layer == GateLayer )
            {
              SetRdsTransGate( NewRec );
            }

            ScanRec = LYNX_LINK( ScanRec );
          }
          while ( ScanRec != FirstRec );
  
          LYNX_LINK( PrevNewRec ) = FirstNewRec;

          Number++;

          PrevInter = &ScanInter->NEXT;
          ScanInter = ScanInter->NEXT;
        }
  
        GateVertical = ! GateVertical;
        Vertical     =   Vertical + 1;
      }

      if ( Interval != (lynxinter_list *)0 )
      {
        LynxDelAllInterval( Interval );
      }
    }
  }

  destroyrdswindow( FigureRds, WindowRds );

  return( Number );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxReconizeTransistor                 |
|                                                             |
\------------------------------------------------------------*/

long LynxReconizeTransistor( FigureRds )

  rdsfig_list *FigureRds;
{
  rdsfig_list *FigureUnify;
  int          RdsLayer;
  long         Number;

  LynxInitializeLayer();
  LynxInitializePattern();

  FigureUnify = addrdsfig( "_unify_", LYNX_MODE );

  for ( RdsLayer = 0; RdsLayer < RDS_MAX_LAYER; RdsLayer++ )
  {
    if ( LYNX_UNIFY_LAYER_TABLE[ RdsLayer ] )
    {
      FigureUnify->LAYERTAB[ RdsLayer ] = FigureRds->LAYERTAB[ RdsLayer ];
      FigureRds->LAYERTAB  [ RdsLayer ] = (rdsrec_list *)0;
    }
  }

  LynxUnifyFigure( FigureUnify );

  Number = LynxPatternMatching( FigureUnify );

  for ( RdsLayer = 0; RdsLayer < RDS_MAX_LAYER; RdsLayer++ )
  {
    if ( LYNX_UNIFY_LAYER_TABLE[ RdsLayer ] )
    {
      FigureRds->LAYERTAB[ RdsLayer ] = FigureUnify->LAYERTAB[ RdsLayer ];
      FigureUnify->LAYERTAB  [ RdsLayer ] = (rdsrec_list *)0;
    }
  }

  delrdsfig( FigureUnify->NAME );

  return( Number );
}
