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
/* file		: pat_error.c						*/
/* date		: Aug 27 1997						*/
/* version	: v107							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: system level functions				*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>

/* ###--------------------------------------------------------------### */
/* function	: pat_error						*/
/* description	: print an error message depending on the arg. `code`	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

unsigned int pat_error (code, str, dat, line)

int           code;
char         *str ;
char          dat ;
unsigned int  line;

  {
  if (code < 100)
    fprintf (stderr,"PAT : Error %d line %d : ", code, line);
  else
    {
    if (code < 200)
      fprintf (stderr,"PAT : Compiler err. %d : ", code);
    else
      fprintf (stderr,"PAT : Error %d : ", code);
    }

  switch (code)
    {
    case 1:
      fprintf (stderr, "illegal format for one-bit signal\n");
      break;
    case 2:
      fprintf (stderr, "illegal input-output value `%c`\n", dat);
      break;
    case 3:
      fprintf (stderr, "too many input-output values\n");
      break;
    case 4:
      fprintf (stderr, "cannot force the value of output `%s`\n", str);
      break;
    case 5:
      fprintf (stderr, "cannot compare the value of input `%s`\n", str);
      break;
    case 6:
      fprintf (stderr, "too many elements in virtual array\n");
      break;
    case 7:
      fprintf (stderr, "`%s` already declared with another mode\n", str);
      break;
    case 8:
      fprintf (stderr, "group `%s` already declared\n", str);
      break;
    case 10:
      fprintf (stderr, "illegal value for `%s` format `%c`\n", str, dat);
      break;
    case 12:
      fprintf (stderr, "range of `%s` and value mismatch\n", str);
      break;
    case 13:
      fprintf (stderr, "`%s` already declared as an input\n", str);
      break;
    case 14:
      fprintf (stderr, "too many hierarchical names\n");
      break;
    case 15:
      fprintf (stderr, "no enough input-output values\n");
      break;
    case 16:
      fprintf (stderr, "patterns must have strictly increasing time\n");
      break;
    case 17:
      fprintf (stderr, "time overflow: max time is 2^31 steps of time unit\n");
      break;

    case 99:
      fprintf (stderr, "%s\n", str);
      break;

    case 100:
      fprintf (stderr, "unexistent or erroneous sequence\n");
      break;
    case 101:
      fprintf (stderr, "cannot drive NULL or erroneous sequence\n");
      break;
    case 102:
      fprintf (stderr, "cannot open file `%s`\n", str);
      break;

    case 200:
      fprintf (stderr, "unknown time unit\n");
      break;
    case 201:
      fprintf (stderr, "illegal input-output value `%c`\n", dat);
      break;
    case 202:
      fprintf (stderr, "illegal input-output mode `%c` : %s \n", dat, str);
      break;

    default :
      fprintf (stderr, "syntax error\n");
      break;
    }

  return (1);
  }
