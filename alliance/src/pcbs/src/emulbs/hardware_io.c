/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Francois Donnet (2001)                     ##*/
/*##                                                                  ##*/
/*##  E-mail support:    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/

#include <sys/types.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "global.h"

#define NOMBRE_ESSAI_MAX 3
#define MAX_WAIT 10000


#ifndef NO_HARD   /* without the testing card*/

/* GLOBAL VARS **********************************************************/
static char hostname[256];
static char PORT_ALREADY_OPENED = 0 ;
#if 0
static char *PciBuf_Write_Read;     /* zone mem physique pour le dma*/
#endif

/* file descriptor of lpscan dev */
static int fd;

#endif


/*****************************************************************************/
/* FONTIONS SE SERVANT DU DRIVER LPSCAN *********************************/
/* elles exit(LPSCAN_ERROR) toutes sauf close_port en cas d'erreur */




/*****************************************************************************/
/*****************************************************************************/
extern char *current_lpscan_device()
{
   if ( pci_flag ) return "aniscan";
   else if ( lpscan_flag )  return "/dev/lpscan0";
   else
   {
     fprintf(stderr, "No device defined\n");
     exit(1);
     return NULL;
   }
}


/*****************************************************************************/
/*  fonction de reset du        */
/*  circuit , 5 cycles de       */
/*  TCK avec TMS=1              */
/*****************************************************************************/
static struct papat *reset_bs(struct papat* ppapat) 
{
  char valeur[5];
  int j;
 
  valeur[TDO]  = '*';
  valeur[TDI]  = '0';
  valeur[TMS]  = '1';
  valeur[TRST] = '1';
  valeur[TCK]  = '0';

  for(j = 0; j < 5; j++) { /* 5 cycle pour effectuer un reset */
    ppapat = TAP_in(ppapat, valeur, namealloc("Reset_BS"));
  }
  
  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Reset_BS"));
  
  /* Un coup de TRST */
  valeur[TRST] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Reset_BS"));
  
  /* retour a RUN_TEST_IDLE */
  valeur[TRST] = '1';
  ppapat = TAP_in(ppapat, valeur, namealloc("Reset_BS"));

  return ppapat;
}

/*****************************************************************************/
/*  renvoie la taille du scan        */
/*****************************************************************************/
static int get_motif(struct papat* ppapat) 
{
  long motif = 0;
  int i = 0;
  int simval = 0;
  struct paevt *paevt;

  for ( ppapat = ppapat->NEXT ; ppapat; ppapat = ppapat->NEXT )
  {
     for ( paevt = ppapat->PAEVT; paevt; paevt = paevt->NEXT )
     {
       if ( TDO == paevt->INDEX ) simval = paevt->SIMVAL == '+' ? 1 : 0;
     }
     motif <<= 1;
     motif  += simval;
     i++;
     if (motif == SCAN_MOTIF) break;
     /*skip the second cycle*/
     ppapat = ppapat->NEXT;
     if ( !ppapat ) break;
  }
  
  return i - 31 /*taille du motif*/;
}

/*****************************************************************************/
/*  fonction de send a motif        */
/*****************************************************************************/
static struct papat* shift_motif(struct papat* ppapat) 
{
  char valeur[5];
  long motif = SCAN_MOTIF;
  int i;
 
  /*  dans l ordre TDI TMS TRST TCK */
  valeur[TDO]  = '*';
  valeur[TDI]  = '0';
  valeur[TMS]  = '0';
  valeur[TRST] = '1';
  valeur[TCK]  = '0';


  /* shift en attendant de retrouver le motif */
  for (i = 1; i<=MAX_SHIFT + 31/*size of motif*/; i++)
  {
    valeur[TDI]  = ( (motif & 0x80000000) >> 31) + '0';
    if (i<=32) ppapat = TAP_in(ppapat, valeur, namealloc("Motif"));
    else       ppapat = TAP_in(ppapat, valeur, namealloc("Scan"));
    motif<<=1;
  }
  return ppapat;
}

/*****************************************************************************/
/*  fonction de shift_ir        */
/*****************************************************************************/
static struct papat* shift_ir(struct papat* ppapat) 
{
  char valeur[5];
  
  /*  dans l ordre TDI TMS TRST TCK */
  valeur[TDO]  = '*';
  valeur[TDI]  = '0';
  valeur[TMS]  = '0';
  valeur[TRST] = '1';
  valeur[TCK]  = '0';


  /*  RUN TEST IDLE  */     
  ppapat = TAP_in(ppapat, valeur, namealloc("Run_Test_Idle"));
  
  /*  Select DR  */
  valeur[TMS]  = '1';
  ppapat = TAP_in(ppapat, valeur, namealloc("Select_dr"));

  /*  Select IR  */
  ppapat = TAP_in(ppapat, valeur, namealloc("Select_ir"));

  /*  Capture IR  */
  valeur[TMS]  = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Capture_ir"));

  /*  Shift IR  */
  ppapat = TAP_in(ppapat, valeur, namealloc("Shift_ir"));

  return ppapat;
}

/*****************************************************************************/
/*  fonction de shift_dr        */
/*****************************************************************************/
static struct papat* shift_dr(struct papat* ppapat) 
{
  char valeur[5];
  
  /*  dans l ordre TDI TMS TRST TCK */
  valeur[TDO]  = '*';
  valeur[TDI]  = '0';
  valeur[TMS]  = '0';
  valeur[TRST] = '1';
  valeur[TCK]  = '0';


  /* RUN TEST IDLE */ 
  ppapat = TAP_in(ppapat, valeur, namealloc("Run_Test_Idle"));

  /* Select DR */ 
  valeur[TMS] = '1';
  ppapat = TAP_in(ppapat, valeur, namealloc("Select_dr"));

  /* Capture DR */ 
  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Capture_dr"));

  /* Shift DR */ 
  ppapat = TAP_in(ppapat, valeur, namealloc("Shift_dr"));

  return ppapat;
}

/*****************************************************************************/
/* fonction de verification de presence LPT1    */
/*****************************************************************************/
static void checkLPT() 
{

#ifndef NO_HARD   /* without the testing card*/

  int res;

  if (fd <= 0 )
  {
    fprintf(stderr, __FUNCTION__ ": Device is not opened\n");
    exit(LPSCAN_ERROR);
  }
  
  errno = 0;
  if ((res = (ioctl(fd, CHECK_CARD_PRESENT, NULL))) < 0 || errno ) {
    fprintf(stderr, "#### ");
    perror("Error accessing " PROGRAM_NAME " device (ioctl)");
    exit(LPSCAN_ERROR);
  }
  if (res != 1) {
    gethostname(hostname,255);
    fprintf(stderr, "Cannot find Boundary-scan Card on this machine (%s)\n",
	hostname);
    exit(LPSCAN_ERROR);
  }

#endif
  
}

/*****************************************************************************/
/*  fonction de determination   */
/*  de la longueur du registre  */
/*  instruction du circuit      */
/*****************************************************************************/
extern int check_mode( ) 
{
#ifndef NO_HARD   /* without the testing card*/
 
  int rinst=0;
  int rbs=0;
  int rbypass=0;
  Infos *inf;

  int j, i, lg_inst, lg_bs, lg_bypass;
  int emulbs = 1;/*pour l'instant c'est un hardware EMULBS31*/
  char valeur[5];
  struct papat *ppapat;


  
  for (inf=InfosVbe; inf; inf=inf->NEXT) 
  {
     /*taille des registres instructions concatenes*/
     rinst   += inf->INST_SIZE;
     /*taille de toutes les chaines bs concatenees*/
     rbs     += inf->BS_SIZE;
     /*longueur du bypass 1 bit de bypass*/
     rbypass += inf->BYPASS_SIZE;
  }   


  if ( !InfosVbe )
  {
    fprintf(stderr, __FUNCTION__ "No info file\n");
    exit(1);
  }
  
  /* --------------  */
  /*   Check du RI   */
  /* --------------  */

  /* BS : entrees positionnees avant front montant
	  sortie valides apres front descendant */
  open_port(BURST_SIZE);  /*  ouverture du port et reset BS */

  ppapat = reset_bs( NULL );
  ppapat = shift_ir( ppapat );

  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE);
  
  pat_frepapat( ppapat );
  
  /* send motif (shift IR) */
  /* shift en attendant de retrouver le motif */
  ppapat = shift_motif( NULL );
 
  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE);
  
  lg_inst = get_motif( ppapat );

  pat_frepapat( ppapat );
  

  if (lg_inst >= MAX_SHIFT) {
    fprintf(stderr, "Cannot check INSTRUCTION REGISTER (longer than %d bits ???)\n",lg_inst);
    fprintf(stderr, "You must check the card connection...\n");
    close_port();
    return(1);
  }
  
  fprintf(stdout,"INSTRUCTION REGISTER seems to be %d bits long\n",lg_inst);

  /* --------------  */
  /* Check du BYPASS */
  /* --------------  */

  /* Chargement de l'instruction BYPASS */
  /* depuis le reset                    */

  /* prevention des courts circuits */
  ppapat = reset_bs(NULL);
  ppapat = shift_ir( ppapat );

  /*  dans l ordre TDI TMS TRST TCK */
  valeur[TDO]  = '*';
  valeur[TDI]  = '1';
  valeur[TMS]  = '0';
  valeur[TRST] = '1';
  valeur[TCK]  = '0';

  for(i = 0; i < lg_inst - 1; i++) {  
    /* Instruction BYPASS : des 1 partout */
    ppapat = TAP_in(ppapat, valeur, namealloc("Shift_ir"));
  }
  
  
  /*  Exit1_ir  */     
  valeur[TMS] = '1';
  valeur[TDI] = '1';
  ppapat = TAP_in(ppapat, valeur, namealloc("Exit1_ir"));

  /* Update_ir */ 
  valeur[TDI] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Update_ir"));

  ppapat = shift_dr( ppapat );
  
  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE);
  
  pat_frepapat( ppapat );
  
  /* send motif (shift DR) */
  /* shift en attendant de retrouver le motif */
  ppapat = shift_motif( NULL );
 
  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE );
  
  lg_bypass = get_motif( ppapat );
  pat_frepapat( ppapat );
  
  
  if (lg_bypass >= MAX_SHIFT) {
    fprintf(stderr, "Cannot check BYPASS register (longer than %d bits ???)\n",lg_bypass);
    fprintf(stderr, "You must check the card connection...\n");
    close_port();
    return(1);
  }

  fprintf(stdout,"BYPASS register seems to be %d bits long\n",lg_bypass);
  

  /* ----------------------  */
  /* Check du BOUNDARY-SCAN  */
  /* ----------------------  */
  
  /* prevention des courts circuits */
  ppapat = reset_bs( NULL );

  ppapat = shift_ir( ppapat );

  /*  dans l ordre TDI TMS TRST TCK */
  valeur[TDO]  = '*';
  valeur[TDI]  = '0';
  valeur[TMS]  = '0';
  valeur[TRST] = '1';
  valeur[TCK]  = '0';
  
  for(i = 1; i <= lg_bypass; i++) 
  {  
    /* Instruction INTEST : que des 1 et un 0 */
    valeur[TDI]  = '1';
    for(j = 1; j < lg_inst/lg_bypass; j++)
    {  
      ppapat = TAP_in(ppapat, valeur, namealloc("Shift_ir"));
    }  
    valeur[TDI]  = '0';
    ppapat = TAP_in(ppapat, valeur, namealloc("Shift_ir"));
  }
  
  /*  Exit1_ir  */     
  valeur[TMS] = '1';
  valeur[TDI] = '1';
  ppapat = TAP_in(ppapat, valeur, namealloc("Exit1_ir"));

  /* Update_ir */ 
  valeur[TDI] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Update_ir"));

  /* Shift_dr */ 
  ppapat = shift_dr( ppapat );

  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE);
  
  pat_frepapat( ppapat );
  
  /* send motif (shift DR) */
  /* shift en attendant de retrouver le motif */
  ppapat = shift_motif( NULL );
 
  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE);
  
  lg_bs = get_motif( ppapat );
  pat_frepapat( ppapat );
  
  
  
  if (lg_bs >= MAX_SHIFT) {
    fprintf(stderr, "Cannot check BOUNDARY-SCAN register (longer than %d bits ???)\n",lg_bs);
    fprintf(stderr, "You must check the card connection...\n");
    close_port();
    return(1);
  }

  fprintf(stdout,"BOUNDARY-SCAN register seems to be %d bits long\n", lg_bs);

  close_port();


  if ( lg_bypass == 1 )
    fprintf(stdout,"Boundary scan chip recognized : RI=%d, BYPASS=%d, BS=%d\n",
		    lg_inst, lg_bypass, lg_bs);
  else fprintf(stdout,"%d Boundary scan chips recognized : RI=%d, BYPASS=%d, BS=%d\n",
		    lg_bypass, lg_inst/lg_bypass, lg_bypass/lg_bypass, lg_bs/lg_bypass);

  if ( rinst != lg_inst ) {
        fprintf(stdout,"Error : %s was attempting a INSTRUCTION REGISTER of %d bits long\n",
            PROGRAM_NAME,
            rinst);
        emulbs = 0;
  }

  if ( rbypass != lg_bypass ) {
        fprintf(stdout,"Error : %s was attempting a BYPASS of %d bits long\n", PROGRAM_NAME,
               rbypass);
        emulbs = 0;
  }

  if ( rbs != lg_bs ) {
        fprintf(stdout,"Error : %s was attempting a BOUNDARY-SCAN of %d bits long\n", 
                PROGRAM_NAME,   rbs);
        emulbs = 0;
  }

  if ( !emulbs )
  {
        fprintf(stdout,
          "Error : BOUNDARY-SCAN register seems to be other than the EMULBS31 card interface\n");
        close_port();
        return (1);
  }
  else fprintf(stdout,"The %d EMULBS31 have been recognized\n", lg_bypass);

#endif

  return(0);

}


/*****************************************************************************/
/*****************************************************************************/
extern int write_read_port( char* buf, unsigned int size)
{
#ifndef NO_HARD   /* without the testing card*/

   int i, nb_read, nb_write;
  
   if (lpscan_flag)
   {
      if (!fd)
      {
         fprintf(stderr,__FUNCTION__": device not opened\n");
         exit( 1 );
      }
   
      for (i=0; i<size; i++)
      {
         errno = 0;
         nb_write = write(fd, buf+i, 1);

         if ( errno ) 
         {
           perror("Error writing " PROGRAM_NAME " device");
           exit(LPSCAN_ERROR);
         }
         
         if ( nb_write != 1 ) 
         {
           fprintf(stderr, PROGRAM_NAME " can only write %d byte\n", nb_write);
           exit(LPSCAN_ERROR);
         }
         
         
         errno = 0;
         nb_read = read(fd, buf+i, 1);
         
         if ( errno ) 
         {
           perror("Error writing " PROGRAM_NAME " device");
           exit(LPSCAN_ERROR);
         }
         
         if ( nb_read != 1 ) 
         {
           fprintf(stderr, PROGRAM_NAME " can only read %d byte\n", nb_read);
           exit(LPSCAN_ERROR);
         }
         
      }   
   }
  else if ( pci_flag )
  {
#if 0
    if (!PciBuf_Write_Read)
    {
       fprintf(stderr,__FUNCTION__": device not opened\n");
       exit( 1 );
    }
    
    /*copie en memoire physique a une adresse fixe*/
    for (i=0; i<size; i++) *( PciBuf_Write_Read + i ) = *( buf + i );
    
    aniscan( PciBuf_Write_Read, size );
    
    /*recopie en memoire virtuelle*/
    for (i=0; i<size; i++)  *( buf + i ) = *( PciBuf_Write_Read + i );

#else
    fprintf(stderr, "PCI not yet implemented....\n");
    exit(1);
#endif
  } /*pci bus*/
  else
  {
    fprintf(stderr, "No device selected pci, parallel....\n");
    exit(1);
  }

#endif

   return 0;
}

/***************************************/
/*    fonction d ouverture du port //  */
/***************************************/
extern void open_port(int DmaBurst_size) 
{
#ifndef NO_HARD   /* without the testing card*/

  int erreur;
  int nombre_essai = 0;
  int wait;
  struct papat *ppapat;

  if (PORT_ALREADY_OPENED) return ;

  if (lpscan_flag)
  {
      errno = 0;
      fd = open(current_lpscan_device(), O_RDWR) ;

      /* On va essayer d'attendre si c'est occupe */
      while ((errno == EBUSY) && (fd < 0)) {

      	if (nombre_essai > NOMBRE_ESSAI_MAX) break;
   	   wait = 0 ; 
      	while (wait < MAX_WAIT) wait++;

      	fprintf(stderr,"%s device is busy. Trying again ...\n",PROGRAM_NAME);
            fd = open(current_lpscan_device(), O_RDWR) ;
   	   nombre_essai++;
  	   }

      if ( fd < 0 ) {
  	      erreur = errno;
         perror("Error opening " PROGRAM_NAME " device");

  	      switch (erreur) {
         case EBUSY : fprintf(stderr,
                   "%s device is currently in use. Please try later\n",PROGRAM_NAME);
  		       break;
         default     : 
              gethostname(hostname,255);
        	     fprintf(stderr,"Cannot find Boundary-scan Card on this machine (%s)\n"
  			      ,hostname);
  	      }
         exit(LPSCAN_ERROR);
	   }
      
  }/*lpscan*/
  else if ( pci_flag )
  {
#if 0
    ani_init( PciBuf_Write_Read, DmaBurst_size );
#else
    fprintf(stderr, "PCI not yet implemented....\n");
    exit(1);
#endif
  } /*pci bus*/
  else
  {
    fprintf(stderr, "No device selected pci, parallel....\n");
    exit(1);
  }
  
  PORT_ALREADY_OPENED = 1;
  
  checkLPT();         /* carte parallele OK ? */

  /* prevention des courts circuits */
  ppapat = reset_bs( NULL );
  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE);
  pat_frepapat( ppapat );

#endif
}


/*****************************************************************************/
/*    fonction de fermeture du port // */
/*****************************************************************************/
extern void close_port() 
{
#ifndef NO_HARD   /* without the testing card*/

  struct papat *ppapat;

  if (!PORT_ALREADY_OPENED) 	return ;

  
  /* prevention des courts circuits */
  ppapat = reset_bs( NULL );
  ppapat = (struct papat*) reverse( (chain_list*) ppapat);
  ppapat = hard_papat(ppapat, BURST_SIZE);
  pat_frepapat( ppapat );
  
  if (lpscan_flag)
  {
    errno = 0;
    if (close(fd) < 0 || errno ) {    /* fermeture du port // */
      fprintf(stderr, "#### ");
      perror("Error closing " PROGRAM_NAME " device");
      exit(-1);
    }
    fd = NULL;
  } /*lpscan*/
  else if ( pci_flag )
  {
#if 0
    /* desallouer? */
    PciBuf_Write_Read = NULL;
#else
    fprintf(stderr, "PCI not yet implemented....\n");
    exit(1);
#endif    
  } /*pci bus*/
  else
  {
    fprintf(stderr, "No device selected pci, parallel....\n");
    exit(1);
  }

  
  PORT_ALREADY_OPENED = 0;

#endif
}

/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/

