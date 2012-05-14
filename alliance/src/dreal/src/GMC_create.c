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
| File    :                  Create.c                         |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# include <Xm/CascadeBG.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMC.h"

# include "GMC_create.h"
# include "GMC_dialog.h"

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
|                           Rectangle                         |
|                                                             |
\------------------------------------------------------------*/

  char  DrealRectangleLayer = 0;
  char *DrealRectangleName  = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealCreateFigureRds                   |
|                                                             |
\------------------------------------------------------------*/

void DrealCreateFigureRds()

{
  if ( DrealFigureRds == (rdsfig_list *)NULL )
  {
    rdsbegin();

    DrealNewFigure( DREAL_DEFAULT_FIGURE_NAME );

    DrealChangeTopLevelTitle( DrealFigureRds->NAME );

    rdsend();
  }
}  

/*------------------------------------------------------------\
|                                                             |
|                     DrealCreateRectangleRds                 |
|                                                             |
\------------------------------------------------------------*/

void DrealCreateRectangleRds( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  rdsrec_list *Rectangle;
  long         Swap;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) &&
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 )
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 )
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }
  }
  else
  {
    DrealErrorMessage( DrealMainWindow, "This rectangle is too small !" );

    rdsend();
    return;
  }

  DrealCreateFigureRds();

  Rectangle = DrealAddRectangle( DrealRectangleName,
                                 DrealRectangleLayer,
                                 LambdaX1 * RDS_PHYSICAL_GRID,
                                 LambdaY1 * RDS_PHYSICAL_GRID,
                                 ( LambdaX2 - LambdaX1 ) * RDS_PHYSICAL_GRID,
                                 ( LambdaY2 - LambdaY1 ) * RDS_PHYSICAL_GRID );

  DrealAddUndo();
  DrealAddUndoRec( Rectangle );

  DrealDisplayRectangle( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   Dreal Create Abutmentbox                  |
|                                                             |
\------------------------------------------------------------*/

void DrealCreateAbutmentBoxRds( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
{
  rdsrec_list *Rectangle;
  long         Swap;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) &&
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 )
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 )
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }
  }  
  else
  {
    DrealErrorMessage( DrealMainWindow, "The abutment box must be rectangular !" );

    rdsend();
    return;
  }

  DrealCreateFigureRds();

  Rectangle = DrealAddAbox( LambdaX1 * RDS_PHYSICAL_GRID,
                            LambdaY1 * RDS_PHYSICAL_GRID,
                            ( LambdaX2 - LambdaX1 ) * RDS_PHYSICAL_GRID,
                            ( LambdaY2 - LambdaY1 ) * RDS_PHYSICAL_GRID );

  DrealAddUndo();
  DrealAddUndoRec( Rectangle );

  DrealDisplayRectangle( Rectangle );
  
  DrealChangeEditMode( DREAL_EDIT_MEASURE,
                       DrealPromptEditMeasure );

  rdsend();
}
