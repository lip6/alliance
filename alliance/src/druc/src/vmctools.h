/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  VMCTOOLS.H                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
| Definition des champs de flags utilises par DRUC            |
| c'est les bits a 1
\------------------------------------------------------------*/
/*\
 *    DRUC_ALL_FLAGS                         (long)0x0FF00000l
\*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
| Definition des flags des rectangles pour l'unification      |
\------------------------------------------------------------*/
#define DRUC_FLAG_SHORT_RECTANGLE            (long)0x00400000l
#define DRUC_FLAG_INCLUSION_RECTANGLE        (long)0x00200000l
#define DRUC_FLAG_INTERSECT_RECTANGLE        (long)0x00100000l
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
| Definition des flags des rectangles pour leur origine       |
\------------------------------------------------------------*/
#define DRUC_FLAG_INSTANCE_RECTANGLE         (long)0x00800000l
#define DRUC_FLAG_FIGURE_RECTANGLE           (long)0x00000000l
#define DRUC_MASK_FIGURE_RECTANGLE           (long)0x00700000l
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
| Definition des flags des rectangles pour leur composition   |
\------------------------------------------------------------*/
#define DRUC_FLAG_REC_INCLUS                 (long)0x02000000l
#define DRUC_FLAG_REMOVE                     (long)0x08000000l
#define DRUC_FLAG_REC_UNIFIE                 (long)0x01000000l
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
| Definition des flags des rectangles pour leurs parcours     |
\------------------------------------------------------------*/
#define DRUC_FLAG_REC_VISITE                 (long)0x10000000l
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
| Definition des flags des rectangles pour leurs parcours     |
\------------------------------------------------------------*/
#define DRUC_FLAG_REC_VERIFIE                (long)0x20000000l
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
| Definition des flags des rectangles equipotentiels          |
\------------------------------------------------------------*/
#define DRUC_FLAG_REC_ISEQUI                 (long)0x04000000l
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Defines                      |
\------------------------------------------------------------*/
#define DRUC_NOFLAGS                         (long)0x00000000l
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools MACROS                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcMesure Orientations                |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_ORIENTATION_ENVELOPPE                           0
#define DRUC_ORIENTATION_MARGE                               1
#define DRUC_ORIENTATION_CROIX                               2
#define DRUC_ORIENTATION_INTERSECTION                        3
#define DRUC_ORIENTATION_EXTENSION                           4
#define DRUC_ORIENTATION_INCLUSION                           5

/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcMesure Position de Rectangles      |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_RECTANGLE_UP                                    0
#define DRUC_RECTANGLE_DO                                    1
#define DRUC_RECTANGLE_LE                                    2
#define DRUC_RECTANGLE_RI                                    3
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcRelat  Macros des mesures          |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_RELATION_UNDEF_MESURE                  0xffffffffl
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcMesur  Macros                      |
| d'acces a la structure DrucStructMesure                     |
|                                                             |
\------------------------------------------------------------*/
#define MACRO_RECTANGLE1          (DrucStructMesure->RECTANGLE1)
#define MACRO_RECTANGLE2          (DrucStructMesure->RECTANGLE2)
#define MACRO_ORIENTATION        (DrucStructMesure->ORIENTATION)
#define MACRO_TOPOLOGIE            (DrucStructMesure->TOPOLOGIE)

#define MACRO_BOOL_DISTANCE (DrucStructMesure->BOOLEEN_DISTANCE)

#define MACRO_SURFACE_INTER_1    (DrucStructMesure.DIMENSION_A)
#define MACRO_LONG_INTER_1       (DrucStructMesure.DIMENSION_B)
#define MACRO_LARGE_INTER_1      (DrucStructMesure.DIMENSION_C)
#define MACRO_SURFACE_INTER      (DrucStructMesure->DIMENSION_A)
#define MACRO_LONG_INTER         (DrucStructMesure->DIMENSION_B)
#define MACRO_LARGE_INTER        (DrucStructMesure->DIMENSION_C)
#define MACRO_PENETRE_INTER      (DrucStructMesure->DIMENSION_D)
#define MACRO_PARAL_INTER        (DrucStructMesure->DIMENSION_G)
#define MACRO_PERPEN_INTER       (DrucStructMesure->DIMENSION_H)

#define MACRO_LONG_MAX           (DrucStructMesure->DIMENSION_E)
#define MACRO_LONG_MIN           (DrucStructMesure->DIMENSION_F)

#define MACRO_RECT_LONG_MAX      (DrucStructMesure->NUM_RECT_E)
#define MACRO_RECT_LONG_MIN      (DrucStructMesure->NUM_RECT_F)

#define MACRO_LARGE_MAX          (DrucStructMesure->DIMENSION_G)
#define MACRO_LARGE_MIN          (DrucStructMesure->DIMENSION_H)

#define MACRO_RECT_LARGE_MAX     (DrucStructMesure->NUM_RECT_G)
#define MACRO_RECT_LARGE_MIN     (DrucStructMesure->NUM_RECT_H)

#define MACRO_FRONT              (DrucStructMesure->DIMENSION_I)
#define MACRO_LATER_MIN          (DrucStructMesure->DIMENSION_J)
#define MACRO_LATER_MAX          (DrucStructMesure->DIMENSION_K)

#define MACRO_RECT_FRONT         (DrucStructMesure->NUM_RECT_I)
#define MACRO_RECT_LATER_MIN     (DrucStructMesure->NUM_RECT_J)
#define MACRO_RECT_LATER_MAX     (DrucStructMesure->NUM_RECT_K)

#define MACRO_SUP                (DrucStructMesure->DIMENSION_L)
#define MACRO_INF                (DrucStructMesure->DIMENSION_M)

#define MACRO_RECT_SUP           (DrucStructMesure->NUM_RECT_L)
#define MACRO_RECT_INF           (DrucStructMesure->NUM_RECT_M)

#define MACRO_LONG_DISTANCE      (DrucStructMesure->DIMENSION_N)
#define MACRO_LARGE_DISTANCE     (DrucStructMesure->DIMENSION_O)

#define MACRO_POSX_DISTANCE      (DrucStructMesure->DIMENSION_P)
#define MACRO_POSY_DISTANCE      (DrucStructMesure->DIMENSION_Q)
#define MACRO_POSDX_DISTANCE     (DrucStructMesure->DIMENSION_R)
#define MACRO_POSDY_DISTANCE     (DrucStructMesure->DIMENSION_S)

#define MACRO_POSX_RECT_UP      (DrucStructMesure->POSX_RECT_UP)
#define MACRO_POSY_RECT_UP      (DrucStructMesure->POSY_RECT_UP)
#define MACRO_POSDX_RECT_UP    (DrucStructMesure->POSDX_RECT_UP)
#define MACRO_POSDY_RECT_UP    (DrucStructMesure->POSDY_RECT_UP)

#define MACRO_POSX_RECT_DO      (DrucStructMesure->POSX_RECT_DO)
#define MACRO_POSY_RECT_DO      (DrucStructMesure->POSY_RECT_DO)
#define MACRO_POSDX_RECT_DO    (DrucStructMesure->POSDX_RECT_DO)
#define MACRO_POSDY_RECT_DO    (DrucStructMesure->POSDY_RECT_DO)

#define MACRO_POSX_RECT_LE      (DrucStructMesure->POSX_RECT_LE)
#define MACRO_POSY_RECT_LE      (DrucStructMesure->POSY_RECT_LE)
#define MACRO_POSDX_RECT_LE    (DrucStructMesure->POSDX_RECT_LE)
#define MACRO_POSDY_RECT_LE    (DrucStructMesure->POSDY_RECT_LE)

#define MACRO_POSX_RECT_RI      (DrucStructMesure->POSX_RECT_RI)
#define MACRO_POSY_RECT_RI      (DrucStructMesure->POSY_RECT_RI)
#define MACRO_POSDX_RECT_RI    (DrucStructMesure->POSDX_RECT_RI)
#define MACRO_POSDY_RECT_RI    (DrucStructMesure->POSDY_RECT_RI)

/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools MACROS                       |
| liees a rds                                                 |
\------------------------------------------------------------*/
#define DRUC_ON_HEAP                                         1
/*------------------------------------------------------------\
|                                                             |
|                  Min et Max de 3 valeurs                    |
|                                                             |
\------------------------------------------------------------*/
#define     MACRO_MAX( Valeur1 , Valeur2 )  \
                  (( Valeur1 < Valeur2 ) ? Valeur2 : Valeur1 )
#define     MACRO_MIN( Valeur1 , Valeur2 )  \
                  (( Valeur2 < Valeur1 ) ? Valeur2 : Valeur1 )

/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools MACROS                       |
| manipulation de la structure complementaire de rectangle    |
\------------------------------------------------------------*/
#define MACRO_PLUS_TAILLE (sizeof( DrucTypeStructPlusRect ))

#define MACRO_REGLE_NUM( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NOMBRE)
#define MACRO_LAYER_NUM( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NOMBRE)

#define MACRO_INSTANCE_NUM( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->INSTANCE_NUM)

#define MACRO_NEXT_EQUI( Rectangle )                          \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NEXT_EQUI)

#define MACRO_NEXT_DEFINE( Rectangle )                       \
    (((DrucTypeStructPlusRect *)((char *)(Rectangle)+sizeof(rdsrec_list)))->NEXT_DEFINE)
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools MACROS                       |
| manipulation de la structure des recles de verif            |
\------------------------------------------------------------*/
#define MACRO_REGLE_COURANTE (DrucTabInstruction [ InstructionCourante ])
#define MACRO_REGLE_VALEUR   (DrucInstructionCourante->VALEUR)
#define MACRO_REGLE_COMPARE  (DrucInstructionCourante->OP_COMPARE)
#define MACRO_REGLE_COMPOSE  (DrucInstructionCourante->OP_COMPOSE)
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools MACROS                       |
| manipulation des rectangles de la structure de fenetre      |
\------------------------------------------------------------*/
#define MACRO_RECT_DEST              (TabRectangleDest->RECTAB)
#define MACRO_RECT_SOURCE          (TabRectangleSource->RECTAB)
#define MACRO_RECT_SOURCE2        (TabRectangleSource2->RECTAB)
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Types                        |
|                                                             |
\------------------------------------------------------------*/
typedef struct struct_plusrect
              {
                int                     NOMBRE;
                rdsins_list            *INSTANCE_NUM;
                rdsrec_list            *NEXT_EQUI;
                struct struct_plusrect *NEXT_DEFINE;
              } DrucTypeStructPlusRect;
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools variables                    |
|                                                             |
\------------------------------------------------------------*/
extern rdsfig_list *DrucFigureRdsErreur;
/*------------------------------------------------------------\
|                                                             |
|                  Druc Regles a 2 Corps TYPE                 |
|                                                             |
\------------------------------------------------------------*/
typedef struct struct_mesure
        {
          boolean      BOOLEEN_DISTANCE;
          long         LAYER_1;
          long         LAYER_2;
          rdsrec_list *RECTANGLE1;
          rdsrec_list *RECTANGLE2;
          int          ORIENTATION;
          int          TOPOLOGIE;
          long         DIMENSION_A;
          long         DIMENSION_B;
          long         DIMENSION_C;
          long         DIMENSION_D;
          long         DIMENSION_E;
          long         DIMENSION_F;
          long         DIMENSION_G;
          long         DIMENSION_H;
          long         DIMENSION_I;
          long         DIMENSION_J;
          long         DIMENSION_K;
          long         DIMENSION_L;
          long         DIMENSION_M;
          long         DIMENSION_N;
          long         DIMENSION_O;
          long         DIMENSION_P;
          long         DIMENSION_Q;
          long         DIMENSION_R;
          long         DIMENSION_S;
          int          NUM_RECT_A;
          int          NUM_RECT_B;
          int          NUM_RECT_C;
          int          NUM_RECT_D;
          int          NUM_RECT_E;
          int          NUM_RECT_F;
          int          NUM_RECT_G;
          int          NUM_RECT_H;
          int          NUM_RECT_I;
          int          NUM_RECT_J;
          int          NUM_RECT_K;
          int          NUM_RECT_L;
          int          NUM_RECT_M;
          int          NUM_RECT_N;
          int          NUM_RECT_O;
          int          NUM_RECT_P;
          int          NUM_RECT_Q;
          int          NUM_RECT_R;
          int          NUM_RECT_S;
          long         MESURE_EFFECTIVE;
          long         POSX_RECT_UP;
          long         POSY_RECT_UP;
          long         POSDX_RECT_UP;
          long         POSDY_RECT_UP;
          long         POSX_RECT_DO;
          long         POSY_RECT_DO;
          long         POSDX_RECT_DO;
          long         POSDY_RECT_DO;
          long         POSX_RECT_LE;
          long         POSY_RECT_LE;
          long         POSDX_RECT_LE;
          long         POSDY_RECT_LE;
          long         POSX_RECT_RI;
          long         POSY_RECT_RI;
          long         POSDX_RECT_RI;
          long         POSDY_RECT_RI;
        } DrucTypeStructMesure;
/*------------------------------------------------------------\
|                                                             |
|                  Druc VmcTools Fonctions                    |
|                                                             |
\------------------------------------------------------------*/
extern void            DrucClearFlag           ();
extern void            DrucComputeEqui         ();
extern rdsrecwin_list *DrucChercheWindow       ();
extern boolean         DrucIsRectangleEquipot  ();
extern boolean         DrucIsRectanglesValides ();
extern void            DrucSetEquiFlag         ();
extern void            DrucClearEquiFlag       ();
extern void            DrucAddRecFigure        ();
extern void            DrucViewString          ();
extern char           *DrucAddSuffixeName      ();
extern void            DrucVerifyEqui          ();
extern void            DrucVerifyEquiLayer     ();
