%option noinput
%option nounput

%{
#include <ctype.h>
#include "mut.h"
#include "rds.h"
#include "drucompi_y.h"
#include "drucutil.h"
#include "drucompi.h"
#include "vmctools.h"
 
#ifdef FLEX_SCANNER

#ifdef YY_INPUT
#undef YY_INPUT
#endif

#define YY_INPUT(buf,result,max_size) \
        do { \
        int c = getc(yyin); \
           result = (c == EOF) ? YY_NULL \
           : (buf[0] = isupper(c) ? toupper(c) : c, 1); \
        } while(0)

int yylineno;

#else /* using lex, for sure */

#ifdef input
#undef input
#endif

#define input() (((yytchar = yysptr > yysbuf ? U(*--yysptr)    \
                   : getc(yyin)) == 10 ?(yylineno++,yytchar)   \
                      : yytchar) == EOF ? 0                    \
                         : isupper(yytchar) ? toupper(yytchar) \
                           : yytchar)
#endif

void DrucEcho ( char *text )
{
  if ( DrucStructStat.FLAG_VIEW_RULE_MODE == DRUC_VERBOSE )
  {
    DrucViewString ( text );
  }
}


%}

%p 3000
%option yylineno

delim [ \n\t]
esp   [ \t]
rc    [\n]
pct   [\.\,\;\:\'\"\?\/\~\`\@\^<>\!\&\*\(\)\-\_\=\+\{\}\[\]\$\%\\\|]
blanc {delim}+

lettre      [A-Za-z]
ident       {lettre}({lettre}*|{dec}*|[_]*)*
dec         [0-9]
decimal     {dec}+
reel        {dec}*([\.]{dec}*)
negat       [\-]{reel}
comment [#]({lettre}*{dec}*{esp}*{pct}*)*{rc}

%%


{blanc}                                          {
                                                   DrucEcho ( yytext );
                                                 }
layer                                            {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LAYER;
                                                 }
regle|rule                                       {
                                                   DrucEcho ( yytext );
                                                   return DRUC_REGLE;
                                                 }
regles|rules                                     {
                                                   DrucEcho ( yytext );
                                                   return DRUC_REGLES;
                                                 }
fin|end                                          {
                                                   DrucEcho ( yytext );
                                                   return DRUC_FIN;
                                                 }
define                                           {
                                                   DrucEcho ( yytext );
                                                   return DRUC_DEFINE;
                                                 }
undefine                                         {
                                                   DrucEcho ( yytext );
                                                   return DRUC_UNDEFINE;
                                                 }
caracterise|characterize                         {
                                                   DrucEcho ( yytext );
                                                   return DRUC_CARACTERISE;
                                                 }
relation                                         {
                                                   DrucEcho ( yytext );
                                                   return DRUC_RELATION;
                                                 }
                                                 
resize                                           {
                                                   DrucEcho ( yytext );
                                                   return DRUC_RESIZE;
                                                 }
exclusion                                        {
                                                   DrucEcho ( yytext );
                                                   return DRUC_EXCLUSION;
                                                 }
union                                            {
                                                   DrucEcho ( yytext );
                                                   return DRUC_UNION;
                                                 }
enveloppe|envelope                               {
                                                   DrucEcho ( yytext );
                                                   return DRUC_ENVELOPPE;
                                                 }
marge|margin                                     {
                                                   DrucEcho ( yytext );
                                                   return DRUC_MARGE;
                                                 }
croix|cross                                      {
                                                   DrucEcho ( yytext );
                                                   return DRUC_CROIX;
                                                 }
intersection                                     {
                                                   DrucEcho ( yytext );
                                                   return DRUC_INTERSECT;
                                                 }
extension                                        {
                                                   DrucEcho ( yytext );
                                                   return DRUC_EXTENSION;
                                                 }
inclusion                                        {
                                                   DrucEcho ( yytext );
                                                   return DRUC_INCLUSION;
                                                 }
distance|spacing                                 {
                                                   DrucEcho ( yytext );
                                                   return DRUC_DISTANCE;
                                                 }
surface_inter|intersection_area                  {
                                                   DrucEcho ( yytext );
                                                   return DRUC_SURFACE_INTER;
                                                 }
longueur_inter|intersection_length               {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LONG_INTER;
                                                 }
largeur_inter|intersection_width                 {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LARGE_INTER;
                                                 }
penetre_inter|inclusion_intersection             {
                                                   DrucEcho ( yytext );
                                                   return DRUC_PENET_INTER;
                                                 }
parallele_inter|parallel_intersection            {
                                                   DrucEcho ( yytext );
                                                   return DRUC_PARAL_INTER;
                                                 }
perpendiculaire_inter|perpendicular_intersection {
                                                   DrucEcho ( yytext );
                                                   return DRUC_PERPEN_INTER;
                                                 }
axiale|axial                                     {
                                                   DrucEcho ( yytext );
                                                   return DRUC_AXIALE;
                                                 }
geodesique|geodetic                              {
                                                   DrucEcho ( yytext );
                                                   return DRUC_GEODESIQUE;
                                                 }
frontale                                         {
                                                   DrucEcho ( yytext );
                                                   return DRUC_FRONTALE;
                                                 }
laterale_min|min_sideways                        {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LATERALE_MIN;
                                                 }
laterale_max|max_sideways                        {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LATERALE_MAX;
                                                 }
longueur_min|min_length                          {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LONGUEUR_MIN;
                                                 }
longueur_max|max_length                          {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LONGUEUR_MAX;
                                                 }
largeur_min|min_width                            {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LARGEUR_MIN;
                                                 }
largeur_max|max_width                            {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LARGEUR_MAX;
                                                 }
superieure|superior                              {
                                                   DrucEcho ( yytext );
                                                   return DRUC_SUP;
                                                 }
inferieure|inferior                              {
                                                   DrucEcho ( yytext );
                                                   return DRUC_INF;
                                                 }
surface|area                                     {
                                                   DrucEcho ( yytext );
                                                   return DRUC_SURFACE;
                                                 }
longueur|length                                  {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LONGUEUR;
                                                 }
largeur|width                                    {
                                                   DrucEcho ( yytext );
                                                   return DRUC_LARGEUR;
                                                 }
notch                                            {
                                                   DrucEcho ( yytext );
                                                   return DRUC_NOTCH;
                                                 }
min|">\:"                                        {
                                                   DrucEcho ( yytext );
                                                   return DRUC_MIN;
                                                 }
max|"<\:"                                        {
                                                   DrucEcho ( yytext );
                                                   return DRUC_MAX;
                                                 }
"<="                                             {
                                                   DrucEcho ( yytext );
                                                   return DRUC_INFEQ;
                                                 }
">="                                             {
                                                   DrucEcho ( yytext );
                                                   return DRUC_SUPEQ;
                                                 }
"->"                                             {
                                                   DrucEcho ( yytext );
                                                   return DRUC_DESTINATION;
                                                 }
{decimal}                                        {
                                                   DrucEcho ( yytext );
                                                   sscanf( yytext,"%d", &DrucInputEntier );
                                                   return DRUC_ENTIER;
                                                 }
{reel}                                           {
                                                   DrucEcho ( yytext );
                                                   sscanf( yytext,"%f", &DrucInputFloat );
                                                   return DRUC_FLOAT;
                                                 }
{negat}                                          {
                                                   DrucEcho ( yytext );
                                                   sscanf( yytext,"%f", &DrucInputFloat );
                                                   return DRUC_NEG_FLOAT;
                                                 }
{ident}                                          {
                                                   DrucEcho ( yytext );
                                                   sscanf( yytext,"%s",DrucInputChaine );
                                                   return DRUC_NOM;
                                                 }
{comment}                                        {
                                                   DrucEcho ( yytext );
                                                 }
.                                                {
                                                   DrucEcho ( yytext );
                                                   return *yytext;
                                                 }

%%

int yywrap( void )
{
  return 1;
}
