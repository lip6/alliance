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
 * Tool    : LooN - library cells utilities
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "lon_lax_param.h"
#include "lon_signal_name.h"
#include "lon_lib_matching.h"
#include "lon_lib_specifications.h"
#include "lon_lib_permute.h"
#include "lon_lib_utils.h"


/*size of memory block*/
#define BLOCK 512


/*heap*/
static port_list* PORT;
/*list of cells with different behaviour*/
static chain_list* CELLS;


/***************************************************************************/
/*  take from the heap                                                     */
/***************************************************************************/
static port_list* newport()
{
   port_list* new;
   int i;

   if (!PORT) {
      PORT=mbkalloc(BLOCK*sizeof(port_list));
      new=PORT;
      for (i = 1; i < BLOCK; i++) {
         new->NEXT = new + 1;
         new++;
      }
      new->NEXT = NULL;
   }

   new=PORT;
   PORT=PORT->NEXT;
   
   /*fill signal*/
   new->NEXT=NULL;      /*not to interfer with below*/
   new->C=0;
   new->R=0;
   new->T=0;
   new->NAME=NULL;
   return new;
}


/******************************************************************************/
/*                    free a list of port                                     */
/******************************************************************************/
extern void delport(port_list* head)
{
   if (!head) return;
   delport(head->NEXT);
   head->NEXT=PORT;
   PORT=head;
}


/******************************************************************************/
/*duplicate a list of port but not its contents                               */
/******************************************************************************/
extern port_list* copyport(port_list* head)
{
   port_list *new;
   
   if (!head) return NULL;
   new=newport();
   new->NEXT=copyport(head->NEXT);
   new->C=head->C;
   new->R=head->R;
   new->T=head->T;
   new->NAME=head->NAME;
   return new;
}


/******************************************************************************/
/*  copy values of head to new recursively                                    */
/******************************************************************************/
extern void moveport(port_list* head, port_list* new)
{
   if (!head && !new) return;
   if (!head || !new) {
      fprintf(stderr,"moveport: discrepancy in length\n");
      exit(1);
   }
   
   moveport(head->NEXT,new->NEXT);
   new->C=head->C;
   new->R=head->R;
   new->T=head->T;
   new->NAME=head->NAME;
}


/******************************************************************************/
/*build a new cell in our library                                             */
/* port of cell is the inverted list of bepor                                 */
/******************************************************************************/
extern cell_list* copyCell(cell_list* old)
{
   cell_list* cell;

   cell=(cell_list*) mbkalloc(sizeof(cell_list));

   cell->BEFIG=old->BEFIG;
   cell->NAME=old->NAME;
   cell->AREA=old->AREA;
   cell->MODE=old->MODE;
   cell->PORT=old->PORT;
   cell->NEXT=old->NEXT;
   
   return cell;
}


/******************************************************************************/
/* build a lofig with namechain for interface and with lofig for instance     */
/*directly connected to external port                                         */
/******************************************************************************/
static lofig_list* build_lofig(lofig_list* lofig, chain_list* nameorder)
{
   long index=0;
   locon_list* locon;
   char memo;
   lofig_list* new;
   char* name;
   chain_list* sigchain=NULL, *namechain;
   losig_list* losig;
   
   memo=SEPAR;
   SEPAR='_';
   new=addlofig(getautoname(lofig->NAME));
   SEPAR=memo;
   
   for ( ; nameorder; nameorder=nameorder->NEXT) {
      name=(char*) nameorder->DATA;
      /*search locon*/
      for (locon=lofig->LOCON; locon; locon=locon->NEXT) {
         if (locon->DIRECTION==UNKNOWN) {
            fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
            locon->NAME,lofig->NAME);
            exit(1);
         }
         if (locon->NAME==name) break;
      }
      if (!locon) {
         fprintf(stderr,"build_lofig: no locon '%s' found in '%s'\n",name,
         lofig->NAME);
         exit(1);
      }
      
      namechain=addchain(NULL,name);
      losig=addlosig(new, index++, namechain, EXTERNAL);
      addlocon(new, locon->NAME, losig, locon->DIRECTION);
      sigchain=addchain(sigchain,losig);
   }
   
   sigchain=reverse(sigchain);
   addloins(new, lofig->NAME, lofig, sigchain);
   freechain(sigchain);

   return new;
}


/******************************************************************************/
/*build a new cell in our library                                             */
/*take properties in order                                                    */
/******************************************************************************/
static cell_list* properties_addCell(befig_list* befig, lofig_list *lofig)
{
   cell_list* cell;
   port_list* new;
   bepor_list* bepor;
   locon_list* locon;
   losig_list* losig;


   cell=(cell_list*) mbkalloc(sizeof(cell_list));

   cell->NAME=lofig->NAME;
   cell->AREA=getgenericarea(befig);
   cell->MODE='A';   /*to flatten later*/
   cell->NEXT=NULL;
   cell->BEFIG=befig;
   cell->PORT=NULL;        

   /*buils port*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      new=newport();
      new->NEXT=cell->PORT;
      cell->PORT=new;
      new->NAME=bepor->NAME;
      /*only for real input*/
      if (bepor->DIRECTION==OUT || bepor->DIRECTION==TRISTATE 
       || isvdd(bepor->NAME) || isvss(bepor->NAME)) continue;

      /*search locon in model*/
      for (locon=lofig->LOCON; locon; locon=locon->NEXT){
         if (bepor->NAME==locon->NAME) break;
      }        
      if (!locon) {
         fprintf(stderr,"properties_addCell: locon '%s' not found\n",
         locon->NAME);
         exit(1);
      }
      losig=locon->SIG;
      
      /*search real connection*/
      for (locon=lofig->LOINS->LOCON; locon; locon=locon->NEXT){
         if (losig==locon->SIG) break;
      }        
      if (!locon) {
         fprintf(stderr,"properties_addCell: locon '%s' not found in cell\n",
         locon->NAME);
         exit(1);
      }
      
      /*apply properties*/
      new->R=getgenericR(befig, locon->NAME);
      new->C=getgenericC(befig, locon->NAME);
      new->T=getgenericT(befig, locon->NAME);
   }


   /*put in the same order than bepor*/
   cell->PORT=(port_list*) reverse((chain_list*) cell->PORT);
   
   return cell;
}


/******************************************************************************/
/*insert cell in library and also its permutations                            */
/******************************************************************************/
static cell_list* insertCell(befig_list* befig, cell_list* top_lib)
{
   lofig_list* lofig, *new;
   cell_list* cell;
   chain_list* orderchain, *order, *chain;
   
   /*insert first cell with right order of port*/
   cell=createCell(befig);
   /*insert new cell in our library*/
   cell->NEXT= top_lib;
   top_lib=cell;
   
   /*build other cells with port permutations */
   lofig=getlofig(befig->NAME,'P');
   orderchain=permutations(befig);
   
   for (chain=orderchain; chain; chain=chain->NEXT) {
      /*build a new cell with order*/
      order=chain->DATA;
      new=build_lofig(lofig,order);
      cell=properties_addCell(befig,new);
      /*insert new cell in our library*/
      cell->NEXT= top_lib;
      top_lib=cell;
   }

   for (chain=orderchain; chain; chain=chain->NEXT) {
      freechain(chain->DATA);
   }
   freechain(orderchain);

   return cell;
}


/******************************************************************************/
/*build a new cell in our library                                             */
/******************************************************************************/
extern cell_list* createCell(befig_list* befig)
{
   cell_list* cell;
   port_list* new;
   bepor_list* bepor;


   cell=(cell_list*) mbkalloc(sizeof(cell_list));

   cell->NAME=befig->NAME;
   cell->AREA=getgenericarea(befig);
   cell->MODE='P';
   cell->NEXT=NULL;
   cell->BEFIG=befig;

   cell->PORT=NULL;        
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      new=newport();
      new->NEXT=cell->PORT;
      cell->PORT=new;
      new->NAME=bepor->NAME;
      /*only for real input*/
      if ((bepor->DIRECTION==IN || bepor->DIRECTION==INOUT 
      || bepor->DIRECTION==TRANSCV) && !isvdd(new->NAME) && !isvss(new->NAME)) {
         new->R=getgenericR(befig,bepor->NAME);
         new->C=getgenericC(befig,bepor->NAME);
         new->T=getgenericT(befig,bepor->NAME);
      }
   }

   /*put in the same order than bepor*/
   cell->PORT=(port_list*) reverse((chain_list*) cell->PORT);
   
   return cell;
}


/******************************************************************************/
/*add and classify a cell to our library                                      */
/******************************************************************************/
extern void addCell(befig_list* befig)
{
   chain_list *chain;
   cell_list* cell;
   
   /*search same behaviour*/
   for (chain=CELLS; chain; chain=chain->NEXT) {
      int x;
      cell= (cell_list*) chain->DATA;
printf("+ %s %s\n", cell->BEFIG->NAME, befig->NAME);
      x = compare_befig(cell->BEFIG,
                        befig);
printf("- %s %s\n", cell->BEFIG->NAME, befig->NAME);
      if (x) break;
   }
   
   /*insert cell*/
   if (!chain) {
      CELLS=addchain(CELLS,NULL);
      chain=CELLS;
   }
   /*insert at the top of chain*/
   chain->DATA=insertCell(befig, (cell_list*) chain->DATA);
}


/******************************************************************************/
/* return the cell named                                                      */
/******************************************************************************/
extern cell_list* getCell(char *name)
{
   chain_list* chain;
   cell_list* cell;

   for (chain=CELLS; chain; chain=chain->NEXT) {
      for (cell=(cell_list*) chain->DATA; cell; cell=cell->NEXT) {
         if (cell->NAME==name) return cell;
      }
   }

   return NULL;
}


/******************************************************************************/
/* return a buffer of our library                                             */
/******************************************************************************/
extern cell_list* getCellbuffer()
{
   chain_list* chain;
   cell_list* cell, *best;
   befig_list* befig;

   for (chain=CELLS; chain; chain=chain->NEXT) {
      cell=(cell_list*) chain->DATA;
      befig=cell->BEFIG;
      if (befig->BEREG || befig->BEBUS || !befig->BEOUT) continue;
      /*simple affect*/
      if (ABL_ATOM(befig->BEOUT->ABL) && ABL_ATOM_VALUE(befig->BEOUT->ABL)!=getablatomone() 
       && ABL_ATOM_VALUE(befig->BEOUT->ABL)!=getablatomzero() ) {
         best=cell;
         for (cell=cell->NEXT; cell; cell=cell->NEXT) {
            if (best->AREA>cell->AREA) cell=best;
         }
         return best;
      }  
   }

   return NULL;
}


/******************************************************************************/
/* return the list of cells with the same behaviour than named                */
/******************************************************************************/
extern cell_list* sameCells(char *name)
{
   chain_list* chain;
   cell_list* cell;

   for (chain=CELLS; chain; chain=chain->NEXT) {
      for (cell=(cell_list*) chain->DATA; cell; cell=cell->NEXT) {
         if (cell->NAME==name) return (cell_list*) chain->DATA;
      }
   }

   return NULL;
}




