/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, MASI, CAO-VLSI Team				      ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL 	      ##*/
/*##                          (Projet Maitrise EEA)                   ##*/
/*##  Initial Revision   : April 1995                                 ##*/
/*##  Revision           : June  1995                                 ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr			      ##*/
/*##                                                                  ##*/
/*####==============================================================####*/

/*####==============================================================####*/
/*##                                                                  ##*/
/*##  lpscan.h   : Driver pour port parallele architecture PC/Linux   ##*/
/*##               Include file                                       ##*/
/*##                                                                  ##*/
/*####==============================================================####*/


#ifndef     _DRVCPARAL_
#define     _DRVCPARAL_

#define	    MAJOR_LPSCAN	60
#define     IN_USE	        1     /* port utilise? */
#define     MAX_BUF             256   /* buffer pour xfer (non utilise) */

/* adresses physiques du port parallele */

#define  Base_Adr   0x378      /*  LPT1 : 0x378                */
			       /*  LPT2 : 0x278                */
			       /*  LPT3 : 0x3BC   (a verifier) */
                               /*  LPT4 : 0x???   (suivant pc) */


#define  DATA       Base_Adr   /* registre DATA, en ecriture      */
			       /*  (OUT) only on most cards       */

#define  STATUS     Base_Adr+1 /* registre de STATUS, en lecture  */
			       /* (IN)                            */

#define  CONTROL    Base_Adr+2 /* registre de CONTROLE, lecture, ecriture */
			       /*   (IN/OUT)                              */



/* DATA REGISTER : OUT                     */
/* bits du registre de donnees de la carte */
#define         DATA0           0x01   /* DATA bit 0 */
#define         DATA1           0x02   /* DATA bit 1 */
#define         DATA2           0x04   /* DATA bit 2 */
#define         DATA3           0x08   /* DATA bit 3 */
#define         DATA4           0x10   /* DATA bit 4 */
#define         DATA5           0x20   /* DATA bit 5 */
#define         DATA6           0x40   /* DATA bit 6 */
#define         DATA7           0x80   /* DATA bit 7 */


/* STATUS REGISTER : IN                 */
/* bits du registre d'etat de la carte  */
/* STATUS bits 0,1,2 sont non definis   */
/* (peut etre a 0 sur certains pc)      */

#define         ERROR           0x08   /* STATUS bit 3          */
#define         SELECT          0x10   /* STATUS bit 4          */
#define         PE              0x20   /* STATUS bit 5          */
#define         ACK             0x40   /* STATUS bit 6          */
#define         BUSY            0x80   /* STATUS bit 7 !!!barre */


/* CONTROL REGISTER : IN/OUT                 */
/* bits du registre de controle de la carte  */
/* CONTROL bits 5,6,7 sont non definis       */

#define         STROBE          0x01   /* CONTROL bit 0 !!!barre */
#define         AUTOFEED        0x02   /* CONTROL bit 1 !!!barre */
#define         INIT            0x04   /* CONTROL bit 2          */
#define         SELECT_IN       0x08   /* CONTROL bit 3 !!!barre */
#define         IRQ_ENABLE      0x10   /* CONTROL bit 4          */
/* Si IRQ_ENABLE=1 alors une IRQ7 est generee lorsque ACK passe a 0 */
/* IRQ_ENABLE ne sort pas sur la DB25                               */

/* !!!barre signifie lecture et/ou ecriture de la valeur complementee */



/* commandes IOCTL du driver */

#define    CHECK_CARD_PRESENT     0x0001 /* pour la verification de        */
                                         /* presence de la carte parallele */
#define    CHECK_BSCARD_PRESENT   0x0002 /* pour la verification de        */
                                         /* presence de la carte boundary  */
                                         /* scan et de son alim            */
#define    GET_TDO_BUFFERED       0x0003 /* implementation de la fonction  */
                                         /* utilisateur getTDO dans le     */
                                         /* driver                         */
#endif
