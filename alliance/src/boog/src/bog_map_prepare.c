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
 * Tool    : BooG - pattern matching for biggest cells
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_lax_param.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_signal_nameindex.h"
#include "bog_normalize_message.h"
#include "bog_normalize_ARITY.h"
#include "bog_map_pattern.h"
#include "bog_map_adapt.h"
#include "bog_map_prepare.h"



/******************************************************************************/
/* return the cell which matches expr, if return NULL expr arity is too big   */
/* (maybe few inversions should be done)                                      */
/******************************************************************************/
extern cell_list* cell_prepare(chain_list* expr)
{   
   cell_list* cell, *best_cell=NULL;
   port_list* port;
   int max_integrated=0, integration;

   if (!expr) {
      fprintf(stderr,"cell_prepare: NULL pointer\n");
      exit(1);
   }

   for (cell=getcell_logic_lib(); cell; cell=cell->NEXT) {
      
      /*improve speed*/
      if (ABL_ATOM(cell->ABL)!=ABL_ATOM(expr)) continue;
      
      /*improve speed*/
      if (!ABL_ATOM(expr) && ABL_OPER(expr)!=ABL_NOT/*match all*/) {
         if (ABL_ARITY(cell->ABL)!=ABL_ARITY(expr)) continue;
         switch (ABL_OPER(expr)) {
         case ABL_AND: case ABL_NOR:
            switch (ABL_OPER(cell->ABL)) {
            case ABL_AND: case ABL_NOR: break;
            default: continue;
            }
            break;
         case ABL_NAND: case ABL_OR:
            switch (ABL_OPER(cell->ABL)) {
            case ABL_NAND: case ABL_OR: break;
            default: continue;
            }
            break;
         default: if (ABL_OPER(cell->ABL)!=ABL_OPER(expr)) continue;   
         }
      }   
       
      for (port=cell->PORT ; port; port=port->NEXT) port->ABL=NULL;
      
      integration=eval_pattern(expr,cell->ABL, 0);

      if (integration==0) continue;
      /*take the cell which matches the most of operators*/
      if (integration>max_integrated) {
         max_integrated=integration; 
         best_cell=cell;
      }
      else if (integration==max_integrated) {
         /*take the litlest*/
         if (cell->AREA<best_cell->AREA) {
            best_cell=cell;
         }
      }
      
   }/*end of loop on cell*/

   /*if arity is too high then operator matched by buffer*/
   if (!best_cell || (ABL_ATOM(best_cell->ABL) && !ABL_ATOM(expr))) return NULL;
   else {
      invert_port(best_cell->PORT); /*invert abl if needed*/
      return best_cell;
   }
   
}


/******************************************************************************/
/*return the bus which matches (maybe few inversions should be done)          */
/******************************************************************************/
extern cell_list* cell_prepare_bus(biabl_list* biabl)
{   
   cell_list* cell, *best_cell=NULL;
   port_list *port;
   int max_integrated=0, integration;
   int test;
   biabl_list *biabl_cell, *biabl_aux;

   if (!biabl) {
      fprintf(stderr,"cell_prepare_bus: NULL pointer\n");
      exit(1);
   }

   for (cell=getcell_tristate_lib(); cell; cell=cell->NEXT) {
      /*prepare cell*/
      for (port=cell->PORT ; port; port=port->NEXT) port->ABL=NULL;
      
      integration=0;
      biabl_aux=biabl;
      for (biabl_cell=cell->BIABL; biabl_cell; biabl_cell=biabl_cell->NEXT) {
         if (!biabl_aux) break;
         test=eval_pattern(biabl_aux->CNDABL,biabl_cell->CNDABL,0);
         if (test==0) break;
         integration+=test;
         test=eval_pattern(biabl_aux->VALABL,biabl_cell->VALABL,0);
         if (test==0) break;
         integration+=test;
         biabl_aux=biabl_aux->NEXT;
      }
      
      if (biabl_cell) continue;
      
      /*take the cell which matches the most of operators*/
      if (integration>max_integrated) {
         max_integrated=integration; 
         best_cell=cell;
      }
      else if (integration==max_integrated) {
         /*take the litlest*/
         if (cell->AREA<best_cell->AREA) {
            best_cell=cell;
         }
      }
   }

   if (best_cell) {
      invert_port(best_cell->PORT); /*invert abl if needed*/
      return best_cell;
   }
   

   fprintf(stderr,"Mapping Error: No cell could match\n");
   for ( ; biabl; biabl=biabl->NEXT) {
      fprintf(stderr,"BLOCK( ");
      display_abl(biabl->CNDABL);
      fprintf(stderr," )\n");
      fprintf(stderr,"\tbus <= GUARDED ");
      display_abl(biabl->VALABL);
      fprintf(stderr,";\n");
   }
   exit(1);
}


/******************************************************************************/
/*return the reg which matches (maybe few inversions should be done)          */
/******************************************************************************/
extern cell_list* cell_prepare_reg(biabl_list* biabl)
{   
   cell_list* cell, *best_cell=NULL;
   port_list *port;
   int max_integrated=0, integration;
   int test;
   biabl_list *biabl_cell, *biabl_aux;
   
   if (!biabl) {
      fprintf(stderr,"cell_prepare_reg: NULL pointer\n");
      exit(1);
   }

   cell=getcell_register_lib();

   for ( ; cell; cell=cell->NEXT) {
      /*prepare cell*/
      for (port=cell->PORT; port; port=port->NEXT) port->ABL=NULL;
      
      integration=0;
      biabl_aux=biabl;
      for (biabl_cell=cell->BIABL; biabl_cell; biabl_cell=biabl_cell->NEXT) {
         if (!biabl_aux) break;
         test=eval_pattern(biabl_aux->CNDABL,biabl_cell->CNDABL,0);
         if (test==0) break;
         integration+=test;
         test=eval_pattern(biabl_aux->VALABL,biabl_cell->VALABL,0);
         if (test==0) break;
         integration+=test;
         biabl_aux=biabl_aux->NEXT;
      }
      
      if (biabl_cell) continue;
      
      /*take the cell which matches the most of operators*/
      if (integration>max_integrated) {
         max_integrated=integration; 
         best_cell=cell;
      }
      else if (integration==max_integrated) {
         /*take the litlest*/
         if (cell->AREA<best_cell->AREA) {
            best_cell=cell;
         }
      }
   }

   if (best_cell) {
      invert_port(best_cell->PORT); /*invert abl if needed*/
      return best_cell;
   }
   

   fprintf(stderr,"Mapping Error: No cell could match\n");
   for ( ; biabl; biabl=biabl->NEXT) {
      fprintf(stderr,"BLOCK( ");
      display_abl(biabl->CNDABL);
      fprintf(stderr," )\n");
      fprintf(stderr,"\treg <= GUARDED ");
      display_abl(biabl->VALABL);
      fprintf(stderr,";\n");
   }
   exit(1);
}


