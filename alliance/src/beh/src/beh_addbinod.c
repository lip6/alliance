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
/* file		: beh_addbinod.c					*/
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
/* function	: beh_addbinode						*/
/* description	: create a BINODE structure at the top the list		*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct binode *beh_addbinode (lastbinode, condition, value)

struct binode *lastbinode;	/* pointer on the last binode structure	*/
struct node   *condition ;	/* guard expression (BDD)		*/
struct node   *value     ;	/* value expression (BDD)		*/

  {
  struct binode *ptbinode;

  ptbinode          = (struct binode *) mbkalloc (sizeof(struct binode));
  ptbinode->TIME    = 0               ;
  ptbinode->FLAG    = BEH_ASG_INERTIAL;
  ptbinode->CNDNODE = condition       ;
  ptbinode->VALNODE = value           ;
  ptbinode->NEXT    = lastbinode      ;

  return (ptbinode);
  }
