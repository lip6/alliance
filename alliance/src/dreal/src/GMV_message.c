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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Message.c                        |
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
# include "GMV.h"
# include "GMV_message.h"

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

  static char MessageBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       DrealPromptZoomIn                     |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptZoomIn()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Zoom In" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select window" );

  switch ( DrealCountEventZoom )
  {
    case 0 : 

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter second corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealPromptZoomPan                    |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptZoomPan()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Zoom Pan" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select line"  );

  switch ( DrealCountEventZoom )
  {
    case 0 : 

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter source point" );

    break;

    case 1 : 

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter target point" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealPromptZoomCenter                    |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptZoomCenter()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Zoom Center" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select point" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter center point" );

  rdsend();
}

