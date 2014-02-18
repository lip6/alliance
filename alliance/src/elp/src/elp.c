/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ELP Verison 1                                               */
/*    Fichier : elp.c                                                       */
/*                                                                          */
/*    (c) copyright 1991-1995 Laboratoire MASI equipe CAO & VLSI            */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Auteur(s) : Payam KIANI                                               */
/*                                                                          */
/****************************************************************************/
/* caracterisation electrique des netlists en fonction des parametres elp   */
/****************************************************************************/

#include <stdlib.h>
#include "elp.h"

extern FILE *elpyyin                                                       ;
extern int   elpyyparse ()                                                 ;
char   elpTechnoName[elpSTRINGSIZE]                                        ;
char   elpTechnoFile[elpSTRINGSIZE]                                        ;
double elpTechnoVersion                                                    ;
char   elpEsimName[elpSTRINGSIZE]                                          ;
char   elpModelName[elpSTRINGSIZE] = "MOS"                                 ;
int    elpModelType                                                        ;
double elpEsimLevel                                                        ;
double elpShrink[elpSHRINKNUM] = {1.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0} ;
double elpModel[elpTRANSNUM][elpMODELNUM]                                  ;
double elpVoltage[elpVOLTNUM]                                              ;
double elpCapa[elpTRANSNUM][elpCAPANUM]                                    ;
double elpGeneral[elpGENERALNUM]           = {0.0,0.0,1000.0}              ;
char   elpLang                             = elpDEFLANG                    ;
int    elpyylineno                                                            ;

/*****************************************************************************/
/*                        function elpenv()                                  */
/* recuperation du nom du fichier elp a partir de la variable:               */
/* ELP_TECHNO_NAME.                                                          */
/*                                                                           */
/* Parametres en entree:                                                     */
/* --------------------                                                      */
/*    Aucun!                                                                */
/*                                                                           */
/* Parametre en sortie:                                                      */
/* -------------------                                                       */
/*    Aucun!                                                                */
/*****************************************************************************/
elpFCT int elpenv()
{
 char *str ;

 str = mbkgetenv("ELP_TECHNO_NAME");
 if(str != NULL)
   strcpy(elpTechnoFile,str) ;
 else 
 {
   str = mbkgetenv( "ALLIANCE_TOP" );
   if ( str == (char *)NULL ) str = ALLIANCE_TOP;
   sprintf( elpTechnoFile, "%s/%s", str, TECHNOLOGY );
 }
 return 0;
}

/*****************************************************************************/
/*                        function LoadElp()                                 */
/* chargement du fichier techno sous forme structure ELP. En cas d'erreur la */
/* fonction affiche un message et renvoie le numero d'erreur. Sinon le code  */
/* de retour est 0.                                                          */
/*                                                                           */
/* Parametres en entree:                                                     */
/* --------------------                                                      */
/*    Aucun!                                                                */
/*                                                                           */
/* Parametre en sortie:                                                      */
/* -------------------                                                       */
/*    La fonction renvoie 0 si le chargement s'est bien effectue, le numero  */
/*    d'erreur sinon.                                                        */
/*****************************************************************************/
elpFCT short elpLoadElp()

{
/* ouverture du fichier techno */
if((elpyyin = fopen(elpTechnoFile,"r")) == NULL)
    {
    elpError(1000,elpTechnoFile) ; /* si fichier pas ouvert => erreur 1000 */
    return 1000 ; /* arret de la procedure elp */
    }

elpyylineno = 1 ;

/* appel au parser pour chargement */
if(elpyyparse() != 0)
    {
    elpError(1002,elpTechnoFile) ; /* si erreur chergement => erreur 1002 */
    return 1002 ; /* arret de la procedure elp */
    }

/* fermeture du fichier techno */
if(fclose(elpyyin) != 0)
elpError(1003,elpTechnoFile) ; /* si fermeture ko => erreur 1003 */

/* type de model du fichier elp */
if(strcmp(elpModelName,"MOS") == 0) elpModelType = elpMOS ;
else if(strcmp(elpModelName,"MES") == 0) elpModelType = elpMES ;
else 
    {
    elpError(1006,elpTechnoFile) ; /* mauvais nom de model => erreur 1006 */
    return 1006 ; /* arret de la procedure elp */
    }
/*-------------------------------------------------------------------*/
/* verification des parametres obligatoires et calcul des parametres */
/* optionnels.                                                       */
/*-------------------------------------------------------------------*/
if( /* verification des parametres obligatoires */
   ((elpModelType == elpMOS) && (
   (elpModel[elpNMOS][elpVT]  == 0.0 ) ||
   (elpModel[elpNMOS][elpA]   == 0.0 ) ||
   (elpModel[elpNMOS][elpB]   == 0.0 ) ||
   (elpModel[elpNMOS][elpRT]  == 0.0 ) ||
   (elpModel[elpPMOS][elpVT]  == 0.0 ) ||
   (elpModel[elpPMOS][elpA]   == 0.0 ) ||
   (elpModel[elpPMOS][elpB]   == 0.0 ) ||
   (elpModel[elpPMOS][elpRT]  == 0.0 ) ||
   (elpVoltage[elpVDDMAX]     == 0.0 ) ||
   (elpCapa[elpNMOS][elpCGS]  == 0.0 ) ||
   (elpCapa[elpNMOS][elpCGP]  == 0.0 ) ||
   (elpCapa[elpPMOS][elpCGS]  == 0.0 ) ||
   (elpCapa[elpPMOS][elpCGP]  == 0.0 ) ||
   (elpEsimName[0]            == '\0') ||
   (elpEsimLevel              == 0.0 ) ||
   (elpGeneral[elpTEMP]       == 0.0 ))) ||
   ((elpModelType == elpMES) && (
   (elpVoltage[elpVDDMAX]     == 0.0 ) ||
   (elpCapa[elpEMES][elpCSGD] == 0.0 ) ||
   (elpCapa[elpEMES][elpCSGS] == 0.0 ) ||
   (elpCapa[elpDMES][elpCSGD] == 0.0 ) ||
   (elpCapa[elpDMES][elpCSGS] == 0.0 ) ||
   (elpEsimName[0]            == '\0') ||
   (elpEsimLevel              == 0.0 ) ||
   (elpGeneral[elpTEMP]       == 0.0 )))
  )
    {
    elpError(1004,elpTechnoFile) ; /* parametre manquant => erreur 1004 */
    return 1004 ; /* arret de la procedure elp */
    }

/* calcul des parametres optionnels ne se trouvant pas dans le fichier tec */
/*-------------------------------------------------------------------------*/
/* les niveaux degrades par defaut: VSSdeg = 1.5*VTP                       */
/*                                  VDDdeg = VDDmax-1.5*VTN                */
/* tension de seuil de calcul     : VTHR   = VDDmax / 2                    */
/*-------------------------------------------------------------------------*/
if(elpVoltage[elpVSSDEG] == 0.0)
elpVoltage[elpVSSDEG] = 1.5*elpModel[elpPMOS][elpVT] ;

if(elpVoltage[elpVDDDEG] == 0.0)
elpVoltage[elpVDDDEG] = elpVoltage[elpVDDMAX] - 1.5*elpModel[elpNMOS][elpVT] ;

if(elpVoltage[elpVTHR] == 0.0)
elpVoltage[elpVTHR] = elpVoltage[elpVDDMAX] / 2 ;

return 0 ; /* retourne 0 si tout s'est bien passe */
}


/****************************************************************************/
/*                           fonction LotrsShrink()                         */
/* modifie les dimensions du transistor en fonction des parametres de       */
/* shrink et du parametre ACM.                                              */
/*                                                                          */
/* Parametres en entree:                                                    */
/* --------------------                                                     */
/*    1) lotrs: transistor logique MBK dont on veut modifier les dimensions */
/*                                                                          */
/* Parametre en sortie:                                                     */
/* -------------------                                                      */
/*    Aucun!                                                                */
/****************************************************************************/
elpFCT int elpLotrsShrink(lotrs)
struct lotrs *lotrs ;

{
double deltal ,
       deltaw ;

if(lotrs->TYPE == TRANSN)
    {
     if(elpModelType == elpMOS)
       {
        deltal = elpShrink[elpDLN] * (double)SCALE_X ;
        deltaw = elpShrink[elpDWN] * (double)SCALE_X ;
       }
     else
       {
        deltal = elpShrink[elpDLE] * (double)SCALE_X ;
        deltaw = elpShrink[elpDWE] * (double)SCALE_X ;
       }
    }
else
    {
     if(elpModelType == elpMOS)
       {
        deltal = elpShrink[elpDLP] * (double)SCALE_X ;
        deltaw = elpShrink[elpDWP] * (double)SCALE_X ;
       }
     else
       {
        deltal = elpShrink[elpDLD] * (double)SCALE_X ;
        deltaw = elpShrink[elpDWD] * (double)SCALE_X ;
       }
    }

/* modification de la longueur */
lotrs->LENGTH = (long)(elpShrink[elpLMLT]*(double)lotrs->LENGTH + deltal) ;

/* modification de la largeur */
lotrs->WIDTH = (long)(elpShrink[elpWMLT]*(double)lotrs->WIDTH + deltaw) ;

/*--------------------------------------------------------------------------*/
/* si la largeur est modifiee, il faut egalement modifier les largeurs      */
/* laterales XS et XD pour la multiplication de XS ou de XD par la largeur  */
/* ne bouge pas. En effet cette multiplication donne l'aire du drain ou de  */
/* la source qui n'est absolument pas affectee par les parametres de shrink.*/
/*--------------------------------------------------------------------------*/
lotrs->XS = (long)((double)lotrs->XS*((double)lotrs->WIDTH-deltaw)/((double)lotrs->WIDTH*elpShrink[elpWMLT]));
lotrs->XD = (long)((double)lotrs->XD*((double)lotrs->WIDTH-deltaw)/((double)lotrs->WIDTH*elpShrink[elpWMLT]));
/*--------------------------------------------------------------------------*/
/* pour ce qui est de l'aire et du perimetre du transistor. Si le parametre */
/* ACM est a 1, l'aire et le perimetre du transistor sont calcules unique-  */
/* ment en fonction de la largeur (quelles que soient leurs valeurs initi-  */
/* ales. Dans ce cas:                                                       */
/*                                                                          */
/*    Aire      = Weffectif*WMLT                                            */
/*    Perimetre = Weffectif                                                 */
/*                                                                          */
/* Or ce qu'on a dans le transistor logique, ce n'est pas la surface, mais  */
/* la largeur laterale X, telle que Aire = X*Weffectif. La transformation   */
/* dans le cas ACM=1 est donc:                                              */
/*                                                                          */
/*    X         = WMLT*SCALE_X                                              */
/*    Perimetre = Weffectif                                                 */
/*--------------------------------------------------------------------------*/
if(elpGeneral[elpACM] == 1.0)
    {
    lotrs->XS = lotrs->XD = (long)(elpShrink[elpWMLT] * (double)SCALE_X) ;
    lotrs->PS = lotrs->PD = lotrs->WIDTH ;
    }

return 0;
}


/****************************************************************************/
/*                           fonction LotrsCapaDrain()                      */
/* calcule la capacite de drain d'un transistor MBK passe en parametre.     */
/*                                                                          */
/* Parametres en entree:                                                    */
/* --------------------                                                     */
/*    1) lotrs: transistor logique MBK dont on veut calculer la capacite de */
/*              drain.                                                      */
/*                                                                          */
/* Parametre en sortie:                                                     */
/* -------------------                                                      */
/*    la fonction renvoie la capcite du drain du transistor en picoF.       */
/****************************************************************************/
elpFCT float elpLotrsCapaDrain(lotrs)
struct lotrs *lotrs ;

{
short lotrstype ; /* variable indiquant le type du transistor a traiter */

/*---------------------------------------------------------------------------*/
/* Methode de calcul des capacites: si le perimetre est manquant, la contri- */
/* bution en perimetre sera remplacee par la contribution en largeur. Sinon, */
/* seul le perimetre sera pris en compte. La variable 'lotrstype' contiendra */
/* le type du transistor qu'on traite.                                       */
/*---------------------------------------------------------------------------*/
if(elpModelType == elpMOS)
{
lotrstype = (lotrs->TYPE == TRANSN) ? elpNMOS : elpPMOS ;
return (float)(
/* contribution en surface   */
((double)lotrs->XD*(double)lotrs->WIDTH / (double)(SCALE_X*SCALE_X)) 
* elpCapa[lotrstype][elpCDS] +
/* contribution en perimetre */
((double)lotrs->PD / (double)SCALE_X ) * elpCapa[lotrstype][elpCDP] +
/* contribution en largeur   */
((double)lotrs->WIDTH / (double)SCALE_X ) * elpCapa[lotrstype][elpCDW]) ;
}
else
{
lotrstype = (lotrs->TYPE == TRANSN) ? elpEMES : elpDMES ;
if(lotrs->GRID->SIG != lotrs->DRAIN->SIG)
return(float)(
/* contribution en surface   */
((double)lotrs->WIDTH*(double)lotrs->LENGTH / (double)(SCALE_X*SCALE_X))
*elpCapa[lotrstype][elpCSGD]) ; 
else return(float)(0.0);
}

}


/****************************************************************************/
/*                           fonction LotrsCapaSource()                     */
/* calcule la capacite de source d'un transistor MBK passe en parametre.    */
/*                                                                          */
/* Parametres en entree:                                                    */
/* --------------------                                                     */
/*    1) lotrs: transistor logique MBK dont on veut calculer la capacite de */
/*              source.                                                     */
/*                                                                          */
/* Parametre en sortie:                                                     */
/* -------------------                                                      */
/*    la fonction renvoie la capcite de la source du transistor en picoF.   */
/****************************************************************************/
elpFCT float elpLotrsCapaSource(lotrs)
struct lotrs *lotrs ;

{
short lotrstype ; /* variable indiquant le type du transistor a traiter */

/*---------------------------------------------------------------------------*/
/* Methode de calcul des capacites: si le perimetre est manquant, la contri- */
/* bution en perimetre sera remplacee par la contribution en largeur. Sinon, */
/* seul le perimetre sera pris en compte. La variable 'lotrstype' contiendra */
/* le type du transistor qu'on traite.                                       */
/*---------------------------------------------------------------------------*/
if(elpModelType == elpMOS)
{
lotrstype = (lotrs->TYPE == TRANSN) ? elpNMOS : elpPMOS ;
return (float)(
/* contribution en surface   */
((double)lotrs->XS*(double)lotrs->WIDTH / (double)(SCALE_X*SCALE_X)) * elpCapa[lotrstype][elpCSS] +
/* contribution en perimetre */
((double)lotrs->PS / (double)SCALE_X ) * elpCapa[lotrstype][elpCSP] +
/* contribution en largeur   */
((double)lotrs->WIDTH / (double)SCALE_X ) * elpCapa[lotrstype][elpCSW]) ;
}
else
{
lotrstype = (lotrs->TYPE == TRANSN) ? elpEMES : elpDMES ;
if(lotrs->GRID->SIG != lotrs->SOURCE->SIG)
return(float)(
/* contribution en surface   */
((double)lotrs->WIDTH*(double)lotrs->LENGTH / (double)(SCALE_X*SCALE_X))
*elpCapa[lotrstype][elpCSGS]) ; 
else return(float)(0.0);
}

}


/****************************************************************************/
/*                           fonction LotrsCapaGrid()                       */
/* calcule la capacite de grille d'un transistor MBK passe en parametre.    */
/*                                                                          */
/* Parametres en entree:                                                    */
/* --------------------                                                     */
/*    1) lotrs: transistor logique MBK dont on veut calculer la capacite de */
/*              grille.                                                     */
/*                                                                          */
/* Parametre en sortie:                                                     */
/* -------------------                                                      */
/*    la fonction renvoie la capcite de la grille du transistor en picoF.   */
/****************************************************************************/
elpFCT float elpLotrsCapaGrid(lotrs)
struct lotrs *lotrs ;

{
/* variable indiquant le type du transistor */
short lotrstype ;

/* les parametres CGxx sont obligatoires et ne sont donc pas manquant */

if(elpModelType == elpMOS)
{
lotrstype = (lotrs->TYPE == TRANSN) ? elpNMOS : elpPMOS ;
return(float)(
/* contribution en surface   */
((double)lotrs->WIDTH*(double)lotrs->LENGTH / (double)(SCALE_X*SCALE_X))
*elpCapa[lotrstype][elpCGS] +
/* contribution en perimetre */
(2.0*(double)lotrs->WIDTH / (double)SCALE_X)*elpCapa[lotrstype][elpCGP]) ;
}
else
{
double cgd = 0;
double cgs = 0;
lotrstype = (lotrs->TYPE == TRANSN) ? elpEMES : elpDMES ;
if(lotrs->GRID->SIG != lotrs->DRAIN->SIG) cgd = elpCapa[lotrstype][elpCSGD];
if(lotrs->GRID->SIG != lotrs->SOURCE->SIG) cgs = elpCapa[lotrstype][elpCSGS];
return(float)(
/* contribution en surface   */
((double)lotrs->WIDTH*(double)lotrs->LENGTH / (double)(SCALE_X*SCALE_X))
*(cgd + cgs)) ;
}

}


/****************************************************************************/
/*                    fonction LofigCapaDiff()                              */
/* calcule les capas de diffusion des transistors de la figure lofig        */
/*                                                                          */
/* Parametres en entree:                                                    */
/* --------------------                                                     */
/*    1) lofig: figure logique MBK dont dans laquelle il faut ajouter les   */
/*              capas de diffusion.                                         */
/*                                                                          */
/* Parametre en sortie:                                                     */
/* -------------------                                                      */
/*    Aucun!                                                                */
/****************************************************************************/
elpFCT int elpLofigCapaDiff(lofig)
struct lofig*lofig ;

{
lotrs_list *lotrs ;

/* si les parametres CDxx ou CSxx sont manquants => erreur 1005 */
if(elpModelType == elpMOS)
  {
   if((
      (elpCapa[elpNMOS][elpCDS] == 0.0) &&
      (elpCapa[elpNMOS][elpCDP] == 0.0) &&
      (elpCapa[elpNMOS][elpCDW] == 0.0) &&
      (elpCapa[elpPMOS][elpCDS] == 0.0) &&
      (elpCapa[elpPMOS][elpCDP] == 0.0) &&
      (elpCapa[elpPMOS][elpCDW] == 0.0)
     ) || 
     (
      (elpCapa[elpNMOS][elpCSS] == 0.0) &&
      (elpCapa[elpNMOS][elpCSP] == 0.0) &&
      (elpCapa[elpNMOS][elpCSW] == 0.0) &&
      (elpCapa[elpPMOS][elpCSS] == 0.0) &&
      (elpCapa[elpPMOS][elpCSP] == 0.0) &&
      (elpCapa[elpPMOS][elpCSW] == 0.0)
     ))
       {
       elpError(1005,"diffusion") ;
       return 0 ;
       }
    }

for(lotrs = lofig->LOTRS ; lotrs != NULL ; lotrs = lotrs->NEXT)
if((lotrs->PS != 0.0) && (lotrs->XS != 0.0))
    {
      if( ! lotrs->DRAIN->SIG->PRCN )
        addlorcnet( lotrs->DRAIN->SIG );
      if( ! lotrs->SOURCE->SIG->PRCN )
        addlorcnet( lotrs->SOURCE->SIG );
      addcapa( lotrs->DRAIN->SIG, elpLotrsCapaDrain(lotrs) ) ;
      addcapa( lotrs->SOURCE->SIG, elpLotrsCapaSource(lotrs) ) ;
    }

return 0;
}


/****************************************************************************/
/*                    fonction LofigCapaGrid()                              */
/* calcule les capas de grille.                                             */
/*                                                                          */
/* Parametres en entree:                                                    */
/* --------------------                                                     */
/*    1) lofig: figure logique MBK dont dans laquelle il faut ajouter les   */
/*              capas de grille.                                            */
/*                                                                          */
/* Parametre en sortie:                                                     */
/* -------------------                                                      */
/*    Aucun!                                                                */
/****************************************************************************/
elpFCT int elpLofigCapaGrid(lofig)
struct lofig *lofig ;

{
lotrs_list *lotrs  ;

for(lotrs = lofig->LOTRS ; lotrs != NULL ; lotrs = lotrs->NEXT)
{
  if( ! lotrs->GRID->SIG->PRCN )
    addlorcnet( lotrs->GRID->SIG );
  
  addcapa( lotrs->GRID->SIG, elpLotrsCapaGrid(lotrs) );
}
return 0;
}


/****************************************************************************/
/*                        fonction LofigShrink()                            */
/* modifie les dimensions de tous les transistors en fonction des parametres*/
/* de shrink.                                                               */
/*                                                                          */
/* Parametres en entree:                                                    */
/* --------------------                                                     */
/*    1) lofig: figure logique MBK dont dans laquelle il faut modifier les  */
/*              dimensions des transistors.                                 */
/*                                                                          */
/* Parametre en sortie:                                                     */
/* -------------------                                                      */
/*    Aucun!                                                                */
/****************************************************************************/
elpFCT int elpLofigShrink(lofig)
struct lofig *lofig ;

{
lotrs_list *lotrs  ;


for(lotrs = lofig->LOTRS ; lotrs != NULL ; lotrs = lotrs->NEXT)
elpLotrsShrink(lotrs) ;

return 0;
}
