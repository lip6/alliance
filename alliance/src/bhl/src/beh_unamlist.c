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
/* file		: beh_unamlist.c					*/
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

/* ###--------------------------------------------------------------### */
/* function	: beh_unamlist						*/
/* description	: find the list of terminals' name for an expression.	*/
/*		  The value returned by the function is the list of	*/
/*		  terminals. Each terminal is represented by its name	*/
/*		  in the list. A given terminal may only appear once	*/
/*		  in the list. Constante values are not added		*/
/*		  to the list.						*/
/* called func.	: beh_namelist, delchain				*/
/* ###--------------------------------------------------------------### */

struct chain *beh_unamlist (pt_exp)

struct chain *pt_exp;

  {
  struct chain *res_chain;
  struct chain *tmp_chain;
  struct chain *del_chain;
  struct chain *prv_chain;

	/* ###------------------------------------------------------### */
	/*   find the list of terminals					*/
	/* ###------------------------------------------------------### */

  res_chain = beh_namelist (pt_exp);

	/* ###------------------------------------------------------### */
	/*   delete multiple refernces to the same terminal		*/
	/* ###------------------------------------------------------### */

  tmp_chain = res_chain;

  while (tmp_chain != NULL)
    {
    prv_chain = tmp_chain      ;
    del_chain = tmp_chain->NEXT;

    while (del_chain != NULL)
      {
      if (del_chain->DATA == tmp_chain->DATA)
        {
        del_chain       = delchain (del_chain, del_chain);
        prv_chain->NEXT = del_chain;
        }
      else
        {
        prv_chain = del_chain      ;
        del_chain = del_chain->NEXT;
        }
      }

    tmp_chain = tmp_chain->NEXT;
    }

  return (res_chain);
  }
