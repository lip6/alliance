%{
#include "ringram.h"
#include "struct.h"
#undef yywrap
%}
delim [ \n\t]
esp   [ \t]
rc    [\n]
pct   [\.\,\;\:\'\"\?\/\~\`\@\^<>\!\&\*\(\)\-\_\=\+\{\}\[\]\$\%\\\|]
blanc {delim}+
chiffre [0-9]
nombre {chiffre}+
lettre [a-zA-Z_]
ident {lettre}({lettre}*{chiffre}*)*
comment [#]({lettre}*{chiffre}*{esp}*{pct}*)*{rc}
%%
{blanc} {if (mode_debug) ECHO;}
"north" {if (mode_debug) ECHO; return(M_NORTH);}
"south" {if (mode_debug) ECHO; return(M_SOUTH);}
"west"  {if (mode_debug) ECHO; return(M_WEST) ;}
"east"  {if (mode_debug) ECHO; return(M_EAST) ;}
"width" {if (mode_debug) ECHO; return(M_WIDTH);}
{nombre} {if (mode_debug) ECHO; sscanf(yytext,"%ld",&yylval.i);
					 return(NOMBRE);
					} 				 
{ident} {if (mode_debug) ECHO; yylval.s = namealloc(yytext);
				return(IDENT);
				}
{comment} {if (mode_debug) {ECHO; printf("commentaire\n");}}
.				{ return(*yytext);}
%%
int yywrap()
	{return(1);}

