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

/* ordre dans pattern TDI TDO TMS TRST TCK      */
#define TDI  0
#define TDO  1
#define TMS  2
#define TRST 3
#define TCK  4

#define TDI_NAME  "tdi"
#define TMS_NAME  "tms"
#define TRST_NAME "trst"
#define TCK_NAME  "tck"
#define TDO_NAME  "tdo"





/*****************************************************************************\
        fonction                 
 envoie d'une serie dePAPAT sur le port  
     // du PC LINUX              
\*****************************************************************************/
extern struct papat *hard_papat(struct papat *head, unsigned int burst_size) ;

/*****************************************************************************\
        fonction                 
 envoie d'une PASEQ sur le port  
     // du PC LINUX              
\*****************************************************************************/
extern struct paseq* hard_paseq( struct paseq* pat, unsigned int burst_size, unsigned long *time_tck );

/*****************************************************************************\
        fonction                 
   save some patterns
\*****************************************************************************/
extern void save_patterns(char *file, struct paseq* pat, unsigned long *time_io);

/*****************************************************************************\
        fonction                 
   load some patterns
\*****************************************************************************/
extern struct paseq* load_patterns(char *file, struct paseq* pat, unsigned int nb_pat, unsigned long *time_io) ;

/*****************************************************************************\
        fonction                 
 envoie d'une PASEQ sur le port  
     // du PC LINUX              
\*****************************************************************************/
extern void hard_paseq_file(char *file, char *resfile, unsigned int nb_pat, unsigned int burst_size); 


#endif
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/












