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
| Tool    :                     Rtn                           |
|                                                             |
| File    :                 Rtn Errors                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   04.07.96                        |
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
# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "rtn.h"
# include "rtnerror.h"

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
|                         Rtl Error Function                  |
|                                                             |
\------------------------------------------------------------*/

void rtl_error( Error, Text1, Text2, File, Line )

  int   Error;
  char *Text1;
  char *Text2;
  char *File;
  long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%ld ", Name, Line );

  switch( Error )
  {
    case RTL_DECLAR_EXIST_ERROR   :
      fprintf( stderr, "symbol %s already declared\n", Text1 );
    break;
    case RTL_MOD_EXIST_ERROR   :
      fprintf( stderr, "model %s already declared\n", Text1 );
    break;
    case RTL_INS_EXIST_ERROR   :
      fprintf( stderr, "instance %s already declared\n", Text1 );
    break;
    case RTL_ILLEGAL_POSITION_ERROR :
      fprintf( stderr, "illegal position for symbol %s %ld\n", Text1, (long)Text2 );
    break;
    case RTL_ILLEGAL_INDEX_ERROR :
      fprintf( stderr, "illegal index for symbol %s %ld\n", Text1, (long)Text2 );
    break;
    case RTL_UNKNOWN_DECLAR_TYPE :
      fprintf( stderr, "unknown declar type %ld for %s\n", (long)Text1, Text2 );
    break;
    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  autexit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                         Rtl Warning Function                |
|                                                             |
\------------------------------------------------------------*/

void rtl_warning( Warning, Text1, Text2, File, Line )

  int   Warning;
  char *Text1;
  char *Text2;
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

      fprintf( stderr, "unknown internal warning %d !\n", Warning );
  }
}
