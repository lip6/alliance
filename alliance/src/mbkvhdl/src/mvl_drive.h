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
/* file		: mvl_drive.h						*/
/* date		: Nov 12 91						*/
/* author	: L.A. TABUSSE & P. BAZARGAN-SABET & VUONG H.N.		*/
/*									*/
/* content	: declaration of functions and global variables used by	*/
/*		  the structural MBK-> VHDL driver			*/
/*									*/
/* ###--------------------------------------------------------------### */


extern void	mvl_addtab();
extern int	mvl_chktab();
extern void     mvl_toolbug();
extern void     mvl_message();
extern void     mvl_error();
extern char     *mvl_vhdlname();
extern void     *mvl_name();
extern char     *mvl_abl2str();
extern void     *mvl_vectnam();
extern char     mvl_chkconsistency();
