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
#include "time.h"
/****************************************************************************/
static Data *data=NULL;                   /*sert a la construction de pattern*/
static int NbPat = 0;                                  /*numero de pattern bs*/
static int CURRENT_PARALLEL_PATTERN_NUMBER = -1;      /*numero de pattern // */
static int CURRENT_PARALLEL_PATTERN_LINE = -1;      /*numero de ligne dans le*/
                                                           /* fichier pat // */
static int CURRENT_BOUNDARY_SCAN_CELL = -1;
static struct papat *Old_pat=NULL;         /*pattern sortant du boundary scan*/
static int EXECUTION_ERRORS = 0;
static int taille=0;                      /*taille de la chaine boundary scan*/
static char *Run_Test_Idle="Run_Test_Idle";
static char *Exit1_ir="Exit1_ir";
static char *Exit1_dr="Exit1_dr";
static char *Shift_ir="Shift_ir";
static char *Shift_dr="Shift_dr";
static char *Select_ir="Select_ir";
static char *Select_dr="Select_dr";
static char *Capture_ir="Capture_ir";;
static char *Capture_dr="Capture_dr";
static char *Update_ir="Update_ir";
static char *Update_dr="Update_dr";
static char *vide="";
/****************************************************************************/
/*affiche l'erreur et la sauvegarde   et si fatal erreur exit*/
static void ExecError(attendu,recu)
char attendu;
char recu;
{
Cellules *cel;

/*Si erreur de synchronisation alors le signaler*/
if (CURRENT_BOUNDARY_SCAN_CELL<0 || CURRENT_BOUNDARY_SCAN_CELL>=taille) { 
     fprintf(stderr,"\rSYNCHRONISATION ERROR!!! --> STOP1 next results will be wrong\n");
     if (verbose_flag) {
	  fprintf(stderr,
		  "Error on pattern %d (line %d) :\n"
		  "CURRENT_BOUNDARY_SCAN_CELL : %d\n"
		  "\tboundary scan rules expecting '%c', actual is '%c'\n"
		  ,CURRENT_PARALLEL_PATTERN_NUMBER
		  ,CURRENT_PARALLEL_PATTERN_LINE
		  ,CURRENT_BOUNDARY_SCAN_CELL
		  ,attendu,recu);}
	  pcbs_exit(1);
}
else cel=data[CURRENT_BOUNDARY_SCAN_CELL].cel;

/*Erreur impossible--> erreur de synchronisation */
if (cel==NULL || (cel->donnees[DMODE]!=OUTPUT2 && cel->donnees[DMODE]!=OUTPUT3 && cel->donnees[DMODE]!=INOUTPUT)) {
     fprintf(stderr,"\rSYNCHRONISATION ERROR!!! --->STOP2 next results will be wrong\n");
     if (verbose_flag) {
	  fprintf(stderr,
		  "Error on pattern %d (line %d) :\n"
		  "\tboundary scan rules expecting '%c', actual is '%c'\n"
		  ,CURRENT_PARALLEL_PATTERN_NUMBER
		  ,CURRENT_PARALLEL_PATTERN_LINE
		  ,recu
		  ,attendu);}
	  pcbs_exit(1);
}


/*affichage*/
if (cel->donnees[DINVERSION]) {
  if (verbose_flag)
     fprintf(stdout,
	 "Error on pattern %d (line %d) : expecting '%c' actual '%c' on %s\n"
	,CURRENT_PARALLEL_PATTERN_NUMBER
	,CURRENT_PARALLEL_PATTERN_LINE
	,recu
	,attendu
	,cel->nom);
}
else { 
 if (verbose_flag)
    fprintf(stdout,
	 "Error on pattern %d (line %d) : expecting '%c' actual '%c' on %s\n"
	,CURRENT_PARALLEL_PATTERN_NUMBER
	,CURRENT_PARALLEL_PATTERN_LINE
	,attendu
	,recu
	,cel->nom);
}

/*comptabilisation des erreurs*/
EXECUTION_ERRORS ++;
}
/****************************************************************************/
/* Envoi un vecteur Boundary Scan (TDI, TDO, TRST, TCK, TMS) sur le circuit
   ou dans une structure de donnees */
static struct papat *Envoi(ppapat, valeur, label)
  struct papat *ppapat;
  char *valeur;
  char *label;  
{
static char ancienne[5]={'9','9','9','9','9'};  /*evenements paevt precedents*/
  int i = 0;
  struct paevt *ppaevt;
  char tdo_recu, val[5];
  Cellules *cel;
  struct paevt *evt;

  NbPat++;                                            /*nombre de patterns bs*/
  tdo_recu=valeur[TDO];                             /*bonne valeur par defaut*/

/****si on utilise une carte hardware alors****/
  if(execute_flag) {

	/**** Ralentissement d'envoi du vecteur ****/
	/*clock_t init;
	init=clock();*/
      
    val[0] = valeur[TDI]; 
    val[1] = valeur[TMS]; 
    val[2] = valeur[TRST]; 
    val[3] = valeur[TCK]; 
    val[4] = '\0';
    tdo_recu = getsimval(val);/*cf. bstools.h*/
    if(valeur[TDO]!='*' && valeur[TDO]!=tdo_recu) ExecError(valeur[TDO],tdo_recu);
  
  /**** Ralentissement d'envoi du vecteur ****/
	/*while (clock()<(init+1))*/
	/*for(i=0; i<1000; i++);*/
	
    /*mise a jour des sorties paralleles si sauvegarde patterns en fichier resultat fonctionnel*/
    if (savePatSRC_flag && Old_pat!=NULL && CURRENT_BOUNDARY_SCAN_CELL>=0 && CURRENT_BOUNDARY_SCAN_CELL<taille) {
	 Old_pat->SIMFLAG='S';
 	 cel=data[CURRENT_BOUNDARY_SCAN_CELL].cel;
	 if (cel!=NULL && (cel->donnees[DMODE]==OUTPUT2 || cel->donnees[DMODE]==OUTPUT3 || cel->donnees[DMODE]==INOUTPUT)){
	      /*fprintf(stdout,"cel=%d, mode=%d\n",CURRENT_BOUNDARY_SCAN_CELL,cel->donnees[DMODE]);fflush(stdout);*/
	      for (evt=Old_pat->PAEVT;evt && evt->INDEX!=data[CURRENT_BOUNDARY_SCAN_CELL].index;evt=evt->NEXT) ;
	      if (evt!=NULL){	      		
			switch (evt->USRVAL){
				case '0': case '1': break; /* pour cellules INOUT en entree; on n'ecrit pas en SIMVAL */
				case '-': case '+': case '*':   /* pour cellules OUTPUT2, OUTPUT3 ou INOUT en sortie */
					evt->SIMVAL=tdo_recu; 
					/* printf("INOUT: valeur TDO recu circuit=%c et valeur attendu=%c\n",evt->SIMVAL,evt->USRVAL);*/
					break;
				}
		}
									     
	      else {Old_pat->PAEVT=pat_addpaevt(Old_pat->PAEVT,data[CURRENT_BOUNDARY_SCAN_CELL].index,data[CURRENT_BOUNDARY_SCAN_CELL].ancien);
	  	     switch (Old_pat->PAEVT->USRVAL){
				case '0': case '1': break; /* pour cellules INOUT en entree; on n'ecrit pas en SIMVAL */
 				case '-': case '+': case '*':   /* pour cellules OUTPUT2, OUTPUT3 ou INOUT en sortie */
					Old_pat->PAEVT->SIMVAL=tdo_recu; 
					/* printf("INOUT: valeur TDO recu circuit=%c et valeur attendu=%c\n",Old_pat->PAEVT->SIMVAL,Old_pat->PAEVT->USRVAL);*/
					break;
				}
		}

	 }
    }

  }/****fin du mode hardware****/


/***si pas de fichier bs resultat alors sortir***/
  if (!savePatBS_flag) return ppapat;



/***construire pattern pour fichier serie resultat***/

	 if(ppapat -> LABEL != NULL) {
       ppapat -> NEXT = pat_addpapat(ppapat, vide, NULL);
       ppapat -> NEXT -> LABEL = label;
       ppapat -> NEXT -> SIMFLAG = 'S';
       ppapat -> NEXT -> NEXT = NULL;
       ppapat = ppapat -> NEXT;
  }
  else {
       ppapat -> LABEL = label;
       ppapat -> SIMFLAG = 'S';
  }
  ppapat -> PAEVT = pat_addpaevt(ppapat -> PAEVT, NULL, NULL);
  ppapat -> PAEVT -> NEXT = NULL;
  ppaevt = ppapat -> PAEVT;
  
  for(i = 0; valeur[i] != '\0'; i++) {
       if(i>=5 || valeur[i]!=ancienne[i]){       /*changing events and alim. */
	    if(ppaevt -> USRVAL == (char)NULL) {
		 ppaevt -> INDEX =  i;
		 ppaevt -> USRVAL = valeur[i];
		 ppaevt -> SIMVAL = (i==TDO) ? tdo_recu : valeur[i];
                                               /*val. recue ou supposee recue*/
	    }
	    else {
		 ppaevt -> NEXT = pat_addpaevt(ppaevt, i, valeur[i]); 
		 ppaevt -> NEXT -> SIMVAL = (i==TDO) ? tdo_recu : valeur[i];
                                                          /*recue ou supposee*/
		 ppaevt -> NEXT -> NEXT = NULL;
		 ppaevt = ppaevt -> NEXT;
	    }
       }
  } 
/***mise a jour des valeurs evenements pour la structure paseq***/
  for(i = 0; valeur[i] != '\0'; i++) ancienne[i] = valeur[i];
  
return(ppapat);
}
/****************************************************************************/
static struct papat *TAP_in(ppapat, valeur, label)
  struct papat *ppapat;
  char *valeur;
  char *label;  
{
  char save=valeur[TDO];
  valeur[TDO] = '*';
  valeur[TCK] = '0';
  ppapat = Envoi(ppapat, valeur, label);
  valeur[TDO]=save;		/*controle toujours sur front montant*/
  valeur[TCK] = '1';
  ppapat = Envoi(ppapat, valeur, vide);
return ppapat;
}
/****************************************************************************/
static struct papat *TAP_out(ppapat, valeur, label)
  struct papat *ppapat;
  char *valeur;
  char *label;  
{
  char save=valeur[TDO];
  valeur[TDO] = '*';
  valeur[TCK] = '0';
  ppapat = Envoi(ppapat, valeur, vide);
  valeur[TDO]=save;/*controle toujours sur front montant*/
  valeur[TCK] = '1';
  ppapat = Envoi(ppapat, valeur, label);
return ppapat;
}
/****************************************************************************/
static struct papat *TAP2(ppapat, valeur, label_in,label_etat)
  struct papat *ppapat;
  char *valeur;
  char *label_in,*label_etat;  
{
  char save=valeur[TDO];
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
static struct papat *ResetBS(ppapat, ppat)
  struct papat *ppapat;
  struct paseq *ppat;
{
  int i;
  char valeur[200];/*nbr d'alimentations inconnu*/ 
  struct paiol *ppaiol;

  valeur[TRST]='0';/*mode reset, car reset=not(TRST) */
  valeur[TDO]='*';
  valeur[TDI]='0';
  valeur[TCK]='0';
  valeur[TMS]='1';

/***positionnement des alimentations si besoin***/
  if (savePatBS_flag) {
     for (i=5,ppaiol = ppat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT)
      if(isvdd(ppaiol -> NAME)) valeur[i++] = '1';
      else if(isvss(ppaiol -> NAME)) valeur[i++] = '0';
    valeur[i] = '\0';
  }
  else {
    valeur[5] = '\0';
  }
  ppapat = Envoi(ppapat, valeur, mon_malloc("Reset_BS"));
  valeur[5] = '\0';
  valeur[TCK] = '1';
  valeur[TRST]='1';/*mode normal, car reset=not(TRST) */
  ppapat = Envoi(ppapat, valeur, vide);


  for(i = 1; i < 5; i++) ppapat=TAP_in(ppapat,valeur,mon_malloc("Reset_BS"));
  /*etat= Run_Test_Idle*/

  return(ppapat);
}
/****************************************************************************/
/* Envoi d'une sequence de test de l'automate a etats finis */
static struct papat *TestTap(ppapat)
  struct papat *ppapat;
{
  int i = 0, j = 0;
  char valeur[6];
  char etapes[29][11];

  strcpy(etapes[0], "Test_Tap_c");
  strcpy(etapes[1], "Test_Tap_7");
  strcpy(etapes[2], "Test_Tap_6");
  strcpy(etapes[3], "Test_Tap_1");
  strcpy(etapes[4], "Test_Tap_3");
  strcpy(etapes[5], "Test_Tap_0");
  strcpy(etapes[6], "Test_Tap_2");
  strcpy(etapes[7], "Test_Tap_1");
  strcpy(etapes[8], "Test_Tap_3");
  strcpy(etapes[9], "Test_Tap_3");
  strcpy(etapes[10],"Test_Tap_0");
  strcpy(etapes[11],"Test_Tap_5");
  strcpy(etapes[12],"Test_Tap_7");
  strcpy(etapes[13], "Test_Tap_4");
  strcpy(etapes[14], "Test_Tap_e");
  strcpy(etapes[15], "Test_Tap_9");
  strcpy(etapes[16], "Test_Tap_b");
  strcpy(etapes[17], "Test_Tap_8");
  strcpy(etapes[18], "Test_Tap_a");
  strcpy(etapes[19], "Test_Tap_9");
  strcpy(etapes[20], "Test_Tap_b");
  strcpy(etapes[21], "Test_Tap_b");
  strcpy(etapes[22], "Test_Tap_8");
  strcpy(etapes[23], "Test_Tap_d");
  strcpy(etapes[24], "Test_Tap_7");
  strcpy(etapes[25], "Test_Tap_4");
  strcpy(etapes[26], "Test_Tap_f");
  strcpy(etapes[27], "Test_Tap_f");
  strcpy(etapes[28], "Test_Tap_c");

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
  valeur[5] = '\0';

/*Run_Test_Idle ou Test_Logic_Reset*/
  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, Run_Test_Idle);

/*Run_Test_Idle*/
  for(i = 0; i < 4; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
  }

/*Exit1_DR*/
  for(i = 0; i < 2; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
  }

/*Pause_DR*/
  for(i = 0; i < 4; i++) {
    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
  }

/*Select_IR_Scan*/
  for(i = 0; i < 3; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));

    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
  }

/*Exit2_IR*/
  for(i = 0; i < 2; i++) {
    valeur[TMS] = '0';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
  }

/*Pause_IR*/
  for(i = 0; i < 6; i++) {
    valeur[TMS] = '1';
    ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
  }

/*Test_Logic_Reset*/
  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, mon_malloc(etapes[j++]));
/*Run_Test_Idle*/

  return(ppapat);
}
/****************************************************************************/
/* Sequence auxiliaire d'envoi d'une instruction par l'automate a etats */
static struct papat *LoadInst_aux(ppapat, nom,first,inf)
  struct papat *ppapat;
  char *nom,*first;
  Infos *inf;
{
  int i = 0;
  char valeur[6], *inst;
  Instructions *pinst;
  Cellules *cell;

if (inf==NULL) return ppapat;
ppapat=LoadInst_aux(ppapat,nom,first,inf->next);

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
  valeur[5] = '\0';


    /*recherche d'une cellule utilisee*/
  for (cell=inf->cel; cell&&cell->donnees[DMODE]==UNDEF; cell=cell->suivant);
  if (cell!=NULL && cell->donnees[DMODE]!=UNDEF) {/* mettre le code*/
    for(pinst = inf-> inst; pinst; pinst = pinst -> suivant)
      if(nom!=NULL && pinst -> nom!=NULL && !strcmp(pinst -> nom, nom)) {
        inst = pinst -> code -> valeur;
	break;
      }}
  else {/*mettre un bypass si pas de cellule active*/
    for(pinst = inf-> inst; pinst; pinst = pinst -> suivant)
      if(pinst -> nom!=NULL && !strcmp(pinst -> nom, "bypass")) {
        inst = pinst -> code -> valeur;
	break;
      }}
  
  if (*first=='*') *first=inst[strlen(inst)-1];
  
  valeur[TMS] = '0';
  valeur[TDI] = inst[strlen(inst)-1]; 
  valeur[TDO] = '-';  /* VERIFICATION de SYNCHRONISATION: always second bit = 0 (norme Boundary Scan). */
  ppapat = TAP_in(ppapat, valeur, pinst->nom);

  for(i = strlen(inst)-2; i >= 0; i--) {
    if(i != 0 || inf!=InfosVbe) {               /*Shift_IR*/
	 valeur[TMS] = '0';
	 valeur[TDI] = inst[i]; 
	 valeur[TDO] = '*';
	 ppapat = TAP_out(ppapat, valeur, vide);
    }
    else {          /*Exit1_IR*/
	 valeur[TMS] = '1';
	 valeur[TDI] = inst[i]; 
	 if ((*first) == '1') valeur[TDO] = '+';
	 if ((*first) == '0') valeur[TDO] = '-';   /* VERIFICATION de SYNCHRONISATION: first in --> first out*/

	 ppapat = TAP_in(ppapat, valeur, Exit1_ir);
    }
  }

  return(ppapat);
}
/****************************************************************************/
/* Sequence d'envoi d'une instruction par l'automate a etats finis */
static struct papat *LoadInst(ppapat, nom)
  struct papat *ppapat;
  char *nom;
{
  char  valeur[6];
  char first[1];

  first[0]='*';
  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
  valeur[5] = '\0';

  ppapat = TAP_in(ppapat, valeur, Select_dr);
  
  ppapat = TAP_in(ppapat, valeur, Select_ir);

  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, Capture_ir);

  valeur[TDO] = '+';	/*VERIFICATION de SYNCHRONISATION: always first bit = 1 (norme Boundary Scan)*/ 
  ppapat = TAP_in(ppapat, valeur, Shift_ir);


  ppapat=LoadInst_aux(ppapat,nom,first,InfosVbe);


  valeur[TMS] = '1';
  valeur[TDO] = '*';
  ppapat = TAP_in(ppapat, valeur, Update_ir);
  
  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, Run_Test_Idle);

  return(ppapat);
}
/*****************************************************************************/
/*affecte une valeur suivant le mode                                         */
static struct papat *configure(struct papat *ppapat,Data *data)
{
  char valeur[6];
  char *label=NULL;
  int i;

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '0';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
  valeur[5] = '\0';
 
  ppapat = TAP_in(ppapat, valeur, Shift_dr);         /* TMS = 0 shift_dr */

  /*initialiser toutes les cellules en in ou en out*/
  for (i=taille-1; i>0;i--) {
       if (data[i].cel==NULL) {
	    valeur[TDI]='0';   /*cellules EMULBS par defaut en reception OUT*/
	    ppapat = TAP_in(ppapat, valeur, vide);}
       else 
	    switch (data[i].courant){
	    case '+':case '-':case '*':  /*configuration pour recevoir valeur*/
		 valeur[TDI]='0';                  /*par rapport a l'EMULBS31*/
		 label=concat("out_",data[i].nom);          /*i.e namealloc()*/
		 ppapat =TAP_in(ppapat, valeur, label);
		 break;
	    case '1':case '0':           /*configuration pour envoyer valeur */
		 valeur[TDI]='1';                  /*par rapport a l'EMULBS31*/
		 label=concat("in_",data[i].nom);           /*i.e namealloc()*/
		 ppapat = TAP_in(ppapat, valeur, label);
		 break;
	    }
  }

  /*le dernier est un peu special*/
  valeur[TMS] = '1';                            /* TMS = 1 exit1_dr */
  if (data[0].cel==NULL) {
       valeur[TDI]='0';         /*cellules EMULBS par defaut en reception OUT*/
       ppapat = TAP_in(ppapat, valeur, Exit1_dr);}
  else 
       switch (data[0].courant) {
       case '+':case '-':case '*':    /*configuration sortie*/
	    valeur[TDI]='0';  
	    label=concat("out_",data[0].nom);               /*i.e namealloc()*/
	    ppapat =TAP_in(ppapat, valeur, label);
	    break;
       case '1':case '0':             /*configuration entree*/
	    valeur[TDI]='1';
	    label=concat("in_",data[0].nom);                /*i.e namealloc()*/
	    ppapat = TAP_in(ppapat, valeur, label);
	    break;
       }

  return(ppapat);
}
/*****************************************************************************/
/*affecte une valeur suivant le mode                                         */
static char ValeurData(int valeur,int mode,int inversion)
{
/* les '+' et '-' ce sont pour les sorties et les 1 et 0 pour les entrees*/
switch(valeur) {
case '0': 
     if(mode==OUTPUT2 || mode==OUTPUT3) return '*';
     else if(inversion) return '1';
     else return '0';
     break;
case  '1':
     if(mode==OUTPUT2 || mode==OUTPUT3) return '*';
     else if(inversion) return '0';
     else return '1';
     break;
case '-':        
     if(mode==OUTPUT2 || mode==OUTPUT3 || mode==INOUTPUT){
	  if(inversion) return '+';
	  else return '-';}
     else if(inversion) return '1';
     else return '0';
     break;
case '+':
     if(mode==OUTPUT2 || mode==OUTPUT3 || mode==INOUTPUT){
	  if(inversion) return '-';
	  else return '+';}
     else if(inversion) return '0';
     else return '1';
     break;
default: return valeur;
}}
/****************************************************************************/
/* Positionnement de l'automate a etats finis pour l'envoi d'une donnee dans 
   le registre Boundary Scan */
static struct papat *LoadData(ppapat)
  struct papat *ppapat;
{
  char valeur[6];

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
  valeur[5] = '\0';

  ppapat = TAP_in(ppapat, valeur, Select_dr);                 /* TMS = 1 */

  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, Capture_dr);     /* TMS = 0 capture_dr */

  return(ppapat);
}
/****************************************************************************/
/* Decalage de la donnee dans le registre Boundary Scan */
static struct papat *ShiftData(ppapat)
  struct papat *ppapat;
{
  char valeur[6];
  int i = 0, j = taille-1;  /*i pour les entrees et j pour les sorties*/
/* pour les labels des i/o*/
  char *label=NULL;
  char *tmp_i_num=itoa_mbk(CURRENT_PARALLEL_PATTERN_NUMBER);
  char *tmp_o_num=itoa_mbk(CURRENT_PARALLEL_PATTERN_NUMBER-1);
  char *num_i_=concat_mbk(tmp_i_num,"_");
  char *num_o_=concat_mbk(tmp_o_num,"_");
  char *in_num=concat_mbk("in",num_i_);
  char *out_num=concat_mbk("out",num_o_);
  mbkfree (tmp_i_num);mbkfree (tmp_o_num);
  mbkfree (num_i_);mbkfree (num_o_);

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '0';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
  valeur[5] = '\0';


  CURRENT_BOUNDARY_SCAN_CELL = j;
/*calcule pour une sortie a effet retarde*/
    if (data[j].cel!=NULL && (data[j].ancien=='+' || data[j].ancien=='-')
     &&(data[j].cel->donnees[DMODE]==OUTPUT2 
     || data[j].cel->donnees[DMODE]==OUTPUT3
     || data[j].cel->donnees[DMODE]==INOUTPUT)) {
	 			valeur[TDO] = data[j].ancien;
	 			label=concat_mbk(out_num,data[j].nom);
	 			/*printf("nom=%s\n",data[j].cel->nom);*/
				ppapat=TAP2(ppapat, valeur, Shift_dr, mon_malloc(label));
	 			mbkfree(label);}
    else {ppapat = TAP_in(ppapat, valeur, Shift_dr);
	 /*printf("nom=%s, j=%d, valeur[TDO] =%c, data[%d].ancien=%c\n",data[j].cel->nom, j, valeur[TDO],j,data[j].ancien);*/}
    j--;


/*deroulement du corps*/
  for(i = taille-1; i > 0; i--) {
    CURRENT_BOUNDARY_SCAN_CELL = j;
    valeur[TCK] = '0';
    valeur[TDI] = '0';
    valeur[TDO] = '*';

/*affichage des entrees*/
    if (data[i].cel!=NULL && (data[i].cel->donnees[DMODE]==INPUT
    || (data[i].cel->donnees[DMODE]==INOUTPUT 
    && (data[i].courant=='0' || data[i].courant=='1'))))   {
	 valeur[TDI]=data[i].courant;
	 label=concat_mbk(in_num,data[i].nom);
	 ppapat = Envoi(ppapat, valeur, mon_malloc(label));
	 mbkfree(label);}
    else ppapat = Envoi(ppapat, valeur, vide);  
    valeur[TCK] = '1';

/*et des sorties*/
    if (data[j].cel!=NULL && (data[j].ancien=='+' || data[j].ancien=='-')
     &&(data[j].cel->donnees[DMODE]==OUTPUT2 
     || data[j].cel->donnees[DMODE]==OUTPUT3
     || data[j].cel->donnees[DMODE]==INOUTPUT)) {
	 			valeur[TDO] = data[j].ancien;
	 			label=concat_mbk(out_num,data[j].nom);
	 			ppapat=Envoi(ppapat, valeur, mon_malloc(label));
	 			mbkfree(label);}
    else ppapat = Envoi(ppapat, valeur, vide);
    j--;
  }/*fin de boucle*/


/*cas particulier du dernier qui doit activer l'automate*/
  CURRENT_BOUNDARY_SCAN_CELL = j;
  valeur[TMS] = '1';                            /* TMS = 1 exit1_dr */
  valeur[TDI] = '0'; 
  switch(data[taille-1].courant){		/*VERIFICATION de SYNCHRONISATION: Le premier bit introduit va sortir */
  	case '0': valeur[TDO] = '-'; break;  	/* Premier cellule en entrée */
 	case '1': valeur[TDO] = '+'; break;
 	default : valeur[TDO] = '-'; break; }	/* Premier cellule en sortie (TDI=0) */
  
  /*la derniere entree*/
  if (data[0].cel!=NULL && (data[0].cel->donnees[DMODE]==INPUT 
  || (data[0].cel->donnees[DMODE]==INOUTPUT 
  && (data[0].courant=='0' || data[0].courant=='1')))) {
       valeur[TDI]=data[0].courant;
       label=concat_mbk(in_num,data[0].nom);
       ppapat = TAP_in(ppapat, valeur,  mon_malloc(label));
       mbkfree(label);}
  else ppapat = TAP_in(ppapat, valeur, Exit1_dr);

  mbkfree(in_num);
  mbkfree(out_num);
  return(ppapat);
}
/****************************************************************************/
/* Fin decalage de la donnee dans le registre Boundary Scan */
static struct papat *EndShiftData(ppapat)
  struct papat *ppapat;
{
  char valeur[6];

  valeur[TCK] = '0';
  valeur[TRST] = '1';
  valeur[TMS] = '1';
  valeur[TDI] = '0';
  valeur[TDO] = '*';
  valeur[5] = '\0';

  ppapat = TAP_in(ppapat, valeur, Update_dr);      /* TMS = 1 update_dr */

  valeur[TMS] = '0';
  ppapat = TAP_in(ppapat, valeur, Run_Test_Idle);        /* TMS = 0 RTI */

  return(ppapat);
}
/****************************************************************************/
/* Initialisation de la table de conversion entre vecteurs paralleles et 
   valeurs des cellules du registre Boundary Scan */ 
static void InitData(struct paseq *pat)
{
  struct paiol *ppaiol;
  Cellules *pcel;
  Infos *inf;
  int i = 0, j = 0;
  int trouve = 0;

  /*taille de toutes les chaines bs concatenees*/
  for (taille=0,inf=InfosVbe; inf;inf=inf->next) {
    /*recherche si cellules utilisées*/
    for (pcel=inf->cel; pcel&&pcel->donnees[DMODE]==UNDEF; pcel=pcel->suivant);
    if (pcel !=NULL && pcel->donnees[DMODE]!=UNDEF) taille=taille+inf->lg_bs;
    else taille=taille+BYPASS_SIZE;
  }
  data = (Data*)mbkalloc(taille * sizeof(Data));

  for(i = 0; i < taille; i++) {
    data[i].ancien = '*';
    data[i].courant = '*';
    data[i].index = UNDEF;
    data[i].cel = NULL;
    data[i].nom= vide;
  }

  i=0;  /* Compteur du nombre de cellules */
  for (inf=InfosVbe;inf;inf=inf->next) {
    /*recherche si cellules utilisées*/
    for (pcel=inf->cel; pcel&&pcel->donnees[DMODE]==UNDEF; pcel=pcel->suivant);
    if (pcel !=NULL && pcel->donnees[DMODE]!=UNDEF) {
      for(pcel = inf->cel; pcel; pcel = pcel -> suivant) {
      j=0;trouve=0;
      for(ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT) {
	if((ppaiol->MODE=='O' || ppaiol->MODE=='I' || ppaiol->MODE=='T') &&
        pcel->nom!=NULL && ppaiol->NAME && !strcmp(pcel->nom,ppaiol->NAME)) {
	     data[i].cel=pcel;
	     data[i].index=j;
	     data[i].nom=del_espace(data[i].cel->nom); /*utilise le namealloc*/
			 trouve=1;
	}
	if (trouve) break;
	j++;  /* Indice pris de la structure PASEQ des PAIOL quand on trouve le meme nom de cellule et connecteur*/
	/* printf("la valeur de j=%d et de data[%d].cel.nom=%s\n",j,i,ppaiol->NAME);*/
      }/*fin de la boucle sur les PAIOL*/
      i++; 
      }
    }/*fin si chaine non-vide*/
    else i++; /*compter le bypass comme une cellule vide*/
  }/*fin de boucle sur Infos*/
}
/****************************************************************************/
/* Affectation des valeurs des cellules du registre Boundary Scan dans la
   structure Data en fonction de la structure de donnees des vecteurs 
   paralleles et de la structure de donnees du fichier informations */
static struct papat *Traduction(papat_parallele, papat, debut)
  struct papat *papat_parallele;
  struct papat *papat;
  int debut;
{
  int i, j;
  struct paevt *ppaevt;
  struct papat *ppapat;
  Cellules *pcel;
  Infos *inf;
  int reconfig=0;                 


/* Pour chaque sequence de n patterns faire*/
for(ppapat=papat_parallele; ppapat; ppapat=ppapat->NEXT) {

/****** partie evaluation ******/        
    /* Pour chaque case de courant */ 
    for(i = 0; i < taille; i++) {
      /*si celle-ci est valide*/
      if (data[i].cel!=NULL) {
        data[i].ancien=data[i].courant;      /* ancien <- courant */
        /*sur l'evenement correspondant a l'index position i dans courant */
        for(ppaevt=ppapat->PAEVT; ppaevt; ppaevt=ppaevt->NEXT)
	     if(ppaevt->INDEX==data[i].index) break;
        if(ppaevt!=NULL) {
	data[i].courant=ValeurData(ppaevt->USRVAL,data[i].cel->donnees[DMODE]
	,data[i].cel->donnees[DINVERSION]);

/* printf("i=%d, mode=%d, datacourant=%c mais paevt->USRVAL=%c\n",i, data[i].cel->donnees[DMODE], data[i].courant,ppaevt->USRVAL);*/

	/*mode special pour les inout sans cellule de controle*/
	if (data[i].cel->donnees[DMODE]==INOUTPUT){ 
	     switch (data[i].courant){
	     case '-':case '+':case '*': 
		if (data[i].ancien=='0' || data[i].ancien=='1') 
		reconfig=1; break;
	     case '0':case '1': 
		if (data[i].ancien=='+' || data[i].ancien=='-' || 
		data[i].ancien=='*') reconfig=1;break;
	     }}

	/*mode special pour les inout avec cellule de controle*/
	else if (data[i].cel->donnees[DMODE]==OUTPUT3) {
	  /* On recherche la cellule qui la controle */
	  for (j=0,inf=InfosVbe; inf; inf=inf->next){
	       for(pcel=inf->cel; pcel&&j!=data[i].cel->donnees[DCONTROLE]
		 ;pcel=pcel->suivant) j++;
	       if (j==data[i].cel->donnees[DCONTROLE]) break;
	  }

          /* verification que la cellule est bien de controle */
	  if (pcel!=NULL && pcel->donnees[DMODE]==CONTROL){ 
	  /*doit etre configurer en sortie*/
	  if(data[i].courant == '+' || data[i].courant == '-')
	       data[j].courant=ValeurData(pcel->donnees[DDEFAUT],
		   INPUT,!pcel->donnees[DINVERSION]);    /*DDEFAUT val de Hiz*/
	  /*doit etre configurer en entree*/
	  else if(data[i].courant == '1' || data[i].courant == '0')
	       data[j].courant=ValeurData(pcel->donnees[DDEFAUT],
		   INPUT,pcel->donnees[DINVERSION]);     /*DDEFAUT val de Hiz*/
	  }/*fin de CONTROL*/
	}/*fin de OUTPUT3*/
	}/*fin du cas: 1 event correspondant a la cellule data[i] est la*/
      }/*fin du cas: cellule valide*/
    }/*fin de boucle sur data[i] */
 

/******* partie chargement *********/
    /*reconfiguration pour les cellules inout de l'emulbs et meme au depart*/
    if(debut || reconfig) {
	 if (emulbs_flag) {
            if(verbose_flag) 
            fprintf(stdout,"###----- cells configured before %d -----####\n",
		    CURRENT_PARALLEL_PATTERN_NUMBER);

	    papat = LoadInst(papat, "config");

	    papat = LoadData(papat);          /*remise a jour de la chaine bs*/
	    papat = configure(papat,data);
	    papat = EndShiftData(papat);/*application en parallele au circuit*/
	 }
      papat = LoadInst(papat, "intest");
      reconfig=0;
      debut=0;
    }

    if(verbose_flag)
      fprintf(stdout,"###----- pattern %d in treatment -----####\n", CURRENT_PARALLEL_PATTERN_NUMBER);

    papat = LoadData(papat);                  /*remise a jour de la chaine bs*/
    papat = ShiftData(papat, data);            /*chargement serie des donnees*/
    papat = EndShiftData(papat);        /*application en parallele au circuit*/

    CURRENT_PARALLEL_PATTERN_NUMBER++;       /*pour message d'erreur*/
    CURRENT_PARALLEL_PATTERN_LINE = ppapat->LINE;
    Old_pat=ppapat;
  }

  return(papat);
}
/****************************************************************************/
/* Creation d'une derniere serie de vecteurs BS du au retard des sorties  */
static struct papat *FinPaseq(ppapat)
struct papat *ppapat;
{
int i;
int valeur_sur_sortie=0;

/*on ne s'occuppe ici que des sorties*/
  for(i = 0; i < taille; i++) {
      /*si celle-ci est valide*/
       if (data[i].cel!=NULL) {
	   if (data[i].cel->donnees[DMODE]==INPUT) data[i].cel=NULL;
	   else {
		data[i].ancien=data[i].courant;         /* ancien <- courant */
		if (data[i].ancien=='+' || data[i].ancien=='-')
		     valeur_sur_sortie=1;
		else data[i].cel=NULL;
	   }}
  }

  if (valeur_sur_sortie) {
  ppapat = LoadData(ppapat);                  /*remise a jour de la chaine bs*/
  ppapat = ShiftData(ppapat);                  /*chargement serie des donnees*/
  ppapat = EndShiftData(ppapat);        /*application en parallele au circuit*/
  }
  

  /*liberation memoire c'est la derniere pattern*/
  mbkfree(data);
return ppapat;
}
/****************************************************************************/
/* Creation de la structure de donnees des vecteurs Boundary Scan */
static struct paseq *DebutPaseq(pat,num_seq)
struct paseq *pat;
int num_seq;
{
  int i = 0;
  char *broches[5];
  struct paiol *ppaiol=NULL, *ppatpaiol=NULL;
  struct papat *ppapat=NULL;
  struct paseq *pat_bs=NULL;


  if (pat==NULL) return NULL;

  broches[TCK]  = namealloc("TCK");
  broches[TRST] = namealloc("TRST");
  broches[TMS]  = namealloc("TMS");
  broches[TDI]  = namealloc("TDI");
  broches[TDO]  = namealloc("TDO");

  pat_bs = pat_addpaseq(NULL);

  pat_bs -> PAIOL = pat_addpaiol(pat_bs -> PAIOL, NULL, NULL, NULL, BS_ESPACE);

/***** chargement des modes pour les 5 broches *****/
  ppaiol = pat_bs -> PAIOL;
  for(i = 0; i < 5; i++) {
    ppaiol -> NAME = broches[i];       /*la reservation memoire a deja ete */
    ppaiol -> FORMAT = 'B';                      /*faite avec namealloc()*/
    if (i == TDO) ppaiol -> MODE = 'O';
    else ppaiol -> MODE = 'I';
    if(i < 4) {
      ppaiol -> NEXT = pat_addpaiol(NULL, NULL, NULL, NULL, BS_ESPACE);
      ppaiol = ppaiol -> NEXT;
    }
    else ppaiol -> NEXT = NULL;
  }

/***** chargement de l'alimentation pour fichier PAT *****/
  if(savePatBS_flag) 
    {
    for(ppatpaiol = pat -> PAIOL; ppatpaiol; ppatpaiol = ppatpaiol -> NEXT) 
      {
      ppatpaiol -> BLANK = BS_ESPACE;			/*espace entre chaque entree*/
      if(isvdd(ppatpaiol -> NAME) || isvss(ppatpaiol -> NAME)) 
        {
	if(ppaiol -> NAME == NULL) 
          {
	  ppaiol -> NAME = ppatpaiol -> NAME;
	  ppaiol -> MODE = 'I';
	  ppaiol -> FORMAT = 'B';
	  }
	else 
          {
	  ppaiol -> NEXT = pat_addpaiol(NULL, NULL, NULL, NULL, BS_ESPACE);
	  ppaiol = ppaiol -> NEXT;
	  ppaiol -> NAME = ppatpaiol -> NAME;
	  ppaiol -> MODE = 'I';
	  ppaiol -> FORMAT = 'B';
	  }
	i++;
       }
    }
  }

  pat_bs->IOLNBR = i;
  


/*creation d'un pointeur sur la pattern BS courante*/
  pat_bs->CURPAT = pat_addpapat(NULL, NULL, NULL);
  ppapat = pat_bs -> CURPAT;
  
/***** les vecteurs BS initialisation *****/
  ppapat = ResetBS(ppapat, pat_bs);
  ppapat = TestTap(ppapat);  
  
/***** traduction vecteurs paralleles *****/
  Old_pat=pat->CURPAT;
  ppapat = Traduction(pat->CURPAT, ppapat, 1);

  if (pat -> ENDFLG=='Y') ppapat=FinPaseq(ppapat);
  /*une en plus pour controler la sortie de la derniere pattern parallele*/

/***** mise a jour des donnees pour pat parallele*****/  
  pat_frepacom(pat->CURCOM);                      /*commentaire debut fichier*/
  pat    -> CURCOM = pat_addpacom(NULL,ENTETE,0); 
  pat    -> DRVSEQ = num_seq-1;                      /*ancienne sequence n-1 */
  pat    -> SUBSEQ = num_seq;       /*sequence courante n puis prochaine n+1 */

/***** mise a jour des donnees pour pat BS*****/  
  pat_bs -> CURCOM = pat_addpacom(NULL,ENTETE,0); /*commentaire debut fichier*/
  if (pat_bs!=NULL) pat_bs -> CURPAT ->SIMFLAG= 'U';
  /*pour le signal -- Beware unprocessed pattern en debut de fichier*/
  pat_bs -> PATNBR = NbPat;        /*nbr de patterns bs dans la sous-sequence*/
  pat_bs -> ENDFLG = pat->ENDFLG;           /*fin de pattern ? non pos encore*/
  pat_bs -> SAVFLG = 'Y';
  pat_bs -> ERRFLG = 0;
  pat_bs -> LINENO = 0;
  pat_bs -> DRVSEQ = num_seq-1;                      /*ancienne sequence n-1 */
  pat_bs -> SUBSEQ = num_seq;       /*sequence courante n puis prochaine n+1 */

return(pat_bs);
}
/****************************************************************************/
/* Creation de la structure de donnees des vecteurs Boundary Scan */
static struct paseq *CorpsPaseq(pat,pat_bs,num_seq)
struct paseq *pat, *pat_bs;
int num_seq;
{
struct papat *ppapat;

  if (pat==NULL) return NULL;
  if (pat->CURPAT==NULL) return NULL;

/*creation d'un pointeur sur la pattern BS courante*/
  pat_bs->CURPAT = pat_addpapat(NULL, NULL, NULL);
  ppapat = pat_bs -> CURPAT;

/***** traduction vecteurs paralleles *****/
  Old_pat=pat->CURPAT;
  ppapat = Traduction(pat->CURPAT->NEXT,ppapat,0);/*premiere pat deja traitee*/
  if (pat -> ENDFLG=='Y') ppapat=FinPaseq(ppapat);
  /*une en plus pour controler la sortie de la derniere pattern parallele*/
  
/***** mise a jour des donnees *****/  
  pat    -> DRVSEQ = num_seq-1;                      /*ancienne sequence n-1 */
  pat    -> SUBSEQ = num_seq;       /*sequence courante n puis prochaine n+1 */
  pat_bs -> PATNBR = NbPat;        /*nbr de patterns bs dans la sous-sequence*/
  pat_bs -> ENDFLG = pat -> ENDFLG;         /*fin de pattern ? non pos encore*/
  pat_bs -> SAVFLG = 'N';
  pat_bs -> ERRFLG = 0;
  pat_bs -> LINENO = 0;
  pat_bs -> DRVSEQ = num_seq-1;                      /*ancienne sequence n-1 */
  pat_bs -> SUBSEQ = num_seq;       /*sequence courante n puis prochaine n+1 */

return(pat_bs);
}
/*****************************************************************************/
/* Creation des vecteurs Boundary Scan en n sequences*/
int traduire(sce, destSRC, destBS, sequence)
char *sce,*destSRC,*destBS;
unsigned int sequence;
{
struct timeb start_tck,end_tck,start_io,end_io,start_prg,end_prg;
unsigned long time_tck=0,time_io=0,time_prg=0;       /* to compute exec time */
int nbr_pat=0;                                   /*nbr de patterns paralleles*/
struct paseq *pat=NULL;                          /* vecteurs paralleles total*/
struct paseq *pat_bs=NULL;                         /* vecteurs Boundary Scan */
unsigned int num_seq=1;              /*decoupage en plusieurs parties des PAT*/

/***INITIALISATION ET COHERENCE***/
  ftime(&start_prg);

  /* Lecture du fichier de patterns 'paralleles' et on les compte*/
  fprintf(stdout,"Loading from %s.pat...",sce);
  fflush(stdout);
  ftime(&start_io);
  if ((nbr_pat=compte_pat(sce))<0) 
    {fprintf(stderr,"Loading failed!\n");return 1;}
  ftime(&end_io);
  time_io+=(end_io.time-start_io.time)*1000 + end_io.millitm-start_io.millitm;

  fprintf(stdout," %d parallel patterns\n",nbr_pat);
  /*allocation minimum memoire sera decidee par utilisateur*/
  init_mem((sequence<nbr_pat&&sequence!=0)?sequence:nbr_pat);
  
  /*l'emulation multiplie par un facteur de l'ordre de 200 la taille des pat      --> mefiance avec la memoire procedure de chargement/dechargement*/
  ftime(&start_io);
  pat = pat_lodpaseq (sce, NULL, sequence);
  ftime(&end_io);
  time_io+=(end_io.time-start_io.time)*1000 + end_io.millitm-start_io.millitm;
  if (pat==NULL || pat->ERRFLG) 
    {fprintf(stderr,"Loading failed!\n");return 1;}
  
  /* Coherence entre le fichier pat et le BSDL */
  if (VerificationsPat(pat)) 
    {fprintf(stderr,"Loading failed!\n");return 1;}
  
  InitData(pat);

CURRENT_PARALLEL_PATTERN_NUMBER=0;
  if (pat->CURPAT!=NULL) CURRENT_PARALLEL_PATTERN_LINE=pat->CURPAT->LINE;
  else CURRENT_PARALLEL_PATTERN_LINE=0;


/***TRADUCTION***/
  if (execute_flag) fprintf(stdout,"Executing patterns on the DUT\n");
  else  
    fprintf(stdout,"Translating pattern from parallel mode to BS mode...\n");
  /*les premieres patterns boundary scan d'initialisation de l'automate*/
  ftime(&start_tck);                                            /*debut timer*/
  pat_bs = DebutPaseq(pat,num_seq);
  ftime(&end_tck);                                                /*fin timer*/
  time_tck += (end_tck.time-start_tck.time) * 1000 + (end_tck.millitm-start_tck.millitm);
  num_seq++;


  /*sauvegarde*/
  if (savePatSRC_flag || savePatBS_flag) {
       if (verbose_flag) fprintf(stdout,"Saving results...\n");
       else {fprintf(stdout,"Saving results...");fflush(stdout);}
       ftime(&start_io);
       if (savePatSRC_flag && pat_savpaseq(destSRC,pat,LABEL_FCL_SIZE)) 
            {fprintf(stderr,"saving file aborted!\n");return 1;}
       if (savePatBS_flag && pat_savpaseq(destBS,pat_bs,LABEL_BS_SIZE)) 
            {fprintf(stderr,"saving file aborted!\n");return 1;}
       ftime(&end_io);
       time_io+=
	    (end_io.time-start_io.time)*1000 + end_io.millitm-start_io.millitm;
  }
  mon_free(); /*liberation des labels de la structure*/

  /*enlever l'entete de debut de fichier*/
  pat_frepacom(pat_bs->CURCOM);                     
  pat_bs->CURCOM=NULL;
  pat_frepacom(pat->CURCOM);                       
  pat->CURCOM=NULL;


/***CONTINUER LA TRADUCTION***/
  /*si le fichier pat parallele est long refaire un cycle*/
  while (pat!=NULL && pat->ENDFLG!='Y') {
    if (verbose_flag) {
	 fprintf(stdout,"%.1f%%   %d bs patterns, continuing...\n",
		 (float)((float)(CURRENT_PARALLEL_PATTERN_NUMBER*100))/nbr_pat,
		 pat_bs->PATNBR);}
    else {
	 fprintf(stdout,"\r%.1f%%   %d bs patterns, continuing...",
		 (float)((float)(CURRENT_PARALLEL_PATTERN_NUMBER*100))/nbr_pat,
		 pat_bs->PATNBR);
	 fflush(stdout);}

    /*la fct lodpaseq retourne toujours la pattern precedente suivie */
    /*des courantes*/
    ftime(&start_io);
    pat = pat_lodpaseq (sce, pat, sequence);
    ftime(&end_io);
    time_io+=(end_io.time-start_io.time)*1000 +end_io.millitm-start_io.millitm;
    if (pat==NULL || pat->ERRFLG) 
      {fprintf(stderr,"Loading failed!\n");return 1;}

    /*traduction*/
    if (savePatBS_flag) pat_frepapat(pat_bs->CURPAT);
    pat_bs->CURPAT=NULL;
    ftime(&start_tck);                                          /*debut timer*/
    pat_bs = CorpsPaseq(pat,pat_bs,num_seq);
    ftime(&end_tck);                                              /*fin timer*/
    time_tck +=
      (end_tck.time-start_tck.time)*1000 + (end_tck.millitm-start_tck.millitm);
    num_seq++;
    
    /*sauvegarde*/
    if (savePatSRC_flag || savePatBS_flag) {
	 ftime(&start_io);
	 if (savePatSRC_flag && pat_savpaseq(destSRC,pat,LABEL_FCL_SIZE)) 
	      {fprintf(stderr,"saving file aborted!\n");return 1;}
	 if (savePatBS_flag && pat_savpaseq(destBS,pat_bs,LABEL_BS_SIZE)) 
	      {fprintf(stderr,"saving file aborted!\n");return 1;}
	 ftime(&end_io);
	 time_io+=
	    (end_io.time-start_io.time)*1000 +end_io.millitm-start_io.millitm;
    }
    mon_free();/*liberation des labels de la structure*/
  }



/***AFFICHAGE STATISTIQUE***/
  /*nbr de patterns*/
  if (execute_flag) 
       fprintf(stdout,"\r%d boundary scan patterns processed",pat_bs->PATNBR);
  else if (savePatBS_flag) 
       fprintf(stdout,"\r%d boundary scan patterns saved in %s.pat",pat_bs->PATNBR,destBS);
  fflush(stdout);

  /*verification*/
  if (EXECUTION_ERRORS == 0) 
     fprintf(stdout," ----------> NO error\n");
  else if (EXECUTION_ERRORS == 1) 
     fprintf(stdout," ----------> One ERROR\n");
  else fprintf(stdout," ----------> %d ERRORS\n", EXECUTION_ERRORS);

  /*temps*/
  ftime(&end_prg);
  time_prg=
       (end_prg.time-start_prg.time)*1000 + end_prg.millitm-start_prg.millitm;
  affiche_time(time_prg);
  fprintf(stdout,"   but only ");fflush(stdout);
  affiche_time(time_tck);
  fprintf(stdout," for vectors --> ");fflush(stdout);

  /*frequence --> seuls les fronts d'horloge*/
  if (time_tck!=0) time_tck=(pat_bs->PATNBR*500)/time_tck;
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


/****nettoyage memoire****/
erase_paseq(pat_bs);
erase_paseq(pat);
return 0;
}
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/













