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
 * Tool    : BooG - evaluate delay of an abl
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef MAP_DELAY_H
#define MAP_DELAY_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/***************************************************************************/
/*eval delay of a node of an abl                                           */ 
/*  node of an abl (sortance is 1)                                         */
/***************************************************************************/
extern float loc_eval_delay __P ((chain_list *abl, float C));

/***************************************************************************/
/*  return the delay of an abl                                             */
/* memorize for this name the delay and the impedance                      */
/***************************************************************************/
extern float eval_delay __P ((char* name, chain_list *abl));

/******************************************************************************/
/*                            eval delay of a tristate                        */
/* memorize for this name the delay and the impedance                         */
/******************************************************************************/
extern float eval_bus __P ((char* name, biabl_list* biabl));


/******************************************************************************/
/*                            eval a register delay                      	   */
/* delay evaluated from the setup command                                     */
/* memorize for this name the delay and the impedance                         */
/******************************************************************************/
extern float eval_reg __P ((char* name, biabl_list* biabl));


/******************************************************************************/
/*eval the delay value of a biabl register                                    */
/******************************************************************************/
extern float eval_value_reg __P ((biabl_list* biabl));

/******************************************************************************/
/*                           eval clock delay                                 */
/******************************************************************************/
extern float eval_clock_reg __P ((biabl_list* biabl));

/******************************************************************************/
/*eval the delay value of a biabl bus                                         */
/******************************************************************************/
extern float loc_eval_bus __P ((biabl_list* biabl, float C));

#endif
