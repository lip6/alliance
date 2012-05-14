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
| File    :                  Select.c                         |
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
# include "GRD.h"
# include "GSB.h"

# include "GRD_window.h"
# include "GRD_select.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  drealselect *DrealHeadSelect  = (drealselect *)NULL;

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
|                        DrealAllocSelect                     |
|                                                             |
\------------------------------------------------------------*/

drealselect *DrealAllocSelect()
{
  return((drealselect *)rdsalloc(sizeof(drealselect), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrealFreeSelect                     |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeSelect( FreeSelect )

   drealselect *FreeSelect;
{
  rdsfree((char *)FreeSelect, sizeof(drealselect));
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAcceptRectangle                  |
|                                                             |
\------------------------------------------------------------*/
 
void DrealAcceptRectangle( Rectangle )
 
  rdsrec_list *Rectangle;
{
  rdsbegin();
 
  SetDrealAccepted( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\ 
|                                                             | 
|                       DrealRejectRectangle                  | 
|                                                             |
\------------------------------------------------------------*/ 

void DrealRejectRectangle( Rectangle ) 

  rdsrec_list *Rectangle;
{ 
  rdsbegin();

  ClearDrealAccepted( Rectangle ); 

  rdsend();
} 

/*------------------------------------------------------------\
|                                                             |
|                       DrealSelectRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void DrealSelectRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsbegin();

  SetDrealSelected( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealUnselectRectangle                 |
|                                                             |
\------------------------------------------------------------*/

void DrealUnselectRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsbegin();

  ClearDrealSelected( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealDeleteRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void DrealDeleteRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsbegin();

  SetDrealDeleted( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealUndeleteRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void DrealUndeleteRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsbegin();

  ClearDrealDeleted( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAddSelect                        |
|                                                             |
\------------------------------------------------------------*/

void DrealAddSelect( Rectangle )

  rdsrec_list *Rectangle;
{
  drealselect *Select;

  rdsbegin();

  Select = DrealAllocSelect();

  Select->RECTANGLE = Rectangle;
  Select->NEXT      = DrealHeadSelect;
  DrealHeadSelect   = Select;

  DrealSelectRectangle( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealDelSelect                        |
|                                                             |
\------------------------------------------------------------*/

void DrealDelSelect()
{
  drealselect    *DelSelect;
  drealselect    *Select;

  rdsbegin();

  Select          = DrealHeadSelect;
  DrealHeadSelect = (drealselect *)NULL;

  while( Select != (drealselect *)NULL )
  {
    DrealRejectRectangle( Select->RECTANGLE );

    DelSelect  = Select;
    Select     = Select->NEXT;
    
    DrealFreeSelect( DelSelect );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealPurgeSelect                      |
|                                                             |
\------------------------------------------------------------*/

void DrealPurgeSelect()
{
  drealselect    *DelSelect;
  drealselect    *Select;
  drealselect   **Previous;

  rdsbegin();

  Previous = &DrealHeadSelect;
  Select   =  DrealHeadSelect;

  while( Select != (drealselect *)NULL )
  {
    if ( ! IsDrealAccepted( Select->RECTANGLE ) )
    {
      DelSelect  = Select;
      Select     = Select->NEXT;
      *Previous  = Select;
    
      DrealFreeSelect( DelSelect );
    }
    else
    {
      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  rdsend();
}
