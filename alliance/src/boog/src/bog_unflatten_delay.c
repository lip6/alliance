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
 * Tool    : BooG - unflatten with optimized delay
 * Date    : 2000
 * Author  : Francois Donnet
 */




#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_ARITY.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_lib_complete.h"
#include "bog_unflatten_utils.h"
#include "bog_unflatten_oper.h"
#include "bog_unflatten_delay.h"




/***************************************************************************/
/*the cost is the place of the new output in the rest of entry            */
/*====> example:  operator is AND_3=1.1 ps                                 */
/*and entry are  a=3.0(used)  b=4.2(used)  c=6.0(used) d=7.0 e=7.2 f=9 ps */
/*====> X=7.1  cost(X)=2   because X arrives after d but before e and f    */
/*====> X=9.5  no solution given by this algorithm                         */
/***************************************************************************/
static note_list* eval_note(note_list* all_note, cell_list* cell, input_list *entry, int size, input_list* last_input)
{
   port_list* port;
   note_list* note;
   float latest=0, delay, R=0;
   int source_positiv, dest_positiv, cost;
   chain_list* portchain;
  
   /*operator too big or match completely(this algorithm do not work)*/
   if (size<=ABL_ARITY(cell->ABL)) return all_note;

   source_positiv=is_source_positiv(ABL_OPER(cell->ABL));
   dest_positiv=is_dest_positiv(ABL_OPER(cell->ABL),ABL_ARITY(cell->ABL));
   /*operator no matches*/
   if (source_positiv==-1 || dest_positiv==-1) return all_note;


   /*search if there is already a couple of this arity*/
   for (note=all_note; note; note=note->NEXT) {
      if (note->ARITY==ABL_ARITY(cell->ABL)) break;
   }
  
   
   /*eval delay*/
   for (portchain=ABL_CDR(cell->ABL); portchain; portchain=portchain->NEXT) {
      port=(port_list*) ABL_ATOM_VALUE(ABL_CAR(portchain));
      delay=port->T ;
      delay+=source_positiv?entry->DELAY:entry->NEG_DELAY;      
      /*sortance=1*/
      delay+= port->C * (source_positiv?entry->R:entry->NEG_R); 
      if (delay>latest) {latest=delay; R=port->R;}
      
      /*if worse than last record, skip it*/
      if (dest_positiv) {
         if (latest>=last_input->DELAY) return all_note;
      }
      else if (latest>=last_input->NEG_DELAY) return all_note;

      if (note) {
         /*if worse than last record, skip it*/
         if (dest_positiv) {
            if (latest>=note->DELAY) return all_note;
         }
         else if (latest>=note->NEG_DELAY) return all_note;
      }
      
      entry=entry->NEXT;
   }


   /*eval cost -> it is the place in the rest of the entry*/
   for (cost=1; entry; entry=entry->NEXT) {
      delay=dest_positiv?entry->DELAY:entry->NEG_DELAY;
      if (latest<=delay) break;
      cost++;
   }

   /*if after all entry algorithm don't work*/
   if (!entry) return all_note;

   /*create a new couple if doesn't exist*/
   if (!note) {
      note=newnote();
      note->NEXT=all_note;
      all_note=note;
      note->ARITY=ABL_ARITY(cell->ABL);
   }
   
   /*take new solution*/
   if (dest_positiv) {
      note->DELAY=latest;
      note->R=R;
      note->COST=cost;
      note->CELL=cell;
   }
   else {
      note->NEG_DELAY=latest;
      note->NEG_R=R;
      note->NEG_COST=cost;
      note->NEG_CELL=cell;
   }   
   
   return all_note;
}


/***************************************************************************/
/*                     unflatten to improve speed                          */
/* try to reduce the number of entries without changing the latest entry   */
/*a cell inserted must produce an output delay shortest than latest entry  */
/* return the new list of entry after insertion                           */
/* if no cell can satisfy condition result the entry list unchanged        */
/***************************************************************************/
extern input_list* unflatten_delay(input_list *entry, int entry_number, input_list* last_entry)
{
   input_list *pred = NULL, *input, *new_input;
   chain_list* abl;
   cell_list* cell;
   note_list* note, *all_note=NULL, *best_note;
   int cell_source_positiv, neg_cell_source_positiv;   /*flag*/

   if (!entry) {
      fprintf(stderr,"unflatten_delay: no input\n");
      exit(1);
   }
   
   /*test all the simple logic library*/
   for (cell=getcell_oper_lib(); cell; cell=cell->NEXT) {
      all_note=eval_note(all_note, cell, entry, entry_number, last_entry);
   }
      
   /*no result pattern, impossible to map, signal it to caller function*/
   if (!all_note) return entry;

   /*take the earliest or the biggest on average results*/
   best_note=all_note;
   best_note->AVERAGE_COST=(best_note->COST+best_note->NEG_COST)/2;
   for (note=all_note->NEXT; note; note=note->NEXT) {
      note->AVERAGE_COST=(note->COST+note->NEG_COST)/2;
      if (note->AVERAGE_COST<best_note->AVERAGE_COST) best_note=note;
      else if (note->AVERAGE_COST==best_note->AVERAGE_COST 
         && note->ARITY>best_note->ARITY) best_note=note;
      else if (note->AVERAGE_COST==best_note->AVERAGE_COST 
         && note->ARITY==best_note->ARITY
         && note->DELAY+note->NEG_DELAY<best_note->DELAY+best_note->NEG_DELAY)
            best_note=note;
   }

   /*only one cell respect condtions, we need its dual cell*/
   if (!best_note->CELL || !best_note->NEG_CELL) {
      /*algorithm don't know how to unflat*/
      freenote(all_note);
      return entry;
   }   

   /*build new input*/
   new_input=newinput();
   new_input->DELAY=best_note->DELAY;
   new_input->NEG_DELAY=best_note->NEG_DELAY;
   new_input->R=best_note->R;
   new_input->NEG_R=best_note->NEG_R;
   new_input->ABL=createabloper(ABL_OPER(best_note->CELL->ABL));
   new_input->NEG_ABL=createabloper(ABL_OPER(best_note->NEG_CELL->ABL));
   cell_source_positiv=is_source_positiv(ABL_OPER(best_note->CELL->ABL));
   neg_cell_source_positiv=is_source_positiv(
                                         ABL_OPER(best_note->NEG_CELL->ABL));
   /*put operands*/
   for (input=entry; input; input=input->NEXT) {
      if (best_note->ARITY==0) break;
      best_note->ARITY--;
      /*positiv*/
      if (cell_source_positiv) {
         abl=input->ABL;
         input->ABL=NULL; /* protect from freeinput() */
      }
      else {   
         abl=input->NEG_ABL;
         input->NEG_ABL=NULL; /* protect from freeinput() */
      }
      ABL_CDR(new_input->ABL)=addchain(ABL_CDR(new_input->ABL),abl);
      /*opposite*/
      if (neg_cell_source_positiv) {
         if (input->ABL) abl=input->ABL;
         else abl=dupablexpr(abl);     /*already used above*/
         input->ABL=NULL; /* protect from freeinput() */
      }
      else {   
         if (input->NEG_ABL) abl=input->NEG_ABL;
         else abl=dupablexpr(abl);     /*already used above*/
         input->NEG_ABL=NULL; /* protect from freeinput() */
      }
      ABL_CDR(new_input->NEG_ABL)=addchain(ABL_CDR(new_input->NEG_ABL),abl);

      pred=input;
   }
   
   pred->NEXT=NULL;
   freeinput(entry);
   freenote(all_note);
   
   return sort_input(input,new_input);      /*return a sort list*/
}



