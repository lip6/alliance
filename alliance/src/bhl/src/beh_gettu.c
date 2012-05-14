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
/* file		: beh_gettu.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_gettimeunit					*/
/* description	: returns a string containg the time unit of the	*/
/*		  description						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char *beh_gettimeunit (pt_fig)

struct befig *pt_fig;

  {
  char *unit_str = "";

  switch (pt_fig->TIME_UNIT)
    {
    case BEH_TU__FS:
      unit_str = "fs"; break;
    case BEH_TU__PS:
      unit_str = "ps"; break;
    case BEH_TU__NS:
      unit_str = "ns"; break;
    case BEH_TU__US:
      unit_str = "us"; break;
    case BEH_TU__MS:
      unit_str = "ms"; break;
    default:
      beh_error (70, NULL);
    }

  return (unit_str);
  }
