/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
