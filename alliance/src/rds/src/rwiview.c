/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rwiview.c                       |
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

# include "rwiview.h"
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
|                        View Rds Window                      |
|                                                             |
\------------------------------------------------------------*/

void viewrdswindow( RdsWindow )

   rdswindow *RdsWindow;
{
  rdswin_list    *ScanWin;
  rdswinrec_list *ScanWinRec;
  rdsrec_list    *Rectangle;
  long            X;
  long            Y;
  long            Xmin;
  long            Ymin;
  long            Xmax;
  long            Ymax;
  long            Counter;
  long            MaskCounter;
  int             Layer;
  int             ScanRec;

  fprintf( stdout, "\n\t--> Window\n" );
  fprintf( stdout, "\n\tXMIN : %ld"  , RdsWindow->XMIN );
  fprintf( stdout, "\n\tXMAX : %ld"  , RdsWindow->XMAX );
  fprintf( stdout, "\n\tYMIN : %ld"  , RdsWindow->YMIN );
  fprintf( stdout, "\n\tYMAX : %ld"  , RdsWindow->YMAX );
  fprintf( stdout, "\n\tMASK : %ld"  , RdsWindow->MASK );
  fprintf( stdout, "\n\tDX   : %ld"  , RdsWindow->DX   );
  fprintf( stdout, "\n\tDY   : %ld"  , RdsWindow->DY   );
  fprintf( stdout, "\n\tSIDE : %ld"  , RdsWindow->SIDE );
  fprintf( stdout, "\n\tSIZE : %ld\n", RdsWindow->SIZE );

  fprintf( stdout, "\n\t--> Dump all windows\n" );

  MaskCounter = 0;
 
  if ( RdsWindow->SIZE != 0 )
  {
    Ymin = RdsWindow->YMIN;
    Ymax = RdsWindow->SIDE + Ymin;
    Y    = 0;

    while ( Y < RdsWindow->DY )
    {
      Xmin = RdsWindow->XMIN;
      Xmax = RdsWindow->SIDE + Xmin;
      X    = 0;

      ScanWin = RdsWindow->WINTAB + ( Y * RdsWindow->DX );

      while ( X < RdsWindow->DX )
      {
        fprintf( stdout, "\n\t--> Window[ %ld, %ld ]\n", X, Y );

        fprintf( stdout, "\n\tXMIN : %ld"  , Xmin );
        fprintf( stdout, "\n\tYMIN : %ld"  , Ymin );
        fprintf( stdout, "\n\tXMAX : %ld"  , Xmax );
        fprintf( stdout, "\n\tYMAX : %ld\n", Ymax );

        Counter = 0;

        if ( ScanWin->LAYERTAB != (rdswinrec_list **)NULL )
        {
          for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
          {
            for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
                  ScanWinRec != (rdswinrec_list *)NULL;
                  ScanWinRec  = ScanWinRec->NEXT )
            {
              for ( ScanRec = 0; ScanRec < RWI_MAX_REC ; ScanRec++ )
              {
                Rectangle = ScanWinRec->RECTAB[ ScanRec ];
   
                if ( Rectangle != (rdsrec_list *)NULL )
                {
                  if ( ( Rectangle->X < Xmax                 ) &&
                       ( Rectangle->Y < Ymax                 ) &&
                       ( Rectangle->X + Rectangle->DX > Xmin ) &&
                       ( Rectangle->Y + Rectangle->DY > Ymin ) )
                  {
                    Counter = Counter + 1;
                  }
  
                  viewrdsrec( Rectangle );
                }
              }
            }
          }
        }

        MaskCounter = MaskCounter + Counter;

        fprintf( stdout, "\n\t<-- Total rectangles %ld", Counter );
        
        Xmin = Xmax;
        Xmax = Xmax + RdsWindow->SIDE;
        X    = X + 1;

        ScanWin = ScanWin + 1;
      }

      Ymin = Ymax;
      Ymax = Ymax + RdsWindow->SIDE;
      Y    = Y + 1;
    }
  }

  fprintf( stdout, "\n<-- Total Rectangles %ld\n", MaskCounter );
}
