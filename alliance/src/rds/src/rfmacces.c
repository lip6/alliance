/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  Rfmacces.c                       |
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
# include "rtl.h"
# include "rpr.h"
# include "rfm.h"
# include "rwi.h"
# include "rut.h"

# include "rfmacces.h"
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

  static char *RfmRefRef = (char *)NULL;

  static char  RfmNewConnectorOrient[ RDS_MAX_TRANSF ] =

   {
     1,  /* NOSYM */
     0,  /* ROT_P */
     1,  /* SYMXY */
     0,  /* ROT_M */
     1,  /* SYM_X */
     0,  /* SY_RM */
     1,  /* SYM_Y */
     0   /* SY_RP */
   };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                MBK Segment To RDS Rectangle                 |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *segmbkrds( Figure, Segment, Lynx )

   rdsfig_list *Figure;
   phseg_list  *Segment;
   char         Lynx;
{
  rdsrec_list  *Rectangle;
  rdsrec_list  *First;
  rdsrec_list  *Link;

  long          X1;
  long          Y1;
  long          X2;
  long          Y2;
  long          WS;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;

  char          TRANS;
  long          DLR;
  long          DWR;
  long          OFFSET;
  char          USE; 

  int           LayerMbk;
  int           LayerRds;
  int           SegConMbk;
  int           GateRds;
  int           Index;

  Rectangle = (rdsrec_list *)NULL;
  First     = (rdsrec_list *)NULL;
  Link      = (rdsrec_list *)NULL;

  X1 = Segment->X1    * RDS_LAMBDA / SCALE_X;
  X2 = Segment->X2    * RDS_LAMBDA / SCALE_X;
  Y1 = Segment->Y1    * RDS_LAMBDA / SCALE_X;
  Y2 = Segment->Y2    * RDS_LAMBDA / SCALE_X;
  WS = Segment->WIDTH * RDS_LAMBDA / SCALE_X;

  X1R = Y1R = X2R = Y2R = 0;

  LayerMbk  = Segment->LAYER;
  Index     = 0;

  if ( ( LayerMbk >= CALU1 ) &&
       ( LayerMbk <= CALU9 ) ) SegConMbk = 1;
  else                         SegConMbk = 0;

  GateRds  = GET_LYNX_TRANSISTOR_GATE_LAYER( LayerMbk );
  LayerRds = GET_SEGMENT_LAYER( LayerMbk, Index );

  while ( LayerRds != RDS_SEGMENT_EMPTY )
  {
    TRANS  = GET_SEGMENT_TRANS ( LayerMbk, Index );
    DLR    = GET_SEGMENT_DLR   ( LayerMbk, Index );
    DWR    = GET_SEGMENT_DWR   ( LayerMbk, Index );
    OFFSET = GET_SEGMENT_OFFSET( LayerMbk, Index );
    USE    = GET_SEGMENT_USE   ( LayerMbk, Index );

    if ( ( ( USE == RDS_USE_ALL     )               ) ||
         ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
         ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
    {
      if ( TRANS == RDS_TRANS_VWS )
      {
        long SHRINK = OFFSET;
        switch ( Segment->TYPE )
        {
          case LEFT   : X1R = X1 - DLR;
                        Y1R = Y1 - ( ( WS + DWR ) >> 1 );
                        X2R = X2 + DLR - abs(SHRINK);
                        Y2R = Y2 + ( ( WS + DWR ) >> 1 ) - abs(SHRINK);

                      break;

          case RIGHT  : X1R = X1 - DLR;
                        Y1R = Y1 - ( ( WS + DWR ) >> 1 );
                        X2R = X2 + DLR - abs(SHRINK);
                        Y2R = Y2 + ( ( WS + DWR ) >> 1 ) - abs(SHRINK); 

                      break;

          case DOWN   : X1R = X1 - ( ( WS + DWR ) >> 1 );
                        Y1R = Y1 - DLR;
                        X2R = X2 + ( ( WS + DWR ) >> 1 ) - abs(SHRINK);
                        Y2R = Y2 + DLR - abs(SHRINK);

                      break;

          case UP     : X1R = X1 - ( ( WS + DWR ) >> 1 );
                        Y1R = Y1 - DLR;
                        X2R = X2 + ( ( WS + DWR ) >> 1 ) - abs(SHRINK);
                        Y2R = Y2 + DLR - abs(SHRINK);

                      break;
        }
      }
      else
      if ( TRANS == RDS_TRANS_VW )
      {
        switch ( Segment->TYPE )
        {
          case LEFT   : X1R = X1 - DLR;
                        Y1R = Y1 - ( ( WS + DWR ) >> 1 ) + OFFSET;
                        X2R = X2 + DLR;
                        Y2R = Y2 + ( ( WS + DWR ) >> 1 ) + OFFSET;

                      break;

          case RIGHT  : X1R = X1 - DLR;
                        Y1R = Y1 - ( ( WS + DWR ) >> 1 ) - OFFSET;
                        X2R = X2 + DLR;
                        Y2R = Y2 + ( ( WS + DWR ) >> 1 ) - OFFSET;

                      break;

          case DOWN   : X1R = X1 - ( ( WS + DWR ) >> 1 ) - OFFSET;
                        Y1R = Y1 - DLR;
                        X2R = X2 + ( ( WS + DWR ) >> 1 ) - OFFSET;
                        Y2R = Y2 + DLR;

                      break;

          case UP     : X1R = X1 - ( ( WS + DWR ) >> 1 ) + OFFSET;
                        Y1R = Y1 - DLR;
                        X2R = X2 + ( ( WS + DWR ) >> 1 ) + OFFSET;
                        Y2R = Y2 + DLR;

                      break;
        }
      }
      else
      if ( TRANS == RDS_TRANS_LCW )
      {
        switch ( Segment->TYPE )
        {
          case RIGHT  : X1R = X1 - DLR;
                        Y1R = Y1 + (WS >> 1) + OFFSET;
                        X2R = X2 + DLR;
                        Y2R = Y2 + (WS >> 1) + DWR + OFFSET;

                      break;

          case LEFT   : X1R = X1 - DLR;
                        Y1R = Y1 - (WS >> 1) - DWR - OFFSET;
                        X2R = X2 + DLR;
                        Y2R = Y2 - (WS >> 1) - OFFSET;

                      break;

          case UP     : X1R = X1 - (WS >> 1) - DWR - OFFSET;
                        Y1R = Y1 - DLR;
                        X2R = X2 - (WS >> 1) - OFFSET;
                        Y2R = Y2 + DLR;

                      break;

          case DOWN   : X1R = X1 + (WS >> 1) + OFFSET;
                        Y1R = Y1 - DLR;
                        X2R = X2 + (WS >> 1) + DWR + OFFSET;
                        Y2R = Y2 + DLR;

                      break;
        }
      }
      else
      if ( TRANS == RDS_TRANS_RCW )
      {
        switch ( Segment->TYPE )
        {
          case RIGHT  : X1R = X1 - DLR;
                        Y1R = Y1 - (WS >> 1) - DWR - OFFSET;
                        X2R = X2 + DLR;
                        Y2R = Y2 - (WS >> 1) - OFFSET;

                      break;

          case LEFT   : X1R = X1 - DLR;
                        Y1R = Y1 + (WS >> 1) + OFFSET;
                        X2R = X2 + DLR;
                        Y2R = Y2 + (WS >> 1) + DWR + OFFSET;

                      break;

          case UP     : X1R = X1 + (WS >> 1) + OFFSET;
                        Y1R = Y1 - DLR;
                        X2R = X2 + (WS >> 1) + DWR + OFFSET;
                        Y2R = Y2 + DLR;

                      break;

          case DOWN   : X1R = X1 - (WS >> 1) - DWR - OFFSET;
                        Y1R = Y1 - DLR;
                        X2R = X2 - (WS >> 1) - OFFSET;
                        Y2R = Y2 + DLR;

                      break;
        }
      }
    
      X1R = RfmRoundLow ( X1R );
      Y1R = RfmRoundLow ( Y1R );
      X2R = RfmRoundHigh( X2R );
      Y2R = RfmRoundHigh( Y2R );

      Rectangle = addrdsfigrec( Figure, Segment->NAME, 
                                LayerRds, X1R, Y1R, 
                                X2R - X1R, Y2R - Y1R );

      SetRdsSegment( Rectangle );

      if ( SegConMbk ) SetRdsConExter( Rectangle );

      if ( LayerRds == GateRds )
      {
        SetRdsTransGate( Rectangle );
      }

      if ( First == (rdsrec_list *)NULL )
      {
        First = Rectangle;
        Link  = Rectangle;
      }
      else
      {
        Link->USER = (void *)Rectangle;
        Link       = Rectangle;
      }
    }

    Index = Index + 1;

    LayerRds = GET_SEGMENT_LAYER( LayerMbk, Index );
  }

  if ( First != (rdsrec_list *)NULL ) Link->USER = (void *)First;

  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|                   MBK Via To RDS Rectangle                  |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *viambkrds( Figure, Via, Lynx )

   rdsfig_list *Figure;
   phvia_list  *Via;
   char         Lynx;
{
  rdsrec_list  *Rectangle;
  rdsrec_list  *First;
  rdsrec_list  *Link;

  long          Xvia;
  long          Yvia;
  long          WSX;
  long          WSY;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;

  long          BX1R;
  long          BY1R;
  long          BX2R;
  long          BY2R;

  long          VX1R;
  long          VY1R;
  long          VX2R;
  long          VY2R;

  long          SIDE;
  long          SIZE;
  long          STEP;
  long          SIDE_STEP;
  long          DWR;
  long          OVERLAP;
  long          MIN_DWR;
  long          MAX_OVER;
  char          USE; 

  int           ViaMbk;
  int           LayerRds;
  int           Index;

  ViaMbk = Via->TYPE;

  Rectangle = (rdsrec_list *)NULL;
  First     = (rdsrec_list *)NULL;
  Link      = (rdsrec_list *)NULL;
  OVERLAP   = 0;
  DWR       = 0;

  Xvia = Via->XVIA * RDS_LAMBDA / SCALE_X;
  Yvia = Via->YVIA * RDS_LAMBDA / SCALE_X;
 
  if ( ( Via->DX != 0 ) ||
       ( Via->DY != 0 ) )
  {
    WSX  = Via->DX * RDS_LAMBDA / SCALE_X;
    WSY  = Via->DY * RDS_LAMBDA / SCALE_X;
    
    LayerRds = GET_BIGVIA_HOLE_LAYER( ViaMbk, 0 );

    if ( LayerRds != RDS_BIGVIA_HOLE_EMPTY )
    {
      X1R = Y1R = X2R = Y2R = 0;
/*
** Metal rectangles
*/
      Index    =  0;
      MIN_DWR  = -1;
      MAX_OVER =  0;
      LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
    
      while ( LayerRds != RDS_BIGVIA_METAL_EMPTY )
      {
        DWR     = GET_BIGVIA_METAL_DWR( ViaMbk, Index );
        OVERLAP = GET_BIGVIA_METAL_OVERLAP( ViaMbk, Index );
        USE     = GET_BIGVIA_METAL_USE( ViaMbk, Index );
    
        if ( ( ( USE == RDS_USE_ALL     )               ) ||
             ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
             ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
        {
          if ( ( MIN_DWR == -1  ) ||
               ( MIN_DWR >  DWR ) ) MIN_DWR = DWR;
    
          if ( MAX_OVER < OVERLAP ) MAX_OVER = OVERLAP;
        }
    
        Index = Index + 1;
    
        LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
      }
    
      if ( MIN_DWR < 0 ) MIN_DWR = 0;
/*
** Hole rectangles
*/
      Index    = 0;
      LayerRds = GET_BIGVIA_HOLE_LAYER( ViaMbk, Index );
    
      while ( LayerRds != RDS_BIGVIA_HOLE_EMPTY )
      {
        SIDE = GET_BIGVIA_HOLE_SIDE( ViaMbk, Index );
        STEP = GET_BIGVIA_HOLE_STEP( ViaMbk, Index );
        USE  = GET_BIGVIA_HOLE_USE(  ViaMbk, Index );

        SIDE_STEP = SIDE + STEP;
    
        if ( ( ( USE == RDS_USE_ALL     )               ) ||
             ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
             ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
        {
          if ( SIDE_STEP == 0 ) break;

          X1R = Xvia + OVERLAP - ( ( WSX + DWR ) >> 1 );
          Y1R = Yvia + OVERLAP - ( ( WSY + DWR ) >> 1 );
          X2R = Xvia - OVERLAP + ( ( WSX + DWR ) >> 1 );
          Y2R = Yvia - OVERLAP + ( ( WSY + DWR ) >> 1 );

          X1R = X1R + (SIDE >> 1);
          Y1R = Y1R + (SIDE >> 1);
      
          X1R = RfmRoundLow ( X1R );
          X2R = RfmRoundHigh( X2R );
          Y1R = RfmRoundLow ( Y1R );
          Y2R = RfmRoundHigh( Y2R );

          if ( X1R >= 0 ) X1R = ( (X1R + SIDE_STEP - 1) / SIDE_STEP ) * SIDE_STEP;
          else            X1R = ( X1R / SIDE_STEP ) * SIDE_STEP;

          if ( Y1R >= 0 ) Y1R = ( (Y1R + SIDE_STEP - 1) / SIDE_STEP ) * SIDE_STEP;
          else            Y1R = ( Y1R / SIDE_STEP ) * SIDE_STEP;

          BX1R = X1R;

          while ( BX1R <= X2R )
          {
            BX1R = BX1R - (SIDE >> 1);
            BX2R = BX1R + SIDE;
            VX1R = RfmRoundLow ( BX1R );
            VX2R = RfmRoundHigh( BX2R );

            if ( VX2R > X2R ) break;
    
            BY1R = Y1R;
    
            while ( BY1R <= Y2R )
            {
              BY1R = BY1R - (SIDE >> 1);
              BY2R = BY1R + SIDE;
              VY1R = RfmRoundLow ( BY1R );
              VY2R = RfmRoundHigh( BY2R );
            
              if ( VY2R > Y2R ) break;
    
              Rectangle = addrdsfigrec( Figure, (char *)0,
                                        LayerRds, VX1R, VY1R,
                                        VX2R - VX1R, VY2R - VY1R );
    
              SetRdsVia( Rectangle );
              if ( First == (rdsrec_list *)NULL )
              {
                First = Rectangle;
                Link  = Rectangle;
              }
              else
              {
                Link->USER = (void *)Rectangle;
                Link       = Rectangle;
              }
    
              BY1R = BY2R + STEP + (SIDE >> 1);
            }
    
            BX1R = BX2R + STEP + (SIDE >> 1);
          }
        }
    
        Index = Index + 1;
    
        LayerRds = GET_BIGVIA_HOLE_LAYER( ViaMbk, Index );
      }
/*
** Metal rectangles
*/
      if ( Rectangle != (rdsrec_list *)0 )
      {
        Index    =  0;
        LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
      
        while ( LayerRds != RDS_BIGVIA_METAL_EMPTY )
        {
          DWR = GET_BIGVIA_METAL_DWR( ViaMbk, Index );
          USE = GET_BIGVIA_METAL_USE( ViaMbk, Index );
      
          if ( ( ( USE == RDS_USE_ALL     )               ) ||
               ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
               ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
          {
            X1R = Xvia - ( ( WSX + DWR ) >> 1 );
            Y1R = Yvia - ( ( WSY + DWR ) >> 1 );
            X2R = Xvia + ( ( WSX + DWR ) >> 1 );
            Y2R = Yvia + ( ( WSY + DWR ) >> 1 );
      
            X1R = RfmRoundLow ( X1R );
            Y1R = RfmRoundLow ( Y1R );
            X2R = RfmRoundHigh( X2R );
            Y2R = RfmRoundHigh( Y2R );
      
            Rectangle = addrdsfigrec( Figure, Via->NAME,
                                      LayerRds, X1R, Y1R,
                                      X2R - X1R, Y2R - Y1R );
      
            SetRdsVia( Rectangle );
      
            if ( First == (rdsrec_list *)NULL )
            {
              First = Rectangle;
              Link  = Rectangle;
            }
            else
            {
              Link->USER = (void *)Rectangle;
              Link       = Rectangle;
            }
          }
      
          Index = Index + 1;
      
          LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
        }
      }
    }
    else
    {
      LayerRds  = GET_TURNVIA_LAYER( ViaMbk, 0 );

      if ( LayerRds != RDS_TURNVIA_EMPTY )
      {
        X1R = Y1R = X2R = Y2R = 0;
  
        Index   = 0;
      
        LayerRds = GET_TURNVIA_LAYER( ViaMbk, Index );
      
        while ( LayerRds != RDS_TURNVIA_EMPTY )
        {
          DWR    = GET_TURNVIA_DWR( ViaMbk, Index );
          USE    = GET_TURNVIA_USE( ViaMbk, Index );
      
          if ( ( ( USE == RDS_USE_ALL     )               ) ||
               ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
               ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
          {
            X1R = Xvia - ( ( WSX + DWR ) >> 1 );
            Y1R = Yvia - ( ( WSY + DWR ) >> 1 );
            X2R = Xvia + ( ( WSX + DWR ) >> 1 );
            Y2R = Yvia + ( ( WSY + DWR ) >> 1 );
      
            X1R = RfmRoundLow ( X1R );
            Y1R = RfmRoundLow ( Y1R );
            X2R = RfmRoundHigh( X2R );
            Y2R = RfmRoundHigh( Y2R );
      
            Rectangle = addrdsfigrec( Figure, Via->NAME,
                                      LayerRds, X1R, Y1R,
                                      X2R - X1R, Y2R - Y1R );
      
            SetRdsVia( Rectangle );
      
            if ( First == (rdsrec_list *)NULL )
            {
              First = Rectangle;
              Link  = Rectangle;
            }
            else
            {
              Link->USER = (void *)Rectangle;
              Link       = Rectangle;
            }
          }
      
          Index = Index + 1;
      
          LayerRds = GET_TURNVIA_LAYER( ViaMbk, Index );
        }
      }
    }
  }

  if ( First == (rdsrec_list *)0 )
  {
    Index     = 0;
    LayerRds  = GET_VIA_LAYER( ViaMbk, Index );
  
    while ( LayerRds != RDS_VIA_EMPTY )
    {
      USE = GET_VIA_USE( ViaMbk, Index );
  
      if ( ( ( USE == RDS_USE_ALL     )               ) ||
           ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
           ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
      {
        SIZE = GET_VIA_SIZE( ViaMbk, Index );
  
        X1R = Xvia - ( SIZE >> 1 );
        Y1R = Yvia - ( SIZE >> 1 );
        X2R = Xvia + ( SIZE >> 1 );
        Y2R = Yvia + ( SIZE >> 1 );
  
        X1R = RfmRoundLow ( X1R );
        Y1R = RfmRoundLow ( Y1R );
        X2R = RfmRoundHigh( X2R );
        Y2R = RfmRoundHigh( Y2R );
  
        Rectangle = addrdsfigrec( Figure, Via->NAME,
                                  LayerRds, X1R, Y1R,
                                  X2R - X1R, Y2R - Y1R );
  
        SetRdsVia( Rectangle );
  
        if ( First == (rdsrec_list *)NULL )
        {
          First = Rectangle;
          Link  = Rectangle;
        }
        else
        {
          Link->USER = (void *)Rectangle;
          Link       = Rectangle;
        }
      }
  
      Index = Index + 1;
  
      LayerRds = GET_VIA_LAYER( ViaMbk, Index );
    }
  }

  if ( First != (rdsrec_list *)NULL ) Link->USER = (void *)First;
 
  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|                   MBK Ref To RDS Rectangle                  |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *refmbkrds( Figure, Reference )

   rdsfig_list *Figure;
   phref_list  *Reference;
{
  rdsrec_list  *Rectangle;
  rdsrec_list  *First;
  rdsrec_list  *Link;

  long          Xref;
  long          Yref;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;

  long          SIZE;

  int           RefMbk;
  int           LayerRds;
  int           Index;

  if ( RfmRefRef == (char *)NULL ) 
  {
    RfmRefRef = namealloc( "ref_ref" );
  }

  Rectangle = (rdsrec_list *)NULL;
  First     = (rdsrec_list *)NULL;
  Link      = (rdsrec_list *)NULL;

  Xref = Reference->XREF * RDS_LAMBDA / SCALE_X;
  Yref = Reference->YREF * RDS_LAMBDA / SCALE_X;

  RefMbk    = (Reference->FIGNAME == RfmRefRef)? MBK_REF_REF:MBK_REF_CON;
  Index     = 0;
  LayerRds  = GET_REFERENCE_LAYER( RefMbk, Index );

  while ( LayerRds != RDS_REFERENCE_EMPTY )
  {
    SIZE = GET_REFERENCE_SIZE( RefMbk, Index );

    X1R = Xref - ( SIZE >> 1 );
    Y1R = Yref - ( SIZE >> 1 );
    X2R = Xref + ( SIZE >> 1 );
    Y2R = Yref + ( SIZE >> 1 );

    X1R = RfmRoundLow ( X1R );
    Y1R = RfmRoundLow ( Y1R );
    X2R = RfmRoundHigh( X2R );
    Y2R = RfmRoundHigh( Y2R );

    Rectangle = addrdsfigrec( Figure, Reference->NAME, 
                              LayerRds, X1R, Y1R,
                              X2R - X1R, Y2R - Y1R );

    SetRdsReference( Rectangle );

    if ( RefMbk == MBK_REF_REF )
    {
      SetRdsRefRef( Rectangle );
    }
    else
    {
      SetRdsRefCon( Rectangle );
    }

    if ( First == (rdsrec_list *)NULL )
    {
      First = Rectangle;
      Link  = Rectangle;
    }
    else
    {
      Link->USER = (void *)Rectangle;
      Link       = Rectangle;
    }


    Index = Index + 1;
 
    LayerRds = GET_REFERENCE_LAYER( RefMbk, Index );
  }

  if ( First != (rdsrec_list *)NULL ) Link->USER = (void *)First;
 
  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|                  MBK Connector To RDS Rectangle             |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *conmbkrds( Figure, Connector )

   rdsfig_list *Figure;
   phcon_list  *Connector;
{
  rdsrec_list  *Rectangle;

  long          Xcon;
  long          Ycon;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;
  long          WS;

  long          DWR;
  long          DER;

  int           LayerMbk;
  int           LayerRds;

  Rectangle = (rdsrec_list *)NULL;

  Xcon = Connector->XCON  * RDS_LAMBDA / SCALE_X;
  Ycon = Connector->YCON  * RDS_LAMBDA / SCALE_X;
  WS   = Connector->WIDTH * RDS_LAMBDA / SCALE_X;
    
  LayerMbk  = Connector->LAYER;
  LayerRds  = GET_CONNECTOR_LAYER( LayerMbk );
    
  if ( LayerRds != RDS_CONNECTOR_EMPTY )
  {
    DWR = GET_CONNECTOR_DWR( LayerMbk );
    DER = GET_CONNECTOR_DER( LayerMbk );

    if ( ( Connector->ORIENT == EAST ) || 
         ( Connector->ORIENT == WEST ) )
    {
      X1R = Xcon - DER;
      Y1R = Ycon - (( WS + DWR ) >> 1);
      X2R = Xcon + DER;
      Y2R = Ycon + (( WS + DWR ) >> 1);
    }
    else
    {
      X1R = Xcon - (( WS + DWR ) >> 1);
      Y1R = Ycon - DER;
      X2R = Xcon + (( WS + DWR ) >> 1);
      Y2R = Ycon + DER;
    }
 
    X1R = RfmRoundLow ( X1R );
    Y1R = RfmRoundLow ( Y1R );
    X2R = RfmRoundHigh( X2R );
    Y2R = RfmRoundHigh( Y2R );

    Rectangle = addrdsfigrec( Figure, Connector->NAME, 
                              LayerRds, X1R, Y1R,
                              X2R - X1R, Y2R - Y1R );
 
    SetRdsConnector( Rectangle );
    SetRdsConExter( Rectangle );

    Rectangle->USER = (void *)Rectangle;
  }

  return( Rectangle );
}

/*------------------------------------------------------------\
|                                                             |
|            MBK Instance Segment To RDS Rectangle            |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *inssegmbkrds( InstanceMbk, ModelMbk, InstanceRds, Lynx )

   phins_list  *InstanceMbk;
   phfig_list  *ModelMbk;
   rdsins_list *InstanceRds;
   char         Lynx;
{
  phseg_list   *Segment;
  rdsrec_list  *Rectangle;
  rdsrec_list  *First;
  rdsrec_list  *Link;

  long          SegX1;
  long          SegX2;
  long          SegY1;
  long          SegY2;

  long          X1;
  long          Y1;
  long          X2;
  long          Y2;
  long          WS;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;

  long          DLR;
  long          DWR;
  long          OFFSET;
  int           TRANS;
  int           USE;
  long          Swap;

  int           LayerMbk;
  int           LayerRds;
  int           SegConMbk;
  int           Index;
  int           Type;

  Rectangle = (rdsrec_list *)NULL;
  First     = (rdsrec_list *)NULL;
  Link      = (rdsrec_list *)NULL;

  X1R = Y1R = X2R = Y2R = 0;

  Type = LEFT;

  for ( Segment  = ModelMbk->PHSEG;
        Segment != (phseg_list *)NULL;
        Segment  = Segment->NEXT )
  { 
    LayerMbk = Segment->LAYER;

    if ( ( LayerMbk >= CALU1 ) &&
         ( LayerMbk <= CALU9 ) ) SegConMbk = 1;
    else                         SegConMbk = 0;

    SegX1 = Segment->X1;
    SegY1 = Segment->Y1;
    SegX2 = Segment->X2;
    SegY2 = Segment->Y2;

    if ( Segment->TYPE == LEFT )
    {
      Swap  = SegX2; SegX2 = SegX1; SegX1 = Swap;
    }
    else
    if ( Segment->TYPE == DOWN )
    {
      Swap  = SegY2; SegY2 = SegY1; SegY1 = Swap;
    }

    xyflat
    ( &X1                  , &Y1,
      SegX1                , SegY1,
      InstanceMbk->XINS    , InstanceMbk->YINS,
      ModelMbk->XAB1       , ModelMbk->YAB1,
      ModelMbk->XAB2       , ModelMbk->YAB2,
      InstanceMbk->TRANSF
    );

    xyflat
    ( &X2                  , &Y2, 
      SegX2                , SegY2,
      InstanceMbk->XINS    , InstanceMbk->YINS,
      ModelMbk->XAB1       , ModelMbk->YAB1,
      ModelMbk->XAB2       , ModelMbk->YAB2,
      InstanceMbk->TRANSF
    );

    if ( ( InstanceMbk->TRANSF == NOSYM ) ||
         ( InstanceMbk->TRANSF == SYMXY ) ||
         ( InstanceMbk->TRANSF == ROT_P ) ||
         ( InstanceMbk->TRANSF == ROT_M ) )
    {
      if ( X1 < X2 ) Type = RIGHT;
      if ( Y1 < Y2 ) Type = UP;
      if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; Type = LEFT; }
      if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; Type = DOWN; }
    }
    else
    {
      if ( X1 < X2 ) Type = LEFT;
      if ( Y1 < Y2 ) Type = DOWN;
      if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; Type = RIGHT; }
      if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; Type = UP;    }
    }

    X1 = X1             * RDS_LAMBDA / SCALE_X;
    X2 = X2             * RDS_LAMBDA / SCALE_X;
    Y1 = Y1             * RDS_LAMBDA / SCALE_X;
    Y2 = Y2             * RDS_LAMBDA / SCALE_X;
    WS = Segment->WIDTH * RDS_LAMBDA / SCALE_X;

    Index     = 0;
    LayerRds  = GET_SEGMENT_LAYER( LayerMbk, Index );

    while ( LayerRds != RDS_SEGMENT_EMPTY )
    {
      TRANS  = GET_SEGMENT_TRANS ( LayerMbk, Index );
      DLR    = GET_SEGMENT_DLR   ( LayerMbk, Index );
      DWR    = GET_SEGMENT_DWR   ( LayerMbk, Index );
      OFFSET = GET_SEGMENT_OFFSET( LayerMbk, Index );
      USE    = GET_SEGMENT_USE   ( LayerMbk, Index );

      if ( ( ( USE == RDS_USE_ALL     )               ) ||
           ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
           ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
      {
        if ( TRANS == RDS_TRANS_VW )
        {
          switch ( Type )
          {
            case LEFT   : X1R = X1 - DLR;
                          Y1R = Y1 - ( ( WS + DWR ) >> 1 ) + OFFSET;
                          X2R = X2 + DLR;
                          Y2R = Y2 + ( ( WS + DWR ) >> 1 ) + OFFSET;

                        break;

            case RIGHT  : X1R = X1 - DLR;
                          Y1R = Y1 - ( ( WS + DWR ) >> 1 ) - OFFSET;
                          X2R = X2 + DLR;
                          Y2R = Y2 + ( ( WS + DWR ) >> 1 ) - OFFSET;

                        break;

            case DOWN   : X1R = X1 - ( ( WS + DWR ) >> 1 ) - OFFSET;
                          Y1R = Y1 - DLR;
                          X2R = X2 + ( ( WS + DWR ) >> 1 ) - OFFSET;
                          Y2R = Y2 + DLR;

                        break;

            case UP     : X1R = X1 - ( ( WS + DWR ) >> 1 ) + OFFSET;
                          Y1R = Y1 - DLR;
                          X2R = X2 + ( ( WS + DWR ) >> 1 ) + OFFSET;
                          Y2R = Y2 + DLR;

                        break;
          }
        }
        else
        if ( TRANS == RDS_TRANS_LCW )
        {
          switch ( Type )
          {
            case RIGHT  : X1R = X1 - DLR;
                          Y1R = Y1 + (WS >> 1) + OFFSET;
                          X2R = X2 + DLR;
                          Y2R = Y2 + (WS >> 1) + DWR + OFFSET;

                        break;

            case LEFT   : X1R = X1 - DLR;
                          Y1R = Y1 - (WS >> 1) - DWR - OFFSET;
                          X2R = X2 + DLR;
                          Y2R = Y2 - (WS >> 1) - OFFSET;

                        break;

            case UP     : X1R = X1 - (WS >> 1) - DWR - OFFSET;
                          Y1R = Y1 - DLR;
                          X2R = X2 - (WS >> 1) - OFFSET;
                          Y2R = Y2 + DLR;

                        break;

            case DOWN   : X1R = X1 + (WS >> 1) + OFFSET;
                          Y1R = Y1 - DLR;
                          X2R = X2 + (WS >> 1) + DWR + OFFSET;
                          Y2R = Y2 + DLR;

                        break;
          }
        }
        else
        if ( TRANS == RDS_TRANS_RCW )
        {
          switch ( Type )
          {
            case RIGHT  : X1R = X1 - DLR;
                          Y1R = Y1 - (WS >> 1) - DWR - OFFSET;
                          X2R = X2 + DLR;
                          Y2R = Y2 - (WS >> 1) - OFFSET;

                        break;

            case LEFT   : X1R = X1 - DLR;
                          Y1R = Y1 + (WS >> 1) + OFFSET;
                          X2R = X2 + DLR;
                          Y2R = Y2 + (WS >> 1) + DWR + OFFSET;

                        break;

            case UP     : X1R = X1 + (WS >> 1) + OFFSET;
                          Y1R = Y1 - DLR;
                          X2R = X2 + (WS >> 1) + DWR + OFFSET;
                          Y2R = Y2 + DLR;

                        break;

            case DOWN   : X1R = X1 - (WS >> 1) - DWR - OFFSET;
                          Y1R = Y1 - DLR;
                          X2R = X2 - (WS >> 1) - OFFSET;
                          Y2R = Y2 + DLR;

                        break;
          }
        }

        X1R = RfmRoundLow ( X1R );
        Y1R = RfmRoundLow ( Y1R );
        X2R = RfmRoundHigh( X2R );
        Y2R = RfmRoundHigh( Y2R );

        Rectangle = addrdsinsrec( InstanceRds, Segment->NAME, 
                                  LayerRds, X1R, Y1R,
                                  X2R - X1R, Y2R - Y1R );

        SetRdsSegment( Rectangle );

        if ( SegConMbk ) SetRdsConInter( Rectangle );

        if ( First == (rdsrec_list *)NULL )
        {
          First = Rectangle;
          Link  = Rectangle;
        }
        else
        {
          Link->USER = (void *)Rectangle;
          Link       = Rectangle;
        }
      }

      Index = Index + 1;

      LayerRds = GET_SEGMENT_LAYER( LayerMbk, Index );
    }
  }

  if ( First != (rdsrec_list *)NULL ) Link->USER = (void *)First;

  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|               MBK Instance Via To RDS Rectangle             |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *insviambkrds( InstanceMbk, ModelMbk, InstanceRds, Lynx )

   phins_list  *InstanceMbk;
   phfig_list  *ModelMbk;
   rdsins_list *InstanceRds;
   char         Lynx;
{
  phvia_list   *Via;
  rdsrec_list  *Rectangle;
  rdsrec_list  *First;
  rdsrec_list  *Link;

  long          Xvia;
  long          Yvia;
  long          WSX;
  long          WSY;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;

  long          BX1R;
  long          BY1R;
  long          BX2R;
  long          BY2R;

  long          VX1R;
  long          VY1R;
  long          VX2R;
  long          VY2R;

  long          SIDE;
  long          SIZE;
  long          STEP;
  long          SIDE_STEP;
  long          DWR;
  long          OVERLAP;
  long          MIN_DWR;
  long          MAX_OVER;
  char          USE; 

  int           ViaMbk;
  int           LayerRds;
  int           Index;

  First   = (rdsrec_list *)NULL;
  Link    = (rdsrec_list *)NULL;
  DWR     = 0;
  OVERLAP = 0;

  for ( Via  = ModelMbk->PHVIA;
        Via != (phvia_list *)NULL;
        Via  = Via->NEXT )
  {
    ViaMbk = Via->TYPE;

    VX1R = Via->XVIA - (Via->DX >> 1);
    VY1R = Via->YVIA - (Via->DY >> 1);
    VX2R = VX1R + Via->DX;
    VY2R = VY1R + Via->DY;

    xyflat
    ( &X1R               , &Y1R,
      VX1R               , VY1R,     
      InstanceMbk->XINS  , InstanceMbk->YINS,
      ModelMbk->XAB1     , ModelMbk->YAB1,
      ModelMbk->XAB2     , ModelMbk->YAB2,
      InstanceMbk->TRANSF
    );

    xyflat
    ( &X2R               , &Y2R,
      VX2R               , VY2R,     
      InstanceMbk->XINS  , InstanceMbk->YINS,
      ModelMbk->XAB1     , ModelMbk->YAB1,
      ModelMbk->XAB2     , ModelMbk->YAB2,
      InstanceMbk->TRANSF
    );

    if ( X1R > X2R )
    {
      Xvia = X1R;
      X1R  = X2R;
      X2R  = Xvia;
    }

    if ( Y1R > Y2R )
    {
      Yvia = Y1R;
      Y1R  = Y2R;
      Y2R  = Yvia;
    }

    Xvia = ( ( X1R + X2R ) >> 1 ) * RDS_LAMBDA / SCALE_X;
    Yvia = ( ( Y1R + Y2R ) >> 1 ) * RDS_LAMBDA / SCALE_X;

    Rectangle = (rdsrec_list *)NULL;

    if ( ( Via->DX != 0 ) ||
         ( Via->DY != 0 ) )
    {
      WSX  = ( X2R - X1R ) * RDS_LAMBDA / SCALE_X;
      WSY  = ( Y2R - Y1R ) * RDS_LAMBDA / SCALE_X;
      
      LayerRds = GET_BIGVIA_HOLE_LAYER( ViaMbk, 0 );
  
      if ( LayerRds != RDS_BIGVIA_HOLE_EMPTY )
      {
        X1R = Y1R = X2R = Y2R = 0;
/*
** Metal rectangles
*/
        Index    =  0;
        MIN_DWR  = -1;
        MAX_OVER =  0;
        LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
      
        while ( LayerRds != RDS_BIGVIA_METAL_EMPTY )
        {
          DWR     = GET_BIGVIA_METAL_DWR( ViaMbk, Index );
          OVERLAP = GET_BIGVIA_METAL_OVERLAP( ViaMbk, Index );
          USE     = GET_BIGVIA_METAL_USE( ViaMbk, Index );
      
          if ( ( ( USE == RDS_USE_ALL     )               ) ||
               ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
               ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
          {
            if ( ( MIN_DWR == -1  ) ||
                 ( MIN_DWR >  DWR ) ) MIN_DWR = DWR;
      
            if ( MAX_OVER < OVERLAP ) MAX_OVER = OVERLAP;
          }
      
          Index = Index + 1;
      
          LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
        }
      
        if ( MIN_DWR < 0 ) MIN_DWR = 0;
/*
** Hole rectangles
*/
        Index    = 0;
        LayerRds = GET_BIGVIA_HOLE_LAYER( ViaMbk, Index );
      
        while ( LayerRds != RDS_BIGVIA_HOLE_EMPTY )
        {
          SIDE = GET_BIGVIA_HOLE_SIDE( ViaMbk, Index );
          STEP = GET_BIGVIA_HOLE_STEP( ViaMbk, Index );
          USE  = GET_BIGVIA_HOLE_USE(  ViaMbk, Index );
  
          SIDE_STEP = SIDE + STEP;
      
          if ( ( ( USE == RDS_USE_ALL     )               ) ||
               ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
               ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
          {
            if ( SIDE_STEP == 0 ) break;
  
            X1R = Xvia + OVERLAP - ( ( WSX + DWR ) >> 1 );
            Y1R = Yvia + OVERLAP - ( ( WSY + DWR ) >> 1 );
            X2R = Xvia - OVERLAP + ( ( WSX + DWR ) >> 1 );
            Y2R = Yvia - OVERLAP + ( ( WSY + DWR ) >> 1 );
  
            X1R = X1R + (SIDE >> 1);
            Y1R = Y1R + (SIDE >> 1);
        
            X1R = RfmRoundLow ( X1R );
            X2R = RfmRoundHigh( X2R );
            Y1R = RfmRoundLow ( Y1R );
            Y2R = RfmRoundHigh( Y2R );
  
            if ( X1R >= 0 ) X1R = ( (X1R + SIDE_STEP - 1) / SIDE_STEP ) * SIDE_STEP;
            else            X1R = ( X1R / SIDE_STEP ) * SIDE_STEP;
  
            if ( Y1R >= 0 ) Y1R = ( (Y1R + SIDE_STEP - 1) / SIDE_STEP ) * SIDE_STEP;
            else            Y1R = ( Y1R / SIDE_STEP ) * SIDE_STEP;
  
            BX1R = X1R;
  
            while ( BX1R <= X2R )
            {
              BX1R = BX1R - (SIDE >> 1);
              BX2R = BX1R + SIDE;
              VX1R = RfmRoundLow ( BX1R );
              VX2R = RfmRoundHigh( BX2R );
  
              if ( VX2R > X2R ) break;
      
              BY1R = Y1R;
      
              while ( BY1R <= Y2R )
              {
                BY1R = BY1R - (SIDE >> 1);
                BY2R = BY1R + SIDE;
                VY1R = RfmRoundLow ( BY1R );
                VY2R = RfmRoundHigh( BY2R );
              
                if ( VY2R > Y2R ) break;
      
                Rectangle = addrdsinsrec( InstanceRds, (char *)0,
                                          LayerRds, VX1R, VY1R,
                                          VX2R - VX1R, VY2R - VY1R );
      
                SetRdsVia( Rectangle );
      
                if ( First == (rdsrec_list *)NULL )
                {
                  First = Rectangle;
                  Link  = Rectangle;
                }
                else
                {
                  Link->USER = (void *)Rectangle;
                  Link       = Rectangle;
                }
      
                BY1R = BY2R + STEP + (SIDE >> 1);
              }
      
              BX1R = BX2R + STEP + (SIDE >> 1);
            }
          }
      
          Index = Index + 1;
      
          LayerRds = GET_BIGVIA_HOLE_LAYER( ViaMbk, Index );
        }
/*
** Metal rectangles
*/
        if ( Rectangle != (rdsrec_list *)0 )
        {
          Index    =  0;
          LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
        
          while ( LayerRds != RDS_BIGVIA_METAL_EMPTY )
          {
            DWR = GET_BIGVIA_METAL_DWR( ViaMbk, Index );
            USE = GET_BIGVIA_METAL_USE( ViaMbk, Index );
        
            if ( ( ( USE == RDS_USE_ALL     )               ) ||
                 ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
                 ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
            {
              X1R = Xvia - ( ( WSX + DWR ) >> 1 );
              Y1R = Yvia - ( ( WSY + DWR ) >> 1 );
              X2R = Xvia + ( ( WSX + DWR ) >> 1 );
              Y2R = Yvia + ( ( WSY + DWR ) >> 1 );
        
              X1R = RfmRoundLow ( X1R );
              Y1R = RfmRoundLow ( Y1R );
              X2R = RfmRoundHigh( X2R );
              Y2R = RfmRoundHigh( Y2R );
        
              Rectangle = addrdsinsrec( InstanceRds, Via->NAME,
                                        LayerRds, X1R, Y1R,
                                        X2R - X1R, Y2R - Y1R );
        
              SetRdsVia( Rectangle );
        
              if ( First == (rdsrec_list *)NULL )
              {
                First = Rectangle;
                Link  = Rectangle;
              }
              else
              {
                Link->USER = (void *)Rectangle;
                Link       = Rectangle;
              }
            }
        
            Index = Index + 1;
        
            LayerRds = GET_BIGVIA_METAL_LAYER( ViaMbk, Index );
          }
        }
      }
      else
      {
        LayerRds  = GET_TURNVIA_LAYER( ViaMbk, 0 );
  
        if ( LayerRds != RDS_TURNVIA_EMPTY )
        {
          X1R = Y1R = X2R = Y2R = 0;
    
          Index   = 0;
        
          LayerRds = GET_TURNVIA_LAYER( ViaMbk, Index );
        
          while ( LayerRds != RDS_TURNVIA_EMPTY )
          {
            DWR    = GET_TURNVIA_DWR( ViaMbk, Index );
            USE    = GET_TURNVIA_USE( ViaMbk, Index );
        
            if ( ( ( USE == RDS_USE_ALL     )               ) ||
                 ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
                 ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
            {
              X1R = Xvia - ( ( WSX + DWR ) >> 1 );
              Y1R = Yvia - ( ( WSY + DWR ) >> 1 );
              X2R = Xvia + ( ( WSX + DWR ) >> 1 );
              Y2R = Yvia + ( ( WSY + DWR ) >> 1 );
        
              X1R = RfmRoundLow ( X1R );
              Y1R = RfmRoundLow ( Y1R );
              X2R = RfmRoundHigh( X2R );
              Y2R = RfmRoundHigh( Y2R );

              Rectangle = addrdsinsrec( InstanceRds, Via->NAME,
                                        LayerRds, X1R, Y1R,
                                        X2R - X1R, Y2R - Y1R );
        
              SetRdsVia( Rectangle );
        
              if ( First == (rdsrec_list *)NULL )
              {
                First = Rectangle;
                Link  = Rectangle;
              }
              else
              {
                Link->USER = (void *)Rectangle;
                Link       = Rectangle;
              }
            }
        
            Index = Index + 1;
        
            LayerRds = GET_TURNVIA_LAYER( ViaMbk, Index );
          }
        }
      }
    }

    if ( Rectangle == (rdsrec_list *)0 )
    {
      Index     = 0;
      LayerRds  = GET_VIA_LAYER( ViaMbk, Index );
  
      while ( LayerRds != RDS_VIA_EMPTY )
      {
        USE = GET_VIA_USE( ViaMbk, Index );
  
        if ( ( ( USE == RDS_USE_ALL     )               ) ||
             ( ( USE == RDS_USE_EXTRACT ) && (   Lynx ) ) ||
             ( ( USE == RDS_USE_DRC     ) && ( ! Lynx ) ) )
        {
          SIZE = GET_VIA_SIZE( ViaMbk, Index );
  
          X1R = Xvia - ( SIZE >> 1 );
          Y1R = Yvia - ( SIZE >> 1 );
          X2R = Xvia + ( SIZE >> 1 );
          Y2R = Yvia + ( SIZE >> 1 );
  
          X1R = RfmRoundLow ( X1R );
          Y1R = RfmRoundLow ( Y1R );
          X2R = RfmRoundHigh( X2R );
          Y2R = RfmRoundHigh( Y2R );
  
          Rectangle = addrdsinsrec( InstanceRds, Via->NAME,
                                    LayerRds, X1R, Y1R,
                                    X2R - X1R, Y2R - Y1R );
  
          SetRdsVia( Rectangle );
  
          if ( First == (rdsrec_list *)NULL )
          {
            First = Rectangle;
            Link  = Rectangle;
          }
          else
          {
            Link->USER = (void *)Rectangle;
            Link       = Rectangle;
          }
        }
  
        Index = Index + 1;
  
        LayerRds = GET_VIA_LAYER( ViaMbk, Index );
      }
    }
  }

  if ( First != (rdsrec_list *)NULL ) Link->USER = (void *)First;
 
  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|          MBK Instance Reference To RDS Rectangle            |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *insrefmbkrds( InstanceMbk, ModelMbk, InstanceRds )

   phins_list  *InstanceMbk;
   phfig_list  *ModelMbk;
   rdsins_list *InstanceRds;

{
  phref_list   *Reference;
  rdsrec_list  *Rectangle;
  rdsrec_list  *First;
  rdsrec_list  *Link;

  long          Xref;
  long          Yref;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;

  long          SIZE;

  int           RefMbk;
  int           LayerRds;
  int           Index;

  if ( RfmRefRef == (char *)NULL ) 
  {
    RfmRefRef = namealloc( "ref_ref" );
  }

  Rectangle = (rdsrec_list *)NULL;
  First     = (rdsrec_list *)NULL;
  Link      = (rdsrec_list *)NULL;

  for ( Reference  = ModelMbk->PHREF;
        Reference != (phref_list *)NULL;
        Reference  = Reference->NEXT )
  { 
    xyflat
    ( &Xref              , &Yref,
      Reference->XREF    , Reference->YREF,
      InstanceMbk->XINS  , InstanceMbk->YINS,
      ModelMbk->XAB1     , ModelMbk->YAB1,
      ModelMbk->XAB2     , ModelMbk->YAB2,
      InstanceMbk->TRANSF
    );

    Xref = Xref * RDS_LAMBDA / SCALE_X;
    Yref = Yref * RDS_LAMBDA / SCALE_X;

    RefMbk    = (Reference->FIGNAME == RfmRefRef)? MBK_REF_REF:MBK_REF_CON;
    Index     = 0;
    LayerRds  = GET_REFERENCE_LAYER( RefMbk, Index );

    while ( LayerRds != RDS_REFERENCE_EMPTY )
    {
      SIZE = GET_REFERENCE_SIZE( RefMbk, Index );

      X1R = Xref - ( SIZE >> 1 );
      Y1R = Yref - ( SIZE >> 1 );
      X2R = Xref + ( SIZE >> 1 );
      Y2R = Yref + ( SIZE >> 1 );

      X1R = RfmRoundLow ( X1R );
      Y1R = RfmRoundLow ( Y1R );
      X2R = RfmRoundHigh( X2R );
      Y2R = RfmRoundHigh( Y2R );

      Rectangle = addrdsinsrec( InstanceRds, Reference->NAME,
                                LayerRds, X1R, Y1R,
                                X2R - X1R, Y2R - Y1R );

      SetRdsReference( Rectangle );

      if ( RefMbk == MBK_REF_REF )
      {
        SetRdsRefRef( Rectangle );
      }
      else
      {   
        SetRdsRefCon( Rectangle );
      }

      if ( First == (rdsrec_list *)NULL )
      {
        First = Rectangle;
        Link  = Rectangle;
      }
      else
      {   
        Link->USER = (void *)Rectangle;
        Link       = Rectangle;
      }

      Index = Index + 1;

      LayerRds  = GET_REFERENCE_LAYER( RefMbk, Index );
    }
  }

  if ( First != (rdsrec_list *)NULL ) Link->USER = (void *)First;

  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|          MBK Instance Connector To RDS Rectangle            |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *insconmbkrds( InstanceMbk, ModelMbk, InstanceRds )

   phins_list  *InstanceMbk;
   phfig_list  *ModelMbk;
   rdsins_list *InstanceRds;
 {
  phcon_list   *Connector;
  rdsrec_list  *Rectangle;
  rdsrec_list  *First;
  rdsrec_list  *Link;

  long          Xcon;
  long          Ycon;

  long          X1R;
  long          Y1R;
  long          X2R;
  long          Y2R;
  long          WS;

  long          DWR;
  long          DER;

  int           Orient;
  int           LayerMbk;
  int           LayerRds;

  Rectangle = (rdsrec_list *)NULL;
  First     = (rdsrec_list *)NULL;
  Link      = (rdsrec_list *)NULL;

  for ( Connector  = ModelMbk->PHCON;
        Connector != (phcon_list *)NULL;
        Connector  = Connector->NEXT )
  {
    xyflat
    ( &Xcon               , &Ycon,
      Connector->XCON     , Connector->YCON,
      InstanceMbk->XINS   , InstanceMbk->YINS,
      ModelMbk->XAB1      , ModelMbk->YAB1,
      ModelMbk->XAB2      , ModelMbk->YAB2,
      InstanceMbk->TRANSF
    );
     
    Xcon = Xcon             * RDS_LAMBDA / SCALE_X;
    Ycon = Ycon             * RDS_LAMBDA / SCALE_X;
    WS   = Connector->WIDTH * RDS_LAMBDA / SCALE_X;

    LayerMbk  = Connector->LAYER;
    LayerRds  = GET_CONNECTOR_LAYER( LayerMbk );

    if ( LayerRds != RDS_REFERENCE_EMPTY )
    {
      DWR    = GET_CONNECTOR_DWR( LayerMbk );
      DER    = GET_CONNECTOR_DER( LayerMbk );
      Orient = RfmNewConnectorOrient[ (int)InstanceMbk->TRANSF ];
 
      if ( ( Connector->ORIENT == EAST ) ||
           ( Connector->ORIENT == WEST ) )
      {
        Orient = ! Orient;
      }

      if ( Orient != 1 )  /* EAST AND WEST */
      {
        X1R = Xcon - DER;
        Y1R = Ycon - ( ( WS + DWR ) >> 1 );
        X2R = Xcon + DER;
        Y2R = Ycon + ( ( WS + DWR ) >> 1 );
      }
      else
      {
        X1R = Xcon - ( ( WS + DWR ) >> 1 );
        Y1R = Ycon - DER;
        X2R = Xcon + ( ( WS + DWR ) >> 1 );
        Y2R = Ycon + DER; 
      }

      X1R = RfmRoundLow ( X1R );
      Y1R = RfmRoundLow ( Y1R );
      X2R = RfmRoundHigh( X2R );
      Y2R = RfmRoundHigh( Y2R );
 
      Rectangle = addrdsinsrec( InstanceRds, Connector->NAME,
                                LayerRds, X1R, Y1R, 
                                X2R - X1R, Y2R - Y1R );
 
      SetRdsConnector( Rectangle );
      SetRdsConInter( Rectangle );

      if ( First == (rdsrec_list *)NULL )
      {
        First = Rectangle;
        Link  = Rectangle;
      }
      else
      {   
        Link->USER = (void *)Rectangle;
        Link       = Rectangle;
      }
    }
  }  

  if ( First != (rdsrec_list *)NULL ) Link->USER = (void *)First;

  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|                MBK Instance To RDS Instance                 |
|                                                             |
\------------------------------------------------------------*/

rdsins_list *insmbkrds( FigureRds, InstanceMbk, Mode, Lynx )

   rdsfig_list *FigureRds;
   phins_list  *InstanceMbk;
   char         Mode;
   char         Lynx;
{
  rdsins_list *InstanceRds;
  rdsrec_list *Rectangle;
  phfig_list  *ModelMbk;

  long         Xins;
  long         Yins;
  long         Xab1;
  long         Xab2;
  long         Yab1;
  long         Yab2;
  long         Swap;

  ModelMbk = getphfig( InstanceMbk->FIGNAME, Mode );

  xyflat
  ( &Xins              , &Yins,
    0                  , 0,
    InstanceMbk->XINS  , InstanceMbk->YINS,
    ModelMbk->XAB1     , ModelMbk->YAB1,
    ModelMbk->XAB2     , ModelMbk->YAB2,
    InstanceMbk->TRANSF
  );

  xyflat
  ( &Xab1              , &Yab1,
    ModelMbk->XAB1     , ModelMbk->YAB1,
    InstanceMbk->XINS  , InstanceMbk->YINS,
    ModelMbk->XAB1     , ModelMbk->YAB1,
    ModelMbk->XAB2     , ModelMbk->YAB2,
    InstanceMbk->TRANSF
  );
 
  xyflat
  ( &Xab2              , &Yab2,
    ModelMbk->XAB2     , ModelMbk->YAB2,
    InstanceMbk->XINS  , InstanceMbk->YINS,
    ModelMbk->XAB1     , ModelMbk->YAB1,
    ModelMbk->XAB2     , ModelMbk->YAB2,
    InstanceMbk->TRANSF
  );

  Xins = RfmRoundLow( Xins * RDS_LAMBDA / SCALE_X );
  Yins = RfmRoundLow( Yins * RDS_LAMBDA / SCALE_X );
  Xab1 = RfmRoundLow( Xab1 * RDS_LAMBDA / SCALE_X );
  Yab1 = RfmRoundLow( Yab1 * RDS_LAMBDA / SCALE_X );
  Xab2 = RfmRoundHigh( Xab2 * RDS_LAMBDA / SCALE_X );
  Yab2 = RfmRoundHigh( Yab2 * RDS_LAMBDA / SCALE_X );
 
  InstanceRds = addrdsins( FigureRds, 
                           InstanceMbk->FIGNAME, InstanceMbk->INSNAME,
                           InstanceMbk->TRANSF,
                           Xins, Yins );

  if ( Xab1 > Xab2 ) { Swap = Xab1; Xab1 = Xab2; Xab2 = Swap; }
  if ( Yab1 > Yab2 ) { Swap = Yab1; Yab1 = Yab2; Yab2 = Swap; }
 
  Rectangle = addrdsinsrec( InstanceRds, InstanceMbk->INSNAME,
                            RDS_ABOX, Xab1, Yab1, Xab2 - Xab1, Yab2 - Yab1 );

  SetRdsInstance( Rectangle );

  Rectangle->USER = (void *)Rectangle;

  inssegmbkrds( InstanceMbk, ModelMbk, InstanceRds, Lynx );
  insconmbkrds( InstanceMbk, ModelMbk, InstanceRds       );
  insrefmbkrds( InstanceMbk, ModelMbk, InstanceRds       );
  insviambkrds( InstanceMbk, ModelMbk, InstanceRds, Lynx );
 
  return( InstanceRds );
}

/*------------------------------------------------------------\
|                                                             |
|                      MBK Abox to RDS Abox                   |
|                                                             |
\------------------------------------------------------------*/

 rdsrec_list *aboxmbkrds( FigureMbk, FigureRds )
 
   phfig_list  *FigureMbk;
   rdsfig_list *FigureRds;
{
  rdsrec_list *Rectangle;
  long         XAB1;
  long         XAB2;
  long         YAB1;
  long         YAB2;

  XAB1 = RfmRoundLow( FigureMbk->XAB1 * RDS_LAMBDA / SCALE_X );
  XAB2 = RfmRoundLow( FigureMbk->XAB2 * RDS_LAMBDA / SCALE_X );
  YAB1 = RfmRoundHigh( FigureMbk->YAB1 * RDS_LAMBDA / SCALE_X );
  YAB2 = RfmRoundHigh( FigureMbk->YAB2 * RDS_LAMBDA / SCALE_X );

  if ( ( XAB1 != 0 ) || ( XAB2 != 0 ) ||
       ( YAB1 != 0 ) || ( YAB2 != 0 ) )
  {
    Rectangle = 
 
      addrdsfigrec( FigureRds, FigureMbk->NAME, 
                    RDS_ABOX, 
                    XAB1, YAB1, XAB2 - XAB1, YAB2 - YAB1 );

    SetRdsFigure( Rectangle );

    Rectangle->USER = (void *)Rectangle;

    return( Rectangle );
  }

  return( (rdsrec_list *)NULL );
}   

/*------------------------------------------------------------\
|                                                             |
|                    MBK Figure To RDS Figure                 |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *figmbkrds( FigureMbk, Size, Lynx )

   phfig_list   *FigureMbk;
   unsigned int  Size;
   char          Lynx;
{
  rdsfig_list *FigureRds;
  void        *Pointer;

  FigureRds = addrdsfig( FigureMbk->NAME, Size );

  aboxmbkrds( FigureMbk, FigureRds );

  for ( Pointer  = (void *)(FigureMbk->PHSEG);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phseg_list *)Pointer)->NEXT))
  {
    segmbkrds( FigureRds, (phseg_list *)Pointer, Lynx );
  }

  for ( Pointer  = (void *)(FigureMbk->PHCON);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phcon_list *)Pointer)->NEXT))
  {
    conmbkrds( FigureRds, (phcon_list *)Pointer);
  }

  for ( Pointer  = (void *)(FigureMbk->PHREF);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phref_list *)Pointer)->NEXT))
  {
    refmbkrds( FigureRds, (phref_list *)Pointer);
  }

  for ( Pointer  = (void *)(FigureMbk->PHVIA);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phvia_list *)Pointer)->NEXT))
  {
    viambkrds( FigureRds, (phvia_list *)Pointer, Lynx );
  }
 
  for ( Pointer  = (void *)(FigureMbk->PHINS);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phins_list *)Pointer)->NEXT))
  {
    insmbkrds( FigureRds, (phins_list *)Pointer, 'P', Lynx );
  }

  return( FigureRds );
}

/*------------------------------------------------------------\
|                                                             |
|                       Model Mbk To Rds                      |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *modelmbkrds( Figure, Lynx )

  rdsfig_list *Figure;
  char         Lynx;
{
  rdsfig_list  *HeadFig;
  rdsfig_list  *StopFig;
  rdsfig_list  *ScanFig;
  rdsfig_list **Previous;
  phfig_list   *FigureMbk;
  rdsins_list  *Instance;


  ScanFig  = HEAD_RDSFIG;
  Previous = &HEAD_RDSFIG;

  while ( ScanFig != (rdsfig_list *)NULL )
  {
    if ( ScanFig == Figure ) break;

    Previous = &ScanFig->NEXT;
    ScanFig  = ScanFig->NEXT;
  }

  if ( ScanFig == (rdsfig_list *)NULL ) 
  {
    return( (rdsfig_list *)NULL );
  }

  *Previous     = Figure->NEXT;
  Figure->NEXT  = HEAD_RDSFIG;
  HEAD_RDSFIG   = Figure;

  StopFig = Figure->NEXT;

  do
  {
    HeadFig = HEAD_RDSFIG;

    for ( ScanFig  = HeadFig;
          ScanFig != StopFig;
          ScanFig  = ScanFig->NEXT )
    {
      for ( Instance  = ScanFig->INSTANCE;
            Instance != (rdsins_list *)NULL;
            Instance  = Instance->NEXT )
      {
        if ( searchrdsfig( Instance->FIGNAME ) == (rdsfig_list *)NULL )
        {
          FigureMbk = getphfig( Instance->FIGNAME, 'A' );
          figmbkrds( FigureMbk, Figure->SIZE, Lynx );
        }
      }
    }

    StopFig = HeadFig;
  }
  while ( HeadFig != HEAD_RDSFIG );

  return( HEAD_RDSFIG );
}
