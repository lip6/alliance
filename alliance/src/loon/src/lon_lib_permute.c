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
 * Tool    : LooN - tree permutations
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include <bdd.h>
#include "lon_lib_matching.h"
#include "lon_lib_specifications.h"
#include "lon_lib_utils.h"
#include "lon_lib_permute.h"



/******************************************************************************/
/*free a list of list                                                         */
/******************************************************************************/
static void freechainchain(chain_list* chainchain)
{
   chain_list* chain;
   
   for (chain=chainchain; chain; chain=chain->NEXT) {
      freechain(chain->DATA);
   }
   freechain(chainchain);
}


/******************************************************************************/
/* duplicate a list                                                           */
/******************************************************************************/
static chain_list* dupchain(chain_list* pthead)
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
/*return a list with data element put in all positions of elemchain           */
/*result is add on top of head                                                */
/******************************************************************************/
static chain_list *all_positions(chain_list* head, chain_list* elemchain, void* data)
{
  chain_list *chain, *list_elem, *pred=NULL;
  
   if (!elemchain) {
      elemchain=addchain(NULL,data);
      return addchain(head,elemchain);
   }   
      
   /*insert elem on a lower position*/
   list_elem=all_positions(NULL,elemchain->NEXT,data);

   /*add the first element of elemchain for the result*/
   for (chain=list_elem; chain; chain=chain->NEXT) {
      chain->DATA=addchain(chain->DATA,elemchain->DATA);
      pred=chain;
   }

   /*create a new element with elem placed in first*/
   elemchain=addchain(dupchain(elemchain),data);
   list_elem=addchain(list_elem,elemchain);
   
   /*take in account the head*/
   pred->NEXT=head;
   
   return list_elem;
}


/******************************************************************************/
/*permute all elements of elemchain and return the list of permutations       */
/* result is add on the top of head                                           */
/******************************************************************************/
static chain_list *swap(chain_list* head, chain_list* elemchain)
{
  chain_list *chain, *list_elem, *swap_elem;
  
   
  if (!elemchain) {
      if (!head) return addchain(NULL,NULL);
      return head;
   }   

   /*return a list of all possibilities with the rest of elemchain*/
   swap_elem=swap(NULL,elemchain->NEXT);

   list_elem=head;
   for (chain=swap_elem; chain; chain=chain->NEXT) {
      /*insert new element in all positions*/
      /*add it to final result*/
      list_elem=all_positions(list_elem,chain->DATA,elemchain->DATA);
   }
   
   /*free mem*/
   freechainchain(swap_elem);

   return list_elem;
}



/******************************************************************************/
/*for a list of variable port, return the list of permutations possible       */
/*it takes in account possible permutation variables contained in group       */
/*result is add on the top of head                                            */
/******************************************************************************/
static chain_list* replace(chain_list* head, chain_list* port, chain_list* group) 
{
   long place;
   chain_list *chain, *elem, *new_port, *port_aux;
   ptype_list* order=NULL, *ptype;
   char* name;
   
   /*memorize group places in port*/
   place=0;
   for (port_aux=port; port_aux; port_aux=port_aux->NEXT) {
      name=port_aux->DATA;   
      for (chain=group->DATA; chain; chain=chain->NEXT) {
          if (chain->DATA==name) {
            order=addptype(order,place,name);
            break;
          }  
      }
      place++;
   }

   order=(ptype_list*)reverse((chain_list*)order);

   /*replace each time by the values of group*/
   for ( ; group; group=group->NEXT) {
      elem=group->DATA;
      new_port=dupchain(port);
      ptype=order;
      place=0;
      /*replace values*/
      for (chain=new_port; chain&&ptype; chain=chain->NEXT) {
         if (ptype->TYPE==place) {
            chain->DATA=elem->DATA;
            elem=elem->NEXT;
            ptype=ptype->NEXT;
         }
         place++;
      }
      head=addchain(head,new_port);
   }

   freeptype(order);
   return head;
}


/******************************************************************************/
/* return the list of possible order for port                                 */
/*it takes in account permutations group list groupchain                      */
/*do not use port after calling function                                      */
/******************************************************************************/
static chain_list* compose(chain_list* port, chain_list* groupchain) 
{
   chain_list* res=NULL, *chain;
   chain_list* portchain, *group_swap;
   
   if (!groupchain) return addchain(NULL,port);

   portchain=compose(port,groupchain->NEXT);
   /*do permutations inside group and return a list of orders*/
   group_swap=swap(NULL,groupchain->DATA);
   
   /*add this group to precedent list of port*/
   for (chain=portchain; chain; chain=chain->NEXT) {
      port=chain->DATA;
      res=replace(res,port,group_swap);
   }
   
   freechainchain(group_swap);
   freechainchain(portchain);
   
   return res;
}


/******************************************************************************/
/* return a list of list of variables whom a permutation lets abl unchanged   */
/*add result on the top of head                                               */
/******************************************************************************/
static chain_list* permute_variable(chain_list* head,chain_list* abl)
{
   bddsystem  *system;
   bddcircuit *circuit;
   bddassoc   *assoc;
   bddvar     a_variable, b_variable;
   bddnode    *a_bdd, *b_bdd;
   bddnode    *bdd, *new;
   chain_list *pred, *first, *second, *save;


   system  = createbddsystem( 100, 1000, 100, 50000 );
   circuit = createbddcircuit( "permute_world", 10, 10, system );
   assoc=addbddassoc(system);
   
   bdd=addbddcircuitabl(circuit,abl);
   
   for (first=getablexprsupport(abl,ABL_SUPPORT_CHAIN); first; ) {
      head=addchain(head,first);
      a_bdd = searchbddcircuitin( circuit, (char*) first->DATA );
      a_variable = getbddvarbyindex( system, a_bdd->INDEX );
   
      pred=first;
      for (second=first->NEXT; second; second=pred->NEXT) {
         b_bdd = searchbddcircuitin( circuit,  (char*) second->DATA );
         b_variable = getbddvarbyindex( system, b_bdd->INDEX );

         /*create association to swap*/
         addbddnodeassoc( system, assoc, b_variable, a_bdd );
         addbddnodeassoc( system, assoc, a_variable, b_bdd );
         /*permute a and b */
         new = substbddnodeassoc( system, bdd, assoc );
         
         /*if same function then a and b indifferent*/
         if (bdd==new) {
            /*remove*/
            pred->NEXT=second->NEXT;
            /*put on new list*/
            second->NEXT=head->DATA;
            head->DATA=second;
         }
         else pred=second;
         
         /*refresh*/
         resetbddassoc(system, assoc);
      } 

      /*next one*/
      save=first;
      first=first->NEXT;
      /*first is contained in a list*/
      save->NEXT=NULL;
   }  
   
   destroybddassoc( system );
   destroybddsystem( system );
   destroybddcircuit( circuit );

   return head;
}


/******************************************************************************/
/*return all possible order of port                                           */
/******************************************************************************/
extern chain_list* permutations(befig_list* befig)
{
   chain_list* port=NULL;
   chain_list* groupchain=NULL;
   bepor_list* bepor;
  
   /*build group of variables which can be permuted*/
   if (befig->BEBUS) {
      groupchain=permute_variable(NULL,befig->BEBUS->BIABL->CNDABL);
      groupchain=permute_variable(groupchain,befig->BEBUS->BIABL->VALABL);
   }
   else if (befig->BEREG) {
      groupchain=permute_variable(NULL,befig->BEREG->BIABL->CNDABL);
      groupchain=permute_variable(groupchain,befig->BEREG->BIABL->VALABL);
   }
   else if (befig->BEOUT) {
      groupchain=permute_variable(NULL,befig->BEOUT->ABL);
   }
   
   /*build a base of order*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      port=addchain(port,bepor->NAME);
   }
   
   port=reverse(port);
   
   /*build different list of permutations*/
   return compose(port, groupchain);
}


