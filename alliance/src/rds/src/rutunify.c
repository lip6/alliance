/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDS                            |
|                                                             |
| File    :                  rutunify.c                       |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  29.03.95                         |
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

# include "rutunify.h"
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

  static char RutUnifyTable[ 16 ][ 3 ] =

  {
    /* R Right, L Left, U Up, B Bottom     Mask    Equal     */

    { RUT_UNIFY_DELETE , 0,  0 },   /*  0                    */
    { RUT_UNIFY_STRETCH, 0, 10 },   /*  1     B    L R       */
    { RUT_UNIFY_STRETCH, 0,  5 },   /*  2    R      U B      */
    { RUT_UNIFY_CROSS  , 0, 12 },   /*  3    RB    L R       */
    { RUT_UNIFY_STRETCH, 0, 10 },   /*  4   U      L R       */
    { RUT_UNIFY_NOTHING, 0, 10 },   /*  5   U B    L R       */
    { RUT_UNIFY_CROSS  , 0,  9 },   /*  6   UR     L  B      */
    { RUT_UNIFY_STRETCH, 1,  8 },   /*  7   URB    L         */
    { RUT_UNIFY_STRETCH, 0,  5 },   /*  8  L        U B      */
    { RUT_UNIFY_CROSS  , 0,  6 },   /*  9  L  B     UR       */
    { RUT_UNIFY_NOTHING, 0,  5 },   /* 10  L R      U B      */
    { RUT_UNIFY_STRETCH, 1,  4 },   /* 11  L RB     U        */
    { RUT_UNIFY_CROSS  , 0,  3 },   /* 12  LU        RB      */
    { RUT_UNIFY_STRETCH, 1,  2 },   /* 13  LU B      R       */
    { RUT_UNIFY_STRETCH, 1,  1 },   /* 14  LUR        B      */
    { RUT_UNIFY_DELETE , 1,  0 },   /* 15  LURB              */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        unifyrdsfig                          |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *unifyrdsfig( FigureRds, HeadEqui )

  rdsfig_list *FigureRds;
  rdsrec_list *HeadEqui;
{
  rdsrec_list  **PrevBeginEqui;
  rdsrec_list   *BeginEqui;
  rdsrec_list   *ScanBeginEqui;
  rdsrec_list   *ScanEqui;
  rdsrec_list   *NewRec;
  rdsrec_list   *ScanRec;
  rdsrec_list  **PrevRec;
  int            Mask;
  int            MaskEqual;
  int            UnifyMask;
  int            Layer;
  int            Index;
  int            BeginModify;
  int            ScanModify;
  int            EndScanEqui;
  int            EndBeginEqui;
  int            ModeCreate;
  long           X1R;
  long           Y1R;
  long           X2R;
  long           Y2R;
  long           X1r;
  long           Y1r;
  long           X2r;
  long           Y2r;
  long           Xn [ 2 ];
  long           Yn [ 2 ];
  long           DXn[ 2 ];
  long           DYn[ 2 ];

  rdsbegin();

  PrevBeginEqui = &HeadEqui;
  BeginEqui     = HeadEqui;
  ModeCreate    = 1;

  while ( BeginEqui != (rdsrec_list *)NULL )
  {
    ScanBeginEqui = BeginEqui;
    EndBeginEqui  = 0;

    while ( ! EndBeginEqui )
    {
      if ( ! IsRdsDeleted( ScanBeginEqui ) )
      {
        Layer = GetRdsLayer( ScanBeginEqui );

        X1R = ScanBeginEqui->X;
        Y1R = ScanBeginEqui->Y;
        X2R = X1R + ScanBeginEqui->DX;
        Y2R = Y1R + ScanBeginEqui->DY;

        ScanEqui    = BeginEqui;
        EndScanEqui = 0;
        BeginModify = 0;

        while ( ( ! EndScanEqui                   ) &&
                ( ! IsRdsDeleted( ScanBeginEqui ) ) )
        {
          if ( ( ScanBeginEqui != ScanEqui  ) &&
               ( ! IsRdsDeleted( ScanEqui ) ) )
          {
            X1r = ScanEqui->X;
            Y1r = ScanEqui->Y;
            X2r = X1r + ScanEqui->DX;
            Y2r = Y1r + ScanEqui->DY;

            if ( ( X1r <= X2R ) &&
                 ( Y1r <= Y2R ) &&
                 ( X2r >= X1R ) &&
                 ( Y2r >= Y1R ) )
            {
              ScanModify = 0;

              Mask = 0;

              if ( X1r < X1R ) Mask |= RUT_UNIFY_LEFT;
              if ( Y1r < Y1R ) Mask |= RUT_UNIFY_BOTTOM;
              if ( X2r > X2R ) Mask |= RUT_UNIFY_RIGHT;
              if ( Y2r > Y2R ) Mask |= RUT_UNIFY_UP;

              MaskEqual = 0;
  
              if ( X1r == X1R ) MaskEqual |= RUT_UNIFY_LEFT;
              if ( Y1r == Y1R ) MaskEqual |= RUT_UNIFY_BOTTOM;
              if ( X2r == X2R ) MaskEqual |= RUT_UNIFY_RIGHT;
              if ( Y2r == Y2R ) MaskEqual |= RUT_UNIFY_UP;

              UnifyMask = RutUnifyTable[ Mask ][ 2 ];
  
              switch ( RutUnifyTable[ Mask ][ 0 ] )
              {
                case RUT_UNIFY_STRETCH :
  
                  if ( RutUnifyTable[ Mask ][ 1 ] )
                  {
                    if ( ( UnifyMask & MaskEqual ) == UnifyMask )
                    {
                      SetRdsDeleted( ScanBeginEqui );
                    }
                    else
                    {
                      switch ( Mask )
                      {
                        case 13 : X1R = X1r;
                        break;

                        case 11 : Y1R = Y1r;
                        break;

                        case 7  : X2R = X2r;
                        break;

                        case 14 : Y2R = Y2r;
                        break;
                      }

                      BeginModify = 1;
                    }
                  }
                  else
                  {
                    if ( ( UnifyMask & MaskEqual ) == UnifyMask )
                    {
                      SetRdsDeleted( ScanBeginEqui );
                    }

                    switch ( Mask )
                    {
                      case 2 : X1r = X1R;
                      break;

                      case 4 : Y1r = Y1R;
                      break;

                      case 8 : X2r = X2R;
                      break;

                      case 1 : Y2r = Y2R;
                      break;
                    }

                    ScanModify = 1;
                  }
                 
  
                break;
  
                case RUT_UNIFY_CROSS  :

                  if ( ( UnifyMask & MaskEqual ) == UnifyMask )
                  {
                    SetRdsDeleted( ScanBeginEqui );
                  }
                  else
                  {
                    if ( UnifyMask & MaskEqual )
                    {
                      switch ( UnifyMask ^ MaskEqual )
                      {
                        case 1 : Y2R = Y2r;
                        break;

                        case 2 : X1R = X1r;
                        break;

                        case 4 : Y1R = Y1r;
                        break;

                        case 8 : X2R = X2r;
                        break;
                      }

                      BeginModify = 1;
                    }
                    else
                    if ( ModeCreate )
                    {
                      switch( Mask )
                      {
                        case  3 : 
                        case 12 :

                          Xn [ 0 ] = X1r;
                          Yn [ 0 ] = Y1r;
                          DXn[ 0 ] = X2R - X1r;
                          DYn[ 0 ] = Y2R - Y1r;

                          Xn [ 1 ] = X1R;
                          Yn [ 1 ] = Y1R;
                          DXn[ 1 ] = X2r - X1R;
                          DYn[ 1 ] = Y2r - Y1R;

                        break;

                        case  6 :
                        case  9 :

                          Xn [ 0 ] = X1R;
                          Yn [ 0 ] = Y1r;
                          DXn[ 0 ] = X2r - X1R;
                          DYn[ 0 ] = Y2R - Y1r;

                          Xn [ 1 ] = X1r;
                          Yn [ 1 ] = Y1R;
                          DXn[ 1 ] = X2R - X1r;
                          DYn[ 1 ] = Y2r - Y1R;

                        break;
                      }

                      for ( Index = 0; Index < 2; Index++ )
                      {
                        if ( ( DXn[ Index ] > 0 ) &&
                             ( DYn[ Index ] > 0 ) )
                        {
                          NewRec = addrdsfigrec( FigureRds, 
                                                 ScanEqui->NAME,
                                                 Layer,
                                                 Xn [ Index ],
                                                 Yn [ Index ],
                                                 DXn[ Index ],
                                                 DYn[ Index ] );

                          SetRdsEqui( NewRec );

                          RDS_EQUI( NewRec ) = *PrevBeginEqui;
                          *PrevBeginEqui      = NewRec;
                        }
                      }
                    }
                  }
                  
                break;
  
                case RUT_UNIFY_DELETE :

                  if ( RutUnifyTable[ Mask ][ 1 ] )
                  {
                    SetRdsDeleted( ScanBeginEqui );
                  }
                  else
                  {
                    SetRdsDeleted( ScanEqui );
                  }
  
                break;
  
                case RUT_UNIFY_NOTHING :

                  if ( ( UnifyMask & MaskEqual ) == UnifyMask )
                  {
                    SetRdsDeleted( ScanBeginEqui );
                  }
                  
                break;
              }

              if ( ScanModify )
              {
                ScanEqui->X  = X1r;
                ScanEqui->Y  = Y1r;
                ScanEqui->DX = X2r - X1r;
                ScanEqui->DY = Y2r - Y1r;
              }
            }
          }

          if ( ! IsRdsEndEqui( ScanEqui ) )
          {
            ScanEqui = RDS_EQUI( ScanEqui );
          }
          else
          {
            EndScanEqui = 1;
          }
        }

        if ( ( ! IsRdsDeleted( ScanBeginEqui ) ) &&
             ( BeginModify                     ) )
        {
          ScanBeginEqui->X  = X1R;
          ScanBeginEqui->Y  = Y1R;
          ScanBeginEqui->DX = X2R - X1R;
          ScanBeginEqui->DY = Y2R - Y1R;
        }
      }

      if ( ! IsRdsEndEqui( ScanBeginEqui ) )
      {
        ScanBeginEqui = RDS_EQUI( ScanBeginEqui );
      }
      else
      {
        EndBeginEqui = 1;
      }
    }

    if ( BeginEqui != *PrevBeginEqui )
    {
      BeginEqui  = *PrevBeginEqui;
      ModeCreate = 0;
    }
    else
    {
      PrevBeginEqui = &RDS_EQUI( ScanBeginEqui );
      BeginEqui     = RDS_EQUI( ScanBeginEqui );
      ModeCreate    = 1;
    }
  }

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    ScanRec = FigureRds->LAYERTAB[ Layer ];
    PrevRec = &FigureRds->LAYERTAB[ Layer ];

    while ( ScanRec != (rdsrec_list *)NULL )
    {
      if ( IsRdsDeleted( ScanRec ) )
      {
        *PrevRec = ScanRec->NEXT;
        freerdsrec(  ScanRec, FigureRds->SIZE );
        ScanRec = *PrevRec;
      }
      else
      {
        PrevRec = &ScanRec->NEXT;
        ScanRec = ScanRec->NEXT;
      }
    }
  }

  rdsend();
  return( HeadEqui );
}
