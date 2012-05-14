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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * Tool    : BooG - change name signals
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef NORMALIZE_NAMEINDEX_H
#define NORMALIZE_NAMEINDEX_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/***************************************************************************/
/*if a vector has some space left between 2 bits, change vector in one to  */
/*one bit no to corrupt befig                                              */
/***************************************************************************/
extern void normalize_nameindex __P ((befig_list* befig));

/***************************************************************************/
/* some radical name can generate conflict with automatic indexation       */
/*change name not to disturb algorithm                                     */
/***************************************************************************/
extern void change_radical __P ((befig_list* befig));

/***************************************************************************/
/*  sort vectors to be recognized by parser/driver                         */
/***************************************************************************/
extern void sort_vector __P ((befig_list* befig));


#endif
