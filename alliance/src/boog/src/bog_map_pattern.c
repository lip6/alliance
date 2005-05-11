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
 * Tool    : BooG - pattern matching for shortest delay
 * Date    : 2000
 * Author  : Francois Donnet
 */


#ifdef AUTO_HAS_VALUES_H
# include <values.h>
#else
# include <float.h>
# ifndef MAXFLOAT
#  define MAXFLOAT FLT_MAX
# endif
#endif

#include <stdlib.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include <mlo.h>
#include "bog_lax_param.h"
#include "bog_signal_nameindex.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_normalize_message.h"
#include "bog_normalize_ARITY.h"
#include "bog_map_delay.h"
#include "bog_map_adapt.h"
#include "bog_map_pattern.h"


/*size of memory block*/
#define BLOCK 512


typedef struct delay {
   struct delay* NEXT;
   float         C;
   float         DELAY;
   chain_list*   ABL;
} delay_list;


/*heap for delay mecanismus*/
static delay_list* HEAP;
/*list of abl already calculated (not to repeat)*/
static delay_list* ALREADY_EVAL;



/***************************************************************************/
/*  take from the heap                                                     */
/***************************************************************************/
static delay_list* newdelay()
{
   delay_list* new;
   int i;

   if (!HEAP) {
      HEAP=mbkalloc(BLOCK*sizeof(delay_list));
      new=HEAP;
      for (i = 1; i < BLOCK; i++) {
         new->NEXT = new + 1;
         new++;
      }
      new->NEXT = NULL;
   }

   new=HEAP;
   HEAP=HEAP->NEXT;
   
   return new;
}


/******************************************************************************/
/* put back to the heap                                                       */
/******************************************************************************/
static void freedelay(delay_list* delay)
{
   if (!delay) return;
   freedelay(delay->NEXT);
   delay->NEXT=HEAP;
   HEAP=delay;
}


/******************************************************************************/
/*  begin of the abl or not, return 1 if first eval                           */
/******************************************************************************/
extern int first_eval()
{
   if (!ALREADY_EVAL) return 1;
   return 0;
}


/******************************************************************************/
/*  last recursion on this abl, free all marks                                */
/******************************************************************************/
extern void free_eval()
{
   freedelay(ALREADY_EVAL);
   ALREADY_EVAL=NULL;
}


/******************************************************************************/
/*  to improve speed do not re-evaluate an abl already done                   */
/******************************************************************************/
static float find_delay(chain_list* abl, float C)
{
   delay_list* delay, *new;
   
   /*search if delay already evaluated*/
   for (delay=ALREADY_EVAL; delay; delay=delay->NEXT) {
      if (delay->ABL==abl && delay->C==C) break;
   }

   /*if found*/
   if (delay) return delay->DELAY;
   
   new=newdelay();
   new->ABL=abl;
   new->C=C;
   new->DELAY=loc_eval_delay(abl,C);
   new->NEXT=ALREADY_EVAL;
   ALREADY_EVAL=new;
   return new->DELAY;
}


/******************************************************************************/
/*   eval opposition of oper1 and oper2                                       */
/* return 1 if opposite leaves are needed                                     */
/******************************************************************************/
static int sign(int oper1, int oper2)
{
   switch (oper1) {
   case ABL_AND: case ABL_NAND:
      switch (oper2) {
      case ABL_AND: case ABL_NAND: return 0;
      case ABL_NOR: case ABL_OR: return 1;
      default: return 0;
      }
   case ABL_NOR: case ABL_OR:
      switch (oper2) {
      case ABL_OR: case ABL_NOR: return 0;
      case ABL_NAND: case ABL_AND: return 1;
      default: return 0;
      }
   case ABL_NXOR: case ABL_XOR:
      switch (oper2) {
      case ABL_XOR: case ABL_NXOR: return 0;
      default: return 0;
      }
   }
   
   return 0;
}


/******************************************************************************/
/* return 0 if not match                                                      */
/******************************************************************************/
static int isablcompatible(chain_list* expr, chain_list* pattern, int negativ)
{
   char * name;

   if (!expr || !pattern) {
      fprintf(stderr,"eval_pattern: NULL pointer\n");
      exit(1);
   }

   /*NOT naturally absorbed*/
   if (!ABL_ATOM(pattern) && ABL_OPER(pattern)==ABL_NOT) {
      return isablcompatible(expr, ABL_CADR(pattern), !negativ);
   }
  
   if (!ABL_ATOM(expr) && ABL_OPER(expr)==ABL_NOT) {
      return isablcompatible(ABL_CADR(expr), pattern, !negativ);
   }
   
   if (ABL_ATOM (pattern) != ABL_ATOM(expr)) return 0;
      
   /*pattern is an atom*/
   if (ABL_ATOM(pattern)) {
      if (negativ) name=getoppositename(ABL_ATOM_VALUE(expr));
      else name=ABL_ATOM_VALUE(expr);
      return name==ABL_ATOM_VALUE(pattern);
   }
   
   /* not the same arity */
   if (ABL_ARITY (expr) != ABL_ARITY (pattern)) return 0;

   /*compare operator*/
   switch (ABL_OPER(pattern)) {
   case ABL_AND: case ABL_NOR:
      switch (ABL_OPER(expr)) {
      case ABL_AND: case ABL_NOR: if (negativ) return 0; break;
      case ABL_NAND: case ABL_OR: if (!negativ) return 0; break;
      default: return 0;
      }
      break;
   case ABL_NAND: case ABL_OR:
      switch (ABL_OPER(expr)) {
      case ABL_AND: case ABL_NOR: if (!negativ) return 0; break;
      case ABL_NAND: case ABL_OR: if (negativ) return 0; break;
      default: return 0;
      }
      break;
   case ABL_XOR:
      switch (ABL_OPER(expr)) {
      case ABL_XOR: if (negativ) return 0; break;
      case ABL_NXOR: if (!negativ) return 0; break;
      default: return 0;
      }
      break;
   case ABL_NXOR:
      switch (ABL_OPER(expr)) {
      case ABL_XOR: if (!negativ) return 0; break;
      case ABL_NXOR: if (negativ) return 0; break;
      default: return 0;
      }
   default: if (ABL_OPER(expr)!=ABL_OPER(pattern)) return 0;   
   }
   
   negativ=sign(ABL_OPER(expr),ABL_OPER(pattern));

   for (pattern = ABL_CDR (pattern); pattern; pattern=ABL_CDR(pattern)) {
     expr = ABL_CDR (expr);
     if (!isablcompatible (ABL_CAR(expr), ABL_CAR(pattern), negativ)) return 0;
   }
  
   return 1;   
}


/******************************************************************************/
/*return number of leaves absorbed by a pattern                               */
/*   if there twice the same value in pattern we should have twice a value on */
/* the same place in expr                                                     */
/* signal inverted values                                                     */
/******************************************************************************/
extern int eval_pattern(chain_list* expr, chain_list* pattern, int negativ)
{
   port_list* port;
   int ret, sum=0;
   char * name;
   chain_list* sav_abl;

   if (!expr || !pattern) {
      fprintf(stderr,"eval_pattern: NULL pointer\n");
      exit(1);
   }

   /*pattern is an atom*/
   if (ABL_ATOM (pattern)) {
      
      /*constants MUST match*/
      if (ABL_ATOM_VALUE(pattern)==getablatomone() 
       || ABL_ATOM_VALUE(pattern)==getablatomzero()) {
         /*search constant*/
         if (!ABL_ATOM(expr) && ABL_OPER(expr)==ABL_NOT) {
          return eval_pattern(ABL_CADR(expr), pattern, !negativ);
         }
         /*search constant*/
         if (!ABL_ATOM(expr)) return 0;
         if (negativ) name=getoppositename(ABL_ATOM_VALUE(expr));
         else name=ABL_ATOM_VALUE(expr);
         return name==ABL_ATOM_VALUE(pattern)?2:0;
      }
      
      /*it is a port*/
      port=(port_list*) ABL_ATOM_VALUE(pattern);
      
      /*try to absorb the last not*/
      if (!ABL_ATOM(expr) && ABL_OPER(expr)==ABL_NOT) {
         sav_abl=port->ABL;
         ret = eval_pattern(ABL_CADR(expr), pattern, !negativ);
         if (ret!=0) return ret;
         /*put back last env if failed*/
         port->ABL=sav_abl;
      }
      
      /*already a leaf on this port*/
      if (port->ABL) {
         /*one case is possible    mux <= x.sel + y.(not sel) */
         if (!isablcompatible(expr,port->ABL,negativ!=port->NEGATIV)) return 0; 
         if (ABL_ATOM(port->ABL) && port->NEGATIV) return 1;  /*we need to insert a NOT*/
         return 2;
      }
      
      port->ABL=expr;
      port->NEGATIV=negativ;
      if (ABL_ATOM(port->ABL) && port->NEGATIV) return 1;  /*we need to insert a NOT*/
      return 2;
   }
   
   /*if pattern is NOT*/
   if (ABL_OPER(pattern)==ABL_NOT) {
      return eval_pattern(expr, ABL_CADR(pattern), !negativ);
   }
  
   /* pattern isn't an atom and expr is*/
   if (ABL_ATOM (expr)) return 0;
   
   /*naturally absorbed*/
   if (ABL_OPER(expr)==ABL_NOT) {
      return eval_pattern(ABL_CADR(expr), pattern, !negativ);
   }
   
   /* not the same arity */
   if (ABL_ARITY (expr) != ABL_ARITY (pattern)) return 0;

   /*compare operator*/
   switch (ABL_OPER(pattern)) {
   case ABL_AND: case ABL_NOR:
      switch (ABL_OPER(expr)) {
      case ABL_AND: case ABL_NOR: if (negativ) return 0; break;
      case ABL_NAND: case ABL_OR: if (!negativ) return 0; break;
      default: return 0;
      }
      break;
   case ABL_NAND: case ABL_OR:
      switch (ABL_OPER(expr)) {
      case ABL_AND: case ABL_NOR: if (!negativ) return 0; break;
      case ABL_NAND: case ABL_OR: if (negativ) return 0; break;
      default: return 0;
      }
      break;
   case ABL_XOR:
      switch (ABL_OPER(expr)) {
      case ABL_XOR: if (negativ) return 0; break;
      case ABL_NXOR: if (!negativ) return 0; break;
      default: return 0;
      }
      break;
   case ABL_NXOR:
      switch (ABL_OPER(expr)) {
      case ABL_XOR: if (!negativ) return 0; break;
      case ABL_NXOR: if (negativ) return 0; break;
      default: return 0;
      }
      break;
   default: if (ABL_OPER(expr)!=ABL_OPER(pattern)) return 0;   
   }
   
   negativ=sign(ABL_OPER(expr),ABL_OPER(pattern));

   for (pattern = ABL_CDR (pattern); pattern; pattern=ABL_CDR(pattern)) {
     expr = ABL_CDR (expr);
     ret=eval_pattern (ABL_CAR(expr), ABL_CAR(pattern), negativ);
     if (ret==0) return 0;
     sum+=ret;
   }
  
   return sum;   
}


/******************************************************************************/
/*            return the cell wich matches expr                               */
/*fulfill the ORDER field of cell with leaves of pattern matching             */
/******************************************************************************/
extern cell_list* cell_pattern(chain_list* expr)
{   
   cell_list* cell, *best_cell=NULL;
   ptype_list* select_cells=NULL, *ptype;
   chain_list* save_port=NULL, *chain;
   float best_delay=MAXFLOAT;
   port_list* port, *best_port = NULL;
   int max_integrated=0, integration;
   float min_area = MAXFLOAT;
   chain_list *not;

   if (!expr) {
      fprintf(stderr,"cell_pattern: NULL pointer\n");
      exit(1);
   }

   /*create a temporary NOT to match perfectly with cell*/
   not=createabloper(ABL_NOT);
   ABL_CDR(not)=addchain(NULL,NULL);
   ABL_ARITY(not)=1;
   
   /*take those are biggest*/
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
       

      /*prepare for use*/
      for (port=cell->PORT ; port; port=port->NEXT) port->ABL=NULL;   

      integration=eval_pattern(expr,cell->ABL, 0);
      
      if (integration==0) continue;
      
      /*take the cell which matches the most of operators*/
      if (integration>=max_integrated) {
         max_integrated=integration; 
         select_cells=addptype(select_cells,integration,cell);
      }
   }
   
   /*optimization in area*/
   if (getoptimlax()<=OPTIM_DELAY1) {
      min_area=MAXFLOAT;
      for (ptype=select_cells; ptype; ptype=ptype->NEXT) {  
         if (ptype->TYPE<select_cells->TYPE) break;
         cell=(cell_list*) ptype->DATA;
         if (cell->AREA<min_area) min_area=cell->AREA;
      }
   }
   
   /*save port from recursive use*/
   for (ptype=select_cells; ptype; ptype=ptype->NEXT) {      
      /*optimization in delay*/
      if (ptype->TYPE<select_cells->TYPE) break;
      cell=(cell_list*) ptype->DATA;
      save_port=addchain(save_port,copyport(cell->PORT));
   }   

   /*put in the same order than select_cells*/
   save_port=reverse(save_port);
   
   /*take the fastest*/
   chain=save_port;
   for (ptype=select_cells; ptype; ptype=ptype->NEXT) {
      float delay, max_delay=0;
      
      cell=(cell_list*) ptype->DATA;
      
      if (ptype->TYPE<select_cells->TYPE) break;
      /*optimization in area*/
      if (getoptimlax()<=OPTIM_DELAY1 && cell->AREA>min_area) {
         /*next port list corresponding with next cell*/
         chain=chain->NEXT;
         continue;
      }

      /*recursion for delay*/
      for (port=(port_list*) chain->DATA; port; port=port->NEXT) {
         if (isvss(port->NAME) || isvdd(port->NAME)) continue;
         switch (port->DIRECTION) {
         case IN: case INOUT: case TRANSCV:
           if (!port->ABL) {
            fprintf(stderr,
            "cell_pattern: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
           }
           if (port->NEGATIV) {
               ABL_CADR(not)=port->ABL;
               delay=port->T+find_delay(not, port->C);
           }
           else delay=port->T+find_delay(port->ABL, port->C);
           if (delay>max_delay) max_delay=delay;
           break;
         }
         if (max_delay>best_delay) break;
      }
         
      if (max_delay>best_delay) continue;
      
      best_delay=max_delay;
      best_port=(port_list*) chain->DATA;      
      best_cell=cell;
      
      /*next port list corresponding with next cell*/
      chain=chain->NEXT;
   }  

   if (!best_cell || (ABL_ATOM(best_cell->ABL) && !ABL_ATOM(expr)) 
    || (ABL_ATOM(expr) && ABL_ATOM(best_cell->ABL) 
        && (ABL_ATOM_VALUE(expr)==getablatomone()
         || ABL_ATOM_VALUE(expr)==getablatomzero())
        &&  ABL_ATOM_VALUE(expr)!=ABL_ATOM_VALUE(best_cell->ABL)) ) {
      fprintf(stderr,"Library Error: No cell could match  '");
      display_abl(expr);
      if (ABL_ATOM(expr)) fprintf(stderr,"'\n");
      else fprintf(stderr,"'    (oper arity=%d)\n",ABL_ARITY(expr));
      exit(1);
   }
   
   /*put back port values*/
   moveport(best_port,best_cell->PORT);
   freeptype(select_cells);
   for (chain=save_port; chain; chain=chain->NEXT) {
      port=(port_list*) chain->DATA;
      delport(port);
   }
   freechain(save_port);
   freechain(not);
  
   /*inverted input*/
   invert_port(best_cell->PORT); /*invert abl if needed*/
   best_cell->DELAY=best_delay;
   return best_cell;
}


/******************************************************************************/
/*               return the cell tristate wich matches biabl                  */
/*fulfill the ORDER field of cell with leaves of pattern matching             */
/******************************************************************************/
extern cell_list* cell_pattern_bus(biabl_list* biabl)
{   
   cell_list* cell, *best_cell=NULL;
   ptype_list* select_cells=NULL, *ptype;
   chain_list* save_port=NULL, *chain;
   float best_delay=MAXFLOAT;
   port_list *port, *best_port = NULL;
   int max_integrated=0, integration;
   int test;
   float min_area = MAXFLOAT;
   biabl_list *biabl_aux, *biabl_cell; 
   chain_list* not;

   /*create a temporary NOT to match perfectly with cell*/
   not=createabloper(ABL_NOT);
   ABL_CDR(not)=addchain(NULL,NULL);
   ABL_ARITY(not)=1;
   
   for (cell=getcell_tristate_lib(); cell; cell=cell->NEXT) {
      /*prepare for use*/
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
      if (integration>=max_integrated) {
         max_integrated=integration; 
         select_cells=addptype(select_cells,integration,cell);
      }
   }
   
   /*optimization in area*/
   if (getoptimlax()<=OPTIM_DELAY1) {
      min_area=MAXFLOAT;
      for (ptype=select_cells; ptype; ptype=ptype->NEXT) {  
         if (ptype->TYPE<select_cells->TYPE) break;
         cell=(cell_list*) ptype->DATA;
         if (cell->AREA<min_area) min_area=cell->AREA;
      }
   }
   
   /*save port from recursive use*/
   for (ptype=select_cells; ptype; ptype=ptype->NEXT) {      
      /*optimization in delay*/
      if (ptype->TYPE<select_cells->TYPE) break;
      cell=(cell_list*) ptype->DATA;
      save_port=addchain(save_port,copyport(cell->PORT));
   }   

   /*put in the same order than select_cells*/
   save_port=reverse(save_port);
   
   /*take the fastest*/
   chain=save_port;
   for (ptype=select_cells; ptype; ptype=ptype->NEXT) {
      float delay, max_delay=0;
      
      cell=(cell_list*) ptype->DATA;
      
      if (ptype->TYPE<select_cells->TYPE) break;
      /*optimization in area*/
      if (getoptimlax()<=OPTIM_DELAY1 && cell->AREA>min_area) {
         /*next port list corresponding with next cell*/
         chain=chain->NEXT;
         continue;
      }
      
      /*recursion for delay*/
      for (port=(port_list*) chain->DATA; port; port=port->NEXT) {
         if (isvss(port->NAME) || isvdd(port->NAME)) continue;
         switch (port->DIRECTION) {
         case IN: case INOUT: case TRANSCV:
           if (!port->ABL) {
            fprintf(stderr,
            "cell_pattern: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
           }
           if (port->NEGATIV) {
               ABL_CADR(not)=port->ABL;
               delay=port->T+find_delay(not, port->C);
           }
           else delay=port->T+find_delay(port->ABL, port->C);
           if (delay>max_delay) max_delay=delay;
           break;
         }
         if (max_delay>best_delay) break;
      }
         
      if (max_delay>best_delay) continue;
      
      best_delay=max_delay;
      best_port=(port_list*) chain->DATA;      
      best_cell=cell;
      
      /*next port list corresponding with next cell*/
      chain=chain->NEXT;
   }  


   if (!best_cell) {
      fprintf(stderr,"Library Error: No cell could match\n");
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

   /*put back port values*/
   moveport(best_port,best_cell->PORT);
   freeptype(select_cells);
   for (chain=save_port; chain; chain=chain->NEXT) {
      port=(port_list*) chain->DATA;
      delport(port);
   }
   freechain(save_port);
   freechain(not);
   
   /*inverted input*/
   invert_port(best_cell->PORT); /*invert abl if needed*/
   best_cell->DELAY=best_delay;
   return best_cell;
}


/******************************************************************************/
/*   return the cell register which matches biabl                             */
/*fulfill the ORDER field of cell with leaves of pattern matching             */
/******************************************************************************/
extern cell_list* cell_pattern_reg(biabl_list* biabl)
{   
   cell_list* cell, *best_cell=NULL;
   port_list *port, *best_port = NULL;
   ptype_list* select_cells=NULL, *ptype;
   chain_list* save_port=NULL, *chain;
   float best_delay=MAXFLOAT;
   int max_integrated=0, integration;
   int test;
   float min_area = MAXFLOAT;
   biabl_list *biabl_aux, *biabl_cell; 
   chain_list *not;
  
   /*create a temporary NOT to match perfectly with cell*/
   not=createabloper(ABL_NOT);
   ABL_CDR(not)=addchain(NULL,NULL);
   ABL_ARITY(not)=1;
   
   cell=getcell_register_lib();

   for ( ; cell; cell=cell->NEXT) {
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
      if (integration>=max_integrated) {
         max_integrated=integration; 
         select_cells=addptype(select_cells,integration,cell);
      }
   }
   
   /*optimization in area*/
   if (getoptimlax()<=OPTIM_DELAY1) {
      min_area=MAXFLOAT;
      for (ptype=select_cells; ptype; ptype=ptype->NEXT) {  
         if (ptype->TYPE<select_cells->TYPE) break;
         cell=(cell_list*) ptype->DATA;
         if (cell->AREA<min_area) min_area=cell->AREA;
      }
   }
   
   /*save port from recursive use*/
   for (ptype=select_cells; ptype; ptype=ptype->NEXT) {      
      /*optimization in delay*/
      if (ptype->TYPE<select_cells->TYPE) break;
      cell=(cell_list*) ptype->DATA;
      save_port=addchain(save_port,copyport(cell->PORT));
   }   

   /*put in the same order than select_cells*/
   save_port=reverse(save_port);
   
   /*take the fastest*/
   chain=save_port;
   for (ptype=select_cells; ptype; ptype=ptype->NEXT) {
      float delay, max_delay=0;
      
      cell=(cell_list*) ptype->DATA;
      
      if (ptype->TYPE<select_cells->TYPE) break;
      /*optimization in area*/
      if (getoptimlax()<=OPTIM_DELAY1 && cell->AREA>min_area) {
         /*next port list corresponding with next cell*/
         chain=chain->NEXT;
         continue;
      }
      
      /*recursion for delay*/
      for (port=(port_list*) chain->DATA; port; port=port->NEXT) {
         if (isvss(port->NAME) || isvdd(port->NAME)) continue;
         switch (port->DIRECTION) {
         case IN: case INOUT: case TRANSCV:
           if (!port->ABL) {
            fprintf(stderr,
            "cell_pattern: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
           }
           if (port->NEGATIV) {
               ABL_CADR(not)=port->ABL;
               delay=port->T+find_delay(not, port->C);
           }
           else delay=port->T+find_delay(port->ABL, port->C);
           if (delay>max_delay) max_delay=delay;
           break;
         }
         if (max_delay>best_delay) break;
      }
         
      if (max_delay>best_delay) continue;
      
      best_delay=max_delay;
      best_port=(port_list*) chain->DATA;      
      best_cell=cell;
      
      /*next port list corresponding with next cell*/
      chain=chain->NEXT;
   }  

   if (!best_cell) {
      fprintf(stderr,"Library Error: No cell could match\n");
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

   /*put back port values*/
   moveport(best_port,best_cell->PORT);
   freeptype(select_cells);
   for (chain=save_port; chain; chain=chain->NEXT) {
      port=(port_list*) chain->DATA;
      delport(port);
   }
   freechain(save_port);
   freechain(not);
  
   /*inverted input*/
   invert_port(best_cell->PORT); /*invert abl if needed*/
   best_cell->DELAY=best_delay;
   return best_cell;
}




/************************************************************************/
/* to locate easier the port of bepor in the abl for later use          */
/*   do pointing the leaf of abl on the elements of port and sorted     */
/*   like bepor                                                         */
/* returning an abl  with each leaf pointing on one port element        */
/************************************************************************/
extern chain_list* build_reference(befig_list* befig, port_list* port, port_list* internal_port, chain_list* abl)
{
   chain_list* chain;
   bepor_list* bepor;
   port_list* port_aux;
   
   if (ABL_ATOM(abl)) {
      /*constante not touched*/
      if (ABL_ATOM_VALUE(abl)==getablatomone()
       || ABL_ATOM_VALUE(abl)==getablatomzero() 
       || ABL_ATOM_VALUE(abl)==getablatomtristate()
       || ABL_ATOM_VALUE(abl)==getablatomdc()) return abl;
      
      /*internal reference already created*/
      for (port_aux=internal_port; port_aux; port_aux=port_aux->NEXT) {
         if (port_aux==(port_list*)ABL_ATOM_VALUE(abl)) return abl;
      }
      
      /*create reference for port*/
      for (bepor=befig->BEPOR; bepor && port; bepor=bepor->NEXT) {
         if (bepor->NAME==ABL_ATOM_VALUE(abl)) break;
         port=port->NEXT;
      }
      if (!bepor || !port) {
         fprintf(stderr,"build_reference: '%s' not found in port in cell '%s'\n",
                 ABL_ATOM_VALUE(abl),befig->NAME);
         exit(1);
      }

      /*refer to new port*/
      ABL_ATOM_VALUE(abl)=(chain_list*)port;
      return abl;   /*head unchanged*/
   }

   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      ABL_CAR(chain)=build_reference(befig,port,internal_port,ABL_CAR(chain));
   }

   /*mark if STABLE*/
   if (ABL_OPER(abl)==ABL_STABLE) {
      port= (port_list*) ABL_ATOM_VALUE(ABL_CADR(abl));
      port->STABLE=1;
   }
   
   return abl;   /*head unchanged*/
}



/************************************************************************/
/* create a pseudo port, if an internal register is used in abl         */
/* return the list of pseudo port                                       */
/************************************************************************/
extern port_list* internal_reference(befig_list* befig, port_list* port, chain_list* abl)
{
   chain_list* chain;
   bereg_list* bereg;
   port_list*  port_aux;
   
   if (ABL_ATOM(abl)) {
      /*constante not touched*/
      if (ABL_ATOM_VALUE(abl)==getablatomone()
       || ABL_ATOM_VALUE(abl)==getablatomzero() 
       || ABL_ATOM_VALUE(abl)==getablatomtristate()
       || ABL_ATOM_VALUE(abl)==getablatomdc()) return port;
      
      for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
         if (bereg->NAME==ABL_ATOM_VALUE(abl)) break;
      }
      if (!bereg) return port;
      
      /*check if internal reference already exists*/
      for (port_aux=port; port_aux; port_aux=port_aux->NEXT) {
         if (port_aux->NAME==bereg->NAME) return port;
      }

      /*create a new internal port*/
      port_aux=newport();
      port_aux->NEXT=port;
      port_aux->NAME=bereg->NAME;
      port_aux->DIRECTION=INOUT;
      
      /*refer to new port*/
      ABL_ATOM_VALUE(abl)=(chain_list*)port_aux;
      return port_aux;   
   }

   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      port=internal_reference(befig,port,ABL_CAR(chain));
   }

   return port;   
}


