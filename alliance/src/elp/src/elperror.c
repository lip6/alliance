/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ELP Verison 1                                               */
/*    Fichier : elperror.c                                                   */
/*                                                                          */
/*    (c) copyright 1991-1995 Laboratoire MASI equipe CAO & VLSI            */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Auteur(s) : Payam KIANI                                               */
/*                                                                          */
/****************************************************************************/
/* traitement des erreur elp                                                */
/****************************************************************************/

#include <stdarg.h>
#include "elp.h"


/*****************************************************************************/
/*                               fonction Error()                            */
/* affichage et gestion des messages. Les erreurs de la bibliotheque ELP n'  */
/* arretent pas l'execution du programme. Ce sont juste des avertissements.  */
/* C'est a celui qui travaille avec de faire ce qu'il en veut!               */
/*                                                                           */
/* Parametres en entree:                                                     */
/* --------------------                                                      */
/*    1) errnum  : le numero de l'erreur (> 1000 pour eviter les conflits).  */
/*    2) va_alist: la fonction est a nombre d'arguments variables. La vari-  */
/*                 able va_alist contient tous les autres arguments que le   */
/*                 numero de l'erreur.                                       */
/*                                                                           */
/* Parametre en sortie:                                                      */
/* -------------------                                                       */
/*    La fonction renvoie le numero de l'erreur.                             */
/*****************************************************************************/
elperrorFCT short elpError(errnum, ...)
short errnum ;
{
va_list arg ;

if(elpLang == elpENGLISH)
fprintf(stderr,"\n!!! elp error: %d !!!\n",errnum) ;
else
fprintf(stderr,"\n!!! erreur elp : %d !!!\n",errnum) ;

va_start(arg, errnum) ;

switch(errnum)
    {
    /* fichier techno ne pouvant pas s'ouvrir */
    case 1000: if(elpLang == elpENGLISH)
               fprintf(stderr,"can not open the file %s\n",
                       va_arg(arg,char *)) ;
               else
               fprintf(stderr,"le fichier %s ne peut pas s'ouvrir\n",
                       va_arg(arg,char *)) ;
               break ;

    /* erreur de syntaxe dans le fichier techno */
    case 1001: if(elpLang == elpENGLISH)
               fprintf(stderr,"syntax error on %s at line %d\n",
                       va_arg(arg,char *),va_arg(arg,int)) ;
               else
               fprintf(stderr,"erreur de syntaxe sur %s ligne %d\n",
                       va_arg(arg,char *),va_arg(arg,int)) ;
               break ;

    /* erreur dans le parser */
    case 1002: if(elpLang == elpENGLISH)
               fprintf(stderr,"loading error: %s\n",va_arg(arg,char *)) ;
               else
               fprintf(stderr,"erreur de chargement: %s\n",
                       va_arg(arg,char *)) ;
               break ;

    /* fichier techno ne pouvant pas etre ferme */
    case 1003: if(elpLang == elpENGLISH)
               fprintf(stderr,"can not close the file %s\n",
                       va_arg(arg,char *)) ;
               else
               fprintf(stderr,"le fichier %s ne peut pas se ferme\n",
                       va_arg(arg,char *)) ;
               break ;

    /* paramtetre obligatoire manquant */
    case 1004: if(elpLang == elpENGLISH)
               fprintf(stderr,"incomplete technology file %s\n",
                       va_arg(arg,char *)) ;
               else
               fprintf(stderr,"fichier techno incomplet %s\n",
                       va_arg(arg,char *)) ;
               break ;

    case 1005: if(elpLang == elpENGLISH)
               fprintf(stderr,"no %s capacitance parameters\n",
                       va_arg(arg,char *)) ;
               else
               fprintf(stderr,"pas de parametre de capacite de %s\n",
                       va_arg(arg,char *)) ;
               break ;
    case 1006: if(elpLang == elpENGLISH)
               fprintf(stderr,"bad transistor model name in %s \n",
                       va_arg(arg,char *)) ;
               else
               fprintf(stderr,"mauvais nom de modele de transistor dans %s\n",
                       va_arg(arg,char *)) ;
               break ;
    }
   va_end(arg);

fflush(stderr) ;
return errnum ;
}
