/******************* Modes des cellules Boundary Scan ************************/
/* valeur du champ MODE   */
#define MODE_IN   1  /* Entree */
#define MODE_OUT  2  /* Sortie 2 etats */
#define MODE_INOUT 3 /* Inout connector */
#define MODE_TRISTATE 4  /* Sortie 3 etats */
#define MODE_COMMAND 5  /* Controle d'une sortie 3 etats */
/****************************************************************************/

/* Liste des instructions Boundary Scan */
typedef struct Instructions {
  struct Instructions *NEXT;
  char *NAME;  /* Nom de l'instruction */
  char *VALUE;  /* codes pour une instruction */
} Instructions;

/* Liste des cellules formant le registre Boundary Scan */
typedef struct Cellules {
  struct Cellules *NEXT;
  char *NAME;  /* Nom de la cellule */
  int  MODE;      /* INPUT, OUTPUT, TRISTATE,INOUTPUT, COMMAND */
  short INVERT;     /*indicateur de cellule inverseuse ou non inverseuse */
  struct Cellules*  COMMAND_CELL;   /*cell COMMAND etablissant la valeur de commande de TRISTATE*/
  char COMMAND_VALUE;               /*valeur pour la mettre en haute impedance TRISTATE*/
} Cellules;

/* Structure de donnees tirees du fichier informations */
typedef struct Infos {
  struct Infos *NEXT;   /*pointeur sur un autre circuit Boudary Scan*/
  Instructions *INST;  /* Liste des instructions */
  Cellules *CELL;  /* Liste des cellules du registre Boundary Scan */
  int INST_SIZE;  /* Longueur du registre instruction */
  int BYPASS_SIZE;
  int BS_SIZE;  /* Longueur du registre Boundary Scan*/
} Infos;

/****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/




