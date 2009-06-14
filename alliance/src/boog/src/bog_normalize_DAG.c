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
 * Tool    : BooG - control and simplify acyclic graph
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdio.h>
#include <stdlib.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include <mlo.h>
#include "bog_lax_param.h"
#include "bog_signal_utils.h"
#include "bog_normalize_message.h"
#include "bog_normalize_ARITY.h"
#include "bog_normalize_DAG.h"


#define USING(node) {node=-mark;}
#define USED(node) {node=(long)((((long)node<0)?0L:(long)node)+1L);}
#define IS_USING(node) ((long)node==-mark)
#define IS_USED(node) ((long)node!=0)
#define IS_UNUSED(node) ((long)node==0)
#define IS_ONE(node) ((long)node==1)


typedef struct equi {
   struct equi *NEXT;
   char        *NAME;      /*name of signal to replace*/
   chain_list  *ABL;      
} equi_list;   



static equi_list* EQUI;  /*list of replacing abl*/
static befig_list *befig;  /*the figure to check*/
static char* CYCLE;  /*name of signal when there is a cycle*/



/***************************************************************************/
/*    free memory for delay                                                */
/***************************************************************************/
static void free_equi(equi_list* equi)
{
   if (!equi) return;
   free_equi(equi->NEXT);
   freeablexpr(equi->ABL);
   mbkfree(equi);
}


/***************************************************************************/
/*  take from the heap                                                     */
/***************************************************************************/
static equi_list* newequi()
{
   equi_list* new;

   new=(equi_list*) mbkalloc(sizeof(equi_list));
   
   /*fill signal*/
   new->NEXT=NULL;      /*not to interfer with below*/
   new->ABL=NULL;
   new->NAME=NULL;
   return new;
}


/******************************************************************************/
/*            memorize equipotential                                          */
/******************************************************************************/
static void addequi(char* name, chain_list* abl)
{
   equi_list* new;

   new=newequi();
   new->NEXT=EQUI;
   new->NAME=name;
   new->ABL=abl;
   EQUI=new;
}


/******************************************************************************/
/* replace connections between equipotentials                                 */
/******************************************************************************/
static chain_list* inter_equi(equi_list *equi, chain_list *abl)
{
   chain_list* head;

   if (ABL_ATOM(abl)) {
      if (equi->NAME==ABL_ATOM_VALUE(abl)) {
            /*remove equi*/
            freeablexpr(abl);
            abl=dupablexpr(equi->ABL);
            /*put an arity if there was already an arity*/
            if (ABL_ARITY(equi->ABL)!=0) put_arity_abl(abl);
            return abl;
      }
      return abl;
   }

   head=abl;
   for (abl=ABL_CDR(head); abl; abl=ABL_CDR(abl)) {
      ABL_CAR_L(abl)=inter_equi(equi,ABL_CAR(abl));
   }
   
   return head;
}


/******************************************************************************/
/*          replace equipotential with real signal name                       */
/******************************************************************************/
static chain_list* replace_equi(chain_list* abl)
{
   chain_list* head;
   equi_list* equi;

   if (ABL_ATOM(abl)) {
      for (equi=EQUI; equi; equi=equi->NEXT) {
         if (equi->NAME==ABL_ATOM_VALUE(abl)) {
            /*remove equi*/
            freeablexpr(abl);
            abl=dupablexpr(equi->ABL);
            /*put an arity if there was already an arity*/
            if (ABL_ARITY(equi->ABL)!=0) put_arity_abl(abl);
            return abl;
         }
      }
      return abl;
   }

   head=abl;
   for (abl=ABL_CDR(head); abl; abl=ABL_CDR(abl)) {
      ABL_CAR_L(abl)=replace_equi(ABL_CAR(abl));
   }
   
   return head;
}


/******************************************************************************/
/*                          remove unused beaux                               */
/******************************************************************************/
static void refresh_signal()
{
   bereg_list* bereg;
   beout_list* beout;
   bebux_list* bebux;
   bebus_list* bebus;
   beaux_list* beaux;
   biabl_list* biabl;

   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      if (!EQUI) break;
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         if (!EQUI) break;
         biabl->CNDABL=replace_equi(biabl->CNDABL);
         biabl->VALABL=replace_equi(biabl->VALABL);
      }   
   }      

   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      if (!EQUI) break;
      beout->ABL=replace_equi(beout->ABL);
   }      

   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      if (!EQUI) break;
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         if (!EQUI) break;
         biabl->CNDABL=replace_equi(biabl->CNDABL);
         biabl->VALABL=replace_equi(biabl->VALABL);
      }   
   }      
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      if (!EQUI) break;
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         if (!EQUI) break;
         biabl->CNDABL=replace_equi(biabl->CNDABL);
         biabl->VALABL=replace_equi(biabl->VALABL);
      }   
   }      
   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      if (!EQUI) break;
      beaux->ABL=replace_equi(beaux->ABL);
   }      
}


/****************************************************************************/
/* unmark all and erase all unused beaux,bebux                              */
/****************************************************************************/
static void unmark_all()
{
   bebus_list *bebus;
   beout_list *beout;
   beaux_list *beaux, *pred_beaux=NULL;
   bebux_list *bebux, *pred_bebux=NULL;
   bereg_list *bereg, *pred_bereg=NULL;

   
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      beout->NODE=NULL;
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      bebus->BINODE=NULL;
   }   

   for (bebux=befig->BEBUX; bebux; ) {
      if (IS_USED(bebux->BINODE)) {
         bebux->BINODE=NULL;
         pred_bebux=bebux;
         bebux=bebux->NEXT;
      }   
      else { /*bebux isn't used by any output  -> erase it*/
         fprintf(stderr,"BEH: bus '%s' unused\n",bebux->NAME);
         if (signalsavelax(bebux->NAME)) {
            /*user doesn't want his signal been erased*/
            bebux->BINODE=NULL;
            pred_bebux=bebux;
            bebux=bebux->NEXT;
            continue;
         }
         if (pred_bebux) pred_bebux->NEXT=bebux->NEXT;
         else befig->BEBUX=bebux->NEXT;
         bebux->NEXT=NULL;   /*break recursivity of  beh_frebebux() */
         delsignal(bebux->NAME);
         beh_frebebux(bebux);
         /*change index for loop*/
         if (pred_bebux) bebux=pred_bebux->NEXT;
         else bebux=befig->BEBUX;
      }  
   }   
   
   for (bereg=befig->BEREG; bereg; ) {
      if (IS_USED(bereg->BINODE)) {
         bereg->BINODE=NULL;
         pred_bereg=bereg;
         bereg=bereg->NEXT;
      }   
      else { /*bereg isn't used by any output  -> erase it*/
         fprintf(stderr,"BEH: register '%s' unused\n",bereg->NAME);
         if (signalsavelax(bereg->NAME)) {
            /*user doesn't want his signal been erased*/
            bereg->BINODE=NULL;
            pred_bereg=bereg;
            bereg=bereg->NEXT;
            continue;
         }
         if (pred_bereg) pred_bereg->NEXT=bereg->NEXT;
         else befig->BEREG=bereg->NEXT;
         bereg->NEXT=NULL;   /*break recursivity of  beh_frebereg() */
         delsignal(bereg->NAME);
         beh_frebereg(bereg);
         /*change index for loop*/
         if (pred_bereg) bereg=pred_bereg->NEXT;
         else bereg=befig->BEREG;
      }  
   }   
   
   for (beaux=befig->BEAUX; beaux; ) {
      if (IS_USED(beaux->NODE) && !IS_ONE(beaux->NODE)
      && !ABL_ATOM(beaux->ABL)) {
         beaux->NODE=NULL;
         pred_beaux=beaux;
         beaux=beaux->NEXT;
      }   
      else { /*beaux isn't used by any output  -> erase it*/
         if (signalsavelax(beaux->NAME)) {
            /*user does not want his signal been erased*/
            beaux->NODE=NULL;
            pred_beaux=beaux;
            beaux=beaux->NEXT;
            continue;
         }   
         if (IS_ONE(beaux->NODE) || ABL_ATOM(beaux->ABL)) { 
            /*used only one time or equipotential*/
            addequi(beaux->NAME,beaux->ABL);
            beaux->ABL=NULL;
         }
         
         /*remove*/
         if (pred_beaux) pred_beaux->NEXT=beaux->NEXT;
         else befig->BEAUX=beaux->NEXT;
         beaux->NEXT=NULL;   /*break recursivity of  beh_frebeaux() */
         delsignal(beaux->NAME);
         beh_frebeaux(beaux);
         /*change index for loop*/
         if (pred_beaux) beaux=pred_beaux->NEXT;
         else beaux=befig->BEAUX;
      }  
   } 
   
}



/******************************************************************************/
/*                mark and follow the tree of abl                             */
/*         return 1 if correct abl                                            */
/******************************************************************************/
static long abl_dispatching(chain_list *abl, long mark)
{
   bebux_list *bebux;
   beaux_list *beaux;
   bereg_list *bereg;
   bepor_list *bepor;
   beout_list *beout;
   bebus_list *bebus;
   biabl_list *biabl;
   ptype_list* ptype;


   if (!abl) {
      fprintf(stderr,"abl_dispatching: NULL pointer\n");
      exit(1);
   }
   
   if (ABL_ATOM(abl)) { /*it is a leaf*/
      if (ABL_ATOM_VALUE(abl)==getablatomone() /* '1'  */|| 
          ABL_ATOM_VALUE(abl)==getablatomzero() /* '0'  */||
          ABL_ATOM_VALUE(abl)==getablatomdc() /* 'd'  */ ||
          ABL_ATOM_VALUE(abl)==getablatomtristate() /* 'z' */) return 1;

      /*search this leaf*/
      for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
         if (ABL_ATOM_VALUE(abl)==beaux->NAME) {
            if (IS_USING(beaux->NODE)) {
               CYCLE=beaux->NAME;
               fprintf(stderr,"BEH: cycle detected on %s, ",beaux->NAME);
               fflush(stderr);
               return 0;
            }
            if (IS_USED(beaux->NODE)) {
               USED(beaux->NODE);
               return 1;
            }
            USING(beaux->NODE);
            if (!abl_dispatching(beaux->ABL, mark)) {
               if (!CYCLE) return 0;   /*for display*/
               if (CYCLE==beaux->NAME) {
                  CYCLE=NULL; 
                  fprintf(stderr,"%s\n",beaux->NAME);
               }
               else fprintf(stderr,"%s, ",beaux->NAME);
               return 0;
            }
            USED(beaux->NODE);
            return 1;
         }
      }
      
      for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
         if (ABL_ATOM_VALUE(abl)==bebux->NAME) {
            if (IS_USING(bebux->BINODE)) {
               CYCLE=bebux->NAME;
               fprintf(stderr,"BEH: cycle detected on %s, ",bebux->NAME);
               fflush(stderr);
               return 0;
            }
            if (IS_USED(bebux->BINODE)) {
               USED(bebux->BINODE);
               return 1;
            }
            USING(bebux->BINODE);
            for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
               if (!abl_dispatching(biabl->CNDABL, mark)
                || !abl_dispatching(biabl->VALABL, mark)) {
                  if (!CYCLE) return 0;   /*for display*/
                  if (CYCLE==bebux->NAME) {
                     CYCLE=NULL; 
                     fprintf(stderr,"%s\n",bebux->NAME);
                  }
                  else fprintf(stderr,"%s, ",bebux->NAME);
                  return 0;
               }
            }   
            USED(bebux->BINODE);
            return 1;
         }
      }
      
      for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
         if (ABL_ATOM_VALUE(abl)==bereg->NAME) {
            if (IS_USING(bereg->BINODE)) {
               CYCLE=bereg->NAME;
               fprintf(stderr,"BEH: cycle detected on %s, ",bereg->NAME);
               fflush(stderr);
               return 0;
            }
            if (IS_USED(bereg->BINODE)) {
               USED(bereg->BINODE);
               return 1;
            }
            /*cycle forbidden on clock*/
            USING(bereg->BINODE);
            for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
               ptype=getptype(biabl->USER,ABL_STABLE); /*search if flip-flop*/
               /*cycle not forbidden on value for flip-flop */
               /*but forbidden for latch*/
               if (!abl_dispatching(biabl->CNDABL, mark)
               || (!ptype && !abl_dispatching(biabl->VALABL, mark))
               || (ptype  && !abl_dispatching(biabl->VALABL, mark+1))) {
                  if (!CYCLE) return 0;   /*for display*/
                  if (CYCLE==bereg->NAME) {
                     CYCLE=NULL; 
                     fprintf(stderr,"%s\n",bereg->NAME);
                  }
                  else fprintf(stderr,"%s, ",bereg->NAME);
                  return 0;
               }    
            }   
            USED(bereg->BINODE);
            /*impossible to look after value now, probably cycled on a signal*/
            /*wait the end of recursion*/
            return 1;
         }
      }
      
      /*control cycle on inout port*/
      for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
         if (ABL_ATOM_VALUE(abl)==beout->NAME) {
            if (IS_USING(beout->NODE)) {
               CYCLE=beout->NAME;
               fprintf(stderr,"BEH: cycle detected on %s, ",beout->NAME);
               fflush(stderr);
               return 0;
            }
            return 1;
         }
      }

      /*control cycle on inout bus port*/
      for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
         if (ABL_ATOM_VALUE(abl)==bebus->NAME) {
            if (IS_USING(bebus->BINODE)) {
               CYCLE=bebus->NAME;
               fprintf(stderr,"BEH: cycle detected on %s, ",bebus->NAME);
               fflush(stderr);
               return 0;
            }
            return 1;
         }
      }

      /*is it input?*/
      for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
         if (bepor->DIRECTION!=IN && bepor->DIRECTION!=INOUT 
          && bepor->DIRECTION!=TRANSCV) continue;
         if (bepor->NAME==ABL_ATOM_VALUE(abl)) return 1;
      }

      /*error: leaf not found*/
      fprintf(stderr,"abl_dispatching: %s not found\n",ABL_ATOM_VALUE(abl));
      exit(1);
   }
   
   /*it is an operator*/
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      /*define each leaf*/
      if (!abl_dispatching(ABL_CAR(abl)/*abl study*/, mark)) return 0;
   }

   return 1;   /*ok*/

}



/***************************************************************************/
/*                   mark each output                                      */
/* return 1 if correct                                                     */
/***************************************************************************/
static long mark_output()
{
   bebus_list *bebus;
   beout_list *beout;
   biabl_list *biabl;
   long mark=1;

   /*create new internal signals and mark the path from output to input*/
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      USING(beout->NODE);
      if (!abl_dispatching(beout->ABL, mark)) {
          if (!CYCLE) return 0;   /*for display*/
          if (CYCLE==beout->NAME) {
             CYCLE=NULL; 
             fprintf(stderr,"%s\n",beout->NAME);
          }
          else fprintf(stderr,"%s, ",beout->NAME);
          return 0;
      }    
      USED(beout->NODE);
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      USING(bebus->BINODE);
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         if (!abl_dispatching(biabl->CNDABL, mark) 
          || !abl_dispatching(biabl->VALABL, mark)) {
          if (!CYCLE) return 0;   /*for display*/
          if (CYCLE==bebus->NAME) {
             CYCLE=NULL; 
             fprintf(stderr,"%s\n",bebus->NAME);
          }
          else fprintf(stderr,"%s, ",bebus->NAME);
          return 0;
         }    
      }
      USED(bebus->BINODE);
   }   

   return 1;   /*ok*/
}


/******************************************************************************/
/*     control cycles in befig and erase unused internal signal               */
/*         return 1 if correct befig                                          */
/******************************************************************************/
extern long DAG_control(befig_list *befig_param)
{
   equi_list* equi, *equi2;
   bereg_list *bereg;
   bebus_list *bebus;
   bebux_list *bebux;

   if (!befig_param) {
      fprintf(stderr,"DAG_control: NULL pointer\n");
      exit(1);
   }   
   
   befig=befig_param;
   EQUI=NULL;
   CYCLE=NULL; /*name of signal*/
   
   /*erase binode*/
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      beh_frebinode(bereg->BINODE);
      bereg->BINODE=NULL;
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      beh_frebinode(bebus->BINODE);
      bebus->BINODE=NULL;
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      beh_frebinode(bebux->BINODE);
      bebux->BINODE=NULL;
   }
   
   /*begin by output*/
   if (!mark_output()) return 0;

   /*unmark and erase unused*/
   unmark_all();

   /*maybe cyclical reference in abl*/
   for (equi=EQUI; equi; equi=equi->NEXT) {
      for (equi2=EQUI; equi2; equi2=equi2->NEXT) {
         equi2->ABL=inter_equi(equi,equi2->ABL);
      }
   }   

   /*insert equipotential*/
   refresh_signal();
   free_equi(EQUI);
   return 1;
}

