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
 * Tool    : BooG - negativ operands
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef LIB_NEGATIV_H
#define LIB_NEGATIV_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/****************************************************************************/
/* build a tree on negativ logic by propagation of NOT to the leaves        */
/* negativ is set to 1 to return the opposite abl (also in negativ logic)   */
/****************************************************************************/
extern chain_list* inv_oper __P ((chain_list* abl, int negativ));

/******************************************************************************/
/*return the abl in negativ logic								                     */
/*we need 2 levels minimum of operator to invert an abl				            */
/*if only 1 level or neither odd nor even level return an unchanged abl		   */
/*parameter is deleted										                           */
/******************************************************************************/
extern chain_list* build_negativ __P ((chain_list* abl));

#endif
