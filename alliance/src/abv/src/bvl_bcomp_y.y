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
/* file		: bvl_bcomp.yac						*/
/* date		: Oct  4 1993						*/
/* version	: v108							*/
/* author	: P. BAZARGAN, L.A. TABUSSE, VUONG H.N.			*/
/* content	: yacc rules for behavioural VHDL			*/
/*									*/
/* ###--------------------------------------------------------------### */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "abe.h"

#include "bvl_utype.h"
#include "bvl_util.h"
#include "bvl_byacc.h"
#include "bvl_bedef.h"

%}

%union  {
	int		 valu;
	char		*text;
        bvl_ablstr       list;
	bvl_name	 name;
        struct g_type dble;
	};

%token         _AND
%token  <valu> PRAGMA
%token         _BEGIN
%token         _END
%token         _EQSym
%token         _EXIT
%token         _FILE
%token         _GESym
%token         _GTSym
%token         _IN
%token         _INOUT
%token         _LABEL
%token         _LESym
%token         _LINKAGE
%token         _LTSym
%token         _NAND
%token         _NESym
%token         _NEXT
%token         _NOR
%token         _NOT
%token         _NULL
%token         _OR
%token         _OUT
%token         _XOR
%token         ABS
%token         ACCESS
%token         AFTER
%token         ALIAS
%token         ALL
%token         ARCHITECTURE
%token         ARRAY
%token         ASSERT
%token         ATTRIBUTE
%token  <text> AbstractLit
%token         Ampersand
%token         Apostrophe
%token         Arrow
%token         BIT
%token         BIT_VECTOR
%token         _BLOCK
%token         BODY
%token         BUFFER
%token         BUS
%token         Bar
%token         BasedInt
%token         BasedReal
%token  <text> BitStringLit
%token         Box
%token         CASE
%token         COMPONENT
%token         CONFIGURATION
%token         CONSTANT
%token  <text> CharacterLit
%token         Colon
%token         Comma
%token         DISCONNECT
%token         DOWNTO
%token         DecimalInt
%token         DecimalReal
%token         Dot
%token         DoubleStar
%token         ELSE
%token         ELSIF
%token         ENTITY
%token         ERROR
%token         FOR
%token         FUNCTION
%token         GENERATE
%token         GENERIC
%token         GUARDED
%token         IF
%token         IS
%token  <text> Identifier
%token         LIBRARY
%token         LOOP
%token         LeftParen
%token         MAP
%token         MOD
%token         MUX_BIT
%token         MUX_VECTOR
%token         Minus
%token         NATURAL
%token         NATURAL_VECTOR
%token         NEW
%token         OF
%token         ON
%token         OPEN
%token         OTHERS
%token         _PACKAGE
%token         PORT
%token         PROCEDURE
%token         PROCESS
%token         Plus
%token         _PS
%token         _NS
%token         _US
%token         _MS
%token         RANGE
%token         RECORD
%token         REG_BIT
%token         REG_VECTOR
%token         REGISTER
%token         REM
%token         REPORT
%token         RETURN
%token         RightParen
%token         SELECT
%token         SEVERITY
%token         SIGNAL
%token         _STABLE
%token         SUBTYPE
%token         Semicolon
%token         Slash
%token         Star
%token  <text> StringLit
%token         THEN
%token         TO
%token         TRANSPORT
%token         _TYPE
%token         UNITS
%token         UNTIL
%token         USE
%token         VARIABLE
%token         VarAsgn
%token         WAIT
%token         WARNING
%token         WHEN
%token         WHILE
%token         WITH
%token         WOR_BIT
%token         WOR_VECTOR

%left         _AND _OR _NAND _NOR _XOR
%left         _EQSym _NESym
%left         _NOT

%type <text> choice
%type <text> .simple_name.
%type <text> simple_name
%type <text> a_label
%type <text> .label.
%type <text> label
%type <valu> severity__message
%type <text> report__message
%type <valu> .SEVERITY__expression.
%type <text> .REPORT__expression.
%type <text> .guard_expression.
%type <text> guard_expression

%type <list> ...waveform__WHEN__choices..
%type <list> waveform__WHEN__choices
%type <list> waveform_element
%type <list> waveform
%type <list> expression
%type <valu> .GUARDED.

%type <list> relation..AND__relation..
%type <list> relation..OR__relation..
%type <list> relation.NAND_NOR__relation.
%type <list> relation..XOR__relation..
%type <list> relation
%type <list> simple_expression
%type <list> .sign.term..add_op__term..
%type <list> term
%type <list> factor
%type <list> primary
%type <valu> relational_operator
%type <text> literal
%type <list> aggregate

%type <valu> .signal_kind.
%type <valu> .mode.
%type <dble> type_mark
%type <name> .constraint.
%type <name> constraint
%type <name> range
%type <valu> direction
%type <valu> abstractlit
%type <name> name
%type <name> slice_name
%type <name> indexed_name
%type <name> target
%type <name> attribute_name

%type <valu> generic_element_association

%type <text> constant_VarAsgn__expression

%start design_file

%%

design_file
	: /*empty*/
		{
		/* ###----------------------------------------------### */
		/*    Initializations					*/
		/*      - erroneous description presumed !		*/
		/* ###----------------------------------------------### */

		BVL_NM1LST = NULL;
		BVL_GRDLST = NULL;
		BVL_CNDLST = NULL;
		BVL_VALLST = NULL;
		BVL_ERRFLG = 0;

		dic                = beh_initab ();
		BVL_BEFPNT         = beh_addbefig (BVL_HEDFIG,NULL);
		BVL_BEFPNT->ERRFLG = 1;
		BVL_HEDFIG         = BVL_BEFPNT;
		}
	  entity_declaration
	  architecture_body
		{
		struct beout *beout_pnt;
		struct beaux *beaux_pnt;
		struct bebus *bebus_pnt;
		struct bereg *bereg_pnt;
		struct berin *berin_pnt;
		struct beaux *beaux_pred;
		struct berin *berin_pred;

		/* ###----------------------------------------------### */
		/*    Checking that each output have at least one driver*/
		/* ###----------------------------------------------### */

		beout_pnt = BVL_BEFPNT->BEOUT;
		while (beout_pnt != NULL)
		  {
		  if (beout_pnt->ABL == NULL)
		    bvl_error (40, beout_pnt->NAME);
		  beout_pnt = beout_pnt->NEXT;
		  }

		beaux_pnt = BVL_BEFPNT->BEAUX;
      beaux_pred=NULL;
		while (beaux_pnt != NULL)
		  {
        if (beaux_pnt->ABL == NULL)
        {
          fprintf (stderr,"`%s` Warning %d line %d :",BVL_CURFIL,40,BVL_LINNUM);
          fprintf (stderr,"signal `%s` never assigned\n",beaux_pnt->NAME);
          
          /*removing signal*/
          if ( beaux_pred ) beaux_pred->NEXT = beaux_pnt->NEXT;
          else BVL_BEFPNT->BEAUX = beaux_pnt->NEXT;

          /*removing other ref to signal*/
          berin_pred = NULL;
          for ( berin_pnt = BVL_BEFPNT->BERIN; berin_pnt; berin_pnt = berin_pnt->NEXT )
          {
             if ( berin_pnt->NAME == beaux_pnt->NAME )
             {
                 if ( berin_pred ) berin_pred->NEXT = berin_pnt->NEXT;
                 else BVL_BEFPNT->BERIN = berin_pnt->NEXT;
                 break;
             }
             berin_pred = berin_pnt;
          }
        }
        else beaux_pred = beaux_pnt;
		  beaux_pnt = beaux_pnt->NEXT;
		  }

		bebus_pnt = BVL_BEFPNT->BEBUS;
		while (bebus_pnt != NULL)
		  {
		  if (bebus_pnt->BIABL == NULL)
		    bvl_error (40, bebus_pnt->NAME);
		  bebus_pnt = bebus_pnt->NEXT;
		  }

		bereg_pnt = BVL_BEFPNT->BEREG;
		while (bereg_pnt != NULL)
		  {
		  if (bereg_pnt->BIABL == NULL)
		    bvl_error (40,bereg_pnt->NAME);
		  bereg_pnt = bereg_pnt->NEXT;
		  }

		if (BVL_ERRFLG == 0)
		  BVL_BEFPNT->ERRFLG = 0;

		beh_fretab (dic);
		}
	;

entity_declaration
	: ENTITY
	  simple_name
	  IS
		{
		BVL_BEFPNT->NAME = $2;
		BVL_MODNAM       = $2;
		}
	  .generic_clause.
	  .port_clause.
	  END_ERR
	  .simple_name.
	  Semicolon_ERR
		{
		if (($8 != NULL) && ($8 != $2))
		  bvl_error (1,$8);
		}
	| ENTITY
	  error
		{
		bvl_error (2,NULL);
		}
	;

.generic_clause.
	: /*empty*/
	| generic_clause
		{
		BVL_BEFPNT->BEGEN = BVL_GENPNT;
		BVL_GENPNT = NULL;
		}
	;

generic_clause
	: GENERIC
	  LeftParen
	  formal_generic_list
	  RightParen_ERR
	  Semicolon_ERR
	| GENERIC
	  error
	  Semicolon_ERR
		{ bvl_error (74,NULL); }
	;

formal_generic_list
	: formal_generic_element
	  ...formal_generic_element..
	;

...formal_generic_element..
	: /*empty*/
	| ...formal_generic_element..
	  Semicolon_ERR
	  formal_generic_element
	;

formal_generic_element
	: CONSTANT
	  identifier_list
	  Colon
	  type_mark
	  .constraint.
	  generic_VarAsgn__expression
		{
		char *type;

		if ($5.FLAG == $4.FLAG)
		  {
		  BVL_NM1LST = reverse (BVL_NM1LST);
		  type = namealloc ("natural");
		  BVL_GENPNT = bvl_addgen (BVL_GENPNT,BVL_INTLST,BVL_NM1LST,
		                           type,$5.LEFT,$5.RIGHT);
		  }
		else
		  bvl_error (33,NULL);
		freechain (BVL_NM1LST);
		freechain (BVL_INTLST);
		BVL_NM1LST = NULL;
		BVL_INTLST = NULL;
		}
	| error
		{
		/* ###----------------------------------------------### */
		/*   The following 3 lines reject tokens until the	*/
		/* sync. token 'Semicolon' is found			*/
		/* ###----------------------------------------------### */

		do
		  yychar = yylex ();
		while ((yychar != Semicolon) && (yychar != 0));
		yyerrok;
		bvl_error (75,NULL);
		}
	;

generic_VarAsgn__expression
	: VarAsgn
	  generic_expression
	;

generic_expression
	: abstractlit
		{
		long *ptlong;

		ptlong = (long *) mbkalloc (sizeof(long));
		*ptlong = $1;
		BVL_INTLST = addchain (BVL_INTLST,(void *)ptlong);
		}
	| generic_aggregate
	;

generic_aggregate
	: LeftParen
	  generic_element_association
		{
		long *ptlong;

		ptlong = (long *) mbkalloc (sizeof(long));
		*ptlong = $2;
		BVL_INTLST = addchain (BVL_INTLST,(void *)ptlong);
		}
	  ...generic_element_association..
	  RightParen_ERR
	;

...generic_element_association..
	: /* empty */
	| ...generic_element_association..
	  Comma
	  generic_element_association
		{
		long *ptlong;

		ptlong = (long *) mbkalloc (sizeof(long));
		*ptlong = $3;
		BVL_INTLST = addchain (BVL_INTLST,(void *)ptlong);
		}
	;

generic_element_association
	: abstractlit
		{$$ = $1;}
	;

.constraint.
	: /*empty*/
		{
		$$.FLAG  = 'S';
		$$.LEFT  = -1;
		$$.RIGHT = -1;
		}
	| constraint
		{ $$ = $1; }
	;

constraint
	: LeftParen
	  range
	  RightParen_ERR
		{ $$ = $2; }
	;

range
	: abstractlit
	  direction
	  abstractlit
		{
		$$.FLAG  = 'A';
		$$.LEFT  = $1;
		$$.RIGHT = $3;

		if ((($1 > $3) && ($2 == BVL_UPTDFN)) ||
		    (($1 < $3) && ($2 == BVL_DWTDFN)) ||
		    (($1 < 0 ) || ($2 < 0 )))
		  {
		  bvl_error (32,NULL);
		  }
		}
	;

direction
	: TO
		{ $$ = BVL_UPTDFN; }
	| DOWNTO
		{ $$ = BVL_DWTDFN; }
	;

.port_clause.
	: /*empty*/
	| port_clause
	;

port_clause
	: PORT
	  LeftParen
	  formal_port_list
	  RightParen_ERR
	  Semicolon_ERR
	| PORT
	  error
	  Semicolon_ERR
		{ bvl_error (3,NULL); }
	;

formal_port_list
	: formal_port_element
	  ...formal_port_element..
	;

...formal_port_element..
	: /*empty*/
	| ...formal_port_element..
	  Semicolon_ERR
	  formal_port_element
	;

formal_port_element
	: .SIGNAL.
	  identifier_list
	  Colon
	  .mode.
	  type_mark
	  .constraint.
	  .signal_kind.
		{
		char *signame;
		int   sigconf;
		void *pnt;

		/* ###----------------------------------------------### */
		/*    First, check the validity of the declaration.	*/
		/* Then, for each port, create the apropriate set of	*/
		/* structures (berin, bepor, beout, bebus)		*/
		/* ###----------------------------------------------### */

		chkdcl ('P', $4, $5.VALU, $5.FLAG, $7, $6.FLAG, &sigconf);

		BVL_NM1LST = reverse (BVL_NM1LST);
		while (BVL_NM1LST != NULL)
		  {
		  signame = (char *)BVL_NM1LST->DATA;
		  if (beh_chktab (dic,signame,BVL_MODNAM,BVL_SIGDFN) != 0)
		    bvl_error (4,signame);

		  pnt = addstr (BVL_BEFPNT,'P',$4,$5.VALU,$5.FLAG,signame,
		                $6.LEFT,$6.RIGHT);

		  beh_addtab (dic,signame,BVL_MODNAM,BVL_SIGDFN,sigconf);
		  beh_addtab (dic,signame,BVL_MODNAM,BVL_WMNDFN,$6.LEFT);
		  beh_addtab (dic,signame,BVL_MODNAM,BVL_WMXDFN,$6.RIGHT);
		  beh_addtab (dic,signame,BVL_MODNAM,BVL_PNTDFN,(int)pnt);

		  BVL_NM1LST = delchain (BVL_NM1LST, BVL_NM1LST);
		  }
		}
	| error
		{
		/* ###----------------------------------------------### */
		/*   The following 3 lines reject tokens until the	*/
		/* sync. token 'Semicolon' is found			*/
		/* ###----------------------------------------------### */

		do
		  yychar = yylex ();
		while ((yychar != Semicolon) && (yychar != 0));
		yyerrok;

		bvl_error (6,NULL);
		}
	;

.SIGNAL.
	: /*empty*/
	| SIGNAL
	;

.mode.
	: /*empty*/
		{ $$ = _IN; }
	| _IN
		{ $$ = _IN; }
	| _OUT
		{ $$ = _OUT; }
	| _INOUT
		{ $$ = _INOUT; }
	;

architecture_body
	: ARCHITECTURE
	  Identifier
	  OF
	  simple_name
	  IS
		{
		if ($4 != BVL_MODNAM)
		  bvl_error (1,$4);
		}
	  architecture_declarative_part
	  _BEGIN
	  architecture_statement_part
	  END_ERR
	  .simple_name.
	  Semicolon_ERR
		{
		if (($11 != NULL) && ($11 != $2))
		  bvl_error (7,$11);
		}
	| ARCHITECTURE
	  error
		{ bvl_error (8,NULL); }
	;

architecture_declarative_part
	: ..block_declarative_item..
	;

..block_declarative_item..
	: /*empty*/
	| ..block_declarative_item..
	  block_declarative_item
	;

block_declarative_item
	: signal_declaration
        | pragma_declaration
	| constant_declaration
	| error
	  Semicolon_ERR
		{ bvl_error (9,NULL); }
	;

pragma_declaration
        :  PRAGMA
           {
             char pragma[30];
             char type  [100];
             char name  [100];

             sscanf((char *)$1,"-- %s %s %s", pragma,type,name);

             BVL_BEFPNT->BEPGM = beh_addbepgm(BVL_BEFPNT->BEPGM,
                                              type,name,(void *)0);
           }
           ;

constant_declaration
	: CONSTANT
	  Identifier
	  Colon
	  type_mark
	  .constraint.
	  constant_VarAsgn__expression
	  Semicolon_ERR
		{
		int sigconf;
		
		if (chkdcl ('C',0,$4.VALU,$4.FLAG,0,$5.FLAG,&sigconf) == 0)
		  {
		  beh_addtab (dic,$2,BVL_MODNAM,BVL_WMNDFN,$5.LEFT);
		  beh_addtab (dic,$2,BVL_MODNAM,BVL_WMXDFN,$5.RIGHT);
		  beh_addtab (dic,$2,BVL_MODNAM,BVL_SIGDFN,sigconf);
		  beh_addtab (dic,$2,NULL,BVL_PNTDFN,$6);
		  }
		}
	;

constant_VarAsgn__expression
	: VarAsgn
	  literal
		{ $$ = $2; }
	;

signal_declaration
	: SIGNAL
	  identifier_list
	  Colon
	  type_mark
	  .constraint.
	  .signal_kind.
	  Semicolon_ERR
		{
		char *signame;
		int   sigconf;
		void *pnt;
		int   errflg;

		errflg = chkdcl ('S',0,$4.VALU,$4.FLAG,$6,$5.FLAG,&sigconf);

		/* ###----------------------------------------------### */
		/*    First, check the validity of the declaration.	*/
		/* Then, for each signal, create the apropriate set of	*/
		/* structures (berin, bereg, beaux, bebux)		*/
		/* ###----------------------------------------------### */

		BVL_NM1LST = reverse (BVL_NM1LST);
		while (BVL_NM1LST != NULL)
		  {
		  signame = (char *)BVL_NM1LST->DATA;
		  if (beh_chktab (dic,signame,BVL_MODNAM,BVL_SIGDFN) != 0)
		    bvl_error (10,signame);

		  pnt = addstr (BVL_BEFPNT,'S',0,$4.VALU,$4.FLAG,signame,
		                $5.LEFT,$5.RIGHT);

		  beh_addtab (dic,signame,BVL_MODNAM,BVL_SIGDFN,sigconf);
		  beh_addtab (dic,signame,BVL_MODNAM,BVL_WMNDFN,$5.LEFT);
		  beh_addtab (dic,signame,BVL_MODNAM,BVL_WMXDFN,$5.RIGHT);
		  beh_addtab (dic,signame,BVL_MODNAM,BVL_PNTDFN,(int)pnt);

		  BVL_NM1LST = delchain (BVL_NM1LST,BVL_NM1LST);
		  }
		}
	;

.signal_kind.
	: /*empty*/
		{ $$ = 0; }
	| REGISTER
		{ $$ = REGISTER; }
	| BUS
		{ $$ = BUS; }
	;

architecture_statement_part
	: ..concurrent_statement..
	;

..concurrent_statement..
	: /*empty*/
	| ..concurrent_statement..
	  concurrent_statement
	;

concurrent_statement
	: block_statement
	| concurrent_assertion_statement
	| concurrent_signal_assignment_statement
	| error
	  Semicolon_ERR
		{ bvl_error (18,NULL); }
	;

block_statement
	: a_label
	  _BLOCK
	  .guard_expression.
	  _BEGIN
		{
		if (beh_chktab (dic,$1,BVL_MODNAM,BVL_LBLDFN) != 0)
		  bvl_error (19,$1);

		beh_addtab (dic,$1,BVL_MODNAM,BVL_LBLDFN,1);
		if ($3 != NULL)
		  BVL_GRDLST = addchain (BVL_GRDLST,(char *)$3);
		}
	  set_of_statements
	  END_ERR
	  _BLOCK
	  .label.
	  Semicolon_ERR
		{
		if ($3 != NULL)
		  BVL_GRDLST = delchain (BVL_GRDLST,BVL_GRDLST);
		if (($9 != NULL) && ($9 != $1))
		  bvl_error (20,$9);
		}
	;

set_of_statements
	: ..concurrent_statement..
	;

concurrent_assertion_statement
	: a_label
	  unlabeled_concurrent_assertion_statement
		{
		if (beh_chktab (dic,$1,BVL_MODNAM,BVL_LBLDFN) != 0)
		  bvl_error (19,$1);

		beh_addtab (dic,$1,BVL_MODNAM,BVL_LBLDFN,1);
		if (BVL_ERRFLG == 0)
		  BVL_BEFPNT->BEMSG->LABEL = $1;
		}
	| unlabeled_concurrent_assertion_statement
	;

concurrent_signal_assignment_statement
	: a_label
	  unlabeled_conditional_signal_assignment
		{
		if (beh_chktab (dic,$1,BVL_MODNAM,BVL_LBLDFN) != 0)
		  bvl_error (19,$1);
		beh_addtab (dic,$1,BVL_MODNAM,BVL_LBLDFN,1);
		}
	| unlabeled_conditional_signal_assignment
	| a_label
	  unlabeled_selected_signal_assignment
		{
		if (beh_chktab (dic,$1,BVL_MODNAM,BVL_LBLDFN) != 0)
		  bvl_error (19,$1);
		beh_addtab (dic,$1,BVL_MODNAM,BVL_LBLDFN,1);
		}
	| unlabeled_selected_signal_assignment
	;

unlabeled_concurrent_assertion_statement
	: assertion_statement
	;

assertion_statement
	: ASSERT
	  expression
	  .REPORT__expression.
	  .SEVERITY__expression.
	  Semicolon_ERR
		{
		if (BVL_ERRFLG == 0)
		  {
		  BVL_BEFPNT->BEMSG = beh_addbemsg(BVL_BEFPNT->BEMSG,NULL,$4,
		                                   $3,$2.LIST_ABL->DATA,NULL);
		  }
		}
	;

unlabeled_conditional_signal_assignment
	: target
	  _LESym
	  .GUARDED.
	  ..waveform__WHEN__condition__ELSE..
	  waveform
	  Semicolon_ERR
		{
		int             i;
		struct beout   *beout_pnt;
		struct bebus   *bebus_pnt;
		struct bereg   *bereg_pnt;
		struct beaux   *beaux_pnt;
		struct bebux   *bebux_pnt;
		struct chain   *abl_pnt;
		struct bvl_expr expr0;
		struct bvl_expr expr1;
		struct bvl_expr expr2;
		struct bvl_expr expr3;
		struct bvl_expr expr4;
		struct bvl_expr expr5;
		struct bvl_expr expr6;
		int             rev_flg = 0;
		int             left_bnd;
		int             right_bnd;
		int             left;
		int             right;
		int             in_bound;
		int             out_bound;
		int             sig_width;
		int             sig_conf;

		expr4 = $5;

		while (BVL_CNDLST != NULL)
		  {
		  expr5 = *((bvl_ablstr *) BVL_CNDLST->DATA);
		  expr6 = *((bvl_ablstr *) BVL_VALLST->DATA);

		  expr0 = bvl_cpyablstr (expr5);
		  expr1 = bvl_crtabl (ABL_NOT , expr5, BVL_EMPSTR,-1,-1);
		  expr5 = expr0;
		  expr2 = bvl_crtabl (ANDM, expr4, expr1     ,-1,-1);
		  expr3 = bvl_crtabl (ANDM, expr6, expr5     ,-1,-1);
		  expr4 = bvl_crtabl (ABL_OR  , expr2, expr3     ,-1,-1);

		  BVL_CNDLST = delchain (BVL_CNDLST,BVL_CNDLST);
		  BVL_VALLST = delchain (BVL_VALLST,BVL_VALLST);
		  }

		left_bnd  = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_WMNDFN);
		right_bnd = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_WMXDFN);
		sig_conf  = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_SIGDFN);

		left  = $1.LEFT;
		right = $1.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  sig_width = right_bnd - left_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 0;
		    in_bound  = right_bnd - right;
		    out_bound = right_bnd - left;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  else
		    {
		    rev_flg   = 1;
		    in_bound  = right_bnd - left;
		    out_bound = right_bnd - right;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  }
		else
		  {
		  sig_width = left_bnd - right_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 1;
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  else
		    {
		    rev_flg   = 0;
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  }

		if ((out_bound - in_bound + 1) != expr4.WIDTH)
		  bvl_error (35,$1.NAME);

		if (rev_flg == 1)
		  expr4.LIST_ABL = reverse (expr4.LIST_ABL);

		abl_pnt = expr4.LIST_ABL;

		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		    bvl_error (21,$1.NAME);
		    break;

		  case (BVL_OCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		    if ($3 == BVL_GRDDFN)
		      bvl_error (22,$1.NAME);
		    beout_pnt = (struct beout *)
		                 beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beout_pnt->ABL != NULL)
		            bvl_error (39,beout_pnt->NAME);
		          else
		            beout_pnt->ABL = (struct chain *)abl_pnt->DATA;
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beout_pnt = beout_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_OCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_OCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		    if (($3 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23,$1.NAME);
		    bebus_pnt = (struct bebus *)
		                 beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebus_pnt->BINODE = beh_addbinode (bebus_pnt->BINODE,
		                                             NULL,NULL);
		          bebus_pnt->BIABL  = beh_addbiabl  (bebus_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             dupablexpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebus_pnt = bebus_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_BITDFN + BVL_NORDFN):
		    if ($3 == BVL_GRDDFN)
		      bvl_error (22,$1.NAME);
		    beaux_pnt = (struct beaux *)
		                beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beaux_pnt->ABL != NULL)
		            bvl_error (39,beaux_pnt->NAME);
		          else
		            beaux_pnt->ABL = (struct chain *)abl_pnt->DATA;
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beaux_pnt = beaux_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_RBIDFN + BVL_REGDFN):
		    if (($3 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23,$1.NAME);
		    bereg_pnt = (struct bereg *)
		                beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bereg_pnt->BINODE = beh_addbinode (bereg_pnt->BINODE,
		                                             NULL,NULL);
		          bereg_pnt->BIABL  = beh_addbiabl  (bereg_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             dupablexpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bereg_pnt = bereg_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_WORDFN + BVL_BUSDFN) :
		    if (($3 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23,$1.NAME);
		    bebux_pnt = (struct bebux *)
		                beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebux_pnt->BINODE = beh_addbinode (bebux_pnt->BINODE,
		                                             NULL,NULL);
		          bebux_pnt->BIABL  = beh_addbiabl  (bebux_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             dupablexpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebux_pnt = bebux_pnt->NEXT;
		        }
		      }
		    break;

		  default :
		    bvl_error (17,$1.NAME);
		    break;
		  }
		}
	;

..waveform__WHEN__condition__ELSE..
	: /*empty*/
	| ..waveform__WHEN__condition__ELSE..
	  waveform
	  WHEN
	  expression
	  ELSE
		{
		struct bvl_expr *expr_pnt;

		expr_pnt           = (bvl_ablstr *)mbkalloc(sizeof(bvl_ablstr));
		expr_pnt->WIDTH    = $4.WIDTH;
		expr_pnt->LIST_ABL = $4.LIST_ABL;
		BVL_CNDLST         = addchain (BVL_CNDLST,(char *) expr_pnt);

		expr_pnt           = (bvl_ablstr *)mbkalloc(sizeof(bvl_ablstr));
		expr_pnt->WIDTH    = $2.WIDTH;
		expr_pnt->LIST_ABL = $2.LIST_ABL;
		BVL_VALLST         = addchain (BVL_VALLST,(char *) expr_pnt);
		}
	;

unlabeled_selected_signal_assignment
	: WITH
	  expression
		{
		BVL_SLCEXP = $2;
		BVL_BDDPNT = NULL;
		}
	  SELECT
	  target
	  _LESym
	  .GUARDED.
	  waveform__WHEN__choices
	  ...waveform__WHEN__choices..
	  Semicolon_ERR
		{
		int             i;
		struct beout   *beout_pnt;
		struct bebus   *bebus_pnt;
		struct bereg   *bereg_pnt;
		struct beaux   *beaux_pnt;
		struct bebux   *bebux_pnt;
		struct chain   *abl_pnt;
		struct bvl_expr expr1;
		int             rev_flg = 0;
		int             left_bnd;
		int             right_bnd;
		int             left;
		int             right;
		int             in_bound;
		int             out_bound;
		int             sig_width;
		int             sig_conf;

		expr1 = bvl_crtabl (ABL_OR ,$8 ,$9,-1,-1);
		if (BVL_BDDPNT != getlogbddnodeone())
		  bvl_error (25,NULL);
		left_bnd  = beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_WMNDFN);
		right_bnd = beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_WMXDFN);
		sig_conf  = beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_SIGDFN);

		left  = $5.LEFT;
		right = $5.RIGHT;
		if (left_bnd <= right_bnd)
		  {
		  sig_width = right_bnd - left_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 0;
		    in_bound  = right_bnd - right;
		    out_bound = right_bnd - left;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36,$5.NAME);
		    }
		  else
		    {
		    rev_flg   = 1;
		    in_bound  = right_bnd - left;
		    out_bound = right_bnd - right;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36,$5.NAME);
		    }
		  }
		else
		  {
		  sig_width = left_bnd - right_bnd + 1;
		  if (left <= right)
		    {
		    rev_flg   = 1;
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36,$5.NAME);
		    }
		  else
		    {
		    rev_flg   = 0;
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36,$5.NAME);
		    }
		  }

		if ((out_bound - in_bound + 1) != expr1.WIDTH)
		  bvl_error (35,$5.NAME);

		if (rev_flg == 1)
		  expr1.LIST_ABL = reverse (expr1.LIST_ABL);

		abl_pnt = expr1.LIST_ABL;

		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		    bvl_error (21,$5.NAME);
		    break;

		  case (BVL_OCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		    if ($7 == BVL_GRDDFN)
		      bvl_error (22,$5.NAME);
		    beout_pnt = (struct beout *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beout_pnt->ABL != NULL)
		            bvl_error (39,beout_pnt->NAME);
		          else
		            beout_pnt->ABL = (struct chain *)abl_pnt->DATA;
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beout_pnt = beout_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_OCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_OCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		    if (($7 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23,$5.NAME);
		    bebus_pnt = (struct bebus *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebus_pnt->BINODE = beh_addbinode (bebus_pnt->BINODE,
		                                             NULL,NULL);
		          bebus_pnt->BIABL  = beh_addbiabl  (bebus_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             dupablexpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebus_pnt = bebus_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_BITDFN + BVL_NORDFN):
		    if ($7 == BVL_GRDDFN)
		      bvl_error (22,$5.NAME);
		    beaux_pnt = (struct beaux *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          if (beaux_pnt->ABL != NULL)
		            bvl_error (39,beaux_pnt->NAME);
		          else
		           {
		            beaux_pnt->ABL = (struct chain *)abl_pnt->DATA;
		            }
		          abl_pnt = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        beaux_pnt = beaux_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_RBIDFN + BVL_REGDFN):
		    if (($7 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23,$5.NAME);
		    bereg_pnt = (struct bereg *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bereg_pnt->BINODE = beh_addbinode (bereg_pnt->BINODE,
		                                             NULL,NULL);
		          bereg_pnt->BIABL  = beh_addbiabl  (bereg_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             dupablexpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bereg_pnt = bereg_pnt->NEXT;
		        }
		      }
		    break;

		  case (BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_WORDFN + BVL_BUSDFN) :
		    if (($7 != BVL_GRDDFN) || (BVL_GRDLST == NULL))
		      bvl_error (23,$5.NAME);
		    bebux_pnt = (struct bebux *)
		                beh_chktab (dic,$5.NAME,BVL_MODNAM,BVL_PNTDFN);
		    if (BVL_ERRFLG == 0)
		      {
		      for (i=0 ; i<sig_width ; i++)
		        {
		        if (i >= in_bound)
		          {
		          bebux_pnt->BINODE = beh_addbinode (bebux_pnt->BINODE,
		                                             NULL,NULL);
		          bebux_pnt->BIABL  = beh_addbiabl  (bebux_pnt->BIABL,
		                                             BVL_LBLNAM,
		                                             dupablexpr(BVL_GRDLST->DATA) ,
		                                             abl_pnt->DATA);
		          abl_pnt           = abl_pnt->NEXT;
		          }
		        if (i >= out_bound)
		          break;
		        bebux_pnt = bebux_pnt->NEXT;
		        }
		      }
		    break;

		  default :
		    bvl_error (17,$5.NAME);
		    break;
		  }
		}
	;

...waveform__WHEN__choices..
	: /*empty*/
		{ $$ = BVL_EMPSTR; }
	| ...waveform__WHEN__choices..
	  Comma
	  waveform__WHEN__choices
		{ $$ = bvl_crtabl (ABL_OR,$1,$3,-1,-1); }
	;

waveform__WHEN__choices
	: waveform
	  WHEN
	  choices
		{
		struct bvl_expr expr1;

		bvl_select (&expr1,BVL_NM1LST,&BVL_BDDPNT,BVL_SLCEXP);
		freechain (BVL_NM1LST);
		BVL_NM1LST = NULL;

		$$ = bvl_crtabl (ANDM,$1,expr1,-1,-1);
		}
	;

waveform
	: waveform_element
		{ $$ = $1; }
	;

waveform_element
	: expression
	  .AFTER__delay_expression.
		{
		$$      = $1;
		}
	;

.AFTER__delay_expression.
	: /*empty*/
		{}
	| AFTER
	  delay_expression
		{}
	;

delay_expression
	: abstractlit
	 .time_unit.
		{}
	;

.time_unit.
	: /*empty*/
		{}
	| _PS
		{}
	| _NS
		{}
	| _US
		{}
	| _MS
		{}
	;

choices
	: choice
		{ BVL_NM1LST = addchain (NULL ,$1); }
	  ..Bar__choice..
		{ yyerrok; }
	;

..Bar__choice..
	: /*empty*/
	| ..Bar__choice..
	  Bar
	  choice
		{
		if ($3 == "others")
		  bvl_error (30,NULL);
		BVL_NM1LST = addchain (BVL_NM1LST ,$3);
		}
	;

choice
	: literal
		{ $$ = $1; }
	| OTHERS
		{ $$ = "others"; }
	| name
		{
		char           *val;
		char            val2[256];
		int             left;
		int             right;
		int             in_bound;
		int             out_bound;
		int             left_bnd;
		int             right_bnd;
		int             sig_conf;

		strcpy (val2,"B\"");
		sig_conf = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_SIGDFN);
		val      = (char *) beh_chktab (dic,$1.NAME,NULL,BVL_PNTDFN);
		if (sig_conf == 0)
		  bvl_error (17,$1.NAME);
		else
		  {
		  if (sig_conf != BVL_CSTDFN)
		    bvl_error (76,$1.NAME);
		  }

		left_bnd  = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_WMNDFN);
		right_bnd = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_WMXDFN);

		left  = $1.LEFT;
		right = $1.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  if (left <= right)
		    {
		    in_bound = left - left_bnd;
		    out_bound = right - left_bnd;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  else
		    {
		    in_bound = left - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  }
		else
		  {
		  if (left <= right)
		    {
		    in_bound = right - left_bnd;
		    out_bound = left - left_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  else
		    {
		    in_bound = right - right_bnd;
		    out_bound = left - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  }

		bvl_tobin (&val2[2],val,in_bound,out_bound);
		strcat (val2,"\"");
		$$ = namealloc (val2);
		}
	;

.REPORT__expression.
	: /*empty*/
		{ $$ = NULL; }
	| REPORT
	  report__message
		{ $$ = $2; }
	;

.SEVERITY__expression.
	: /*empty*/
		{ $$ = 'E'; }
	| SEVERITY
	  severity__message
		{ $$ = $2; }
	;

report__message
	: StringLit
		{ $$ = $1; }
	;

severity__message
	: ERROR
		{ $$ = 'E'; }
	| WARNING
		{ $$ = 'W'; }
	;

expression
	: relation..AND__relation..
		{ $$ = $1; }
	| relation..OR__relation..
		{ $$ = $1; }
	| relation.NAND_NOR__relation.
		{ $$ = $1; }
	| relation..XOR__relation..
		{ $$ = $1; }
	;

relation..AND__relation..
	: relation
	  _AND
	  relation
		{ $$ = bvl_crtabl (ABL_AND ,$1 ,$3 ,-1,-1); }
	| relation..AND__relation..
	  _AND
	  relation
		{ $$ = bvl_crtabl (ABL_AND ,$1 ,$3 ,-1,-1); }
	;

relation..OR__relation..
	: relation
	  _OR
	  relation
		{ $$ = bvl_crtabl (ABL_OR ,$1 ,$3 ,-1,-1); }
	| relation..OR__relation..
	  _OR
	  relation
		{ $$ = bvl_crtabl (ABL_OR ,$1 ,$3 ,-1,-1); }
	;

relation.NAND_NOR__relation.
	: relation
		{ $$ = $1; }
	| relation
	  _NAND
	  relation
		{ $$ = bvl_crtabl (ABL_NAND ,$1 ,$3 ,-1,-1); }
	| relation
	  _NOR
	  relation
		{ $$ = bvl_crtabl (ABL_NOR ,$1 ,$3 ,-1,-1); }
	;


relation..XOR__relation..
	: relation
	  _XOR
	  relation
		{ $$ = bvl_crtabl (ABL_XOR ,$1 ,$3 ,-1,-1); }
	| relation..XOR__relation..
	  _XOR
	  relation
		{ $$ = bvl_crtabl (ABL_XOR ,$1 ,$3 ,-1,-1); }
	;

relation
	: simple_expression
		{ $$ = $1; }
	| simple_expression
	  relational_operator
	  simple_expression
		{ $$ = bvl_crtabl ($2 ,$1 ,$3 ,-1,-1); }
	;

simple_expression
	: .sign.term..add_op__term..
		{ $$ = $1; }
	;

.sign.term..add_op__term..
	: term
		{ $$ = $1; }
	| .sign.term..add_op__term..
	  Ampersand
	  term
		{ $$ = bvl_crtabl (CONC,$1,$3,-1,-1); }

	;

term
	: factor
		{ $$ = $1; }
	;

factor
	: primary
		{ $$ = $1; }
	| _NOT
	  primary
		{ $$ = bvl_crtabl (ABL_NOT,$2,BVL_EMPSTR,-1,-1); }
	;

primary
	: literal
		{
		struct bvl_expr expr1;
		expr1.IDENT = $1;
		$$ = bvl_crtabl (NOPS,expr1,BVL_EMPSTR,-1,-1);
		}
	| aggregate
		{ $$ = $1; }
	| name
		{
		struct bvl_expr expr1;
		int             left;
		int             right;
		int             left_bnd;
		int             right_bnd;
		int             in_bound;
		int             out_bound;
		int             sig_conf;

		sig_conf = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_SIGDFN);
		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BITDFN + BVL_NORDFN):
		  case (BVL_MUXDFN + BVL_BUSDFN):
		  case (BVL_WORDFN + BVL_BUSDFN):
		  case (BVL_RBIDFN + BVL_REGDFN):
		  case (BVL_CSTDFN):
		    break;
		  case (BVL_OCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_OCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_OCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		    bvl_error (26,$1.NAME);
		    break;
		  default :
		    bvl_error (17,$1.NAME);
		    break;
		  }

		left_bnd  = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_WMNDFN);
		right_bnd = beh_chktab (dic,$1.NAME,BVL_MODNAM,BVL_WMXDFN);

		left  = $1.LEFT;
		right = $1.RIGHT;

		if (left_bnd <= right_bnd)
		  {
		  if (left <= right)
		    {
		    in_bound  = left  - left_bnd;
		    out_bound = right - left_bnd;
		    if ((left < left_bnd) || (right > right_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  else
		    {
		    in_bound  = right - left_bnd;
		    out_bound = left  - left_bnd;
		    if ((left > right_bnd) || (right < left_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  }
		else
		  {
		  if (left <= right)
		    {
		    in_bound  = left  - right_bnd;
		    out_bound = right - right_bnd;
		    if ((left < right_bnd) || (right > left_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  else
		    {
		    in_bound  = right - right_bnd;
		    out_bound = left  - right_bnd;
		    if ((left > left_bnd) || (right < right_bnd))
		      bvl_error (36,$1.NAME);
		    }
		  }

		if (sig_conf != BVL_CSTDFN)
		  {
		  expr1.IDENT = $1.NAME;
		  if($1.FLAG == 'X')
			{
		  	expr1 = bvl_crtabl (NOPI,expr1,BVL_EMPSTR,left,right);
		  	$$ = bvl_crtabl (ABL_STABLE,expr1,BVL_EMPSTR,left,right);
			}
		  else 
		  	$$ = bvl_crtabl (NOPI,expr1,BVL_EMPSTR,left,right);
		  }
		else
		  {
		  expr1.IDENT = (char *)beh_chktab(dic,$1.NAME,NULL,BVL_PNTDFN);
		  $$ = bvl_crtabl (NOPS,expr1,BVL_EMPSTR,in_bound,out_bound);
		  }
		}
	;

relational_operator
	: _EQSym
		{ $$ = EQ; }
	| _NESym
		{ $$ = NE; }
	;

literal
	: CharacterLit
		{ $$ = $1; }
	| StringLit
		{ $$ = $1; }
	| BitStringLit
		{ $$ = $1; }
	;

aggregate
	: LeftParen
	  expression
	  RightParen
		{ $$ = $2; }
	;

name
	: simple_name
		{
		$$.NAME  = $1;
		$$.LEFT  = beh_chktab (dic,$1,BVL_MODNAM,BVL_WMNDFN);
		$$.RIGHT = beh_chktab (dic,$1,BVL_MODNAM,BVL_WMXDFN);
		}
	| indexed_name
		{ $$ = $1; }
	| slice_name
		{ $$ = $1; }
	| attribute_name
		{ $$ = $1; }
	;

indexed_name
	: simple_name
	  LeftParen
	  abstractlit
	  RightParen_ERR
		{
		$$.NAME  = $1;
		$$.LEFT  = $3;
		$$.RIGHT = $3;
		}
	;

slice_name
	: simple_name
	  LeftParen
	  abstractlit
	  direction
	  abstractlit
	  RightParen_ERR
		{
		if ((($5 > $3) && ($4 != BVL_UPTDFN)) ||
		    (($5 < $3) && ($4 != BVL_DWTDFN)))
		  bvl_error (32,$1);

		$$.NAME  = $1;
		$$.LEFT  = $3;
		$$.RIGHT = $5;
		}
	;

attribute_name
	: simple_name
	  Apostrophe
	  attribute_designator
		{
		char             extname[100];
		char            *lclname;
		int              sig_conf;
		struct bvl_expr  expr1;
		struct bvl_expr  expr2;
		struct chain    *ptabl;

		sig_conf = beh_chktab (dic,$1,BVL_MODNAM,BVL_SIGDFN);
		switch (sig_conf)
		  {
		  case (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_BITDFN + BVL_NORDFN) :
		  case (BVL_BCNDFN + BVL_MUXDFN + BVL_BUSDFN) :
		  case (BVL_BCNDFN + BVL_WORDFN + BVL_BUSDFN) :
		  case (BVL_BITDFN + BVL_NORDFN):
		  case (BVL_MUXDFN + BVL_BUSDFN):
		  case (BVL_WORDFN + BVL_BUSDFN):
		  case (BVL_RBIDFN + BVL_REGDFN):
		    break;
		  default :
		    bvl_error (79,$1);
		  }

		if (beh_chktab (dic,$1,BVL_MODNAM,BVL_WMNDFN) != -1)
		  bvl_error (79,$1);

		sprintf (extname,"%s'delayed",$1);
		lclname = namealloc (extname);

		if (BVL_ERRFLG == 0)
		  {
		  if (beh_chktab (dic,$1,BVL_MODNAM,BVL_STBDFN) == 0)
		    {
		    expr1.IDENT = $1;
		    expr1.WIDTH  = 1;
		    expr2 = bvl_crtabl (NOPI,expr1,BVL_EMPSTR,-1,-1);
		    ptabl = expr2.LIST_ABL->DATA;

		    if (BVL_AUXMOD == 1)
		      BVL_BEFPNT->BERIN=beh_addberin(BVL_BEFPNT->BERIN,lclname);

		    BVL_BEFPNT->BEDLY = beh_addbeaux(BVL_BEFPNT->BEDLY,lclname,
		                                     ptabl,NULL);
		    beh_addtab (dic,$1     ,BVL_MODNAM,BVL_STBDFN,1);
		    beh_addtab (dic,lclname,BVL_MODNAM,BVL_WMNDFN,-1);
		    beh_addtab (dic,lclname,BVL_MODNAM,BVL_WMXDFN,-1);
		    beh_addtab (dic,lclname,BVL_MODNAM,BVL_SIGDFN,
		                (BVL_ICNDFN + BVL_BITDFN + BVL_NORDFN));
		    }
		  }
		BVL_BEFPNT->TYPE |= BEH_STABLE;
		$$.NAME = $1;
		$$.LEFT = -1;
		$$.RIGHT = -1;
		$$.FLAG  = 'X';

		}
	;

attribute_designator
	: _STABLE
	;

type_mark
	: BIT
		{ $$.VALU = BIT; $$.FLAG = 'S'; }
	| WOR_BIT
		{ $$.VALU = WOR_BIT; $$.FLAG = 'S'; }
	| MUX_BIT
		{ $$.VALU = MUX_BIT; $$.FLAG = 'S'; }
	| BIT_VECTOR
		{ $$.VALU = BIT; $$.FLAG = 'A'; }
	| WOR_VECTOR
		{ $$.VALU = WOR_BIT; $$.FLAG = 'A'; }
	| MUX_VECTOR
		{ $$.VALU = MUX_BIT; $$.FLAG = 'A'; }
	| REG_BIT
		{ $$.VALU = REG_BIT; $$.FLAG = 'S'; }
	| REG_VECTOR
		{ $$.VALU = REG_BIT; $$.FLAG = 'A'; }
	| NATURAL
		{ $$.VALU = NATURAL; $$.FLAG = 'S'; }
	| NATURAL_VECTOR
		{ $$.VALU = NATURAL; $$.FLAG = 'A'; }
	;

identifier_list
	: Identifier
		{ BVL_NM1LST = addchain (BVL_NM1LST,$1); }
	  ...identifier..
	;

...identifier..
	: /*empty*/
	| ...identifier..
	  Comma
	  Identifier
		{ BVL_NM1LST = addchain (BVL_NM1LST,$3); }
	;

.label.
	: /*empty*/
		{ $$ = NULL; }
	| label
		{ $$ = $1; }
	;

.guard_expression.
	: /*empty*/
		{ $$ = NULL; }
	| guard_expression
		{ $$ = $1; }
	;

guard_expression
	: LeftParen
	  expression
	  RightParen_ERR
		{ $$ = (char *)$2.LIST_ABL->DATA; }
	;

.GUARDED.
	: /*empty*/
		{ $$ = BVL_UNGDFN ; }
	| GUARDED
		{ $$ = BVL_GRDDFN; }
	;

.simple_name.
	: /*empty*/
		{ $$ = NULL; }
	| simple_name
		{ $$ = $1; }
	;

simple_name
	: Identifier
		{ $$ = $1; }
	;

target
	: name
		{ $$ = $1; }
	;

a_label
	: label
	  Colon
		{
		BVL_LBLNAM = $1;
		$$ = $1;
		}
	;

label
	: Identifier
		{ $$ = $1; }
	;

abstractlit
	: AbstractLit
		{ $$ = atoi ($1); }
	;

RightParen_ERR
	: RightParen
		{ yyerrok; }
	;

Semicolon_ERR
	: Semicolon
		{ yyerrok; }
	;

END_ERR
	: _END
		{ yyerrok; }
	;
%%

void *addstr (ptfig, object, mode, type, flag, name, left, right)

struct befig *ptfig;
char          object;
int           mode;
int           type;
char          flag;
char         *name;
short         left;
short         right;

  {
  void  *pnt    = NULL;
  char   porflg = 0;
  char   rinflg = 0;
  char   outflg = 0;
  char   busflg = 0;
  char   auxflg = 0;
  char   buxflg = 0;
  char   regflg = 0;
  char   lclmod;
  char   lcltyp;
  char   extname[100];
  short  i;
  short  inc = 1;

  switch (object)
    {
    case 'P':

	/* ###------------------------------------------------------### */
	/*   if object is a port ...					*/
	/* ###------------------------------------------------------### */

      porflg = 1;
      switch (mode)
        {
        case _IN:
          lclmod = 'I'; lcltyp = 'B'; rinflg = 1; break;
        case _OUT:
          switch (type)
            {
            case BIT:
              lclmod = 'O'; lcltyp = 'B'; outflg = 1; break;
            case MUX_BIT:
              lclmod = 'Z'; lcltyp = 'M'; busflg = 1; break;
            case WOR_BIT:
              lclmod = 'Z'; lcltyp = 'W'; busflg = 1; break;
            }
          break;
        case _INOUT:
          rinflg = 1;
          switch (type)
            {
            case BIT:
              lclmod = 'B'; lcltyp = 'B'; outflg = 1; break;
            case MUX_BIT:
              lclmod = 'T'; lcltyp = 'M'; busflg = 1; break;
            case WOR_BIT:
              lclmod = 'T'; lcltyp = 'W'; busflg = 1; break;
            }
          break;
        }
      break;

    case 'S':

	/* ###------------------------------------------------------### */
	/*   if object is a signal ...					*/
	/* ###------------------------------------------------------### */

      switch (type)
        {
        case BIT:
          lcltyp = 'B'; rinflg = BVL_AUXMOD; auxflg = 1; break;
        case REG_BIT:
          rinflg = 1; regflg = 1; break;
        case MUX_BIT:
          lcltyp = 'M'; rinflg = 1; buxflg = 1; break;
        case WOR_BIT:
          lcltyp = 'W'; rinflg = 1; buxflg = 1; break;
        }
      break;
    }

  if (flag == 'S')
    {
	/* ###------------------------------------------------------### */
	/*   if object is a scalar ...					*/
	/* ###------------------------------------------------------### */

    if (porflg == 1)
      ptfig->BEPOR = beh_addbepor (ptfig->BEPOR,name,lclmod,lcltyp);
    if (rinflg == 1)
      ptfig->BERIN = beh_addberin (ptfig->BERIN,name);
    if (outflg == 1)
      ptfig->BEOUT = beh_addbeout (ptfig->BEOUT,name,NULL,NULL);
    if (busflg == 1)
      ptfig->BEBUS = beh_addbebus (ptfig->BEBUS,name,NULL,NULL,lcltyp);
    if (auxflg == 1)
      ptfig->BEAUX = beh_addbeaux (ptfig->BEAUX,name,NULL,NULL);
    if (buxflg == 1)
      ptfig->BEBUX = beh_addbebux (ptfig->BEBUX,name,NULL,NULL,lcltyp);
    if (regflg == 1)
      ptfig->BEREG = beh_addbereg (ptfig->BEREG,name,NULL,NULL);

    }
  else
    {
	/* ###------------------------------------------------------### */
	/*   if object is an array ...					*/
	/* ###------------------------------------------------------### */

    if (left >= right)
      inc = -1;

    for (i=left ; i!=(right+inc) ; i+=inc)
      {
      sprintf (extname,"%s %d",name,i);

      if (porflg == 1)
        ptfig->BEPOR = beh_addbepor (ptfig->BEPOR,extname,lclmod,lcltyp);
      if (rinflg == 1)
        ptfig->BERIN = beh_addberin (ptfig->BERIN,extname);
      if (outflg == 1)
        ptfig->BEOUT = beh_addbeout (ptfig->BEOUT,extname,NULL,NULL);
      if (busflg == 1)
        ptfig->BEBUS = beh_addbebus (ptfig->BEBUS,extname,NULL,NULL,lcltyp);
      if (auxflg == 1)
        ptfig->BEAUX = beh_addbeaux (ptfig->BEAUX,extname,NULL,NULL);
      if (buxflg == 1)
        ptfig->BEBUX = beh_addbebux (ptfig->BEBUX,extname,NULL,NULL,lcltyp);
      if (regflg == 1)
        ptfig->BEREG = beh_addbereg (ptfig->BEREG,extname,NULL,NULL);
      }
    }

  if (outflg == 1)
    pnt = (void *) ptfig->BEOUT;
  if (busflg == 1)
    pnt = (void *) ptfig->BEBUS;
  if (auxflg == 1)
    pnt = (void *) ptfig->BEAUX;
  if (buxflg == 1)
    pnt = (void *) ptfig->BEBUX;
  if (regflg == 1)
    pnt = (void *) ptfig->BEREG;

  return (pnt);
  }

int chkdcl (object,mode,type,flag,kind,constraint,conf)

char object;
int  mode;
int  type;
char flag;
int  kind;
char constraint;
int  *conf;

  {
  int errflg = 0;
  int lclcnf = 0;

  if (flag != constraint)
    {
    errflg = 1;
    bvl_error (33,NULL);
    }
  else
    {
    switch (object)
      {
      case 'P':
	/* ###------------------------------------------------------### */
	/*    If object is a port :					*/
	/*       - if type is bit, no guard indication can be used	*/
	/*       - if type is wor_bit or mux_bit, bus kind must be used	*/
	/*       - other types are illegal				*/
	/* ###------------------------------------------------------### */

        switch (type)
          {
          case BIT:

            lclcnf += BVL_BITDFN + BVL_NORDFN;
            switch (mode)
              {
              case _IN :
                lclcnf += BVL_ICNDFN; break;
              case _OUT:
                lclcnf += BVL_OCNDFN; break;
              case _INOUT :
                lclcnf += BVL_BCNDFN; break;
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != 0)
              errflg = 1;
            break;

          case MUX_BIT:

            lclcnf += BVL_MUXDFN + BVL_BUSDFN;
            switch (mode)
              {
              case _OUT :
                lclcnf += BVL_OCNDFN; break;
              case _INOUT :
                lclcnf += BVL_BCNDFN; break;
              case _IN :
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != BUS)
              errflg = 1;
            break;

          case WOR_BIT:

            lclcnf += BVL_WORDFN + BVL_BUSDFN;
            switch (mode)
              {
              case _OUT :
                lclcnf += BVL_OCNDFN; break;
              case _INOUT :
                lclcnf += BVL_BCNDFN; break;
              case _IN :
              case _LINKAGE :
              case 0 :
                errflg = 1; break;
              }
            if (kind != BUS)
              errflg = 1;
            break;

          case REG_BIT:
          case NATURAL:

            errflg = 1;
            break;
          }
        if (errflg == 1)
          bvl_error (5,NULL);
        break;

      case 'G':
	/* ###------------------------------------------------------### */
	/*    If object is a generic :					*/
	/*       - only natural type is allowed				*/
	/* ###------------------------------------------------------### */

        if ((type != NATURAL) || (mode != 0) || (kind != 0))
          {
          errflg = 1;
          bvl_error (77,NULL);
          }
        break;

      case 'S':
	/* ###------------------------------------------------------### */
	/*    If object is a signal :					*/
	/*       - no mode can be specified				*/
	/*       - if type is bit no guard indication can be used	*/
	/*       - if type is wor_bit or mux_bit, bus kind must be used	*/
	/*       - if type is reg_bit, register kind must be used	*/
	/*       - other types are illegal				*/
	/* ###------------------------------------------------------### */

        switch (type)
          {
          case BIT:
            lclcnf += BVL_BITDFN + BVL_NORDFN;
            if ((mode != 0) || (kind != 0))
              errflg = 1;
            break;
          case MUX_BIT:
            lclcnf += BVL_MUXDFN + BVL_BUSDFN;
            if ((mode != 0) || (kind != BUS))
              errflg = 1;
            break;
          case WOR_BIT:
            lclcnf += BVL_WORDFN + BVL_BUSDFN;
            if ((mode != 0) || (kind != BUS))
              errflg = 1;
            break;
          case REG_BIT:
            lclcnf += BVL_RBIDFN + BVL_REGDFN;
            if ((mode != 0) || (kind != REGISTER))
              errflg = 1;
            break;
          case NATURAL:
            errflg = 1; break;
          }

        if (mode != 0)
          errflg = 1;

        if (errflg == 1)
          bvl_error (11,NULL);
        break;

      case 'C':
	/* ###------------------------------------------------------### */
	/*    If object is a constant :					*/
	/*       - only bit type without any other indication is legal	*/
	/* ###------------------------------------------------------### */

        lclcnf += BVL_CSTDFN;
        if ((type != BIT) || (mode != 0) || (kind != 0))
          {
          errflg = 1;
          bvl_error (78,NULL);
          }
        break;
      }
    }

  *conf = lclcnf ;
  return (errflg);
  }
