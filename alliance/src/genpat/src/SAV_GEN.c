/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/***************************************************************/
/* module   : fnct_genpat                                      */
/* date     : 28/03/92                                         */
/* auteurs  : RENAUD & VENOT & PITON                           */
/***************************************************************/

#ident "$Id: SAV_GEN.c,v 1.3 2012/05/14 14:20:14 alliance Exp $"

#include <stdio.h>

#include "mut.h"
#include "pat.h"
#include "phl.h"
#include "ppt.h"

#include "libpat_l.h"

/*--------------------------------------------------------------*/
void d_SAV_GENPAT()

  {
  struct paiol *tmp_paiol;
  struct paevt *tmp_paevt;
  int           i = 0;

    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

  if (racine_pagrp != NULL)
    racine_pagrp = (struct pagrp*) reverse ((void *)racine_pagrp);

  if (racine_papat == NULL)
    Erreur("not all inputs initialized \n");
  else
    racine_papat = (struct papat*) reverse ((void *)racine_papat);

  if ( racine_papat->PAEVT == NULL )
    Erreur("not all inputs initialized \n");

  tmp_paiol = racine_paiol;
  while (tmp_paiol != NULL)
    {
    if (tmp_paiol->MODE == 'I')
      {
      tmp_paevt = racine_papat->PAEVT;
      while (tmp_paevt != NULL)
        {
        if (tmp_paevt->INDEX == i)
          break;
        tmp_paevt = tmp_paevt->NEXT;
        }

      if (tmp_paevt == NULL)
        Erreur ("not all inputs initialized at pattern zero \n");
      }
    i++;
    tmp_paiol = tmp_paiol->NEXT;
    }	

  racine_paseq->SUBSEQ = 1;
  racine_paseq->ENDFLG = 'Y';
  racine_paseq->ERRFLG = 0;

/*--------
pat_debug (racine_paseq,"paseq");
---------*/

  pat_savpaseq (racine_paseq->NAME, racine_paseq, MAX_LABEL);

  if (racine_pagrp != NULL)
    racine_pagrp = (struct pagrp*) reverse ((void *)racine_pagrp);

  }
