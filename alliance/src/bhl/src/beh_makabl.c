/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
/* file		: beh_makabl.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: DUNOYER julien, Pirouz BAZARGAN SABET			*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

static ht           *HashTable  = NULL;

/* ###--------------------------------------------------------------### */
/* function     : beh_dupabl                                            */
/* description  : duplicate recursively an abl 				*/
/* called func. : ATOM,CDR,VALUE_ATOM,addchain                          */
/* ###--------------------------------------------------------------### */
  
struct chain *beh_dupdabl (pt_abl1)

struct chain *pt_abl1;

  {
  struct chain *op    ;
  struct chain *pt_abl;

  if (ATOM(pt_abl1))
    {
    return (createAtom(VALUE_ATOM(pt_abl1)));
    }
  else 
    {
    pt_abl = createExpr(OPER(pt_abl1));

    while (pt_abl1 = CDR(pt_abl1))
      {
      pt_abl->NEXT=addchain(pt_abl->NEXT, (void *)beh_dupabl(CAR(pt_abl1)));
      }
    return pt_abl;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_makabl						*/
/* description	: make abl for each signal of a befig structure 	*/
/* called func.	: beh_dupabl, addht, addhtitem, delht			*/
/* ###--------------------------------------------------------------### */

void beh_makabl (ptr_befig, node_flag)

struct befig *ptr_befig;		/* befig containing expressions	*/
char          node_flag;

  {
  struct beaux  *ptr_beaux;
  struct beaux  *ptr_bedly;
  struct bemsg  *ptr_bemsg;
  struct beout  *ptr_beout;
  struct bebus  *ptr_bebus;
  struct bebux  *ptr_bebux;
  struct bereg  *ptr_bereg;
  struct biabl  *ptr_biabl;
  struct binode *ptr_binode;

  if (!(ptr_befig->TYPE & BEH_NODE_ABL))
    {

	/* ###------------------------------------------------------### */
	/*    make a abl for each simple internal signal		*/
	/* ###------------------------------------------------------### */

    ptr_beaux = ptr_befig->BEAUX;
    while (ptr_beaux != NULL)
      {
      if (ptr_befig->TYPE & BEH_NODE_QUAD)
        ((bequad_list *)(ptr_beaux->NODE))->ABL = beh_dupabl (ptr_beaux->ABL);
      else
        ptr_beaux->NODE= (pNode)beh_dupabl (ptr_beaux->ABL);

      ptr_beaux       = ptr_beaux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a abl for each delayed internal signal		*/
	/* ###------------------------------------------------------### */

    ptr_bedly = ptr_befig->BEDLY;
    while (ptr_bedly != NULL)
      {
      if (ptr_befig->TYPE & BEH_NODE_QUAD)
        ((bequad_list *)(ptr_bedly->NODE))->ABL = beh_dupabl (ptr_bedly->ABL);
      else 
        ptr_bedly->NODE= (pNode)beh_dupabl(ptr_bedly->ABL);

      ptr_bedly       = ptr_bedly->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a abl for each assertion				*/
	/* ###------------------------------------------------------### */

    ptr_bemsg = ptr_befig->BEMSG;
    while (ptr_bemsg != NULL)
      {
      if (ptr_befig->TYPE & BEH_NODE_QUAD)
        ((bequad_list *)(ptr_bemsg->NODE))->ABL = beh_dupabl (ptr_bemsg->ABL);
      else
        ptr_bemsg->NODE=(pNode)beh_dupabl(ptr_bemsg->ABL);
      ptr_bemsg       = ptr_bemsg->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a abl for each simple output				*/
	/* ###------------------------------------------------------### */

    ptr_beout = ptr_befig->BEOUT;
    while (ptr_beout != NULL)
      {
      if (ptr_befig->TYPE & BEH_NODE_QUAD)
        ((bequad_list *)(ptr_beout->NODE))->ABL = beh_dupabl (ptr_beout->ABL);
      else 
        ptr_beout->NODE=(pNode) beh_dupabl (ptr_beout->ABL);

      ptr_beout = ptr_beout->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a abl for each bussed output				*/
	/* ###------------------------------------------------------### */

    ptr_bebus = ptr_befig->BEBUS;
    while (ptr_bebus != NULL)
      {
      ptr_biabl  = ptr_bebus->BIABL;
      ptr_binode = ptr_bebus->BINODE;

      while (ptr_biabl != NULL)
        {
        if (ptr_befig->TYPE & BEH_NODE_QUAD)
	  {
          ((bequad_list *)(ptr_binode->CNDNODE))->ABL =
		        beh_dupabl (ptr_biabl->CNDABL);
          ((bequad_list *)(ptr_binode->VALNODE))->ABL =
		        beh_dupabl (ptr_biabl->VALABL);
          }
        else 
	  {
	  ptr_binode->CNDNODE=(pNode)beh_dupabl(ptr_biabl->CNDABL);
	  ptr_binode->VALNODE=(pNode)beh_dupabl(ptr_biabl->VALABL);
	  }

        ptr_biabl  = ptr_biabl->NEXT;
        ptr_binode = ptr_binode->NEXT;
        }
      ptr_bebus = ptr_bebus->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a abl for each bussed internal signal		*/
	/* ###------------------------------------------------------### */

    ptr_bebux = ptr_befig->BEBUX;
    while (ptr_bebux != NULL)
      {
      ptr_biabl  = ptr_bebux->BIABL;
      ptr_binode = ptr_bebux->BINODE;

      while (ptr_biabl != NULL)
        {
        if (ptr_befig->TYPE & BEH_NODE_QUAD)
	  {
          ((bequad_list *)(ptr_binode->CNDNODE))->ABL  =
		        beh_dupabl(ptr_biabl->CNDABL);
          ((bequad_list *)(ptr_binode->VALNODE))->ABL   =
		        beh_dupabl(ptr_biabl->VALABL);
          }
        else 
	  {
	  ptr_binode->CNDNODE=(pNode)beh_dupabl(ptr_biabl->CNDABL);
	  ptr_binode->VALNODE=(pNode)beh_dupabl(ptr_biabl->VALABL);
	  }

        ptr_biabl  = ptr_biabl->NEXT;
        ptr_binode = ptr_binode->NEXT;
        }
      ptr_bebux = ptr_bebux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    make a abl for each internal register			*/
	/* ###------------------------------------------------------### */

    ptr_bereg = ptr_befig->BEREG;
    while (ptr_bereg != NULL)
      {
      ptr_biabl  = ptr_bereg->BIABL;
      ptr_binode = ptr_bereg->BINODE;

      while (ptr_biabl != NULL)
        {
        if (ptr_befig->TYPE & BEH_NODE_QUAD)
	  {
          ((bequad_list *)(ptr_binode->CNDNODE))->ABL  =
		        beh_dupabl(ptr_biabl->CNDABL);
          ((bequad_list *)(ptr_binode->VALNODE))->ABL   =
		        beh_dupabl(ptr_biabl->VALABL);
          }
        else 
	  {
	  ptr_binode->CNDNODE=(pNode)beh_dupabl(ptr_biabl->CNDABL);
	  ptr_binode->VALNODE=(pNode)beh_dupabl(ptr_biabl->VALABL);
	  }


        ptr_biabl  = ptr_biabl->NEXT;
        ptr_binode = ptr_binode->NEXT;
        }
      ptr_bereg = ptr_bereg->NEXT;
      }

    ptr_befig->TYPE |= BEH_NODE_ABL;
    }
  }
