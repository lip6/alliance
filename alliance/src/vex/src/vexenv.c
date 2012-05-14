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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Vex                           |
|                                                             |
| File    :                   vexenv.c                        |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "vex.h"

# include <stdio.h>
# include "vexenv.h"
# include "vexerror.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  char *VEX_OPER_UPPER_NAME[ VEX_MAX_OPERATOR ] =
  {
    "&"     ,
    "NOT"   ,
    "NEG"   ,
    "EVENT" ,
    "OR"    ,
    "AND"   ,
    "XOR"   ,
    "NOR"   ,
    "NAND"  ,
    "XNOR"  ,
    "="     ,
    "/="    ,
    "<"     ,
    "<="    ,
    ">"     ,
    ">="    ,
    "+"     , 
    "-"     , 
    "*"     , 
    "/"     , 
    "**"    ,
    "MOD"   ,
    "REM"   ,
    "TO"    ,
    "DOWNTO",
    "INDEX" ,
    "LEFT"  ,
    "RIGHT",
    "LOW",
    "HIGH",
    "LENGTH",
    "RANGE",
    "REV_RANGE",
    "DRIVER",
    "IFT",
    "ARRAY",
    "INDEX_N",
    "OTHERS =>",
    "NUM_BIT",
    "ABS",
    "AF",
    "AG",
    "AX",
    "AU",
    "EF",
    "EG",
    "EX",
    "EU",
    "<->",
    "->"
  };

/* A few declaration that should be in vexenv I believe, but I leave
 * this kind of update to Mr Jacomme, master of these sources */
/* A sharp '#' is used where an operator exists but has to be
 * treated specifically.
 * Quite a few VHDL operators have no equivalent in verilog for my
 * current knowledge: NULL indicates that! */

  char *VEX_OPER_VERILOG_NAME[ VEX_MAX_OPERATOR ] =
  {
    "," ,    /* VEX_CONCAT       */
    "~" ,    /* VEX_NOT          */
    "-" ,    /* VEX_NEG          */
    NULL,    /* VEX_EVENT        */
    "|" ,    /* VEX_OR           */
    "&" ,    /* VEX_AND          */
    "^" ,    /* VEX_XOR          */
    "#" ,    /* VEX_NOR          */
    "#" ,    /* VEX_NAND         */
    "#" ,    /* VEX_NXOR         */
    "==",    /* VEX_EQ           */
    "!=",    /* VEX_NE           */
    "<" ,    /* VEX_LT           */
    "<=",    /* VEX_LE           */
    ">" ,    /* VEX_GT           */
    ">=",    /* VEX_GE           */
    "+" ,    /* VEX_ADD          */
    "-" ,    /* VEX_SUB          */
    "*" ,    /* VEX_MUL          */
    "/" ,    /* VEX_DIV          */
    "**",    /* VEX_EXP          */
    "%" ,    /* VEX_MOD          */
    "%" ,    /* VEX_REM          */
    NULL,    /* VEX_TO           */
    NULL,    /* VEX_DOWNTO       */
    NULL,    /* VEX_INDEX        */
    NULL,    /* VEX_LEFT         */
    NULL,    /* VEX_RIGHT        */
    NULL,    /* VEX_LOW          */
    NULL,    /* VEX_HIGH         */
    NULL,    /* VEX_LENGTH       */
    NULL,    /* VEX_RANGE        */
    NULL,    /* VEX_REV_RANGE    */
    NULL,    /* VEX_DRIVER       */
    "IFT",   /* VEX_IFT          */
    NULL ,   /* VEX_ARRAY        */
    NULL ,   /* VEX_INDEX_N      */
    NULL ,   /* VEX_OTHERS       */
    NULL ,   /* VEX_NUM_BIT      */
    NULL     /* VEX_ABS          */
  };

  char VEX_LITERAL_BY_ID[ VEX_MAX_ID ] =
  {
    'u', 'x', '0', '1', 'z', 'w', 'l', 'h', '-'
  };

  char *VEX_TYPE_NAME[ VEX_MAX_TYPE ] =
  {
    "SEVERITY",
    "BOOLEAN",
    "CHARACTER",
    "STRING",
    "BIT",
    "INTEGER",
    "NATURAL",
    "BIT_VECTOR",
    "STD_ULOGIC",
    "STD_LOGIC",
    "STD_ULOGIC_VECTOR",
    "STD_LOGIC_VECTOR",
    "X01",
    "X01Z",
    "UX01",
    "UX01Z",
    "UNSIGNED",
    "SIGNED",
    "SMALL_INT",
    "REG_BIT",
    "REG_VECTOR",
    "MUX_BIT",
    "MUX_VECTOR",
    "WOR_BIT",
    "WOR_VECTOR",
    "ENUMERATE",
    "ARRAY"
  };

  char *VEX_STD_FUNC_UPPER_NAME[ VEX_MAX_STD_FUNC ] =
  {
    "TO_BIT",
    "TO_BITVECTOR",
    "TO_STDULOGIC",
    "TO_STDLOGICVECTOR",
    "TO_STDULOGICVECTOR",
    "TO_X01",
    "TO_X01Z",
    "TO_UX01",
    "RISING_EDGE",
    "FALLING_EDGE",
    "IS_X",
    "ABS",
    "SHL",
    "SHR",
    "CONV_INTEGER",
    "CONV_UNSIGNED",
    "CONV_SIGNED",
    "CONV_STD_LOGIC_VECTOR",
    "EXT",
    "SXT",
    "SHIFT_LEFT",
    "SHIFT_RIGHT",
    "ROTATE_LEFT",
    "ROTATE_RIGHT",
    "RESIZE",
    "TO_INTEGER",
    "TO_UNSIGNED",
    "TO_SIGNED",
    "STD_LOGIC_VECTOR",
    "STD_ULOGIC_VECTOR",
    "SIGNED",
    "UNSIGNED"
  };

  char *VEX_ATOM_BY_ID[ VEX_MAX_ID ];
  char *VEX_OPER_NAME[ VEX_MAX_OPERATOR ];
  char *VEX_STD_FUNC_NAME[ VEX_MAX_STD_FUNC ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Vex Env                           |
|                                                             |
\------------------------------------------------------------*/

void vexenv()
{
  char Buffer[ 16 ];
  int  Index;

  for ( Index = 0; Index < VEX_MAX_ID; Index++ )
  {
    sprintf( Buffer, "'%c'", VEX_LITERAL_BY_ID[ Index ] );
    VEX_ATOM_BY_ID[ Index ] = namealloc( Buffer );
  }

  for ( Index = 0; Index < VEX_MAX_OPERATOR; Index++ )
  {
    VEX_OPER_NAME[ Index ] = namealloc( VEX_OPER_UPPER_NAME[ Index ] );
  }

  for ( Index = 0; Index < VEX_MAX_STD_FUNC; Index++ )
  {
    VEX_STD_FUNC_NAME[ Index ] = namealloc( VEX_STD_FUNC_UPPER_NAME[ Index ] );
  }
}
