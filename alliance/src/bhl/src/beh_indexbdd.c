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
/* file		: beh_indexbdd.c					*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_indexbdd						*/
/* description	: initilize the index table for bdd			*/
/* called func.	: initializeCct, addInputCct				*/
/* ###--------------------------------------------------------------### */

void beh_indexbdd (pt_fig)

struct befig *pt_fig;

  {
  struct berin *pt_rin  = NULL;
  struct beout *pt_out  = NULL;
  struct bebus *pt_bus  = NULL;
  struct beaux *pt_aux  = NULL;
  struct bebux *pt_bux  = NULL;
  struct bereg *pt_reg  = NULL;
  struct bemsg *pt_msg  = NULL;
  struct biabl *pt_bia  = NULL;
  unsigned int  cnt_in  = 0   ;
  unsigned int  cnt_out = 0   ;

  if ((pt_fig != NULL) && (pt_fig->CIRCUI == NULL))
    {
      /* ###--------------------------------------------------------### */
      /*    count inputs						*/
      /* ###--------------------------------------------------------### */

    pt_rin = pt_fig->BERIN;
    while (pt_rin != NULL)
      {
      cnt_in ++;
      pt_rin = pt_rin->NEXT;
      }

      /* ###--------------------------------------------------------### */
      /*    count outputs						*/
      /* ###--------------------------------------------------------### */

    pt_out = pt_fig->BEOUT;
    while (pt_out != NULL)
      {
      cnt_out ++;
      pt_out = pt_out->NEXT;
      }

    pt_bus = pt_fig->BEBUS;
    while (pt_bus != NULL)
      {
      pt_bia = pt_bus->BIABL;
      while (pt_bia != NULL)
        {
        cnt_out += 2;
        pt_bia   = pt_bia->NEXT;
        }
      pt_bus = pt_bus->NEXT;
      }

    pt_aux = pt_fig->BEAUX;
    while (pt_aux != NULL)
      {
      cnt_out ++;
      pt_aux = pt_aux->NEXT;
      }

    pt_aux = pt_fig->BEDLY;
    while (pt_aux != NULL)
      {
      cnt_out ++;
      pt_aux = pt_aux->NEXT;
      }

    pt_bux = pt_fig->BEBUX;
    while (pt_bux != NULL)
      {
      pt_bia = pt_bux->BIABL;
      while (pt_bia != NULL)
        {
        cnt_out += 2;
        pt_bia   = pt_bia->NEXT;
        }
      pt_bux = pt_bux->NEXT;
      }

    pt_reg = pt_fig->BEREG;
    while (pt_reg != NULL)
      {
      pt_bia = pt_reg->BIABL;
      while (pt_bia != NULL)
        {
        cnt_out += 2;
        pt_bia   = pt_bia->NEXT;
        }
      pt_reg = pt_reg->NEXT;
      }

    pt_msg = pt_fig->BEMSG;
    while (pt_msg != NULL)
      {
      cnt_out ++;
      pt_msg = pt_msg->NEXT;
      }

      /* ###--------------------------------------------------------### */
      /*    initialization						*/
      /* ###--------------------------------------------------------### */

    pt_fig->CIRCUI = initializeCct (pt_fig->NAME, cnt_in*2, cnt_out);

      /* ###--------------------------------------------------------### */
      /*    define an index for each primary signal			*/
      /* ###--------------------------------------------------------### */

    pt_rin = pt_fig->BERIN;
    while (pt_rin != NULL)
      {
      addInputCct (pt_fig->CIRCUI, pt_rin->NAME);
      pt_rin = pt_rin->NEXT;
      }
    }
  }
