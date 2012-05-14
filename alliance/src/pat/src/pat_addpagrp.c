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
/* file		: pat_addpagrp.c					*/
/* date		: Jul 16 1993						*/
/* version	: v104							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_addpagrp						*/
/* description	: create a PAGRP (array of input-outputs) structure at	*/
/*		  the top of a list					*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct pagrp *pat_addpagrp (lastpagrp, name, length, index, flag)

struct pagrp   *lastpagrp;	/* pointer on the last pagrp structure	*/
char           *name;		/* array's name				*/
unsigned short  length;		/* array's number of bit		*/
unsigned short  index;		/* index of the first paiol of the array*/
short           flag;		/* actual or virtual (0,1) array	*/

  {
  struct pagrp *ptgrp;

  name          = namealloc (name);

  ptgrp         = (struct pagrp *) mbkalloc (sizeof(struct pagrp));
  ptgrp->NAME   = name;
  ptgrp->LENGTH = length;
  ptgrp->FINDEX = index;
  ptgrp->FLAG   = flag;
  ptgrp->NEXT   = lastpagrp;

  return (ptgrp);
  }
