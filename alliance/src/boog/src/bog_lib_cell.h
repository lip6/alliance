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
 * Tool    : BooG - library cells
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef LIB_CELL_H
#define LIB_CELL_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/******************************************************************************/
/*add a pure logic cell										                           */
/*return the new cell if inserted in library else return NULL                 */
/******************************************************************************/
extern cell_list* addCell_logic __P ((befig_list* befig, chain_list* abl));

/******************************************************************************/
/*build a new bus cell in our library	                     						*/
/*return the new cell if inserted in library else return NULL                 */
/******************************************************************************/
extern cell_list* addCell_bus __P ((befig_list* befig, biabl_list* biabl));

/******************************************************************************/
/*build a new flip-flop or latch cell in our library                          */
/*return the new cell if inserted in library else return NULL                 */
/******************************************************************************/
extern cell_list* addCell_register __P ((befig_list* befig, biabl_list *biabl));

/******************************************************************************/
/*       return the list of register  cells                                   */
/******************************************************************************/
extern cell_list* getcell_register_lib __P (());

/******************************************************************************/
/*       return the list of tristate cells                                   */
/******************************************************************************/
extern cell_list* getcell_tristate_lib __P (());

/******************************************************************************/
/*       return the list of pure logical cells                                */
/******************************************************************************/
extern cell_list* getcell_logic_lib __P (());

/******************************************************************************/
/*          set the list of register  cells                                   */
/******************************************************************************/
extern void setcell_register_lib __P ((cell_list* cells_lib));

/******************************************************************************/
/*           set the list of tristate cells                                   */
/******************************************************************************/
extern void setcell_tristate_lib __P ((cell_list* cells_lib));

/******************************************************************************/
/*          set the list of pure logical cells                                */
/******************************************************************************/
extern void setcell_logic_lib __P ((cell_list* cells_lib));

/******************************************************************************/
/*              return 1 if a tristate inverter exists in library          	*/
/******************************************************************************/
extern int is_tristate_inverter_lib __P (());


#endif
