/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/

%{
#include "global.h"
#include "emul_y.h"

int NBR_LIGNES = 1;

%}
nbr (([1-9][0-9]*)|0)
/*les noms en vhdl ne peuvent avoir plusieurs tirets consecutifs, de plus ils ne peuvent finir par un tiret*/
mot ([a-zA-Z](_?[a-zA-Z0-9])*)

%%
[ \t]				;
\n						{NBR_LIGNES++;}
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
{mot}				{yylval.MOT=(char*)namealloc(yytext);
				/*reservation espace memoire  cf. MUT_H*/
				 return TMot;}
{nbr}				{yylval.VAL=atoi(yytext);
				 return TNbr;}
\-\-.*				;
.				{return *yytext;}
%%


int yywrap(){return 1;}


