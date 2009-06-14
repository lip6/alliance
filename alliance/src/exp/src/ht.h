#ifndef _HT_H_
#define _HT_H_

/* declaration de types : 
** 1) union de types pour la valeur d'une entree dans le dicionnaire
** 2) type d'une entree
** 3) type du dictionnaire
** -----------------------------------------------------------------
*/
typedef union
{
  struct htelt_st  *e;
  double f;
  long   i;
  char   *s;
}
elt_t;

typedef struct htelt_st
{
  struct htelt_st * NEXT;
  char *KEY;
  long TYPE;
  long USED;
  long FLINE;
  char * FNAME;
  elt_t VAL;
}
htelt_t;

typedef htelt_t * ht_t;

enum htelt_e {HT_EMPTY, HT_INT, HT_STRING, HT_FLOAT};


/* ajoute une entree (initialisé à 0) en tete de liste
** --------------------------------------------------- 
*/
extern htelt_t *eltadd (htelt_t * list, char *key);

/* replace une entree dans la liste des entrees libres
** rend le pointeur NEXT de l'entree effacée
** ---------------------------------------------------
*/
extern htelt_t *eltdel (htelt_t * list);

/* efface une liste d'elements
** ---------------------------
*/
extern void eltremove (htelt_t * list);

/* donne des info sur l'usage du dico
** ----------------------------------
*/
extern void htremove (ht_t * ht);

/* efface le dictionnaire complet
** ------------------------------
*/
extern void htremove (ht_t * ht);

/* creation du dictionnaire
** ------------------------
*/
extern ht_t *htinit (long size);

/* recherche d'une entree dans la table
** rend un pointeur sur l'element
** htget()->KEY pointe sur la cle key
** htget()->VAL.i contient la valeur int
** htget()->VAL.f contient la valeur double
** htget()->VAL.s contient la valeur char *
** ----------------------------------------
*/
extern htelt_t *htget (ht_t * ht, char *key);

/* ajout d'une nouvelle entree dans la table
** NE VERIFIE PAS SI L'ELEMENT N'EST PAS DEJA PRESENT
** --------------------------------------------------
*/
extern htelt_t *htadd (ht_t * ht, char *key);

/* recherche d'une entree dans la table et ajout si absente
** --------------------------------------------------------
*/
htelt_t *htset (ht_t * ht, char *key);

/* recherche d'une regexp dans la table et ajout si absente
** rend un pointeur sur la liste CIRCULAIRE d'elements trouves
** NULL sinon
** --------------------------------------------------------
*/
htelt_t *htsetre (ht_t * ht, char *key);

#endif
