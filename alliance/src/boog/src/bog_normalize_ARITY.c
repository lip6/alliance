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
 * Tool    : BooG - put arity notion in abl
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <mut.h>
#include <abl.h>
#include <abe.h>
#include "bog_signal_utils.h"
#include "bog_normalize_ARITY.h"


/***************************************************************************/
/*             put arity operator in abl->DATA->NEXT field                 */
/*  Warning: simpablexpr() do not work anymore after pulling arity         */
/***************************************************************************/
extern void put_arity_abl (chain_list* abl)
{
   chain_list* pattern;
   int arity;

   /*pattern is an atom*/
   if (ABL_ATOM (abl)) return;
   
   arity=0;
 	for (pattern = ABL_CDR (abl); pattern; pattern=ABL_CDR(pattern)) {
     arity++;
     put_arity_abl((chain_list*)ABL_CAR(pattern));
	}

   ABL_ARITY(abl)=arity;
   return ;
}


/***************************************************************************/
/*                   count the arity of each operator                      */
/***************************************************************************/
extern void put_arity(befig_list* befig)
{
   bebus_list* bebus;
   beaux_list* beaux;
   beout_list* beout;
   bereg_list* bereg;
   bebux_list* bebux;
   biabl_list* biabl;

   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      put_arity_abl(beout->ABL);
   }
   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      put_arity_abl(beaux->ABL);
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         put_arity_abl(biabl->CNDABL);
         put_arity_abl(biabl->VALABL);
      }
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         put_arity_abl(biabl->CNDABL);
         put_arity_abl(biabl->VALABL);
      }
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         put_arity_abl(biabl->CNDABL);
         put_arity_abl(biabl->VALABL);
      }
   }
   
}

