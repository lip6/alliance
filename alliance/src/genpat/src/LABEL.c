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

/****************************************************************/
/* module   : label.c						*/
/* date     : 02/09/93						*/
/* auteurs  : PITON 						*/
/****************************************************************/

#ident "$Id: LABEL.c,v 1.3 2012/05/14 14:20:14 alliance Exp $"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mut.h"
#include "pat.h"
#include "libpat_l.h"

/* ###--------------------------------------------------------------### */
/* function     : LABEL                                                 */
/* description  : set current pattern label				*/
/* called func. : none                                                  */
/* ###--------------------------------------------------------------### */

void d_LABEL(ident)
char *ident;
{
char  buffer[20];
int i = 1;

    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

   if (ident == NULL)
	Erreur ("LABEL missing argument");
   strncpy(buffer,ident,19);
   KillEsp(buffer);

   if( isalpha(buffer[0]) == 0 )  
         Erreur ("IDENT error not an identifier");

   while (buffer[i] != '\0')
   {
   if( (isalpha(buffer[i])==0) && (isalnum(buffer[i])==0) && (buffer[i] != '_'))  
      Erreur ("IDENT error not an identifier");
   i++;
   }
   CreatePapat (buffer, NULL,NULL);
}
