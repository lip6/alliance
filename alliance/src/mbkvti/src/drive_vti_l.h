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

/*******************************************************************************
* drive_vti_l.h : header file for the hns and fne driver                       *
*                                                                              *
* version : 4.11                                                               *
* date    : 12/06/1998                                                         *
*                                                                              *
* Modified, rewritten and maintained by Frederic Petrot since september 1990   *
* Modified by Gregoire Avot for RCN support.                                   *
*******************************************************************************/
static void vtigetdate();
void vtisavelofig();

#define HNSRCN_X 543345
#define HNSRCN_I 543346
struct rcnhns {
	char *insname;
	int   position;
};

void    wire_point();
