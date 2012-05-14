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
/* file		: beh_delbeout.c					*/
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
/* function	: beh_delbeout						*/
/* description	: delete a BEOUT structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree, freeExpr					*/
/* ###--------------------------------------------------------------### */

struct beout *beh_delbeout (listbeout, ptbeout, mode)

struct beout *listbeout;	/* list of beout containing the object	*/
struct beout *ptbeout;		/* pointer of the BEOUT to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct beout  headout;
  struct beout *ptlastout;

  if ((listbeout != NULL) && (ptbeout != NULL))
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headout.NEXT = listbeout;
    ptlastout    = &headout;
    while ((ptlastout != NULL) && (ptlastout->NEXT != ptbeout))
      ptlastout = ptlastout->NEXT;

    if (ptlastout != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptbeout->ABL != NULL)
          (void) fprintf (stderr,"BEH_warning : beout `%s` not empty\n",
                          ptbeout->NAME);
        }
      else
        freeExpr (ptbeout->ABL);

      ptlastout->NEXT = ptbeout->NEXT;
      mbkfree (ptbeout);
      }

    listbeout = headout.NEXT;
    }

  return(listbeout);
  }
