/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  Drucutil.h                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   03/07/95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc          Defines                      |
|                                                             |
| Defines pour le code                                        |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_FALSE                                           0
#define DRUC_TRUE                                            1

#define DRUC_MAX_FILE_BUFFER                               127
#define DRUC_MAX_STRING_BUFFER                              80

#define DRUC_DRC_RULES                             "DRC_RULES"
#define DRUC_END_DRC_RULES                     "END_DRC_RULES"

#define DRUC_DRC_COMMENT                          "DRC_COMMENT"
#define DRUC_DRC_END_COMMENT                  "END_DRC_COMMENT"
/*------------------------------------------------------------\
|                                                             |
|                  Druc          Default Names                |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_ERROR_FILE_NAME                      "DRUC_ERROR"
#define DRUC_ERROR_FILE_SUFFIXE                         ".DRC"
#define DRUC_ERROR_FIGURE_SUFFIXE                       "_ERR"
#define DRUC_ERROR_EXTEND                               "_DRC"

/*------------------------------------------------------------\
|                                                             |
|                  Druc Compiler Defines                      |
|                                                             |
| Defines pour les erreurs                                    |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_ERROR_NOT_DECLARED_LAYER                       21
#define DRUC_ERROR_EMPTY_LAYER                              20
#define DRUC_ERROR_NOT_EMPTY_LAYER                          19
#define DRUC_ERROR_FILE_PARAM                               18
#define DRUC_ERROR_READ_PARAM                               17
#define DRUC_ERROR_CLOSE_DRC_FILE                           16
#define DRUC_ERROR_COMMENT_FILE                             15
#define DRUC_ERROR_BAD_COMMENT                              14
#define DRUC_ERROR_MAX_OPERATOR                             13
#define DRUC_ERROR_CHECKSUMM                                12
#define DRUC_ERROR_MEMOIRE                                  11
#define DRUC_ERROR_DEFINE_USER_LAYER                        10
#define DRUC_ERROR_RULE_NUMBER                               9
#define DRUC_ERROR_IN_FILE                                   8
#define DRUC_ERROR_NO_CHECKSUM                               7
#define DRUC_ERROR_NO_RULE_IN_FILE                           6
#define DRUC_ERROR_NO_RULE_FILE                              5
#define DRUC_ERROR_OUT_FILE                                  4
#define DRUC_ERROR_OUT_NAME                                  3
#define DRUC_ERROR_UNDEFINED_LAYER                           2
#define DRUC_ERROR_UNDEFINED                                 1
#define DRUC_GOOD                                            0

/*------------------------------------------------------------\
|                                                             |
|                  Druc          Defines                      |
|                                                             |
| Defines pour les structures                                 |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_UNDEFINED_CHECKSUMM                            -1
#define DRUC_UNDEFINED_CTM                                  -1
#define DRUC_UNDEFINED_FLAGS                                -1
#define DRUC_FLAG_LAYER_INTERDIT                             1
#define DRUC_FLAG_LAYER_LIBRE                                2
#define DRUC_FLAG_LAYER_OCCUPE                               3

#define DRUC_UNDEFINED_LAYER                                -1
#define DRUC_UNDEFINED_WIDTH                                -1
#define DRUC_UNDEFINED_OP_COMPOSE                           -1
#define DRUC_UNDEFINED_REGLE                                -1
#define DRUC_UNDEFINED_INSTRUCTION                          -1
#define DRUC_UNDEFINED_RELATION                             -1
#define DRUC_UNDEFINED_MESURE                               -1
#define DRUC_UNDEFINED_OPERATEUR                            -1
#define DRUC_UNDEFINED_VALEUR                               -1

#define DRUC_MAX_USER_LAYER                                  3
#define DRUC_USER_LAYER_0                    RDS_MAX_LAYER - 4
#define DRUC_USER_LAYER_1                    RDS_MAX_LAYER - 3
#define DRUC_USER_LAYER_2                    RDS_MAX_LAYER - 2
/*\ 
 * RDS_MAX_LAYER -1 correspond a RDS_ABOX
\*/

#define DRUC_NO_ERROR_FILE                                   0
#define DRUC_ERROR_FILE                                      1
#define DRUC_FLATTEN                                         0
#define DRUC_HIERARCHIE                                      1
#define DRUC_SILENT                                          0
#define DRUC_VERBOSE                                         1

/*------------------------------------------------------------\
|                                                             |
|                  Druc  Maximum de mesures                   |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_MAX_MESURES                                     8
#define DRUC_UN_CORPS_TRANS_METAL                         1000
#define DRUC_UN_CORPS_VIRT_METAL                          2000
#define DRUC_DEUX_CORPS_TRANS_METAL                       3000
#define DRUC_DEUX_CORPS_VIRT_METAL                        4000
#define DRUC_DEUX_CORPS_METAL_TRANS                       5000
#define DRUC_DEUX_CORPS_TRANS_TRANS                       6000
#define DRUC_DEUX_CORPS_VIRT_TRANS                        7000
#define DRUC_DEUX_CORPS_METAL_VIRT                        8000
#define DRUC_DEUX_CORPS_TRANS_VIRT                        9000
#define DRUC_DEUX_CORPS_VIRT_VIRT                        10000
#define DRUC_RULES_BASE_ABOX                             11000

/*------------------------------------------------------------\
|                                                             |
|                  Druc          Language                     |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_ASM_BEGIN_LAYER                               128
#define DRUC_ASM_END_LAYER                                 129
#define DRUC_ASM_BEGIN_DEFINE                              130
#define DRUC_ASM_END_DEFINE                                131
#define DRUC_ASM_BEGIN_UNDEFINE                            132
#define DRUC_ASM_END_UNDEFINE                              133
#define DRUC_ASM_BEGIN_REGLE                               134
#define DRUC_ASM_END_REGLE                                 135
#define DRUC_ASM_BEGIN_CHECKSUMM                           136
#define DRUC_ASM_END_CHECKSUMM                             137

#define DRUC_COMPOSE_UNION                                   8
#define DRUC_COMPOSE_INTERSECT                               9
#define DRUC_COMPOSE_EXCLUSION                              10
#define DRUC_COMPOSE_INCLUSION                              11

#define DRUC_COMPOSE_RESIZE                                 12

#define DRUC_INSTR_CARACTERISE                               1
#define DRUC_INSTR_RELATION                                  2

#define DRUC_RELATION_DISTANCE                              16
#define DRUC_RELATION_ENVELOPPE                             17
#define DRUC_RELATION_MARGE                                 18
#define DRUC_RELATION_CROIX                                 19
#define DRUC_RELATION_INTERSECTION                          20
#define DRUC_RELATION_EXTENSION                             21
#define DRUC_RELATION_INCLUSION                             22

#define DRUC_MESURE_SURFACE_INTER                           30
#define DRUC_MESURE_LONGUEUR_INTER                          31
#define DRUC_MESURE_LARGEUR_INTER                           32
#define DRUC_MESURE_PENETRE_INTER                           33
#define DRUC_MESURE_PARALEL_INTER                           34
#define DRUC_MESURE_PERPEND_INTER                           35
#define DRUC_MESURE_LONGUEUR_MIN                            36
#define DRUC_MESURE_LONGUEUR_MAX                            37
#define DRUC_MESURE_LARGEUR_MIN                             38
#define DRUC_MESURE_LARGEUR_MAX                             39
#define DRUC_MESURE_LATERALE_MIN                            40
#define DRUC_MESURE_LATERALE_MAX                            41
#define DRUC_MESURE_FRONTALE                                42
#define DRUC_MESURE_INF                                     43
#define DRUC_MESURE_SUP                                     44
#define DRUC_MESURE_AXIALE                                  45
#define DRUC_MESURE_GEODESIQUE                              46

#define DRUC_MESURE_SURFACE                                 48
#define DRUC_MESURE_LONGUEUR                                49
#define DRUC_MESURE_LARGEUR                                 50
#define DRUC_MESURE_NOTCH                                   51

#define DRUC_OPERATION_MIN                                  64
#define DRUC_OPERATION_MAX                                  65
#define DRUC_OPERATION_INFEQ                                66
#define DRUC_OPERATION_SUPEQ                                67
#define DRUC_OPERATION_EQUAL                                68
#define DRUC_OPERATION_DIFF                                 69
#define DRUC_OPERATION_INF                                  70
#define DRUC_OPERATION_SUP                                  71

/*------------------------------------------------------------\
|                                                             |
|                  Druc          Types                        |
|                                                             |
\------------------------------------------------------------*/
typedef char boolean;

typedef struct struct_instruction
              {
               struct struct_instruction *NEXT;
                int                       INSTRUCTION;
                int                       LAYER_DEST;
                int                       LAYER_1;
                int                       LAYER_2;
                int                       OP_COMPOSE;
                int                       OP_UNAIRE;
                long                      DEF_VALEUR;
                int                       REGLE_NUM  [ DRUC_MAX_MESURES ];
                int                       RELATION   [ DRUC_MAX_MESURES ];
                int                       MESURE     [ DRUC_MAX_MESURES ];
                int                       OP_COMPARE [ DRUC_MAX_MESURES ];
                int                       VALEUR     [ DRUC_MAX_MESURES ];
              } DrucTypeStructInstruction;

typedef struct struct_rdslayer
              {
               int   RDS_INDEX;
               int   WIDTH;
               int   CTM;
               int   FLAGS;
              } DrucTypeStructRdsLayer;

typedef struct struct_stat
              {
                unsigned long  ERROR_NUMBER;
                boolean        FLAG_FLATTEN_MODE;
                boolean        FLAG_VERBOSE_MODE;
                boolean        FLAG_VIEW_RULE_MODE;
                boolean        FILE_ERROR;
              } DrucTypeStructStat;

typedef struct struct_comment
              {
                struct struct_comment *NEXT;
                long                   RULE;
                char                  *COMMENT;
              } DrucTypeStructRuleComment;

/*------------------------------------------------------------\
|                                                             |
|                  Druc          variables                    |
|                                                             |
\------------------------------------------------------------*/
extern DrucTypeStructRdsLayer     DrucStructRdsLayer[];
extern char                      *DrucCompilErrorMessage[];
extern DrucTypeStructInstruction *DrucTabInstruction;
extern int                        DrucNombreInstructions;
extern long                       DrucCtmMax;
extern DrucTypeStructRuleComment *DrucRuleCommentList;

extern char                      *DrucErrorMessage [];

extern DrucTypeStructStat         DrucStructStat;

/*------------------------------------------------------------\
|                                                             |
|                  Druc Variables RDS                         |
|                                                             |
\------------------------------------------------------------*/
/*
 extern char   RDS_LAYER_NAME [] [];
*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc  Macros                               |
|                                                             |
\------------------------------------------------------------*/

#define DRUC_EXIT(Entier)                            \
   do{                                               \
   extern int vrd_y_lineno;                              \
   fprintf ( stdout,                                 \
             "\nline %d DRUC ERROR %d : %s\n",       \
             vrd_y_lineno, Entier,                       \
             DrucCompilErrorMessage [ Entier ]);     \
   EXIT( Entier );                                   \
   } while (0)

/*------------------------------------------------------------\
|                                                             |
|                  Druc Fonctions                             |
|                                                             |
\------------------------------------------------------------*/
extern void  DrucComputeCheckSumm();
extern char *DrucGetRuleComment  ();
extern void  DrucViewRuleComment ();
extern chain_list *getrdswindow ();
extern void DrucStatInit          ();
