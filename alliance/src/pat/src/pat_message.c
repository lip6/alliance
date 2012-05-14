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
/* file		: pat_message.c						*/
/* date		: Aug 16 1993						*/
/* version	: v104							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: system level functions				*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>

/* ###--------------------------------------------------------------### */
/* function	: pat_message						*/
/* description	: print a message depending on the argument `code`	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int pat_message (code, str1, str2, line)

int           code;
char         *str1;
char         *str2;
unsigned int  line;

  {
  int flag = 1;

  fprintf (stdout, "%s : ", str1);

  switch (code)
    {
    case 1:
      fprintf (stdout, "stop mark set\n");
      break;

    case 2:
      fprintf (stdout, "\tSTACK OVERFLOW !!\tSTACK OVERFLOW !!\n");
      break;

    default :
      fprintf (stderr,"unknown message\n");
      break;
    }

  return (flag);
  }
