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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Modify.c                         |
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
# include "GSB.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GME.h"

# include "GME_modify.h"
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

  char DrealModifyAbort  = 0;
  char DrealModifyLock   = 0;
  char DrealModifyApply  = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Rectangle                          |
|                                                             |
\------------------------------------------------------------*/

  char  DrealRectangleMLayer;
  char *DrealRectangleMName;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Changed Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       DrealRectangleChanged                 |
|                                                             |
\------------------------------------------------------------*/

char DrealRectangleChanged( Rectangle )

  rdsrec_list *Rectangle;
{
  return 

   ( (   Rectangle->NAME              != DrealRectangleMName  ) ||
     (   GetRdsLayer( Rectangle )     != DrealRectangleMLayer ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealEditModify                       |
|                                                             |
\------------------------------------------------------------*/

void DrealEditModify()

{
  drealselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  char         FirstUndo;

  rdsbegin();

  FirstUndo = 1;

  DrealModifyAbort = 0;
  DrealModifyApply = 0;
  DrealModifyLock  = 0;
  
  for ( Select  = DrealHeadSelect;
        Select != (drealselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( (   IsRdsFigRec( Rectangle ) ) &&
         ( ! IsRdsFigure( Rectangle ) ) )
    {
      DrealAcceptRectangle( Rectangle );
      DrealDisplayRectangle( Rectangle );

      DrealRectangleMLayer = GetRdsLayer( Rectangle );
      DrealRectangleMName  = Rectangle->NAME;

      DrealPromptModifyRectangle();
      DrealEnterPanel( &DrealModifyRectanglePanel );

      DrealModifyLock = DREAL_TRUE;

      while ( DrealModifyLock )
      {
        DrealLimitedLoop( DrealModifyRectanglePanel.PANEL );
      }

      DrealRejectRectangle( Rectangle );
      DrealDisplayRectangle( Rectangle );

      if ( DrealModifyAbort ) break;

      if ( DrealModifyApply )
      {
        if ( ! DrealRectangleChanged( Rectangle ) ) 

          continue; 

        if ( FirstUndo )
        {
          DrealAddUndo();

          FirstUndo = 0;
        }

        DrealDeleteRectangle( Rectangle );
        DrealAddUndoRec( Rectangle );

        NewRec = DrealAddRectangle( DrealRectangleMName,
                                    DrealRectangleMLayer,
                                    Rectangle->X , Rectangle->Y,
                                    Rectangle->DX, Rectangle->DY );

        DrealAddUndoRec( NewRec );

        DrealDisplayRectangle( NewRec );
      }
    }
  }

  DrealExitPanel( &DrealModifyRectanglePanel );

  DrealDelSelect();
  DrealZoomRefresh();

  rdsend();
}
