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
 * Tool    : LooN - statistics
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "lon_lib_utils.h"
#include "lon_signal_netlist.h"
#include "lon_optim_stats.h"

/*worst RC*/
static long AREA; 
static long NUM_INS;




/****************************************************************************/
/* return the area of lofig                                                 */
/*count_instance() should be caught first                                   */
/****************************************************************************/
extern long getarea(lofig_list* lofig)
{
   return AREA;
}


/****************************************************************************/
/* return the area of lofig                                                 */
/*percent_instance() should be caught first                                 */
/****************************************************************************/
extern long getnum_ins(lofig_list* lofig)
{
   return NUM_INS;
}


/****************************************************************************/
/* return the number of loins used in lofig                                 */
/****************************************************************************/
extern ptype_list* count_instance(lofig_list* lofig)
{
   ptype_list *ptype, *temp=NULL, *inst=NULL, *low, *pred, *pred_low;   
   loins_list* loins;

   if (!lofig) {
      fprintf(stderr,"count_instance: NULL pointer\n");
      exit(1);
   }
   
   NUM_INS=0;
   
   for (loins=lofig->LOINS; loins; loins=loins->NEXT) {
      for (ptype=temp; ptype; ptype=ptype->NEXT) {
         if ((char*)ptype->DATA==loins->FIGNAME) break;
      }
      if (ptype) ptype->TYPE++;
      else temp=addptype(temp,1,loins->FIGNAME);
      NUM_INS++;
   }
   
   /*sort*/
   while (temp) {
      low=temp;
      pred_low=NULL;
      pred=temp;
      
      /*search the lowest elem*/
      for (ptype=temp->NEXT; ptype; ptype=ptype->NEXT) {
         if (ptype->TYPE<low->TYPE) {
            pred_low=pred;
            low=ptype;
         }   
         pred=ptype;
      }
   
      /*extract it from netlist*/
      if (!pred_low) temp=low->NEXT;
      else pred_low->NEXT=low->NEXT;
      
      low->NEXT=inst;
      inst=low;
   }
   
   return inst;
}


/****************************************************************************/
/*give the percentage of area of each modelcontained in model_list          */
/*count_instance() should be caught first                                   */
/****************************************************************************/
extern ptype_list* percent_instance(ptype_list* model_list)
{
   ptype_list *ptype, *inst=NULL;   
   cell_list* cell;

   AREA=0;
   
   if (!model_list) return NULL;
   
   /*eval area occupied by each model*/
   for (ptype=model_list; ptype; ptype=ptype->NEXT) {
      cell=getCell((char*)ptype->DATA);
      if (!cell) {
         fprintf(stderr,"Library Error: cell '%s' not found\n",cell->NAME);
         exit(1);
      }
      inst=addptype(inst,cell->AREA*ptype->TYPE,ptype->DATA);
      AREA+=cell->AREA*ptype->TYPE;
   }
   inst=(ptype_list*) reverse((chain_list*) inst);

   /*in percent*/
   for (ptype=inst; ptype; ptype=ptype->NEXT) {
      ptype->TYPE=(long)  ((double)ptype->TYPE*100)/((double)AREA);
   }
   
   return inst;
}


/***************************************************************************/
/*return the RC maximum of lofig                                           */
/***************************************************************************/
extern double getmaxRC(lofig_list* lofig)
{
   double t, RC=0;
   loins_list* loins;
   locon_list* locon;
   losig_list* losig;
   
   for (loins=lofig->LOINS; loins; loins=loins->NEXT) { 
      for (locon=loins->LOCON; locon; locon=locon->NEXT) {
         if (isvdd(locon->NAME) || isvss(locon->NAME) 
          || locon->DIRECTION==IN) continue; 
         losig=locon->SIG;
         if (!losig->NAMECHAIN) {
            fprintf(stderr,"getmaxRC: no name on signal\n");
            exit(1);
         }
         t=loins_max_RC(loins,losig->NAMECHAIN->DATA);
         if (t>RC) RC=t;
      }   
   }   

   return RC;
}


