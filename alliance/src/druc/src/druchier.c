/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  DRUCHIER.C                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   03/07/95                        |
|                                                             |
\------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mph.h"
#include "mut.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include "rtl.h"
#include "rfm.h"
#include "rpr.h"

#include "drucutil.h"
#include "druchier.h"
/*------------------------------------------------------------\
|                                                             |
|                         DRUCRING Constantes                 |
|                                                             |
\------------------------------------------------------------*/
/*\
 *  tableau des equivalences layer <-> transparences
\*/
DrucTypeStructTransEquiv TabTransEquivalence [ ] =
                         {
                           { RDS_LAYER_ALU1 , RDS_LAYER_TALU1 },
                           { RDS_LAYER_ALU2 , RDS_LAYER_TALU2 },
                           { RDS_LAYER_ALU3 , RDS_LAYER_TALU3 },
                           { RDS_LAYER_POLY , RDS_LAYER_TPOLY }
                         };

/*\
 *  tableau des equivalences layer <-> virtuels
\*/
DrucTypeStructVirtuEquiv TabVirtuEquivalence [ ] =
                         {
                           { RDS_LAYER_ALU1 , RDS_LAYER_VALU1 }
                         };

/*------------------------------------------------------------\
|                                                             |
|                  DRUCHIER  variables                        |
|                                                             |
\------------------------------------------------------------*/
extern long DrucCtmMax;

static char DrucFileBuffer [ DRUC_MAX_FILE_BUFFER + 1 ];

/*------------------------------------------------------------\
|                                                             |
|                  DRUCHIER  Fonctions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  DrucAsTransparenceEquivalent               |
|                                                             |
\------------------------------------------------------------*/
int DrucAsTransparenceEquivalent ( LayerInitial 
                                 )
    int LayerInitial;
{
int IndexTransp;

  LayerInitial = RDS_STATIC_LAYER[ LayerInitial ];

  for ( IndexTransp  = 0;
        IndexTransp  < DRUC_MAX_TRANSPARENCES;
        IndexTransp ++
      )
  {
    if ( ( ( TabTransEquivalence [ IndexTransp ] ).LAYER_INDEX ) == 
         LayerInitial 
       )
    {
      return ( RDS_DYNAMIC_LAYER[ ( TabTransEquivalence [ IndexTransp ] ).LAYER_TRANSPARENCE ] ); 
    }
  }
/*\
 * erreur l'appelant a trouve une transparence et la on ne l'a pas trouve
\*/
  return ( DRUC_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucAsVietuelEquivalent                    |
|                                                             |
\------------------------------------------------------------*/
int DrucAsVirtuelEquivalent ( LayerInitial 
                            )
    int LayerInitial;
{
int IndexVirtu;

  LayerInitial = RDS_STATIC_LAYER[ LayerInitial ];

  for ( IndexVirtu  = 0;
        IndexVirtu  < DRUC_MAX_VIRTUELS;
        IndexVirtu ++
      )
  {
    if ( ( ( TabVirtuEquivalence [ IndexVirtu ] ).LAYER_INDEX ) == 
         LayerInitial 
       )
    {
int toto;
toto = RDS_DYNAMIC_LAYER[ ( TabVirtuEquivalence [ IndexVirtu ] ).LAYER_VIRTUEL ] ; 
      return ( toto ); 
    }
  }
/*\
 * erreur l'appelant a trouve une transparence et la on ne l'a pas trouve
\*/
  return ( DRUC_UNDEFINED_LAYER );
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucInitialiseRegle                        |
|                                                             |
\------------------------------------------------------------*/
void DrucInitialiseRegle ( DrucStructInstruction
                         )
DrucTypeStructInstruction *DrucStructInstruction;
{
int IndexMesure;

  DrucStructInstruction->NEXT        = ( DrucTypeStructInstruction *)NULL;
  DrucStructInstruction->INSTRUCTION =   DRUC_UNDEFINED_INSTRUCTION;
  DrucStructInstruction->LAYER_DEST  =   DRUC_UNDEFINED_LAYER;
  DrucStructInstruction->LAYER_1     =   DRUC_UNDEFINED_LAYER;;
  DrucStructInstruction->LAYER_2     =   DRUC_UNDEFINED_LAYER;;
  DrucStructInstruction->OP_COMPOSE  =   DRUC_UNDEFINED_OPERATEUR;
  for ( IndexMesure = 0;
        IndexMesure < DRUC_MAX_MESURES;
        IndexMesure ++
      )
  {
    DrucStructInstruction->REGLE_NUM  [ IndexMesure ] = DRUC_UNDEFINED_REGLE;
    DrucStructInstruction->RELATION   [ IndexMesure ] = DRUC_UNDEFINED_RELATION;
    DrucStructInstruction->MESURE     [ IndexMesure ] = DRUC_UNDEFINED_MESURE;
    DrucStructInstruction->OP_COMPARE [ IndexMesure ] = DRUC_UNDEFINED_OPERATEUR;
    DrucStructInstruction->VALEUR     [ IndexMesure ] =DRUC_UNDEFINED_VALEUR;
  }
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucAddBBoxRules                           |
|                                                             |
\------------------------------------------------------------*/
void DrucAddABoxRules ( DrucStructInstruction,
                        DrucNombreInstructions
                      )
DrucTypeStructInstruction **DrucStructInstruction;
int                        *DrucNombreInstructions;
{
DrucTypeStructInstruction *TmpStructInstruction;
int                        InstructionAddNumber ;
int                        IndexMesure;

InstructionAddNumber = DRUC_RULES_BASE_ABOX;
  TmpStructInstruction =
    ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
  if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
  {
    DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
  }
  DrucInitialiseRegle ( TmpStructInstruction
                       );
  TmpStructInstruction->NEXT        =  *DrucStructInstruction;
  TmpStructInstruction->INSTRUCTION =   DRUC_ASM_BEGIN_REGLE;
  TmpStructInstruction->LAYER_DEST  =   DRUC_UNDEFINED_LAYER;
  TmpStructInstruction->LAYER_1     =   DRUC_BBOX;
  TmpStructInstruction->LAYER_2     =   DRUC_BBOX;
  TmpStructInstruction->OP_COMPOSE  =   DRUC_INSTR_RELATION;

  for ( IndexMesure = 0;
        IndexMesure < 6;
        IndexMesure ++
      )
  {
    TmpStructInstruction->REGLE_NUM  [ IndexMesure ] = InstructionAddNumber ++;
  }
  TmpStructInstruction->RELATION   [ 0 ] = DRUC_RELATION_ENVELOPPE; 
  TmpStructInstruction->MESURE     [ 0 ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ 0 ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ 0 ] = 0;
  TmpStructInstruction->RELATION   [ 1 ] = DRUC_RELATION_MARGE; 
  TmpStructInstruction->MESURE     [ 1 ] = DRUC_MESURE_PENETRE_INTER;
  TmpStructInstruction->OP_COMPARE [ 1 ] = DRUC_OPERATION_INFEQ;
  TmpStructInstruction->VALEUR     [ 1 ] = 0;
  TmpStructInstruction->RELATION   [ 2 ] = DRUC_RELATION_CROIX; 
  TmpStructInstruction->MESURE     [ 2 ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ 2 ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ 2 ] = 0;
  TmpStructInstruction->RELATION   [ 3 ] = DRUC_RELATION_INTERSECTION; 
  TmpStructInstruction->MESURE     [ 3 ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ 3 ] = DRUC_OPERATION_INFEQ;
  TmpStructInstruction->VALEUR     [ 3 ] = 0;
  TmpStructInstruction->RELATION   [ 4 ] = DRUC_RELATION_EXTENSION; 
  TmpStructInstruction->MESURE     [ 4 ] = DRUC_MESURE_PENETRE_INTER;
  TmpStructInstruction->OP_COMPARE [ 4 ] = DRUC_OPERATION_INFEQ;
  TmpStructInstruction->VALEUR     [ 4 ] = 0;
  TmpStructInstruction->RELATION   [ 5 ] = DRUC_RELATION_INCLUSION; 
  TmpStructInstruction->MESURE     [ 5 ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ 5 ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ 5 ] = 0;
  *DrucStructInstruction                 = TmpStructInstruction;
  (*DrucNombreInstructions) ++;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucAddOneLayerTransparentRule             |
| cree les regles specifiques de contact nul et distance      |
| identiques                                                  |
|                                                             |
\------------------------------------------------------------*/
void DrucAddOneLayerTransparentRule ( DrucInstructionRef,
                                      DrucStructInstruction,
                                      DrucNombreInstructions
                                    )
DrucTypeStructInstruction  *DrucInstructionRef;
DrucTypeStructInstruction **DrucStructInstruction;
int                        *DrucNombreInstructions;
{
DrucTypeStructInstruction *TmpStructInstruction;
int                        InstructionAddNumber ;
int                        IndexMesure;
int                        IndexSuite;
int                        LayerTransparent;
int                        LayerSource;
int                        LayerSource2;

  LayerSource       = DrucInstructionRef->LAYER_1 ;
  LayerSource2      = DrucInstructionRef->LAYER_2 ;
  LayerTransparent  = DrucAsTransparenceEquivalent ( LayerSource );

    TmpStructInstruction =
    ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
    if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
    {
      DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
    }
    DrucInitialiseRegle ( TmpStructInstruction);

    TmpStructInstruction->NEXT        =  *DrucStructInstruction;
    TmpStructInstruction->INSTRUCTION = DrucInstructionRef->INSTRUCTION;
    TmpStructInstruction->LAYER_DEST  = DrucInstructionRef->LAYER_DEST;
    TmpStructInstruction->LAYER_1     =   LayerTransparent;
    TmpStructInstruction->LAYER_2     =   LayerSource2;
    TmpStructInstruction->OP_COMPOSE  = DrucInstructionRef->OP_COMPOSE;

    DrucStructRdsLayer [ LayerTransparent ].RDS_INDEX = LayerTransparent;
    DrucStructRdsLayer [ LayerTransparent ].WIDTH     =
         ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
    DrucStructRdsLayer [ LayerTransparent ].CTM       =
         ( DrucStructRdsLayer [ LayerSource ] ).CTM;

    /*\
     * recopier les regles de distance
    \*/
    IndexSuite = 0;
    for ( IndexMesure = 0;
          IndexMesure < DRUC_MAX_MESURES;
          IndexMesure ++
        )
    {
      if ( ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
           != DRUC_UNDEFINED_REGLE
         )
      {
        if ( (DrucInstructionRef->RELATION [ IndexMesure ] ) == 
             DRUC_RELATION_DISTANCE 
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
            ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
              DRUC_DEUX_CORPS_TRANS_METAL;
	        TmpStructInstruction->RELATION   [ IndexSuite ] = 
            DrucInstructionRef->RELATION [ IndexMesure ];
          TmpStructInstruction->MESURE     [ IndexSuite ] =
            DrucInstructionRef->MESURE [ IndexMesure] ;
          TmpStructInstruction->OP_COMPARE [ IndexSuite ] =
            DrucInstructionRef->OP_COMPARE [ IndexMesure ] ;
          TmpStructInstruction->VALEUR     [ IndexSuite ] =
            DrucInstructionRef->VALEUR [ IndexMesure] ;
          IndexSuite ++ ;
        }
      }
    }

  /*\
   * creer les regles de contact nulles
  \*/
  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_ENVELOPPE;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_TRANS_METAL;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_MARGE;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_PENETRE_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_TRANS_METAL;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_CROIX;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_TRANS_METAL;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_INTERSECTION;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_TRANS_METAL;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_EXTENSION;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_PENETRE_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_TRANS_METAL;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_INCLUSION;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_TRANS_METAL;

  *DrucStructInstruction             = TmpStructInstruction;
  (*DrucNombreInstructions) ++;

  /*\
   * creer les regles Transparences<-->Transparences pour le meme layer
  \*/
  TmpStructInstruction =
  ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
  if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
  {
    DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
  }
  DrucInitialiseRegle ( TmpStructInstruction);

  TmpStructInstruction->NEXT        = *DrucStructInstruction;
  TmpStructInstruction->INSTRUCTION =  DrucInstructionRef->INSTRUCTION;
  TmpStructInstruction->LAYER_DEST  =  DrucInstructionRef->LAYER_DEST;
  TmpStructInstruction->LAYER_1     =  LayerTransparent;
  TmpStructInstruction->LAYER_2     =  LayerTransparent;
  TmpStructInstruction->OP_COMPOSE  =  DrucInstructionRef->OP_COMPOSE;

  /*\
   * recopier les regles
  \*/
  IndexSuite = 0;
  for ( IndexMesure = 0;
        IndexMesure < DRUC_MAX_MESURES;
        IndexMesure ++
      )
  {
    if ( ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
         != DRUC_UNDEFINED_REGLE
       )
    {
      TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
        ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_TRANS_TRANS;
      TmpStructInstruction->RELATION         [ IndexSuite  ] = 
        DrucInstructionRef->RELATION   [ IndexMesure ];
      TmpStructInstruction->MESURE           [ IndexSuite  ] =
        DrucInstructionRef->MESURE     [ IndexMesure ];
      TmpStructInstruction->OP_COMPARE       [ IndexSuite  ] =
        DrucInstructionRef->OP_COMPARE [ IndexMesure ];
      TmpStructInstruction->VALEUR           [ IndexSuite  ] =
        DrucInstructionRef->VALEUR     [ IndexMesure ];
      IndexSuite ++ ;
    }
  }
  *DrucStructInstruction             = TmpStructInstruction;
  (*DrucNombreInstructions) ++;

}
/*------------------------------------------------------------\
|                                                             |
|                  DrucDuplicateUnCorpsTransparent            |
| cree les regles specifiques de contact possible et distance |
| identiques                                                  |
|                                                             |
\------------------------------------------------------------*/
void DrucDuplicateUnCorpsTransparent ( DrucInstructionRef,
                                       DrucStructInstruction,
                                       DrucNombreInstructions
                                     )
DrucTypeStructInstruction  *DrucInstructionRef;
DrucTypeStructInstruction **DrucStructInstruction;
int                        *DrucNombreInstructions;
{
DrucTypeStructInstruction *TmpStructInstruction;
int                        InstructionAddNumber ;
int                        IndexMesure;
int                        IndexSuite;
int                        LayerTransparent;
int                        LayerSource;
int                        LayerSource2;

  LayerSource       = DrucInstructionRef->LAYER_1 ;
  LayerSource2      = DrucInstructionRef->LAYER_2 ;
  LayerTransparent  = DrucAsTransparenceEquivalent ( LayerSource );

    TmpStructInstruction =
    ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
    if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
    {
      DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
    }
    DrucInitialiseRegle ( TmpStructInstruction);

    TmpStructInstruction->NEXT        =  *DrucStructInstruction;
    TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
    TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
    TmpStructInstruction->LAYER_1     =   LayerTransparent;
    TmpStructInstruction->LAYER_2     =   LayerSource2;
    TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;

    DrucStructRdsLayer [ LayerTransparent ].RDS_INDEX = LayerTransparent;
    DrucStructRdsLayer [ LayerTransparent ].WIDTH     =
         ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
    DrucStructRdsLayer [ LayerTransparent ].CTM       =
         ( DrucStructRdsLayer [ LayerSource ] ).CTM;

  /*\
   * creer les regles identiques
  \*/
      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_UN_CORPS_TRANS_METAL;
          TmpStructInstruction->RELATION     [ IndexMesure ] =
              DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE       [ IndexMesure ] =
              DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE   [ IndexMesure ] =
              DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR       [ IndexMesure ] =
              DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
    *DrucStructInstruction             = TmpStructInstruction;
    (*DrucNombreInstructions) ++;

}
/*------------------------------------------------------------\
|                                                             |
|                  DrucDuplicateUnCorpsVirtuel                |
| cree les regles specifiques de contact possible et distance |
| identiques                                                  |
|                                                             |
\------------------------------------------------------------*/
void DrucDuplicateUnCorpsVirtuel ( DrucInstructionRef,
                                   DrucStructInstruction,
                                   DrucNombreInstructions
                                 )
DrucTypeStructInstruction  *DrucInstructionRef;
DrucTypeStructInstruction **DrucStructInstruction;
int                        *DrucNombreInstructions;
{
DrucTypeStructInstruction *TmpStructInstruction;
int                        InstructionAddNumber ;
int                        IndexMesure;
int                        IndexSuite;
int                        LayerVirtuel;
int                        LayerSource;
int                        LayerSource2;

  LayerSource       = DrucInstructionRef->LAYER_1 ;
  LayerSource2      = DrucInstructionRef->LAYER_2 ;
  LayerVirtuel      = DrucAsVirtuelEquivalent ( LayerSource );

    TmpStructInstruction =
    ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
    if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
    {
      DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
    }
    DrucInitialiseRegle ( TmpStructInstruction);

    TmpStructInstruction->NEXT        =  *DrucStructInstruction;
    TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
    TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
    TmpStructInstruction->LAYER_1     =   LayerVirtuel;
    TmpStructInstruction->LAYER_2     =   LayerSource2;
    TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;

    DrucStructRdsLayer [ LayerVirtuel ].RDS_INDEX = LayerVirtuel;
    DrucStructRdsLayer [ LayerVirtuel ].WIDTH     =
         ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
    DrucStructRdsLayer [ LayerVirtuel ].CTM       =
         ( DrucStructRdsLayer [ LayerSource ] ).CTM;

  /*\
   * creer les regles identiques
  \*/
      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_UN_CORPS_VIRT_METAL;
          TmpStructInstruction->RELATION     [ IndexMesure ] =
              DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE       [ IndexMesure ] =
              DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE   [ IndexMesure ] =
              DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR       [ IndexMesure ] =
              DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
    *DrucStructInstruction             = TmpStructInstruction;
    (*DrucNombreInstructions) ++;

}
/*------------------------------------------------------------\
|                                                             |
|                  DrucAddOneLayerVirtuelRule                 |
| cree les regles specifiques de contact possible et distance |
| identiques                                                  |
|                                                             |
\------------------------------------------------------------*/
void DrucAddOneLayerVirtuelRule ( DrucInstructionRef,
                                  DrucStructInstruction,
                                  DrucNombreInstructions
                                )
DrucTypeStructInstruction  *DrucInstructionRef;
DrucTypeStructInstruction **DrucStructInstruction;
int                        *DrucNombreInstructions;
{
DrucTypeStructInstruction *TmpStructInstruction;
int                        InstructionAddNumber ;
int                        IndexMesure;
int                        IndexSuite;
int                        LayerTransparent;
int                        LayerVirtuel;
int                        LayerSource;
int                        LayerSource2;

  LayerSource       = DrucInstructionRef->LAYER_1 ;
  LayerSource2      = DrucInstructionRef->LAYER_2 ;
  LayerVirtuel      = DrucAsVirtuelEquivalent      ( LayerSource );
  LayerTransparent  = DrucAsTransparenceEquivalent ( LayerSource );


    TmpStructInstruction =
    ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
    if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
    {
      DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
    }
    DrucInitialiseRegle ( TmpStructInstruction);

    TmpStructInstruction->NEXT        =  *DrucStructInstruction;
    TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
    TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
    TmpStructInstruction->LAYER_1     =   LayerVirtuel;
    TmpStructInstruction->LAYER_2     =   LayerSource2;
    TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;

    DrucStructRdsLayer [ LayerVirtuel ].RDS_INDEX = LayerVirtuel;
    DrucStructRdsLayer [ LayerVirtuel ].WIDTH     =
         ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
    DrucStructRdsLayer [ LayerVirtuel ].CTM       =
         ( DrucStructRdsLayer [ LayerSource ] ).CTM;

  /*\
   * creer les regles identiques
  \*/
      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_VIRT_METAL;
          TmpStructInstruction->RELATION     [ IndexMesure ] =
              DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE       [ IndexMesure ] =
              DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE   [ IndexMesure ] =
              DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR       [ IndexMesure ] =
              DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
    *DrucStructInstruction             = TmpStructInstruction;
    (*DrucNombreInstructions) ++;

  if ( LayerTransparent != DRUC_UNDEFINED_LAYER )
  {
    /*\
     * creer les regles virtuel<-->transparence pour le meme layer
    \*/
    TmpStructInstruction =
    ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
    if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
    {
      DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
    }
    DrucInitialiseRegle ( TmpStructInstruction);

    TmpStructInstruction->NEXT        =  *DrucStructInstruction;
    TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
    TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
    TmpStructInstruction->LAYER_1     =   LayerVirtuel;
    TmpStructInstruction->LAYER_2     =   LayerTransparent;
    TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;

    /*\
     * recopier les regles de distance
    \*/
    IndexSuite = 0;
    for ( IndexMesure = 0;
          IndexMesure < DRUC_MAX_MESURES;
          IndexMesure ++
        )
    {
      if ( ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
           != DRUC_UNDEFINED_REGLE
         )
      {
        if ( (DrucInstructionRef->RELATION [ IndexMesure ] ) == 
             DRUC_RELATION_DISTANCE 
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
            ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
              DRUC_DEUX_CORPS_VIRT_TRANS;
	        TmpStructInstruction->RELATION [ IndexSuite  ] = 
            DrucInstructionRef->RELATION       [ IndexMesure ];
          TmpStructInstruction->MESURE         [ IndexSuite  ] =
            DrucInstructionRef->MESURE         [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE     [ IndexSuite  ] =
            DrucInstructionRef->OP_COMPARE     [ IndexMesure ];
          TmpStructInstruction->VALEUR         [ IndexSuite  ] =
            DrucInstructionRef->VALEUR         [ IndexMesure ];
          IndexSuite ++ ;
        }
      }
    }

  /*\
   * creer les regles de contact nulles
  \*/
  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_ENVELOPPE;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_VIRT_TRANS;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_MARGE;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_PENETRE_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_VIRT_TRANS;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_CROIX;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_VIRT_TRANS;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_INTERSECTION;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_VIRT_TRANS;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_EXTENSION;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_PENETRE_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_VIRT_TRANS;
  IndexSuite ++ ;

  TmpStructInstruction->RELATION   [ IndexSuite ] = DRUC_RELATION_INCLUSION;
  TmpStructInstruction->MESURE     [ IndexSuite ] = DRUC_MESURE_LARGEUR_INTER;
  TmpStructInstruction->OP_COMPARE [ IndexSuite ] = DRUC_OPERATION_INF;
  TmpStructInstruction->VALEUR     [ IndexSuite ] = 0;
  TmpStructInstruction->REGLE_NUM  [ IndexSuite ] =
          ( DrucInstructionRef->REGLE_NUM [ 0 ] ) + IndexSuite +
          DRUC_DEUX_CORPS_VIRT_TRANS;

    *DrucStructInstruction             = TmpStructInstruction;
    (*DrucNombreInstructions) ++;
  }

    /*\
     * creer les regles virtuel<-->virtuel pour le meme layer
    \*/
  TmpStructInstruction =
  ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
  if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
  {
    DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
  }
  DrucInitialiseRegle ( TmpStructInstruction);

  TmpStructInstruction->NEXT        =  *DrucStructInstruction;
  TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
  TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
  TmpStructInstruction->LAYER_1     =   LayerVirtuel;
  TmpStructInstruction->LAYER_2     =   LayerVirtuel;
  TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;

  /*\
   * creer les regles identiques
  \*/
    for ( IndexMesure = 0;
          IndexMesure < DRUC_MAX_MESURES;
          IndexMesure ++
        )
    {
      if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
           != DRUC_UNDEFINED_REGLE
         )
      {
        TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
          (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
          DRUC_DEUX_CORPS_VIRT_VIRT;
        TmpStructInstruction->RELATION     [ IndexMesure ] =
            DrucInstructionRef->RELATION   [ IndexMesure ];
        TmpStructInstruction->MESURE       [ IndexMesure ] =
            DrucInstructionRef->MESURE     [ IndexMesure ];
        TmpStructInstruction->OP_COMPARE   [ IndexMesure ] =
            DrucInstructionRef->OP_COMPARE [ IndexMesure ];
        TmpStructInstruction->VALEUR       [ IndexMesure ] =
            DrucInstructionRef->VALEUR     [ IndexMesure ];
      }
    }
  *DrucStructInstruction             = TmpStructInstruction;
  (*DrucNombreInstructions) ++;
}
/*------------------------------------------------------------\
|                                                             |
|                  DrucDuplicateHierarchicalRule              |
|                                                             |
\------------------------------------------------------------*/
void DrucDuplicateHierarchicalRule ( DrucStructInstruction,
                                     DrucNombreInstructions
                                   )
DrucTypeStructInstruction **DrucStructInstruction;
int                        *DrucNombreInstructions;
{
DrucTypeStructInstruction *DrucInstructionRef;
DrucTypeStructInstruction *TmpStructInstruction;
int                        LayerTransparent;
int                        LayerTransparent2;
int                        IndexMesure;
int                        LayerSource;
int                        LayerSource2;
int                        LayerVirtuel;
int                        LayerVirtuel2;

  DrucInstructionRef = *DrucStructInstruction;
  LayerSource        = (*DrucStructInstruction)->LAYER_1 ;
  LayerSource2       = (*DrucStructInstruction)->LAYER_2 ;
  LayerTransparent   = DrucAsTransparenceEquivalent ( LayerSource );
  LayerTransparent2  = DrucAsTransparenceEquivalent ( LayerSource2 );
  LayerVirtuel       = DrucAsVirtuelEquivalent      ( LayerSource );
  LayerVirtuel2      = DrucAsVirtuelEquivalent      ( LayerSource2 );

  if ( DrucInstructionRef->INSTRUCTION == DRUC_INSTR_CARACTERISE )
  {
  /*\
   * c'est une instruction a un corps
  \*/
    if ( LayerTransparent != DRUC_UNDEFINED_LAYER )
    {
      DrucDuplicateUnCorpsTransparent ( DrucInstructionRef,
                                        DrucStructInstruction,
                                        DrucNombreInstructions
                                      );
    }
    if ( LayerVirtuel != DRUC_UNDEFINED_LAYER )
    {
      DrucDuplicateUnCorpsVirtuel ( DrucInstructionRef,
                                    DrucStructInstruction,
                                    DrucNombreInstructions
                                  );
    }
    return;
  }

  /*\
   * Donc c'est une instruction a 2 corps
  \*/
  if ( DrucInstructionRef->LAYER_1 == DrucInstructionRef->LAYER_2 )
  {
    /*\
     * on n'a qu'une transformation a faire L1-->T1  L1-->V1
     * et T1 ne doit pas toucher L1 
    \*/
    if ( LayerTransparent != DRUC_UNDEFINED_LAYER ) 
    {
      DrucAddOneLayerTransparentRule ( DrucInstructionRef,
                                       DrucStructInstruction,
                                       DrucNombreInstructions
                                     );  
    }
    if ( LayerVirtuel != DRUC_UNDEFINED_LAYER )
    {
      DrucAddOneLayerVirtuelRule ( DrucInstructionRef,
                                   DrucStructInstruction,
                                   DrucNombreInstructions
                                 );
    }
  }
  else
  {
    /*\
     * on doit faire les transformations suivantes:
     * L1,L2 -->L1,T2 .
     *          L1,V2 .
     *          T1,L2 .
     *          T1,T2 .
     *          T1,V2
     *          V1,L2 .
     *          V1,T2
     *          V1,V2 .
    \*/
    if ( LayerTransparent != DRUC_UNDEFINED_LAYER )
    {
      /*\
       * creer T1,L2
      \*/
      TmpStructInstruction = 
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction
                           );
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerTransparent;
      TmpStructInstruction->LAYER_2     =   LayerSource2;
      TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;
  
      DrucStructRdsLayer [ LayerTransparent ].RDS_INDEX = LayerTransparent;
      DrucStructRdsLayer [ LayerTransparent ].WIDTH     = 
           ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
      DrucStructRdsLayer [ LayerTransparent ].CTM       = 
           ( DrucStructRdsLayer [ LayerSource ] ).CTM;
  
      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) 
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) + 
            DRUC_DEUX_CORPS_TRANS_METAL;
          TmpStructInstruction->RELATION     [ IndexMesure ] = 
              DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE       [ IndexMesure ] =
              DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE   [ IndexMesure ] = 
              DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR       [ IndexMesure ] =
              DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;
    }
    if ( LayerTransparent2 != DRUC_UNDEFINED_LAYER )
    {
     /*\
      * creer L1,T2
     \*/
      TmpStructInstruction =
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction
                           );
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerSource;
      TmpStructInstruction->LAYER_2     =   LayerTransparent2;
      TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;
 
      DrucStructRdsLayer [ LayerTransparent2 ].RDS_INDEX = LayerTransparent2;
      DrucStructRdsLayer [ LayerTransparent2 ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource2 ] ).WIDTH;
      DrucStructRdsLayer [ LayerTransparent2 ].CTM       =
           ( DrucStructRdsLayer [ LayerSource2 ] ).CTM;
 
      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_METAL_TRANS;
          TmpStructInstruction->RELATION     [ IndexMesure ] =
              DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE       [ IndexMesure ] =
              DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE   [ IndexMesure ] =
              DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR       [ IndexMesure ] =
              DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;
    }
    if ( ( LayerTransparent  != DRUC_UNDEFINED_LAYER ) &&
         ( LayerTransparent2 != DRUC_UNDEFINED_LAYER )
       )
    {
      /*\
       * creer T1,T2
      \*/
      TmpStructInstruction =
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction);
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION = DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  = DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerTransparent;
      TmpStructInstruction->LAYER_2     =   LayerTransparent2;
      TmpStructInstruction->OP_COMPOSE  = DrucInstructionRef->OP_COMPOSE;
  
      DrucStructRdsLayer [ LayerTransparent ].RDS_INDEX = LayerTransparent;
      DrucStructRdsLayer [ LayerTransparent ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
      DrucStructRdsLayer [ LayerTransparent ].CTM       =
           ( DrucStructRdsLayer [ LayerSource ] ).CTM;
  
      DrucStructRdsLayer [ LayerTransparent2 ].RDS_INDEX = LayerTransparent2;
      DrucStructRdsLayer [ LayerTransparent2 ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource2 ] ).WIDTH;
      DrucStructRdsLayer [ LayerTransparent2 ].CTM       =
           ( DrucStructRdsLayer [ LayerSource2 ] ).CTM;
  
      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_TRANS_TRANS;
          TmpStructInstruction->RELATION   [ IndexMesure ] =
            DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE     [ IndexMesure ] =
            DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE [ IndexMesure ] =
            DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR     [ IndexMesure ] =
            DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;
    }
    if ( LayerVirtuel != DRUC_UNDEFINED_LAYER )
    {
     /*\
      * creer V1,L2
     \*/
      TmpStructInstruction =
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction
                           );
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerVirtuel;
      TmpStructInstruction->LAYER_2     =   LayerSource2;
      TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;
 
      DrucStructRdsLayer [ LayerVirtuel ].RDS_INDEX = LayerVirtuel;
      DrucStructRdsLayer [ LayerVirtuel ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
      DrucStructRdsLayer [ LayerVirtuel ].CTM       =
           ( DrucStructRdsLayer [ LayerSource ] ).CTM;

      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_VIRT_METAL;
          TmpStructInstruction->RELATION     [ IndexMesure ] =
              DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE       [ IndexMesure ] =
              DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE   [ IndexMesure ] =
              DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR       [ IndexMesure ] =
              DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;
    }
    if ( LayerVirtuel2 != DRUC_UNDEFINED_LAYER )
    {
     /*\
      * creer L1,V2
     \*/
      TmpStructInstruction =
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction
                           );
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION =   DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  =   DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerSource;
      TmpStructInstruction->LAYER_2     =   LayerVirtuel2;
      TmpStructInstruction->OP_COMPOSE  =   DrucInstructionRef->OP_COMPOSE;

      DrucStructRdsLayer [ LayerVirtuel2 ].RDS_INDEX = LayerVirtuel2;
      DrucStructRdsLayer [ LayerVirtuel2 ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource2 ] ).WIDTH;
      DrucStructRdsLayer [ LayerVirtuel2 ].CTM       =
           ( DrucStructRdsLayer [ LayerSource2 ] ).CTM;

      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            (   DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_METAL_VIRT;
          TmpStructInstruction->RELATION     [ IndexMesure ] =
              DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE       [ IndexMesure ] =
              DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE   [ IndexMesure ] =
              DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR       [ IndexMesure ] =
              DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;
    }
    if ( ( LayerVirtuel  != DRUC_UNDEFINED_LAYER ) &&
         ( LayerVirtuel2 != DRUC_UNDEFINED_LAYER )
       )
    {
     /*\
      * creer V1,V2
     \*/
      TmpStructInstruction =
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction);
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION = DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  = DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerVirtuel;
      TmpStructInstruction->LAYER_2     =   LayerVirtuel2;
      TmpStructInstruction->OP_COMPOSE  = DrucInstructionRef->OP_COMPOSE;

      DrucStructRdsLayer [ LayerVirtuel ].RDS_INDEX = LayerVirtuel;
      DrucStructRdsLayer [ LayerVirtuel ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
      DrucStructRdsLayer [ LayerVirtuel ].CTM       =
           ( DrucStructRdsLayer [ LayerSource ] ).CTM;

      DrucStructRdsLayer [ LayerVirtuel2 ].RDS_INDEX = LayerVirtuel2;
      DrucStructRdsLayer [ LayerVirtuel2 ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource2 ] ).WIDTH;
      DrucStructRdsLayer [ LayerVirtuel2 ].CTM       =
           ( DrucStructRdsLayer [ LayerSource2 ] ).CTM;

      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_VIRT_VIRT;
          TmpStructInstruction->RELATION   [ IndexMesure ] =
            DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE     [ IndexMesure ] =
            DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE [ IndexMesure ] =
            DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR     [ IndexMesure ] =
            DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;
    }
    if ( ( LayerTransparent != DRUC_UNDEFINED_LAYER ) &&
         ( LayerVirtuel2    != DRUC_UNDEFINED_LAYER )
       )
    {
     /*\
      * creer T1,V2
     \*/
      TmpStructInstruction =
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction);
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION = DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  = DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerTransparent;
      TmpStructInstruction->LAYER_2     =   LayerVirtuel2;
      TmpStructInstruction->OP_COMPOSE  = DrucInstructionRef->OP_COMPOSE;

      DrucStructRdsLayer [ LayerTransparent ].RDS_INDEX = LayerTransparent;
      DrucStructRdsLayer [ LayerTransparent ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
      DrucStructRdsLayer [ LayerTransparent ].CTM       =
           ( DrucStructRdsLayer [ LayerSource ] ).CTM;

      DrucStructRdsLayer [ LayerVirtuel2 ].RDS_INDEX = LayerVirtuel2;
      DrucStructRdsLayer [ LayerVirtuel2 ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource2 ] ).WIDTH;
      DrucStructRdsLayer [ LayerVirtuel2 ].CTM       =
           ( DrucStructRdsLayer [ LayerSource2 ] ).CTM;

      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_TRANS_VIRT;
          TmpStructInstruction->RELATION   [ IndexMesure ] =
            DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE     [ IndexMesure ] =
            DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE [ IndexMesure ] =
            DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR     [ IndexMesure ] =
            DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;

    }
    if ( ( LayerVirtuel      != DRUC_UNDEFINED_LAYER ) &&
         ( LayerTransparent2 != DRUC_UNDEFINED_LAYER )
       )
    {
     /*\
      * creer V1,T2
     \*/
      TmpStructInstruction =
      ( DrucTypeStructInstruction *)malloc (sizeof ( DrucTypeStructInstruction ));
      if (TmpStructInstruction == (DrucTypeStructInstruction *)NULL )
      {
        DRUC_EXIT ( DRUC_ERROR_MEMOIRE );
      }
      DrucInitialiseRegle ( TmpStructInstruction);
      TmpStructInstruction->NEXT        =  *DrucStructInstruction;
      TmpStructInstruction->INSTRUCTION = DrucInstructionRef->INSTRUCTION;
      TmpStructInstruction->LAYER_DEST  = DrucInstructionRef->LAYER_DEST;
      TmpStructInstruction->LAYER_1     =   LayerVirtuel;
      TmpStructInstruction->LAYER_2     =   LayerTransparent2;
      TmpStructInstruction->OP_COMPOSE  = DrucInstructionRef->OP_COMPOSE;

      DrucStructRdsLayer [ LayerVirtuel ].RDS_INDEX = LayerVirtuel;
      DrucStructRdsLayer [ LayerVirtuel ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource ] ).WIDTH;
      DrucStructRdsLayer [ LayerVirtuel ].CTM       =
           ( DrucStructRdsLayer [ LayerSource ] ).CTM;

      DrucStructRdsLayer [ LayerTransparent2 ].RDS_INDEX = LayerTransparent2;
      DrucStructRdsLayer [ LayerTransparent2 ].WIDTH     =
           ( DrucStructRdsLayer [ LayerSource2 ] ).WIDTH;
      DrucStructRdsLayer [ LayerTransparent2 ].CTM       =
           ( DrucStructRdsLayer [ LayerSource2 ] ).CTM;

      for ( IndexMesure = 0;
            IndexMesure < DRUC_MAX_MESURES;
            IndexMesure ++
          )
      {
        if ( ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] )
             != DRUC_UNDEFINED_REGLE
           )
        {
          TmpStructInstruction->REGLE_NUM  [ IndexMesure ] =
            ( DrucInstructionRef->REGLE_NUM [ IndexMesure ] ) +
            DRUC_DEUX_CORPS_VIRT_TRANS;
          TmpStructInstruction->RELATION   [ IndexMesure ] =
            DrucInstructionRef->RELATION   [ IndexMesure ];
          TmpStructInstruction->MESURE     [ IndexMesure ] =
            DrucInstructionRef->MESURE     [ IndexMesure ];
          TmpStructInstruction->OP_COMPARE [ IndexMesure ] =
            DrucInstructionRef->OP_COMPARE [ IndexMesure ];
          TmpStructInstruction->VALEUR     [ IndexMesure ] =
            DrucInstructionRef->VALEUR     [ IndexMesure ];
        }
      }
      *DrucStructInstruction             = TmpStructInstruction;
      (*DrucNombreInstructions) ++;

    }
  }
}
