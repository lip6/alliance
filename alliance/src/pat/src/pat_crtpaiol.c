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
/* file		: pat_crtpaiol.c					*/
/* date		: Sep 30 1996						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_crtpaiol						*/
/* description	: create a table of PAIOL (input-output) structures	*/
/*		  organized in a simply chained list. Then, copy the	*/
/*		  content of the old paiol list in the new table	*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct paiol *pat_crtpaiol (paiollist)

struct paiol *paiollist;	/* list of PAIOLs to be put in a table	*/

  {
  struct paiol *ptiol = NULL;
  struct paiol *lcl_ptiol;
  int           size  = 0;

  lcl_ptiol = paiollist;
  while (lcl_ptiol != NULL)
    {
    size++;
    lcl_ptiol = lcl_ptiol->NEXT;
    }

  if (size != 0)
    {
    ptiol     = (struct paiol *) mbkalloc (size * sizeof(struct paiol));
    lcl_ptiol = ptiol;

    while (paiollist != NULL)
      {
      lcl_ptiol->NAME   = paiollist->NAME;
      lcl_ptiol->FORMAT = paiollist->FORMAT;
      lcl_ptiol->MODE   = paiollist->MODE;
      lcl_ptiol->BLANK  = paiollist->BLANK;
      lcl_ptiol->VALUE  = paiollist->VALUE;
      lcl_ptiol->SIG    = paiollist->SIG;
      lcl_ptiol->FLAG   = paiollist->FLAG;
      lcl_ptiol->NEXT   = lcl_ptiol + 1;

      paiollist         = paiollist->NEXT;
      lcl_ptiol++;
      }
    (lcl_ptiol - 1)->NEXT = NULL;

    }
  return (ptiol);
  }
