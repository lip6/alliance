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
/* file		: beh_addbegen.c					*/
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
/* function	: beh_addbegen						*/
/* description	: create a BEGEN structure at the top the list		*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct begen *beh_addbegen (lastbegen, name, type, value)

struct begen *lastbegen;	/* pointer on the last begen structure	*/
char         *name;		/* generic's name			*/
char         *type;		/* generic's type mark			*/
void         *value;		/* generic's value			*/

  {

  struct begen *ptgen;

  name         = namealloc (name);
  type         = namealloc (type);
  
  ptgen        = (struct begen *) mbkalloc (sizeof(struct begen));
  ptgen->NAME  = name;
  ptgen->TYPE  = type;
  ptgen->VALUE = value;
  ptgen->NEXT  = lastbegen;

  return (ptgen);
  }
