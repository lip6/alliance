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
/* file		: beh_rmvbepor.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbepor						*/
/* description	: delete a BEPOR structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct bepor *beh_rmvbepor (listbepor, beporname)

struct bepor *listbepor;	/* list of bepor containing the object	*/
char         *beporname;	/* name of the BEPOR to be deleted	*/

  {
  struct bepor  headpor;
  struct bepor *ptlastpor;
  struct bepor *ptbepor;

  if (listbepor != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headpor.NEXT = listbepor;
    headpor.NAME = NULL     ;
    ptbepor      = &headpor;
    while ((ptbepor != NULL) && (ptbepor->NAME != beporname))
      {
      ptlastpor = ptbepor;
      ptbepor   = ptbepor->NEXT;
      }

    if (ptbepor != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */

      ptlastpor->NEXT = ptbepor->NEXT;
      mbkfree (ptbepor);
      }

    listbepor = headpor.NEXT;
    }

  return(listbepor);
  }
