/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rwidestroy.c                     |
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

# include "rwidestroy.h"
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
|                      Destroy Rds Window                     |
|                                                             |
\------------------------------------------------------------*/

void destroyrdswindow( Figure, RdsWindow )

   rdsfig_list *Figure;
   rdswindow   *RdsWindow;
{
  rdsins_list    *Instance;
  rdsrec_list    *Rectangle;
  rdswin_list    *WinList;
  rdsrecwin_list *RecWin;
  rdsrecwin_list *DelRecWin;
  rdswinrec_list *WinRec;
  rdswinrec_list *DelWinRec;
  long            Counter;
  int             Layer;

  for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = Figure->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)NULL;
          Rectangle  = Rectangle->NEXT )
    {
      if ( ! IsRdsOneWindow( Rectangle ) )
      {
        RecWin = (rdsrecwin_list *)Rectangle->USER;
 
        while ( RecWin != (rdsrecwin_list *)NULL )
        {
          DelRecWin = RecWin;
          RecWin    = RecWin->NEXT;

          freerdsrecwin( DelRecWin );
        }
      }

      Rectangle->USER = (void *)NULL;

      ClearRdsOneWindow( Rectangle );
    }
  }

  for ( Instance  = Figure->INSTANCE; 
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
    {
      for ( Rectangle  = Instance->LAYERTAB[ Layer ];
            Rectangle != (rdsrec_list *)NULL;
            Rectangle  = Rectangle->NEXT )
      {
        if ( ! IsRdsOneWindow( Rectangle ) )
        {
          RecWin = (rdsrecwin_list *)Rectangle->USER;
 
          while ( RecWin != (rdsrecwin_list *)NULL )
          {
            DelRecWin = RecWin;
            RecWin    = RecWin->NEXT;

            freerdsrecwin( DelRecWin );
          }
        }

        Rectangle->USER = (void *)NULL;

        ClearRdsOneWindow( Rectangle );
      }
    }
  }

  WinList = RdsWindow->WINTAB;
  
  for ( Counter = 0; Counter < RdsWindow->SIZE; Counter++ )
  {
    if ( WinList->LAYERTAB != (rdswinrec_list **)NULL )
    {
      for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
      {
        WinRec = WinList->LAYERTAB[ Layer ];
      
        while ( WinRec != (rdswinrec_list *)NULL )
        {
          DelWinRec = WinRec;
          WinRec    = WinRec->NEXT;

          freerdswinrec( DelWinRec );
        }
      }
      
      freerdswinlayer( WinList );
    }

    WinList = WinList + 1;
  }

  freerdswin( RdsWindow->WINTAB );
  freerdswindow( RdsWindow );
}

