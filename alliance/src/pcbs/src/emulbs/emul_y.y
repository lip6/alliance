/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##  Authors            : Francois DONNET (2001)                     ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
/***************** parseur etablissant les connections ***********************/
/*************** entre le PGA et les interfaces EMULBS 31 ********************/
/*									     */
/*		 parametres : FILE *yyin				     */
/*			      Infos *InfosVbe				     */
/*		 erreur :     int FLAG_ERR 				     */
/*									     */
/*****************************************************************************/
/*
 --exemple for emul_connexions     
 --left-hand is for EMULBS connector
 --right-hand is for PGA connector 
 0    <=in a(1);                        
 185  <=out b(2);                   
 56   <=inout toto;                      
 76   <=in g;                      
 80   <=in a(0);                      
*/

%{

#include "global.h"

extern FILE* yyin;
extern int yylex();				     /*fct du lexeur emul.lex*/
extern Infos *InfosVbe;				  /*resultat final du parseur*/
extern int NBR_LIGNES;			     	      /*pour message d'erreur*/


static int FLAG_ERR;				  /*si une erreur est arrivee*/
static int        BS_Card_Max;
static Cellules **BS_Card;   	      /*lien entre n° et EMUL*/
void emul_flag(int x);



%}

%union {
	int VAL;
	char *MOT;
}

%token _IN 
%token _OUT 
%token _INOUT 
%token Pt_virg
%token Db_pt
%token Assign
%token TNbr
%token TMot
%token Lettre
%token Par_ouv Par_fer
%token Cro_ouv Cro_fer
%type <MOT> rconnector
%type <VAL> rtype
%type <MOT> TMot
%type <VAL> TNbr
%type <VAL> rindex
%start fichier


%%

fichier
	:
{
Infos* inf;
Cellules* cell;
int count = 0;

/*initialisation*/
FLAG_ERR=0;
BS_Card_Max = 0;

  for ( inf = InfosVbe; inf; inf = inf->NEXT ) BS_Card_Max += inf->BS_SIZE; 
   
  BS_Card = (Cellules**) mbkalloc(sizeof(Cellules*) * BS_Card_Max);
  
  for ( inf = InfosVbe; inf; inf = inf->NEXT)
    for (cell = inf->CELL; cell; cell = cell->NEXT) BS_Card[count++] = cell;
}
	..declarations..
{
  mbkfree(BS_Card);
  if ( FLAG_ERR ) 
  {
     fclose(yyin);
     exit(1);
  }   
}
;


declaration
	:rdonnee
	|error			/*gestion des erreurs*/
{/*synchronisation avec point virgule ou fin de fichier*/
while (yychar!=Pt_virg && yychar!=0) yychar=yylex(); 
yychar=yylex();
yyerrok;}
;

rdonnee
	:TNbr
	Assign
	rtype
	rconnector
	Pt_virg
{
int   t_Nbr = $1;
int   t_type = $3;
char* t_connector = $4;
Cellules *cell;

/* il y a un nombre maximum de connecteurs sur la carte */
if ( t_Nbr >= BS_Card_Max || t_Nbr < 0 )
{
   fprintf(stderr,"%d: EMULBS31 n°%d doesn't exist!   Index must be between 0 and %d\n",
            NBR_LIGNES, t_Nbr, BS_Card_Max-1);
   emul_flag(-1);
   break;
}

/* alimentation Vdd du CHIP */
if ( isvdd(t_connector) || isvss(t_connector) ) 
{ 
	fprintf(stderr,"%d: power %s shouldn't be connected!\n", NBR_LIGNES, t_connector);
   emul_flag(-3);
   break;
}

/******* chargement des donnees ******/
cell = BS_Card[t_Nbr];

/* connecteur sur carte  deja utilise */
if ( cell->NAME )
{
   fprintf(stderr,"%d: connector n°%d already used by %s!\n", NBR_LIGNES, t_Nbr, cell->NAME);
   emul_flag(-2);
   break;
}

cell->MODE = t_type;
cell->NAME = t_connector;
}
;




..declarations..
	:/*empty*/
	|declaration ..declarations..
;

rconnector
	:TMot rindex			{char memo = SEPAR; SEPAR=' '; $$=nameindex($1,$2); SEPAR=memo;}
	|TMot        		{$$=$1;}
;

rindex:
	Par_ouv
	TNbr
	Par_fer
{$$=$2;}
;

rtype
	:_IN			{$$=MODE_IN;}
	|_OUT			{$$=MODE_OUT;}
	|_INOUT			{$$=MODE_INOUT;}
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
	case TMot: 	fprintf(stderr," append at '%s'\n",yylval.MOT);break;
	case TNbr: 	fprintf(stderr," append at '%d'\n",yylval.VAL);break;
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
	case 0:		fprintf(stderr,"  ';' is missing\n");break;/*EOF*/
	default:	fprintf(stderr," for '%c' not allowed!\n",yychar);
}
emul_flag(-1);
return 1;
}
/*****************************************************************************/



/**************************** END of LEX&YACC ********************************/
/*****************************************************************************/








