%{
#include <stdio.h>
#include "iocgram.h"

#define YY_NO_UNPUT	/* Avoids warning */

int	 yylineno = 1;

%}

%%
#.*				{}
\n				{ ++yylineno; }
[Ii][Gg][Nn][Oo][Rr][Ee]	{return IGNORE;	}
[Tt][Oo][Pp]			{return TOP;	}
[Bb][Oo][Tt][Tt][Oo][Mm]	{return BOTTOM;	}
[Ll][Ee][Ff][Tt]		{return LEFT;	}
[Rr][Ii][Gg][Hh][Tt]		{return RIGHT;	}
[Ii][Oo][Pp][Ii][Nn]		{return IOPIN;	}
[Ss][Pp][Aa][Cc][Ee]		{return SPACE;	}
"("				{return PAROUV;	}
")"				{return PARFER;	}
;				{return PTVIRG;	}
"\.0"				{return PTZR;	}
[a-zA-Z0-9_]+			{yylval.text = yytext;
				 return IOCID;	}
[ \t]+				{}
.				{return *yytext;}

%%
int yywrap()
{
  return 1;
}
