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
 * Tool    : BooG - count Capacitance
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
#include "bog_lib_negativ.h"
#include "bog_signal_utils.h"
#include "bog_signal_nameindex.h"
#include "bog_map_adapt.h"
#include "bog_signal_adapt.h"


/******************************************************************************/
/*  do an average betwenn each values and its opposite                        */
/******************************************************************************/
extern void average_capacitance(befig_list *befig)
{
   bereg_list* bereg;
   bebux_list* bebux;
   beaux_list* beaux;
   bepor_list* bepor;
   char* name;
   float capa;


   /*init*/
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      name=getoppositename(beaux->NAME);
      capa=getcapacitance(beaux->NAME);
      capa+=getcapacitance(name);
      capa=capa/2;
      putcapacitance(beaux->NAME,capa);
      putcapacitance(name,capa);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      name=getoppositename(bereg->NAME);
      capa=getcapacitance(bereg->NAME);
      capa+=getcapacitance(name);
      capa=capa/2;
      putcapacitance(bereg->NAME,capa);
      putcapacitance(name,capa);
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      name=getoppositename(bebux->NAME);
      capa=getcapacitance(bebux->NAME);
      capa+=getcapacitance(name);
      capa=capa/2;
      putcapacitance(bebux->NAME,capa);
      putcapacitance(name,capa);
   }

   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      /*inputs only*/
      if (isvss(bepor->NAME) || isvdd(bepor->NAME)) continue;
      switch (bepor->DIRECTION) {case OUT: case TRISTATE: continue;}
      name=getoppositename(bepor->NAME);
      capa=getcapacitance(bepor->NAME);
      capa+=getcapacitance(name);   
      capa=capa/2;
      putcapacitance(bepor->NAME,capa);
      putcapacitance(name,capa);
   }

}


/******************************************************************************/
/*calculate the capacitance for all signals                                   */
/* change also form of abl to adapt to the biggest cell                       */
/******************************************************************************/
extern void adapt_for_cell(befig_list *befig)
{
   beout_list* beout;
   bebus_list* bebus;
   bereg_list* bereg;
   bebux_list* bebux;
   beaux_list* beaux;
   biabl_list* biabl;
   bepor_list* bepor;
   char* name;


   /*constants*/
   putcapacitance(getablatomone(),0);
   putcapacitance(getablatomzero(),0);

   /*init*/
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      name=getoppositename(beaux->NAME);
      putcapacitance(beaux->NAME,0);
      putcapacitance(name,0);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      name=getoppositename(bereg->NAME);
      putcapacitance(bereg->NAME,0);
      putcapacitance(name,0);
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      name=getoppositename(bebux->NAME);
      putcapacitance(bebux->NAME,0);
      putcapacitance(name,0);
   }

   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      /*inputs only*/
      if (isvss(bepor->NAME) || isvdd(bepor->NAME)) continue;
      switch (bepor->DIRECTION) {case OUT: case TRISTATE: continue;}
      name=getoppositename(bepor->NAME);
      putcapacitance(bepor->NAME,0);
      putcapacitance(name,0);   
   }


   /*adapt abl for cell of library and eval capacitance*/
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      beout->ABL=inv_oper(beout->ABL,0);  /*propagate NOT*/
      beout->ABL=build_negativ(beout->ABL);  /*minimize inverters*/
      /*adapt the abl to match with a big cell later*/
      beout->ABL=adapt_abl(beout->ABL);    /*count capacitance of leaves*/
   }

   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      beaux->ABL=inv_oper(beaux->ABL,0);  /*propagate NOT*/
      beaux->ABL=build_negativ(beaux->ABL);  /*minmize number of NOT*/ 
      beaux->ABL=adapt_abl(beaux->ABL);   /*count capacitance of leaves*/
   }

   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->VALABL=inv_oper(biabl->VALABL,0);  /*propagate NOT*/
         biabl->CNDABL=inv_oper(biabl->CNDABL,0);  /*propagate NOT*/
         biabl->VALABL=build_negativ(biabl->VALABL);  /*minimize NOT*/
         biabl->CNDABL=build_negativ(biabl->CNDABL);  
      }   
      bebus->BIABL=adapt_bus(bebus->BIABL);       /*count capacitance of leaves*/
   }

   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->VALABL=inv_oper(biabl->VALABL,0);  /*propagate NOT*/
         biabl->CNDABL=inv_oper(biabl->CNDABL,0);  /*propagate NOT*/
         biabl->VALABL=build_negativ(biabl->VALABL);  /*minimize NOT*/
         biabl->CNDABL=build_negativ(biabl->CNDABL);  
      }
      bereg->BIABL=adapt_reg(bereg->BIABL);       /*count capacitance of leaves*/
   }

   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         biabl->VALABL=inv_oper(biabl->VALABL,0);  /*propagate NOT*/
         biabl->CNDABL=inv_oper(biabl->CNDABL,0);  /*propagate NOT*/
         biabl->VALABL=build_negativ(biabl->VALABL);  /*minimize NOT*/
         biabl->CNDABL=build_negativ(biabl->CNDABL);  
      }
      bebux->BIABL=adapt_bus(bebux->BIABL);       /*count capacitance of leaves*/
   }      

}


/******************************************************************************/
/* undefine all variables in hash table                                       */
/******************************************************************************/
extern void undefine_delay(befig_list *befig)
{
   bereg_list* bereg;
   bebux_list* bebux;
   beaux_list* beaux;
   bepor_list* bepor;


   /*init*/
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      undefdelay(beaux->NAME);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      undefdelay(bereg->NAME);
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      undefdelay(bebux->NAME);
   }

   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      /*inputs only*/
      if (isvss(bepor->NAME) || isvdd(bepor->NAME)) continue;
      switch (bepor->DIRECTION) {case OUT: case TRISTATE: continue;}
      undefdelay(bepor->NAME);
   }

}




