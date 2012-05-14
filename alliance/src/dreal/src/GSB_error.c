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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   DREAL                           |
|                                                             |
| File    :                 Gsb Errors                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   28.03.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "GSB.h"
# include "GSB_error.h"

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

void GsbError( ErrorType, Message, Data )

     char  ErrorType; 
     char *Message;
     long  Data;
{
  fflush(stdout);

  fprintf( stderr, "Gsb %s: ", DREAL_TECHNO_NAME );

  switch ( ErrorType ) 
  {
    case DEFINE_MISSING : 

      fprintf( stderr, "Some defines missing\n");
      break;

    case TABLE_MISSING  : 
 
      fprintf( stderr, "Some tables missing\n");
      break;

    case LINE_EXPECTED  :
 
      fprintf( stderr, "%s expected line %lu\n", 
      Message, Data );
      break;

    case UNKNOWN_DEFINE : 

      fprintf( stderr, "Unknown define %s line %lu\n",
      Message, Data );
      break;

    case NOT_DEFINED    : 

      fprintf( stderr, "%s not defined line %lu\n",
      Message, Data );
      break;

    case TOO_SMAL       :

      fprintf( stderr, "%s too smal line %lu\n",
      Message, Data );
      break;

    case MISSING_VALUE  :

      fprintf( stderr, "Missing value at line %lu\n",
      Data );
      break;

    case MISSING_NAME   :

      fprintf( stderr, "Missing name of %s line %lu\n",
      Message, Data );
      break;

    case UNEXPECTED_LINE :

      fprintf( stderr, "%s unexpected line %lu\n",
      Message, Data );
      break;

    case UNEXPECTED_EOF  :

      fprintf( stderr, "Unexpected end of file, missing definitions\n");
      break;

    case TOO_MANY_WORDS  :

      fprintf( stderr, "Too many words %s unexpected line %lu\n",
      Message, Data );
      break;

    case MISSING_TABLE  :

      fprintf( stderr, "Missing value in %s table line %lu\n",
      Message, Data );
      break;

    case OPEN_FILE      :

      fprintf( stderr, "Parameters file can't be opened\n" );
      break;

    case UNKNOWN_TABLE  :

      fprintf( stderr, "Unknown table %s line %lu\n",
      Message , Data );
      break;

    case SYNTAX_ERROR   :

      fprintf( stderr, "Syntax Error %s at line %lu\n", 
      Message , Data );
      break;

    case ILLEGAL_FLOAT  :

      fprintf( stderr, "Illegal floating point number %s line %lu\n",
      Message , Data );
      break;
  }

  fflush( stderr );

  exit( 1 );
}
