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
/* file		: beh_addbiabl.c					*/
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
/* function	: beh_addbiabl						*/
/* description	: create a BIABL structure at the top the list		*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct biabl *beh_addbiabl (lastbiabl, label, condition, value)

struct biabl *lastbiabl;	/* pointer on the last biabl structure	*/
char         *label    ;	/* block's label			*/
struct chain *condition;	/* guard expression (ABL)		*/
struct chain *value    ;	/* value expression (ABL)		*/

  {
  struct biabl  *ptbiabl;

  ptbiabl         = (struct biabl *) mbkalloc (sizeof(struct biabl));

  ptbiabl->LABEL  = label           ;
  ptbiabl->CNDABL = condition       ;
  ptbiabl->VALABL = value           ;
  ptbiabl->TIME   = 0               ;
  ptbiabl->FLAG   = BEH_ASG_INERTIAL;
  ptbiabl->USER   = NULL            ;
  ptbiabl->NEXT   = lastbiabl       ;

  return (ptbiabl);
  }
