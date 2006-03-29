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
 * Tool    : BooG - evaluate Capacitance
 * Date    : 2000
 * Author  : Francois Donnet
 */

#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_lib_utils.h"
#include "bog_lib_negativ.h"
#include "bog_lib_cell.h"
#include "bog_signal_utils.h"
#include "bog_signal_nameindex.h"
#include "bog_normalize_message.h"
#include "bog_normalize_ARITY.h"
#include "bog_map_prepare.h"
#include "bog_map_adapt.h"


/******************************************************************************/
/* swap the contain of 2 abl                                                  */
/******************************************************************************/
extern void swap_pointers(chain_list* abl1, chain_list* abl2)
{
   chain_list* cdr, *car;
   
   car=ABL_CAR(abl1);
   cdr=ABL_CDR(abl1);
   ABL_CAR_L(abl1)=ABL_CAR(abl2);
   ABL_CDR(abl1)=ABL_CDR(abl2);
   ABL_CAR_L(abl2)=car;
   ABL_CDR(abl2)=cdr;
}


/******************************************************************************/
/* invert abl of port whose NEGATIV flag is set                               */
/******************************************************************************/
extern void invert_port(port_list* port)
{
   chain_list *not;
   char *name;
   
   /*inverted input*/
   for ( ; port; port=port->NEXT) {
      if (!port->NEGATIV) continue;
      if (isvss(port->NAME) || isvdd(port->NAME)) continue;
      if (port->DIRECTION!=IN && port->DIRECTION!=INOUT && port->DIRECTION!=TRANSCV)
         continue;

      port->NEGATIV=0;
     
#if 1

      if (ABL_ATOM(port->ABL)) {
        name=getoppositename(ABL_ATOM_VALUE(port->ABL));
        if (is_signal(name)) ABL_CAR_L(port->ABL)=name;
        else 
        {
          /*add a not*/
          not=createabloper(ABL_NOT);
          ABL_ARITY_L(not)=1;
          /*swap pointers references*/
          swap_pointers(port->ABL,not);
          ABL_CDR(port->ABL)=addchain(NULL,not);
        }
      }
      else  
      {
        switch (ABL_OPER(port->ABL)) {
        case ABL_AND: ABL_OPER_L(port->ABL)=ABL_NAND; break;
        case ABL_NAND: ABL_OPER_L(port->ABL)=ABL_AND; break;
        case ABL_OR: ABL_OPER_L(port->ABL)=ABL_NOR; break;
        case ABL_NOR: ABL_OPER_L(port->ABL)=ABL_OR; break;
        case ABL_XOR: ABL_OPER_L(port->ABL)=ABL_NXOR; break;
        case ABL_NXOR: ABL_OPER_L(port->ABL)=ABL_XOR; break;
        case ABL_NOT: /*no need to insert*/
            port->ABL=ABL_CADR_L(port->ABL); continue;
        default:
            fprintf(stderr,
            "invert_port: oper %s forbidden at this level\n",
            getabloperuppername(ABL_OPER(port->ABL)));
            exit(1);
        } 
      }
      
      /*insert a NOT to match perfectly with cell*/
      not=createabloper(ABL_NOT);
      ABL_ARITY_L(not)=1;
      /*swap pointers references*/
      swap_pointers(port->ABL,not);
      ABL_CDR(port->ABL)=addchain(NULL,not);

      /* port->ABL = not( not a )   ---->  port->ABL = not a   */
      port->ABL=not;

#else

      port->ABL=createablnotexpr(port->ABL);
      ABL_ARITY(port->ABL)=1;
      
#endif

   }
}


/******************************************************************************/
/* adapt expr to match cell library                                           */
/* if negativ is non zero opposite value if expr is wanted                    */
/* evaluate capacitance of leaves                                             */
/******************************************************************************/
static chain_list* loc_adapt_abl(chain_list* expr, float C)
{   
   cell_list* cell;
   port_list* port, *top;
   chain_list* abl;

   if (!expr) {
      fprintf(stderr,"loc_adapt_abl: NULL pointer\n");
      exit(1);
   }

   /*do not need to insert a buffer*/
   if (ABL_ATOM(expr)) {
      inccapacitance(ABL_ATOM_VALUE(expr),C);
      return expr;
   }
   
   cell=cell_prepare(expr);

   /*arity is too big to be matched*/
   if (!cell) {   
      /*evaluate with the biggest oper*/
      int arity=ABL_ARITY(expr);  /*memorize arity*/
      /*search the biggest arity which matches expr*/
      for (ABL_ARITY_L(expr)=ABL_ARITY(expr)-1 ; ABL_ARITY(expr)>0; 
      ABL_ARITY_L(expr)--) {
         cell=cell_prepare(expr);
         if (cell) break;
      }
      ABL_ARITY_L(expr)=arity;   /*put back arity*/
      if (!cell) {
         fprintf(stderr,"Library Error: No cell could match  '");
         display_abl(expr);
         if (ABL_ATOM(expr)) fprintf(stderr,"'\n");
         else fprintf(stderr,"'    (oper arity=%d)\n",ABL_ARITY(expr));
         exit(1);      
      }
      
      /*take the first value*/
      for (port=cell->PORT; port; port=port->NEXT) if (port->C!=0) break;
      if (!port)
      {
	 return expr;
      }
      for (abl=ABL_CDR(expr); abl; abl=ABL_CDR(abl)) {
         ABL_CAR_L(abl)=loc_adapt_abl(ABL_CAR(abl),port->C);
      }
      return expr;
   }
   
   top=copyport(cell->PORT);  /*because this port can be used later*/
   
   for (port=top; port; port=port->NEXT) {
    if (isvss(port->NAME) || isvdd(port->NAME)) continue;
    else switch (port->DIRECTION) {
    case OUT: case TRISTATE: break;
    default: 
      if (!port->ABL) {
         fprintf(stderr,
         "loc_adapt_abl: compute error %s not really match on port %s\n",
         cell->NAME,port->NAME);
         exit(1);
      }
      port->ABL=loc_adapt_abl(port->ABL,port->C);
    }  
   }   

   delport(top);
   return expr;
}


/******************************************************************************/
/* adapt expr to match cell library                                           */
/* if negativ is non zero opposite value if expr is wanted                    */
/* evaluate capacitance of leaves                                             */
/******************************************************************************/
extern chain_list* adapt_abl(chain_list* expr)
{   
   cell_list* cell;
   port_list* port, *top;
   chain_list* abl;

   if (!expr) {
      fprintf(stderr,"adapt_abl: NULL pointer\n");
      exit(1);
   }

   cell=cell_prepare(expr);

   /*arity is too big to be matched*/
   if (!cell) {   
      int arity;

      if (ABL_ATOM(expr)) {
         /*no buffer?!!!*/
         fprintf(stderr,"Library Error: No cell could match  '");
         display_abl(expr);
         fprintf(stderr,"'\n");
         exit(1);
      }
      
      /*evaluate with the biggest oper*/
      arity=ABL_ARITY(expr);  /*memorize arity*/
      /*search the biggest arity which matches expr*/
      for (ABL_ARITY_L(expr)=ABL_ARITY(expr)-1 ; ABL_ARITY(expr)>0; 
      ABL_ARITY_L(expr)--) {
         cell=cell_prepare(expr);
         if (cell) break;
      }
      ABL_ARITY_L(expr)=arity;   /*put back arity*/
      if (!cell) {
         fprintf(stderr,"Library Error: No cell could match  '");
         display_abl(expr);
         fprintf(stderr,"'    (oper arity=%d)\n",ABL_ARITY(expr));
         exit(1);      
      }
      
      /*take the first value*/
      for (port=cell->PORT; port; port=port->NEXT) if (port->C!=0) break;
      if (!port)
      {
   	return expr;
      }
      for (abl=ABL_CDR(expr); abl; abl=ABL_CDR(abl)) {
         ABL_CAR_L(abl)=loc_adapt_abl(ABL_CAR(abl),port->C);
      }
      return expr;
   }
   
   
   top=copyport(cell->PORT);  /*because this port can be used later*/
   
   for (port=top; port; port=port->NEXT) {
    if (isvss(port->NAME) || isvdd(port->NAME)) continue;
    else switch (port->DIRECTION) {
    case OUT: case TRISTATE: break;
    default: 
      if (!port->ABL) {
         fprintf(stderr,
         "adapt_abl: compute error %s not really match on port %s\n",
         cell->NAME,port->NAME);
         exit(1);
      }
      port->ABL=loc_adapt_abl(port->ABL,port->C);
    }  
   }   

   delport(top);
   return expr;
}


/******************************************************************************/
/* adapt biabl to match cell library                                          */
/* evaluate capacitance of leaves                                             */
/******************************************************************************/
extern biabl_list* adapt_bus(biabl_list* biabl)
{   
   cell_list* cell;
   port_list *port, *top;
   biabl_list* biabl_aux, *biabl_head;

   if (!biabl) {
      fprintf(stderr,"adapt_bus: NULL pointer\n");
      exit(1);
   }

   biabl_head=biabl;
   
   /*check if there is a tristate inverter in library*/
   /* is-it an advantage to use a tristate inverter here? */
   if (is_tristate_inverter_lib() &&
    !ABL_ATOM(biabl->VALABL) && ABL_OPER(biabl->VALABL)!=ABL_NOT) {
      /*put a NOT in front of expression to be mapped by bus inverter*/
      biabl->VALABL=inv_oper(biabl->VALABL,1);
      biabl->VALABL=build_negativ(biabl->VALABL);  
      biabl->VALABL=createablnotexpr(biabl->VALABL);
      /* createablnotexpr() can simplify*/
      if (!ABL_ATOM(biabl->VALABL)) ABL_ARITY_L(biabl->VALABL)=1;
   }

   cell=cell_prepare_bus(biabl);
   
   top=copyport(cell->PORT);  /*because this port can be used later*/
   
   for (port=top; port; port=port->NEXT) {
    if (isvss(port->NAME) || isvdd(port->NAME)) continue;
    else switch (port->DIRECTION) {
    case OUT: case TRISTATE: break;
    default: 
      if (!port->ABL) {
         fprintf(stderr,
         "adapt_bus: compute error %s not really match on port %s\n",
         cell->NAME,port->NAME);
         exit(1);
      }
      port->ABL=loc_adapt_abl(port->ABL,port->C);
    }  
   }   

   delport(top);
   
   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
   /*look the other blocks*/
   if (biabl) {
      adapt_bus(biabl);
   }
   
   return biabl_head;
}


/******************************************************************************/
/* adapt biabl to match cell library                                          */
/* evaluate capacitance of leaves                                             */
/******************************************************************************/
extern biabl_list* adapt_reg(biabl_list* biabl)
{   
   cell_list* cell;
   port_list *port, *top;
   biabl_list* biabl_aux, *biabl_head;

   if (!biabl) {
      fprintf(stderr,"adapt_reg: NULL pointer\n");
      exit(1);
   }

   biabl_head=biabl;
   
   /*pattern matching*/
   cell=cell_prepare_reg(biabl);
   
   top=copyport(cell->PORT);  /*because this port can be used later*/
   
   for (port=top; port; port=port->NEXT) {
    if (isvss(port->NAME) || isvdd(port->NAME)) continue;
    else switch (port->DIRECTION) {
    case OUT: case TRISTATE: break;
    default: 
      if (!port->ABL) {
         fprintf(stderr,
         "adapt_reg: compute error %s not really match on port %s\n",
         cell->NAME,port->NAME);
         exit(1);
      }
      port->ABL=loc_adapt_abl(port->ABL,port->C);
    }  
   }   

   delport(top);
   
   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
   /*look the other blocks*/
   if (biabl) {
      adapt_reg(biabl);
   }
   
   return biabl_head;
}



