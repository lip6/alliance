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
/* file		: beh_rmvbebus.c					*/
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
/* function	: beh_rmvbebus						*/
/* description	: delete a BEBUS structure and return the pointer of	*/
/*		  the next object. A warning is printed out if the	*/
/*		  object to be deleted is not empty when the mode is N	*/
/* called func.	: mbkfree, beh_frebiabl, beh_frebinode			*/
/* ###--------------------------------------------------------------### */

struct bebus *beh_rmvbebus (listbebus, bebusname, mode)

struct bebus *listbebus;	/* list of bebus containing the object	*/
char         *bebusname;	/* name of the BEBUS to be deleted	*/
char          mode;		/* recursive delete or not (Y or N)	*/

  {
  struct bebus  headbus;
  struct bebus *ptlastbus;
  struct bebus *ptbebus;

  if (listbebus != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Search the object to be deleted				*/
	/* ###------------------------------------------------------### */

    headbus.NEXT   = listbebus;
    headbus.NAME   = NULL     ;
    headbus.BIABL  = NULL     ;
    headbus.BINODE = NULL     ;
    ptbebus        = &headbus;
    while ((ptbebus != NULL) && (ptbebus->NAME != bebusname))
      {
      ptlastbus = ptbebus;
      ptbebus   = ptbebus->NEXT;
      }

    if (ptbebus != NULL)
      {

	/* ###------------------------------------------------------### */
	/*    If the object doesn't exist return the list without	*/
	/* modification.						*/
	/*    If the object has been found check the mode and, if asked	*/
	/* delete pointed objects recursively.				*/
	/* ###------------------------------------------------------### */

      if (mode == 'N')
        {
        if ((ptbebus->BIABL != NULL) || (ptbebus->BINODE != NULL))
          (void) fprintf (stderr,"BEH_warning : bebus `%s` not empty\n",
                          bebusname);
        }
      else
        {
        beh_frebiabl  (ptbebus->BIABL);
        beh_frebinode (ptbebus->BINODE);
        }

      ptlastbus->NEXT = ptbebus->NEXT;
      mbkfree (ptbebus);
      }

    listbebus = headbus.NEXT;
    }

  return(listbebus);
  }
