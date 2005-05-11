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
 * Tool    : BooG - library cells utilities
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_lax_param.h"
#include "bog_lib_matching.h"
#include "bog_lib_specifications.h"
#include "bog_lib_utils.h"


/*size of memory block*/
#define BLOCK 512


static port_list* PORT;


/***************************************************************************/
/* return a new port taken from the heap                                   */
/***************************************************************************/
extern port_list* newport()
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
   new->ABL=NULL;
   new->C=0;
   new->R=0;
   new->T=0;
   new->DIRECTION=0;
   new->STABLE =0;
   new->NEGATIV=0;
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
   new->ABL=head->ABL;
   new->C=head->C;
   new->R=head->R;
   new->T=head->T;
   new->DIRECTION=head->DIRECTION;
   new->STABLE =head->STABLE;
   new->NEGATIV=head->NEGATIV;
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
   new->ABL=head->ABL;
   new->C=head->C;
   new->R=head->R;
   new->T=head->T;
   new->DIRECTION=head->DIRECTION;
   new->STABLE =head->STABLE;
   new->NEGATIV=head->NEGATIV;
   new->NAME=head->NAME;
}


/******************************************************************************/
/*build a new cell in our library                                             */
/* port of cell is the inverted list of bepor                                 */
/******************************************************************************/
extern cell_list* addCell(befig_list* befig)
{
   cell_list* cell;
   port_list* new;
   bepor_list* bepor;


   cell=(cell_list*) mbkalloc(sizeof(cell_list));

   cell->NAME=befig->NAME;
   cell->DELAY=0;
   cell->MODE='P';                  /*cell won't be developped in lofig result*/
   cell->AREA=getgenericarea(befig);
   cell->ABL=NULL;
   cell->BIABL=NULL;
   cell->NEXT=NULL;
   cell->BEFIG=befig;

   cell->PORT=NULL;        
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      new=newport();
      new->NEXT=cell->PORT;
      cell->PORT=new;
      new->NAME=bepor->NAME;
      new->DIRECTION=bepor->DIRECTION;
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
/*build a new cell in our library                                             */
/* port of cell is the inverted list of bepor                                 */
/******************************************************************************/
extern cell_list* copyCell(cell_list* old)
{
   cell_list* cell;

   cell=(cell_list*) mbkalloc(sizeof(cell_list));

   cell->BEFIG=old->BEFIG;
   cell->NAME=old->NAME;
   cell->MODE=old->MODE;       
   cell->AREA=old->AREA;
   cell->ABL=old->ABL;
   cell->BIABL=old->BIABL;
   cell->PORT=old->PORT;
   cell->NEXT=old->NEXT;
   
   return cell;
}


/******************************************************************************/
/*insert a new cell in a list of cells                                        */
/* the sort is the biggest to the lowest cell                                 */
/*  (AND a b) is after (AND a (NOT b))                                        */
/* because is always better to take the biggest cell to match for example:    */
/*  (AND (NOT a) (NOT b))  will be matched by (AND a (NOT b))                 */
/*return the new list if inserted else return NULL                            */
/******************************************************************************/
extern cell_list* classCell(cell_list* new, cell_list* list)
{
   cell_list* cell, *pred=NULL;
   biabl_list *biabl_new, *biabl_cell;

   for (cell=list; cell; cell=cell->NEXT) {
      /*new more precise than old -> put new before*/
      if (new->BIABL) {
        biabl_cell=cell->BIABL;
        for (biabl_new=new->BIABL; biabl_cell && biabl_new; biabl_new=biabl_new->NEXT) {
           if (pattern_matching(biabl_new->CNDABL,biabl_cell->CNDABL)
            && pattern_matching(biabl_new->VALABL,biabl_cell->VALABL)) break;
           biabl_cell=biabl_cell->NEXT;
        }
        if (biabl_new) break;   /*insert cell here*/
      }
      else {
         if (pattern_matching(new->ABL,cell->ABL)) break;   /*insert cell here*/
      }
      pred=cell;
   }

   new->NEXT=cell;
   /*new is at the beginning*/
   if (!pred) return new;
   
   /*new is placed inside list*/
   pred->NEXT=new;
   return list;   
}


