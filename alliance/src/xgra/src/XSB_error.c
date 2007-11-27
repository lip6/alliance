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
| Tool    :                   XGRA                           |
|                                                             |
| File    :                 Xsb Errors                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
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
# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XSB_error.h"

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

void XsbError( ErrorType, Message, Data )

     char  ErrorType; 
     char *Message;
     long  Data;
{
  fflush(stdout);

  switch ( ErrorType ) 
  {
    case DEFINE_MISSING : 

      fprintf( stderr, "Xsb: Some defines missing\n");
      break;

    case TABLE_MISSING  : 
 
      fprintf( stderr, "Xsb: Some tables missing\n");
      break;

    case LINE_EXPECTED  :
 
      fprintf( stderr, "Xsb: %s expected line %lu\n", 
      Message, Data );
      break;

    case UNKNOWN_DEFINE : 

      fprintf( stderr, "Xsb: Unknown define %s line %lu\n",
      Message, Data );
      break;

    case NOT_DEFINED    : 

      fprintf( stderr, "Xsb: %s not defined line %lu\n",
      Message, Data );
      break;

    case TOO_SMAL       :

      fprintf( stderr, "Xsb: %s too smal line %lu\n",
      Message, Data );
      break;

    case MISSING_VALUE  :

      fprintf( stderr, "Xsb: Missing value at line %lu\n",
      Data );
      break;

    case MISSING_NAME   :

      fprintf( stderr, "Xsb: Missing name of %s line %lu\n",
      Message, Data );
      break;

    case UNEXPECTED_LINE :

      fprintf( stderr, "Xsb: %s unexpected line %lu\n",
      Message, Data );
      break;

    case UNEXPECTED_EOF  :

      fprintf( stderr, "Xsb: Unexpected end of file, missing definitions\n");
      break;

    case TOO_MANY_WORDS  :

      fprintf( stderr, "Xsb: Too many words %s unexpected line %lu\n",
      Message, Data );
      break;

    case MISSING_TABLE  :

      fprintf( stderr, "Xsb: Missing value in %s table line %lu\n",
      Message, Data );
      break;

    case OPEN_FILE      :

      fprintf( stderr, "Xsb: Parameters file %s can't be opened\n", 
      Message );
      break;

    case UNKNOWN_TABLE  :

      fprintf( stderr, "Xsb: Unknown table %s line %lu\n",
      Message , Data );
      break;

    case SYNTAX_ERROR   :

      fprintf( stderr, "Xsb: Syntax Error %s at line %lu\n", 
      Message , Data );
      break;

    case ILLEGAL_FLOAT  :

      fprintf( stderr, "Xsb: Illegal floating point number %s line %lu\n", 
      Message , Data );
      break;
  }

  exit( 1 );
}
