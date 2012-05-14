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
/* file		: beh_makquad.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Laurent VUILLEMIN					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"

/* ###--------------------------------------------------------------### */
/* function     : beh_makquad						*/
/* description  : add a BEQUAD structure for each expression		*/
/* called func. : beh_addbequad	 					*/
/* ###--------------------------------------------------------------### */

void beh_makquad ( struct befig *pt_befig )

  {
  struct beaux  *pt_beaux;
  struct berin  *pt_berin;
  struct beout  *pt_beout;
  struct bebus  *pt_bebus;
  struct bebux  *pt_bebux;
  struct bereg  *pt_bereg;
  struct bemsg  *pt_bemsg;
  struct binode *pt_binod;
  struct biabl  *pt_biabl;

	/* ###------------------------------------------------------### */
	/*   simple output ports					*/
	/* ###------------------------------------------------------### */

  pt_beout = pt_befig->BEOUT;
  while (pt_beout != NULL)
     {
     pt_beout->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_beout       = pt_beout->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   bussed ouput ports						*/
	/* ###------------------------------------------------------### */

  pt_bebus = pt_befig->BEBUS;
  while (pt_bebus != NULL)
     {
     pt_binod = pt_bebus->BINODE;
     while (pt_binod != NULL)
       {
       pt_binod->CNDNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
       pt_binod->VALNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);

       pt_binod          = pt_binod->NEXT;
       }
     pt_bebus = pt_bebus->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal bussed signals					*/
	/* ###------------------------------------------------------### */

  pt_bebux = pt_befig->BEBUX;
  while (pt_bebux != NULL)
     {
     pt_binod = pt_bebux->BINODE;
     while (pt_binod != NULL)
       {
       pt_binod->CNDNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
       pt_binod->VALNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);

       pt_binod          = pt_binod->NEXT;
       }
     pt_bebux = pt_bebux->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal registers						*/
	/* ###------------------------------------------------------### */

  pt_bereg = pt_befig->BEREG;
  while (pt_bereg != NULL)
     {
     pt_binod = pt_bereg->BINODE;
     while (pt_binod != NULL)
       {
       pt_binod->CNDNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
       pt_binod->VALNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);

       pt_binod          = pt_binod->NEXT;
       }
     pt_bereg = pt_bereg->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal simple signals					*/
	/* ###------------------------------------------------------### */
    
  pt_beaux = pt_befig->BEAUX;
  while (pt_beaux != NULL)
     {
     pt_beaux->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_beaux       = pt_beaux->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal delayed signals					*/
	/* ###------------------------------------------------------### */

  pt_beaux = pt_befig->BEDLY;
  while (pt_beaux != NULL)
     {
     pt_beaux->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_beaux       = pt_beaux->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   assertions							*/
	/* ###------------------------------------------------------### */

  pt_bemsg = pt_befig->BEMSG;
  while (pt_bemsg != NULL)
     {
     pt_bemsg->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_bemsg       = pt_bemsg->NEXT;
     }

  pt_befig->TYPE = pt_befig->TYPE | BEH_NODE_QUAD;
  }
