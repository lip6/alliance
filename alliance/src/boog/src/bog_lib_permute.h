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
 * Tool    : BooG - tree permutations
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef LIB_PERMUTE_H
#define LIB_PERMUTE_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/******************************************************************************/
/* duplicate a list                                                           */
/******************************************************************************/
extern chain_list* dupchain __P ((chain_list* chain));

/******************************************************************************/
/* return a list of list of elements                                          */
/*which compose all the possible different orders of elem_chain               */
/* elements aren't duplicated                                                 */
/******************************************************************************/
extern chain_list *swapchain __P ((chain_list* elem_chain));

/******************************************************************************/
/*parameter is a list of element list: for a place of the final list, several */
/*elements are possible                                                       */
/* return a list of element chain based on all choices proposed in parameter  */
/******************************************************************************/
extern chain_list* composechain __P ((chain_list* leaves_list));

/******************************************************************************/
/*    return the list of different abl	resulting of permutations  				*/
/******************************************************************************/
extern chain_list* different_abl __P ((befig_list* befig, chain_list* abl));

/******************************************************************************/
/*    return the list of different biabl	resulting of permutations  		   */
/******************************************************************************/
extern chain_list* different_biabl __P ((befig_list* befig, biabl_list* biabl));


#endif

