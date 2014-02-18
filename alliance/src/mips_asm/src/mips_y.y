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

/* ###--------------------------------------------------------------### */
/* file		: mips_mips.yac						*/
/* date		: Dec  6 1999						*/
/* version	: v0.3							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: MIPS assembler - yacc rules				*/
/* ###--------------------------------------------------------------### */

%{
#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "mips_type.h"
#include "mips_defs.h"
#include "mips_yacc.h"

extern  void  yyrestart ( );
extern  int   yyparse   ( );
extern  int   yylex     ( );
extern  void  yyerror   ( );


int  main (argc, argv)

int   argc   ;
char *argv [];

  {
  extern FILE  *yyin          ;
  extern FILE  *MPS_WRTFIL    ;
  extern FILE  *MPS_TXTFIL    ;
  extern FILE  *MPS_DATFIL    ;
  extern FILE  *MPS_SYMFIL    ;
  FILE         *fp_read       ;
  char          arg_flg [ 32] ;
  unsigned int  txt_siz       = 256;
  unsigned int  dat_siz       =  64;
  int           exit_val      =   1;
  int           asm_fil       =   0;
  int           txt_fil       =   0;
  int           dat_fil       =   0;
  int           i             ;
  char         *symbol_fil    = NULL;
  char          arg_err       =   0;
  static int    call_nbr      =   0;

	/* ###------------------------------------------------------### */
	/*   initializations						*/
	/* ###------------------------------------------------------### */

  mbkenv ();

  alliancebanner_with_authors( "asm", "v0.3",
                  "MIPS Assembler", "1992",
                  ALLIANCE_VERSION,
                  "Pirouz Bazargan Sabet" );

  for (i=1 ; i<argc ; i++)
    arg_flg [i] = 0;

  if (MPS_HSHTAB == NULL)
    MPS_HSHTAB = beh_initab ();

	/* ###------------------------------------------------------### */
	/*   analyze the command line:					*/
	/*     - for each argument set a flag to 1 if recognized	*/
	/*     - find the last recognized option to identify the source	*/
	/*       and result files					*/
	/* ###------------------------------------------------------### */

  arg_flg [0] = 1;

  for (i=0 ; i<argc ; i++)
    {
    if (arg_flg [i] == 0)
      {
      if (!strcmp (argv[i], "-textsize"))
        {
        sscanf (argv[i+1], "%u", &txt_siz);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

      if (!strcmp (argv[i], "-datasize"))
        {
        sscanf (argv[i+1], "%u", &dat_siz);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

      if (!strcmp (argv[i], "-dump"))
        {
        MPS_DUMP      = 'y'       ;
        arg_flg [i]   = 1         ;
        }

      if (!strcmp (argv[i], "-symbol"))
        {
        MPS_SYMBOL    = 'y'       ;
        symbol_fil    = argv [i+1];
        arg_flg [i]   = 1         ;
        arg_flg [i+1] = 1         ;
        }

      }
    }

  for (i=(argc-1) ; i>0 ; i--)
    {
    if (arg_flg [i] == 1)
      break;
    }

  asm_fil       = i + 1;
  arg_flg [i+1] = 1    ;
  txt_fil       = i + 2;
  arg_flg [i+2] = 1    ;

  if ((i+3) < argc)
    {
    dat_fil       = i + 3;
    arg_flg [i+3] = 1    ;
    }


	/* ###------------------------------------------------------### */
	/*   cases of errors on the command line:			*/
	/*     - a unrecognized argument				*/
	/*     - not enough arguments					*/
	/*     - too many arguments					*/
	/* ###------------------------------------------------------### */

  if ((argc > 11) || (argc < 3))
    arg_err = 1;

  for (i=0 ; i<argc ; i++)
    {
    if (arg_flg [i] == 0)
      arg_err = 1;
    }

  if (arg_err == 1)
    {
    mips_error (101, NULL);
    exit (1);
    }

	/* ###------------------------------------------------------### */
	/*   open source file (assembly) and result files (vhdl)	*/
	/* ###------------------------------------------------------### */

  if ((fp_read = fopen (argv [asm_fil], "r")) == NULL)
    {
    mips_error (100, argv [asm_fil]);
    exit (1);
    }

  if (MPS_DUMP != 'y')
    {
    if ((MPS_TXTFIL = mbkfopen (argv[txt_fil], "vbe", WRITE_TEXT)) == NULL)
      {
      mips_error (100, argv [txt_fil]);
      exit (1);
      }

    if (dat_fil != 0)
      {
      if ((MPS_DATFIL = mbkfopen (argv[dat_fil], "vbe", WRITE_TEXT)) == NULL)
        {
        mips_error (100, argv [dat_fil]);
        exit (1);
        }
      }
    }
  else
    {
    if ((MPS_TXTFIL = fopen (argv[txt_fil], "w")) == NULL)
      {
      mips_error (100, argv [txt_fil]);
      exit (1);
      }
    if (dat_fil != 0)
      {
      if ((MPS_DATFIL = fopen (argv[txt_fil], "w")) == NULL)
        {
        mips_error (100, argv [dat_fil]);
        exit (1);
        }
      }
    }

  if (MPS_SYMBOL == 'y')
    {
    if ((MPS_SYMFIL = fopen (symbol_fil, "w")) == NULL)
      {
      mips_error (100, symbol_fil);
      exit (1);
      }
    }

	/* ###------------------------------------------------------### */
	/*   compute the length (number of bits) of the text address.	*/
	/* The address is a word (4 bytes) address			*/
	/* ###------------------------------------------------------### */

  txt_siz = (txt_siz >> 2) - 1;
  while (txt_siz != 0)
    {
    txt_siz = txt_siz >> 1;
    MPS_TXTSIZ ++;
    }

	/* ###------------------------------------------------------### */
	/*   compute the length (number of bits) of the data address	*/
	/* ###------------------------------------------------------### */

  dat_siz -= 1;
  while (dat_siz != 0)
    {
    dat_siz = dat_siz >> 1;
    MPS_DATSIZ ++;
    }

	/* ###------------------------------------------------------### */
	/*   call the assembly file parser (two passes):		*/
	/*	- set the address space (by default) to text		*/
	/*      - in the first pass identify labels			*/
	/*      - if no error has been found print the result files'	*/
	/*        header						*/
	/*      - translate insstructions in the second pass		*/
	/*      - print the result files' foot				*/
	/* ###------------------------------------------------------### */

  MPS_ADRSPC = 'T'       ;
  MPS_ADDRES = MPS_TXTADR;
  MPS_WRTFIL = MPS_TXTFIL;
  MPS_ADRSIZ = MPS_TXTSIZ;
  MPS_WRDSIZ = 4         ;

  MPS_PASNBR = 1  ;

  if (call_nbr != 0)
    yyrestart (yyin);

  yyin = fp_read;
  yyparse ();

  call_nbr++;

  if (MPS_ERRFLG == 0)
    {
    fclose (fp_read);
    fp_read = fopen (argv [asm_fil], "r");
    yyin    = fp_read;

    MPS_ADRSPC = 'T';
    MPS_ADDRES = MPS_TXTADR;
    MPS_WRTFIL = MPS_TXTFIL;
    MPS_ADRSIZ = MPS_TXTSIZ;
    MPS_WRDSIZ = 4;

    MPS_PASNBR = 2  ;
    MPS_LINNUM = 1  ;

    if (MPS_DUMP != 'y')
      {
      mips_head (MPS_TXTFIL, argv [txt_fil], MPS_TXTSIZ, 4);
      if (MPS_DATFIL != NULL)
        mips_head (MPS_DATFIL, argv [dat_fil], MPS_DATSIZ, 1);
      }

    yyrestart (yyin);
    yyparse ();

    if (MPS_DUMP != 'y')
      {
      mips_foot (MPS_TXTFIL, 4);
      if (MPS_DATFIL != NULL)
        mips_foot (MPS_DATFIL, 1);
      }

    exit_val = 0;

    }

	/* ###------------------------------------------------------### */
	/*   close source and result files				*/
	/* ###------------------------------------------------------### */

  fclose (fp_read);
  fclose (MPS_TXTFIL);

  if (MPS_DATFIL != NULL)
    fclose (MPS_DATFIL);

  if (MPS_SYMBOL == 'y')
    fclose (MPS_SYMFIL);

  exit (exit_val);
  }

%}

%union
  {
  unsigned long  valu;
  struct immd    immd;
  char          *text;
  }

%token        _ALIGN
%token        _ASCII
%token        _ASCIIZ
%token        _BYTE
%token        _BYTEZ
%token        _DATA
%token        _END
%token        _EQU
%token        _GLOBAL
%token        _ORG
%token        _SPACE
%token        _SKIP
%token        _START
%token        _TEXT
%token        _WORD
%token        Ampersand
%token        Bar
%token        CarriageReturn
%token        Circomflex
%token <valu> Codop_rd
%token <valu> Codop_rd_rs_rt
%token <valu> Codop_rd_rt_sham
%token <valu> Codop_rd_rt_rs
%token <valu> Codop_rd_rs_imd
%token <valu> Codop_rs_rt_labl
%token <valu> Codop_od_rs
%token <valu> Codop_rs
%token <valu> Codop_rd_mem
%token <valu> Codop_rt_mem
%token <valu> Codop_rt_cp0rd
%token <valu> Codop_rs_labl
%token <valu> Codop_rd_imd
%token <valu> Codop_labl
%token <valu> Codop_imd
%token <valu> Codop_n
%token        Colon
%token        Comma
%token <valu> CP0Register
%token        DoubleGreat
%token        DoubleLess
%token <text> Identifier
%token <valu> IntegerRegister
%token        LeftParen
%token <valu> Litteral
%token <valu> Macro_n
%token <valu> Macro_rd_longi
%token        Minus
%token        Plus
%token <valu> Register
%token        RightParen
%token        Sharp
%token        Slash
%token        Star
%token <text> String
%token        Tilda

%type  <immd> effective_instruction

%type  <immd> mips_instruction
%type  <immd> format_n
%type  <immd> format_labl
%type  <immd> format_imd
%type  <immd> format_rd_imd
%type  <immd> format_rs_labl
%type  <immd> format_rs_rt_labl
%type  <immd> format_rd_mem
%type  <immd> format_rt_mem
%type  <immd> format_rd_rs_imd
%type  <immd> format_rd_rt_sham
%type  <immd> format_rd
%type  <immd> format_od_rs
%type  <immd> format_rs
%type  <immd> format_rd_rs_rt
%type  <immd> format_rd_rt_rs
%type  <immd> format_rt_cp0rd

%type  <immd> macro_instruction
%type  <immd> macro_ri
%type  <immd> macro_n

%type  <valu> cp0_reg
%type  <valu> destination_register
%type  <valu> source_register
%type  <valu> integer_register

%type  <valu> adding_operator
%type  <valu> logical_operator
%type  <valu> mul_div_operator
%type  <valu> shift_operator

%type  <immd> term
%type  <immd> expression
%type  <immd> expr..addop__expr..
%type  <immd> expr__mulop__expr..mulop__expr..
%type  <immd> expr__logop__expr..logop__expr..
%type  <immd> expr__shfop__expr
%type  <immd> .immediate.
%type  <immd> immediate
%type  <text> label
%type  <immd> offset
%type  <immd> byte_alloc
%type  <immd> bytez_alloc
%type  <immd> word_alloc
%type  <immd> ascii_alloc
%type  <immd> asciiz_alloc
%type  <immd> mem_alloc_directive

%start mips_assembly_prog

%%
mips_assembly_prog
	: assembly_instructions
	  .end_pseudo_inst.
        ;

assembly_instructions
	: ..an_assembly_instruction..
        ;

..an_assembly_instruction..
	: /*empty*/
	| ..an_assembly_instruction..
	  an_assembly_instruction
	;

an_assembly_instruction
	: .a_label.
	  .unlabeled_assembly_instruction.
	  CarriageReturn_ERR
	| unlabeled_pseudo_instruction
	  CarriageReturn_ERR
	| error
	  CarriageReturn_ERR
	;

.a_label.
	: /*empty*/
	| label
	  Colon
		{
		/* ###----------------------------------------------### */
		/*   in the first pass, check that the label is unique.	*/
		/* Save its address in the dictionary and flag it as a	*/
		/* label.						*/
		/* ###----------------------------------------------### */

		if (MPS_PASNBR == 1)
		  {
		  if (beh_chktab (MPS_HSHTAB, $1, NULL, 0) == 0)
		    {
		    beh_addtab (MPS_HSHTAB, $1, NULL, 0, 'l'       );
		    beh_addtab (MPS_HSHTAB, $1, NULL, 7, MPS_ADDRES);
		    if (MPS_SYMBOL == 'y')
		      mips_symbol ($1);
		    }
		  else
		    mips_error (3, NULL);
		  }
		}
	;

.unlabeled_assembly_instruction.
	: /*empty*/
	| mem_alloc_directive
		{
		if ((MPS_PASNBR == 2) && ($1.flag == 0))
		  mips_error (4, NULL);

		if (MPS_PASNBR == 2)
		  mips_print ($1.valu);

		MPS_ADDRES += $1.valu;
		}
	| effective_instruction
		{
		if ((MPS_PASNBR == 2) && ($1.flag == 0))
		  mips_error (4, NULL);

		if (MPS_PASNBR == 2)
		  mips_print ($1.valu);

		MPS_ADDRES += $1.valu;
		}
	;

mem_alloc_directive
	: word_alloc
		{ $$ = $1; }
	| byte_alloc
		{ $$ = $1; }
	| bytez_alloc
		{ $$ = $1; }
	| ascii_alloc
		{ $$ = $1; }
	| asciiz_alloc
		{ $$ = $1; }
	| skip_pseudo_inst
		{
		$$.valu = 0  ;
		$$.flag = 'n';
		}
	| space_pseudo_inst
		{
		$$.valu = 0  ;
		$$.flag = 'n';
		}
	;

word_alloc
	: _WORD
	  immediate
		{
		MPS_BYTTAB [3] = $2.valu >> 24;
		MPS_BYTTAB [2] = $2.valu >> 16;
		MPS_BYTTAB [1] = $2.valu >>  8;
		MPS_BYTTAB [0] = $2.valu      ;

		$$.valu = 4;
		$$.flag = $2.flag;
		}
	;

byte_alloc
	: _BYTE
	  immediate
		{
		/* ###----------------------------------------------### */
		/*   initialize a byte address with a value		*/
		/* ###----------------------------------------------### */

		MPS_BYTTAB [0] = $2.valu;

		$$.valu = 1      ;
		$$.flag = $2.flag;
		}
	;

bytez_alloc
	: _BYTEZ
	  immediate
		{
		/* ###----------------------------------------------### */
		/*   initialize a byte table with zeros			*/
		/* ###----------------------------------------------### */

		unsigned int i;

		for (i=0; i<$2.valu ; i++)
		  MPS_BYTTAB [i] = 0x00;

		$$.valu = $2.valu;
		$$.flag = $2.flag;
		}
	;

ascii_alloc
	: _ASCII
	  String
		{
		/* ###----------------------------------------------### */
		/*   initialize a set of addresses with a string	*/
		/* ###----------------------------------------------### */

		$$.valu = mips_strtobin ($2, MPS_BYTTAB);
		$$.flag = 'n';
		}
	;

asciiz_alloc
	: _ASCIIZ
	  String
		{
		/* ###----------------------------------------------### */
		/*   initialize a set of addresses with a string	*/
		/* followed by a null character				*/
		/* ###----------------------------------------------### */

		unsigned int size;

		size = mips_strtobin ($2, MPS_BYTTAB);
		MPS_BYTTAB [size] = '\0';

		$$.valu = size + 1;
		$$.flag = 'n';
		}
	;

unlabeled_pseudo_instruction
	: align_pseudo_inst
	| data_pseudo_inst
	| equ_pseudo_inst
	| global_pseudo_inst
	| org_pseudo_inst
	| start_pseudo_inst
	| text_pseudo_inst
	;

data_pseudo_inst
	: _DATA
	  .immediate.
		{
		/* ###----------------------------------------------### */
		/*   if the current region is the 'text region', switch	*/
		/* to the 'data region'.				*/
		/*   if an address is defined, set the current data	*/
		/* address.						*/
		/* ###----------------------------------------------### */

		if (MPS_ADRSPC == 'T')
		  {
		  MPS_TXTADR = MPS_ADDRES;

		  MPS_ADDRES = MPS_DATADR;
		  MPS_WRTFIL = MPS_DATFIL;
		  MPS_ADRSIZ = MPS_DATSIZ;
		  MPS_WRDSIZ =  1 ;
		  MPS_ADRSPC = 'D';
		  }

		if ($2.flag == 'i')
		  MPS_ADDRES = $2.valu;
		else
		  {
		  if ($2.flag != 'n')
		    mips_error (4, NULL);
		  }
		}
	;

text_pseudo_inst
	: _TEXT
	  .immediate.
		{
		/* ###----------------------------------------------### */
		/*   if the current region is the 'data region', switch	*/
		/* to the 'text region'.				*/
		/*   if an address is defined, set the current data	*/
		/* address.						*/
		/* ###----------------------------------------------### */

		if (MPS_ADRSPC == 'D')
		  {
		  MPS_DATADR = MPS_ADDRES;

		  MPS_ADDRES = MPS_TXTADR;
		  MPS_WRTFIL = MPS_TXTFIL;
		  MPS_ADRSIZ = MPS_TXTSIZ;
		  MPS_WRDSIZ = 4;
		  MPS_ADRSPC = 'T';
		  }

		if ($2.flag == 'i')
		  MPS_ADDRES = $2.valu;
		else
		  {
		  if ($2.flag != 'n')
		    mips_error (4, NULL);
		  }
		}
	;

global_pseudo_inst
	: _GLOBAL
	  label
	;

align_pseudo_inst
	: _ALIGN
	  Litteral
		{
		/* ###----------------------------------------------### */
		/*   align the current address				*/
		/* ###----------------------------------------------### */

		switch ($2)
		  {
		  case 1:
		    break;
		  case 2:
		    MPS_ADDRES = (MPS_ADDRES + 1) & 0xfffffffe;
		    break;
		  case 4:
		    MPS_ADDRES = (MPS_ADDRES + 3) & 0xfffffffc;
		    break;
		  case 8:
		    MPS_ADDRES = (MPS_ADDRES + 7) & 0xfffffff8;
		    break;
		  default:
		    mips_error (4, NULL);
		  }
		}
	;

.end_pseudo_inst.
	: /*empty*/
	| end_pseudo_inst
	;

end_pseudo_inst
	: _END
	  ..CR..
	;

start_pseudo_inst
	: _START
	  label
	;

space_pseudo_inst
	: _SPACE
	  Litteral
		{
		MPS_ADDRES += $2;
		}
	;

skip_pseudo_inst
	: _SKIP
	  Litteral
		{
		MPS_ADDRES += $2;
		}
	;

org_pseudo_inst
	: _ORG
	  immediate
		{
		/* ###----------------------------------------------### */
		/*   define the first address (must be an immediate)	*/
		/* ###----------------------------------------------### */

		if ($2.flag == 'i')
		  MPS_ADDRES = $2.valu;
		else
		  mips_error (4, NULL);
		}
	;

equ_pseudo_inst
	: Identifier
	  _EQU
	  immediate
		{
		/* ###----------------------------------------------### */
		/*   in the first pass, check that the identifier has	*/
		/* not been defined ealier.				*/
		/*   Save its value in the dictionary and flag it as	*/
		/* an immediate.					*/
		/* ###----------------------------------------------### */

		if (MPS_PASNBR == 1)
		  {
		  if ($3.flag == 'i')
		    {
		    if (beh_chktab (MPS_HSHTAB, $1, NULL, 0) == 0)
		      {
		      beh_addtab (MPS_HSHTAB, $1, NULL, 0, 'i'    );
		      beh_addtab (MPS_HSHTAB, $1, NULL, 7, $3.valu);
		      }
		    else
		      mips_error (3, NULL);
		    }
		  else
		    mips_error (4, NULL);
		  }
		}
	;

effective_instruction
	: mips_instruction
		{ $$ = $1; }
	| macro_instruction
		{ $$ = $1; }
	;

mips_instruction
	: format_n
		{ $$ = $1; }
	| format_labl
		{ $$ = $1; }
	| format_imd
		{ $$ = $1; }
	| format_rd_imd
		{ $$ = $1; }
	| format_rs_labl
		{ $$ = $1; }
	| format_rs_rt_labl
		{ $$ = $1; }
	| format_rd_mem
		{ $$ = $1; }
	| format_rt_mem
		{ $$ = $1; }
	| format_rd_rs_imd
		{ $$ = $1; }
	| format_rd_rt_sham
		{ $$ = $1; }
	| format_rd
		{ $$ = $1; }
	| format_od_rs
		{ $$ = $1; }
	| format_rs
		{ $$ = $1; }
	| format_rd_rs_rt
		{ $$ = $1; }
	| format_rd_rt_rs
		{ $$ = $1; }
	| format_rt_cp0rd
		{ $$ = $1; }
	;

format_rd_rs_rt
	: Codop_rd_rs_rt
	  destination_register
	  Comma
	  source_register
	  Comma
	  source_register
		{
		unsigned long inst = 0;

		inst           = $1 | ($4 << 21) | ($6 << 16) | ($2 << 11);
		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = 'n';
		}
	;

format_rd_rt_rs
	: Codop_rd_rt_rs
	  destination_register
	  Comma
	  source_register
	  Comma
	  source_register
		{
		unsigned long inst = 0;

		inst           = $1 | ($6 << 21) | ($4 << 16) | ($2 << 11);
		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4  ;
		$$.flag = 'n';
		}
	;

format_rt_cp0rd
	: Codop_rt_cp0rd
	  integer_register
	  Comma
	  cp0_reg
		{
		unsigned long inst = 0;

		inst           = $1 | ($2 << 16) | ($4 << 11);
		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4  ;
		$$.flag = 'n';
		}
	;

format_rd
	: Codop_rd
	  destination_register
		{
		unsigned long inst = 0;

		inst           = $1 | ($2 << 11);
		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = 'n';
		}
	;

format_od_rs
	: Codop_od_rs
	  source_register
		{
		unsigned long inst = 0x0000f800;

		inst           |= $1 | ($2 << 21);
		MPS_BYTTAB [3]  = inst >> 24;
		MPS_BYTTAB [2]  = inst >> 16;
		MPS_BYTTAB [1]  = inst >>  8;
		MPS_BYTTAB [0]  = inst      ;

		$$.valu = 4;
		$$.flag = 'n';
		}
	| Codop_od_rs
	  destination_register
	  Comma
	  source_register
		{
		unsigned long inst = 0;

		inst           = $1 | ($4 << 21) | ($2 << 11);
		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = 'n';
		}
	;

format_rs
	: Codop_rs
	  source_register
		{
		unsigned long inst = 0;

		inst           = $1 | ($2 << 21);
		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = 'n';
		}
	;

format_rd_rs_imd
	: Codop_rd_rs_imd
	  destination_register
	  Comma
	  source_register
	  Comma
	  .sharp.
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		imd  = 0x0000ffff & $7.valu;
		inst = $1 | ($4 << 21) | ($2 << 16) | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $7.flag;
		}
	;

format_rd_rt_sham
	: Codop_rd_rt_sham
	  destination_register
	  Comma
	  source_register
	  Comma
	  .sharp.
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		imd  = 0x0000001f & $7.valu;
		inst = $1 | ($4 << 16) | ($2 << 11) | (imd << 6);

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $7.flag;
		}
	;

format_rd_mem
	: Codop_rd_mem
	  destination_register
	  Comma
	  offset
	  LeftParen
	  source_register
	  RightParen
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		imd  = 0x0000ffff & $4.valu;
		inst = $1 | ($6 << 21) | ($2 << 16) | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $4.flag;
		}
	;

format_rt_mem
	: Codop_rt_mem
	  source_register
	  Comma
	  offset
	  LeftParen
	  source_register
	  RightParen
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		imd  = 0x0000ffff & $4.valu;
		inst = $1 | ($6 << 21) | ($2 << 16) | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $4.flag;
		}
	;

format_rs_rt_labl
	: Codop_rs_rt_labl
	  source_register
	  Comma
	  source_register
	  Comma
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		if ($6.flag == 'l')
		  imd = $6.valu - MPS_ADDRES - 4;
		else
		  {
		  if (MPS_PASNBR == 2)
		    mips_error (4, NULL);
		  }

		imd  = 0x0000ffff & (imd >> 2);
		inst = $1 | ($2 << 21) | ($4 << 16) | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4      ;
		$$.flag = $6.flag;
		}
	;

format_rs_labl
	: Codop_rs_labl
	  source_register
	  Comma
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		if ($4.flag == 'l')
		  imd = $4.valu - MPS_ADDRES - 4;
		else
		  {
		  if (MPS_PASNBR == 2)
		    mips_error (4, NULL);
		  }

		imd  = 0x0000ffff & (imd >> 2);
		inst = $1 | ($2 << 21) | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $4.flag;
		}
	;

format_rd_imd
	: Codop_rd_imd
	  destination_register
	  Comma
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		imd  = 0x0000ffff & $4.valu;
		inst = $1 | ($2 << 16) | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $4.flag;
		}
	;

format_imd
	: Codop_imd
	  .sharp.
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		imd  = 0x0000ffff & $3.valu;
		inst = $1 | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $3.flag;
		}
	;

format_labl
	: Codop_labl
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		imd = $2.valu;
/*--------
		if ($2.flag == 'l')
		  imd = $2.valu - MPS_ADDRES - 4;
		else
		  imd = $2.valu;
----------*/

		imd  = 0x03ffffff & (imd >> 2);
		inst = $1 | imd;

		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = $2.flag;
		}
	;

format_n
	: Codop_n
		{
		unsigned long inst = 0;

		inst           = $1;
		MPS_BYTTAB [3] = inst >> 24;
		MPS_BYTTAB [2] = inst >> 16;
		MPS_BYTTAB [1] = inst >>  8;
		MPS_BYTTAB [0] = inst      ;

		$$.valu = 4;
		$$.flag = 'n';
		}
	;

.sharp.
	: /*empty*/
	| Sharp
	;

macro_instruction
	: macro_ri
		{ $$ = $1; }
	| macro_n
		{ $$ = $1; }
	;

macro_ri
	: Macro_rd_longi
	  destination_register
	  Comma
	  immediate
		{
		unsigned long imd  = 0;
		unsigned long inst = 0;

		switch ($1)
		  {
		  case _LOADI :
		    imd  = 0x0000ffff & ($4.valu >> 16);
		    inst = _LUI | ($2 << 16) | imd;

		    MPS_BYTTAB [3] = inst >> 24;
		    MPS_BYTTAB [2] = inst >> 16;
		    MPS_BYTTAB [1] = inst >>  8;
		    MPS_BYTTAB [0] = inst      ;

		    imd  = 0x0000ffff & $4.valu;
		    inst = _ORI | ($2 << 21) | ($2 << 16) | imd;

		    MPS_BYTTAB [7] = inst >> 24;
		    MPS_BYTTAB [6] = inst >> 16;
		    MPS_BYTTAB [5] = inst >>  8;
		    MPS_BYTTAB [4] = inst      ;

		    $$.valu = 8;
		    $$.flag = $4.flag;
		    break;

		  default:
		    mips_error (5, NULL);
		  }
		}
	;

macro_n
	: Macro_n
		{
		switch ($1)
		  {
		  case _NOP :
		    MPS_BYTTAB [3] = 0x00;
		    MPS_BYTTAB [2] = 0x00;
		    MPS_BYTTAB [1] = 0x00;
		    MPS_BYTTAB [0] = 0x21;

		    $$.valu = 4;
		    $$.flag = 'n';
		    break;

		  default:
		    mips_error (5, NULL);
		  }
		}
	;

offset
	: immediate
		{ $$ = $1; }
	;

.immediate.
	: /*empty*/
		{
		$$.valu = 0  ;
		$$.flag = 'n';
		}
	| immediate
		{ $$ = $1; }
	;

immediate
	: expr..addop__expr..
		{ $$ = $1; }
	| expr__mulop__expr..mulop__expr..
		{ $$ = $1; }
	| expr__logop__expr..logop__expr..
		{ $$ = $1; }
	| expr__shfop__expr
		{ $$ = $1; }
	;

expr..addop__expr..
	: expression
		{ $$ = $1; }
	| expr..addop__expr..
	  adding_operator
	  expression
		{
		if ($2 == MPS_PLSDFN)
		  $$.valu = $1.valu + $3.valu;
		else
		  $$.valu = $1.valu - $3.valu;

		if (($1.flag == 0) || ($3.flag == 0))
		  $$.flag = 0;
		else
		  {
		  if (($1.flag == 'l') || ($3.flag == 'l'))
		    $$.flag = 'l';
		  else
		    $$.flag = 'i';
		  }
		}
	;

expr__mulop__expr..mulop__expr..
	: expression
	  mul_div_operator
	  expression
		{
		if ($2 == MPS_MULDFN)
		  $$.valu = $1.valu * $3.valu;
		else
		  $$.valu = $1.valu / $3.valu;

		if (($1.flag == 0) || ($3.flag == 0))
		  $$.flag = 0;
		else
		  {
		  if (($1.flag == 'l') || ($3.flag == 'l'))
		    $$.flag = 'l';
		  else
		    $$.flag = 'i';
		  }
		}
	| expr__mulop__expr..mulop__expr..
	  Star
	  expression
		{
		$$.valu = $1.valu * $3.valu;

		if (($1.flag == 0) || ($3.flag == 0))
		  $$.flag = 0;
		else
		  {
		  if (($1.flag == 'l') || ($3.flag == 'l'))
		    $$.flag = 'l';
		  else
		    $$.flag = 'i';
		  }
		}
	;

expr__logop__expr..logop__expr..
	: expression
	  logical_operator
	  expression
		{
		if ($2 == MPS_ORDFN)
		  $$.valu = $1.valu | $3.valu;

		if ($2 == MPS_ANDDFN)
		  $$.valu = $1.valu & $3.valu;

		if ($2 == MPS_XORDFN)
		  $$.valu = $1.valu ^ $3.valu;

		if (($1.flag == 0) || ($3.flag == 0))
		  $$.flag = 0;
		else
		  {
		  if (($1.flag == 'l') || ($3.flag == 'l'))
		    $$.flag = 'l';
		  else
		    $$.flag = 'i';
		  }
		}
	| expr__logop__expr..logop__expr..
	  logical_operator
	  expression
		{
		if ($2 == MPS_ORDFN)
		  $$.valu = $1.valu | $3.valu;

		if ($2 == MPS_ANDDFN)
		  $$.valu = $1.valu & $3.valu;

		if ($2 == MPS_XORDFN)
		  $$.valu = $1.valu ^ $3.valu;

		if (($1.flag == 0) || ($3.flag == 0))
		  $$.flag = 0;
		else
		  {
		  if (($1.flag == 'l') || ($3.flag == 'l'))
		    $$.flag = 'l';
		  else
		    $$.flag = 'i';
		  }
		}
	;

expr__shfop__expr
	: expression
	  shift_operator
	  expression
		{
		if ($2 == MPS_SHLDFN)
		  $$.valu = $1.valu << $3.valu;
		else
		  $$.valu = $1.valu >> $3.valu;

		if (($1.flag == 0) || ($3.flag == 0))
		  $$.flag = 0;
		else
		  {
		  if (($1.flag == 'l') || ($3.flag == 'l'))
		    $$.flag = 'l';
		  else
		    $$.flag = 'i';
		  }
		}
	;

shift_operator
	: DoubleLess
		{ $$ = MPS_SHLDFN; }
	| DoubleGreat
		{ $$ = MPS_SHRDFN; }
	;

mul_div_operator
	: Star
		{ $$ = MPS_MULDFN; }
	| Slash
		{ $$ = MPS_DIVDFN; }
	;

expression
	: LeftParen
	  immediate
	  RightParen
		{
		$$ = $2;
		}
	| Tilda
	  LeftParen
	  immediate
	  RightParen
		{
		$$.valu = ~ ($3.valu);
		$$.flag = $3.flag;
		}
	| Minus
	  LeftParen
	  immediate
	  RightParen
		{
		$$.valu = - $3.valu;
		$$.flag = $3.flag;
		}
	| term
		{ $$ = $1; }
	;

logical_operator
	: Bar
		{ $$ = MPS_ORDFN;  }
	| Ampersand
		{ $$ = MPS_ANDDFN; }
	| Circomflex
		{ $$ = MPS_XORDFN; }
	;

adding_operator
	: Plus
		{ $$ = MPS_PLSDFN; }
	| Minus
		{ $$ = MPS_MNSDFN; }
	;

term
	: Litteral
		{
		$$.valu = $1;
		$$.flag = 'i';
		}
	| Minus
	  Litteral
		{
		$$.valu = - $2;
		$$.flag = 'i';
		}
	| Tilda
	  Litteral
		{
		$$.valu = ~ $2;
		$$.flag = 'i';
		}
	| Identifier
		{
		unsigned long imd;

		imd     = beh_chktab (MPS_HSHTAB, $1, NULL, 7);
		$$.valu = imd;
		$$.flag = beh_chktab (MPS_HSHTAB, $1, NULL, 0);
		}
	| Minus
	  Identifier
		{
		unsigned long imd;

		imd     = beh_chktab (MPS_HSHTAB, $2, NULL, 7);
		$$.valu = -imd;
		$$.flag = beh_chktab (MPS_HSHTAB, $2, NULL, 0);
		}
	| Tilda
	  Identifier
		{
		unsigned long imd;

		imd     = beh_chktab (MPS_HSHTAB, $2, NULL, 7);
		$$.valu = ~imd;
		$$.flag = beh_chktab (MPS_HSHTAB, $2, NULL, 0);
		}
	;

destination_register
	: IntegerRegister
		{ $$ = $1; }
	| Register
		{ $$ = $1; }
	;

source_register
	: IntegerRegister
		{ $$ = $1; }
	| Register
		{ $$ = $1; }
	;

integer_register
	: IntegerRegister
		{ $$ = $1; }
	| Register
		{ $$ = $1; }
	;

cp0_reg
	: CP0Register
		{ $$ = $1; }
	| Register
		{ $$ = $1; }
	;

label
	: Identifier
		{ $$ = $1; }
	;

..CR..
	: /*empty*/
	| ..CR..
	  CarriageReturn_ERR
	;

CarriageReturn_ERR
	: CarriageReturn
		{ yyerrok; }
	;
%%

/* ###--------------------------------------------------------------### */
/* function	: mips_error						*/
/* description	: print an error message				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void mips_error (code, str)

int   code;
char *str;

  {
  MPS_ERRFLG = 1;
  if (code <100)
    fprintf (stderr,"Error %d near line %d : ",code, MPS_LINNUM);

  switch (code)
    {
    case 2:
      fprintf (stderr, "missing label\n");
      break;
    case 3:
      fprintf (stderr, "redefinition of label\n");
      break;
    case 4:
      fprintf (stderr, "undifined string\n");
      break;
    case 5:
      fprintf (stderr, "parse error\n");
      break;
    case 6:
      fprintf (stderr, "no output file has been specified\n");
      break;
    case 100:
      fprintf (stderr, "cannot open file `%s`\n",str);
      break;
    case 101:
      fprintf (stderr, "usage: mips_asm [<options>] <assembly_file> <vhdl_file> <vhdl_file>\n");
      break;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: yyerror						*/
/* description	: print yacc error messages				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void yyerror (str)

char *str;

  {
  MPS_ERRFLG = 1;
  fprintf (stderr, "line %d : %s\n",MPS_LINNUM, str);
  }

/* ###--------------------------------------------------------------### */
/* function	: mips_foot						*/
/* description	: print the head lines of the behavioural description	*/
/*		  of a ROM. The 'address size' defines the number of	*/
/*		  bits of the address, the 'word size' defines the size	*/
/*		  of the rom line in bytes and the 'name' specifies the	*/
/*		  name of the Entity.					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void mips_head (fp, name, adr_siz, wrd_siz)

FILE         *fp;
char         *name;
unsigned int  adr_siz;
unsigned int  wrd_siz;

  {
  adr_siz --;
  wrd_siz = (wrd_siz * 8) - 1;

  fprintf (fp, "entity %s is\n  port (\n", name);
  fprintf (fp, "    address : in  bit_vector (%u downto 0);\n", adr_siz);
  fprintf (fp, "    e_n     : in  bit;\n");
  fprintf (fp, "    data    : out mux_vector (%u downto 0) bus;\n", wrd_siz);
  fprintf (fp, "    vdd     : in bit;\n");
  fprintf (fp, "    vss     : in bit);\nend %s;\n\n",name);
  fprintf (fp, "architecture VBE of %s is\n\n", name);
  fprintf (fp, "  signal rom_out : bit_vector (%u downto 0);\n\n", wrd_siz);
  fprintf (fp, "begin\n\n");
  fprintf (fp, "  write_out : block (e_n = '0')\n  begin\n");
  fprintf (fp, "    data <= guarded rom_out;\n  end block;\n\n");
  fprintf (fp, "  with address (%u downto 0) select\n  rom_out <=\n", adr_siz);
  }

/* ###--------------------------------------------------------------### */
/* function	: mips_foot						*/
/* description	: print the foot lines of the behavioural description	*/
/*		  of a ROM. The 'word size' defines the size of the	*/
/*		  rom line in bytes.					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void mips_foot (fp, wrd_siz)

FILE         *fp     ;
unsigned int  wrd_siz;

  {
  unsigned int i;

  fprintf (fp, "          X\"");

  for (i=0 ; i<wrd_siz ; i++)
    fprintf (fp, "00");

  fprintf (fp, "\" when others;\n\nend;\n");
  }

/* ###--------------------------------------------------------------### */
/* function	: mips_print						*/
/* description	: print one or several lines of the rom content. the	*/
/*		  global variable 'word size' defines the size of the	*/
/*		  rom line in bytes.					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void mips_print (byte_nbr)

int byte_nbr;

  {
  unsigned int  i    ;
  unsigned int  j    ;
  unsigned long adr  ;
  unsigned long mask ;
  char          binadr [36];

  if (MPS_WRTFIL == NULL)
    {
    mips_error (6, NULL);
    exit (1);
    }
  else
    {
    if (byte_nbr != 0)
      {
      if (MPS_DUMP == 'y')
        {
        adr = MPS_ADDRES;
        for (i=0 ; i<byte_nbr ; i++)
          fprintf (MPS_WRTFIL, "%.8lx : %.2x\n", adr+i, MPS_BYTTAB [i]);
        }
      else
        {
        if (MPS_WRDSIZ == 4)
          adr = MPS_ADDRES >> 2;
        else
          adr = MPS_ADDRES;

        for (i=0 ; i<byte_nbr ; i += MPS_WRDSIZ)
          {
          fprintf (MPS_WRTFIL, "          X\"");

          for (j=0 ; j<MPS_WRDSIZ ; j++)
            fprintf (MPS_WRTFIL, "%.2x", MPS_BYTTAB [i+MPS_WRDSIZ-j-1]);

          mask = 0x00000001 << (MPS_ADRSIZ - 1);
          for (j=0 ; j<MPS_ADRSIZ ; j++)
            {
            if ((adr & mask) != 0)
              binadr [j] = '1';
            else
              binadr [j] = '0';
            mask = mask >> 1;
            }
          binadr [j] = '\0';

          fprintf (MPS_WRTFIL, "\" when B\"%s\",\n", binadr);
          adr++;
          }
        }
      }
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: mips_symbol						*/
/* description	: print out the location (in text or data region) of a	*/
/*		  symbol						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void mips_symbol (str)

char *str;
  {
  char *seg;

  if (MPS_ADRSPC == 'T')
    seg = "text";
  else
    seg = "data";

  fprintf (MPS_SYMFIL, "%s\t: location %lx in %s segment\n", str,
           MPS_ADDRES, seg);
  }

/* ###--------------------------------------------------------------### */
/* function	: mips_strtobin						*/
/* description	: make a binary  ? from a string			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static int mips_strtobin (str, tab)

char *str;
char *tab;

  {
  int  i  = 1;
  int  j  = 0;

  while (str [i] != '\0')
    {
    tab [j] = str [i];
    i++;
    if (str[i-1] == '\\')
      {
      if ((str [i] <= '7') && (str [i] >= '0'))
        {
        tab [j] = str [i] - '0';
        i++;
        if ((str [i] <= '7') && (str [i] >= '0'))
          {
          tab [j] = (tab [j] * 8) + (str [i] - '0');
          i++;
          if ((str [i] <= '7') && (str [i] >= '0'))
            tab [j] = (tab [j] * 8) + (str [i] - '0');
          }
        }
      else
        {
        if (str [i] == '"')
          {
          tab [j] = str [i];
          i++;
          }
        }
      }

    j++;
    }

  return (j-1);
  }
