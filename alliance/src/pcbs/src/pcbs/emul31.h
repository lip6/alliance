/***************************** taille PGA ************************************/
#define NBR_LIGNES_PGA PCBS_PGA_SIZE                         /*hauteur du PGA*/
#define NBR_COLONNES_PGA PCBS_PGA_SIZE                       /*largeur du PGA*/
#define NBR_DEB_PGA 1                                /*premier chiffre du PGA*/
#define NBR_FIN_PGA NBR_COLONNES_PGA                 /*dernier chiffre du PGA*/
#define LETTRE_DEB_PGA 'a'                           /*premiere lettre du PGA*/
#define LETTRE_FIN_PGA 'a'+NBR_LIGNES_PGA+1          /*derniere lettre du PGA*/
/*********** Donnees relatives a l'ensemble Boundary Scan ********************/
#define INS_LEN 2      /*taille du registre instruction de la carte EMULBS 31*/
#define NBR_CELLS 31     /*nombre de cellules BS dans EMUL31    Rq: de 0 a 30*/
#define BYPASS_SIZE 1         /*revient a indiquer le nbr de cycles de retard*/
#define _BYPASS0 "00"                          	      /*instructions d'EMUL31*/
#define _BYPASS1 "11"
#define _INTEST  "10"
#define _CONFIG  "01"
/*****************************************************************************/
#define EMUL_FILE_NAME "emul_connexions"    /*pour la verification du parseur*/
#define PGA_FILE_NAME  "pga_connexions"     /*pour la verification du parseur*/
/*****************************************************************************/


/*****************************************************************************/
/***************************** END OF FILE ***********************************/
/*****************************************************************************/












