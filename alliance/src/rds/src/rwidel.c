/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rwidel.c                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>
# include <math.h>

# include <mut.h>
# include "rds.h"
# include "rtl.h"
# include "rwi.h"

# include "rwidel.h"
# include "rwierror.h"

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
|                     Del Rectangle In Window                 |
|                                                             |
\------------------------------------------------------------*/

void delrdsrecwindow( Rectangle, RdsWindow )

     rdsrec_list *Rectangle;
     rdswindow   *RdsWindow;
{
  rdsrecwin_list  StaticRecWin;
  rdsrecwin_list *ScanRecWin;
  rdsrecwin_list *DelRecWin;
  rdswinrec_list *ScanWinRec;
  rdswinrec_list *FirstWinRec;
  rdswin_list    *ScanWin;

  int             Layer;
  int             Index;
  int             Found;

  Layer = GetRdsLayer( Rectangle );

  if ( IsRdsOneWindow( Rectangle ) )
  {
    StaticRecWin.NEXT   = (rdsrecwin_list *)NULL;
    StaticRecWin.WINDOW = (rdswin_list *)Rectangle->USER;
    ScanRecWin          = &StaticRecWin;
  }
  else
  {
    ScanRecWin = (rdsrecwin_list *)(Rectangle->USER);
  }

  Rectangle->USER = (void *)NULL;
  ClearRdsOneWindow( Rectangle );

  while ( ScanRecWin != (rdsrecwin_list *)NULL )
  {
    DelRecWin  = ScanRecWin;
    ScanRecWin = ScanRecWin->NEXT;

    ScanWin = DelRecWin->WINDOW;
    Found   = RWI_MAX_REC;

    if ( ScanWin->LAYERTAB == (rdswinrec_list **)NULL )
    {
      allocrdswinlayer( ScanWin );
    }

    FirstWinRec = ScanWin->LAYERTAB[ Layer ];

    for ( ScanWinRec  = FirstWinRec;
          ScanWinRec != (rdswinrec_list *)NULL;
          ScanWinRec  = ScanWinRec->NEXT )
    {
      for ( Index = 0; Index < RWI_MAX_REC; Index++ )
      {
        if ( ScanWinRec->RECTAB[ Index ] == Rectangle )
        {
          Found = Index; break;
        }
      }

      if ( Found != RWI_MAX_REC ) break;
    }

    if ( ScanWinRec == FirstWinRec )
    {
      ScanWinRec->RECTAB[ Found ] = (rdsrec_list *)NULL;
    }
    else
    {
      for ( Index = 0; Index < RWI_MAX_REC; Index++ )
      {
        if ( FirstWinRec->RECTAB[ Index ] != (rdsrec_list *)NULL ) break;
      }

      ScanWinRec->RECTAB[ Found ]  = FirstWinRec->RECTAB[ Index ];
      FirstWinRec->RECTAB[ Index ] = (rdsrec_list *)NULL;
    }

    for ( Index = 0; Index < RWI_MAX_REC; Index++ )
    {
      if ( FirstWinRec->RECTAB[ Index ] != (rdsrec_list *)NULL ) break;
    }

    if ( Index == RWI_MAX_REC )
    {
      ScanWin->LAYERTAB[ Layer ] = FirstWinRec->NEXT;

      freerdswinrec( FirstWinRec );
    }

    if ( DelRecWin != &StaticRecWin )
    {
      freerdsrecwin( DelRecWin );
    }
  }
}

