/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                     Aut                           |
|                                                             |
| File    :                 Aut Errors                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   03.12.96                        |
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
# include <signal.h>


# include <mut.h>
# include "aut.h"

# include "auterror.h"

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
|                         Aut Error Function                  |
|                                                             |
\------------------------------------------------------------*/

void aut_error( Error, Text, File, Line )

  int   Error;
  char *Text;
  char *File;
  long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%ld ", Name, Line );

  switch( Error )
  {
    case AUT_ALLOC_ERROR :
      fprintf( stderr, "alloc error, can't continue !\n");
    break;

    case AUT_ALLOC_SIZE_ERROR :
      fprintf( stderr, "null size alloc error, can't continue !\n");
    break;

    case AUT_RESIZE_ERROR :
      fprintf( stderr, "resize error, can't continue !\n");
    break;

    case AUT_HASH_SIZE_ERROR :
      fprintf( stderr, "hash table size %ld error !\n", (long)Text );
    break;

    case AUT_HASH_KEY_ERROR :
      fprintf( stderr, "hash key %lx error !\n", (long)Text );
    break;

    case AUT_OPEN_FILE_ERROR :
      fprintf( stderr, "open file %s error !\n", Text );
    break;

    case AUT_CLOSE_FILE_ERROR :
      fprintf( stderr, "close file %s error !\n", Text );
    break;

    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  autexit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                         Aut Warning Function                |
|                                                             |
\------------------------------------------------------------*/

void aut_warning( Warning, Text, File, Line )

  int   Warning;
  char *Text;
  char *File;
  long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%ld ", Name, Line );

  switch( Warning )
  {
    case AUT_EXIT_CORE_WARNING :
      fprintf( stderr, "warning: abnormal autexit generates a core file !\n" );
    break;

    case AUT_ALLOC_DEBUG_WARNING :
      fprintf( stderr, "warning: autalloc and autfree debug mode on !\n" );
    break;

    case AUT_ABNORMAL_EXIT_WARNING :
      fprintf( stderr, "warning: abnormal autexit %ld !\n", (long)Text );
    break;

    default :

      fprintf( stderr, "unknown internal warning %d !\n", Warning );
  }
}

