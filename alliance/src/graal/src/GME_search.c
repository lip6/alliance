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
| File    :                  Search.c                         |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GSB.h"
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GMV.h"
# include "GME.h"

# include "GME_search.h"
# include "GME_panel.h"
# include "GME_edit.h"
# include "GME_message.h"

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

  char  GraalSearchAbort;
  int   GraalSearchNumber;
  char *GraalSearchString;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   GraalEditInitializeSearch                 |
|                                                             |
\------------------------------------------------------------*/

void GraalEditInitializeSearch( Name )

  char *Name;
{
  char *Star;

  rdsbegin();

  Star = strrchr( Name, '*' );

  if ( Star != (char *)NULL )
  {
    GraalSearchNumber = (int)(Star - Name);
  }
  else
  {
    GraalSearchNumber = 0;
  }

  GraalSearchString = Name;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalEditSearchCompare                    |
|                                                             |
\------------------------------------------------------------*/

int GraalEditSearchCompare( Name )

  char *Name;
{
  if ( Name != (char *)NULL )
  {
    if ( GraalSearchNumber )
    {
      return( ! strncmp( GraalSearchString, Name, GraalSearchNumber ) );
    }
    else
    {
      return( Name == GraalSearchString );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalEditSearchConnector               |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSearchConnector( ConnectorName )

  char *ConnectorName;
{
  graalconrec *ScanCon;
  graalsearch *Search;
  rdsrec_list *Rec;
  long         X1;
  long         Y1;

  rdsbegin();

  GraalEditInitializeSearch( ConnectorName );

  for ( ScanCon  = GraalHeadConRec;
        ScanCon != (graalconrec *)NULL;
        ScanCon  = ScanCon->NEXT )
  {
    Rec = ScanCon->RECTANGLE;

    if ( ( ! IsGraalDeleted( Rec )             ) &&
         ( GraalEditSearchCompare( Rec->NAME ) ) )
    {
      GraalAddSearch( Rec );
    }
  }

  if ( GraalHeadSearch == (graalsearch *)NULL )
  {
    GraalWarningMessage( GraalMainWindow, "No connector found !" );
  }
  else
  {
    for ( Search  = GraalHeadSearch;
          Search != (graalsearch *)NULL;
          Search  = Search->NEXT )
    {
      Rec = Search->RECTANGLE;

      X1 = ( Rec->X + ( Rec->DX >> 1 ) ) / GRAAL_RDS_LAMBDA;
      Y1 = ( Rec->Y + ( Rec->DY >> 1 ) ) / GRAAL_RDS_LAMBDA;

      GraalZoomCenter( X1, Y1 );

      if ( Search->NEXT != (graalsearch *)NULL )
      {
        GraalSearchAbort = GRAAL_FALSE;

        GraalEnterPanel( &GraalEditSearchViewPanel ); 
        GraalLimitedLoop( GraalEditSearchViewPanel.PANEL );

        if ( GraalSearchAbort ) break;
      }
    }

    GraalDelSearch();

    GraalExitPanel( &GraalEditSearchViewPanel );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalEditSearchInstance                |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSearchInstance( InstanceName )

  char *InstanceName;
{
  graalsearch *Search;
  rdsins_list *Instance;
  rdsrec_list *Rec;
  long         X1;
  long         Y1;

  if ( GraalFigureMbk == (phfig_list *)NULL ) return;

  rdsbegin();

  GraalEditInitializeSearch( InstanceName );

  for ( Instance  = GraalFigureRds->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    if ( ( ! IsGraalDeleted( Instance->LAYERTAB[ RDS_ABOX ] ) ) &&
         ( GraalEditSearchCompare( Instance->INSNAME        ) ) )
    {
      GraalAddSearch( Instance->LAYERTAB[ RDS_ABOX ] );
    }
  }

  if ( GraalHeadSearch == (graalsearch *)NULL )
  {
    GraalWarningMessage( GraalMainWindow, "No instance found !" );
  }
  else
  {
    for ( Search  = GraalHeadSearch;
          Search != (graalsearch *)NULL;
          Search  = Search->NEXT )
    {
      Rec = Search->RECTANGLE;

      X1 = ( Rec->X + ( Rec->DX >> 1 ) ) / GRAAL_RDS_LAMBDA;
      Y1 = ( Rec->Y + ( Rec->DY >> 1 ) ) / GRAAL_RDS_LAMBDA;

      GraalZoomCenter( X1, Y1 );

      if ( Search->NEXT != (graalsearch *)NULL )
      {
        GraalSearchAbort = GRAAL_FALSE;

        GraalEnterPanel( &GraalEditSearchViewPanel ); 
        GraalLimitedLoop( GraalEditSearchViewPanel.PANEL );

        if ( GraalSearchAbort ) break;
      }
    }

    GraalDelSearch();

    GraalExitPanel( &GraalEditSearchViewPanel );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalEditSearchSegment                 |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSearchSegment( SegmentName )

  char *SegmentName;
{
  graalsearch *Search;
  rdsins_list *Instance;
  rdsfig_list *Figure;
  rdsrec_list *Rec;
  char         Layer;
  long         X1;
  long         Y1;

  if ( GraalFigureMbk == (phfig_list *)NULL ) return;

  rdsbegin();

  GraalEditInitializeSearch( SegmentName );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rec  = GraalFigureRds->LAYERTAB[ Layer ];
          Rec != (rdsrec_list *)NULL;
          Rec  = Rec->NEXT )
    {
      if ( ( ! IsGraalDeleted( Rec )             ) &&
           ( IsRdsSegment( Rec )                 ) &&
           ( GraalEditSearchCompare( Rec->NAME ) ) )
      {
        GraalAddSearch( Rec );
      }
    }
  }

  for ( Instance  = GraalFigureRds->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    if ( IsGraalDeleted( Instance->LAYERTAB[ RDS_ABOX ] ) ) 

        continue;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      for ( Rec  = Instance->LAYERTAB[ Layer ];
            Rec != (rdsrec_list *)NULL;
            Rec  = Rec->NEXT )
      {
        if ( ( IsRdsSegment( Rec )                 ) &&
             ( GraalEditSearchCompare( Rec->NAME ) ) )
        {
          GraalAddSearch( Rec );
        }
      }  
    }  
  }

  if ( GraalHeadSearch == (graalsearch *)NULL )
  {
    GraalWarningMessage( GraalMainWindow, "No segment found !" );
  }
  else
  {
    for ( Search  = GraalHeadSearch;
          Search != (graalsearch *)NULL;
          Search  = Search->NEXT )
    {
      Rec = Search->RECTANGLE;

      X1 = ( Rec->X + ( Rec->DX >> 1 ) ) / GRAAL_RDS_LAMBDA;
      Y1 = ( Rec->Y + ( Rec->DY >> 1 ) ) / GRAAL_RDS_LAMBDA;

      GraalZoomCenter( X1, Y1 );

      if ( Search->NEXT != (graalsearch *)NULL )
      {
        GraalSearchAbort = GRAAL_FALSE;

        GraalEnterPanel( &GraalEditSearchViewPanel ); 
        GraalLimitedLoop( GraalEditSearchViewPanel.PANEL );

        if ( GraalSearchAbort ) break;
      }
    }

    GraalDelSearch();

    GraalExitPanel( &GraalEditSearchViewPanel );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalEditSearchReference               |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSearchReference( ReferenceName )

  char *ReferenceName;
{
  graalsearch *Search;
  rdsins_list *Instance;
  rdsfig_list *Figure;
  rdsrec_list *Rec;
  char         Layer;
  long         X1;
  long         Y1;

  if ( GraalFigureMbk == (phfig_list *)NULL ) return;

  rdsbegin();

  GraalEditInitializeSearch( ReferenceName );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rec  = GraalFigureRds->LAYERTAB[ Layer ];
          Rec != (rdsrec_list *)NULL;
          Rec  = Rec->NEXT )
    {
      if ( ( ! IsGraalDeleted( Rec )             ) &&
           ( IsRdsReference( Rec )               ) &&
           ( GraalEditSearchCompare( Rec->NAME ) ) )
      {
        GraalAddSearch( Rec );
      }
    }
  }

  for ( Instance  = GraalFigureRds->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    if ( IsGraalDeleted( Instance->LAYERTAB[ RDS_ABOX ] ) ) 

        continue;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      for ( Rec  = Instance->LAYERTAB[ Layer ];
            Rec != (rdsrec_list *)NULL;
            Rec  = Rec->NEXT )
      {
        if ( ( IsRdsReference( Rec )             ) &&
           ( GraalEditSearchCompare( Rec->NAME ) ) )
        {
          GraalAddSearch( Rec );
        }
      }  
    }  
  }

  if ( GraalHeadSearch == (graalsearch *)NULL )
  {
    GraalWarningMessage( GraalMainWindow, "No reference found !" );
  }
  else
  {
    for ( Search  = GraalHeadSearch;
          Search != (graalsearch *)NULL;
          Search  = Search->NEXT )
    {
      Rec = Search->RECTANGLE;

      X1 = ( Rec->X + ( Rec->DX >> 1 ) ) / GRAAL_RDS_LAMBDA;
      Y1 = ( Rec->Y + ( Rec->DY >> 1 ) ) / GRAAL_RDS_LAMBDA;

      GraalZoomCenter( X1, Y1 );

      if ( Search->NEXT != (graalsearch *)NULL )
      {
        GraalSearchAbort = GRAAL_FALSE;

        GraalEnterPanel( &GraalEditSearchViewPanel ); 
        GraalLimitedLoop( GraalEditSearchViewPanel.PANEL );

        if ( GraalSearchAbort ) break;
      }
    }

    GraalDelSearch();

    GraalExitPanel( &GraalEditSearchViewPanel );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalEditSearchVia                  |
|                                                             |
\------------------------------------------------------------*/

void GraalEditSearchVia( ViaName )

  char *ViaName;
{
  graalsearch *Search;
  rdsins_list *Instance;
  rdsfig_list *Figure;
  rdsrec_list *Rec;
  char         Layer;
  long         X1;
  long         Y1;

  if ( GraalFigureMbk == (phfig_list *)NULL ) return;

  rdsbegin();

  GraalEditInitializeSearch( ViaName );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rec  = GraalFigureRds->LAYERTAB[ Layer ];
          Rec != (rdsrec_list *)NULL;
          Rec  = Rec->NEXT )
    {
      if ( ( ! IsGraalDeleted( Rec )             ) &&
           ( IsRdsVia( Rec )                     ) &&
           ( GraalEditSearchCompare( Rec->NAME ) ) )
      {
        GraalAddSearch( Rec );
      }
    }
  }

  for ( Instance  = GraalFigureRds->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    if ( IsGraalDeleted( Instance->LAYERTAB[ RDS_ABOX ] ) ) 

        continue;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      for ( Rec  = Instance->LAYERTAB[ Layer ];
            Rec != (rdsrec_list *)NULL;
            Rec  = Rec->NEXT )
      {
        if ( ( IsRdsVia( Rec )                     ) &&
             ( GraalEditSearchCompare( Rec->NAME ) ) )
        {
          GraalAddSearch( Rec );
        }
      }  
    }  
  }

  if ( GraalHeadSearch == (graalsearch *)NULL )
  {
    GraalWarningMessage( GraalMainWindow, "No via found !" );
  }
  else
  {
    for ( Search  = GraalHeadSearch;
          Search != (graalsearch *)NULL;
          Search  = Search->NEXT )
    {
      Rec = Search->RECTANGLE;

      X1 = ( Rec->X + ( Rec->DX >> 1 ) ) / GRAAL_RDS_LAMBDA;
      Y1 = ( Rec->Y + ( Rec->DY >> 1 ) ) / GRAAL_RDS_LAMBDA;

      GraalZoomCenter( X1, Y1 );

      if ( Search->NEXT != (graalsearch *)NULL )
      {
        GraalSearchAbort = GRAAL_FALSE;

        GraalEnterPanel( &GraalEditSearchViewPanel ); 
        GraalLimitedLoop( GraalEditSearchViewPanel.PANEL );

        if ( GraalSearchAbort ) break;
      }
    }

    GraalDelSearch();

    GraalExitPanel( &GraalEditSearchViewPanel );
  }

  rdsend();
}

