%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exp.h"
#include "ht.h"

#define VERBOSE        (int)((htget(dico, "verbose")->VAL.f+EPSILON))
#define EPSILON        0.00001
#define anint(x)       floor((x) + 0.5) 
#define test_init(i)   if (((i)->TYPE==0) && ((pcond==0) || (cond[pcond]>0.))) {\
                         printf(" %-3d %s : <%s> undefined\n", lineno, expfname, (i)->KEY);\
                         exit (1);}\
                       (i)->USED = 1
#define pa(v,e) if (VERBOSE>2) printf("%7.3f -> %s\n",e,v->KEY)
#define pi(e)   if (VERBOSE>2) printf("%7.3f <- %s\n",e->VAL.f,e->KEY)
#define pe(s,n) if (VERBOSE>2) printf("%7.3f %s\n",n,s)
#define PCONDMAX 16 /* profondeur de la pile de conditions */

int lineno=1;
char * expfname;
ht_t * dico;

static int pcond=0;
static double cond[PCONDMAX]; /* pile de conditions : +1:vrai, -1:faux */
static char * string_fmt = "%-65s";
static char * float_fmt = "%7.3f";
static char * integer_fmt = "%4d";
static htelt_t * ele;

double limit (int fun, htelt_t * args)
{
  htelt_t *ele;
  double res;
  for (ele = args; ele && ele->TYPE == 0; ele = ele->NEXT);
  if (ele == NULL)
  {  
    printf(" %-3d %s : none variable defined\n", lineno, expfname);\
    exit (1);
  }
  res = ele->VAL.f;
  for (; ele; ele = ele->NEXT)
    if ((ele->TYPE != 0) && (((fun == MAX) && (res < ele->VAL.f)) || ((fun == MIN) && (res > ele->VAL.f))))
      res = ele->VAL.f;
  eltremove (args);
  return res;
}

htelt_t *bsort (int sens, htelt_t * args)
{
  htelt_t *ele;
  int bubble = 1;
  if (args->NEXT) /* au moins de 2 elements dans la liste */
    while (bubble)
    {
      bubble = 0;
      for (ele = args; ele->NEXT; ele = ele->NEXT)
        if (((sens == SORT) && (((ele->VAL.e)->VAL.f) > (((ele->NEXT)->VAL.e)->VAL.f)))
        || ((sens == RSORT) && (((ele->VAL.e)->VAL.f) < (((ele->NEXT)->VAL.e)->VAL.f)))
        || ((sens == MESS) && (strcmp((ele->VAL.e)->KEY,((ele->NEXT)->VAL.e)->KEY)>0)))
        {
          htelt_t *tmp = ele->VAL.e;
          ele->VAL.e = (ele->NEXT)->VAL.e;
          (ele->NEXT)->VAL.e = tmp;
          bubble = 1;
        }
    }    
  return args;
}

htelt_t *mess (int type, char *fmt, htelt_t * args)
{
  htelt_t *ele;
  if (VERBOSE)
  {
    for (ele = args; ele; ele = ele->NEXT)
    {
      if ((type == MESS) || ((ele->VAL.e)->TYPE == HT_FLOAT))
      {
        printf (string_fmt, (ele->VAL.e)->KEY);
        if ((ele->VAL.e)->TYPE == HT_FLOAT)
        {
          printf (" = ");
          printf (fmt, (ele->VAL.e)->VAL.f);
        }
        printf ("\n");
      }
    }
  }
  eltremove (args);
}

%}
%union
{
  htelt_t * e;
  double    f;
}
%token IF PP MM MAX MIN SUP INF SORT RSORT MESS DEF NDEF
%token <f>NUMBER
%token <e>IDENT REGEX
%type  <f>exp cond
%type  <e>args idents
%right '='
%right '<' '>'
%left  '+' '-'
%left  '*' '/'
%%
blocs  : /* rien */                   { if (VERBOSE>1) htstat(dico); return 1;}   
       | bloc blocs                     
       ;                                 
bloc   : '[' exps exp ']'             { fprintf (yyout, float_fmt, $3);}
       | '{' exps exp '}'             { fprintf (yyout, integer_fmt, (int)$3);}
       | '[' exp ']'                  { fprintf (yyout, float_fmt, $2);}
       | '{' exp '}'                  { fprintf (yyout, integer_fmt, (int)$2);}
       | '[' SORT '(' idents ')'  ']' { mess(SORT,float_fmt, bsort (SORT, $4));}
       | '{' SORT '(' idents ')'  '}' { mess(SORT,integer_fmt, bsort (SORT, $4));}
       | '[' RSORT '(' idents ')' ']' { mess(SORT,float_fmt, bsort (RSORT, $4));}
       | '{' RSORT '(' idents ')' '}' { mess(SORT,integer_fmt, bsort (RSORT, $4));}
       | '[' MESS '(' idents ')'  ']' { mess(MESS,float_fmt, bsort (MESS, $4));}
       | '{' MESS '(' idents ')'  '}' { mess(MESS,integer_fmt, bsort (MESS, $4));}
       | '[' exps ']'                 { }
       | '{' exps '}'                 { }
       ;                                
exps   : exp ';'                      { }
       | exps  exp ';'                { }
       ;                                
exp    : IDENT '=' exp                { if ((pcond==0) || (cond[pcond]>0.)) {
                                          $1->VAL.f=$3; $1->TYPE=HT_FLOAT; 
                                          $1->FNAME=expfname; $1->FLINE=lineno; 
                                          $$=$3; pa($1,$$);
                                        }
                                      }
       | NUMBER                       { $$ = $1;  pe(" ",$$);}
       | IDENT                        { test_init($1); $$ = $1->VAL.f; pi($1);}  
       | IDENT PP                     { test_init($1); $$ = $1->VAL.f; $1->VAL.f += 1.0;}
       | IDENT MM                     { test_init($1); $$ = $1->VAL.f; $1->VAL.f -= 1.0;}
       | exp '<' exp                  { $$ = ($1 < $3) ? 1. : -1.; pe(" < ",$$);}
       | exp '>' exp                  { $$ = ($1 > $3) ? 1. : -1.; pe(" > ",$$);}
       | exp '+' exp                  { $$ = $1 + $3; pe(" + ",$$);}
       | exp '-' exp                  { $$ = $1 - $3; pe(" - ",$$);}
       | exp '*' exp                  { $$ = $1 * $3; pe(" * ",$$);}
       | exp '/' exp                  { $$ = $1 / $3; pe(" / ",$$);}
       | '(' exp ')'                  { $$ = $2;}
       | '-' exp                      { $$ = -$2; pe(" - ",$$);}
       | IF '(' cond ',' args ')'     { for (ele = $5; ele->NEXT; ele = ele->NEXT);
                                        $$ = ele->VAL.f;
                                        pcond--; eltremove ($5); 
                                      }
       | SUP '(' exp ',' exp ')'      { $$ = anint(($5+($3/2)-EPSILON)/$3)*$3; pe(" sup ",$$);}
       | INF '(' exp ',' exp ')'      { $$ = ((int)(floor(($5+EPSILON)/$3)))*$3; pe(" inf ",$$);}
       | MAX '(' args ')'             { $$ = limit (MAX, $3); pe(" max ",$$);}
       | MIN '(' args ')'             { $$ = limit (MIN, $3); pe(" min ",$$);}
       | DEF '(' IDENT ')'            { $$ = ($3->TYPE==0) ? -1. : +1.; pe($3,$$);}
       | NDEF '(' IDENT ')'           { $$ = ($3->TYPE==0) ? +1. : -1.; pe($3,$$);}
       ;                                
cond   : exp                          { if (pcond < PCONDMAX) 
                                          cond[++pcond] = $1; 
                                        else
                                          yyerror ("too much conditions");
                                      }
       ;
idents : IDENT                        { $$ = eltadd (NULL,NULL); $$->VAL.e=$1;}  
       | IDENT ',' idents             { $$ = eltadd ($3,NULL); $$->VAL.e=$1;}  
       | REGEX                        { $$ = $1->NEXT; $1->NEXT=NULL;}  
       | REGEX ',' idents             { $$ = $1->NEXT; $1->NEXT=$3;}  
       ;                                
args   : exp                          { $$ = eltadd (NULL,NULL); $$->VAL.f=$1; $$->TYPE=HT_FLOAT;pe(" , ",$1);}  
       | exp ',' args                 { $$ = eltadd ($3,NULL); $$->VAL.f=$1; $$->TYPE=HT_FLOAT;pe(" , ",$3);}  
       | REGEX                        { $$ = $1->NEXT; $1->NEXT=NULL; 
                                        for (ele = $$; ele; ele = ele->NEXT)
                                        {  
                                          ele->VAL.e->USED = 1;
                                          ele->TYPE = (ele->VAL.e)->TYPE;
                                          ele->VAL.f = (ele->VAL.e)->VAL.f;
                                        }  
                                      }
       | REGEX ',' args               { $$ = $1->NEXT; $1->NEXT=$3;   
                                        for (ele = $1; ele; ele = ele->NEXT)
                                        {
                                          ele->VAL.e->USED = 1;
                                          ele->TYPE = (ele->VAL.e)->TYPE;
                                          ele->VAL.f = ele->VAL.e->VAL.f;
                                        }  
                                      }
       ;
%%
int yyerror(char *s)
{
  printf(" %-3d %s : %s\n", lineno, expfname, s);
  exit (1);
}
