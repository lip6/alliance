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
| Tool    :                     Bhl                           |
|                                                             |
| File    :                 Bhl Errors                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   01.02.95                        |
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
# include "aut.h"
# include "abl.h"
# include "bhl_error.h"

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

void bhl_error( Error, Text, File, Line )

  int   Error;
  char *Text;
  char *File;
  int   Line;
{
  char *Name;

  Name = strdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%d ", Name, Line );

  switch( Error )
  {
    case BHL_EXPR_NULL_ERROR :

      fprintf( stderr, "null expression !\n" );

    break;

    case BHL_EXPR_LOOP_ERROR :

      fprintf( stderr, "combinatory loop on %s !\n", Text );

    break;

    case BHL_UNKNOWN_ATOM_ERROR :

      fprintf( stderr, "unknown atom %s !\n", Text );

    break;

    case BHL_SIMPLIFY_ERROR :
 
      fprintf( stderr, "cannot simplify internal signals\n" );

    break;

    case BHL_ILLEGAL_STABLE_ERROR :
 
      fprintf( stderr, "illegal use of STABLE operator\n" );

    break;

    case BHL_OPERATOR_ERROR :

      fprintf( stderr, "illegal use of operator %ld\n", (long)Text );

    break;

    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  autexit( 1 );
}
