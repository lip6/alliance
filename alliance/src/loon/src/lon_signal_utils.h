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
 * Tool    : LooN - signals table
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/***************************************************************************/
/*    free memory from image of signals in hash table                      */
/***************************************************************************/
extern void free_signals __P (());

/***************************************************************************/
/*   put a delay to a signal (local hash table)                            */
/***************************************************************************/
extern void putdelay __P ((char* name, double delay));

/***************************************************************************/
/*   return the signal delay   (local hash table)                          */
/***************************************************************************/
extern double getdelay __P ((char* name));

/***************************************************************************/
/*   add a delay to a signal                                               */
/***************************************************************************/
extern void incdelay __P ((char* name, double delay));

/***************************************************************************/
/*   return the signal capacitance                                         */
/***************************************************************************/
extern double getcapacitance __P ((char* name));

/***************************************************************************/
/*   add a capacitance to a signal                                         */
/***************************************************************************/
extern void inccapacitance __P ((char* name, double capacitance));

/***************************************************************************/
/*   put a capacitance to a signal                                         */
/***************************************************************************/
extern void putcapacitance __P ((char* name, double capacitance));


#endif
