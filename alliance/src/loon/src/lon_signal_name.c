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
 * Tool    : LooN - names of input
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include "lon_signal_name.h"

/*to distinguish output of circuit*/
#define MBK_OUT namealloc("MBK_OUT")
/*other name of same instance*/
#define MBK_BUF namealloc("MBK_BUF")
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
/* return name concatenated with an index and seperated by '_'             */
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
      /*add an occurence*/
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
/*      build a generic name with an incremented index                        */
/******************************************************************************/
extern char* getautoname(char* name)
{
   char memo_char;

   name=namealloc(name);
   
   memo_char=SEPAR;     /*external value from MBK environment*/
   SEPAR='_';
   name=concatname(MBK_BUF,name);
   SEPAR=memo_char;

   return getnameindex(name);
}


/***************************************************************************/
/*return a new name to distinguish output value from input when inout      */
/***************************************************************************/
extern char* output_name(char* name)
{
   char memo;

   memo=SEPAR;
   SEPAR='_';
   
   name=concatname(MBK_OUT,name);

   SEPAR=memo;

   return name;
}

