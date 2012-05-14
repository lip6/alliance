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
 * Tool    : BooG - unflatten with optimized delay
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef UNFLATTEN_DELAY_H
#define UNFLATTEN_DELAY_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/***************************************************************************/
/* try to reduce the number of entries without changing the latest entry   */
/*a cell inserted must produce an output delay shortest than latest entry  */
/* return the new list of inputs after insertion                           */
/* if no cell can satisfy condition result the entry list unchanged        */
/***************************************************************************/
extern input_list* unflatten_delay __P ((input_list *entry, int entry_number, input_list* last_entry));

#endif
