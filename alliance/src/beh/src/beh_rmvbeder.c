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
/* file		: beh_rmvbeder.c					*/
/* date		: Mar 22 1995						*/
/* version	: v100							*/
/* authors	: Laurent VUILLEMIN					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_rmvbeder						*/
/* description	: delete a BERIN structure and return the pointer of	*/
/*		  the next object.					*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct beder *beh_rmvbeder (listbeder, bederindex)

struct beder *listbeder;	/* list of beder containing the object	*/
short         bederindex;	/* index of the BEDER to be deleted	*/

  {
  struct beder  headder;
  struct beder *ptlastder;
  struct beder *ptbeder;

  if (listbeder != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headder.NEXT    = listbeder;
    headder.INDEX=0;
    ptbeder      = &headder;
    while ((ptbeder != NULL) && (ptbeder->INDEX	 != bederindex))
      {
      ptlastder = ptbeder;
      ptbeder   = ptbeder->NEXT;
      }

    if (ptbeder != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/* ###------------------------------------------------------### */


      ptlastder->NEXT = ptbeder->NEXT;
      mbkfree (ptbeder);
      }

    listbeder = headder.NEXT;
    }

  return(listbeder);
  }
