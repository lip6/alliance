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
 * Tool    : BooG - format library cells
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
#include "bog_lib_format.h"



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
static int used_inputs(befig_list* befig)
{
   bepor_list* bepor;
   bereg_list *bereg;
   beaux_list *beaux;
   bebus_list *bebus;
   bebux_list *bebux;
   beout_list *beout;
   biabl_list *biabl;
  
   
   /*all inputs should be used*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      /*only input*/
      if (bepor->DIRECTION!=IN && bepor->DIRECTION!=INOUT
       && bepor->DIRECTION!=TRANSCV) continue;
      if (!isvdd(bepor->NAME) && !isvss(bepor->NAME)) {
      
         for ( beout = befig->BEOUT; beout; beout = beout->NEXT )
         {
            if ( is_input_used(bepor->NAME, beout->ABL) ) break;
         }
         if ( beout ) continue;
         
         for ( beaux = befig->BEAUX; beaux; beaux = beaux->NEXT )
         {
            if ( is_input_used(bepor->NAME, beaux->ABL) ) break;
         }
         if ( beaux ) continue;
         
         for ( bebux = befig->BEBUX; bebux; bebux = bebux->NEXT )
         {
            for ( biabl = bebux->BIABL; biabl; biabl = biabl->NEXT )
            {
               if ( is_input_used(bepor->NAME, biabl->CNDABL) ) break;
               if ( is_input_used(bepor->NAME, biabl->VALABL) ) break;
            }
            if ( biabl ) break;
         }
         if ( bebux ) continue;
         
         for ( bereg = befig->BEREG; bereg; bereg = bereg->NEXT )
         {
            for ( biabl = bereg->BIABL; biabl; biabl = biabl->NEXT )
            {
               if ( is_input_used(bepor->NAME, biabl->CNDABL) ) break;
               if ( is_input_used(bepor->NAME, biabl->VALABL) ) break;
            }
            if ( biabl ) break;
         }
         if ( bereg ) continue;
         
         for ( bebus = befig->BEBUS; bebus; bebus = bebus->NEXT )
         {
            for ( biabl = bebus->BIABL; biabl; biabl = biabl->NEXT )
            {
               if ( is_input_used(bepor->NAME, biabl->CNDABL) ) break;
               if ( is_input_used(bepor->NAME, biabl->VALABL) ) break;
            }
            if ( biabl ) break;
         }
         if ( bebus ) continue;
         
         fprintf(stderr,"BEH: %s input unused\n",bepor->NAME);
         return 0;

      }
   }
   
   return 1;/*all used*/
}


/******************************************************************************/
/******************************************************************************/
static chain_list* format_subst_abl( chain_list* abl, beaux_list *BeauxSubst )
{
   chain_list *chain;

   if (ABL_ATOM(abl)) {
      char *value=ABL_ATOM_VALUE(abl);
      if (value == BeauxSubst->NAME) return dupablexpr( BeauxSubst->ABL );
      return abl;
   }
   
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      ABL_CAR(chain) = format_subst_abl( ABL_CAR( chain ), BeauxSubst );
   }

   return abl;  
}

/******************************************************************************/
/******************************************************************************/
static void format_subst_befig( befig_list *befig, beaux_list *BeauxSubst )
{
   bereg_list *bereg;
   beaux_list *beaux;
   bebus_list *bebus;
   bebux_list *bebux;
   beout_list *beout;
   biabl_list *biabl;
  
   for ( beout = befig->BEOUT; beout; beout = beout->NEXT )
   {
      beout->ABL = format_subst_abl( beout->ABL, BeauxSubst );
   }
   for ( beaux = befig->BEAUX; beaux; beaux = beaux->NEXT )
   {
      if ( beaux == BeauxSubst ) continue;
      beaux->ABL = format_subst_abl( beaux->ABL, BeauxSubst );
   }
   for ( bebux = befig->BEBUX; bebux; bebux = bebux->NEXT )
   {
      for ( biabl = bebux->BIABL; biabl; biabl = biabl->NEXT )
      {
         biabl->CNDABL = format_subst_abl( biabl->CNDABL, BeauxSubst );
         biabl->VALABL = format_subst_abl( biabl->VALABL, BeauxSubst );
      }
   }
   for ( bereg = befig->BEREG; bereg; bereg = bereg->NEXT )
   {
      for ( biabl = bereg->BIABL; biabl; biabl = biabl->NEXT )
      {
         biabl->CNDABL = format_subst_abl( biabl->CNDABL, BeauxSubst );
         biabl->VALABL = format_subst_abl( biabl->VALABL, BeauxSubst );
      }
   }
   for ( bebus = befig->BEBUS; bebus; bebus = bebus->NEXT )
   {
      for ( biabl = bebus->BIABL; biabl; biabl = biabl->NEXT )
      {
         biabl->CNDABL = format_subst_abl( biabl->CNDABL, BeauxSubst );
         biabl->VALABL = format_subst_abl( biabl->VALABL, BeauxSubst );
      }
   }
}



/******************************************************************************/
/*                         return 1 if format is ok                           */
/******************************************************************************/
extern int format_cell(befig_list* befig)
{
   biabl_list* biabl;
   beaux_list* beaux;

   /*internal signal forbidden*/
   for ( beaux = befig->BEAUX; beaux; beaux = beaux->NEXT )
   {
      format_subst_befig( befig, beaux );
   }
   befig->BEAUX = NULL;


   if (befig->BEREG) {
      /*only one register*/
      if (befig->BEREG->NEXT || befig->BEBUS || befig->BEBUX) return 0;
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
         for (biabl=befig->BEREG->BIABL; biabl; biabl=biabl->NEXT) {
            biabl->VALABL=simpablexpr(createablnotexpr(biabl->VALABL));
         }   
     }                                            
   }
   
   if (befig->BEBUS) {
      /*only one bus*/
      if (befig->BEBUS->NEXT || befig->BEREG || befig->BEBUX) return 0;
      /*one ouput: bebus*/
      if (befig->BEOUT) return 0;
   }

   if (befig->BEBUX) {
      /*only one internal bus*/
      if (befig->BEBUX->NEXT || befig->BEREG || befig->BEBUS) return 0;
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
         for (biabl=befig->BEBUX->BIABL; biabl; biabl=biabl->NEXT) {
            biabl->VALABL=simpablexpr(createablnotexpr(biabl->VALABL));
         }   
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
   }

   if (befig->BEOUT) {
      /*one ouput: beout*/
      if (befig->BEOUT->NEXT || befig->BEBUS) return 0;
   }

   return used_inputs(befig);
   
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



