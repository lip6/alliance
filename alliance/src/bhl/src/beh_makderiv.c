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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: beh_makderiv.c					*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Laurent VUILLEMIN					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function     : beh_derivSigByVar					*/
/* description  : derivate a BDD by a variable				*/
/* called func. : constraintBdd, applyBinBdd				*/
/* ###--------------------------------------------------------------### */

pNode beh_derivSigByVar (sigbdd, index_var)

pNode sigbdd   ;
short index_var;

  {
  pNode high  ;
  pNode low   ;
  pNode varbdd;

  varbdd = createNodeTermBdd (index_var);

  high   = constraintBdd (sigbdd, varbdd);
  low    = constraintBdd (sigbdd, notBdd(varbdd));

  return (applyBinBdd (XOR,high,low));
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_derivSigByAllVar					*/
/* description	: derivate a BDD by all variables			*/
/* called func.	: derivSigByVar, supportIndexBdd			*/
/* ###--------------------------------------------------------------### */

struct beder *beh_derivSigByAllVar (pt_bequad)

struct bequad *pt_bequad;

  {
  pNode         pt_node     ;
  struct chain *pt_varidx   ;
  struct beder *list_derive = NULL;

  pt_varidx = pt_bequad->VARLIST;
  while (pt_varidx != NULL)
    {
    pt_node     = beh_derivSigByVar (pt_bequad->BDD, (short)pt_varidx->DATA);
    list_derive = beh_addbeder      (list_derive   , pt_node, pt_varidx->DATA);

    pt_varidx   = pt_varidx->NEXT;
    }

  return (list_derive);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_makderiv						*/
/* description	: build the list of partial derivate for all signals	*/
/* called func.	: derivSigByAllVar					*/
/* ###--------------------------------------------------------------### */

void beh_makderiv (pt_befig)

struct befig *pt_befig;

  {
  struct beaux  *pt_beaux;
  struct beout  *pt_beout;
  struct bebus  *pt_bebus;
  struct bebux  *pt_bebux;
  struct bereg  *pt_bereg;
  struct bemsg  *pt_bemsg;
  struct binode *pt_binod;

	/* ###------------------------------------------------------### */
	/*   check that the BEFIG is ready to support the derivation	*/
	/* ###------------------------------------------------------### */

  if ((pt_befig->TYPE & BEH_NODE_QUAD) == 0)
    beh_error (6, NULL);

  else
    {
	/* ###------------------------------------------------------### */
	/*   simple output ports					*/
	/* ###------------------------------------------------------### */

    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      ((struct bequad *) (pt_beout->NODE))->DERIVATE =
        beh_derivSigByAllVar ((struct bequad *) (pt_beout->NODE));

      pt_beout = pt_beout->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   bussed output ports					*/
	/* ###------------------------------------------------------### */

    pt_bebus = pt_befig->BEBUS;
    while (pt_bebus != NULL)
      {
      pt_binod = pt_bebus->BINODE;
      while (pt_binod != NULL)
         {
         ((struct bequad *) (pt_binod->CNDNODE))->DERIVATE =
           beh_derivSigByAllVar ((struct bequad *) (pt_binod->CNDNODE));

         ((struct bequad *) (pt_binod->VALNODE))->DERIVATE =
           beh_derivSigByAllVar ((struct bequad *) (pt_binod->VALNODE));

         pt_binod = pt_binod->NEXT;
         }

      pt_bebus = pt_bebus->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   bussed internal signals					*/
	/* ###------------------------------------------------------### */

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      pt_binod = pt_bebux->BINODE;
      while (pt_binod != NULL)
         {
         ((struct bequad *) (pt_binod->CNDNODE))->DERIVATE =
           beh_derivSigByAllVar ((struct bequad *) (pt_binod->CNDNODE));

         ((struct bequad *) (pt_binod->VALNODE))->DERIVATE =
           beh_derivSigByAllVar ((struct bequad *) (pt_binod->VALNODE));

         pt_binod = pt_binod->NEXT;
         }
      pt_bebux = pt_bebux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   internal registers						*/
	/* ###------------------------------------------------------### */

    pt_bereg = pt_befig->BEREG;
    while (pt_bereg != NULL)
      {
      pt_binod = pt_bereg->BINODE;
      while (pt_binod != NULL)
         {
         ((struct bequad *) (pt_binod->CNDNODE))->DERIVATE =
           beh_derivSigByAllVar ((struct bequad *) (pt_binod->CNDNODE));

         ((struct bequad *) (pt_binod->VALNODE))->DERIVATE =
           beh_derivSigByAllVar ((struct bequad *) (pt_binod->VALNODE));

         pt_binod = pt_binod->NEXT;
         }
      pt_bereg = pt_bereg->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   internal simple signals					*/
	/* ###------------------------------------------------------### */

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      ((struct bequad *) (pt_beaux->NODE))->DERIVATE =
        beh_derivSigByAllVar ((struct bequad *) ( pt_beaux->NODE));

      pt_beaux = pt_beaux->NEXT;
      }


	/* ###------------------------------------------------------### */
	/*   delayed internal signals					*/
	/* ###------------------------------------------------------### */

    pt_beaux = pt_befig->BEDLY;
    while (pt_beaux != NULL)
      {
      ((struct bequad *) (pt_beaux->NODE))->DERIVATE =
        beh_derivSigByAllVar ((struct bequad *) (pt_beaux->NODE));

      pt_beaux = pt_beaux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   assertions							*/
	/* ###------------------------------------------------------### */

    pt_bemsg = pt_befig->BEMSG;
    while (pt_bemsg != NULL)
      {
      ((struct bequad *) (pt_bemsg->NODE))->DERIVATE =
        beh_derivSigByAllVar ((struct bequad *) (pt_bemsg->NODE));

      pt_bemsg = pt_bemsg->NEXT;
      }

    pt_befig->TYPE=pt_befig->TYPE | BEH_NODE_DERIVATE;
    }

  }
