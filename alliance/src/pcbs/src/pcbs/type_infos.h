/*********** Donnees relatives a la cellule Boundary Scan *******************/
#define DMODE 0    /* INPUT, OUTPUT2, OUTPUT3,INOUTPUT, CONTROL et CLOCK */
#define DDEFAUT 1  /* Pour les cellules de mode CONTROL, valeur par default */
           /*pour mettre  en haute impedance l'OUTPUT3 auquel il est rattache*/
#define DCONTROLE 2  /* Si cellule 3 etats, Index de la cellule qui la      */
                     /* controle                                            */
#define DCOMMANDE 3  /* Valeur pour activer la sortie d'un OUTPUT3       */
#define DINVERSION 4 /* Cellule inverseuse (1) ou non inverseuse (0)    */

/******************* Modes des cellules Boundary Scan ************************/
/* valeur du champ donnees[DMODE]   */
#define UNDEF  -1  /* non-defini, equivalent a '*'    */
#define INPUT   0  /* Entree */
#define OUTPUT3 1  /* Sortie 3 etats */
#define OUTPUT2 2  /* Sortie 2 etats */
#define CONTROL 3  /* Controle d'une sortie 3 etats */
#define CLOCK   4  /* Horloge */
#define INOUTPUT 5 /* Inout connector */
/****************************************************************************/
/* Liste des codes de l'instruction Boundary Scan */
typedef struct Codes {
  char *valeur;
  struct Codes *suivant;
} Codes;

/* Liste des instructions Boundary Scan */
typedef struct Instructions {
  char *nom;  /* Nom de l'instruction */
  Codes *code;  /* Liste des codes pour une instruction */
  struct Instructions *suivant;
} Instructions;

/* Liste des cellules formant le registre Boundary Scan */
typedef struct Cellules {
  char *nom;  /* Nom de la cellule */
  int donnees[5];  /* Donnees sur la cellule : 
                      le mode, 
                      valeur par defaut des cellules de type controle, 
                      libelle de la cellule de controle,
                      valeur de commande,
                      indicateur de cellule inverseuse ou non inverseuse */
  struct Cellules *suivant;
} Cellules;

/* Structure de donnees tirees du fichier informations */
typedef struct Infos {
  struct Infos *next;   /*pointeur sur un autre circuit Boudary Scan*/
  int lg_inst;  /* Longueur du registre instruction */
  Instructions *inst;  /* Liste des instructions */
  int lg_bs;  /* Longueur du registre Boundary Scan*/
  Cellules *cel;  /* Liste des cellules du registre Boundary Scan */
} Infos;

/****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/




