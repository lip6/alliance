/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Rds                           |
|                                                             |
| File    :                  rdsadebug.c                      |
|                                                             |
| Date    :                   25.10.94                        |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <signal.h>

# include <mut.h>
# include "rds.h"

# include "rdsdebug.h"

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

  rdsdebug_list *HEAD_RDSDEBUG = (rdsdebug_list *)NULL;
  char           RDS_DEBUG_ON  = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Privates                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Rds Alloc Debug                      |
|                                                             |
\------------------------------------------------------------*/

rdsdebug_list *allocrdsdebug()
{
  return( (rdsdebug_list *)(rdsallocheap( sizeof( rdsdebug_list ) ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rds Free Debug                       |
|                                                             |
\------------------------------------------------------------*/

void freerdsdebug( Debug )

  rdsdebug_list *Debug;
{
  rdsfreeheap( (char *)Debug, sizeof( rdsdebug_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          traprdsdebug                       |
|                                                             |
\------------------------------------------------------------*/

void traprdsdebug()
{
  rdsdebug_list *ScanDebug;

  for ( ScanDebug  = HEAD_RDSDEBUG;
        ScanDebug != (rdsdebug_list *)NULL;
        ScanDebug  = ScanDebug->NEXT )
  {
    fprintf( stdout, "rdsdebug: file %s line %d\n", 
             ScanDebug->NAME, ScanDebug->LINE );
  }

  fflush( stdout );

  signal( SIGQUIT, traprdsdebug );
  signal( SIGSEGV, SIG_DFL      );
  signal( SIGBUS , SIG_DFL      );
  signal( SIGILL , SIG_DFL      );
}

/*------------------------------------------------------------\
|                                                             |
|                          rdsdebug                           |
|                                                             |
\------------------------------------------------------------*/

void rdsdebug()
{
  signal( SIGSEGV, traprdsdebug );
  signal( SIGBUS,  traprdsdebug );
  signal( SIGILL,  traprdsdebug );
  signal( SIGQUIT, traprdsdebug );

  RDS_DEBUG_ON = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                          addrdsdebug                        |
|                                                             |
\------------------------------------------------------------*/

void addrdsdebug( Line, File )

  int   Line;
  char *File;
{
  rdsdebug_list *NewDebug;

  NewDebug       = allocrdsdebug();
  NewDebug->NEXT = HEAD_RDSDEBUG;
  NewDebug->LINE = Line;
  NewDebug->NAME = File;
  HEAD_RDSDEBUG  = NewDebug;
}

/*------------------------------------------------------------\
|                                                             |
|                          delrdsdebug                        |
|                                                             |
\------------------------------------------------------------*/

void delrdsdebug()
{
  rdsdebug_list *DelDebug;

  DelDebug = HEAD_RDSDEBUG;

  if ( DelDebug != (rdsdebug_list *)NULL )
  {
    HEAD_RDSDEBUG = DelDebug->NEXT;
    freerdsdebug( DelDebug );
  }
}
