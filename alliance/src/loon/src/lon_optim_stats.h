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
 * Tool    : LooN - statistics
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef OPTIM_STATS_H
#define OPTIM_STATS_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/***************************************************************************/
/*return the RC maximum of lofig                                           */
/***************************************************************************/
extern double getmaxRC __P ((lofig_list* lofig));

/****************************************************************************/
/* return the number of loins used in lofig                                 */
/****************************************************************************/
extern ptype_list* count_instance __P ((lofig_list* lofig));

/****************************************************************************/
/*give the percentage of area of each modelcontained in model_list          */
/*count_instance() should be caught first                                   */
/****************************************************************************/
extern ptype_list* percent_instance __P ((ptype_list* model_list));

/****************************************************************************/
/* return the area of lofig                                                 */
/*percent_instance() should be caught first                                 */
/****************************************************************************/
extern long getnum_ins __P ((lofig_list* lofig));

/****************************************************************************/
/* return the area of lofig                                                 */
/*count_instance() should be caught first                                   */
/****************************************************************************/
extern long getarea __P ((lofig_list* lofig));

#endif
