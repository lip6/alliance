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
 * Tool    : BooG - negativ operands
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <mut.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_ARITY.h"
#include "bog_lib_negativ.h"



/****************************************************************************/
/* build a tree on negativ logic by propagation of NOT to the leaves        */
/* negativ is set to 1 to return the opposite abl (also in negativ logic)   */
/****************************************************************************/
extern chain_list* inv_oper(chain_list* abl, int negativ)
{
   chain_list* chain;
   
   if (ABL_ATOM(abl)) {
      if (ABL_ATOM_VALUE(abl)==getablatomzero()) {
         if (negativ) ABL_ATOM_VALUE(abl)=getablatomone();
      }
      else if (ABL_ATOM_VALUE(abl)==getablatomone()) {
         if (negativ) ABL_ATOM_VALUE(abl)=getablatomzero();
      }
      else {
         if (negativ) {
            abl=createablnotexpr(abl);
            ABL_ARITY(abl)=1;
         }   
      }
      return abl;
   }

   switch (ABL_OPER(abl)) {
      case ABL_AND: 
         if (negativ) ABL_OPER(abl)=ABL_NAND;
         else ABL_OPER(abl)=ABL_NOR;
         negativ=!negativ;
         break;
      case ABL_NAND: 
         if (negativ) ABL_OPER(abl)=ABL_NOR;
         else ABL_OPER(abl)=ABL_NAND;
         break;
      case ABL_OR:
         if (negativ) ABL_OPER(abl)=ABL_NOR;
         else ABL_OPER(abl)=ABL_NAND;
         negativ=!negativ;
         break;
      case ABL_NOR:
         if (negativ) ABL_OPER(abl)=ABL_NAND;
         else ABL_OPER(abl)=ABL_NOR;
         break;
      case ABL_XOR:
         if (negativ) ABL_OPER(abl)=ABL_NXOR;
         negativ=0;
         /*nothing to do:  same size XOR and NXOR*/
         break;
      case ABL_NXOR:
         if (negativ) ABL_OPER(abl)=ABL_XOR;
         negativ=0;
         /*nothing to do*/
         break;
      case ABL_NOT:   /*erase NOT*/
         chain=abl;
         abl=inv_oper(ABL_CADR(abl),!negativ);
         freechain(chain);
         return abl;
      case ABL_STABLE:
         if (negativ) {
            abl=createablnotexpr(abl);
            ABL_ARITY(abl)=1;
         }   
         return abl;
      default:
         fprintf(stderr,"inv_oper: unknown operator %ld\n",ABL_OPER(abl));
         exit(1);
   }

   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      ABL_CAR(chain)=inv_oper(ABL_CAR(chain),negativ);
   }
   
   return abl;
}


/******************************************************************************/
/*return the abl in negativ logic                                             */
/*we need 2 levels minimum of operator to invert an abl                       */
/*if only 1 level or neither odd nor even level return an unchanged abl       */
/* minimize NOT operators number                                              */
/*parameter is deleted                                                        */
/******************************************************************************/
extern chain_list* build_negativ(chain_list* abl)
{
   int to_inv=0;  /*leaf to change*/
   int to_let=0;  /*leaf to let like this*/
   chain_list *leaf=NULL, *chain; 
     
   if (ABL_ATOM(abl)) return abl;
   /*NOT special case try to erase it*/
   if (ABL_OPER(abl)==ABL_NOT) {
      /*at the end, nothing to do*/
      if (ABL_ATOM(ABL_CADR(abl))) return abl;
      /*propagate inverters to the leaves*/
      abl=inv_oper(abl,0);
      /*abl has changed, try again*/
      return build_negativ(abl);
   }  
   
   if (ABL_OPER(abl)==ABL_STABLE) return abl;
   
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      ABL_CAR(chain)=build_negativ(ABL_CAR(chain));
   }

   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      leaf=ABL_CAR(chain);
      if (ABL_ATOM(leaf)) {
         /*constant undifferent*/
         if (ABL_ATOM_VALUE(leaf)!=getablatomzero()
          && ABL_ATOM_VALUE(leaf)!=getablatomone()) to_let++;
         continue;
      }
      switch (ABL_OPER(leaf)) {
         case ABL_AND: case ABL_OR: case ABL_NOT: to_inv++; break;
         case ABL_NAND: case ABL_NOR: case ABL_STABLE: to_let++; break;
         case ABL_XOR: case ABL_NXOR: /*undifferent*/ break;
         default:
            fprintf(stderr,"build_negativ: oper %ld unknown\n",ABL_OPER(leaf));
            exit(1);
      }
   }
   
   /*no gain, do not change*/
   if (to_let>to_inv) return abl;
   else if (to_let==to_inv) {
         /*no advantage to invert*/
      switch (ABL_OPER(abl)) {
         case ABL_NAND: case ABL_NOR: return abl; 
      }
   }  
   
   /*forbidden to invert*/
   switch (ABL_OPER(abl)) {
         case ABL_STABLE: 
         case ABL_NXOR: case ABL_XOR: return abl; 
   }
   
   for (chain=ABL_CDR(abl); chain; chain=ABL_CDR(chain)) {
      leaf=ABL_CAR(chain);
      if (ABL_ATOM(leaf)) {
         /*constant undifferent*/
         if (ABL_ATOM_VALUE(leaf)==getablatomzero()) 
            ABL_ATOM_VALUE(leaf)=getablatomone();
         else if (ABL_ATOM_VALUE(leaf)==getablatomone()) 
            ABL_ATOM_VALUE(leaf)=getablatomzero();
         else {
            ABL_CAR(chain)=createablnotexpr(ABL_CAR(chain));
            ABL_ARITY(ABL_CAR(chain))=1;
         }   
         continue;
      }
      else if (ABL_OPER(leaf)==ABL_STABLE) {
         ABL_CAR(chain)=createablnotexpr(ABL_CAR(chain));
         ABL_ARITY(ABL_CAR(chain))=1;
         continue;
      }
      switch (ABL_OPER(leaf)) {
         case ABL_AND: ABL_OPER(leaf)=ABL_NAND; break; 
         case ABL_OR: ABL_OPER(leaf)=ABL_NOR; break; 
         case ABL_NAND: ABL_OPER(leaf)=ABL_AND; break; 
         case ABL_NOR: ABL_OPER(leaf)=ABL_OR; break; 
         case ABL_XOR: ABL_OPER(leaf)=ABL_NXOR; break; 
         case ABL_NXOR: ABL_OPER(leaf)=ABL_XOR; break; 
         case ABL_NOT: 
            ABL_CAR(chain)=ABL_CADR(leaf);
            freechain(leaf);
            break;
         default:
            fprintf(stderr,"build_negativ: oper %ld unknown\n",ABL_OPER(leaf));
            exit(1);
      }
   }
   
   switch (ABL_OPER(abl)) {
      case ABL_AND: ABL_OPER(abl)=ABL_NOR; break; 
      case ABL_OR: ABL_OPER(abl)=ABL_NAND; break; 
      case ABL_NAND: ABL_OPER(abl)=ABL_OR; break; 
      case ABL_NOR: ABL_OPER(abl)=ABL_AND; break; 
      case ABL_NOT: 
         chain=abl;
         abl=ABL_CADR(abl);
         freechain(chain);
         break;
      default:
         fprintf(stderr,"build_negativ: oper %ld unknown\n",ABL_OPER(leaf));
         exit(1);
   }
   
   return abl;
}


