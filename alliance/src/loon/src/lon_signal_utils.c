/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 *
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Tool    : LooN - signals table
 * Date    : 2000
 * Author  : Francois Donnet
 */




#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include "lon_lax_param.h"
#include "lon_signal_utils.h"


/*size of memory block*/
#define BLOCK 2048


typedef struct signal{
   struct signal* NEXT;
   double          T;      
   double          C;
} signal_list;  


/*table of signal delay*/
static authtable* HTABLE=NULL;
/*heap of signal structures*/
static signal_list* HEAD=NULL;
/*list of block allocated*/
static chain_list* BLOCK_LIST=NULL;


/***************************************************************************/
/*    init memory for delay                                                */
/***************************************************************************/
static void init_signals()
{
   HTABLE= createauthtable (BLOCK);
}


/***************************************************************************/
/*    free memory for delay                                                */
/***************************************************************************/
extern void free_signals()
{
   chain_list* chain;
   if (HTABLE) destroyauthtable(HTABLE);
   HTABLE=NULL;
   /*destroy block per block*/
   for (chain=BLOCK_LIST; chain; chain=chain->NEXT) {
      HEAD=chain->DATA;
      mbkfree(HEAD);
   }
   if (BLOCK_LIST) freechain(BLOCK_LIST);
   BLOCK_LIST=NULL;
   HEAD=NULL;
}


/***************************************************************************/
/* return a new signal                                                     */
/***************************************************************************/
static signal_list* newsignal()
{
   signal_list* new;
   int i;

   if (!HEAD) {
      HEAD=mbkalloc(BLOCK*sizeof(signal_list));
      BLOCK_LIST=addchain(BLOCK_LIST,HEAD);
      new=HEAD;
      for (i = 1; i < BLOCK; i++) {
         new->NEXT = new + 1;
         new++;
      }
      new->NEXT = NULL;
   }
   
   new=HEAD;
   HEAD=HEAD->NEXT;
   
   /*fill signal*/
   new->NEXT=NULL;      /*not to interfer with below*/
   new->C=DEFAULT_CAPACITANCE;
   new->T=DEFAULT_DELAY;
   return new;
}


/***************************************************************************/
/* put a signal in hash table                                              */
/***************************************************************************/
static void putsignal(signal_list* signal, char* name)
{
   if (!name || !signal) {
      fprintf(stderr,"putsignal: NULL pointer\n");
      exit(1);
   }

   /*if doesn't exist hash table, init*/
   if (!HTABLE) init_signals();
   
   addauthelem(HTABLE,name,(int)signal);
}   


/***************************************************************************/
/* search a signal in hash table                                           */
/*if doesn't exist, create the signal if create param is 1                 */
/***************************************************************************/
static signal_list* getsignal(char *name, int create)
{
   authelem* elem;
   signal_list* signal;
   
   if (!name) {
      fprintf(stderr,"getsignal: no name\n");
      exit(1);
   }

   /*if doesn't exist hash table, init*/
   if (!HTABLE) init_signals();
   
   elem=searchauthelem(HTABLE,name);
   if (elem) return (signal_list*) elem->VALUE;

   if (create==0) {
      fprintf(stderr,"getsignal: '%s' not found in hash table\n",name);
      exit(1);
   }
   
   /*create new signal*/
   signal=newsignal();
   putsignal(signal,name);
   return signal;
}   


/***************************************************************************/
/*   put a delay to a signal                                               */
/***************************************************************************/
extern void putdelay(char* name, double delay)
{
   signal_list* signal;

   if (!name) {
      fprintf(stderr,"putdelay: no name\n");
      exit(1);
   }

   signal=getsignal(name,1/*if doesn't exist create it*/);
   signal->T=delay;
   if (signal->T<0) signal->T=0;
}


/***************************************************************************/
/*   return the signal delay                                               */
/***************************************************************************/
extern double getdelay(char* name)
{ 
   signal_list* signal;
   
   if (!name) {
      fprintf(stderr,"getdelay: no name\n");
      exit(1);
   }

   signal=getsignal(name,0/*if doesn't exist then error*/);
   return signal->T;
}


/***************************************************************************/
/*   add a delay to a signal                                               */
/***************************************************************************/
extern void incdelay(char* name, double delay)
{
   signal_list* signal;

   if (!name) {
      fprintf(stderr,"incdelay: no name\n");
      exit(1);
   }

   signal=getsignal(name,0);
   signal->T+=delay;
   if (signal->T<0) signal->T=0;
}


/***************************************************************************/
/*   return the signal capacitance                                         */
/***************************************************************************/
extern double getcapacitance(char* name)
{ 
   signal_list* signal;
   
   if (!name) {
      fprintf(stderr,"getcapacitance: no name\n");
      exit(1);
   }

   signal=getsignal(name,0/*if doesn't exist then error*/);
   return signal->C;
}


/***************************************************************************/
/*   add a capacitance to a signal                                         */
/***************************************************************************/
extern void inccapacitance(char* name, double capacitance)
{
   signal_list* signal;

   if (!name) {
      fprintf(stderr,"inccapacitance: no name\n");
      exit(1);
   }

   signal=getsignal(name,0/*if doesn't exist then error*/);
   signal->C+=capacitance;
   if (signal->C<0) signal->C=0;
}


/***************************************************************************/
/*   put a capacitance to a signal                                         */
/***************************************************************************/
extern void putcapacitance(char* name, double capacitance)
{
   signal_list* signal;

   if (!name) {
      fprintf(stderr,"putcapacitance: no name\n");
      exit(1);
   }

   signal=getsignal(name,1/*if doesn't exist create it*/);
   signal->C=capacitance;
   if (signal->C<0) signal->C=0;
}



