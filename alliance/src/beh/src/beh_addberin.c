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
/* file		: beh_addberin.c					*/
/* date		: Sep 20 1994						*/
/* version	: v107							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_addberin						*/
/* description	: create a BERIN structure at the top the list		*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct berin *beh_addberin (lastberin, name)

struct berin *lastberin;	/* pointer on the last berin structure	*/
char         *name;		/* signal's name			*/

  {
  struct berin *ptrin;

  name         = namealloc (name);

  ptrin        = (struct berin *) mbkalloc (sizeof(struct berin));
  ptrin->NAME  = name;
  ptrin->NEXT  = lastberin;

  ptrin->REG_REF = NULL;
  ptrin->MSG_REF = NULL;
  ptrin->AUX_REF = NULL;
  ptrin->BUX_REF = NULL;
  ptrin->BUS_REF = NULL;
  ptrin->OUT_REF = NULL;
  ptrin->DLY_REF = NULL;

  return (ptrin);
  }
