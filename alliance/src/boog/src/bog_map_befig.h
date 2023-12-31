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
 * Tool    : BooG - mapping standard cells on befig
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef MAP_BEFIG_H
#define MAP_BEFIG_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/****************************************************************************/
/*return an unique index for losig in lofig under building                  */
/****************************************************************************/
extern long getindex __P (());

/****************************************************************************/
/* set   an unique index for losig in lofig under building                  */
/****************************************************************************/
extern void setindex __P ((long new_index));

/****************************************************************************/
/* return the losig named name in lofig under building                      */
/****************************************************************************/
extern losig_list* seeksignal __P ((char* name));

/****************************************************************************/
/*return an unique losig named as losig_sce                                 */
/****************************************************************************/
extern losig_list* make_equi __P ((losig_list* losig_sce, losig_list* losig_dest));

/****************************************************************************/
/*                 build the lofig from befig                               */
/****************************************************************************/
extern lofig_list* map_befig __P ((befig_list *befig, char* lofig_name));

/****************************************************************************/
/* return the number of loins used in lofig                                 */
/****************************************************************************/
extern ptype_list* count_instance __P ((lofig_list* lofig));

#endif
