/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rwiadd.c                        |
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
# include "rwi.h"
# include "rut.h"

# include "rwiadd.h"
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
|                     Add Rectangle In Window                 |
|                                                             |
\------------------------------------------------------------*/

void addrdsrecwindow( Rectangle, RdsWindow )

     rdsrec_list *Rectangle;
     rdswindow   *RdsWindow;
{
  rdswin_list    *LinkWinList;
  rdswin_list    *WinList;
  rdsrecwin_list *RecWin;
  rdswinrec_list *WinRec;
  int             Layer;

  long            X1;
  long            X2;
  long            Y1;
  long            Y2;
  long            FirstX;
 
  long            Counter;

  Y1 = ( Rectangle->Y - 1             - RdsWindow->YMIN ) / RdsWindow->SIDE;
  Y2 = ( Rectangle->Y + Rectangle->DY - RdsWindow->YMIN ) / RdsWindow->SIDE;
  X1 = ( Rectangle->X - 1             - RdsWindow->XMIN ) / RdsWindow->SIDE;
  X2 = ( Rectangle->X + Rectangle->DX - RdsWindow->XMIN ) / RdsWindow->SIDE;

  if ( X1 < 0              ) X1 = 0;
  if ( X2 < 0              ) X2 = 0;
  if ( X1 >= RdsWindow->DX ) X1 = RdsWindow->DX - 1;
  if ( X2 >= RdsWindow->DX ) X2 = RdsWindow->DX - 1;

  if ( Y1 < 0              ) Y1 = 0;
  if ( Y2 < 0              ) Y2 = 0;
  if ( Y1 >= RdsWindow->DY ) Y1 = RdsWindow->DY - 1;
  if ( Y2 >= RdsWindow->DY ) Y2 = RdsWindow->DY - 1;

  FirstX = X1;

  /* End Modif 04/10/94 */

  while ( Y1 <= Y2 )
  {
    WinList = RdsWindow->WINTAB + ( Y1 * RdsWindow->DX );

    X1 = FirstX;

    while ( X1 <= X2 )
    {
      LinkWinList = WinList + X1;

      if ( LinkWinList->LAYERTAB == (rdswinrec_list **)NULL )
      {
        allocrdswinlayer( LinkWinList );
      }

      if ( Rectangle->USER == (void *)NULL )
      {
        Rectangle->USER = (void *)LinkWinList;
        SetRdsOneWindow( Rectangle );
      }
      else
      {
        if ( IsRdsOneWindow( Rectangle ) )
        {
          ClearRdsOneWindow( Rectangle );

          RecWin          = allocrdsrecwin();
          RecWin->WINDOW  = (rdswin_list *)Rectangle->USER;
          Rectangle->USER = (void *)RecWin;
        }

        RecWin          = allocrdsrecwin();
        RecWin->WINDOW  = LinkWinList;
        RecWin->NEXT    = (rdsrecwin_list *)Rectangle->USER;
        Rectangle->USER = (void *)RecWin;
      }

      Layer = GetRdsLayer( Rectangle );

      WinRec = LinkWinList->LAYERTAB[ Layer ];

      if ( WinRec == (rdswinrec_list *)NULL )
      {
        WinRec                         = allocrdswinrec();
        LinkWinList->LAYERTAB[ Layer ] = WinRec;
        WinRec->RECTAB[ 0 ]            = Rectangle;
      }
      else
      {
        for ( Counter = 0; Counter < RWI_MAX_REC; Counter++ )
        {
          if ( WinRec->RECTAB[ Counter ] == (rdsrec_list *)NULL ) break;
        }

        if ( Counter == RWI_MAX_REC )
        {
          WinRec                         = allocrdswinrec();
          WinRec->NEXT                   = LinkWinList->LAYERTAB[ Layer ];
          LinkWinList->LAYERTAB[ Layer ] = WinRec;
          WinRec->RECTAB[ 0 ]            = Rectangle;
        }
        else
        {
          WinRec->RECTAB[ Counter ] = Rectangle;
        }
      }

      X1 = X1 + 1;
    }   

    Y1  = Y1 + 1;
  }
}

