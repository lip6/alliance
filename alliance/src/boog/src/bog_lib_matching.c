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
 * Tool    : BooG - pattern matching between cells
 * Date    : 2000
 * Author  : Francois Donnet
 */

#include <stdlib.h>
#include <mut.h>
#include <abl.h>
#include <aut.h>
#include <mlo.h>
#include <abe.h>
#include "bog_normalize_ARITY.h"
#include "bog_lib_matching.h"


static authtable* HTABLE = NULL;

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

   addauthelem(HTABLE, pattern,(long)expr);
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
   if (ABL_ARITY (expr) != ABL_ARITY (pattern)) return 0;

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

   if ( HTABLE == (authtable *)0 )
   {
     HTABLE=createauthtable ( 13 );  
   }
   else
   {
     resetauthtable( HTABLE );
   }

   ret=loc_pattern_matching(expr,pattern);


   if ( HTABLE->TABLE_SIZE > 50 )
   {
     destroyauthtable( HTABLE );
     HTABLE = (authtable *)0;
   }

   return ret;
}
