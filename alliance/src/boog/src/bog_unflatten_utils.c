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


#ifdef AUTO_HAS_VALUES_H
# include <values.h>
#else
# include <float.h>
# ifndef MAXFLOAT
#  define MAXFLOAT FLT_MAX
# endif
#endif

#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_lax_param.h"
#include "bog_lib_utils.h"
#include "bog_unflatten_utils.h"

#define BLOCK 512

/*heaps of structure*/
static input_list* HEAD;
static note_list*  HEAD_NOTE;



/***************************************************************************/
/*                 give a new element from the heap                        */
/***************************************************************************/
extern input_list* newinput()
{
   input_list* new;
   int i;

   if (!HEAD) {
      HEAD=mbkalloc(BLOCK*sizeof(input_list));
      new=HEAD;
      for (i = 1; i < BLOCK; i++) {
         new->NEXT = new + 1;
         new++;
      }
      new->NEXT = NULL;
   }

   new=HEAD;
   HEAD=HEAD->NEXT;
   new->NEXT=NULL;
   new->ABL=NULL;
   new->NEG_ABL=NULL;
   new->R=DEFAULT_CAPACITANCE;
   new->NEG_R=DEFAULT_CAPACITANCE;
   new->DELAY=MAXFLOAT;             /*very important value*/
   new->NEG_DELAY=MAXFLOAT;         /*very important value*/
   return new;
}


/***************************************************************************/
/*    free recursively a list of inputs and also their abl                 */
/***************************************************************************/
extern void freeinput(input_list* head)
{
   if (!head) return;

   freeinput(head->NEXT);
   if (head->ABL) freeablexpr(head->ABL);
   if (head->NEG_ABL) freeablexpr(head->NEG_ABL);
   
   /*put back to the heap*/
   head->NEXT=HEAD;
   HEAD=head;
}


/***************************************************************************/
/* create a new structure and copy all the fields of input                 */
/***************************************************************************/
extern input_list* dupinput(input_list* input)
{
   input_list* new;
   new=newinput();
   new->NEXT=input->NEXT;
   new->ABL=input->ABL;
   new->NEG_ABL=input->NEG_ABL;
   new->DELAY=input->DELAY;
   new->NEG_DELAY=input->NEG_DELAY;
   return new;
}


/***************************************************************************/
/*               insert new in the list of inputs                          */
/* sorted by the average of delay and its inverted value delay             */
/***************************************************************************/
extern input_list* sort_input(input_list* inputs, input_list* new_input)
{
   float average;
   input_list* sort, *pred;
  
   if (!new_input) {
      fprintf(stderr,"sort_input: no new input to insert\n");
      exit(1);
   }
   
   average=(new_input->DELAY+new_input->NEG_DELAY)/2;

   /*find place of new input*/
   pred=NULL;
   for (sort=inputs; sort; sort=sort->NEXT) {
      if (average<=(sort->DELAY+sort->NEG_DELAY)/2) break;
      pred=sort;
   }

   new_input->NEXT=sort;
   if (pred) {
      pred->NEXT=new_input;
      return inputs;
   }      
   else return new_input; 
      
}


/***************************************************************************/
/*                 give a new element from the heap                        */
/***************************************************************************/
extern note_list* newnote()
{
   note_list* new;
   int i;

   if (!HEAD_NOTE) {
      HEAD_NOTE=mbkalloc(BLOCK*sizeof(note_list));
      new=HEAD_NOTE;
      for (i = 1; i < BLOCK; i++) {
         new->NEXT = new + 1;
         new++;
      }
      new->NEXT = NULL;
   }

   new=HEAD_NOTE;
   HEAD_NOTE=HEAD_NOTE->NEXT;
   new->NEXT=NULL;
   new->CELL=NULL;
   new->NEG_CELL=NULL;
   new->COST=MAXFLOAT;
   new->NEG_COST=MAXFLOAT;
   new->AVERAGE_COST=MAXFLOAT;
   new->DELAY=MAXFLOAT;          /*very important value, do not change!*/
   new->NEG_DELAY=MAXFLOAT;      /*do not change*/
   new->R=DEFAULT_CAPACITANCE;
   new->NEG_R=DEFAULT_CAPACITANCE;
   new->ARITY=0;
   return new;
}


/***************************************************************************/
/*    free recursively a list of notes                                     */
/***************************************************************************/
extern void freenote(note_list* head)
{
   if (!head) return;

   freenote(head->NEXT);
   /*put back to the heap*/
   head->NEXT=HEAD_NOTE;
   HEAD_NOTE=head;
}



