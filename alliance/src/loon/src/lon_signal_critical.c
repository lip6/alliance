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
 * Tool    : LooN - critical path 
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include "lon_lax_param.h"
#include "lon_lib_utils.h"
#include "lon_signal_utils.h"
#include "lon_signal_name.h"
#include "lon_signal_netlist.h"
#include "lon_signal_critical.h"



/***************************************************************************/
/*return the signal with the highest delay                                 */
/***************************************************************************/
extern losig_list* critical_output(lofig_list* lofig)
{
   double delay, max_delay=-1;
   losig_list* output=NULL, *losig;
   char* signame;
   
   for (losig=lofig->LOSIG; losig; losig=losig->NEXT) {
      if (!losig->NAMECHAIN) {
         fprintf(stderr,"critical_output: no losig name\n");
         exit(1);
      }
      signame=losig->NAMECHAIN->DATA;
      if (losig->TYPE==EXTERNAL) delay=getdelay(output_name(signame));
      else delay=getdelay(signame);
      if (delay>max_delay) {
         max_delay=delay;
         output=losig;
      }   
   }
   
   return output;
}


/***************************************************************************/
/* cost function for lofig                                                 */
/***************************************************************************/
extern double critical_delay(lofig_list* lofig)
{
   losig_list* losig;
   char* signame;

   losig=critical_output(lofig);
   
   if (!losig->NAMECHAIN) {
      fprintf(stderr,"critical_delay: no losig name\n");
      exit(1);
   }
   signame=losig->NAMECHAIN->DATA;

   if (losig->TYPE==EXTERNAL) return getdelay(output_name(signame));
   return getdelay(signame);
}


/****************************************************************************/
/*       return the list of losig which are belonging to path               */
/* if ck_include is set to one, include clock of register in path           */
/****************************************************************************/
static ptype_list* search_long_path(losig_list* losig, int ck_include)
{
   loins_list* best_loins=NULL;
   locon_list* locon;
   losig_list *best_losig=NULL;
   loins_list* loins;
   befig_list* befig;
   char* signame, *ck=NULL;
   int reg=0;  /*flag for flip-flop*/
   chain_list* lofigchain;
   ptype_list* ptype, *ret;
   double max_delay=-1, delay;
   cell_list* cell;

   if (!losig) {
      fprintf(stderr,"search_long_path: NULL pointer\n");
      exit(1);
   }
   
   if (!losig->NAMECHAIN) {
      fprintf(stderr,"search_long_path: no losig name\n");
      exit(1);
   }
   signame=(char*) losig->NAMECHAIN->DATA;
   
   /*search drivers*/
   ptype=getptype(losig->USER,LOFIGCHAIN);
   if (!ptype || !ptype->DATA) {
      fprintf(stderr,"search_long_path: no lofigchain found\n");
      exit(1);
   }
   
   for (lofigchain=(chain_list*) ptype->DATA; lofigchain; lofigchain=lofigchain->NEXT) {
      locon=(locon_list*) lofigchain->DATA;
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' isn't accepted\n",
         locon->NAME);
         exit(1);
      }
      /*only outputs*/
      if (isvss(locon->NAME) || isvdd(locon->NAME) 
       || locon->DIRECTION==IN || locon->TYPE==EXTERNAL) continue;
      loins=locon->ROOT;
      delay=loins_delay(loins,signame);
      if (delay>max_delay) {
         best_loins=loins;
         max_delay=delay;
      }   
   }   
   
   
   loins=best_loins; 
   ret=addptype(NULL, (long) max_delay>=0?max_delay:getdelay(signame), losig);
   if (!loins) return ret; 
 
   /*stop at flip-flop*/
   cell=getCell(loins->FIGNAME);
   if (!cell) {
      fprintf(stderr,"library error: no cell '%s.vbe' found\n",
      loins->FIGNAME);
      exit(1);
   }
   befig=cell->BEFIG;
   if (befig->BEREG) {
      ptype=getptype(befig->BEREG->BIABL->USER,ABL_STABLE);
      if (ptype) {
         /*do not include clock in path*/
         if (!ck_include) return ret;
         reg=1;
         ck=ptype->DATA;
      }
   }
   
   max_delay=-1;
   /*seek the latest input in cell*/
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins->FIGNAME);
         exit(1);
      }
      /*only inputs*/
      if (locon->DIRECTION==OUT || locon->DIRECTION==TRISTATE
       || isvss(locon->NAME) || isvdd(locon->NAME)) continue;
      /* if flip-flop, accept only clock */
      if (reg && ck!=locon->NAME) continue; 
      losig=locon->SIG;
      if (!losig->NAMECHAIN) {
         fprintf(stderr,"search_long_path: no name on signal\n");
         exit(1);
      }
      signame=(char*) losig->NAMECHAIN->DATA;
      delay=getdelay(signame);
      if (delay>max_delay) {
         best_losig=losig;
         max_delay=delay;
      }   
   }
   
   losig=best_losig;
   
   
   /*no input found  -> constant*/
   if (!losig) return ret;
   
   ret->NEXT=search_long_path(losig,ck_include);
   return ret;
}


/***************************************************************************/
/*return the list of losig with delays which are composing critical path   */
/***************************************************************************/
extern ptype_list* critical_path(lofig_list* lofig)
{
   losig_list* losig;
   
   losig=critical_output(lofig);
   /*lofig is empty*/
   if (!losig) return NULL;
   return (ptype_list*) reverse( (chain_list*) search_long_path(losig,1));
}


/***************************************************************************/
/*return the list of losig with delays which are composing critical path   */
/*path do not continue on clock if a flip-flop is met                      */
/***************************************************************************/
extern ptype_list* critical_path_without_clock(lofig_list* lofig)
{
   losig_list* losig;
   
   losig=critical_output(lofig);
   /*lofig is empty*/
   if (!losig) return NULL;
   return (ptype_list*) reverse( (chain_list*) search_long_path(losig,0));
}



