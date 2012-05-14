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
 * Tool    : Loon and Boog - xsch color driver
 * Date    : 2000
 * Author  : Francois Donnet, Ludovic Jacomme(for xsch)
 */



#ifndef XSCH_DRIVER_H
#define XSCH_DRIVER_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


#define XSCH_CRITICAL_PATH 0
#define XSCH_GRADIENT 1


/***************************************************************************************/
/* dump in xsch_file with format .xsc the weight of signals in delay                   */
/* if signals belong to long_path a warning color is chosen                            */
/*lofigchain is needed in lofig                                                        */
/***************************************************************************************/
extern void save_xsch __P ((FILE* xsch_stream, lofig_list* lofig, ptype_list* long_path, int color_mode));

#endif
