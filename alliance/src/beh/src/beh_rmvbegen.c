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
/* file		: beh_rmvbegen.c					*/
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
/* function	: beh_rmvbegen						*/
/* description	: delete a BEGEN structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

struct begen *beh_rmvbegen (listbegen, begenname, mode)

struct begen *listbegen;	/* list of begen containing the object	*/
char         *begenname;	/* name of the BEGEN to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct begen  headgen;
  struct begen *ptlastgen;
  struct begen *ptbegen;

  if (listbegen != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headgen.NEXT  = listbegen;
    headgen.NAME  = NULL     ;
    headgen.VALUE = NULL     ;
    ptbegen       = &headgen;
    while ((ptbegen != NULL) && (ptbegen->NAME != begenname))
      {
      ptlastgen = ptbegen;
      ptbegen   = ptbegen->NEXT;
      }

    if (ptbegen != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if (ptbegen->VALUE != NULL)
          (void) fprintf (stderr,"BEH_warning : begen `%s` not empty\n",
                          begenname);
        }
      else
        mbkfree (ptbegen->VALUE);

      ptlastgen->NEXT = ptbegen->NEXT;
      mbkfree (ptbegen);
      }

    listbegen = headgen.NEXT;
    }

  return(listbegen);
  }
