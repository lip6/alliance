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
#include <stdlib.h>
#include <string.h>
#include "iocheader.h"

extern int      yylineno;
extern char    *yytext;

extern int yyerror(char *str);
extern int yylex();

char con_orient = ' ';
int onvavoir = 0;
int top_used = 0;
int bottom_used = 0;
int right_used = 0;
int left_used = 0;
int l, l2;
char buf[256];
con_list *pt_list = NULL;


%}

%union {
char *text;
}

%token IGNORE TOP BOTTOM LEFT RIGHT IOPIN SPACE PAROUV PARFER PTVIRG PTZR
%token <text> IOCID
%start file

%%
file :
	| orientation PAROUV expr PARFER file
	;

orientation : 	  TOP {
			if (top_used == 0){
		   	 con_orient = 'T';
		   	 top_used = 1;
		  	} else yyerror("on ioc file : TOP declared twice");
		      }
		| BOTTOM {
			  if (bottom_used == 0){
			   con_orient = 'B';
		   	   bottom_used = 1;
		  	  } else yyerror("on ioc file : BOTTOM declared twice");
			}
		| RIGHT	{
			  if (right_used == 0){
			   con_orient = 'R';
		   	   right_used = 1;
		  	  } else yyerror("on ioc file : RIGHT declared twice");
			}
		| LEFT {
			if (left_used == 0){
			 con_orient = 'L';
		   	 left_used = 1;
		  	} else yyerror("on ioc file : LEFT declared twice");
		       }
		| IGNORE {con_orient = ' ';}
		;

expr : 
	| iopin expr
	| space expr
	;


iopin : PAROUV IOPIN iopin1 PARFER PTVIRG
      ;

space : SPACE IOCID PTVIRG { pt_list = add_space(pt_list, con_orient, $2);
}
      ;

iopin1 :   IOCID PTZR			 {
		  l = 0;
		  while ($1[l] != '.') l++;
		  strncpy(buf,$1,l);
		  buf[l] = '\0';
		  pt_list = add_con(pt_list, con_orient, buf);
		}
	| IOCID PAROUV IOCID PARFER PTZR {
		  l = 0;
		  while ($1[l] != '(') l++;
		  strncpy(buf,$1,l);
		  buf[l] = ' ';
		  l += 1;	
		  l2 = l; 
		  while ($1[l] != ')') l++;
		  strncpy(&buf[l2],&$1[l2],l-l2);
		  buf[l] = '\0';
		  pt_list = add_con(pt_list, con_orient, buf);
		}
	;
%%

int yyerror (char *str)
{
    fflush(stdout);
    fprintf(stderr, "error parsing the ioc file: %d: %s before %s\n", yylineno, str, yytext);
    exit(1);
}


con_list* add_con(con_list* ptlist, char orient, char* name)
{
  con_list* ptcon;
  ptcon = (con_list*) malloc(sizeof(con_list));

  ptcon->ORIENT = orient;
  ptcon->NAME = strdup(name);
  ptcon->NEXT = ptlist;

  return ptcon;
}

con_list* add_space(con_list* ptlist, char orient, char *value)
{
  con_list* ptcon;
  ptcon = (con_list*) malloc(sizeof(con_list));

  ptcon->NAME = strdup("SPACE");
  ptcon->ORIENT = orient;
  ptcon->VALUE = atoi(value);
  ptcon->NEXT = ptlist;

  return ptcon; 
}

con_list* iocparse(char *file)
{ 
  extern FILE *yyin;
  if ((yyin = fopen(file,"r")) != NULL) {
  yyparse();
  fclose(yyin);
 }
  else 
 {
   printf("error : ioc file not present\n");
   exit(1);
 }   
  return pt_list;
}
