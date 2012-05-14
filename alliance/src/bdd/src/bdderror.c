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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                 Bdd Errors                        |
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
# include <string.h>
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "bdderror.h"

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

void bdd_error( Error, Value, File, Line )

  int   Error;
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
    case BDD_MODEL_VAR_ERROR  :

      fprintf( stderr, "system model variable %ld", Value );

    break;

    case BDD_MODEL_OPER_ERROR  :

      fprintf( stderr, "system model oper %ld", Value );

    break;

    case BDD_HNODE_SIZE_ERROR :

      fprintf( stderr, "hash node table size %ld", Value );

    break;

    case BDD_HNODE_KEY_ERROR  :

      fprintf( stderr, "hash node key 0" );

    break;

    case BDD_HOPER_SIZE_ERROR :

      fprintf( stderr, "hash oper table size %ld", Value );

    break;

    case BDD_HOPER_KEY_ERROR  :

      fprintf( stderr, "hash oper key 0" );

    break;

    case BDD_BAD_INDEX_ERROR :

      fprintf( stderr, "index %ld out of range", Value );

    break;

    case BDD_BAD_VAR_ERROR :

      fprintf( stderr, "variable %ld out of range", Value );

    break;

    case BDD_BAD_CHILD_ERROR :

      fprintf( stderr, "child %ld out of range", Value );

    break;

    case BDD_BAD_IN_ERROR :

      fprintf( stderr, "input %s exist already", (char *)Value );

    break;

    case BDD_OPERATOR_ERROR :

      fprintf( stderr, "bad operator %ld", Value );

    break;

    case BDD_BAD_LIST_ERROR :

      fprintf( stderr, "bad list" );

    break;

    case BDD_NO_MORE_VAR_ERROR :

      fprintf( stderr, "no more variables" );

    break;

    case BDD_NEGATIVE_REF_INT_ERROR :

      fprintf( stderr, "negative internal reference, index %ld", Value );

    break;

    case BDD_NEGATIVE_REF_EXT_ERROR :

      fprintf( stderr, "negative external reference, index %ld", Value );

    break;

    case BDD_UNKNOWN_IN_MODE_ERROR :

      fprintf( stderr, "unknwon in mode %ld", Value );

    break;

    case BDD_MAX_NODE_ERROR :

      fprintf( stderr, "node limit reached %ld", Value );

    break;

    case BDD_BAD_ASSOC_ERROR :

      fprintf( stderr, "bad variable association %ld", Value );

    break;

    case BDD_NO_MORE_ASSOC_ERROR :

      fprintf( stderr, "no more variable association" );

    break;

    case BDD_UNKNOWN_AUX_MODE_ERROR :

      fprintf( stderr, "unknwon aux mode %ld", Value );

    break;

    case BDD_DUMP_FILE_ERROR :

      fprintf( stderr, "bad dump file %s", (char *)Value );

    break;

    case BDD_UNDUMP_FILE_ERROR :

      fprintf( stderr, "undump file name %s", (char *)Value );

    break;

    case BDD_UNDUMP_LINE_ERROR :

      fprintf( stderr, "undump line %ld syntax", Value );

    break;

    default :

      fprintf( stderr, "unknown internal %d", Error );
  }

  fprintf( stderr, " error !\n" );

  autexit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

void bdd_warning( Warning, Value1, Value2, File, Line )

  int   Warning;
  long  Value1;
  long  Value2;
  char *File;
  long  Line;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%ld ", Name, Line );

  switch( Warning )
  {
    case BDD_DYNAMIC_REORDER_WARNING :

      fprintf( stderr, "dynamic reorder %ld -> %ld", Value1, Value2 );

    break;

    case BDD_DYNAMIC_EXPLOSION_WARNING :

      fprintf( stderr, "dynamic explosion %ld -> %ld", Value1, Value2 );

    break;

    case BDD_OVERFLOW_REF_INT_WARNING :

      fprintf( stderr, "internal reference overflow, index %ld", Value1 );

    break;

    case BDD_OVERFLOW_REF_EXT_WARNING :

      fprintf( stderr, "external reference overflow, index %ld", Value1 );

    break;

    default :

      fprintf( stderr, "unknown internal %d", Warning );
  }

  fprintf( stderr, " warning !\n" );
}
