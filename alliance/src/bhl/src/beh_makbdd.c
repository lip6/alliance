/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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

/* ###--------------------------------------------------------------### */
/* file		: beh_makbdd.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: VUONG H.N., L.A. TABUSSE, Pirouz BAZARGAN SABET	*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */
/* modifs	: DUNOYER J., VUILLEMIN L.				*/
/*		- Refonte du pointeur NODE pour avoir BDD, GEX,		*/
/*		  VARIABLE-LISTS, DERIVATE.				*/
/* 		- Changement de l'interface de beh_makbdd,		*/
/* 		  Plus de trace_flg, plus de aux_flg, mais un node_flg.	*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_abl2bdd						*/
/* description	: transform recursively an abl into a bdd 		*/
/* called func.	: searchInputCct, createNodeTermBdd, addListBdd,	*/
/*		  beh_abl2bdd   , applyBdd         , freechain ,	*/
/*		  beh_error     ,					*/
/* ###--------------------------------------------------------------### */

static pNode beh_abl2bdd (pt_befig, expr)

struct befig *pt_befig;		/* pointer on  behavioural figure	*/
struct chain *expr    ;		/* pointer abl expression		*/

  {
  short         oper        ;
  struct node  *pt_bdd      = zero;
  struct chain *lstGbd      = NULL;
  pCircuit      ptr_circuit = pt_befig->CIRCUI;
  int           err_flg     = 0;
  int           index       ;
  char         *atom        ;
  char          name [256]  ;

	/* ###------------------------------------------------------### */
	/*    check that the expression exists				*/
	/* ###------------------------------------------------------### */

  if (expr == NULL)
    err_flg = beh_error (2, NULL);
  else
    {

    if (ATOM(expr))
      {
	/* ###------------------------------------------------------### */
	/*    if the expression is a terminal (atom)			*/
	/* ###------------------------------------------------------### */

      atom = (char *) VALUE_ATOM (expr);

	/* ###------------------------------------------------------### */
	/*    if the terminal is a primary signal create a single node	*/
	/* with its index and return					*/
	/* ###------------------------------------------------------### */

      if ((index = searchInputCct (ptr_circuit, atom)) != VIDE)
        pt_bdd = createNodeTermBdd (index);
      else
        {
	/* ###------------------------------------------------------### */
	/* the terminal should be a const. '0', '1' or 'd' (don't care)	*/
	/* ###------------------------------------------------------### */

        if ((!strcmp(atom, "'0'")) || (!strcmp(atom, "'d'")))
          pt_bdd = zero;
        else
          {
          if (!strcmp(atom, "'1'"))
            pt_bdd = one;
          else
            {
            err_flg = beh_error (3, atom);
            }
          }
        }
      }
    else
      {
	/* ###------------------------------------------------------### */
	/*    if the expression is not a terminal (is a complex expr.)	*/
	/* get the operator, create the bdd of each operand then,	*/
	/* combine operands applying the operator			*/
	/* ###------------------------------------------------------### */

      lstGbd = NULL;

      if ((oper = OPER(expr)) != STABLE)
       {
       while ((expr = CDR(expr)) != NULL)
        lstGbd = addListBdd (lstGbd, beh_abl2bdd (pt_befig, CAR(expr)));

       pt_bdd = applyBdd (oper, lstGbd);
       freechain (lstGbd);
       }
      else
       {
	expr = CDR(expr);
	if (!ATOM(expr))
	  err_flg = beh_error (4, NULL);
        else
          {
	  atom = (char *)VALUE_ATOM (CAR(expr));

          if ((index = searchInputCct (ptr_circuit, atom)) != VIDE)
            {
            lstGbd = addListBdd (lstGbd, createNodeTermBdd (index));
	    sprintf (name, "%s'delayed", atom);
	    atom = namealloc (name);
            if ((index = searchInputCct (ptr_circuit, atom)) != VIDE)
              {
              lstGbd = addListBdd (lstGbd, createNodeTermBdd (index));
              pt_bdd = applyBdd (NXOR, lstGbd);
              }
            else
	      err_flg = beh_error (3, atom);
            freechain (lstGbd);
            }
          else
	    err_flg = beh_error (3, atom);
          }
        }
      }
    }

  if (err_flg != 0)
    pt_befig->ERRFLG = 1;

  return (pt_bdd);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_mbddtot						*/
/* description	: make bdds for all primary signals: simple and bussed	*/
/*		  outputs, simple and bussed internal signals and	*/
/*		  registers						*/
/* called func.	: beh_abl2bdd, addchain , delchain			*/
/* ###--------------------------------------------------------------### */

static void beh_mbddtot (pt_befig)

struct befig *pt_befig;		/* pointer on current BEFIG	*/

  {
  struct beaux  *pt_beaux;
  struct bemsg  *pt_bemsg;
  struct beout  *pt_beout;
  struct bebus  *pt_bebus;
  struct bebux  *pt_bebux;
  struct bereg  *pt_bereg;
  struct biabl  *pt_biabl;
  struct binode *pt_binode;
  struct node   *pt_cndbdd;
  struct node   *pt_valbdd;

	/* ###------------------------------------------------------### */
	/*    make a bdd for each simple internal signal		*/
	/* ###------------------------------------------------------### */

  pt_beaux = pt_befig->BEAUX;

  while (pt_beaux != NULL)
    {
    pt_valbdd = beh_abl2bdd (pt_befig, pt_beaux->ABL);

    if (pt_befig->TYPE & BEH_NODE_QUAD)
      ((struct bequad *)(pt_beaux->NODE))->BDD = pt_valbdd;
    else
      pt_beaux->NODE = pt_valbdd;

    pt_beaux = pt_beaux->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a bdd for each delayed internal signal		*/
	/* ###------------------------------------------------------### */

  pt_beaux = pt_befig->BEDLY;
  while (pt_beaux != NULL)
    {
    pt_valbdd = beh_abl2bdd (pt_befig, pt_beaux->ABL);

    if (pt_befig->TYPE & BEH_NODE_QUAD)
      ((struct bequad *)(pt_beaux->NODE))->BDD = pt_valbdd;
    else
      pt_beaux->NODE = pt_valbdd;
    pt_beaux = pt_beaux->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a bdd for each assertion				*/
	/* ###------------------------------------------------------### */

  pt_bemsg = pt_befig->BEMSG;
  while (pt_bemsg != NULL)
    {
    pt_valbdd = beh_abl2bdd (pt_befig, pt_bemsg->ABL);

    if (pt_befig->TYPE & BEH_NODE_QUAD)
      ((bequad_list *)(pt_bemsg->NODE))->BDD = pt_valbdd;
    else
      pt_bemsg->NODE = pt_valbdd;

    pt_bemsg = pt_bemsg->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a bdd for each simple output port			*/
	/* ###------------------------------------------------------### */

  pt_beout = pt_befig->BEOUT;
  while (pt_beout != NULL)
    {
    pt_valbdd = beh_abl2bdd (pt_befig, pt_beout->ABL);

    if (pt_befig->TYPE & BEH_NODE_QUAD)
      ((bequad_list *)(pt_beout->NODE))->BDD = pt_valbdd;
    else
      pt_beout->NODE = pt_valbdd;

    pt_beout = pt_beout->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a bdd for each bussed output port			*/
	/* ###------------------------------------------------------### */

  pt_bebus = pt_befig->BEBUS;
  while (pt_bebus != NULL)
    {
    pt_biabl  = pt_bebus->BIABL;
    pt_binode = pt_bebus->BINODE;

    while (pt_biabl != NULL)
      {
      pt_cndbdd = beh_abl2bdd (pt_befig, pt_biabl->CNDABL);
      pt_valbdd = beh_abl2bdd (pt_befig, pt_biabl->VALABL);

      if (pt_befig->TYPE & BEH_NODE_QUAD)
        {
        ((bequad_list *)(pt_binode->CNDNODE))->BDD = pt_cndbdd;
        ((bequad_list *)(pt_binode->VALNODE))->BDD = pt_valbdd;
        }
      else
        {
        pt_binode->CNDNODE = pt_cndbdd;
        pt_binode->VALNODE = pt_valbdd;
        }

      pt_biabl  = pt_biabl->NEXT;
      pt_binode = pt_binode->NEXT;
      }

    pt_bebus = pt_bebus->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a bdd for each bussed internal signal		*/
	/* ###------------------------------------------------------### */

  pt_bebux = pt_befig->BEBUX;
  while (pt_bebux != NULL)
    {
    pt_biabl  = pt_bebux->BIABL;
    pt_binode = pt_bebux->BINODE;

    while (pt_biabl != NULL)
      {
      pt_cndbdd = beh_abl2bdd (pt_befig, pt_biabl->CNDABL);
      pt_valbdd = beh_abl2bdd (pt_befig, pt_biabl->VALABL);

      if (pt_befig->TYPE & BEH_NODE_QUAD)
        {
        ((bequad_list *)(pt_binode->CNDNODE))->BDD = pt_cndbdd;
        ((bequad_list *)(pt_binode->VALNODE))->BDD = pt_valbdd;
        }
      else
        {
        pt_binode->CNDNODE = pt_cndbdd;
        pt_binode->VALNODE = pt_valbdd;
        }

      pt_biabl  = pt_biabl->NEXT ;
      pt_binode = pt_binode->NEXT;
      }
    pt_bebux = pt_bebux->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make a bdd for each internal register			*/
	/* ###------------------------------------------------------### */

  pt_bereg = pt_befig->BEREG;
  while (pt_bereg != NULL)
    {
    pt_biabl  = pt_bereg->BIABL;
    pt_binode = pt_bereg->BINODE;

    while (pt_biabl != NULL)
      {
      if (pt_befig->TYPE & BEH_NODE_QUAD)
        {
        ((bequad_list *)(pt_binode->CNDNODE))->BDD = 
                              beh_abl2bdd (pt_befig, pt_biabl->CNDABL);
        ((bequad_list *)(pt_binode->VALNODE))->BDD = 
                              beh_abl2bdd (pt_befig, pt_biabl->VALABL);
        }
      else
        {
        pt_binode->CNDNODE = beh_abl2bdd (pt_befig, pt_biabl->CNDABL);
        pt_binode->VALNODE = beh_abl2bdd (pt_befig, pt_biabl->VALABL);
        }

      pt_biabl  = pt_biabl->NEXT;
      pt_binode = pt_binode->NEXT;
      }
    pt_bereg = pt_bereg->NEXT;
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: beh_makbdd						*/
/* description	: translate all abls into bdds				*/
/* called func.	: beh_indexbdd, beh_mbddtot				*/
/* ###--------------------------------------------------------------### */

void beh_makbdd (pt_befig)

struct befig *pt_befig;

  {
  int err_flg = 0;

  if (!(pt_befig->TYPE & BEH_NODE_BDD));
    {
    beh_indexbdd (pt_befig);

        /* ###------------------------------------------------------### */
        /*    change ABLs to BDDs					*/
        /* ###------------------------------------------------------### */

    beh_mbddtot (pt_befig);
    pt_befig->TYPE |= BEH_NODE_BDD;

    if (err_flg != 0)
      pt_befig->ERRFLG = 1;
    }
  }
