/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, MASI, CAO-VLSI Team                                   ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL               ##*/
/*##                          (Projet Maitrise EEA)                   ##*/
/*##  Initial Revision   : April 1995                                 ##*/
/*##  Revision                 : 20 may 1995                          ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/



#ifndef     _BSTESTHEADER_
#define     _BSTESTHEADER_

/* index des signaux boundary-scan  */
/* TCK   relie a DATA0              */ 
/* TRST  relie a DATA1              */ 
/* TMS   relie a DATA2              */ 
/* TDI   relie a DATA3              */ 
/* TDO   relie a STATUS10 (SELECT)  */ 
/*                                  */ 
/* ordre TCK TDI TDO TMS TRST       */
/* pattern :  "01?010"              */
#define TDI  0
#define TMS  3
#define TRST 4
#define TCK  2
#define TDO  1


/* .pat */
#define LABELSIZE 20
#define ABSENT 0
#define PRESENT 1

/* motif que l'on envoi au TAP (mode check) */
#define SCAN_MOTIF      0xE04C4F48            /* "BS" (32 bits) */
#define MAX_SHIFT       5000 /* nb de shift max avant de laisser tomber */

/* code d'erreur renvoye lors d'un LPSCAN ERROR */
#define LPSCAN_ERROR    10


/* STATUS REGISTER de la carte parallele */
#define         SELECT          0x10   /* STATUS bit 4          */

/* commandes IOCTL du driver LPSCAN */

#define    CHECK_CARD_PRESENT     0x0001 /* pour la verification de        */
					 /* presence de la carte paralelle */
#define    CHECK_BSCARD_PRESENT   0x0002 /* pour la verification de        */
					 /* presence de la carte boundary  */
					 /* scan et de son alim            */

/*****************************************************************************/
/*ouvre le port parallele*/
void open_port();

/*ferme le port parallele*/
void close_port();

/*envoie sur le hardware de patterns au format boundary scan source en n sequences, les parametres sont des noms de fichier*/
int send_pat_file(char *source,char *destination,unsigned int nbr_seq);

/*envoi d'une sequence de patterns sur le port paralelle et retourne le nombre de patterns erronnees*/
int send_paseq(struct papat *ppapat);

/*envoi d'une pattern sur le port paralelle et renvoie 0 si erreur sur valeur, 0 sinon*/
int send_papat(struct papat *ppapat);

/*envoi le parametre s dans l'ordre TDI TMS TRST TCK sur le DUT et renvoie la valeur de TDO*/
char getsimval(char *s);

/*verifie que le hardware a des caracteristiques boundary scan et/ou emul31bs et renvoie 0 si carte bs bien connectee et 0 sinon*/
int check_mode();

/*retourne le nom du driver de la carte externe*/
char *current_lpscan_device();

#endif
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/












