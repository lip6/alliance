/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/timeb.h>
#include <mut.h>
#include <log.h>
#include <mlo.h>
#include <beh.h>
#include <pat.h>
#include <phl.h>
#include <ppt.h>
/*****************************************************************************/

/* Les variables de chaque fichier .c */
#include  "type_infos.h"
#include  "infos.h"
#include  "verif.h"
#include  "traduction.h"
#include  "util.h"
#include  "bstools.h"
#include  "hardware_io.h"

/************************ presentation des patterns **************************/
#define BS_ESPACE 2                            /*espaces entre les veteurs BS*/
/****************************************************************************/
/*nombre de patterns par sequence chargee*/
#define BURST_SIZE 1024
#define PAT_SIZE 512
/*****************************************************************************/

/************************** variables globales *******************************/
extern struct Infos *InfosVbe;  /* Pour les informations */
/* flags and vars for command line processing */
extern int verbose_flag;               /*affichage de tous les commmentaires */
extern int execute_flag;                                /*mode avec hardware */
extern int lpscan_flag;              /*type of hardware device*/
extern int pci_flag;                 /*type of hardware device*/
extern int EXECUTION_ERRORS;         /*nombre d'erreurs a l'execution*/
extern struct paseq* PASEQ_PARALLEL; /*sequence de patterns en cours de traduction*/

/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/

