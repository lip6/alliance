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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


/* ###--------------------------------------------------------------### */
/* file		: pat_warning.c						*/
/* date		: Aug 16 1993						*/
/* version	: v104							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: system level functions				*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>

/* ###--------------------------------------------------------------### */
/* function	: pat_warning						*/
/* description	: print a warning message depending on the arg. `code`	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int pat_warning (code, str, dat, line)

int           code;
char         *str ;
char          dat ;
unsigned int  line;

  {
  if (code > 100)
    fprintf (stderr, "PAT : Warning %d line %d : ", code, line);
  else
    {
    fprintf (stderr, "PAT : Warning %d : ", code);
    }

  switch (code)
    {
    case 1:
      fprintf (stderr, "switching to new file (%s) ?!\n", str);
      break;

    default :
      fprintf (stderr, "Warning\n");
      break;
    }

  return (1);
  }
