/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##                                                                  ##*/
/*##  E-mail support:    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/

%{
#include "global.h"
#include "yyy.h"

extern int NBR_LIGNES;
%}
nbr (([1-9][0-9]*)|0)
/*les noms en vhdl ne peuvent avoir plusieurs tirets consecutifs, de plus ils ne peuvent finir par un tiret*/
mot ([a-zA-Z](_?[a-zA-Z0-9])*)
lettre ([a-zA-Z])

%%
[ \t]				;
\n						{NBR_LIGNES++;}
[pP][cC][bB][sS]/[ \t\n]+{mot}			{return _PCBS;}
[vV]([eE][rR][sS][iI][oO][nN])?/[ \t\n]*{nbr}\.	{return Version;}
[iI][nN]					{return _IN;}
[oO][uU][tT]					{return _OUT;}
[iI][nN][oO][uU][tT]				{return _INOUT;}
[;]+						{return Pt_virg;}
:						{return Db_pt;}
\<\=						{return Assign;}
\(						{return Par_ouv;}
\]						{return Cro_fer;}
\)						{return Par_fer;}
\[						{return Cro_ouv;}
{lettre}/[ \t\n]*:		{/*remarque: le [ \t\n]*: n'est pas parse*/
				 yylval.lettre=(char)tolower(yytext[0]);
				/*reservation espace memoire  cf. MUT_H*/
				 return Lettre;}
{mot}				{yylval.mot=(char*)namealloc(yytext);
				/*reservation espace memoire  cf. MUT_H*/
				 return Mot;}
{nbr}\.{nbr}			{yylval.flottant=atof(yytext);
				/*reservation espace memoire  cf. MUT_H*/
				return Num_version;}
{nbr}/[ \t\n]*[\]\)]		{/*remarque: [ \t\n]*[\)\]] n'est pas parse*/
     				 yylval.mot=(char*)namealloc(yytext);
				/*reservation espace memoire  cf. MUT_H*/
				 return Index;}
{nbr}				{yylval.val=atoi(yytext);
				 return Nbr;}
\-\-.*				;
.				{return *yytext;}
%%


int yywrap(){return 1;}


