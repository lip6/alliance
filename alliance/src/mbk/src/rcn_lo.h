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

#ident "$Id: rcn_lo.h,v 1.2 2002/09/30 16:20:51 czo Exp $"

/*******************************************************************************
* rcn     : rcn_lo.c header file for local function  declaration               *
*                                                                              *
* version : 2.00                                                               *
* date    : 05/08/1997                                                         *
*******************************************************************************/

/*******************************************************************************
* Grégoire AVOT                                                                *
* Laboratoire LIP6 - Département ASIM                                          *
* Universite Paris 6 - Pierre et Marie Curie                                   *
* 4, Place Jussieu                                                             *
* Paris Cedex 05.                                                              *
*******************************************************************************/

int		 chkloopmain __P((losig_list *ptsig, unsigned char flag,
		                  long org, lowire_list *form));

/* Heap */

lowire_list	*heaplowire __P((void));
lorcnet_list	*heaprcnet __P((void));
loctc_list	*heaploctc __P((void));
void		 freercnet __P((lorcnet_list *pt));
void		 freelowire __P((lowire_list *pt));
void		 freeloctc __P((loctc_list *pt));
