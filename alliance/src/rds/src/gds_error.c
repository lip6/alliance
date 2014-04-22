/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSGDS                         |
|                                                             |
| File    :                  gds_error.c                      |
|                                                             |
| Authors :                 Pierre Vittet                     |
|                    Modified by Jacomme Ludovic              |
| Date    :                   04/07/93                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <mut.h>
# include "gds.h"
# include "gds_error.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  int pv_warning;     /* indicateur d'envoi d'avertissement. */
  struct var pv_error;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/*********
 *
 *        GIVE_ERROR
 *
 *  Cette fonction utilise la variable pv_error (struct var) et sert a imprimer
 *  un message d'erreur sur stderr si le programmeur le desire. Son but est
 *  d'essayer de documenter de la facon la plus claire possible une erreur.
 *
 * ENTREE : message a afficher
 *
 * RETOUR : le numero d'erreur
 *        0 si il n'y avait pas d'erreur au depart
 *
 ***/

int  pv_give_error ( message )
char  *message;
{
   register int   error_num = pv_error.v_error;

   if (!error_num)
      return(0);  /* s'il n'y a pas eu d'erreur, on sort immediatement */

   switch (error_num) {
   case ENOMEM :
      (void)fprintf(stderr, "%s : Memoire insuffisante !\n", message);
      break;
   case EIO :
      (void)fprintf(stderr, "%s : Erreur d'entree/sortie !\n", message);
      break;
   case EFTRUNC :
      (void)fprintf(stderr, "%s : Fin de fichier prematuree !\n", message);
      break;
   case ENOFILE :
      (void)fprintf(stderr, "%s : Je n'arrive pas a ouvrir le fichier %s !\n", message, pv_error.v_textp);
      pv_error.v_textp = (char *)NULL; /* etant donne que textp sert a donner le nom du fichier, on evite de    */
      break;         /* le re-imprimer par la suite.                              */
   case ENOSPACE :
      (void)fprintf(stderr, "%s : Il n'y a plus de place sur le disque !\n", message);
      break;
   case ECLOCK :
      (void)fprintf(stderr, "%s : Probleme de lecture de l'horloge interne !\n", message);
      break;
   case EOVERFL :
      (void)fprintf(stderr, "%s : Depassement de capacite !\n", message);
      break;
   case ENOMODEL :
      (void)fprintf(stderr, "%s : Modele inconnu de la base RDS !\n", message);
      break;
   case ENOCODE :
      (void)fprintf(stderr, "%s : Le fichier n'est pas au format voulu !\n", message);
      break;
   case ENORELE :
      (void)fprintf(stderr, "%s : Le fichier correspond a une version trop recente du format !\n", message);
      (void)fprintf(stderr, "Il est grand temps de mettre a jour le programme.\n");
      break;
   case ENODONE :
      (void)fprintf(stderr, "%s : La fonction %s n'est pas terminee.", message, pv_error.v_textp);
      (void)fprintf(stderr, "Il faudrait peut-etre la finir...\n");
      pv_error.v_textp = (char *)NULL;
      break;
   case ENOTHER :
      break;
   default :
      (void)fprintf(stderr, "%s : Erreur non documentee...\n", message);
      (void)fprintf(stderr, "Il faudrait peut-etre mettre a jour les messages d'erreur !\n");
      break;
   }
   if (pv_error.v_textp)                        /* si il y a un texte supplementaire,   */
      (void)fprintf(stderr, "%s\n", pv_error.v_textp); /* on l'imprime.                */

   pv_init_error();              /* une fois qu'on a utilise le contenu de pv_error, on la re-initialise. */
   EXIT(1);
   return(error_num);
}

/*********
*
*         INIT_V
*
*   La fonction est utilisee pour remettre a zero chaque champ de la variable
*   pv_error.
*
***/

void pv_init_error()
{
   pv_error.v_error = 0;
   pv_error.v_textp = (char *)NULL;
}

/*********
*
*  SET_V_ERROR

*
*  Cette fonction, appelee depuis la macro traitebool, ne sert qu'a positionner
*  le champs v_error de la variable v, interne au fichier.
*
***/

void
   pv_set_pv_error( error_number )
int error_number;
{
   pv_init_error();
   switch (error_number) {
   case TRUNCERR :      /* Ici, pas d'erreur d'E/S mais le fichier */
      pv_error.v_error = EFTRUNC; /* n'est pas complet. Voir pv_error */
      break;
   case NOMEMORY :
      pv_error.v_error = ENOMEM;
      break;
   case IOERR :
      pv_error.v_error = EIO;
      break;
   case CODEINCONNU :
      pv_error.v_error = ENOCODE;
      break;
   case ATTRTABLERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur ATTRTABLE";
      break;
   case BGNSTRERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur BGNSTR";
      break;
   case STRTYPERR :
   case STYPTABLERR :
   case TEXTNODERR :
      pv_error.v_error = ENORELE;
      break;
   case MASKERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur MASK";
      break;
   case BOUNDARYERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur BOUNDARY";
      break;
   case PATHERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur PATH";
      break;
   case SREFERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur SREF";
      break;
   case AREFERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur AREF";
      break;
   case TEXTERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur AREF";
      break;
   case NODERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur NODE";
      break;
   case BOXERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur BOX";
      break;
   case REFLIBSERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur REFLIBS";
      break;
   case FONTSERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur FONTS";
      break;
   case STRNAMERR :
      pv_error.v_error = ENOTHER;
      pv_error.v_textp = "Erreur sur STRNAMERR";
      break;
   case FALSE :
   case TRUE :
      break;      /* Ici, rien a faire, v_error a du etre positionne */
   default :
      pv_error.v_error = error_number;
      break;
   }
}

/***********
*
*  EMET_WARNING
*
*  Cette fonction est utilisee pour envoyer des messages d'avertissement a la
*  console sur la sortie standard stderr. Ces messages ont pour but de donner
*  a l'utilisateur des indications lorsque le parser de GDS se trouve en face
*  de problemes insurmontables de traduction mais non bloquants. Par exemple,
*  un cas typique est celui ou un objet GDS ne trouve pas  d'equivalent  dans
*  le contexte RDS (i.e. un "path" ou l'equivalent d'un fil) ou encore si une
*  operation geometrique est appliquee a un objet mais n'existe pas en RDS.
*
***/
void
   pv_emet_warning( Name, texte1, texte2 )
char *Name, *texte1, *texte2;
{
   if (pv_warning)
   {
     if (texte2)
      (void)fprintf(stderr, "%s : %s %s.\n", Name, texte1, texte2);
     else
     (void)fprintf(stderr, "%s : %s.\n", Name, texte1);
   }
}
