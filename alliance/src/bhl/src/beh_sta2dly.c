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
/* file		: beh_sta2dly.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: high-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_sta2dly						*/
/* description	: replace the STABLE attribute by DELAYED in the	*/
/*		  expressions of the description			*/
/* called func.	: beh_rplstable						*/
/* ###--------------------------------------------------------------### */

void beh_sta2dly (pt_fig, mode)

struct befig *pt_fig;
unsigned int  mode  ;

  {
  struct beout *pt_out  ;
  struct bebus *pt_bus  ;
  struct beaux *pt_aux  ;
  struct bebux *pt_bux  ;
  struct bereg *pt_reg  ;
  struct bemsg *pt_msg  ;
  struct biabl *pt_biabl;

  if ((pt_fig != NULL) && (pt_fig->ERRFLG == 0))
    {
    if ((mode & BEH_STA_REPLACE_OUT) != 0)
      {
      pt_out = pt_fig->BEOUT;
      while (pt_out != NULL)
        {
        pt_out->ABL = beh_rplstable (pt_out->ABL);
        pt_out      = pt_out->NEXT;
        }
      }

    if ((mode & BEH_STA_REPLACE_AUX) != 0)
      {
      pt_aux = pt_fig->BEAUX;
      while (pt_aux != NULL)
        {
        pt_aux->ABL = beh_rplstable (pt_aux->ABL);
        pt_aux      = pt_aux->NEXT;
        }
      }

    if ((mode & BEH_STA_REPLACE_MSG) != 0)
      {
      pt_msg = pt_fig->BEMSG;
      while (pt_msg != NULL)
        {
        pt_msg->ABL = beh_rplstable (pt_msg->ABL);
        pt_msg      = pt_msg->NEXT;
        }
      }

    if ((mode & BEH_STA_REPLACE_BUS) != 0)
      {
      pt_bus = pt_fig->BEBUS;
      while (pt_bus != NULL)
        {
        pt_biabl = pt_bus->BIABL;
        while (pt_biabl != NULL)
          {
          pt_biabl->VALABL = beh_rplstable (pt_biabl->VALABL);
          pt_biabl->CNDABL = beh_rplstable (pt_biabl->CNDABL);
          pt_biabl         = pt_biabl->NEXT;
          }
        pt_bus = pt_bus->NEXT;
        }
      }

    if ((mode & BEH_STA_REPLACE_BUX) != 0)
      {
      pt_bux = pt_fig->BEBUX;
      while (pt_bux != NULL)
        {
        pt_biabl = pt_bux->BIABL;
        while (pt_biabl != NULL)
          {
          pt_biabl->VALABL = beh_rplstable (pt_biabl->VALABL);
          pt_biabl->CNDABL = beh_rplstable (pt_biabl->CNDABL);
          pt_biabl         = pt_biabl->NEXT;
          }
        pt_bux = pt_bux->NEXT;
        }
      }

    if ((mode & BEH_STA_REPLACE_REG) != 0)
      {
      pt_reg = pt_fig->BEREG;
      while (pt_reg != NULL)
        {
        pt_biabl = pt_reg->BIABL;
        while (pt_biabl != NULL)
          {
          pt_biabl->VALABL = beh_rplstable (pt_biabl->VALABL);
          pt_biabl->CNDABL = beh_rplstable (pt_biabl->CNDABL);
          pt_biabl         = pt_biabl->NEXT;
          }
        pt_reg = pt_reg->NEXT;
        }
      }
    }
  }
