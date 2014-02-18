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

/*
 * Tool    : ABL, BDD, HT Librarie
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 */




#ident "$Id: log_thash.c,v 1.4 2012/05/14 14:20:19 alliance Exp $"

/****************************************************************************/
/*    Produit : librairie TSH - Gestion de tables de hachage                */
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include  "mut.h"
#include "log.h"

#undef MAX_CALLS
#define MAX_CALLS 20

/* les fonction de l'utilisateurs :
   ------------------------------

   a. creation de table 

   pTH createTH(len)
   long len;

   b. destruction de la table

   destroyTH(pTable)
   pTH pTable;

   c. recherche d'un element

   long searchTH(pTable,key)
   pTH pTable;
   char *key;

   d. ajout d'un element (ecrasement de la valeur s'il existe deja un element
   dans la table possedant la meme cle)

   long addTH(pTable,key,value)
   pTH pTable;
   char *key;
   long value;

   e. test d'existence et ajout d'un element dans la table.( renvoie 0 si
   l'element n'existait pas avant son ajout, 1 sinon).

   long addExistTH(pTable,key,value)
   pTH pTable;
   char *key;
   long value;

   f. destruction d'un element

   long deleteTH(pTable,key)
   pTH pTable;
   char *key;

   g. affichage total d'une table avec la cle en %s et la valeur en %d.

   void displayTH(pTable)
   pTH pTable;

   h. reallocation d'une table 

   void reAllocTH(pTable)
   pTH pTable ;
 */

/*--------------- la fonction de base pour le hachage.---------------- */


long 
hashTH (pn)
     char *pn;
{

  /* return(abs((long) pn * ((long) pn >> 5) >> 4)); */
  return (((unsigned long) pn >> 2));

}

   /* creation d'une table de hachage */

pTH 
createTH (len)
     long len;
{
  pTH pTable;
  pElemTH pEl;
  long i;

  if (len <= 0)
    {
      printf ("createTH : error - length <= 0\n");
      exit (-1);
    }
  pTable = (pTH) mbkalloc (sizeof (struct tableTH));
  pTable->length = len;

  pEl = (pElemTH) mbkalloc (len * (sizeof (struct elemTH)));
  pTable->pElem = pEl;
  for (i = 0; i < len; i++)
    {
      pEl->key = NULL;
      pEl->value = EMPTYTH;
      pEl++;
    }
  pTable->count = 0;
  return (pTable);
}

   /* destruction d'une table de hachage */

void 
destroyTH (pTable)
     pTH pTable;
{
  pElemTH pEl;

  pEl = pTable->pElem;
  mbkfree (pEl);
  mbkfree (pTable);
}

  /* recherche d'un element dans la table
     renvoie -1 si la recherche echoue. */


long 
searchTH (pTable, key)
     pTH pTable;
     char *key;
{
  long co = 0;
  long indice = 0;
  pElemTH pEl;

  indice = hashTH (key) % pTable->length;
  do
    {
      if (co++ > MAX_CALLS)
	{
	  reAllocTH (pTable);
	  return (searchTH (pTable, key));
	}

      pEl = (pTable->pElem) + indice;
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  if ((long) key == (long) pEl->key)
	    return (pEl->value);
	}
      else if (pEl->value == EMPTYTH)
	return (EMPTYTH);
      indice = (indice + 1) % pTable->length;
    }
  while (1);
}


 /* ajout d'un element a la table */

long 
addTH (pTable, key, value)
     pTH pTable;
     char *key;
     long value;
{
  long indice = 0;
  pElemTH pEl;
  long co = 0;

  if (value == EMPTYTH || value == DELETETH)
    {
      printf ("addTH : error - value is EMPTYTH or DELETETH\n");
      exit (-1);
    }
  if (pTable->count++ > (pTable->length) * 8 / 10)
    {
      reAllocTH (pTable);
      return (addTH (pTable, key, value));
    }

  indice = hashTH (key) % pTable->length;
  do
    {
      if (co++ > MAX_CALLS)
	{
	  reAllocTH (pTable);
	  return (addTH (pTable, key, value));
	}
      pEl = (pTable->pElem) + indice;
      if (pEl->value == EMPTYTH || pEl->value == DELETETH)
	{
	  pEl->value = value;
	  pEl->key = key;
	  return (value);
	}
      else if ((long) pEl->key == (long) key)
	{
	  pTable->count--;
	  pEl->value = value;
	  return (value);
	}

      indice = (indice + 1) % pTable->length;
    }
  while (1);
}

 /* test d'existence et ajout d'un element a la table */

long 
addExistTH (pTable, key, value)
     pTH pTable;
     char *key;
     long value;
{
  long indice = 0;
  pElemTH pEl;
  long co = 0;

  if (value == EMPTYTH || value == DELETETH)
    {
      printf ("addExistTH : error - value is EMPTYTH or DELETETH\n");
      exit (-1);
    }
  if (pTable->count++ > (pTable->length) * 8 / 10)
    {
      reAllocTH (pTable);
      return (addExistTH (pTable, key, value));
    }

  indice = hashTH (key) % pTable->length;
  do
    {
      if (co++ > MAX_CALLS)
	{
	  reAllocTH (pTable);
	  return (addExistTH (pTable, key, value));
	}
      pEl = (pTable->pElem) + indice;
      if (pEl->value == EMPTYTH || pEl->value == DELETETH)
	{
	  pEl->value = value;
	  pEl->key = key;
	  return (0);
	}
      else if ((long) pEl->key == (long) key)
	{
	  pTable->count--;
	  pEl->value = value;
	  return (1);
	}

      indice = (indice + 1) % pTable->length;
    }
  while (1);
}




   /* elimination d'un element de la table */


long 
deleteTH (pTable, key)
     pTH pTable;
     char *key;
{
  long indice = 0;
  pElemTH pEl;
  long co = 0;

  indice = hashTH (key) % pTable->length;
  do
    {
      if (co++ > MAX_CALLS)
	{
	  reAllocTH (pTable);
	  return (deleteTH (pTable, key));
	}
      pEl = (pTable->pElem) + indice;
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  if ((long) key == (long) pEl->key)
	    {
	      pTable->count--;
	      pEl->value = DELETETH;
	      return (pEl->value);
	    }
	}
      else if (pEl->value == EMPTYTH)
	return (EMPTYTH);
      indice = (indice + 1) % pTable->length;
    }
  while (1);			/* on sort quand on arrive sur EMPTYTH */
}


/* affichage des elements de la table */

void 
displayTH (pTable)
     pTH pTable;
{
  long i;
  pElemTH pEl;
  pEl = pTable->pElem;
  printf ("================== DISPLAYTH ================\n");
  printf ("length = %ld\t		count = %ld\n", pTable->length, pTable->count);
  printf ("=============================================\n");
  for (i = 0; i < pTable->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  printf ("index  %ld\t", i);
	  printf ("key    %s\t", pEl->key);
	  printf ("value  %ld \n", pEl->value);
	}
      pEl++;
    }
}

/********** reallocation table d'entiers **************/

void 
reAllocTH (pTable)
     pTH pTable;
{
  pTH tabBis;
  pElemTH pEl;
  long i;

  pEl = pTable->pElem;
  tabBis = createTH ((pTable->length) * 5);
  for (i = 0; i < pTable->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	addTH (tabBis, pEl->key, pEl->value);
      pEl++;
    }
  mbkfree (pTable->pElem);
  pTable->length = tabBis->length;
  pTable->pElem = tabBis->pElem;
  pTable->count = tabBis->count;
  mbkfree (tabBis);
}
