/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##  Authors            : Francois Donnet (2001)                     ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/

#include <sys/types.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "global.h"

#define LABEL_BS_SIZE 30     /*taille des labels des patterns BS sauvegardées*/


/* check BS i/o in .pat file */
static int table_index[5];

/*****************************************************************************/

/*****************************************************************************\
        fonction                 
 envoie d'une PAPAT sur le port 
     // du PC LINUX              
\*****************************************************************************/
static char* buf_papat(char* head, struct papat *ppapat) 
{   
  struct paevt *ppaevt;
  static char   tdi  = 0;
  static char   tms  = 0;
  static char   trst = 0;
  static char   tck  = 0;
  
  for(ppaevt = ppapat->PAEVT; ppaevt; ppaevt = ppaevt->NEXT) {
    if(ppaevt->INDEX == table_index[TCK])  tck  = (ppaevt->USRVAL - '0') << TCK_OFFSET;
    if(ppaevt->INDEX == table_index[TRST]) trst = (ppaevt->USRVAL - '0') << TRST_OFFSET;
    if(ppaevt->INDEX == table_index[TMS])  tms  = (ppaevt->USRVAL - '0') << TMS_OFFSET;
    if(ppaevt->INDEX == table_index[TDI])  tdi  = (ppaevt-> USRVAL - '0') << TDI_OFFSET;
  }
  
  *head = tck + trst + tms + tdi;
  head++;
  
  return head;
}

/*****************************************************************************\
        fonction                 
 recoie d'une PAPAT sur le port 
     // du PC LINUX              
\*****************************************************************************/
static char* unbuf_papat(char* head, struct papat *ppapat) 
{   
  struct paevt *ppaevt;
  struct paiol *paiol;
  struct paevt *parallel_evt = NULL;
  char          tdo;
  char          parallel_evt__USRVAL_not;
  int           index = 0;
  static int    parallel_num = 0;
  static int    parallel_line;
  static char   old_tdo     = '9';  /*evenement precedent*/
  static char   usrval      = '9';  /*evenement precedent*/
  static unsigned long number_bs = 0;

  ppapat->SIMFLAG = 'S';
  tdo = ( (*head >> TDO_OFFSET) & 0x1 ) == 1 ? '+' : '-';
  head++;
  
  for(ppaevt = ppapat->PAEVT; ppaevt; ppaevt = ppaevt->NEXT) 
  {
    if ( ppaevt->INDEX == table_index[TDO] ) 
    {
      usrval         = ppaevt->USRVAL;
      break;
    }
  }
  
  /*hack: PAINI not in the right use.  It is an event of // patterns !!! */
  /* it has been produced by traduction.c in Envoi() */
  if ( ppapat->PAINI )
  {
    parallel_evt = (struct paevt*) ppapat->PAINI;
    
    /* si sur bs resultat oppose, alors sur // resultat oppose */
    /* car il se peut que les cellules bs soient inverseuses */
    /* construction de l'oppose*/
    parallel_evt__USRVAL_not = parallel_evt->USRVAL == '+' ? '-' : '+';
    parallel_evt->SIMVAL     = tdo==usrval ? parallel_evt->USRVAL 
                                           : parallel_evt__USRVAL_not;
    
    parallel_line = ppapat->LINE;
    parallel_num  = ppapat->TIME; 
    ppapat->PAINI = NULL;  /*erase hacking*/
    ppapat->LINE  = 0;
    ppapat->TIME  = 0;
  }
 
  /* resultat ne correspond pas aux previsions */
  if ( usrval != '*' && usrval != tdo )
  {
     EXECUTION_ERRORS ++;
     
     if ( parallel_evt ) 
     {
       for ( paiol = PASEQ_PARALLEL->PAIOL; paiol; paiol = paiol->NEXT )
       {
         if ( index == parallel_evt->INDEX ) break;
         index++;
       }
       
       if ( !paiol || ( paiol->MODE!='O' && paiol->MODE!='T' ) )
       {
          fprintf(stderr, "%s : Output with index %d not found\n", __FUNCTION__,index);
          exit(1);
       }
         
       if (verbose_flag) {
         fprintf(stderr, 
	      "Error on pattern %d (line %d) : expecting '%c' actual '%c' on %s\n"
	        ,parallel_num
	        ,parallel_line
	   	  ,parallel_evt->USRVAL
	   	  ,parallel_evt->SIMVAL
           ,paiol->NAME);
       }    
     }
     else
     {
         if ( PASEQ_PARALLEL )
         {
           if (verbose_flag) {
             fprintf(stderr,
			    "Error on pattern %d (line %d)\n"
	  		    ,parallel_num
			    ,parallel_line);
           }
           
           fprintf(stderr,
             "SYNCHRONISATION ERROR on Boundary-scan hardware!!!\n" );
           
           fprintf(stderr,
             "-----> STOP! next results will be wrong...\n");
         }
         
         if (verbose_flag) 
         {
            fprintf(stderr,
		      "Error on Boundary Scan pattern %ld (line %d %s) : Expecting '%c' actual '%c'\n"
		      ,number_bs
		      ,ppapat->LINE
            ,ppapat->LABEL ? ppapat->LABEL : ""
		      ,usrval
		      ,tdo);
         }

         if ( PASEQ_PARALLEL ) exit( 1 );
     }
  }
  
  number_bs++;
  
  /*event and paev not found, must create*/
  if ( !ppaevt )
  {
    if ( tdo == old_tdo ) return head;
    ppapat->PAEVT  = pat_addpaevt(ppapat -> PAEVT, table_index[TDO], usrval );
    ppaevt         = ppapat->PAEVT;
  }

  ppaevt->SIMVAL = tdo;
  old_tdo        = tdo;
  
  return head;
}


/*****************************************************************************\
        fonction                 
 envoie d'une serie dePAPAT sur le port  
     // du PC LINUX              
\*****************************************************************************/
static struct papat *loc_hard_papat(struct papat *head, unsigned int burst_size) 
{
  struct papat *scanpat;
  struct papat *ppapat;
  char         *send;
  char         *current;
  unsigned int  count;
  
  send    = mbkalloc( burst_size * sizeof(char) );
  
  for(ppapat = head; ppapat; ppapat = ppapat->NEXT) 
  {
     current = send;
     count   = 0;
     for( scanpat = ppapat ; scanpat; scanpat = scanpat->NEXT ) 
     {
        current = buf_papat( current, scanpat );
        count++;
        if ( count >= burst_size ) break;
     }
     
     /*send and call in burst*/
     write_read_port( send, count );
     current = send;
     count   = 0;
     
     for( scanpat = ppapat ; scanpat; scanpat = scanpat->NEXT ) 
     {
        current = unbuf_papat( current, scanpat );
        count++;
        if ( count >= burst_size ) break;
     }
 
     if ( !scanpat ) break;
     ppapat = scanpat;
  }/*fin boucle sur les patterns*/
 
  mbkfree( send );

  return head;
}


/*****************************************************************************\
        fonction                 
 envoie d'une serie dePAPAT sur le port  
     // du PC LINUX              
\*****************************************************************************/
extern struct papat *hard_papat(struct papat *head, unsigned int burst_size) 
{
 /*default index*/
 table_index[TRST] = TRST;
 table_index[TMS]  = TMS;
 table_index[TCK]  = TCK;
 table_index[TDI]  = TDI;
 table_index[TDO]  = TDO;

 return loc_hard_papat(head, burst_size);
}


/*****************************************************************************\
        fonction                 
 envoie d'une PASEQ sur le port  
     // du PC LINUX              
\*****************************************************************************/
extern struct paseq* hard_paseq( struct paseq* pat, unsigned int burst_size, unsigned long *time_tck ) 
{
struct timeval  start_tck,end_tck;      /* to compute exec time */
char         *tdi  = namealloc(TDI_NAME);
char         *tms  = namealloc(TMS_NAME);
char         *trst = namealloc(TRST_NAME);
char         *tck  = namealloc(TCK_NAME);
char         *tdo  = namealloc(TDO_NAME);
int           count;
struct paiol *ppaiol;

  /* envoie des patterns sur la carte */
  gettimeofday(&start_tck, NULL);
  
  fprintf(stdout,"\rExecuting %d patterns on %s...                               \r",
                 pat->PATNBR, current_lpscan_device() );
  fflush(stdout);
  
  table_index[TCK]  = -1;
  table_index[TDO]  = -1;
  table_index[TMS]  = -1;
  table_index[TCK]  = -1;
  table_index[TRST] = -1;
  count             = 0;
  
  for( ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol->NEXT )
  {
  
    if( ppaiol->NAME == trst)    table_index[TRST] = count;     
    else if( ppaiol->NAME == tms) table_index[TMS] = count;     
    else if( ppaiol->NAME == tck) table_index[TCK] = count;     
    else if( ppaiol->NAME == tdi) table_index[TDI] = count;     
    else if( ppaiol->NAME == tdo) table_index[TDO] = count;     
    
    count++;
  }
  
  if( table_index[TCK]  == -1
   || table_index[TDO]  == -1
   || table_index[TMS]  == -1
   || table_index[TCK]  == -1
   || table_index[TRST] == -1 )
  {
    fprintf(stderr,
	    "Patterns aren't boundary-scan IO (tdi, tdo, tms, trst, tck)\n");
    exit(1);
  }
  
  /*send to hard*/
  pat->CURPAT = loc_hard_papat( pat->CURPAT, burst_size );
  
  gettimeofday(&end_tck, NULL);
  
  *time_tck += 
	 (end_tck.tv_sec-start_tck.tv_sec)*1000+(end_tck.tv_usec-start_tck.tv_usec)/1000;

  return pat;  
}


/*****************************************************************************\
        fonction                 
   save some patterns
\*****************************************************************************/
extern void save_patterns(char *file, struct paseq* pat, unsigned long *time_io) 
{
struct timeval start_io,end_io;      /* to compute exec time */
const char* entete  = PROGRAM_NAME " version " EMULBS_VERSION " for boundary scan";

  gettimeofday(&start_io, NULL);
  
  fprintf(stdout,"\rSaving %d patterns in %s.pat...                               ",
                  pat->PATNBR, file);
  fflush(stdout);
  
  /*si pas encore commence a sauver*/
  if ( !pat->DRVSEQ )
  {
    pat_frepacom(pat->CURCOM);                /*commentaire debut fichier*/
    pat->CURCOM = pat_addpacom(NULL,entete,0); 
  }  

  if ( pat_savpaseq(file, pat, LABEL_BS_SIZE) ) 
  {
     fprintf(stderr,"\nSaving failed!\n");
     exit(1);
  }
  
  if ( pat->CURCOM )
  {
    pat_frepacom(pat->CURCOM);                /*commentaire debut fichier*/
    pat->CURCOM = NULL; 
  }  
  
  gettimeofday(&end_io, NULL);
  
  *time_io += (end_io.tv_sec-start_io.tv_sec)*1000 + (end_io.tv_usec-start_io.tv_usec)/1000;
}


/*****************************************************************************\
        fonction                 
   load some patterns
\*****************************************************************************/
extern struct paseq* load_patterns(char *file, struct paseq* pat, unsigned int nb_pat, unsigned long *time_io) 
{
struct timeval start_io,end_io;      /* to compute exec time */

  gettimeofday(&start_io, NULL);
  
  if (nb_pat <= 0 )
     fprintf(stdout,"\rLoading all patterns from %s.pat...                            ",
                  file);
  else                
     fprintf(stdout,"\rLoading %d patterns from %s.pat...                            ",
                  nb_pat, file);
  fflush(stdout);
  
 
  pat = pat_lodpaseq (file, pat, nb_pat, 0);
  
  if ( pat==NULL || pat->ERRFLG ) 
  {
     fprintf(stderr,"Loading failed!\n");
     exit( 1 );
  }
  
  gettimeofday(&end_io, NULL);
  
  *time_io += (end_io.tv_sec-start_io.tv_sec)*1000 + (end_io.tv_usec-start_io.tv_usec)/1000;
  
  return pat;  
}


/*****************************************************************************\
        fonction                 
 envoie d'une PASEQ sur le port  
     // du PC LINUX              
\*****************************************************************************/
extern void hard_paseq_file(char *scefile, char *resfile, unsigned int nb_pat, unsigned int burst_size) 
{
struct timeval start_prg,end_prg;      /* to compute exec time */
unsigned long time_tck=0,time_io=0,time_prg=0;       /* to compute exec time */
struct paseq *pat = NULL;                                   /* for .pat file */
int pat_tot = 0;

  if ( !scefile ) return;
  if ( !execute_flag ) return;
  
  gettimeofday(&start_prg, NULL);
  
  open_port(burst_size);  /*  ouverture du port et reset BS */
  
  do 
  {
    pat = load_patterns(scefile, pat, nb_pat, &time_io);
    pat = hard_paseq( pat, burst_size, &time_tck );
    if ( resfile ) save_patterns(resfile, pat, &time_io);
    pat_tot += pat->PATNBR;
  }
  while ( pat->ENDFLG != 'Y' ); 
    
  close_port();  /*  reset BS et fermeture du port */


  /*temps*/
  gettimeofday(&end_prg, NULL);
  time_prg=
       (end_prg.tv_sec-start_prg.tv_sec)*1000 + (end_prg.tv_usec-start_prg.tv_usec)/1000;
  
  statistic(time_prg, time_tck, time_io, pat_tot );
}
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/
















