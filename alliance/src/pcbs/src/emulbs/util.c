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
/****************************************************************************/
#include "global.h"

/******************************Global******************************************/

int EXECUTION_ERRORS = 0;

/*****************************************************************************/
   static char* p_mem = NULL;     /*pointeur courant dans le block memeoire*/
   static char* end_mem = NULL;   /*end of block*/
   static long  tampon_mem = 1024;   /* taille de depart des blocks = 1 Ko */
   static chain_list* stack = NULL;   /*liste des blocks alloues*/



/*****************************************************************************/
/*remplace tous les espaces d'un mot par des '_'  ------>     ne peut pas etre desallouer!! */
char *del_espace(char *s)
{
char *res,*mbk_res;
int i=0;

/*tests chaines vides*/
if (s==NULL || *s=='\0') return namealloc(s);

	res=(char*) mbkalloc(strlen(s)+1);
	for (i=0;i<=strlen(s);i++)
	     if (s[i]==' ') res[i]='_';
	     else res[i]=s[i];

mbk_res=namealloc(res);
mbkfree(res);
return mbk_res;
}


/*****************************************************************************/
/* compte le nombre de pattern d'un fichier PAT  et retourne...
  ---> nombre de patterns*/
extern int Compte_file_pat( char *pat)
{
    int i=0;
    char c;
    char *patname;
    FILE *pat_in;
    int not_com=2;                                             /*il faut 2 tirets*/

     patname = concatname(pat,".pat");
     pat_in  = fopen(patname,"r");
     
     if ( !pat_in || ferror(pat_in) ) {
	    fprintf(stderr,"Unable to load %s\n",patname);
	    exit(1);
     }
/*compter le nbr de patterns c'est compter le nombre de ':' */
     while ( (c=fgetc(pat_in))!=EOF ) {
   	  if (c=='-') not_com--;
	     else if (not_com==1) not_com=2;
	     if (c=='\n') not_com=2;
	     if (c==':' && not_com) i++;
     }

     fclose(pat_in);
     
     return i;
}

/*****************************************************************************/
/*recoit en parametre un temps en millseconds et l'affiche avec un format    */
static void affiche_time(unsigned long ms)
{
  if (ms<=0)                                   /*pas de temps suffisant*/
       fprintf(stdout,"0 ms");
  else if (ms<=1)                                   /*ordre de la millisecondes*/
       fprintf(stdout,"one millisecond");
  else if (ms<1000)                            /*ordre de la millisecondes*/
       fprintf(stdout,"%ld milliseconds",ms);
  else if (ms<2000)                                  /*ordre de la seconde*/
       fprintf(stdout,"1.%03ld second",ms%1000);
  else if (ms<60000)                                 /*ordre de + secondes*/
       fprintf(stdout,"%ld.%03ld seconds",ms/1000,ms%1000);
  else if (ms<120000)                                 /*ordre de la minute*/
       fprintf(stdout,"1 minute %02ld",(ms/1000)%60);
  else if (ms<3600000)                                /*ordre de + minutes*/
       fprintf(stdout,"%ld minutes %02ld",ms/60000,(ms/1000)%60);
  else if (ms<7200000)                                  /*ordre de l'heure*/
       fprintf(stdout,"1 hour %02ld",(ms/60000)%60);
  else                                                    /*ordre de + heures*/
       fprintf(stdout,"%ld hours %02ld",ms/3600000,(ms/60000)%60);
fflush(stdout);
}

/*****************************************************************************\
affichage des temps d'acces, de io etc...
\*****************************************************************************/
extern void statistic( unsigned long time_prg, unsigned long time_tck, unsigned long time_io, unsigned long nbr_pat ) 
{
  /*verification*/
  fprintf(stdout, "\n\n");
  if (EXECUTION_ERRORS == 0) 
     fprintf(stdout," ----------> No error\n");
  else if (EXECUTION_ERRORS == 1) 
     fprintf(stdout," ----------> One ERROR\n");
  else fprintf(stdout," ----------> %d ERRORS\n", EXECUTION_ERRORS);

  fprintf(stdout,"Global: ");
  affiche_time(time_prg);
  fprintf(stdout,"\nHardware: ");
  affiche_time(time_tck);
  fprintf(stdout,"  (%.1f%%)\n",time_prg?(float)((float)time_tck*100)/time_prg:0);
  fprintf(stdout,"I/O: ");
  affiche_time(time_io);
  fprintf(stdout,"  (%.1f%%)\n",time_prg?(float)((float)time_io*100)/time_prg:0);
  fprintf(stdout,"Patterns BS: %ld", nbr_pat);

  if (time_tck==0) {
    fprintf(stdout,"\n");
    return;
  }
  
  /*frequence --> seuls les fronts d'horloge*/
  if (time_tck!=0) time_tck=nbr_pat/(time_tck*2);
  
  if (time_tck<1000)
    fprintf(stdout,"  (%ld Hz)\n",time_tck);
  else if (time_tck<1000000)
    fprintf(stdout,"  (%ld.%03ld KHz)\n",time_tck/1000,time_tck%1000);
  else 
    fprintf(stdout,"  (%ld.%03ld MHz)\n",
	    time_tck/1000000,(time_tck/1000)%1000000);
}

/****************************************************************************/
/* Visualisation de la structure de donnees */
extern int AffichePaseq( struct paseq *pat)
{
  struct paiol *ppaiol;
  struct papat *ppapat;
  struct paevt *ppaevt;
  int i = 1;

  fprintf(stdout,"Paseq -> Paiol :\n");
  for(ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT) {
    fprintf(stdout,"\tPaiol -> name : %s\n", (char*)ppaiol -> NAME);
    fprintf(stdout,"\tPaiol -> formmat : %c\n", ppaiol -> FORMAT);
    fprintf(stdout,"\tPaiol -> mode : %c\n\n", ppaiol -> MODE);
  }
  fprintf(stdout,"Paseq -> iolnbr : %d\n", pat -> IOLNBR);
  fprintf(stdout,"Appuyer sur une touche...\n");
  getchar();
  fprintf(stdout,"Paseq -> Papat :\n");
  for(ppapat = pat -> CURPAT; ppapat; ppapat = ppapat -> NEXT) {
    fprintf(stdout,"\tNumero : %d\n", i++);
    fprintf(stdout,"\tPapat -> label : %s\n", (char*)ppapat -> LABEL);
    fprintf(stdout,"\tPapat -> simflag : %c\n", ppapat -> SIMFLAG);
    fprintf(stdout,"\tPapat -> Paevt\n");
    for(ppaevt = ppapat -> PAEVT; ppaevt; ppaevt = ppaevt -> NEXT) {
      fprintf(stdout,"\t\tPaevt -> index  : %d\n", ppaevt -> INDEX);
      fprintf(stdout,"\t\tPaevt -> usrval : %c\n", ppaevt -> USRVAL);
      fprintf(stdout,"\t\tPaevt -> simval : %c\n\n", ppaevt -> SIMVAL);
    }
    fprintf(stdout,"Appuyer sur une touche...\n");
    getchar();
    fprintf(stdout,"\n");
  }
  fprintf(stdout,"Paseq -> errflg : %d\n", pat -> ERRFLG);
  return(1);
}


/****************************************************************************/
/****************************************************************************/
extern char *block_malloc(char *s)
{
   char*        alloc_mem;
   long         taille;

   if (s==NULL) return NULL;

   taille    = strlen(s) + 1;
   if ( taille > tampon_mem ) tampon_mem = taille;
   
   if ( p_mem+taille >= end_mem )
   {
      p_mem   = (char*) mbkalloc( tampon_mem * sizeof(char));  
      end_mem = p_mem + (tampon_mem * sizeof(char));
      stack   = addchain( stack, p_mem );
   }
   
   alloc_mem = p_mem;
   p_mem    += taille;

   strcpy( alloc_mem, s);
   return alloc_mem;   
}


/****************************************************************************/
extern void block_free()
{
  chain_list *chain;

  p_mem   = NULL;
  end_mem = NULL;

  for ( chain = stack; chain; chain=chain->NEXT )  mbkfree( (char*) chain->DATA );
  freechain(stack);
  stack   = NULL;
}

/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/

