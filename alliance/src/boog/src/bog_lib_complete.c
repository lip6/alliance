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
 * Tool    : BooG - library completion
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_lax_param.h"
#include "bog_normalize_ARITY.h"
#include "bog_lib_specifications.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_map_befig.h"
#include "bog_map_pattern.h"
#include "bog_signal_utils.h"
#include "bog_lib_complete.h"


/*to memorize kind of operator: dual cells with different arities*/
typedef struct dual{
   struct dual *NEXT;
   int         OPER;    /*oper expected*/
   int         ARITY;
   cell_list   *CELL;
   cell_list   *NEG_CELL;   /*negation of expected*/
} dual_list;


/*simple operator  n1_y, a2_y, a3_y, xor, or, nand3, nxr2 ...*/
static cell_list* SIMPLE_OPER;



/******************************************************************************/
/*  add a cell in a sorted list of dual cells                                 */
/******************************************************************************/
static dual_list* add_dual(dual_list* top, cell_list* cell)
{
   dual_list* dual, *pred=NULL, *new;
   
   /*search place*/
   for (dual=top; dual; dual=dual->NEXT) {
      if (dual->ARITY>=ABL_ARITY(cell->ABL)) break;
      pred=dual;
   } 

   /*insert if possible*/
   if (dual && dual->ARITY==ABL_ARITY(cell->ABL)) {
      /*not the operator wanted!*/
      if (dual->OPER!=ABL_OPER(cell->ABL)
      && (!dual->CELL || ABL_OPER(cell->ABL)!=ABL_OPER(dual->CELL->ABL))){
         fprintf(stderr,
         "add_dual: computing error on cell with oper %s and %s (arity %ld)\n",
         getabloperuppername(ABL_OPER(cell->ABL)),
         getabloperuppername(dual->OPER),ABL_ARITY(cell->ABL));
         exit(1);
      }
      
      /*memo new cell*/
      if (dual->OPER==ABL_OPER(cell->ABL)) {
         /*if already one take the littlest*/
         if (!dual->NEG_CELL || cell->AREA<dual->NEG_CELL->AREA) 
            dual->NEG_CELL=cell;
      }      
      else {
         /*if already one take the littlest*/
         if (!dual->CELL || cell->AREA<dual->CELL->AREA) 
            dual->CELL=cell;
      }
      return top;
   }
   
   new=mbkalloc(sizeof(dual_list));
   new->NEXT=dual;
   new->NEG_CELL=NULL;
   new->CELL=cell;
   new->ARITY=ABL_ARITY(cell->ABL);
   switch (ABL_OPER(cell->ABL)) {                     
      case ABL_AND: new->OPER=ABL_NAND; break;              /*oper expected*/
      case ABL_NAND: new->OPER=ABL_AND; break;
      case ABL_OR: new->OPER=ABL_NOR; break;
      case ABL_NOR: new->OPER=ABL_OR; break;
      case ABL_XOR: new->OPER=ABL_NXOR; break;
      case ABL_NXOR: new->OPER=ABL_XOR; break;
   }

   if (!pred) return new;
   pred->NEXT=new;
   return top;
}


/******************************************************************************/
/* free recursively a list of dual cell                                       */
/******************************************************************************/
static void free_dual(dual_list* top)
{
   if (!top) return;
   free_dual(top->NEXT);
   mbkfree(top);
}


/******************************************************************************/
/*to improve speed of unflattening algorithm                                  */
/*isolate simple operator                                                     */
/******************************************************************************/
static void simpleCell(cell_list* cell)
{
   chain_list* abl;
   cell_list* new, *list;

   /*only pure logic*/
   if (cell->BIABL) return;
   /*no constant*/
   if (ABL_ATOM(cell->ABL)) return;

   /*control all leaves are atomic*/
   for (abl=ABL_CDR(cell->ABL); abl; abl=ABL_CDR(abl)) {
      if (!ABL_ATOM(ABL_CAR(abl))) return;
      /*constant forbidden*/
      if (ABL_ATOM_VALUE(ABL_CAR(abl))==getablatomone()) return;
      if (ABL_ATOM_VALUE(ABL_CAR(abl))==getablatomzero()) return;
   }
   
   new=copyCell(cell);
   /*sort list (return NULL if not inserted)*/
   list=classCell(new,SIMPLE_OPER);
   if (list) SIMPLE_OPER=list;
}


/***************************************************************************/
/*                 return the area of cells matching abl                   */
/***************************************************************************/
static int addgeneric(chain_list *abl, befig_list* befig, int area, float R, float C, float T)
{
   port_list* port;
   port_list* top;
   cell_list* cell;

   if ( !abl )
   {
      fprintf(stderr,"addgeneric() : NULL pointer\n");
      exit(1);
   }

   if (ABL_ATOM(abl)) {
      if (ABL_ATOM_VALUE(abl)!=getablatomone() 
       && ABL_ATOM_VALUE(abl)!=getablatomzero()) { 
         putgenericT(befig,ABL_ATOM_VALUE(abl),T,T,T,T);
         putgenericR(befig,ABL_ATOM_VALUE(abl),R,R);
         putgenericC(befig,ABL_ATOM_VALUE(abl),C);
      }
      return area;
   }

   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern(abl);

   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME) || isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE:
         /*no delay for output*/
         break;
      case IN: case INOUT: case TRANSCV:
        area=addgeneric(port->ABL, befig, area, R==0?port->R/*gate on top*/:R,
                     port->C /*last gate concerned*/,  T + port->T + port->R*C);
        break;
      }
   }
   
   delport(top);

   return area+cell->AREA;
}


/******************************************************************************/
/* put the bepor with the leaf of abl                                         */
/******************************************************************************/
static void addinput(befig_list* befig, chain_list *abl)
{
   bepor_list* bepor;
   
   if (!befig || !abl) {
      fprintf(stderr,"addinput: NULL pointer\n");
      exit(1);
   }

   if (ABL_ATOM(abl)) {
      if (ABL_ATOM_VALUE(abl)==getablatomone()) return;
      if (ABL_ATOM_VALUE(abl)==getablatomzero()) return;
      /*already defined*/
      for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
         if (bepor->NAME==ABL_ATOM_VALUE(abl)) return;
      }
      befig->BEPOR=beh_addbepor(befig->BEPOR,ABL_ATOM_VALUE(abl),IN,'B');
      befig->BERIN=beh_addberin(befig->BERIN,ABL_ATOM_VALUE(abl));
      return;
   }
   
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      addinput(befig,ABL_CAR(abl));
   }
}


/******************************************************************************/
/*    return a befig with abl_out for output and without beaux                */
/* build also a lofig to use as if it was a cell in library                   */
/*when you build a cell with this befig to complete a library, you must put   */
/*the flag MODE of the cell resulting to 'A' (for later mapping)              */
/******************************************************************************/
static befig_list* complete_lib(char* name, chain_list* abl_out)
{
   befig_list* befig;
   char *out_name;
   int area;
   bepor_list* bepor;
   beaux_list* beaux;
   bebux_list* bebux;
   bereg_list* bereg;
   beout_list* beout;

   if (!name || !abl_out) {
      fprintf(stderr,"complete_lib: NULL pointer\n");
      exit(1);
   }

   name=namealloc(name);
   befig=beh_addbefig(NULL,name);
    
   out_name=namealloc("f");
   befig->BEOUT=beh_addbeout(NULL,out_name,abl_out,NULL);

   /*put arity oper*/
   put_arity(befig);
      
   befig->BEPOR=beh_addbepor(befig->BEPOR,VSS,IN,'B');
   befig->BEPOR=beh_addbepor(befig->BEPOR,VDD,IN,'B');
   befig->BEPOR=beh_addbepor(befig->BEPOR,out_name,OUT,'B');
   /*all inputs*/
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      addinput(befig,beout->ABL);
   }
   
   /*define all signals*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) putdelay(bepor->NAME,0);
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) putdelay(beaux->NAME,0);
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) putdelay(bereg->NAME,0);
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) putdelay(bebux->NAME,0);
   putdelay(getablatomone(),0);
   putdelay(getablatomzero(),0);
   
   
   area=addgeneric(befig->BEOUT->ABL,befig,0,0,0,0);
   putgenericarea(befig,area);
   
  
   /*create a lofig*/
   map_befig(befig,befig->NAME);
   
   /*undefine all signals not to interfer with work*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) undefdelay(bepor->NAME);
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) undefdelay(beaux->NAME);
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) undefdelay(bereg->NAME);
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) undefdelay(bebux->NAME);
   undefdelay(getablatomone());
   undefdelay(getablatomzero());
   
   return befig;
}


/******************************************************************************/
/*          return a cell with the opposite behave of cell                    */
/******************************************************************************/
static cell_list* complete_neg_lib(cell_list* cell)
{
   chain_list *out_abl, *abl;
   char *name;
   char memo;
   befig_list *befig, *new;
   
   if (!cell) {
      fprintf(stderr,"complete_neg_lib: NULL pointer\n");
      exit(1);
   }
   
   befig=cell->BEFIG;
   
   if (!befig || !befig->BEOUT || !befig->BEOUT->ABL) {
      fprintf(stderr,"complete_neg_lib: NULL pointer\n");
      exit(1);
   }
   
   out_abl=createablnotexpr(dupablexpr(befig->BEOUT->ABL));
   
   memo=SEPAR;
   SEPAR='_';
   name=concatname("not",cell->NAME);
   SEPAR=memo;
   
   new=complete_lib(name,out_abl);

   abl=dupablexpr(createablnotexpr(befig->BEOUT->ABL));  /* f<= not  oper (i)  */ 
   abl=simpablexpr(abl);                             /* f<= noper (i)      */  
   put_arity_abl(abl);
   
   /*put in library*/
   return addCell_logic(new,abl);
}


/******************************************************************************/
/*          return a cell generating XOR                                      */
/******************************************************************************/
static cell_list* complete_xor_lib()
{
   chain_list *out_abl, *abl1, *abl2, *abl;
   chain_list *i0, *i1, *ni1, *ni0;
   befig_list  *new;
   
   i0=createablatom(namealloc("i0"));
   i1=createablatom(namealloc("i1"));
   ni0=createablnotexpr(dupablexpr(i0));
   ni1=createablnotexpr(dupablexpr(i1));
   
   abl1=createablbinexpr(ABL_NOR,i0,ni1);
   abl2=createablbinexpr(ABL_NOR,ni0,i1);
   out_abl=createablbinexpr(ABL_NAND,abl1,abl2);
   
   /*create befig and lofig*/
   new=complete_lib(namealloc("xr2_y"),out_abl);

   abl=createablbinexpr(ABL_XOR, createablatom(namealloc("i0")),
                                 createablatom(namealloc("i1")));
                                 
   put_arity_abl(abl);
   /*put in library*/
   return addCell_logic(new,abl);
}


/******************************************************************************/
/*          return a cell generating NXOR                                     */
/******************************************************************************/
static cell_list* complete_nxor_lib()
{
   chain_list *out_abl, *abl1, *abl2, *abl;
   chain_list *i0, *i1, *ni1, *ni0;
   befig_list  *new;
   
   i0=createablatom(namealloc("i0"));
   i1=createablatom(namealloc("i1"));
   ni0=createablnotexpr(dupablexpr(i0));
   ni1=createablnotexpr(dupablexpr(i1));
   
   abl1=createablbinexpr(ABL_NOR,i0,ni1);
   abl2=createablbinexpr(ABL_NOR,ni0,i1);
   out_abl=createablbinexpr(ABL_AND,abl1,abl2);
   
   /*create befig and lofig*/
   new=complete_lib(namealloc("nxr2_y"),out_abl);

   abl=createablbinexpr(ABL_XOR, createablatom(namealloc("i0")),
                                 createablatom(namealloc("i1")));
   
   put_arity_abl(abl);
   /*put in library*/
   return addCell_logic(new,abl);
}


/******************************************************************************/
/*          control if there is a NOT, a AND, a OR                            */
/*  complete the library if some cells are missing                            */
/******************************************************************************/
extern void control_lib()
{
   cell_list* not=NULL; /*flag*/
   cell_list* buff=NULL;   /*flag*/
   cell_list* one=NULL;   /*flag*/
   cell_list* zero=NULL;   /*flag*/
   dual_list* or=NULL;   
   dual_list* and=NULL;  
   dual_list* xor=NULL;  
   dual_list* dual;
   cell_list* cell;
   int arity;  
   
   for (cell=getcell_logic_lib(); cell; cell=cell->NEXT) {
      simpleCell(cell);       /*build SIMPLE_OPER variable*/
      if (ABL_ATOM(cell->ABL)) {
         if (ABL_ATOM_VALUE(cell->ABL)==getablatomone()) one=cell;
         else if (ABL_ATOM_VALUE(cell->ABL)==getablatomzero()) zero=cell;
         else buff=cell;
      }
   }
   
   for (cell=SIMPLE_OPER; cell; cell=cell->NEXT) {
      switch ABL_OPER(cell->ABL) {
         case ABL_AND: case ABL_NAND: and=add_dual(and,cell); break;
         case ABL_NOR: case ABL_OR: or=add_dual(or,cell); break;
         case ABL_XOR: case ABL_NXOR: xor=add_dual(xor,cell); break;
         case ABL_NOT: not=cell; break;
         default: 
            fprintf(stderr,"control_lib: unknow oper %ld\n",
            ABL_OPER(cell->ABL));
            exit(1);
      }
   }

   if (!one) {
      fprintf(stderr,
      "Mapping Warning: 'ONE' constant cell is missing in library\n");
   }
   if (!zero) {
      fprintf(stderr,
      "Mapping Warning: 'ZERO' constant cell is missing in library\n");
   }
   if (!not) {
      fprintf(stderr,"Library Error: 'NOT' cell is missing in library\n");
      exit(1);
   }
   
   if (!or) {
      fprintf(stderr,"Library Error: 'OR' cell is missing in library\n");
      exit(1);
   }
   else if (or->ARITY!=2) {
      fprintf(stderr,
      "Library Error: 'OR 2' is missing in library\n");
      exit(1);
   }

   if (!and) {
      fprintf(stderr,"Library Error: 'AND' cell is missing in library\n");
      exit(1);
   }
   else if (and->ARITY!=2) {
      fprintf(stderr,
      "Library Error: 'AND 2' is missing in library\n");
      exit(1);
   }
   
   if (xor && xor->ARITY!=2) {
      fprintf(stderr,
      "Library Error: 'XOR 2' is missing in library\n");
      exit(1);
   }

   arity=2;
   for (dual=and; dual; dual=dual->NEXT) {
      if (!dual->NEG_CELL) {
         fprintf(stderr,
         "Mapping Warning: a '%s%d' is missing in cell library...Generating\n",
         getabloperuppername(dual->OPER), dual->ARITY);
         /*build a buffer with 2 inverters*/
         cell=complete_neg_lib(dual->CELL);    
         if (cell) {
            cell->MODE='A';      /*doesn't really exist in lib*/
            simpleCell(cell);       /*build SIMPLE_OPER variable*/
         }
      }
      arity++;
   }

   arity=2;
   for (dual=or; dual; dual=dual->NEXT) {
      if (!dual->NEG_CELL) {
         fprintf(stderr,
         "Mapping Warning: a '%s%d' is missing in cell library...Generating\n",
         getabloperuppername(dual->OPER), dual->ARITY);
         /*build a buffer with 2 inverters*/
         cell=complete_neg_lib(dual->CELL);    
         if (cell) {
            cell->MODE='A';      /*doesn't really exist in lib*/
            simpleCell(cell);       /*build SIMPLE_OPER variable*/
         }
      }
      arity++;
   }
   
   arity=2;
   for (dual=xor; dual; dual=dual->NEXT) {
      if (!dual->NEG_CELL) {
         fprintf(stderr,
         "Mapping Warning: a '%s%d' is missing in cell library...Generating\n",
         getabloperuppername(dual->OPER), dual->ARITY);
         /*build a buffer with 2 inverters*/
         cell=complete_neg_lib(dual->CELL);    
         if (cell) {
            cell->MODE='A';      /*doesn't really exist in lib*/
            simpleCell(cell);       /*build SIMPLE_OPER variable*/
         }
      }
      arity++;
   }

   /*if buffer doesn't exist, build*/
   if (!buff) {
      fprintf(stderr,
      "Mapping Warning: a buffer is missing in cell library...Generating\n");
      /*build a buffer with 2 inverters*/
      cell=complete_neg_lib(not);    
      if (cell) {
         cell->MODE='A';      /*doesn't really exist in lib*/
         simpleCell(cell);       /*build SIMPLE_OPER variable*/
      }
   }

   /*if xor doesn't exist, build*/
   if (xor == NULL) {
      fprintf(stderr,
      "Mapping Warning: 'xor' is missing in cell library...Generating\n");
      cell=complete_xor_lib();    
      if (cell) {
         cell->MODE='A';      /*doesn't really exist in lib*/
         simpleCell(cell);       /*build SIMPLE_OPER variable*/
      }
      cell=complete_nxor_lib();    
      if (cell) {
         cell->MODE='A';      /*doesn't really exist in lib*/
         simpleCell(cell);       /*build SIMPLE_OPER variable*/
      }
   }   

   free_dual(xor);
   free_dual(and);
   free_dual(or);
}


/***************************************************************************/
/*   return the list of simple oper of the library                         */
/***************************************************************************/
extern cell_list* getcell_oper_lib()
{
   return SIMPLE_OPER;
}





