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
 * Tool    : BooG - count Capacitance
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef SIGNAL_ADAPT_H
#define SIGNAL_ADAPT_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/******************************************************************************/
/*calculate the capacitance for all signals                                   */
/* change also form of abl to adapt to the biggest cell                       */
/******************************************************************************/
extern void adapt_for_cell __P ((befig_list *befig));

/******************************************************************************/
/*  do an average betwenn each values and its opposite                        */
/******************************************************************************/
extern void average_capacitance __P ((befig_list *befig));

/******************************************************************************/
/* undefine all variables in hash table                                       */
/******************************************************************************/
extern void undefine_delay __P ((befig_list *befig));

#endif
