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
 * Tool    : BooG - inputs treatment
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef UNFLATTEN_UTILS_H
#define UNFLATTEN_UTILS_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/*to represent the duality of each input*/
typedef struct input {
	struct input *NEXT;
	chain_list   *ABL;
	chain_list   *NEG_ABL;
	float 		 DELAY;
	float 		 NEG_DELAY;
	float 		 R;
	float 		 NEG_R;
} input_list;

/*for evaluation of a dual solution which gives an output and its opposite*/
typedef struct note {
   struct note*   NEXT;
   cell_list*     CELL;
   cell_list*     NEG_CELL;
   float          COST;
   float          NEG_COST;
   float          AVERAGE_COST;
   float          DELAY;
   float          NEG_DELAY;
	float 		   R;
	float 		   NEG_R;
   int            ARITY;
} note_list;




/***************************************************************************/
/*                 give a new element from the heap                        */
/***************************************************************************/
extern input_list* newinput __P (());

/***************************************************************************/
/*    free recursively a list of inputs and also their abl                 */
/***************************************************************************/
extern void freeinput __P ((input_list* head));

/***************************************************************************/
/* create a new structure and copy all the fields of input                 */
/***************************************************************************/
extern input_list* dupinput __P ((input_list* input));

/***************************************************************************/
/*               insert new in the list of inputs                          */
/* sorted by the average of delay and its inverted value delay             */
/***************************************************************************/
extern input_list* sort_input __P ((input_list* new_input, input_list* inputs));

/***************************************************************************/
/*                 give a new element from the heap                        */
/***************************************************************************/
extern note_list* newnote __P (());

/***************************************************************************/
/*    free recursively a list of notes                                     */
/***************************************************************************/
extern void freenote __P ((note_list* head));

#endif
