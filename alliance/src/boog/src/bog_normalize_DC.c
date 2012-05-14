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
 * Tool    : BooG - quick simplify Don't Care
 * Date    : 2000
 * Author  : Francois Donnet
 */




#include <stdlib.h>
#include <stdio.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_DC.h"



/***************************************************************************/
/*      choose '1' or '0' to simplify the expression as well as we can     */
/***************************************************************************/
static void find_d_z_abl(chain_list* abl, long value)
{
   
  if (ABL_ATOM(abl)) {    
      if (ABL_ATOM_VALUE(abl)==getablatomdc()/* = namealloc("'d'")*/) {
         /*non standard IEEE VHDL*/  /*it means "don't care"*/
         /* we can put zero either one, only to simplify*/
         ABL_CAR_L(abl)=value?getablatomone():getablatomzero(); 
      }   
      else if (ABL_ATOM_VALUE(abl)==getablatomtristate()/* =namealloc("'z'")*/){
         /*no drive on signal*/
         /* a pull-up is done for better conductance*/
         ABL_CAR_L(abl)=getablatomone()/* = namealloc("'1'")*/;
      }   
      return;
   }
   
   /*the first operator influences the most*/
   switch (ABL_OPER(abl)) {
      case ABL_OR: case ABL_NOR: value=1;   
      case ABL_AND: case ABL_NAND: value=0;  
   }
   
   /*for each operator*/
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      find_d_z_abl(ABL_CAR(abl),value);
   }   
   
}


/****************************************************************************/
/*  change 'z' and 'd' occurence in '0' or '1' in all expressions of befig  */
/****************************************************************************/
extern void remove_DC(befig_list* befig)
{
   beaux_list *beaux;
   bebux_list *bebux;
   bebus_list *bebus;
   beout_list *beout;
   bereg_list *bereg;
   biabl_list *biabl;


   if (!befig) {
      fprintf(stderr,"remove_Z_D: NULL pointer\n");
      exit(1);
   }   

   /*INTERNAL signals*/
   for (beaux=befig->BEAUX; beaux; beaux=beaux->NEXT) {
      find_d_z_abl(beaux->ABL,1);
   }
   /*INTERNAL bus*/
   for (bebux=befig->BEBUX; bebux; bebux=bebux->NEXT) {
      for (biabl=bebux->BIABL; biabl; biabl=biabl->NEXT) {
         find_d_z_abl(biabl->CNDABL,1);
         find_d_z_abl(biabl->VALABL,1);
      }
   }   
   /*register and latch*/
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         find_d_z_abl(biabl->CNDABL,1);
         find_d_z_abl(biabl->VALABL,1);
      }
   }
   /*EXTERNAL signals*/
   for (beout=befig->BEOUT; beout; beout=beout->NEXT) {
      find_d_z_abl(beout->ABL,1);
   }
   /* bussed output */
   for (bebus=befig->BEBUS; bebus; bebus=bebus->NEXT) {
      for (biabl=bebus->BIABL; biabl; biabl=biabl->NEXT) {
         find_d_z_abl(biabl->CNDABL,1);
         find_d_z_abl(biabl->VALABL,1);
      }
   }
   
}
