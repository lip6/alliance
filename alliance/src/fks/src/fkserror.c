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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     FSM                           |
|                                                             |
| File    :                   fkserror.c                      |
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

# include "fkserror.h"

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

void fks_error( Error, Text, File, Line )

    char  Error;
    char *Text;
    char *File;
    long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%ld ", Name, Line );

  switch ( Error )
  {
    case KISS_ERROR_OPEN_FILE : 

      fprintf( stderr, "unable to open file \"%s\"\n", Text );

    break;

    case KISS_ERROR_PARSE     :

      fprintf( stderr, "parse error line %ld\n", (long)Text );

    break;

    case KISS_ERROR_END       :

      fprintf( stderr, "unexpected end of file line %ld\n", (long)Text );

    break;

    case KISS_ERROR_VALUE     :

      fprintf( stderr, "bad value line %ld\n", (long)Text );

    break;

    case KISS_ERROR_NAME      :

      fprintf( stderr, "bad name line %ld\n", (long)Text );

    break;

    case KISS_ERROR_PORT      :

      fprintf( stderr, "missing port(s) declaration line %ld\n", (long)Text );

    break;

    case KISS_ERROR_STATE     :

      fprintf( stderr, "missing state(s) declaration line %ld\n", (long)Text );

    break;

    case KISS_ERROR_FSM_STACK :

      fprintf( stderr, "unable to drive FSM stack of \"%s\"\n", Text );

    break;


    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  autexit( 1 );
}
