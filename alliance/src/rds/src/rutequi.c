/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDS                            |
|                                                             |
| File    :                  rutequi.c                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  13.03.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>

# include <mut.h>
# include "rds.h"
# include "rwi.h"
# include "rut.h"

# include "rutequi.h"
# include "ruterror.h"

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
|                        equirdsfig                           |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *equirdsfig( FigureRds, Window )

  rdsfig_list *FigureRds;
  rdswindow   *Window;
{
  rdsrec_list    *HeadEqui;
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
 
  long            X1;
  long            Y1;
  long            X2;
  long            Y2;

  int             RecIndex;
  int             ScanRecIndex;
  long            WinIndex;

  int             Layer;

  rdsbegin();

  if ( FigureRds->SIZE < RDS_EQUI_SIZE )
  {
    ruterror( RDS_INVALID_FIGURE_SIZE, FigureRds->NAME );
  }

  HeadEqui = (rdsrec_list *)NULL;
  Previous = &HeadEqui;

  StaticRecWin.NEXT = (rdsrecwin_list *)NULL;

  for ( WinIndex = 0; WinIndex < Window->SIZE; WinIndex++ )
  {
    Win = Window->WINTAB + WinIndex;

    if ( Win->LAYERTAB == (rdswinrec_list **)NULL ) continue;

    for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
    {
      for ( WinRec  = Win->LAYERTAB[ Layer ];
            WinRec != (rdswinrec_list *)NULL;
            WinRec  = WinRec->NEXT )
      {
        for ( RecIndex = 0; RecIndex < RWI_MAX_REC; RecIndex++ )
        {
          Rec = WinRec->RECTAB[ RecIndex ];

          if ( ( Rec != (rdsrec_list *)NULL ) &&
               ( ! IsRdsEqui( Rec )         ) )
          {
            SetRdsEqui( Rec );
    
            *Previous = Rec;
            Previous  = &RDS_EQUI( Rec );
    
            EquiRec = Rec;
    
            do
            {
              X1 = EquiRec->X;
              Y1 = EquiRec->Y;
              X2 = EquiRec->DX + X1;
              Y2 = EquiRec->DY + Y1;
    
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
                    ScanRecWin != (rdsrecwin_list *) NULL;
                    ScanRecWin  = ScanRecWin->NEXT )
              {
                ScanWin = ScanRecWin->WINDOW;
    
                for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                      ScanWinRec != (rdswinrec_list *)NULL;
                      ScanWinRec  = ScanWinRec->NEXT )
                {
                  for ( ScanRecIndex = 0; 
                        ScanRecIndex < RWI_MAX_REC;
                        ScanRecIndex++ )
                  {
                    ScanRec = ScanWinRec->RECTAB[ ScanRecIndex ];
  
                    if ( ( ScanRec != (rdsrec_list *)NULL     ) &&
                         ( ! IsRdsEqui( ScanRec )             ) &&
                         ( ( ScanRec->X               ) <= X2 ) &&
                         ( ( ScanRec->Y               ) <= Y2 ) &&
                         ( ( ScanRec->X + ScanRec->DX ) >= X1 ) &&
                         ( ( ScanRec->Y + ScanRec->DY ) >= Y1 ) )
                    {
                      SetRdsEqui( ScanRec );
  
                      *Previous = ScanRec;
                      Previous  = &RDS_EQUI( ScanRec );
                    }
                  }
                }
              }
    
              ScanRec = EquiRec;
              EquiRec = RDS_EQUI( EquiRec );
            }
            while ( EquiRec != (rdsrec_list *)NULL );
    
            SetRdsEndEqui( ScanRec );
          }
        }
      }
    }
  }

  rdsend();
  return( HeadEqui );
}
