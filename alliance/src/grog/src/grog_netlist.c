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

/*******************************************************************************
* Grog : generic rom generator                                                 *
*                                                                              *
* Architecture and leaf cells defined at Bull's research center at les Clayes  *
*                                                                              *
* Leaf cells modifications to meet idps design rules                           *
* version 0.0 on April/June 1992, Frederic Petrot                              *
*                                                                              *
* All programming in Genlib                                                    *
* version 0.0 on April/June 1992, Frederic Petrot                              *
*                                                                              *
* version 0.1, by Frederic Petrot                                              *
*                                                                              *
*******************************************************************************/

#include "genlib.h"
#include "grog.h"

void grognetlist(char *name, int nb, int nw, int zh, int msb)
{
long i;

	GENLIB_DEF_LOFIG(name);
	if (!msb)
	GENLIB_LOCON(GENLIB_BUS("adr", ln2p(nw) - 1L, 0L), IN, GENLIB_BUS("adr", ln2p(nw) - 1L, 0L));
	else
	GENLIB_LOCON(GENLIB_BUS("adr", 0L, ln2p(nw) - 1L), IN, GENLIB_BUS("adr", 0L, ln2p(nw) - 1L));
   if (nw == 64)
      GENLIB_LOCON("ck", IN, "ck");
   else if (nw == 128 || nw == 256)
      GENLIB_LOCON("ck[0:1]", IN, "ck[0:1]");
   else for (i = 0; i < nw / 512; i += 2)
      GENLIB_LOCON(GENLIB_ELM("ck", i / 2), IN, GENLIB_ELM("ck", i / 2));
	GENLIB_LOCON(GENLIB_BUS("f", 0L, nb - 1L), OUT, GENLIB_BUS("f", 0L, nb - 1L));
	if (zh)
		GENLIB_LOCON("oe", IN, "oe");
	GENLIB_LOCON("vdd", IN, "vdd");
	GENLIB_LOCON("vss", IN, "vss");
	GENLIB_SAVE_LOFIG();
}
