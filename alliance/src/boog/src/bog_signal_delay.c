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
 * Tool    : BooG - delay on signals
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdio.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include <mlo.h>
#include "bog_lax_param.h"
#include "bog_lib_utils.h"
#include "bog_signal_utils.h"
#include "bog_signal_nameindex.h"
#include "bog_map_prepare.h"
#include "bog_map_pattern.h"
#include "bog_map_delay.h"
#include "bog_signal_delay.h"



/*to avoid warning*/
static void search_name(char* name, befig_list* befig);




/******************************************************************************/
/* return the max delay output for a befig                                    */
/******************************************************************************/
static ptype_list* max_delay(befig_list* befig)
{
   bereg_list* bereg;
   bebus_list* bebus;
   beout_list* beout;
   float max_delay=-1;
   float delay, ck;
   char* name=NULL;
   
   
   /*delay ot yet calculate for output*/
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      delay=loc_eval_delay(beout->ABL, getcapacitancelax(beout->NAME));
      /*not to erase input values*/
      putdelay(output_name(beout->NAME),delay);
      if (delay>max_delay) {max_delay=delay; name=beout->NAME;}
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      delay=loc_eval_bus(bebus->BIABL,getcapacitancelax(bebus->NAME));
      /*not to erase input values*/
      putdelay(output_name(bebus->NAME),delay);
      if (delay>max_delay) {max_delay=delay; name=bebus->NAME;}
   }   
   
   /*calculate for register values*/
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      delay=eval_value_reg(bereg->BIABL);   
      ck=eval_clock_reg(bereg->BIABL);
      /*not to erase input values*/
      putdelay(output_name(bereg->NAME),delay);
      delay-=ck;                                    /*take care of ck setup time*/
      if (delay>max_delay) {max_delay=delay; name=bereg->NAME;}
   }   
   

   return addptype(NULL, (long)max_delay, name);
}


/***************************************************************************************/
/*return the list of local instances which are belonging to path                       */
/***************************************************************************************/
static ptype_list* search_long_path(loins_list* loins, befig_list* befig)
{
   loins_list* loins_aux, *best_loins=NULL;
   locon_list* locon;
   losig_list* losig, *best_losig=NULL;
   char* signame;
   chain_list* loconchain;
   ptype_list* ptype, *ret;
   float max_delay=-1, delay;
   bereg_list* bereg;
   biabl_list* biabl;

   if (!loins) {
      fprintf(stderr,"search_long_path: NULL pointer\n");
      exit(1);
   }
   
   /*seek the latest input in cell*/
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      /*only inputs*/
      if (locon->DIRECTION==OUT || locon->DIRECTION==TRISTATE
       || isvss(locon->NAME) || isvdd(locon->NAME)) continue;
      losig=locon->SIG;
      if (!losig->NAMECHAIN) {
         fprintf(stderr,"search_long_path: NULL pointer\n");
         exit(1);
      }
      signame=(char*) losig->NAMECHAIN->DATA;
      delay=getdelay(signame);
      if (delay>max_delay) {
         best_losig=losig;
         max_delay=delay;
      }   
   }
   
   /*no input found  -> constant*/
   if (!best_losig) return NULL;
   
   losig=best_losig;
   ptype=getptype(losig->USER,LOFIGCHAIN);
   if (!ptype || !ptype->DATA) {
      fprintf(stderr,"search_long_path: NULL pointer\n");
      exit(1);
   }
   signame=(char*) losig->NAMECHAIN->DATA;
   
   ret=addptype(NULL, (int) max_delay, signame);

   /*stop at leaves or flip-flop */
   if (losig->TYPE==EXTERNAL) return ret;
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      if (bereg->NAME!=signame) continue;
      /*is it a flip-flop?*/
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         if (getptype(biabl->USER,ABL_STABLE)) return ret; 
      }
   }

   /*search driver of signal (latest if bus)*/
   max_delay=-1;   
   
   for (loconchain=(chain_list*) ptype->DATA; loconchain; loconchain=loconchain->NEXT) {
      locon=(locon_list*) loconchain->DATA;
      /*only outputs*/
      if (locon->DIRECTION==IN || isvss(locon->NAME) || isvdd(locon->NAME) 
       || locon->TYPE==EXTERNAL) continue;
      loins_aux=locon->ROOT;
      if (loins_aux==loins) continue;
      delay=getdelay(loins_aux->INSNAME);
      if (delay>max_delay) {
         best_loins=loins_aux;
         max_delay=delay;
      }   
   }   

   if (!best_loins) {
      fprintf(stderr,"search_long_path: no loins found for driving losig '%s'\n",signame);
      exit(1);
   }
   
   ret->NEXT=search_long_path(best_loins, befig);
   return ret;
}


/******************************************************************************/
/* latest delay path                                                          */
/*return list of signals in critical path. PTYPE is delay on signal           */
/*if return NULL, no path has been found                                      */
/******************************************************************************/
extern ptype_list* max_delay_path(befig_list* befig, lofig_list* lofig)
{
   ptype_list* head, *path;
   loins_list* loins;
   char *insname;
  
   if (!befig) {
      fprintf(stderr,"max_delay_path: NULL pointer\n");
      exit(1);
   }
 
   if (!lofig->LOINS) return NULL;
   
   head=max_delay(befig);
   
   if (head) {
      /*search instance which begins*/
      for (loins=lofig->LOINS; loins; loins=loins->NEXT) {
         insname=loins_name(head->DATA);
         if (insname==loins->INSNAME) break;
      }
      if (!loins) {
         fprintf(stderr,"max_delay_path: no loins '%s' found\n", insname);
         exit(1);
      }
      path=search_long_path(loins, befig);
      head->NEXT=path;
   }
   
   return (ptype_list*) reverse((chain_list*)head);
}



/***************************************************************************/
/*   eval the delay of each abl variables                                  */
/***************************************************************************/
static void search_abl(chain_list* abl, befig_list* befig)
{
   if (!abl) {
      fprintf(stderr,"search_abl: NULL pointer\n");
      exit(1);
   }

   if (!ABL_ATOM(abl)) {   /*operator*/
      for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
         search_abl(ABL_CAR(abl),befig);
      }
      return;
   }
   
   if (isdelaydefined(ABL_ATOM_VALUE(abl))) return;

   search_name(ABL_ATOM_VALUE(abl),befig);
}


/******************************************************************************/
/*search a signal and define its delay                                        */
/******************************************************************************/
static void search_name(char* name, befig_list* befig)
{
   bereg_list* bereg;
   beaux_list* beaux;
   bebux_list* bebux;
   biabl_list* biabl;

   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      if (beaux->NAME!=name) continue;
      search_abl(beaux->ABL,befig);
      eval_delay(beaux->NAME,beaux->ABL);
      return;      
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      if (bebux->NAME!=name) continue;
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         search_abl(biabl->CNDABL,befig);
         search_abl(biabl->VALABL,befig);
      }
      eval_bus(bebux->NAME,bebux->BIABL);
      return;      
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      if (bereg->NAME!=name) continue;
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         search_abl(biabl->CNDABL,befig);             /*only condition*/
         if (!getptype(biabl->USER,ABL_STABLE)) { /*only latch*/
            search_abl(biabl->VALABL,befig);
         }
      }
      /*calculate delay on clock setup for flip-flop and on all entries */
      /*for latch*/
      eval_reg(bereg->NAME,bereg->BIABL);
      return;
   }         
   

   fprintf(stderr,"search_name: %s unknown\n",name);
   exit(1);
}


/***************************************************************************/
/*global delay estimation on internal signals                              */
/***************************************************************************/
extern void final_eval_delay(befig_list *befig)
{
   bereg_list *bereg;
   bebus_list *bebus;
   beout_list *beout;
   biabl_list *biabl;
   bepor_list* bepor;
   float delay;
   
   if (!befig) {
      fprintf(stderr,"eval_delay: NULL pointer\n");
      exit(1);
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
   }

   /*separ value from condition for flip-flop to avoid cycle*/
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         if (getptype(biabl->USER,ABL_STABLE)) { /*only flip-flop*/
            search_abl(biabl->VALABL,befig);
         }
      }
   }
   
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      search_abl(beout->ABL,befig);
      /*do not define delay now*/
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         search_abl(biabl->CNDABL,befig);
         search_abl(biabl->VALABL,befig);
         /*do not define delay now*/
      }
   }   
   
}



