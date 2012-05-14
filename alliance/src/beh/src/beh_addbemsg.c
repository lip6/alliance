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
/* file		: beh_addbemsg.c					*/
/* date		: Nov  6 1995						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_addbemsg						*/
/* description	: create BEMSG structure at the top the list		*/
/* called func.	: mbkalloc, namealloc					*/
/* ###--------------------------------------------------------------### */

struct bemsg *beh_addbemsg (lastbemsg,label,level,message,abl_expr,bdd_expr)

struct bemsg *lastbemsg;	/* pointer on the last bemsg structure	*/
char         *label;		/* assert's label			*/
char          level;		/* severity level (E or W)		*/
char         *message;		/* reported message			*/
struct chain *abl_expr;		/* assert's condition (ABL)		*/
struct node  *bdd_expr;		/* assert's condition (BDD)		*/

  {

  struct bemsg *ptmsg;

  label          = namealloc (label);

  ptmsg          = (struct bemsg *) mbkalloc (sizeof(struct bemsg));
  ptmsg->LABEL   = label;
  ptmsg->LEVEL   = level;
  ptmsg->MESSAGE = message;
  ptmsg->ABL     = abl_expr;
  ptmsg->NODE    = bdd_expr;
  ptmsg->NEXT    = lastbemsg;

  return (ptmsg);
  }
