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
/* file		: pat_globals.c						*/
/* date		: Mar  9 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level pat function				*/
/* ###--------------------------------------------------------------### */

#include "mut.h"
#include "pat.h"

unsigned int  PAT_LINNUM   = 1         ;

unsigned char PAT_TIMEUNIT = PAT_TU__PS;
unsigned int  PAT_TIMESTEP = 1         ;

float         PAT_CNV_FS  ;
float         PAT_CNV_PS  ;
float         PAT_CNV_NS  ;
float         PAT_CNV_US  ;
float         PAT_CNV_MS  ;
