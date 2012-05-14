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

/************************************************************************/
/* module   : def_gen.c							*/
/* date     : 02/09/93							*/
/* auteurs  : RENAUD & VENOT & PITON					*/
/************************************************************************/

#ident "$Id: DEF_GEN.c,v 1.3 2012/05/14 14:20:14 alliance Exp $"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mut.h"
#include "pat.h"
#include "libpat_l.h"

/* ###--------------------------------------------------------------### */ 
/* function     : DEF_GENPAT                                            */
/* description  : create PASEQ struct and initialization                */
/* called func. : pat_addpaseq, CreateLabel                             */
/* ###--------------------------------------------------------------### */ 
 

void d_DEF_GENPAT(chaine)
char *chaine;
  {

    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

  racine_paseq        = pat_addpaseq (racine_paseq);
  racine_paseq->NAME  = namealloc (chaine);
  racine_paseq->FLAGS |= PAT_SEQ__IOLARRAY; /* Czo */

  GNP_PATTERN_COURANT = 0;
  GNP_PAIOL_INDEX     = 0;
  IOLNUMBER           = 0;

  InitHash (hash_paiol);
  InitHash (hash_pagrp);
  }
