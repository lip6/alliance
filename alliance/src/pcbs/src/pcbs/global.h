/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <mut.h>
#include <log.h>
#include <mlo.h>
#include <beh.h>
#include <bvl.h>
#include <pat.h>
#include <phl.h>
#include <ppt.h>
/************************* version de PcBs ***********************************/
#define TRUE 1
#define FALSE 0
#define ENTETE PROGRAM_NAME " version " PCBS_VERSION " for boundary scan"
/************************ presentation des patterns **************************/
#define LABEL_BS_SIZE 30     /*taille des labels des patterns BS sauvegardées*/
#define LABEL_FCL_SIZE 15     /*taille des labels des patterns fonctionnelles*/
#define BS_ESPACE 2                            /*espaces entre les veteurs BS*/
/*****************************************************************************/
/************************** variables globales *******************************/
extern struct Infos *InfosVbe;  /* Pour les informations */
/* flags and vars for command line processing */
extern int verbose_flag;               /*affichage de tous les commmentaires */
extern int execute_flag;                                /*mode avec hardware */
extern int savePatSRC_flag;                          /*sauvegardr pattern // */
extern int savePatBS_flag;                          /*sauvegarder pattern BS */
extern int check_flag;                                /*est-ce le mode check */
extern int emulbs_flag;                  /*est-ce pour l'interface EMULBS31? */
/*****************************************************************************/

/* Les variables de chaque fichier .c */
#include  "emul31.h"
#include  "pcbs.h"
#include  "type_infos.h"
#include  "infos.h"
#include  "verif.h"
#include  "traduction.h"
#include  "util.h"
#include  "bstools.h"
#include  "parseur.h"
#include  "sig.h"
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/


