/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/* ###--------------------------------------------------------------### */
/*									*/
/* file		: ctp_util.c						*/
/* date		: Sep 14 1992						*/
/* version	: v100							*/
/* author	: Pirouz BAZARGAN SABET, M.HANAFI			*/
/* description	: This file contains some utility functions :		*/
/*		  ctp_error  , ctp_y_error, ctp_toolbug, ctp_message,	*/
/*									*/
/* ###--------------------------------------------------------------### */
	
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

#include "mut.h"
#include "aut.h"
#include "vex.h"
#include "ctl.h"

#include "ctp_utype.h"
#include "ctp_util.h"

char             CTP_ERRFLG  = 0;            /* if = 1 no structure is made  */
char            *CTP_FILENAME = (char *)0;
long             CTP_LINNUM  = 1;            /* file's line number           */
int              CTP_NUMPTP  = 0;            /* file's line number           */
int              CTP_NUMCHOICE = 0;
char            *CTP_MODNAM  = (char *)0;
ctlfig_list     *CTP_HEADFIG = (ctlfig_list *)0;

                char *ctp_func_std_logic_1164[] =
                {
                  "to_bit",
                  "to_bitvector",
                  "to_stdulogic",
                  "to_stdlogicvector",
                  "to_stdulogicvector",
                  "to_x01",
                  "to_x01z",
                  "to_ux01",
                  "rising_edge",
                  "falling_edge",
                  "is_x",
                  NULL
                };

                char *ctp_func_std_logic_arith[] =
                {
                  "abs",
                  "shl",
                  "shr",
                  "conv_integer",
                  "conv_unsigned",
                  "conv_signed",
                  "conv_std_logic_vector",
                  "ext",
                  "sxt",
                  NULL
                };

                char *ctp_func_std_numeric_std[] =
                {
                  "shift_left",
                  "shift_right",
                  "rotate_left",
                  "rotate_right",
                  "resize",
                  "to_integer",
                  "to_unsigned",
                  "to_signed",
                  NULL
                };


/* ###--------------------------------------------------------------### */
/*  function : ctp_toolbug						*/
/* ###--------------------------------------------------------------### */
void ctp_toolbug (code,str1,str2,nbr1)

int   code;
char *str1;
char *str2;
int   nbr1;

{
  (void) fprintf (stderr,"Fatal error %d executing `%s`: ", code,str1);
  switch (code)
    {
    case 1:
      (void) fprintf (stderr,"unknown operator\n");
      break;
    case 2:
      (void) fprintf (stderr,"cannot create empty atom : %s\n",str2);
      break;
    case 3:
      (void) fprintf (stderr,"cannot build NOT of empty expression\n");
      break;
    case 4:
      (void) fprintf (stderr,"cannot combine empty expressions\n");
      break;
    case 5:
      (void) fprintf (stderr,"cannot find terminal\n");
      break;
    case 6:
      (void) fprintf (stderr,"cannot make BDD of empty expression\n");
      break;
    case 7:
      (void) fprintf (stderr,"unknown type `%c` for IO `%s`\n",nbr1,str2);
      break;
    case 8:
      (void) fprintf (stderr,"illegal signal value : hex `%x`\n",nbr1);
      break;
    case 9:
      (void) fprintf (stderr,"unknown mode `%c` for IO `%s`\n",nbr1,str2);
      break;
    case 10:
      (void) fprintf (stderr,"decompiler called on empty vbfig\n");
      break;
    case 12:
      (void) fprintf (stderr,"illegal user value '%c' for IO\n",nbr1);
      break;
    case 13 :
      (void) fprintf (stderr,"cannot find model of `%s`\n",str2);
      break;
    case 14 :
      (void) fprintf (stderr,"cannot convert empty expression\n");
      break;
    case 15 :
      (void) fprintf (stderr,"illegal bit string value : `%c`\n",nbr1);
      break;
    case 16 :
      (void) fprintf (stderr,"the same expression cannot be used twice\n");
      break;
    case 17 :
      (void) fprintf (stderr,"unknown base specifier : `%c`\n",nbr1);
      break;
    case 18:
      (void) fprintf (stderr,"unknown mode `%c` for port `%s`\n",nbr1,str2);
      break;
    case 19:
      (void) fprintf (stderr,"empty guard expression: `%s`\n",str2);
      break;
    case 20:
      (void) fprintf (stderr,"empty waveform expression: `%s`\n",str2);
      break;
    case 21:
      (void) fprintf (stderr,"cannot find structural level\n");
      break;
    case 22:
      (void) fprintf (stderr,"`%s` : `%c` unknown resolution function\n",str2,nbr1);
      break;
    }
  autexit (1);
}

/* ###--------------------------------------------------------------### */
/*  function : ctp_message						*/
/* ###--------------------------------------------------------------### */
void ctp_message (code,str1,nmb1)

int   code;
char *str1;
int   nmb1;

{
  switch (code)
    {
    case 1:
      (void) fprintf(stderr,"USAGE is ctp [filename1],[filename2],..  \n\n\n");
      break;
    case 2:
      (void) fprintf (stdout,"Initializing ...\n");
      break;
    case 3:
      (void) fprintf (stdout,"Compiling `%s` (Behaviour) ...\n",str1);
      break;
    case 4:
      (void) fprintf (stdout,"Compiling `%s` (Structural) ...\n\n",str1);
      break;
    case 5:
      (void) fprintf (stdout,"Searching `%s` ...\n",str1);
      break;
    case 8:
      (void) fprintf (stdout,"Compiling `%s` (Pattern) ...\n\n",str1);
      break;
    case 9:
      (void) fprintf (stdout,"Linking ...\n");
      break;
    case 10:
      (void) fprintf (stdout,"###----- processing pattern %d -----###\n",nmb1);
      break;
    case 11:
      (void) fprintf (stdout,"Decompiling ...\n");
      break;
    case 12:
      (void) fprintf (stdout,"Generating the file '%s' ...\n",str1);
      break;
    case 13:
      (void) fprintf (stdout,"Saving '%s' in a vhdl file (vbe)\n",str1);
      break;
    case 14:
      (void) fprintf (stdout,"File '%s' has been generated.\n",str1);
      break;
    case 15:
      (void) fprintf (stdout,"Making Bdd ...\n\n");
      break;
    case 16:
      (void) fprintf (stdout,"Restoring ...\n\n");
      break;
    case 17:
      (void) fprintf (stdout,"Parsing the file %s .........\n\n",str1);
      break;
    case 18:
      (void) fprintf (stderr,"ERROR: %s VHDL file does not exist !!! \n",str1);
      (void) fprintf (stderr,"          Verify the file and restart  \n");
      break;	
    default:
      (void) fprintf (stderr,"ctp_message : code %d unknown.\n",code);
    }
}

/* ###--------------------------------------------------------------### */
/*  function : ctp_error						*/
/* ###--------------------------------------------------------------### */

void ctp_error (code,str1)

int   code;
char *str1;

{
  CTP_ERRFLG++;
  if (code < 100)
    (void)fprintf (stderr,"Error %d line %ld in file %s :",code,CTP_LINNUM, CTP_FILENAME);
  else
    {
    if (code < 200)
      (void)fprintf (stderr,"Error %d :",code);
    }

  switch (code)
    {
    case 9:
      (void) fprintf (stderr,"illegal declaration\n");
      break;
    case 10:
      (void) fprintf (stderr,"symbol `%s` already declared\n",str1);
      break;
    case 18:
      (void) fprintf (stderr,"illegal statement\n");
      break;
    case 32:
      (void) fprintf (stderr,"null array not supported\n");
      break;
    case 33:
      (void) fprintf (stderr,"incompatible constraint and type\n");
      break;
    case 35:
      (void) fprintf (stderr,"illegal assignment of `%s` (widths mismatch)\n",str1);
      break;
    case 36:
      (void) fprintf (stderr,"symbol `%s` used out of declared range\n",str1);
      break;
    case 38:
      (void) fprintf (stderr,"width or/and type mismatch %s\n",str1);
      break;
    case 73:
      (void) fprintf (stderr,"`%s` is not a bit string litteral\n",str1);
      break;
    case 78:
      (void) fprintf (stderr,"illegal constant declaration\n");
      break;
    case 79:
      (void) fprintf (stderr,"illegal use of attribute on `%s`\n",str1);
      break;
    case 80:
      (void) fprintf (stderr,"variable `%s` already declared\n",str1);
      break;
    case 82:
      (void) fprintf (stderr,"enumerate valu `%s` already declared\n",str1);
      break;
    case 83:
      (void) fprintf (stderr,"'%s' illegal index subtype definition \n",str1);
      break;
    case 84:
      (void) fprintf (stderr,"'%s' illegal type mark definition \n",str1);
      break;
    case 86:
      (void) fprintf (stderr,"illegal variable declaration\n");
      break;
    case 87:
      (void) fprintf (stderr,"illegal type declaration\n");
      break;
    case 97:
      (void) fprintf (stderr,"width mismatch on signal initialisation\n");
      break;

    case 100:
      (void) fprintf (stderr,"cannot find `%s`\n",str1);
      break;
    case 107:
      (void) fprintf (stderr,"Cannot open result file\n");
      break;
    case 114:
      (void) fprintf (stderr,"illegal aggregate\n");
      break;
    case 123:
      (void) fprintf (stderr,"such array type definition is not supported\n");
      break;

    default:
      (void) fprintf (stderr,"syntax error\n");
      break;
    }

  if (CTP_ERRFLG > CTP_MXRDFN)
    {
    (void) fprintf (stderr,"Too many errors. Cannot continue further more\n");
    (void) fprintf (stderr,"\n		Have a nice day...\n");
    EXIT (1);
    }

  autexit(1);
}

/* ###--------------------------------------------------------------### */
/*  function : ctp_y_error						*/
/* ###--------------------------------------------------------------### */
void ctp_y_error (str)

char *str;
{
  CTP_ERRFLG++;
  (void)fprintf (stderr,"Error line %ld : %s\n",CTP_LINNUM,str);
}
