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
/* file		: beh_clrdepend.c					*/
/* date		: Mar  8
/* version	: v111							*/
/* author	: Laurent VUILLEMIN, Pirouz BAZARGAN SABET		*/
/* description	: beh_clrdepend  			           	*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_clrdepend						*/
/* description	: clear the dependecy lists in the description		*/
/* called func.	: freechain						*/
/* ###--------------------------------------------------------------### */

void beh_clrdepend (pt_fig)

struct befig *pt_fig;

  {
  struct berin *pt_rin;

  if (( pt_fig                           != NULL) &&
      ( pt_fig->ERRFLG                   == 0   ) &&
      ((pt_fig->FLAG   & BEH_FIG_DEPEND) != 0   )   )
    {
    pt_rin = pt_fig->BERIN;
    while (pt_rin != NULL)
      {
      freechain (pt_rin->OUT_REF);
      pt_rin->OUT_REF = NULL;

      freechain (pt_rin->AUX_REF);
      pt_rin->AUX_REF = NULL;

      freechain (pt_rin->REG_REF);
      pt_rin->REG_REF = NULL;

      freechain (pt_rin->BUS_REF);
      pt_rin->BUS_REF = NULL;

      freechain (pt_rin->BUX_REF);
      pt_rin->BUX_REF = NULL;

      freechain (pt_rin->MSG_REF);
      pt_rin->MSG_REF = NULL;

      freechain (pt_rin->DLY_REF);
      pt_rin->DLY_REF = NULL;

      pt_rin = pt_rin->NEXT;
      }

    pt_fig->FLAG ^= BEH_FIG_DEPEND;
    }
  }
