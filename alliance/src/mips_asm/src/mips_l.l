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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: mips_mips.lex						*/
/* date		: Dec  6 1999						*/
/* version	: v0.3							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: MIPS assembler - lex rules				*/
/* ###--------------------------------------------------------------### */

%{
#include <stdio.h>
#include "mut.h"
#include "mips_type.h"
#include "mips_y.h"
#include "mips_defs.h"
#include "mips_lex.h"
%}

register		\$([0-9]|[12][0-9]|3[01])
digit			[0-9]
hexadecimal_valu	0x[0-9a-fA-F]+
octal_valu		0o[0-7]+
binary_valu		0b[0-1]+
decimal_valu		{digit}+
letter			[a-zA-Z]
letter_or_digit		{letter}|{digit}
ident			(\.|_|{letter})(\.|_|{letter_or_digit})*

%%
\n				{
/*printf ("CarriageReturn : %d\n", MPS_LINNUM);*/
			MPS_LINNUM++;
			return (CarriageReturn);
				}
\:				{
/*printf ("Colon\n");*/
			return (Colon);
				}
[ \t]				{
/*printf ("space\n");*/
				}
#				{
/*printf ("Sharp\n");*/
			return (Sharp);
				}
\&				{
/*printf ("Ampersand\n");*/
			return (Ampersand);
				}
\|				{
/*printf ("Bar\n");*/
			return (Bar);
				}
\~				{
/*printf ("Tilda\n");*/
			return (Tilda);
				}
\^				{
/*printf ("Circomflex\n");*/
			return (Circomflex);
				}
\+				{
/*printf ("Plus\n");*/
			return (Plus);
				}
\/				{
/*printf ("Slash\n");*/
			return (Slash);
				}
\*				{
/*printf ("Star\n");*/
			return (Star);
				}
\<\<				{
/*printf ("DoubleLess\n");*/
			return (DoubleLess);
				}
\>\>				{
/*printf ("DoubleGreat\n");*/
			return (DoubleGreat);
				}
\-				{
/*printf ("Minus\n");*/
			return (Minus);
				}
\(				{
/*printf ("LeftParen\n");*/
			return (LeftParen);
				}
\)				{
/*printf ("RightParen\n");*/
			return (RightParen);
				}
\,				{
/*printf ("Comma\n");*/
			return (Comma);
				}
{register}			{
/*printf ("Register : %s\n", yytext);*/
			sscanf (&yytext[1], "%lu", &yylval.valu);
			return (Register);
				}
{hexadecimal_valu}		{
/*printf ("Literal (X) : %s\n", yytext);*/
 			yylval.valu = hextoint (yytext);
			return (Litteral);
				}
{octal_valu}			{
/*printf ("Literal (O) : %s\n", yytext);*/
 			yylval.valu = octtoint (yytext);
			return (Litteral);
				}
{binary_valu}			{
/*printf ("Literal (B) : %s\n", yytext);*/
 			yylval.valu = bintoint (yytext);
			return (Litteral);
				}
{decimal_valu}			{
/*printf ("Literal (D) : %s\n", yytext);*/
 			if (sscanf (yytext, "%ld", &yylval.valu) == 0)
			  yylval.valu = 0;
			return (Litteral);
				}

\".*\"				{
			extern char *mips_allocstr ();

/*printf ("String : %s\n", yytext);*/
			yylval.text = mips_allocstr (yytext);
			return (String);
				}
{ident}				{
			int          type            ;
			extern char *mips_allocstr ();

			yylval.text = mips_allocstr (yytext);

			type = searchtype (yylval.text);
			if (type == -1)
			  {
/*printf ("Identifier : %s\n", yytext);*/
			  return (Identifier);
			  }
			else
			  {
/*printf ("Key Word : %s\n", yytext);*/
			  yylval.valu = searchvalu (yylval.text);
			  return (type);
			  }
			}
\;.*$				{
/*printf ("Comment : %s\n", yytext);*/
				}
%%

static int searchtype (str)

char *str;
  {
  extern char *mips_allocstr ()  ;
  static ht   *type_hashpt = NULL;

  if (type_hashpt == NULL)
    {
    type_hashpt = addht (128);

    addhtitem (type_hashpt, mips_allocstr ("zero"    ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("at"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("v0"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("v1"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("a0"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("a1"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("a2"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("a3"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t0"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t1"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t2"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t3"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t4"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t5"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t6"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t7"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s0"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s1"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s2"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s3"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s4"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s5"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s6"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s7"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t8"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("t9"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("k0"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("k1"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("gp"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("sp"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("s8"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("ra"      ), IntegerRegister );

    addhtitem (type_hashpt, mips_allocstr ("r0"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r1"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r2"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r3"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r4"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r5"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r6"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r7"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r8"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r9"      ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r10"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r11"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r12"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r13"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r14"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r15"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r16"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r17"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r18"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r19"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r20"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r21"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r22"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r23"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r24"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r25"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r26"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r27"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r28"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r29"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r30"     ), IntegerRegister );
    addhtitem (type_hashpt, mips_allocstr ("r31"     ), IntegerRegister );

    addhtitem (type_hashpt, mips_allocstr ("badvaddr"), CP0Register     );
    addhtitem (type_hashpt, mips_allocstr ("status"  ), CP0Register     );
    addhtitem (type_hashpt, mips_allocstr ("cause"   ), CP0Register     );
    addhtitem (type_hashpt, mips_allocstr ("epc"     ), CP0Register     );

    addhtitem (type_hashpt, mips_allocstr (".align"  ), _ALIGN          );
    addhtitem (type_hashpt, mips_allocstr (".ascii"  ), _ASCII          );
    addhtitem (type_hashpt, mips_allocstr (".asciiz" ), _ASCIIZ         );
    addhtitem (type_hashpt, mips_allocstr (".byte"   ), _BYTE           );
    addhtitem (type_hashpt, mips_allocstr (".bytez"  ), _BYTEZ          );
    addhtitem (type_hashpt, mips_allocstr (".data"   ), _DATA           );
    addhtitem (type_hashpt, mips_allocstr (".end"    ), _END            );
    addhtitem (type_hashpt, mips_allocstr (".equ"    ), _EQU            );
    addhtitem (type_hashpt, mips_allocstr (".global" ), _GLOBAL         );
    addhtitem (type_hashpt, mips_allocstr (".org"    ), _ORG            );
    addhtitem (type_hashpt, mips_allocstr (".skip"   ), _SKIP           );
    addhtitem (type_hashpt, mips_allocstr (".space"  ), _SPACE          );
    addhtitem (type_hashpt, mips_allocstr (".start"  ), _START          );
    addhtitem (type_hashpt, mips_allocstr (".text"   ), _TEXT           );
    addhtitem (type_hashpt, mips_allocstr (".word"   ), _WORD           );

    addhtitem (type_hashpt, mips_allocstr ("nop"     ), Macro_n         );
    addhtitem (type_hashpt, mips_allocstr ("loadi"   ), Macro_rd_longi  );
    addhtitem (type_hashpt, mips_allocstr ("la"      ), Macro_rd_longi  );
    addhtitem (type_hashpt, mips_allocstr ("li"      ), Macro_rd_longi  );

    addhtitem (type_hashpt, mips_allocstr ("add"     ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("addi"    ), Codop_rd_rs_imd );
    addhtitem (type_hashpt, mips_allocstr ("addu"    ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("addiu"   ), Codop_rd_rs_imd );
    addhtitem (type_hashpt, mips_allocstr ("and"     ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("andi"    ), Codop_rd_rs_imd );
    addhtitem (type_hashpt, mips_allocstr ("beq"     ), Codop_rs_rt_labl);
    addhtitem (type_hashpt, mips_allocstr ("bgez"    ), Codop_rs_labl   );
    addhtitem (type_hashpt, mips_allocstr ("bgezal"  ), Codop_rs_labl   );
    addhtitem (type_hashpt, mips_allocstr ("bgtz"    ), Codop_rs_labl   );
    addhtitem (type_hashpt, mips_allocstr ("blez"    ), Codop_rs_labl   );
    addhtitem (type_hashpt, mips_allocstr ("bltz"    ), Codop_rs_labl   );
    addhtitem (type_hashpt, mips_allocstr ("bltzal"  ), Codop_rs_labl   );
    addhtitem (type_hashpt, mips_allocstr ("bne"     ), Codop_rs_rt_labl);
    addhtitem (type_hashpt, mips_allocstr ("break"   ), Codop_imd       );
    addhtitem (type_hashpt, mips_allocstr ("j"       ), Codop_labl      );
    addhtitem (type_hashpt, mips_allocstr ("jal"     ), Codop_labl      );
    addhtitem (type_hashpt, mips_allocstr ("jalr"    ), Codop_od_rs     );
    addhtitem (type_hashpt, mips_allocstr ("jr"      ), Codop_rs        );
    addhtitem (type_hashpt, mips_allocstr ("lb"      ), Codop_rd_mem    );
    addhtitem (type_hashpt, mips_allocstr ("lbu"     ), Codop_rd_mem    );
    addhtitem (type_hashpt, mips_allocstr ("lh"      ), Codop_rd_mem    );
    addhtitem (type_hashpt, mips_allocstr ("lhu"     ), Codop_rd_mem    );
    addhtitem (type_hashpt, mips_allocstr ("lui"     ), Codop_rd_imd    );
    addhtitem (type_hashpt, mips_allocstr ("lw"      ), Codop_rd_mem    );

    addhtitem (type_hashpt, mips_allocstr ("mfc0"    ), Codop_rt_cp0rd  );
    addhtitem (type_hashpt, mips_allocstr ("mfhi"    ), Codop_rd        );
    addhtitem (type_hashpt, mips_allocstr ("mflo"    ), Codop_rd        );
    addhtitem (type_hashpt, mips_allocstr ("mtc0"    ), Codop_rt_cp0rd  );
    addhtitem (type_hashpt, mips_allocstr ("mthi"    ), Codop_rs        );
    addhtitem (type_hashpt, mips_allocstr ("mtlo"    ), Codop_rs        );

    addhtitem (type_hashpt, mips_allocstr ("nor"     ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("or"      ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("ori"     ), Codop_rd_rs_imd );
    addhtitem (type_hashpt, mips_allocstr ("rfe"     ), Codop_n         );
    addhtitem (type_hashpt, mips_allocstr ("sb"      ), Codop_rt_mem    );
    addhtitem (type_hashpt, mips_allocstr ("sh"      ), Codop_rt_mem    );
    addhtitem (type_hashpt, mips_allocstr ("sleep"   ), Codop_n         );
    addhtitem (type_hashpt, mips_allocstr ("sll"     ), Codop_rd_rt_sham);
    addhtitem (type_hashpt, mips_allocstr ("sllv"    ), Codop_rd_rt_rs  );
    addhtitem (type_hashpt, mips_allocstr ("slt"     ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("slti"    ), Codop_rd_rs_imd );
    addhtitem (type_hashpt, mips_allocstr ("sltiu"   ), Codop_rd_rs_imd );
    addhtitem (type_hashpt, mips_allocstr ("sltu"    ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("sra"     ), Codop_rd_rt_sham);
    addhtitem (type_hashpt, mips_allocstr ("srav"    ), Codop_rd_rt_rs  );
    addhtitem (type_hashpt, mips_allocstr ("srl"     ), Codop_rd_rt_sham);
    addhtitem (type_hashpt, mips_allocstr ("srlv"    ), Codop_rd_rt_rs  );
    addhtitem (type_hashpt, mips_allocstr ("sub"     ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("subu"    ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("sw"      ), Codop_rt_mem    );
    addhtitem (type_hashpt, mips_allocstr ("swap"    ), Codop_rt_mem    );
    addhtitem (type_hashpt, mips_allocstr ("syscall" ), Codop_n         );
    addhtitem (type_hashpt, mips_allocstr ("xor"     ), Codop_rd_rs_rt  );
    addhtitem (type_hashpt, mips_allocstr ("xori"    ), Codop_rd_rs_imd );

    }
  return (gethtitem (type_hashpt, str));
  }

static int searchvalu (str)

char *str;
  {
  extern char *mips_allocstr ()  ;
  static ht   *valu_hashpt = NULL;

  if (valu_hashpt == NULL)
    {
    valu_hashpt = addht (128);

    addhtitem (valu_hashpt, mips_allocstr ("zero"    ),  0       );
    addhtitem (valu_hashpt, mips_allocstr ("at"      ),  1       );
    addhtitem (valu_hashpt, mips_allocstr ("v0"      ),  2       );
    addhtitem (valu_hashpt, mips_allocstr ("v1"      ),  3       );
    addhtitem (valu_hashpt, mips_allocstr ("a0"      ),  4       );
    addhtitem (valu_hashpt, mips_allocstr ("a1"      ),  5       );
    addhtitem (valu_hashpt, mips_allocstr ("a2"      ),  6       );
    addhtitem (valu_hashpt, mips_allocstr ("a3"      ),  7       );
    addhtitem (valu_hashpt, mips_allocstr ("t0"      ),  8       );
    addhtitem (valu_hashpt, mips_allocstr ("t1"      ),  9       );
    addhtitem (valu_hashpt, mips_allocstr ("t2"      ), 10       );
    addhtitem (valu_hashpt, mips_allocstr ("t3"      ), 11       );
    addhtitem (valu_hashpt, mips_allocstr ("t4"      ), 12       );
    addhtitem (valu_hashpt, mips_allocstr ("t5"      ), 13       );
    addhtitem (valu_hashpt, mips_allocstr ("t6"      ), 14       );
    addhtitem (valu_hashpt, mips_allocstr ("t7"      ), 15       );
    addhtitem (valu_hashpt, mips_allocstr ("s0"      ), 16       );
    addhtitem (valu_hashpt, mips_allocstr ("s1"      ), 17       );
    addhtitem (valu_hashpt, mips_allocstr ("s2"      ), 18       );
    addhtitem (valu_hashpt, mips_allocstr ("s3"      ), 19       );
    addhtitem (valu_hashpt, mips_allocstr ("s4"      ), 20       );
    addhtitem (valu_hashpt, mips_allocstr ("s5"      ), 21       );
    addhtitem (valu_hashpt, mips_allocstr ("s6"      ), 22       );
    addhtitem (valu_hashpt, mips_allocstr ("s7"      ), 23       );
    addhtitem (valu_hashpt, mips_allocstr ("t8"      ), 24       );
    addhtitem (valu_hashpt, mips_allocstr ("t9"      ), 25       );
    addhtitem (valu_hashpt, mips_allocstr ("k0"      ), 26       );
    addhtitem (valu_hashpt, mips_allocstr ("k1"      ), 27       );
    addhtitem (valu_hashpt, mips_allocstr ("gp"      ), 28       );
    addhtitem (valu_hashpt, mips_allocstr ("sp"      ), 29       );
    addhtitem (valu_hashpt, mips_allocstr ("s8"      ), 30       );
    addhtitem (valu_hashpt, mips_allocstr ("ra"      ), 31       );

    addhtitem (valu_hashpt, mips_allocstr ("r0"      ), 0        );
    addhtitem (valu_hashpt, mips_allocstr ("r1"      ), 1        );
    addhtitem (valu_hashpt, mips_allocstr ("r2"      ), 2        );
    addhtitem (valu_hashpt, mips_allocstr ("r3"      ), 3        );
    addhtitem (valu_hashpt, mips_allocstr ("r4"      ), 4        );
    addhtitem (valu_hashpt, mips_allocstr ("r5"      ), 5        );
    addhtitem (valu_hashpt, mips_allocstr ("r6"      ), 6        );
    addhtitem (valu_hashpt, mips_allocstr ("r7"      ), 7        );
    addhtitem (valu_hashpt, mips_allocstr ("r8"      ), 8        );
    addhtitem (valu_hashpt, mips_allocstr ("r9"      ), 9        );
    addhtitem (valu_hashpt, mips_allocstr ("r10"     ), 10       );
    addhtitem (valu_hashpt, mips_allocstr ("r11"     ), 11       );
    addhtitem (valu_hashpt, mips_allocstr ("r12"     ), 12       );
    addhtitem (valu_hashpt, mips_allocstr ("r13"     ), 13       );
    addhtitem (valu_hashpt, mips_allocstr ("r14"     ), 14       );
    addhtitem (valu_hashpt, mips_allocstr ("r15"     ), 15       );
    addhtitem (valu_hashpt, mips_allocstr ("r16"     ), 16       );
    addhtitem (valu_hashpt, mips_allocstr ("r17"     ), 17       );
    addhtitem (valu_hashpt, mips_allocstr ("r18"     ), 18       );
    addhtitem (valu_hashpt, mips_allocstr ("r19"     ), 19       );
    addhtitem (valu_hashpt, mips_allocstr ("r20"     ), 20       );
    addhtitem (valu_hashpt, mips_allocstr ("r21"     ), 21       );
    addhtitem (valu_hashpt, mips_allocstr ("r22"     ), 22       );
    addhtitem (valu_hashpt, mips_allocstr ("r23"     ), 23       );
    addhtitem (valu_hashpt, mips_allocstr ("r24"     ), 24       );
    addhtitem (valu_hashpt, mips_allocstr ("r25"     ), 25       );
    addhtitem (valu_hashpt, mips_allocstr ("r26"     ), 26       );
    addhtitem (valu_hashpt, mips_allocstr ("r27"     ), 27       );
    addhtitem (valu_hashpt, mips_allocstr ("r28"     ), 28       );
    addhtitem (valu_hashpt, mips_allocstr ("r29"     ), 29       );
    addhtitem (valu_hashpt, mips_allocstr ("r30"     ), 30       );
    addhtitem (valu_hashpt, mips_allocstr ("r31"     ), 31       );

    addhtitem (valu_hashpt, mips_allocstr ("badvaddr"), _BADVADDR);
    addhtitem (valu_hashpt, mips_allocstr ("status"  ), _STATUS  );
    addhtitem (valu_hashpt, mips_allocstr ("cause"   ), _CAUSE   );
    addhtitem (valu_hashpt, mips_allocstr ("epc"     ), _EPC     );

    addhtitem (valu_hashpt, mips_allocstr (".align"  ), _ALIGN   );
    addhtitem (valu_hashpt, mips_allocstr (".ascii"  ), _ASCII   );
    addhtitem (valu_hashpt, mips_allocstr (".asciiz" ), _ASCIIZ  );
    addhtitem (valu_hashpt, mips_allocstr (".byte"   ), _BYTE    );
    addhtitem (valu_hashpt, mips_allocstr (".bytez"  ), _BYTEZ   );
    addhtitem (valu_hashpt, mips_allocstr (".data"   ), _DATA    );
    addhtitem (valu_hashpt, mips_allocstr (".end"    ), _END     );
    addhtitem (valu_hashpt, mips_allocstr (".equ"    ), _EQU     );
    addhtitem (valu_hashpt, mips_allocstr (".global" ), _GLOBAL  );
    addhtitem (valu_hashpt, mips_allocstr (".org"    ), _ORG     );
    addhtitem (valu_hashpt, mips_allocstr (".skip"   ), _SKIP    );
    addhtitem (valu_hashpt, mips_allocstr (".space"  ), _SPACE   );
    addhtitem (valu_hashpt, mips_allocstr (".start"  ), _START   );
    addhtitem (valu_hashpt, mips_allocstr (".text"   ), _TEXT    );
    addhtitem (valu_hashpt, mips_allocstr (".word"   ), _WORD    );

    addhtitem (valu_hashpt, mips_allocstr ("nop"     ), _NOP     );
    addhtitem (valu_hashpt, mips_allocstr ("loadi"   ), _LOADI   );
    addhtitem (valu_hashpt, mips_allocstr ("la"      ), _LOADI   );
    addhtitem (valu_hashpt, mips_allocstr ("li"      ), _LOADI   );

    addhtitem (valu_hashpt, mips_allocstr ("add"     ), _ADD     );
    addhtitem (valu_hashpt, mips_allocstr ("addi"    ), _ADDI    );
    addhtitem (valu_hashpt, mips_allocstr ("addu"    ), _ADDU    );
    addhtitem (valu_hashpt, mips_allocstr ("addiu"   ), _ADDIU   );
    addhtitem (valu_hashpt, mips_allocstr ("and"     ), _AND     );
    addhtitem (valu_hashpt, mips_allocstr ("andi"    ), _ANDI    );
    addhtitem (valu_hashpt, mips_allocstr ("beq"     ), _BEQ     );
    addhtitem (valu_hashpt, mips_allocstr ("bgez"    ), _BGEZ    );
    addhtitem (valu_hashpt, mips_allocstr ("bgezal"  ), _BGEZAL  );
    addhtitem (valu_hashpt, mips_allocstr ("bgtz"    ), _BGTZ    );
    addhtitem (valu_hashpt, mips_allocstr ("blez"    ), _BLEZ    );
    addhtitem (valu_hashpt, mips_allocstr ("bltz"    ), _BLTZ    );
    addhtitem (valu_hashpt, mips_allocstr ("bltzal"  ), _BLTZAL  );
    addhtitem (valu_hashpt, mips_allocstr ("bne"     ), _BNE     );
    addhtitem (valu_hashpt, mips_allocstr ("break"   ), _BREAK   );
    addhtitem (valu_hashpt, mips_allocstr ("j"       ), _J       );
    addhtitem (valu_hashpt, mips_allocstr ("jal"     ), _JAL     );
    addhtitem (valu_hashpt, mips_allocstr ("jalr"    ), _JALR    );
    addhtitem (valu_hashpt, mips_allocstr ("jr"      ), _JR      );
    addhtitem (valu_hashpt, mips_allocstr ("lb"      ), _LB      );
    addhtitem (valu_hashpt, mips_allocstr ("lbu"     ), _LBU     );
    addhtitem (valu_hashpt, mips_allocstr ("lh"      ), _LH      );
    addhtitem (valu_hashpt, mips_allocstr ("lhu"     ), _LHU     );
    addhtitem (valu_hashpt, mips_allocstr ("lui"     ), _LUI     );
    addhtitem (valu_hashpt, mips_allocstr ("lw"      ), _LW      );
    addhtitem (valu_hashpt, mips_allocstr ("mfc0"    ), _MFC0    );
    addhtitem (valu_hashpt, mips_allocstr ("mfhi"    ), _MFHI    );
    addhtitem (valu_hashpt, mips_allocstr ("mflo"    ), _MFLO    );
    addhtitem (valu_hashpt, mips_allocstr ("mtc0"    ), _MTC0    );
    addhtitem (valu_hashpt, mips_allocstr ("mthi"    ), _MTHI    );
    addhtitem (valu_hashpt, mips_allocstr ("mtlo"    ), _MTLO    );
    addhtitem (valu_hashpt, mips_allocstr ("nor"     ), _NOR     );
    addhtitem (valu_hashpt, mips_allocstr ("or"      ), _OR      );
    addhtitem (valu_hashpt, mips_allocstr ("ori"     ), _ORI     );
    addhtitem (valu_hashpt, mips_allocstr ("rfe"     ), _RSTFE   );
    addhtitem (valu_hashpt, mips_allocstr ("sb"      ), _SB      );
    addhtitem (valu_hashpt, mips_allocstr ("sh"      ), _SH      );
    addhtitem (valu_hashpt, mips_allocstr ("sleep"   ), _SLEEP   );
    addhtitem (valu_hashpt, mips_allocstr ("sll"     ), _SLL     );
    addhtitem (valu_hashpt, mips_allocstr ("sllv"    ), _SLLV    );
    addhtitem (valu_hashpt, mips_allocstr ("slt"     ), _SLT     );
    addhtitem (valu_hashpt, mips_allocstr ("slti"    ), _SLTI    );
    addhtitem (valu_hashpt, mips_allocstr ("sltiu"   ), _SLTIU   );
    addhtitem (valu_hashpt, mips_allocstr ("sltu"    ), _SLTU    );
    addhtitem (valu_hashpt, mips_allocstr ("sra"     ), _SRA     );
    addhtitem (valu_hashpt, mips_allocstr ("srav"    ), _SRAV    );
    addhtitem (valu_hashpt, mips_allocstr ("srl"     ), _SRL     );
    addhtitem (valu_hashpt, mips_allocstr ("srlv"    ), _SRLV    );
    addhtitem (valu_hashpt, mips_allocstr ("sub"     ), _SUB     );
    addhtitem (valu_hashpt, mips_allocstr ("subu"    ), _SUBU    );
    addhtitem (valu_hashpt, mips_allocstr ("sw"      ), _SW      );
    addhtitem (valu_hashpt, mips_allocstr ("swap"    ), _SWAP    );
    addhtitem (valu_hashpt, mips_allocstr ("syscall" ), _SYSCALL );
    addhtitem (valu_hashpt, mips_allocstr ("xor"     ), _XOR     );
    addhtitem (valu_hashpt, mips_allocstr ("xori"    ), _XORI    );

    }
  return (gethtitem (valu_hashpt, str));
  }

static unsigned long hextoint (str)

char *str;

  {
  unsigned long valu = 0;
  char          c    = 0;

  while ((c = *str) != '\0')
    {
    if ((c >= '0') && (c <= '9'))
      valu = (valu << 4) + c - '0';
    else
      if ((c >= 'a') && (c <= 'f'))
        valu = (valu << 4) + c - 'a' + 10;
      else
        if ((c >= 'A') && (c <= 'F'))
          valu = (valu << 4) + c - 'A' + 10;
    str++;
    }

  return (valu);
  }

static unsigned long octtoint (str)

char *str;

  {
  unsigned long valu = 0;
  char          c    = 0;

  while ((c = *str) != '\0')
    {
    if ((c >= '0') && (c <= '7'))
      valu = (valu << 3) + c - '0';

    str++;
    }

  return (valu);
  }

static unsigned long bintoint (str)

char *str;

  {
  unsigned long valu = 0;
  char          c    = 0;

  while ((c = *str) != '\0')
    {
    if ((c == '0') || (c == '1'))
      valu = (valu << 1) + c - '0';

    str++;
    }

  return (valu);
  }

int yywrap ()
  {
  return (1);
  }
