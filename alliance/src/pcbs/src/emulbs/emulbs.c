/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##                                                                  ##*/
/*##  Modifications      : Ana ABRIL (2000)                           ##*/
/*##                       Francois Donnet (2001)                     ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
/****************************************************************************/
#include "global.h"


/************************* version de PcBs ***********************************/

/****************************************************************************/
/* Variables globales                                                       */
/****************************************************************************/

/* flags and vars for command line processing */
int verbose_flag     = 0;              /*affichage de tous les commmentaires */
int execute_flag     = 0;                               /*mode avec hardware */
int check_flag       = 0;                             /*est-ce le mode check */
int lpscan_flag;              /*type of hardware device*/
int pci_flag;                 /*type of hardware device*/
static unsigned int patlimit_flag   = PAT_SIZE;      /*nbr de patterns dans une sequence */
static unsigned int burstlimit_flag = BURST_SIZE;    /* taille du burst sur le port */
static char* emulbs_flag      = NULL;                /*est-ce pour l'interface EMULBS31? */
static char* loadPatSRC_flag  = NULL;            /*charger pattern  */
static char* savePatSRC_flag  = NULL;            /*sauvegarder pattern */
static char* savePatBS_flag   = NULL;                /*sauvegarder pattern BS */


/****************************************************************************/
/* Aide sur la ligne de commande */
/****************************************************************************/
static void usage() 
{
fprintf(stdout,"\
%s [ -hv ]  [ --pci | --parallel ]  [ -l <number> ]  [ -b <number> ] \
     [ <source.PAT> [ <res.PAT> ] ]  [ -e <connections_file> [ -s <bs.PAT> ] ]",
     PROGRAM_NAME);
}


/****************************************************************************/
/****************************************************************************/
static void help() 
{
fprintf(stdout,"\
%s [ -hv ]  [ --pci | --parallel ]  [ -l <number> ]  [ -b <number> ] \n\
   [ <source.PAT> [ <res.PAT> ] ]  [ -e <connections_file> [ -s <bs.PAT> ] ]\n\
  -h,--help    Display this current message.\n\
  -v,--verbose Display all warnings and actions.\n\
  -l,--load    Load only <number> patterns from <source.PAT> to save memory.\n\
  -b,--burst   Execute a burst of <number> boundary-scan patterns on device.\n\
  <source.PAT> Functional or boundary-scan patterns to execute.\n\
  <res.PAT>    Functional or boundary-scan executed patterns.\n\
  -e,--emulbs  Load <connections_file> which describes the device connections\n\
               between the chip and the test card to mute functional patterns\n\
               <source.PAT> into boundary-scan patterns <bs.PAT>.\n\
  -s,--save    Save boundary-scan patterns in <bs.PAT>.\n\
  --pci        Check device, execute <source.PAT> on device by pci port\n\
               and save result in <res.PAT>.\n\
  --parallel   Check device, execute <source.PAT> on device by parallel port\n\
               and save result in <res.PAT>.\n",
PROGRAM_NAME);
}



/******************************************************************************/
/*              set variables to values of command line                       */
/******************************************************************************/
static void set_param(int argc, char* argv[])
{
   int i;

   if ( argc <= 1 )
   {
     usage();
     exit(0);
   }
   
   /*verify for Sun and Solaris.  getopt() isn't the same than Linux_elf  */
   for ( i=1; i<argc; i++ )
   {
      if ( !strcmp(argv[i],"-h") || !strcmp(argv[i],"--help") ) 
      {
         help(); 
         exit(0);
      }
      if ( !strcmp(argv[i],"-v") || !strcmp(argv[i],"--verbose") ) 
      {
         verbose_flag = 1;
         continue;
      }
      if ( !strcmp(argv[i],"-b") || !strcmp(argv[i],"--burst") ) 
      {
         if (++i>=argc) {
           help();
           exit(1);
         }
         burstlimit_flag = atoi( argv[i] );
         continue;
      }
      if ( !strcmp(argv[i],"-l") || !strcmp(argv[i],"--load") ) 
      {
         if (++i>=argc) {
           help();
           exit(1);
         }
         patlimit_flag = atoi( argv[i] );
         continue;
      }
      if ( !strcmp(argv[i],"-e") || !strcmp(argv[i],"--emulbs") ) 
      {
         if (++i>=argc) {
           help();
           exit(1);
         }
         emulbs_flag = argv[i];
         continue;
      }
      if ( !strcmp(argv[i],"-s") || !strcmp(argv[i],"--save") ) 
      {
         if (++i>=argc) {
           help();
           exit(1);
         }
         savePatBS_flag = argv[i];
         continue;
      }
      if ( !strcmp(argv[i],"--pci") ) 
      {
         pci_flag = 1;
         continue;
      }
      if ( !strcmp(argv[i],"--parallel") ) 
      {
         lpscan_flag = 1;
         continue;
      }
      if ( argv[i][0]=='-' ) /*invalid option*/
      {
         fprintf(stderr, "Invalid option '%s'\n", argv[i]);
         help();
         exit(1);
      }
      if ( !loadPatSRC_flag )
      {
         loadPatSRC_flag = argv[i];
         continue;
      }
      if ( !savePatSRC_flag )
      {
         savePatSRC_flag = argv[i];
         continue;
      }
      help();
      exit(1);
   }  
   
   /*if there is source and hardware then execute*/
   if ( ( pci_flag || lpscan_flag ) && loadPatSRC_flag ) execute_flag = 1;
   
   /*coherence in options*/
   if ( ( savePatBS_flag && !emulbs_flag )
   || ( lpscan_flag && pci_flag ) 
   || ( !loadPatSRC_flag && !pci_flag && !lpscan_flag ) ) 
   {
      help();
      exit(1);
   }
}



/****************************************************************************/
/****************************************************************************/
extern int main(int argc, char *argv[]) 
{

/****initialisation environnement****/
  mbkenv();
  initializeBdd(SMALL_BDD);                                     /*SMALL_BDD=0*/
  alliancebanner_with_authors("EmulBs", EMULBS_VERSION "[2003/02/20]", 
                              "Emulated Boundary-Scan Tester Platform", 
                "2001", ALLIANCE_VERSION,
                "Ana ABRIL, François DONNET, Eric MECHIN, Philippe OLEK");

  fprintf( stdout, "\n\n");
  fprintf( stdout, "               Compiled for this required hardware...\n" );
  fprintf( stdout, "                      ===> Number of hardware emulators   : %d\n", EMULBS_NBR_EMULBS ); 
  fprintf( stdout, "                      ===> Depth of one hardware emulator : %d\n", EMULBS_BS_SIZE ); 
  fprintf( stdout, "\n\n");

  set_param(argc,argv);

  /*variable globale InfosVbe */
  creer_InfosVbe_EMULBS();
  
/****traitement****/
  /* Chargement de la  description du boundary scan */
  if (emulbs_flag)
  {
    description_cartes(emulbs_flag);
    VerificationsInfos();
  } 
  
  /*verification hardware*/
  if (pci_flag || lpscan_flag) {
       fprintf(stdout,"Checking DUT connected to %s\n",
	       (char*) current_lpscan_device());
       if (check_mode()) exit(1);
  }

  if ( emulbs_flag && loadPatSRC_flag ) /*traduction en bs puis peut etre envoi*/
  {
     traduire( loadPatSRC_flag, savePatSRC_flag, savePatBS_flag,
              patlimit_flag, burstlimit_flag );
  }
  else if ( loadPatSRC_flag )/*envoi sur hard sans traduction*/
  {
     hard_paseq_file( loadPatSRC_flag, savePatSRC_flag,
              patlimit_flag, burstlimit_flag );
  }
  
  exit(0);
}


/*****************************************************************************/
/******************************** END OF FILE ********************************/
/*****************************************************************************/





