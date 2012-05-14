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
 * Tool    : BooG - unflatten a befig
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <stdio.h>
#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include "bog_lax_param.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_map_pattern.h"
#include "bog_normalize_ARITY.h"
#include "bog_signal_nameindex.h"
#include "bog_map_adapt.h"
#include "bog_map_delay.h"
#include "bog_unflatten_abl.h"
#include "bog_signal_utils.h"
#include "bog_unflatten_befig.h"


static befig_list *befig;  /*the figure to check*/

/*to avoid warning during compiling*/
static void search_name(char* name);


/******************************************************************************/
/*  change all leaf expressions from 'mbk_not_a' to 'NOT (a)'                 */
/******************************************************************************/
static chain_list* change_atom(chain_list* abl,int invert)
{
   chain_list* chain;
   
   if (ABL_ATOM(abl)) {
      if (!invert) return abl;
      ABL_CAR_L(abl)=getoppositename(ABL_ATOM_VALUE(abl));
      return createablnotexpr(abl);
   }
   
   switch (ABL_OPER(abl)) {
      case ABL_NAND: case ABL_NOR: case ABL_NOT: invert=1; break;
      default: invert=0;
   }
   
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      ABL_CAR_L(chain)=change_atom(ABL_CAR(chain),invert);
   }   

   return abl;
}


/***************************************************************************/
/*create the opposite beaux of name                                        */
/***************************************************************************/
extern beaux_list* build_inv_name(char *name)
{
   chain_list* abl;
   
   abl=createablnotexpr(createablatom(name));
   put_arity_abl(abl);
   befig->BEAUX=
      beh_addbeaux(befig->BEAUX,getoppositename(name),abl,NULL);
   return befig->BEAUX;
}


/***************************************************************************/
/*create the opposite of beaux                                             */
/***************************************************************************/
static beaux_list* build_inv_beaux(beaux_list *beaux)
{
   chain_list* abl;

   /*create inverted value abl*/
   if (ABL_ATOM(beaux->ABL)) {
      abl=createablatom(getoppositename(ABL_ATOM_VALUE(beaux->ABL)));
   }
   else if (getoptimlax()>=OPTIM_DELAY4) {
      abl=createablnotexpr(dupablexpr(beaux->ABL));
      abl=unflatten_abl(abl);
   }
   else if (getoptimlax()<=OPTIM_DELAY1) {
      abl=createablnotexpr(createablatom(beaux->NAME));
   }
   else {
#if 0
      char *signame;
      cell_list* cell;
      port_list* port, *top;
      int only_atom=1;

      /*search cell which matches*/
      cell=cell_pattern(beaux->ABL);
      top=copyport(cell->PORT);  /*can be used by eval_delay()*/
      
      /*build auxiliary signal to put in common use with opposite*/
      for (port=top; port; port=port->NEXT) {
          if (isvss(port->NAME) || isvdd(port->NAME)) continue;
          else switch (port->DIRECTION) {
          case OUT: case TRISTATE: break;
          default: 
            if (!port->ABL) {
              fprintf(stderr,
              "build_inv_beaux: compute error %s not really match on port %s\n",
              cell->NAME,port->NAME);
              exit(1);
            }
            /*if already a signal no use to build a new one*/
            if (ABL_ATOM(port->ABL)) break;
            only_atom=0;
            /*create an auxiliary signal*/
            signame=getautoname(beaux->NAME);
            /*swap pointers references*/
            abl=createablatom(signame);
            swap_pointers(port->ABL,abl); /*abl is port->ABL, port->ABL is abl*/
            /*create commun leaves*/
            putcapacitance(signame,port->C*2/*the other cell*/);
            befig->BEAUX=beh_addbeaux(befig->BEAUX,signame,abl,NULL);
            eval_delay(befig->BEAUX->NAME,befig->BEAUX->ABL);
          }     
      }
   
      delport(top);

      /*create opposite beaux*/
      abl=createablnotexpr(dupablexpr(beaux->ABL));
      /* mbk_not_a ------> NOT (a)    (NOT will be absorbed by oper ahead) */
      if (only_atom) change_atom(abl,1);
#else
      abl=createablnotexpr(createablatom(beaux->NAME));
#endif
   }   
   
   put_arity_abl(abl);
   
   /*new cell*/
   befig->BEAUX=
      beh_addbeaux(befig->BEAUX,getoppositename(beaux->NAME),abl,NULL);
   
   return befig->BEAUX;
}


/***************************************************************************/
/*   eval the delay of each abl variables                                  */
/***************************************************************************/
static void search_abl(chain_list* abl)
{
   if (!abl) {
      fprintf(stderr,"search_abl: NULL pointer\n");
      exit(1);
   }

   if (!ABL_ATOM(abl)) {   /*operator*/
      for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
         search_abl(ABL_CAR(abl));
      }
      return;
   }
   
   if (isdelaydefined(ABL_ATOM_VALUE(abl))) return;

   search_name(ABL_ATOM_VALUE(abl));
}


/******************************************************************************/
/*search a signal and define its delay                                        */
/* unflatten the expression of this signal                                    */
/******************************************************************************/
static void search_name(char* name)
{
   bereg_list* bereg;
   beaux_list* beaux, *new_beaux;
   bebux_list* bebux;
   biabl_list* biabl;

   beaux=getbeaux(name); /*used only by beaux*/
   if (beaux) {
      search_abl(beaux->ABL);   /*define first leaves*/
      beaux->ABL=unflatten_abl(beaux->ABL);
      /*recount Capacitance of leaves*/
      new_beaux=build_inv_beaux(beaux);
      eval_delay(new_beaux->NAME,new_beaux->ABL);
      eval_delay(beaux->NAME,beaux->ABL);
      return;
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      if (bebux->NAME==name || getoppositename(bebux->NAME)==name) {
         for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
            search_abl(biabl->CNDABL);
            search_abl(biabl->VALABL);
            /*check if there is a tristate inverter in library*/
            /* keep the NOT to use inverter bus */
            if (is_tristate_inverter_lib() &&
              !ABL_ATOM(biabl->VALABL) && ABL_OPER(biabl->VALABL)==ABL_NOT) {
               ABL_CADR_L(biabl->VALABL)=unflatten_abl(ABL_CADR(biabl->VALABL));
            }
            else biabl->VALABL=unflatten_abl(biabl->VALABL);   /*arity solver*/
            biabl->CNDABL=unflatten_abl(biabl->CNDABL);        /*arity solver*/
         }
         
         /*recount Capacitance of leaves*/
         eval_bus(bebux->NAME,bebux->BIABL);
         
         /*evaluate opposite value delay*/
         new_beaux=build_inv_name(bebux->NAME);
         eval_delay(new_beaux->NAME,new_beaux->ABL);
         return;      
      }
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      if (bereg->NAME==name || getoppositename(bereg->NAME)==name) {
         for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
            search_abl(biabl->CNDABL);             /*only condition*/
            if (!getptype(biabl->USER,ABL_STABLE)) { /*only latch*/
               biabl->CNDABL=unflatten_abl(biabl->CNDABL);     /*arity solver*/
               search_abl(biabl->VALABL);
               biabl->VALABL=unflatten_abl(biabl->VALABL);     /*arity solver*/
               /*recount Capacitance of leaves only for latch*/
            }
         }
         /*calculate delay on clock setup for flip-flop and on all entries */
         /*for latch*/
         eval_reg(bereg->NAME,bereg->BIABL);
         
         /*evaluate opposite value delay*/
         new_beaux=build_inv_name(bereg->NAME);
         eval_delay(new_beaux->NAME,new_beaux->ABL);
         return;
      }   
   }         
   

   fprintf(stderr,"search_name: %s unknown\n",name);
   exit(1);
}


/***************************************************************************/
/*   estimate delay and change abl structure to match cell library and     */
/* optimize either circuit size or circuit delay                           */
/***************************************************************************/
extern void unflatten_befig(befig_list *befig_param)
{
   beaux_list* beaux, *new_beaux;
   bereg_list *bereg;
   bebus_list *bebus;
   beout_list *beout;
   biabl_list *biabl;
   bepor_list* bepor;
   float delay;
   
   if (!befig_param) {
      fprintf(stderr,"delay_multigraph: NULL pointer\n");
      exit(1);
   }   
   
   befig=befig_param;


   /*to fasten search because of hash table*/
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      putbeaux(beaux->NAME,beaux);   /*put in hash table for quick search*/
      /*invert value refers to the same abl*/
      putbeaux(getoppositename(beaux->NAME),beaux);   
   }

   /*constants*/
   putdelay(getablatomone(),0);
   putdelay(getablatomzero(),0);

   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      /*only input*/
      if (bepor->DIRECTION!=IN && bepor->DIRECTION!=INOUT 
       && bepor->DIRECTION!=TRANSCV) continue;
      if (isvdd(bepor->NAME) || isvss(bepor->NAME)) continue;
      /*  T + RC  */
      delay=getdelaylax(bepor->NAME) + getimpedancelax(bepor->NAME)
                  * getcapacitance(bepor->NAME);
      putdelay(bepor->NAME,delay);
      /*evaluate opposite value delay*/
      new_beaux=build_inv_name(bepor->NAME);
      eval_delay(new_beaux->NAME,new_beaux->ABL);   
   }

   /*separ value from condition for flip-flop to avoid cycle*/
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         if (getptype(biabl->USER,ABL_STABLE)) { /*only flip-flop*/
            search_abl(biabl->VALABL);
            biabl->VALABL=unflatten_abl(biabl->VALABL);     /*arity solver*/
            /*recount Capacitance of leaves*/
         }
      }
   }
   
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      search_abl(beout->ABL);
      beout->ABL=unflatten_abl(beout->ABL);           /*arity solver*/
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         search_abl(biabl->CNDABL);
         search_abl(biabl->VALABL);
         /*check if there is a tristate inverter in library*/
         /* keep the NOT to use inverter bus */
         if (is_tristate_inverter_lib() &&
           !ABL_ATOM(biabl->VALABL) && ABL_OPER(biabl->VALABL)==ABL_NOT) {
            ABL_CADR_L(biabl->VALABL)=unflatten_abl(ABL_CADR(biabl->VALABL));
         }
         else biabl->VALABL=unflatten_abl(biabl->VALABL);      /*arity solver*/
         biabl->CNDABL=unflatten_abl(biabl->CNDABL);           /*arity solver*/
      }
   }   
   
}

