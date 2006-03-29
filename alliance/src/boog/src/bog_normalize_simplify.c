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
 * Tool    : BooG - simplify befig expressions
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_simplify.h"




/***************************************************************************/
/*    build binary negativ operators                                       */
/***************************************************************************/
static chain_list* unflatnegexpr(chain_list* abl)
{
   chain_list *chain;
   int count=0;

   if (ABL_ATOM(abl)) return abl;
   
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      ABL_CAR_L(chain)=unflatnegexpr(ABL_CAR(chain));
      count++;
   }   
   
   /*already binary  NXOR not accepted by parser/driver*/
   if (ABL_OPER(abl)!=ABL_NXOR && count<=2) return abl;
   
   switch(ABL_OPER(abl)) {
   case ABL_NAND: 
      ABL_OPER_L(abl)=ABL_AND;
      return createablnotexpr(abl);
   case ABL_NOR: 
      ABL_OPER_L(abl)=ABL_OR;
      return createablnotexpr(abl);
   case ABL_NXOR: 
      ABL_OPER_L(abl)=ABL_XOR;
      return createablnotexpr(abl);
   default: return abl;         
   }
}


/******************************************************************************/
/* put binary oper to befig to be understood by parser and PROOF (CAO util.)  */
/* for debugging mode                                                         */
/******************************************************************************/
extern void binary_oper(befig_list *befig)
{
   beout_list* beout;
   bebus_list* bebus;
   bereg_list* bereg;
   bebux_list* bebux;
   beaux_list* beaux;
   biabl_list* biabl;

   for (beout=befig->BEOUT; beout; beout=beout->NEXT) 
      beout->ABL=unflatnegexpr(beout->ABL);

   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) 
      beaux->ABL=unflatnegexpr(beaux->ABL);

   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->CNDABL=unflatnegexpr(biabl->CNDABL); 
         biabl->VALABL=unflatnegexpr(biabl->VALABL); 
      }   
   }

   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->CNDABL=unflatnegexpr(biabl->CNDABL); 
         biabl->VALABL=unflatnegexpr(biabl->VALABL); 
      }
   }

   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->CNDABL=unflatnegexpr(biabl->CNDABL); 
         biabl->VALABL=unflatnegexpr(biabl->VALABL); 
      }
   }      

}


/***************************************************************************/
/*    merge operator                                                       */
/***************************************************************************/
static void flattenablexpr(chain_list* abl)
{
   chain_list* leaf, *chain, *last, *pred;

   if (ABL_ATOM(abl)) return;
   
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      flattenablexpr(ABL_CAR(chain));
   }   
   pred=abl;
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      leaf=ABL_CAR(chain);
      if (ABL_ATOM(leaf)) {pred=chain; continue;}

      switch(ABL_OPER(abl)) {
         case ABL_NAND: case ABL_AND: 
            if (ABL_OPER(leaf)==ABL_AND) {
               /*seek the last*/
               for (last=ABL_CDR(leaf); ABL_CDR(last); last=ABL_CDR(last)) ;
               /*free connections*/
               ABL_CDR(pred)=ABL_CDR(chain);
               ABL_CDR(chain)=NULL;
               freechain(chain);
               chain=pred;
               /*insert leaves in abl*/
               ABL_CDR(last)=ABL_CDR(abl);
               ABL_CDR(abl)=ABL_CDR(leaf);
               /*free connections*/
               ABL_CDR(leaf)=NULL;
               freechain(leaf);     
            }
            break;
         case ABL_NOR: case ABL_OR: 
            if (ABL_OPER(leaf)==ABL_OR) {
               /*seek the last*/
               for (last=ABL_CDR(leaf); ABL_CDR(last); last=ABL_CDR(last)) ;
               /*free connections*/
               ABL_CDR(pred)=ABL_CDR(chain);
               ABL_CDR(chain)=NULL;
               freechain(chain);
               chain=pred;
               /*insert leaves in abl*/
               ABL_CDR(last)=ABL_CDR(abl);
               ABL_CDR(abl)=ABL_CDR(leaf);
               /*free connections*/
               ABL_CDR(leaf)=NULL;
               freechain(leaf);     
            }
            break;
         case ABL_NXOR: case ABL_XOR: 
            if (ABL_OPER(leaf)==ABL_XOR) {
               /*seek the last*/
               for (last=ABL_CDR(leaf); ABL_CDR(last); last=ABL_CDR(last)) ;
               /*free connections*/
               ABL_CDR(pred)=ABL_CDR(chain);
               ABL_CDR(chain)=NULL;
               freechain(chain);
               chain=pred;
               /*insert leaves in abl*/
               ABL_CDR(last)=ABL_CDR(abl);
               ABL_CDR(abl)=ABL_CDR(leaf);
               /*free connections*/
               ABL_CDR(leaf)=NULL;
               freechain(leaf);     
            }
            break;
         case ABL_STABLE: case ABL_NOT: break;
         default:
            fprintf(stderr,"unflattenexprabl: operator %ld unknown\n",
            ABL_OPER(abl));
            exit(1);
      }
      pred=chain;
   }
}


/***************************************************************************/
/*                   Simplify the abl of befig                             */
/***************************************************************************/
extern void simplify_expr(befig_list* befig)
{
   bebus_list* bebus;
   beaux_list* beaux;
   beout_list* beout;
   bereg_list* bereg;
   bebux_list* bebux;
   biabl_list* biabl;


   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      beout->ABL=simpablexpr(beout->ABL); 
      flattenablexpr(beout->ABL);
   }
   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      beaux->ABL=simpablexpr(beaux->ABL); 
      flattenablexpr(beaux->ABL);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->CNDABL=simpablexpr(biabl->CNDABL); 
         biabl->VALABL=simpablexpr(biabl->VALABL); 
         flattenablexpr(biabl->CNDABL);
         flattenablexpr(biabl->VALABL);
      }
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->CNDABL=simpablexpr(biabl->CNDABL); 
         biabl->VALABL=simpablexpr(biabl->VALABL); 
         flattenablexpr(biabl->CNDABL);
         flattenablexpr(biabl->VALABL);
      }
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->CNDABL=simpablexpr(biabl->CNDABL); 
         biabl->VALABL=simpablexpr(biabl->VALABL); 
         flattenablexpr(biabl->CNDABL);
         flattenablexpr(biabl->VALABL);
      }
   }

}

