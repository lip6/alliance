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
 * Tool    : BooG - unflatten an abl
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_signal_nameindex.h"
#include "bog_normalize_ARITY.h"
#include "bog_lib_utils.h"
#include "bog_lib_complete.h"
#include "bog_signal_utils.h"
#include "bog_unflatten_utils.h"
#include "bog_unflatten_oper.h"
#include "bog_unflatten_abl.h"



/***************************************************************************/
/* unflatten each oper of abl                                              */
/***************************************************************************/
static input_list* unflatten_each_oper(chain_list* abl)
{
   chain_list* chain;
   input_list* new, *inputs=NULL;
   char* name, *opposite;
   int oper;

   /*for leaf build also opposite*/
   if (ABL_ATOM(abl)) {
      name=ABL_ATOM_VALUE(abl);
      opposite=getoppositename(name);
      new=newinput();
      new->ABL=abl;
      new->NEG_ABL=createablatom(opposite);
      new->DELAY=getdelay(name);
      new->NEG_DELAY=getdelay(opposite);
      new->R=0;
      new->NEG_R=0;
      return new;
   }

   /*buid a list of inputs for algorithm*/
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      new=unflatten_each_oper(ABL_CAR(chain));
      /*sort from the shortest to the longest delay*/
      inputs=sort_input(inputs,new);
   }
   oper=ABL_OPER(abl);
   freechain(abl);

   return unflatten_oper(oper, inputs);
}


/***************************************************************************/
/* unflatten an abl to match with the cell library                         */
/* optimize in delay or in area depending of .lax file                     */
/***************************************************************************/
extern chain_list* unflatten_abl(chain_list* abl)
{
   input_list* input;

   if (!abl) {
      fprintf(stderr,"unflatten_abl: NULL pointer\n");
      exit(1);
   }

   /*return the cone head and the opposite value with their delays*/
   input=unflatten_each_oper(abl);

   abl=input->ABL;
   input->ABL=NULL;     /*protect from freeinput() */
   freeinput(input);
         
   put_arity_abl(abl);

   return abl;
}


