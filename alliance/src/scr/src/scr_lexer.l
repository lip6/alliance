%{
#include <stdio.h>
#include "scr_grammar.h"
#include <mut.h>
extern int tpllineno ;
%}

%%
[Pp][Ll][Aa][Cc][Ee]_[Pp][Hh][Cc][Oo][Nn]_[Nn][Oo][Rr][Tt][Hh] { 
					return (M_PLACE_PHCON_NORTH);}
[Pp][Ll][Aa][Cc][Ee]_[Pp][Hh][Cc][Oo][Nn]_[Ss][Oo][Uu][Tt][Hh] { 
					return (M_PLACE_PHCON_SOUTH);}
[Pp][Ll][Aa][Cc][Ee]_[Pp][Hh][Cc][Oo][Nn]_[Ww][Ee][Ss][Tt] { 
					return (M_PLACE_PHCON_WEST);}
[Pp][Ll][Aa][Cc][Ee]_[Pp][Hh][Cc][Oo][Nn]_[Ee][Aa][Ss][Tt] { 
					return (M_PLACE_PHCON_EAST);}
[Vv][Ee][Rr]_[Ff][Ee][Ee][Dd]_[Tt][Hh][Rr][Oo][Uu][Gg][Hh] { 
					return (M_VER_FEED_THROUGH);}
[Hh][Oo][Rr]_[Ff][Ee][Ee][Dd]_[Tt][Hh][Rr][Oo][Uu][Gg][Hh] { 
					return (M_HOR_FEED_THROUGH);}
[0-9\-]+					{
                                        sscanf(yytext,"%d",&(yylval.valu));
					return(Number);
                                        }

[a-zA-Z][a-zA-Z0-9\._]*			{
					yylval.text = namealloc (yytext);
					return(Identifier);
					}
\[					{ return(Lcro); }
\]					{ return(Rcro); }
\(					{ return(Lparen); }
\)					{ return(Rparen); }
\,					{ return (Coma); }
\;					{ return (Semicolon); }
:					{ return (Colon); }
[ \t]					;
\n					{tpllineno++;}
#.*$					;
%%
