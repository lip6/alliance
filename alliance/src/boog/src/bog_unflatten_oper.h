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
 * Tool    : BooG - unflatten an operator
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef UNFLATTEN_OPER_H
#define UNFLATTEN_OPER_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/***************************************************************************/
/* return the unflat output and its inverted value                         */
/***************************************************************************/
extern input_list* unflatten_oper __P ((int oper, input_list* inputs));

/**************************************************************************/
/* return -1 if oper not match with OPER                                  */
/*         0 if oper needs inverted source                                */
/*         1 if oper needs positiv source                                 */
/**************************************************************************/
extern int is_source_positiv __P ((int oper));

/**************************************************************************/
/* return -1 if oper not match with OPER                                  */
/*         0 if oper gives inverted destination                           */
/*         1 if oper gives positiv destination                            */
/**************************************************************************/
extern int is_dest_positiv __P ((int oper, int arity));

#endif
