/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rwialloc.c                       |
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
# include <math.h>

# include <mut.h>
# include "rds.h"
# include "rwi.h"
# include "rut.h"

# include "rwialloc.h"
# include "rwierror.h"

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
|                        Alloc Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Alloc Rds Win Rec                      |
|                                                             |
\------------------------------------------------------------*/

rdswinrec_list *allocrdswinrec()
 
{
  return( (rdswinrec_list *)(rdsallocheap( sizeof(rdswinrec_list) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Alloc Rds Rec Win                      |
|                                                             |
\------------------------------------------------------------*/

rdsrecwin_list *allocrdsrecwin()
 
{
  return((rdsrecwin_list *)(rdsallocheap( sizeof(rdsrecwin_list) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Alloc Rds Window                     |
|                                                             |
\------------------------------------------------------------*/

rdswin_list *allocrdswin( Number )

  unsigned long Number;
{
  return((rdswin_list *)(rdsallocblock( sizeof(rdswin_list) * Number ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Alloc Rds Window                     |
|                                                             |
\------------------------------------------------------------*/

rdswin_list *allocrdswinlayer( RdsWin )

  rdswin_list *RdsWin;
{
  if ( RdsWin->LAYERTAB == (rdswinrec_list **)NULL )
  {
    RdsWin->LAYERTAB = (rdswinrec_list **)rdsallocblock( sizeof(rdswinrec_list *) * RWI_MAX_LAYER );
  }

  return( RdsWin );
}

/*------------------------------------------------------------\
|                                                             |
|                       Alloc Rds Window                      |
|                                                             |
\------------------------------------------------------------*/

rdswindow *allocrdswindow() 

{
  return((rdswindow *)(rdsallocblock(sizeof(rdswindow) ) ) );
}
