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
/* file		: beh_frebinod.c					*/
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
/* function	: beh_frebinode						*/
/* description	: delete a list of BINODE structures and all objects	*/
/*		  pointed by any os BINODEs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void beh_frebinode (listbinode)

struct binode *listbinode;	/* list of binode to be deleted		*/

  {
  struct binode *ptbinode;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbinode != NULL)
    {
    ptbinode   = listbinode;
    listbinode = listbinode->NEXT;
    mbkfree (ptbinode);
    }

  }
