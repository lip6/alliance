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
/* file		: beh_rplstable.c					*/
/* date		: Dec  2 1999						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_rplstable						*/
/* description	: replace STABLE attribute by DELAYED in an expression	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

struct chain *beh_rplstable (pt_abl)

struct chain *pt_abl;

  {
  struct chain *opr_lst;
  char         *sig_nam;
  struct chain *new_abl;
  unsigned int  opera  ;

  if (pt_abl->NEXT != NULL)
    {
    opera = (unsigned int) ((struct chain *) pt_abl->DATA)->DATA;
    if (opera == STABLE)
      {
      sig_nam = (char *) ((struct chain *) pt_abl->NEXT->DATA)->DATA;
      new_abl = beh_expstable (sig_nam);
      freeExpr (pt_abl);
      pt_abl  = new_abl;
      }
    else
      {
      opr_lst = pt_abl->NEXT;
      while (opr_lst != NULL)
        {
        opr_lst->DATA = beh_rplstable (opr_lst->DATA);
        opr_lst       = opr_lst->NEXT;
        }
      }
    }

  return (pt_abl);
  }
