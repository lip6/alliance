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
/* file		: beh_rmvberin.c					*/
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
/* function	: beh_rmvberin						*/
/* description	: delete a BERIN structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct berin *beh_rmvberin (listberin, berinname)

struct berin *listberin;	/* list of berin containing the object	*/
char         *berinname;	/* name of the BERIN to be deleted	*/

  {
  struct berin  headrin;
  struct berin *ptlastrin;
  struct berin *ptberin;

  if (listberin != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headrin.NEXT    = listberin;
    headrin.NAME    = NULL;
    headrin.OUT_REF = NULL;
    headrin.MSG_REF = NULL;
    headrin.AUX_REF = NULL;
    headrin.BUX_REF = NULL;
    headrin.BUS_REF = NULL;
    headrin.REG_REF = NULL;
    headrin.DLY_REF = NULL;
    ptberin      = &headrin;
    while ((ptberin != NULL) && (ptberin->NAME != berinname))
      {
      ptlastrin = ptberin;
      ptberin   = ptberin->NEXT;
      }

    if (ptberin != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      freechain (ptberin->OUT_REF);
      freechain (ptberin->MSG_REF);
      freechain (ptberin->AUX_REF);
      freechain (ptberin->BUX_REF);
      freechain (ptberin->BUS_REF);
      freechain (ptberin->REG_REF);
      freechain (ptberin->DLY_REF);

      ptlastrin->NEXT = ptberin->NEXT;
      mbkfree (ptberin);
      }

    listberin = headrin.NEXT;
    }

  return(listberin);
  }
