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



#ifndef     _BSHARDHEADER_
#define     _BSHARDHEADER_

/* index des signaux boundary-scan  */
/* TCK   relie a DATA0              */ 
/* TRST  relie a DATA1              */ 
/* TMS   relie a DATA2              */ 
/* TDI   relie a DATA3              */ 
/* TDO   relie a STATUS10 (SELECT)  */ 
#define TDI_OFFSET 3
#define TMS_OFFSET 2
#define TRST_OFFSET 1
#define TCK_OFFSET 0
#define TDO_OFFSET 4  /* STATUS REGISTER de la carte parallele bit 4 */



/* motif que l'on envoi au TAP (mode check) */
#define SCAN_MOTIF      0xE04C4F48            /* "BS" (32 bits) */
#define MAX_SHIFT       5000 /* nb de shift max avant de laisser tomber */

/* code d'erreur renvoye lors d'un LPSCAN ERROR */
#define LPSCAN_ERROR    10


/* commandes IOCTL du driver LPSCAN */

#define    CHECK_CARD_PRESENT     0x0001 /* pour la verification de        */
					 /* presence de la carte paralelle */
#define    CHECK_BSCARD_PRESENT   0x0002 /* pour la verification de        */
					 /* presence de la carte boundary  */
					 /* scan et de son alim            */

/*****************************************************************************/
/*retourne le nom du device*/
extern char *current_lpscan_device();

/*ouvre le port parallele*/
extern void open_port(int DmaBurst_size);

/*ferme le port parallele*/
extern void close_port();

/*verifie que le hardware a des caracteristiques boundary scan et/ou emul31bs et renvoie 0 si carte bs bien connectee et 0 sinon*/
extern int check_mode( ); 

extern int write_read_port( char* buf, unsigned int size);

#endif
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/












