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
| Tool    :                     BOOM                           |
|                                                             |
| File    :                  boom_error.c                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   06.06.00                        |
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
# include "boom_error.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       BoomDisplayError                       |
|                                                             |
\------------------------------------------------------------*/

void BoomDisplayError( File, Line, Error, Text )

   char *File;
   int   Line;
   int   Error;
   char *Text;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%d ", Name, Line );

  switch( Error )
  {
    case BOOM_EXPR_NULL_ERROR :

      fprintf( stderr, "null expression !\n" );

    break;

    case BOOM_EXPR_LOOP_ERROR :

      fprintf( stderr, "combinatory loop on %s !\n", Text );

    break;

    case BOOM_UNKNOWN_ATOM_ERROR :

      fprintf( stderr, "unknown atom %s !\n", Text );

    break;

    case BOOM_SIMPLIFY_ERROR :

      fprintf( stderr, "cannot simplify internal signals\n" );

    break;

    case BOOM_ILLEGAL_STABLE_ERROR :

      fprintf( stderr, "illegal use of STABLE operator\n" );

    break;

    case BOOM_OPERATOR_ERROR :

      fprintf( stderr, "illegal use of operator %ld\n", (long)Text );

    break;

    default : fprintf( stderr, "Internal error number %d\n", Error );
  }

  autexit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomDisplayWarning                     |
|                                                             |
\------------------------------------------------------------*/

void BoomDisplayWarning( File, Line, Warning, Text )

   char *File;
   int   Line;
   int   Warning;
   char *Text;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%d ", Name, Line );

  switch ( Warning )
  {
    default : fprintf( stderr, "Internal warning number %d\n", Warning );
  }
}
