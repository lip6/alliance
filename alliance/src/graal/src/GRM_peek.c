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
| File    :                   Peek.c                          |
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
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
# include "GSB.h"
# include "GRM_peek.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  graalpeek *GraalHeadPeek = (graalpeek *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          GraalAllocPeek                     |
|                                                             |
\------------------------------------------------------------*/

graalpeek *GraalAllocPeek()
{
  return((graalpeek *)rdsalloc(sizeof(graalpeek), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           GraalFreePeek                     |
|                                                             |
\------------------------------------------------------------*/

void GraalFreePeek( FreePeek )

   graalpeek *FreePeek;
{
  rdsfree((char *)FreePeek, sizeof(graalpeek));
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalAddPeek                        |
|                                                             |
\------------------------------------------------------------*/

void GraalAddPeek( Rectangle )

   rdsrec_list *Rectangle;
{
  graalpeek *Peek;

  rdsbegin();

  Peek = GraalAllocPeek();

  Peek->RECTANGLE = Rectangle;
  Peek->NEXT      = GraalHeadPeek;
  GraalHeadPeek   = Peek;

  SetGraalPeek( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalDelPeek                        |
|                                                             |
\------------------------------------------------------------*/

void GraalDelPeek()

{
  graalpeek    *Peek;
  rdsins_list  *Instance;
  rdsrec_list  *ScanRec;
  rdsrec_list  *DelRec;
  rdsrec_list **Previous;
  char          Layer;

  rdsbegin();

  while ( GraalHeadPeek != (graalpeek *)0 )
  {
    Peek    = GraalHeadPeek;
    ScanRec = Peek->RECTANGLE;

    ClearGraalPeek( ScanRec );

    Instance = (rdsins_list *)GRAAL_PREVIOUS( ScanRec );

    for ( Layer = 0; Layer < RDS_ABOX; Layer++ )
    {
      Previous = &Instance->LAYERTAB[ Layer ];
      ScanRec  = *Previous;

      while( ScanRec != (rdsrec_list *)0 )
      {
        DelRec  = ScanRec;
        ScanRec = ScanRec->NEXT;

        if ( IsGraalPeek( DelRec ) )
        {
          *Previous = ScanRec;

          GraalEraseRectangle( DelRec );

          freerdsrec( DelRec, GRAAL_SIZE );
        }
        else
        {
          Previous = &DelRec->NEXT;
        }
      }
    }

    GraalHeadPeek = GraalHeadPeek->NEXT;

    GraalFreePeek( Peek );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   Graal Peek Instance                       |
|                                                             |
\------------------------------------------------------------*/

void GraalPeekInstance( Rectangle, LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   rdsrec_list *Rectangle;
   long         LambdaX1;
   long         LambdaY1;
   long         LambdaX2;
   long         LambdaY2;
{
  phfig_list   *ModelMbk;
  phfig_list   *FigureMbk;

  phins_list   *InstanceMbk;
  phins_list   *SourceMbk;
  phins_list  **PreviousMbk;

  rdsfig_list  *FigureRds;
  rdsins_list  *TargetRds;
  rdsrec_list  *ScanRec; 
  rdsrec_list  *DelRec; 
  void         *Pointer;
  void         *Delete;
  long          X1;
  long          Y1;
  long          X2;
  long          Y2;
  long          Xab1;
  long          Yab1;
  long          Xab2;
  long          Yab2;
  long          XabMin;
  long          YabMin;
  long          XabMax;
  long          YabMax;
  long          Swap;
  char          Layer;

  rdsbegin();

  XabMin = LambdaX1 - GRAAL_PEEK_BOUND;
  YabMin = LambdaY1 - GRAAL_PEEK_BOUND;
  XabMax = LambdaX2 + GRAAL_PEEK_BOUND;
  YabMax = LambdaY2 + GRAAL_PEEK_BOUND;
  
  FigureRds   = addrdsfig( "_ludo_", GRAAL_SIZE );
  FigureMbk   = addphfig( "_ludo_"              );
  SourceMbk   = (phins_list  *)GRAAL_MBK( Rectangle );
  TargetRds   = (rdsins_list *)GRAAL_PREVIOUS( Rectangle );

  InstanceMbk = addphins( FigureMbk,
                          SourceMbk->FIGNAME, SourceMbk->INSNAME, 
                          SourceMbk->TRANSF, 
                          SourceMbk->XINS, SourceMbk->YINS );
  do
  {
    Pointer = (void *)

      Graalflattenphfig( FigureMbk, FigureMbk->PHINS->INSNAME, YES );

    if ( Pointer == (void *)0 ) break;

    Pointer         = (void *)FigureMbk->PHSEG;
    FigureMbk->PHSEG = (phseg_list *)0;

    while( Pointer != (void *)0 )
    {
      Delete  = Pointer;
      Pointer = ((phseg_list *)Pointer)->NEXT;

      segmbkrds( FigureRds, (phseg_list *)Delete, 1 );
      mbkfree( Delete );
    }

    Pointer         = (void *)FigureMbk->PHREF;
    FigureMbk->PHREF = (phref_list *)0; 
 
    while( Pointer != (void *)0 ) 
    { 
      Delete  = Pointer;
      Pointer = ((phref_list *)Pointer)->NEXT; 
     
      refmbkrds( FigureRds, (phref_list *)Delete );
      mbkfree( Delete ); 
    }

    Pointer         = (void *)FigureMbk->PHVIA;
    FigureMbk->PHVIA = (phvia_list *)0; 
 
    while( Pointer != (void *)0 ) 
    { 
      Delete  = Pointer;
      Pointer = ((phvia_list *)Pointer)->NEXT; 

      viambkrds( FigureRds, (phvia_list *)Delete, 1 );
      mbkfree( Delete ); 
    }

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      ScanRec = FigureRds->LAYERTAB[ Layer ];

      while ( ScanRec != (rdsrec_list *)0 )
      {
        DelRec  = ScanRec;
        ScanRec = ScanRec->NEXT;

        X1 = DelRec->X;
        Y1 = DelRec->Y;
        X2 = DelRec->DX + X1;
        Y2 = DelRec->DY + Y1;

        if ( ( X1 < LambdaX2 ) &&
             ( Y1 < LambdaY2 ) &&
             ( X2 > LambdaX1 ) &&
             ( Y2 > LambdaY1 ) )
        {
          SetGraalPeek( DelRec );
          ClearRdsFigRec( DelRec );

          DelRec->NEXT                 = TargetRds->LAYERTAB[ Layer ];
          TargetRds->LAYERTAB[ Layer ] = DelRec;

          GRAAL_MBK( DelRec )      = (void         *)SourceMbk;
          GRAAL_PREVIOUS( DelRec ) = (rdsrec_list **)TargetRds;

          if ( X1 < LambdaX1 ) { X1 = LambdaX1; }
          if ( Y1 < LambdaY1 ) { Y1 = LambdaY1; }
          if ( X2 > LambdaX2 ) { X2 = LambdaX2; }
          if ( Y2 > LambdaY2 ) { Y2 = LambdaY2; }

          DelRec->X    = X1;
          DelRec->Y    = Y1;
          DelRec->DX   = X2 - X1;
          DelRec->DY   = Y2 - Y1;

          GraalInsertRectangle( DelRec );
        }
        else
        {
          freerdsrec( DelRec, GRAAL_SIZE );
        }
      }

      FigureRds->LAYERTAB[ Layer ] = (rdsrec_list *)0;
    }

    InstanceMbk = FigureMbk->PHINS;
    PreviousMbk = &FigureMbk->PHINS;

    while( InstanceMbk != (phins_list *)0 )
    {
      ModelMbk = Graalgetphfig( InstanceMbk->FIGNAME, 'P' );

      if ( ModelMbk != (phfig_list *)0 )
      {
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
 
        Xab1 = RfmRoundLow( (long)( Xab1 * GRAAL_RDS_LAMBDA / GRAAL_SCALE_X ) );
        Yab1 = RfmRoundLow( (long)( Yab1 * GRAAL_RDS_LAMBDA / GRAAL_SCALE_X ) );
        Xab2 = RfmRoundHigh( (long)( Xab2 * GRAAL_RDS_LAMBDA / GRAAL_SCALE_X ) );
        Yab2 = RfmRoundHigh( (long)( Yab2 * GRAAL_RDS_LAMBDA / GRAAL_SCALE_X ) );
 
        if ( Xab1 > Xab2 ) { Swap = Xab1; Xab1 = Xab2; Xab2 = Swap; }
        if ( Yab1 > Yab2 ) { Swap = Yab1; Yab1 = Yab2; Yab2 = Swap; }
        
        if ( ( Xab1 < XabMax ) &&
             ( Yab1 < YabMax ) &&
             ( Xab2 > XabMin ) &&
             ( Yab2 > YabMin ) )
        {
          PreviousMbk = &InstanceMbk->NEXT;
          InstanceMbk = InstanceMbk->NEXT;

          continue;
        }
      }

      *PreviousMbk = InstanceMbk->NEXT;
      Delete       = (void *)InstanceMbk;
      InstanceMbk  = InstanceMbk->NEXT;

      mbkfree( Delete );
    }
  }
  while( FigureMbk->PHINS != (phins_list *)0 );

  delphfig( "_ludo_" );
  delrdsfig( "_ludo_" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   Graal Real Flat Instance                  |
|                                                             |
\------------------------------------------------------------*/

char GraalRealFlatInstance( Rectangle )

   rdsrec_list *Rectangle;
{
  phfig_list   *FigureMbk;
  phins_list   *InstanceMbk;
  phins_list   *SourceMbk;

  void         *Pointer;
  void         *Previous;
  void         *SavePointer;

  rdsbegin();

  FigureMbk = addphfig( "_ludo_" );
  SourceMbk = (phins_list *)GRAAL_MBK( Rectangle );

  InstanceMbk = addphins( FigureMbk,
                          SourceMbk->FIGNAME, SourceMbk->INSNAME, 
                          SourceMbk->TRANSF, 
                          SourceMbk->XINS, SourceMbk->YINS );
  Pointer = (void *)

      Graalflattenphfig( FigureMbk, FigureMbk->PHINS->INSNAME, YES );

  if ( Pointer == (void *)0 )
  {
    delphfig( "_ludo_" );

    rdsend();
    return( GRAAL_FALSE );
  }

  SavePointer = (void *)(GraalFigureMbk->PHSEG);
  GraalFigureMbk->PHSEG = FigureMbk->PHSEG;
  Previous = (void *)(&(GraalFigureMbk->PHSEG));

  for ( Pointer  = (void *)(FigureMbk->PHSEG);
        Pointer != (void *)0;
        Pointer  = (void *)(((phseg_list *)Pointer)->NEXT))
  {
    ((phseg_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phseg_list *)Pointer)->NEXT);

    GraalAddSegment( (phseg_list *)Pointer );
  }

  if ( SavePointer != (void *)0 )
  {
    *(phseg_list **)Previous = (phseg_list *)SavePointer;
    ((phseg_list *)SavePointer)->USER = Previous;
  }

  FigureMbk->PHSEG = (phseg_list *)0;

  SavePointer = (void *)(GraalFigureMbk->PHREF);
  GraalFigureMbk->PHREF = FigureMbk->PHREF;
  Previous = (void *)(&(GraalFigureMbk->PHREF));
 
  for ( Pointer  = (void *)(FigureMbk->PHREF);
        Pointer != (void *)0;
        Pointer  = (void *)(((phref_list *)Pointer)->NEXT))
  {
    ((phref_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phref_list *)Pointer)->NEXT);

    GraalAddReference( (phref_list *)Pointer );
  }
 
  if ( SavePointer != (void *)0 )
  {
    *(phref_list **)Previous = (phref_list *)SavePointer;
    ((phref_list *)SavePointer)->USER = Previous;
  }

  FigureMbk->PHREF = (phref_list *)0;

  SavePointer = (void *)(GraalFigureMbk->PHVIA);
  GraalFigureMbk->PHVIA = FigureMbk->PHVIA;
  Previous = (void *)(&(GraalFigureMbk->PHVIA));
 
  for ( Pointer  = (void *)(GraalFigureMbk->PHVIA);
        Pointer != (void *)0;
        Pointer  = (void *)(((phvia_list *)Pointer)->NEXT))
  {
    ((phvia_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phvia_list *)Pointer)->NEXT);

    GraalAddVia( (phvia_list *)Pointer );
  }
 
  if ( SavePointer != (void *)0 )
  {
    *(phvia_list **)Previous = (phvia_list *)SavePointer;
    ((phvia_list *)SavePointer)->USER = Previous;
  }

  FigureMbk->PHVIA = (phvia_list *)0;

  SavePointer = (void *)(GraalFigureMbk->PHINS);
  GraalFigureMbk->PHINS = FigureMbk->PHINS;
  Previous = (void *)(&(GraalFigureMbk->PHINS));
 
  for ( Pointer  = (void *)(FigureMbk->PHINS);
        Pointer != (void *)0;
        Pointer  = (void *)(((phins_list *)Pointer)->NEXT))
  {
    ((phins_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phins_list *)Pointer)->NEXT);

    GraalAddInstance( (phins_list *)Pointer );
  }

  if ( SavePointer != (void *)0 )
  {
    *(phins_list **)Previous = (phins_list *)SavePointer;
    ((phins_list *)SavePointer)->USER = Previous;
  }

  FigureMbk->PHINS = (phins_list *)0;

  delphfig( "_ludo_" );

  GraalDelUndo( &GraalHeadRedo );
  GraalDelUndo( &GraalHeadUndo );

  GraalDelInstance( (rdsins_list *)GRAAL_PREVIOUS( Rectangle ) );

  rdsend();
  return( GRAAL_TRUE );
}
