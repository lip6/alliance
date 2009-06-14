/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rtlacces.c                      |
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
# include <string.h>
# include <libgen.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"

# include "rtlacces.h"
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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          loadrdsfig                         |
|                                                             |
\------------------------------------------------------------*/

void loadrdsfig( Figure, Name, Mode )

  rdsfig_list *Figure;
  char        *Name;
  char         Mode;
{
  if ( RDS_IN == RDS_GDS_FORMAT ) 
  {
    gdsloadrdsfig( Figure, Name, Mode );
  }
  else
  if ( RDS_IN == RDS_CIF_FORMAT )
  {
    cifloadrdsfig( Figure, Name, Mode );
  }
  else
  {
    rtlerror( RTL_UNKNOWN_RDS_IN, RDS_IN );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          saverdsfig                         |
|                                                             |
\------------------------------------------------------------*/

void saverdsfig( Figure )

  rdsfig_list *Figure;
{
  if ( RDS_OUT == RDS_GDS_FORMAT )
  {
    gdssaverdsfig( Figure );
  }
  else
  if ( RDS_OUT == RDS_CIF_FORMAT )
  {
    cifsaverdsfig( Figure );
  }
  else
  {
    rtlerror( RTL_UNKNOWN_RDS_OUT, RDS_OUT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          getrdsfig                          |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *getrdsfig( Name, Mode, Size )

   char        *Name;
   char         Mode;
   unsigned int Size;
{
  rdsfig_list *Figure;

  Name = namealloc( Name );

  for ( Figure  = HEAD_RDSFIG;
        Figure != (rdsfig_list *)NULL;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;
  }

  if ( ( Figure != (rdsfig_list *)NULL ) &&
       ( Mode   != Figure->MODE        ) )
  {
    if ( Figure->MODE == 'P' )
    {
      delrdsfig( Figure->NAME );
      Figure = (rdsfig_list *)NULL;
    }
    else
    {
      applyrdsmode( Figure, Mode );
    }
  }

  if ( Figure == (rdsfig_list *)NULL )
  {
    Figure = addrdsfig( Name, Size );

    loadrdsfig( Figure, Name, Mode );
  }

  return( Figure );
}
