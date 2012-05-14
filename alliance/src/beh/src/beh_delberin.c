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
/* file		: beh_delberin.c					*/
/* date		: Sep 20 1994						*/
/* version	: v107							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_delberin						*/
/* description	: delete a BERIN structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct berin *beh_delberin (listberin, ptberin)

struct berin *listberin;	/* list of berin containing the object	*/
struct berin *ptberin;		/* pointer of the BERIN to be deleted	*/

  {
  struct berin  headrin;
  struct berin *ptlastrin;

  if ((listberin != NULL) && (ptberin != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headrin.NEXT = listberin;
    ptlastrin    = &headrin;
    while ((ptlastrin != NULL) && (ptlastrin->NEXT != ptberin))
      ptlastrin = ptlastrin->NEXT;

    if (ptlastrin != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      ptlastrin->NEXT = ptberin->NEXT;
      mbkfree (ptberin);
      }

    listberin = headrin.NEXT;
    }

  return(listberin);
  }
