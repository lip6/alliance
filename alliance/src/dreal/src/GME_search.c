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
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GMV.h"
# include "GME.h"

# include "GME_search.h"
# include "GME_panel.h"
# include "GME_edit.h"
# include "GME_message.h"
# include "GTB_dialog.h"

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

  char  DrealSearchAbort;
  int   DrealSearchNumber;
  char *DrealSearchString;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   DrealEditInitializeSearch                 |
|                                                             |
\------------------------------------------------------------*/

void DrealEditInitializeSearch( Name )

  char *Name;
{
  char *Star;

  rdsbegin();

  Star = strrchr( Name, '*' );

  if ( Star != (char *)NULL )
  {
    DrealSearchNumber = (int)(Star - Name);
  }
  else
  {
    DrealSearchNumber = 0;
  }

  DrealSearchString = Name;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   DrealEditSearchCompare                    |
|                                                             |
\------------------------------------------------------------*/

int DrealEditSearchCompare( Name )

  char *Name;
{
  if ( Name != (char *)NULL )
  {
    if ( DrealSearchNumber )
    {
      return( ! strncmp( DrealSearchString, Name, DrealSearchNumber ) );
    }
    else
    {
      return( Name == DrealSearchString );
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealEditSearchRectangle               |
|                                                             |
\------------------------------------------------------------*/

void DrealEditSearchRectangle( RectangleName )

  char *RectangleName;
{
  drealsearch *Search;
  rdsrec_list *Rec;
  char         Layer;
  long         X1;
  long         Y1;

  if ( DrealFigureRds == (rdsfig_list *)NULL ) return;

  rdsbegin();

  DrealEditInitializeSearch( RectangleName );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rec  = DrealFigureRds->LAYERTAB[ (int)Layer ];
          Rec != (rdsrec_list *)NULL;
          Rec  = Rec->NEXT )
    {
      if ( ( ! IsDrealDeleted( Rec )             ) &&
           ( DrealEditSearchCompare( Rec->NAME ) ) )
      {
        DrealAddSearch( Rec );
      }
    }
  }

  if ( DrealHeadSearch == (drealsearch *)NULL )
  {
    DrealWarningMessage( DrealMainWindow, "No rectangle found !" );
  }
  else
  {
    for ( Search  = DrealHeadSearch;
          Search != (drealsearch *)NULL;
          Search  = Search->NEXT )
    {
      Rec = Search->RECTANGLE;

      X1 = ( Rec->X + ( Rec->DX >> 1 ) ) / RDS_PHYSICAL_GRID;
      Y1 = ( Rec->Y + ( Rec->DY >> 1 ) ) / RDS_PHYSICAL_GRID;

      DrealZoomCenter( X1, Y1 );

      if ( Search->NEXT != (drealsearch *)NULL )
      {
        DrealSearchAbort = DREAL_FALSE;

        DrealEnterPanel( &DrealEditSearchViewPanel ); 
        DrealLimitedLoop( DrealEditSearchViewPanel.PANEL );

        if ( DrealSearchAbort ) break;
      }
    }

    DrealDelSearch();

    DrealExitPanel( &DrealEditSearchViewPanel );
  }

  rdsend();
}
