/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rwibuild.c                       |
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
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rwi.h"

# include "rwibuild.h"
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
|                        Build Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Build Rds Window                     |
|                                                             |
\------------------------------------------------------------*/

rdswindow *buildrdswindow( Figure )

   rdsfig_list *Figure;
{
  rdswindow   *RdsWindow;
  rdsrec_list *Rectangle;
  rdsins_list *Instance;
  int          Layer;
 
  long         X1;
  long         Y1;
  long         X2;
  long         Y2;

  long         Mask;
  long         Xmin;
  long         Ymin;
  long         Xmax;
  long         Ymax;

  RdsWindow = allocrdswindow();

  Mask = 0;

  Xmin = Xmax = Ymin = Ymax = 0;

  for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = Figure->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)NULL;
          Rectangle  = Rectangle->NEXT )
    {
      Rectangle->USER = (void *)NULL;

      ClearRdsOneWindow( Rectangle );

      X1 = Rectangle->X;
      X2 = X1 + Rectangle->DX;
      Y1 = Rectangle->Y;
      Y2 = Y1 + Rectangle->DY;

      if ( Mask == 0 )
      {
        Xmax = X2;
        Ymax = Y2;
        Xmin = X1;
        Ymin = Y1;
      }
      else
      {   
        if ( Xmax < X2 ) Xmax = X2;
        if ( Ymax < Y2 ) Ymax = Y2;
        if ( Xmin > X1 ) Xmin = X1;
        if ( Ymin > Y1 ) Ymin = Y1;
      }

      Mask = Mask + 1;
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
        Rectangle->USER = (void *)NULL;

        ClearRdsOneWindow( Rectangle );

        X1 = Rectangle->X;
        X2 = X1 + Rectangle->DX;
        Y1 = Rectangle->Y;
        Y2 = Y1 + Rectangle->DY;

        if ( Mask == 0 )
        {
          Xmax = X2;
          Ymax = Y2;
          Xmin = X1;
          Ymin = Y1;
        }
        else
        {   
          if ( Xmax < X2 ) Xmax = X2;
          if ( Ymax < Y2 ) Ymax = Y2;
          if ( Xmin > X1 ) Xmin = X1;
          if ( Ymin > Y1 ) Ymin = Y1;
        }

        Mask = Mask + 1;
      }  
    }  
  }

  RdsWindow->XMIN = Xmin; 
  RdsWindow->XMAX = Xmax; 
  RdsWindow->YMIN = Ymin; 
  RdsWindow->YMAX = Ymax; 
  RdsWindow->MASK = Mask; 

  if ( Mask == 0 ) return( RdsWindow );

  /* Begin modif May, 5 1994 */

  if ( RDS_WINDOW_AREA == 0 )  
  {
    RdsWindow->SIDE = ( long )( sqrt( ( double )( Xmax - Xmin ) *
                                      ( double )( Ymax - Ymin ) ) /
                                sqrt( sqrt( ( double )Mask ) ) );

    RdsWindow->DX   = 1 + ( Xmax - Xmin ) / RdsWindow->SIDE;
    RdsWindow->DY   = 1 + ( Ymax - Ymin ) / RdsWindow->SIDE;
    RdsWindow->SIZE = RdsWindow->DX * RdsWindow->DY;
  }
  else
  {
    RdsWindow->SIDE = ( long )( sqrt( (double)RDS_WINDOW_AREA * 
                                      (double)RDS_LAMBDA2 ) );
    RdsWindow->DX   = 1 + ( Xmax - Xmin ) / RdsWindow->SIDE;
    RdsWindow->DY   = 1 + ( Ymax - Ymin ) / RdsWindow->SIDE;
    RdsWindow->SIZE = RdsWindow->DX * RdsWindow->DY;
  }

  /* End modif May, 5 1994 */

  RdsWindow->WINTAB = allocrdswin( RdsWindow->SIZE );

  for ( Layer = 0; Layer < RWI_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = Figure->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)NULL;
          Rectangle  = Rectangle->NEXT )
    {
      addrdsrecwindow( Rectangle, RdsWindow );
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
        addrdsrecwindow( Rectangle, RdsWindow );
      }
    }
  }

  return( RdsWindow );
}
