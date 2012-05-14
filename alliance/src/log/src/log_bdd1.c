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




#ident "$Id: log_bdd1.c,v 1.4 2012/05/14 14:20:19 alliance Exp $"


/****************************************************************************/
/*    Produit : librairie BDD - Gestion de BDD                              */
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mut.h"
#include "log.h"

/*------------------------------------------------------------------------------
initializeCct 	 :cree un circuit . 
-------------------------------------------------------
parametres 	 :nom du circuit, nombre d'entrees et nombre de sorties .
-------------------------------------------------------
return 		 :pointeur de circuit.
------------------------------------------------------------------------------*/
pCircuit 
initializeCct (name, nbI, nbO)
     char *name;
     long nbI, nbO;
{
  pCircuit pC;
  char **pt;
  long i;
  /* on assure ... */
  nbI++;
  nbO++;

  pC = (pCircuit) mbkalloc (sizeof (struct circuit));
  pC->countI = 2;
  pC->pTI = createTH (2 * nbI);
  pC->pTO = createTH (2 * nbO);
  pC->pNameI = (char **) mbkalloc (sizeof (char *) * 2 * nbI);
  pt = pC->pNameI;
  for (i = 0; i < 2 * nbI; i++)
    {
      *pt = NULL;
      pt++;
    }
  pC->name = name;
  return (pC);
}
/*------------------------------------------------------------------------------
resetCct 	 :vide un circuit . 
-------------------------------------------------------
parametres 	 :pointeur de circuit.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/
void 
resetCct (pC)
     pCircuit pC;
{
  pElemTH pEl;
  short i;
  char **pt;

  pEl = (pC->pTI)->pElem;
  for (i = 0; i < (pC->pTI)->length; i++)
    {
      pEl->value = EMPTYTH;
      pEl++;
    }
  pEl = (pC->pTO)->pElem;
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      pEl->value = EMPTYTH;
      pEl++;
    }
  pt = pC->pNameI;
  for (i = 0; i < (pC->pTI)->length; i++)
    {
      *pt = NULL;
      pt++;
    }
  pC->countI = 2;
}

/*------------------------------------------------------------------------------
destroyCct 	 :desalloue un circuit . 
-------------------------------------------------------
parametres 	 :pointeur de circuit.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/
void 
destroyCct (pC)
     pCircuit pC;
{
  destroyTH (pC->pTI);
  destroyTH (pC->pTO);
  mbkfree (pC->pNameI);
  mbkfree (pC);
}
/*------------------------------------------------------------------------------
searchOutputCct	 :recherche le GDB associe a une sortie . 
-------------------------------------------------------
parametres 	 :un pointeur sur le circuit, et le nom de la sortie.
-------------------------------------------------------
return 		 :pointeur de GDB ou null.
------------------------------------------------------------------------------*/
pNode 
searchOutputCct (pC, name)
     pCircuit pC;
     char *name;
{
  long res;

  if ((res = searchTH (pC->pTO, namealloc (name))) != EMPTYTH)
    return ((pNode) res);
  else
    return (NULL);
}
/*------------------------------------------------------------------------------
addOutputCct	 :ajoute un GDB associe a une sortie . 
-------------------------------------------------------
parametres 	 :un pointeur sur le GDB,un pointeur de circuit, et le nom 
		  de la sortie.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/
void 
addOutputCct (pC, name, pt)
     pCircuit pC;
     char *name;
     pNode pt;
{
  name = namealloc (name);
  addTH (pC->pTO, name, (long)pt);
}
/*------------------------------------------------------------------------------
searchIndexCct	 :recherche entree associe a un index . 
-------------------------------------------------------
parametres 	 :un pointeur sur le circuit, et l'index
-------------------------------------------------------
return 		 : NULL ou le pointeur namealloc
------------------------------------------------------------------------------*/
char *
searchIndexCct (pC, index)
     pCircuit pC;
     short index;
{
  if (index <= pC->countI)
    return (*(pC->pNameI + index - 2));
  else
    return (NULL);
}
/*------------------------------------------------------------------------------
searchInputCct	 :recherche index associe a une entree . 
-------------------------------------------------------
parametres 	 :un pointeur sur le circuit, et le nom de l' entree.
-------------------------------------------------------
return 		 :index ou EMPTYTH.
------------------------------------------------------------------------------*/
short 
searchInputCct (pC, name)
     pCircuit pC;
     char *name;
{
  long reallocTH;
  long resul;

  reallocTH = (pC->pTI)->length;
  resul = searchTH (pC->pTI, namealloc (name));

  /* on doit reallouer pNameI */

  if (reallocTH != (pC->pTI)->length)
    {
      long i, j;
      char **pOldName = pC->pNameI;
      char **pOldSave = pC->pNameI;
      char **pt;

      pC->pNameI = (char **) mbkalloc (sizeof (char *) * (pC->pTI)->length);
      pt = pC->pNameI;
      for (i = 0; i < (pC->pTI)->length && *pOldName != NULL; i++)
	{
	  *pt = *pOldName;
	  pt++;
	  pOldName++;
	}

      /* mise a null du reste de la table */

      for (j = i; j < (pC->pTI)->length; j++)
	{
	  *pt = NULL;
	  pt++;
	}
      mbkfree (pOldSave);
    }
  return (resul);
}
/*------------------------------------------------------------------------------
addInputCct	 :ajoute une entree dans la table des var_index. 
-------------------------------------------------------
parametres 	 :un pointeur de circuit, et le nom de la variable 
-------------------------------------------------------
return 		 :index.
------------------------------------------------------------------------------*/
short 
addInputCct (pC, name)
     pCircuit pC;
     char *name;
{
  short index;
  char **ptName;

  name = namealloc (name);
  if ((index = searchInputCct (pC, name)) != EMPTYTH)
    {
      createNodeTermBdd (index);
      return (index);
    }
  else
    {
      long reallocTH;

      index = pC->countI;
      reallocTH = (pC->pTI)->length;
      addTH (pC->pTI, name, index);

      /* on doit reallouer pNameI */

      if (reallocTH != (pC->pTI)->length)
	{
	  long i, j;
	  char **pOldName = pC->pNameI;
	  char **pOldSave = pC->pNameI;
	  char **pt;

	  pC->pNameI = (char **) mbkalloc (sizeof (char *) * (pC->pTI)->length);
	  pt = pC->pNameI;
	  for (i = 0; i < (pC->pTI)->length && *pOldName != NULL; i++)
	    {
	      *pt = *pOldName;
	      pt++;
	      pOldName++;
	    }

	  /* mise a null du reste de la table */

	  for (j = i; j < (pC->pTI)->length; j++)
	    {
	      *pt = NULL;
	      pt++;
	    }
	  mbkfree (pOldSave);
	}
      ptName = pC->pNameI + pC->countI - 2;	/* ajout du nom d'INPUT */
      *ptName = name;
      pC->countI++;
      createNodeTermBdd (index);
      return (index);
    }
}
/*------------------------------------------------------------------------------
delInputCct	 :detruit une entree dans la table des var_index. 
-------------------------------------------------------
parametres 	 :un pointeur de circuit, et le nom de la variable 
-------------------------------------------------------
return 		 : void.
------------------------------------------------------------------------------*/
void 
delInputCct (pC, name)
     pCircuit pC;
     char *name;
{
  deleteTH (pC->pTI, name);
}

/*------------------------------------------------------------------------------
displayCct	 :visualise le circuit .
-------------------------------------------------------
parametres 	 :un pointeur de circuit.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/
void 
displayCct (pC, mode)
     pCircuit pC;
     long mode;
{
  pElemTH pEl;
  short i, cpt = 0;

  pEl = (pC->pTI)->pElem;
  printf ("\n******* DISPLAY %s *******\n", pC->name);
  for (i = 0; i < (pC->pTI)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	printf ("INPUT = %s	INDEX = %d\n", pEl->key, pEl->value);
      pEl++;
    }
  printf ("\n-------------  NUMBER OF INPUTS : %d\n\n", (pC->countI) - 2);
  pEl = (pC->pTO)->pElem;
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	markBdd ((pNode)pEl->value, 0);
      pEl++;
    }
  pEl = (pC->pTO)->pElem;
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  cpt++;
	  printf ("OUTPUT = %s", pEl->key);
	  if (mode == 0)
	    {
	      printf ("	BDD = %d\n", pEl->value);
	      displayBddName (TRUE, (pNode)pEl->value, pC->pNameI);
	    }
	  else
	    {
	      printf ("\n%s = ", pEl->key);
	      displayExpr (bddToAbl ((pNode)pEl->value, pC->pNameI));
	    }
	  printf ("\n");
	}
      pEl++;
    }
  pEl = (pC->pTO)->pElem;
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	markBdd ((pNode)pEl->value, 0);
      pEl++;
    }
  printf ("-------------  NUMBER OF OUTPUTS : %d\n", cpt);
  printf ("**************************************\n\n");
}
/*------------------------------------------------------------------------------
composeCct	 :compose tous les GDB de pC en expansant l'entree name .
-------------------------------------------------------
parametres 	 :un pointeur de circuit,nom d'entree,le GDB associe.
-------------------------------------------------------
return 		 :rien .
------------------------------------------------------------------------------*/
void 
composeCct (pC, name, pt)
     pCircuit pC;
     char *name;
     pNode pt;
{
  long i;
  short index;
  pElemTH pEl;

  pEl = (pC->pTO)->pElem;	/* pointeur courant de la table pTI */
  index = searchInputCct (pC, name);
  if (index == EMPTYTH)
    {
      printf ("bdd1-composeCct : la variable a expanser n'est pas une entree\n");
      exit (-1);
    }
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	pEl->value = (long) composeBdd ((pNode)pEl->value, pt, index);
      pEl++;
    }
  deleteTH (pC->pTI, name);	/* on elimine name des INPUT */
}

/*------------------------------------------------------------------------------
constraintCct	 :contraint tous les GDB de pC avec le GDB pointe par pt .
-------------------------------------------------------
parametres 	 :un pointeur de circuit,le GDB .
-------------------------------------------------------
return 		 :rien .
------------------------------------------------------------------------------*/
void 
constraintCct (pC, pt)
     pCircuit pC;
     pNode pt;
{
  long i;
  pElemTH pEl;

  pEl = (pC->pTO)->pElem;	/* pointeur courant de la table pTO */
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	pEl->value = (long) constraintBdd ((pNode)pEl->value, pt);
      pEl++;
    }
}
/*------------------------------------------------------------------------------
proofCct	 :preuve formelle de circuits .
-------------------------------------------------------
parametres 	 :deux pointeurs de circuits .
-------------------------------------------------------
return 		 :rien .
------------------------------------------------------------------------------*/
void 
proofCct (pC1, pC2)
     pCircuit pC1, pC2;
{
  pElemTH pEl;
  pNode noeudCC2;
  long i;
  short indexCC2;
  chain_list *expr;

  printf ("\n\n*******  PROOF between %s & %s *******\n", pC1->name, pC2->name);
  pEl = (pC1->pTI)->pElem;	/* pointeur courant de la table pTI */
  for (i = 0; i < (pC1->pTI)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  /* recherche dans CC2 */

	  indexCC2 = searchTH (pC2->pTI, pEl->key);


	  if (pEl->value != indexCC2)
	    {
	      printf ("INDEX (%s) differents pour les deux circuits\n", pEl->key);
	      printf ("**********  END OF PROOF **********\n\n");
	      break;
	    }
	}
    }

  pEl = (pC1->pTO)->pElem;	/* pointeur courant de la table CC1 */

  for (i = 0; i < (pC1->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  /* recherche dans CC2 */

	  noeudCC2 = (pNode) searchTH (pC2->pTO, pEl->key);


	  if (noeudCC2 != (pNode) EMPTYTH)
	    {
	      printf ("OUTPUT %s ", pEl->key);
	      if (noeudCC2 == (pNode) pEl->value)
		printf ("is equal\n");
	      else
		{
		  printf ("is different\n");
		  printf ("%s = ", pEl->key);
		  expr = bddToAbl ((pNode) pEl->value, pC1->pNameI);
		  if (numberAtomExpr (expr) < 50)
		    displayExpr (expr);
		  else
		    printf ("too small...");
		  printf ("\n");
		  printf ("%s = ", pEl->key);
		  expr = bddToAbl (noeudCC2, pC2->pNameI);
		  if (numberAtomExpr (expr) < 50)
		    displayExpr (expr);
		  else
		    printf ("too small...");
		  printf ("\n");
		}
	    }
	}
      pEl++;
    }
  printf ("**********  END OF PROOF **********\n\n");
}

/*------------------------------------------------------------------------------
ablToBddCct	 :traduit la forme prefixee ABL en un GDB .
-------------------------------------------------------
parametres 	 : un circuit et un pointeur d'ABL .
-------------------------------------------------------
return 		 :GDB .
------------------------------------------------------------------------------*/
pNode 
ablToBddCct (pC, expr)
     pCircuit pC;
     chain_list *expr;
{
  short oper;
  pNode pt;
  chain_list *lstGdb;

  if (ATOM (expr))
    {
      if ((pt = searchOutputCct (pC, VALUE_ATOM (expr))) != NULL)
	return (pt);
      else
	{
	  if (!strcmp (VALUE_ATOM (expr), "'0'"))
	    return (zero);
	  if (!strcmp (VALUE_ATOM (expr), "'d'"))
	    return (zero);
	  if (!strcmp (VALUE_ATOM (expr), "'1'"))
	    return (one);
	  pt = createNodeTermBdd (addInputCct (pC, VALUE_ATOM (expr)));
	  return (pt);
	}
    }
  else
    {
      oper = OPER (expr);
      lstGdb = NULL;
      while ((expr = CDR (expr)))
	lstGdb = addListBdd (lstGdb, ablToBddCct (pC, CAR (expr)));
      pt = applyBdd (oper, lstGdb);
      freechain (lstGdb);
      return (pt);
    }
}
/*------------------------------------------------------------------------------
cpOrderCct	 :copie la table des index de CC1 dans CC2 . 
-------------------------------------------------------
parametres 	 :deux pointeurs de circuits. 
		  de la sortie.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/
void 
cpOrderCct (CC1, CC2)
     pCircuit CC1, CC2;
{
  long i;
  pElemTH pEl1;
  pElemTH pEl2;
  char **pt1, **pt2;
  pTH tab;

  if (CC2->pTI->length != CC1->pTI->length)
    {
      tab = createTH ((CC1->pTI->length));
      destroyTH (CC2->pTI);
      CC2->pTI = tab;
    }
  pEl1 = (CC1->pTI)->pElem;
  pEl2 = (CC2->pTI)->pElem;
  for (i = 0; i < (CC1->pTI)->length; i++)
    {
      pEl2->value = pEl1->value;
      pEl2->key = pEl1->key;
      pEl1++;
      pEl2++;
    }
  CC2->pTI->count = CC1->pTI->count;
  CC2->countI = CC1->countI;
  pt1 = CC1->pNameI;
  pt2 = CC2->pNameI;
  for (i = 0; i < CC1->countI; i++)
    {
      *pt2 = *pt1;
      pt1++;
      pt2++;
    }
}

/*-------------------------------------------------------------------------
upVarCct 	: remontee d'une variable dans un circuit 
---------------------------------------------------------------------------
retour		: un void.
---------------------------------------------------------------------------*/

void 
upVarCct (pC, ptOldIndex, newIndex)
     pCircuit pC;
     pNode ptOldIndex;
     short newIndex;
{
  long i;
  pElemTH pEl;

  pEl = (pC->pTO)->pElem;
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	pEl->value = (long) upVarBdd ((pNode)pEl->value, ptOldIndex, newIndex);
      pEl++;
    }
  deleteTH (pC->pTI, *(pC->pNameI + newIndex - 2));
  *(pC->pNameI + newIndex - 2) = *(pC->pNameI + ptOldIndex->index - 2);
  addTH (pC->pTI, *(pC->pNameI + ptOldIndex->index - 2), newIndex);
}

/*-------------------------------------------------------------------------
numberNodeCct 	: calcule le nombre de noeud d'un circuit 
---------------------------------------------------------------------------
retour		: un entier.
---------------------------------------------------------------------------*/

long 
numberNodeCct (pC)
     pCircuit pC;
{
  pElemTH pEl;
  long i, number_node;

  number_node = 0;
  markAllBdd (0);
  pEl = (pC->pTO)->pElem;
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  number_node = number_node + countNode ((pNode) pEl->value);
	}
      pEl++;
    }
  markAllBdd (0);
  return (number_node);
}

/*-------------------------------------------------------------------------
numberNodeTdgCct : calcule le nombre de noeud equivalent TDG d'un circuit 
---------------------------------------------------------------------------
retour	    	: un entier.
---------------------------------------------------------------------------*/

long 
numberNodeTdgCct (pC)
     pCircuit pC;
{
  pElemTH pEl;
  long i, number_node;

  number_node = 0;
  markAllBdd (0);
  pEl = (pC->pTO)->pElem;
  for (i = 0; i < (pC->pTO)->length; i++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	{
	  number_node = number_node + countNodeTdg ((pNode) pEl->value);
	}
      pEl++;
    }
  markAllBdd (0);
  return (number_node);
}

/*------------------------------------------------------------------------------
bddToAblCct      :traduit un BDD en ABL d'une maniere simplifie.
-------------------------------------------------------
parametres 	 :un pointeur de NODE.
-------------------------------------------------------
return 		 :une pointeur de CHAIN_LIST.
------------------------------------------------------------------------------*/
chain_list *
bddToAblCct (pC, pBdd)
     pCircuit pC;
     pNode pBdd;
{
  return (bddToAbl (pBdd, pC->pNameI));
}

/*------------------------------------------------------------------------------
gcNodeCct	 :effectue un garbage collecteur sur tous les noeuds de systeme
		  en sauvegardant les noeuds pointes par les pNode(s) du circuit.
-------------------------------------------------------
parametres 	 : pointeur de chain_list. 
-------------------------------------------------------
return 		 : 1 si ok ;
		   0 si erreur .
------------------------------------------------------------------------------*/
void 
gcNodeCct (pC)
     pCircuit pC;
{
  struct systemBdd sysBddAux;
  pNode zeroAux, oneAux;
  pTH pTHNode;
  pElemTH pEl;
  long j;

  pTHNode = createTH (MEDIUM);
  sysBddAux.pRT = createTableBdd (MEDIUM);
  sysBddAux.pMC = createTabLoc (MEDIUM);
  sysBddAux.indiceAT = MAX_PACK;
  sysBddAux.lpAT = NULL;
  zeroAux = initVertexBddAux (0, (pNode) 0, (pNode) 1, &sysBddAux);
  oneAux = initVertexBddAux (1, (pNode) 0, (pNode) 1, &sysBddAux);
  addTH (pTHNode, (char *)zero, (long) zeroAux);
  addTH (pTHNode, (char *)one, (long) oneAux);


  /* on regenere les graphes */

  pEl = (pC->pTO)->pElem;
  for (j = 0; j < (pC->pTO)->length; j++)
    {
      if (pEl->value != EMPTYTH && pEl->value != DELETETH)
	pEl->value = (long) regenereBdd ((pNode) pEl->value, &sysBddAux, pTHNode);
      pEl++;
    }

  destroyBdd (1);
  destroyTH (pTHNode);
  sysBdd.pRT = sysBddAux.pRT;
  sysBdd.pMC = sysBddAux.pMC;
  sysBdd.indiceAT = sysBddAux.indiceAT;
  sysBdd.lpAT = sysBddAux.lpAT;
  sysBdd.pAT = sysBddAux.pAT;
  zero = zeroAux;
  one = oneAux;
}
