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
/* file		: pat_decl.yac						*/
/* date		: Mar 13 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: parser-driver function				*/
/* ###--------------------------------------------------------------### */

%{

#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "pat.h"
#include "ppt.h"
#include "pat_defs.h"
#include "pat_type.h"
#include "pat_decl.h"

extern  void  pat_decl_y_restart ( );
extern  int   pat_decl_y_parse   ( );
extern  int   pat_decl_y_lex     ( );

/* ###--------------------------------------------------------------### */
/* function	: yyerror						*/
/* description	: print an error message (errors detected by yacc)	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void pat_decl_y_error (str)

char *str;
  {
  ERR_FLG += pat_error (99, str, ' ', PAT_LINNUM);
  }

/* ###--------------------------------------------------------------### */
/* function	: addiol						*/
/* description	: add a list of PAIOL structure at the top of the list	*/
/* called func.	: pat_addpaiol						*/
/* ###--------------------------------------------------------------### */

static struct paiol *addiol (lastiol, gname, format, mode, blank, flag)

struct paiol *lastiol;
struct array  gname  ;
char          format ;
char          mode   ;
short         blank  ;
char          flag   ;

  {
  struct paiol *ptiol         = lastiol;
  char          extname [100] ;
  short         i             ;
  short         inc           = 1;

  if (gname.left == -1)
    {
    ptiol       = pat_addpaiol (ptiol, gname.ident, format, mode, blank);
    ptiol->FLAG = flag;
    }
  else
    {
    if (gname.left >= gname.right)
      inc = -1;

    for (i=gname.left ; i!=(gname.right+inc) ; i+=inc)
      {
      sprintf (extname, "%s %d", gname.ident, i);
      ptiol       = pat_addpaiol (ptiol, extname, format, mode, blank);
      ptiol->FLAG = flag;
      }
    }

  return (ptiol);
  }

/* ###--------------------------------------------------------------### */
/* function	: pshtab						*/
/* description	: push a name on the TAB stack to build a virtual array	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void pshtab (gname)

struct array gname;

  {
  if (TAB_IDX > 256)
    ERR_FLG += pat_error (6, NULL, ' ', PAT_LINNUM);
  else
    {
    TAB [TAB_IDX] = gname;
    TAB_IDX++;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: pshnam						*/
/* description	: push a name on the NAMTAB stack to build a		*/
/*		  hierarchical name					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void pshnam (str)

char *str;

  {
  if (NAM_IDX > 30)
    ERR_FLG += pat_error (14, NULL, ' ', PAT_LINNUM);
  else
    {
    NAMTAB [NAM_IDX] = str;
    NAM_IDX++;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: initab						*/
/* description	: create a new dictionary				*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

static struct entry **initab ()

  {
  struct entry **head;
  int            i;

  head = (struct entry **) mbkalloc (sizeof(struct entry *) * PAT_HSZDFN);

  for (i=0 ; i<PAT_HSZDFN ; i++)
    head[i] = NULL;

  return (head);
  }

/* ###--------------------------------------------------------------### */
/* function	: addent						*/
/* description	: add a new entry in a dictionary			*/
/* called func.	: mbkaloc						*/
/* ###--------------------------------------------------------------### */

static struct entry *addent (head, key)

struct entry *head;
char         *key;

  {
  struct entry *entry;
  int           i;

  if (ENTRY_HEAD == NULL)
    {
    ENTRY_HEAD = (struct entry *) mbkalloc (sizeof(struct entry) * PAT_ALODFN);

    entry = ENTRY_HEAD;
    for (i=1 ; i<PAT_ALODFN ; i++)
      {
      entry->next = entry + 1;
      entry++;
      }
    entry->next = NULL;
    }

  entry       = ENTRY_HEAD;
  ENTRY_HEAD  = ENTRY_HEAD->next;

  entry->next     = head;

  entry->fd0_val  = 0;
  entry->fd1_val  = 0;
  entry->pt0_val  = 0;
  entry->pt1_val  = 0;

  entry->key      = key;

  return (entry);
  }

/* ###--------------------------------------------------------------### */
/* function	: addtab						*/
/* description	: add a new information into a dictionary. If the key	*/
/*		  doesn't exist create a new entry			*/
/* called func.	: addent						*/
/* ###--------------------------------------------------------------### */

static void addtab (head, key_str, field, valu)

struct entry **head   ;
char          *key_str;
int            field  ;
int            valu   ;

  {
  int           found    = 0;
  int           index    ;
  struct entry *entry_pnt;

  index     = ((unsigned long) key_str) % PAT_HSZDFN;
  entry_pnt = head [index];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->key == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->next;
    }

  if (found == 0)
    {
    head [index] = addent (head [index], key_str);
    entry_pnt    = head [index];
    }

  switch (field)
    {
    case 0 :
      entry_pnt->fd0_val = valu; break;
    case 1 :
      entry_pnt->fd1_val = valu; break;
    case 6 :
      entry_pnt->pt0_val = valu; break;
    case 7 :
      entry_pnt->pt1_val = valu; break;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: chktab						*/
/* description	: extract an information from a dictionary. Return	*/
/*		  0 if the information has not been found.		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static int chktab (head, key_str, field)

struct entry **head   ;
char          *key_str;
int            field  ;

  {
  int           found = 0;
  int           valu  = 0;
  struct entry *entry_pnt;

  entry_pnt = head [((unsigned long)key_str) % PAT_HSZDFN];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->key == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->next;
    }

  if (found == 1)
    {
    switch (field)
      {
      case 0 :
        valu = entry_pnt->fd0_val; break;
      case 1 :
        valu = entry_pnt->fd1_val; break;
      case 6 :
        valu = entry_pnt->pt0_val; break;
      case 7 :
        valu = entry_pnt->pt1_val; break;
      }
    }

  return (valu);
  }

/* ###--------------------------------------------------------------### */
/* function	: fretab						*/
/* description	: remove entirely a dictionary				*/
/* called func.	: mbkfree						*/
/* ###--------------------------------------------------------------### */

static void fretab (pt_hash)

struct entry **pt_hash;
  {
  struct entry *pt_entry;
  int           i       ;

  if (pt_hash != NULL)
    {
    for (i=0 ; i<PAT_HSZDFN ; i++)
      {
      if ((pt_entry = pt_hash[i]) != NULL)
        {
        while (pt_entry->next != NULL)
          pt_entry = pt_entry->next;

        pt_entry->next = ENTRY_HEAD ;
        ENTRY_HEAD     = pt_hash [i];
        }
      }
    mbkfree (pt_hash);
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: pat_declpat						*/
/* description	: parse a file in pat format and return a PASEQ		*/
/* called func.	: yyparse			 			*/
/* ###--------------------------------------------------------------### */

struct paseq *pat_declpat (fp, ptseq)

FILE          *fp    ;
struct paseq  *ptseq ;

  {
  extern FILE *pat_decl_y_in     ;
  static int   call_nbr = 0;

  PAT_SEQPNT = ptseq;
  pat_decl_y_in        = fp;

  PAT_INSPNT = NULL;
  PAT_COMPNT = NULL;
  PAT_GRPPNT = NULL;
  PAT_IOLPNT = NULL;
  PAT_LINNUM = 1;
  STT_NBR    = 0;
  DCL_NBR    = 0;
  ERR_FLG    = 0;
  TAB_IDX    = 0;
  NAM_IDX    = 0;

  HSHTAB             = initab ();

  PAT_SEQPNT         = pat_addpaseq (PAT_SEQPNT);
  PAT_SEQPNT->FLAGS |= PAT_SEQ__ZERODELAY;

  if (call_nbr != 0)
    pat_decl_y_restart (pat_decl_y_in);

  pat_decl_y_parse ();
  call_nbr++;
  
  PAT_SEQPNT->PAINS   = (struct pains *) reverse ((struct chain *)PAT_INSPNT);
  PAT_SEQPNT->DECCOM  = (struct pacom *) reverse ((struct chain *)PAT_COMPNT);
  PAT_SEQPNT->PAGRP   = (struct pagrp *) reverse ((struct chain *)PAT_GRPPNT);
  PAT_IOLPNT          = (struct paiol *) reverse ((struct chain *)PAT_IOLPNT);
  PAT_SEQPNT->PAIOL   = pat_crtpaiol (PAT_IOLPNT);
  PAT_SEQPNT->FLAGS  |= PAT_SEQ__IOLARRAY;
  PAT_SEQPNT->LINENO  = PAT_LINNUM ;
  PAT_SEQPNT->IOLNBR  = DCL_NBR;

  if (ERR_FLG == 0)
    PAT_SEQPNT->ERRFLG = 0;
  else
    PAT_SEQPNT->ERRFLG = 1;

  pat_frepaiol (PAT_IOLPNT);
  fretab (HSHTAB);

  return (PAT_SEQPNT);
  }

%}

%union
  {
  long          valu;
  float         fval;
  char          immd;
  char         *text;
  struct array  arra;
  struct label  labe;
  }

%token        _GTSym
%token        _IN
%token        _INOUT
%token        _LESym
%token        _LTSym
%token        _OUT
%token <valu> AbstractLit
%token <text> BitStringLit
%token        BEGIN_
%token        Colon
%token        Comma
%token <text> Comment
%token        DOWNTO
%token        Dot
%token        END_
%token        FS
%token <immd> Format
%token        INSPECT
%token <text> Identifier
%token        LeftParen
%token <text> Literal
%token        NS
%token        MS
%token        PS
%token        Plus
%token        REGISTER
%token        RightParen
%token        SAVE
%token        SIGNAL
%token <valu> Semicolons
%token        SPY
%token        TO
%token        TRACE
%token        US
%token        UnknownChar

%type  <arra> name
%type  <arra> simple_name
%type  <text> instance_name
%type  <text> file_name
%type  <arra> indexed_name
%type  <arra> slice_name
%type  <immd> .Format.
%type  <immd> mode
%type  <text> Identifier...Identifier..
%type  <valu> Semicolons_ERR
%type  <valu> .SPY.
%type  <valu> .TRACE.

/* %pure_parser */
%start        pat_format_file

%%
pat_format_file
	: declarative_block
	  BEGIN_
		{
		YYACCEPT;
		}
	;

declarative_block
	: ..declaration_item..
	;

..declaration_item..
	: declaration_item
	| ..declaration_item..
	  declaration_item
	;

declaration_item
	: input_output_declaration
	| instance_inspection
	| other_statement
	| error
	  Semicolons_ERR
	;

instance_inspection
	: INSPECT
	  instance_name
	  file_name
	  Semicolons_ERR
		{ PAT_INSPNT = pat_addpains (PAT_INSPNT, $2, $3); }
	;

instance_name
	: Identifier...Identifier..
		{ $$ = $1; }
	;

file_name
	: Identifier...Identifier..
		{ $$ = $1; }
	;

other_statement
	: Comment
		{
		PAT_COMPNT = pat_addpacom (PAT_COMPNT, $1, STT_NBR);
		STT_NBR    = 0;
		}
	;

input_output_declaration
	: signal_declaration
	| group_declaration
	;

signal_declaration
	: mode
	  name
	  .Format.
	  .SPY.
	  .TRACE.
	  Semicolons_ERR
		{
		int           length;
		int           lclmod;
		unsigned char flag  ;

		if ((lclmod = chktab (HSHTAB, $2.ident, PAT_MODDFN)) != 0)
		  {
		  if (lclmod == 'I')
		    ERR_FLG += pat_error (13, $2.ident, ' ', PAT_LINNUM);
		  if (lclmod != $1)
		    ERR_FLG += pat_error (7, $2.ident, ' ', PAT_LINNUM);
		  }
		else
		  addtab (HSHTAB, $2.ident, PAT_MODDFN, $1);

		if ($2.left < $2.right)
		  length = $2.right - $2.left + 1;
		else
		  length = $2.left - $2.right + 1;

		STT_NBR ++;
		flag       = $4 | $5;
		PAT_IOLPNT = addiol (PAT_IOLPNT, $2, $3, $1, $6, flag);

		if (length != 1)
		  PAT_GRPPNT = pat_addpagrp (PAT_GRPPNT, $2.ident, length,
		                             DCL_NBR   , 0);
		else
		  {
		  if ($3 != 'B')
		    ERR_FLG += pat_error (1, NULL, ' ', PAT_LINNUM);
		  }

		DCL_NBR += length;
		}
	;

group_declaration
	: mode
	  Identifier...Identifier..
	  LeftParen
	  ...name..
	  RightParen
	  .Format.
	  .SPY.
	  .TRACE.
	  Semicolons_ERR
		{
		int           i      ;
		int           lclmod ;
		int           length = 0;
		unsigned char flag   ;

		if (chktab (HSHTAB, $2, PAT_MODDFN) != 0)
		  ERR_FLG += pat_error (8, $2, ' ', PAT_LINNUM);
		else
		  addtab (HSHTAB, $2, PAT_MODDFN, $1);

		STT_NBR++;

		for (i=0 ; i<TAB_IDX ; i++)
                  {
		  if ((lclmod = chktab(HSHTAB,TAB[i].ident,PAT_MODDFN)) != 0)
		    {
		    if (lclmod != $1)
		      ERR_FLG += pat_error (7, TAB[i].ident, ' ', PAT_LINNUM);
		    }
		  else
		    addtab (HSHTAB, TAB[i].ident, PAT_MODDFN, $1);

		  flag       = $7 | $8;
		  PAT_IOLPNT = addiol (PAT_IOLPNT, TAB[i], $6, $1, $9, flag);
		  if (TAB[i].left < TAB[i].right)
		    length += TAB[i].right - TAB[i].left + 1;
		  else
		    length += TAB[i].left - TAB[i].right + 1;
                  }

		PAT_GRPPNT = pat_addpagrp (PAT_GRPPNT, $2, length, DCL_NBR, 1);
		TAB_IDX    = 0;
		DCL_NBR   += length;
		}
	;

.Format.
	: /*empty*/
		{ $$ = 'B'; }
	| Format
		{ $$ = $1; }
	;

...name..
	: name
		{ pshtab ($1); }
	| ...name..
	  Comma
	  name
		{ pshtab ($3); }
	;

name
	: simple_name
		{ $$ = $1; }
	| indexed_name
		{ $$ = $1; }
	| slice_name
		{ $$ = $1; }
	;

simple_name
	: Identifier...Identifier..
		{
		$$.ident = $1;
		$$.left  = -1;
		$$.right = -1;
		}
	;

indexed_name
	: Identifier...Identifier..
	  LeftParen
	  AbstractLit
	  RightParen
		{
		$$.ident = $1;
		$$.left  = $3;
		$$.right = $3;
		}
	;

slice_name
	: Identifier...Identifier..
	  LeftParen
	  AbstractLit
	  direction
	  AbstractLit
	  RightParen
		{
		$$.ident = $1;
		$$.left  = $3;
		$$.right = $5;
		}
	;

Identifier...Identifier..
	: Identifier
	  ...Identifier..
		{
		char name [256];
		int  i         ;

		strcpy (name, $1);
		for (i=0; i<NAM_IDX ; i++)
		 {
		 strcat (name, ".");
		 strcat (name, NAMTAB [i]);
		 }
		NAM_IDX = 0;
		$$ = namealloc (name);
		}
	;

...Identifier..
	: /*empty*/
	| ...Identifier..
	  Dot
	  Identifier
		{ pshnam ($3); }
	;

direction
	: TO
	| DOWNTO
	;

mode
	: _IN
		{ $$ = 'I'; }
	| _OUT
		{ $$ = 'O'; }
	| _INOUT
		{ $$ = 'T'; }
	| SIGNAL
		{ $$ = 'S'; }
	| REGISTER
		{ $$ = 'R'; }
	;

Semicolons_ERR
	: Semicolons
		{
		if (ERR_FLG > 30)
		  YYABORT;
		else
		   yyerrok;
		}
	;

.SPY.
	: /*empty*/
		{ $$ = 0 ;           }
	| SPY
		{ $$ = PAT_IOL__SPY; }
	;

.TRACE.
	: /*empty*/
		{ $$ = 0 ;             }
	| TRACE
		{ $$ = PAT_IOL__TRACE; }
	;
%%
