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

            DRUC_ENVELOPPE     DRUC_MARGE         DRUC_CROIX
            DRUC_INTERSECT     DRUC_EXTENSION     DRUC_INCLUSION
            DRUC_DISTANCE

            DRUC_SURFACE_INTER DRUC_LONG_INTER    DRUC_LARGE_INTER
            DRUC_PENET_INTER   DRUC_PARAL_INTER   DRUC_PERPEN_INTER

            DRUC_FRONTALE      DRUC_LATERALE_MIN  DRUC_LATERALE_MAX
            DRUC_LARGEUR_MIN   DRUC_LARGEUR_MAX
            DRUC_LONGUEUR_MIN  DRUC_LONGUEUR_MAX
            DRUC_SUP           DRUC_INF
            DRUC_AXIALE        DRUC_GEODESIQUE

            DRUC_SURFACE       DRUC_LONGUEUR      DRUC_LARGEUR

            DRUC_MIN           DRUC_MAX
            DRUC_INFEQ         DRUC_SUPEQ
            DRUC_DIFF          DRUC_NOTCH

            DRUC_DESTINATION

            DRUC_ENTIER        DRUC_FLOAT         DRUC_NEG_FLOAT

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
	    DRUC_REGLES      { return ;}
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
            ','
	    resiz
            DRUC_DESTINATION
	    DRUC_NOM         { DrucInitDefineLayerDest ( DrucInputChaine ) ;}
            ';'              
	  ;

resiz     : valresiz
            defmodif
            defoper
          | DRUC_NOM         { DrucInitDefineLayerB ( DrucInputChaine ) ;} 
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
            mesure
            compar
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

regle_2   : DRUC_REGLE       { DrucIndexMesure ++ ;}
	    DRUC_ENTIER      { DrucInitRegleNumero ( DrucInputEntier,
                                                     DrucIndexMesure ) ;}
            ':'
            config
            mesure_rel
            compar
	    DRUC_FLOAT       { DrucInitRegleValeur ( RprTranslateParam ( DrucInputFloat ),
                                                     DrucIndexMesure );}
            ';'
          ;

config    : DRUC_DISTANCE    { DrucInitRegleRelation( DRUC_RELATION_DISTANCE,
                                                      DrucIndexMesure );}
	  | DRUC_INTERSECT   { DrucInitRegleRelation( DRUC_RELATION_INTERSECTION,
                                                      DrucIndexMesure );}
	  | DRUC_EXTENSION   { DrucInitRegleRelation( DRUC_RELATION_EXTENSION,
                                                      DrucIndexMesure );}
	  | DRUC_INCLUSION   { DrucInitRegleRelation( DRUC_RELATION_INCLUSION,
                                                      DrucIndexMesure );}
	  | DRUC_ENVELOPPE   { DrucInitRegleRelation( DRUC_RELATION_ENVELOPPE,
                                                      DrucIndexMesure );}
	  | DRUC_MARGE       { DrucInitRegleRelation( DRUC_RELATION_MARGE,
                                                      DrucIndexMesure );}
	  | DRUC_CROIX       { DrucInitRegleRelation( DRUC_RELATION_CROIX,
                                                      DrucIndexMesure );}
          ;

mesure_rel: DRUC_SURFACE_INTER{ DrucInitRegleMesure( DRUC_MESURE_SURFACE_INTER,
                                                    DrucIndexMesure );}
          | DRUC_LONG_INTER  { DrucInitRegleMesure( DRUC_MESURE_LONGUEUR_INTER,
                                                    DrucIndexMesure );}
	  | DRUC_LARGE_INTER { DrucInitRegleMesure( DRUC_MESURE_LARGEUR_INTER,
                                                    DrucIndexMesure );}
	  | DRUC_PENET_INTER { DrucInitRegleMesure( DRUC_MESURE_PENETRE_INTER,
                                                    DrucIndexMesure );}
          | DRUC_PARAL_INTER { DrucInitRegleMesure( DRUC_MESURE_PARALEL_INTER,
                                                    DrucIndexMesure );}
          | DRUC_PERPEN_INTER{ DrucInitRegleMesure( DRUC_MESURE_PERPEND_INTER,
                                                    DrucIndexMesure );}
	  | DRUC_LONGUEUR_MIN{ DrucInitRegleMesure( DRUC_MESURE_LONGUEUR_MIN,
                                                    DrucIndexMesure );}
	  | DRUC_LONGUEUR_MAX{ DrucInitRegleMesure( DRUC_MESURE_LONGUEUR_MAX,
                                                    DrucIndexMesure );}
	  | DRUC_LARGEUR_MIN { DrucInitRegleMesure( DRUC_MESURE_LARGEUR_MIN,
                                                    DrucIndexMesure );}
	  | DRUC_LARGEUR_MAX { DrucInitRegleMesure( DRUC_MESURE_LARGEUR_MAX,
                                                    DrucIndexMesure );}
	  | DRUC_FRONTALE    { DrucInitRegleMesure( DRUC_MESURE_FRONTALE,
                                                    DrucIndexMesure );}
	  | DRUC_LATERALE_MIN{ DrucInitRegleMesure( DRUC_MESURE_LATERALE_MIN,
                                                    DrucIndexMesure );}
	  | DRUC_LATERALE_MAX{ DrucInitRegleMesure( DRUC_MESURE_LATERALE_MAX,
                                                    DrucIndexMesure );}
	  | DRUC_SUP         { DrucInitRegleMesure( DRUC_MESURE_SUP,
                                                    DrucIndexMesure );}
	  | DRUC_INF         { DrucInitRegleMesure( DRUC_MESURE_INF,
                                                    DrucIndexMesure );}
	  | DRUC_AXIALE      { DrucInitRegleMesure( DRUC_MESURE_AXIALE,
                                                    DrucIndexMesure );}
	  | DRUC_GEODESIQUE  { DrucInitRegleMesure( DRUC_MESURE_GEODESIQUE,
                                                    DrucIndexMesure );}
          ;

mesure    : DRUC_SURFACE     { DrucInitRegleMesure( DRUC_MESURE_SURFACE,
                                                    DrucIndexMesure );}
	  | DRUC_LONGUEUR    { DrucInitRegleMesure( DRUC_MESURE_LONGUEUR,
                                                    DrucIndexMesure );}
	  | DRUC_LARGEUR     { DrucInitRegleMesure( DRUC_MESURE_LARGEUR,
                                                    DrucIndexMesure );}
	  | DRUC_NOTCH       { DrucInitRegleMesure( DRUC_MESURE_NOTCH,
                                                    DrucIndexMesure );}
          | DRUC_SURFACE_INTER { DrucInitRegleMesure( DRUC_MESURE_SURFACE_INTER,
                                                    DrucIndexMesure );}
          | DRUC_LONG_INTER  { DrucInitRegleMesure( DRUC_MESURE_LONGUEUR_INTER,
                                                    DrucIndexMesure );}
          | DRUC_LARGE_INTER { DrucInitRegleMesure( DRUC_MESURE_LARGEUR_INTER,
                                                    DrucIndexMesure );}
          ;

compar    : DRUC_MIN         { DrucInitRegleOpCompare ( DRUC_OPERATION_MIN,
                                                        DrucIndexMesure );}
	  | DRUC_MAX         { DrucInitRegleOpCompare ( DRUC_OPERATION_MAX,
                                                        DrucIndexMesure );}
	  | DRUC_INFEQ       { DrucInitRegleOpCompare ( DRUC_OPERATION_INFEQ,
                                                        DrucIndexMesure );}
	  | DRUC_SUPEQ       { DrucInitRegleOpCompare ( DRUC_OPERATION_SUPEQ,
                                                        DrucIndexMesure );}
	  | '<'              { DrucInitRegleOpCompare ( DRUC_OPERATION_INF,
                                                        DrucIndexMesure );}
	  | '>'              { DrucInitRegleOpCompare ( DRUC_OPERATION_SUP,
                                                        DrucIndexMesure );}
          | '='              { DrucInitRegleOpCompare ( DRUC_OPERATION_EQUAL,
                                                        DrucIndexMesure );}
          | DRUC_DIFF        { DrucInitRegleOpCompare ( DRUC_OPERATION_DIFF,
                                                        DrucIndexMesure );}
          ;
 

%%
