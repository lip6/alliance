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
/*************** entre le PGA et les interfaces EMULBS 31 ********************/
/*									     */
/*		 parametres : FILE *yyin				     */
/*			      Infos *InfosVbe				     */
/*		 resultats:   ht *HSH_DATA,*HSH_VDD,*HSH_VSS		     */
/*		 erreur :     int FLAG_ERR 				     */
/*									     */
/*****************************************************************************/
/*
 --exemple for emul_connexions     
 --left-hand is for EMULBS connector
 --right-hand is for PGA connector 
 pcbs emul_connexions V2.0;        
 0    <=a:1;                        
 185  <=r:16;                   
 56   <=d:5;                      
 76   <=d:4;                      
 80   <=g:2;                      
 Vdd  <=e:6;                      
 d_Vss<=a:2;                      
*/

%{

#include "global.h"


extern int yylex();				     /*fct du lexeur emul.lex*/
extern Infos *InfosVbe;				  /*resultat final du parseur*/
extern int FLAG_ERR;				  /*si une erreur est arrivee*/
extern int NBR_LIGNES;			     	      /*pour message d'erreur*/

extern Cellules *EMUL[PCBS_NBR_EMULBS*NBR_CELLS];   	      /*lien entre n° et EMUL*/
extern ht *HSH_DATA;	    			     /*lien entre PGA et EMUL*/
extern ht *HSH_VSS;				      /*lien entre PGA et Vss*/
extern ht *HSH_VDD;				      /*lien entre PGA et Vdd*/

static ht *HSH_PGA_USED;		      /*connecteurs PGA deja affectes*/
static ht *HSH_EMUL_USED;			/*broches emul deja utilisees*/

void emul_flag(int x);

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
HSH_PGA_USED=addht(NBR_LIGNES_PGA*NBR_COLONNES_PGA);
HSH_EMUL_USED=addht(PCBS_NBR_EMULBS*NBR_CELLS);
}

	..declarations..
{
/*liberation memoire*/
delht(HSH_PGA_USED);
delht(HSH_EMUL_USED);
}
;


declaration
	:alimentation
	|donnee
	|error			/*gestion des erreurs*/
{/*synchronisation avec point virgule ou fin de fichier*/
while (yychar!=Pt_virg && yychar!=0) yychar=yylex(); 
yychar=yylex();
yyerrok;}
;

alimentation
	:Mot
	.index.
	Assign
	Lettre
	Db_pt
	Nbr
	Pt_virg
{
char *pga=(char*)concat(ctoa($4),itoa($6));        

/****** test du PGA *******/
/*ligne PGA incorrecte*/
if ($4<LETTRE_DEB_PGA || $4>LETTRE_FIN_PGA || $4=='i' || $4=='o') {fprintf(stderr,"%d: letter '%c' for a line is forbidden!\n",NBR_LIGNES,$4);emul_flag(-2);break;}

/*colonne PGA incorrecte*/
if ($6<NBR_DEB_PGA || $6>NBR_FIN_PGA) {fprintf(stderr,"%d: number '%d' for a column is forbidden!\n",NBR_LIGNES,$6);emul_flag(-2);break;}

/* connecteur PGA deja utilise */
if (gethtitem(HSH_PGA_USED,pga)!=EMPTYHT) {fprintf(stderr,"%d: connector PGA %c %d already used!\n",NBR_LIGNES,$4,$6);emul_flag(-2);break;}
	else addhtitem(HSH_PGA_USED,pga, 1);	


/****** alimentation ******/
if (isvss($1)) addhtitem(HSH_VSS, pga, 1);         /*attention EMPTYHT=-1!!!!*/

else if (isvdd($1)) addhtitem(HSH_VDD, pga, 1);    /*attention EMPTYHT=-1!!!!*/

else {fprintf(stderr,"%d: left-hand connector %s must be Vdd/Vss or n° between 0 and %d!\n",NBR_LIGNES,$1,PCBS_NBR_EMULBS*NBR_CELLS-1);emul_flag(-2);break;}

}
;


donnee
	:Nbr
	Assign
	Lettre
	Db_pt
	Nbr
	Pt_virg
{
char *pga=(char*)concat(ctoa($3),itoa($5));        

/****** les EMULBS *******/
/* il y a un nombre maximum de connecteurs EMUL */
if ($1>=PCBS_NBR_EMULBS*NBR_CELLS || $1<0) {fprintf(stderr,"%d: EMULBS31 n°%d doesn't exist!   Index must be between 0 and %d\n",NBR_LIGNES,$1,PCBS_NBR_EMULBS*NBR_CELLS-1);emul_flag(-1);break;}

/* connecteur EMUL  deja utilise */
if (gethtitem(HSH_EMUL_USED,EMUL[$1])!=EMPTYHT) {fprintf(stderr,"%d: connector n°%d already used!\n",NBR_LIGNES,$1);emul_flag(-2);break;}
	else addhtitem(HSH_EMUL_USED,EMUL[$1], 1);	


/****** test du PGA *******/
/*ligne PGA incorrecte*/
if ($3<LETTRE_DEB_PGA || $3>LETTRE_FIN_PGA || $3=='i' || $3=='o') {fprintf(stderr,"%d: letter '%c' for a line is forbidden!\n",NBR_LIGNES,$3);emul_flag(-2);break;}

/*colonne PGA incorrecte*/
if ($5<NBR_DEB_PGA || $5>NBR_FIN_PGA) {fprintf(stderr,"%d: number '%d' for a column is forbidden!\n",NBR_LIGNES,$5);emul_flag(-2);break;}

/* connecteur PGA deja utilise */
if (gethtitem(HSH_PGA_USED,pga)!=EMPTYHT) {fprintf(stderr,"%d: connector PGA %c %d already used!\n",NBR_LIGNES,$3,$5);emul_flag(-2);break;}
	else addhtitem(HSH_PGA_USED,pga, 1);	

/******* chargement des donnees ******/
addhtitem(HSH_DATA, pga, (int)EMUL[$1]);
}
;




header
	:_PCBS
	Mot
	Version
	Num_version
	Pt_virg
{
char c;
if (strcmp($2,EMUL_FILE_NAME)) {
 fprintf(stderr,"%s doesn't recognise it\nDo you want to continue? (y/n) ",PROGRAM_NAME);
 fflush(stderr);                                         /*affichage immediat*/
 fseek(stdin,0,SEEK_END);                    /*enlever les anciens caracteres*/
 c=getc(stdin);
 getc(stdin);
 if (c!='y' && c!='Y') {emul_flag(-3);return -1;}
 emul_flag(3);
}
if (FLAG_ERR==0 && $4>atof(PCBS_VERSION)) {
 fprintf(stderr,"You use an old version of %s\nDo you want to continue? (y/n) ",PROGRAM_NAME);
 fflush(stderr);                                         /*affichage immediat*/
 fseek(stdin,0,SEEK_END);                    /*enlever les anciens caracteres*/
 c=getc(stdin);
 getc(stdin);
 if (c!='y' && c!='Y') {emul_flag(-4);return -1;}
 emul_flag(4);
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
	:/*empty*/
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

%%



/**************************** change le FLAG_ERR *****************************/
void emul_flag(int x)
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
emul_flag(-1);
return 1;
}
/*****************************************************************************/



/**************************** END of LEX&YACC ********************************/
/*****************************************************************************/








