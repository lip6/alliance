/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ELP Verison 1                                               */
/*    Fichier : elp104.h                                                   */
/*                                                                          */
/*    (c) copyright 1991-1995 Laboratoire MASI equipe CAO & VLSI            */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Auteur(s) : Payam KIANI                                               */
/*                                                                          */
/****************************************************************************/

/* les inclusions */
#include <stdio.h>
#include <varargs.h>
#include "mut.h"
#include "mlo.h"
#include "rcn.h"

/* les definitions */

#define elperrorFCT  /* identification des fonctions du fichier elperror.c */
#define elpFCT       /* identification des fonctions du fichier elp.c      */

extern char elpLang ;
#define elpTRANSNUM      4         /* nombre de type de transistor     */
#define elpNMOS          0         /* pour les parametres du NMOS      */
#define elpPMOS          1         /* pour les parametres du PMOS      */
#define elpEMES          2         /* pour les parametres du EMES      */
#define elpDMES          3         /* pour les parametres du DMES      */
#define elpSTRINGSIZE    256       /* taille des chaines de caracteres */
#define elpENGLISH      'E'        /* langue anglaise                  */
#define elpFRENCH       'F'        /* langue francaise                 */
#define elpDEFLANG      elpENGLISH /* langue par defaut                */

/* definition du chemin d'acces de quelques fichiers techno */
#define elpECPD10
#define elpECPD12
#define elpECPD15
#define elpAMS12
#define elpHCMOS5

/* les informations sur la technologie utilisee */
extern char   elpTechnoName[elpSTRINGSIZE] ;
extern char   elpTechnoFile[elpSTRINGSIZE] ;
extern double elpTechnoVersion             ;

/* les parametres du shrink du transistor */
#define elpSHRINKNUM 10  /* nombre de parametres du tableau Shrink     */
extern double elpShrink[elpSHRINKNUM] ;
#define elpLMLT      0   /* parametre multiplicatif de la longueur     */
#define elpWMLT      1   /* parametre multiplicatif de la largeur      */
#define elpDLN       2   /* parametre de reduction de la longueur NMOS */
#define elpDWN       3   /* parametre de reduction de la largeur NMOS  */
#define elpDLP       4   /* parametre de reduction de la longueur PMOS */
#define elpDWP       5   /* parametre de reduction de la largeur PMOS  */
#define elpDLE       6   /* parametre de reduction de la longueur EMES */
#define elpDWE       7   /* parametre de reduction de la largeur EMES  */
#define elpDLD       8   /* parametre de reduction de la longueur DMES */
#define elpDWD       9   /* parametre de reduction de la largeur DMES  */

/* les parametres du modele des transistors (modele MCC) */
#define elpMODELNUM 4   /* nombre de parametres pour chaque modele */
extern double elpModel[elpTRANSNUM][elpMODELNUM] ;
#define elpVT       0   /* tension de seuil                        */
#define elpA        1   /* parametre A                             */
#define elpB        2   /* parametre B                             */
#define elpRT       3   /* resistance en mode lineaire             */

/* les parametres definissant les tensions */
#define elpVOLTNUM 4   /* nombre d'elements du tableau Voltage */
extern double elpVoltage[elpVOLTNUM] ;
#define elpVDDMAX  0   /* tension d'alimentation               */
#define elpVDDDEG  1   /* tension du niveau degrade haut       */
#define elpVSSDEG  2   /* tension du niveau degrade bas        */
#define elpVTHR    3   /* seuil de mesure des temps de propa   */

/* les parametres definissant les capacites dynamiques */
#define elpCAPANUM 10  /* nombre de capacites carrees               */
extern double elpCapa[elpTRANSNUM][elpCAPANUM] ;
#define elpCGS     0   /* capacite grille contribution en surface   */
#define elpCGP     1   /* capacite grille contribution en perimetre */
#define elpCDS     2   /* capacite drain contribution en surface    */
#define elpCDP     3   /* capacite drain contribution en perimetre  */
#define elpCDW     4   /* capacite drain contribution en largeur    */
#define elpCSS     5   /* capacite source contribution en surface   */
#define elpCSP     6   /* capacite source contribution en perimetre */
#define elpCSW     7   /* capacite source contribution en largeur   */
#define elpCSGD    8   /* capacite grille drain pour le mesfet      */
#define elpCSGS    9   /* capacite grille source pour le mesfet     */

/* les parametres du simulateur electrique */
extern char   elpEsimName[elpSTRINGSIZE] ;
extern char   elpModelName[elpSTRINGSIZE] ;
extern double elpEsimLevel               ;
#define elpMOS    0   /* type de model de transistor mosfet        */
#define elpMES    1   /* type de model de transistor mesfet        */

/* les parametres generaux */
#define elpGENERALNUM 3  /* nombre de parametres du tableau General    */
extern double elpGeneral[elpGENERALNUM] ;
#define elpACM        0  /* methode de calcul des capacites dynamiques */
#define elpTEMP       1  /* temperature de simulation et d'analyse     */
#define elpSLOPE      2  /* front sur les connecteurs d'entree         */


/* les fonctions externes */
extern int elpenv() ;
extern short elpLoadElp() ;
extern int elpLotrsShrink() ;
extern float elpLotrsCapaDrain() ;
extern float elpLotrsCapaSource() ;
extern float elpLotrsCapaGrid() ;
extern int elpLofigCapaDiff() ;
extern int elpLofigCapaGrid() ;
extern int elpLofigShrink() ;
extern short elpError() ;
