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
 * Tool    : LooN - format library cells
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <mut.h>
#include <abl.h>
#include <mlo.h>
#include <abe.h>
#include <mlu.h>
#include <abv.h>
#include "lon_lib_format.h"



/******************************************************************************/
/* return 1 if input is found in abl                                          */
/******************************************************************************/
static int is_input_used(char* input, chain_list* abl)
{
   if (!abl) return 0;
   
   if (ABL_ATOM(abl)) {
      char *value=ABL_ATOM_VALUE(abl);
      /*constant*/
      if (value==getablatomzero() || value==getablatomone() 
       || value==getablatomtristate() || value==getablatomdc()) return 0;
      if (value==input) return 1;
      return 0;  /*not found*/
   }
   
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      if (is_input_used(input,ABL_CAR(abl))) return 1;
   }

   return 0;   /*not found*/
}


/******************************************************************************/
/*             seek if all inputs are used in abl                             */
/******************************************************************************/
static int used_inputs(befig_list* befig, chain_list* abl1, chain_list* abl2)
{
   bepor_list* bepor;
   
   /*all inputs should be used*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      /*only input*/
      if (bepor->DIRECTION!=IN && bepor->DIRECTION!=INOUT
       && bepor->DIRECTION!=TRANSCV) continue;
      if (!isvdd(bepor->NAME) && !isvss(bepor->NAME)) {
         if (!is_input_used(bepor->NAME,abl1) 
          && !is_input_used(bepor->NAME,abl2)) {
            fprintf(stderr,"BEH: %s input unused\n",bepor->NAME);
            return 0;
         }
      }
   }
   
   return 1;/*all used*/
}


/******************************************************************************/
/*                         return 1 if format is ok                           */
/******************************************************************************/
extern int format_cell(befig_list* befig)
{
   /*internal signal forbidden*/
   if (befig->BEAUX) return 0;

   if (befig->BEREG) {
      /*only one register*/
      if (befig->BEREG->NEXT || befig->BEBUS || befig->BEBUX) return 0;
      /*only one condition*/
      if (!befig->BEREG->BIABL || befig->BEREG->BIABL->NEXT) return 0;
      /*one ouput*/
      if (!befig->BEOUT || befig->BEOUT->NEXT) return 0;
      /*  forbid logic on output */ 
      if (!ABL_ATOM(befig->BEOUT->ABL)) {
         chain_list* abl;
         abl=befig->BEOUT->ABL;
         if (ABL_OPER(abl)!=ABL_NOT) return 0;
         abl=ABL_CADR(abl);
         if (!ABL_ATOM(abl) || ABL_ATOM_VALUE(abl)!=befig->BEREG->NAME)return 0;
         /* output <= not reg;    ->  move not to register value*/
         freeablexpr(befig->BEOUT->ABL);
         befig->BEOUT->ABL=createablatom(befig->BEREG->NAME);
         befig->BEREG->BIABL->VALABL=simpablexpr(
                              createablnotexpr(befig->BEREG->BIABL->VALABL));
     }                                            
      return used_inputs(befig,befig->BEREG->BIABL->CNDABL,
                          befig->BEREG->BIABL->VALABL);
   }
   
   if (befig->BEBUS) {
      /*only one bus*/
      if (befig->BEBUS->NEXT || befig->BEREG || befig->BEBUX) return 0;
      /*only one condition*/
      if (!befig->BEBUS->BIABL || befig->BEBUS->BIABL->NEXT) return 0;
      /*one ouput: bebus*/
      if (befig->BEOUT) return 0;
      return used_inputs(befig,befig->BEBUS->BIABL->CNDABL,
                          befig->BEBUS->BIABL->VALABL);
   }

   if (befig->BEBUX) {
      /*only one internal bus*/
      if (befig->BEBUX->NEXT || befig->BEREG || befig->BEBUS) return 0;
      /*only one condition*/
      if (!befig->BEBUX->BIABL || befig->BEBUX->BIABL->NEXT) return 0;
      /*one ouput: beout*/
      if (!befig->BEOUT || befig->BEOUT->NEXT) return 0;
      /*  forbid logic on output */ 
      if (!ABL_ATOM(befig->BEOUT->ABL)) {
         chain_list* abl;
         abl=befig->BEOUT->ABL;
         if (ABL_OPER(abl)!=ABL_NOT) return 0;
         abl=ABL_CADR(abl);
         if (!ABL_ATOM(abl) || ABL_ATOM_VALUE(abl)!=befig->BEBUX->NAME)return 0;
         /* output <= not bus;  */
         freeablexpr(befig->BEOUT->ABL);
         befig->BEOUT->ABL=createablatom(befig->BEBUX->NAME);
         befig->BEBUX->BIABL->VALABL=simpablexpr(
                                 createablnotexpr(befig->BEBUX->BIABL->VALABL));
      }                                            
      /*normalize befig  -> move bebux to bebus */
      befig->BEBUS=beh_addbebus(befig->BEBUS,befig->BEOUT->NAME,
                 befig->BEBUX->BIABL,NULL,'M'/*mux_bit instead of wor_bit'W'*/);
      befig->BEBUX->BIABL=NULL;     /*protect from beh_frebebux()*/
      beh_frebebux(befig->BEBUX);
      freeablexpr(befig->BEOUT->ABL);
      beh_frebeout(befig->BEOUT);
      befig->BEBUX=NULL;
      befig->BEOUT=NULL;
      return used_inputs(befig,befig->BEBUS->BIABL->CNDABL,
                          befig->BEBUS->BIABL->VALABL);
   }

   if (befig->BEOUT) {
      /*one ouput: beout*/
      if (!befig->BEOUT || befig->BEOUT->NEXT) return 0;
      return used_inputs(befig,befig->BEOUT->ABL,NULL);
   }

   /*illegal befig*/
   return 0;
}


/******************************************************************************/
/*put the locon of the lofig associated to the befig                          */
/*in the same order than bepor                                                */
/* return 0 if no error                                                       */
/******************************************************************************/
extern int sortbepor(befig_list* befig)
{
   lofig_list* lofig;
   bepor_list* bepor;
   locon_list* locon, *pred;

   lofig=getlofig(befig->NAME,'P');
   
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {

      pred=NULL;
      for (locon=lofig->LOCON; locon; locon=locon->NEXT) {
         if (bepor->NAME==locon->NAME) {
            /*disconnect*/
            if (pred) pred->NEXT=locon->NEXT;
            else lofig->LOCON=locon->NEXT;
            /*reconnect*/
            locon->NEXT=lofig->LOCON;
            lofig->LOCON=locon;
            break;
         }
         pred=locon;
      }
      
      if (!locon) {
         fprintf(stderr,
         "BEH: mismatch with %s.al for cell %s.vbe on port %s\n",
         befig->NAME, befig->NAME, bepor->NAME);
         return 1;
      }
   }

   lofig->LOCON=(locon_list*) reverse((chain_list*) lofig->LOCON);
   
   return 0;
}



