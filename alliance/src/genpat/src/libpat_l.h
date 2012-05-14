/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/***************************************************************/
/* module   : libpat_l.h				       */
/* date     : 12/04/92                                         */
/* auteurs  : RENAUD & VENOT                                   */
/***************************************************************/

/* $Id: libpat_l.h,v 1.3 2012/05/14 14:20:14 alliance Exp $ */

#ifndef LIBPAT_L_H
#define LIBPAT_L_H

/*------------------------------------------------------------
 *      CONSTANTES
 *------------------------------------------------------------*/
#define GNP_MAXPILE_PARAM   40
/* sp_.. pointe#include MUT_H tjrs sur le suivant */
#define GNP_PARAM_COURANT (sp_file_param - 1) 
#define GNP_MAXTAB_IDENT   4096
#define GNP_MAXTAB_PAIOL   4096
#define GNP_NO_DUPLICATE    -1
#define GNP_NOT_FOUND       -1
#define GNP_ACTIF 1
#define GNP_INACTIF 0
#define GNP_VECTEUR_VIRTUEL 1
#define GNP_VECTEUR_EFFECTIF 0
#define GNP_UP 1
#define GNP_DO -1

/*------------------------------------------------------------
 *      STRUCTURE PAT
 *------------------------------------------------------------*/
#define racine_pagrp  (racine_paseq->PAGRP)
#define racine_paiol  (racine_paseq->PAIOL)
#define racine_papat  (racine_paseq->CURPAT)
#define racine_pacom  (racine_paseq->CURCOM)
#define racine_paevt  (racine_paseq->CURPAT->PAEVT)
#define racine_paini  (racine_paseq->CURPAT->PAINI)
#define PATNUMBER     (racine_paseq->PATNBR)
#define IOLNUMBER     (racine_paseq->IOLNBR)

extern struct  paseq  *racine_paseq;
/*------------------------------------------------------------
 *      NUMERO DE PATTERN COURANT
 *------------------------------------------------------------*/
extern unsigned long GNP_PATTERN_COURANT;
/*------------------------------------------------------------
 *       TAILLE DU MAX. LABEL 
 *------------------------------------------------------------*/
extern unsigned int   MAX_LABEL;
/*------------------------------------------------------------
 *      TABLE DES PAIOLs
 *------------------------------------------------------------*/
extern struct paiol *tab_paiol[];
extern int      GNP_PAIOL_INDEX;
extern int GNP_TAB_PAIOL_CREE;
/*------------------------------------------------------------
 *      GESTION DES PARAMETRES A NOMBRE INDETERMINE
 *------------------------------------------------------------*/
struct  alpha_num {
                   char gauche[6];
                   char droite[6];
                   char alpha[7];
                  };
extern char    *pile_param[];
extern int      sp_pile_param ;
extern int      sp_file_param ;
extern FILE    *fp;
/*------------------------------------------------------------
 *      TABLE DES IDENTIFICATEURS
 *------------------------------------------------------------*/
typedef struct {
          int duplicate;
          int num_paiol;
          void * pointeur;
          } Tptab_ident;

extern Tptab_ident tab_ident[];
extern int     ind_tab_ident;

/*------------------------------------------------------------*
 *      CONSTANTES DE LA LIBRAIRIE                            *
 *------------------------------------------------------------*/
#define GNP_HASHSIZE  29
/*------------------------------------------------------------*
 *      TABLES DE HACHAGE                                     *
 *------------------------------------------------------------*/
typedef struct _elt
                  {
                    char *name;
                    int  num_index;
                    int  sens;  /* definit l'ordre de declaration des poids
                                                  des elements d'un vecteur */
                    struct _elt *next;
                  }elt;

extern elt    *hash_paiol[GNP_HASHSIZE];
extern elt    *hash_pagrp[GNP_HASHSIZE];

/*------------------------------------------------------------*
 *      FONCTIONS DE LA LIBRAIRIE                             *
 *------------------------------------------------------------*/

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


  extern           void  InitHash __P((elt *tab[]));
  extern           elt * ExisteIdent __P((char *ident, elt *tab[]));
  extern           elt * SymbAlloc __P((char *ident, elt *tab[]));
  extern            int  FindIdent __P((char *ident, elt *tab[]));

  extern           void  Erreur __P((char *chaine));

  extern           void  KillEsp __P((char *name));

  extern           void  CheckIdent __P((char *ident));
  extern           char  CheckSeparate __P((char *chn_blk));
  extern           char  CheckFormat __P((char *format));
  extern           char  CheckType __P((char *type));
  extern           char  CheckOption __P((char *option));
  extern           void  CheckPattern __P((char *pattern));
  extern           void  CheckWidth __P((struct alpha_num *chaine));

  extern           void  DupPaiol __P((int duplicate, char format, char *param_effectif, char *param_virtuel, char i_o, char blk, char flg));

  extern           void  TraiterChaine __P((char *chaine, struct alpha_num *traite));
  extern           void  SplitIdent __P((char *chaine, char *ident));

  extern           void  FormatValue __P((char *ret_val, char *value, int nb_car, int sens, int i_o));

  extern          char * DefileParam __P(());
  extern           void  EmpileParam __P((char *param));

  extern           void  CreatePapat __P((char *label, struct paevt *lst_event, struct paini *lst_ini));
  extern           void  CreatePaiol __P((char format, char *ident, char mode, char blk, char flg));
  extern           void  CreatePagrp __P((char *ident, int length, int sens, short type_vecteur));
  extern           void  CreateAction __P((char *act_type));
  extern  struct paevt * CreatePaevt __P((struct paevt *tete_paevt, int num_ident, char valeur));

  extern  struct paevt * FusionPaevt __P((struct paevt *lst_event, struct paevt *tete_event));
  extern  struct paini * FusionPaini __P((struct paini *fst_ini, struct paini *scd_ini));
  extern           void  MajPatternCourant __P((char *pattern));


#endif
