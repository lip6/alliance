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
| Tool    :                     Vpn                           |
|                                                             |
| File    :                 Vpn Errors                        |
|                                                             |
| Vpnhors :                Jacomme Ludovic                    |
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
# include "vpn.h"
# include "vpnerror.h"

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
|                         Vpn Error Function                  |
|                                                             |
\------------------------------------------------------------*/

void vpn_error( Error, Text1, Text2, File, Line )

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
    case VPN_PROC_EXIST_ERROR :
      fprintf( stderr, "process %s exists already\n", Text1 );
    break;
    case VPN_FUNC_EXIST_ERROR :
      fprintf( stderr, "function %s exists already\n", Text1 );
    break;
    case VPN_PLACE_EXIST_ERROR :
      fprintf( stderr, "place %s exists already\n", Text1 );
    break;
    case VPN_TRANS_EXIST_ERROR :
      fprintf( stderr, "transition %s exists already\n", Text1 );
    break;
    case VPN_PORT_EXIST_ERROR :
      fprintf( stderr, "port %s exists already\n", Text1 );
    break;
    case VPN_ARC_PLACE_EXIST_ERROR :
      fprintf( stderr, "arc place %s to %s exists already\n", Text1, Text2 );
    break;
    case VPN_ARC_TRANS_EXIST_ERROR :
      fprintf( stderr, "arc trans %s to %s exists already\n", Text1, Text2 );
    break;
    case VPN_UNKNOWN_DECLAR_TYPE :
      fprintf( stderr, "unknown declar type %ld for %s\n", (long)Text1, Text2 );
    break;
    case VPN_SIGNAL_EXIST_ERROR :
      fprintf( stderr, "signal %s exists already\n", Text1 );
    break;
    case VPN_VARIABLE_EXIST_ERROR :
      fprintf( stderr, "variable %s exists already\n", Text1 );
    break;
    case VPN_DEFINE_EXIST_ERROR :
      fprintf( stderr, "define %s exists already\n", Text1 );
    break;
    case VPN_ARGUMENT_EXIST_ERROR :
      fprintf( stderr, "argument %s exists already\n", Text1 );
    break;
    case VPN_CONSTANT_EXIST_ERROR :
      fprintf( stderr, "constant %s exists already\n", Text1 );
    break;
    case VPN_DEL_ARC_PLACE_ERROR :
      fprintf( stderr, "unable to delete arc from place %s\n", Text1 );
    break;
    case VPN_DEL_ARC_TRANS_ERROR :
      fprintf( stderr, "unable to delete arc from trans %s\n", Text1 );
    break;
    case VPN_TRANS_FATHER_UNKNOWN :
      fprintf( stderr, "unknown father type %ld for transition %s\n", (long)Text1, Text2 );
    break;
    case VPN_DECLAR_EXIST_ERROR   :
      fprintf( stderr, "symbol %s already declared\n", Text1 );
    break;
    case VPN_BAD_DECLAR_TYPE   :
      fprintf( stderr, "bad declaration type %ld\n", (long)Text1 );
    break;
    case VPN_BAD_ASSIGN_TYPE   :
      fprintf( stderr, "bad assign %s, type %ld\n", Text1, (long)Text2 );
    break;
    case VPN_UNDECLARED_SYMBOL   :
      fprintf( stderr, "undeclared symbol %s\n", Text1 );
    break;
    case VPN_ILLEGAL_POSITION_ERROR :
      fprintf( stderr, "illegal position for symbol %s %ld\n", Text1, (long)Text2 );
    break;
    case VPN_ILLEGAL_INDEX_ERROR :
      fprintf( stderr, "illegal index for symbol %s %ld\n", Text1, (long)Text2 );
    break;
    case VPN_MOD_EXIST_ERROR :
      fprintf( stderr, "model %s exists already\n", Text1 );
    break;
    case VPN_INS_EXIST_ERROR :
      fprintf( stderr, "instance %s exists already\n", Text1 );
    break;
    case VPN_GENERIC_EXIST_ERROR :
      fprintf( stderr, "generic %s exists already\n", Text1 );
    break;
    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  autexit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                         Vpn Warning Function                |
|                                                             |
\------------------------------------------------------------*/

void vpn_warning( Warning, Text1, Text2, File, Line )

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
