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
 * Tool    : BooG - evaluate Capacitance
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef MAP_ADAPT_H
#define MAP_ADAPT_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/******************************************************************************/
/* swap the contain of 2 abl                                                  */
/******************************************************************************/
extern void swap_pointers __P ((chain_list* abl1, chain_list* abl2));

/******************************************************************************/
/* invert abl of port whose NEGATIV flag is set                               */
/******************************************************************************/
extern void invert_port __P ((port_list* port));

/******************************************************************************/
/* adapt expr to match cell library                                           */
/* if negativ is non zero opposite value if expr is wanted                    */
/* evaluate capacitance of leaves                                             */
/******************************************************************************/
extern chain_list* adapt_abl __P ((chain_list* expr));

/******************************************************************************/
/* adapt biabl to match cell library                                          */
/* evaluate capacitance of leaves                                             */
/******************************************************************************/
extern biabl_list* adapt_bus __P ((biabl_list* biabl));

/******************************************************************************/
/* adapt biabl to match cell library                                          */
/* evaluate capacitance of leaves                                             */
/******************************************************************************/
extern biabl_list* adapt_reg __P ((biabl_list* biabl));


#endif
