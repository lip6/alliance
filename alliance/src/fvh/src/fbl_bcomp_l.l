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
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mut.h" 
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "fvh.h"

typedef struct fbl_expr
  {
  char	 	*IDENT; 	/* identifier or constant name		*/
  struct chain	*LIST_ABL;	/* pointer on fbl_abllst list		*/
  short		 WIDTH;         /* width of bit vector			*/
  }
fbl_ablstr;

typedef struct
  {
  char 		*NAME;		/* identifier name */
  short  	 LEFT;		/* vector's left index */
  short 	 RIGHT;		/* vector's right index */
  char           FLAG;
  }
fbl_name;

struct g_type
  {
  int  VALU;
  char FLAG;
  };



#include "fbl_bcomp_y.h"
#include "fbl_bedef.h"
#include "fbl_blex.h"

static el_mc tab_mc []=
  {
    {"abs"		,ABS},
    {"access"		,ACCESS},
    {"after"		,AFTER},
    {"alias"		,ALIAS},
    {"all"		,ALL},
    {"and"		,_AND},
    {"architecture"	,ARCHITECTURE},
    {"array"		,ARRAY},
    {"assert"		,ASSERT},
    {"attribute"	,ATTRIBUTE},
    {"begin"		,_BEGIN},
    {"block"		,_BLOCK},
    {"body"		,BODY},
    {"buffer"		,BUFFER},
    {"bus"		,BUS},
    {"case"		,CASE},
    {"component"	,COMPONENT},
    {"configuration"	,CONFIGURATION},
    {"constant"		,CONSTANT},
    {"disconnect"	,DISCONNECT},
    {"downto"		,DOWNTO},
    {"else"		,ELSE},
    {"elsif"		,ELSIF},
    {"end"		,_END},
    {"entity"		,ENTITY},
    {"error"		,ERROR},
    {"event"            ,_EVENT},
    {"exit"		,_EXIT},
    {"file"		,_FILE},
    {"for"		,FOR},
    {"function"		,FUNCTION},
    {"generate"		,GENERATE},
    {"generic"		,GENERIC},
    {"guarded"		,GUARDED},
    {"if"		,IF},
    {"in"		,_IN},
    {"inout"		,_INOUT},
    {"is"		,IS},
    {"label"		,_LABEL},
    {"library"		,LIBRARY},
    {"linkage"		,_LINKAGE},
    {"loop"		,LOOP},
    {"map"		,MAP},
    {"mod"		,MOD},
    {"nand"		,_NAND},
    {"new"		,NEW},
    {"next"		,_NEXT},
    {"nor"		,_NOR},
    {"not"		,_NOT},
    {"null"		,_NULL},
    {"of"		,OF},
    {"on"		,ON},
    {"open"		,OPEN},
    {"or"		,_OR},
    {"others"		,OTHERS},
    {"out"		,_OUT},
    {"package"		,PACKAGE},
    {"port"		,PORT},
    {"procedure"	,PROCEDURE},
    {"process"		,PROCESS},
    {"range"		,RANGE},
    {"record"		,RECORD},
    {"register"		,REGISTER},
    {"rem"		,REM},
    {"report"		,REPORT},
    {"return"		,RETURN},
    {"select"		,SELECT},
    {"severity"		,SEVERITY},
    {"signal"		,SIGNAL},
    {"stable"		,_STABLE},
    {"subtype"		,SUBTYPE},
    {"then"		,THEN},
    {"to"		,TO},
    {"transport"	,TRANSPORT},
    {"type"		,FBHTYPE},
    {"units"		,UNITS},
    {"until"		,UNTIL},
    {"use"		,USE},
    {"variable"		,VARIABLE},
    {"wait"		,WAIT},
    {"warning"		,WARNING},
    {"when"		,WHEN},
    {"while"		,WHILE},
    {"with"		,WITH},
    {"xor"		,_XOR}
  };

static int find_mc(s)

  char *s;
  {
  char *loc;
  el_mc *pt;

  loc = namealloc(s);
  pt= (el_mc *) bsearch(loc, (char *)tab_mc,FBL_NB_MC,sizeof(el_mc),strcmp);
  if (pt==NULL) return(-1);
  return(pt->kval);
  }


%}

upper_case_letter 	  [A-Z]
digit 			  [0-9]
special_character   	  [\#\&\'\(\)\*\+\,\-\.\/\:\;\<\=\>\_\|]
space_character 	  [ \t]
format_effector		  [\t\v\r\l\f]
end_of_line		  \n
lower_case_letter 	  [a-z]
other_special_character   [\!\$\@\?\[\\\]\^\`\{\}\~]

graphic_character	  ({basic_graphic_character}|{lower_case_letter}|{other_special_character})
basic_graphic_character	  ({upper_case_letter}|{digit}|{special_character}|{space_character})
letter		   	  ({upper_case_letter}|{lower_case_letter})
letter_or_digit	   	  ({letter}|{digit})
decimal_literal	   	  {integer}(\.{integer})?({exponent})?
integer		   	  {digit}(_?{digit})*
exponent	   	  ([eE][-+]?{integer})
base		   	  {integer}
based_integer	   	  {extended_digit}(_?{extended_digit})*
extended_digit	   	  ({digit}|[a-fA-F])
base_specifier	  	  (B|b|O|o|X|x)

%%
[ \t]			;
\&			{ return(Ampersand); }
\'			{ return(Apostrophe); }
\(			{ return(LeftParen); }
\)			{ return(RightParen); }
"**"			{ return(DoubleStar); }
\*			{ return(Star); }
\+			{ return(Plus); }
\,			{ return(Comma); }
\-			{ return(Minus); }
":="			{ return(VarAsgn); }
\:			{ return(Colon); }
\;			{ return(Semicolon); }
"<="			{ return(_LESym); }
">="			{ return(_GESym); }
\<			{ return(_LTSym); }
\>			{ return(_GTSym); }
=			{ return(_EQSym); }
\/=			{ return(_NESym); }
"=>"			{ return(Arrow); }
"<>"			{ return(Box); }
\|			{ return(Bar); }
!			{ return(Bar); }
\.			{ return(Dot); }

{letter}(_?{letter_or_digit})*		{
			int itoken;
			itoken=find_mc(yytext);
			if (itoken== -1) 
			  {
			  yylval.text = namealloc(yytext);
			  return ( Identifier );
			  }
			else
			  {
			  return ( itoken );
			  }
					}
({decimal_literal})|({base}#{based_integer}(\.{based_integer})?#({exponent})?)|({base}:{based_integer}(\.{based_integer})?:({exponent})?)		{
			yylval.text = mbkalloc((unsigned int)strlen(yytext)+1);
	 		strcpy(yylval.text,yytext);
			return ( AbstractLit );
									}
'({graphic_character}|\"|\%)' 						{
			yylval.text = namealloc (yytext);
			return ( CharacterLit );
									}
(\"({graphic_character}|(\"\")|\%)*\")|(\%({graphic_character}|(\%\%)|\")*\%) {
			yylval.text = namealloc (yytext);
			return ( StringLit );
									}
{base_specifier}((\"{extended_digit}(_?{extended_digit})*\")|(\%{extended_digit}(_?{extended_digit})*\%)) 							{
			yylval.text = namealloc (yytext);
			return ( BitStringLit );
									}
\n									{
			/* end of line */
			 FBL_LINNUM++;
									}
\-\-{space_character}*[pP][rR][aA][gG][mM][aA].*$ 							{
			/* pragma */
			yylval.text = namealloc (yytext);
			return(Pragma);
									}
\-\-.*$ 								{
			/* comment */
			/* nothing */
									}
.									{
			return (*yytext);
									}
%%
int yywrap()
{
 return(1);
}
