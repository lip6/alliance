/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##  								      ##*/
/*##  Modifications	 : Ana Abril (2000)			      ##*/
/*##								      ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
/****************************************************************************/
#include "global.h"

/**********************************Global*****************************************/

struct paseq* PASEQ_PARALLEL = NULL;       /*sequence de patterns en construction*/


/****************************************************************************/

static Data *data = NULL;                   /*sert a la construction de pattern*/
static int CURRENT_BOUNDARY_SCAN_CELL = -1;
static int taille = 0;                      /*taille de la chaine boundary scan*/
static long Nb_bs_pat = 0;
static int PATTERN_NUMBER_PARALLEL = 0;      /*numero de pattern  */
static int PATTERN_LINE_PARALLEL = 0;      /*numero de ligne dans le*/
static struct papat* Current_Parallel_pat = NULL;  

/****************************************************************************/
/* Envoi un vecteur Boundary Scan (TDI, TDO, TMS, TRST, TCK) sur le circuit
   ou dans une structure de donnees */
/****************************************************************************/
extern struct papat *Envoi(struct papat *ppapat, char valeur[5], char *label)
{
  static char ancienne[5]={'9','9','9','9','9'};  /*evenements paevt precedents*/
  int i = 0;

  /*initialisation en cours de programme*/
  if ( !valeur )
  {
    for(i = 0; i < 5; i++)   ancienne[i]='9';
    return NULL;
  }

  /***construire pattern pour fichier serie resultat***/
  ppapat = pat_addpapat(ppapat, label ? label : namealloc("") , NULL);
  Nb_bs_pat++;

  for(i = 0; i < 5; i++) 
  {
     if ( valeur[i]!=ancienne[i] )
     {  /*changing events  */
        ppapat->PAEVT = pat_addpaevt(ppapat->PAEVT, i, valeur[i] );
     }

     /*check answer for output TDO*/
     switch ( valeur[i] )
     {
     case '+': case '-': case '*':
       /*hacking corrected in unbuf_papat() */
       if ( execute_flag && CURRENT_BOUNDARY_SCAN_CELL>=0
         && data[CURRENT_BOUNDARY_SCAN_CELL].CELL 
         && data[CURRENT_BOUNDARY_SCAN_CELL].OLD_PAEV )
       {
         switch ( data[CURRENT_BOUNDARY_SCAN_CELL].NEW_VAL )
         {
         case '+': case '-': case '*':
           ppapat->PAINI = (struct paini*) data[CURRENT_BOUNDARY_SCAN_CELL].OLD_PAEV;
           ppapat->LINE  = PATTERN_LINE_PARALLEL ;
           ppapat->TIME  = PATTERN_NUMBER_PARALLEL - 1;
         }
       }  
     }
     
     /***mise a jour des valeurs evenements pour la structure paseq***/
     ancienne[i] = valeur[i];
  } 
  
  
  return ppapat;
}

/****************************************************************************\
 front montant puis descendant label sur 1ere pattern
\****************************************************************************/
extern struct papat *TAP_in(struct papat *ppapat, char valeur[5], char *label)
{
  char save;
  
  save        = valeur[TDO];
  valeur[TDO] = '*';
  valeur[TCK] = '0';
  ppapat = Envoi(ppapat, valeur, label);
  valeur[TDO] = save;		/*controle toujours sur front montant*/
  valeur[TCK] = '1';
  ppapat = Envoi(ppapat, valeur, namealloc(""));

  return ppapat;
}

/****************************************************************************\
 front montant puis descendant label sur 2eme pattern
\****************************************************************************/
static struct papat *TAP_out(struct papat *ppapat, char valeur[5], char *label)
{
  char save;

  save        = valeur[TDO];
  valeur[TDO] = '*';
  valeur[TCK] = '0';
  ppapat = Envoi(ppapat, valeur, namealloc(""));
  valeur[TDO]=save;/*controle toujours sur front montant*/
  valeur[TCK] = '1';
  ppapat = Envoi(ppapat, valeur, label);
  
  return ppapat;
}

/****************************************************************************/
/****************************************************************************/
static struct papat *TAP2(struct papat *ppapat, char valeur[5], char *label_in, char *label_etat)
{
  char save;
  
  save        = valeur[TDO];
  valeur[TDO] = '*';
  valeur[TCK] = '0';
  ppapat = Envoi(ppapat, valeur, label_in);
  valeur[TDO]=save;/*controle toujours sur front montant*/
  valeur[TCK] = '1';
  ppapat = Envoi(ppapat, valeur, label_etat);
  
  return ppapat;
}

/****************************************************************************/
/* Envoi d'une sequence de remise a zero de l'automate a etats finis */
/****************************************************************************/
static struct papat *ResetBS(struct papat *ppapat)
{
  char valeur[5];
  int i;

  valeur[TRST] = '0';/*mode reset, car reset=not(TRST) */
  valeur[TDO]  = '*';
  valeur[TDI]  = '0';
  valeur[TCK]  = '0';
  valeur[TMS]  = '1';

  ppapat = Envoi(ppapat, valeur, namealloc("Reset_BS"));

  valeur[TCK] = '1';
  valeur[TRST]='1';/*mode normal, car reset=not(TRST) */
  ppapat = Envoi(ppapat, valeur, namealloc(""));


  for(i = 1; i < 5; i++) ppapat=TAP_in(ppapat,valeur, namealloc("Reset_BS"));
  /*etat= Reset*/

  return(ppapat);
}

/****************************************************************************/
/* Envoi d'une sequence de test de l'automate a etats finis */
/****************************************************************************/
static struct papat *TestTap(struct papat *ppapat)
{
  int i = 0, j = 0;
  char valeur[5];
  char* etapes[29];

  etapes[0]  = namealloc("Test_Tap_c");
  etapes[1]  = namealloc("Test_Tap_7");
  etapes[2]  = namealloc("Test_Tap_6");
  etapes[3]  = namealloc("Test_Tap_1");
  etapes[4]  = namealloc("Test_Tap_3");
  etapes[5]  = namealloc("Test_Tap_0");
  etapes[6]  = namealloc("Test_Tap_2");
  etapes[7]  = namealloc("Test_Tap_1");
  etapes[8]  = namealloc("Test_Tap_3");
  etapes[9]  = namealloc("Test_Tap_3");
  etapes[10] = namealloc("Test_Tap_0");
  etapes[11] = namealloc("Test_Tap_5");
  etapes[12] = namealloc("Test_Tap_7");
  etapes[13] = namealloc("Test_Tap_4");
  etapes[14] = namealloc("Test_Tap_e");
  etapes[15] = namealloc("Test_Tap_9");
  etapes[16] = namealloc("Test_Tap_b");
  etapes[17] = namealloc("Test_Tap_8");
  etapes[18] = namealloc("Test_Tap_a");
  etapes[19] = namealloc("Test_Tap_9");
  etapes[20] = namealloc("Test_Tap_b");
  etapes[21] = namealloc("Test_Tap_b");
  etapes[22] = namealloc("Test_Tap_8");
  etapes[23] = namealloc("Test_Tap_d");
  etapes[24] = namealloc("Test_Tap_7");
  etapes[25] = namealloc("Test_Tap_4");
  etapes[26] = namealloc("Test_Tap_f");
  etapes[27] = namealloc("Test_Tap_f");
  etapes[28] = namealloc("Test_Tap_c");
 


  valeur[TCK]  = '0';
  valeur[TRST] = '1';
  valeur[TDI]  = '0';
  valeur[TDO]  = '*';
  valeur[TMS]  = '0';

/*namealloc("Run_Test_Idle") ou Test_Logic_Reset*/
  ppapat = TAP_in(ppapat, valeur, namealloc("Run_Test_Idle"));

/*Run_Test_Idle*/
  for(i = 0; i < 4; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);
    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);
  }

/*Exit1_DR*/
  for(i = 0; i < 2; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);
  }

/*Pause_DR*/
  for(i = 0; i < 4; i++) {
    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);
  }

/*Select_IR_Scan*/
  for(i = 0; i < 3; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);

    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);
  }

/*Exit2_IR*/
  for(i = 0; i < 2; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);
  }

/*Pause_IR*/
  for(i = 0; i < 6; i++) {
    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, etapes[j++]);
  }

/*Test_Logic_Reset*/
  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, etapes[j++]);
/*Run_Test_Idle*/

  return(ppapat);
}

/****************************************************************************/
/* Positionnement de l'automate a etats finis pour l'envoi d'une donnee dans 
   le registre Boundary Scan */
/****************************************************************************/
static struct papat *LoadData(struct papat *ppapat)
{
  char valeur[5];

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';

  ppapat = TAP_in(ppapat, valeur, namealloc("Select_dr"));                 /* TMS = 1 */

  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Capture_dr"));     /* TMS = 0 capture_dr */

  return(ppapat);
}

/****************************************************************************/
/* Fin decalage de la donnee dans le registre Boundary Scan */
/****************************************************************************/
static struct papat *EndShiftData(struct papat *ppapat)
{
  char valeur[5];

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';

  ppapat = TAP_in(ppapat, valeur, namealloc("Update_dr"));      /* TMS = 1 update_dr */

  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Run_Test_Idle"));        /* TMS = 0 RTI */

  return(ppapat);
}

/****************************************************************************/
/* Sequence auxiliaire d'envoi d'une instruction par l'automate a etats */
/****************************************************************************/
static struct papat *LoadInst_aux(struct papat *ppapat, char *nom, char *first, Infos *inf)
{
  int i = 0;
  char valeur[5], *inst = NULL;
  Instructions *pinst;
  Cellules *cell;

  
  /* stop recursivity */
  if (inf==NULL) return ppapat;
  ppapat=LoadInst_aux(ppapat,nom,first,inf->NEXT);


  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';


  /*recherche d'une cellule utilisee*/
  for (cell=inf->CELL; cell; cell=cell->NEXT)
     if ( cell->NAME ) break;
     
  /*mettre un bypass si pas de cellule active*/
  if ( !cell ) nom = namealloc(BYPASS_NAME);
    
  for(pinst = inf->INST; pinst; pinst = pinst ->NEXT)
  {
    if( pinst->NAME == nom ) {
      inst = pinst->VALUE;
      break;
    }
  }  
  
  
  if ( !inst )
  {
    fprintf(stderr, "Command %s not found for boundary-scan chip\n", nom );
    exit(1);
  }
  
  if (*first=='*') *first=inst[strlen(inst)-1];
  
  valeur[TMS] = '0';
  valeur[TDI] = inst[strlen(inst)-1]; 
  valeur[TDO] = '-';  /* VERIFICATION de SYNCHRONISATION: always second bit = 0 (norme Boundary Scan). */
  ppapat = TAP_in(ppapat, valeur, pinst->NAME);

  for(i = strlen(inst)-2; i >= 0; i--) {
    if(i != 0 || inf!=InfosVbe) {               /*Shift_IR*/
	 valeur[TMS] = '0';
	 valeur[TDI] = inst[i]; 
	 valeur[TDO] = '*';
	 ppapat = TAP_out(ppapat, valeur, namealloc(""));
    }
    else {          /*Exit1_IR*/
	 valeur[TMS] = '1';
	 valeur[TDI] = inst[i]; 
	 if ((*first) == '1') valeur[TDO] = '+';
	 if ((*first) == '0') valeur[TDO] = '-';   /* VERIFICATION de SYNCHRONISATION: first in --> first out*/

	 ppapat = TAP_in(ppapat, valeur, namealloc("Exit1_ir"));
    }
  }

  return(ppapat);
}

/****************************************************************************/
/* Sequence d'envoi d'une instruction par l'automate a etats finis */
/*****************************************************************************/
static struct papat *LoadInst(struct papat *ppapat, char *nom)
{
  char valeur[5];
  char first[1];

  first[0]     = '*';
  valeur[TCK]  = '0';
  valeur[TRST] = '1';
  valeur[TMS]  = '1';
  valeur[TDI]  = '0';
  valeur[TDO]  = '*';

  ppapat = TAP_in(ppapat, valeur, namealloc("Select_dr"));
  
  ppapat = TAP_in(ppapat, valeur, namealloc("Select_ir"));

  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Capture_ir"));

  valeur[TDO] = '+';	/*VERIFICATION de SYNCHRONISATION: always first bit = 1 (norme Boundary Scan)*/ 
  ppapat = TAP_in(ppapat, valeur, namealloc("Shift_ir"));


  ppapat = LoadInst_aux(ppapat, namealloc(nom), first, InfosVbe);

  valeur[TMS] = '1';
  valeur[TDO] = '*';
  ppapat = TAP_in(ppapat, valeur, namealloc("Update_ir"));
  
  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, namealloc("Run_Test_Idle"));

  return(ppapat);
}

/*****************************************************************************/
/*affecte une valeur suivant le mode                                         */
/*****************************************************************************/
static struct papat *configure(struct papat *ppapat,Data *data)
{
  char valeur[5];
  char *label=NULL;
  int i;
  
  SEPAR = '_';
  
  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '0';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
 
  ppapat = TAP_in(ppapat, valeur, namealloc("Shift_dr"));         /* TMS = 0 shift_dr */

  /*initialiser toutes les cellules en in ou en out*/
  for (i = taille-1; i >= 0; i-- )
  {
    if ( !data[i].CELL )
    {
	    valeur[TDI]='0';                  /*par rapport a l'EMULBS31*/
	    label  = namealloc(BYPASS_NAME);
    }
    else
    switch (data[i].NEW_VAL)
    {
	 case '+':case '-':case '*':  /*configuration pour recevoir valeur*/
	    valeur[TDI]='0';                  /*par rapport a l'EMULBS31*/
	    label  = concatname("config_out",data[i].NAME);          /*i.e namealloc()*/
	    break;
	 case '1':case '0':           /*configuration pour envoyer valeur */
	    valeur[TDI]='1';                  /*par rapport a l'EMULBS31*/
	    label  = concatname("config_in",data[i].NAME);           /*i.e namealloc()*/
	    break;
	 }
    
    /*le dernier est un peu special*/
    if ( i == 0 )  valeur[TMS] = '1';                  /* TMS = 1 exit1_dr */

    ppapat = TAP_in(ppapat, valeur, label);
  }

  return(ppapat);
}

/*****************************************************************************/
/*affecte une valeur suivant le mode                                         */
/*****************************************************************************/
static char ValeurData(int valeur, int inversion)
{
  if (!inversion) return valeur;
  
  /* les '+' et '-' ce sont pour les sorties et les 1 et 0 pour les entrees*/
  switch(valeur) {
     case '0': return '1'; 
     case '1': return '0';
     case '-': return '+';       
     case '+': return '-';
     case '*': return '*';
  }
  
  fprintf(stderr,__FUNCTION__": Sign '%c' unknown\n",valeur);
  exit( 1 );
  return 0;
}

/****************************************************************************/
/* Decalage de la donnee dans le registre Boundary Scan */
/****************************************************************************/
static struct papat *ShiftData(struct papat *ppapat)
{
  char valeur[5];
  int i = 0, j = taille-1;  /*i pour les entrees et j pour les sorties*/
/* pour les labels des i/o*/
  char  In_num[32];
  char  Out_num[32];
  char  buf[1024];
  char *label;
  
  sprintf(In_num, "in%d_", PATTERN_NUMBER_PARALLEL );
  sprintf(Out_num, "out%d_", PATTERN_NUMBER_PARALLEL - 1 );
  
  valeur[TCK]  = '0';
  valeur[TRST] = '1';
  valeur[TMS]  = '0';
  valeur[TDI]  = '0';
  valeur[TDO]  = '*';


  CURRENT_BOUNDARY_SCAN_CELL = j;

  /*calcule pour une sortie a effet retarde*/
  if ( data[j].CELL!=NULL
  && ( data[j].OLD_VAL=='+' || data[j].OLD_VAL=='-' || data[j].OLD_VAL=='*' )
  && PATTERN_NUMBER_PARALLEL > 0 )
  {
	 	valeur[TDO] = data[j].OLD_VAL;
	 	sprintf(buf, "%s%s", Out_num, data[j].NAME);
      label = block_malloc( buf );
	 	/*printf("nom=%s\n",data[j].CELL->NAME);*/
		ppapat=TAP2(ppapat, valeur, namealloc("Shift_dr"), label);
  }
  else 
  {
     ppapat = TAP_in(ppapat, valeur, namealloc("Shift_dr"));
#if 0     
	  printf("nom=%s, j=%d, valeur[TDO] =%c, data[%d].OLD_VAL=%c\n",data[j].CELL->NAME, j,
            valeur[TDO],j,data[j].OLD_VAL);
#endif     
  }
  
  j--;


/*deroulement du corps*/
  for(i = taille-1; i > 0; i--, j--) {
    CURRENT_BOUNDARY_SCAN_CELL = j;
    valeur[TCK] = '0';
    valeur[TDI] = '0';
    valeur[TDO] = '*';

    /*affichage des entrees*/
    if ( data[i].CELL!=NULL 
    && ( data[i].NEW_VAL=='0' || data[i].NEW_VAL=='1' ) )
    {
	   valeur[TDI] = data[i].NEW_VAL;
	 	sprintf(buf, "%s%s", In_num, data[i].NAME);
	 	label = block_malloc( buf );
    }
    else label = namealloc("");  
	 
    ppapat = Envoi(ppapat, valeur, label);
    
    valeur[TCK] = '1';

    /*et des sorties*/
    if ( data[j].CELL!=NULL
    && ( data[j].OLD_VAL=='+' || data[j].OLD_VAL=='-' || data[j].OLD_VAL=='*' )
    && PATTERN_NUMBER_PARALLEL > 0 )
    {
	 		valeur[TDO] = data[j].OLD_VAL;
	 	   sprintf(buf, "%s%s", Out_num, data[j].NAME);
	 	   label = block_malloc( buf );
    }
    else label = namealloc("");
    
    ppapat = Envoi(ppapat, valeur, label);
    
  }/*fin de boucle*/


/*cas particulier du dernier qui doit activer l'automate*/
  CURRENT_BOUNDARY_SCAN_CELL = j;
  valeur[TMS] = '1';                            /* TMS = 1 exit1_dr */
  valeur[TDI] = '0'; 
  
  /*la derniere entree*/
  if ( data[i].CELL!=NULL 
  && ( data[i].NEW_VAL=='0' || data[i].NEW_VAL=='1' ) )
  {
       valeur[TDI]=data[i].NEW_VAL;
	 	 sprintf(buf, "%s%s", In_num, data[i].NAME);
	 	 label = block_malloc( buf );
       ppapat = TAP_in(ppapat, valeur,  label );
  }
  else ppapat = TAP_in(ppapat, valeur, namealloc("Exit1_dr"));
  
  
  CURRENT_BOUNDARY_SCAN_CELL = -1;
  return(ppapat);
}

/****************************************************************************/
/* Initialisation de la table de conversion entre vecteurs paralleles et 
   valeurs des cellules du registre Boundary Scan */ 
/****************************************************************************/
static void InitData(struct paseq *pat)
{
  struct paiol *ppaiol;
  Cellules *pcel;
  Infos *inf;
  int i = 0, j = 0;

  /*taille de toutes les chaines bs concatenees*/
  for (taille=0,inf=InfosVbe; inf;inf=inf->NEXT) {
    /*recherche si cellules utilisées*/
    for (pcel=inf->CELL; pcel; pcel=pcel->NEXT) 
      if (pcel->NAME) break;
    if ( pcel) taille += inf->BS_SIZE;
    else       taille += inf->BYPASS_SIZE;
  }
  data = (Data*)mbkalloc(taille * sizeof(Data));

  for(i = 0; i < taille; i++) {
    data[i].OLD_VAL = '*';
    data[i].NEW_VAL = '*';
    data[i].INDEX = -1;
    data[i].CELL = NULL;
    data[i].NAME = NULL;
    data[i].OLD_PAEV = NULL;
    data[i].NEW_PAEV = NULL;
  }

  i=0;  /* Compteur du nombre de cellules */
  for (inf=InfosVbe; inf; inf=inf->NEXT) {
    
    /*recherche si cellules utilisées*/
    for (pcel=inf->CELL; pcel; pcel=pcel->NEXT)
       if (pcel->NAME) break;
    
    if ( !pcel ) 
    {
      i += inf->BYPASS_SIZE;    /*count bypass of boundary*/
      continue;
    }
    
    for(pcel = inf->CELL; pcel; pcel = pcel->NEXT) 
    {
     
      if ( !pcel->NAME )
      {
        /*count also empty cell*/
        i++;
        continue;
      }
      
      /*search entree*/
      j=0;
      for(ppaiol = pat->PAIOL; ppaiol; ppaiol = ppaiol->NEXT) 
      {
         if ( ppaiol->MODE!='O' && ppaiol->MODE!='I' && ppaiol->MODE!='T' ) continue;
         if ( pcel->NAME== ppaiol->NAME ) break;
         j++;
      }   
     
      if ( !ppaiol )
      {
         fprintf(stderr, __FUNCTION__ ": entry '%s' not found\n", pcel->NAME);
         exit(1);
      }
      
	   data[i].CELL   = pcel;
	   data[i].INDEX  = j;
	   data[i].NAME   = del_espace(data[i].CELL->NAME); /*utilise le namealloc*/
#if 0
      printf("la valeur de j=%d et de data[%d].CELL.NAME=%s\n",j,i,ppaiol->NAME);
#endif
      
      i++;            /*count used cell*/
      
    }
    
  }/*fin de boucle sur Infos*/


}

/****************************************************************************/
/* Affectation des valeurs des cellules du registre Boundary Scan dans la
   structure Data en fonction de la structure de donnees des vecteurs 
   paralleles et de la structure de donnees du fichier informations */
/****************************************************************************/
static struct papat *Traduction(struct papat *ppapat, struct papat *pat_bs )
{
  int i, j;
  struct paevt *ppaevt;
  Cellules *pcel;
  Cellules *command;
  Infos *inf;
  static int reconfig = 1;                 


    /* Pour chaque case de current */ 
    for(i = 0; i < taille; i++) 
    {
 	   data[i].OLD_VAL   = data[i].NEW_VAL;   /* ancien <- current */
      data[i].OLD_PAEV = data[i].NEW_PAEV;
  
      
      /*cell active*/
      if ( !data[i].CELL ) continue;
      
      for( ppaevt = ppapat->PAEVT; ppaevt; ppaevt = ppaevt->NEXT )
        if ( ppaevt->INDEX == data[i].INDEX ) break;
      
      if( !ppaevt ) 
      {
         ppapat->PAEVT = pat_addpaevt( ppapat->PAEVT, data[i].INDEX, 
                                 data[i].OLD_VAL );
         ppaevt        = ppapat->PAEVT;
      }

	   data[i].NEW_VAL  = ValeurData(ppaevt->USRVAL, data[i].CELL->INVERT);
      data[i].NEW_PAEV = ppaevt;

#if 0
      printf("i=%d, mode=%d, datacurrent=%c mais paevt->USRVAL=%c\n",i, data[i].CELL->donnees[DMODE], data[i].NEW_VAL,ppaevt->USRVAL);
#endif

		/*mode special pour les inout sans cellule de controle*/
		if (data[i].CELL->MODE==MODE_INOUT)
      { 
		     switch (data[i].NEW_VAL){
		     case '-':case '+':case '*': 
			    if (data[i].OLD_VAL=='0' || data[i].OLD_VAL=='1') 
			    reconfig=1; 
             break;
		     case '0':case '1': 
			    if (data[i].OLD_VAL=='+' || data[i].OLD_VAL=='-' || 
			        data[i].OLD_VAL=='*') reconfig=1;
             break;
		     }
      }
		/*mode special pour les inout avec cellule de controle*/
		else if (data[i].CELL->MODE==MODE_TRISTATE) 
      {
		  command = data[i].CELL->COMMAND_CELL;
        /* On recherche la cellule qui la controle */
		  for (j=0,inf=InfosVbe; inf; inf=inf->NEXT){
		       for( pcel=inf->CELL; pcel; pcel=pcel->NEXT )
             {
               if ( pcel == command ) break;
               j++;
             }  
		       if ( pcel ) break;
		  }

		  /*doit etre configurer en entree*/
		  if(data[i].NEW_VAL == '1' || data[i].NEW_VAL == '0')
	       data[j].NEW_VAL=ValeurData( pcel->COMMAND_VALUE, command->INVERT ); 
		  /*doit etre configurer en sortie:   DDEFAUT val de Hiz*/
		  else if(data[i].NEW_VAL == '+' || data[i].NEW_VAL == '-')
	       data[j].NEW_VAL = ValeurData( pcel->COMMAND_VALUE, !command->INVERT );
		}/*fin de MODE_TRISTATE*/
   
   }/*fin de boucle sur data[i] */
 

    /******* partie chargement *********/
    /*reconfiguration pour les cellules inout de l'emulbs et meme au depart*/
    if( reconfig ) {
       if(verbose_flag) 
       {
          fprintf(stdout,"###----- cells configured before %d -----####\n",
		       PATTERN_NUMBER_PARALLEL);
       }      

	    pat_bs = LoadInst(pat_bs, "config");

	    pat_bs = LoadData(pat_bs);          /*remise a jour de la chaine bs*/
	    pat_bs = configure(pat_bs,data);
	    pat_bs = EndShiftData(pat_bs);/*application en parallele au circuit*/
       
       pat_bs = LoadInst(pat_bs, INTEST_NAME);
       reconfig=0;
    }

    if(verbose_flag)
    {
      fprintf(stdout,"###----- pattern %d in treatment -----####\n",
         PATTERN_NUMBER_PARALLEL);
    }

    pat_bs = LoadData(pat_bs);                  /*remise a jour de la chaine bs*/
    pat_bs = ShiftData(pat_bs);            /*chargement serie des donnees*/
    pat_bs = EndShiftData(pat_bs);        /*application en parallele au circuit*/

    PATTERN_NUMBER_PARALLEL++;       /*pour message d'erreur*/
    PATTERN_LINE_PARALLEL = ppapat->LINE;

  return pat_bs;
}

/****************************************************************************/
/****************************************************************************/
static struct papat *DebutPapat()
{
  struct papat *pat_bs;

  /*reinitialiser pour ne pas etre influencer par les patterns precedentes
    i.e  le check du hardware  */
  Envoi(NULL,NULL,NULL);
  /*pat reelle*/
  pat_bs = ResetBS(NULL);
  pat_bs = TestTap(pat_bs);  

  /*all the bs pattern was inverted*/
  return  (struct papat*) reverse( (chain_list*) pat_bs );
}


/****************************************************************************/
/* Creation de la structure de donnees des vecteurs Boundary Scan */
/****************************************************************************/
static struct paseq *DebutPaseq(struct paseq *pat)
{
  int i = 0;
  char *broches[5];
  struct paiol *ppaiol=NULL;
  struct paseq *pat_bs;
  int count;

  count = Nb_bs_pat;


  if (pat==NULL) return NULL;

  broches[TCK]  = namealloc(TCK_NAME);
  broches[TRST] = namealloc(TRST_NAME);
  broches[TMS]  = namealloc(TMS_NAME);
  broches[TDI]  = namealloc(TDI_NAME);
  broches[TDO]  = namealloc(TDO_NAME);

  pat_bs = pat_addpaseq(NULL);
  pat_bs->CURPAT = DebutPapat();


  /***** chargement des modes pour les 5 broches *****/
  for(i = 0; i < 5; i++) {
    pat_bs -> PAIOL = pat_addpaiol(pat_bs->PAIOL, broches[i], 'B', i == TDO ? 'O' : 'I',
                                 BS_ESPACE);
    pat_bs->IOLNBR++;
  }
  
  /***** chargement de l'alimentation pour fichier PAT *****/
  for(ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT) 
  {
      ppaiol -> BLANK = BS_ESPACE;			/*espace entre chaque entree*/
      if( !isvdd(ppaiol -> NAME) && !isvss(ppaiol -> NAME)) continue;
      
      pat_bs -> PAIOL = pat_addpaiol(pat_bs -> PAIOL, ppaiol -> NAME, 'B', 'I', 
                               BS_ESPACE ); 
      
      pat_bs->IOLNBR++;
  }

  pat_bs -> PAIOL = (struct paiol*) reverse( (chain_list*)  pat_bs -> PAIOL);
  
  
  /***positionnement des alimentations si besoin***/
  i = 0;
  for(ppaiol = pat_bs -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT) 
  {
      if(isvdd(ppaiol -> NAME)) 
        pat_bs->CURPAT -> PAEVT = pat_addpaevt(pat_bs->CURPAT -> PAEVT, i, '1' );
      else if(isvss(ppaiol -> NAME)) 
        pat_bs->CURPAT -> PAEVT = pat_addpaevt(pat_bs->CURPAT -> PAEVT, i, '0' );
      i++;  
  }    

  pat_bs->ENDFLG = 'N';
  pat_bs->PATNBR = Nb_bs_pat - count;
  pat_bs->DRVSEQ = 0;                      /*ancienne sequence n-1 */
  pat_bs->SUBSEQ = 1;                     /*sequence currente n  */
  
  return(pat_bs);
}


/****************************************************************************/
/* Creation de la structure de donnees des vecteurs Boundary Scan */
/****************************************************************************/
static struct papat *CorpsPapat(int nb_pat)
{
  struct papat *pat_bs = NULL;
  int count = 0;

  
  /***** traduction vecteurs paralleles *****/
  /* Pour chaque sequence de n patterns faire*/
  while ( Current_Parallel_pat )
  {
     pat_bs = Traduction(Current_Parallel_pat,pat_bs);/*premiere pat deja traitee*/
     count++;
     Current_Parallel_pat = Current_Parallel_pat->NEXT;
     if ( count >= nb_pat ) break;
  }
  
  /*all the bs pattern was inverted*/
  return  (struct papat*) reverse( (chain_list*) pat_bs );
}


/****************************************************************************/
/* Creation de la structure de donnees des vecteurs Boundary Scan */
/****************************************************************************/
static struct paseq *CorpsPaseq( struct paseq *pat_bs, int nb_pat )
{
  int count;

  count = Nb_bs_pat;
  pat_bs->CURPAT = CorpsPapat(nb_pat);
  
  /***** mise a jour des donnees *****/  
  pat_bs->ENDFLG = 'N';         /*fin de pattern ? non pos encore*/
  pat_bs->PATNBR = Nb_bs_pat - count;
  pat_bs->DRVSEQ = pat_bs->SUBSEQ;                      /*ancienne sequence n-1 */
  pat_bs->SUBSEQ++;                     /*sequence currente n puis prochaine n+1 */

  return pat_bs;
}


/****************************************************************************/
/* Creation d'une derniere serie de vecteurs BS du au retard des sorties  */
/****************************************************************************/
static struct papat *FinPapat()
{
int i;
int valeur_sur_sortie=0;
struct papat* ppapat;

/*on ne s'occuppe ici que des sorties*/
  for(i = 0; i < taille; i++) {
      /*si celle-ci est valide*/
      if (data[i].CELL!=NULL) {
	      if (data[i].CELL->MODE==MODE_IN) data[i].CELL=NULL;
	      else {
		      data[i].OLD_VAL=data[i].NEW_VAL;         /* ancien <- current */
		      if (data[i].OLD_VAL=='+' || data[i].OLD_VAL=='-')
		         valeur_sur_sortie=1;
		      else data[i].CELL=NULL;
	      }
      }
  }

  if (valeur_sur_sortie) {
    ppapat = LoadData(NULL);                  /*remise a jour de la chaine bs*/
    ppapat = ShiftData(ppapat);                /*chargement serie des donnees*/
    ppapat = EndShiftData(ppapat);      /*application en parallele au circuit*/
    ppapat = ResetBS(ppapat);
  }
  

  return  (struct papat*) reverse( (chain_list*) ppapat );
}


/****************************************************************************/
/* Creation de la structure de donnees des vecteurs Boundary Scan */
/****************************************************************************/
static struct paseq *FinPaseq(struct paseq *pat_bs)
{
  int count;

  count = Nb_bs_pat;
  pat_bs->CURPAT = FinPapat();
  
  /***** mise a jour des donnees *****/  
  pat_bs->ENDFLG = 'Y';         /*fin de pattern ? non pos encore*/
  pat_bs->PATNBR = Nb_bs_pat - count;
  pat_bs->DRVSEQ = pat_bs->SUBSEQ;                      /*ancienne sequence n-1 */
  pat_bs->SUBSEQ++;                     /*sequence current n  */

  return pat_bs;
}


/*****************************************************************************/
/* Creation des vecteurs Boundary Scan en n sequences*/
void traduire(char *sce, char *destSRC, char *destBS, unsigned int nb_pat, unsigned int burst_size)
{
struct timeb start_prg,end_prg;
unsigned long time_tck=0,time_io=0,time_prg=0;       /* to compute exec time */
struct paseq *pat=NULL;                          /* vecteurs paralleles total*/
struct paseq *pat_bs;                         /* vecteurs Boundary Scan */
unsigned int pat_tot = 0;

  if ( !sce ) return;

  /***INITIALISATION ET COHERENCE***/
  ftime(&start_prg);

  /*  ouverture du port et reset BS */
  if (execute_flag) open_port(burst_size);
  
  pat = load_patterns( sce, pat, 0/*all*/, &time_io);
  PASEQ_PARALLEL = pat;
  
  /* Coherence entre le fichier pat et le BSDL */
  VerificationsPat(pat);
  InitData(pat);
  
  /*les premieres patterns boundary scan d'initialisation de l'automate*/
  pat_bs = DebutPaseq( pat );
  if ( execute_flag    )  pat_bs = hard_paseq(pat_bs, burst_size, &time_tck);
  if ( destBS  )  save_patterns(destBS, pat_bs, &time_io);
  else pat_frepapat( pat_bs->CURPAT );
 
  pat_tot += pat_bs->PATNBR;
 
  Current_Parallel_pat = pat->CURPAT;
  
  while ( Current_Parallel_pat ) 
  {
    pat_bs = CorpsPaseq( pat_bs, nb_pat );
    
    if ( execute_flag    )  pat_bs = hard_paseq(pat_bs, burst_size, &time_tck);
    if ( destBS  )  save_patterns(destBS, pat_bs, &time_io);
    else pat_frepapat( pat_bs->CURPAT );
    
    pat_tot += pat_bs->PATNBR;
    block_free(); /*liberation des labels de la structure*/
  }
 
 
  pat_bs = FinPaseq( pat_bs );
  if ( execute_flag    )  pat_bs = hard_paseq(pat_bs, burst_size, &time_tck);
  if ( destBS  )  save_patterns(destBS, pat_bs, &time_io);
  else pat_frepapat( pat_bs->CURPAT );

  pat_tot += pat_bs->PATNBR;

  if ( destSRC )  save_patterns(destSRC, pat, &time_io);
  

  /*liberation memoire c'est la derniere pattern*/
  mbkfree(data);
  

  if (execute_flag) close_port();
  
  /*temps*/
  ftime(&end_prg);
  time_prg=
       (end_prg.time-start_prg.time)*1000 + end_prg.millitm-start_prg.millitm;

  statistic( time_prg, time_tck, time_io, pat_tot);
}

/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/

