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
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
/****************************************************************************/
#include "global.h"
/****************************************************************************/
/* Variables globales                                                       */
/****************************************************************************/
Infos *InfosVbe=NULL;                              /* Pour les informations */

/* flags and vars for command line processing */
int verbose_flag     = 0;              /*affichage de tous les commmentaires */
int execute_flag     = 0;                               /*mode avec hardware */
int savePatSRC_flag  = 0;                            /*sauvegardr pattern // */
int savePatBS_flag   = 0;                           /*sauvegarder pattern BS */
int check_flag       = 0;                             /*est-ce le mode check */
int emulbs_flag      = 0;                /*est-ce pour l'interface EMULBS31? */
static int bsdl_flag = 0;                                 /*bsdl description */
static int patlimit_flag = 0;            /*nbr de patterns dans une sequence */
static char *sce=NULL,*destSRC=NULL,*destBS=NULL,*bsdl=NULL,*emu=NULL,*pga=NULL;
static unsigned int pat_limit=PAT_LIMIT;

/****************************************************************************/
/****************************************************************************/
/* Aide sur la ligne de commande */
static void usage() 
{
fprintf(stdout,"\
%s version %s :\n\
   %s   --help\n\
   %s   [ -hvcxeb ]  [ -p [all|<number>] ]  [ <description files> ]\n\
           [ <source.PAT> ]  [ [-fs] <destination.PAT> ]\n\
",PROGRAM_NAME,PCBS_VERSION,PROGRAM_NAME,PROGRAM_NAME);
}



/****************************************************************************/
static void help() 
{
fprintf(stdout,"\
%s version %s :\n\
   %s   [ -hvcxeb ]  [ -p [all|<number>] ]  [ <description files> ]\n\
           [ <source.PAT> ]  [ [-fs] <destination.PAT> ]\n\
  -h,--help\tDisplay this current message.\n\
  -v,--verbose\tDisplay all warnings and actions.\n\
  -p,--pattern\tMust be followed by the <number> (or \"all\") of patterns %s\n\
\t\twill treat in one sequence. If you have memory problems put the\n\
\t\tpatterns number lower than the default value %d. You can also\n\
\t\traise it up.\n\
  -s,--serial\tKind of patterns to be saved. Take the boundary scan serial\n\
\t\tform. Beware destination file size is many times source size!\n\
  -f,--functionnal\tKind of patterns to be saved. Keep the functionnal form of the\n\
\t\tpattern source file.\n\
  -c,--check\tCheck the boundary scan hardware. It can be followed by a\n\
\t\tdescription to compare the file with the hardware.\n\
  -e,--emulbs,-b,--bsdl\n\
\t\tTraduce functionnal patterns into boundary scan patterns. These\n\
\t\toptions need both functionnal pattern file for <source> and\n\
\t\toptionnaly a <destination> file to keep results. The difference\n\
\t\tis in <description files>. If there is an EMULBS31 interface\n\
\t\tcard on the DUT, you must use --emulbs option. If you don't have\n\
\t\tsuch card, put --bsdl option.\n\
\t\t\t--emulbs <emulbs_connexions> <pga_connexions>\n\
\t\t\t--bsdl   <description.BSDL>\n\
\t\tsee manual page for more information.\n\
  -x,--execute\tSend boundary scan patterns to the DUT hardware. It can be\n\
\t\tcombinated with -e or -b options.
",PROGRAM_NAME,PCBS_VERSION,PROGRAM_NAME,PROGRAM_NAME,PAT_LIMIT);
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
      if ( !strcmp(argv[i],"-h") || !strcmp(argv[i],"--usage") ) 
      {
         help(); 
         exit(0);
      }
      if ( !strcmp(argv[i],"-v") || !strcmp(argv[i],"--verbose") ) 
      {
         verbose_flag = 1;
         continue;
      }
      if ( !strcmp(argv[i],"-c") || !strcmp(argv[i],"--check") ) 
      {
         check_flag = 1;
         continue;
      }
      if ( !strcmp(argv[i],"-x") || !strcmp(argv[i],"--execute") ) 
      {
	      execute_flag=1;
         continue;
      }
      if ( !strcmp(argv[i],"-b") || !strcmp(argv[i],"--bsdl") ) 
      {
	      bsdl_flag=1;
         if (++i>=argc) {
           usage();
           exit(1);
         }
         bsdl = argv[i];
         continue;
      }
      if ( !strcmp(argv[i],"-e") || !strcmp(argv[i],"--emulbs") ) 
      {
	      emulbs_flag=1;
         if (++i>=argc) {
           usage();
           exit(1);
         }
         emu = argv[i];
         if (++i>=argc) {
           usage();
           exit(1);
         }
         pga = argv[i];
         continue;
      }
      if ( !strcmp(argv[i],"-f") || !strcmp(argv[i],"--functionnal") ) 
      {
	      savePatSRC_flag=1;
         if (++i>=argc) {
           usage();
           exit(1);
         }
         destSRC = argv[i];
         continue;
      }
      if ( !strcmp(argv[i],"-s") || !strcmp(argv[i],"--serial") ) 
      {
	      savePatBS_flag=1;
         if (++i>=argc) {
           usage();
           exit(1);
         }
         destBS = argv[i];
         continue;
      }
      if ( !strcmp(argv[i],"-p") || !strcmp(argv[i],"--pattern") ) 
      {
	      patlimit_flag=1;
         if (++i>=argc) {
           usage();
           exit(1);
         }
         if ( !strcmp("all",argv[i]) )  pat_limit=0;
	      else pat_limit=atoi(argv[i]);
         continue;
      }
      if ( argv[i][0]=='-' ) /*invalid option*/
      {
         fprintf(stderr, "Invalid option '%s'\n", argv[i]);
         usage();
         exit(1);
      }
      /*sources*/
      if (!sce) 
      {
         sce=argv[i];
         continue;
      }
      /*destination optionnelle si...*/
      if (( execute_flag || emulbs_flag||bsdl_flag )&& !savePatSRC_flag) 
      {
         savePatSRC_flag=1;
         destSRC=argv[i];
         continue;
      }
      if (!savePatBS_flag) 
      {
         savePatBS_flag=1;
         destBS=argv[i];
         continue;
      }
      usage();
      exit(1);
   }  
   


/****COHERENCE****/
  if ( destSRC && destBS && !strcmp(destSRC,destBS)) {
	    fprintf(stdout,"same name %s for two destination files!\n",destBS);
	    usage();pcbs_exit(0);}	      
/*impossible produire patterns paralleles corriges sans hardware*/
  if (savePatSRC_flag && !execute_flag) {
	       fprintf(stderr,"'-f' option needs '-x'!\n");
	       usage();pcbs_exit(0);}
/*impossible produire les patterns paralleles sans ceux d'origine*/
  if (savePatSRC_flag && !emulbs_flag && !bsdl_flag) {
	       fprintf(stderr,"'-f' option needs '-e' or '-b'!\n");
	       usage();pcbs_exit(0);}
/*emulation*/
  if (emulbs_flag) {
	    if (bsdl_flag)  {
	       fprintf(stderr,"'-e' option doesn't match with '-b'!\n");
	       usage();pcbs_exit(0);}
       if (!check_flag&&!execute_flag && !savePatSRC_flag && !savePatBS_flag) {
	    fprintf(stderr,"'-e' option needs a destination!\n");
	    usage();pcbs_exit(0);}
  }
/*bsdl*/
  else if (bsdl_flag) {
       if (!execute_flag && !savePatSRC_flag && !savePatBS_flag) {
	    fprintf(stderr,"'-b' option needs a destination!\n");
	    usage();pcbs_exit(0);}
  }
/*execution*/
  else if (execute_flag) {
       if (!execute_flag && !savePatSRC_flag && !savePatSRC_flag) {
	    fprintf(stderr,"'-b' option needs a destination!\n");
	    usage();pcbs_exit(0);}
  }
/*check*/
  else if (check_flag) {
  }
/*aucune option*/
  else {usage();pcbs_exit(0);}

}




/****************************************************************************/
int    main(int argc, char *argv[]) 
{


/****initialisation environnement****/
  mbkenv();
  initializeBdd(SMALL_BDD);                                     /*SMALL_BDD=0*/
  error_signal_handler();                                     /*interruptions*/
  alliancebanner_with_authors("PcBs", PCBS_VERSION " [2002/02/18]", 
                              "PC/Boundary-Scan Tester Platform", 
                "2000", ALLIANCE_VERSION,
                "Ana ABRIL, François DONNET, Eric MECHIN, Philippe OLEK");


  /*take parameters*/
  set_param( argc, argv);

  
/*meme source et destination*/
  if (destSRC!=NULL && destBS!=NULL && !strcmp(destSRC,destBS)) {
       fprintf(stderr,"%s represents two destination files!\n",destSRC);
       usage();pcbs_exit(0);}
  if (destSRC!=NULL && sce!=NULL && !strcmp(destSRC,sce)) {
       fprintf(stderr,"%s represents a source and a destination file!\n",sce);
       usage();pcbs_exit(0);}
  if (sce!=NULL && destBS!=NULL && !strcmp(sce,destBS)) {
       fprintf(stderr,"%s represents a source and a destination file!\n",sce);
       usage();pcbs_exit(0);}


/****traitement****/
  /* Chargement de la  description du boundary scan */
  if (bsdl_flag && ReadBSDL(bsdl)) pcbs_exit(1);
  if (emulbs_flag && description_cartes(emu,pga)<0) pcbs_exit(1);
  if ((emulbs_flag || bsdl_flag) && VerificationsInfos()) pcbs_exit(1);
  
  /*verification hardware*/
  if (check_flag || execute_flag) {
       fprintf(stdout,"Checking DUT connected to %s\n",
	       (char*) current_lpscan_device());
       if (check_mode()) pcbs_exit(1);
  }

  /*sans traduction*/
  if (execute_flag && !emulbs_flag && !bsdl_flag) 
       {if (send_pat_file(sce,destBS,pat_limit)) pcbs_exit(1);pcbs_exit(0);}

  /*avec traduction*/
  if (execute_flag) open_port();
  if ((emulbs_flag || bsdl_flag) && traduire(sce,destSRC,destBS,pat_limit))
       pcbs_exit(1);                /*traduction*/
  if (execute_flag) close_port();
  
  pcbs_exit(0);


return 1;
}


/*****************************************************************************/
/******************************** END OF FILE ********************************/
/*****************************************************************************/





