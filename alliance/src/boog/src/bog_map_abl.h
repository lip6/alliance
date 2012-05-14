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
 * Tool    : BooG - mapping standard cells on abl
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef MAP_ABL_H
#define MAP_ABL_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/******************************************************************************/
/*                initialize area evaluation                                  */
/******************************************************************************/
extern void begin_count_area __P (());

/******************************************************************************/
/*                return area evaluation                                      */
/******************************************************************************/
extern unsigned int count_area __P (());

/****************************************************************************/
/* build loins and its connections in lofig according to abl                */
/* output is the signal of the variable which the abl defines               */
/****************************************************************************/
extern void map_abl __P ((lofig_list *lofig, chain_list *abl, losig_list *output));

/******************************************************************************/
/*                            map a tristate                                  */
/******************************************************************************/
extern void map_bus __P ((lofig_list* lofig, biabl_list* biabl, losig_list* output_losig));

/******************************************************************************/
/*                    map on a simple register                                */
/******************************************************************************/
extern void map_register __P ((lofig_list* lofig, biabl_list* biabl, losig_list* output_losig));



#endif
