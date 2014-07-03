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
 * Tool    : LooN - display message
 * Date    : 2000
 * Author  : Francois Donnet
 */



#include <stdlib.h>
#include <stdio.h>
#include <mut.h>
#include <aut.h>
#include <abl.h>
#include "lon_normalize_message.h"




/***************************************************************************/
/*                   display an abl tree                                   */
/***************************************************************************/
extern void display_abl(chain_list* abl)
{
   static int first=1;  /*flags for braces*/
   int sig_first=0;
   int oper;

   if (!abl) {
      fprintf (stderr,"(null)");
      fflush(stderr);
      return;
   }
   
   if (ABL_ATOM (abl))   {   /* Traitement atomique */
      fprintf (stderr,"%s", ABL_ATOM_VALUE (abl));
      fflush(stderr);
      return;
   }  

   oper=ABL_OPER(abl);      /*memorisation de l'operateur*/
   
   /*operateur unaire*/
   switch (oper) {
   case ABL_NOT:
      fprintf (stderr,"%s ",getabloperuppername(oper));
      fflush(stderr);
      display_abl (ABL_CADR (abl));
      return;
   case ABL_STABLE:
      display_abl (ABL_CADR (abl));
      fprintf (stderr," %s",getabloperuppername(oper));
      fflush(stderr);
      return;
   }
   
   /*need of brace?*/
   if (first) {first=0; sig_first=1;}
   else fprintf (stderr,"(");
   fflush(stderr);
   
   /* Traitement des autres operateurs */
   for (abl=ABL_CDR(abl); abl; abl=ABL_CDR(abl)) {
      display_abl(ABL_CAR(abl));
      /* Un operateur en moins que le nombre d'arguments */
      if (ABL_CDR (abl))   fprintf (stderr," %s ",getabloperuppername(oper));   
      fflush(stderr);
   }

   if (sig_first) first=1;
   else fprintf(stderr,")");
   fflush(stderr);
}



/***************************************************************************/
/*             display a message error and an abl                          */
/***************************************************************************/
extern void display_error_in_abl(char* message, chain_list *abl)
{
   if (!message || !abl) {
      fprintf(stderr,"display_error_in_abl: NULL pointer\n");
      autexit(1);
   }

   fprintf(stderr,"BEH: ");
   fprintf(stderr,"%s",message);
   fprintf(stderr," in '");
   fflush(stderr);
   display_abl(abl);
   fprintf(stderr,"'\n");
}

