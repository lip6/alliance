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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  mbkrds.c                         |
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
# include <string.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"
# include "rtl.h"
# include "mpu.h"

# include "mbkrds.h"

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
|                          Local Variables                    |
|                                                             |
\------------------------------------------------------------*/

  static int LynxConnectorOrientTable[ 4 ] =

  {
    NORTH, SOUTH, EAST, WEST
  };

  static int LynxSegmentOrientTable[ 4 ] =
  
  {
    UP, DOWN, RIGHT, LEFT
  };

  static char LynxNameBuffer[ 512 ];
  static char *LynxStaticRefRef = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      LynxGetReferenceName                   |
|                                                             |
\------------------------------------------------------------*/

static char *LynxGetReferenceName( Name )

  char *Name;
{
  char *ScanName;

  ScanName = strrchr( Name, '_' );

  if ( ScanName != (char *)0 )
  {
    strcpy( LynxNameBuffer, Name );

    Name = strrchr( LynxNameBuffer, '_' );
    if ( Name != (char *)0 ) *Name = '\0';

    Name = namealloc( LynxNameBuffer );
  }

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxGetVectorName                      |
|                                                             |
\------------------------------------------------------------*/

static char *LynxGetVectorName( Name )

  char *Name;
{
  char *ScanName;

  ScanName = strrchr( Name, ']' );

  if ( ScanName != (char *)0 )
  {
    strcpy( LynxNameBuffer, Name );

    Name = strrchr( LynxNameBuffer, ']' );
    if ( Name != (char *)0 ) *Name = '\0';

    Name = strrchr( LynxNameBuffer, '[' );
    if ( Name != (char *)0 ) *Name = ' ';

    Name = namealloc( LynxNameBuffer );
  }

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxsegmbkrds                        |
|                                                             |
\------------------------------------------------------------*/

void Lynxsegmbkrds( FigureRds, Segment )

  rdsfig_list *FigureRds;
  phseg_list  *Segment;
{
  rdsrec_list *Rectangle;
  rdsrec_list *ScanRec;
  char         Orient=0;

  rdsbegin();

  ScanRec = Rectangle = segmbkrds( FigureRds, Segment, 1 );

  if ( Rectangle == (rdsrec_list *)0 ) 
  {
    rdsend();
    return;
  }

  SetLynxFirstLink( Rectangle );

  switch ( Segment->TYPE ) 
  {
    case UP    : Orient = LYNX_NORTH;
      break;
    case DOWN  : Orient = LYNX_SOUTH;
      break;
    case LEFT  : Orient = LYNX_WEST;
      break;
    case RIGHT : Orient = LYNX_EAST;
      break;
  }

  do
  {
    SetLynxAttribute( ScanRec, Segment->LAYER );
    SetLynxOrient(    ScanRec, Orient         );

    LYNX_LINK( ScanRec ) = (rdsrec_list *)ScanRec->USER;
    ScanRec              = (rdsrec_list *)ScanRec->USER;
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxconmbkrds                        |
|                                                             |
\------------------------------------------------------------*/

void Lynxconmbkrds( FigureRds, Connector )

  rdsfig_list *FigureRds;
  phcon_list  *Connector;
{
  rdsrec_list *Rectangle;
  rdsrec_list *ScanRec;
  char         Orient=0;

  rdsbegin();

  ScanRec = Rectangle = conmbkrds( FigureRds, Connector );

  if ( Rectangle == (rdsrec_list *)0 )
  {
    rdsend();
    return;
  }

  SetLynxFirstLink( Rectangle );

  switch( Connector->ORIENT )
  {
    case NORTH : Orient = LYNX_NORTH;
      break;
    case SOUTH : Orient = LYNX_SOUTH;
      break;
    case EAST  : Orient = LYNX_EAST;
      break;
    case WEST  : Orient = LYNX_WEST;
      break;
  }

  do
  {
    SetLynxAttribute( ScanRec, Connector->LAYER );
    SetLynxOrient(    ScanRec, Orient           );

    LYNX_LINK( ScanRec ) = (rdsrec_list *)ScanRec->USER;
    ScanRec              = (rdsrec_list *)ScanRec->USER;
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxrefmbkrds                        |
|                                                             |
\------------------------------------------------------------*/

void Lynxrefmbkrds( FigureRds, Reference )

  rdsfig_list *FigureRds;
  phref_list  *Reference;
{
  rdsrec_list *Rectangle;
  rdsrec_list *ScanRec;
  char        *ReferenceName;

  rdsbegin();

  if ( LynxStaticRefRef == (char *)0 )
  {
    LynxStaticRefRef = namealloc( "ref_ref" );
  }

  if ( Reference->FIGNAME == LynxStaticRefRef )
  {
    rdsend();
    return;
  }

  ScanRec = Rectangle = refmbkrds( FigureRds, Reference );

  if ( Rectangle == (rdsrec_list *)0 )
  {
    rdsend();
    return;
  }

  SetLynxFirstLink( Rectangle );

  if ( ( IsRdsRefCon( Rectangle )     ) &&
       ( Rectangle->NAME != (char *)0 ) )
  {
    ReferenceName = LynxGetReferenceName( Rectangle->NAME );
  }
  else
  {
    ReferenceName = Rectangle->NAME;
  }

  do
  {
    ScanRec->NAME = ReferenceName;
 
    LYNX_LINK( ScanRec ) = (rdsrec_list *)ScanRec->USER;
    ScanRec              = (rdsrec_list *)ScanRec->USER;
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxviambkrds                        |
|                                                             |
\------------------------------------------------------------*/

void Lynxviambkrds( FigureRds, Via )

  rdsfig_list *FigureRds;
  phvia_list  *Via;
{
  rdsrec_list *Rectangle;
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = Rectangle = viambkrds( FigureRds, Via, 1 );

  if ( Rectangle == (rdsrec_list *)0 )
  {
    rdsend();
    return;
  }

  SetLynxFirstLink( Rectangle );

  do
  {
    SetLynxAttribute( ScanRec, Via->TYPE );

    LYNX_LINK( ScanRec ) = (rdsrec_list *)ScanRec->USER;
    ScanRec              = (rdsrec_list *)ScanRec->USER;
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxinsmbkrds                        |
|                                                             |
\------------------------------------------------------------*/

void Lynxinsmbkrds( FigureRds, InstanceMbk )

   rdsfig_list *FigureRds;
   phins_list  *InstanceMbk;
{
  rdsrec_list  *Rectangle;
  rdsrec_list  *DelRec;
  rdsrec_list **Previous;
  rdsins_list  *InstanceRds;
  int           Layer;

  rdsbegin();

  InstanceRds = insmbkrds( FigureRds, InstanceMbk, 'P', 1 );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    Rectangle =  InstanceRds->LAYERTAB[ Layer ];
    Previous  = &InstanceRds->LAYERTAB[ Layer ];

    while ( Rectangle != (rdsrec_list *)0 )
    {
      if ( IsRdsRefRef( Rectangle ) )
      {
        DelRec    = Rectangle;
        Rectangle = Rectangle->NEXT;
        *Previous = Rectangle;
       
        freerdsrec( DelRec, FigureRds->SIZE );
      }
      else
      {
        if ( ( IsRdsRefCon( Rectangle )        ) &&
             ( Rectangle->NAME != (char *)0 ) )
        {
          Rectangle->NAME = LynxGetReferenceName( Rectangle->NAME );
        }
      
        LYNX_LINK( Rectangle ) = (rdsrec_list *)InstanceRds;
        Previous  = &Rectangle->NEXT;
        Rectangle = Rectangle->NEXT;
      }
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxfigmbkrds                        |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *Lynxfigmbkrds( FigureMbk )
 
   phfig_list *FigureMbk;
{
  rdsfig_list *FigureRds;

  rdsbegin();

  FigureRds = addrdsfig( FigureMbk->NAME, LYNX_MODE );

  aboxmbkrds( FigureMbk, FigureRds );

  rdsbegin();

  while ( FigureMbk->PHSEG != (phseg_list *)0 )
  {
    Lynxsegmbkrds( FigureRds, FigureMbk->PHSEG, 1 );
    delphseg( FigureMbk, FigureMbk->PHSEG );
  }

  while ( FigureMbk->PHCON != (phcon_list *)0 )
  {
    Lynxconmbkrds( FigureRds, FigureMbk->PHCON );
    delphcon( FigureMbk, FigureMbk->PHCON );
  }

  while ( FigureMbk->PHREF != (phref_list *)0 )
  {
    Lynxrefmbkrds( FigureRds, FigureMbk->PHREF );
    delphref( FigureMbk, FigureMbk->PHREF );
  }

  while ( FigureMbk->PHVIA != (phvia_list *)0 )
  {
    Lynxviambkrds( FigureRds, FigureMbk->PHVIA );
    delphvia( FigureMbk, FigureMbk->PHVIA );
  }

  while ( FigureMbk->PHINS != (phins_list *)0 )
  {
    Lynxinsmbkrds( FigureRds, FigureMbk->PHINS );
    delphins( FigureMbk, FigureMbk->PHINS->INSNAME );
  }

  while ( HEAD_PHFIG != (phfig_list *)0 )
  {
    delphfig( HEAD_PHFIG->NAME );
  }

  rdsend();
  return( FigureRds );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxTreatRectangle                     |
|                                                             |
\------------------------------------------------------------*/

void LynxTreatRectangle( Rectangle )
 
  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = Rectangle;
 
  do
  {
    SetLynxTreat( ScanRec );

    ScanRec = LYNX_LINK( ScanRec );
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxrecrdsmbk                        |
|                                                             |
\------------------------------------------------------------*/

void Lynxrecrdsmbk( Figure, Rectangle, Real )

  void        *Figure;
  rdsrec_list *Rectangle;
  char         Real;
{
  rdsins_list *InstanceRds;
 
  long         X1;
  long         Y1;
  long         X2;
  long         Y2;
  long         Width;
  char         Orient;
  int          Layer;
  int          LayerRds;
  long         DLR;
  long         DWR;
  long         OFFSET;
  long         WS=0;
  char         TRANS;         
  char         USE;
  int          Index;

  if ( Rectangle == (rdsrec_list *)0 ) return;

  rdsbegin();

  if ( IsRdsFigRec( Rectangle ) )
  {
    if ( ( ! IsLynxTreat( Rectangle ) ) &&
         ( !   IsLynxCut( Rectangle ) ) )
    {
      if ( Real )
      {
        LayerRds = GetRdsLayer( Rectangle );
        addrdsfigrec( Figure, Rectangle->NAME, LayerRds,
                      Rectangle->X , Rectangle->Y, 
                      Rectangle->DX, Rectangle->DY );

        if ( LayerRds != RDS_ABOX )
        {
          SetLynxTreat( Rectangle );
        }

        rdsend();
        return;
      }
      else
      if ( IsRdsFigure( Rectangle ) )
      {
        ((phfig_list *)Figure)->XAB1 = Rectangle->X  * SCALE_X / RDS_LAMBDA;
        ((phfig_list *)Figure)->YAB1 = Rectangle->Y  * SCALE_X / RDS_LAMBDA;
        ((phfig_list *)Figure)->XAB2 = Rectangle->DX * SCALE_X / RDS_LAMBDA;
        ((phfig_list *)Figure)->YAB2 = Rectangle->DY * SCALE_X / RDS_LAMBDA;

        ((phfig_list *)Figure)->XAB2 += ((phfig_list *)Figure)->XAB1;
        ((phfig_list *)Figure)->YAB2 += ((phfig_list *)Figure)->YAB1;

        rdsend();
        return;
      }
      else
      if ( IsRdsSegment( Rectangle ) )
      {
        Orient = LynxSegmentOrientTable[ GetLynxOrient( Rectangle ) ];
        Layer  = GetLynxAttribute( Rectangle );

        while ( ! IsLynxFirstLink( Rectangle ) )
        {
          Rectangle = LYNX_LINK( Rectangle );
        }

        Index    = 0;
        LayerRds = GET_SEGMENT_LAYER( Layer, 0 );

        while ( LayerRds != RDS_SEGMENT_EMPTY  )
        {
          TRANS = GET_SEGMENT_TRANS( Layer, Index );
          USE   = GET_SEGMENT_USE( Layer, Index );

          if ( ( USE == RDS_USE_ALL     ) ||
               ( USE == RDS_USE_EXTRACT ) )
          {
            if ( TRANS == RDS_TRANS_VW )
            {
              X1 = Rectangle->X;
              Y1 = Rectangle->Y;
              X2 = Rectangle->DX + X1;
              Y2 = Rectangle->DY + Y1;

              DLR    = GET_SEGMENT_DLR   ( Layer, Index );
              DWR    = GET_SEGMENT_DWR   ( Layer, Index );
              OFFSET = GET_SEGMENT_OFFSET( Layer, Index );

              switch ( Orient )
              {
                case RIGHT  : WS = Rectangle->DY - DWR;
                              X1 = X1 + DLR;
                              Y1 = Y1 + ( ( Rectangle->DY ) >> 1 ) + OFFSET;
                              X2 = X2 - DLR;
                              Y2 = Y2 - ( ( Rectangle->DY ) >> 1 ) + OFFSET;

                            break;

                case LEFT   : WS = Rectangle->DY - DWR;
                              X1 = X1 + DLR;
                              Y1 = Y1 + ( ( Rectangle->DY ) >> 1 ) - OFFSET;
                              X2 = X2 - DLR;
                              Y2 = Y2 - ( ( Rectangle->DY ) >> 1 ) - OFFSET;

                            break;

                case UP     : WS = Rectangle->DX - DWR;
                              X1 = X1 + ( ( Rectangle->DX ) >> 1 ) + OFFSET;
                              Y1 = Y1 + DLR;
                              X2 = X2 - ( ( Rectangle->DX ) >> 1 ) + OFFSET;
                              Y2 = Y2 - DLR;
 
                            break;

                case DOWN   : WS = Rectangle->DX - DWR;
                              X1 = X1 + ( ( Rectangle->DX ) >> 1 ) - OFFSET;
                              Y1 = Y1 + DLR;
                              X2 = X2 - ( ( Rectangle->DX ) >> 1 ) - OFFSET;
                              Y2 = Y2 - DLR;

                            break;
              }

              addphseg( Figure,
                        Layer,
                        WS * SCALE_X / RDS_LAMBDA,
                        X1 * SCALE_X / RDS_LAMBDA,
                        Y1 * SCALE_X / RDS_LAMBDA,
                        X2 * SCALE_X / RDS_LAMBDA,
                        Y2 * SCALE_X / RDS_LAMBDA,
                        Rectangle->NAME );
              break;
            }
          }

          Rectangle = LYNX_LINK( Rectangle );
          Index     = Index + 1;
          LayerRds  = GET_SEGMENT_LAYER( Layer, Index );
        }
      }
      else
      if ( IsRdsConnector( Rectangle ) )
      {
        Orient = LynxConnectorOrientTable[ GetLynxOrient( Rectangle ) ];
        Layer  = GetLynxAttribute( Rectangle );

        X1 = Rectangle->X + ( Rectangle->DX >> 1 );
        Y1 = Rectangle->Y + ( Rectangle->DY >> 1 );

        Width = ( Rectangle->DX > Rectangle->DY ) 
                ? Rectangle->DX : Rectangle->DY;

        addphcon( Figure, 
                  Orient, 
                  Rectangle->NAME, 
                  X1 * SCALE_X / RDS_LAMBDA,
                  Y1 * SCALE_X / RDS_LAMBDA,
                  Layer,
                  Width * SCALE_X / RDS_LAMBDA );
      }
      else
      if ( IsRdsVia( Rectangle ) )
      {
        X1 = Rectangle->X + ( Rectangle->DX >> 1 );
        Y1 = Rectangle->Y + ( Rectangle->DY >> 1 );

        addphvia( Figure, 
                  GetLynxAttribute( Rectangle ), 
                  X1    * SCALE_X / RDS_LAMBDA,
                  Y1    * SCALE_X / RDS_LAMBDA, 0, 0, NULL );
      }
      else
      if ( IsRdsReference( Rectangle ) )
      {
        X1 = Rectangle->X + ( Rectangle->DX >> 1 );
        Y1 = Rectangle->Y + ( Rectangle->DY >> 1 );

        addphref( Figure,
                  ( IsRdsRefRef( Rectangle ) ) ?
                  "ref_ref" : "ref_con",
                  Rectangle->NAME,
                  X1 * SCALE_X / RDS_LAMBDA,
                  Y1 * SCALE_X / RDS_LAMBDA );
      }

      LynxTreatRectangle( Rectangle );
    }
  }
  else
  {
    InstanceRds = (rdsins_list *)LYNX_LINK( Rectangle );
    Rectangle   = InstanceRds->LAYERTAB[ RDS_ABOX ];

    if ( Rectangle != (rdsrec_list *)0 )
    {
      if ( ! IsLynxTreat( Rectangle ) )
      {
        if ( Real )
        {
          addrdsins( Figure,
                     InstanceRds->FIGNAME,
                     InstanceRds->INSNAME,
                     InstanceRds->TRANSF,
                     InstanceRds->X,
                     InstanceRds->Y );
        }
        else
        {
          addphins( Figure,
                    InstanceRds->FIGNAME,
                    InstanceRds->INSNAME,
                    InstanceRds->TRANSF,
                    Rectangle->X * SCALE_X / RDS_LAMBDA,
                    Rectangle->Y * SCALE_X / RDS_LAMBDA );
        }
  
        SetLynxTreat( Rectangle );
      }
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        Lynxfigrdsmbk                        |
|                                                             |
\------------------------------------------------------------*/

void Lynxfigrdsmbk( FigureRds, Figure )

  rdsfig_list *FigureRds;
  void        *Figure;
{
  rdsins_list *InstanceRds;
  rdsrec_list *Rectangle;
  int          Layer;

  rdsbegin();

  Lynxrecrdsmbk( Figure, FigureRds->LAYERTAB[ RDS_ABOX ], 0 );

  for ( Layer = 0; Layer < RDS_ABOX; Layer++ )
  {
    for ( Rectangle  = FigureRds->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)0;
          Rectangle  = Rectangle->NEXT )
    {
      Lynxrecrdsmbk( Figure, Rectangle, 0 );
    }

  }

  for ( InstanceRds = FigureRds->INSTANCE;
        InstanceRds != (rdsins_list *)0;
        InstanceRds  = InstanceRds->NEXT )
  {
    Lynxrecrdsmbk( Figure, InstanceRds->LAYERTAB[ RDS_ABOX ], 0 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Lynxfigrds                          |
|                                                             |
\------------------------------------------------------------*/

void Lynxfigrds( FigureRds )

  rdsfig_list *FigureRds;
{
  rdsrec_list *Rectangle;
  rdsins_list *Instance;
  int          Layer;

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = FigureRds->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)0;
          Rectangle  = Rectangle->NEXT )
    {
      if ( Rectangle->NAME != (char *)0 )
      {
        if ( IsRdsRefCon( Rectangle ) )
        {
          Rectangle->NAME = LynxGetReferenceName( Rectangle->NAME );
        }

        Rectangle->NAME = LynxGetVectorName( Rectangle->NAME );
      }
    }
  }

  for ( Instance  = FigureRds->INSTANCE;
        Instance != (rdsins_list *)0;
        Instance  = Instance->NEXT )
  {
    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      for ( Rectangle  = Instance->LAYERTAB[ Layer ];
            Rectangle != (rdsrec_list *)0;
            Rectangle  = Rectangle->NEXT )
      {
        if ( Rectangle->NAME != (char *)0 )
        {
          if ( IsRdsRefCon( Rectangle ) )
          {
            Rectangle->NAME = LynxGetReferenceName( Rectangle->NAME );
          }

          Rectangle->NAME = LynxGetVectorName( Rectangle->NAME );
        }

        LYNX_LINK( Rectangle ) = (rdsrec_list *)Instance;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         LynxSaveFigure                      |
|                                                             |
\------------------------------------------------------------*/

void LynxSaveFigure( Figure, Real )

  void *Figure;
  char  Real;
{
  if ( Real )
  {
    saverdsfig( Figure );
  }
  else
  {
    savephfig( Figure );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          LynxDelFigure                      |
|                                                             |
\------------------------------------------------------------*/

void LynxDelFigure( Figure, Real )

  void *Figure;
  char  Real;
{
  if ( Real )
  {
    delrdsfig( ((rdsfig_list *)Figure)->NAME );
  }
  else
  {
    delphfig( ((phfig_list *)Figure)->NAME );
  }
}
