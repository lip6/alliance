%{
#include "y.tab.h"
%}

car	[a-zA-Z_]
num	[0-9]
num_ex	[0-9a-f]
E	e[-+]?{num}+

%%

\.stabs		{	return SYMBL;}
\.stabd		{	return LIGNE_DIR;}
\.stabn		{	return BLOC;}

\.text		{	return TEXT;}
\.data		{	return DATA;}

\.align		{	return ALIGN;}
\.ascii		{	return ASCII;}
\.skip		{	return SKIP;}

\.lcomm		{	return STATIC;}
\.global		{	return GLOBL;}
\.comm		{	return COMM;}

\.char		{	return CHAR;}
\.short		{	return SHORT;}
\.int			{	return INT_DIR;}
\.float		{	return FLOAT;}
\.double		{	return DOUBLE;}
\.string		{	return STRING_DIR;}

\.file		{	return FILE_DIR;}
\.version	{	return VERSION;}

\"[^"]* 		{	if (yytext[yyleng -1]=='\\' && yytext[yyleng-2]!='\\')
#ifdef FLEX_SCANNER
						yymore();   /* not semi-colon, know that! */
#else
						yymore();
#endif
					else {
						yylval.str = (char *)mbkalloc(yyleng);
						strncpy(yylval.str, &(yytext[1]), yyleng-1);
						yylval.str[yyleng-1] = '\0';
						input();
						return STRING; 
					}
				}

\,				{	return VIRGULE;}
\#				{	return IMMEDIAT;}
\$				{	return DIRECT;}
\$\#			{	return DIRECT;}
\(				{	return IND_DEB;}
\)				{	return IND_FIN;}
\-				{	yylval.val = '-'; return SIGN;}
\+				{	yylval.val = '+'; return SIGN;}
\;.*			;

d{num}*		{	yylval.val = atoi(&(yytext[1])); return REGISTRE;}

{num}+		{	yylval.val = atoi(yytext); return NOMBRE;}

{num_ex}+	{	yylval.fl = atof(yytext); return FLOTTANT;}

{num}+"."{num}*({E})?	{	sscanf(yytext,"%lf",&yylval.fl); return FLOTTANT;};

{car}+({car}|{num}|\.)+	{	if (yylval.val = RechCodeInst(yytext))
										return CODE_OP;
									else {
										yylval.str = (char *)mbkalloc(yyleng+1);
										strcpy(yylval.str, yytext);
										return NAME;
									 }
								 }

\.({car}|{num})+	{	yylval.str = (char *)mbkalloc(yyleng+1);
							strcpy(yylval.str, yytext);
							return NAME;}

_({car}|{num})+\:	{	yylval.str = (char *)mbkalloc(yyleng);
							strncpy(yylval.str, yytext, yyleng-1);
							yylval.str[yyleng-1] = '\0';
							return ETIQ;}

\.({car}|{num})+\:	{	yylval.str = (char *)mbkalloc(yyleng);
								strncpy(yylval.str, yytext, yyleng-1);
								yylval.str[yyleng-1] = '\0';
								return ETIQ;}

{car}+({car}|{num}|\.)+\:	{	yylval.str = (char *)mbkalloc(yyleng);
										strncpy(yylval.str, yytext, yyleng-1);
										yylval.str[yyleng-1] = '\0';
										return ETIQ;}


[\n\t ]+	;

%%

yywrap()
{
	return 1;
}

yyerror(str)
char *str;
{
	printf("%s on %s\n",str,yytext);
	exit(-1);
}
