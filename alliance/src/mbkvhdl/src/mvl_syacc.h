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
/*									*/
/* file		: mvl_syacc.h						*/
/* date		: Nov 07 1991						*/
/* author	: P.BAZARGAN, L.A.TABUSSE, VUONG H.N.			*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/*									*/
/* ###--------------------------------------------------------------### */

extern char          MVL_MBKMOD;        /* 'A' or 'P' or 'C')           */
extern char          MVL_ERRFLG;	/* if = 1 no structure is made	*/
extern struct lofig *MVL_LOFPNT;        /* current LOFIG pointer        */ 


extern struct dct_entry **MVL_HSHTAB;	/* dictionnary's entry points	*/

extern struct dct_entry **mvl_initab();
extern void          mvl_addtab();
extern int           mvl_chktab();
extern void          mvl_fretab();
extern void          mvl_error();
extern void          mvl_warning();

/* ###--------------------------------------------------------------### */

extern losig_list *mvl_addlosig ();
extern locon_list *mvl_addlocon ();

/* ###--------------------------------------------------------------### */
