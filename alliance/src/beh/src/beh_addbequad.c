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
/* file		: beh_addbequad.c					*/
/* date		: Mar  17 1995						*/
/* version	: v000							*/
/* authors	: Laurent VUILLEMIN					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_addbequad						*/
/* description	: create a BEQUAD structure at the top of the list	*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct bequad *beh_addbequad ( bdd_expr, gex_expr, var_list, der_list)

struct node *bdd_expr;
struct begex *gex_expr;
struct chain *var_list;
struct beder *der_list;

  {
  struct bequad *ptquad;


  ptquad           = (struct bequad *) mbkalloc (sizeof(struct bequad));
  ptquad->BDD      = bdd_expr;
  ptquad->GEX      = gex_expr;
  ptquad->VARLIST  = var_list;
  ptquad->DERIVATE = der_list;
  ptquad->USER     = NULL;
  return (ptquad);
  }
