/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rdserror.c                       |
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
# include "rds.h"

# include "rdserror.h"

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

void rds_error( Error, Text, File, Line )

  char  Error;
  char *Text;
  char *File;
  long  Line;
{
  char Name[ 128 ];

  strcpy( Name, File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "rds:%s%ld ", Name, Line );

  switch( Error )
  {
    case RDS_ALLOC_ERROR     :
  
      fprintf( stderr, "alloc error, can't continue !\n");

    break;

    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  EXIT( 1 );
}
