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
 * Tool    : BooG - unflatten an operator
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_lax_param.h"
#include "bog_normalize_ARITY.h"
#include "bog_normalize_message.h"
#include "bog_lib_utils.h"
#include "bog_lib_complete.h"
#include "bog_signal_utils.h"
#include "bog_map_pattern.h"
#include "bog_unflatten_utils.h"
#include "bog_unflatten_delay.h"
#include "bog_unflatten_area.h"
#include "bog_unflatten_oper.h"

static int OPER;  /*oper we have to reduce*/
static int SIZE;  /*number of inputs of this oper*/


/**************************************************************************/
/* return -1 if oper not match with OPER                                  */
/*         0 if oper gives inverted destination                           */
/*         1 if oper gives positiv destination                            */
/**************************************************************************/
extern int is_dest_positiv(int oper, int arity)
{
   /*WARNING it's not so easy to understand...*/
   /*NAND, NOR become and AND, OR  if they aren't last*/

   switch (OPER) {
   case ABL_NAND:
      if (arity==SIZE)      /*if not last operator we need positive operator*/
      switch (oper) {
      case ABL_OR:  case ABL_NAND: return 1;
      case ABL_AND: case ABL_NOR:  return 0;
      default: return -1;
      }
   case ABL_AND:            /*ABL_NAND is like ABL_AND if not last*/
      switch (oper) {
      case ABL_AND: case ABL_NOR:  return 1;
      case ABL_OR:  case ABL_NAND: return 0;
      default: return -1;
      }
   case ABL_NOR:
      if (arity==SIZE)      /*if not last operator we need positive operator*/
      switch (oper) {
      case ABL_AND: case ABL_NOR:  return 1;
      case ABL_OR:  case ABL_NAND: return 0;
      default: return -1;
      }
   case ABL_OR:            /*ABL_NOR is like ABL_OR if not last*/
      switch (oper) {
      case ABL_OR:  case ABL_NAND: return 1;
      case ABL_AND: case ABL_NOR:  return 0;
      default: return -1;
      }
   case ABL_NXOR:
      if (arity==SIZE)      /*if not last operator we need positive operator*/
      switch (oper) {
      case ABL_XOR:  return 0;
      case ABL_NXOR: return 1;
      default: return -1;
      }
   case ABL_XOR:
      switch (oper) {
      case ABL_XOR:  return 1;
      case ABL_NXOR: return 0;
      default: return -1;
      }
   case ABL_STABLE: case ABL_NOT: return -1;
   default:
      fprintf(stderr,"is_dest_positiv: %d operator unknown\n",OPER);
      exit(1);
   }
}


/**************************************************************************/
/* return -1 if oper not match with OPER                                  */
/*         0 if oper needs inverted source                                */
/*         1 if oper needs positiv source                                 */
/**************************************************************************/
extern int is_source_positiv(int oper)
{
   switch (OPER) {
   case ABL_AND: case ABL_NAND:
      switch (oper) {
      case ABL_AND: case ABL_NAND:  return 1;
      case ABL_OR:  case ABL_NOR: return 0;
      default: return -1;
      }
   case ABL_OR: case ABL_NOR:
      switch (oper) {
      case ABL_OR:  case ABL_NOR: return 1;
      case ABL_AND: case ABL_NAND:  return 0;
      default: return -1;
      }
   case ABL_XOR: 
      switch (oper) {
      case ABL_XOR:  return 1;
      case ABL_NXOR: return 1;
      default: return -1;
      }
   case ABL_NXOR:
      switch (oper) {
      case ABL_XOR:  return 1;
      case ABL_NXOR: return 1;
      default: return -1;
      }
   case ABL_STABLE: case ABL_NOT: return -1;
   default:
      fprintf(stderr,"is_source_positiv: %d operator unknown\n",OPER);
      exit(1);
   }
}


/***************************************************************************/
/* return the unflat output and its inverted value                         */
/***************************************************************************/
extern input_list* unflatten_oper(int oper, input_list* entry)
{
   input_list* pred, *last = NULL, *input;

   if (!entry) {
      fprintf(stderr,"unflatten_oper: NULL pointer\n");
      exit(1);
   }

   /*no problem of arity for not*/
   if (oper==ABL_NOT) {
      chain_list* abl;
      float delay,R;
      
      R=entry->R;
      delay=entry->DELAY;
      abl=entry->ABL;
      entry->R=entry->NEG_R;
      entry->DELAY=entry->NEG_DELAY;
      entry->ABL=entry->NEG_ABL;
      entry->NEG_R=R;
      entry->NEG_DELAY=delay;
      entry->NEG_ABL=abl;
      return entry;
   }

   /*solve problem of arity*/
   OPER=oper;
   is_source_positiv(oper);/*verify validity of operator*/
   
   /*we want only one input*/
   while (entry->NEXT) {
   
      /*number of entry*/
      SIZE=0;
      for (input=entry; input; input=input->NEXT) {
         SIZE++;
         last=input;
      }   
   
      pred=entry;
      /*optimize in area or in delay*/
      if (getoptimlax()>OPTIM_DELAY0) {
         entry=unflatten_delay(entry, SIZE, last);
         if (!entry) {
            fprintf(stderr,"unflatten_oper: unflatten_delay() return NULL\n");
            exit(1);      
         }
         if (entry!=pred) continue;
         /*if no result, try another algorithm*/
      }
      
      /*optimize in area*/
      entry=unflatten_area(entry, SIZE);
      if (!entry) {
         fprintf(stderr,"unflatten_oper: unflatten_area() return NULL\n");
         exit(1);      
      }
      if (entry==pred) {
         fprintf(stderr,
                 "unflatten_oper: Impossible to map oper %d with arity %d\n",
                 OPER, SIZE);
         exit(1);
      }
   
   }

   return entry;
}

