/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
#include "global.h"

extern FILE *emul_in;                        /*communication avec le emul.yac*/
extern FILE *pga_in;                          /*communication avec le pga.yac*/

extern Infos *InfosVbe;				  /*resultat final du parseur*/
int FLAG_ERR=0;				          /*si une erreur est arrivee*/
int NBR_LIGNES=1;				      /*pour message d'erreur*/


Cellules *EMUL[PCBS_NBR_EMULBS*NBR_CELLS];            /*lien entre n° et EMUL*/
ht *HSH_DATA;	    			             /*lien entre PGA et EMUL*/
ht *HSH_VSS;				              /*lien entre PGA et Vss*/
ht *HSH_VDD;				              /*lien entre PGA et Vdd*/

extern void pga_parse();
extern void emul_parse();

 
/* charge le fichier de description de la chaine boundary scan
carte_emul--> nom fichier connexions PGA / EMULBS  
 et retourne...
   0 ---> pas d'erreur le resultat est dans la variable globale InfosVbe 
   (cf. globale.h)
  -1 ---> erreur de chargement du fichier emul
  -2 ---> erreur de chargement du fichier pga
   1 ---> warnings*/
int description_cartes(char *emul,char *pga)
{
     emul_in=fopen(emul,"r");
     if (ferror(emul_in)!=0) {
	  fprintf(stderr,"Unable to load %s\n",emul);
	  return -1;
     }

     /*creer les n EMULBS31 avec des donnees vides*/
     creer_infos();
     HSH_DATA=(ht*)addht(PCBS_NBR_EMULBS*NBR_CELLS);
     HSH_VDD=(ht*)addht(NBR_LIGNES_PGA*NBR_COLONNES_PGA-PCBS_NBR_EMULBS*NBR_CELLS);
     HSH_VSS=(ht*)addht(NBR_LIGNES_PGA*NBR_COLONNES_PGA-PCBS_NBR_EMULBS*NBR_CELLS);
     fprintf(stdout,"Loading file %s...", emul);
     fflush(stdout);
     emul_parse();    /*appel a LEX&YACC->carte_emul.yac,carte_emul.lex*/
     fclose(emul_in);
     if (FLAG_ERR<0) 
     {delht(HSH_DATA);delht(HSH_VDD);delht(HSH_VSS);return -1;}

     pga_in=fopen(pga,"r");
     if (ferror(pga_in)!=0) {
	  fprintf(stderr,"Unable to load %s\n",pga);
	  delht(HSH_DATA);delht(HSH_VDD);delht(HSH_VSS);
	  return -2;
     }

     fprintf(stdout,"Loading file %s...", pga);
     fflush(stdout);
     pga_parse();    /*appel a LEX&YACC->carte_emul.yac,carte_emul.lex*/
     fclose(pga_in);
     delht(HSH_DATA);delht(HSH_VDD);delht(HSH_VSS);
     if (FLAG_ERR<0) return -2;

return FLAG_ERR;
}



/********************************* InfosVbe **********************************/

/* affichage de la variable globale InfosVbe*/
void affich_infos()
{
Infos *inf=InfosVbe;          /*variable globale  cf. pcbs.h*/
Instructions *ins;
Codes *cod;
Cellules *cell;
int i=0;
while (inf!=NULL){
     fprintf(stdout,"\n************************** boundary scan chain n°%d **************************\n",i++);
     fprintf(stdout,"instruction register length=%d\t",inf->lg_inst);
     ins=inf->inst;
     while (ins!=NULL) {
	  fprintf(stdout,"%s( ",ins->nom);
	  cod=ins->code;
	  while (cod!=NULL) {
	       fprintf(stdout,"%s ",cod->valeur);
	       cod=cod->suivant;
	  }
	  fprintf(stdout,")  ");
	  ins=ins->suivant;
     }
     fprintf(stdout,"\n\n");
     cell=inf->cel;
     while (cell!=NULL) {
	  switch (cell->donnees[DMODE]) {
	  case OUTPUT2:case OUTPUT3:fprintf(stdout,"out %s; ",cell->nom);break;
	  case INPUT:fprintf(stdout,"in %s; ",cell->nom);break;
	  case INOUTPUT:fprintf(stdout,"inout %s; ",cell->nom);break;
	  }
	  cell=cell->suivant;
     }
     fprintf(stdout,"\n");
     inf=inf->next;
}
fprintf(stdout,"\n");
}




/* suppression en memoire de la variable globale InfosVbe*/
void del_infos()
{
Infos *inf2,*inf=InfosVbe;          /*variable globale  cf. pcbs.h*/
Instructions *ins,*ins2;
Codes *cod,*cod2;
Cellules *cell,*cell2;

while (inf!=NULL){
     ins=inf->inst;
     while (ins!=NULL) {
	  cod=ins->code;
	  while (cod!=NULL) {
	       cod2=cod;
	       cod=cod->suivant;
	       mbkfree(cod2);
	  }
	  ins2=ins;
	  ins=ins->suivant;
	  mbkfree(ins2);
     }
     cell=inf->cel;
     while (cell!=NULL) {
	  cell2=cell;
	  cell=cell->suivant;
	  mbkfree(cell2);
     }
     inf2=inf;
     inf=inf->next;
     mbkfree(inf2);
}
InfosVbe=NULL;
}





/********** allocation memoire pour la variable globale InfosVbe *************/

/*construire la structure INSTRUCTIONS acceptees par EMUL31*/
static Instructions *creer_inst()
{
/*bypass*/
	Instructions *j,*i=(Instructions*) mbkalloc(sizeof(Instructions));
	if (i==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	i->nom=(char*)namealloc("BYPASS");
	i->code=(Codes*) mbkalloc(sizeof(Codes));
	if (i->code==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	i->code->valeur=(char*) namealloc(_BYPASS0);
	i->code->suivant=(Codes*) mbkalloc(sizeof(Codes));
	if (i->code->suivant==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	i->code->suivant->valeur=(char*) namealloc(_BYPASS1);
	i->code->suivant->suivant=NULL;

/*intest*/
	i->suivant=(Instructions*) mbkalloc(sizeof(Instructions));
	j=i->suivant;
	if (j==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	j->nom=(char*)namealloc("INTEST");
	j->code=(Codes*) mbkalloc(sizeof(Codes));
	if (j->code==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	j->code->valeur=(char*) namealloc(_INTEST);
	j->code->suivant=NULL;

/*config*/
	j->suivant=(Instructions*) mbkalloc(sizeof(Instructions));
	j=j->suivant;
	if (j==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	j->nom=(char*)namealloc("CONFIG");
	j->code=(Codes*) mbkalloc(sizeof(Codes));
	if (j->code==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	j->code->valeur=(char*) namealloc(_CONFIG);
	j->code->suivant=NULL;

	j->suivant=NULL;

return i;	
}



/*construire toutes les cellules des chaines BS des n EMUL31 
---> resultat dans la variable globale TAB_EMUL[][] */
static Cellules* creer_cellules()
{
static int i=0; /*pour le rangement dans la variable globale EMUL[] */
int taille=sizeof(Cellules);
int j;
Cellules *tab[NBR_CELLS];
char *nom_cell=(char*) namealloc("*");

for (j=0;j<NBR_CELLS;j++) {	/*nbr de cells d'une chaine BS */
	tab[j]=(Cellules*) mbkalloc(taille);
	if (tab[j]==NULL) {
		fprintf(stderr,"Not enough memory!"); 
		exit(1);
	}
	if (j>0) tab[j-1]->suivant=tab[j];
	tab[j]->nom= nom_cell;  
	/*le premier n'a pas de precedent*/
	tab[j]->donnees[DINVERSION]= 0;  
	/*0:  pas d'inversion      1:  inversion*/
	tab[j]->donnees[DCONTROLE]= UNDEF; 
	/*UNDEF=-1: la cell n'est pas de controle*/
	/*n°: index de la cellule controlee*/
	tab[j]->donnees[DDEFAUT]= UNDEF;  
	/*UNDEF=-1: la cell n'est pas de controle    n°: valeur par defaut*/
	tab[j]->donnees[DCOMMANDE]= UNDEF;  
	/*UNDEF=-1: la cell n'est pas controlee*/
	/*0 ou 1: valeur de la commande active*/
	tab[j]->donnees[DMODE]= UNDEF;  
	/* UNDEF-->cellule non-active qui sera 'enlevee' de la chaine*/
	/* cf.  global.h  et output.c*/
	EMUL[i++]=tab[j];
	/* chargement en variable globale dans EMUL[] */
}
tab[j-1]->suivant=NULL;      /*le dernier n'a pas de suivant*/
return tab[0];
}



/*creer les n EMUL31 chaines*/
void creer_infos()
{
Infos *inf[PCBS_NBR_EMULBS];
int i;

for (i=0;i<PCBS_NBR_EMULBS;i++) {	/*faire une chaine de n EMUL31*/

	inf[i]=(Infos*)mbkalloc(sizeof(Infos));
	if (inf[i]==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	inf[i]->lg_inst=INS_LEN;   	 
	/*taille du registre instruction de la carte EMULBS 31*/
	inf[i]->inst=creer_inst();	/*liste des instructions acceptees*/
	inf[i]->cel=creer_cellules();	/*chaine boundary-scan*/
	inf[i]->lg_bs=NBR_CELLS;		/*longueur de la chaine*/
	if (i>0) inf[i-1]->next=inf[i];	/*le premier n'a pas de precedent*/
}
inf[i-1]->next=NULL;			/*le dernier n'a pas de suivant*/
InfosVbe= inf[0];
}
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/




