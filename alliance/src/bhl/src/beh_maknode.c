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
/* file		: beh_maknode.c						*/
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
/* function     : beh_maknode						*/
/* description  : create node for each signal register or bus		*/ 
/* called func. : beh_makquad beh_makbdd beh_makderiv beh_makgex	*/
/* ###--------------------------------------------------------------### */

void beh_maknode (pt_befig, node_flag, trace)

struct befig *pt_befig ;
char          node_flag;
char          trace    ;

  {
	/* ###------------------------------------------------------### */
	/*   setting missing flags :					*/
	/*     - derivatives need quads, Bdd and list of variables	*/
	/*     - if two flags are set, quads are needed			*/
	/* ###------------------------------------------------------### */

  if ((node_flag & BEH_NODE_DERIVATE) != 0)
    node_flag |= BEH_NODE_BDD | BEH_NODE_VARLIST | BEH_NODE_QUAD;

  if ((node_flag & BEH_NODE_VARLIST) != 0)
    {
    if (((node_flag & BEH_NODE_BDD)   || (node_flag & BEH_NODE_GEX) ||
         (node_flag & BEH_NODE_USER)) != 0)
      {
      node_flag |= BEH_NODE_QUAD ;
      }
    }
  else
    {
    if ((node_flag & BEH_NODE_USER) != 0)
      {
      if (((node_flag & BEH_NODE_BDD) || (node_flag & BEH_NODE_GEX)) != 0)
        {
        node_flag |= BEH_NODE_QUAD ;
        }
      else
        {
        if (((node_flag & BEH_NODE_BDD) && (node_flag & BEH_NODE_GEX)) != 0)
          {
          node_flag |= BEH_NODE_QUAD ;
          }
        }
      }
    }

	/* ###------------------------------------------------------### */
	/*    call the appropriate function two build the requested	*/
	/* structures							*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*   build quads to support further structures			*/
	/* ###------------------------------------------------------### */

  if ((node_flag & BEH_NODE_QUAD) != 0)
    {
    if (trace != 0)
      beh_message (14, "QUAD");
    beh_makquad (pt_befig);
    }

	/* ###------------------------------------------------------### */
	/*   build general expressions					*/
	/* ###------------------------------------------------------### */

  if ((node_flag & BEH_NODE_GEX) != 0)
    {
    if (trace != 0)
      beh_message (14, "GEX");
    beh_makgex (pt_befig);
    }

	/* ###------------------------------------------------------### */
	/*   build binary decision diagrams				*/
	/* ###------------------------------------------------------### */

  if ((node_flag & BEH_NODE_BDD) != 0)
    {
    if (trace != 0)
      beh_message (14, "BDD");
    beh_makbdd (pt_befig);
    }

	/* ###------------------------------------------------------### */
	/*   build list of variables					*/
	/* ###------------------------------------------------------### */

  if ((node_flag & BEH_NODE_VARLIST) != 0)
    {
    if (trace != 0)
      beh_message (14, "list of variables");
    beh_makvarlist (pt_befig);
    }

	/* ###------------------------------------------------------### */
	/*   build derivatives of binary expressions (list of Bdds)	*/
	/* ###------------------------------------------------------### */

  if ((node_flag & BEH_NODE_DERIVATE) != 0)
    {
    if (trace != 0)
      beh_message (14, "derivatives");
    beh_makderiv (pt_befig);
    }

  }
