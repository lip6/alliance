/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rprerror.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"

# include "rprerror.h"

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

void rpr_error( Error, Text, Data, File, Line )

  char  Error;
  char *Text;
  long  Data;
  char *File;
  long  Line;
{
  char Name[ 128 ];

  strcpy( Name, File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "rpr:%s%ld error ", Name, Line );

  switch( Error )
  {
    case RPR_DEFINE_MISSING : 
  
      fprintf( stderr, "some defines missing\n");
      break;
  
    case RPR_TABLE_MISSING  : 
   
      fprintf( stderr, "some tables missing\n");
      break;
  
    case RPR_LINE_EXPECTED  :
   
      fprintf( stderr, "%s expected line %lu\n", Text, Data );
      break;
  
    case RPR_UNKNOWN_DEFINE : 
  
      fprintf( stderr, "unknown define %s line %lu\n",
      Text, Data );
      break;
  
    case RPR_NOT_DEFINED    : 
  
      fprintf( stderr, "%s not defined line %lu\n",
      Text, Data );
      break;
  
    case RPR_MULTIPLE_GRID  : 
   
      fprintf( stderr, "%s not multiple of physical grid line %lu\n",
      Text, Data );
      break;
  
    case RPR_TOO_SMAL       :
  
      fprintf( stderr, "%s too smal line %lu\n",
      Text, Data );
      break;
  
    case RPR_MISSING_VALUE  :
  
      fprintf( stderr, "missing value line %lu\n", Data );
      break;
  
    case RPR_MISSING_NAME   :
  
      fprintf( stderr, "missing name of %s line %lu\n",
      Text, Data );
      break;
  
    case RPR_ILLEGAL_FLOAT  :
  
      fprintf( stderr, "illegal floating point number %s line %lu\n",
      Text, Data );
      break;
  
    case RPR_UNEXPECTED_LINE :
  
      fprintf( stderr, "%s unexpected line %lu\n",
      Text, Data );
      break;
  
    case RPR_UNEXPECTED_EOF  :
  
      fprintf( stderr, "unexpected end of file, missing definitions\n");
      break;
  
    case RPR_TOO_MANY_WORDS  :
  
      fprintf( stderr, "too many words %s unexpected line %lu\n",
      Text, Data );
      break;
  
    case RPR_MISSING_TABLE  :
  
      fprintf( stderr, "table %s Missing value line %lu\n",
      Text, Data );
      break;
  
    case RPR_OPEN_FILE      :
  
      fprintf( stderr, "can't open file\n" );
      break;
  
    case RPR_MISSING_POST   :
  
      fprintf( stderr, "missing %s value in postreat table line %lu\n",
      Text , Data );
      break;
  
    case RPR_NEGATIVE_VALUE :
  
      fprintf( stderr, "%s has to be positive value line %lu\n",
      Text , Data );
      break;
  
    case RPR_SYNTAX_ERROR   :
  
      fprintf( stderr, "syntax Error %s at line %lu\n",
      Text , Data );
      break;

    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  fprintf( stderr, "in technological file %s\n", RDS_TECHNO_NAME );

  EXIT( 1 );
}
