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
/* file		: pat_addpaiol.c					*/
/* date		: Mar 13 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_addpaiol						*/
/* description	: create a PAIOL (input-output) structure at the top	*/
/*		  of a list						*/
/* called func.	: namealloc, mbkalloc					*/
/* ###--------------------------------------------------------------### */

struct paiol *pat_addpaiol (lastpaiol, name, format, mode, blank)

struct paiol  *lastpaiol;	/* pointer on the last paiol structure	*/
char          *name;		/* input-output's name			*/
char           format;		/* input-output format			*/
char           mode;		/* input-output mode			*/
unsigned char  blank;		/* nbr of blank column to be printed	*/
				/* after the input-output value		*/

  {
  struct paiol *ptiol;

  name          = namealloc (name);

  ptiol         = (struct paiol *) mbkalloc (sizeof(struct paiol));
  ptiol->NAME   = name;
  ptiol->FORMAT = format;
  ptiol->MODE   = mode;
  ptiol->BLANK  = blank;
  ptiol->VALUE  = '*';
  ptiol->SIG    = NULL;
  ptiol->FLAG   = 0     ;
  ptiol->NEXT   = lastpaiol;

  return (ptiol);
  }
