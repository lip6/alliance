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



#ident "$Id: log_thashloc.c,v 1.4 2012/05/14 14:20:20 alliance Exp $"

/*--------------------------------------------------------------------------
   la table de hachage local 
   la version du 14.12.90 
  -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include  "mut.h"
#include "log.h"


/* les fonction de l'utilisateurs :
   ------------------------------

   initialisation au depart des pointeurs de vertex a NULL.
   Les fonctions qui utilisent le hachage peuvent renvoye DDBTABLE_PLEINE
   si la table est trop remplie.
   Il n'y a pas de destruction possible d'un element.

   a. creation de table 

   pTableLoc createTabLoc(len)
   long len;

   b. destruction de la table

   void destroyTabLoc(pTab)
   pTableLoc pTab;

   c. re-allocation d'une table de hachage

   reAllocTabLoc(pTab)
   pTableLoc pTab;

   d. recherche d'un element

   pNode searchTabLoc(pTab,high,low)
   pTableLoc pTab;
   pVertexLoc high,low;

   e. ajout d'un element

   long addTabLoc(pTab,pLoc)
   pTableLoc pTab;
   pVertexLoc pLoc;

   f. affichage total d'une table 

   void displayLoc(pTab)
   pTableLoc pTab;


 */

/*-------------------- la fonction de hachage ---------------------------- */

long 
hashLoc (high, low)
     pNode high, low;
{
  return (abs (high->index + low->index +
	       (((long) high) >> 4) + (((long) low) >> 5) +
	       (long) high + (long) low));
}

/*--------------- La table de hachage pour des LOC ------------ */

/* la fonction de creation de table de hachage pour les LOC .
   On alloue en premier lieu une structure TABLE, puis une table
   qui n'est rien d'autre qu'un tableau de pointeurs de LOC. Il est
   donc possible de travailler avec plusieurs table a la fois. */

pTableLoc 
createTabLoc (len)
     long len;
{
  pTableLoc pTab;
  pVertexLoc pLoc;
  long i;

  if (!(pTab = (pTableLoc) mbkalloc (sizeof (struct tableLoc))))
    {
      printf ("allocation impossible\n");
      exit (-1);
    }
  pTab->lenTabLoc = len;

  if (!(pLoc = (pVertexLoc) mbkalloc (len * sizeof (struct vertexLoc))))
    {
      printf ("allocation impossible\n");
      exit (-1);
    }
  pTab->pLoc = pLoc;
  for (i = 0; i < len; i++)
    {
      pLoc->oper = EMPTYTH;	/* -1 */
      pLoc++;
    }
  return (pTab);
}

   /* destruction d'une table de hachage */

void 
destroyTabLoc (pTab)
     pTableLoc pTab;
{
  mbkfree (pTab->pLoc);
  mbkfree (pTab);
}

  /* recherche d'un element dans la table
     renvoie NULL si la recherche echoue. */

pNode 
searchTabLoc (pTab, high, low, oper)
     pTableLoc pTab;
     pNode high, low;
     short oper;
{
  pVertexLoc pLoc;
  long indice;

  /* un seul acces permis */

  indice = hashLoc (high, low) % pTab->lenTabLoc;
  pLoc = pTab->pLoc;
  if (pLoc[indice].oper == oper)
    if (high == pLoc[indice].high && low == pLoc[indice].low)
      return (pLoc[indice].father);
  return (NULL);
}

 /* ajout d'un element a la table */


long 
addTabLoc (pTab, high, low, father, oper)
     pTableLoc pTab;
     pNode high, low, father;
     short oper;
{
  pVertexLoc pLoc;
  long indice;

  /* un seul acces permis */

  indice = hashLoc (high, low) % pTab->lenTabLoc;
  pLoc = pTab->pLoc;
  pLoc[indice].oper = oper;
  pLoc[indice].high = high;
  pLoc[indice].low = low;
  pLoc[indice].father = father;
  return (indice);		/* retourne la place utilisee */
}


/* affichage des elements de la table */

void 
displayLoc (pTab)
     pTableLoc pTab;
{
  long i;
  long co = 0;
  pVertexLoc pLoc;

  pLoc = pTab->pLoc;

  printf ("\n---------------------------------------------------------------");
  printf ("---------\n               AFFICHAGE DE LA TABLE DE HACHAGE\n\n");

  for (i = 0; i < pTab->lenTabLoc; i++)
    {
      if (pLoc[i].oper != EMPTYTH)
	{
	  co++;
	  printf ("****** indice %ld ****** \n", i);
	  printf ("HIGH %d LOW %ld FATHER %ld\n", (long) pLoc[i].high, (long) pLoc[i].low, (long) pLoc[i].father);
	  printf ("\n");
	}
    }
  printf ("\n****** Nombre de noeuds dans la table  = %ld\n", co);
}



/*------------------------------------------------------------------------------
videTabLoc       :vide une table de hachage locale.
-------------------------------------------------------
parametres 	 :une table de hashage locale.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/

void 
videTabLoc (pTab)
     pTableLoc pTab;
{
  pVertexLoc pLoc;
  long i;

  pLoc = pTab->pLoc;


  for (i = 0; i < pTab->lenTabLoc; i++)
    {
      pLoc[i].oper = EMPTYTH;
    }
}
