/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/

/* POUR LE DEBUG */
#undef DUMMY

#include "global.h"
#include <sys/time.h>

/* FUNCTION DECL *******************************************************/
static void checkLPT();
static void outout();
static char inin();
static void reset_bs();
static int verif_io();
/* send a pattern ("0110") ordre TDI TMS TRST TCK */
static void sendTAP();
/* send a pattern ("0110") and return TDO = 1 or 0 */
static char getTDO();

/* GLOBAL VARS **********************************************************/
static char hostname[256];

/* file descriptor of lpscan dev */
static int fd;

/* check BS i/o in .pat file */
static int table_index[5];
static int flag_TRST;

/*nombre de patterns BS deja envoyees*/
static unsigned int BSpat=0;
/*****************************************************************************/

char *current_lpscan_device()
{
#ifdef  DUMMY
return "/dev/null" ;
#else
return "/dev/lpscan0";
#endif
}
/*****************************************************************************/
/* FONTIONS SE SERVANT DU DRIVER LPSCAN *********************************/
/* elles exit(LPSCAN_ERROR) toutes sauf close_port en cas d'erreur */

/***************************************/
/*    fonction d ouverture du port //  */
/***************************************/

char PORT_ALREADY_OPENED = FALSE ;
#define NOMBRE_ESSAI_MAX 3
#define MAX_WAIT 10000

void open_port() {
  int erreur;
  int nombre_essai;
  int wait;

  nombre_essai = 0;

  fd = open(current_lpscan_device(), O_RDWR) ;

  /* On va essayer d'attendre si c'est occupe */
  while ((errno == EBUSY) && (fd < 0)) {

	if (nombre_essai > NOMBRE_ESSAI_MAX)
		break;
	wait = 0 ; 
	while (wait < MAX_WAIT)
		wait++;

	fprintf(stderr,"%s device is busy. Trying again ...\n",PROGRAM_NAME);
        fd = open(current_lpscan_device(), O_RDWR) ;
	nombre_essai++;
	}

  if ( fd < 0 ) {
	erreur = errno;
        perror("Error opening " PROGRAM_NAME " device");

	switch (erreur) {
	 case EBUSY : fprintf(stderr,"%s device is currently in use. Please try later\n",PROGRAM_NAME);
		       pcbs_exit(LPSCAN_ERROR);
		       break;
         default     : 
          gethostname(hostname,255);
    	  fprintf(stderr,"Cannot find Boundary-scan Card on this machine (%s)\n"
			,hostname);
	 }
         pcbs_exit(LPSCAN_ERROR);

	}

  PORT_ALREADY_OPENED = TRUE;
  checkLPT();         /* carte parallele OK ? */
  reset_bs();         /* reset du BS du circuit */
}
/*****************************************************************************/
/***************************************/
/*    fonction de fermeture du port // */
/***************************************/
void close_port() {

if (PORT_ALREADY_OPENED == FALSE) 
	return ;

  reset_bs();         /* reset du BS du circuit */
  if (close(fd) < 0) {    /* fermeture du port // */
    fprintf(stderr, "#### ");
    perror("Error closing " PROGRAM_NAME " device");
    pcbs_exit(-1);
  }
PORT_ALREADY_OPENED = FALSE;
}
/*****************************************************************************/
/***************************************/
/*   fonction ecriture du port //      */
/***************************************/
static void outout(what)
  int what;
{
  char value;
  
  value =  what;
  if(write(fd, &value, 1) < 0) {
    fprintf(stderr, "#### ");
    perror("Error writing " PROGRAM_NAME " device");
    close_port();
    pcbs_exit(LPSCAN_ERROR);
  }
}
/*****************************************************************************/
/***********************************/
/* fonction lecture du port //     */
/***********************************/
static char inin() {
  char value = 0;

  if(read(fd, &value, 1) < 0) {
    fprintf(stderr, "#### ");
    perror("Error reading " PROGRAM_NAME " device");
    pcbs_exit(LPSCAN_ERROR);
  }
  return((value & SELECT) != 0);
}
/*****************************************************************************/
/************************************************/
/* fonction de verification de presence LPT1    */
/************************************************/
static void checkLPT() {
  int res;

#ifndef DUMMY
  if ((res = (ioctl(fd, CHECK_CARD_PRESENT, NULL))) < 0) {
    fprintf(stderr, "#### ");
    perror("Error accessing " PROGRAM_NAME " device (ioctl)");
    pcbs_exit(LPSCAN_ERROR);
  }
  if (res != 1) {
    gethostname(hostname,255);
    fprintf(stderr, "Cannot find Boundary-scan Card on this machine (%s)\n",
	hostname);
    pcbs_exit(LPSCAN_ERROR);
  }
#endif
}
/*****************************************************************************/
/*****************************************/
/* send a pattern (eg:"0110") to the TAP */
/*         ordre TDI TMS TRST TCK        */
/*****************************************/
static void sendTAP(vecteur)
  char *vecteur;
{
  outout(abintoi(vecteur));
}

/*****************************************************************************/
/**************************************************/
/* fonction d'envoie du vecteur                   */
/* send a pattern ("0110") and return TDO= + or - */
/**************************************************/
char getsimval(vecteur) 
  char *vecteur;
{
  sendTAP(vecteur); /*vecteur = TDI, TMS, TRST, TCK, '\0'*/
  return ((inin() > 0) ? '+' : '-'); /* TDO = + ou - */
}

/*****************************************************************************/
/**************************************************/
/* send a pattern ("0110") and return TDO= 1 or 0 */
/**************************************************/
static char getTDO(vecteur)
  char *vecteur;
{
  sendTAP(vecteur);
  return (inin());                  /* TDO = 1 ou 0 */
}
/*****************************************************************************/
/********************************/
/*  fonction de reset du        */
/*  circuit , 5 cycles de       */
/*  TCK avec TMS=1              */
/********************************/
static void reset_bs() {
  int j;

  for(j = 0; j < 5; j++) { /* 5 cycle pour effectuer un reset */
    /* ordre TDI TMS TRST TCK   */
    getTDO("0110");    /* TCK = 0 TMS = 1 TDI = 0   */
    getTDO("0111");    /* TCK = 1 TMS = 1 TDI = 0   */
  }
  getTDO("0010");
  getTDO("0011");
  
  /* Un coup de TRST */
  getTDO("0000");
  getTDO("0001");
  
  /* retour a RUN_TEST_IDLE */
  getTDO("0010");
  getTDO("0011");
  
}
/*****************************************************************************/
/*******************************************************/
/* Process des pattern du .pat de la ligne de commande */
/*******************************************************/
int send_pat_file(file,resfile,sequence) 
char *file,*resfile;
unsigned int sequence;
{
struct timeval start_tck,end_tck,start_io,end_io,start_prg,end_prg;
unsigned long time_tck=0,time_io=0,time_prg=0;       /* to compute exec time */
struct paseq *pat = NULL;                                   /* for .pat file */
int process_error=0;                                              /* error ? */
int nbr_pat=0;                                    /*nbr de patterns bs total */


/***INITIALISATION ET COHERENCE***/
  gettimeofday(&start_prg,NULL);

  /* Lecture du fichier de patterns 'paralleles' et on les compte*/
  fprintf(stdout,"Loading from %s.pat...",file);
  fflush(stdout);
  gettimeofday(&start_io,NULL);
  if ((nbr_pat=compte_pat(file))<0) 
    {fprintf(stderr," Loading failed!\n");return 1;}
  gettimeofday(&end_io,NULL);
  time_io+=(end_io.tv_sec-start_io.tv_sec)*1000 + (end_io.tv_usec-start_io.tv_usec)/1000;



/*les fichiers boundary scan sont grands donc partitionner*/
  gettimeofday(&start_io,NULL);
  pat = pat_lodpaseq (file, NULL, sequence, 0);
  gettimeofday(&end_io,NULL);
  time_io+=(end_io.tv_sec-start_io.tv_sec)*1000 + (end_io.tv_usec-start_io.tv_usec)/1000;
  if (pat==NULL || pat->ERRFLG) 
    {fprintf(stderr,"Loading failed!\n");return 1;}
 
  if (verif_io(pat) == 1) {
    fprintf(stderr,
	    "\nFile %s does not contain boundary-scan IO (tdi, tck, ...)\n", 
	    file);
    return(1);}
  else fprintf(stdout," %d boundary scan patterns\n",nbr_pat);

  open_port();  /*  ouverture du port et reset BS */

  /* envoie des patterns sur la carte */
  gettimeofday(&start_tck,NULL);
  process_error += send_paseq(pat->CURPAT);
  gettimeofday(&end_tck,NULL);
  time_tck += 
       (end_tck.tv_sec-start_tck.tv_sec)*1000 + (end_tck.tv_usec-start_tck.tv_usec)/1000;
  pat_frepacom(pat->CURCOM);                /*commentaire debut fichier*/
  pat    -> CURCOM = pat_addpacom(NULL,ENTETE,0); 

  /*sauvegarde*/
  if (savePatBS_flag) pat_savpaseq(resfile, pat, 20);     


/****CONTINUER****/
 while (pat!=NULL && pat->ENDFLG!='Y') {
    /*affichage pour attendre*/   
    if (verbose_flag) {
	 fprintf(stdout,"%.1f%%   %d bs patterns, continuing...\n",
		 (float)((float)(BSpat*100))/nbr_pat,BSpat);}
    else {
	 fprintf(stdout,"\r%.1f%%   %d bs patterns, continuing...",
		 (float)((float)(BSpat*100))/nbr_pat,BSpat);
	 fflush(stdout);}
  
       gettimeofday(&start_io,NULL);
       pat = pat_lodpaseq (file, pat, sequence, 0);
       gettimeofday(&end_io,NULL);
       time_io+=
	    (end_io.tv_sec-start_io.tv_sec)*1000 + (end_io.tv_usec-start_io.tv_usec)/1000;
       if (pat==NULL || pat->ERRFLG) 
          {fprintf(stderr,"Loading failed!\n");return 1;}

       /* envoie des patterns sur la carte */
       gettimeofday(&start_tck,NULL);
       process_error += send_paseq(pat->CURPAT->NEXT);/*du au n+1 patterns deja chargees par pat_lodpaseq*/
       gettimeofday(&end_tck,NULL);
       time_tck += 
	 (end_tck.tv_sec-start_tck.tv_sec)*1000+(end_tck.tv_usec-start_tck.tv_usec)/1000;

       /*sauvegarde*/
       if (savePatBS_flag) pat_savpaseq(resfile, pat, 20);     
  }


  close_port();  /*  reset BS et fermeture du port */

  if (savePatBS_flag) {
       fprintf(stdout,"Saving execution results in %s.pat\n",resfile);}


/****STATISTIQUE****/
  /*verification*/
  if (process_error)
    fprintf(stdout,
	    "\rExecution of %s on DUT has FAILED (%d error(s))\n",file,
	    process_error);
  else fprintf(stdout,"\rExecution of %s on DUT is successful\n",file);

  /*temps*/
  gettimeofday(&end_prg,NULL);
  time_prg=
       (end_prg.tv_sec-start_prg.tv_sec)*1000 + (end_prg.tv_usec-start_prg.tv_usec)/1000;
  affiche_time(time_prg);
  fprintf(stdout,"   but only ");fflush(stdout);
  affiche_time(time_tck);
  fprintf(stdout," for vectors --> ");fflush(stdout);

  /*frequence --> seuls les fronts d'horloge*/
  if (time_tck!=0) time_tck=(nbr_pat*500)/time_tck;
  if (time_tck<1000)
    fprintf(stdout,"%ld Hz on TCK\n",time_tck);
  else if (time_tck<1000000)
    fprintf(stdout,"%ld.%03ld KHz on TCK\n",time_tck/1000,time_tck%1000);
  else 
    fprintf(stdout,"%ld.%03ld MHz on TCK\n",
	    time_tck/1000000,(time_tck/1000)%1000000);

  /*les entrees/sortie*/
  fprintf(stdout,"and %.1f%% of time was for i/o file\n",
	  (float)((float)time_io*100)/time_prg);

erase_paseq(pat);
  return (process_error);
}
/*****************************************************************************/
/***********************************/
/*        fonction                 */
/* envoie d'une PASEQ sur le port  */
/*     // du PC LINUX              */
/***********************************/
int send_paseq(ppapat) 
  struct papat *ppapat;
{
  struct paevt *ppaevt, *ptdo;
  int flag_evt_tdo, fail_flag = 0;
  static char vecteur[5]={'0','0','0','0','\0'};        /*TDI, TMS, TRST, TCK*/
  static char vect_tdo[1]={'*'};                                   /*TDO*/
	
  while(ppapat != NULL) { 
    flag_evt_tdo = ABSENT ;
    for(ppaevt = ppapat -> PAEVT; ppaevt; ppaevt = ppaevt -> NEXT) {   
      if(ppaevt -> INDEX == table_index[TCK]) 
	vecteur[3] = ppaevt -> USRVAL;/*TCK*/
      if(ppaevt -> INDEX == table_index[TRST]) 
	vecteur[2] = ppaevt -> USRVAL;  /*TRST*/
      if(ppaevt -> INDEX == table_index[TMS]) 
	vecteur[1] = ppaevt -> USRVAL;  /*TMS*/
      if(ppaevt -> INDEX == table_index[TDI]) 
	vecteur[0] = ppaevt -> USRVAL;  /*TDI*/
      if(ppaevt -> INDEX == table_index[TDO]) {
	flag_evt_tdo = PRESENT;
	vect_tdo[0] = ppaevt -> USRVAL;
	ptdo = ppaevt ;
      }
    }/*fin de la boucle sur les evenements*/
    if(flag_evt_tdo == ABSENT) {
      ppapat -> PAEVT = pat_addpaevt(ppapat -> PAEVT, table_index[TDO], vect_tdo[0]);
      ptdo = ppapat -> PAEVT ;
    }
    if(flag_TRST == ABSENT)
      vecteur[2]='1';/*TRST*/
    vecteur[4]='\0' ;    /*fin du string*/
    ptdo -> SIMVAL = getsimval(vecteur);   /* envoie du vecteur */
    if((ptdo -> USRVAL != '*') && (ptdo -> USRVAL != ptdo -> SIMVAL)) {
      if (verbose_flag)
	   fprintf(stderr, 
		   "Error on pattern %d (Line %d)"
		   " : expected '%c' actual '%c'  on TDO\n",
		   BSpat,
		   ppapat->LINE,
		   ptdo->USRVAL,
		   ptdo->SIMVAL);
      fail_flag ++;
    }
    ppapat -> SIMFLAG = 'S';
    ppapat = ppapat -> NEXT;
    BSpat++;
  }/*fin boucle sur les patterns*/
 
  return fail_flag ;
}
/*****************************************************************************/
/***********************************/
/*        fonction                 */
/* envoie d'une PAPAT sur le port  */
/*     // du PC LINUX              */
/***********************************/
int send_papat(ppapat) 
  struct papat *ppapat;
{   
  struct paevt *ppaevt, *ptdo;
  int flag_evt_tdo, fail_flag = 0;
  static char vecteur[5]={'0','0','0','0','\0'};        /*TDI, TMS, TRST, TCK*/
  static char vect_tdo[1]={'*'};

  
  flag_evt_tdo = ABSENT ;
  for(ppaevt = ppapat -> PAEVT; ppaevt; ppaevt = ppaevt -> NEXT) {
    if(ppaevt -> INDEX == table_index[TCK]) 
      vecteur[3] = ppaevt -> USRVAL;/*TCK*/
    if(ppaevt -> INDEX == table_index[TRST]) 
      vecteur[2] = ppaevt -> USRVAL;  /*TRST*/
    if(ppaevt -> INDEX == table_index[TMS]) 
      vecteur[1] = ppaevt -> USRVAL;  /*TMS*/
    if(ppaevt -> INDEX == table_index[TDI]) 
      vecteur[0] = ppaevt -> USRVAL;  /*TDI*/
    if(ppaevt -> INDEX == table_index[TDO]) {
      flag_evt_tdo = PRESENT;
      vect_tdo[0] = ppaevt -> USRVAL;
      ptdo = ppaevt ;
    }
  }
  if(flag_evt_tdo == ABSENT) {
    ppapat -> PAEVT = pat_addpaevt(ppapat -> PAEVT, table_index[TDO], vect_tdo[0]);
    ptdo = ppapat -> PAEVT ;
  }
  if(flag_TRST == ABSENT)    vecteur[2]='1';/*TRST*/
  ptdo -> SIMVAL = getsimval(vecteur);   /* envoie du vecteur */
  if((ptdo -> USRVAL != '*') && (ptdo -> USRVAL != ptdo -> SIMVAL)) {
    fprintf(stderr, "Error : expected value and real test mismatch on 'tdo'\n");
    fail_flag = 1;
  }
  return fail_flag;
}
/*****************************************************************************/
/****************************************/
/*   fonction verification des IO       */
/*   presence des io du boundary-scan   */
/****************************************/
static int verif_io(ppaseq) 
  struct paseq *ppaseq;
{
  int flag_TCK, flag_TDI, flag_TDO, flag_TMS;
  int j = 0;
  struct paiol *ppaiol;

  ppaiol = ppaseq -> PAIOL;
  flag_TRST = ABSENT;
  flag_TCK = ABSENT;
  flag_TDI = ABSENT;
  flag_TDO = ABSENT;
  flag_TMS = ABSENT;
  while(ppaiol) { 
    if(!strcmp(ppaiol -> NAME, "tck")) {
      table_index[TCK] = j;     
      flag_TCK = PRESENT;
    }
    else if(!strcmp(ppaiol -> NAME, "trst")) {
      table_index[TRST] = j;    
      flag_TRST = PRESENT;
    }
    else if(!strcmp(ppaiol -> NAME, "tms")) {
      table_index[TMS] = j;     
      flag_TMS = PRESENT;
    }
    else if(!strcmp(ppaiol -> NAME, "tdi")) {
      table_index[TDI] = j;
      flag_TDI = PRESENT;
    }
    else if(!strcmp(ppaiol -> NAME, "tdo")) {
      table_index[TDO] = j;
      flag_TDO = PRESENT;
    }
    else {

	if (!isvdd(ppaiol->NAME) && !isvss(ppaiol->NAME) && verbose_flag)
	fprintf(stderr,"[WARNING] Ignoring value referring to I/O %s (%s.pat)\n",
		ppaiol -> NAME,ppaseq->NAME);
	}
    ppaiol = ppaiol -> NEXT;
    j++;
  }
  if((flag_TCK && flag_TMS && flag_TDI && flag_TDO) == 0)
    return 1 ;
  if(flag_TRST == ABSENT) {       
    fprintf(stderr, "[INFO] There is no TRST Pin \n");
    return 2;
  }
  else  
    return 0;       
}
/*****************************************************************************/
/********************************/
/*  fonction de determination   */
/*  de la longueur du registre  */
/*  instruction du circuit      */
/********************************/
int check_mode() 
{
  int rinst=0;
  int rbs=0;
  int rbypass=0;
  Infos *inf;

  int registre = 0;
  int motif = SCAN_MOTIF;
  int i, lg_inst, lg_bs, lg_bypass;
  int emulbs=1;/*pour l'instant c'est un hardware EMULBS31*/

  /*taille de toutes les chaines bs concatenees*/
  for (rbs=0,inf=InfosVbe; inf;inf=inf->next) rbs+=inf->lg_bs;
  /*taille des registres instructions concatenes*/
  for (rinst=0,inf=InfosVbe; inf;inf=inf->next) rinst+=inf->lg_inst;
  /*longueur du bypass*/
  if (emulbs_flag) rbypass=PCBS_NBR_EMULBS;
  else if (InfosVbe!=NULL) rbypass=BYPASS_SIZE;/*la description BSDL suppose de longueur 1; la longueur du registre bypass pour un seul processeur BS*/
  if (rbs!=NBR_CELLS*PCBS_NBR_EMULBS) emulbs=0;
  if (rinst!=INS_LEN*PCBS_NBR_EMULBS) emulbs=0;


  /* --------------  */
  /*   Check du RI   */
  /* --------------  */

  /* BS : entrees positionnees avant front montant
	  sortie valides apres front descendant */
  open_port();  /*  ouverture du port et reset BS */

  /*  dans l ordre TDI TMS TRST TCK */
  getTDO("0010");   /*  RUN TEST IDLE  */     
  getTDO("0011");   

  getTDO("0110");
  getTDO("0111");   /*  Select DR  */

  getTDO("0110");
  getTDO("0111");   /*  Select IR  */

  getTDO("0010");
  getTDO("0011");   /* Capture  IR */  

  getTDO("0010");
  getTDO("0011");   /* Shift IR    */

  /* send motif (shift IR) */
  /* shift en attendant de retrouver le motif */
  i = 1;
  while (registre != SCAN_MOTIF) {
    if(motif & 0x80000000) {      /* on traite le bit de poids fort de motif */
      getTDO("1010");
      getTDO("1011");
      registre = (registre << 1) | getTDO("1010");
    }
    else {
      getTDO("0010");
      getTDO("0011");
      registre = (registre << 1) | getTDO("0010");
    }
    motif = motif << 1;  /* bit suivant */
    i++;
    if(i == MAX_SHIFT) 
      break; /* si jamais ca deconne! */
  }
  /* prevention des courts circuits */
  reset_bs();

  if (i == MAX_SHIFT) {
    fprintf(stderr, "Cannot check INSTRUCTION Register (longer than %d bits ???)\n",MAX_SHIFT-32);
    fprintf(stderr, "You must check the card connection...\n");
    close_port();
    return(1);
    }

  lg_inst = i - 32;                      /*il faut enlever la taille du motif*/

  if (lg_inst!=INS_LEN*PCBS_NBR_EMULBS) emulbs=0;/*recherche du hardware EMULBS31*/

  if (rinst == 0) {
	fprintf(stdout,"INSTRUCTION Register seems to be %d bits long\n",lg_inst);
	}
  else if (rinst != lg_inst) {
        fprintf(stdout,"Error : INSTRUCTION Register seems to be %d bits long, but file claims it must be %d bits long\n",lg_inst,rinst);
	close_port();
	return (1);
	}

  /* --------------  */
  /* Check du BYPASS */
  /* --------------  */

  registre = 0;
  motif = SCAN_MOTIF;

  /* Chargement de l'instruction BYPASS */
  /* depuis le reset                    */

  /*  dans l ordre TDI TMS TRST TCK */
  getTDO("0010");   /*  RUN TEST IDLE  */     
  getTDO("0011");   

  getTDO("0110");
  getTDO("0111");   /*  Select DR  */

  getTDO("0110");
  getTDO("0111");   /*  Select IR  */

  getTDO("0010");
  getTDO("0011");   /* Capture IR */  

  getTDO("0010");
  getTDO("0011");   /* Shift IR    */

  for(i = 0; i < lg_inst - 1; i++) {  
    /* Instruction BYPASS : des 1 partout */
    getTDO("1010");   /* Shift IR    */
    getTDO("1011");
  }
  /* TMS = 1 */ 
  getTDO("1110");   /* Exit1 IR    */
  getTDO("1111");

  /* TMS = 1 */ 
  getTDO("0110");   /* Update IR    */
  getTDO("0111");

  /* TMS = 0 */ 
  getTDO("0010");
  getTDO("0011");   /*  RUN TEST IDLE  */     

  /* TMS = 1 */ 
  getTDO("0110");
  getTDO("0111");   /*  Select DR  */

  /* TMS = 0 */ 
  getTDO("0010");
  getTDO("0011");   /* Capture DR */  

  /* TMS = 0 */ 
  getTDO("0010");
  getTDO("0011");   /* Shift DR   */

  /* send motif (shift DR) */
  /* shift en attendant de retrouver le motif */
  i = 1;
  while (registre != SCAN_MOTIF) {
    if(motif & 0x80000000) {      /* on traite le bit de poids fort de motif */
      getTDO("1010");
      getTDO("1011");
      registre = (registre << 1) | getTDO("1010");
    }
    else {
      getTDO("0010");
      getTDO("0011");
      registre = (registre << 1) | getTDO("0010");
    }
    motif = motif << 1;  /* bit suivant */
    i++;
    if(i == MAX_SHIFT) 
      break; /* si jamais ca deconne! */
  }
  /* prevention des courts circuits */
  reset_bs();

  if (i == MAX_SHIFT) {
    fprintf(stderr, "Cannot check BYPASS register (longer than %d bits ???)\n",MAX_SHIFT-32);
    fprintf(stderr, "You must check the card connection...\n");
    close_port();
    return(1);
    }

  lg_bypass = i - 32;/*32=longueur du motif*/

  if (lg_bypass!=PCBS_NBR_EMULBS) emulbs=0;/*recherche du hardware EMULBS31*/ 
  

  if (rbypass == 0) {
	fprintf(stdout,"BYPASS register seems to be %d bits long\n",lg_bypass);
	if (lg_bypass > 1) 
		fprintf(stdout,"--> There seem to be %d Boundary-scan chips on this device\n",lg_bypass);
	}
  else if (!emulbs && rbypass != lg_bypass) {
        fprintf(stdout,"Error : BYPASS register seems to be %d bits long, but file claims it must be %d bits long\n",lg_bypass,rbypass);
	close_port();
	return (1);
	}
  

  /* ----------------------  */
  /* Check du BOUNDARY-SCAN  */
  /* ----------------------  */
  registre = 0;
  motif = SCAN_MOTIF;
  /*  dans l ordre TDI TMS TRST TCK */
  getTDO("0010");   /*  RUN TEST IDLE  */     
  getTDO("0011");   

  getTDO("0110");
  getTDO("0111");   /*  Select DR  */

  getTDO("0110");
  getTDO("0111");   /*  Select IR  */

  getTDO("0010");
  getTDO("0011");   /* Capture IR */  

  getTDO("0010");
  getTDO("0011");   /* Shift IR    */

  for(i = 0; i < lg_inst - 1; i++) {  
  /* Instruction EXTEST : que des 0 ------>pour EMULBS31 c'est le code BYPASS*/
    getTDO("0010");   /* Shift IR    */
    getTDO("0011");
  }
  /* TMS = 1 */ 
  getTDO("0110");   /* Exit1 IR    */
  getTDO("0111");

  /* TMS = 1 */ 
  getTDO("0110");   /* Update IR    */
  getTDO("0111");

  /* TMS = 0 */ 
  getTDO("0010");
  getTDO("0011");   /*  RUN TEST IDLE  */     

  /* TMS = 1 */ 
  getTDO("0110");
  getTDO("0111");   /*  Select DR  */

  /* TMS = 0 */ 
  getTDO("0010");
  getTDO("0011");   /* Capture DR */  

  /* TMS = 0 */ 
  getTDO("0010");
  getTDO("0011");   /* Shift DR   */

  /* send motif (shift DR) */
  /* shift en attendant de retrouver le motif */
  i = 1;
  while (registre != SCAN_MOTIF) {
    if(motif & 0x80000000) {      /* on traite le bit de poids fort de motif */
      getTDO("1010");
      getTDO("1011");
      registre = (registre << 1) | getTDO("1010");
    }
    else {
      getTDO("0010");
      getTDO("0011");
      registre = (registre << 1) | getTDO("0010");
    }
    motif = motif << 1;  /* bit suivant */
    i++;
    if(i == MAX_SHIFT) 
      break; /* si jamais ca deconne! */
  }

  /* prevention des courts circuits */
  reset_bs();

if (i == MAX_SHIFT) {
    fprintf(stderr, "Cannot check BOUNDARY-SCAN register (longer than %d bits ???)\n",MAX_SHIFT-32);
    fprintf(stderr, "You must check the card connection...\n");
    close_port();
    return(1);
    }

  lg_bs = i - 32;

  if (lg_bs!=PCBS_NBR_EMULBS) emulbs=0;                    /*comme pour le bypass!!!*/

  if (rbs == 0) {
	fprintf(stdout,"BOUNDARY-SCAN register seems to be %d bits long\n",lg_bs);
	}
  else if (!emulbs && rbs != lg_bs) {             /*emulbs est un cas special*/
        fprintf(stdout,"Error : BOUNDARY-SCAN register seems to be %d bits long, but file claims it must be %d bits long\n",lg_bs,rbs);
        close_port();
	return (1);
	}

  if (emulbs_flag && !emulbs)  {             /*emulbs est un cas special*/
        fprintf(stdout,"Error : BOUNDARY-SCAN register seems to be other than the EMULBS card interface\n");
        close_port();
	return (1);
	}

  if (verbose_flag || check_flag) {
       if (emulbs) fprintf(stdout,"the %d EMULBS31 have been recognized\n",
			   PCBS_NBR_EMULBS);
       else fprintf(stdout,"boundary scan chip recognized : RI=%d, BS=%d\n",
		    lg_inst,lg_bs);
  }

close_port();
return(0);
}
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/
















