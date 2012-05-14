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
/* file		: beh_error.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>

/* ###--------------------------------------------------------------### */
/* function	: beh_error						*/
/* description	: print an errorr message				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

unsigned int beh_error (code, str1)

int   code;
char *str1;

  {
  fprintf (stderr, "BEH : Error %d: ", code);

  switch (code)
    {
    case 1:
      fprintf (stderr, "combinatory loop: `%s`\n", str1);
      break;
    case 2:
      fprintf (stderr, "cannot make bdd of empty expression\n");
      break;
    case 3:
      fprintf (stderr, "cannot find terminal `%s`\n", str1);
      break;
    case 4:
      fprintf (stderr, "illegal use of STABLE attribute\n");
      break;
    case 5:
      fprintf (stderr, "cannot simplify internal signals\n");
      break;
    case 6:
      fprintf (stderr, "cannot make derivatives of expressions\n");
      break;

    case 40:
      fprintf (stderr, "signal `%s` never assigned\n", str1);
      break;
    case 41:
      fprintf (stderr, "`%s` has not an empty architecture\n", str1);
      break;
    case 67:
      fprintf (stderr, "signal `%s` has unknwon type\n", str1);
      break;
    case 68:
      fprintf (stderr, "port `%s` has unknwon type\n", str1);
      break;
    case 69:
      fprintf (stderr, "port `%s` has unknwon mode\n", str1);
      break;
    case 70:
      fprintf (stderr, "unknown time unit\n");
      break;

    case 100:
      fprintf (stderr, "cannot find `%s`\n", str1);
      break;
    case 107:
      fprintf (stderr, "cannot open result file\n");
      break;

    default:
      fprintf (stderr, "syntax error\n");
      break;
    }

  return (1);
  }
