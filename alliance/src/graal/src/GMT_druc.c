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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Druc.c                           |
|                                                             |
| Author  :              Jacomme Ludovic                      |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rfm.h"
# include "rpr.h"
# include "vrd.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GRM.h"
# include "GMT.h"

# include "GMT_druc.h"
# include "GMT_panel.h"
# include "GMT_message.h"

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

  char GraalDrucErrorMessage[ GRAAL_DRUC_MESSAGE_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   GraalToolsDrucNoMoreError                 |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayDrucNoMoreError()
{
  rdsbegin();

  sprintf( GraalDrucErrorMessage, "No more error\n" );
  GraalDisplayDrucMessage( GraalDrucErrorMessage );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalToolsDrucError                    |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayDrucError( Rectangle )

  rdsrec_list *Rectangle;
{
  char *Comment;
  long  Error;

  rdsbegin();

  Error   = MACRO_REGLE_NUM( Rectangle );
  Comment = drucgeterror( Error );

  sprintf( GraalDrucErrorMessage, "Rule error %d\n%s", 
           Error, Comment );

  GraalDisplayDrucMessage( GraalDrucErrorMessage );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalToolsDrucAbortError                 |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsDrucAbortError()
{
  if ( GraalRectangleDruc != (rdsrec_list *)0 )
  {
    GraalDeleteRectangle( GraalRectangleDruc );
    GraalDisplayRectangle( GraalRectangleDruc );
  }

  GraalDelDrucFigure();
  GraalExitPanel( &GraalToolsDrucPanel );
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalToolsDrucNextError                  |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsDrucNextError()
{
  rdsrec_list  *DrucRec;
  rdsrec_list  *ScanRec;
  char          Layer;
  char          Error;
  long          X1;
  long          Y1;


  if ( GraalFigureDruc != (rdsfig_list *)0 )
  {
    if ( GraalRectangleDruc != (rdsrec_list *)0 )
    {
      GraalDeleteRectangle( GraalRectangleDruc );
      GraalDisplayRectangle( GraalRectangleDruc );
      GraalDelDruc( GraalRectangleDruc );

      GraalRectangleDruc = (rdsrec_list *)0;
    }

    DrucRec = (rdsrec_list *)0;

    if ( GraalCurrentDruc == (rdsrec_list *)0 )
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        for ( GraalCurrentDruc  = GraalFigureDruc->LAYERTAB[ Layer ];
              GraalCurrentDruc != (rdsrec_list *)0;
              GraalCurrentDruc  = GraalCurrentDruc->NEXT )
        {
          DrucRec = GraalAddDruc( GraalCurrentDruc );
          if ( DrucRec != (rdsrec_list *)0 ) break;
        }

        if ( DrucRec != (rdsrec_list *)0 ) break;
      }
    }
    else
    {
      ScanRec = GraalCurrentDruc->NEXT;

      while ( ScanRec != (rdsrec_list *)0 )
      {
        DrucRec = GraalAddDruc( ScanRec );
        if ( DrucRec != (rdsrec_list *)0 ) break;

        ScanRec = ScanRec->NEXT;
      }

      if ( ScanRec == (rdsrec_list *)0 )
      {
        for ( Layer = GetRdsLayer( GraalCurrentDruc ) + 1; Layer < RDS_MAX_LAYER; Layer++ )
        {
          for ( GraalCurrentDruc  = GraalFigureDruc->LAYERTAB[ Layer ];
                GraalCurrentDruc != (rdsrec_list *)0;
                GraalCurrentDruc  = GraalCurrentDruc->NEXT )
          {
            DrucRec = GraalAddDruc( GraalCurrentDruc );
            if ( DrucRec != (rdsrec_list *)0 ) break;
          }

          if ( DrucRec != (rdsrec_list *)0 ) break;
        }
      }
      else
      {
        GraalCurrentDruc = ScanRec;
      }
    }

    if ( GraalCurrentDruc == (rdsrec_list *)0 )
    {
      GraalDisplayDrucNoMoreError();

      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        for ( ScanRec  = GraalFigureDruc->LAYERTAB[ Layer ];
              ScanRec != (rdsrec_list *)0;
              ScanRec  = ScanRec->NEXT )
        {
          ClearGraalDruc( ScanRec );
        }
      }
    }
    else
    {
      GraalRectangleDruc = DrucRec;
      GraalDisplayDrucError( GraalCurrentDruc );

      X1 = ( DrucRec->X + ( DrucRec->DX >> 1 ) ) / GRAAL_RDS_LAMBDA;
      Y1 = ( DrucRec->Y + ( DrucRec->DY >> 1 ) ) / GRAAL_RDS_LAMBDA;

      if ( ( X1 < GraalLambdaGridX                       ) ||
           ( Y1 < GraalLambdaGridY                       ) ||
           ( X1 > (GraalLambdaGridX + GraalLambdaGridDx) ) ||
           ( Y1 > (GraalLambdaGridY + GraalLambdaGridDy) ) )
      {
        GraalZoomCenter( X1, Y1 );
      }
      else
      {
        GraalDisplayRectangle( DrucRec );
      }

      GraalEnterPanel( &GraalToolsDrucPanel );
    }
  }
  else
  {
    GraalExitPanel( &GraalToolsDrucPanel );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalToolsDruc                       |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsDruc( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **Previous;
  rdsrec_list  *Rectangle;
  rdsrec_list **PrevRectangle;
  rdsrec_list  *DrucRec;
  void         *Pointer;
  char          Layer;
  char          Error;
  long          X1;
  long          Y1;

  rdsbegin();

  if ( GraalFigureDruc != (rdsfig_list *)0 )
  {
    GraalDelDrucFigure();
  }

  GraalEditSelectFence( LambdaX1, LambdaY1, LambdaX2, LambdaY2 );

  Previous = &GraalHeadSelect;
  Select   = GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( IsRdsFigure( Select->RECTANGLE   ) ||
         IsRdsInstance( Select->RECTANGLE ) )
    {
      DelSelect = Select;
      Select    = Select->NEXT;
      *Previous = Select;
 
      GraalFreeSelect( DelSelect );
    }
    else
    {
      GraalAcceptRectangle( Select->RECTANGLE );
 
      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }
 
  if ( GraalHeadSelect == (graalselect *)NULL  )
  {
    GraalWarningMessage( GraalMainWindow, "No element found !" );
  }
  else
  {
    GraalFigureDruc = addrdsfig( "_patrick_", MACRO_PLUS_TAILLE );

    for ( Select  = GraalHeadSelect;
          Select != (graalselect *)NULL;
          Select  = Select->NEXT )
    {
      Rectangle = Select->RECTANGLE;
      Pointer   = GRAAL_MBK( Rectangle );

      if ( IsRdsSegment( Rectangle ) )
      {
        segmbkrds( GraalFigureDruc, (phseg_list *)Pointer, 0 );
      }
      else
      if ( IsRdsConnector( Rectangle ) )
      {
        conmbkrds( GraalFigureDruc, (phcon_list *)Pointer );
      }
      else
      if ( IsRdsReference( Rectangle ) )
      {
        refmbkrds( GraalFigureDruc, (phref_list *)Pointer );
      }
      else
      if ( IsRdsVia( Rectangle ) )
      {
        viambkrds( GraalFigureDruc, (phvia_list *)Pointer, 0 );
      }
    }

    GraalDelSelect();

    if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) ||
         ( GraalHeadPeek != (graalpeek   *)NULL ) )
    {
      GraalDelEqui();
      GraalDelPeek();
      GraalZoomRefresh();
    }

    GraalFigureDruc = Graaldrucrdsfig( GraalFigureDruc );

    GraalDisplayToolsMessage();

    if ( GraalFigureDruc == (rdsfig_list *)NULL )
    {
      GraalErrorMessage( GraalMainWindow, "Unable to execute druc !" );
    }
    else
    {
      Error = GRAAL_FALSE;

      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        for ( Rectangle  = GraalFigureDruc->LAYERTAB[ Layer ];
              Rectangle != (rdsrec_list *)NULL;    
              Rectangle  = Rectangle->NEXT )
        {
          ClearGraalDruc( Rectangle ); Error = GRAAL_TRUE;
        }
      }

      if ( Error == GRAAL_FALSE )
      {
        GraalWarningMessage( GraalMainWindow, "No design error !" );
        GraalDelDrucFigure();
        GraalExitPanel( &GraalToolsDrucPanel );
      }
      else
      {
        PrevRectangle = &GraalFigureDruc->LAYERTAB[ RDS_USER0 ];

        for ( Layer = RDS_USER0; Layer <= RDS_USER8; Layer++ )
        {
          *PrevRectangle = GraalFigureDruc->LAYERTAB[ Layer ];

          for ( Rectangle  = GraalFigureDruc->LAYERTAB[ Layer ];
                Rectangle != (rdsrec_list *)0;
                Rectangle  = Rectangle->NEXT )
          {
            SetRdsLayer( Rectangle, RDS_USER0 );
            PrevRectangle = &Rectangle->NEXT;
          }

          GraalFigureDruc->LAYERTAB[ Layer ] = (rdsrec_list *)0;
        }

        *PrevRectangle = GraalFigureDruc->LAYERTAB[ RDS_USER9 ];
        GraalFigureDruc->LAYERTAB[ RDS_USER9 ] = (rdsrec_list *)0;
        
        GraalCurrentDruc = (rdsrec_list *)0;
        GraalToolsDrucNextError();
      }
    }
  }

  rdsend();
}
