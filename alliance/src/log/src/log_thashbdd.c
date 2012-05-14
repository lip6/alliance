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



#ident "$Id: log_thashbdd.c,v 1.5 2012/05/14 14:20:20 alliance Exp $"

/*--------------------------------------------------------------------------
   la table de hachage des BDD 
   la version du 10.12.90 
  -------------------------------------------------------------------------- */
#include <stdlib.h>
#include "mut.h"
#include "log.h"

#undef NB_APPEL_MAX
#define NB_APPEL_MAX 50

/* les fonction de l'utilisateurs :
   ------------------------------

   initialisation au depart des pointeurs de vertex a NULL.
   un pointeur de Vertex desalloue est mis a BDDDELETE.
   Les fonctions qui utilisent le hachage peuvent renvoye BDDTABLE_PLEINE
   si la table est trop remplie.

   a. creation de table 

   pTableBdd createTableBdd(len)
   long len;

   b. destruction de la table

   destroyTableBdd(pTab)
   pTableBdd pTab;

   c. re-allocation d'une table de hachage

   reAllocTableBdd(pTab)
   pTableBdd pTab;

   d. recherche d'un element

   pNode searchTableBdd(pTab,index,high,low)
   pTableBdd pTab;
   long index;
   pNode high,low;

   e. ajout d'un element

   long addTableBdd(pTab,pBdd)
   pTableBdd pTab;
   pNode pBdd;

   f. destruction d'un element

   long deleteTableBdd(pTab,key)
   pTableBdd pTab;
   pNode pBdd;

   g. affichage total d'une table 

   void displayBdd(pTab)
   pTableBdd pTab;

   h. le garbage collector d'une table de hachage

   void gcTableBdd(pTab);
   pTableBdd pTab;


 */

/*-------------------- la fonction de hachage ---------------------------- */

long 
hashBdd (index, high, low)
     long index;
     pNode high, low;
{
  return (abs (index + ((long) high << 1) + (long) low +
	       ((long) high >> 4) + ((long) low >> 5)));
}

/*--------------- la fonction de changement de cle ------------------------- */

long 
newKeyBdd (index, high, low)
     long index;
     pNode high, low;
{
  return (index + (index << 2) + (long) high + ((long) low << 1));
}

/*--------------- La table de hachage pour des BDD ------------ */

/* la fonction de creation de table de hachage pour les BDD .
   On alloue en premier lieu une structure TABLE, puis une table
   qui n'est rien d'autre qu'un tableau de pointeurs de BDD. Il est
   donc possible de travailler avec plusieurs tables a la fois. */

pTableBdd 
createTableBdd (len)
     long len;
{
  pTableBdd pTab;
  pNode *pBdd;
  long i;

  if (!(pTab = (pTableBdd) mbkalloc (sizeof (struct tableBdd))))
    {
      printf ("impossible allocation\n");
      exit (-1);
    }
  pTab->lenTableBdd = len;

  if (!(pBdd = (pNode *) mbkalloc (len * sizeof (pNode))))
    {
      printf ("impossible allocation\n");
      exit (-1);
    }
  pTab->pBdd = pBdd;
  for (i = 0; i < len; i++)
    {
      *pBdd = NULL;
      pBdd++;
    }
  pTab->compteur = 0;
  return (pTab);
}

   /* destruction d'une table de hachage */

void 
destroyTableBdd (pTab)
     pTableBdd pTab;
{
  pNode *pBdd;

  pBdd = pTab->pBdd;
  mbkfree (pBdd);
  mbkfree (pTab);
}

/*------------------------------------------------------------------------------
markBddLst 	 :met a jour les marques de l'ensemble des graphes de LstGdb.
-------------------------------------------------------
parametres 	 :une liste LstGdb.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/
/*
   void markBddLst(pC,value)
   pCircuit pC;
   long value;
   {
   pNode pBdd;

   pEl = (pC->pTO)->pElemT ;

   for (i =0 ; i < (pC->pTO)->lenTable; i++)
   {
   if(pEl->value != VIDE && pEl->value != DELETE)
   markBdd((pNode)pEl->value) ;
   pEl++ ;
   }
   }
 */

  /* re-allocation d'une table de hachage devenue trop petite... */

pTableBdd 
reAllocTableBdd (pTab)
     pTableBdd pTab;
{
  long i;
  pNode *pBdd;
  pTableBdd pTabBis;

  if (pTab->lenTableBdd > MAX_SIZE_BDD)
    {
      printf ("BDD's system : not enough memory...\n");
      exit (-1);
    }
  pTabBis = createTableBdd (pTab->lenTableBdd * 3);	/* trois fois plus grande */
  pBdd = pTab->pBdd;
  for (i = 0; i < pTab->lenTableBdd; i++)
    {
      if (*pBdd != NULL && *pBdd != BDDDELETE)	/* la recopie */
	addTableBdd (pTabBis, *pBdd);
      pBdd++;
    }
  destroyTableBdd (pTab);
  return (pTabBis);
}

  /* recherche d'un element dans la table
     renvoie NULL si la recherche echoue. */

pNode 
searchTableBdd (pTab, index, high, low)
     pTableBdd pTab;
     long index;
     pNode high, low;
{
  long co = 0;
  pNode pBddCur;
  long key = index;
  long indice;

  do
    {
      if (co++ > NB_APPEL_MAX)
	return (BDDTABLE_PLEINE);
      indice = hashBdd (key, high, low) % pTab->lenTableBdd;
      pBddCur = *((pTab->pBdd) + indice);
      if (pBddCur != NULL && pBddCur != BDDDELETE)
	if (high == pBddCur->high &&
	    low == pBddCur->low &&
	    index == pBddCur->index)
	  return (pBddCur);
      key = newKeyBdd (key, high, low);
    }
  while (pBddCur != NULL);
  return (NULL);
}

 /* ajout d'un element a la table */


long 
addTableBdd (pTab, pBdd)
     pTableBdd pTab;
     pNode pBdd;
{
  long co = 0;
  pNode *pBddCur;
  long index = pBdd->index;
  pNode high = pBdd->high;
  pNode low = pBdd->low;
  long key = index;
  long indice;

  if (pTab->compteur++ > (pTab->lenTableBdd) * 8 / 10)	/* remplissage au 8/10 */
    return (TABLE_PLEINE);
  do
    {
      if (co++ > NB_APPEL_MAX)
	return (TABLE_PLEINE);
      indice = hashBdd (key, high, low) % pTab->lenTableBdd;
      pBddCur = ((pTab->pBdd) + indice);
      if (*pBddCur == NULL || *pBddCur == BDDDELETE)
	{
	  *pBddCur = pBdd;
	  return (indice);	/* retourne la place utilisee */
	}
      key = newKeyBdd (key, high, low);
    }
  while (1);
}


   /* elimination d'un element de la table */


long 
deleteTableBdd (pTab, pBdd)
     pTableBdd pTab;
     pNode pBdd;
{
  long co = 0;
  pNode *pBddCur;
  pNode high = pBdd->high;
  pNode low = pBdd->low;
  long key = pBdd->index;
  long indice;

  do
    {
      if (co++ > NB_APPEL_MAX)
	return (TABLE_PLEINE);
      indice = hashBdd (key, high, low) % pTab->lenTableBdd;
      pBddCur = (pTab->pBdd) + indice;
      if (*pBddCur != NULL && *pBddCur != BDDDELETE)
	{
	  if (pBdd == *pBddCur)
	    {
	      *pBddCur = BDDDELETE;
	      return (DELETE);
	    }
	}
      key = newKeyBdd (key, high, low);
    }
  while (pBddCur != NULL);
  return ((long) NULL);
}

/* affichage des elements de la table */

void 
displayHashBdd (pTab)
     pTableBdd pTab;
{
  long i;
  long co = 0;
  pNode *pBdd;

  pBdd = pTab->pBdd;
  {
    printf ("\n---------------------------------------------------------------");
    printf ("---------\n               AFFICHAGE DE LA TABLE DE HACHAGE\n\n");
  }
  for (i = 0; i < pTab->lenTableBdd; i++)
    {
      if (*pBdd != NULL && *pBdd != BDDDELETE)
	{
	  co++;
	  printf ("****** indice %d ****** \n", i);
	  displayBdd (*pBdd , FALSE);
	  printf ("\n");
	}
      pBdd++;
    }
  printf ("\n****** Nombre de noeuds dans la table  = %d\n", co);
}
