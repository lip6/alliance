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
/* file		: beh_addbefig.c					*/
/* date		: Mar  8 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_addbefig						*/
/* description	: create an empty BEFIG structure at the top of the list*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct befig *beh_addbefig (lastbefig, name)

struct befig *lastbefig;	/* pointer on the last befig structure	*/
char         *name     ;	/* figure's name			*/

  {
  struct befig  *ptfig   ;
  unsigned char  err_flg = 0;

  ptfig = (struct befig *) mbkalloc (sizeof(struct befig));

  ptfig->NAME      = namealloc (name);
  ptfig->BEREG     = NULL            ;
  ptfig->BEMSG     = NULL            ;
  ptfig->BERIN     = NULL            ;
  ptfig->BEOUT     = NULL            ;
  ptfig->BEBUS     = NULL            ;
  ptfig->BEAUX     = NULL            ;
  ptfig->BEDLY     = NULL            ;
  ptfig->BEBUX     = NULL            ;
  ptfig->BEPOR     = NULL            ;
  ptfig->CIRCUI    = NULL            ;
  ptfig->USER      = NULL            ;
  ptfig->BEGEN     = NULL            ;
  ptfig->ERRFLG    = err_flg         ;
  ptfig->TYPE      = 0               ;
  ptfig->FLAG      = 0               ;
  ptfig->TIME_UNIT = BEH_TIMEUNIT    ;
  ptfig->TIME_STEP = BEH_TIMESTEP    ;
  ptfig->NEXT      = lastbefig       ;

  return (ptfig);
  }
