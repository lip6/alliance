/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   ruterror.c                      |
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

# include "ruterror.h"

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

void rut_error( Error, Text, File, Line )

  char  Error;
  char *Text;
  char *File;
  long  Line;
{
  char Name[ 128 ];

  strcpy( Name, File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "rut:%s%ld ", Name, Line );

  switch( Error )
  {
    case RDS_MODEL_NOT_FOUND   :

      fprintf( stderr, "Model %s not found in rds structure\n", Text );
 
    break;

    case RDS_NOT_ENOUGH_MEMORY :

      fprintf( stderr, "Not enough memory while spliting polygons in %s\n", Text );

    break;

    case RDS_INVALID_FIGURE_SIZE :

      fprintf( stderr, "Invalid figure size, can't extract equi in %s\n", Text );

    break;

    default :

      fprintf( stderr, "unknown internal error %d !\n", Error );
  }

  EXIT( 1 );
}
