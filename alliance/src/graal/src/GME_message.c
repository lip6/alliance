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
# include "GRM.h"
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
|                 GraalPromptSelectPointIdentify              |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectPointIdentify()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE  , "Identify" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO  , "Select object" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalPromptSelectWindowIdentify              |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectWindowIdentify()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Identify" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select window" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalPromptSelectPointCopy                  |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectPointCopy()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE  , "Copy" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO  , "Select object" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalPromptSelectWindowCopy                  |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectWindowCopy()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Copy" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select window" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalPromptEditCopy                        |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptEditCopy()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Copy" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select vector" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter source point" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter target point" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalPromptSelectPointMove                  |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectPointMove()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE  , "Move" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO  , "Select object" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalPromptSelectWindowMove                  |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectWindowMove()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Move" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select window" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalPromptEditMove                        |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptEditMove()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Move" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select vector" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter source point" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter target point" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalPromptSelectPointDelete                |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectPointDelete()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE  , "Delete" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO  , "Select object" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalPromptSelectWindowDelete                |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectWindowDelete()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Delete" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select window" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalPromptSelectPointStretch               |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectPointStretch()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE  , "Stretch" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO  , "Select object" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalPromptSelectWindowStretch               |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectWindowStretch()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Stretch" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select window" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalPromptEditStretch                     |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptEditStretch()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Stretch" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select vector" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter source point" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter target point" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalPromptSelectPointModify                |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectPointModify()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE  , "Modify" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO  , "Select object" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalPromptSelectWindowModify                |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptSelectWindowModify()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select window" );

  switch ( GraalCountEventEdit )
  {
    case 0 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter next corner" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalPromptModifySegment                 |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptModifySegment()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify Segment" );

  sprintf( MessageBuffer, "Layer: %s   Width: %2d   Name: %s",
           GRAAL_SEGMENT_NAME_TABLE[ GraalSegmentMLayer ][0] ,
           GraalSegmentMWidth,
           ( GraalSegmentMName ) ? GraalSegmentMName : "none" );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter new properties for this segment" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalPromptModifyConnector               |
|                                                             |
\------------------------------------------------------------*/
 
void GraalPromptModifyConnector()
 
{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify Connector" );

  sprintf( MessageBuffer, "Layer: %s   Width: %2d   Orient: %s   Name: %s",
           GRAAL_CONNECTOR_NAME_TABLE[ GraalConnectorMLayer ][0],
           GraalConnectorMWidth,
           GRAAL_ORIENT_NAME_TABLE[ GraalConnectorMOrient ][0],
           ( GraalConnectorMName ) ? GraalConnectorMName : "none" );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter new properties for this connector" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalPromptModifyReference               |
|                                                             |
\------------------------------------------------------------*/
 
void GraalPromptModifyReference()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify Reference" );
 
  if ( GraalReferenceMName != (char *)NULL )
  {
    sprintf( MessageBuffer, "Type: %s   Name: %s",
             GRAAL_REFERENCE_NAME_TABLE[GraalReferenceMType][0],
             GraalReferenceMName );
  }
  else
  {   
    sprintf( MessageBuffer, "Type: %s   Name: none",
             GRAAL_REFERENCE_NAME_TABLE[GraalReferenceMType][0] );
  }

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter new properties for this reference" );

  rdsend();
}  

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptModifyVia                   |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptModifyVia()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify Via" );

  sprintf( MessageBuffer, "Type: %s  Name: %s",
           GRAAL_VIA_NAME_TABLE[ GraalViaMType ][0],
           GraalViaMName ? GraalViaMName : "none" );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter new properties for this via" );

  rdsend();
}  

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptModifyBigVia                |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptModifyBigVia()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify big Via" );

  sprintf( MessageBuffer, "Type: %s  Name: %s",
           GRAAL_BIGVIA_NAME_TABLE[ GraalViaMType ][0],
           GraalViaMName ? GraalViaMName : "none" );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter new properties for this big via" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptModifyTransistor            |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptModifyTransistor()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify Transistor" );

  if ( GraalTransistorMName == (char *)NULL )
  {
    sprintf( MessageBuffer, "Type: %s   Width: %2d   Name: None",
             GRAAL_TRANSISTOR_NAME_TABLE[ GraalTransistorMType ][0],
             GraalTransistorMWidth );
  }
  else
  {
    sprintf( MessageBuffer, "Type: %s   Width: %2d   Name: %s",
             GRAAL_TRANSISTOR_NAME_TABLE[ GraalTransistorMType ][0],
             GraalTransistorMWidth, GraalTransistorMName );
  }

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter new properties for this transistor" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptModifyInstance              |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptModifyInstance()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Modify Instance" );

  sprintf( MessageBuffer, "Symmetry: %s   Model: %s   Name: %s",
           GRAAL_SYMMETRY_NAME_TABLE[ GraalInstanceMSym ][0],
           GraalInstanceMModel, GraalInstanceMName  );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter new properties for this instance" );

  rdsend();
}
