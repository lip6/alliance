#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSGDS                         |
|                                                             |
| File    :                  gds_error.h                      |
|                                                             |
| Authors :                 Pierre Vittet                     |
|                    Modified by Jacomme Ludovic              |
| Date    :                   04/07/93                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RDSGDS_ERROR
# define RDSGDS_ERROR

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

/* erreurs generales */

#define ENOMEM       1          /* No enough core. */
#define EIO          2          /* IO error. */
#define EFTRUNC      3          /* The file is teuncated somewhere. */
#define ENOFILE      4          /* Cannot open file. */
#define ENOSPACE     5          /* No more space on disk. */
#define ECLOCK       6          /* Cannot read internal clock well. */
#define EOVERFL      7          /* Overflow or underflow. */
#define ENOMODEL     8          /* Unknown model in Rds. */
#define ENODONE      99

/* Programme en cours de finition... Cela permet de definir par exemple l'en-tete d'une */
/* fonction et de ne pas faire son corps. C'est utile si on doit fabriquer une librai-  */
/* rie et que l'on est en retard : n'importe qui peut quand meme inclure cette fonction */
/* dans un executable, il ne se passera rien lors de son appel. Il suffit simplement de */
/* recompiler lorsque la librairie est terminee.                                        */

/* erreurs specifiques aux parser/driver */

#define ENOCODE      101        /* Le code rencontre n'est pas reconnu.
           */
#define ENORELE      102        /* Le code existe bien mais correspond a une version trop recente du format d
e stockage */
                                /* (GDS2, CIF...), dans la version avec laquelle on travaille, ce code etait
envisage   */
                                /* mais pas encore implemente.
           */
#define ENOTHER      255        /* Any other (wierd !) error. */

/*********
*
* Voici la liste des valeurs que peut prendre la variable "error" dans toutes
* les fonctions de lecture qui vont suivre.
* Remarque : "error" correspond a "a" dans la macro traitebool et a "c" dans la macro
* setbool (bien qu'elle y prenne, dans la pratique,  toujours la valeur TRUNCERR).
*
***/

#define TRUNCERR  TRUE + 2 /* Comme ca, on est sur que c'est different de  */
#define NOMEMORY  TRUE + 3 /* la valeur qui peut avoir ete definie pour */
#define IOERR     TRUE + 4 /* TRUE              */
#define CODEINCONNU  TRUE + 5
#define ATTRTABLERR  TRUE + 6
#define BGNSTRERR TRUE + 7
#define STYPTABLERR  TRUE + 8
#define MASKERR      TRUE + 9
#define STRTYPERR TRUE + 10
#define BOUNDARYERR  TRUE + 11
#define PATHERR      TRUE + 12
#define SREFERR      TRUE + 13
#define AREFERR      TRUE + 14
#define TEXTERR      TRUE + 15
#define NODERR    TRUE + 16
#define BOXERR    TRUE + 17
#define REFLIBSERR   TRUE + 18
#define FONTSERR  TRUE + 19
#define TEXTNODERR   TRUE + 20
#define STRNAMERR TRUE + 21

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

/*********
*
*           STRUCT VAR
*
* Cette structure est utilisee pour contenir certaines variables du programme.
*
***/

struct var {
        short v_error;          /* code d'erreur         */
        char *v_textp;          /* pointeur vers un texte quelconque.   */
                                /* Peut, par exemple, etre utilise pour completer un message d'erreur   */
};

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

 extern int pv_warning;
 extern struct var pv_error;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern            int  pv_give_error __P((char *message));
  extern           void  pv_init_error __P(());
  extern           void  pv_set_pv_error __P((int error_number));
  extern           void  pv_emet_warning __P((char *Name, char *texte1, char *texte2));

# endif
