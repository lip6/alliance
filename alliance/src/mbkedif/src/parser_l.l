
%option nounput

%p 5000
%k 1200


%{
#include <string.h>
#include "parser_y.h"

#undef ECHO
#define ECHO {strcpy(yylval.sval, yytext);} 
void yyJumpOver();
int  yylineno;
%}

minus		[a-z]
majus   	[A-Z]
digit		[0-9]
spec_carac	[\40-\57]|[\72-\100]|[\133-\140]|[\173-\176]
blank		[ \t]*
newline     [\n]
identif		(\042|\056|\055|\137|\046|{minus}|{majus}|{digit})+
identif_ren	({spec_carac}|{minus}|{majus}|{digit})+
r           	[(]{blank}
views       	(SYMBOLIC|symbolic|BEHAVIOR|behavior|MASKLAYOUT|masklayout)

%%

{blank}							{ }
{newline}						{ yylineno ++; }
{r}[eE][dD][iI][fF]					{ ECHO return T_EDIF ; }
{r}[eE][dD][iI][fF][vV][eE][rR][sS][iI][oO][nN]		{ ECHO return T_EDIFVERSION ; }
{r}[eE][dD][iI][fF][lL][eE][vV][eE][lL]			{ ECHO return T_EDIFLEVEL ; }
{r}[lL][iI][bB][rR][aA][rR][yY]				{ ECHO return T_LIBRARY ; }
{r}[cC][eE][lL][lL]					{ ECHO return T_CELL ; }
{r}[eE][xX][tT][eE][rR][nN][aA][lL]			{ ECHO return T_EXTERNAL ; }
{r}[cC][eE][lL][lL][tT][yY][pP][eE]			{ ECHO return T_CELLTYPE ; }
[gG][eE][nN][eE][rR][iI][cC]				{ ECHO return T_CELLTYPE_GENERIC ; }
[tT][iI][eE]						{ ECHO return T_CELLTYPE_TIE ; }
[rR][iI][pP][pP][eE][rR]				{ ECHO return T_CELLTYPE_RIPPER ; }
{r}[pP][rR][oO][pP][eE][rR][tT][yY]	 		{ ECHO return T_PROPERTY ; }
{r}[vV][iI][eE][wW]{blank}{identif}{blank}{r}[vV][iI][eE][wW][Tt][yY][pP][eE]{blank}{views}   {yyJumpOver(2);}
{r}[vV][iI][eE][wW]					{ ECHO return T_VIEW ; }
{r}[vV][iI][eE][wW][tT][yY][pP][eE]			{ ECHO return T_VIEWTYPE ; }
{r}[iI][nN][tT][eE][rR][fF][aA][cC][eE]			{ ECHO return T_INTERFACE ; }
{r}[cC][oO][nN][tT][eE][nN][tT][sS]			{ ECHO return T_CONTENTS ; }
{r}[nN][eE][tT]						{ ECHO return T_NET ; }
{r}[jJ][oO][iI][nN][eE][dD]				{ ECHO return T_JOINED ; }
{r}[iI][nN][sS][tT][aA][nN][cC][eE]			{ ECHO return T_INSTANCE ; }
{r}[pP][oO][rR][tT][iI][nN][sS][tT][aA][nN][cC][eE]	{ ECHO return T_PORTINSTANCE ; }
{r}[vV][iI][eE][wW][rR][eE][fF]				{ ECHO return T_VIEWREF ; }
{r}[cC][eE][lL][lL][rR][eE][fF]				{ ECHO return T_CELLREF ; }
{r}[lL][iI][bB][rR][aA][rR][yY][rR][eE][fF]		{ ECHO return T_LIBRARYREF ; }
{r}[pP][oO][rR][tT]					{ ECHO return T_PORT ; }
{r}[dD][iI][rR][eE][cC][tT][iI][oO][nN]			{ ECHO return T_DIRECTION ; }
[iI][nN][oO][uU][tT]					{ ECHO return T_INOUT ; }
[iI][nN][pP][uU][tT]					{ ECHO return T_INPUT; }
[oO][uU][tT][pP][uU][tT]				{ ECHO return T_OUTPUT ; }
{r}[pP][oO][rR][tT][rR][eE][fF]				{ ECHO return T_PORTREF ; }
{r}[pP][oO][rR][tT][lL][iI][sS][tT]			{ ECHO return T_PORTLIST ; }
{r}[iI][nN][sS][tT][aA][nN][cC][eE][rR][eE][fF]		{ ECHO return T_INSTANCEREF ; }
{r}[nN][aA][mM][eE]					{ ECHO return T_NAME ; }
{r}[mM][eE][mM][bB][eE][rR]				{ ECHO return T_MEMBER ;}
[nN][eE][tT][lL][iI][sS][tT]				{ ECHO return T_NETLIST ; }
{r}[rR][eE][nN][aA][mM][eE]				{ ECHO return T_RENAME ; }
{r}[aA][rR][rR][aA][yY]					{ ECHO return T_ARRAY ;}
{digit}+						{ ECHO sscanf(yytext, "%ld", &(yylval.lval));
		       						return T_INTEGER ; 
							}
{r}{identif}						{ yyJumpOver(1); }
\"{identif_ren}\"					{ ECHO return T_RENAME_STRING ; }
\"{identif}\"						{ ECHO return T_STRING ; }
{identif}						{ ECHO return T_IDENT; }
")"							{ ECHO return ')';}

%%

void yyJumpOver (nParenthesis)
int nParenthesis;
{
  char cRead;
  while(nParenthesis!=0) {
    if((cRead=input())==0) break;   
    switch(cRead) {
        case '\n': yylineno ++;     break;
        case '(' : nParenthesis ++; break;
        case ')' : nParenthesis --; break;
    }
  }
	if (nParenthesis > 0) puts("Not enought closing parentesis");
	else if(nParenthesis < 0) puts("Too many closing parentesis");
}
