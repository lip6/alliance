/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##  Authors            : Olivier FLORENT (1997)                     ##*/
/*##                                                                  ##*/
/*##  E-mail support:    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
#include "global.h"
/*****************************************************************************/
/*fermer les ports, desallouer la memoire et exit*/
void pcbs_exit(code)
int code;
{
del_infos();
close_port();
exit(code);
}
/*****************************************************************************/
/*sortie avec message sur interruption*/
void emergency(sig)
int sig;
{
char nomsig[24];

switch (sig) {
  case SIGTERM : strcpy(nomsig,"SIGTERM"); break ;
  case SIGINT  : strcpy(nomsig,"SIGINT "); break ;
  case SIGBUS  : strcpy(nomsig,"SIGBUS "); break ;
  case SIGSEGV : strcpy(nomsig,"SIGSEGV"); break ;
  default      : strcpy(nomsig,"(???)")  ; break;
  }

fprintf(stderr,"#### Emergency OUTPUT on signal %s ...\n",nomsig);
pcbs_exit(-1);
}
/*****************************************************************************/
/*mise en place des interruptions*/
void error_signal_handler()
{
signal(SIGTERM ,emergency);
signal(SIGINT  ,emergency);
signal(SIGBUS  ,emergency);
signal(SIGSEGV ,emergency);
}
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/








