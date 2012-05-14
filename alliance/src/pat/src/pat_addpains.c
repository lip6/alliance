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
/* file		: pat_addpains.c					*/
/* date		: Sep 30 1996						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_addpains						*/
/* description	: create an empty PAINS (inspected instance) structure	*/
/*		  at the top of a list					*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct pains *pat_addpains (lastpains, insname, filname)

struct pains   *lastpains;	/* pointer on the last pains structure	*/
char           *insname  ;	/* name of instance			*/
char           *filname  ;	/* name of generated file		*/

  {
  struct pains *ptins;

  ptins           = (struct pains *) mbkalloc (sizeof(struct pains));

  ptins->INSNAME  = insname;
  ptins->FILNAME  = filname;
  ptins->MODEL    = NULL;
  ptins->VALUE    = NULL;
  ptins->INSTANCE = NULL;
  ptins->FILPNT   = NULL;

  ptins->NEXT     = lastpains;

  return (ptins);
  }
