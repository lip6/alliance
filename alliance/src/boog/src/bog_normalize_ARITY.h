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
 * Tool    : BooG - put arity notion in abl
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef NORMALIZE_ARITY_H
#define NORMALIZE_ARITY_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/*head operator arity*/
/*  Warning: simpablexpr() do not work anymore after pulling arity         */
#define ABL_ARITY(abl) ((long)ABL_CDR((chain_list*)ABL_CAR(abl)))
#define ABL_ARITY_L(abl) (ABL_CDR((chain_list*)ABL_CAR(abl)))

/***************************************************************************/
/*             put arity operator in abl->DATA->NEXT field                 */
/*  Warning: simpablexpr() do not work anymore after pulling arity         */
/***************************************************************************/
extern void put_arity_abl __P ((chain_list* abl));

/***************************************************************************/
/* arity is put on abl->DATA->NEXT field for each operator                 */
/***************************************************************************/
extern void put_arity __P ((befig_list* befig));

#endif
