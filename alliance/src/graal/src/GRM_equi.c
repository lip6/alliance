/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Equi.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
# include "GSB.h"
# include "GRM_equi.h"

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

  rdsrec_list *GraalHeadEqui = (rdsrec_list *)NULL;

  static char GraalReferenceBuffer[ GRAAL_REFERENCE_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         GraalCutCx                          |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *GraalCutCx( Rec, X1, Y1, X2, Y2  )

  rdsrec_list *Rec;
  long         X1;
  long         Y1;
  long         X2;
  long         Y2;
{
  void        *Pointer;
  phseg_list  *Segment;
  rdsrec_list *Diff;
  rdsrec_list *Save;
  rdsrec_list *ScanRec;
  graalrecwin *RecWin;
  graalrecwin *ScanRecWin;
  graalrecwin  StaticRecWin;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  long        *Table;
  long        *LineTable;
  long         Xvia;
  long         Yvia;
  char         Index;
  char         Mask;
  char         MbkLayer;
  char         DiffLayer;
  char         GateLayer;
  char         CXType;

  rdsbegin();

  Pointer = GRAAL_MBK( Rec );
  Xvia    = ((phvia_list *)Pointer)->XVIA;
  Yvia    = ((phvia_list *)Pointer)->YVIA;
  CXType  = ((phvia_list *)Pointer)->TYPE;

  MbkLayer  = GRAAL_CUT_C_X_LIST[ CXType ];
  GateLayer = GET_LYNX_TRANSISTOR_GATE_LAYER( MbkLayer );
  DiffLayer = GET_LYNX_TRANSISTOR_DIFF_LAYER( MbkLayer );

  if ( GetRdsLayer( Rec ) != DiffLayer )
  {
    rdsend();
    return( Rec );
  }

  Table = GRAAL_CUT_C_X_ARRAY[ MbkLayer ];
  Mask  = 0;
  Save  = Rec;

  if ( IsGraalOneWindow ( Rec ) )
  {
    StaticRecWin.NEXT   = (graalrecwin *)NULL;
    StaticRecWin.WINDOW = (graalwin *)GRAAL_WINDOW( Rec );
    RecWin              = &StaticRecWin;
  }
  else
  {
    RecWin = GRAAL_WINDOW( Rec );
  }

  for ( ScanRecWin  = RecWin ;
        ScanRecWin != (graalrecwin *) NULL ;
        ScanRecWin  = ScanRecWin->NEXT )
  {
    ScanWin = ScanRecWin->WINDOW;

    for ( ScanWinRec  = ScanWin->LAYERTAB[ GateLayer ];
          ScanWinRec != (graalwinrec *)NULL;
          ScanWinRec  = ScanWinRec->NEXT )
    {
      for ( Index = 0; Index < GRAAL_MAX_REC; Index++ )
      {
        ScanRec = ScanWinRec->RECTAB[ Index ];

        if ( ( ScanRec != (rdsrec_list *)NULL    ) &&
             ( ! IsGraalDeleted( ScanRec )       ) &&
             (   IsRdsFigRec( ScanRec )          ) &&
             (   IsRdsSegment( ScanRec )         ) )
        {
          Segment = (phseg_list *)GRAAL_MBK( ScanRec );

          if ( Segment->LAYER == MbkLayer )
          {
            if ( ( Segment->TYPE == UP   ) ||
                 ( Segment->TYPE == DOWN ) )
            {
              if ( Segment->X1 == Xvia )
              {
                if ( Segment->Y1 == Yvia ) Mask |= GRAAL_NORTH_MASK;
                else
                if ( Segment->Y2 == Yvia ) Mask |= GRAAL_SOUTH_MASK;
              }
            }
            else
            {
              if ( Segment->Y1 == Yvia )
              {
                if ( Segment->X1 == Xvia ) Mask |= GRAAL_EAST_MASK;
                else
                if ( Segment->X2 == Xvia ) Mask |= GRAAL_WEST_MASK;
              }
            }
          }
        }
      }
    }
  }

  if ( Mask != 0 )
  {
    Diff = (rdsrec_list *)NULL;

    LineTable = &Table[ Mask << 4 ];

    for ( Index = 0; Index < 16; Index = Index + 4 )
    {
      if ( LineTable[ Index ]  == -1 ) break;

      Diff = addrdsfigrec( GraalFigureRds,
                           NULL, DiffLayer,  
                           Rec->X + LineTable[ Index + 0 ],
                           Rec->Y + LineTable[ Index + 1 ],
                           LineTable[ Index + 2 ],
                           LineTable[ Index + 3 ] );

      GRAAL_PREVIOUS( Diff ) =

        &(GraalFigureRds->LAYERTAB[ DiffLayer ]);

      if ( Diff->NEXT != (rdsrec_list *)NULL )
      {
        GRAAL_PREVIOUS( Diff->NEXT ) = &Diff->NEXT;
      }

      GRAAL_MBK( Diff ) = GRAAL_MBK( Rec );

      Diff->USER = Rec->USER;
      Rec->USER  = (void *)Diff;

      SetGraalCut( Diff );
      GraalInsertRectangle( Diff );

      if ( ( ( Diff->X            ) <= X2 ) &&
           ( ( Diff->Y            ) <= Y2 ) &&
           ( ( Diff->X + Diff->DX ) >= X1 ) &&
           ( ( Diff->Y + Diff->DY ) >= Y1 ) )

      {
        Save = Diff;
      }
    }

    if ( Save != Rec ) SetGraalInvisible( Rec );
  }

  rdsend();
  return( Save );
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalExtractEqui                     |
|                                                             |
\------------------------------------------------------------*/

void GraalExtractEqui( Rec ) 

  rdsrec_list *Rec;
{
  rdsrec_list *LastRec;
  rdsrec_list *ScanRec;
  rdsrec_list *ScanInsRec;
  rdsins_list *ScanIns;
  graalrecwin *RecWin;
  graalrecwin *ScanRecWin;
  graalrecwin  StaticRecWin;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  char        *RefName;
  char        *ScanRecName;
  char        *ScanInsRecName;
  void        *Pointer;
  int          Index;
  int          LayerIns;
  int          LayerRec;
  int          Layer;
  int          Counter;
  long         X1;
  long         X2;
  long         Y1;
  long         Y2;

  rdsbegin();

  if ( GraalHeadEqui != (rdsrec_list *)NULL )
  {
    GraalDelEqui();
  }

  StaticRecWin.NEXT = (graalrecwin *)NULL;

  GraalHeadEqui = Rec;
  SetGraalTreated( Rec );

  ScanRec = (rdsrec_list *)Rec->USER;
  LastRec = Rec;

  while ( ScanRec != Rec )
  {
    SetGraalTreated( ScanRec );

    GRAAL_EQUI( LastRec ) = ScanRec;
    LastRec               = ScanRec;
    ScanRec               = (rdsrec_list *)ScanRec->USER;
  }

  Rec = GraalHeadEqui;

  do
  {
    X1 = Rec->X;
    Y1 = Rec->Y;
    X2 = Rec->DX + X1;
    Y2 = Rec->DY + Y1;

    if ( IsGraalOneWindow ( Rec ) )
    {
      StaticRecWin.WINDOW = (graalwin *)GRAAL_WINDOW( Rec );
      RecWin              = &StaticRecWin;
    }
    else
    {  
      RecWin = GRAAL_WINDOW( Rec );
    }
 
    LayerRec = GetRdsLayer ( Rec );
 
    for ( ScanRecWin  = RecWin ;
          ScanRecWin != (graalrecwin *) NULL ;
          ScanRecWin  = ScanRecWin->NEXT )
    {
      ScanWin = ScanRecWin->WINDOW;

      Index = 0;

      while ( ( Layer = GET_LYNX_GRAPH_LAYER ( LayerRec, Index ) ) != RDS_LYNX_GRAPH_EMPTY )
      {
        for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
              ScanWinRec != (graalwinrec *)NULL;
              ScanWinRec  = ScanWinRec->NEXT )
        {
          for ( Counter = 0; Counter < GRAAL_MAX_REC; Counter++ )
          {
            ScanRec = ScanWinRec->RECTAB[ Counter ];
 
            if ( ( ScanRec != (rdsrec_list *)NULL     ) && 
                 ( ! IsGraalDeleted( ScanRec )        ) &&
                 ( ! IsGraalTreated( ScanRec )        ) &&
                 ( ! IsGraalInvisible( ScanRec )      ) &&
                 ( ( ScanRec->X               ) <= X2 ) &&
                 ( ( ScanRec->Y               ) <= Y2 ) &&
                 ( ( ScanRec->X + ScanRec->DX ) >= X1 ) &&
                 ( ( ScanRec->Y + ScanRec->DY ) >= Y1 ) )
            {
              if ( IsRdsFigRec( ScanRec ) )
              {
                if ( ( IsRdsVia( ScanRec           ) ) &&
                     (   IsRdsFigRec( ScanRec      ) ) &&
                     ( ! IsGraalInvisible( ScanRec ) ) &&
                     ( ! IsGraalCut( ScanRec       ) ) )
                {
                  Pointer = GRAAL_MBK( ScanRec );

                  if ( GRAAL_CUT_C_X_LIST[ ((phvia_list *)Pointer)->TYPE ] < MBK_MAX_LAYER )
                  {
                    ScanRec = GraalCutCx( ScanRec, X1, Y1, X2, Y2 );
                  }
                }

                SetGraalTreated( ScanRec );

                GRAAL_EQUI( LastRec ) = ScanRec;
                LastRec               = ScanRec;
              }
              else
              if ( ( IsRdsConInter( ScanRec ) ) ||
                   ( IsRdsRefCon( ScanRec   ) ) )
              {
                SetGraalTreated( ScanRec );

                if ( ( IsRdsRefCon( ScanRec )        ) &&
                     ( ScanRec->NAME != (char *)NULL ) )
                {
                  strcpy( GraalReferenceBuffer, ScanRec->NAME );

                  RefName = strrchr( GraalReferenceBuffer, '_' );

                  if ( RefName != (char *)NULL )
                  {
                    *RefName = '\0';
                  }

                  ScanRecName = namealloc( GraalReferenceBuffer );
                }
                else
                {
                  ScanRecName = ScanRec->NAME;
                }

                GRAAL_EQUI( LastRec ) = ScanRec;
                LastRec               = ScanRec;

                ScanIns = (rdsins_list *)GRAAL_PREVIOUS( ScanRec );
 
                if ( ! IsGraalDeleted( ScanIns->LAYERTAB[ RDS_ABOX ] ) )
                {
                  for ( LayerIns = 0 ; LayerIns < RDS_MAX_LAYER ; LayerIns ++ )
                  {
                    for ( ScanInsRec  = ScanIns->LAYERTAB[ LayerIns ];
                          ScanInsRec != (rdsrec_list *)NULL;
                          ScanInsRec  = ScanInsRec->NEXT )
                    {
                      if ( ! IsGraalTreated( ScanInsRec ) )
                      {
                        if ( ( IsRdsRefCon( ScanInsRec )        ) &&
                             ( ScanInsRec->NAME != (char *)NULL ) )
                        {
                          strcpy( GraalReferenceBuffer, ScanInsRec->NAME );

                          RefName = strrchr( GraalReferenceBuffer, '_' );

                          if ( RefName != (char *)NULL )
                          {
                            *RefName = '\0';
                          }

                          ScanInsRecName = namealloc( GraalReferenceBuffer );
                        }
                        else
                        {
                          ScanInsRecName = ScanInsRec->NAME;
                        }

                        if ( ( ( IsRdsConInter( ScanInsRec ) ) ||
                               ( IsRdsRefCon( ScanInsRec )   ) ) && 
                             ( ScanRecName == ScanInsRecName   ) )
                        {
                          SetGraalTreated( ScanInsRec );

                          GRAAL_EQUI( LastRec ) = ScanInsRec;
                          LastRec               = ScanInsRec;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
 
        Index = Index + 1;
      }
    }

    Rec = GRAAL_EQUI( Rec );
  }
  while ( Rec != (rdsrec_list *)NULL );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalDelEqui                         |
|                                                             |
\------------------------------------------------------------*/

void GraalDelEqui() 

{
  rdsrec_list  *Rec;
  rdsrec_list  *ScanRec;
  rdsrec_list  *DelEqui;
  rdsrec_list  *DelRec;
  rdsrec_list  *FirstRec;
  void        **Previous;

  rdsbegin();

  Rec = GraalHeadEqui;

  while ( Rec != (rdsrec_list *)NULL )
  {
    ClearGraalTreated( Rec );

    DelEqui = Rec;
    Rec     = GRAAL_EQUI( Rec );

    GRAAL_EQUI( DelEqui ) = (rdsrec_list *)NULL;

    if ( IsGraalCut( DelEqui  ) )
    {
      ScanRec  = DelEqui;
      Previous = &ScanRec->USER;
      FirstRec = ScanRec;
      ScanRec  = (rdsrec_list *)(ScanRec->USER);

      do
      {
        if ( ( IsGraalCut( ScanRec )                        ) &&
             ( GRAAL_EQUI( ScanRec ) == (rdsrec_list *)NULL ) )
        {
          *Previous = ScanRec->USER;
          DelRec    = ScanRec;
          ScanRec   = (rdsrec_list *)(ScanRec->USER);
   
          *(GRAAL_PREVIOUS( DelRec )) = DelRec->NEXT;

          if ( DelRec->NEXT != (rdsrec_list *)NULL )
          {
            GRAAL_PREVIOUS( DelRec->NEXT ) = GRAAL_PREVIOUS( DelRec );
          }

          GraalEraseRectangle( DelRec );

          freerdsrec( DelRec, GRAAL_SIZE );
        }
        else
        {
          ClearGraalInvisible( ScanRec );

          Previous = &(ScanRec->USER);
          ScanRec  = (rdsrec_list *)(ScanRec->USER);
        }
      }
      while ( ScanRec != FirstRec );

      if ( ( IsGraalCut( ScanRec )                        ) &&
           ( GRAAL_EQUI( ScanRec ) == (rdsrec_list *)NULL ) )
      {
        *Previous = ScanRec->USER;
        DelRec    = ScanRec;
        ScanRec   = (rdsrec_list *)(ScanRec->USER);
  
        *(GRAAL_PREVIOUS( DelRec )) = DelRec->NEXT;

        if ( DelRec->NEXT != (rdsrec_list *)NULL )
        {
          GRAAL_PREVIOUS( DelRec->NEXT ) = GRAAL_PREVIOUS( DelRec );
        }

        GraalEraseRectangle( DelRec );

        freerdsrec( DelRec, GRAAL_SIZE );
      }
      else
      {
        ClearGraalInvisible( ScanRec );
      }
    }
  }

  GraalHeadEqui = (rdsrec_list *)NULL;

  rdsend();
}
