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
/* file		: pat_addpapat.c					*/
/* date		: Mar 13 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_addpapat						*/
/* description	: create an empty PAPAT (pattern) structure at the top	*/
/*		  of a list						*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct papat *pat_addpapat (lastpapat, label, line)

struct papat   *lastpapat;	/* pointer on the last papat structure	*/
char           *label;		/* pattern's label			*/
unsigned short  line;		/* pattern' line number			*/

  {
  struct papat *ptpat;

  label          = namealloc (label);

  ptpat          = (struct papat *) mbkalloc (sizeof(struct papat));
  ptpat->LABEL   = label    ;
  ptpat->PAEVT   = NULL     ;
  ptpat->PAINI   = NULL     ;
  ptpat->TIME    =  0       ;
  ptpat->LINE    = line     ;
  ptpat->BLANK   =  0       ;
  ptpat->ACTFLAG = 'U'      ;
  ptpat->SIMFLAG = 'U'      ;
  ptpat->FLAG    =  0       ;
  ptpat->NEXT    = lastpapat;

  return (ptpat);
  }
