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
/* file		: beh_expstable.c					*/
/* date		: Dec  2 1999						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_expstable						*/
/* description	: create an expression equivalent to STABLE attribute	*/
/* called func.	: addchain						*/
/* ###--------------------------------------------------------------### */

struct chain *beh_expstable (sig_nam)

char *sig_nam;

  {
  char         *dly_nam;
  struct chain *dly_atm;
  struct chain *opr_atm;
  struct chain *operat ;
  struct chain *pt_abl ;
  char          buffer [128];

  sprintf (buffer, "%s'delayed", sig_nam);
  dly_nam = namealloc (buffer); 

  dly_atm = addchain (NULL  ,          dly_nam);
  opr_atm = addchain (NULL  ,          sig_nam);
  operat  = addchain (NULL  , (char *) XOR    );
  pt_abl  = addchain (NULL  ,          dly_atm);
  pt_abl  = addchain (pt_abl,          opr_atm);
  pt_abl  = addchain (pt_abl,          operat );

  operat  = addchain (NULL  , (char *) NOT    );
  pt_abl  = addchain (NULL  ,          pt_abl );
  pt_abl  = addchain (pt_abl,          operat );

  return (pt_abl);
  }
