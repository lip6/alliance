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
# include "GMT.h"
# include "GMT_tools.h"
# include "GMT_message.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     GraalPromptToolsPeek                    |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsPeek()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Peek" );
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
|                     GraalPromptToolsDruc                    |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsDruc()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Druc" );
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
|                     GraalPromptToolsEqui                    |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsEqui()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Equi" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select object" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalPromptToolsFlatten                 |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsFlatten()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Flatten" );
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
|                  GraalPromptToolsRealFlatten                |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsRealFlatten()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Real flatten" );
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
|                     GraalPromptToolsUnflatten               |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsUnflatten()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Unflatten" );
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
|                     GraalPromptToolsHierarchyDown           |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsHierarchyDown()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Go down" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select instance" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter select point" );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalPromptToolsHierarchyShow           |
|                                                             |
\------------------------------------------------------------*/

void GraalPromptToolsHierarchyShow()

{
  rdsbegin();

  GraalDisplayMessage( GRAAL_MESSAGE_MODE, "Show path" );
  GraalDisplayMessage( GRAAL_MESSAGE_INFO, "Select root point" );
  GraalDisplayMessage( GRAAL_MESSAGE_PROMPT, "Enter point" );

  rdsend();
}
