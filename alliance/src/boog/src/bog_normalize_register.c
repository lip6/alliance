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
 * Tool    : BooG - format flip-flop
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <stdlib.h>
#include <stdio.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_message.h"
#include "bog_signal_nameindex.h"
#include "bog_normalize_register.h"

/*flag if an VHDL error is met*/
static int BEH_ERROR;


/******************************************************************************/
/* return the number of times ck is found in abl (expect under stable)        */
/******************************************************************************/
static int clock_values(char* ck, chain_list* abl)
{
   int ret=0;

   if (ABL_ATOM(abl)) {
      if (ABL_ATOM_VALUE(abl)==ck) return 1;
      else return 0;
   }

   if (ABL_OPER(abl)==ABL_STABLE) return 0;

   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      ret+=clock_values(ck,ABL_CAR(abl));
   }
   
   return ret;
}



/******************************************************************************/
/*                   extract the clock                                        */
/*verify unicity of clock                                                     */
/******************************************************************************/
static char* find_stable(chain_list* abl)
{
   char* ck=NULL, *new;
   
   /*if it's a leaf, we don't have met a stable at this level*/
   if (ABL_ATOM(abl)) return NULL;
   
   if (ABL_OPER(abl)==ABL_STABLE/*ABL library*/) {
      return ABL_ATOM_VALUE(ABL_CADR(abl));
   }
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      new=find_stable(ABL_CAR(abl));
      if (new) {
         if (ck) {
            BEH_ERROR=1;
            return ck;
         }
         ck=new;
      }
   }   

   /*not found*/
   return ck;
}


/******************************************************************************/
/* latch, flip-flop, flip-flop with write enable are accepted                 */
/*remove stable and put a PTYPE in BIABL field ABL_STABLE with clock name     */
/* mute all write enable condition on multiplexor!   (only for flip-flop)     */
/*return 1 if has been formatted                                              */
/******************************************************************************/
static int loc_format_register(bereg_list* bereg)
{
   char* ck, *save_ck=NULL;
   biabl_list* biabl;
#if 0  
   chain_list* abl, *current;
   chain_list *setup=NULL, *not_stable=NULL, *wen=NULL;
#endif

   if (!bereg || !bereg->BIABL) {
      fprintf(stderr,"identify_register: NULL pointer\n");
      exit(1);
   }

   /*control all condition block*/
   for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
   
      /*extract stable variable*/
      ck=find_stable(biabl->CNDABL);
      if (BEH_ERROR && save_ck) {
         fprintf(stderr,
         "BEH: Bad usage of stable for register %s\n",bereg->NAME);
         return 0;
      }      
      save_ck=ck;

      if (ck) {/*for flip-flop memorize the clock setup signal*/
         
         if (ABL_ATOM(biabl->CNDABL) || ABL_OPER(biabl->CNDABL)!=ABL_AND || 
             clock_values(ck,biabl->CNDABL)!=1) {
            fprintf(stderr,
            "BEH: Clock '%s' setup value isn't explicit for register %s\n",
            ck,bereg->NAME);
            return 0;
         }
         
         /*memo clock name*/
         biabl->USER=addptype(biabl->USER,ABL_STABLE,ck);
         
#if 0
         /*separate clock setup value, stable and write enable*/
         for (abl=ABL_CDR(biabl->CNDABL); abl; abl=ABL_CDR(abl)) {
            current=ABL_CAR(abl);
            /*clock setup value?   ck   .....   not ck   .....  */
            if ((ABL_ATOM(current) && ABL_ATOM_VALUE(current)==ck) || 
                (ABL_OPER(current)==ABL_NOT && ABL_ATOM(ABL_CADR(current)) 
                 && ABL_ATOM_VALUE(ABL_CADR(current))==ck)) {
               setup=current;
            }
            else if (!ABL_ATOM(current) && ABL_OPER(current)==ABL_NOT 
             && !ABL_ATOM(ABL_CADR(current)) 
             && ABL_OPER(ABL_CADR(current))==ABL_STABLE) {
               not_stable=current;
           }     
            else {
               wen=current;
            }
         }

         /*build write enable register and condition without stable*/
         if (!setup || !not_stable) {
            fprintf(stderr,
            "BEH: Clock '%s' setup value isn't explicit for register %s\n",
            ck,bereg->NAME);
            return 0;
         }

         freechain(biabl->CNDABL);
         freeablexpr(not_stable);
         if (!wen) biabl->CNDABL=setup;
         else biabl->CNDABL=createablbinexpr(ABL_AND, wen, setup);
#endif

      } /*end of flip-flop*/

   }  /*loop on condition blocks*/

   return 1;  /*ok*/   
}


/******************************************************************************/
/*return 1 if all registers have been formatted                               */
/* verify the condition block and put the clock name in biabl->USER field     */
/* as ABL_STABLE type                                                         */
/******************************************************************************/
extern int format_register(befig_list* befig)
{
   bereg_list* bereg;

   if (!befig) {
      fprintf(stderr,"format_register: NULL pointer\n");
      exit(1);
   }

   BEH_ERROR=0;

   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      if (!loc_format_register(bereg)) return 0;
   }

   return 1;  /*ok: formatted*/
}


/******************************************************************************/
/*    put back stable clock expression in condition register                  */
/******************************************************************************/
extern void put_back_STABLE(befig_list *befig)
{
#if 0   
   bereg_list* bereg;
   ptype_list* ptype;
   biabl_list* biabl;
   char* ck;
   chain_list* abl;
   
   for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
      for (biabl=bereg->BIABL; biabl; biabl=biabl->NEXT) {
         ptype=getptype(biabl->USER,ABL_STABLE);
         if (!ptype) continue;
         ck=(char*) ptype->DATA;

         /*compare clock and setup value*/
         if (!ABL_ATOM(biabl->CNDABL)) {  /*not ck*/
            abl=ABL_CADR(biabl->CNDABL);
            if (!ABL_ATOM(abl) || ABL_OPER(biabl->CNDABL)!=ABL_NOT) {
               fprintf(stderr,"BEH: error on condition register of %s\n",
               bereg->NAME);
            }
            else {
               if (ck!=ABL_ATOM_VALUE(abl)) { /* not mbk_not_ck=1  ->  ck=1 */
                  ABL_ATOM_VALUE(abl)=getoppositename(ABL_ATOM_VALUE(abl));
                  freechain(biabl->CNDABL);
                  biabl->CNDABL=abl;
                  if (ck!=ABL_ATOM_VALUE(abl)) {
                     fprintf(stderr,"BEH: error on condition register of %s\n",
                     bereg->NAME);
                  }
               }
            }
         }
         else if (ABL_ATOM_VALUE(biabl->CNDABL)!=ck) {  /* mbk_not_ck=1 */
            ABL_ATOM_VALUE(biabl->CNDABL)=
                  getoppositename(ABL_ATOM_VALUE(biabl->CNDABL));
            if (ck!=ABL_ATOM_VALUE(biabl->CNDABL)) {          /* not ck = 1 */
               fprintf(stderr,"BEH: error on condition register of %s\n",
               bereg->NAME);
            }
            biabl->CNDABL=createablnotexpr(biabl->CNDABL);  
         }   
         
         /*add STABLE*/
         abl=createabloper(ABL_STABLE);
         ABL_CDR(abl)=addchain(NULL,createablatom(ck));
         abl=createablnotexpr(abl);
         biabl->CNDABL=createablbinexpr(ABL_AND,abl,biabl->CNDABL);
      }
   }
#endif   
}


