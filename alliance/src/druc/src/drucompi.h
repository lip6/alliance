/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  Drucompi.h                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc Compiler Defines                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc Compiler Types                        |
|                                                             |
\------------------------------------------------------------*/
typedef struct data_list
              {
              struct data_list *NEXT;
              int REGLE_NUM;
              } DataList;

typedef struct struct_define
              {
                int   LAYER_DEST;
                int   OP_COMPOSE;
                int   OP_UNAIRE;
                int   LAYER_1;
                int   LAYER_2;
                long  VALEUR;
                int   USER_LAYER_0;
                int   USER_LAYER_1;
                int   USER_LAYER_2;
                char *USER_LAYER_NAME_0;
                char *USER_LAYER_NAME_1;
                char *USER_LAYER_NAME_2;
              } DrucTypeStructDefine;

typedef struct struct_regle
              {
                DataList *LIST_REGLE_NUM;
                int   REGLE_NUM [ DRUC_MAX_MESURES ];
                int   INSTRUCTION;
                int   RELATION [ DRUC_MAX_MESURES ];
                int   LAYER_1;
                int   LAYER_2;
                int   MESURE [ DRUC_MAX_MESURES ];
                int   OP_COMPARE [ DRUC_MAX_MESURES ];
                long  VALEUR [ DRUC_MAX_MESURES ];
              } DrucTypeStructRegle;

/*------------------------------------------------------------\
|                                                             |
|                  Druc Compiler variables                    |
|                                                             |
\------------------------------------------------------------*/
extern DrucTypeStructDefine       DrucStructDefine;
extern DrucTypeStructRegle        DrucStructRegle;
extern DrucTypeStructInstruction *DrucStructInstruction;
/*------------------------------------------------------------\
|                                                             |
|                  Druc Variables LEX/YACC                    |
|                                                             |
\------------------------------------------------------------*/
extern FILE *vrd_y_in;
extern int   vrd_y_lineno;
extern int   DrucInputEntier;
extern float DrucInputFloat;
extern char  DrucInputChaine[];
/*------------------------------------------------------------\
|                                                             |
|                  Druc Fonctions LEX/YACC                    |
|                                                             |
\------------------------------------------------------------*/
extern int  vrd_y_parse                      ();
extern void vrd_y_error                      ();
/*------------------------------------------------------------\
|                                                             |
|                  Druc Fonctions                             |
|                                                             |
\------------------------------------------------------------*/
extern void DrucInitCompiler             ();
extern void DrucCloseCompiler            ();

extern void DrucInitRdsLayerName         ();
extern void DrucInitRdsLayerDef          ();
extern void DrucStoreRdsLayer            ();

extern void DrucInitDefineLayerDest      ();
extern void DrucStructDefineOpCompose    ();
extern void DrucInitDefineLayerA         ();
extern void DrucInitDefineLayerB         ();
extern void DrucStoreInstructionDefine   ();

extern void DrucLoadInstructionDefine    ();
extern void DrucLoadInstructionUndefine  ();
extern void DrucInitUndefineUserLayer    ();
extern void DrucStoreInstructionUndefine ();

extern void DrucInitOpUnaire             ();
extern void DrucInitRegleNumero          ();
extern void DrucInitRegleInstruction     ();
extern void DrucInitRegleLayerA          ();
extern void DrucInitRegleLayerB          ();
extern void DrucInitRegleValeur          ();
extern void DrucInitRegleRelation        ();
extern void DrucInitRegleMesure          ();
extern void DrucInitRegleOpCompare       ();
extern void DrucInitDefineValeur         ();
extern void DrucLoadInstructionRegle     ();
extern void DrucStoreInstructionRegle    ();

extern void DrucEcho                     ();
