/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rtlerror.c                       |
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

# include "rtlerror.h"

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

void rtl_error( Error, Text, File, Line )

  char  Error;
  char *Text;
  char *File;
  long  Line;
{
  char Name[ 128 ];

  strcpy( Name, File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "rtl:%s%ld ", Name, Line );

  switch( Error )
  {
    case RTL_UNKNOWN_RDS_OUT   :

      fprintf( stderr, "Unknown rds output format %s\n", Text );

      break;

    case RTL_UNKNOWN_RDS_IN    :

      fprintf( stderr, "Unknown rds input format %s\n", Text );

      break;

    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  EXIT( 1 );
}
