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
/* file		: beh_addbeaux.c					*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_addbeaux						*/
/* description	: create a BEAUX structure at the top the list		*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct beaux *beh_addbeaux (lastbeaux, name, abl_expr, bdd_expr)

struct beaux *lastbeaux;	/* pointer on the last beaux structure	*/
char         *name     ;	/* simple internal signal's name	*/
struct chain *abl_expr ;	/* simple internal signal's expr. (ABL)	*/
struct node  *bdd_expr ;	/* simple internal signal's expr. (BDD)	*/

  {
  struct beaux *ptaux;

  name        = namealloc (name);

  ptaux       = (struct beaux *) mbkalloc (sizeof(struct beaux));
  ptaux->NAME = name            ;
  ptaux->TIME = 0               ;
  ptaux->FLAG = BEH_ASG_INERTIAL;
  ptaux->ABL  = abl_expr        ;
  ptaux->NODE = bdd_expr        ;
  ptaux->NEXT = lastbeaux       ;

  return (ptaux);
  }
