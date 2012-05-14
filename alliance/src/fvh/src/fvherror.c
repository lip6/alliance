/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| Tool    :                     FVH                           |
|                                                             |
| File    :                   fvherror.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"

# include "fvherror.h"

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

void fvh_error( Error, Text, Value, File, Line )

    char  Error;
    char *Text;
    long  Value;
    char *File;
    long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%ld ", Name, Line );

  switch( Error )
  {
    case FVH_ERROR_OPEN_FILE :
      fprintf( stderr, "Unable to open file \"%s\"\n", Text );
    break;
    case FVH_ERROR_PARSE_FILE :
      fprintf( stderr, "Unable to parse file \"%s\"\n", Text );
    break;
    case FVH_ERROR_SIGNAL_NOT_FOUND :
      fprintf( stderr, "Signal \"%s\" not found line %ld\n", Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_DEFAULT_ASSIGN :
      fprintf( stderr, "Illegal default output assignation\n" );
    break;
    case FVH_ERROR_NO_STATE :
      fprintf( stderr, "Number of state is zero in \"%s\" (FSM %s)\n",
               Text, (char *)Value );
    break;
    case FVH_ERROR_CONTROL_TYPE :
      fprintf( stderr, "Bad control type size in \"%s\" line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_CASE :
      fprintf( stderr, "Illegal case condition, type \"%s\" line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_STATE :
      fprintf( stderr, "Illegal state name \"%s\" in case condition line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_CONTROL :
      fprintf( stderr, "Illegal control name \"%s\" in case condition line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_INSTRUCTION :
      fprintf( stderr, "Illegal instruction type %ld\n", (long)Text );
    break;
    case FVH_ERROR_ILLEGAL_STATE_ASSIGN :
      fprintf( stderr, "Illegal state name \"%s\" in assignation line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_STACK_ASSIGN :
      fprintf( stderr, "Illegal stack name \"%s\" in assignation line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_CONTROL_ASSIGN :
      fprintf( stderr, "Illegal control name \"%s\" in assignation line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_ASSIGNATION :
      fprintf( stderr, "Illegal use of \"%s\" in main process line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_MAIN_PROCESS :
      fprintf( stderr, "Illegal figure \"%s\" missing main process\n", Text );
    break;
    case FVH_ERROR_CLOCK_PROCESS :
      fprintf( stderr, "Illegal figure \"%s\" missing clock process\n", Text );
    break;
    case FVH_ERROR_PROCESS_SENSITIVITY :
      fprintf( stderr, "Illegal sensitivity list in the process \"%s\"\n", Text );
    break;
    case FVH_ERROR_MISSING_SIGNAL :
      fprintf( stderr, "\"%s\" must be in the sensitivity list line %ld\n", Text, Value );
    break;
    case FVH_ERROR_IN_MAIN_PROCESS :
      fprintf( stderr, "Illegal use of \"%s\" in main process line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_IN_CLOCK_PROCESS :
      fprintf( stderr, "Illegal use of \"%s\" in clock process line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_ILLEGAL_OUT_ASSIGN :
      fprintf( stderr, "Illegal output \"%s\" assignation line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_MISSING_CLOCK_PORT :
      fprintf( stderr, "Missing clock declaration in figure \"%s\" (fsm %s)\n",
          Text, (char *)Value );
    break;
    case FVH_ERROR_STACK_SIZE_ZERO :
      fprintf( stderr, "Size of stack is zero in \"%s\"\n",  Text );
    break;
    case FVH_ERROR_MISSING_STACK_CONTROL :
      fprintf( stderr, "Missing stack control in figure \"%s\" line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_MISSING_STATE_CONTROL :
      fprintf( stderr, "Missing state control in figure \"%s\"\n",  Text );
    break;
    case FVH_ERROR_NO_CONTROL_CONDITION :
      fprintf( stderr, "Missing control condition in figure \"%s\" line %ld\n",  Text, Value );
    break;
    case FVH_ERROR_BAD_STACK_CONTROL :
      fprintf( stderr, "Bad stack control in figure \"%s\"\n",  Text );
    break;
    case FVH_ERROR_ILLEGAL_IF_CONDITION :
      fprintf( stderr, "Illegal IF condition line %ld\n", Value );
    break;
    case FVH_ERROR_TWO_PROCESS :
      fprintf( stderr, "Should have two process for each FSM in figure \"%s\"\n", Text );
    break;
    case FVH_ERROR_ILLEGAL_PROCESS_BODY :
      fprintf( stderr, "Illegal body structure for process \"%s\" line %ld\n",
          Text, Value );
    break;
    case FVH_ERROR_PRAGMA_SPECIFICATION :
      fprintf( stderr, "Illegal pragma specification in figure \"%s\"\n", Text );
    break;
    case FVH_ERROR_BAD_PROCESS :
      fprintf( stderr, "Process \"%s\" not compatible with the FSM VHDL subset in figure \"%s\"\n", Text, (char *)Value );
    break;
    case FVH_ERROR_NOT_YET_IMPLEMENTED :
      fprintf( stderr, "Multi FSM driver not yet implemented for figure \"%s\"\n", Text );
    default :
      fprintf( stderr, "Internal error number %d\n", Error );
  }

  autexit( 1 );
}

void fvh_warning( Warning, Text, Value, File, Line )

    char  Warning;
    char *Text;
    long  Value;
    char *File;
    long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%ld ", Name, Line );

  switch ( Warning )
  {
    default :
      fprintf( stderr, "Internal warning number %d\n", Warning );
  }
}
