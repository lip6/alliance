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
 * Tool    : BooG - tree permutations
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_ARITY.h"
#include "bog_lib_matching.h"
#include "bog_lib_specifications.h"
#include "bog_lib_utils.h"
#include "bog_lib_permute.h"



/******************************************************************************/
/* duplicate a list                                                           */
/******************************************************************************/
extern chain_list* dupchain(chain_list* pthead)
{
    chain_list *chain;
    chain_list *pred  = NULL;
    chain_list *first = NULL;

    if ( !pthead ) return NULL;
    
    first = addchain( NULL, pthead->DATA );
    pred  = first;

    for ( chain = pthead->NEXT; chain; chain = chain->NEXT )
    {
        ABL_CDR( pred ) = addchain( NULL, chain->DATA );
        pred = ABL_CDR( pred );
    }

    return first;
}


/******************************************************************************/
/* duplicate a list of abl and its abl                                        */
/******************************************************************************/
static chain_list* dupablchain(chain_list* ablchain)
{
   chain_list* chain,*res=NULL;

   for (chain=ablchain;chain;chain=chain->NEXT) {
      res=addchain(res,dupablexpr(chain->DATA));
   }
   
   return res;
}


/******************************************************************************/
/*return a list of list with elem inserted in all positions of elem_chain     */
/*result is put at the top of head_positions                                  */
/******************************************************************************/
extern chain_list *all_positions(chain_list* head_positions, chain_list* elem_chain, void* elem)
{
  chain_list *chain, *pred=NULL;
  chain_list *other_positions;
  
   if (!elem_chain)
   {
      elem_chain=addchain(NULL,elem);
      head_positions=addchain(head_positions, elem_chain);
      return head_positions;
   }
   
   /*put in other places*/
   other_positions=all_positions(NULL, elem_chain->NEXT, elem);
   /*complete the result*/
   for (chain=other_positions; chain; chain=chain->NEXT) {
      chain->DATA=addchain(chain->DATA, elem_chain->DATA);
      pred=chain;
   }

   /*put at the top*/
   pred->NEXT=head_positions;
   head_positions=other_positions;

   /*put in first place*/
   elem_chain=addchain(dupchain(elem_chain),elem);
   head_positions=addchain(head_positions, elem_chain);

   return head_positions;
}


/******************************************************************************/
/* return a list of list of elements                                          */
/*which compose all the possible different orders of elem_chain               */
/* elements aren't duplicated                                                 */
/* result is put at the top of head_swap                                      */
/******************************************************************************/
static chain_list *swapchain_aux(chain_list* head_swap, chain_list* elem_chain)
{
  chain_list *chain;
  void       *elem;
  chain_list *other_swap;
  
   if (!elem_chain) return NULL;
   
   other_swap=swapchain_aux(NULL, elem_chain->NEXT);
      
   elem=elem_chain->DATA;
   
   if (!other_swap) return all_positions(head_swap, NULL, elem);
   
   for (chain=other_swap; chain; chain=chain->NEXT) {
      head_swap=all_positions(head_swap, chain->DATA, elem);
   }   
   
   freechain(other_swap);

   return head_swap;
}


/******************************************************************************/
/* return a list of list of elements                                          */
/*which compose all the possible different orders of elem_chain               */
/* elements aren't duplicated                                                 */
/******************************************************************************/
extern chain_list *swapchain(chain_list* elem_chain)
{
   return swapchain_aux(NULL, elem_chain);
}


/******************************************************************************/
/*parameter is a list of element list: for a place of the final list, several */
/*elements are possible                                                       */
/* return a list of element chain based on all choices proposed in parameter  */
/******************************************************************************/
static chain_list* composechain_aux(chain_list* elem_chain_chain)
{
   void       *elem;
   chain_list *elem_chain, *other_elem_chain, *current_elem_chain;
   chain_list *chain; 
   chain_list *head_elem_chain=NULL;

   /*the list is empty*/
   if (!elem_chain_chain) return addchain(NULL,NULL);

   other_elem_chain=composechain_aux(elem_chain_chain->NEXT);

   /*several elements for this place*/
   for (elem_chain=elem_chain_chain->DATA; elem_chain; elem_chain=elem_chain->NEXT) {
      elem=elem_chain->DATA;

      /*build new list with these choices*/
      for (chain=other_elem_chain; chain; chain=chain->NEXT) {
         current_elem_chain = chain->DATA;
         current_elem_chain = dupchain(current_elem_chain);
         current_elem_chain = addchain(current_elem_chain, elem);
         head_elem_chain    = addchain(head_elem_chain, current_elem_chain);
      }
      
   }

   /*free mem*/
   for (chain=other_elem_chain; chain; chain=chain->NEXT) freechain(chain->DATA);
   freechain(other_elem_chain);
   
   return head_elem_chain;
}


/******************************************************************************/
/*parameter is a list of element list: for each place of the final list,      */
/*several elements are possible                                               */
/* return a list of element chain based on all choices proposed in parameter  */
/******************************************************************************/
extern chain_list* composechain(chain_list* elem_chain_chain)
{
   chain_list* compose_chain;
   chain_list* head_swap_chain=NULL;
   chain_list* chain, *elem_chain;

   /*build all the possible abl with this choice of leaves*/
   compose_chain=composechain_aux(elem_chain_chain);
   
   /*build all permutations with this composition*/
   for (chain=compose_chain; chain; chain=chain->NEXT) {
      elem_chain=chain->DATA;
      /*list of permutations with these elements of ablchain*/
      /*add it to final result*/
      head_swap_chain=swapchain_aux(head_swap_chain, elem_chain);
   }

   freechain(compose_chain);
   
   return head_swap_chain;
}


/******************************************************************************/
/*return a list of abl. It is the list of all possible permutations of an abl */
/*abl parameter isn't deleted                                                 */
/******************************************************************************/
static chain_list *permutations_abl(chain_list* abl)
{
   chain_list *head, *list_abl;
   chain_list *permutations_leaves_list=NULL;
   chain_list *chain, *chain2;
   
   /*leaf*/
   if (ABL_ATOM(abl)) return addchain(NULL,dupablexpr(abl));

   /*build first permutations on leaves and get back*/
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      /*list of list of permutated leaves*/
      permutations_leaves_list=addchain(permutations_leaves_list,
                                        permutations_abl(ABL_CAR(chain)));
   }
   
   /*build all the possible abl with this choice of leaves*/
   list_abl=composechain(permutations_leaves_list);
   
   /*put the operator*/
   for (chain=list_abl; chain; chain=chain->NEXT) {
      head=createabloper(ABL_OPER(abl));
      ABL_ARITY(head)=ABL_ARITY(abl);
      ABL_CDR(head)=dupablchain(chain->DATA);
      chain->DATA=head;
   }   

   /*free previous search*/
   for (chain=permutations_leaves_list; chain; chain=chain->NEXT) {
      for (chain2=chain->DATA; chain2; chain2=chain2->NEXT) {
         freeablexpr(chain2->DATA);
      }
      freechain(chain->DATA);
   }
   freechain(permutations_leaves_list);

   return list_abl;
}


/******************************************************************************/
/* return 1 if abl1 and abl2 heave the same properties                        */
/******************************************************************************/
static int same_properties(chain_list* abl1, chain_list* abl2, befig_list* befig)
{
   if (ABL_ATOM(abl1)!=ABL_ATOM(abl2)) {
      fprintf(stderr,"same_properties: compute error\n");
      exit(1);
   }

   if (ABL_ATOM(abl1)) {
      /*no use to compare if the same leaf*/
      if (ABL_ATOM_VALUE(abl1)==ABL_ATOM_VALUE(abl2)) return 0;
      
      /*comparison*/
      if (getgenericT(befig,ABL_ATOM_VALUE(abl1))
         !=getgenericT(befig,ABL_ATOM_VALUE(abl2))) return 0;
      
      if (getgenericR(befig,ABL_ATOM_VALUE(abl1))   /* Rup+Rdown/2 */
         !=getgenericR(befig,ABL_ATOM_VALUE(abl2))) return 0;
      
      if (getgenericC(befig,ABL_ATOM_VALUE(abl1))   /* entry Capacitance */
         !=getgenericC(befig,ABL_ATOM_VALUE(abl2))) return 0;

      return 1;   /*equality*/
   }

   /*operator*/
   abl2=ABL_CDR(abl2);
   for (abl1=ABL_CDR(abl1); abl1; abl1=ABL_CDR(abl1)) {
      if (!same_properties(ABL_CAR(abl1),ABL_CAR(abl2),befig)) return 0;
      abl2=ABL_CDR(abl2);
   }
   
   return 0;   /*equality*/
}


/******************************************************************************/
/*    return the list of different abl   resulting of permutations            */
/******************************************************************************/
extern chain_list* different_abl(befig_list* befig, chain_list* abl)
{
   chain_list* list_abl;

   /*get all permutations possible*/
   list_abl=permutations_abl(abl);

#if 0   /*no use, all abl are different*/
   bepor_list *bepor, *bepor_aux;
   chain_list* chain;
   chain_list* control, *pred;
   
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      if (isvdd(bepor->NAME) || isvss(bepor->NAME)) continue;
      if (bepor->DIRECTION=='O' || bepor->DIRECTION=='Z') continue;
      for (bepor_aux=befig->BEPOR; bepor_aux; bepor_aux=bepor_aux->NEXT) {
        if (isvdd(bepor_aux->NAME) || isvss(bepor_aux->NAME)) continue;
        if (bepor_aux->DIRECTION=='O' || bepor_aux->DIRECTION=='Z') continue;

        /*search a leaves with equal properties*/
        if (getgenericT(befig,bepor->NAME)==getgenericT(befig,bepor_aux->NAME)
         && getgenericR(befig,bepor->NAME)==getgenericR(befig,bepor_aux->NAME)
         && getgenericC(befig,bepor->NAME)==getgenericC(befig,bepor_aux->NAME)){
           
            /*put off duplicate use*/
            for (control=list_abl; control; control=control->NEXT) {
               pred=control;
               for (chain=control->NEXT; chain; chain=chain->NEXT) {
                  /*erase doublon*/
                  if (same_properties(control->DATA,chain->DATA,befig)) {
                     freeablexpr(chain->DATA);
                     pred->NEXT=chain->NEXT;
                     chain->NEXT=NULL;
                     freechain(chain);
                     chain=pred;
                  }   
                  pred=chain;
               }
            } /*end of loop*/
        
        }
     }/*end of loop*/
   }/*end of loop*/
#endif   
   
   return list_abl;
}


/***************************************************************************/
/*    return the list of different biabl   resulting of permutations       */
/***************************************************************************/
static chain_list* permutations_biabl(biabl_list* biabl)
{
   chain_list *permutations_val, *permutations_cnd;
   chain_list *chain_cnd, *chain_val, *permute=NULL;
   chain_list *head_biabl_chain=NULL, *swap_biabl;
   chain_list *permutations_biabl, *chain, *order;
   biabl_list *biabl_aux;
   chain_list *mem_permutations=NULL;

   for ( ; biabl; biabl=biabl->NEXT) {
      /*give the list of different permutations*/
      permutations_cnd=permutations_abl(biabl->CNDABL);
      permutations_val=permutations_abl(biabl->VALABL);
      
      mem_permutations=addchain(mem_permutations,permutations_cnd);
      mem_permutations=addchain(mem_permutations,permutations_val);
      
      permutations_biabl=NULL;
      /*put in cell*/
      for (chain_cnd=permutations_cnd; chain_cnd; chain_cnd=chain_cnd->NEXT) {
         for (chain_val=permutations_val; chain_val; chain_val=chain_val->NEXT) {
            biabl_aux=beh_addbiabl(NULL, biabl->LABEL, chain_cnd->DATA, 
               chain_val->DATA);
            biabl_aux->USER=biabl->USER;
            permutations_biabl=addchain(permutations_biabl, biabl_aux);
         }
      }
      permute=addchain(permute, permutations_biabl);
   }   
 
   
   /*swap*/
   swap_biabl=composechain(permute);
   
   /*build real biabl*/
   for (chain=swap_biabl; chain; chain=chain->NEXT) {
      biabl_aux=NULL;
      for (order=chain->DATA; order; order=order->NEXT) {
         biabl=(biabl_list*) order->DATA;
         biabl_aux=beh_addbiabl(biabl_aux, biabl->LABEL, dupablexpr(biabl->CNDABL), 
                     dupablexpr(biabl->VALABL));
         biabl_aux->USER=biabl->USER;
      }
      
      biabl_aux=(biabl_list*) reverse((chain_list*) biabl_aux);
      head_biabl_chain=addchain(head_biabl_chain, biabl_aux);
   }

   /*free memory*/
   for (chain=permute; chain; chain=chain->NEXT) {
      permutations_biabl=chain->DATA;
      for (order=permutations_biabl; order; order=order->NEXT) {
         biabl=(biabl_list*) order->DATA;
         biabl->CNDABL=NULL;
         biabl->VALABL=NULL;
         biabl->USER  =NULL;
         beh_frebiabl(biabl);
      }
      freechain(permutations_biabl);
   }
   freechain(permute);

   /*free mem*/
   for (chain=swap_biabl; chain; chain=chain->NEXT) {
      freechain(chain->DATA);
   }
   freechain(swap_biabl);

   /*free mem*/
   for (chain=mem_permutations; chain; chain=chain->NEXT) {
      permutations_cnd=chain->DATA;
      for (order=permutations_cnd; order; order=order->NEXT) {
         freeablexpr(order->DATA);
      }
      freechain(permutations_cnd);
   }
   freechain(mem_permutations);
   
   return head_biabl_chain;
}

      
/******************************************************************************/
/*    return the list of different biabl   resulting of permutations          */
/******************************************************************************/
extern chain_list* different_biabl(befig_list* befig, biabl_list* biabl)
{
   /*get all permutations possible*/
   return permutations_biabl(biabl);
}

