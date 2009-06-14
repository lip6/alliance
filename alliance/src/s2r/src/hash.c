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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"

static int primes[] = {
  101, 223, 311, 419, 547, 659, 727, 877, 967,
  1061, 2053, 3299, 4391, 5309, 6421, 7069, 8543, 9397,
  10337, 20143, 32423, 43151, 52223, 63179, 0
};

void htremove (ht_t * htable)
{
  long i;
  for (i = 1; i < (long)htable[0]; i++)
    freechain (htable[i]);
  free (htable);
}

ht_t *htinit (long size)
{
  long i;
  ht_t *htable;

  /* prend le premier nombre premier au dela de size */
  for (i = 0; primes[i] && (size > primes[i]); i++);
  if (primes[i] == 0)
  {
    fprintf (stderr, "Erreur : hash table trop grande (> %d)\n", primes[i - 1]);
    exit (1);
  }
  size = primes[i];
  if ((htable = calloc (size + 1, sizeof (chain_list *))) == NULL)
  {
    perror ("htinit");
    exit (1);
  }
  htable[0] = (chain_list *) size; /* taille de la table dans premiere case */
  return htable;
}

static long hash (ht_t * htable, char *key)
{
  long alveole = 0;
  long length = strlen (key);
  long segment;
  long l;

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
  alveole %= (long) htable[0];   /* htable[0] == la taille de la table */
  return (alveole + 1);         /* +1 car on ne doit rien mettre dans case 0 */
}

chain_list *htget (ht_t * htable, char *key)
{
  long alveole = hash (htable, key);
  chain_list *p;

  for (p = htable[alveole]; p && strcmp (p->DATA, key); p = p->NEXT);
  return p;
}

chain_list *htset (ht_t * htable, char *key)
{
  long alveole = hash (htable, key);
  chain_list *p;

  for (p = htable[alveole]; p && strcmp (p->DATA, key); p = p->NEXT);
  if (p)
    return p;
  htable[alveole] = addchain (htable[alveole], strdup (key));
  return htable[alveole];
}
