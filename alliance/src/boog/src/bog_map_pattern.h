/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 *
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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

/*
 * Tool    : BooG - pattern matching for shortest delay
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef MAP_PATTERN_H
#define MAP_PATTERN_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/******************************************************************************/
/*return number of leaves absorbed by a pattern                               */
/*   if there twice the same value in pattern we should have twice a value on */
/* the same place in expr                                                     */
/* signal inverted values                                                     */
/******************************************************************************/
extern int eval_pattern __P ((chain_list* expr, chain_list* pattern, int negativ));

/******************************************************************************/
/*				return the cell wich matches expr				                     */
/*fulfill the ORDER field of cell with leaves of pattern matching			      */
/******************************************************************************/
extern cell_list* cell_pattern __P ((chain_list* expr));

/******************************************************************************/
/*		         return the cell tristate wich matches biabl				         */
/*fulfill the ORDER field of cell with leaves of pattern matching			      */
/******************************************************************************/
extern cell_list* cell_pattern_bus __P ((biabl_list* biabl));

/******************************************************************************/
/*		return the cell register wich matches biabl				                  */
/*fulfill the ORDER field of cell with leaves of pattern matching			      */
/******************************************************************************/
extern cell_list* cell_pattern_reg __P ((biabl_list* biabl));

/************************************************************************/
/* to locate easier the port of bepor in the abl for later use		      */
/*	do pointing the leaf of abl on the elements of port and sorted 	   */
/*	like bepor										                              */
/* returning an abl  with each leaf pointing on one port element		   */
/************************************************************************/
extern chain_list* build_reference __P ((befig_list* befig, port_list* port, port_list* internal_port, chain_list* abl));

/************************************************************************/
/* create a pseudo port, if an internal register is used in abl         */
/* return the list of pseudo port                                       */
/************************************************************************/
extern port_list* internal_reference __P ((befig_list* befig, port_list* port, chain_list* abl));

/******************************************************************************/
/*  begin of the abl or not, return 1 if first eval                           */
/******************************************************************************/
extern int first_eval __P (());

/******************************************************************************/
/*  last recursion on this abl, free all marks                                */
/******************************************************************************/
extern void free_eval __P (());


#endif
