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

/* ###--------------------------------------------------------------### */
/* file		: pat_pars.lex						*/
/* date		: Mar 13 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: parser-driver function				*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "pat.h"
#include "pat_type.h"
#include "pat_decl_y.h"

extern int yychar;
extern YYSTYPE yylval;
extern int yynerrs;


/* ICI LUDO
#ifdef YY_DECL
#undef YY_DECL
#endif

#define YY_DECL int pat_lex (lval_pnt) YYSTYPE *lval_pnt;
#define yylval  (*lval_pnt)
 FIN LUDO
*/

static char buff [1024];
%}

%s INS_PAT
%s OUT_PAT
%s DTC_FRM

letter		[a-zA-Z]

%%
\:								{
/*printf ("Colon\n");*/
		BEGIN INS_PAT;
		return (Colon);
								}
(\;)+								{
/*printf ("Semicolon\n");*/
		BEGIN OUT_PAT;

		if (yyleng < 16)
		  yylval.valu = yyleng - 1;
		else
		  yylval.valu = 15;

		return (Semicolons);
								}
\.								{
/*printf ("Dot\n");*/
		BEGIN OUT_PAT;
		return (Dot);
								}
\,								{
/*printf ("Comma\n");*/
		return (Comma);
								}
\<								{
/*printf ("_LTSym\n");*/
		return (_LTSym);
								}
\>								{
/*printf ("_GTSym\n");*/
		return (_GTSym);
								}
\(								{
/*printf ("LeftParen\n");*/
		BEGIN INITIAL;
		return (LeftParen);
								}
\)								{
/*printf ("RightParen\n");*/
		BEGIN DTC_FRM;
		return (RightParen);
								}
<OUT_PAT>\+							{
/*printf ("Plus\n");*/
		return (Plus);
								}
\<=								{
/*printf ("_LESym\n");*/
		return (_LESym);
								}
<DTC_FRM>[xobXOB]						{
/*printf ("Format\n");*/
		yylval.immd = yytext[0];
		return (Format);
								}
\'[01]\'							{
/*printf ("OneBit BitStringLit\n");*/
		strcpy (buff,yytext);
		yylval.text = buff;
		return (BitStringLit);
								}
[xX]\"[0-9a-fA-F]+\"						{
/*printf ("Hexa BitStringLit\n");*/
		strcpy (buff,yytext);
		yylval.text = buff;
		return (BitStringLit);
								}
[oO]\"[0-7]+\"							{
/*printf ("Octal BitStringLit\n");*/
		strcpy (buff,yytext);
		yylval.text = buff;
		return (BitStringLit);
								}
[bB]?\"[01]+\"							{
/*printf ("Binary BitStringLit\n");*/
		strcpy (buff,yytext);
		yylval.text = buff;
		return (BitStringLit);
								}
<INITIAL,OUT_PAT,DTC_FRM>(0|[1-9][0-9]*)			{
/*printf ("AbstractLit\n");*/
		yylval.valu =  atoi (yytext);
		return (AbstractLit);
								}
<INS_PAT>[0-9a-fA-FuU\*\+\-\? \t\n]+				{
		int i = 0;
		int j = 0;

/*printf ("Literal\n");*/
		while ((buff[j] = yytext[i++]) != '\0')
		  {
		  if ((buff[j] != ' ' ) && (buff[j] != '\t') &&
		      (buff[j] != '\n'))
		    {
		    if ((buff[j] == 'U') || (buff[j] == 'u'))
		      {
		      buff[j] = '*';
		      }
		    j++;
		    }
		  else
		    {
		    if (buff[j] == '\n')
		      PAT_LINNUM++;
		    }
		  }

		buff[j] = '\0';

		yylval.text = buff;
		return (Literal);
								}
<INITIAL,OUT_PAT,DTC_FRM>{letter}(_?({letter}|[0-9]))*		{
		int         code         ;
		static int  lcl_search ();

		yylval.text = namealloc (yytext);
		if ((code = lcl_search (yylval.text)) == -1)
		  {
/*printf ("Identifier : %s\n", yytext);*/
		  BEGIN DTC_FRM;
		  return (Identifier);
		  }
		else
		  {
/*printf ("KeyWord : %s\n", yytext);*/
		  return (code);
		  }
								}
\#.*$								{
		yylval.text = namealloc (&yytext[1]);
		return (Comment);
								}
<INITIAL,OUT_PAT,DTC_FRM>\-\-.*$				{
								}
[ \t]								{
								}
\n								{
		PAT_LINNUM++;
								}
.								{
		return (UnknownChar);
								}
%%

/* ###--------------------------------------------------------------### */
/* function	: yywrap						*/
/* description	: return 1						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int pat_decl_y_wrap ()
  {
  return (1);
  }

/* ###--------------------------------------------------------------### */
/* function	: lcl_search						*/
/* description	: check that an identifier is a reserved word or not	*/
/* called func.	: addht, addhtitem, gethtitem, namealloc		*/
/* ###--------------------------------------------------------------### */

static int lcl_search (key)

char  *key;

  {
  static ht *pt_hash = NULL;

  if (pt_hash == NULL)
    {
    pt_hash = addht (32);

    addhtitem (pt_hash, namealloc ("begin"   ), BEGIN_  );
    addhtitem (pt_hash, namealloc ("downto"  ), DOWNTO  );
    addhtitem (pt_hash, namealloc ("end"     ), END_    );
    addhtitem (pt_hash, namealloc ("fs"      ), FS      );
    addhtitem (pt_hash, namealloc ("in"      ), _IN     );
    addhtitem (pt_hash, namealloc ("inout"   ), _INOUT  );
    addhtitem (pt_hash, namealloc ("inspect" ), INSPECT );
    addhtitem (pt_hash, namealloc ("ms"      ), MS      );
    addhtitem (pt_hash, namealloc ("ns"      ), NS      );
    addhtitem (pt_hash, namealloc ("ps"      ), PS      );
    addhtitem (pt_hash, namealloc ("out"     ), _OUT    );
    addhtitem (pt_hash, namealloc ("register"), REGISTER);
    addhtitem (pt_hash, namealloc ("save"    ), SAVE    );
    addhtitem (pt_hash, namealloc ("signal"  ), SIGNAL  );
    addhtitem (pt_hash, namealloc ("spy"     ), SPY     );
    addhtitem (pt_hash, namealloc ("to"      ), TO      );
    addhtitem (pt_hash, namealloc ("trace"   ), TRACE   );
    addhtitem (pt_hash, namealloc ("us"      ), US      );
    }
  return (gethtitem (pt_hash, namealloc (key)));
  }
