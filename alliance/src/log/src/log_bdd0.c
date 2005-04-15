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

/*
 * Tool    : ABL, BDD, HT Librarie
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 */



#ident "$Id: log_bdd0.c,v 1.4 2005/04/15 08:38:49 alliance Exp $"

/****************************************************************************/
/*    Produit : librairie BDD - Gestion de BDD                              */
/****************************************************************************/

#include<stdio.h>
#include "mut.h"
#include "log.h"

pNode one, zero;
struct systemBdd sysBdd;

#undef NAME_ATOM
#undef MIN_OPER
#undef MAX_OPER
#define MIN_OPER 0
#define MAX_OPER 7

#define NAME_ATOM createAtom(*(tabName + pt->index - 2))

/*------------------------------------------------------------------------------
initVertexBdd 	 :cree un noeud BDD . 
-------------------------------------------------------
parametres 	 :index de la variable.
		  pointeurs sur les noeuds fils.
-------------------------------------------------------
return 		 :pointeur sur le noeud cree.
------------------------------------------------------------------------------*/

pNode 
initVertexBdd (index, high, low)
     int index;
     pNode high, low;
{
  pNode pt;


  if ((pt = searchTableBdd (sysBdd.pRT, index, high, low)) != NULL)
    if (pt != BDDTABLE_PLEINE)
      return (pt);
    else
      {
	sysBdd.pRT = reAllocTableBdd (sysBdd.pRT);
	return (initVertexBdd (index, high, low));
      }

  if (high == low)		/* noeud eliminable */
    return (high);

  if (sysBdd.indiceAT == MAX_PACK)
    {
      sysBdd.pAT = (pNode) mbkalloc (MAX_PACK * sizeof (struct node));
      sysBdd.indiceAT = 1;
      sysBdd.lpAT = addchain (sysBdd.lpAT, (void *) sysBdd.pAT);
    }
  else
    {
      sysBdd.pAT++;
      sysBdd.indiceAT++;
    }

  pt = sysBdd.pAT;
  pt->index = index;
  pt->high = high;
  pt->low = low;
  pt->mark = 0;
  if (index > 1)
    if (addTableBdd (sysBdd.pRT, pt) == TABLE_PLEINE)	/* table pleine */
      {
	sysBdd.pRT = reAllocTableBdd (sysBdd.pRT);
	return (initVertexBdd (index, high, low));
      }
  return (pt);
}

/*------------------------------------------------------------------------------
createNodeTermBdd  :cree un noeud variable . 
-------------------------------------------------------
parametres 	 :index de la variable.
-------------------------------------------------------
return 		 :pointeur sur le noeud cree.
------------------------------------------------------------------------------*/

pNode 
createNodeTermBdd (index)
     short index;
{
  pNode pt;

  if (index <= 1)
    {
      printf ("createNodeTermBdd : error index <= 1\n");
      exit (-1);
    }
  else
    {
      pt = initVertexBdd (index, one, zero);
      return (pt);
    }
}

/*------------------------------------------------------------------------------
initializeBdd: Initialise le systeme de la boite a outils des BDD.
-------------------------------------------------------
parametres 	 :rien .
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/

void 
initializeBdd (size)
     int size;
{
  switch (size)
    {
    case 0:
      sysBdd.pRT = createTableBdd (SMALL);
      break;
    case 1:
      sysBdd.pRT = createTableBdd (MEDIUM);
      break;
    case 2:
      sysBdd.pRT = createTableBdd (LARGE);
      break;
    default:
      printf ("initializeBdd : size out of bound\n");
      exit (-1);
    }

  sysBdd.pMC = createTabLoc (MEDIUM);
  sysBdd.indiceAT = MAX_PACK;
  sysBdd.lpAT = NULL;

  zero = initVertexBdd (0, (pNode) 0, (pNode) 1);
  one = initVertexBdd (1, (pNode) 0, (pNode) 1);
}

/*------------------------------------------------------------------------------
destroyBdd        :desalloue le systeme de la boite a outils des BDD.
-------------------------------------------------------
parametres 	 :niveau de desallocation .
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/

void 
destroyBdd (level)
     int level;
{
  chain_list *pt;

  if (level == 1)
    {
      pt = sysBdd.lpAT;
      while (pt != NULL)	/* desallocation des pages de noeuds */
	{
	  mbkfree ((pNode) pt->DATA);
	  pt = pt->NEXT;
	}
      freechain (sysBdd.lpAT);
      sysBdd.lpAT = NULL;
    }
  else
    {
      freechain (sysBdd.lpAT);
      sysBdd.lpAT = NULL;
    }
  destroyTableBdd (sysBdd.pRT);	/* les tables de hachage */
  destroyTabLoc (sysBdd.pMC);
}

/*------------------------------------------------------------------------------
resetBdd        :vide le systeme de la boite a outils des BDD.
-------------------------------------------------------
parametres 	 :rien
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/

void 
resetBdd ()
{
  int i;
  pNode *pBdd;
  chain_list *pt;

  pt = sysBdd.lpAT;
  while (pt)			/*vide la liste d'allocation par paquet */
    {
      mbkfree ((pNode) pt->DATA);
      pt = pt->NEXT;
    }
  freechain (sysBdd.lpAT);
  sysBdd.lpAT = NULL;

  videTabLoc (sysBdd.pMC);

  pBdd = (sysBdd.pRT)->pBdd;
  for (i = 0; i < sysBdd.pRT->lenTableBdd; i++)		/* vide la table de reduction */
    {
      *pBdd = NULL;
      pBdd++;
    }
  (sysBdd.pRT)->compteur = 0;
  sysBdd.indiceAT = MAX_PACK;

  zero = initVertexBdd (0, (pNode) 0, (pNode) 1);
  one = initVertexBdd (1, (pNode) 0, (pNode) 1);
}

/*------------------------------------------------------------------------------
numberNodeAllBdd :compte le nombre de noeud utilise dans le systeme. 
-------------------------------------------------------
return 		 :le nombre de noeud.
------------------------------------------------------------------------------*/

int 
numberNodeAllBdd ()
{
  return ((sysBdd.pRT)->compteur);
}

/*------------------------------------------------------------------------------
numberNodeBdd       :compte le nombre de noeud reduits d'un graphe.
-------------------------------------------------------
parametres 	 :un pointeur de Node.
-------------------------------------------------------
return 		 :le nombre de noeud.
------------------------------------------------------------------------------*/

int 
numberNodeBdd (pBdd)
     pNode pBdd;
{
  int val;

  markBdd (pBdd, -1);
  markBdd (pBdd, 0);
  val = countNode (pBdd);
  markBdd (pBdd, 0);
  return (val);
}

/*------------------------------------------------------------------------------
countNode        : calcul du nombre de noeuds BDD 
-------------------------------------------------------
parametres 	 : une pNode
-------------------------------------------------------
return 		 : int
------------------------------------------------------------------------------*/
int 
countNode (pt)
     pNode pt;
{
  if (pt->index > 1)
    {
      if (pt->mark == 0)
	{
	  pt->mark = 1;
	  return (countNode (pt->high) + countNode (pt->low) + 1);
	}
    }
  return (0);
}

/*------------------------------------------------------------------------------
countNodeTdg     : calcul du nombre de noeuds equivalent TDG sur un BDD 
-------------------------------------------------------
parametres 	 : une pNode
-------------------------------------------------------
return 		 : int
------------------------------------------------------------------------------*/
int 
countNodeTdg (pt)
     pNode pt;
{
  if (pt->index > 1)
    {
      if (pt->mark == 0)
	{
	  int val;
	  pt->mark = 1;
	  val = countNodeTdg (pt->high) + countNodeTdg (pt->low) + 1;
	  markBdd (notBdd (pt), 1);
	  return val;
	}
    }
  return (0);
}

/*------------------------------------------------------------------------------
muxAbl          : realise le multiplexeur a.H + a'.L
-------------------------------------------------------
parametres 	 : Deux pointeurs de Node et un pointeur de CHAIN_LIST.
-------------------------------------------------------
return 		 :une pointeur de CHAIN_LIST.
------------------------------------------------------------------------------*/
chain_list *
muxAbl (high, low, a, tabName)
     pNode high, low;
     chain_list *a;
     char **tabName;
{
  pNode pBdd;
  chain_list *expr1, *expr2;

/*--------- multiplexeur terminal ---------*/

  if (low->index < 2 && high->index < 2)
    {
      if (low == one)		/* not de la variable */
	return (notExpr (a));
      else			/* variable directe */
	return (a);
    }

/*--------- multiplexeur semi-terminal ---------*/

  if (low == one)		/* F = (or (not a) H) */
    return (createBinExpr (OR, bddToAbl (high, tabName), notExpr (a)));

  if (low == zero)		/* F = (and a H) */
    return (createBinExpr (AND, bddToAbl (high, tabName), a));

  if (high == one)		/* F = (or a L) */
    return (createBinExpr (OR, bddToAbl (low, tabName), a));

  if (high == zero)		/* F = (and (not a) L) */
    return (createBinExpr (AND, bddToAbl (low, tabName), notExpr (a)));


  pBdd = applyBinBdd (AND, high, low);

  if (pBdd == zero && applyBinBdd (OR, low, high) == one)

    /* (xor a L) */
    {
      /* choix L ou H ? */

      return (createBinExpr (XOR, bddToAbl (low, tabName), a));
    }

  /* H est inclu dans L */

  if (pBdd == high)
    {

      expr2 = createBinExpr (OR,
			     bddToAbl (high, tabName),
			     createBinExpr (AND,
					    bddToAbl (low, tabName),
					    notExpr (a)));

      return (expr2);
    }

  /* L est inclu dans H */

  if (pBdd == low)
    {

      expr2 = createBinExpr (OR,
			     bddToAbl (low, tabName),
			     createBinExpr (AND,
					    bddToAbl (high, tabName),
					    copyExpr (a)));

      return (expr2);
    }

  /* cas general */

  expr1 = bddToAbl (low, tabName);
  expr2 = bddToAbl (high, tabName);

  expr1 = createBinExpr (OR,
			 createBinExpr (AND, expr1, notExpr (a)),
			 createBinExpr (AND, expr2, copyExpr (a)));

  return (expr1);
}

/*------------------------------------------------------------------------------
bddToAbl        :traduit un BDD en ABL d'une maniere simplifie. Cette fonction
                 recupere des noms de variables que l'on a empile par index
                 croissant dans une table.
-------------------------------------------------------
parametres 	 :un pointeur de NODE.
-------------------------------------------------------
return 		 :une pointeur de CHAIN_LIST.
------------------------------------------------------------------------------*/
chain_list *
bddToAbl (pt, tabName)
     pNode pt;
     char **tabName;
{
  chain_list *expr1;
  pNode pBdd;
  chain_list *res = NULL;

/*----------------- noeud ONE ou ZERO ------------------*/

  if (pt->index < 2)
    {
      if (pt->index == 0)
	res = createAtom ("'0'");
      else
	res = createAtom ("'1'");
      return (res);
    }

/*----------------- variable terminale -----------------*/

  if ((pt->low)->index < 2 || (pt->high)->index < 2)
    {
      if ((pt->low)->index < 2 && (pt->high)->index < 2)
	{
	  if (pt->low == one)	/* not de la variable */
	    res = notExpr (NAME_ATOM);
	  else			/* variable directe */
	    res = NAME_ATOM;
	}
      else
	/* variable semi-terminale */
	{
	  if (pt->low == one)	/* F = (or (not a) H) */
	    res = createBinExpr (OR, bddToAbl (pt->high, tabName), notExpr (NAME_ATOM));

	  if (pt->low == zero)	/* F = (and a H) */
	    res = createBinExpr (AND, bddToAbl (pt->high, tabName), NAME_ATOM);

	  if (pt->high == one)	/* F = (or a L) */
	    res = createBinExpr (OR, bddToAbl (pt->low, tabName), NAME_ATOM);

	  if (pt->high == zero)	/* F = (and (not a) L) */
	    res = createBinExpr (AND, bddToAbl (pt->low, tabName), notExpr (NAME_ATOM));
	}
      return (res);
    }
  else
    {

/*---------------- variable non-terminale ----------------*/

      /* recherche de noyaux non-atomique pour le multiplexeur */

      /* noyaux a double polarite */

      if (pt->high == (pt->low)->high || pt->high == (pt->low)->low)
	{
	  expr1 = createExpr (OR);
	  pBdd = pt;		/* sauvegarde de pt */
	  while (pt->high == pBdd->high || pt->low == pBdd->high)
	    {
	      if (pt->high == pBdd->high)
		{
		  addQExpr (expr1, NAME_ATOM);
		  pt = pt->low;
		}
	      else
		{
		  addQExpr (expr1, notExpr (NAME_ATOM));
		  pt = pt->high;
		}
	    }
	  res = muxAbl (pBdd->high, pt, expr1, tabName);
	  pt = pBdd;
	  return (res);
	}
      else
	{
	  if (pt->low == (pt->high)->low || pt->low == (pt->high)->high)
	    {
	      expr1 = createExpr (AND);
	      pBdd = pt;	/* sauvegarde de pt */
	      while (pt->low == pBdd->low || pt->high == pBdd->low)
		{
		  if (pt->low == pBdd->low)
		    {
		      addQExpr (expr1, NAME_ATOM);
		      pt = pt->high;
		    }
		  else
		    {
		      addQExpr (expr1, notExpr (NAME_ATOM));
		      pt = pt->low;
		    }
		}
	      res = muxAbl (pt, pBdd->low, expr1, tabName);
	      pt = pBdd;
	      return (res);
	    }
	  else
	    {

	      /* noyaux XOR-NXOR */

	      if ((pt->high)->high == (pt->low)->low &&
		  (pt->high)->low == (pt->low)->high &&
		  (pt->high)->index == (pt->low)->index)
		{
		  expr1 = createExpr (XOR);
		  addQExpr (expr1, NAME_ATOM);
		  pBdd = pt;
		  pt = pt->low;
		  addQExpr (expr1, NAME_ATOM);
		  pt = pBdd;

		  res = muxAbl ((pt->low)->high, (pt->low)->low, expr1, tabName);
		  return (res);
		}
	    }
	}
      /*  (or (and a H) (and (not a) L) */

      expr1 = muxAbl (pt->high, pt->low, NAME_ATOM, tabName);

      return (expr1);
    }
}

/*------------------------------------------------------------------------------
displayBdd 	: visualise le BDD . 
-------------------------------------------------------
parametres 	 :niveau d'affichage ,pointeur sur le noeud tete de graphe.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/

void 
displayBddLoc (level, pt)
     short level;
     pNode pt;
{

  if (pt->mark == 1)
    return;
  if (pt->index > 1)
    pt->mark = 1;
  if (pt->index > 1)
    {
      printf ("%d\t INDEX = %d\t", (int) pt, (int) pt->index);

      if ((pt->low)->index == 0)
	printf (" LOW = ZERO");
      else if ((pt->low)->index == 1)
	printf (" LOW = ONE");
      else
	printf (" LOW = %d", (int) pt->low);

      if ((pt->high)->index == 0)
	printf ("\t HIGH = ZERO\n");
      else if ((pt->high)->index == 1)
	printf ("\t HIGH = ONE\n");
      else
	printf ("\t HIGH = %d\n", (int) pt->high);

      /* appel recursif */

      if (level == 1)
	{
	  if ((pt->low)->index > 1)
	    displayBddLoc (level, pt->low);
	  if ((pt->high)->index > 1)
	    displayBddLoc (level, pt->high);
	}
    }
  else if (pt->index == 1)
    printf ("ONE      INDEX = 1\n");
  else
    printf ("ZERO     INDEX = 0\n");
}

void 
displayBdd (pBdd, level)
     pNode pBdd;
     int level;
{
  markBdd (pBdd, 0);
  displayBddLoc (level, pBdd);
  markBdd (pBdd, 0);
}

/*----------------------------------------------------------------------------
assignNumNodeBdd : assigns with an integer each vertex  in a Hash Table
------------------------------------------------------------------------------
return          : nothing
------------------------------------------------------------------------------*/

void 
assignNumNodeBdd (bdd, vTable, pNodeNumber)
     pNode bdd;
     pTH vTable;
     int *pNodeNumber;
{
  if (bdd != one && bdd != zero)
    {
      if (searchTH (vTable, (char *) bdd) == EMPTYTH || searchTH (vTable, (char *) bdd) == DELETETH)
	{
	  addTH (vTable, (char *)bdd, *pNodeNumber);
	  *pNodeNumber = *pNodeNumber + 1;
	}

      assignNumNodeBdd (bdd->low, vTable, pNodeNumber);
      assignNumNodeBdd (bdd->high, vTable, pNodeNumber);
    }
  else
    {
      if (bdd == one)
	{
	  addTH (vTable, (char *)bdd, 1);
	}
      else
	addTH (vTable, (char *)bdd, 0);
    }
}
/*------------------------------------------------------------------------------
displayGraphicBDD : visualise le BDD . 
-------------------------------------------------------
parametres 	 : pointeur sur le noeud tete de graphe.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/
void 
displayGraphicBdd (pBdd)
     pNode pBdd;
{
  pTH vTable;
  int numNode = 2;
  chain_list *lst, *supp;
  int i;

  supp = reverse (supportChain_listBdd (pBdd));

  /* on refait la numerotation */

  vTable = createTH (100);

  addTH (vTable, (char *)zero, 0);
  addTH (vTable, (char *)one, 1);
  for (i = pBdd->index; i > 1; i--)
    {
      lst = supp;
      while (lst)
	{
	  pNode pt = (pNode) lst->DATA;
	  if (i == pt->index)
	    {
	      addTH (vTable, (char *)pt, numNode);
	      numNode++;
	    }
	  lst = lst->NEXT;
	}
    }


  printf ("\n");
  printf ("INDEX |                                  ROBDD\n");
  printf ("=========================================================================\n");
  printf ("      |\n");

  for (i = pBdd->index; i > 1; i--)
    {
      if (i < 10)
	printf ("  %d   |  ", i);
      else
	printf ("  %d  |  ", i);
      lst = supp;
      while (lst)
	{
	  pNode pt = (pNode) lst->DATA;

	  if (i == pt->index)
	    {
	      int numLow = searchTH (vTable, (char *) pt->low);
	      int numHigh = searchTH (vTable, (char *) pt->high);
	      int num = searchTH (vTable, (char *)pt);

	      printf ("  %d_(%d)_%d  ", numLow, num, numHigh);
	    }
	  lst = lst->NEXT;
	}
      printf ("\n");
      printf ("      |\n");
    }
  printf ("=========================================================================\n");
  destroyTH (vTable);
}

/*------------------------------------------------------------------------------
displayBddName :visualise le BDD en substituant l'index par son nom INPUT . 
-------------------------------------------------------
parametres 	 :niveau d'affichage ,pointeur sur le noeud tete de graphe et le
                  le pointeur de table de Name.
-------------------------------------------------------
return 		 :rien.
------------------------------------------------------------------------------*/

void 
displayBddName (level, pt, TabName)
     short level;
     pNode pt;
     char **TabName;
{

  if (pt->mark == 1)
    return;
  pt->mark = 1;
  if (pt->index > 1)
    {
      printf ("%d\t INPUT = %s\t", (int) pt, *(TabName + pt->index - 2));
      if ((pt->high)->index == 0)
	printf (" HIGH = ZERO");
      else if ((pt->high)->index == 1)
	printf (" HIGH = ONE");
      else
	printf (" HIGH = %d", (int) pt->high);

      if ((pt->low)->index == 0)
	printf ("\t LOW = ZERO\n");
      else if ((pt->low)->index == 1)
	printf ("\t LOW = ONE\n");
      else
	printf ("\t LOW = %d\n", (int) pt->low);
      if (level == 1)
	{
	  if ((pt->low)->index > 1)
	    displayBddName (level, pt->low, TabName);
	  if ((pt->high)->index > 1)
	    displayBddName (level, pt->high, TabName);
	}
    }
  else if (pt->index == 1)
    printf ("ONE      INDEX = 1\n");
  else
    printf ("ZERO     INDEX = 0\n");
}

/*------------------------------------------------------------------------------
notBdd           :inversion d'un arbre Bdd.
-------------------------------------------------------
parametres 	 :un pointeur de Bdd.
-------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/

pNode 
notBdd (pBdd)
     pNode pBdd;
{
  pNode pBddLoc;

  if (pBdd == zero)
    return (one);
  if (pBdd == one)
    return (zero);
  pBddLoc = searchTabLoc (sysBdd.pMC, pBdd, pBdd, NOT);
  if (pBddLoc != NULL)
    return (pBddLoc);
  pBddLoc = initVertexBdd (pBdd->index, notBdd (pBdd->high), notBdd (pBdd->low));
  addTabLoc (sysBdd.pMC, pBdd, pBdd, pBddLoc, NOT);
  return (pBddLoc);
}

/*------------------------------------------------------------------------------
applyTerm        :application d'un operateur sur un Bdd avec un noeud terminal.
-------------------------------------------------------
parametres 	 :un operateur,un index de noeud terminal,un pointeur de Bdd.
-------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/

pNode 
applyTerm (oper, index, pBdd)
     int oper;
     short index;
     pNode pBdd;
{
  /* noeud one */

  if (index == 1)
    if (oper == OR)
      return (one);
    else if (oper == NAND || oper == XOR)
      return (notBdd (pBdd));
    else if (oper == AND || oper == NXOR)
      return (pBdd);
    else
      return (zero);

  /* noeud zero */

  if (oper == AND)
    return (zero);
  else if (oper == NOR || oper == NXOR)
    return (notBdd (pBdd));
  else if (oper == OR || oper == XOR)
    return (pBdd);
  else
    return (one);		/* c'est un "nand" */
}


/*------------------------------------------------------------------------------
applyBinBdd  	:application d'un operateur sur deux Bdd.
-------------------------------------------------------
parametres 	 :un operateur,deux pointeurs de Bdd.
-------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/

pNode 
applyBinBdd (oper, pBdd1, pBdd2)
     short oper;
     pNode pBdd1, pBdd2;
{

  pNode pBddLoc;
  short index1 = pBdd1->index;
  short index2 = pBdd2->index;

  if ((index1 < 2) || (index2 < 2))	/* il existe un noeud terminal */
    if ((index1 < 2) && (index2 < 2))	/* tous les deux sont terminaux */

      if (index1 != index2)	/* 01 ou 10 */
	if (oper == OR || oper == NAND || oper == XOR)
	  return (one);
	else
	  return (zero);
      else if (index1 == 0)	/* 00 */
	if (oper == NOR || oper == NAND || oper == NXOR)
	  return (one);
	else
	  return (zero);
      else
	/* 11 */ if (oper == OR || oper == AND || oper == NXOR)
	return (one);
      else
	return (zero);
    else if (index1 < 2)
      return (applyTerm (oper, index1, pBdd2));
    else
      return (applyTerm (oper, index2, pBdd1));

  /* les index ne correspondent pas a des noeuds terminaux */


  /* recherche dans la table de hachage locale */

  if (pBdd1 > pBdd2)
    pBddLoc = searchTabLoc (sysBdd.pMC, pBdd1, pBdd2, oper);
  else
    pBddLoc = searchTabLoc (sysBdd.pMC, pBdd2, pBdd1, oper);

  /* operation deja calcule... */

  if (pBddLoc != NULL)
    return (pBddLoc);


  if (index1 == index2)		/* deux noeuds de meme index */
    {
      if (pBdd1 == pBdd2)	/* egalite des pointeurs ==> simplification */
	{
	  if (oper == OR || oper == AND)
	    return (pBdd1);
	  if (oper == NOR || oper == NAND)
	    return (notBdd (pBdd1));
	  if (oper == XOR)
	    return (zero);
	  else
	    return (one);
	}
      pBddLoc = initVertexBdd (index1, applyBinBdd (oper, pBdd1->high, pBdd2->high),
			       applyBinBdd (oper, pBdd1->low, pBdd2->low));
    }
  else if (index1 < index2)	/* deux noeuds d'index different */
    pBddLoc = initVertexBdd (index2, applyBinBdd (oper, pBdd1, pBdd2->high),
			     applyBinBdd (oper, pBdd1, pBdd2->low));
  else
    pBddLoc = initVertexBdd (index1, applyBinBdd (oper, pBdd1->high, pBdd2),
			     applyBinBdd (oper, pBdd1->low, pBdd2));

  /* ajout d'un noeud dans la table de hash locale */

  if (pBdd1 > pBdd2)
    addTabLoc (sysBdd.pMC, pBdd1, pBdd2, pBddLoc, oper);
  else
    addTabLoc (sysBdd.pMC, pBdd2, pBdd1, pBddLoc, oper);

  return (pBddLoc);
}

/*------------------------------------------------------------------------------
applyBdd         :application d'un operateur sur une liste de Bdd.
-------------------------------------------------------
parametres 	 :un operateur,un pointeur de liste de Bdd.
-------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/

pNode 
applyBdd (oper, pt)
     short oper;
     chain_list *pt;
{
  short operateur_N = 0;
  pNode pBdd;
  pNode pBddResul, pBddInter;

  if (oper > MAX_OPER || oper < MIN_OPER)
    {
      printf ("applyBdd : error - unknown operator %d\n", oper);
      exit (-1);
    }
  if (pt == NULL)
    {
      printf ("applyBdd : error chained list is empty\n");
      exit (-1);
    }

  pBdd = (pNode) pt->DATA;

  if (pt->NEXT == NULL)		/* operateur unaire NOT */
    {
      if (oper != NOT)
	{
	  printf ("applyBdd : error - bad operator %s\n", operToChar (oper));
	  exit (-1);
	}
      return (notBdd (pBdd));
    }


  if ((pt->NEXT)->NEXT == NULL)	/* operateur binaire */
    {
      pBddResul = applyBinBdd (oper, pBdd, (pNode) (pt->NEXT)->DATA);
      return (pBddResul);
    }

  /* operateur negatif d'arite superieure a trois */

  if (oper == NXOR || oper == NAND || oper == NOR)
    {
      operateur_N = 1;
      if (oper == NOR)
	oper = OR;
      else if (oper == NXOR)
	oper = XOR;
      else
	oper = AND;
    }

  pBddInter = applyBdd (oper, pt->NEXT);
  pBddResul = applyBinBdd (oper, pBdd, pBddInter);

  if (operateur_N == 1)
    return (notBdd (pBddResul));
  else
    return (pBddResul);
}
/*----------------------------------------------------------------------------
cnstBdd     :constrainte d'un graphe sur un autre.
------------------------------------------------------------------------------
parametres 	 :deux graphes Bdd.
-----------------------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/
pNode 
cnstBdd (pBdd1, pBddGc)
     pNode pBdd1, pBddGc;
{
  short index1 = pBdd1->index;
  short indexGc = pBddGc->index;
  pNode pBddLoc;

  if (indexGc == 1)
    return (pBdd1);

  if (index1 < 2)
    return (pBdd1);

  pBddLoc = searchTabLoc (sysBdd.pMC, pBdd1, pBddGc, CNST);
  if (pBddLoc != NULL)
    return (pBddLoc);

  if (index1 == indexGc)
    {
      if (pBddGc->high == zero)
	pBddLoc = cnstBdd (pBdd1->low, pBddGc->low);
      else if (pBddGc->low == zero)
	pBddLoc = cnstBdd (pBdd1->high, pBddGc->high);
      else
	pBddLoc = initVertexBdd (indexGc,
				 cnstBdd (pBdd1->high, pBddGc->high),
				 cnstBdd (pBdd1->low, pBddGc->low));
    }
  else if (index1 < indexGc)
    pBddLoc = initVertexBdd (indexGc, cnstBdd (pBdd1, pBddGc->high),
			     cnstBdd (pBdd1, pBddGc->low));
  else
    pBddLoc = initVertexBdd (index1, cnstBdd (pBdd1->high, pBddGc),
			     cnstBdd (pBdd1->low, pBddGc));
  addTabLoc (sysBdd.pMC, pBdd1, pBddGc, pBddLoc, CNST);
  return (pBddLoc);
}

/*----------------------------------------------------------------------------
restrictBdd     :constrainte d'un graphe sur un autre.
------------------------------------------------------------------------------
parametres 	 :deux graphes Bdd.
-----------------------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/

pNode 
restrictBdd (pBdd1, pBddGc)
     pNode pBdd1, pBddGc;
{
  short index1 = pBdd1->index;
  short indexGc = pBddGc->index;
  pNode pBddLoc;

  if (indexGc == 1)
    return (pBdd1);

  if (index1 < 2)
    return (pBdd1);

  pBddLoc = searchTabLoc (sysBdd.pMC, pBdd1, pBddGc, RESTRICT);
  if (pBddLoc != NULL)
    return (pBddLoc);

  if (index1 == indexGc)
    {
      if (pBddGc->high == zero)
	pBddLoc = restrictBdd (pBdd1->low, pBddGc->low);
      else if (pBddGc->low == zero)
	pBddLoc = restrictBdd (pBdd1->high, pBddGc->high);
      else
	pBddLoc = initVertexBdd (indexGc,
				 restrictBdd (pBdd1->high, pBddGc->high),
				 restrictBdd (pBdd1->low, pBddGc->low));
    }
  else if (index1 < indexGc)
    pBddLoc = restrictBdd (pBdd1, applyBinBdd (OR, pBddGc->low, pBddGc->high));
  else
    pBddLoc = initVertexBdd (index1, restrictBdd (pBdd1->high, pBddGc),
			     restrictBdd (pBdd1->low, pBddGc));
  addTabLoc (sysBdd.pMC, pBdd1, pBddGc, pBddLoc, RESTRICT);
  return (pBddLoc);
}

/*----------------------------------------------------------------------------
constraintBdd     :constrainte d'un graphe sur un autre.
------------------------------------------------------------------------------
parametres 	 :deux graphes Bdd.
-----------------------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/

pNode 
constraintBdd (pBdd1, pBddGc)
     pNode pBdd1, pBddGc;
{
  short index1 = pBdd1->index;
  short indexGc = pBddGc->index;
  pNode pBddLoc;

  if (index1 == 0)
    return (zero);
  if (index1 == 1)
    return (one);
  if (indexGc == 1)
    return (pBdd1);

  if (pBdd1 == pBddGc)
    return (one);		/*  pBdd1 = pBddGc  ==>  ONE  */

  pBddLoc = searchTabLoc (sysBdd.pMC, pBdd1, pBddGc, CONTRAINT);
  if (pBddLoc != NULL)
    return (pBddLoc);

  if (index1 == indexGc)
    {
      if (pBddGc->high == zero)
	pBddLoc = constraintBdd (pBdd1->low, pBddGc->low);
      else if (pBddGc->low == zero)
	pBddLoc = constraintBdd (pBdd1->high, pBddGc->high);
      else
	pBddLoc = initVertexBdd (indexGc,
				 constraintBdd (pBdd1->high, pBddGc->high),
				 constraintBdd (pBdd1->low, pBddGc->low));
    }
  else if (index1 < indexGc)
    pBddLoc = initVertexBdd (indexGc, constraintBdd (pBdd1, pBddGc->high),
			     constraintBdd (pBdd1, pBddGc->low));
  else
    pBddLoc = initVertexBdd (index1, constraintBdd (pBdd1->high, pBddGc),
			     constraintBdd (pBdd1->low, pBddGc));
  addTabLoc (sysBdd.pMC, pBdd1, pBddGc, pBddLoc, CONTRAINT);
  return (pBddLoc);
}

/*------------------------------------------------------------------------------
simplifDcZeroBdd :simplifie un BDD avec un BDD correspondant a un Don't Care.
                  pGdc est inclu dans pGon';
-------------------------------------------------------
parametres 	 :deux pointeurs de NODE.
-------------------------------------------------------
return 		 :un pointeur de NODE.
------------------------------------------------------------------------------*/
pNode 
simplifDcZeroBdd (pGon, pGdc)
     pNode pGon, pGdc;
{
  pNode pBddLoc;
  pNode pAOH, pAOL;

  /* simplification terminale */

  if (pGdc == zero)
    return (pGon);
  if (pGon->index < 2)
    return (pGon);

  pBddLoc = searchTabLoc (sysBdd.pMC, pGon, pGdc, DONTCARE0);
  if (pBddLoc != NULL)
    return (pBddLoc);

  /* noeuds de meme index */

  if (pGdc->index == pGon->index)
    {
      /* simplification terminales sur les fils */

      if (pGdc->high == one)
	return (simplifDcZeroBdd (pGon->low, pGdc->low));
      if (pGdc->low == one)
	return (simplifDcZeroBdd (pGon->high, pGdc->high));

      /* assignation a une tautologie */

/*
   if (applyBinBdd(OR,pGon,pGdc) == one) return(one);
 */

      /* elimination du noeud racine de pGon par inclusion */

      if (pGon->high == zero && applyBinBdd (AND, pGon->low, pGdc->high) == pGon->low)
	return (pGon->low);

      if (pGon->low == zero && applyBinBdd (AND, pGon->high, pGdc->low) == pGon->high)
	return (pGon->high);

      pAOH = applyBinBdd (AND, pGon->high, applyBinBdd (OR, pGon->low, pGdc->low));
      pAOL = applyBinBdd (AND, pGon->low, applyBinBdd (OR, pGon->high, pGdc->high));

      if (pAOH == pGon->high && pAOL == pGon->low)
	{
	  pBddLoc = simplifDcZeroBdd (applyBinBdd (OR, pGon->low, pGon->high),
				  applyBinBdd (AND, pGdc->low, pGdc->high));
	}
      else
	{
	  /* cas general */

	  pBddLoc = initVertexBdd (pGon->index,
				   simplifDcZeroBdd (pGon->high, pGdc->high),
				   simplifDcZeroBdd (pGon->low, pGdc->low));

	}
    }
  else
    /* index differents */

  if (pGon->index > pGdc->index)
    pBddLoc = initVertexBdd (pGon->index, simplifDcZeroBdd (pGon->high, pGdc),
			     simplifDcZeroBdd (pGon->low, pGdc));
  else
    pBddLoc = simplifDcZeroBdd (pGon, applyBinBdd (AND, pGdc->low, pGdc->high));

  addTabLoc (sysBdd.pMC, pGon, pGdc, pBddLoc, DONTCARE0);
  return (pBddLoc);
}

/*------------------------------------------------------------------------------
simplifPlusDcZeroBdd 
               :simplifie plus fortement un BDD avec un BDD correspondant
                a un Don't Care, pGdc est inclu dans pGon';
-------------------------------------------------------
parametres 	 :deux pointeurs de NODE.
-------------------------------------------------------
return 		 :un pointeur de NODE.
------------------------------------------------------------------------------*/
pNode 
simplifPlusDcZeroBdd (pGon, pGdc)
     pNode pGon, pGdc;
{
  pNode pBddLoc;
  pNode pAOH, pAOL;

  /* simplification terminale */

  if (pGdc == zero)
    return (pGon);
  if (pGdc == one)
    return (one);
  if (pGon == zero)
    return (zero);

  pBddLoc = searchTabLoc (sysBdd.pMC, pGon, pGdc, DONTCARE2);
  if (pBddLoc != NULL)
    return (pBddLoc);

  /* noeuds de meme index */

  if (pGdc->index == pGon->index)
    {
      /* simplification terminales sur les fils */

      if (pGdc->high == one)
	return (simplifPlusDcZeroBdd (pGon->low, pGdc->low));
      if (pGdc->low == one)
	return (simplifPlusDcZeroBdd (pGon->high, pGdc->high));

/*
   if (applyBinBdd(OR,pGon,pGdc) == one) return(one);
 */

      if (pGon->high == zero && applyBinBdd (AND, pGon->low, pGdc->high) == pGon->low)
	return (pGon->low);

      if (pGon->low == zero && applyBinBdd (AND, pGon->high, pGdc->low) == pGon->high)
	return (pGon->high);

      pAOH = applyBinBdd (AND, pGon->high, applyBinBdd (OR, pGon->low, pGdc->low));
      pAOL = applyBinBdd (AND, pGon->low, applyBinBdd (OR, pGon->high, pGdc->high));

      if (pAOH == pGon->high && pAOL == pGon->low)
	{
	  pBddLoc = simplifPlusDcZeroBdd (applyBinBdd (OR, pGon->low, pGon->high),
				  applyBinBdd (AND, pGdc->low, pGdc->high));
	}
      else
	{
	  if (pAOL == pGon->low)	/* Lon inclu dans Hon+Hdc */
	    {			/* on maintient la propriete d'inclusion */
	      pNode HonRes;

	      HonRes = simplifPlusDcZeroBdd (
				    applyBinBdd (OR, pGon->low, pGon->high),
			 applyBinBdd (AND, pGdc->high, notBdd (pGon->low)));
	      pBddLoc = initVertexBdd (pGon->index, HonRes,
				       simplifPlusDcZeroBdd (pGon->low,
				     applyBinBdd (AND, pGdc->low, HonRes)));
	    }
	  else
	    {
	      if (pAOH == pGon->high)	/* Hon inclu dans Lon+Ldc */
		{		/* on maintient la propriete d'inclusion */
		  pNode LonRes;

		  LonRes = simplifPlusDcZeroBdd (
				    applyBinBdd (OR, pGon->low, pGon->high),
			 applyBinBdd (AND, pGdc->low, notBdd (pGon->high)));
		  pBddLoc = initVertexBdd (pGon->index,
					   simplifPlusDcZeroBdd (pGon->high,
				     applyBinBdd (AND, pGdc->high, LonRes)),
					   LonRes);
		}
	      else
		{

		  /* cas general */

		  pBddLoc = initVertexBdd (pGon->index,
			      simplifPlusDcZeroBdd (pGon->high, pGdc->high),
			       simplifPlusDcZeroBdd (pGon->low, pGdc->low));

		}
	    }
	}
    }
  else
    /* index differents */

  if (pGon->index > pGdc->index)
    pBddLoc = initVertexBdd (pGon->index, simplifPlusDcZeroBdd (pGon->high, pGdc),
			     simplifPlusDcZeroBdd (pGon->low, pGdc));
  else
    pBddLoc = simplifPlusDcZeroBdd (pGon, applyBinBdd (AND, pGdc->low, pGdc->high));

  addTabLoc (sysBdd.pMC, pGon, pGdc, pBddLoc, DONTCARE2);
  return (pBddLoc);
}

/*------------------------------------------------------------------------------
simplifDcOneBdd  :simplifie un BDD avec un BDD correspondant a un Don't Care.
		  pGdc est inclu dans pGon;
-------------------------------------------------------
parametres 	 :deux pointeurs de NODE.
-------------------------------------------------------
return 		 :un pointeur de NODE.
------------------------------------------------------------------------------*/
pNode 
simplifDcOneBdd (pGon, pGdc)
     pNode pGon, pGdc;
{
  pNode pBddLoc, ptEtOn;

  /* simplification terminale */

  if (pGdc == zero)
    return (pGon);
  if (pGon == pGdc)
    return (zero);
  if (pGon->index < 2)
    return (pGon);

  pBddLoc = searchTabLoc (sysBdd.pMC, pGon, pGdc, DONTCARE1);
  if (pBddLoc != NULL)
    return (pBddLoc);

  /* noeuds de meme index */

  if (pGdc->index == pGon->index)
    {
      /* simplification terminales sur les fils */

      if (pGdc->high == one)
	return (simplifDcOneBdd (pGon->low, pGdc->low));
      if (pGdc->low == one)
	return (simplifDcOneBdd (pGon->high, pGdc->high));

      if (pGon->high == one &&
	  applyBinBdd (AND, notBdd (pGdc->high), pGon->low) == notBdd (pGdc->high))
	return (simplifDcOneBdd (pGon->low, applyBinBdd (AND, pGdc->high, pGdc->low)));

      if (pGon->low == one &&
	  applyBinBdd (AND, pGon->high, notBdd (pGdc->low)) == notBdd (pGdc->low))
	return (simplifDcOneBdd (pGon->high, applyBinBdd (AND, pGdc->high, pGdc->low)));

      ptEtOn = applyBinBdd (AND, pGon->high, pGon->low);

      if (applyBinBdd (AND, ptEtOn, notBdd (pGdc->low)) ==
	  applyBinBdd (AND, pGon->low, notBdd (pGdc->low)) &&
	  applyBinBdd (AND, ptEtOn, notBdd (pGdc->high)) ==
	  applyBinBdd (AND, pGon->high, notBdd (pGdc->high)))
	{
	  pBddLoc = simplifDcOneBdd (ptEtOn, applyBinBdd (AND, pGdc->low, pGdc->high));
	}
      else
	/* cas general */

	pBddLoc = initVertexBdd (pGon->index, simplifDcOneBdd (pGon->high, pGdc->high),
				 simplifDcOneBdd (pGon->low, pGdc->low));
    }
  else
    /* index differents */

  if (pGon->index > pGdc->index)
    pBddLoc = initVertexBdd (pGon->index, simplifDcOneBdd (pGon->high, pGdc),
			     simplifDcOneBdd (pGon->low, pGdc));
  else
    pBddLoc = simplifDcOneBdd (pGon, applyBinBdd (AND, pGdc->low, pGdc->high));

  addTabLoc (sysBdd.pMC, pGon, pGdc, pBddLoc, DONTCARE1);
  return (pBddLoc);
}

/*------------------------------------------------------------------------------
simplifDcOneFPGABdd  :simplifie un BDD avec un BDD correspondant a un Don't Care.
		  pGdc est inclu dans pGon;
-------------------------------------------------------
parametres 	 :deux pointeurs de NODE.
-------------------------------------------------------
return 		 :un pointeur de NODE.
------------------------------------------------------------------------------*/
pNode 
simplifDcOneFPGABdd (pGon, pGdc)
     pNode pGon, pGdc;
{
  pNode pBddLoc;

  /* simplification terminale */

  if (pGdc == zero)
    return (pGon);
  if (pGon == pGdc)
    return (zero);
  if (pGon->index < 2)
    return (pGon);

  pBddLoc = searchTabLoc (sysBdd.pMC, pGon, pGdc, DONTCARE1);
  if (pBddLoc != NULL)
    return (pBddLoc);

  /* noeuds de meme index */

  if (pGdc->index == pGon->index)
    {
      /* simplification terminales sur les fils */

      if (pGdc->high == one)
	return (simplifDcOneFPGABdd (pGon->low, pGdc->low));
      if (pGdc->low == one)
	return (simplifDcOneFPGABdd (pGon->high, pGdc->high));

      if (pGon->high == one &&
	  applyBinBdd (AND, notBdd (pGdc->high), pGon->low) == notBdd (pGdc->high))
	return (simplifDcOneFPGABdd (pGon->low, applyBinBdd (AND, pGdc->high, pGdc->low)));

      if (pGon->low == one &&
	  applyBinBdd (AND, pGon->high, notBdd (pGdc->low)) == notBdd (pGdc->low))
	return (simplifDcOneFPGABdd (pGon->high, applyBinBdd (AND, pGdc->high, pGdc->low)));

      /* cas general */

      pBddLoc = initVertexBdd (pGon->index, simplifDcOneFPGABdd (pGon->high, pGdc->high),
			       simplifDcOneFPGABdd (pGon->low, pGdc->low));
    }
  else
    /* index differents */

  if (pGon->index > pGdc->index)
    pBddLoc = initVertexBdd (pGon->index, simplifDcOneFPGABdd (pGon->high, pGdc),
			     simplifDcOneFPGABdd (pGon->low, pGdc));
  else
    pBddLoc = simplifDcOneFPGABdd (pGon, applyBinBdd (AND, pGdc->low, pGdc->high));

  addTabLoc (sysBdd.pMC, pGon, pGdc, pBddLoc, DONTCARE1);
  return (pBddLoc);
}

/*----------------------------------------------------------------------------
composeBdd       :Composition de deux graphes
------------------------------------------------------------------------------
soient index l'index de la variable de decomposition,
       G1 le graphe BDD a recalculer et G2 le graphe de V

on calcule G = G1(V(index)=0).not(G2(V)) + G1(V(index)=1).G2(V)
-----------------------------------------------------------------------
parametres 	 :un operateur,un pointeur de liste de Bdd.
-----------------------------------------------------------------------
return 		 :un pointeur de Bdd.
------------------------------------------------------------------------------*/

pNode 
composeBdd (pBdd1, pBdd2, index)
     pNode pBdd1, pBdd2;
     int index;
{
  pNode r1, r2, resul;

  r1 = constraintBdd (pBdd1, initVertexBdd (index, one, zero));

  if (r1 == pBdd1)		/* V n'apparaissait pas dans pBdd1 */
    return (pBdd1);

  r2 = constraintBdd (pBdd1, initVertexBdd (index, zero, one));

  resul = applyBinBdd (OR, applyBinBdd (AND, pBdd2, r1),
		       applyBinBdd (AND, notBdd (pBdd2), r2));

  return (resul);
}

/*------------------------------------------------------------------------------
addListBdd  : 	ajoute un nouveau noeud dans la liste des noeuds.
-------------------------------------------------------
parametres   :	pointeur sur le debut de la liste.
-------------------------------------------------------
return 	     :	nouveau pointeur debut de la liste.
------------------------------------------------------------------------------*/

chain_list *
addListBdd (pt, pBdd)
     chain_list *pt;
     pNode pBdd;
{
  chain_list *new_lstGdb, *pCur, *pCurSup;
  int index;


  index = pBdd->index;

  new_lstGdb = addchain (NULL, (void *) pBdd);

  /* l'insertion par index decroissant */

  if (pt == NULL)
    return (new_lstGdb);

  pCur = pt;
  pCurSup = pCur;
  while (pCur->NEXT != NULL && index < ((pNode) (pCur->NEXT)->DATA)->index)
    {
      pCurSup = pCur;
      pCur = pCur->NEXT;
    }

  if (index < ((pNode) pCur->DATA)->index)	/* ajout apres */
    {
      new_lstGdb->NEXT = pCur->NEXT;
      pCur->NEXT = new_lstGdb;
    }
  else
    /* ajout avant */
    {
      new_lstGdb->NEXT = pCur;
      if (pt == pCur)		/* ajout devant la liste */
	return (new_lstGdb);
      pCurSup->NEXT = new_lstGdb;
    }
  return (pt);
}
/*----------------------------------------------------------------------------
oneBdd       :tautologie
------------------------------------------------------------------------------
renvoie 1 si le graphe est une tautologie
-----------------------------------------------------------------------
parametres 	 :un pointeur de Bdd.
-----------------------------------------------------------------------
return 		 :un int.
------------------------------------------------------------------------------*/

int 
oneBdd (pBdd)
     pNode pBdd;
{
  if (pBdd == one)
    return (1);
  else
    return (0);
}
/*----------------------------------------------------------------------------
zeroBdd       :antilogie
------------------------------------------------------------------------------
renvoie 1 si le graphe est une antilogie
-----------------------------------------------------------------------
parametres 	 :un pointeur de Bdd.
-----------------------------------------------------------------------
return 		 :un int.
------------------------------------------------------------------------------*/

int 
zeroBdd (pBdd)
     pNode pBdd;
{
  if (pBdd == zero)
    return (1);
  else
    return (0);
}
/*----------------------------------------------------------------------------
equalBdd       :egalite de Bdd
------------------------------------------------------------------------------
renvoie 1 si Bdd1 = Bdd2 
-----------------------------------------------------------------------
parametres 	 :deux pointeurs de Bdd.
-----------------------------------------------------------------------
return 		 :un short.
------------------------------------------------------------------------------*/

int 
equalBdd (pBdd1, pBdd2)
     pNode pBdd1, pBdd2;
{
  if (pBdd1 == pBdd2)
    return (1);
  else
    return (0);
}

/*------------------------------------------------------------------------------
markBdd         :met a jour les marques d'un BDD.
-------------------------------------------------------
parametres 	:un pointeur de Bdd,la valeur du marquage.
-------------------------------------------------------
return 		:rien.
------------------------------------------------------------------------------*/
void 
markBdd (pBdd, value)
     pNode pBdd;
     short value;
{
  if (pBdd->index > 1)
    {
      if (pBdd->mark != value)	/* noeud non encore marque */
	{
	  pBdd->mark = value;
	  markBdd (pBdd->high, value);
	  markBdd (pBdd->low, value);
	}
    }
}

/*-------------------------------------------------------------------------
upVarBdd 	: remonte une variable dans un BDD a l'index i. 
---------------------------------------------------------------------------
retour		: un pNode.
---------------------------------------------------------------------------*/
pNode 
upVarBdd (pF, pFoldIndex, newIndex)
     pNode pF, pFoldIndex;
     short newIndex;
{
  pNode pBddLoc;

  pBddLoc = searchTabLoc (sysBdd.pMC, pF, pFoldIndex, newIndex + 10);
  if (pBddLoc != NULL)
    return (pBddLoc);

  if (pF->index < pFoldIndex->index)
    return (pF);
  if (pF->index > newIndex)
    pBddLoc = initVertexBdd (pF->index, upVarBdd (pF->high, pFoldIndex, newIndex),
			     upVarBdd (pF->low, pFoldIndex, newIndex));
  else
    /* pF->index < newIndex */

    pBddLoc = initVertexBdd (newIndex,
			     constraintBdd (pF, pFoldIndex),
			     constraintBdd (pF, notBdd (pFoldIndex)));
  addTabLoc (sysBdd.pMC, pF, pFoldIndex, pBddLoc, newIndex + 10);
  return (pBddLoc);
}
/*-------------------------------------------------------------------------
markAllBdd 	: marque tous les noeuds BDDs dans la table de reduction . 
---------------------------------------------------------------------------
retour		: void.
---------------------------------------------------------------------------*/
void 
markAllBdd (value)
     short value;
{
  pNode pBdd, *ppBdd;
  int i;

  ppBdd = (sysBdd.pRT)->pBdd;

  for (i = 0; i < sysBdd.pRT->lenTableBdd; i++)
    {
      pBdd = *ppBdd;
      if (pBdd != NULL)
	pBdd->mark = value;
      ppBdd++;
    }
}

/*-------------------------------------------------------------------------
supportChain_listBdd 	: calcule le support en noeud d'un graphe.
---------------------------------------------------------------------------
retour		: une liste chainee
---------------------------------------------------------------------------*/
void 
supportBddInt (pt, ppCL)
     pNode pt;
     chain_list **ppCL;
{
  if (pt->index > 1)
    if (pt->mark == 0)
      {
	*ppCL = addchain (*ppCL, (void *) pt);
	supportBddInt (pt->low, ppCL);
	supportBddInt (pt->high, ppCL);
	pt->mark = 1;
      }
}

chain_list *
supportChain_listBdd (pBdd)
     pNode pBdd;
{
  chain_list *res;

  res = NULL;
  markBdd (pBdd, 0);
  supportBddInt (pBdd, &res);
  markBdd (pBdd, 0);
  return (res);
}

/*------------------------------------------------------------------------------
initVertexBddAux : creation d'un noeud dans la structure sysCible. 
-------------------------------------------------------
parametres 	 : index,high,low et system cible. 
-------------------------------------------------------
return 		 : le noeud cree. 
------------------------------------------------------------------------------*/
pNode 
initVertexBddAux (index, high, low, sysCible)
     short index;
     pNode high;
     pNode low;
     struct systemBdd *sysCible;
{
  pNode pt;


  if ((pt = searchTableBdd (sysCible->pRT, index, high, low)) != NULL)
    if (pt != BDDTABLE_PLEINE)
      return (pt);
    else
      {
	sysCible->pRT = reAllocTableBdd (sysCible->pRT);
	return (initVertexBddAux (index, high, low, sysCible));
      }

  if (high == low)
    {
      printf ("gcNode : error - there's a node that isn't reduced\n");
      exit (-1);
    }

  if (sysCible->indiceAT == MAX_PACK)
    {
      sysCible->pAT = (pNode) mbkalloc (MAX_PACK * sizeof (struct node));
      sysCible->indiceAT = 1;
      sysCible->lpAT = addchain (sysCible->lpAT, (void *) sysCible->pAT);
    }
  else
    {
      sysCible->pAT++;
      sysCible->indiceAT++;
    }

  pt = sysCible->pAT;
  pt->index = index;
  pt->high = high;
  pt->low = low;
  pt->mark = 0;
  if (index > 1)
    if (addTableBdd (sysCible->pRT, pt) == TABLE_PLEINE)	/* table pleine */
      {
	sysCible->pRT = reAllocTableBdd (sysCible->pRT);
	return (initVertexBddAux (index, high, low, sysCible));
      }
  return (pt);
}

/*------------------------------------------------------------------------------
regenereBdd	 : regnere un Bdd dans une structure systeme cible.
-------------------------------------------------------
parametres 	 : pointeur Bdd et une structure cible. 
-------------------------------------------------------
return 		 : le pointeur dans la structure system cible.
------------------------------------------------------------------------------*/
pNode 
regenereBdd (pBdd, sysCible, pTHNode)
     pNode pBdd;
     struct systemBdd *sysCible;
     pTH pTHNode;
{
  int resul;

  if ((resul = searchTH (pTHNode, (char *)pBdd)) != EMPTYTH)
    return ((pNode) resul);
  else
    {
      if (pBdd->index < 2)
	{
	  printf ("gcNode : error - bad index %d\n", pBdd->index);
	  exit (-1);
	}
      resul = (int) initVertexBddAux (pBdd->index,
				regenereBdd (pBdd->high, sysCible, pTHNode),
				 regenereBdd (pBdd->low, sysCible, pTHNode),
				      sysCible);
      addTH (pTHNode, (char *)pBdd, resul);
      return ((pNode) resul);
    }
}
/*------------------------------------------------------------------------------
gcNodeBdd	 :effectue un garbage collecteur sur tous les noeuds de systeme
		  en sauvegardant les noeuds pointes par les pNode de la liste 
		  chainee pChain. 
-------------------------------------------------------
parametres 	 : pointeur de chain_list. 
-------------------------------------------------------
return 		 : 1 si ok ;
		   0 si erreur .
------------------------------------------------------------------------------*/
void 
gcNodeBdd (pt)
     chain_list *pt;
{
  struct systemBdd sysBddAux;
  pNode zeroAux, oneAux;
  pTH pTHNode;

  pTHNode = createTH (MEDIUM);
  sysBddAux.pRT = createTableBdd (MEDIUM);
  sysBddAux.pMC = createTabLoc (MEDIUM);
  sysBddAux.indiceAT = MAX_PACK;
  sysBddAux.lpAT = NULL;
  zeroAux = initVertexBddAux (0, (pNode) 0, (pNode) 1, &sysBddAux);
  oneAux = initVertexBddAux (1, (pNode) 0, (pNode) 1, &sysBddAux);
  addTH (pTHNode, (char *)zero, (int) zeroAux);
  addTH (pTHNode, (char *)one, (int) oneAux);

  while (pt)
    {
      pNode ptNode;
      ptNode = regenereBdd ((pNode) pt->DATA, &sysBddAux, pTHNode);
      pt->DATA = ((void *) ptNode);
      pt = pt->NEXT;
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
/*------------------------------------------------------------------------------
supportIndexBdd	 : calcule le support d'un BDD 
                   index decroissant : sens = 1
                   index croissant :   sens = 0
-------------------------------------------------------
parametres 	 : pNode 
-------------------------------------------------------
return 		 : une liste chainee d'index 
------------------------------------------------------------------------------*/

			/* fonction interne */

void 
rempTabIndex (pt, tabIndex)
     pNode pt;
     char *tabIndex;
{
  if (pt->index > 1 && pt->mark == 0)
    {
      tabIndex[pt->index - 2] = 'O';
      pt->mark = 1;
      rempTabIndex (pt->high, tabIndex);
      rempTabIndex (pt->low, tabIndex);
    }
}


chain_list *
supportIndexBdd (pt, sens)
     pNode pt;
     int sens;
{
  char *tabIndex;
  int i;
  chain_list *ret;

  /*  initialisation du tableau d'index utilises */

  tabIndex = (char *) mbkalloc (pt->index - 1);
  for (i = 0; i <= pt->index - 2; i++)
    tabIndex[i] = 'N';

  rempTabIndex (pt, tabIndex);
  markBdd (pt, 0);

  ret = NULL;

  if (sens == 1)		/* index decroissant */
    {
      for (i = 0; i <= pt->index - 2; i++)
	if (tabIndex[i] == 'O')
	  ret = addchain (ret, (void *)(i + 2));
    }
  else
    {
      for (i = pt->index - 2; i >= 0; i--)
	if (tabIndex[i] == 'O')
	  ret = addchain (ret, (void *)(i + 2));
    }
  mbkfree (tabIndex);
  return ret;
}
