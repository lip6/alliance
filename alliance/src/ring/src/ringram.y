
%{
#include <stdlib.h> 
#include "struct.h"

extern  void  yyrestart ( );
extern  int   yyparse   ( );
extern  int   yylex     ( );
extern  void  yyerror   ( );
%}
%token M_NORTH  M_SOUTH  M_WEST  M_EAST  M_WIDTH
%token NOMBRE
%token IDENT 
%union {
	long i;
	char *s;
	}
%type <i> NOMBRE
%type <s> IDENT
%start programme
%%
programme: ldirective {if (mode_debug) printf("yacc: programme \n");}
	;
ldirective: ldirective directive {if (mode_debug) printf("yacc: ldirective \n");}
	| directive
	;
directive: M_NORTH '(' lnom_n ')' {if (mode_debug) printf("yacc: north \n");}
  | M_NORTH '(' ')'               {if (mode_debug) printf("yacc: north \n");}
	| M_SOUTH '(' lnom_s ')' {if (mode_debug) printf("yacc: south \n");}
	| M_SOUTH '(' ')'        {if (mode_debug) printf("yacc: south \n");}
	| M_WEST  '(' lnom_w ')' {if (mode_debug) printf("yacc: west  \n");}
	| M_WEST  '('  ')'       {if (mode_debug) printf("yacc: west  \n");}
	| M_EAST  '(' lnom_e ')' {if (mode_debug) printf("yacc: east  \n");}
	| M_EAST  '('  ')'       {if (mode_debug) printf("yacc: east  \n");}
  | M_WIDTH '(' lnomchiffre ')'  {if (mode_debug) printf("yacc: width \n");}
	| M_WIDTH '('  ')'             {if (mode_debug) printf("yacc: width \n");}
  ;
lnom_n: lnom_n  IDENT { declaration_plot_n($2);if (mode_debug) printf("yacc: ident \n"); }
	| IDENT { declaration_plot_n($1);if (mode_debug) printf("yacc: ident \n");}
	;
lnom_s: lnom_s IDENT { declaration_plot_s($2); if (mode_debug) printf("yacc: ident \n");}
	| IDENT { declaration_plot_s($1);if (mode_debug) printf("yacc: ident \n");}
	;
lnom_w: lnom_w IDENT { declaration_plot_w($2);if (mode_debug) printf("yacc: ident \n");}
	| IDENT { declaration_plot_w($1);if (mode_debug) printf("yacc: ident \n");}
	;
lnom_e: lnom_e IDENT { declaration_plot_e($2);if (mode_debug) printf("yacc: ident \n");}
	| IDENT { declaration_plot_e($1);if (mode_debug) printf("yacc: ident \n");}
	;
lnomchiffre: lnomchiffre IDENT NOMBRE { declaration_width($2,$3);if (mode_debug) printf("yacc lnomchiffre \n");}
	| IDENT NOMBRE { declaration_width($1,$2);if (mode_debug) printf("yacc lnomchiffre \n");}
	;

