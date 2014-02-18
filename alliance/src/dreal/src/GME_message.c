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
# include "GME.h"
# include "GME_modify.h"
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

  static char MessageBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 DrealPromptSelectPointIdentify              |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectPointIdentify()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE  , "Identify" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO  , "Select object" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                DrealPromptSelectWindowIdentify              |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectWindowIdentify()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Identify" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select window" );

  switch ( DrealCountEventEdit )
  {
    case 0 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealPromptSelectPointCopy                  |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectPointCopy()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE  , "Copy" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO  , "Select object" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                DrealPromptSelectWindowCopy                  |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectWindowCopy()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Copy" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select window" );

  switch ( DrealCountEventEdit )
  {
    case 0 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealPromptEditCopy                        |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptEditCopy()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Copy" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select vector" );

  switch ( DrealCountEventEdit )
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
|                 DrealPromptSelectPointMove                  |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectPointMove()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE  , "Move" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO  , "Select object" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                DrealPromptSelectWindowMove                  |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectWindowMove()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Move" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select window" );

  switch ( DrealCountEventEdit )
  {
    case 0 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealPromptEditMove                        |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptEditMove()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Move" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select vector" );

  switch ( DrealCountEventEdit )
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
|                 DrealPromptSelectPointDelete                |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectPointDelete()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE  , "Delete" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO  , "Select object" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                DrealPromptSelectWindowDelete                |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectWindowDelete()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Delete" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select window" );

  switch ( DrealCountEventEdit )
  {
    case 0 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealPromptSelectPointStretch               |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectPointStretch()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE  , "Stretch" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO  , "Select object" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealPromptEditStretch                     |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptEditStretch()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Stretch" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select vector" );

  switch ( DrealCountEventEdit )
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
|                 DrealPromptSelectPointModify                |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectPointModify()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE  , "Modify" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO  , "Select object" );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                DrealPromptSelectWindowModify                |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptSelectWindowModify()

{
  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Modify" );
  DrealDisplayMessage( DREAL_MESSAGE_INFO, "Select window" );

  switch ( DrealCountEventEdit )
  {
    case 0 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealPromptModifyRectangle               |
|                                                             |
\------------------------------------------------------------*/

void DrealPromptModifyRectangle()

{
  char StaticLayer;

  rdsbegin();

  DrealDisplayMessage( DREAL_MESSAGE_MODE, "Modify Rectangle" );

  StaticLayer = RDS_STATIC_LAYER[ (int)DrealRectangleMLayer ];

  if ( DrealRectangleMName == (char *)NULL )
  {
    sprintf( MessageBuffer, "Layer: %s   Name: none",
             DREAL_RDS_LAYER_NAME_TABLE[ (int)StaticLayer ][0] );
  }
  else
  {   
    sprintf( MessageBuffer, "Layer: %s   Name: %s",
             DREAL_RDS_LAYER_NAME_TABLE[ (int)StaticLayer ][0] ,
             DrealRectangleMName );
  }

  DrealDisplayMessage( DREAL_MESSAGE_INFO, MessageBuffer );
  DrealDisplayMessage( DREAL_MESSAGE_PROMPT, "Enter new properties for this rectangle" );

  rdsend();
}
