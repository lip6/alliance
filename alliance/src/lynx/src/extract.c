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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                 extract.c                         |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"

# include "mbkrds.h"
# include "extract.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        LynxBuildEqui                        |
|                                                             |
\------------------------------------------------------------*/

long LynxBuildEqui( HeadEqui, FigureRds, Window )

  rdsrec_list **HeadEqui;
  rdsfig_list  *FigureRds;
  rdswindow    *Window;
{
  rdswin_list    *Win;
  rdswin_list    *ScanWin;
  rdswinrec_list *WinRec;
  rdswinrec_list *ScanWinRec;
  rdsrecwin_list *RecWin;
  rdsrecwin_list *ScanRecWin;
  rdsrecwin_list  StaticRecWin;

  rdsrec_list   **Previous;
  rdsrec_list    *Rec;
  rdsrec_list    *EquiRec;
  rdsrec_list    *ScanRec;
  rdsrec_list    *ScanInsRec;
  rdsins_list    *ScanIns;
 
  long            X1;
  long            Y1;
  long            X2;
  long            Y2;

  int             RecIndex;
  int             ScanRecIndex;
  int             Index;
  long            WinIndex;

  int             Layer;
  int             WinLayer;
  int             RecLayer;
  int             InsLayer;

  long            Counter;

  rdsbegin();

  Counter = 0;

  Previous = HeadEqui;

  StaticRecWin.NEXT = (rdsrecwin_list *)0;

  for ( WinIndex = 0; WinIndex < Window->SIZE; WinIndex++ )
  {
    Win = Window->WINTAB + WinIndex;

    if ( Win->LAYERTAB == (rdswinrec_list **)0 ) continue;

    for ( WinLayer = 0; WinLayer < RWI_MAX_LAYER; WinLayer++ )
    {
      Layer = GET_LYNX_GRAPH_LAYER( WinLayer, 0 );

      if ( Layer == RDS_LYNX_GRAPH_EMPTY ) continue;

      for ( WinRec  = Win->LAYERTAB[ WinLayer ];
            WinRec != (rdswinrec_list *)0;
            WinRec  = WinRec->NEXT )
      {
        for ( RecIndex = 0; RecIndex < RWI_MAX_REC; RecIndex++ )
        {
          Rec = WinRec->RECTAB[ RecIndex ];

          if ( ( Rec != (rdsrec_list *)0 ) &&
               ( ! IsLynxDeleted( Rec )     ) &&
               ( ! IsLynxEqui( Rec )        ) )
          {
            if ( IsRdsFigRec( Rec ) )
            {
              SetLynxEqui( Rec );
    
              *Previous = Rec;
              Previous  = &LYNX_EQUI( Rec );
            }
            else
            if ( ( IsRdsConInter( Rec ) ) ||
                 ( IsRdsRefCon( Rec   ) ) )
            {
              SetLynxEqui( Rec );
    
              *Previous = Rec;
              Previous  = &LYNX_EQUI( Rec );
    
              ScanIns = (rdsins_list *)LYNX_LINK( Rec );
    
              for ( InsLayer = 0 ;
                    InsLayer < RWI_MAX_LAYER ;
                    InsLayer++ )
              {
                for ( ScanInsRec  = ScanIns->LAYERTAB[ InsLayer ];
                      ScanInsRec != (rdsrec_list *)0;
                      ScanInsRec  = ScanInsRec->NEXT )
                {
                  if ( ( ( IsRdsConInter( ScanInsRec ) ) ||
                       ( IsRdsRefCon( ScanInsRec )   )   ) &&
                       ( ! IsLynxEqui( ScanInsRec )      ) &&
                       ( Rec->NAME == ScanInsRec->NAME ) )
                  {
                    SetLynxEqui( ScanInsRec );
    
                    *Previous = ScanInsRec;
                    Previous  = &LYNX_EQUI( ScanInsRec );
                  }
                }
              }
            }
    
            EquiRec = Rec;
    
            do
            {
              X1 = EquiRec->X;
              Y1 = EquiRec->Y;
              X2 = EquiRec->DX + X1;
              Y2 = EquiRec->DY + Y1;
    
              RecLayer = GetRdsLayer ( EquiRec );
    
              if ( IsRdsOneWindow( EquiRec ) )
              {
                StaticRecWin.WINDOW = (rdswin_list *)EquiRec->USER;
                RecWin              = &StaticRecWin;
              }
              else
              {
                RecWin = (rdsrecwin_list *)EquiRec->USER;
              }
    
              for ( ScanRecWin  = RecWin;
                    ScanRecWin != (rdsrecwin_list *) 0;
                    ScanRecWin  = ScanRecWin->NEXT )
              {
                ScanWin = ScanRecWin->WINDOW;
    
                Index = 0;
                Layer = GET_LYNX_GRAPH_LAYER( RecLayer, Index );
    
                while ( Layer != RDS_LYNX_GRAPH_EMPTY )
                {
                  for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                        ScanWinRec != (rdswinrec_list *)0;
                        ScanWinRec  = ScanWinRec->NEXT )
                  {
                    for ( ScanRecIndex = 0; 
                          ScanRecIndex < RWI_MAX_REC;
                          ScanRecIndex++ )
                    {
                      ScanRec = ScanWinRec->RECTAB[ ScanRecIndex ];
    
                      if ( ( ScanRec != (rdsrec_list *)0     ) &&
                           ( ! IsLynxEqui( ScanRec )            ) &&
                           ( ! IsLynxDeleted( ScanRec )         ) &&
                           ( ( ScanRec->X               ) <= X2 ) &&
                           ( ( ScanRec->Y               ) <= Y2 ) &&
                           ( ( ScanRec->X + ScanRec->DX ) >= X1 ) &&
                           ( ( ScanRec->Y + ScanRec->DY ) >= Y1 ) )
                      {
                        SetLynxEqui( ScanRec );
    
                        *Previous = ScanRec;
                        Previous  = &LYNX_EQUI( ScanRec );
    
                        if ( ( ! IsRdsFigRec( ScanRec ) ) &&
                             ( ( IsRdsConInter( ScanRec ) ) ||
                               ( IsRdsRefCon( ScanRec   ) ) ) )
                        {
                          ScanIns = (rdsins_list *)LYNX_LINK( ScanRec );
    
                          for ( InsLayer = 0 ; 
                                InsLayer < RWI_MAX_LAYER ; 
                                InsLayer++ )
                          {
                            for ( ScanInsRec  = ScanIns->LAYERTAB[ InsLayer ];
                                  ScanInsRec != (rdsrec_list *)0;
                                  ScanInsRec  = ScanInsRec->NEXT )
                            {
                              if ( ( ( IsRdsConInter( ScanInsRec ) ) ||  
                                     ( IsRdsRefCon( ScanInsRec )   )   ) &&
                                     ( ! IsLynxEqui( ScanInsRec )      ) &&
                                     ( ScanRec->NAME == ScanInsRec->NAME ) )
                              {
                                SetLynxEqui( ScanInsRec );
    
                                *Previous = ScanInsRec;
                                Previous  = &LYNX_EQUI( ScanInsRec );
                              }
                            }
                          }
                        }
                      }
                    }
                  }
    
                  Index = Index + 1;
                  Layer = GET_LYNX_GRAPH_LAYER( RecLayer, Index );
                }
              }
    
              ScanRec = EquiRec;
              EquiRec = LYNX_EQUI( EquiRec );
            }
            while ( EquiRec != (rdsrec_list *)0 );
    
            SetLynxEndEqui( ScanRec );
    
            Counter = Counter + 1;
          }
        }
      }
    }
  }

  rdsend();
  return( Counter );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxDumpAllEqui                        |
|                                                             |
\------------------------------------------------------------*/

void LynxDumpAllEqui( FigureRds, HeadEqui, Figure, Real )

  rdsfig_list *FigureRds;
  rdsrec_list *HeadEqui;
  void        *Figure;
  char         Real;
{
  rdsrec_list *ScanEqui;

  rdsbegin();

  Lynxrecrdsmbk( Figure, FigureRds->LAYERTAB[ RDS_ABOX ], Real );

  for ( ScanEqui  = HeadEqui;
        ScanEqui != (rdsrec_list *)0;
        ScanEqui  = LYNX_EQUI( ScanEqui ) )
  {
    Lynxrecrdsmbk( Figure, ScanEqui, Real );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        LynxDumpEqui                         |
|                                                             |
\------------------------------------------------------------*/

void LynxDumpEqui( Figure, FirstEqui, Real )

  void        *Figure;
  rdsrec_list *FirstEqui;
  char         Real;
{
  rdsbegin();

  while ( FirstEqui != (rdsrec_list *)0 )
  {
    Lynxrecrdsmbk( Figure, FirstEqui, Real );

    if ( IsLynxEndEqui( FirstEqui ) ) break;

    FirstEqui = LYNX_EQUI( FirstEqui );
  }

  rdsend();
}
