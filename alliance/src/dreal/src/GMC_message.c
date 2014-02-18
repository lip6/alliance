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
| File    :                  Message.c                        |
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
# include "GMC_message.h"
# include "GMC_create.h"

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
|                    DrealPromptCreateRectangle               |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptCreateRectangle()

{
  char StaticLayer;

  StaticLayer = RDS_STATIC_LAYER[ (int)DrealRectangleLayer ];

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Create Rectangle" );

  if ( DrealRectangleName == (char *)NULL )
  {
    sprintf( MessageBuffer, "Layer: %s   Name: none",
             DREAL_RDS_LAYER_NAME_TABLE[ (int)StaticLayer ][0] );
  }
  else
  {
    sprintf( MessageBuffer, "Layer: %s   Name: %s",
             DREAL_RDS_LAYER_NAME_TABLE[ (int)StaticLayer ][0] ,
             DrealRectangleName );
  }

  DrealDisplayMessage( DREAL_MESSAGE_INFO, MessageBuffer );

  switch ( DrealCountEventEdit )
  {
    case 0 : 

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first point" );

    break;

    case 1 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter second point" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealPromptCreateAbutmentBox             |
|                                                             |
\------------------------------------------------------------*/
 
void DrealPromptCreateAbutmentBox()
 
{
  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Create Abutmentbox" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select window" );
 
  switch ( DrealCountEventEdit )
  {
    case 0 :
 
      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first corner" );
 
    break;

    case 1 :
 
      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter second corner" );
  } 
}
