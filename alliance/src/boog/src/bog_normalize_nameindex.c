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
 * Tool    : BooG - change name signals
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <abl.h>
#include <aut.h>
#include <abe.h>
#include "bog_signal_nameindex.h"
#include "bog_normalize_nameindex.h"


/*size of memory block*/
#define BLOCK 256

static authtable* HTABLE;
static ptype_list *VECTOR;


/***************************************************************************/
/*    change name of vector                                                */
/***************************************************************************/
static void changename(chain_list* abl)
{
   authelem *elem;

   if (ABL_ATOM(abl)) {
      elem=searchauthelem(HTABLE,ABL_ATOM_VALUE(abl));
      if (!elem) return;
      ABL_ATOM_VALUE(abl)=(char*) elem->VALUE;
      return;
   }
   
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      changename(ABL_CAR(abl));
   }   
}


/***************************************************************************/
/* return 1 if there some vector index to change                           */
/***************************************************************************/
static int control_vector()
{
   ptype_list* ptype;
   char *radical=NULL, *name;
   int index=0;
   char memo;
   int ret=0;

   memo=SEPAR;       /*separator for nameindex()*/
   
   for (ptype=VECTOR; ptype; ptype=ptype->NEXT) {
      if (radical==(char*) ptype->DATA) {
         index++;
         if (index!=ptype->TYPE) {
            ret=1;
            SEPAR=' ';
            name=nameindex((char*) ptype->DATA,ptype->TYPE);
            SEPAR='_';
            /*build a bit from a vector*/
            addauthelem(HTABLE,name,(int)nameindex(ptype->DATA,ptype->TYPE));
         }
      }
      else {
         radical=(char*) ptype->DATA;
         index=ptype->TYPE;
      }   
   }   

   SEPAR=memo;
   return ret;
}


/***************************************************************************/
/*insert in a sort list a vector                                           */
/***************************************************************************/
static void addvector(char *name)
{
   char *radical;
   int index;
   ptype_list *pred=NULL, *ptype;
   
   radical=vectorradical(name);
   index=vectorindex(name);

   for (ptype=VECTOR; ptype; ptype=ptype->NEXT) {
      if ((char*)ptype->DATA==radical) {
         for ( ; ptype; ptype=ptype->NEXT) {
            if ((char*)ptype->DATA!=radical || ptype->TYPE>index) break;
            pred=ptype;
         }
         break;
      }   
      pred=ptype;
   }
   
   ptype=addptype(ptype,index,radical);
   if (pred) pred->NEXT=ptype;
   else VECTOR=ptype;
}


/***************************************************************************/
/*if a vector has some space left between 2 bits, change vector in one to  */
/*one bit no to corrupt befig                                              */
/***************************************************************************/
extern void normalize_nameindex(befig_list* befig)
{
   bebus_list* bebus;
   beaux_list* beaux;
   beout_list* beout;
   bereg_list* bereg;
   bebux_list* bebux;
   biabl_list* biabl;
   authelem *elem;

   HTABLE=createauthtable (BLOCK);
   VECTOR=NULL;
   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      if (vectorindex(beaux->NAME)==-1) continue;
      addvector(beaux->NAME);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      if (vectorindex(bereg->NAME)==-1) continue;
      addvector(bereg->NAME);
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      if (vectorindex(bebux->NAME)==-1) continue;
      addvector(bebux->NAME);
   }

   /*is there any space in vectors?*/
   if (!control_vector()) {
      /*free table*/
      destroyauthtable(HTABLE);
      HTABLE=NULL;
      if (VECTOR) freeptype(VECTOR);
      VECTOR=NULL;
      return;
   }
   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      elem=searchauthelem(HTABLE,beaux->NAME);
      if (elem) beaux->NAME=(char*) elem->VALUE;
      changename(beaux->ABL);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      elem=searchauthelem(HTABLE,bereg->NAME);
      if (elem) bereg->NAME=(char*) elem->VALUE;
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         changename(biabl->CNDABL);
         changename(biabl->VALABL);
      }
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      elem=searchauthelem(HTABLE,bebux->NAME);
      if (elem) bebux->NAME=(char*) elem->VALUE;
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         changename(biabl->CNDABL);
         changename(biabl->VALABL);
      }
   }
   
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      /*bepor vector do not need to be controled*/
      changename(beout->ABL);
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      /*bepor vector do not need to be controled*/
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         changename(biabl->CNDABL);
         changename(biabl->VALABL);
      }
   }
   
   /*free table*/
   destroyauthtable(HTABLE);
   HTABLE=NULL;
   freeptype(VECTOR);
   VECTOR=NULL;

}


/***************************************************************************/
/*   sort vector in order                                                  */
/***************************************************************************/
static beaux_list* sortgeneric(beaux_list* top, beaux_list* generic)
{
   char *radical=NULL;
   beaux_list* pred=NULL, *beaux;
   int index;
   
   radical=vectorradical(generic->NAME);
   index=vectorindex(generic->NAME);
   
   /*search the good place*/
   for (beaux=top; beaux; beaux=beaux->NEXT) {
      if (radical==vectorradical(beaux->NAME)) {
         for ( ; beaux; beaux=beaux->NEXT) {
            if (radical!=vectorradical(beaux->NAME) 
             || index<vectorindex(beaux->NAME)) break;
            pred=beaux;
         }
         break;
      }   
      pred=beaux;
   }
   
   generic->NEXT=beaux;
   if (!pred) return generic;
   pred->NEXT=generic;
   return top;
}


/***************************************************************************/
/*  sort vectors to be recognized by parser/driver                         */
/***************************************************************************/
extern void sort_vector(befig_list* befig)
{
   beaux_list* beaux, *new_beaux=NULL, *pred_beaux=NULL, *beaux_next;
   bereg_list* bereg, *new_bereg=NULL, *pred_bereg=NULL, *bereg_next;
   bebux_list* bebux, *new_bebux=NULL, *pred_bebux=NULL, *bebux_next;
   
   for (beaux=befig->BEAUX; beaux; ) {
      if (vectorindex(beaux->NAME)!=-1) {
         if (pred_beaux) pred_beaux->NEXT=beaux->NEXT;
         else befig->BEAUX=beaux->NEXT;
         beaux_next=beaux->NEXT;
         new_beaux=sortgeneric(new_beaux,beaux);
         beaux=beaux_next;
      }
      else {
         pred_beaux=beaux;
         beaux=beaux->NEXT;
      }   
   }
   
   /*add sort list*/ 
   if (new_beaux) {
      /*reach last*/
      for (beaux=new_beaux; beaux->NEXT; beaux=beaux->NEXT) ;
      beaux->NEXT=befig->BEAUX;
      befig->BEAUX=new_beaux;
   }
   
   for (bebux=befig->BEBUX; bebux; ) {
      if (vectorindex(bebux->NAME)!=-1) {
         if (pred_bebux) pred_bebux->NEXT=bebux->NEXT;
         else befig->BEBUX=bebux->NEXT;
         bebux_next=bebux->NEXT;
         new_bebux=(bebux_list*) 
               sortgeneric((beaux_list*) new_bebux,(beaux_list*) bebux);
         bebux=bebux_next;
      }
      else {
         pred_bebux=bebux;
         bebux=bebux->NEXT;
      }   
   }
   
   /*add sort list*/ 
   if (new_bebux) {
      /*reach last*/
      for (bebux=new_bebux; bebux->NEXT; bebux=bebux->NEXT) ;
      /*connect*/
      bebux->NEXT=befig->BEBUX;
      befig->BEBUX=new_bebux;
   }
   
   for (bereg=befig->BEREG; bereg; ) {
      if (vectorindex(bereg->NAME)!=-1) {
         if (pred_bereg) pred_bereg->NEXT=bereg->NEXT;
         else befig->BEREG=bereg->NEXT;
         bereg_next=bereg->NEXT;
         new_bereg=(bereg_list*) 
                  sortgeneric((beaux_list*) new_bereg,(beaux_list*) bereg);
         bereg=bereg_next;
      }
      else {
         pred_bereg=bereg;
         bereg=bereg->NEXT;
      }   
   }
   
   /*add sort list*/ 
   if (new_bereg) {
      for (bereg=new_bereg; bereg->NEXT; bereg=bereg->NEXT) ;
      /*reach last*/
      bereg->NEXT=befig->BEREG;
      befig->BEREG=new_bereg;
   }

}


/***************************************************************************/
/* some radical name can generate conflict with automatic indexation       */
/*change name not to disturb algorithm                                     */
/***************************************************************************/
extern void change_radical(befig_list* befig)
{
   bebus_list* bebus;
   beaux_list* beaux;
   beout_list* beout;
   bereg_list* bereg;
   bebux_list* bebux;
   biabl_list* biabl;
   char* name;

   HTABLE=createauthtable (BLOCK);
   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      if (!forbid_radical(beaux->NAME)) continue;
      name=beaux->NAME;       /*name begins by MBK_NOT_..... */
      beaux->NAME=getautoname(beaux->NAME);
      addauthelem(HTABLE,name,(int)beaux->NAME);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      if (!forbid_radical(bereg->NAME)) continue;
      name=bereg->NAME;
      bereg->NAME=getautoname(bereg->NAME);
      addauthelem(HTABLE,name,(int)bereg->NAME);
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      if (!forbid_radical(bebux->NAME)) continue;
      name=bebux->NAME;
      bebux->NAME=getautoname(bebux->NAME);
      addauthelem(HTABLE,name,(int)bebux->NAME);
   }

   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      changename(beaux->ABL);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         changename(biabl->CNDABL);
         changename(biabl->VALABL);
      }
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         changename(biabl->CNDABL);
         changename(biabl->VALABL);
      }
   }
   
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      changename(beout->ABL);
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         changename(biabl->CNDABL);
         changename(biabl->VALABL);
      }
   }
   
   /*free table*/
   destroyauthtable(HTABLE);
   HTABLE=NULL;
   free_nameindex();   /*to save memory, we do not use these indexes later*/
}

