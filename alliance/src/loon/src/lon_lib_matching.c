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
 * Tool    : LooN - pattern matching between cells
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <abl.h>
#include <aut.h>
#include <mlo.h>
#include <abe.h>
#include "lon_lib_matching.h"


/*size of memory block*/
#define BLOCK 256

static authtable* HTABLE;




/******************************************************************************/
/* return 0, if there is already a variable with pattern                      */
/******************************************************************************/
static int relation_between(chain_list* expr, char* pattern)
{
   authelem *elem;

   elem=searchauthelem(HTABLE,pattern);
   if (elem) {
      chain_list* old_abl=(chain_list*)elem->VALUE;
      if (ABL_ATOM(old_abl) && ABL_ATOM(expr) 
       && ABL_ATOM_VALUE(old_abl)==ABL_ATOM_VALUE(expr)) return 1;
      else return 0;
   }

   addauthelem(HTABLE, pattern,(int)expr);
   return 1;
}


/****************************************************************************/
/* Pattern matching between an expr and a pattern. Return true if expr has  */
/* the same operators and in the same order than pattern                    */
/* '1' , '0' , 'd' and 'z' are also checked if matched                      */
/****************************************************************************/
static int loc_pattern_matching(chain_list* expr, chain_list* pattern)
{
   if (!expr || !pattern) {
      fprintf(stderr,"loc_pattern_matching: NULL pointer\n");
      exit(1);
   }

   /*pattern is an atom*/
   if (ABL_ATOM (pattern)) {
      /*constants MUST match*/
      if (ABL_ATOM_VALUE(pattern)==getablatomone()) {
         if (ABL_ATOM(expr) && ABL_ATOM_VALUE(expr)==getablatomone()) return 1;
         else return 0;   
   }
      if (ABL_ATOM_VALUE(pattern)==getablatomzero()) {
         if (ABL_ATOM(expr) && ABL_ATOM_VALUE(expr)==getablatomzero()) return 1;
         else return 0;   
   }
      if (ABL_ATOM_VALUE(pattern)==getablatomdc()   /* 'd'  */) {
         if (ABL_ATOM(expr) && ABL_ATOM_VALUE(expr)==getablatomdc()) return 1;
         else return 0;   
   }
      if (ABL_ATOM_VALUE(pattern)==getablatomtristate() /* 'z' */) {
         if (ABL_ATOM(expr) && ABL_ATOM_VALUE(expr)==getablatomtristate()) 
            return 1;
         else return 0;   
      }
      return relation_between(expr,ABL_ATOM_VALUE(pattern));
   }
   
   /* pattern isn't an atom and expr is*/
   if (ABL_ATOM (expr)) return 0;
   
   /* not the same oper */
   if (ABL_OPER (expr) != ABL_OPER (pattern)) return 0;

   for (pattern = ABL_CDR(pattern); pattern&&expr; pattern=ABL_CDR(pattern)) {
     expr = ABL_CDR(expr);
     if (!expr) return 0;
     if (!loc_pattern_matching(ABL_CAR (expr), ABL_CAR (pattern))) return 0;
   }

   return 1;
   
}


/****************************************************************************/
/* Pattern matching between an expr and a pattern. Return true if expr has  */
/* the same operators and in the same order than pattern                    */
/* '1' , '0' , 'd' and 'z' are also checked if matched                      */
/*quite slow comparison                                                     */
/****************************************************************************/
extern int pattern_matching(chain_list* expr, chain_list* pattern)
{
   int ret;
   
   if (!expr || !pattern) {
      fprintf(stderr,"pattern_matching: NULL pointer\n");
      exit(1);
   }

   HTABLE=createauthtable (BLOCK);

   ret=loc_pattern_matching(expr,pattern);

   /*free table*/
   destroyauthtable(HTABLE);
   HTABLE=NULL;

   return ret;
}


/****************************************************************************/
/* return 1 if befig1 and befig2 are strictly identical                     */
/****************************************************************************/
extern int compare_befig(befig_list* befig1, befig_list* befig2)
{
   biabl_list *biabl1;
   biabl_list *biabl2;

   if (befig1->BEBUS) {
      if (!befig2->BEBUS) return 0;
      return isablequalexpr(befig1->BEBUS->BIABL->VALABL,befig2->BEBUS->BIABL->VALABL) && isablequalexpr(befig1->BEBUS->BIABL->CNDABL,befig2->BEBUS->BIABL->CNDABL);
   }
   else if (befig1->BEREG) {
      if (!befig2->BEREG) return 0;
      /*flip-flop or latch*/
      for ( biabl1 = befig1->BEREG->BIABL; biabl1; biabl1 = biabl1->NEXT )
      {
         for ( biabl2 = befig2->BEREG->BIABL; biabl2; biabl2 = biabl2->NEXT )
         {
            if ( getptype(biabl1->USER,ABL_STABLE)!=getptype(biabl2->USER,ABL_STABLE) ) continue;
            if ( isablequalexpr(biabl1->VALABL,biabl2->VALABL) && isablequalexpr(biabl1->CNDABL,biabl2->CNDABL) ) break;
         }
         /*equivalence not found*/
         if ( !biabl2 ) return 0;
      }
      for ( biabl2 = befig2->BEREG->BIABL; biabl2; biabl2 = biabl2->NEXT )
      {
         for ( biabl1 = befig1->BEREG->BIABL; biabl1; biabl1 = biabl1->NEXT )
         {
            if ( getptype(biabl1->USER,ABL_STABLE)!=getptype(biabl2->USER,ABL_STABLE) ) continue;
            if ( isablequalexpr(biabl1->VALABL,biabl2->VALABL) && isablequalexpr(biabl1->CNDABL,biabl2->CNDABL) ) break;
         }
         /*equivalence not found*/
         if ( !biabl1 ) return 0;
      }
      return 1;
   }
   else if (befig1->BEOUT) {
      if (!befig2->BEOUT) return 0;
      return isablequalexpr(befig1->BEOUT->ABL,befig2->BEOUT->ABL);
   }

   return 0;
}




