/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rwiget.c                        |
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

# include "rwiget.h"
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

  static chain_list *HEAD_RWICHAIN = (chain_list *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         getrdswindow                        |
|                                                             |
\------------------------------------------------------------*/

chain_list *getrdswindow( X, Y, Dx, Dy, RdsWindow )

   long       X;
   long       Y;
   long       Dx;
   long       Dy;
   rdswindow *RdsWindow;
{
  rdswin_list    *WinList;
  long            X1;
  long            X2;
  long            Y1;
  long            Y2;
  long            FirstX;

  if ( HEAD_RWICHAIN != (chain_list *)NULL )
  {
    freechain( HEAD_RWICHAIN );
    HEAD_RWICHAIN = (chain_list *)NULL;
  }

  Y1 = ( Y - 1  - RdsWindow->YMIN ) / RdsWindow->SIDE;
  Y2 = ( Y + Dy - RdsWindow->YMIN ) / RdsWindow->SIDE;
  X1 = ( X - 1  - RdsWindow->XMIN ) / RdsWindow->SIDE;
  X2 = ( X + Dx - RdsWindow->XMIN ) / RdsWindow->SIDE;

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
      allocrdswinlayer( WinList + X1 );

      HEAD_RWICHAIN = addchain( HEAD_RWICHAIN, 
                                (void *)(WinList + X1) );
      X1 = X1 + 1;
    }

    Y1  = Y1 + 1;
  }

  return( HEAD_RWICHAIN );
}

