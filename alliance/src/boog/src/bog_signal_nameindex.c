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
 * Tool    : BooG - give unique index
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <string.h>
#include <mut.h>
#include <abl.h>
#include <aut.h>
#include <abe.h>
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_signal_nameindex.h"


#define MBK_NOT namealloc("not")
#define MBK_SIG namealloc("sig")
#define MBK_OUT namealloc("out")
#define MBK_INS namealloc("ins")
#define BLOCK 1024

/*table of used name*/
static authtable* HTABLE;  /*VALUE the occurrence of name*/


/***************************************************************************/
/*  free index referencing                                                 */
/***************************************************************************/
extern void free_nameindex()
{
   if (HTABLE) destroyauthtable(HTABLE);
   HTABLE=NULL;
}


/***************************************************************************/
/* return 1 if radical name can generate conflict with automatic indexation*/
/***************************************************************************/
extern int forbid_radical(char* name)
{
   int size;
   cell_list *cell;

   name=vectorradical(name);

   /*control first if it is the negation itself*/
   size=strlen(MBK_NOT);
   if (strlen(name)>=size && !strncmp(name,MBK_NOT,size)) return 1;

   /*control with the names of library*/
   for (cell=getcell_logic_lib(); cell; cell=cell->NEXT) {
      size=strlen(cell->NAME);
      if (strlen(name)>=size && !strncmp(name,cell->NAME,size)) return 1;
   }
   
   for (cell=getcell_tristate_lib(); cell; cell=cell->NEXT) {
      size=strlen(cell->NAME);
      if (strlen(name)>=size && !strncmp(name,cell->NAME,size)) return 1;
   }

   for (cell=getcell_register_lib(); cell; cell=cell->NEXT) {
      size=strlen(cell->NAME);
      if (strlen(name)>=size && !strncmp(name,cell->NAME,size)) return 1;
   }

   return 0;      /*no conflict*/
}


/***************************************************************************/
/* return name concatenated with an index and separated by '_'             */
/* this index is : how many times I have sent this name to getnameindex()  */
/* unicity is guaranteed until you run Put_index_to_zero() below           */
/***************************************************************************/
extern char* getnameindex(char* name)
{
   char memo_char;
   char *radical;
   int index;
   authelem* elem;

   name=namealloc(name);
   radical=vectorradical(name);
   index=vectorindex(name);

   if (!HTABLE) HTABLE=createauthtable(BLOCK);
   elem=searchauthelem(HTABLE,name);
   
   if (!elem) {
      addauthelem(HTABLE,name,1);
      return name;      /*unchanged*/
   }
   else {
      /*add an occurrence*/
      elem->VALUE++;
      memo_char=SEPAR;     /*external value from MBK environment*/
      SEPAR='_';
      name=nameindex(radical,elem->VALUE); /*take value SEPAR for separator*/
      SEPAR=' ';
      if (index!=-1) name=nameindex(name,index);  /*rebuild vector*/
      SEPAR=memo_char;        /*not to change global environment*/
      return name;
   }   
}


/******************************************************************************/
/*      build a generic opposite name                                         */
/******************************************************************************/
extern char* getoppositename(char* name)
{
   char memo_char;
   int size;
  
   name=namealloc(name);
   
   /*constant 1 0*/
   if (name==getablatomone()) return getablatomzero();
   if (name==getablatomzero()) return getablatomone();
   
   /*control first if it is the negation itself*/
   size=strlen(MBK_NOT);    
   if (!strncmp(name,MBK_NOT,size)) {
      name=name+size+1;    /* separator "_"  */
      return namealloc(name);
   }
   memo_char=SEPAR;     /*external value from MBK environment*/
   SEPAR='_';
   name=concatname(MBK_NOT,name);
   SEPAR=memo_char;
  
   return name;
}


/******************************************************************************/
/*      build a generic name with an incremented index                        */
/******************************************************************************/
extern char* getautoname(char* name)
{
   name=losig_name(name);
   
   return getnameindex(name);
}


/******************************************************************************/
/* return an artificial name to distinguish loins from signal                 */
/******************************************************************************/
extern char* losig_name(char* name)
{
   char memo_char;

   name=namealloc(name);
   
   memo_char=SEPAR;     /*external value from MBK environment*/
   SEPAR='_';
   name=concatname(name,MBK_SIG);
   SEPAR=memo_char;

   return name;
}


/******************************************************************************/
/* return an artificial name to distinguish loins from signal                 */
/* only used to hash values for internal treatment                            */
/******************************************************************************/
extern char* loins_name(char* name)
{
   char memo_char;

   name=namealloc(name);
   
   memo_char=SEPAR;     /*external value from MBK environment*/
   SEPAR='_';
   name=concatname(name,MBK_INS);
   SEPAR=memo_char;

   return name;
}


/******************************************************************************/
/* return an artificial name to distinguish input datas from output datas in  */
/* the case of inout port                                                     */
/* only used to hash values for internal treatment                            */
/******************************************************************************/
extern char* output_name(char* name)
{
   char memo_char;

   name=namealloc(name);
   
   memo_char=SEPAR;     /*external value from MBK environment*/
   SEPAR='_';
   name=concatname(name,MBK_SIG);
   name=concatname(name,MBK_OUT);
   SEPAR=memo_char;

   return name;
}

