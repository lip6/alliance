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
 * Tool    : BooG - mapping standard cells on befig
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <stdlib.h>
#include <stdio.h>
#include <mut.h>
#include <mlo.h>
#include <aut.h>
#include <abl.h>
#include <abe.h>
#include "bog_lib_utils.h"
#include "bog_signal_utils.h"
#include "bog_signal_nameindex.h"
#include "bog_map_pattern.h"
#include "bog_map_abl.h"
#include "bog_map_befig.h"


/*size of memory block*/
#define BLOCK 512

/*table of signal index*/
static authtable* HTABLE=NULL;
/*lofig in creation*/
static lofig_list *lofig;
/*befig in mapping*/
static befig_list* befig;
/*index of losig*/
static long index;



/****************************************************************************/
/* set   an unique index for losig in lofig under building                  */
/****************************************************************************/
extern void setindex(long new_index)
{
   index = new_index;
}


/****************************************************************************/
/*return an unique index for losig in lofig under building                  */
/****************************************************************************/
extern long getindex()
{
   long ret;
   ret=index;
   index++;
   return ret;
}


/****************************************************************************/
/* return the losig named name in lofig under building                      */
/****************************************************************************/
extern losig_list* seeksignal(char* name)
{
   authelem* elem;
   
   elem=searchauthelem(HTABLE,name);
   if (!elem) {
      fprintf(stderr,"seeksignal: losig '%s' not found\n",name);
      exit(1);
   }

   return (losig_list*) elem->VALUE;
}


/****************************************************************************/
/*return an unique losig named as losig_sce                                 */
/****************************************************************************/
extern losig_list* make_equi(losig_list* losig_sce, losig_list* losig_dest)
{
   ptype_list *ptype_dest, *ptype_sce;
   chain_list* loconchain;
   locon_list* locon;
   chain_list* namechain;
   char* name;
   
   /*change lofigchain and reference*/
   ptype_sce=getptype(losig_sce->USER,LOFIGCHAIN);
   ptype_dest=getptype(losig_dest->USER,LOFIGCHAIN);
   if (!ptype_sce || !ptype_dest) {
      fprintf(stderr,
      "make_equi: LOFIGCHAIN is empty for '%s' and/or '%s'\n",
      (char*) losig_sce->NAMECHAIN->DATA, (char*) losig_dest->NAMECHAIN->DATA);
      exit(1);
   }
   
   for (loconchain=(chain_list*) ptype_sce->DATA; loconchain; 
   loconchain=loconchain->NEXT) {
      locon=(locon_list*) loconchain->DATA;
      locon->SIG=losig_dest;
      ptype_dest->DATA=addchain((chain_list*) ptype_dest->DATA,locon);  
   }
   
   /*new losig takes all properties*/
   for (namechain=losig_dest->NAMECHAIN; namechain; namechain=namechain->NEXT) {
      name=(char*) namechain->DATA;
      losig_dest->NAMECHAIN=addchain(losig_dest->NAMECHAIN,name);
      addauthelem(HTABLE,name,(int)losig_dest);
   }
   
   losig_dest->TYPE=losig_sce->TYPE;
   /*mask old losig*/
   dellosig(lofig,losig_sce->INDEX);

   return losig_dest;
}


/****************************************************************************/
/*                    body of lofig                                         */
/****************************************************************************/
static void add_loins_to_lofig()
{
   beaux_list *beaux;
   bebux_list *bebux;
   beout_list *beout;
   bereg_list *bereg;
   bebus_list *bebus;
   losig_list* output_losig;
   
   
   /*INTERNAL signals*/
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      /*build loins*/
      output_losig=seeksignal(beaux->NAME);
      map_abl(lofig,beaux->ABL,output_losig);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      /*build loins*/
      output_losig=seeksignal(bereg->NAME);
      map_register(lofig,bereg->BIABL,output_losig);
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      output_losig=seeksignal(bebux->NAME); 
      /*build loins*/
      map_bus(lofig,bebux->BIABL,output_losig);
   }
   
   
   /*EXTERNAL signals*/
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      output_losig=seeksignal(beout->NAME); 
      /*build loins,  put a buffer if needed*/
      map_abl(lofig,beout->ABL,output_losig);
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      output_losig=seeksignal(bebus->NAME); 
      /*build loins*/
      map_bus(lofig,bebus->BIABL,output_losig);
   }
  
}



/****************************************************************************/
/*  build interface, all INTERNAL signals                                   */
/****************************************************************************/
static void add_losig_to_lofig()
{
   chain_list *namechain;
   beaux_list *beaux;
   bebux_list *bebux;
   bepor_list *bepor;
   bereg_list *bereg;
   losig_list *losig;
   int index;

   /*copy of port signals and create locon for lofig*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      namechain=addchain(NULL,bepor->NAME);
      index=getindex();
      losig=addlosig(lofig,index,namechain,EXTERNAL/*no more capa parameter*/);
      addlocon(lofig,bepor->NAME,losig,bepor->DIRECTION);

      /*seek fast*/
      if (isvss(bepor->NAME)) addauthelem(HTABLE,VSS,(int) losig);
      else if (isvdd(bepor->NAME)) addauthelem(HTABLE,VDD,(int) losig);
      else addauthelem(HTABLE,bepor->NAME,(int) losig);
   }
   
   /*copy of INTERNAL signals*/
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      namechain=addchain(NULL,beaux->NAME);
      index=getindex();
      losig=addlosig(lofig,index,namechain,INTERNAL/*no more capa parameter*/);    
      /*seek fast*/
      addauthelem(HTABLE,beaux->NAME,(int) losig);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      namechain=addchain(NULL,bereg->NAME);
      index=getindex();
      losig=addlosig(lofig,index,namechain,INTERNAL/*no more capa parameter*/);
      addauthelem(HTABLE,bereg->NAME,(int) losig);
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      namechain=addchain(NULL,bebux->NAME);
      index=getindex();
      losig=addlosig(lofig,index,namechain,INTERNAL/*no more capa parameter*/);
      addauthelem(HTABLE,bebux->NAME,(int) losig);
   }

}


/***************************************************************************/
/*    build the one and zero constants                                     */
/***************************************************************************/
static void add_one_zero_losig()
{
   chain_list *namechain;
   int index;
   losig_list* one, *zero;
   char * name;
   
   /*add constants signals*/
   name=getautoname("one");
   /*report same properties*/
   putdelay(name,getdelay(getablatomone()));
   putcapacitance(name,getdelay(getablatomone()));
   namechain=addchain(NULL,name);
   index=getindex();
   one=addlosig(lofig,index,namechain,INTERNAL/*no more capa parameter*/);    
   addauthelem(HTABLE,getablatomone(),(int) one);

   name=getautoname("zero");
   /*report same properties*/
   putdelay(name,getdelay(getablatomzero()));
   putcapacitance(name,getdelay(getablatomzero()));
   namechain=addchain(NULL,name);
   index=getindex();
   zero=addlosig(lofig,index,namechain,INTERNAL/*no more capa parameter*/);    
   addauthelem(HTABLE,getablatomzero(),(int) zero);
}

 
/***************************************************************************/
/*    build the one and zero constants if needed in lofig                  */
/***************************************************************************/
static void add_one_zero_loins()
{
   losig_list *losig;
   chain_list* abl;
   ptype_list* ptype;
   
   /*one*/
   losig=seeksignal(getablatomone());
   abl=createablatom(getablatomone());
 
   ptype=getptype(losig->USER,LOFIGCHAIN);
   /*if not used erase signal*/
   if (!ptype || !ptype->DATA) dellosig(lofig,losig->INDEX);
   else map_abl(lofig,abl,losig);
   freeablexpr(abl);

   /*zero*/
   losig=seeksignal(getablatomzero());
   abl=createablatom(getablatomzero());
 
   ptype=getptype(losig->USER,LOFIGCHAIN);
   /*if not used erase signal*/
   if (!ptype || !ptype->DATA) dellosig(lofig,losig->INDEX);
   else map_abl(lofig,abl,losig);
   freeablexpr(abl);
   
}

 
/****************************************************************************/
/*                 build the lofig from befig                               */
/****************************************************************************/
extern lofig_list* map_befig(befig_list *befig_param, char* lofig_name)
{
      
   if (!befig_param || !lofig_name) {
      fprintf(stderr,"map_befig: NULL pointer\n");
      exit(1);
   }
   
   befig=befig_param;
   HTABLE= createauthtable (BLOCK);
   index=0;
   begin_count_area();
   
   /*header of lofig created*/
   lofig=addlofig(lofig_name);
   lofig->NAME=lofig_name;   /* not to change in lower letters */
  
   /*look inside signals*/
   add_losig_to_lofig();

   /*to obtain the same order than in befig*/
   lofig->LOCON=(locon_list*) reverse((chain_list*) lofig->LOCON);

   /*build the constant signal*/
   add_one_zero_losig();
   
   /*look inside instances*/
   lofigchain(lofig);  /*beginning of lofigchain*/
   add_loins_to_lofig();
   
   /*add or remove constant instance*/
   add_one_zero_loins();

   destroyauthtable(HTABLE);
   HTABLE=NULL;
   index=0;
   
   return lofig;
}


/****************************************************************************/
/* return the number of loins used in lofig                                 */
/****************************************************************************/
extern ptype_list* count_instance(lofig_list* lofig)
{
   ptype_list *ptype, *temp=NULL, *inst=NULL, *low, *pred=NULL, *pred_low;   
   loins_list* loins;

   if (!lofig) {
      fprintf(stderr,"count_instance: NULL pointer\n");
      exit(1);
   }
   
   for (loins=lofig->LOINS; loins; loins=loins->NEXT) {
      for (ptype=temp; ptype; ptype=ptype->NEXT) {
         if ((char*)ptype->DATA==loins->FIGNAME) break;
      }
      if (ptype) ptype->TYPE++;
      else temp=addptype(temp,1,loins->FIGNAME);
   }
   
   /*sort*/
   while (temp) {
      low=temp;
      pred_low=NULL;
   
      /*search elem*/
      for (ptype=temp; ptype; ptype=ptype->NEXT) {
         if (ptype->TYPE<low->TYPE) {
            pred_low=pred;
            low=ptype;
         }   
         pred=ptype;
      }
   
      if (!pred_low) temp=low->NEXT;
      else pred_low->NEXT=low->NEXT;
      
      low->NEXT=inst;
      inst=low;
   }
   
   return inst;
}





