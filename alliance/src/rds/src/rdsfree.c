/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rdsfree.c                       |
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
# include <malloc.h>
# include <stdlib.h>
# include <string.h>


# include <mut.h>
# include "rds.h"

# include "rdsalloc.h"
# include "rdsfree.h"
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
/*------------------------------------------------------------\
|                                                             |
|                          Rds Free                           |
|                                                             |
\------------------------------------------------------------*/

void  rdsfree( Pointer, Size )

      char         *Pointer;
      unsigned int  Size;
{
  unsigned int Index;

  Index = ( Size + 3 ) >> 2;

  if ( ( Size  == 0              ) ||
       ( Index > 255             ) )
  {
    free( Pointer );
  }
  else
  {
    *((char **)Pointer) = RdsHeapHeadArray[ Index ];

    RdsHeapHeadArray[ Index ] = Pointer;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Rds Free Figure                        |
|                                                             |
\------------------------------------------------------------*/

void freerdsfig( Figure )

     rdsfig_list *Figure;
{
  rdsfreeblock( (char *)Figure->LAYERTAB );
  rdsfreeblock( (char *)Figure->FLAGTAB  );
  rdsfreeblock( (char *)Figure->BBOXTAB  );
  rdsfreeblock( (char *)Figure           );
}

/*------------------------------------------------------------\
|                                                             |
|                      Rds Free Instance                      |
|                                                             |
\------------------------------------------------------------*/

void freerdsins( Instance )

     rdsins_list *Instance;
{
  rdsfreeblock( (char *)Instance->LAYERTAB );
  rdsfreeblock( (char *)Instance->COURONNE );
  rdsfreeblock( (char *)Instance           );
}

/*------------------------------------------------------------\
|                                                             |
|                      Rds Free Rectangle                     |
|                                                             |
\------------------------------------------------------------*/

void freerdsrec( Rectangle, Size )

     rdsrec_list  *Rectangle;
     unsigned int  Size;
{
  rdsfreeheap( (char *)Rectangle, sizeof( rdsrec_list ) + Size);
}
