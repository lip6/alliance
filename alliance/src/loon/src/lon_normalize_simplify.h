/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 *
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Tool    : LooN - simplify befig expressions
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef NORMALIZE_SIMPLIFY_H
#define NORMALIZE_SIMPLIFY_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/******************************************************************************/
/* put binary oper to befig to be understood by parser and PROOF (CAO util.)  */
/* for debugging mode                                                         */
/******************************************************************************/
extern void binary_oper __P ((befig_list *befig));

/***************************************************************************/
/*                   Simplify the abl of befig                             */
/***************************************************************************/
extern void simplify_expr __P ((befig_list* befig));

#endif
