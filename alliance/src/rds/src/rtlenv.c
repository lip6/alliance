/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rtlenv.c                        |
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

# include <stdlib.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"

# include "rtlenv.h"
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

  char *RDS_IN;
  char *RDS_OUT;
  char *RDS_TECHNO_NAME;

  long  RDS_WINDOW_AREA = 0;

  char *RDS_CIF_FORMAT = (char *)NULL;
  char *RDS_GDS_FORMAT = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            rdsenv                           |
|                                                             |
\------------------------------------------------------------*/

void rdsenv()
{
  char *String;
  char  Buffer[ 512 ];

  RDS_TECHNO_NAME = mbkgetenv( "RDS_TECHNO_NAME");

  if ( RDS_TECHNO_NAME == (char *)NULL )
  {
    String = mbkgetenv( "ALLIANCE_TOP" );
    if ( String == (char *)NULL ) String = ALLIANCE_TOP;

    sprintf( Buffer, "%s/%s", String, RTL_DEFAULT_TECHNO );
    RDS_TECHNO_NAME = mbkstrdup( Buffer );
  }

  RDS_IN = namealloc( mbkgetenv( "RDS_IN") );

  if ( RDS_IN == (char *)NULL )
  {
    RDS_IN = namealloc( RTL_DEFAULT_IN );
  }
 
  RDS_OUT = namealloc( mbkgetenv( "RDS_OUT") );

  if ( RDS_OUT == (char *)NULL )
  {
    RDS_OUT = namealloc( RTL_DEFAULT_OUT );
  }

  String = mbkgetenv( "RDS_WINDOW_AREA") ;

  if ( String != (char *)NULL )
  {
    RDS_WINDOW_AREA = atol( String );
  }

  if ( RDS_WINDOW_AREA == 0 )
  {
    RDS_WINDOW_AREA = RTL_DEFAULT_AREA;
  }

  if( RDS_WINDOW_AREA < 0 )
  {
    RDS_WINDOW_AREA = 0;
  }

  RDS_GDS_FORMAT = namealloc( "gds" );
  RDS_CIF_FORMAT = namealloc( "cif" );
}
