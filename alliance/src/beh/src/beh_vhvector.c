/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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

/* ###--------------------------------------------------------------### */
/* file		: beh_vhvector.c					*/
/* date		: Mar  1 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_vhvector						*/
/* description	: put parenthesis on element of a vector		*/
/* called func.	: namealloc, beh_addtab, beh_initab, beh_chktab		*/
/* ###--------------------------------------------------------------### */

char *beh_vhvector (name)

char *name;
  {
  char                 *new_name   ;
  char                  tmp1 [256] ;
  long                   i          = 0   ;
  static struct beden **tab        = NULL;

	/* ###------------------------------------------------------### */
	/*   create a local dictionary to save the corresponding VHDL	*/
	/* vector element name						*/
	/* ###------------------------------------------------------### */

  if (tab == NULL)
    tab = beh_initab ();

	/* ###------------------------------------------------------### */
	/*   check if the string has already been translated to a VHDL	*/
	/* vector element name or not. If not make the translation and	*/
	/* save the name in the dictionary.				*/
	/* ###------------------------------------------------------### */

  if ((new_name = (char *) beh_chktab (tab, name, NULL, 7)) == NULL)
    {
    strcpy (tmp1, name);

    while ((name [i] != '\0') && (name [i] != ' ' ) && (name [i] != '\''))
      i ++;

    tmp1 [i] = '\0';
    new_name = beh_vhdlname (tmp1);

    if (name [i] != '\0')
      {
      if (name [i] == ' ')
        sprintf (tmp1, "%s (%s)", new_name, &name [i+1]); 
      else
        sprintf (tmp1, "%s'%s"  , new_name, &name [i+1]); 

      new_name = namealloc (tmp1);
      }
    beh_addtab (tab, name, NULL, 7, new_name);
    }

  return (new_name);
  }
