/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
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

/****************************************************************/
/* module   : settunit.c					*/
/* date     : 02/09/93						*/
/* auteurs  : PITON 						*/
/****************************************************************/

#ident "$Id: SETTUNIT.c,v 1.1 2002/05/30 13:28:15 xtof Exp $"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mut.h"
#include "pat.h"
#include "libpat_l.h"

/* ###--------------------------------------------------------------### */
/* function     : SETTUNIT                                              */
/* description  : set pat time unit        				*/
/* called func. : none                                                  */
/* ###--------------------------------------------------------------### */

/* Czo : 2000/09/04 */ 
void d_SETTUNIT(ident)
char *ident;
{

    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

   if (ident == NULL)
	Erreur ("SETTUNIT missing argument");

   if ((strlen (ident) == 0) || (namealloc(ident) == namealloc("vu")))
      {
      PAT_TIMEUNIT = PAT_TU__VU ;
      PAT_CNV_FS = 0.0 ;
      PAT_CNV_PS = 0.0 ;
      PAT_CNV_NS = 0.0 ;
      PAT_CNV_US = 0.0 ;
      PAT_CNV_MS = 0.0 ;
      }
    else if (namealloc(ident) == namealloc("fs"))
      {
      PAT_TIMEUNIT = PAT_TU__FS ;
      PAT_CNV_FS = 1.0    ;
      PAT_CNV_PS = 1.0E3  ;
      PAT_CNV_NS = 1.0E6  ;
      PAT_CNV_US = 1.0E9  ;
      PAT_CNV_MS = 1.0E12 ;
      }
    else if (namealloc(ident) == namealloc("ps"))
      {
      PAT_TIMEUNIT = PAT_TU__PS ;
      PAT_CNV_FS = 1.0E-3 ;
      PAT_CNV_PS = 1.0    ;
      PAT_CNV_NS = 1.0E3  ;
      PAT_CNV_US = 1.0E6  ;
      PAT_CNV_MS = 1.0E9  ;
      }
    else if (namealloc(ident) == namealloc("ns"))
      {
      PAT_TIMEUNIT = PAT_TU__NS ;
      PAT_CNV_FS = 1.0E-6 ;
      PAT_CNV_PS = 1.0E-3 ;
      PAT_CNV_NS = 1.0    ;
      PAT_CNV_US = 1.0E3  ;
      PAT_CNV_MS = 1.0E6  ;
      }
    else if (namealloc(ident) == namealloc("us"))
      {
      PAT_TIMEUNIT = PAT_TU__US ;
      PAT_CNV_FS = 1.0E-9 ;
      PAT_CNV_PS = 1.0E-6 ;
      PAT_CNV_NS = 1.0E-3 ;
      PAT_CNV_US = 1.0    ;
      PAT_CNV_MS = 1.0E3  ;
      }
    else if (namealloc(ident) == namealloc("ms"))
      {
      PAT_TIMEUNIT = PAT_TU__MS ;
      PAT_CNV_FS = 1.0E-12 ;
      PAT_CNV_PS = 1.0E-9 ;
      PAT_CNV_NS = 1.0E-6 ;
      PAT_CNV_US = 1.0E-3 ;
      PAT_CNV_MS = 1.0    ;
      }
    else
      {
      Erreur ("SETTUNIT : illegal time unit");
      }

if (racine_paseq != NULL)
  racine_paseq->TIME_UNIT = PAT_TIMEUNIT;
}

