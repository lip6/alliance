/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
/* file		: bhl111.h						*/
/* date		: Mar  2 2000						*/
/* version	: v111							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* contents	: high level library for behavioural description	*/
/* ###--------------------------------------------------------------### */

#ifndef BEH_BHLDEF
#define BEH_BHLDEF

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define BEH_STA_REPLACE_OUT	0x01
#define BEH_STA_REPLACE_BUS	0x02
#define BEH_STA_REPLACE_AUX	0x04
#define BEH_STA_REPLACE_BUX	0x08
#define BEH_STA_REPLACE_MSG	0x10
#define BEH_STA_REPLACE_REG	0x20

	/* ###------------------------------------------------------### */
	/*    structure definitions					*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*    functions							*/
	/* ###------------------------------------------------------### */

extern int           beh_chkbefig    ();
extern void          beh_debug       ();
extern void          beh_makbdd      ();
extern void          beh_makgex      ();
extern void          beh_makderiv    ();
extern void          beh_makvarlist  ();
extern void          beh_freabl      ();
extern void          beh_depend      ();

extern char         *beh_gettimeunit ();

extern void          beh_dly2sta     ();
extern void          beh_sta2dly     ();

extern struct chain *beh_namelist    ();
extern struct chain *beh_unamlist    ();
extern void          beh_indexbdd    ();

#endif
