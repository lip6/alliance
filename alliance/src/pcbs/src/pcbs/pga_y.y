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
/***************** parseur etablissant les connections ***********************/
/************************ entre le PGA et le chip ****************************/
/*									     */
/*		 parametres: FILE *yyin 				     */
/*		 	     ht *HSH_EMUL,*HSH_VDD,*HSH_VSS		     */
/*		 resultat :  Infos *InfosVbe 				     */
/*		 erreur :    int FLAG_ERR 				     */
/*									     */
/*****************************************************************************/
/*
 --exemple for chip_connexions
 --left-hand is for PGA connector
 --right-hand is for CHIP connector
 pcbs chip_connexions version 2.0;
 a:1 <=in     ck  ;
 r:16<=out    y   ;
 d:4 <=in     i(0);
 d:5 <=in     i[1];
 g:2 <=inout  sh  ;
 e:6 <=in   d_Vdd ;
 a:2 <=in   d_Vss ;
*/
%{

#include "global.h"

extern int yylex();				     /*fct du lexeur pga.lex*/
extern Infos *InfosVbe;				 /*resultat final du parseur*/
extern int FLAG_ERR;				 /*si une erreur est arrivee*/
extern NBR_LIGNES;				     /*pour message d'erreur*/

extern ht *HSH_DATA;	    			     /*lien entre PGA et EMUL*/
extern ht *HSH_VSS;				      /*lien entre PGA et Vss*/
extern ht *HSH_VDD;				      /*lien entre PGA et Vdd*/

static ht *HSH_PGA_USED;		     /*connecteurs PGA deja utilises*/
static ht *HSH_CHIP_USED;		    /*broches du chip deja affectees*/

void pga_flag(int x);

%}

%union {
	int val;
	float flottant;
	char *mot;
	char lettre;
}


%token _PCBS
%token Version
%token Num_version
%token _IN 
%token _OUT 
%token _INOUT 
%token Pt_virg
%token Db_pt
%token Assign
%token Index
%token Nbr
%token Mot
%token Lettre
%token Par_ouv Par_fer
%token Cro_ouv Cro_fer
%type <mot> Index
%type <mot> Mot
%type <lettre> Lettre
%type <val> Nbr
%type <val> type
%type <mot> index .index.
%type <flottant> Num_version
%start fichier

%%

fichier
	:
{
/*initialisation*/
NBR_LIGNES=1;
FLAG_ERR=0;
}
	header
{
HSH_CHIP_USED=addht(PCBS_NBR_EMULBS*NBR_CELLS);
HSH_PGA_USED=addht(NBR_LIGNES_PGA*NBR_COLONNES_PGA);
}
	..declarations..
{
/*liberation memoire*/
delht(HSH_CHIP_USED);
delht(HSH_PGA_USED);
}
;



declaration
	:Lettre
	Db_pt
	Nbr
	Assign
	type
	Mot
	.index.
	Pt_virg
{
char *chip=(char*)concat_espace($6,$7);        /*avec un espace*/
char *pga=(char*)concat(ctoa($1),itoa($3));        
Cellules *cell;

/****** test du PGA ******/
/* ligne PGA incorrecte */
if ($1<LETTRE_DEB_PGA || $1>LETTRE_FIN_PGA || $1=='i' || $1=='o') {fprintf(stderr,"%d: letter '%c' for a line is forbidden!\n",NBR_LIGNES,$1);pga_flag(-2);break;}

/* colonne PGA incorrecte */
if ($3<NBR_DEB_PGA || $3>NBR_FIN_PGA) {fprintf(stderr,"%d: number '%d' for a column is forbidden!\n",NBR_LIGNES,$3);pga_flag(-2);break;}
 
/* test de double utilisation d'une broche PGA */
if (gethtitem(HSH_PGA_USED,pga)!=EMPTYHT) {fprintf(stderr,"%d: PGA connector line %c column %d already used!\n",NBR_LIGNES,$1,$3);pga_flag(-2);break;}
	else addhtitem(HSH_PGA_USED,pga,1);        /*attention EMPTYHT=-1!!!!*/


/***** test du CHIP *****/
/* alimentation Vdd du CHIP */
if (isvdd(chip)) { 
	if (gethtitem(HSH_VDD,pga)==EMPTYHT) {fprintf(stderr,"%d: power %s isn't connected to Vdd!\n",NBR_LIGNES,chip);pga_flag(-1);break;}
	else addhtitem(HSH_CHIP_USED, chip, 1);
}

/* alimentation Vss du CHIP*/
else if (isvss(chip)) {
	if (gethtitem(HSH_VSS,pga)==EMPTYHT) {fprintf(stderr,"%d: power %s isn't connected to Vss!\n",NBR_LIGNES,chip);pga_flag(-1);break;}
	else addhtitem(HSH_CHIP_USED, chip, 1);    /*attention EMPTYHT=-1!!!!*/
}

/* les datas du CHIP */
else if ((cell=(Cellules*)gethtitem(HSH_DATA,pga))==(Cellules*)EMPTYHT) {fprintf(stderr,"%d: data %s isn't connected to the emulator!\n",NBR_LIGNES,chip);pga_flag(-1);break;}
     else {
	addhtitem(HSH_CHIP_USED, chip, 1); /*attention EMPTYHT=-1!!!!*/
	cell->donnees[DMODE]=$5;
	cell->nom=chip;
	}
}
	|error			/*gestion des erreurs*/
{/*synchronisation avec point virgule ou fin de fichier*/
while (yychar!=Pt_virg && yychar!=0) yychar=yylex(); 
yychar=yylex();
yyerrok;}
;





header
	:_PCBS
	Mot
	Version
	Num_version
	Pt_virg
{
char c;
if (strcmp($2,PGA_FILE_NAME)) {
 fprintf(stderr,"%s doesn't recognise it\nDo you want to continue? (y/n) ",PROGRAM_NAME);
 fflush(stderr);                                         /*affichage immediat*/
 fseek(stdin,0,SEEK_END);                    /*enlever les anciens caracteres*/
 c=getc(stdin);
 getc(stdin);
 if (c!='y' && c!='Y') {pga_flag(-3);return -1;}
 pga_flag(3);
}
if (FLAG_ERR==0 && $4>atof(PCBS_VERSION)) {
 fprintf(stderr,"You use an old version of %s\nDo you want to continue? (y/n) ",PROGRAM_NAME);
 fflush(stderr);                                         /*affichage immediat*/
 fseek(stdin,0,SEEK_END);                    /*enlever les anciens caracteres*/
 c=getc(stdin);
 getc(stdin);
 if (c!='y' && c!='Y') {pga_flag(-4);return -1;}
 pga_flag(4);
}
if (FLAG_ERR==0) fprintf(stdout,"\n");           /*coherence dans l'affichage*/
}
	|error			/*gestion des erreurs*/
{/*synchronisation avec point virgule ou fin de fichier*/
char c;
 while (yychar!=Pt_virg && yychar!=0) yychar=yylex(); 
 yychar=yylex();
 yyerrok;
/*demande d'arret*/
 fprintf(stderr,"%s doesn't recognise this file\nDo you want to continue? (y/n) ",PROGRAM_NAME);
 fflush(stderr);                                         /*affichage immediat*/
 fseek(stdin,0,SEEK_END);                    /*enlever les anciens caracteres*/
 c=getc(stdin);
 getc(stdin);
 if (c!='y' && c!='Y') {emul_flag(-3);return -1;}
 FLAG_ERR=3;
}
;

..declarations..
	:
	|declaration ..declarations..
;

.index.
	:index			{$$=$1;}
	|/*empty*/		{$$=NULL;}
;

index:
	Par_ouv
	Index
	Par_fer
{$$=$2;}
	|Cro_ouv
	Index
	Cro_fer
{$$=$2;}
;

type
	:_IN			{$$=INPUT;}
	|_OUT			{$$=OUTPUT2;}
	|_INOUT			{$$=INOUTPUT;}
;

%%



/**************************** change le FLAG_ERR *****************************/
void pga_flag(int x)
{
/*sauvegarde de la premiere erreur ou du dernier warning*/
if (FLAG_ERR>=0) FLAG_ERR=x; 
}
/****************** traitement des erreurs de syntaxe ************************/
int yyerror()
{
fprintf(stderr,"%d: syntax error",NBR_LIGNES);
switch ((int)yychar) {
	case Mot: 	fprintf(stderr," append at '%s'\n",yylval.mot);break;
	case Lettre: 	fprintf(stderr," append at '%s'\n",yylval.mot);break;
	case Index: 	fprintf(stderr," append at '%s'\n",yylval.mot);break;
	case Nbr: 	fprintf(stderr," append at '%d'\n",yylval.val);break;
	case Pt_virg:	fprintf(stderr," append at ';'\n");break;
	case Assign:	fprintf(stderr," append at '<='\n");break;
	case Db_pt:	fprintf(stderr," append at ':'\n");break;
	case Cro_ouv:	fprintf(stderr," append at '['\n");break;
	case Cro_fer:	fprintf(stderr," append at ']'\n");break;
	case Par_ouv:	fprintf(stderr," append at '('\n");break;
	case Par_fer:	fprintf(stderr," append at ')'\n");break;
	case _IN:	fprintf(stderr," append at 'in'\n");break;
	case _OUT:	fprintf(stderr," append at 'out'\n");break;
	case _INOUT:	fprintf(stderr," append at 'inout'\n");break;
	case _PCBS:	fprintf(stderr," append at 'pcbs'\n");break;
	case Version:	fprintf(stderr," append at 'Version'\n");break;
        case Num_version:fprintf(stderr," append at '%.1f'\n",yylval.flottant);break;
	case 0:		fprintf(stderr,"  ';' is missing\n");break;/*EOF*/
	default:	fprintf(stderr," for '%c' not allowed!\n",yychar);
}
pga_flag(-1);
return 1;
}
/*****************************************************************************/


/**************************** END of LEX&YACC ********************************/
/*****************************************************************************/









