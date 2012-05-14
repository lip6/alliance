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




#ifndef OPTIM_CAPA_H
#define OPTIM_CAPA_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/***************************************************************************/
/* improve capacitance of lofig on critical path by repowered instance     */
/*critical path could be changed and can't be improved any more            */
/***************************************************************************/
extern void improve_capa_critical_path __P ((lofig_list* lofig, int optim_level));

/***************************************************************************/
/*improve big RC of circuit                                                */
/***************************************************************************/
extern void improve_RC_circuit __P ((lofig_list* lofig, int optim_level));

/***************************************************************************/
/* improve capacitance of lofig on critical path by adding buffer          */
/*critical path could be changed and can't be improved any more            */
/*return the number of buffers inserted                                    */
/***************************************************************************/
extern int insert_buffer_critical_path __P ((lofig_list* lofig, int optim_level));

#endif
