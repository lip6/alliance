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
/* file		: pat_frepapat.c					*/
/* date		: Jul 16 1993						*/
/* version	: v104							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_frepapat						*/
/* description	: release a list of PAPAT (pattern) structures		*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

void pat_frepapat (listpapat)

struct papat *listpapat;

  {
  struct papat *ptpat;

        /* ###------------------------------------------------------### */
        /*    for each object of the list delete the object and all	*/
	/* referenced objects						*/
        /* ###------------------------------------------------------### */

  while (listpapat != NULL)
    {
    ptpat     = listpapat;

    pat_frepaini (ptpat->PAINI);
    pat_frepaevt (ptpat->PAEVT);

    listpapat = listpapat->NEXT;
    mbkfree (ptpat);
    }
  }
