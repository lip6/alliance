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
/*                                                                      */
/* file         : mvl_parse.h 	                                     */
/* date         : Nov  12 1991                                          */
/* author       : L.A. TABUSSE & H.G. VUONG & P. BAZARGAN-SABET         */
/* description  : variable for Parser VHDL --> MBK                      */
/*                                                                      */
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */

char          MVL_MBKMOD;               /* The mode of getlofig         */
char          MVL_ERRFLG = 0;           /* if = 1 no structure is made  */
int           MVL_LINNUM = 1;           /* file's line number           */
char          MVL_CURFIL[200];          /* current file's name          */

struct dct_entry **MVL_HSHTAB;          /* dictionnary's entry points   */

extern void          mvl_error();
extern struct lofig *mvl_fill();

extern struct dct_entry **mvl_initab();
extern void               mvl_fretab();


/* ###--------------------------------------------------------------### */

struct lofig   *MVL_LOFPNT = NULL;      /* current LOFIG pointer        */

extern FILE    *mvl_y_in;
extern int      mvl_y_parse();
extern void 	mvl_decomp();

/* ###--------------------------------------------------------------### */
