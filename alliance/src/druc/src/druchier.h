/*------------------------------------------------------------\
|                                                             |
| Tool    :                     DRUC                          |
|                                                             |
| File    :                  DRUCHIER.H                       |
|                                                             |
| Authors :                Patrick Renaud                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
| Modifications:  voir fin de fichier                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  DrucHier    Types                          |
|                                                             |
\------------------------------------------------------------*/
typedef struct struct_trans_equiv
               {
                 int LAYER_INDEX;
                 int LAYER_TRANSPARENCE;
               }DrucTypeStructTransEquiv;

typedef struct struct_virtuel_equiv
               {
                 int LAYER_INDEX;
                 int LAYER_VIRTUEL;
               }DrucTypeStructVirtuEquiv;

/*------------------------------------------------------------\
|                                                             |
|                  DrucHier    Defines                        |
|                                                             |
\------------------------------------------------------------*/
#define DRUC_MAX_VIRTUELS                                    1
#define DRUC_MAX_TRANSPARENCES                               4
#define DRUC_BBOX                                    RDS_USER2
/*------------------------------------------------------------\
|                                                             |
|                  DrucHier    Variables                      |
|                                                             |
\------------------------------------------------------------*/
extern DrucTypeStructTransEquiv         TabTransEquivalence [];
extern DrucTypeStructVirtuEquiv         TabVirtuEquivalence [];

/*------------------------------------------------------------\
|                                                             |
|                  DrucHier    Modifications                  |
|                                                             |
| Patrick 31/03/94:                                           |
|   ligne 36                                                  |
|   ajouter extern au tableau DrucTypeStructTransEquiv        |
|                                                             |
\------------------------------------------------------------*/
