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
 * Tool    : LooN - delay netlits
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef SIGNAL_NETLIST_H
#define SIGNAL_NETLIST_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/***************************************************************************/
/* increment(if 1) or decrement(if 0) the capacitance on signals branched  */
/* to loins from the values of its ports                                   */
/***************************************************************************/
extern void loins_capacitance __P ((loins_list* loins, int increment));

/***************************************************************************/
/* return the Time of a loins                                              */
/***************************************************************************/
extern double loins_max_T __P ((loins_list* loins));

/***************************************************************************/
/* return the RC of a loins. output is the name of the signal drived       */
/***************************************************************************/
extern double loins_max_RC __P ((loins_list* loins, char* output));

/***************************************************************************/
/* return the delay of a loins. output is the name of the signal drived    */
/***************************************************************************/
extern double loins_delay __P ((loins_list* loins, char* output));

/***************************************************************************/
/* return the delay of  a losig                                            */
/***************************************************************************/
extern double losig_delay __P ((losig_list* losig));

/***************************************************************************/
/* propagate modification of a loins for delay                             */
/***************************************************************************/
extern void propagate_loins_delay __P ((loins_list *loins));

/***************************************************************************/
/* propagate modification of a losig for delay                             */
/***************************************************************************/
extern void propagate_losig_delay __P ((losig_list *losig));

/***************************************************************************/
/* set all capacitances of signals                                         */
/***************************************************************************/
extern void set_circuit_capacitance __P ((lofig_list *lofig));

/***************************************************************************/
/*set all circuit delays with recursiv mode                                */
/***************************************************************************/
extern void set_circuit_delay __P ((lofig_list* lofig));


#endif
