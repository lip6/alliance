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
/* file		: beh_depend.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

	/* ###------------------------------------------------------### */
	/*    A hash table containing inputs name and their pointers	*/
	/* ###------------------------------------------------------### */

static ht *rin_list = NULL;

/* ###--------------------------------------------------------------### */
/* function	: beh_expdepend						*/
/* description	: find dependencies for an expression			*/
/* called func.	: beh_namelist, gethtitem				*/
/* ###--------------------------------------------------------------### */

static struct chain *beh_expdepend (pt_fig, pt_exp)

struct befig *pt_fig;
struct chain *pt_exp;

  {
  struct chain *res_chain = NULL;
  struct chain *del_chain = NULL;
  struct chain *tmp_chain = NULL;
  struct chain *mem_chain = NULL;
  int           value           ;

  if (pt_fig->ERRFLG == 0)
    {
	/* ###------------------------------------------------------### */
	/*   if there is no error in the current figure, find the list	*/
	/* of terminals for the expression				*/
	/* ###------------------------------------------------------### */

    res_chain = beh_namelist (pt_exp);

	/* ###------------------------------------------------------### */
	/*   scan the list of terminals to replace terminal names by	*/
	/* the pointer on the corresponding primary input (berin).	*/
	/*   check the unicity of terminals ...				*/
	/* ###------------------------------------------------------### */

    tmp_chain = res_chain;

    while (tmp_chain != NULL)
      {
      del_chain = tmp_chain->NEXT;
      while (del_chain != NULL)
        {
        mem_chain = del_chain->NEXT;

        if (del_chain->DATA == tmp_chain->DATA)
          tmp_chain = delchain (tmp_chain, del_chain);

        del_chain = mem_chain;
        }

      value = gethtitem (rin_list, tmp_chain->DATA);

      if (value == EMPTYHT)
      {
        pt_fig->ERRFLG = beh_error (3, tmp_chain->DATA);
        exit( pt_fig->ERRFLG );
      }
      else
        tmp_chain->DATA = (void *) value;

      tmp_chain = tmp_chain->NEXT;
      }
    }

  return (res_chain);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_depend						*/
/* description	: find dependencies between signals for a behavioural	*/
/*		  description.						*/
/* called func.	: beh_expdepend, addht    , addhtitem, addchain,	*/
/*		  freechain    , append   , delhtitem 			*/
/* ###--------------------------------------------------------------### */

void beh_depend (pt_fig)

struct befig *pt_fig;

  {
  struct chain *res_chain = NULL;
  struct beaux *pt_aux    ;
  struct beaux *pt_dly    ;
  struct bebux *pt_bux    ;
  struct bereg *pt_reg    ;
  struct bemsg *pt_msg    ;
  struct berin *pt_rin    ;
  struct beout *pt_out    ;
  struct bebus *pt_bus    ;
  struct biabl *pt_biabl  ;
  unsigned int  count     = 0;

	/* ###------------------------------------------------------### */
	/*   if there is no error in the current figure ...		*/
	/* ###------------------------------------------------------### */

  if ((pt_fig != NULL) && (pt_fig->ERRFLG == 0))
    {
	/* ###------------------------------------------------------### */
	/*   initialize a hash table with inputs			*/
	/* ###------------------------------------------------------### */

    pt_rin = pt_fig->BERIN;
    while (pt_rin != NULL)
      {
      count++;
      pt_rin = pt_rin->NEXT;
      }

    pt_rin = pt_fig->BERIN;

    if (pt_rin != NULL)
      rin_list = addht (count);

    while (pt_rin != NULL)
      {
      addhtitem (rin_list, pt_rin->NAME, (int) pt_rin);
      pt_rin = pt_rin->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   process simple internal signals.				*/
	/*   check that the signal does not already belong to the	*/
	/* dependency list before adding it to the list			*/
	/* ###------------------------------------------------------### */

    pt_aux = pt_fig->BEAUX;
    while (pt_aux != NULL)
      {
      res_chain = beh_expdepend (pt_fig, pt_aux->ABL);
      while (res_chain != NULL)
        {
        pt_rin = (struct berin *) res_chain->DATA;

        if ((pt_rin->AUX_REF == NULL) || (pt_rin->AUX_REF->DATA != pt_aux))
          pt_rin->AUX_REF = addchain (pt_rin->AUX_REF, pt_aux);

        res_chain = res_chain->NEXT;
        }
      freechain (res_chain);
      pt_aux = pt_aux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   process delayed internal signals				*/
	/*     check that the signal does not already belong to the	*/
	/*     dependency list before adding it to the list		*/
	/* ###------------------------------------------------------### */

    pt_dly = pt_fig->BEDLY;
    while (pt_dly != NULL)
      {
      res_chain = beh_expdepend (pt_fig, pt_dly->ABL);
      while (res_chain != NULL)
        {
        pt_rin = (struct berin *) res_chain->DATA;

        if ((pt_rin->DLY_REF == NULL) || (pt_rin->DLY_REF->DATA != pt_dly))
          pt_rin->DLY_REF = addchain (pt_rin->DLY_REF, pt_dly);

        res_chain = res_chain->NEXT;
        }
      freechain (res_chain);
      pt_dly = pt_dly->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   process simple ouputs					*/
	/*     check that the signal does not already belong to the	*/
	/*     dependency list before adding it to the list		*/
	/* ###------------------------------------------------------### */

    pt_out = pt_fig->BEOUT;
    while (pt_out != NULL)
      {
      res_chain = beh_expdepend (pt_fig, pt_out->ABL);
      while (res_chain != NULL)
        {
        pt_rin = (struct berin *) res_chain->DATA;

        if ((pt_rin->OUT_REF == NULL) || (pt_rin->OUT_REF->DATA != pt_out))
          pt_rin->OUT_REF = addchain (pt_rin->OUT_REF, pt_out);

        res_chain = res_chain->NEXT;
        }
      freechain (res_chain);
      pt_out = pt_out->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   process bussed ouputs.					*/
	/*   check that the signal does not already belong to the	*/
	/* dependency list before adding it to the list			*/
	/* ###------------------------------------------------------### */

    res_chain = NULL;
    pt_bus    = pt_fig->BEBUS;
    while (pt_bus != NULL)
      {
      pt_biabl = pt_bus->BIABL;
      while (pt_biabl != NULL)
        {
        res_chain = append (res_chain, beh_expdepend (pt_fig,pt_biabl->VALABL));
        res_chain = append (res_chain, beh_expdepend (pt_fig,pt_biabl->CNDABL));
        pt_biabl  = pt_biabl->NEXT;
        }

      while (res_chain != NULL)
        {
        pt_rin = (struct berin *) res_chain->DATA;

        if ((pt_rin->BUS_REF == NULL) || (pt_rin->BUS_REF->DATA != pt_bus))
          pt_rin->BUS_REF = addchain (pt_rin->BUS_REF, pt_bus);

        res_chain = res_chain->NEXT;
        }
      freechain (res_chain);
      pt_bus = pt_bus->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   process bussed internal signals.				*/
	/*   check that the signal does not already belong to the	*/
	/* dependency list before adding it to the list			*/
	/* ###------------------------------------------------------### */

    res_chain = NULL;
    pt_bux    = pt_fig->BEBUX;
    while (pt_bux != NULL)
      {
      pt_biabl = pt_bux->BIABL;
      while (pt_biabl != NULL)
        {
        res_chain = append (res_chain, beh_expdepend (pt_fig,pt_biabl->VALABL));
        res_chain = append (res_chain, beh_expdepend (pt_fig,pt_biabl->CNDABL));
        pt_biabl  = pt_biabl->NEXT;
        }

      while (res_chain != NULL)
        {
        pt_rin = (struct berin *) res_chain->DATA;

        if ((pt_rin->BUX_REF == NULL) || (pt_rin->BUX_REF->DATA != pt_bux))
          pt_rin->BUX_REF = addchain (pt_rin->BUX_REF, pt_bux);

        res_chain = res_chain->NEXT;
        }
      freechain (res_chain);
      pt_bux = pt_bux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   process internal registers.				*/
	/*   check that the signal does not already belong to the	*/
	/* dependency list before adding it to the list			*/
	/* ###------------------------------------------------------### */

    res_chain = NULL;
    pt_reg    = pt_fig->BEREG;
    while (pt_reg != NULL)
      {
      pt_biabl = pt_reg->BIABL;
      while (pt_biabl != NULL)
        {
        res_chain = append (res_chain, beh_expdepend (pt_fig,pt_biabl->VALABL));
        res_chain = append (res_chain, beh_expdepend (pt_fig,pt_biabl->CNDABL));
        pt_biabl  = pt_biabl->NEXT;
        }

      while (res_chain != NULL)
        {
        pt_rin = (struct berin *) res_chain->DATA;

        if ((pt_rin->REG_REF == NULL) || (pt_rin->REG_REF->DATA != pt_reg))
          pt_rin->REG_REF = addchain (pt_rin->REG_REF, pt_reg);

        res_chain = res_chain->NEXT;
        }
      freechain (res_chain);
      pt_reg = pt_reg->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   process assert statements.					*/
	/*   check that the statement does not already belong to the	*/
	/* dependency list before adding it to the list			*/
	/* ###------------------------------------------------------### */

    res_chain = NULL;
    pt_msg    = pt_fig->BEMSG;
    while (pt_msg != NULL)
      {
      res_chain = beh_expdepend (pt_fig, pt_msg->ABL);
      while (res_chain != NULL)
        {
        pt_rin = (struct berin *) res_chain->DATA;

        if ((pt_rin->MSG_REF == NULL) || (pt_rin->MSG_REF->DATA != pt_msg))
          pt_rin->MSG_REF = addchain (pt_rin->MSG_REF, pt_msg);

        res_chain = res_chain->NEXT;
        }
      freechain (res_chain);
      pt_msg = pt_msg->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   release hash tables and lists that they contain		*/
	/* ###------------------------------------------------------### */

    if (pt_fig->BERIN != NULL)
      {
      delht (rin_list);
      rin_list = NULL;
      }

    pt_fig->FLAG |= BEH_FIG_DEPEND;
    }
  }
