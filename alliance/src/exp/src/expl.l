%{
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "exp.h"
#include "expy.h"

extern int lineno;
#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
int lineno_stack[MAX_INCLUDE_DEPTH];
char * fname_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

%}
localcomment [ \t]*\/\/.*\n
comment      ([ \t]*[#].*)
regex        (\'[^'\n#]+\')
ident        ([a-zA-Z][a-zA-Z0-9_]*)|(\"[^"\n#]+\")
number       ([0-9]+|([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?
blank        [ \t]+
if           [iI][fF]  
min          [mM][iI][nN]
max          [mM][aA][xX]
inf          [iI][nN][fF]  
sup          [sS][uU][pP]  
sort         [sS][oO][rR][tT]
rsort        [rR][sS][oO][rR][tT]
def          [Dd][Ee][Ff] 
ndef         [Nn][Dd][Ee][Ff] 
include      [Ii][Nn][Cc][Ll][Uu][Dd][Ee]
message      [Mm][Ee][Ss][Ss][Aa][Gg][Ee]
%x EXP INC
%%
{localcomment}     {lineno++;}
[\[\{]             {BEGIN(EXP);return *yytext;}
<EXP>[\]}]         {BEGIN(INITIAL);return *yytext;}

<INITIAL>\n        {lineno++;ECHO;}
<INITIAL>{comment} {ECHO;}
<INITIAL>{include} {BEGIN(INC);}
<INC>{ident}       { /* got the include file name */
                     char * filename = yytext;
                     if (include_stack_ptr >= MAX_INCLUDE_DEPTH) {
                       fprintf (stderr, " %-3d %s : Includes nested too deeply\n", lineno, expfname);
                       exit (1);
                     }
                     if (*filename == '"') {
                       *(filename+yyleng-1)='\0';
                       filename +=1;
                     }
                     if ((yyin = fopen( filename, "r" )) == NULL) {
                       fprintf (stderr, " %-3d %s : %s cannot be opened\n",
                       lineno, expfname, filename);
                       exit (1);
                     }
                     include_stack[include_stack_ptr] = YY_CURRENT_BUFFER;
                     lineno_stack[include_stack_ptr] = lineno;
                     fname_stack[include_stack_ptr++] = expfname;
                     if ((expfname = strdup (filename)) == NULL) {
                       fprintf (stderr, " %-3d %s : not enough memory\n", lineno, expfname, filename);
                       exit (1);
                     }   
                     lineno = 1;
                     yy_switch_to_buffer( yy_create_buffer( yyin, YY_BUF_SIZE ) );
                     BEGIN(INITIAL);
                   }
<INITIAL><<EOF>>   { if ( --include_stack_ptr < 0 )
                       yyterminate();
                     else {
                       yy_delete_buffer (YY_CURRENT_BUFFER );
                       yy_switch_to_buffer (include_stack[include_stack_ptr] );
                       expfname = fname_stack[include_stack_ptr];
                       lineno = lineno_stack[include_stack_ptr];
                     }
                   }
<EXP>\n            {lineno++;}
<EXP>{comment}     {}
<EXP>{if}          {return IF;}
<EXP>{min}         {return MIN;}
<EXP>{max}         {return MAX;}
<EXP>{inf}         {return INF;}
<EXP>{sup}         {return SUP;}
<EXP>{def}         {return DEF;}
<EXP>{ndef}        {return NDEF;}
<EXP>{sort}        {return SORT;}
<EXP>{rsort}       {return RSORT;}
<EXP>{message}     {return MESS;}
<EXP>{regex}       { 
                     *(yytext+yyleng-1)='\0';
                     yylval.e=htsetre(dico,yytext+1); 
                     return REGEX;
                   }
<EXP>{ident}       { if (dico==NULL) dico=htinit(10000);
                     if (*yytext == '"') {
                       *(yytext+yyleng-1)='\0';
                       yylval.e=htset(dico,yytext+1); 
                     }
                     else
                       yylval.e=htset(dico,yytext); 
                     return IDENT;
                   }
<EXP>{number}      {yylval.f=atof(yytext); return NUMBER;}
<EXP>"++"          {return PP;}
<EXP>"--"          {return MM;}
<EXP,INC>{blank}   {}
<EXP>.             {return *yytext;}
.                  {ECHO;}
%%

