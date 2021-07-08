%{
#include <stdio.h>
#include "mph.h"
#include "mut.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include "rtl.h"
#include "rfm.h"
#include "rpr.h"
#include "drucutil.h"
#include "drucompi.h"

/*------------------------------------------------------------\
|                                                             |
|                  Druc MODIFICATIONS                         |
|                                                             |
|  Patrick: 07/03/94                                          |
|    Ajouter DRUC_FRONTALE et DRUC_LATERALE_MIN               |
|    qui etaient oublies dans les derivations                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc Variables LEX/YACC                    |
|                                                             |
\------------------------------------------------------------*/
extern int   yylineno;
extern FILE *yyin;
extern int   yylex ();

int   DrucIndexMesure;
int   DrucInputEntier;
int   DrucNegatif;
float DrucInputFloat;
char  DrucInputChaine[DRUC_MAX_STRING_BUFFER + 1];
%}

/*------------------------------------------------------------\
|                                                             |
|                  Druc Types LEX/YACC                        |
|                                                             |
\------------------------------------------------------------*/
%union {
   unsigned int uint;
}

/*------------------------------------------------------------\
|                                                             |
|                  Druc Terminaux                             |
|                                                             |
\------------------------------------------------------------*/
%token      DRUC_REGLES        DRUC_FIN           DRUC_REGLE

            DRUC_DEFINE        DRUC_UNDEFINE
            DRUC_CARACTERISE   DRUC_RELATION

            DRUC_LAYER         DRUC_NOM

            DRUC_UNION         DRUC_EXCLUSION     DRUC_RESIZE

            DRUC_DESTINATION

            DRUC_ENTIER        DRUC_FLOAT         DRUC_NEG_FLOAT

%type  <uint> config
%token <uint> DRUC_DISTANCE DRUC_INTERSECT DRUC_EXTENSION
              DRUC_INCLUSION DRUC_ENVELOPPE DRUC_MARGE DRUC_CROIX
%type  <uint> mesure_rel
%token <uint> DRUC_SURFACE_INTER DRUC_LONG_INTER DRUC_LARGE_INTER
              DRUC_PENET_INTER DRUC_PARAL_INTER DRUC_PERPEN_INTER
              DRUC_LONGUEUR_MIN DRUC_LONGUEUR_MAX DRUC_LARGEUR_MIN
              DRUC_LARGEUR_MAX DRUC_FRONTALE DRUC_LATERALE_MIN
              DRUC_LATERALE_MAX DRUC_SUP DRUC_INF DRUC_AXIALE
              DRUC_GEODESIQUE
%type  <uint> mesure
%token <uint> DRUC_SURFACE DRUC_LONGUEUR DRUC_LARGEUR DRUC_NOTCH
%type  <uint> compar
%token <uint> DRUC_MIN DRUC_MAX DRUC_INFEQ DRUC_SUPEQ '<' '>'
              '=' DRUC_DIFF
/*------------------------------------------------------------\
|                                                             |
|                  Druc Racine                                |
|                                                             |
\------------------------------------------------------------*/
%start drc

/*------------------------------------------------------------\
|                                                             |
|                  Druc Regles                                |
|                                                             |
\------------------------------------------------------------*/
%%

drc       :                  { DrucNombreInstructions = 0;
                               DrucInitCompiler () ;}
            list_layer
            DRUC_REGLES
            list_bloc
            DRUC_FIN
            DRUC_REGLES      { return 0;}
          ;

list_layer: list_layer layer
          | layer
          ;

layer     : DRUC_LAYER
            DRUC_NOM         { DrucInitRdsLayerName ( DrucInputChaine ) ;}
            DRUC_FLOAT       { DrucInitRdsLayerDef ( RprTranslateParam ( DrucInputFloat ) );}
            ';'
          ;

list_bloc : list_bloc
            bloc
          | bloc
          ;

bloc      : define           { DrucLoadInstructionDefine () ;}
          | undefine         { DrucLoadInstructionUndefine () ;}
          | caracter         { DrucLoadInstructionRegle () ;}
          | relation         { DrucLoadInstructionRegle () ;}
          ;

define    : DRUC_DEFINE
            DRUC_NOM         { DrucInitDefineLayerA ( DrucInputChaine ) ;}
            resiz
            DRUC_DESTINATION
            DRUC_NOM         { DrucInitDefineLayerDest ( DrucInputChaine ) ;}
            ';'
          ;

resiz     : valresiz
            defmodif
	  | mesure_rel	     
	    valresiz         { DrucStructDefineOpCompose ( $1 ) ; }
          | ','
            DRUC_NOM         { DrucInitDefineLayerB ( DrucInputChaine ) ;}
            compose
          ;

valresiz  :  DRUC_FLOAT      { DrucInitDefineValeur ( RprTranslateParam ( DrucInputFloat)) ;}
          | DRUC_NEG_FLOAT   { long i ;
                               i =  RprTranslateParam ( DrucInputFloat );
                               DrucInitDefineValeur ( i );}
          ;

defoper   : ','              { DrucInitOpUnaire   ( ) ;}
            DRUC_NOM         { DrucInitDefineLayerB ( DrucInputChaine ) ;}
            compose
          |
          ;

compose   : DRUC_UNION       { DrucStructDefineOpCompose( DRUC_COMPOSE_UNION );}
          | DRUC_INTERSECT   { DrucStructDefineOpCompose( DRUC_COMPOSE_INTERSECT );}
          | DRUC_EXCLUSION   { DrucStructDefineOpCompose( DRUC_COMPOSE_EXCLUSION );}
          | DRUC_INCLUSION   { DrucStructDefineOpCompose( DRUC_COMPOSE_INCLUSION );}
          ;

defmodif  : DRUC_RESIZE      { DrucStructDefineOpCompose( DRUC_COMPOSE_RESIZE    );}
          ;

undefine  : DRUC_UNDEFINE
            DRUC_NOM         { DrucInitUndefineUserLayer ( DrucInputChaine ) ;}
            ';'
          ;

caracter  : DRUC_CARACTERISE
            DRUC_NOM         { DrucInitRegleLayerA ( DrucInputChaine ) ;
                               DrucInitRegleInstruction ( DRUC_INSTR_CARACTERISE ) ;
                               DrucIndexMesure = -1 ;}
            '('
            lst_rgl_1
            ')'
            ';'
          ;

lst_rgl_1 : lst_rgl_1
            regle_1
          | regle_1
          ;

regle_1   : DRUC_REGLE       { DrucIndexMesure ++ ;}
            DRUC_ENTIER      { DrucInitRegleNumero ( DrucInputEntier,
                                                     DrucIndexMesure) ;}
            ':'
            mesure           { DrucInitRegleMesure( $6,
                                                    DrucIndexMesure );}
            compar           { DrucInitRegleOpCompare ( $8,
                                                        DrucIndexMesure );}
            DRUC_FLOAT       { DrucInitRegleValeur ( RprTranslateParam ( DrucInputFloat ),
                                                     DrucIndexMesure );}
            ';'
          ;

relation  : DRUC_RELATION    { DrucInitRegleInstruction ( DRUC_INSTR_RELATION ) ;}
            DRUC_NOM         { DrucInitRegleLayerA ( DrucInputChaine ) ;}
            ','
            DRUC_NOM         { DrucInitRegleLayerB ( DrucInputChaine ) ;
                               DrucIndexMesure = -1 ; }
            '('
            lst_rgl_2
            ')'
            ';'
          ;

lst_rgl_2 : lst_rgl_2
            regle_2
          | regle_2
          ;

regle_2   : DRUC_REGLE         { DrucIndexMesure++ ;}
            DRUC_ENTIER        { DrucInitRegleNumero ( DrucInputEntier,
                                                       DrucIndexMesure ) ;}
            ':'
            config             { DrucInitRegleRelation( $6,
                                                          DrucIndexMesure );}
            mesure_rel         { DrucInitRegleMesure( $8,
                                                      DrucIndexMesure );}
            compar             { DrucInitRegleOpCompare ( $10,
                                                          DrucIndexMesure );}
            DRUC_FLOAT         { DrucInitRegleValeur ( RprTranslateParam ( DrucInputFloat ),
                                                       DrucIndexMesure );}
            ';'
          ;

config    : DRUC_DISTANCE      { $$ = DRUC_RELATION_DISTANCE;}
          | DRUC_INTERSECT     { $$ = DRUC_RELATION_INTERSECTION;}
          | DRUC_EXTENSION     { $$ = DRUC_RELATION_EXTENSION;}
          | DRUC_INCLUSION     { $$ = DRUC_RELATION_INCLUSION;}
          | DRUC_ENVELOPPE     { $$ = DRUC_RELATION_ENVELOPPE;}
          | DRUC_MARGE         { $$ = DRUC_RELATION_MARGE;}
          | DRUC_CROIX         { $$ = DRUC_RELATION_CROIX;}
          ;

mesure_rel: DRUC_SURFACE_INTER { $$ = DRUC_MESURE_SURFACE_INTER;}
          | DRUC_LONG_INTER    { $$ = DRUC_MESURE_LONGUEUR_INTER;}
          | DRUC_LARGE_INTER   { $$ = DRUC_MESURE_LARGEUR_INTER;}
          | DRUC_PENET_INTER   { $$ = DRUC_MESURE_PENETRE_INTER;}
          | DRUC_PARAL_INTER   { $$ = DRUC_MESURE_PARALEL_INTER;}
          | DRUC_PERPEN_INTER  { $$ = DRUC_MESURE_PERPEND_INTER;}
          | DRUC_LONGUEUR_MIN  { $$ = DRUC_MESURE_LONGUEUR_MIN;}
          | DRUC_LONGUEUR_MAX  { $$ = DRUC_MESURE_LONGUEUR_MAX;}
          | DRUC_LARGEUR_MIN   { $$ = DRUC_MESURE_LARGEUR_MIN;}
          | DRUC_LARGEUR_MAX   { $$ = DRUC_MESURE_LARGEUR_MAX;}
          | DRUC_FRONTALE      { $$ = DRUC_MESURE_FRONTALE;}
          | DRUC_LATERALE_MIN  { $$ = DRUC_MESURE_LATERALE_MIN;}
          | DRUC_LATERALE_MAX  { $$ = DRUC_MESURE_LATERALE_MAX;}
          | DRUC_SUP           { $$ = DRUC_MESURE_SUP;}
          | DRUC_INF           { $$ = DRUC_MESURE_INF;}
          | DRUC_AXIALE        { $$ = DRUC_MESURE_AXIALE;}
          | DRUC_GEODESIQUE    { $$ = DRUC_MESURE_GEODESIQUE;}
          ;

mesure    : DRUC_SURFACE       { $$ = DRUC_MESURE_SURFACE;}
          | DRUC_LONGUEUR      { $$ = DRUC_MESURE_LONGUEUR;}
          | DRUC_LARGEUR       { $$ = DRUC_MESURE_LARGEUR;}
          | DRUC_NOTCH         { $$ = DRUC_MESURE_NOTCH;}
          | DRUC_SURFACE_INTER { $$ = DRUC_MESURE_SURFACE_INTER;}
          | DRUC_LONG_INTER    { $$ = DRUC_MESURE_LONGUEUR_INTER;}
          | DRUC_LARGE_INTER   { $$ = DRUC_MESURE_LARGEUR_INTER;}
          ;

compar    : DRUC_MIN           { $$ = DRUC_OPERATION_MIN;}
          | DRUC_MAX           { $$ = DRUC_OPERATION_MAX;}
          | DRUC_INFEQ         { $$ = DRUC_OPERATION_INFEQ;}
          | DRUC_SUPEQ         { $$ = DRUC_OPERATION_SUPEQ;}
          | '<'                { $$ = DRUC_OPERATION_INF;}
          | '>'                { $$ = DRUC_OPERATION_SUP;}
          | '='                { $$ = DRUC_OPERATION_EQUAL;}
          | DRUC_DIFF          { $$ = DRUC_OPERATION_DIFF;}
          ;

%%
