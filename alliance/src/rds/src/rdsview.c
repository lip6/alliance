/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rdsview.c                       |
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

# include <mut.h>
# include <mph.h>
# include "rds.h"

# include "rdsview.h"
# include "rdserror.h"

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
|                         View Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       View Rds Rectangle                    |
|                                                             |
\------------------------------------------------------------*/

void viewrdsrec( Rectangle )

     rdsrec_list *Rectangle;
{
  char *LayerName;

  LayerName = RDS_LAYER_NAME[ GetRdsLayer( Rectangle ) ];

  fprintf( stdout, "\n\t--> Rectangle" );

  if ( Rectangle->NAME != (char *)NULL )
  {
    fprintf( stdout, "\n\t\tNAME  : %s" , Rectangle->NAME  );
  }
  else
  {   
    fprintf( stdout, "\n\t\tNAME  : NO NAME" );
  }

  fprintf( stdout, "\n\t\tX     : %ld" , Rectangle->X     );
  fprintf( stdout, "\n\t\tY     : %ld" , Rectangle->Y     );
  fprintf( stdout, "\n\t\tDX    : %ld" , Rectangle->DX    );
  fprintf( stdout, "\n\t\tDY    : %ld" , Rectangle->DY    );
  fprintf( stdout, "\n\t\tFLAGS : %0lX", Rectangle->FLAGS );
  fprintf( stdout, "\n\t\tLAYER : %s"  , LayerName        );
}

/*------------------------------------------------------------\
|                                                             |
|                       View Rds Instance                     |
|                                                             |
\------------------------------------------------------------*/

void viewrdsins( Instance )

     rdsins_list *Instance;
{
  int          Layer;
  rdsrec_list *Rectangle;
  char        *TransfName;

  TransfName = RDS_TRANSF_NAME[ (int)Instance->TRANSF ];

  fprintf( stdout, "\n--> Instance" );

  fprintf( stdout, "\n\tINSNAME : %s" , Instance->INSNAME );
  fprintf( stdout, "\n\tFIGNAME : %s" , Instance->FIGNAME );
  fprintf( stdout, "\n\tX       : %ld", Instance->X       );
  fprintf( stdout, "\n\tY       : %ld", Instance->Y       );
  fprintf( stdout, "\n\tTRANSF  : %s" , TransfName        );
  fprintf( stdout, "\n\tSIZE    : %d" , Instance->SIZE    );
  fprintf( stdout, "\n\tFLAGS   : %x" , Instance->FLAGS   );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = Instance->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)NULL;
          Rectangle  = Rectangle->NEXT )
    {
      viewrdsrec( Rectangle );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       View Rds Figure                       |
|                                                             |
\------------------------------------------------------------*/

void viewrdsfig( Figure )

     rdsfig_list *Figure;
{
  int          Layer;
  rdsins_list *Instance;
  rdsrec_list *Rectangle;

  fprintf( stdout, "\n--> Figure" );

  fprintf( stdout, "\n\tNAME  : %s", Figure->NAME  );
  fprintf( stdout, "\n\tMODE  : %c", Figure->MODE  );
  fprintf( stdout, "\n\tSIZE  : %d", Figure->SIZE  );
  fprintf( stdout, "\n\tFLAGS : %x", Figure->FLAGS );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = Figure->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)NULL;
          Rectangle  = Rectangle->NEXT )
    {
      viewrdsrec( Rectangle );
    }
  }

  for ( Instance  = Figure->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    viewrdsins( Instance );
  }
}
