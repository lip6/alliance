/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
/* file		: vbl_util.c						*/
/* date		: Sep 14 1992						*/
/* version	: v100							*/
/* author	: Pirouz BAZARGAN SABET, M.HANAFI			*/
/* description	: This file contains some utility functions :		*/
/*		  vbl_error  , vbl_y_error, vbl_toolbug, vbl_message,	*/
/*									*/
/* ###--------------------------------------------------------------### */
	
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "mut.h"
#include "aut.h"
#include "vex.h"
#include "vbh.h"

#include "vbl_utype.h"
#include "vbl_util.h"

char             VBL_ERRFLG  = 0;            /* if = 1 no structure is made  */
char            *VBL_FILENAME = (char *)0;
long             VBL_LINNUM  = 1;            /* file's line number           */
int              VBL_NUMPTP  = 0;            /* file's line number           */
int              VBL_NUMCHOICE = 0;
vbfig_list      *VBL_HEADFIG = (vbfig_list *)0;
vbmap_list      *VBL_GENMAP  = (vbmap_list *)0;

                char *vbl_func_std_logic_1164[] =
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

                char *vbl_func_std_logic_arith[] =
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

                char *vbl_func_std_numeric_std[] =
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
/*  function : vbl_toolbug						*/
/* ###--------------------------------------------------------------### */
void vbl_toolbug (code,str1,str2,nbr1)

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
/*  function : vbl_message						*/
/* ###--------------------------------------------------------------### */
void vbl_message (code,str1,nmb1)

int   code;
char *str1;
int   nmb1;

{
  switch (code)
    {
    case 1:
      (void) fprintf(stderr,"USAGE is vbl [filename1],[filename2],..  \n\n\n");
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
      (void) fprintf (stderr,"vbl_message : code %d unknown.\n",code);
    }
}

/* ###--------------------------------------------------------------### */
/*  function : vbl_error						*/
/* ###--------------------------------------------------------------### */

void vbl_error (code,str1)

int   code;
char *str1;

{
  VBL_ERRFLG++;
  if (code < 100)
    (void)fprintf (stderr,"Error %d line %ld in file %s :",code,VBL_LINNUM, VBL_FILENAME);
  else
    {
    if (code < 200)
      (void)fprintf (stderr,"Error %d :",code);
    }

  switch (code)
    {
    case 1:
      (void) fprintf (stderr,"`%s` is incompatible with the entity name\n",str1);
      break;
    case 2:
      (void) fprintf (stderr,"bad entity declaration\n");
      break;
    case 3:
      (void) fprintf (stderr,"bad port clause declaration\n");
      break;
    case 4:
      (void) fprintf (stderr,"port `%s` already declared\n",str1);
      break;
    case 5:
      (void) fprintf (stderr,"illegal port declaration\n");
      break;
    case 6:
      (void) fprintf (stderr,"bad port declaration\n");
      break;
    case 7:
      (void) fprintf (stderr,"`%s` is incompatible with the architecture name\n",str1);
      break;
    case 8:
      (void) fprintf (stderr,"bad architecture declaration\n");
      break;
    case 9:
      (void) fprintf (stderr,"illegal declaration\n");
      break;
    case 10:
      (void) fprintf (stderr,"symbol `%s` already declared\n",str1);
      break;
    case 11:
      (void) fprintf (stderr,"illegal signal declaration\n");
      break;
    case 17:
      (void) fprintf (stderr,"`%s` unknown port or signal\n",str1);
      break;
    case 18:
      (void) fprintf (stderr,"illegal concurrent statement\n");
      break;
    case 19:
      (void) fprintf (stderr,"label `%s` already used\n",str1);
      break;
    case 20:
      (void) fprintf (stderr,"`%s` is incompatible with the block's label\n",str1);
      break;
    case 21:
      (void) fprintf (stderr,"input port `%s` cannot be assigned\n",str1);
      break;
    case 22:
      (void) fprintf (stderr,"unguarded signal `%s` assigned by guarded assignment\n",str1);
      break;
    case 23:
      (void) fprintf (stderr,"guarded signal `%s` assigned by unguarded assignment\n",str1);
      break;
    case 25:
      (void) fprintf (stderr,"some choices missing in the selected signal assignment\n");
      break;
    case 26:
      (void) fprintf (stderr,"output port `%s` cannot be read\n",str1);
      break;
    case 27:
      (void) fprintf (stderr," '%s' only variable can be assigned with ':=' \n",str1);
      break;
    case 28:
      (void) fprintf (stderr,"duplicate choice in selected signal assignment\n");
      break;
    case 30:
      (void) fprintf (stderr,"illegal use of OTHERS in selected signal assignment\n");
      break;
    case 31:
      (void) fprintf (stderr,"illegal use of OTHERS in case statement \n");
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
      (void) fprintf (stderr,"signal `%s` used out of declared range\n",str1);
      break;
    case 38:
      (void) fprintf (stderr,"width or/and type mismatch %s\n",str1);
      break;
    case 39:
      (void) fprintf (stderr,"signal `%s` assigned more than once\n",str1);
      break;
    case 40:
      (void) fprintf (stderr,"signal `%s` never assigned\n",str1);
      break;
    case 41:
      (void) fprintf (stderr,"usage of GUARD statement outside a block not permitted\n");
      break;
    case 68:
      (void) fprintf (stderr,"BEPOR type is unknown\n");
      break;
    case 73:
      (void) fprintf (stderr,"`%s` is not a bit string litteral\n",str1);
      break;
    case 74:
      (void) fprintf (stderr,"bad generic declaration\n");
      break;
    case 75:
      (void) fprintf (stderr,"bad generic element\n");
      break;
    case 76:
      (void) fprintf (stderr,"`%s`: when expression must be a constant\n",str1);
      break;
    case 77:
      (void) fprintf (stderr,"illegal generic declaration\n");
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
    case 81:
      (void) fprintf (stderr,"internal signal not allowed `%s`\n",str1);
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
    case 85:
      (void) fprintf (stderr,"when choice is empty in case statement \n");
      break;
    case 86:
      (void) fprintf (stderr,"illegal variable declaration\n");
      break;
    case 87:
      (void) fprintf (stderr,"illegal type declaration\n");
      break;
    case 94:
      (void) fprintf (stderr,"The %s instruction is not accepted in the VHDL SUBSET  \n",str1);
      break;
    case 95:
      (void) fprintf (stderr,"`%s` is incompatible with the loop's label\n",str1);
      break;
    case 96:
      (void) fprintf (stderr,"A process with sensitivity list cannot have WAIT statements\n");
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
    case 112:
      (void) fprintf (stderr,"multiple driver on guarded signal `%s`\n",str1);
      break;

    case 113:
      (void) fprintf (stderr,"bad subprogram declaration\n");
      break;
    case 114:
      (void) fprintf (stderr,"illegal aggregate\n");
      break;
    case 115:
      (void) fprintf (stderr,"illegal function declaration\n");
      break;
    case 116:
      (void) fprintf (stderr,"function `%s` already declared\n",str1);
      break;
    case 117:
      (void) fprintf (stderr,"procedure `%s` already declared\n",str1);
      break;
    case 118:
      (void) fprintf (stderr,"unknown procedure or function `%s`\n",str1);
      break;
    case 119:
      (void) fprintf (stderr,"illegal procedure call `%s`\n",str1);
      break;
    case 120:
      (void) fprintf (stderr,"illegal return statement\n");
      break;
    case 121:
      (void) fprintf (stderr,"illegal for statement\n");
      break;
    case 122:
      (void) fprintf (stderr,"illegal qualified expression\n");
      break;
    case 123:
      (void) fprintf (stderr,"such array type definition is not supported\n");
      break;
    case 124:
      (void) fprintf (stderr,"bad association list (%s)\n",str1);
      break;
    case 125:
      (void) fprintf (stderr,"unknown model name (%s)\n",str1);
      break;
    case 126:
      (void) fprintf (stderr,"bad generic declaration\n");
      break;
    case 127:
      (void) fprintf (stderr,"unknown package %s\n", str1 );
      break;
    case 128:
      (void) fprintf (stderr,"`%s` is incompatible with the package name\n",str1);
      break;
    case 129:
      (void) fprintf (stderr,"bad package body declaration\n");
      break;

    default:
      (void) fprintf (stderr,"syntax error\n");
      break;
    }

  if (VBL_ERRFLG > VBL_MXRDFN)
    {
    (void) fprintf (stderr,"Too many errors. Cannot continue further more\n");
    (void) fprintf (stderr,"\n		Have a nice day...\n");
    EXIT (1);
    }

  autexit(1);
}

/* ###--------------------------------------------------------------### */
/*  function : vbl_y_error						*/
/* ###--------------------------------------------------------------### */
void vbl_y_error (str)

char *str;
{
  VBL_ERRFLG++;
  (void)fprintf (stderr,"Error line %ld : %s\n",VBL_LINNUM,str);
}
