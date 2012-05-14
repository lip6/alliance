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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Rds.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rtl.h"
# include "rpr.h"
# include "rfm.h"
# include "GSB.h" 
# include "GRD.h"
# include "GRD_error.h"
# include "GRD_window.h"
# include "GRD_rds.h"

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

  static jmp_buf DrealJumpBuffer;
  static void    (*OldExitHandler)() = NULL;

/*------------------------------------------------------------\
|                                                             |
|                         Dreal Figure                        |
|                                                             |
\------------------------------------------------------------*/

  rdsfig_list *DrealFigureRds     = (rdsfig_list *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                         Dreal Abox                          |
|                                                             |
\------------------------------------------------------------*/

  long DrealFigureXAB1 = 0;
  long DrealFigureXAB2 = 0;
  long DrealFigureYAB1 = 0;
  long DrealFigureYAB2 = 0;

/*------------------------------------------------------------\
|                                                             |
|                 Dreal Exit Handler Functions                |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealExitHandler                       |
|                                                             |
\------------------------------------------------------------*/

void DrealExitHandler()
{
  DrealFlushErrorMessage();

  longjmp( DrealJumpBuffer, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealActiveExitHandler                 |
|                                                             |
\------------------------------------------------------------*/

void DrealActiveExitHandler()
{
  MBK_EXIT_FUNCTION = DrealExitHandler;
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealResetExitHandler                 |
|                                                             |
\------------------------------------------------------------*/
 
void DrealResetExitHandler() 
{ 
  MBK_EXIT_FUNCTION = NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                     Dreal Add Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Dreal New Figure                        |
|                                                             |
\------------------------------------------------------------*/

void DrealNewFigure( Name )

  char *Name;
{
  rdsbegin();

  DrealFigureRds = addrdsfig( Name, DREAL_SIZE );

  DrealFigureXAB1 = 0;
  DrealFigureXAB2 = 0;
  DrealFigureYAB1 = 0;
  DrealFigureYAB2 = 0;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Dreal Add Figure                        |
|                                                             |
\------------------------------------------------------------*/

void DrealAddFigure( Name )

  char *Name;
{
  char          Layer;
  rdsrec_list  *Rectangle;
  rdsrec_list  *Abox;
  rdsrec_list **Previous;
  rdsins_list  *Instance;

  rdsbegin();

  DrealFigureRds = Drealgetrdsfig( Name, 'V', DREAL_SIZE );

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    Abox = DrealFigureRds->LAYERTAB[ RDS_ABOX ];

    if ( Abox != (rdsrec_list *)0 )
    {
      DrealCheckWindow( Abox->X           , Abox->Y,
                        Abox->X + Abox->DX, Abox->Y + Abox->DY );

      DrealFigureXAB1 = Abox->X;
      DrealFigureXAB2 = Abox->X + Abox->DX;
      DrealFigureYAB1 = Abox->Y;
      DrealFigureYAB2 = Abox->Y + Abox->DY;
    }

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      Previous = &DrealFigureRds->LAYERTAB[ Layer ];

      for ( Rectangle  = DrealFigureRds->LAYERTAB[ Layer ];
            Rectangle != (rdsrec_list *)NULL;
            Rectangle  = Rectangle->NEXT )
      {
        DREAL_PREVIOUS_L( Rectangle ) = Previous;
        Previous = &Rectangle->NEXT;

        DrealInsertRectangle( Rectangle );
      }
    }

    for ( Instance  = DrealFigureRds->INSTANCE;
          Instance != (rdsins_list *)NULL;
          Instance  = Instance->NEXT )
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        for ( Rectangle  = Instance->LAYERTAB[ Layer ];
              Rectangle != (rdsrec_list *)NULL;
              Rectangle  = Rectangle->NEXT )
        {
          DREAL_PREVIOUS_L( Rectangle ) = (rdsrec_list **)Instance;

          DrealInsertRectangle( Rectangle );
        }
      }
    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Dreal Add Rectangle                     |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *DrealAddRectangle( Name, Layer, X, Y, Dx, Dy )

   char        *Name;
   char         Layer;
   long         X;
   long         Y;
   long         Dx;
   long         Dy;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = addrdsfigrec( DrealFigureRds, Name, Layer, X, Y, Dx, Dy );

  DrealInsertRectangle( ScanRec );

  if ( ScanRec->NEXT != (rdsrec_list *)0 )
  {
    DREAL_PREVIOUS_L( ScanRec->NEXT ) = &ScanRec->NEXT;
  }

  DREAL_PREVIOUS_L( ScanRec ) = &DrealFigureRds->LAYERTAB[ Layer ];

  rdsend();
  return( ScanRec );
}

/*------------------------------------------------------------\
|                                                             |
|                     Dreal Add Abox                          |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *DrealAddAbox( X, Y, Dx, Dy )

   long         X;
   long         Y;
   long         Dx;
   long         Dy;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = addrdsfigrec( DrealFigureRds, DrealFigureRds->NAME, RDS_ABOX, X, Y, Dx, Dy );

  SetRdsFigure( ScanRec );

  DrealFigureXAB1 = X;
  DrealFigureXAB2 = X + Dx;
  DrealFigureYAB1 = Y;
  DrealFigureYAB2 = Y + Dy;

  DrealInsertRectangle( ScanRec );

  if ( ScanRec->NEXT != (rdsrec_list *)0 )
  {
    DREAL_PREVIOUS_L( ScanRec->NEXT ) = &ScanRec->NEXT;
  }

  DREAL_PREVIOUS_L( ScanRec ) = &DrealFigureRds->LAYERTAB[ RDS_ABOX ];

  rdsend();
  return( ScanRec );
}

/*------------------------------------------------------------\
|                                                             |
|                    Dreal Delete Functions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Dreal Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

void DrealDelFigure()

{
  char         Layer;
  rdsrec_list *DelRec;
  rdsrec_list *ScanRec;
  rdsins_list *ScanIns;
  rdsins_list *DelIns;
  rdsfig_list *ScanFig;

  rdsbegin();

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    ScanRec  = DrealFigureRds->LAYERTAB[ Layer ];  

    while ( ScanRec != (rdsrec_list *)NULL )
    {
      DelRec   = ScanRec;
      ScanRec  = ScanRec->NEXT;

      DrealEraseRecWin( DelRec );

      freerdsrec( DelRec, DREAL_SIZE );
    }

    DrealFigureRds->LAYERTAB[ Layer ] = (rdsrec_list *)NULL;
  }

  ScanIns  = DrealFigureRds->INSTANCE;

  while ( ScanIns != (rdsins_list *)NULL )
  {
    DelIns   = ScanIns;
    ScanIns  = ScanIns->NEXT;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      ScanRec  = DrealFigureRds->LAYERTAB[ Layer ];  

      while ( ScanRec != (rdsrec_list *)NULL )
      {
        DelRec   = ScanRec;
        ScanRec  = ScanRec->NEXT;

        DrealEraseRecWin( DelRec );
      
        freerdsrec( DelRec, DREAL_SIZE );
      }
    }

    freerdsins( DelIns );
  }

  DrealFigureRds->INSTANCE = (rdsins_list *)NULL;
  DrealEraseWindow();

  while ( HEAD_RDSFIG != (rdsfig_list *)NULL )
  {
    delrdsfig( HEAD_RDSFIG->NAME );
  }

  DrealFigureRds = (rdsfig_list *)NULL;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Del Rectangle                    |
|                                                             |
\------------------------------------------------------------*/

void DrealDelRectangle( DelRec )

  rdsrec_list *DelRec;
{
  *DREAL_PREVIOUS( DelRec ) = DelRec->NEXT;

  if ( DelRec->NEXT != (rdsrec_list *)0 )
  {
    DREAL_PREVIOUS_L( DelRec->NEXT ) = DREAL_PREVIOUS( DelRec );
  }

  DrealEraseRectangle( DelRec );

  freerdsrec( DelRec, DREAL_SIZE );
}

/*------------------------------------------------------------\
|                                                             |
|                  Dreal Flatten Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Dreal Flatten Figure                     |
|                                                             |
\------------------------------------------------------------*/

void DrealFlattenFigure()
{
  rdsfig_list  *Figure;
  rdsins_list  *ScanIns;
  rdsrec_list  *ScanRec;
  rdsrec_list  *DelRec;
  rdsrec_list **Previous;
  char          Layer;

  rdsbegin();

  for ( ScanIns  = DrealFigureRds->INSTANCE;
        ScanIns != (rdsins_list *)NULL;
        ScanIns  = ScanIns->NEXT )
  {
    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      ScanRec = ScanIns->LAYERTAB[ Layer ];

      ScanIns->LAYERTAB[ Layer ] = (rdsrec_list *)NULL;

      while ( ScanRec  != (rdsrec_list *)NULL )
      {
        DelRec  = ScanRec;
        ScanRec = ScanRec->NEXT;

        DrealEraseRectangle( DelRec );

        freerdsrec( DelRec, DREAL_SIZE );
      }
    }
  }

  Figure = addrdsfig( "_ludo_", DREAL_SIZE );
  Figure->INSTANCE = DrealFigureRds->INSTANCE;
  DrealFigureRds->INSTANCE = (rdsins_list *)NULL;

  Drealrflattenrdsfig( Figure, RDS_YES, RDS_NO );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    Previous = &DrealFigureRds->LAYERTAB[ Layer ];
    ScanRec  = Figure->LAYERTAB[ Layer ]; 

    while ( ScanRec != (rdsrec_list *)NULL )
    {
      DREAL_PREVIOUS_L( ScanRec ) = Previous;
      Previous = &ScanRec->NEXT;

      DrealInsertRectangle( ScanRec );

      if ( ScanRec->NEXT == (rdsrec_list *)NULL )
      {
        ScanRec->NEXT = DrealFigureRds->LAYERTAB[ Layer ];
        DrealFigureRds->LAYERTAB[ Layer ] = Figure->LAYERTAB[ Layer ];
        Figure->LAYERTAB[ Layer ] = (rdsrec_list *)NULL;

        if ( ScanRec->NEXT != (rdsrec_list *)NULL )
        {
          DREAL_PREVIOUS_L( ScanRec->NEXT ) = &ScanRec->NEXT;
        }

        break;
      }

      ScanRec = ScanRec->NEXT;
    }
  }

  delrdsfig( "_ludo_" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Dreal Save Figure                      |
|                                                             |
\------------------------------------------------------------*/

char DrealSaveFigure()

{
  rdsbegin();

  DrealDelUndo( &DrealHeadRedo );
  DrealDelUndo( &DrealHeadUndo );

  rdsend();
  return( Drealsaverdsfig( DrealFigureRds ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    Drealgetrdsfig                           |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *Drealgetrdsfig( FigureName, Mode )

   char        *FigureName;
   char         Mode;
{
  rdsfig_list *FigureRds;

  rdsbegin();

  FigureName = namealloc( FigureName );

  DrealActiveExitHandler();

  if ( setjmp( DrealJumpBuffer ) == 0 )
  {
    FigureRds = getrdsfig( FigureName, Mode, DREAL_SIZE );

    DrealResetExitHandler();

    rdsend();
    return( FigureRds );
  }

  DrealResetExitHandler();

  delrdsfig( FigureName );

  rdsend();
  return( (rdsfig_list *)NULL );
}

/*------------------------------------------------------------\
|                                                             |
|                    Drealrflattenrdsfig                      |
|                                                             |
\------------------------------------------------------------*/

void Drealrflattenrdsfig( Figure, Concat, Catal )

   rdsfig_list *Figure;
   char         Concat;
   char         Catal;
{
  rdsbegin();

  DrealActiveExitHandler();

  if ( setjmp( DrealJumpBuffer ) == 0 )
  {
    rflattenrdsfig( Figure, Concat, Catal );
  }

  DrealResetExitHandler();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Drealsaverdsfig                         |
|                                                             |
\------------------------------------------------------------*/

char Drealsaverdsfig( FigureRds )

   rdsfig_list *FigureRds;
{
  rdsbegin();

  DrealActiveExitHandler();

  if ( setjmp( DrealJumpBuffer ) == 0 )
  {
    saverdsfig( FigureRds );

    DrealResetExitHandler();

    rdsend();
    return( DREAL_TRUE );
  }

  DrealResetExitHandler();

  rdsend();
  return( DREAL_FALSE );
}
