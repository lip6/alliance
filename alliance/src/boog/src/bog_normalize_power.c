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
 * Tool    : BooG - control power supply
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <stdlib.h>
#include <stdio.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include <mlo.h>
#include "bog_normalize_power.h"



/******************************************************************************/
/*       if power supply is found in abl expression generate an error         */
/* return 1 if no problem encountered                                         */
/******************************************************************************/
static int power_ok(befig_list* befig, chain_list* abl)
{
   if (ABL_ATOM(abl)) {
      if (isvdd(ABL_ATOM_VALUE(abl)) || isvss(ABL_ATOM_VALUE(abl))) {
         fprintf(stderr,"BEH: power '%s' is used in an expression\n",
                  ABL_ATOM_VALUE(abl));
         return 0;                        
      }
      return 1;
   }
   
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      if (!power_ok(befig,ABL_CAR(abl))) return 0;
   }
   
   return 1;  /*ok*/
}


/******************************************************************************/
/*          detect power and forbid Vdd and Vss in abl expression             */
/* return 1 if no problem encountered                                         */
/******************************************************************************/
extern int detect_power(befig_list* befig)
{
   bebus_list* bebus;
   beaux_list* beaux;
   beout_list* beout;
   bereg_list* bereg;
   bebux_list* bebux;
   biabl_list* biabl;
   bepor_list* bepor;
   int vss=0, vdd=0;    /*flag for detection*/

   /*search power*/
   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
    if (isvdd(bepor->NAME)) {
      /*only input*/
      if (bepor->DIRECTION!=IN) {
        fprintf(stderr,"BEH: '%s: IN bit;' is needed in port of %s\n",
         bepor->NAME,befig->NAME);
        return 0;
      }
      if (vdd) {
        fprintf(stderr,"BEH: duplicate Vdd power in port for %s in %s\n",
        bepor->NAME,befig->NAME);
        return 0;
      }
      vdd=1;
    }
    else if (isvss(bepor->NAME))  {
      /*only input*/
      if (bepor->DIRECTION!=IN) {
        fprintf(stderr,"BEH: '%s: IN bit;' is needed in port of %s\n",
        bepor->NAME,befig->NAME);
        return 0;
      }
      if (vss) {
        fprintf(stderr,"BEH: duplicate Vss power in port for %s in %s\n",
         bepor->NAME,befig->NAME);
        return 0;
      }
      vss=1;
    }
   }
   
   /*power must exist*/
   if (!vdd || !vss) {
      fprintf(stderr,"BEH: Vdd and Vss are needed in port of %s\n",befig->NAME);
      return 0;
   }

   /*forbid the use of power in expression*/
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      if (!power_ok(befig,beout->ABL)) return 0;
   }
   
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      if (!power_ok(befig,beaux->ABL)) return 0;
   }
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         if (!power_ok(befig,biabl->CNDABL)) return 0;
         if (!power_ok(befig,biabl->VALABL)) return 0;
      }
   }
   
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         if (!power_ok(befig,biabl->CNDABL)) return 0;
         if (!power_ok(befig,biabl->VALABL)) return 0;
      }
   }
   
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         if (!power_ok(befig,biabl->CNDABL)) return 0;
         if (!power_ok(befig,biabl->VALABL)) return 0;
      }
   }
   
   return 1; /*ok*/
}

