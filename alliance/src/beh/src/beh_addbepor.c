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
/* file		: beh_addbepor.c					*/
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
/* function	: beh_addbepor						*/
/* description	: create a BEPOR structure at the top the list		*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct bepor *beh_addbepor (lastbepor, name, dir, type)

struct bepor *lastbepor;	/* pointer on the last bepor structure	*/
char         *name;		/* signal's name			*/
char          dir;     		/* signal's mode (I, O, B, Z, or T)	*/
char          type;     	/* signal'type mark (M or W)		*/

  {
  struct bepor *ptbepor;

  name               = namealloc (name);

  ptbepor            = (struct bepor *) mbkalloc (sizeof(struct bepor));
  ptbepor->NAME      = name;
  ptbepor->DIRECTION = dir;
  ptbepor->TYPE      = type;
  ptbepor->NEXT      = lastbepor;

  return (ptbepor);
  }
