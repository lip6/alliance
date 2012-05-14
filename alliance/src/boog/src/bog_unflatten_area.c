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
 * Tool    : BooG - unflatten with optimize in area
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_ARITY.h"
#include "bog_normalize_message.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_lib_complete.h"
#include "bog_unflatten_utils.h"
#include "bog_unflatten_oper.h"
#include "bog_unflatten_area.h"




/***************************************************************************/
/* cost is the number of inputs left if we map all entries with this cell  */
/* for first level of tree                                                 */
/*====>  cost = size/arity + rest                                          */
/***************************************************************************/
static note_list* eval_note(note_list* all_note, cell_list* cell, input_list *entry, int size)
{
   port_list* port;
   note_list* note;
   float latest=0, delay, R=0;
   int source_positiv, dest_positiv, cost;
   chain_list* portchain;
  
   /*operator too big*/
   if (size<ABL_ARITY(cell->ABL)) return all_note;

   source_positiv=is_source_positiv(ABL_OPER(cell->ABL));
   dest_positiv=is_dest_positiv(ABL_OPER(cell->ABL),ABL_ARITY(cell->ABL));
   /*operator no matches*/
   if (source_positiv==-1 || dest_positiv==-1) return all_note;


   /*eval cost */
   cost=size/ABL_ARITY(cell->ABL)+size%ABL_ARITY(cell->ABL);

   /*search if there is already a couple of this arity*/
   for (note=all_note; note; note=note->NEXT) {
      if (note->ARITY==ABL_ARITY(cell->ABL)) {
         /*if worse than last record, skip it*/
         if (dest_positiv) {
            if (cost>note->COST) return all_note;
         }
         else if (cost>note->NEG_COST) return all_note;
         break;
      }   
   }
   
   
   /*eval delay*/
   for (portchain=ABL_CDR(cell->ABL); portchain; portchain=portchain->NEXT) {
      port=(port_list*) ABL_ATOM_VALUE(ABL_CAR(portchain));
      delay=port->T;
      delay+=source_positiv?entry->DELAY:entry->NEG_DELAY;      
      /*sortance=1*/
      delay+= port->C * (source_positiv?entry->R:entry->NEG_R); 
      if (delay>latest) {latest=delay; R=port->R;}

      if (note) {
         /*if worse than last record, skip it*/
         if (dest_positiv) {
            if (latest>=note->DELAY) return all_note;
         }
         else if (latest>=note->NEG_DELAY) return all_note;
      }
      
      entry=entry->NEXT;
   }

      
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
/*                     unflatten to improve area                           */
/* insert a cell to produce the most equilibrated tree                     */
/* return the new list of entry after insertion                           */
/* if no cell can satisfy condition result the entry list unchanged       */
/***************************************************************************/
extern input_list* unflatten_area(input_list *entry, int entry_number)
{
   input_list *pred = NULL, *input, *new_input;
   chain_list* abl;
   cell_list* cell;
   note_list* note, *all_note=NULL, *best_note;
   int cell_source_positiv, neg_cell_source_positiv;   /*flag*/

   if (!entry) {
      fprintf(stderr,"unflatten_area: no input\n");
      exit(1);
   }
   
   /*test all the simple logic library*/
   for (cell=getcell_oper_lib(); cell; cell=cell->NEXT) {
      all_note=eval_note(all_note, cell, entry, entry_number);
   }
      
   /*no result pattern, impossible to map, signal it to caller function*/
   if (!all_note) return entry;

   /*take the best for a positiv and negativ results*/
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

   /*one of those cells are missing, impossible to unflat*/
   if (!best_note->CELL || !best_note->NEG_CELL) {
      fprintf(stderr,
      "unflatten_area: opposite cell of %s is missing (%d inputs)\n",
      best_note->CELL?best_note->CELL->NAME:best_note->NEG_CELL->NAME,
      entry_number);
      exit(1);
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
                                         ABL_OPER(best_note->NEG_CELL->ABL));   /*put operands*/
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



