%{
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 /*
  * Parsing Philips modgen format, done during the IDPS project.
  * Author: Frédéric Pétrot
  */

#include <string.h>
#include <ctype.h>
#include <mut.h>
#include "mg2mbk_y.h"
static void zweep();
extern char *filenameforlex();
/* change lex input routine to a non case sensitive one */
#ifdef FLEX_SCANNER
#ifdef YY_INPUT
#undef YY_INPUT
#endif
#define YY_INPUT(buf,result,max_size) \
	do { \
	int c = getc(yyin); \
		result = (c == EOF) ? YY_NULL \
									: (buf[0] = isupper(c) ? tolower(c) : c, 1); \
	} while(0)

int yylineno;
#else /* using lex, for sure */
#ifdef input
#undef input
#endif
#define input()	(((yytchar = yysptr > yysbuf ? U(*--yysptr)    \
							: getc(yyin)) == 10 ?(yylineno++,yytchar)   \
								: yytchar) == EOF ? 0                    \
									: isupper(yytchar) ? tolower(yytchar) \
										: yytchar)
#endif
%}

cset		[a-zA-Z_/]
digit		[0-9]
sign		[-+]
blank		[ \t]*
ident		{cset}({cset}|{digit}|\[{digit}+\]|\.|$)*
fp			{sign}?{digit}*\.?{digit}+

%%
{blank}					{}
\n							{
#ifdef FLEX_SCANNER
								yylineno++;
#endif
							}
\/\*						{zweep('/');}
\#							{zweep('\n');}
property					{zweep('}');}
node						{zweep('}');}
text						{zweep(';');}
constraint				{zweep(';');}
cell						{return CELL;}
place						{return PLACE;}
terminal					{return TERMINAL;}
ra							{return RA;}
stretch					{return STRETCH;}
wire						{return WIRE;}
width						{return MWIDTH;}
hor						{return HORIZ;}
ver						{return VERTI;}
left						{return MLEFT;}
right						{return MRIGHT;}
top						{return MTOP;}
bottom					{return MBOTTOM;}
at							{return AT;}
mx							{return MX;}
my							{return MY;}
r							{return R;}
ntr						{return NTR;}
ptr						{return PTR;}
cop						{return COP;}
con						{return CON;}
cops						{return COPS;}
copw						{return COPW;}
conw						{return CONW;}
pvia						{return PVIA;}
cxn						{return CXN;}
cxp						{return CXP;}
"="						{return '=';}
"{"						{return '{';}
"}"						{return '}';}
"("						{return '(';}
")"						{return ')';}
","						{return ',';}
";"						{return ';';}
{ident}					{	yylval.s = mbkstrdup(yytext);
								return STRING;}
{fp}						{float f;
								sscanf(yytext, "%f", &f);
								yylval.n = (long)(f * SCALE_X);
#ifdef GRIDDED
								yylval.n *= 2;
#endif
								return NUM;}
%%

int yywrap()
{
	return 1;
}

static void zweep(find_it)
char find_it;
{
char c;

	while (1) {
		if ((c = input()) == 0) {
			(void)fprintf(stderr, "reached EOF before expected end line %d\n",
								yylineno);
			exit(12);
		}
		/* special case for comments */
		if (c == '*')
			if (find_it == '/')
				if ((c = input()) == 0) {
					(void)fprintf(stderr,
										"reached EOF before expected end of comment\n");
					exit(12);
				}
		if (c == find_it)
			return;
	}
}

int yyerror(s)
char *s;
{
	fflush(stdout);
	fprintf(stderr, "modgen parser : syntax error line %d on %s file %s\n",
				yylineno, yytext, filenameforlex());
	exit(13);
}
