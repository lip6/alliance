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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                 MbkRds.c                          |
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
# include <stdlib.h>
# include <signal.h>
# include <setjmp.h>
# include "mut.h"
# include "mph.h"
# include "mpu.h"
# include "rds.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"
# include "rtl.h"
# include "rfm.h"
# include "GSB.h" 
# include "GRM.h"
# include "vrd.h"
# include "GRM_error.h"
# include "GRM_window.h"
# include "GRM_mbkrds.h"

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
|                       For Set Long Jump                     |
|                                                             |
\------------------------------------------------------------*/

  static jmp_buf GraalJumpBuffer;

/*------------------------------------------------------------\
|                                                             |
|                         Graal Figure                        |
|                                                             |
\------------------------------------------------------------*/

  rdsfig_list *GraalFigureRds     = (rdsfig_list *)NULL;
  graalconrec *GraalHeadConnector = (graalconrec *)NULL;
  phfig_list  *GraalFigureMbk     = (phfig_list  *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                 Graal Exit Handler Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalExitHandler                       |
|                                                             |
\------------------------------------------------------------*/

void GraalExitHandler()
{
  GraalFlushErrorMessage();

  longjmp( GraalJumpBuffer, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalActiveExitHandler                 |
|                                                             |
\------------------------------------------------------------*/

void GraalActiveExitHandler()
{
  MBK_EXIT_FUNCTION = GraalExitHandler;
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalResetExitHandler                 |
|                                                             |
\------------------------------------------------------------*/
 
void GraalResetExitHandler() 
{ 
  MBK_EXIT_FUNCTION = NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                     Graal Add Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Graal Add Figure                        |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *GraalAddFigure()

{
  void        *Pointer;
  void        *Previous;
  rdsrec_list *Abox;

  rdsbegin();

  GraalFigureRds = addrdsfig( GraalFigureMbk->NAME, GRAAL_SIZE );

  Abox = aboxmbkrds( GraalFigureMbk, GraalFigureRds );

  if ( Abox != (rdsrec_list *)NULL )
  {
    GraalCheckWindow( Abox->X           , Abox->Y, 
                      Abox->X + Abox->DX, Abox->Y + Abox->DY );

    delrdsfigrec( GraalFigureRds, Abox );
  }

  GraalAddAbox();

  Previous = (void *)(&(GraalFigureMbk->PHSEG));

  for ( Pointer  = (void *)(GraalFigureMbk->PHSEG);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phseg_list *)Pointer)->NEXT))
  {
    ((phseg_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phseg_list *)Pointer)->NEXT);

    GraalAddSegment( (phseg_list *)Pointer );
  }

  Previous = (void *)(&(GraalFigureMbk->PHCON));

  for ( Pointer  = (void *)(GraalFigureMbk->PHCON);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phcon_list *)Pointer)->NEXT))
  {
    ((phcon_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phcon_list *)Pointer)->NEXT);

    GraalAddConnector( (phcon_list *)Pointer );
  }

  Previous = (void *)(&(GraalFigureMbk->PHREF));
 
  for ( Pointer  = (void *)(GraalFigureMbk->PHREF);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phref_list *)Pointer)->NEXT))
  {
    ((phref_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phref_list *)Pointer)->NEXT);

    GraalAddReference( (phref_list *)Pointer );
  }
 
  Previous = (void *)(&(GraalFigureMbk->PHVIA));
 
  for ( Pointer  = (void *)(GraalFigureMbk->PHVIA);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phvia_list *)Pointer)->NEXT))
  {
    ((phvia_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phvia_list *)Pointer)->NEXT);

    GraalAddVia( (phvia_list *)Pointer );
  }
 
  Previous = (void *)(&(GraalFigureMbk->PHINS));
 
  for ( Pointer  = (void *)(GraalFigureMbk->PHINS);
        Pointer != (void *)NULL;
        Pointer  = (void *)(((phins_list *)Pointer)->NEXT))
  {
    ((phins_list *)Pointer)->USER = Previous;
    Previous = (void *)(&((phins_list *)Pointer)->NEXT);

    GraalAddInstance( (phins_list *)Pointer );
  }

  rdsend();
  return( GraalFigureRds );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graal Add Instance                      |
|                                                             |
\------------------------------------------------------------*/

rdsins_list *GraalAddInstance( InstanceMbk )

   phins_list *InstanceMbk;
{
  rdsins_list *InstanceRds;
  rdsrec_list *Rectangle;
  char         Layer;

  rdsbegin();
 
  delphfig( InstanceMbk->FIGNAME );

  if ( Graalgetphfig( InstanceMbk->FIGNAME, 'P' ) == (phfig_list *)NULL )
  {
    rdsend();
    return( (rdsins_list *)NULL );
  }

  InstanceRds = insmbkrds( GraalFigureRds, InstanceMbk, 'P', 1 );
  InstanceRds->USER = (void *)(&GraalFigureRds->INSTANCE);

  if ( InstanceRds->NEXT != (rdsins_list *)NULL )
  {
    InstanceRds->NEXT->USER = (void *)(&InstanceRds->NEXT);
  }

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = InstanceRds->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)NULL;
          Rectangle  = Rectangle->NEXT )
    { 
      GRAAL_MBK( Rectangle )      = (void         *)InstanceMbk;
      GRAAL_PREVIOUS( Rectangle ) = (rdsrec_list **)InstanceRds;

      GraalInsertRectangle( Rectangle );
    } 
  } 

  rdsend();
  return( InstanceRds );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graal Flat Instance                     |
|                                                             |
\------------------------------------------------------------*/
 
rdsins_list *GraalFlatInstance( InstanceRds )
 
   rdsins_list *InstanceRds;
{
  phfig_list  *ModelMbk;
  phins_list  *InstanceMbk;
  rdsrec_list *ScanRec;
  rdsrec_list *DelRec;
  char         Layer;

  rdsbegin();

  ModelMbk = Graalgetphfig( InstanceRds->FIGNAME, 'A' );

  if ( ModelMbk == (phfig_list *)NULL )
  {
    rdsend();
    return( (rdsins_list *)NULL );
  }

  ModelMbk = Graalrflattenphfig( ModelMbk, YES, NO  );

  if ( ModelMbk == (phfig_list *)NULL )
  {
    rdsend();
    return( (rdsins_list *)NULL );
  }

  for ( Layer = 0; Layer < RDS_ABOX; Layer++ )
  {
    ScanRec = InstanceRds->LAYERTAB[ Layer ];

    InstanceRds->LAYERTAB[ Layer ] = (rdsrec_list *)NULL;

    while( ScanRec != (rdsrec_list *)NULL )
    {
      DelRec  = ScanRec;
      ScanRec = ScanRec->NEXT;

      GraalEraseRectangle( DelRec );

      freerdsrec( DelRec, GRAAL_SIZE );
    }
  }

  ScanRec = InstanceRds->LAYERTAB[ RDS_ABOX ];

  SetGraalFlatten( ScanRec );

  InstanceMbk = (phins_list *)GRAAL_MBK( ScanRec );

  inssegmbkrds( InstanceMbk, ModelMbk, InstanceRds, 1 );
  insconmbkrds( InstanceMbk, ModelMbk, InstanceRds    );
  insrefmbkrds( InstanceMbk, ModelMbk, InstanceRds    );
  insviambkrds( InstanceMbk, ModelMbk, InstanceRds, 1 );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( ScanRec  = InstanceRds->LAYERTAB[ Layer ];
          ScanRec != (rdsrec_list *)NULL;
          ScanRec  = ScanRec->NEXT )
    {
      GRAAL_MBK( ScanRec )      = (void         *)InstanceMbk;
      GRAAL_PREVIOUS( ScanRec ) = (rdsrec_list **)InstanceRds;
 
      GraalInsertRectangle( ScanRec );
    }
  }

  delphfig( ModelMbk->NAME );

  rdsend();
  return( InstanceRds );
}

/*------------------------------------------------------------\
|                                                             |
|                   Graal Unflat Instance                     |
|                                                             |
\------------------------------------------------------------*/
 
rdsins_list *GraalUnflatInstance( InstanceRds )
 
   rdsins_list *InstanceRds;
{
  phfig_list  *ModelMbk;
  phins_list  *InstanceMbk;
  rdsrec_list *Rectangle;
  rdsrec_list *ScanRec;
  rdsrec_list *DelRec;
  char         Layer;

  rdsbegin();

  delphfig( InstanceRds->FIGNAME );

  ModelMbk = Graalgetphfig( InstanceRds->FIGNAME, 'P' );

  if ( ModelMbk == (phfig_list *)NULL )
  {
    rdsend();
    return( (rdsins_list *)NULL );
  }

  for ( Layer = 0; Layer < RDS_ABOX; Layer++ )
  {
    ScanRec = InstanceRds->LAYERTAB[ Layer ];

    InstanceRds->LAYERTAB[ Layer ] = (rdsrec_list *)NULL;

    while( ScanRec != (rdsrec_list *)NULL )
    {
      DelRec  = ScanRec;
      ScanRec = ScanRec->NEXT;

      GraalEraseRectangle( DelRec );

      freerdsrec( DelRec, GRAAL_SIZE );
    }
  }

  ScanRec = InstanceRds->LAYERTAB[ RDS_ABOX ];

  ClearGraalFlatten( ScanRec );

  InstanceMbk = (phins_list *)GRAAL_MBK( ScanRec );

  inssegmbkrds( InstanceMbk, ModelMbk, InstanceRds, 1 );
  insconmbkrds( InstanceMbk, ModelMbk, InstanceRds    );
  insrefmbkrds( InstanceMbk, ModelMbk, InstanceRds    );
  insviambkrds( InstanceMbk, ModelMbk, InstanceRds, 1 );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( ScanRec  = InstanceRds->LAYERTAB[ Layer ];
          ScanRec != (rdsrec_list *)NULL;
          ScanRec  = ScanRec->NEXT )
    {
      GRAAL_MBK( ScanRec )      = (void         *)InstanceMbk;
      GRAAL_PREVIOUS( ScanRec ) = (rdsrec_list **)InstanceRds;
 
      GraalInsertRectangle( ScanRec );
    }
  }

  delphfig( ModelMbk->NAME );
 
  rdsend();
  return( InstanceRds );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graal Add Segment                       |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *GraalAddSegment( SegmentMbk )

   phseg_list *SegmentMbk;
{
  rdsrec_list  *ScanRec;
  rdsrec_list  *ScanPrev;
  rdsrec_list  *FirstRec;
  rdsrec_list **Previous;

  rdsbegin();

  ScanRec = FirstRec = segmbkrds( GraalFigureRds, SegmentMbk, 1 );

  if ( FirstRec == (rdsrec_list *)NULL )
  {
    rdsend();
    return( FirstRec );
  }

  do 
  {
    GRAAL_MBK( ScanRec ) = (void *)SegmentMbk;

    if ( ScanRec != FirstRec ) ScanRec->NAME = (char *)0;

    GraalInsertRectangle( ScanRec );

    if ( GRAAL_PREVIOUS( ScanRec ) == (rdsrec_list **)NULL )
    {
      Previous = &(GraalFigureRds->LAYERTAB[ GetRdsLayer( ScanRec ) ]);

      for ( ScanPrev  = *Previous;
            ScanPrev != ScanRec;
            ScanPrev  = ScanPrev->NEXT )
      {
        if ( GRAAL_PREVIOUS( ScanPrev ) == (rdsrec_list **)NULL )
        {
          GRAAL_PREVIOUS( ScanPrev ) = Previous;
        }

        Previous = &ScanPrev->NEXT;
      }

      GRAAL_PREVIOUS( ScanRec ) = Previous;

      ScanPrev = ScanPrev->NEXT;

      if ( ( ScanPrev                   != (rdsrec_list  *)NULL ) &&
           ( GRAAL_PREVIOUS( ScanPrev ) != (rdsrec_list **)NULL ) )
      {
        GRAAL_PREVIOUS( ScanPrev ) = &ScanRec->NEXT;
      }
    }

    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while ( ScanRec != FirstRec );

  rdsend();
  return( FirstRec );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graal Add Connector                     |
|                                                             |
\------------------------------------------------------------*/
 
rdsrec_list *GraalAddConnector( ConnectorMbk )

   phcon_list *ConnectorMbk;
{
  rdsrec_list  *Rectangle;

  rdsbegin();

  Rectangle = conmbkrds( GraalFigureRds, ConnectorMbk );
  
  if ( Rectangle == (rdsrec_list *)NULL )
  {
    rdsend();
    return( Rectangle );
  }

  GraalAddConRec( Rectangle );

  GRAAL_MBK( Rectangle ) = (void *)ConnectorMbk;

  GraalInsertRectangle( Rectangle );

  GRAAL_PREVIOUS( Rectangle ) = 

    &(GraalFigureRds->LAYERTAB[ GetRdsLayer( Rectangle ) ]);

  if ( Rectangle->NEXT != (rdsrec_list *)NULL )
  {
    GRAAL_PREVIOUS( Rectangle->NEXT ) = &Rectangle->NEXT;
  }

  rdsend();
  return( Rectangle );
}

/*------------------------------------------------------------\
|                                                             |
|                        Graal Add Via                        |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *GraalAddVia( ViaMbk )

   phvia_list *ViaMbk;
{
  rdsrec_list  *ScanRec;
  rdsrec_list  *ScanPrev;
  rdsrec_list  *FirstRec;
  rdsrec_list **Previous;

  rdsbegin();

  ScanRec = FirstRec = viambkrds( GraalFigureRds, ViaMbk, 1 );

  if ( FirstRec == (rdsrec_list *)NULL )
  {
    rdsend();
    return( FirstRec );
  }

  do 
  {
    GRAAL_MBK( ScanRec ) = (void *)ViaMbk;

    GraalInsertRectangle( ScanRec );

    if ( GRAAL_PREVIOUS( ScanRec ) == (rdsrec_list **)NULL )
    {
      Previous = &(GraalFigureRds->LAYERTAB[ GetRdsLayer( ScanRec ) ]);

      for ( ScanPrev  = *Previous;
            ScanPrev != ScanRec;
            ScanPrev  = ScanPrev->NEXT )
      {
        if ( GRAAL_PREVIOUS( ScanPrev ) == (rdsrec_list **)NULL )
        {
          GRAAL_PREVIOUS( ScanPrev ) = Previous;
        }

        Previous = &ScanPrev->NEXT;
      }

      GRAAL_PREVIOUS( ScanRec ) = Previous;

      ScanPrev = ScanPrev->NEXT;

      if ( ( ScanPrev                   != (rdsrec_list  *)NULL ) &&
           ( GRAAL_PREVIOUS( ScanPrev ) != (rdsrec_list **)NULL ) )
      {
        GRAAL_PREVIOUS( ScanPrev ) = &ScanRec->NEXT;
      }
    }

    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while ( ScanRec != FirstRec );

  rdsend();
  return( FirstRec );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graal Add Reference                     |
|                                                             |
\------------------------------------------------------------*/
 
rdsrec_list *GraalAddReference( ReferenceMbk )

   phref_list *ReferenceMbk;
{
  rdsrec_list  *ScanRec;
  rdsrec_list  *ScanPrev;
  rdsrec_list  *FirstRec;
  rdsrec_list **Previous;

  rdsbegin();

  ScanRec = FirstRec = refmbkrds( GraalFigureRds, ReferenceMbk );

  if ( FirstRec == (rdsrec_list *)NULL )
  {
    rdsend();
    return( FirstRec );
  }

  do 
  {
    GRAAL_MBK( ScanRec ) = (void *)ReferenceMbk;

    GraalInsertRectangle( ScanRec );

    if ( GRAAL_PREVIOUS( ScanRec ) == (rdsrec_list **)NULL )
    {
      Previous = &(GraalFigureRds->LAYERTAB[ GetRdsLayer( ScanRec ) ]);

      for ( ScanPrev  = *Previous;
            ScanPrev != ScanRec;
            ScanPrev  = ScanPrev->NEXT )
      {
        if ( GRAAL_PREVIOUS( ScanPrev ) == (rdsrec_list **)NULL )
        {
          GRAAL_PREVIOUS( ScanPrev ) = Previous;
        }

        Previous = &ScanPrev->NEXT;
      }

      GRAAL_PREVIOUS( ScanRec ) = Previous;

      ScanPrev = ScanPrev->NEXT;

      if ( ( ScanPrev                   != (rdsrec_list  *)NULL ) &&
           ( GRAAL_PREVIOUS( ScanPrev ) != (rdsrec_list **)NULL ) )
      {
        GRAAL_PREVIOUS( ScanPrev ) = &ScanRec->NEXT;
      }
    }

    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while ( ScanRec != FirstRec );

  rdsend();
  return( FirstRec );
}

/*------------------------------------------------------------\
|                                                             |
|                       Graal Add Abox                        |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *GraalAddAbox()

{
  rdsrec_list *Rectangle;

  rdsbegin();

  Rectangle = aboxmbkrds( GraalFigureMbk, GraalFigureRds );

  if ( Rectangle != (rdsrec_list *)NULL )
  {
    GRAAL_PREVIOUS( Rectangle ) =

      &(GraalFigureRds->LAYERTAB[ RDS_ABOX ]);

    if ( Rectangle->NEXT != (rdsrec_list *)NULL )
    {
      GRAAL_PREVIOUS( Rectangle->NEXT ) = &Rectangle->NEXT;
    }

    GRAAL_MBK( Rectangle ) = (void *)GraalFigureMbk;

    GraalInsertRectangle( Rectangle );
  }
 
  rdsend();
  return( Rectangle );
}

/*------------------------------------------------------------\
|                                                             |
|                    Graal Delete Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Graal Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

void GraalDelFigure()

{
  char         Layer;
  rdsrec_list *DelRec;
  rdsrec_list *ScanRec;
  rdsins_list *ScanIns;
  rdsins_list *DelIns;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    ScanRec  = GraalFigureRds->LAYERTAB[ Layer ];  

    while ( ScanRec != (rdsrec_list *)NULL )
    {
      DelRec   = ScanRec;
      ScanRec  = ScanRec->NEXT;

      GraalEraseRecWin( DelRec );

      freerdsrec( DelRec, GRAAL_SIZE );
    }

    GraalFigureRds->LAYERTAB[ Layer ] = (rdsrec_list *)NULL;
  }

  ScanIns  = GraalFigureRds->INSTANCE;

  while ( ScanIns != (rdsins_list *)NULL )
  {
    DelIns   = ScanIns;
    ScanIns  = ScanIns->NEXT;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      ScanRec  = GraalFigureRds->LAYERTAB[ Layer ];  

      while ( ScanRec != (rdsrec_list *)NULL )
      {
        DelRec   = ScanRec;
        ScanRec  = ScanRec->NEXT;

        GraalEraseRecWin( DelRec );
      
        freerdsrec( DelRec, GRAAL_SIZE );
      }
    }

    freerdsins( DelIns );
  }

  GraalFigureRds->INSTANCE = (rdsins_list *)NULL;

  while ( HEAD_RDSFIG != (rdsfig_list *)0 )
  {
    delrdsfig( HEAD_RDSFIG->NAME );
  }

  while ( HEAD_PHFIG != (phfig_list *)0 )
  {
    delphfig( HEAD_PHFIG->NAME );
  }

  GraalDelAllConRec();

  GraalFigureRds = (rdsfig_list *)NULL;
  GraalFigureMbk = (phfig_list  *)NULL;

  GraalEraseWindow();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Graal Del Instance                     |
|                                                             |
\------------------------------------------------------------*/

void GraalDelInstance( InstanceRds )

  rdsins_list *InstanceRds;
{
  phins_list  *InstanceMbk;
  rdsrec_list *ScanRec;
  rdsrec_list *DelRec;
  char         Layer;
 
   rdsbegin();
 
  *(rdsins_list **)(InstanceRds->USER) = InstanceRds->NEXT; 
 
  if ( InstanceRds->NEXT != (rdsins_list *)NULL )
  {
    InstanceRds->NEXT->USER = InstanceRds->USER;
  }
 
  InstanceMbk = (phins_list *)GRAAL_MBK( InstanceRds->LAYERTAB[ RDS_ABOX ] );
 
  *(phins_list **)(InstanceMbk->USER) = InstanceMbk->NEXT;
 
  if ( InstanceMbk->NEXT != (phins_list *)NULL )
  {
    InstanceMbk->NEXT->USER = InstanceMbk->USER;
  }
 
  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    ScanRec = InstanceRds->LAYERTAB[ Layer ];
 
    while( ScanRec != (rdsrec_list *)NULL )
    {
      DelRec  = ScanRec;
      ScanRec = ScanRec->NEXT;
 
      GraalEraseRectangle( DelRec );
 
      freerdsrec( DelRec, GRAAL_SIZE );
    }
  }
 
  freerdsins( InstanceRds );
  mbkfree( InstanceMbk );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Graal Del Rectangle                    |
|                                                             |
\------------------------------------------------------------*/

void GraalDelRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;
  rdsrec_list *DelRec;
  void        *Pointer;

  rdsbegin();

  Pointer = GRAAL_MBK( Rectangle );

  if ( IsRdsSegment( Rectangle ) )
  { 
    *((phseg_list **)(((phseg_list *)Pointer)->USER)) = ((phseg_list *)Pointer)->NEXT;

    if ( ((phseg_list *)Pointer)->NEXT != (phseg_list *)NULL )
    {
      ((phseg_list *)Pointer)->NEXT->USER = ((phseg_list *)Pointer)->USER;
    }

    mbkfree( Pointer );
  }
  else
  if ( IsRdsVia( Rectangle ) )
  {
    *(phvia_list **)(((phvia_list *)Pointer)->USER) = ((phvia_list *)Pointer)->NEXT;

    if (((phvia_list *)Pointer)->NEXT != (phvia_list *)NULL )
    {
      ((phvia_list *)Pointer)->NEXT->USER = ((phvia_list *)Pointer)->USER;
    }

    mbkfree( Pointer );
  }
  else
  if ( IsRdsConnector( Rectangle ) )
  { 
    *(phcon_list **)(((phcon_list *)Pointer)->USER) = ((phcon_list *)Pointer)->NEXT;

    if (((phcon_list *)Pointer)->NEXT != (phcon_list *)NULL )
    {
      ((phcon_list *)Pointer)->NEXT->USER = ((phcon_list *)Pointer)->USER;
    }

    GraalDelConRec( Rectangle );

    mbkfree( Pointer );
  }
  else
  if ( IsRdsReference( Rectangle ) )
  {
    *(phref_list **)(((phref_list *)Pointer)->USER) = ((phref_list *)Pointer)->NEXT;

    if (((phref_list *)Pointer)->NEXT != (phref_list *)NULL )
    {
      ((phref_list *)Pointer)->NEXT->USER = ((phref_list *)Pointer)->USER;
    }

    mbkfree( Pointer );
  }
/*\
  else
  if ( IsRdsFigure( Rectangle ) )
  {
    if ( GraalFigureRds->LAYERTAB[ RDS_ABOX ] == Rectangle )
    {
      GraalFigureMbk->XAB1 = 0;
      GraalFigureMbk->YAB1 = 0;
      GraalFigureMbk->XAB2 = 0;
      GraalFigureMbk->YAB2 = 0;
    }
  }
\*/

  ScanRec = Rectangle;

  do
  {
    DelRec  = ScanRec;
    ScanRec = (rdsrec_list *)(ScanRec->USER);

    *(GRAAL_PREVIOUS( DelRec )) = DelRec->NEXT;

    if ( DelRec->NEXT != (rdsrec_list *)NULL )
    {
      GRAAL_PREVIOUS( DelRec->NEXT ) = GRAAL_PREVIOUS( DelRec );
    }

    GraalEraseRectangle( DelRec );

    freerdsrec( DelRec, GRAAL_SIZE );
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Graal Save Figure                      |
|                                                             |
\------------------------------------------------------------*/

char GraalSaveFigure()

{
  rdsbegin();

  GraalDelUndo( &GraalHeadRedo );
  GraalDelUndo( &GraalHeadUndo );
 
  rdsend();
  return( Graalsavephfig( GraalFigureMbk ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graalgetphfig                           |
|                                                             |
\------------------------------------------------------------*/

phfig_list *Graalgetphfig( FigureName, Mode )

   char *FigureName;
   char  Mode;
{
  phfig_list *FigureMbk;

  rdsbegin();

  GraalActiveExitHandler();

  if ( setjmp( GraalJumpBuffer ) == 0 )
  {
    FigureMbk = getphfig( FigureName, Mode );

    GraalResetExitHandler();

    if ( ( FigureMbk->XAB1 == FigureMbk->XAB2 ) &&
         ( FigureMbk->YAB1 == FigureMbk->YAB2 ) ) 
    {
      FigureMbk->XAB1 = 0;
      FigureMbk->XAB2 = 0;
      FigureMbk->YAB1 = 0;
      FigureMbk->YAB2 = 0;
    }

    rdsend();
    return( FigureMbk );
  }

  GraalResetExitHandler();

  delphfig( FigureName );

  rdsend();
  return( (phfig_list *)NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graalrflattenphfig                      |
|                                                             |
\------------------------------------------------------------*/

phfig_list *Graalrflattenphfig( FigureMbk, Concat, Catal )

   phfig_list *FigureMbk;
   char        Concat;
   char        Catal;
{
  rdsbegin();

  GraalActiveExitHandler();

  if ( setjmp( GraalJumpBuffer ) == 0 )
  {
    rflattenphfig( FigureMbk, Concat, Catal );

    GraalResetExitHandler();

    rdsend();
    return( FigureMbk );
  }

  GraalResetExitHandler();

  rdsend();
  return( (phfig_list *)NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                      Graalflattenphfig                      |
|                                                             |
\------------------------------------------------------------*/

phfig_list *Graalflattenphfig( FigureMbk, Name, Concat )

   phfig_list *FigureMbk;
   char       *Name;
   char        Concat;
{
  rdsbegin();

  GraalActiveExitHandler();

  if ( setjmp( GraalJumpBuffer ) == 0 )
  {
    flattenphfig( FigureMbk, Name, Concat );

    GraalResetExitHandler();

    rdsend();
    return( FigureMbk );
  }

  GraalResetExitHandler();

  rdsend();
  return( (phfig_list *)NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graalsavephfig                          |
|                                                             |
\------------------------------------------------------------*/

char Graalsavephfig( FigureMbk )

   phfig_list *FigureMbk;
{
  rdsbegin();

  GraalActiveExitHandler();

  if ( setjmp( GraalJumpBuffer ) == 0 )
  {
    savephfig( FigureMbk );

    GraalResetExitHandler();
 
    rdsend();
    return( GRAAL_TRUE );
  } 

  GraalResetExitHandler();
 
  rdsend();
  return( GRAAL_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graaldrucrdsfig                         |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *Graaldrucrdsfig( Figure )

   rdsfig_list *Figure;
{
  rdsfig_list *FigureError;

  rdsbegin();

  GraalActiveExitHandler();

  if ( setjmp( GraalJumpBuffer ) == 0 )
  {
    FigureError = drucrdsfig( Figure, 
                              RDS_TECHNO_NAME,
                              "_druc_err_" );
    GraalResetExitHandler();

    rdsend();
    return( FigureError );
  }

  GraalResetExitHandler();

  delrdsfig( Figure->NAME );

  rdsend();
  return( (rdsfig_list *)NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                     Graalgetrdsfig                          |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *Graalgetrdsfig( FigureName, Mode )

   char        *FigureName;
   char         Mode;
{
  rdsfig_list *FigureRds;

  rdsbegin();

  FigureName = namealloc( FigureName );
  GraalActiveExitHandler();

  if ( setjmp( GraalJumpBuffer ) == 0 )
  {
    FigureRds = getrdsfig( FigureName, Mode, GRAAL_SIZE );

    GraalResetExitHandler();

    rdsend();
    return( FigureRds );
  }

  GraalResetExitHandler();

  delrdsfig( FigureName );

  rdsend();
  return( (rdsfig_list *)NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                    Graalrflattenrdsfig                      |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *Graalrflattenrdsfig( Figure, Concat, Catal )

   rdsfig_list *Figure;
   char         Concat;
   char         Catal;
{
  rdsbegin();

  GraalActiveExitHandler();

  if ( setjmp( GraalJumpBuffer ) == 0 )
  {
    rflattenrdsfig( Figure, Concat, Catal );

    GraalResetExitHandler();

    rdsend();
    return( Figure );
  }

  GraalResetExitHandler();

  rdsend();
  return( (rdsfig_list *)0 );
}
