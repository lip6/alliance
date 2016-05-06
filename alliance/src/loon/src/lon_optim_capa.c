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
 * Tool    : LooN - cpapcitance optimization
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include "lon_lax_param.h"
#include "lon_lib_utils.h"
#include "lon_signal_utils.h"
#include "lon_signal_name.h"
#include "lon_signal_netlist.h"
#include "lon_signal_critical.h"
#include "lon_optim_capa.h"



/***************************************************************************/
/* sort a list of signal in capacitance order                              */
/***************************************************************************/
static ptype_list* sort_capa(ptype_list* capa)
{
   ptype_list* ptype, *best=NULL, *pred, *pred_best=NULL;
   double C, best_C;
   losig_list* losig;

   if (!capa) return NULL;
   
   best=capa;
   losig=(losig_list*) capa->DATA;
   if (!losig->NAMECHAIN) {
     fprintf(stderr,"sort_capa: no name on signal\n");
     autexit(1);
   }
   best_C=getcapacitance(losig->NAMECHAIN->DATA);
   pred=capa;
   
   /*seek the greatest*/
   for (ptype=capa->NEXT; ptype; ptype=ptype->NEXT) {
      losig=(losig_list*) ptype->DATA;
      if (!losig->NAMECHAIN) {
        fprintf(stderr,"sort_capa: no name on signal\n");
        autexit(1);
      }
      C=getcapacitance((char*) losig->NAMECHAIN->DATA);
      if (C>best_C) {
         best=ptype;
         best_C=C;
         pred_best=pred;
      }   
      pred=ptype;
   }
   
   if (pred_best) pred_best->NEXT=best->NEXT;
   else capa=capa->NEXT;
   
   best->NEXT=sort_capa(capa);
   return best;
}


/***************************************************************************/
/* try to improve timing by changing cell impedance                        */
/* return 1 if replace is successful                                      */
/***************************************************************************/
static int change_instance(loins_list* loins, losig_list* losig, lofig_list* lofig, int optim_level)
{
   losig_list* losig_aux;
   lofig_list* lofig_aux;
   double critical, best_critical, delay, best_delay;
   double RC_max, T_max;
   locon_list* locon, *locon_aux;
   int change=0;
   cell_list* cell, *best;

   best=getCell(loins->FIGNAME);
   if (!best || best->NAME!=loins->FIGNAME) {
      fprintf(stderr,"library error: no cell '%s.vbe' found\n",loins->FIGNAME);
      autexit(1);
   }
  
   if (!losig->NAMECHAIN) {
      fprintf(stderr,"change_instance: no name on signal\n");
      autexit(1);
   }

   best_critical=critical_delay(lofig);
   best_delay=loins_delay(loins,losig->NAMECHAIN->DATA);
   /*compare RC and delay of instance*/
   RC_max=loins_max_RC(loins,losig->NAMECHAIN->DATA);
   T_max=loins_max_T(loins);
   
   /*check all cell of the same kind*/
   for (cell=sameCells(loins->FIGNAME); cell; cell=cell->NEXT) {
      /*eval critical*/
      loins_capacitance(loins,0);  /*remove own capacity*/
      loins->FIGNAME=cell->NAME;   /*change cell*/
      loins_capacitance(loins,1); /*add its own capacity (cell has changed)*/
      propagate_loins_delay(loins);
      critical=critical_delay(lofig);
      delay=loins_delay(loins,losig->NAMECHAIN->DATA);
     
      /*take new solution?*/
      if ( 
      (critical<best_critical && (cell->AREA<=best->AREA || optim_level>OPTIM_DELAY0))
      || (critical==best_critical && delay<best_delay && (cell->AREA<=best->AREA || optim_level>=OPTIM_DELAY4 || (RC_max>T_max && optim_level>=OPTIM_DELAY1))) 
      || (critical==best_critical && delay==best_delay && cell->AREA<best->AREA)
      ) {
         best_critical=critical;
         best=cell;
         best_delay=delay;
         change=1;
      }
      
   }  /*end of loop on cell*/
   
   loins_capacitance(loins,0);  /*substract own capacity*/
   loins->FIGNAME=best->NAME;
  
   /*capacitance and critical delay*/
   loins_capacitance(loins,1); /*add it own capacity (cell has changed)*/
   propagate_loins_delay(loins);
   
   /*verify change of loins*/
   critical=critical_delay(lofig);
   delay=loins_delay(loins,losig->NAMECHAIN->DATA);
   if ((int)critical!=(int)best_critical || (int)delay!=(int)best_delay) {
      fprintf(stderr,
      "change_instance: compute error %f!=%f ps   %f!=%f ps    (%sdue to caller)\n",
      critical,best_critical,delay,best_delay,change?"not ":"");
      autexit(1);
   }  
   
   
   
   /*if cell doesn't exist but is composed by several cells*/
   /*map port with real order*/
   if (best->MODE=='A' || best->MODE=='a') {
      loins_capacitance(loins,0);  /*substract own capacity*/
      /*change names*/
      loins->FIGNAME=best->BEFIG->NAME;
      lofig_aux=getlofig(best->NAME,'A');
      for (locon=loins->LOCON; locon; locon=locon->NEXT) {
         /*search locon in model*/
         for (locon_aux=lofig_aux->LOCON; locon_aux; locon_aux=locon_aux->NEXT){
            if (locon->NAME==locon_aux->NAME) break;
         }        
         if (!locon_aux) {
            fprintf(stderr,"change_instance: locon '%s' not found\n",
            locon->NAME);
            autexit(1);
         }
         losig_aux=locon_aux->SIG;
         /*search real connection*/
         for (locon_aux=lofig_aux->LOINS->LOCON; locon_aux; 
           locon_aux=locon_aux->NEXT){
            if (losig_aux==locon_aux->SIG) break;
         }        
         if (!locon_aux) {
            fprintf(stderr,"change_instance: locon '%s' not found in cell\n",
            locon->NAME);
            autexit(1);
         }
         locon->NAME=locon_aux->NAME;
      }
     
      /*capacitance and critical delay*/
      loins_capacitance(loins,1); /*add it own capacity (cell has changed)*/
      propagate_loins_delay(loins);
      
      /*verify change of loins*/
      critical=critical_delay(lofig);
      delay=loins_delay(loins,losig->NAMECHAIN->DATA);
      if ((int)critical!=(int)best_critical || (int)delay!=(int)best_delay) {
         fprintf(stderr,
         "change_instance: flatten error %f!=%f ps   %f!=%f ps\n",
         critical,best_critical,delay,best_delay);
         autexit(1);
      }   
   
   }  /*end of change loins*/

   
   return change;
}


/***************************************************************************/
/* improve capacitance of lofig on critical path by repowered instance     */
/*critical path could be changed and can't be improved any more            */
/***************************************************************************/
extern void improve_capa_critical_path(lofig_list* lofig, int optim_level)
{
   ptype_list* ptype, *ptype2;
   loins_list* loins;
   locon_list* locon;
   chain_list *lofigchain;
   losig_list* losig;
   ptype_list* long_path;
   int change=1;

   
   /*relaxation algorithm*/
   while (change) {
      change=0;
      long_path=critical_path(lofig);
      long_path=sort_capa(long_path);
      
      /* on all signals of the critical path, improve capa*/
      for (ptype=long_path; ptype; ptype=ptype->NEXT) {
         losig=(losig_list*) ptype->DATA;
         if (!losig->NAMECHAIN) {
            fprintf(stderr,"improve_capa_critical_path: no name on signal\n");
            autexit(1);
         }
         /*seek latest driver*/
         ptype2=getptype(losig->USER,LOFIGCHAIN);
         if (!ptype2) {
            fprintf(stderr,
            "improve_capa_critical_path: no lofigchain on losig '%s'\n",
            (char*) losig->NAMECHAIN->DATA);
            autexit(1);
         }
         loins=NULL;
         for (lofigchain=ptype2->DATA; lofigchain; lofigchain=lofigchain->NEXT){
            locon= (locon_list*) lofigchain->DATA;
            if (locon->DIRECTION==UNKNOWN) {
               fprintf(stderr,"BEH: 'linkage %s' isn't accepted\n",
               locon->NAME);
               autexit(1);
            }
            if (locon->TYPE==EXTERNAL || locon->DIRECTION==IN) continue;
            loins=locon->ROOT;
            if (loins_delay(loins,losig->NAMECHAIN->DATA)>ptype->TYPE) break;
         }

         /*instance has changed, re-evaluate critical path*/
         if (loins && change_instance(loins, losig, lofig, optim_level)) {
            change=1;
            break;
         }   
      }

      freeptype(long_path);
   }

}


/***************************************************************************/
/*improve big RC of circuit                                                */
/***************************************************************************/
extern void improve_RC_circuit(lofig_list* lofig, int optim_level)
{
   loins_list* loins;
   locon_list* locon;
   losig_list* losig;

   for (loins=lofig->LOINS; loins; loins=loins->NEXT) {
      for (locon=loins->LOCON; locon; locon=locon->NEXT) {
         if (locon->DIRECTION==UNKNOWN) {
            fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
            locon->NAME,loins->INSNAME);
            autexit(1);
         }
         if (isvdd(locon->NAME) || isvss(locon->NAME)) continue;
         if (locon->DIRECTION!=IN) break;
      }
      if (!locon) {
         fprintf(stderr,"improve_RC_circuit: no output found for '%s'\n",
         loins->FIGNAME);
         autexit(1);
      }
      losig=locon->SIG;
      change_instance(loins, losig, lofig, optim_level);
   }
}



/***************************************************************************/
/* return 1 if we have added a buffer                                      */
/***************************************************************************/
static int insert_buffer(losig_list* losig, lofig_list* lofig, int optim_level, long index)
{
   double capa, init_capa;
   cell_list* buffer;
   chain_list* namechain, *sigchain=NULL;
   char* signame;
   lofig_list* model;
   losig_list* losig_buf;
   loins_list* loins_buf;
   locon_list* locon;
   losig_list* losig_vdd=NULL, *losig_vss=NULL, *losig_aux;
   ptype_list* ptype, *buffer_ptype;
   double delay, best_delay, init_delay;
   loins_list* loins;
   chain_list* lofigchain,*newlofigchain=NULL;
   int buffer_is_better=0, change=1;    /*flags*/
   chain_list* pred;
   chain_list* temp;


   buffer=getCellbuffer();
   /*no buffer in library*/
   if (!buffer) return 0;
   
   if (!losig->NAMECHAIN) {
      fprintf(stderr,"insert_buffer: no name on signal\n");
      autexit(1);
   }

   best_delay=critical_delay(lofig);
   init_capa=getcapacitance(losig->NAMECHAIN->DATA);

   /*add buffer to netlist*/
   signame=getautoname(losig->NAMECHAIN->DATA);
   namechain=addchain(NULL,signame);
   losig_buf=addlosig(lofig,index,namechain,INTERNAL);
   putcapacitance(signame,0);
   putdelay(signame,0);
   model=getlofig(buffer->BEFIG->NAME,'A');
   
   /*search vdd and vss*/
   for (locon=lofig->LOCON; locon; locon=locon->NEXT) {
      if (isvdd(locon->NAME)) losig_vdd=locon->SIG;
      if (isvss(locon->NAME)) losig_vss=locon->SIG;
   }
   
   /*build list of signal*/
   for (locon=model->LOCON;locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,model->NAME);
         autexit(1);
      }
      if (isvdd(locon->NAME)) losig_aux=losig_vdd;
      else if (isvss(locon->NAME)) losig_aux=losig_vss;
      else if (locon->DIRECTION==OUT) losig_aux=losig_buf;
      else if (locon->DIRECTION==IN) losig_aux=losig;
      else {
         fprintf(stderr,"insert_buffer: buffer port '%s' unknown\n",locon->NAME);
         autexit(1);
      }
      sigchain=addchain(sigchain,losig_aux);
   }

   
   sigchain=reverse(sigchain);
   loins_buf=addloins(lofig,signame,model,sigchain);
   freechain(sigchain);
   /*to check changes*/
   init_delay=getdelay(losig->NAMECHAIN->DATA);
   init_capa=getcapacitance(losig->NAMECHAIN->DATA);
   loins_capacitance(loins_buf,1/*add capa*/);
  
   /*lofigchain*/
   for (locon=loins_buf->LOCON;locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins_buf->INSNAME);
         autexit(1);
      }
      if (isvdd(locon->NAME)) losig_aux=losig_vdd;
      else if (isvss(locon->NAME)) losig_aux=losig_vss;
      else if (locon->DIRECTION==OUT) losig_aux=losig_buf;
      else if (locon->DIRECTION==IN) losig_aux=losig;
      else {
         fprintf(stderr,"insert_buffer: buffer port '%s' unknown\n",locon->NAME);
         autexit(1);
      }
      ptype=getptype(losig_aux->USER,LOFIGCHAIN);
      if (!ptype) losig_aux->USER=addptype(losig_aux->USER,LOFIGCHAIN,addchain(NULL,locon));
      else ptype->DATA=addchain(ptype->DATA,locon);
   }
   
   /*move all instance after buffer*/
   ptype=getptype(losig->USER,LOFIGCHAIN);
   buffer_ptype=getptype(losig_buf->USER,LOFIGCHAIN);
   if (!ptype || !buffer_ptype) {
      fprintf(stderr,"insert_buffer: LOFIGCHAIN not found\n");
      autexit(1);
   }
   
   for (lofigchain=((chain_list*)ptype->DATA)->NEXT/*first is entry of buffer*/;
        lofigchain; lofigchain=lofigchain->NEXT) {
      locon=(locon_list*) lofigchain->DATA;
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' isn't accepted\n",
         locon->NAME);
         autexit(1);
      }
      /*do not move drivers and port of circuit*/
      if (locon->TYPE==EXTERNAL || locon->DIRECTION!=IN) {
         newlofigchain=addchain(newlofigchain,locon);
         continue;
      }
      
      loins=locon->ROOT;
      /*change capacitance*/
      loins_capacitance(loins,0/*remove  capa*/);
      /*change netlist*/
      locon->SIG=losig_buf;
      buffer_ptype->DATA=addchain(buffer_ptype->DATA,locon);
      loins_capacitance(loins,1/*add capa*/);
   }
   
   /*put back drivers*/
   freechain(((chain_list*)ptype->DATA)->NEXT/*first is entry of buffer*/);
   ((chain_list*)ptype->DATA)->NEXT=newlofigchain;
   
   /*eval all changes*/
   propagate_loins_delay(loins_buf);

   /*relaxation algorithm*/
   while (change) {
      change=0;
      pred=NULL;
      buffer_ptype=getptype(losig_buf->USER,LOFIGCHAIN);
      ptype=getptype(losig->USER,LOFIGCHAIN);
      if (!buffer_ptype || !buffer_ptype->DATA) {
         fprintf(stderr,"insert_buffer: LOFIGCHAIN is empty\n");
         autexit(1);
      }
      
      /*put critical path before buffer*/
      for (lofigchain=buffer_ptype->DATA; lofigchain->NEXT/*last is buffer output*/;
           lofigchain=lofigchain->NEXT) {
         locon=lofigchain->DATA;
         loins=locon->ROOT;
      
         /*change capacitance*/
         loins_capacitance(loins,0/*remove capa*/);
         /*change netlist*/
         locon->SIG=losig;
         ptype->DATA=addchain(ptype->DATA,locon);
         loins_capacitance(loins,1/*add capa*/);

         /*put over*/
         if (pred) pred->NEXT=lofigchain->NEXT;
         else buffer_ptype->DATA=lofigchain->NEXT;
      
         /*eval all changes*/
         propagate_loins_delay(loins);
         propagate_loins_delay(loins_buf);
         delay=critical_delay(lofig);
      
         if (delay<best_delay) {
            /*finish change*/
            best_delay=delay; 
            change=1;   /*flag of change*/
            buffer_is_better=1;
            lofigchain->NEXT=NULL;
            freechain(lofigchain);
            break;
         }
         else {
            /*remove change*/
            if (pred) pred->NEXT=lofigchain;
            else buffer_ptype->DATA=lofigchain;
            
            /*unchange capacitance*/
            loins_capacitance(loins,0/*remove capa*/);
            /*unchange netlist*/
            locon->SIG=losig_buf;
            temp=ptype->DATA;
            ptype->DATA=temp->NEXT;
            temp->NEXT=NULL;
            freechain(temp);
            loins_capacitance(loins,1/*add capa*/);
            /*avoid change*/
            propagate_loins_delay(loins_buf);
            propagate_loins_delay(loins);
            
            /*for next loop*/
            pred=lofigchain;
         }
      }
      
   }/*end of while*/
   
   if (buffer_is_better) {
      /*chose the best buffer*/
      change_instance(loins_buf, losig_buf, lofig, optim_level);
   }   
   else {  /*if insert buffer is worst than before, remove it*/
      /*remove all references of buffer in lofigchain*/
      for (locon=loins_buf->LOCON; locon; locon=locon->NEXT) {
         losig_aux=locon->SIG;
         ptype=getptype(losig_aux->USER,LOFIGCHAIN);
         if (!ptype) break;
         pred=NULL;
         for (lofigchain=ptype->DATA; lofigchain; lofigchain=lofigchain->NEXT) {
            if (lofigchain->DATA==locon) {
               if (pred) pred->NEXT=lofigchain->NEXT;
               else ptype->DATA=lofigchain->NEXT;
               lofigchain->NEXT=NULL;
               freechain(lofigchain);
               break;
            }
            pred=lofigchain;
         }
      }
      
      buffer_ptype=getptype(losig_buf->USER,LOFIGCHAIN);
      ptype=getptype(losig->USER,LOFIGCHAIN);
      if (buffer_ptype) {
       for (lofigchain=buffer_ptype->DATA; lofigchain; lofigchain=lofigchain->NEXT) {
         locon=lofigchain->DATA;
         loins=locon->ROOT;
         /*change capacitance*/
         loins_capacitance(loins,0/*remove capa*/);
         /*change netlist*/
         locon->SIG=losig;
         ptype->DATA=addchain(ptype->DATA,locon);
         loins_capacitance(loins,1/*add capa*/);
         propagate_loins_delay(loins);
       }
      
       freechain(buffer_ptype->DATA);
       buffer_ptype->DATA=NULL;
      }
      
      loins_capacitance(loins_buf,0/*remove capa*/);
      dellosig(lofig,index);
      delloins(lofig,loins_buf->INSNAME);
      propagate_losig_delay(losig);
   
      /*verify no change on timing*/
      delay=critical_delay(lofig);
      capa=getcapacitance(losig->NAMECHAIN->DATA);
      //01/09/2004 xtof's modification: rounding problem
      if ((int)(capa + 0.5)!=(int)(init_capa + 0.5) || (int)(delay + 0.5)!=(int)(best_delay + 0.5) 
       || (int)(init_delay + 0.5)!=(int)(getdelay(losig->NAMECHAIN->DATA) + 0.5)) {
         fprintf(stderr,
         "insert_buffer: compute error %e!=%e fF   %f!=%f ps    %f!=%f ps\n",
         capa,init_capa,delay,best_delay, init_delay, getdelay(losig->NAMECHAIN->DATA));
         autexit(1);
      }   
   
   }
      
   return buffer_is_better;
}


/***************************************************************************/
/* improve capacitance of lofig on critical path by adding buffer          */
/*critical path could be changed and can't be improved any more            */
/*return the number of buffers inserted                                    */
/***************************************************************************/
extern int insert_buffer_critical_path(lofig_list* lofig, int optim_level)
{
   ptype_list* ptype;
   losig_list* losig;
   ptype_list* long_path;
   long count=0;
   long index=0;
   int change=1;

   /*search new index*/
   for (losig = lofig->LOSIG; losig; losig = losig->NEXT) {
     if (index < losig->INDEX) index = losig->INDEX;
   }
   if (index!=0) index++;


   /*relaxation algorithm*/
   while (change) {
      change=0;
      /*too dangerous to put some buffer on clock!!!*/
      long_path=critical_path_without_clock(lofig);
      long_path=sort_capa(long_path);
      
      /* on all signals of the critical path, improve capa*/
      for (ptype=long_path; ptype; ptype=ptype->NEXT) {
         losig=(losig_list*) ptype->DATA;
         if (insert_buffer(losig, lofig, optim_level, index)) {
            count++;
            index++;
            change=1;
            break;
         }   
      }

      freeptype(long_path);
   }
   
   return count;
}



