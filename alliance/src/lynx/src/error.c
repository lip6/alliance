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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  error.c                          |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h> 
# include <string.h>

# include "mut.h"

# include "error.h"

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
|                          Lynx Error                         |
|                                                             |
\------------------------------------------------------------*/

void lynx_error( Error, Text1, Text2, File, Line )

  char  Error;
  char *Text1;
  char *Text2;
  char *File;
  long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "\n\t%s%ld Error : ", Name, Line );

  switch( Error )
  {
    case LYNX_ERROR_PHYSICAL_CONNECTOR    :
      fprintf( stderr, "connector \"%s\" is on two different equis !\n\n", Text1 );
    break;

    case LYNX_ERROR_PHYSICAL_CONNECTORS   :
      fprintf( stderr, "connectors \"%s\" and \"%s\" are on the same equi !\n\n", Text1, Text2 );
    break;

    case LYNX_ERROR_MISSING_CONNECTOR     :
      fprintf( stderr, "No connector found in %s !\n\n", Text1 );
    break;

    case LYNX_ERROR_CUT_TRANSISTOR_ELBOW  :
      fprintf( stderr, "Unable to cut transistor elbow\n\n" );
    break;

    case LYNX_ERROR_MASK_COUNTER_NULL     :
      fprintf( stderr, "No rectangle found\n\n" );
    break;

    case LYNX_ERROR_ILLEGAL_COMBINATION   :
      fprintf( stderr, "Illegal combination %s %s !\n\n", Text1, Text2 );
    break;

    case LYNX_ERROR_ILLEGAL_OPTION        :
      fprintf( stderr, "Illegal option %s !\n\n", Text1 );
    break;

    case LYNX_ERROR_ILLEGAL_FILENAME      :
      fprintf( stderr, "Illegal file name \"%s\" !\n\n", Text1 );
    break;

    case LYNX_ERROR_MINIMUM_LENGTH        :
      fprintf( stderr, "Unable to compute minimum length\n\n" );
    break;

    case LYNX_ERROR_TRANSISTOR_GATE       :
      fprintf( stderr, "Unable to compute transistor gate\n\n" );
    break;

    case LYNX_ERROR_TRANSISTOR_LOGICAL    :
      fprintf( stderr, "Unable to create logical transistor (X=%ld,Y=%ld)\n\n",
               (long)Text1, (long)Text2 );
    break;

    default :
      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Lynx Warning                       |
|                                                             |
\------------------------------------------------------------*/

void lynx_warning( Warning, Text, File, Line )

  char  Warning;
  char *Text;
  char *File;
  long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "\n\t%s%ld Warning : ", Name, Line );

  switch( Warning )
  {
    case LYNX_WARNING_PHYSICAL_CONNECTOR    :
      fprintf( stderr, "connector %s is on two different equis !\n", Text );
    break;
    case LYNX_WARNING_CORE_FILE :
      fprintf( stderr, "Generates %s file !\n", Text );
    break;
    default :
      fprintf( stderr, "unknown internal warning %d !\n", Warning );
  }
}
