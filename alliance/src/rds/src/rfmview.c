/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rfmview.c                       |
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

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "rwi.h"
# include "rut.h"

# include "rfmview.h"
# include "rfmerror.h"

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
|                       View Functions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   View Linked Rectangle                     |
|                                                             |
\------------------------------------------------------------*/

void viewrfmrec( Rectangle )

   rdsrec_list *Rectangle;
{
  rdsrec_list *Scan;
  char        *LayerName;

  long         X;
  long         Y;
  long         DX;
  long         DY;

  Scan = Rectangle;

  if ( Scan != (rdsrec_list *)NULL )
  {
    do
    {
      LayerName = RDS_LAYER_NAME[ GetRdsLayer( Scan ) ];

      fprintf( stdout, "\n--> in rectangle" );

      X  = Scan->X  * SCALE_X / RDS_LAMBDA;
      Y  = Scan->Y  * SCALE_X / RDS_LAMBDA;
      DX = Scan->DX * SCALE_X / RDS_LAMBDA;
      DY = Scan->DY * SCALE_X / RDS_LAMBDA;

      if ( Scan->NAME != (char *)NULL )
      {
        fprintf( stdout, "\n\tNAME  : %s" , Scan->NAME  );
      }
      else
      {
        fprintf( stdout, "\n\tNAME  : NO NAME" );
      }

      fprintf( stdout, "\n\tX     : %ld" , X           );
      fprintf( stdout, "\n\tY     : %ld" , Y           );
      fprintf( stdout, "\n\tDX    : %ld" , DX          );
      fprintf( stdout, "\n\tDY    : %ld" , DY          );
      fprintf( stdout, "\n\tFLAGS : %0lX", Scan->FLAGS );
      fprintf( stdout, "\n\tLAYER : %s"  , LayerName   );

      Scan = (rdsrec_list *)(Scan->USER);
    }
    while ( Scan != Rectangle );
  }
} 

/*------------------------------------------------------------\
|                                                             |
|                   View Segment Translation                  |
|                                                             |
\------------------------------------------------------------*/

void viewrfmseg( Segment, Rectangle )

   phseg_list  *Segment;
   rdsrec_list *Rectangle;
{
  char *LayerName;

  LayerName = MBK_LAYER_NAME[ (int)Segment->LAYER ];

  fprintf( stdout, "\n--> Translate segment" );

  if ( Segment->NAME != (char *)NULL )
  {
    fprintf( stdout, "\n\tNAME  : %s" , Segment->NAME  );
  }
  else
  {
    fprintf( stdout, "\n\tNAME  : NO NAME" );
  }

  fprintf( stdout, "\n\tX1    : %ld", Segment->X1    );
  fprintf( stdout, "\n\tY1    : %ld", Segment->Y1    );
  fprintf( stdout, "\n\tX2    : %ld", Segment->X2    );
  fprintf( stdout, "\n\tY2    : %ld", Segment->Y2    );
  fprintf( stdout, "\n\tWIDTH : %ld", Segment->WIDTH );
  fprintf( stdout, "\n\tLAYER : %s" , LayerName      );

  viewrfmrec( Rectangle );

  fprintf( stdout, "\n<-- End" );
}

/*------------------------------------------------------------\
|                                                             |
|                      View Via Translation                   |
|                                                             |
\------------------------------------------------------------*/

void viewrfmvia( Via, Rectangle )

   phvia_list  *Via;
   rdsrec_list *Rectangle;
{
  char *ViaName;
 
  ViaName = MBK_VIA_NAME[ (int)Via->TYPE ];

  fprintf( stdout, "\n--> Translate via" );

  fprintf( stdout, "\n\tXVIA  : %ld", Via->XVIA );
  fprintf( stdout, "\n\tYVIA  : %ld", Via->YVIA );
  fprintf( stdout, "\n\tTYPE  : %s" , ViaName   );
 
  viewrfmrec( Rectangle );

  fprintf( stdout, "\n<-- End" );
}

/*------------------------------------------------------------\
|                                                             |
|                   View Reference Translation                |
|                                                             |
\------------------------------------------------------------*/

void viewrfmref( Reference, Rectangle )

   phref_list  *Reference;
   rdsrec_list *Rectangle;
{
  fprintf( stdout, "\n--> Translate reference" );

  fprintf( stdout, "\n\tXREF  : %ld", Reference->XREF    );
  fprintf( stdout, "\n\tYREF  : %ld", Reference->YREF    );
  fprintf( stdout, "\n\tTYPE  : %s" , Reference->FIGNAME );

  viewrfmrec( Rectangle );

  fprintf( stdout, "\n<-- End" );
}

/*------------------------------------------------------------\
|                                                             |
|                   View Connector Translation                |
|                                                             |
\------------------------------------------------------------*/

void viewrfmcon( Connector, Rectangle )

  phcon_list  *Connector;
  rdsrec_list *Rectangle;
{
  char *LayerName;

  LayerName = MBK_LAYER_NAME[ (int)Connector->LAYER ];

  fprintf( stdout, "\n--> Translate connector" );

  if ( Connector->NAME != (char *)NULL )
  {
    fprintf( stdout, "\n\tNAME   : %s" , Connector->NAME  );
  }
  else
  {
    fprintf( stdout, "\n\tNAME   : NO NAME" );
  }

  fprintf( stdout, "\n\tXCON   : %ld", Connector->XCON  );
  fprintf( stdout, "\n\tYCON   : %ld", Connector->YCON  );
  fprintf( stdout, "\n\tWIDTH  : %ld", Connector->WIDTH );
  fprintf( stdout, "\n\tLAYER  : %s" , LayerName        );

  viewrfmrec( Rectangle );

  fprintf( stdout, "\n<-- End" );
}

/*------------------------------------------------------------\
|                                                             |
|                    View Instance Translation                |
|                                                             |
\------------------------------------------------------------*/

void viewrfmins( InstanceMbk, InstanceRds )

   phins_list  *InstanceMbk;
   rdsins_list *InstanceRds;
{
  char *TransfName;

  long  X;
  long  Y;

  TransfName = RDS_TRANSF_NAME[ (int)InstanceMbk->TRANSF ];

  fprintf( stdout, "\n--> Translate instance" );

  fprintf( stdout, "\n\tINSNAME : %s" , InstanceMbk->INSNAME );
  fprintf( stdout, "\n\tFIGNAME : %s" , InstanceMbk->FIGNAME );
  fprintf( stdout, "\n\tXINS    : %ld", InstanceMbk->XINS    );
  fprintf( stdout, "\n\tYINS    : %ld", InstanceMbk->YINS    );
  fprintf( stdout, "\n\tTRANSF  : %s" , TransfName           );

  TransfName = RDS_TRANSF_NAME[ (int)InstanceRds->TRANSF ];

  fprintf( stdout, "\n--> in instance" );

  X = InstanceRds->X * SCALE_X / RDS_LAMBDA;
  Y = InstanceRds->Y * SCALE_X / RDS_LAMBDA;
 
  fprintf( stdout, "\n\tINSNAME : %s" , InstanceRds->INSNAME );
  fprintf( stdout, "\n\tFIGNAME : %s" , InstanceRds->FIGNAME );
  fprintf( stdout, "\n\tX       : %ld", X                    );
  fprintf( stdout, "\n\tY       : %ld", Y                    );
  fprintf( stdout, "\n\tTRANSF  : %s" , TransfName           );
  fprintf( stdout, "\n\tSIZE    : %d" , InstanceRds->SIZE    );

  fprintf( stdout, "\n<-- End" );
}

/*------------------------------------------------------------\
|                                                             |
|                      View Figure Translation                |
|                                                             |
\------------------------------------------------------------*/

void viewrfmfig( FigureMbk, FigureRds )

   phfig_list  *FigureMbk;
   rdsfig_list *FigureRds;
{
  fprintf( stdout, "\n--> Translate figure" );

  fprintf( stdout, "\n\tNAME : %s" , FigureMbk->NAME );

  fprintf( stdout, "\n--> in figure" );
 
  fprintf( stdout, "\n\tNAME : %s" , FigureRds->NAME );
  fprintf( stdout, "\n\tMODE : %c" , FigureRds->MODE );
  fprintf( stdout, "\n\tSIZE : %d" , FigureRds->SIZE );

  fprintf( stdout, "\n<-- End" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Round Rds Rectangle                     |
|                                                             |
\------------------------------------------------------------*/

void roundrdsrec( Rectangle )

   rdsrec_list *Rectangle;
{
   long X2, Y2;

   X2 = RfmRoundHigh( Rectangle->X + Rectangle->DX );
   Y2 = RfmRoundHigh( Rectangle->Y + Rectangle->DY );

   Rectangle->X  = RfmRoundLow( Rectangle->X );
   Rectangle->Y  = RfmRoundLow( Rectangle->Y );
   Rectangle->DX = X2 - Rectangle->X;
   Rectangle->DY = Y2 - Rectangle->Y;
}
