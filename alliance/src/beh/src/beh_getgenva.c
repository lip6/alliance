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
/* file		: beh_getgenva.c					*/
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
/* function	: beh_getgenval						*/
/* description	: Search an find a BEGEN structure (known from its name)*/
/*		  then return its value field. If the BEGEN is not found*/
/*		  a NULL pointer is returned				*/
/* called func.	:							*/
/* ###--------------------------------------------------------------### */

void *beh_getgenval (listbegen, begenname)

struct begen *listbegen;	/* head of BEGEN list			*/
char         *begenname;	/* name of the structure 		*/

  {
  struct begen *ptbegen;
  void         *value = NULL;

	/* ###------------------------------------------------------### */
	/*    searching the object					*/
	/* ###------------------------------------------------------### */

  ptbegen = listbegen;
  while ((ptbegen != NULL) && (ptbegen->NAME != begenname))
    ptbegen = ptbegen->NEXT;

	/* ###------------------------------------------------------### */
	/*    if found return its value field				*/
	/* ###------------------------------------------------------### */

  if (ptbegen != NULL)
    value = ptbegen->VALUE;

  return (value);
  }
