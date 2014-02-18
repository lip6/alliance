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
 * Tool    : Logic Synthesis - boolean optimisation
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 * Modified by Ludo 2002
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mut.h"
#include "log.h"
#include "beh.h"

pTH pTabExpr;
chain_list *ptr_ctrlst;

/*-------------------------------------------------------------------------
nameIntVect 	: calcule un identificateur en gardant les vecteurs.
                  utilise par proof...
---------------------------------------------------------------------------
retour		: un pointeur de chaine de caracteres.
---------------------------------------------------------------------------*/
char *
nameIntVect (name)
     char *name;
{
  char *name1;
  char *retour;
  int i;
  int length = strlen (name);

  name1 = (char *) mbkalloc (length + 2);
  strcpy (name1, name);
  i = 0;
  while (name1[i] != '\0')
    {
      if (name1[i] == '(')
	name1[i] = '[';
      if (name1[i] == ')')
	{
	  name1[i] = ']';
	}
      if (name1[i] == ' ')
	name1[i] = '_';
      i++;
    }
  name = namealloc (name1);
  mbkfree (name1);		/* chaine */

  if (name[0] == 'a' && name[1] == 'u' && name[2] == 'x')
    {
      length = strlen (name);
      name1 = (char *) mbkalloc (length + 3);

      strcpy (name1, name);
      name1[length] = '_';
      name1[length + 1] = 'a';
      name1[length + 2] = '\0';
      retour = namealloc (name1);
      mbkfree (name1);		/* chaine */
    }
  else
    retour = name;

  return (retour);
}

/*------------------------------------------------------------------
  IDENT : mayCoverBdd
  ------------------------------------------------------------------
  FUNCTION : calcule si deux fonctions phi-Boolennes ont une 
             couverture commune 
  ------------------------------------------------------------------ */

int
mayCoverBdd (on1, dc1, on2, dc2)
     pNode on1, dc1, on2, dc2;
{
  if ((applyBinBdd (AND, applyBinBdd (OR, on1, dc1), on2) == on2) &&
      (applyBinBdd (AND, applyBinBdd (OR, on2, dc2), on1) == on1))
    return 1;
  else
    return 0;
}

/*------------------------------------------------------------------
  IDENT : displayExprLax
  ------------------------------------------------------------------
  FUNCTION : affiche l'expression a la sauce LISP --> (oper list_arg)
  ------------------------------------------------------------------ */

void
displayExprLaxInt (expr)
     chain_list *expr;
{
  int oper;

  if (ATOM (expr))
    printf (" %s", VALUE_ATOM (expr));
  else
    {
      oper = OPER (expr);
      if (oper >= 0 && oper <= 1000)
	{
	  printf (" (%s", operToChar (OPER (expr)));
	  mapExpr (displayExprLaxInt, expr);
	  printf (")");
	}
      else
	{
	  printf ("\ndisplayExprLax : error - unknown operator %d\n", oper);
	  exit (-1);
	}
    }
}

void
displayExprLax (expr)
     chain_list *expr;
{
  displayExprLaxInt (expr);
  printf ("\n");
}


	/* recherche d'une expression a partir d'un nom de sortie */

chain_list *
searchExprOutBeh (beh, name)
     befig_list *beh;
     char *name;
{
  beout_list *out = beh->BEOUT;

  while (out)
    {
      if (!strcmp (out->NAME, name))
	return (out->ABL);
      out = out->NEXT;
    }
  return ((chain_list *) NULL);
}

/*------------------------------------------------------------------------
substExprBeh : substitue dans toutes les expressions de 'beh'
               'name' par 'expr'
--------------------------------------------------------------------------
retour       : rien
--------------------------------------------------------------------------*/
void
substExprBeh (beh, name, sexpr)
     befig_list *beh;
     char *name;
     chain_list *sexpr;
{
  beout_list *out;
  bereg_list *reg;
  bebus_list *bus;
  bebux_list *bux;
  biabl_list *biabl;
  beaux_list *aux;
  chain_list *expr;


  aux = beh->BEAUX;
  while (aux)
    {
      if (aux->ABL)
	{
	  expr = substExpr (aux->ABL, name, sexpr);
	  freeExpr (aux->ABL);
	  aux->ABL = expr;
	}
      aux = aux->NEXT;
    }

  out = beh->BEOUT;
  while (out)
    {
      if (out->ABL)
	{
	  expr = substExpr (out->ABL, name, sexpr);
	  freeExpr (out->ABL);
	  out->ABL = expr;
	}
      out = out->NEXT;
    }

  reg = beh->BEREG;
  while (reg)
    {
      biabl = reg->BIABL;
      while (biabl)
	{
	  if (biabl->CNDABL && biabl->VALABL)
	    {
	      expr = substExpr (biabl->CNDABL, name, sexpr);
	      freeExpr (biabl->CNDABL);
	      biabl->CNDABL = expr;


	      expr = substExpr (biabl->VALABL, name, sexpr);
	      freeExpr (biabl->VALABL);
	      biabl->VALABL = expr;
	    }
	  biabl = biabl->NEXT;
	}
      reg = reg->NEXT;
    }

  bus = beh->BEBUS;
  while (bus)
    {
      biabl = bus->BIABL;
      while (biabl)
	{
	  if (biabl->CNDABL && biabl->VALABL)
	    {
	      expr = substExpr (biabl->CNDABL, name, sexpr);
	      freeExpr (biabl->CNDABL);
	      biabl->CNDABL = expr;

	      expr = substExpr (biabl->VALABL, name, sexpr);
	      freeExpr (biabl->VALABL);
	      biabl->VALABL = expr;
	    }
	  biabl = biabl->NEXT;
	}
      bus = bus->NEXT;
    }

  bux = beh->BEBUX;
  while (bux)
    {
      biabl = bux->BIABL;
      while (biabl)
	{
	  if (biabl->CNDABL && biabl->VALABL)
	    {
	      expr = substExpr (biabl->CNDABL, name, sexpr);
	      freeExpr (biabl->CNDABL);
	      biabl->CNDABL = expr;

	      expr = substExpr (biabl->VALABL, name, sexpr);
	      freeExpr (biabl->VALABL);
	      biabl->VALABL = expr;
	    }
	  biabl = biabl->NEXT;
	}
      bux = bux->NEXT;
    }
}

/*------------------------------------------------------------------------
polarBDBeh : inverse la polarite des BD d'une befig 
--------------------------------------------------------------------------
retour       : rien
--------------------------------------------------------------------------*/
void
polarBDBeh (beh)
     befig_list *beh;
{
  bereg_list *reg;
  biabl_list *biabl;

  /* inversion de polarite des cones DATA des BD */

  reg = beh->BEREG;
  while (reg)
    {
      int BD = 0;

      biabl = reg->BIABL;
      while (biabl)
	{
	  if (biabl->USER)	/* BD detectee */
	    {
	      BD = 1;
	      /* inversion de polarite */

	      biabl->VALABL = notExpr (biabl->VALABL);

	      /* les don't care "'d'" ne doivent pas etre inverses */

	      substPhyExpr (biabl->VALABL, "'d'", notExpr (createAtom ("'d'")));
	    }
	  biabl = biabl->NEXT;
	}
      /* substitution generalisee dans la befig */

      if (BD)
	substExprBeh (beh, reg->NAME, notExpr (createAtom (reg->NAME)));

      reg = reg->NEXT;
    }
}

/*------------------------------------------------------------------------
elimInoutBeh : patch qui permet de voir un inout comme un out
--------------------------------------------------------------------------
retour       : rien
--------------------------------------------------------------------------*/
void
elimInoutBeh (beh)
     befig_list *beh;
{
  bepor_list *port;
  chain_list *expr;

  /* recherche des INOUT qui ne sont pas des bus */

  port = beh->BEPOR;
  while (port)
    {
      if (port->DIRECTION == 'B' && port->TYPE == 'B')
	{
	  printf ("WARNING : %s is considered as an output signal\n", port->NAME);

	  /* on recherche l'expression associee */

	  expr = searchExprOutBeh (beh, port->NAME);
	  if (expr == NULL)
	    {
	      printf ("elimInoutSignal : error signal hasn't expression\n");
	      exit (-1);
	    }
	  beh->BEAUX = beh_addbeaux (beh->BEAUX, port->NAME, expr, NULL);

	  /* elimination de la liste des BERIN */

	  beh->BERIN = beh_rmvberin (beh->BERIN, port->NAME);
	}
      port = port->NEXT;
    }
}
/*-------------------------------------------------------------------------
sumCNDBinode 	: calcule la somme des commandes d'un bus
---------------------------------------------------------------------------
retour		:un pNode. 
---------------------------------------------------------------------------*/
pNode
sumCNDBinode (binode)
     binode_list *binode;
{
  pNode res = zero;
  while (binode)
    {
      res = applyBinBdd (OR, binode->CNDNODE, res);
      binode = binode->NEXT;
    }
  return res;
}

/*-------------------------------------------------------------------------
AND2	: creation d'un and2 
---------------------------------------------------------------------------
retour		: une expression 
---------------------------------------------------------------------------*/
chain_list *
AND2 (expr1, expr2)
     chain_list *expr1, *expr2;
{
  chain_list *ret;

  ret = addchain (NULL, (void *) addchain (NULL, (void *) AND));
  ret->NEXT = addchain (ret->NEXT, (void *) expr2);
  ret->NEXT = addchain (ret->NEXT, (void *) expr1);
  return ret;
}

/*-------------------------------------------------------------------------
OR2	: creation d'un or2 
---------------------------------------------------------------------------
retour		: une expression 
---------------------------------------------------------------------------*/
chain_list *
OR2 (expr1, expr2)
     chain_list *expr1, *expr2;
{
  chain_list *ret;

  ret = addchain (NULL, (void *) addchain (NULL, (void *) OR));
  ret->NEXT = addchain (ret->NEXT, (void *) expr2);
  ret->NEXT = addchain (ret->NEXT, (void *) expr1);
  return ret;
}


/*-------------------------------------------------------------------------
sumBiabl 	: calcule la FND  en ABL (ON-set) d'un bus
---------------------------------------------------------------------------
retour		:un chain_list *. 
---------------------------------------------------------------------------*/
chain_list *
sumBiabl (biabl)
     biabl_list *biabl;
{
  chain_list *res = createAtom ("'0'");

  while (biabl)
    {
      res = OR2 (AND2 (copyExpr (biabl->VALABL), copyExpr (biabl->CNDABL)), res);
      biabl = biabl->NEXT;
    }
  flatArityExpr (res);
  return res;
}


/*------------------------------------------------------------------------
elimBusMuxBeh : transformation des bus mutiplex en mutiplex combinatoire 
                il est necessaire d'avoir deja calculer les BDD
--------------------------------------------------------------------------
retour        : 1 si un bus a ete transforme, 0 sinon
--------------------------------------------------------------------------*/
int
elimBusMuxBeh (beh)
     befig_list *beh;
{
  bebus_list *bus;
  bebux_list *bux;
  chain_list *delBus = NULL;
  int retour = 0;


  bus = beh->BEBUS;
  while (bus)
    {
      if (sumCNDBinode (bus->BINODE) == one)
	{
	  delBus = addchain (delBus, bus);
	  beh->BEOUT = beh_addbeout (beh->BEOUT, bus->NAME,
				     sumBiabl (bus->BIABL), NULL);

	  retour = 1;
	}
      bus = bus->NEXT;
    }

  while (delBus)
    {
      beh->BEBUS = beh_delbebus (beh->BEBUS, (bebus_list *) (delBus->DATA), 'Y');
      delBus = delBus->NEXT;
    }

  delBus = NULL;

  bux = beh->BEBUX;
  while (bux)
    {
      if (sumCNDBinode (bux->BINODE) == one)
	{
	  delBus = addchain (delBus, bux);
	  beh->BEAUX = beh_addbeaux (beh->BEAUX, bux->NAME,
				     sumBiabl (bux->BIABL), NULL);

	  /* pour les bus internes (points d'arret) 
	     on elimine leur occurence dans BERIN */

	  beh->BERIN = beh_rmvberin (beh->BERIN, bux->NAME);
	  retour = 1;
	}
      bux = bux->NEXT;
    }

  while (delBus)
    {
      beh->BEBUX = beh_delbebux (beh->BEBUX, (bebux_list *) (delBus->DATA), 'Y');
      delBus = delBus->NEXT;
    }

  return retour;
}

/*-------------------------------------------------------------------------
berinToChain_list 	: prend l'odre d'apparition.
---------------------------------------------------------------------------
retour		: un pointeur de chain_list.
---------------------------------------------------------------------------*/
chain_list *
berinToChain_list (beh)
     befig_list *beh;
{
  berin_list *in;
  chain_list *lst;

  in = beh->BERIN;		/* entrees primaires */
  lst = NULL;
  while (in)
    {
      lst = addchain (lst, (void *) in->NAME);
      in = in->NEXT;
    };

  return (lst);
}

/*-------------------------------------------------------------------------
videBddBefig 	: vide tous les bdd calcules dans une befig. 
---------------------------------------------------------------------------
retour		: rien.
---------------------------------------------------------------------------*/
void
videBddBefig (beh)
     befig_list *beh;
{
  beout_list *out;
  beaux_list *aux;
  bebus_list *bus;
  bebux_list *bux;
  bereg_list *reg;
  bemsg_list *msg;
  binode_list *binode;

  out = beh->BEOUT;
  while (out)			/* les sorties primaires */
    {
      out->NODE = NULL;
      out = out->NEXT;
    }

  aux = beh->BEAUX;
  while (aux)			/* les auxiliaires */
    {
      aux->NODE = NULL;
      aux = aux->NEXT;
    }

  bus = beh->BEBUS;
  while (bus)			/* les bus */
    {
      binode = bus->BINODE;
      while (binode)
	{
	  binode->CNDNODE = NULL;
	  binode->VALNODE = NULL;
	  binode = binode->NEXT;
	}
      bus = bus->NEXT;
    }

  bux = beh->BEBUX;
  while (bux)			/* les bux */
    {
      binode = bux->BINODE;
      while (binode)
	{
	  binode->CNDNODE = NULL;
	  binode->VALNODE = NULL;
	  binode = binode->NEXT;
	}
      bux = bux->NEXT;
    }


  reg = beh->BEREG;
  while (reg)			/* les registres */
    {
      binode = reg->BINODE;
      while (binode)
	{
	  binode->CNDNODE = NULL;
	  binode->VALNODE = NULL;
	  binode = binode->NEXT;
	}
      reg = reg->NEXT;
    }
  msg = beh->BEMSG;
  while (msg)			/* les messages */
    {
      msg->NODE = NULL;
      msg = msg->NEXT;
    }
}

/*-------------------------------------------------------------------------
countInputBeh 	: compte les entrees d'un circuit (berin)
---------------------------------------------------------------------------
retour		: un int.
---------------------------------------------------------------------------*/

int
countInputBeh (beh)
     befig_list *beh;
{
  berin_list *in;
  int count = 0;

  in = beh->BERIN;		/* entrees primaires */
  while (in)
    {
      in = in->NEXT;
      count++;
    };

  return (count);
}

/*-------------------------------------------------------------------------
existAuxBeh	: verifie l'existence de name dans BEAUX de beh. 
---------------------------------------------------------------------------
retour		: 1 si OK, 0 sinon. 
---------------------------------------------------------------------------*/
int
existAuxBeh (beh, name)
     befig_list *beh;
     char *name;
{
  beaux_list *aux = beh->BEAUX;
  while (aux)
    {
      if (!strcmp (name, aux->NAME))
	return (1);
      aux = aux->NEXT;
    }
  return (0);
}

/* ###--------------------------------------------------------------### */
/* function     : makeBddAbl                                            */
/* description  : transform recursively abl in bdd                      */
/* algorithm    : if ATOM(expr)is an operand --terminal variable,       */
/*                then                                                  */
/*                  if (it is a constant or a primary variable)         */
/*                  then return that bdd that is already known          */
/*                  else                                                */
/*                    if ((it is an auxilliary variable)&&              */
/*                        (there is no self-dependence))                */
/*                    then return makeBddAbl(that terminal variable)    */
/*                    else ERROR                                        */
/*                else apply OPER(expr)                                 */
/*                      to makeBddAbl(every operands)                   */
/* called func. : seachInputCct <bdd>, createNodeTermBdd <bdd>,         */
/*              : addListBdd <bdd>, applyBdd <bdd>,                     */
/*              : addchain <mbk>, freechain <mbk>,                      */
/*              : vhb_mfrfrst                                           */
/*              : makeBddAbl -- that function is recursive !!!          */
/* ###--------------------------------------------------------------### */

pNode
makeBddAbl (ptr_circuit, expr, ptr_befig, vhb_count, createAux, ppAux)
     pCircuit ptr_circuit;	/* circuit for level 1 bdd functions    */
     chain_list *expr;		/* pointer abl expression               */
     struct befig *ptr_befig;	/* pointer on  behavioural figure       */
     int *vhb_count;
     int createAux;
     chain_list **ppAux;
{
  short oper;
  pNode pt;
  chain_list *lstGbd;
  chain_list *ptr_cntrl = ptr_ctrlst;
  struct beaux *ptr_beaux = ptr_befig->BEAUX;
  int index;
  char *atom;

  if (expr == NULL)
    {
      printf ("makeBddAbl : Error - ABL = NULL \n");
      exit (1);
    }

  if (ATOM (expr))
    {				/* is atom a constant ?           */
      atom = (char *) VALUE_ATOM (expr);

      if (!strcmp (atom, "'0'"))
	return (zero);
      if (!strcmp (atom, "'1'"))
	return (one);
      if (!strcmp (atom, "'d'"))
	return (zero);
      /* is atom a primary variable ?     */
      if ((index = searchInputCct (ptr_circuit, atom)) != VIDETH)
	{
	  return (createNodeTermBdd (index));
	}

      else
	/* neither constant, nor primary     */
	{			/* it must be an auxilliary variable */

	  while ((ptr_beaux != NULL) && (strcmp (ptr_beaux->NAME, atom)))
	    ptr_beaux = ptr_beaux->NEXT;

	  if (ptr_beaux == NULL)
	    {
	      printf ("makeBddAbl : Error - signal '%s' not found\n", atom);
	      exit (1);
	    }
	  else
	    /* does that signal depends on        */
	    {			/* himself ? ptr_ctrlst is the        */
	      /* dependance list of expression being */
	      /* transformed from abl to bdd        */

	      while ((ptr_cntrl != NULL) && ((char *) ptr_cntrl->DATA != atom))
		ptr_cntrl = ptr_cntrl->NEXT;

	      if (ptr_cntrl != NULL)
		/* to ensure that no signal depends  */
		/* on himself                        */
		{		/* if it does it's a fatal error!    */
		  printf ("makeBddAbl : Error - signal '%s' belongs to a combinatory loop \n", atom);
		  exit (1);
		}
	      else
		{
		  if (ptr_beaux->NODE == NULL)	/* no treated signal */
		    {
		      ptr_ctrlst = addchain (ptr_ctrlst, (void *) atom);
		      /* add a signal to the dependences    */
		      /* list that ptr_ctrlst is pointing at */
		      ptr_beaux->NODE = makeBddAbl (ptr_circuit, ptr_beaux->ABL, ptr_befig,
					       vhb_count, createAux, ppAux);

		      *vhb_count = *vhb_count + 1;
		      ptr_ctrlst = delchain (ptr_ctrlst, ptr_ctrlst);


		      if (numberNodeBdd (ptr_beaux->NODE) > createAux)
			{
			  *ppAux = addchain (*ppAux, ptr_beaux->NAME);
			  return (createNodeTermBdd
			      (addInputCct (ptr_circuit, ptr_beaux->NAME)));
			}

		      return (ptr_beaux->NODE);
		    }
		  else
		    {
		      return (ptr_beaux->NODE);
		    }
		}
	    }
	}
    }
  else
    {
      oper = OPER (expr);	/* operator to be applyed on lstgbd  */
      lstGbd = NULL;		/* list of operands                  */
      while ((expr = CDR (expr)))
	{
	  pNode ptNode = makeBddAbl (ptr_circuit, CAR (expr), ptr_befig,
				     vhb_count, createAux, ppAux);
	  lstGbd = addListBdd (lstGbd, ptNode);
	}

      /* patch pour stable */

      if (oper == STABLE)
	oper = NOT;
      pt = applyBdd (oper, lstGbd);
      freechain (lstGbd);
      return (pt);
    }
}

/* ###--------------------------------------------------------------### */
/* function     : makeBddBeh                                            */
/* description  : evalue bdds for each input, output or bus, connector  */
/*                register signal, and auxilliary signal of ptr_befig   */
/* called func. : makeBddAbl, addchain <mbk>                            */
/* ###--------------------------------------------------------------### */


int
makeBddBeh (ptr_befig, ptr_circuit, reorder, trace, max_node, createAux, ppAux)
     struct befig *ptr_befig;	/* pointer on current BEFIG     */
     pCircuit ptr_circuit;	/* pointer on current circuit   */
     int reorder;		/* order parameter              */
     int trace;
     int max_node;
     int createAux;
     chain_list **ppAux;

{
  pNode makeBddAbl ();
  int vhb_numberNode = numberNodeAllBdd ();
  int vhb_count = 0;

  ptr_ctrlst = NULL;

  if (trace)
    {
      printf ("Running Abl2Bdd on `%s`\n", ptr_befig->NAME);
    }


  if (ptr_befig->BEOUT != NULL)
    {
      struct beout *ptr_beout;
      ptr_beout = ptr_befig->BEOUT;
      while (ptr_beout != NULL)
	{
	  if ((ptr_beout->ABL != NULL) && (ptr_beout->NODE == NULL))
	    {
	      ptr_ctrlst = addchain (ptr_ctrlst, (void *) ptr_beout->NAME);
	      ptr_beout->NODE = makeBddAbl (ptr_circuit, ptr_beout->ABL, ptr_befig, &vhb_count, createAux, ppAux);
	      ptr_ctrlst = delchain (ptr_ctrlst, ptr_ctrlst);
	    }

	  ptr_beout = ptr_beout->NEXT;
	  vhb_count++;
	  if (vhb_count > 1 &&
	      reorder &&
	    ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
	    {
	      return (1);
	    }
	}
    }

  if (ptr_befig->BEBUS != NULL)
    {
      struct bebus *ptr_bebus;
      ptr_bebus = ptr_befig->BEBUS;
      while (ptr_bebus != NULL)
	{
	  if (ptr_bebus->BIABL != NULL)
	    {
	      struct biabl *ptr_biabl;
	      struct binode *ptr_binode;
	      ptr_biabl = ptr_bebus->BIABL;
	      ptr_binode = ptr_bebus->BINODE;
	      while (ptr_biabl != NULL)
		{
		  if (ptr_biabl->CNDABL != NULL)
		    {
		      ptr_binode->CNDNODE = makeBddAbl (ptr_circuit, ptr_biabl->CNDABL, ptr_befig, &vhb_count, createAux, ppAux);
		    }

		  vhb_count++;
		  if (vhb_count > 1 &&
		      reorder &&
		      ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
		    {
		      return (1);
		    }

		  if (ptr_biabl->VALABL != NULL)
		    {
		      ptr_ctrlst = addchain (ptr_ctrlst, (void *) ptr_bebus->NAME);
		      ptr_binode->VALNODE = makeBddAbl (ptr_circuit, ptr_biabl->VALABL, ptr_befig, &vhb_count, createAux, ppAux);
		      ptr_ctrlst = delchain (ptr_ctrlst, ptr_ctrlst);
		    }

		  vhb_count++;
		  if (vhb_count > 1 &&
		      reorder &&
		      ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
		    {
		      return (1);
		    }

		  ptr_biabl = ptr_biabl->NEXT;
		  ptr_binode = ptr_binode->NEXT;
		}
	    }
	  ptr_bebus = ptr_bebus->NEXT;
	}
    }

  if (ptr_befig->BEBUX != NULL)
    {
      struct bebux *ptr_bebux;
      ptr_bebux = ptr_befig->BEBUX;
      while (ptr_bebux != NULL)
	{
	  if (ptr_bebux->BIABL != NULL)
	    {
	      struct biabl *ptr_biabl;
	      struct binode *ptr_binode;
	      ptr_biabl = ptr_bebux->BIABL;
	      ptr_binode = ptr_bebux->BINODE;
	      while (ptr_biabl != NULL)
		{
		  if (ptr_biabl->CNDABL != NULL)
		    {
		      ptr_binode->CNDNODE = makeBddAbl (ptr_circuit, ptr_biabl->CNDABL, ptr_befig, &vhb_count, createAux, ppAux);
		    }

		  vhb_count++;
		  if (vhb_count > 1 &&
		      reorder &&
		      ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
		    {
		      return (1);
		    }

		  if (ptr_biabl->VALABL != NULL)
		    {
		      ptr_ctrlst = addchain (ptr_ctrlst, (void *) ptr_bebux->NAME);
		      ptr_binode->VALNODE = makeBddAbl (ptr_circuit, ptr_biabl->VALABL, ptr_befig, &vhb_count, createAux, ppAux);
		      ptr_ctrlst = delchain (ptr_ctrlst, ptr_ctrlst);
		    }

		  vhb_count++;
		  if (vhb_count > 1 &&
		      reorder &&
		      ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
		    {
		      return (1);
		    }

		  ptr_biabl = ptr_biabl->NEXT;
		  ptr_binode = ptr_binode->NEXT;
		}
	    }
	  ptr_bebux = ptr_bebux->NEXT;
	}
    }

  if (ptr_befig->BEREG != NULL)
    {
      struct bereg *ptr_bereg;
      ptr_bereg = ptr_befig->BEREG;
      while (ptr_bereg != NULL)
	{
	  if (ptr_bereg->BIABL != NULL)
	    {
	      struct biabl *ptr_biabl;
	      struct binode *ptr_binode;
	      ptr_biabl = ptr_bereg->BIABL;
	      ptr_binode = ptr_bereg->BINODE;
	      while (ptr_biabl != NULL)
		{
		  if (ptr_biabl->CNDABL != NULL)
		    {
		      ptr_binode->CNDNODE = makeBddAbl (ptr_circuit, ptr_biabl->CNDABL, ptr_befig, &vhb_count, createAux, ppAux);
		    }

		  vhb_count++;
		  if (vhb_count > 1 &&
		      reorder &&
		      ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
		    {
		      return (1);
		    }

		  if (ptr_biabl->VALABL != NULL)
		    {
		      ptr_ctrlst = addchain (ptr_ctrlst, (void *) ptr_bereg->NAME);
		      ptr_binode->VALNODE = makeBddAbl (ptr_circuit, ptr_biabl->VALABL, ptr_befig, &vhb_count, createAux, ppAux);
		      ptr_ctrlst = delchain (ptr_ctrlst, ptr_ctrlst);
		    }

		  vhb_count++;
		  if (vhb_count > 1 &&
		      reorder &&
		      ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
		    {
		      return (1);
		    }

		  ptr_biabl = ptr_biabl->NEXT;
		  ptr_binode = ptr_binode->NEXT;
		}
	    }
	  ptr_bereg = ptr_bereg->NEXT;
	}
    }

  if (ptr_befig->BEMSG != NULL)
    {
      struct bemsg *ptr_bemsg;
      ptr_bemsg = ptr_befig->BEMSG;
      while (ptr_bemsg != NULL)
	{
	  if ((ptr_bemsg->ABL != NULL) && (ptr_bemsg->NODE == NULL))
	    {
	      ptr_bemsg->NODE = makeBddAbl (ptr_circuit, ptr_bemsg->ABL, ptr_befig, &vhb_count, createAux, ppAux);
	    }
	  ptr_bemsg = ptr_bemsg->NEXT;
	}
    }

  /* on devrait traiter les noeuds auxilliaires par profondeur
     decroissante ... */

  if (ptr_befig->BEAUX != NULL)
    {
      struct beaux *ptr_aux;
      ptr_aux = ptr_befig->BEAUX;
      while (ptr_aux != NULL)
	{
	  if ((ptr_aux->ABL != NULL) && (ptr_aux->NODE == NULL))
	    {
	      ptr_ctrlst = addchain (ptr_ctrlst, (void *) ptr_aux->NAME);
	      ptr_aux->NODE = makeBddAbl (ptr_circuit, ptr_aux->ABL, ptr_befig, &vhb_count, createAux, ppAux);
	      ptr_ctrlst = delchain (ptr_ctrlst, ptr_ctrlst);
	      addOutputCct (ptr_circuit, ptr_aux->NAME, ptr_aux->NODE);
	    }
	  ptr_aux = ptr_aux->NEXT;
	  vhb_count++;
	  if (vhb_count > 1 &&
	      reorder &&
	    ((numberNodeAllBdd () - vhb_numberNode) / vhb_count) > max_node)
	    {
	      return (1);
	    }
	}
    }

  /* destruction des auxiliares */

  destroyTH (ptr_circuit->pTO);
  ptr_circuit->pTO = createTH (100);

  return (0);
}

/*------------------------------------------------------------------------------
gcNodeBeh	 :effectue un garbage collecteur sur tous les noeuds de systeme
		  en sauvegardant les noeuds pointes par les pNode de la liste 
		  des befig .
-------------------------------------------------------
parametres 	 : pointeur sur befig .
-------------------------------------------------------
return 		 : 1 si ok ;
		   0 si erreur .
------------------------------------------------------------------------------*/
void
gcNodeBeh (beh, lstbeh)
     befig_list *beh;
     int lstbeh;
{
  struct systemBdd sysBddAux;
  beout_list *out;
  bereg_list *reg;
  bebus_list *bus;
  bebux_list *bux;
  binode_list *binode;
  beaux_list *aux;
  bemsg_list *mesg;
  pNode zeroAux, oneAux;
  pTH pTHNode;

  if (sysBdd.pRT->compteur > LARGE)
    {
      pTHNode = createTH (LARGE);
      sysBddAux.pRT = createTableBdd (LARGE);
    }
  else if (sysBdd.pRT->compteur > MEDIUM)
    {
      pTHNode = createTH (MEDIUM);
      sysBddAux.pRT = createTableBdd (MEDIUM);
    }
  else
    {
      pTHNode = createTH (SMALL);
      sysBddAux.pRT = createTableBdd (SMALL);
    }

  sysBddAux.pMC = createTabLoc (MEDIUM);
  sysBddAux.indiceAT = MAX_PACK;
  sysBddAux.lpAT = NULL;
  zeroAux = initVertexBddAux (0, (pNode)0, (pNode)1, &sysBddAux);
  oneAux = initVertexBddAux (1, (pNode)0, (pNode)1, &sysBddAux);
  addTH (pTHNode, (char *)zero, (long)zeroAux);
  addTH (pTHNode, (char *)one, (long)oneAux);

  /* si lstbeh = 1 on garbage sur toutes les befig */
  /* sinon on ne prends que la premiere befig */

  do
    {
      out = beh->BEOUT;
      while (out)
	{
	  if (out->NODE)
	    {
	      out->NODE = regenereBdd (out->NODE, &sysBddAux, pTHNode);
	    }
	  out = out->NEXT;
	}

      aux = beh->BEAUX;
      while (aux)
	{
	  if (aux->NODE)
	    {
	      aux->NODE = regenereBdd (aux->NODE, &sysBddAux, pTHNode);
	    }
	  aux = aux->NEXT;
	}

      reg = beh->BEREG;
      while (reg)
	{
	  binode = reg->BINODE;
	  while (binode)
	    {
	      if (binode->CNDNODE && binode->VALNODE)
		{
		  binode->CNDNODE = regenereBdd (binode->CNDNODE, &sysBddAux, pTHNode);
		  binode->VALNODE = regenereBdd (binode->VALNODE, &sysBddAux, pTHNode);
		}
	      binode = binode->NEXT;
	    }
	  reg = reg->NEXT;
	}

      bus = beh->BEBUS;
      while (bus)
	{
	  binode = bus->BINODE;
	  while (binode)
	    {
	      if (binode->CNDNODE && binode->VALNODE)
		{
		  binode->CNDNODE = regenereBdd (binode->CNDNODE, &sysBddAux, pTHNode);
		  binode->VALNODE = regenereBdd (binode->VALNODE, &sysBddAux, pTHNode);
		}
	      binode = binode->NEXT;
	    }
	  bus = bus->NEXT;
	}

      bux = beh->BEBUX;
      while (bux)
	{
	  binode = bux->BINODE;
	  while (binode)
	    {
	      if (binode->CNDNODE && binode->VALNODE)
		{
		  binode->CNDNODE = regenereBdd (binode->CNDNODE, &sysBddAux, pTHNode);
		  binode->VALNODE = regenereBdd (binode->VALNODE, &sysBddAux, pTHNode);
		}
	      binode = binode->NEXT;
	    }
	  bux = bux->NEXT;
	}

      mesg = beh->BEMSG;
      while (mesg)
	{
          if ( mesg->NODE )
            {
	       mesg->NODE = regenereBdd (mesg->NODE, &sysBddAux, pTHNode);
            }
	  mesg = mesg->NEXT;
	}

      beh = beh->NEXT;
    }
  while (beh && lstbeh == 1);

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


/*------------------------------------------------------------------------
orderProofBeh      : calcul d'un ordonnancement pour 2 circuits 
--------------------------------------------------------------------------
retour        : 1 si un bus a ete transforme, 0 sinon
--------------------------------------------------------------------------*/
void
orderProofBeh (bef1, bef2, varaux)
     befig_list *bef1, *bef2;
     char varaux;
{
  chain_list *order;
  chain_list *ptChain;
  beaux_list *aux;
  chain_list *makeOrderAbl ();

  order = berinToChain_list (bef1);

  videBddBefig (bef1);
  videBddBefig (bef2);

  /* creation des bdd */

  bef1->CIRCUI = initializeCct (bef1->NAME, 5 * countInputBeh (bef1), 100);
  bef2->CIRCUI = initializeCct (bef2->NAME, 5 * countInputBeh (bef2), 100);

  ptChain = order;
  while (ptChain)
    {
      addInputCct (bef1->CIRCUI, ptChain->DATA);
      ptChain = ptChain->NEXT;
    }

  /* recherche des auxiliaires communs */

  if (varaux == 'A')
    {
      aux = bef1->BEAUX;
      printf ("Looking for the common auxiliary signals :\n");
      while (aux)
	{
	  if (existAuxBeh (bef2, aux->NAME))
	    {
	      addInputCct (bef1->CIRCUI, aux->NAME);
	      printf ("%s ", aux->NAME);
	      order = addchain (order, aux->NAME);
	    }
	  aux = aux->NEXT;
	}
      printf ("\n\n");
    }

  makeBddBeh (bef1, bef1->CIRCUI, 0, 0, 100, 10000000, NULL);

  printf ("---> final number of nodes = %ld", (sysBdd.pRT)->compteur);

  gcNodeBeh (bef1, 0);

  printf ("(%ld)\n\n", (sysBdd.pRT)->compteur);

  /* calcul des graphes pour le 2eme circuit */

  cpOrderCct (bef1->CIRCUI, bef2->CIRCUI);

  /* ajout des inputs specifiques a bef2 */

  ptChain = berinToChain_list (bef2);
  while (ptChain)
    {
      if (searchInputCct (bef2->CIRCUI, ptChain->DATA) == EMPTYTH)
	{
	  printf ("Warning : Input '%s' only exists in the second description\n",
		  (char *)ptChain->DATA);
	  addInputCct (bef2->CIRCUI, ptChain->DATA);
	}
      ptChain = ptChain->NEXT;
    }

  makeBddBeh (bef2, bef2->CIRCUI, 0, 1, 100, 100000000, NULL);
}


/*-------------------------------------------------------------------------
verifIOBeh 	: verifie la coherence des interfaces des deux befig. 
---------------------------------------------------------------------------
retour		: le nombre d'erreurs trouvees.
---------------------------------------------------------------------------*/
int
verifIOBeh (beh1, beh2, mode)
     befig_list *beh1, *beh2;
     int mode;
{
  berin_list *in1, *in2;
  beout_list *out1, *out2;
  bebus_list *bus1, *bus2;
  bebux_list *bux1, *bux2;
  bereg_list *reg1, *reg2;
  int numberError = 0;


  in1 = beh1->BERIN;
  while (in1)			/* les sorties primaires */
    {
      in2 = beh2->BERIN;
      while (in2 && strcmp (in1->NAME, in2->NAME))
	{
	  in2 = in2->NEXT;
	}
      if (in2 == NULL)
	{
	  if (mode)
	    printf ("Primary input '%s' exists only in '%s'\n", in1->NAME, beh1->NAME);
	  if (mode)
	    printf ("\n");
	  numberError++;
	}
      in1 = in1->NEXT;
    }
  in2 = beh2->BERIN;
  while (in2)
    {
      in1 = beh1->BERIN;
      while (in1 && strcmp (in1->NAME, in2->NAME))
	{
	  in1 = in1->NEXT;
	}
      if (in1 == NULL)
	{
	  if (mode)
	    printf ("Primary input '%s' exists only in '%s'\n", in2->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  numberError++;
	}
      in2 = in2->NEXT;
    }


  out1 = beh1->BEOUT;
  while (out1)			/* les sorties primaires */
    {
      out2 = beh2->BEOUT;
      while (out2 && strcmp (out1->NAME, out2->NAME))
	{
	  out2 = out2->NEXT;
	}
      if (out2 == NULL)
	{
	  if (mode)
	    printf ("Primary output '%s' exists only in '%s'\n", out1->NAME, beh1->NAME);
	  if (mode)
	    printf ("\n");
	  numberError++;
	}
      out1 = out1->NEXT;
    }

  out2 = beh2->BEOUT;
  while (out2)
    {
      out1 = beh1->BEOUT;
      while (out1 && strcmp (out1->NAME, out2->NAME))
	{
	  out1 = out1->NEXT;
	}
      if (out1 == NULL)
	{
	  if (mode)
	    printf ("Primary output '%s' exists only in '%s'\n", out2->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  numberError++;
	}
      out2 = out2->NEXT;
    }


  bus1 = beh1->BEBUS;
  while (bus1)			/* les bus */
    {
      bus2 = beh2->BEBUS;
      while (bus2 && strcmp (bus1->NAME, bus2->NAME))	/* recherche du meme ident. */
	{
	  bus2 = bus2->NEXT;
	}
      if (bus2 == NULL)
	{
	  if (mode)
	    printf ("Bus '%s' exists only in '%s'\n",
		    bus1->NAME, beh1->NAME);
	  numberError++;
	}
      bus1 = bus1->NEXT;
    }

  bus2 = beh2->BEBUS;
  while (bus2)
    {
      bus1 = beh1->BEBUS;
      while (bus1 && strcmp (bus1->NAME, bus2->NAME))	/* recherche du meme ident. */
	{
	  bus1 = bus1->NEXT;
	}
      if (bus1 == NULL)
	{
	  if (mode)
	    printf ("Bus '%s' exists only in '%s'\n", bus2->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  numberError++;
	}
      bus2 = bus2->NEXT;
    }


  bux1 = beh1->BEBUX;
  while (bux1)			/* les bux */
    {
      bux2 = beh2->BEBUX;
      while (bus2 && strcmp (bux1->NAME, bux2->NAME))	/* recherche du meme ident. */
	{
	  bux2 = bux2->NEXT;
	}
      if (bux2 == NULL)
	{
	  if (mode)
	    printf ("Bux '%s' exists only in '%s'\n",
		    bux1->NAME, beh1->NAME);
	  numberError++;
	}
      bux1 = bux1->NEXT;
    }

  bux2 = beh2->BEBUX;
  while (bux2)
    {
      bux1 = beh1->BEBUX;
      while (bux1 && strcmp (bux1->NAME, bux2->NAME))	/* recherche du meme ident. */
	{
	  bux1 = bux1->NEXT;
	}
      if (bux1 == NULL)
	{
	  if (mode)
	    printf ("Bux '%s' exists only in '%s'\n", bux2->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  numberError++;
	}
      bux2 = bux2->NEXT;
    }

  reg1 = beh1->BEREG;
  while (reg1)			/* les registres */
    {
      reg2 = beh2->BEREG;
      while (reg2 && strcmp (reg1->NAME, reg2->NAME))	/* recherche du meme ident. */
	{
	  reg2 = reg2->NEXT;
	}
      if (reg2 == NULL)
	{
	  if (mode)
	    printf ("Register '%s' exists only in '%s'\n",
		    reg1->NAME, beh1->NAME);
	  numberError++;
	}
      reg1 = reg1->NEXT;
    }

  reg2 = beh2->BEREG;
  while (reg2)
    {
      reg1 = beh1->BEREG;
      while (reg1 && strcmp (reg1->NAME, reg2->NAME))	/* recherche du meme ident. */
	{
	  reg1 = reg1->NEXT;
	}
      if (reg1 == NULL)
	{
	  if (mode)
	    printf ("Register '%s' exists only in '%s'\n", reg2->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  numberError++;
	}
      reg2 = reg2->NEXT;
    }

  return (numberError);
}


/*-------------------------------------------------------------------------
verifPNode 	: verifie la coherence le calcul des graphes 
---------------------------------------------------------------------------
retour		: 1 si OK, 0 sinon. 
---------------------------------------------------------------------------*/
int
verifPNode (beh1, beh2, mode)
     befig_list *beh1, *beh2;
     int mode;
{
  beout_list *out;
  beaux_list *aux;
  bebus_list *bus;
  bebux_list *bux;
  bereg_list *reg;
  binode_list *binode;


  aux = beh1->BEAUX;
  while (aux)
    {
      if (aux->NODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : auxiliary signal '%s' have no BDD in '%s'\n",
		    aux->NAME, beh1->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      aux = aux->NEXT;
    }

  aux = beh2->BEAUX;
  while (aux)
    {
      if (aux->NODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : auxiliary signal '%s' have no BDD in '%s'\n",
		    aux->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      aux = aux->NEXT;
    }

  out = beh1->BEOUT;
  while (out)			/* les sorties primaires */
    {
      if (out->NODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : output signal '%s' have no BDD in '%s'\n",
		    out->NAME, beh1->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      out = out->NEXT;
    }

  out = beh2->BEOUT;
  while (out)			/* les sorties primaires */
    {
      if (out->NODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : output signal '%s' have no BDD in '%s'\n",
		    out->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      out = out->NEXT;
    }

  bux = beh1->BEBUX;
  while (bux)			/* les bux */
    {
      binode = bux->BINODE;
      if (binode->CNDNODE == NULL || binode->VALNODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : bux signal '%s' have no BDD in '%s'\n",
		    bux->NAME, beh1->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      bux = bux->NEXT;
    }

  bux = beh2->BEBUX;
  while (bux)			/* les bux */
    {
      binode = bux->BINODE;
      if (binode->CNDNODE == NULL || binode->VALNODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : bux signal '%s' have no BDD in '%s'\n",
		    bux->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      bux = bux->NEXT;
    }

  bus = beh1->BEBUS;
  while (bus)			/* les bus */
    {
      binode = bus->BINODE;
      if (binode->CNDNODE == NULL || binode->VALNODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : bus signal '%s' have no BDD in '%s'\n",
		    bus->NAME, beh1->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      bus = bus->NEXT;
    }

  bus = beh2->BEBUS;
  while (bus)			/* les bus */
    {
      binode = bus->BINODE;
      if (binode->CNDNODE == NULL || binode->VALNODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : bus signal '%s' have no BDD in '%s'\n",
		    bus->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      bus = bus->NEXT;
    }

  reg = beh1->BEREG;
  while (reg)
    {
      binode = reg->BINODE;
      if (binode->CNDNODE == NULL || binode->VALNODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : bus signal '%s' have no BDD in '%s'\n",
		    reg->NAME, beh1->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      reg = reg->NEXT;
    }

  reg = beh2->BEREG;
  while (reg)
    {
      binode = reg->BINODE;
      if (binode->CNDNODE == NULL || binode->VALNODE == NULL)
	{
	  if (mode)
	    printf ("Fatal error : bus signal '%s' have no BDD in '%s'\n",
		    reg->NAME, beh2->NAME);
	  if (mode)
	    printf ("\n");
	  return (0);
	}
      reg = reg->NEXT;
    }

  return (1);
}

/*-------------------------------------------------------------------------
sumBinode 	: calcule la FND (ON-set) d'un bus
---------------------------------------------------------------------------
retour		:un pNode. 
---------------------------------------------------------------------------*/
pNode
sumBinode (binode)
     binode_list *binode;
{
  pNode res = zero;
  while (binode)
    {
      res = applyBinBdd (OR, applyBinBdd (AND, binode->VALNODE, binode->CNDNODE), res);
      binode = binode->NEXT;
    }
  return res;
}

/*-------------------------------------------------------------------------
busOnToBddBeh 	: calcule le ON-set d'un bus 
---------------------------------------------------------------------------
retour		:un pNode. 
---------------------------------------------------------------------------*/
pNode
busOnToBddBeh (beh, name)
     befig_list *beh;
     char *name;
{
  bebus_list *bus = beh->BEBUS;
  binode_list *binode;
  pNode res;

  while (bus && strcmp (bus->NAME, name))
    bus = bus->NEXT;

  if (bus == NULL)
    {
      printf ("error : busOnToBdd %s doesn't exist as a bus in %s\n",
	      name, beh->NAME);
      exit (-1);
    }

  res = zero;
  binode = bus->BINODE;
  while (binode)
    {
      res = applyBinBdd (OR, applyBinBdd (AND, binode->VALNODE, binode->CNDNODE), res);
      binode = binode->NEXT;
    }
  return (res);
}

/*-------------------------------------------------------------------------
buxOnToBddBeh 	: calcule le ON-set d'un bux 
---------------------------------------------------------------------------
retour		:un pNode. 
---------------------------------------------------------------------------*/
pNode
buxOnToBddBeh (beh, name)
     befig_list *beh;
     char *name;
{
  bebux_list *bux = beh->BEBUX;
  binode_list *binode;
  pNode res;

  while (bux && strcmp (bux->NAME, name))
    bux = bux->NEXT;

  if (bux == NULL)
    {
      printf ("error : buxOnToBdd %s doesn't exist as a bux in %s\n",
	      name, beh->NAME);
      exit (-1);
    }

  res = zero;
  binode = bux->BINODE;
  while (binode)
    {
      res = applyBinBdd (OR, applyBinBdd (AND, binode->VALNODE, binode->CNDNODE), res);
      binode = binode->NEXT;
    }
  return (res);
}


/*-------------------------------------------------------------------------
displayDiff 	: affiche si possible les equations differentes
	          que l'on a trouve dans chaque description. 
---------------------------------------------------------------------------
retour		: rien.
---------------------------------------------------------------------------*/
void
displayDiff (pt1, pt2, beh1, beh2, mode)
     pNode pt1, pt2;
     befig_list *beh1, *beh2;
     int mode;
{
  if (pt1 == NULL || pt2 == NULL)
    {
      printf ("displayDiff : bad args - pNode = NULL\n");
      exit (-1);
    }
  else
    {
      if (mode)
	printf ("         Description %s :\n", beh1->NAME);
      if (numberNodeBdd(pt1) > 50)
	{
	  if (mode)
	    printf ("         Logical expression too big...impossible display!\n");
	}
      else
	{
	  if (mode == 1)
	    {
	      printf ("      ");
	      displayExprLax (bddToAbl (pt1, (beh1->CIRCUI)->pNameI));
	    }
	}
      if (mode)
	printf ("\n");
      if (mode)
	printf ("        Description %s :\n", beh2->NAME);
      if (numberNodeBdd (pt2) > 50)
	{
	  if (mode)
	    printf ("         Logical expression too big...impossible display!\n");
	}
      else
	{
	  if (mode == 1)
	    {
	      printf ("      ");
	      displayExprLax (bddToAbl (pt2, (beh2->CIRCUI)->pNameI));
	    }
	}
      if (mode)
	printf ("\n");
    }
}

/*-------------------------------------------------------------------------
exprToCLBeh 	: remplit une liste chainee de pointeurs sur tout les ABL
                  d'une BEFIG 
---------------------------------------------------------------------------
retour		: un pointeur de chain_list.
---------------------------------------------------------------------------*/
chain_list *
exprToCLBeh (beh)
     befig_list *beh;
{
  beout_list *out;
  beaux_list *aux;
  bebus_list *bus;
  bebux_list *bux;
  bereg_list *reg;
  bemsg_list *msg;
  biabl_list *biabl;
  chain_list *ret = NULL;

  out = beh->BEOUT;
  while (out)			/* les sorties primaires */
    {
      ret = addchain (ret, out->ABL);
      out = out->NEXT;
    }

  aux = beh->BEAUX;
  while (aux)			/* les auxiliaires */
    {
      ret = addchain (ret, aux->ABL);
      aux = aux->NEXT;
    }

  bus = beh->BEBUS;
  while (bus)			/* les bus */
    {
      biabl = bus->BIABL;
      while (biabl)
	{
	  ret = addchain (ret, biabl->VALABL);
	  ret = addchain (ret, biabl->CNDABL);
	  biabl = biabl->NEXT;
	}
      bus = bus->NEXT;
    }

  bux = beh->BEBUX;
  while (bux)			/* les bux */
    {
      biabl = bux->BIABL;
      while (biabl)
	{
	  ret = addchain (ret, biabl->VALABL);
	  ret = addchain (ret, biabl->CNDABL);
	  biabl = biabl->NEXT;
	}
      bux = bux->NEXT;
    }


  reg = beh->BEREG;
  while (reg)			/* les registres */
    {
      biabl = reg->BIABL;
      while (biabl)
	{
	  ret = addchain (ret, biabl->VALABL);
	  ret = addchain (ret, biabl->CNDABL);
	  biabl = biabl->NEXT;
	}
      reg = reg->NEXT;
    }
  msg = beh->BEMSG;
  while (msg)			/* les messages */
    {
      ret = addchain (ret, msg->ABL);
      msg = msg->NEXT;
    }
  return ret;
}

/*------------------------------------------------------------------------------
numberBddBeh     : calcule le nombre de BDD (out + reg + bux + bus)
-------------------------------------------------------
parametres 	 : une befig
-------------------------------------------------------
return 		 : int
------------------------------------------------------------------------------*/
int
numberBddBeh (beh)
     befig_list *beh;
{
  beout_list *out;
  bereg_list *reg;
  bebus_list *bus;
  bebux_list *bux;
  biabl_list *biabl;
  int numberGraph = 0;

  out = beh->BEOUT;
  while (out)
    {
      if (out->ABL)
	{
	  numberGraph++;
	}
      out = out->NEXT;
    }

  reg = beh->BEREG;
  while (reg)
    {
      biabl = reg->BIABL;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      numberGraph++;
	    }
	  biabl = biabl->NEXT;
	}
      reg = reg->NEXT;
    }

  bus = beh->BEBUS;
  while (bus)
    {
      biabl = bus->BIABL;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      numberGraph++;
	    }
	  biabl = biabl->NEXT;
	}
      bus = bus->NEXT;
    }

  bux = beh->BEBUX;
  while (bux)
    {
      biabl = bux->BIABL;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      numberGraph++;
	    }
	  biabl = biabl->NEXT;
	}
      bux = bux->NEXT;
    }
  return numberGraph;
}

/*------------------------------------------------------------------------------
CalculDCBeh      : calcule les DC d'une befig 
-------------------------------------------------------
parametres 	 : une befig, un tableau de on, de dc et un compteur
-------------------------------------------------------
return 		 : void
------------------------------------------------------------------------------*/
void
calculDCBeh (beh, pON, pDC)
     befig_list *beh;
     pNode *pON, *pDC;
{
  beout_list *out;
  beaux_list *aux;
  bereg_list *reg;
  bebus_list *bus;
  bebux_list *bux;
  biabl_list *biabl;
  binode_list *binode;
  int i = 0;

  /* ajout dans la couche circuit des BDD des auxiliaires */

  aux = beh->BEAUX;
  while (aux)
    {
      if (aux->NODE)
	addOutputCct (beh->CIRCUI, aux->NAME, aux->NODE);
      aux = aux->NEXT;
    }

  /* traitement des sorties primaires */

  out = beh->BEOUT;
  while (out)
    {
      if (out->ABL)
	{
	  pON[i] = out->NODE;

	  /* recherche des DC locaux */

	  if (searchExpr (out->ABL, "'d'"))
	    {
	      chain_list *expr;

	      expr = substExpr (out->ABL, "'d'", createAtom ("'1'"));

	      pDC[i] = applyBinBdd (AND, notBdd (pON[i]),
				    ablToBddCct (beh->CIRCUI, expr));
	      freeExpr (expr);
	    }
	  else
	    pDC[i] = zero;

	  /* report des DC provenant du support */
/*
   reportDC = sigmaDFBeh(supportChain_listExpr(out->ABL),out->NODE,
   beh,dejaTraite);
   if (reportDC != zero)
   pDC[i] = applyBinBdd(OR,pDC[i],reportDC);
 */

	  i++;
	}
      out = out->NEXT;
    }

  reg = beh->BEREG;
  while (reg)
    {
      biabl = reg->BIABL;
      binode = reg->BINODE;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      pON[i] = binode->VALNODE;

	      /* recherche des DC locaux */

	      if (searchExpr (biabl->VALABL, "'d'"))
		{
		  chain_list *expr;

		  expr = substExpr (biabl->VALABL, "'d'", createAtom ("'1'"));

		  pDC[i] = applyBinBdd (AND, notBdd (pON[i]),
					ablToBddCct (beh->CIRCUI, expr));
		  freeExpr (expr);
		}
	      else
		pDC[i] = zero;

	      /* report des DC provenant du support */
/*
   reportDC = sigmaDFBeh(supportChain_listExpr(biabl->VALABL),
   binode->VALNODE,beh,dejaTraite);

   if (reportDC != zero)
   pDC[i] = applyBinBdd(OR,pDC[i],reportDC);
 */

	      i++;
	    }
	  biabl = biabl->NEXT;
	  binode = binode->NEXT;
	}
      reg = reg->NEXT;
    }

  bus = beh->BEBUS;
  while (bus)
    {
      biabl = bus->BIABL;
      binode = bus->BINODE;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      pON[i] = binode->VALNODE;

	      /* recherche des DC locaux */

	      if (searchExpr (biabl->VALABL, "'d'"))
		{
		  chain_list *expr;

		  expr = substExpr (biabl->VALABL, "'d'", createAtom ("'1'"));

		  pDC[i] = applyBinBdd (AND, notBdd (pON[i]),
					ablToBddCct (beh->CIRCUI, expr));
		  freeExpr (expr);
		}
	      else
		pDC[i] = zero;

	      /* report des DC provenant du support */

/*
   reportDC = sigmaDFBeh(supportChain_listExpr(biabl->VALABL),
   binode->VALNODE,beh,dejaTraite);

   if (reportDC != zero)
   pDC[i] = applyBinBdd(OR,pDC[i],reportDC);
 */

	      i++;
	    }
	  biabl = biabl->NEXT;
	  binode = binode->NEXT;
	}
      bus = bus->NEXT;
    }

  bux = beh->BEBUX;
  while (bux)
    {
      biabl = bux->BIABL;
      binode = bux->BINODE;
      while (biabl)
	{
	  if (biabl->VALABL)
	    {
	      pON[i] = binode->VALNODE;

	      /* recherche des DC locaux */

	      if (searchExpr (biabl->VALABL, "'d'"))
		{
		  chain_list *expr;

		  expr = substExpr (biabl->VALABL, "'d'", createAtom ("'1'"));

		  pDC[i] = applyBinBdd (AND, notBdd (pON[i]),
					ablToBddCct (beh->CIRCUI, expr));
		  freeExpr (expr);
		}
	      else
		pDC[i] = zero;

	      /* report des DC provenant du support */

/*
   reportDC = sigmaDFBeh(supportChain_listExpr(biabl->VALABL),
   binode->VALNODE,beh,dejaTraite);

   if (reportDC != zero)
   pDC[i] = applyBinBdd(OR,pDC[i],reportDC);
 */

	      i++;
	    }
	  biabl = biabl->NEXT;
	  binode = binode->NEXT;
	}
      bux = bux->NEXT;
    }
}

/*-------------------------------------------------------------------------
proofBeh 	: Effectue une preuve formelle entre deux comportements. 
		  mode a 1 ==> affichage a l'ecran des erreurs.
 	          Tous les graphes doivent au prealable avoir ete calcule
	          dans les deux descriptions.
		  si exitIO est a 0, on ne sort pas si il existe une
                  difference dans l'interface.
---------------------------------------------------------------------------
retour		: le nombre d'erreurs trouvees.
---------------------------------------------------------------------------*/
int
proofBeh (beh1, beh2, mode, exitIO)
     befig_list *beh1, *beh2;
     int mode, exitIO;
{
  beout_list *out1, *out2;
  beaux_list *aux1, *aux2;
  bebus_list *bus1, *bus2;
  bebux_list *bux1, *bux2;
  bereg_list *reg1, *reg2;
  binode_list *binode1, *binode2;
  biabl_list *biabl1, *biabl2;
  pNode *pDC1 = NULL, *pON1;
  pNode *pDC2 = NULL, *pON2;
  int numberError = 0;
  int countDC1;
  int countDC2;
  int numOUTDC2;
  chain_list *lst, *lstExpr;
  int existDC = 0;
  int numberGraph = 0;

  /* existe-t-il des constantes 'd' dans les expressions ? */

  lstExpr = exprToCLBeh (beh1);
  lst = lstExpr;
  while (lst)
    {
      if (searchExpr ((chain_list *) lst->DATA, "'d'"))
	existDC = 1;
      lst = lst->NEXT;
    }
  freechain (lstExpr);

  lstExpr = exprToCLBeh (beh2);
  lst = lstExpr;
  while (lst)
    {
      if (searchExpr ((chain_list *) lst->DATA, "'d'"))
	existDC = 1;
      lst = lst->NEXT;
    }
  freechain (lstExpr);


  if (existDC)
    {
      int numberGraph1, numberGraph2;
      numberGraph1 = numberBddBeh (beh1);
      numberGraph2 = numberBddBeh (beh2);

      printf ("Don't Cares are taken into account...\n\n");

      /* le max des deux */

      if (numberGraph1 > numberGraph2)
	numberGraph = numberGraph1;
      else
	numberGraph = numberGraph2;

      pDC1 = (pNode *) mbkalloc (numberGraph * sizeof (pNode));
      pON1 = (pNode *) mbkalloc (numberGraph * sizeof (pNode));
      calculDCBeh (beh1, pON1, pDC1);

      pDC2 = (pNode *) mbkalloc (numberGraph * sizeof (pNode));
      pON2 = (pNode *) mbkalloc (numberGraph * sizeof (pNode));
      calculDCBeh (beh2, pON2, pDC2);
    }

  countDC1 = 0;
  countDC2 = 0;

  if (mode == 1)
    pTabExpr = createTH (10000);

  if (mode)
    printf ("--------------------------------------------------------------------------------\n");
  if (mode == 1)
    if (mode)
      printf ("             Formal proof with Ordered Binary Decision Diagrams between   \n\n");
  if (mode)
    printf ("             '%s'  and  '%s'\n", beh1->NAME, beh2->NAME);
  if (mode)
    printf ("--------------------------------------------------------------------------------\n");

  numberError = verifIOBeh (beh1, beh2, mode);
  if (numberError > 0 && exitIO)
    {
      if (mode)
	printf ("Fatal error : IO or/and register declaration differs\n");
      if (mode)
	printf ("--------------------------------------------------------------------------------\n");
      return (numberError);
    }
  if (verifPNode (beh1, beh2, mode) == 0)
    {
      if (mode)
	printf ("Warning : some OUT,AUX,REG or BUS have no expression  \n");
    }
  if (mode)
    printf ("==============================  PRIMARY OUTPUT  ===============================\n");
  out1 = beh1->BEOUT;
  while (out1)			/* les sorties primaires */
    {
      countDC2 = 0;
      out2 = beh2->BEOUT;
      while (out2 && strcmp (out1->NAME, out2->NAME))
	{
	  out2 = out2->NEXT;
	  countDC2++;
	}
      if (out2)
	{
	  if (!existDC)
	    {
	      if (out1->NODE != out2->NODE)
		{
		  if (mode)
		    printf ("Error : Behaviour of primary output '%s' differs in the two descriptions\n", out1->NAME);
		  displayDiff (out1->NODE, out2->NODE, beh1, beh2, mode);
		  if (mode)
		    printf ("\n");
		  numberError++;
		}
	    }
	  else
	    /* DC traitement */
	    {
	      if (!mayCoverBdd (out1->NODE, pDC1[countDC1], out2->NODE, pDC2[countDC2]))
		{
		  if (mode)
		    printf ("Error : Behaviour of primary output '%s' differs in the two descriptions\n", out1->NAME);
		  if (mode)
		    printf ("---> On-set :\n");
		  displayDiff (out1->NODE, out2->NODE, beh1, beh2, mode);
		  if (mode)
		    printf ("---> DC-set :\n");
		  displayDiff (pDC1[countDC1], pDC2[countDC2], beh1, beh2, mode);
		  if (mode)
		    printf ("\n");
		  numberError++;
		}
	    }
	}

      countDC1++;

      out1 = out1->NEXT;
    }


  if (mode)
    printf ("=============================  AUXILIARY SIGNAL  ==============================\n");
  aux1 = beh1->BEAUX;
  while (aux1)			/* les auxiliaires */
    {
      aux2 = beh2->BEAUX;
      while (aux2 && strcmp (aux1->NAME, aux2->NAME))
	{
	  aux2 = aux2->NEXT;
	}
      if (aux2)
	{
	  if (aux1->NODE != aux2->NODE)
	    {
	      if (mode)
		printf ("Warning : Behaviour of auxiliary signal '%s' differs in the two descriptions\n",
			aux1->NAME);
	      displayDiff (aux1->NODE, aux2->NODE, beh1, beh2, mode);
	      if (mode)
		printf ("\n");
	    }
	}
      aux1 = aux1->NEXT;
    }

  /* base pour trouver l'index des bdd DC2 dans la table pDC2 */

  numOUTDC2 = countDC1;

  if (mode)
    printf ("==============================  REGISTER SIGNAL  ==============================\n");
  reg1 = beh1->BEREG;
  while (reg1)			/* les registres */
    {
      reg2 = beh2->BEREG;
      countDC2 = numOUTDC2;
      while (reg2 && strcmp (reg1->NAME, reg2->NAME))	/* recherche du meme ident. */
	{
	  reg2 = reg2->NEXT;
	  countDC2++;
	}
      if (reg2)
	{
	  binode1 = reg1->BINODE;
	  biabl1 = reg1->BIABL;
	  while (binode1)	/* preuve sur les commandes et les valeurs */
	    {
	      binode2 = reg2->BINODE;	/* recherche de la meme commande */
	      biabl2 = reg2->BIABL;
	      while (binode2 && (binode1->CNDNODE != binode2->CNDNODE))
		{
		  binode2 = binode2->NEXT;
		  biabl2 = biabl2->NEXT;
		}
	      if (binode2)
		{
		  if (!existDC)
		    {
		      if (binode1->VALNODE != binode2->VALNODE)
			{
			  if (mode)
			    printf ("error : behaviour of register '%s' differs in the two descriptions\n", reg1->NAME);
			  if (mode)
			    printf ("        ---> Value differs for the command =\n");
			  if (mode == 1)
			    displayExprLax (bddToAbl(binode1->CNDNODE, (beh1->CIRCUI)->pNameI));
			  if (mode)
			    printf ("\n");
			  displayDiff (binode1->VALNODE, binode2->VALNODE, beh1, beh2, mode);
			  if (mode)
			    printf ("\n");
			  numberError++;
			}
		    }
		  else
		    /* traitement DC */
		    {
		      if (!mayCoverBdd (binode1->VALNODE, pDC1[countDC1], binode2->VALNODE, pDC2[countDC2]))
			{
			  if (mode)
			    printf ("error : behaviour of register '%s' differs in the two descriptions\n", reg1->NAME);
			  if (mode)
			    printf ("        ---> Value differs for the command =\n");
			  if (mode == 1)
			    displayExprLax (bddToAbl (binode1->CNDNODE, (beh1->CIRCUI)->pNameI));
			  if (mode)
			    printf ("---> On-set :\n");
			  displayDiff (binode1->VALNODE, binode2->VALNODE, beh1, beh2, mode);
			  if (mode)
			    printf ("---> DC-set :\n");
			  displayDiff (pDC1[countDC1], pDC2[countDC2], beh1, beh2, mode);
			  if (mode)
			    printf ("\n");
			  numberError++;
			}
		    }
		  /* bascule D ou latche */

		  if ((biabl1->USER == 0 && biabl2->USER) ||
		      (biabl2->USER == 0 && biabl1->USER))
		    {
		      if (mode)
			printf ("        ---> type of the register differs for the command =\n");
		      if (mode == 1)
			displayExprLax (bddToAbl (binode1->CNDNODE, (beh1->CIRCUI)->pNameI));

		      if (biabl1->USER)
			{
			  if (mode)
			    printf ("        %s is a latch in %s but is an edge triggered in %s\n",
				    reg2->NAME, beh2->NAME, beh1->NAME);
			}
		      else
			{
			  if (mode)
			    printf ("        %s is a latch in %s but is an edge triggered in %s\n",
				    reg1->NAME, beh1->NAME, beh2->NAME);
			}
		      numberError++;
		    }
		}
	      else
		{
		  if (mode)
		    printf ("Error : Register '%s' differs in the two descriptions\n",
			    reg1->NAME);
		  if (mode)
		    printf ("        ---> Driver exist only in %s for the command  =\n",
			    beh1->NAME);
		  if (mode == 1)
		    displayExprLax (bddToAbl (binode1->CNDNODE, (beh1->CIRCUI)->pNameI));
		  if (mode)
		    printf ("\n\n");
		  numberError++;
		}
	      binode1 = binode1->NEXT;
	      biabl1 = biabl1->NEXT;
	    }

	  binode2 = reg2->BINODE;
	  while (binode2)
	    {
	      binode1 = reg1->BINODE;
	      while (binode1 && (binode1->CNDNODE != binode2->CNDNODE))
		{
		  binode1 = binode1->NEXT;
		}

	      if (binode1 == NULL)
		{
		  if (mode)
		    printf ("Error : Register '%s' differs in the two descriptions\n",
			    reg2->NAME);
		  if (mode)
		    printf ("        ---> Driver exist only in %s for the command  = \n",
			    beh2->NAME);
		  if (mode == 1)
		    displayExprLax (bddToAbl (binode2->CNDNODE, (beh2->CIRCUI)->pNameI));
		  if (mode)
		    printf ("\n\n");
		  numberError++;
		}
	      binode2 = binode2->NEXT;
	    }
	}

      countDC1++;

      reg1 = reg1->NEXT;
    }


  if (mode)
    printf ("===============================  EXTERNAL BUS =================================\n");
  bus1 = beh1->BEBUS;
  while (bus1)			/* les bus */
    {
      bus2 = beh2->BEBUS;
      while (bus2 && strcmp (bus1->NAME, bus2->NAME))	/* recherche du meme ident. */
	{
	  bus2 = bus2->NEXT;
	}
      if (bus2)
	{
	  /* difference fonctionnelle */

	  if (busOnToBddBeh (beh1, bus1->NAME) != busOnToBddBeh (beh2, bus2->NAME))
	    {
	      binode1 = bus1->BINODE;
	      while (binode1)	/* preuve sur les commandes et les valeurs */
		{
		  binode2 = bus2->BINODE;	/* recherche de la meme commande */
		  while (binode2 && (binode1->CNDNODE != binode2->CNDNODE))
		    {
		      binode2 = binode2->NEXT;
		    }
		  if (binode2)
		    {
		      if (binode1->VALNODE != binode2->VALNODE)
			{
			  if (mode)
			    printf ("Error : Behaviour of bus '%s' differs in the two descriptions\n",
				    bus1->NAME);
			  if (mode)
			    printf ("        ---> Value differs for the command =\n");
			  if (mode == 1)
			    displayExprLax (bddToAbl (binode1->CNDNODE, (beh1->CIRCUI)->pNameI));
			  if (mode)
			    printf ("\n");
			  displayDiff (binode1->VALNODE, binode2->VALNODE, beh1, beh2, mode);
			  if (mode)
			    printf ("\n");
			  numberError++;
			}
		    }
		  else
		    {
		      if (mode)
			printf ("Error : Bus '%s' differs in the two descriptions\n",
				bus1->NAME);
		      if (mode)
			printf ("        ---> Driver exist only in %s for the command  =\n",
				beh1->NAME);
		      if (mode == 1)
			displayExprLax (bddToAbl (binode1->CNDNODE, (beh1->CIRCUI)->pNameI));
		      if (mode)
			printf ("\n\n");
		      numberError++;
		    }
		  binode1 = binode1->NEXT;
		}
	      binode2 = bus2->BINODE;
	      while (binode2)
		{
		  binode1 = bus1->BINODE;
		  while (binode1 && (binode1->CNDNODE != binode2->CNDNODE))
		    binode1 = binode1->NEXT;

		  if (binode1 == NULL)
		    {
		      if (mode)
			printf ("Error : Bus '%s' differs in the two descriptions\n",
				bus2->NAME);
		      if (mode)
			printf ("        ---> Driver exist only in %s for the command  = \n",
				beh2->NAME);
		      if (mode == 1)
			displayExprLax (bddToAbl (binode2->CNDNODE, (beh2->CIRCUI)->pNameI));
		      if (mode)
			printf ("\n\n");
		      numberError++;
		    }
		  binode2 = binode2->NEXT;
		}
	    }
	}

      bus1 = bus1->NEXT;
    }


  if (mode)
    printf ("================================ INTERNAL BUS =================================\n");
  bux1 = beh1->BEBUX;
  while (bux1)			/* les bux */
    {
      bux2 = beh2->BEBUX;
      while (bux2 && strcmp (bux1->NAME, bux2->NAME))	/* recherche du meme ident. */
	{
	  bux2 = bux2->NEXT;
	}
      if (bux2)
	{
	  /* difference fonctionnelle */

	  if (buxOnToBddBeh (beh1, bux1->NAME) != buxOnToBddBeh (beh2, bux2->NAME))
	    {
	      binode1 = bux1->BINODE;
	      while (binode1)	/* preuve sur les commandes et les valeurs */
		{
		  binode2 = bux2->BINODE;	/* recherche de la meme commande */
		  while (binode2 && (binode1->CNDNODE != binode2->CNDNODE))
		    {
		      binode2 = binode2->NEXT;
		    }
		  if (binode2)
		    {
		      if (binode1->VALNODE != binode2->VALNODE)
			{
			  if (mode)
			    printf ("Error : Behaviour of bus '%s' differs in the two descriptions\n",
				    bux1->NAME);
			  if (mode)
			    printf ("        ---> Value differs for the command =\n");
			  if (mode == 1)
			    displayExprLax (bddToAbl (binode1->CNDNODE, (beh1->CIRCUI)->pNameI));
			  if (mode)
			    printf ("\n");
			  displayDiff (binode1->VALNODE, binode2->VALNODE, beh1, beh2, mode);
			  if (mode)
			    printf ("\n");
			  numberError++;
			}
		    }
		  else
		    {
		      if (mode)
			printf ("Error : Bus '%s' differs in the two descriptions\n",
				bux1->NAME);
		      if (mode)
			printf ("        ---> Driver exist only in %s for the command  =\n",
				beh1->NAME);
		      if (mode == 1)
			displayExprLax (bddToAbl (binode1->CNDNODE, (beh1->CIRCUI)->pNameI));
		      if (mode)
			printf ("\n\n");
		      numberError++;
		    }
		  binode1 = binode1->NEXT;
		}
	      binode2 = bux2->BINODE;
	      while (binode2)
		{
		  binode1 = bux1->BINODE;
		  while (binode1 && (binode1->CNDNODE != binode2->CNDNODE))
		    binode1 = binode1->NEXT;

		  if (binode1 == NULL)
		    {
		      if (mode)
			printf ("Error : Bus '%s' differs in the two descriptions\n",
				bux2->NAME);
		      if (mode)
			printf ("        ---> Driver exist only in %s for the command  = \n",
				beh2->NAME);
		      if (mode == 1)
			displayExprLax (bddToAbl (binode2->CNDNODE, (beh2->CIRCUI)->pNameI));
		      if (mode)
			printf ("\n\n");
		      numberError++;
		    }
		  binode2 = binode2->NEXT;
		}
	    }
	}
      bux1 = bux1->NEXT;
    }

  if (mode)
    printf ("\n                            Formal Proof : ");
  if (numberError > 0)
    {
      if (mode)
	printf ("%d error%s detected\n\n", numberError, (numberError > 1) ? "s" : "");
    }
  else if (mode)
    printf ("OK\n\n");
  if (mode)
    printf ("pppppppppppppppppppppppprrrrrrrrrrrrooooooooooooooooooooooooooooofffffffffffffff\n");
  if (mode)
    printf ("--------------------------------------------------------------------------------\n");
  if (mode == 1)
    destroyTH (pTabExpr);

  return (numberError);
}
