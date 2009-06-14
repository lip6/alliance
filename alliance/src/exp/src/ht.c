#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "ht.h"

/* gestion des entrees du dictionnaire
** =============================================================================
*/
static htelt_t *free_htelt = NULL;

/* ajoute une entree (initialisé à 0) en tete de liste
** --------------------------------------------------- 
*/
htelt_t *eltadd (htelt_t * list, char *key)
{
  htelt_t *new_elt;
  if (free_htelt)
  {
    new_elt = free_htelt;
    free_htelt = free_htelt->NEXT;
  }
  else if ((new_elt = malloc (sizeof (htelt_t))) == NULL)
  {
    perror ("add elt in dictionnary");
    exit (1);
  }
  if (key)
    if ((new_elt->KEY = strdup (key)) == NULL)
    {
      perror ("add elt in dictionnary");
      exit (1);
    }
  new_elt->NEXT = list;
  new_elt->TYPE = 0;
  new_elt->USED = 0;
  new_elt->VAL.f = 0.;
  return new_elt;
}

/* replace une entree dans la liste des entrees libres
** rend le pointeur NEXT de l'entree effacée
** ---------------------------------------------------
*/
htelt_t *eltdel (htelt_t * list)
{
  htelt_t *del_elt = NULL;
  if (list)
  {
    del_elt = list->NEXT;
    list->NEXT = free_htelt;
    free_htelt = list;
  }
  return del_elt;
}

/* efface une liste d'elements
** ---------------------------
*/
void eltremove (htelt_t * list)
{
  htelt_t *elt;
  for (elt = list; elt; elt = eltdel (elt));
}

/* efface le dictionnaire complet
** ------------------------------
*/
void htremove (ht_t * ht)
{
  long i;
  for (i = 1; i < (long) ht[0]; i++)
    eltremove (ht[i]);
  free (ht);
}

/* statistiques
** ------------
*/
void htstat (ht_t * ht)
{
  long i;
  htelt_t *elt;
  if (ht)
  {
    static long start=1;
    for (i = 1; i < (long) ht[0]; i++)
      for (elt = ht[i]; elt; elt = elt->NEXT)
        if (elt->USED == 0)
        {  
          if (start)  
          {
            start=0;
            printf ("Warning : following variables declared but never used :\n");
          }
          printf (" %-3d %-20s : %s\n", elt->FLINE, elt->FNAME, elt->KEY);
        }
    printf ("\n");
    free (ht);
  }
}

/* entrees sorties du dictionnaire
** =============================================================================
*/

static int primes[] = {
  101, 223, 311, 419, 547, 659, 727, 877, 967,
  1061, 2053, 3299, 4391, 5309, 6421, 7069, 8543, 9397,
  10337, 20143, 32423, 43151, 52223, 63179, 0
};

/* creation d'un dictionnaire
** --------------------------
*/
ht_t *htinit (long size)
{
  long i;
  ht_t *ht;

  /* prend le premier nombre premier au dela de size */
  for (i = 0; primes[i] && (size > primes[i]); i++);
  if (primes[i] == 0)
  {
    fprintf (stderr, "Erreur : hash table trop grande (> %d)\n", primes[i - 1]);
    exit (1);
  }
  size = primes[i];
  if ((ht = calloc (size + 1, sizeof (htelt_t *))) == NULL)
  {
    perror ("htinit");
    exit (1);
  }
  ht[0] = (htelt_t *) size;     /* taille de la table dans premiere case */
  return ht;
}

/* calcul de la valeur de hachage primaire
** ---------------------------------------
*/
static long hash (ht_t * ht, char *key)
{
  long alveole = 0;
  long length = strlen (key);
  long segment;
  long l;

  if (ht == NULL)
  {
    fprintf (stderr, "dictionnary not allocated\n");
    exit (1);
  }
  if ((key == NULL) || (length == 0))
  {
    fprintf (stderr, "Error : hash (NULL)\n");
    exit (1);
  }
  for (l = 0; l < length; l += 2)
  {
    segment = 0xFFFF & ((key[l] << 8) | key[l + 1]);
    alveole = alveole ^ ((segment << 1) | (segment >> 15));
  }
  alveole %= (long) ht[0];       /* ht[0] == la taille de la table */
  return (alveole + 1);         /* +1 car on ne doit rien mettre dans case 0 */
}

/* recherche d'une entree dans la table
** rend un pointeur sur l'element
** htget()->KEY pointe sur la cle key
** htget()->VAL.i contient la valeur int
** htget()->VAL.f contient la valeur double
** htget()->VAL.s contient la valeur char *
** ----------------------------------------
*/
htelt_t *htget (ht_t * ht, char *key)
{
  long alveole = hash (ht, key);
  htelt_t *p;

  for (p = ht[alveole]; p && strcmp (p->KEY, key); p = p->NEXT);
  return p;
}

/* ajout d'une nouvelle entree dans la table
** NE VERIFIE PAS SI L'ELEMENT N'EST PAS DEJA PRESENT
** --------------------------------------------------
*/
htelt_t *htadd (ht_t * ht, char *key)
{
  long alveole = hash (ht, key);

  return ht[alveole] = eltadd (ht[alveole], key);
}

/* recherche d'une entree dans la table et ajout si absente
** --------------------------------------------------------
*/
htelt_t *htset (ht_t * ht, char *key)
{
  long alveole = hash (ht, key);
  htelt_t *p;

  for (p = ht[alveole]; p && strcmp (p->KEY, key); p = p->NEXT);
  if (p)
    return p;
  return ht[alveole] = eltadd (ht[alveole], key);
}

/* recherche d'une regexp dans la table et ajout si absente
** rend un pointeur sur la liste CIRCULAIRE d'element trouves
** NULL sinon
** --------------------------------------------------------
*/
htelt_t *htsetre (ht_t * ht, char *key)
{
  regex_t preg;
  htelt_t *elt, *res = NULL;
  long i;

  if (regcomp(&preg, key, REG_EXTENDED | REG_NOSUB) != 0)
    return NULL;
  if (ht)
    for (i = 1; i < (long) ht[0]; i++)
      for (elt = ht[i]; elt; elt = elt->NEXT)
        if (regexec (&preg, elt->KEY, (size_t) 0, NULL, 0) == 0)
        {
          if (res)
          {
            htelt_t * new = eltadd (res->NEXT, NULL);
            new->VAL.e = elt;
            res->NEXT = new ;
            res = new;
          }
          else
          {
            res = eltadd (NULL, NULL);
            res->NEXT = res;
            res->VAL.e = elt;
          }
        }  
  regfree (&preg);
  return res;
}

