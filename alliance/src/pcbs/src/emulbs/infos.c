/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##  Authors            : Francois DONNET (2001)                     ##*/
/*##                                                                  ##*/
/*##  E-mail support:    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/


#include "global.h"
#include "emul31.h"


/************************************************************************/

extern FILE *emul_in;                        /*communication avec le emul.yac*/

Infos *InfosVbe = NULL;				  /*resultat final du parseur*/


/************************************************************************/

extern int emul_parse();

/************************************************************************/
/************************************************************************/
 


/************************************************************************/
/********** allocation memoire pour la variable globale InfosVbe *************/
/*construire la structure INSTRUCTIONS acceptees par EMUL31*/
/************************************************************************/
static Instructions *creer_INST()
{
/*bypass*/
	Instructions *i0,*i1,*i2,*i3;
   
   i0 = (Instructions*) mbkalloc(sizeof(Instructions));
   i1 = (Instructions*) mbkalloc(sizeof(Instructions));
   i2 = (Instructions*) mbkalloc(sizeof(Instructions));
   i3 = (Instructions*) mbkalloc(sizeof(Instructions));
	
   i0->NAME  = namealloc(BYPASS_NAME);
	i0->VALUE = namealloc(EMULBS_BYPASS0_VALUE);
	
	i1->NAME  = namealloc(BYPASS_NAME);
	i1->VALUE = namealloc(EMULBS_BYPASS1_VALUE);
   
	i2->NAME  = namealloc(INTEST_NAME);
	i2->VALUE = namealloc(EMULBS_INTEST_VALUE);
   
	i3->NAME  = namealloc(CONFIG_NAME);
	i3->VALUE = namealloc(EMULBS_CONFIG_VALUE);
   
   i0->NEXT  = i1;
   i1->NEXT  = i2;
   i2->NEXT  = i3;
   i3->NEXT  = NULL;

   return i0;	
}



/************************************************************************/
/*
construire toutes les cellules des chaines BS des n EMUL31 
*/
/************************************************************************/
static Cellules* creer_cellules()
{
int j;
Cellules *cell;
Cellules *last_cell = NULL;

for (j=1; j<=EMULBS_BS_SIZE; j++) 
{	
   /*nbr de cells d'une chaine BS */
	cell = (Cellules*) mbkalloc(sizeof(Cellules));
	
   cell->NEXT = last_cell;
	last_cell  = cell;
   
	cell->NAME    = NULL;  
   
   /*0:  pas d'inversion      1:  inversion*/
	cell->INVERT  = 0;  
	
	/*index de la cellule qui controle*/
	cell->COMMAND_CELL  = NULL; 
	cell->COMMAND_VALUE = '1'; 
	
   /*pas de court-circuit*/
	cell->MODE    = MODE_OUT;  
}

return cell;
}



/************************************************************************/
/*creer les n EMUL31 chaines*/
/************************************************************************/
extern void creer_InfosVbe_EMULBS()
{
Infos *last_inf = NULL;
int i;

  for (i=1; i<=EMULBS_NBR_EMULBS; i++) {	/*faire une chaine de n EMUL31*/
    
    InfosVbe = (Infos*)mbkalloc(sizeof(Infos));
    
    InfosVbe->INST_SIZE = EMULBS_INST_SIZE;   
    
    /*taille du registre INSTruction de la carte EMULBS 31*/
    InfosVbe->INST    = creer_INST();	/*liste des INSTructions acceptees*/
    InfosVbe->CELL    = creer_cellules();	/*chaine boundary-scan*/
    InfosVbe->BS_SIZE = EMULBS_BS_SIZE;		/*longueur de la chaine*/
    InfosVbe->NEXT    = last_inf;
    InfosVbe->BYPASS_SIZE = EMULBS_BYPASS_SIZE;
    last_inf          = InfosVbe;
  }

}


/************************************************************************/
/* affichage de la variable globale InfosVbe*/
/************************************************************************/
extern void affich_infos()
{
Infos *inf=InfosVbe;          /*variable globale  cf. pcbs.h*/
Instructions *ins;
Cellules *cell;
int i=0;

  while (inf!=NULL){
     fprintf(stdout,"\n************************** boundary scan chain n°%d **************************\n",i++);
     fprintf(stdout,"INSTruction register length=%d\t",inf->INST_SIZE);
     ins=inf->INST;
     while (ins!=NULL) {
	  fprintf(stdout,"%s(",ins->NAME);
	       fprintf(stdout,"\"%s\"",ins->VALUE);
	  fprintf(stdout,")  ");
	  ins=ins->NEXT;
     }
     fprintf(stdout,"\n\n");
     cell=inf->CELL;
     while (cell!=NULL && cell->NAME) {
	  switch (cell->MODE) {
	  case MODE_OUT:case MODE_TRISTATE:fprintf(stdout,"out %s; ",cell->NAME);break;
	  case MODE_IN:fprintf(stdout,"in %s; ",cell->NAME);break;
	  case MODE_INOUT:fprintf(stdout,"inout %s; ",cell->NAME);break;
	  case MODE_COMMAND:fprintf(stdout,"command %s; ",cell->NAME);break;
     }
	  cell=cell->NEXT;
     }
     fprintf(stdout,"\n");
     inf=inf->NEXT;
  }
fprintf(stdout,"\n");
}


/************************************************************************/
/*
charge le fichier de description de la chaine boundary scan
carte_emul--> nom fichier connexions PGA / EMULBS  
*/
/************************************************************************/
extern void description_cartes(char *emul)
{
     fprintf(stdout,"Loading file %s...\n", emul);
     
     emul_in = fopen( emul, "r" );
     
     if (!emul_in || ferror(emul_in)) 
     {
	    fprintf(stderr,"Unable to load %s\n",emul);
	    exit(1);
     }

     /*creer les n EMULBS31 avec des donnees vides*/
     emul_parse();    /*appel a LEX&YACC->carte_emul.yac,carte_emul.lex*/
     fclose(emul_in);
}






/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/




