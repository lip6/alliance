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
/* file		: beh_frebefig.c					*/
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
/* function	: beh_frebefig						*/
/* description	: delete a list of BEFIG structures and all objects	*/
/*		  pointed by any os BEFIGs in the list			*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void beh_frebefig (listbefig)

struct befig *listbefig;	/* list of befig to be deleted		*/

  {
  struct befig *ptbefig;
  struct ptype *ptptype;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listbefig != NULL)
    {
    beh_frebereg (listbefig->BEREG);
    beh_frebemsg (listbefig->BEMSG);
    beh_freberin (listbefig->BERIN);
    beh_frebeout (listbefig->BEOUT);
    beh_frebebus (listbefig->BEBUS);
    beh_frebeaux (listbefig->BEAUX);
    beh_frebeaux (listbefig->BEDLY);
    beh_frebebux (listbefig->BEBUX);
    beh_frebepor (listbefig->BEPOR);
    if ((ptptype = getptype (listbefig->USER,BEH_GENERIC)) != NULL)
      beh_frebegen (ptptype->DATA);

    ptbefig   = listbefig;
    listbefig = listbefig->NEXT;
    mbkfree (ptbefig);
    }

  }
