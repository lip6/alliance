/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   LYNX                            |
|                                                             |
| File    :                 cutelbow.c                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"
# include "mpu.h"

# include "mbkrds.h"
# include "cutelbow.h"

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

static phfig_list *FigureDumpContact = (phfig_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                       Cut Transistor Table                  |
|                                                             |
\------------------------------------------------------------*/

static long LYNX_CUT_C_X_PARAM[ 5 ];

/* 0 -> Constant 0    */
/* 1 -> Nothing to do */
/* 2 -> D             */
/* 3 -> ( D + d ) / 2 */
/* 4 -> ( D - d ) / 2 */

long LYNX_CUT_C_X_TABLE [ 16 ] [ 4 ] [ 4 ] =

{
  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /*   O   */
  { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /*   O   */
  { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /*   O   */
  { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*   |   */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /*   O   */
  { { 0, 0, 4, 2 }, { 3, 0, 4, 2 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*   |   */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /* --O   */
  { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /* --O   */
  { { 0, 0, 2, 4 }, { 0, 3, 4, 4 }, { 3, 4, 4, 3 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /* --O   */
  { { 0, 0, 4, 4 }, { 3, 0, 4, 2 }, { 0, 3, 3, 4 }, { 1, 1, 1, 1 } }, /*   |   */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /* --O   */
  { { 0, 0, 4, 4 }, { 3, 0, 4, 2 }, { 0, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*   |   */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /*   O-- */
  { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /*   O-- */
  { { 0, 0, 4, 2 }, { 4, 0, 3, 4 }, { 3, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /*   O-- */
  { { 0, 0, 4, 2 }, { 3, 0, 4, 4 }, { 4, 3, 3, 4 }, { 1, 1, 1, 1 } }, /*   |   */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /*   O-- */
  { { 0, 0, 4, 2 }, { 3, 0, 4, 4 }, { 3, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*   |   */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /* --O-- */
  { { 0, 0, 2, 4 }, { 0, 3, 2, 4 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /* --O-- */
  { { 0, 0, 2, 4 }, { 0, 3, 4, 4 }, { 3, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*       */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                      /* --O-- */
  { { 0, 0, 4, 4 }, { 3, 0, 4, 4 }, { 0, 3, 2, 4 }, { 1, 1, 1, 1 } }, /*   |   */

  /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                      /* --O-- */
  { { 0, 0, 4, 4 }, { 3, 0, 4, 4 }, { 0, 3, 4, 4 }, { 3, 3, 4, 4 } }, /*   |   */

};

long *LYNX_CUT_C_X_ARRAY[ MBK_MAX_LAYER ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   LynxInitializeCutCxTable                  |
|                                                             |
\------------------------------------------------------------*/

void LynxInitializeCutCxTable()

{
  int   Index;    
  long  DeltaGate;
  long  DeltaDiff;
  char  Layer;
  char  GateLayer;
  char  DiffLayer;
  long *Table;
  int   MbkLayer;
  int   TransType;
  int   CXType;

  rdsbegin();

  for ( MbkLayer = 0; MbkLayer < MBK_MAX_LAYER; MbkLayer++ )
  {
    TransType = GET_LYNX_TRANSISTOR_TYPE( MbkLayer );
    CXType    = GET_LYNX_TRANSISTOR_CX( MbkLayer );

    if ( ( TransType == RDS_LYNX_TRANSISTOR_EMPTY ) ||
         ( CXType    == RDS_LYNX_TRANSISTOR_EMPTY ) )
    {
      LYNX_CUT_C_X_ARRAY[ MbkLayer ] = (long *)0; 
    }
    else
    {
      Table = (long *)rdsalloc( sizeof( LYNX_CUT_C_X_TABLE ), RDS_ALLOC_BLOCK );
      memcpy( Table, LYNX_CUT_C_X_TABLE, sizeof( LYNX_CUT_C_X_TABLE ) );

      LYNX_CUT_C_X_ARRAY[ MbkLayer ] = Table;

      DeltaGate = -1;
      DeltaDiff = -1;
      Index     = 0;
      GateLayer = GET_LYNX_TRANSISTOR_GATE_LAYER( MbkLayer );
      DiffLayer = GET_LYNX_TRANSISTOR_DIFF_LAYER( MbkLayer );
      Layer     = GET_VIA_LAYER( CXType, Index );

      while ( Layer != RDS_VIA_EMPTY )
      {
        if ( Layer == GateLayer )
        {
          DeltaGate = GET_VIA_SIZE( CXType, Index );
        }
        else
        if ( Layer == DiffLayer )
        {
          DeltaDiff = GET_VIA_SIZE( CXType, Index );
        }
  
        Index = Index + 1;
  
        Layer = GET_VIA_LAYER( CXType, Index );
      }
  
      if ( ( DeltaGate == -1 ) || 
           ( DeltaDiff == -1 ) )
      {
        for ( Index = 0; Index < 256; Index++ )
        {  
          Table[ Index ] = -1;
        }
      }
      else
      {
        /* 0 -> Constant 0    */
        /* 1 -> Nothing to do */
        /* 2 -> D             */
        /* 3 -> ( D + d ) / 2 */
        /* 4 -> ( D - d ) / 2 */
         
        LYNX_CUT_C_X_PARAM[ 0 ] =  0;
        LYNX_CUT_C_X_PARAM[ 1 ] = -1;
        LYNX_CUT_C_X_PARAM[ 2 ] = DeltaDiff;
        LYNX_CUT_C_X_PARAM[ 3 ] = ( DeltaDiff + DeltaGate ) >> 1;
        LYNX_CUT_C_X_PARAM[ 4 ] = ( DeltaDiff - DeltaGate ) >> 1;
  
        for ( Index = 0; Index < 256; Index++ )
        {
          Table[ Index ] = LYNX_CUT_C_X_PARAM[ Table[ Index ] ];
        }
      }
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   LynxCutTransistorElbow                    |
|                                                             |
\------------------------------------------------------------*/

long LynxCutTransistorElbow( Figure, Window, Core )

  rdsfig_list *Figure;
  rdswindow   *Window;
  char         Core;
{
  rdsrec_list    *Rectangle;
  rdsrec_list    *ScanRec;
  rdsrecwin_list  StaticRecWin;
  rdsrecwin_list *RecWin;
  rdsrecwin_list *ScanRecWin;
  rdswinrec_list *ScanWinRec;
  rdswin_list    *ScanWin;
  long           *LineTable;
  long           *Table;
  long            Counter;
  char            FoundError;
  char            Error;
  int             Flag;
  char            Type;
  int             Index;
  int             GateLayer;
  int             Layer;
  int             DiffLayer;
  long            X1;
  long            X2;
  long            Y1;
  long            Y2;
  long            X;
  long            Y;
  int             MbkLayer;
  char            TransType;
  char            CXType;

  rdsbegin();

  StaticRecWin.NEXT  = (rdsrecwin_list *)0;
  FigureDumpContact  = addphfig( "core_cx" );
  FoundError         = 0;
  Counter            = 0;

  Lynxrecrdsmbk( FigureDumpContact, Figure->LAYERTAB[ RDS_ABOX ], 0 );

  for ( MbkLayer = 0; MbkLayer < MBK_MAX_LAYER; MbkLayer++ )
  {
    Table = LYNX_CUT_C_X_ARRAY[ MbkLayer ];

    if ( Table == (long *)0 ) continue;

    TransType = GET_LYNX_TRANSISTOR_TYPE( MbkLayer );
    CXType    = GET_LYNX_TRANSISTOR_CX( MbkLayer );
    GateLayer = GET_LYNX_TRANSISTOR_GATE_LAYER( MbkLayer );
    DiffLayer = GET_LYNX_TRANSISTOR_DIFF_LAYER( MbkLayer );

    for ( Rectangle  = Figure->LAYERTAB[ GateLayer ];
          Rectangle != (rdsrec_list *)0;
          Rectangle  = Rectangle->NEXT )
    {
      if ( IsRdsVia( Rectangle ) )
      {
        Type = GetLynxAttribute( Rectangle );

        if ( Type == CXType )
        {
          X1 = Rectangle->X;
          X2 = Rectangle->DX + X1;
          Y1 = Rectangle->Y;
          Y2 = Rectangle->DY + Y1;
          X  = X1 + ( Rectangle->DX >> 1 );
          Y  = Y1 + ( Rectangle->DY >> 1 );

          Error = 0;
          Flag  = 0;

          if ( IsRdsOneWindow ( Rectangle ) )
          {
            StaticRecWin.WINDOW = (rdswin_list *)Rectangle->USER;
            RecWin              = &StaticRecWin;
          }
          else
          {
            RecWin = (rdsrecwin_list *)Rectangle->USER;
          }

          for ( ScanRecWin  = RecWin ;
                ( ScanRecWin != (rdsrecwin_list *)0 ) && 
                ( Error      == 0                      );
                ScanRecWin  = ScanRecWin->NEXT )
          {
            ScanWin = ScanRecWin->WINDOW;

            for ( ScanWinRec  = ScanWin->LAYERTAB[ GateLayer ];
                  ( ScanWinRec != (rdswinrec_list *)0 ) &&
                  ( Error      == 0                      );
                  ScanWinRec  = ScanWinRec->NEXT )
            {
              for ( Index = 0; Index < RWI_MAX_REC; Index++ )
              {
                ScanRec = ScanWinRec->RECTAB[ Index ];

                if ( ( ScanRec != (rdsrec_list *)0 ) &&
                     (   IsRdsFigRec( ScanRec )       ) &&
                     (   IsRdsSegment( ScanRec )      ) )
                {
                  Layer = GetLynxAttribute( ScanRec );

                  if ( ( Layer == MbkLayer                  ) &&
                       ( ScanRec->X                   <= X2 ) &&
                       ( ScanRec->Y                   <= Y2 ) &&
                       ( ( ScanRec->X + ScanRec->DX ) >= X1 ) &&
                       ( ( ScanRec->Y + ScanRec->DY ) >= Y1 ) )
                  {
                    if ( ScanRec->Y == Y )
                    {
                      Flag  |= LYNX_NORTH_MASK;
                    }
                    else
    
                    if ( ( ScanRec->Y + ScanRec->DY ) == Y )
                    {
                      Flag  |= LYNX_SOUTH_MASK;
                    }
                    else
    
                    if ( ( ScanRec->X + ScanRec->DX ) == X )
                    {
                      Flag  |= LYNX_WEST_MASK;
                    }
                    else
                    if ( ScanRec->X == X )
                    {
                      Flag  |= LYNX_EAST_MASK;
                    }
                    else 
                    {
                      Error = 1; break;
                    }
                  }
                }
              }
            }
          }

          if ( Error == 0 )
          {
            if ( Flag != 0 )
            {
              LineTable = &Table[ Flag << 4 ];
              ScanRec   = LYNX_LINK( Rectangle );

              while ( ScanRec != Rectangle )
              {
                if ( GetRdsLayer( ScanRec ) == DiffLayer )
                {
                  SetLynxDeleted( ScanRec );

                  X = ScanRec->X;
                  Y = ScanRec->Y;
                }

                ScanRec = LYNX_LINK( ScanRec );
              }

              for ( Index = 0; Index < 16; Index = Index + 4 )
              {
                if ( LineTable[ Index ]  == -1 ) break;

                ScanRec = addrdsfigrec( Figure,
                                        0, 
                                        DiffLayer,
                                        X + LineTable[ Index + 0 ],
                                        Y + LineTable[ Index + 1 ],
                                        LineTable[ Index + 2 ],
                                        LineTable[ Index + 3 ] );

                SetLynxCut( ScanRec );

                addrdsrecwindow( ScanRec, Window );
              }

              Counter = Counter + 1;
            }
          }
          else
          {
            if ( Core ) 
            {
              Lynxrecrdsmbk( FigureDumpContact, Rectangle, 0 );

              FoundError = 1;
            }
            else
            {
              rdsend();
              return( -1 );
            }
          }
        }
      }
    }
  }

  if ( FoundError )
  {
    savephfig( FigureDumpContact );

    rdsend();
    return( -1 );
  }

  rdsend();
  return( Counter );
}
