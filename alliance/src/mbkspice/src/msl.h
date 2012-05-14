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


/*******************************************************************************
*                                                                              *
*  Tool        : Spice parser / driver v 7.00                                  *
*  Author(s)   : Gregoire AVOT                                                 *
*  Updates     : March, 18th 1998                                              *
*                                                                              *
*******************************************************************************/
#ifndef MSLH
#define MSLH

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern int          msl_line;        /* current line number */
extern int          msl_figloaded;   /* TRUE if inside a SUBCKT */
extern char        *msl_figname;     /* name of main SUBCKT */
extern char        *msl_subckt;      /* name of current SUBCKT */
extern lofig_list  *msl_ptfig;       /* last lofig parsed */

void mslAddExtension __P(( int(*)() )); /* add function called during parsing */
void mslRmvExtension __P((          )); /* remove this function */

void mslAddCompletion __P(( int(*)() )); /* add function called after parsing */
void mslRmvCompletion __P((          )); /* remove this function */

#ifndef TRUE
#define TRUE (1==1)	/* c'est ben vrai	*/
#endif

#ifndef FALSE
#define FALSE (1==0)	/* tout faux		*/
#endif

extern int SPI_LANG;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

