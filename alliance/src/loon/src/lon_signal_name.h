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
 * Tool    : LooN - delay netlits
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef SIGNAL_NAME_H
#define SIGNAL_NAME_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/***************************************************************************/
/*  free index referencing                                                 */
/***************************************************************************/
extern void free_nameindex __P (());

/***************************************************************************/
/*return a new name to distinguish output value from input when inout      */
/***************************************************************************/
extern char* output_name __P ((char* name));

/***************************************************************************/
/* return name concatenated with an index and seperated by '_'             */
/* this index is : how many times I have sent this name to getnameindex()  */
/* unicity is guaranteed until you run Put_index_to_zero() below           */
/***************************************************************************/
extern char* getnameindex __P ((char* name));

/******************************************************************************/
/*      build a generic name with an incremented index                        */
/******************************************************************************/
extern char* getautoname __P ((char* name));

#endif
