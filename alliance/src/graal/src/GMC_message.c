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
|                    GraalPromptCreateSegment                 |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptCreateSegment()

{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Segment" );

  sprintf( MessageBuffer, "Layer: %s   Width: %2d   Name: %s",
           GRAAL_SEGMENT_NAME_TABLE[ GraalSegmentLayer ][0] ,
           GraalSegmentWidth,
           ( GraalSegmentName ) ? GraalSegmentName : "none" );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );

  switch ( GraalCountEventEdit )
  {
    case 0 : 

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first point" );

    break;

    case 1 :

      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter second point" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalPromptCreateConnector               |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptCreateConnector()

{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Connector" );

  sprintf( MessageBuffer, "Layer: %s   Width: %2d   Orient: %s",
           GRAAL_CONNECTOR_NAME_TABLE[ GraalConnectorLayer ][0],
           GraalConnectorWidth, 
           GRAAL_ORIENT_NAME_TABLE[ GraalConnectorOrient ][0] );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter center point" );
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalPromptCreateReference               |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptCreateReference()

{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Reference" );

  sprintf( MessageBuffer, "Type: %s",
           GRAAL_REFERENCE_NAME_TABLE[GraalReferenceType][0] );

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter center point" );
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptCreateVia                   |
|                                                             |
\------------------------------------------------------------*/
 
void GraalPromptCreateVia()
 
{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Via" );
 
  sprintf( MessageBuffer, "Type: %s  Name: %s",
           GRAAL_VIA_NAME_TABLE[ GraalViaType ][0],
           ( GraalViaName ) ? GraalViaName : "none" );
 
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter center point" );
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptCreateBigVia                |
|                                                             |
\------------------------------------------------------------*/
 
void GraalPromptCreateBigVia()
 
{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Big Via" );
 
  sprintf( MessageBuffer, "Type: %s  Name: %s",
           GRAAL_BIGVIA_NAME_TABLE[ GraalBigViaType ][0],
           ( GraalBigViaName ) ? GraalBigViaName : "none" );
 
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
 
  switch ( GraalCountEventEdit )
  {
    case 0 :
 
      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );
 
    break;

    case 1 :
 
      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter second corner" );
  } 
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptCreateTransistor            |
|                                                             |
\------------------------------------------------------------*/
 
void GraalPromptCreateTransistor()
 
{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Transistor" );
 
  if ( GraalTransistorName == (char *)NULL )
  {
    sprintf( MessageBuffer, "Type: %s   Width: %2d   Name: none",
             GRAAL_TRANSISTOR_NAME_TABLE[ GraalTransistorType ][0],
             GraalTransistorWidth );
  }
  else
  {
    sprintf( MessageBuffer, "Type: %s   Width: %2d   Name: %s",
             GRAAL_TRANSISTOR_NAME_TABLE[ GraalTransistorType ][0],
             GraalTransistorWidth, GraalTransistorName );
  }

  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
 
  switch ( GraalCountEventEdit )
  {
    case 0 :
 
      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first point" );
 
    break;

    case 1 :
 
      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter second point" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalPromptCreateAbutmentBox             |
|                                                             |
\------------------------------------------------------------*/
 
void GraalPromptCreateAbutmentBox()
 
{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Abutmentbox" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select window" );
 
  switch ( GraalCountEventEdit )
  {
    case 0 :
 
      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter first corner" );
 
    break;

    case 1 :
 
      GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter second corner" );
  } 
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalPromptCreateInstance              |
|                                                             |
\------------------------------------------------------------*/
 
void GraalPromptCreateInstance()
 
{
  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Create Instance" );
 
  sprintf( MessageBuffer, "Symmetry: %s",
           GRAAL_SYMMETRY_NAME_TABLE[ GraalInstanceSym ][0]  );
 
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, MessageBuffer );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter base point" );
}
