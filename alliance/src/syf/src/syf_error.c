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
| Tool    :                     SYF                           |
|                                                             |
| File    :                  syf_error.c                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"

# include <stdio.h>
# include <string.h>

# include "syf_error.h"

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
|                       SyfDisplayError                       |
|                                                             |
\------------------------------------------------------------*/

void SyfDisplayError( File, Line, Error, Text )

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
    case SYF_ERROR_OPEN_FILE :
      fprintf( stderr, "Unable to open file \"%s\"\n", Text );
    break;
    case SYF_ERROR_MISSING_SCAN_PORT :
      fprintf( stderr, "Missing scan-port declaration in figure \"%s\"\n",  Text );
    break;
    case SYF_ERROR_UNKNOWN_ENCODE_TYPE :
      fprintf( stderr, "Unknown encode type \"%c\"\n",  (char)((long)Text) );
    break;
    case SYF_ERROR_BAD_STACK_NO_PUSH :
      fprintf( stderr, "Bad stack, no \"push\" in figure \"%s\"\n",  Text );
    break;
    case SYF_ERROR_BAD_STACK_NO_POP :
      fprintf( stderr, "Bad stack, no \"pop\" in figure \"%s\"\n",  Text );
    break;
    case SYF_ERROR_SYNTAX :
      fprintf( stderr, "Syntax error %s\n",  Text );
    break;
    case SYF_ERROR_UNEXPECTED_EOF :
      fprintf( stderr, "Unexpected end of file error %s\n",  Text );
    break;
    case SYF_ERROR_UNKNOWN_STATE :
      fprintf( stderr, "Unknown state %s\n",  Text );
    break;
    case SYF_ERROR_DUPLICATE_CODE :
      fprintf( stderr, "Duplicate state code %s\n",  Text );
    break;
    case SYF_ERROR_WRONG_CODE :
      fprintf( stderr, "Bad state code %s\n", Text );
    break;
    case SYF_ERROR_BDD_ATOM_UNKNOWN :
      fprintf( stderr, "Unknown atom %s, unable to make BDD\n", Text );
    break;
    case SYF_ERROR_BDD_OPER :
      fprintf( stderr, "Illegal oper %ld, unable to make BDD\n", (long)Text );
    break;
    case SYF_ERROR_BDD_EXPR_NULL :
      fprintf( stderr, "Expression NULL, unable to make BDD\n" );
    break;
    case SYF_ERROR_BDD_LOOP :
      fprintf( stderr, "Loop on %s, unable to make BDD\n", Text );
    break;
    case SYF_ERROR_ORDER_ATOM_UNKNOWN :
      fprintf( stderr, "Unknown atom %s, unable to order BDD\n", Text );
    break;
    case SYF_ERROR_ORDER_OPER :
      fprintf( stderr, "Illegal oper %ld, unable to order BDD\n", (long)Text );
    break;
    case SYF_ERROR_ORDER_EXPR_NULL :
      fprintf( stderr, "Expression NULL, unable to order BDD\n" );
    break;
    case SYF_ERROR_ORDER_LOOP :
      fprintf( stderr, "Loop on %s, unable to order BDD\n", Text );
    break;

    default :
      fprintf( stderr, "Internal error number %d\n", Error );

  }

  autexit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       SyfDisplayWarning                     |
|                                                             |
\------------------------------------------------------------*/

void SyfDisplayWarning( File, Line, Warning, Text1, Text2 )

   char *File;
   int   Line;
   int   Warning;
   char *Text1;
   char *Text2;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%d ", Name, Line );

  switch ( Warning )
  {
    case SYF_WARNING_NO_TRANS_TO :
      fprintf( stderr, "No transition to \"%s\"\n", Text1 );
    break;
    case SYF_WARNING_NO_TRANS_FROM :
      fprintf( stderr, "No transition from \"%s\"\n", Text1 );
    break;
    case SYF_WARNING_REMOVE_UNUSED_STACK :
      fprintf( stderr, "Remove unused stack in figure \"%s\"\n", Text1 );
    break;
    case SYF_WARNING_REMOVE_STACK :
      fprintf( stderr, "Remove stack in state \"%s\"\n", Text1 );
    break;
    case SYF_WARNING_REMOVE_LOCOUT :
      fprintf( stderr, "Remove local output in state \"%s\"\n", Text1 );
    break;
    case SYF_WARNING_REMOVE_TRANS :
      fprintf( stderr, "Remove transition from \"%s\" to \"%s\"\n", Text1, Text2 );
    break;
    case SYF_WARNING_SUM_TRANS :
      fprintf( stderr, "All the possible combinations in \"%s\" are not evaluated\n", Text1 );
    break;
    case SYF_WARNING_INTER_TRANS :
      fprintf( stderr, "Intersection of all transitions leaving \"%s\" is non-empty\n", Text1 );
    break;
    case SYF_WARNING_OUTPUT_NOT_ASSIGNED :
      fprintf( stderr, "Output \"%s\" is assigned by only %ld states\n", Text1, (long)Text2 );
    break;
    case SYF_WARNING_MODIFY_POLARITY :
      fprintf( stderr, "Inverse the polarity of the output \"%s\"\n", Text1 );
    break;
    default :
      fprintf( stderr, "Internal warning number %d\n", Warning );
  }
}
