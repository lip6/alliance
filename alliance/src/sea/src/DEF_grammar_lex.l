%{
/*
 * $Author: jpc $
 * $Id: DEF_grammar_lex.l,v 1.3 2011/02/06 15:14:31 jpc Exp $
 * $Date: 2011/02/06 15:14:31 $
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "util_Defs.h"
#include "DEF_grammar_yacc.h"


#define    YY_NO_UNPUT
#define    yylval       DEF_grammarlval
#define    yylineno     DEF_grammarlineno


#ifndef FLEX_BETA 
        int  yylineno = 1;
#endif


static  int  yywrap(void);
static  int  string(void);
static  int  history(void);

%}


L		[_A-Za-z]
D		[0-9]
S		[\-\+]
E		[eE]
X		[\.\-\<\>\[\]\(\)]

%%

[ \t\f]				{ }

\n				{ ++yylineno; }
#.*				{ }

"VERSION"			{ return T_VERSION; }
"NAMESCASESENSITIVE"		{ return T_NAMESCASESENSITIVE; }
"NAMEMAPSTRING"			{ return T_NAMEMAPSTRING; }
"HISTORY"			{ history (); }
"DESIGN"			{ return T_DESIGN; }
"VIAS"				{ return T_VIAS; }
"TECHNOLOGY"			{ return T_TECH; }
"UNITS"				{ return T_UNITS; }
"BUSBITCHARS"			{ return T_BUSBITCHARS; }
"DIVIDERCHAR"			{ return T_DIVIDERCHAR; }
"DISTANCE"			{ return T_DISTANCE; }
"MICRONS"			{ return T_MICRONS; }
"RECT"				{ return T_RECT; }
"REENTRANTPATHS"		{ return T_REENTRANTPATHS; }
"SITE"				{ return T_SITE; }
"CANPLACE"			{ return T_CANPLACE; }
"CANNOTOCCUPY"			{ return T_CANNOTOCCUPY; }
"DIEAREA"			{ return T_DIE_AREA; }
"PINS"				{ return T_PINS; }
"DEFAULTCAP"			{ return T_DEFAULTCAP; }
"MINPINS"			{ return T_MINPINS; }
"WIRECAP"			{ return T_WIRECAP; }
"TRACKS"			{ return T_TRACKS; }
"GCELLGRID"			{ return T_GCELLGRID; }
"DO"				{ return T_DO; }
"BY"				{ return T_BY; }
"STEP"				{ return T_STEP; }
"LAYER"				{ return T_LAYER; }
"COMPONENTS"			{ return T_COMPS; }
"GENERATE"			{ return T_COMP_GEN; }
"SOURCE"			{ return T_SOURCE; }
"WEIGHT"			{ return T_WEIGHT; }
"FIXED"				{ return T_FIXED; }
"COVER"				{ return T_COVER; }
"PLACED"			{ return T_PLACED; }
"UNPLACED"			{ return T_UNPLACED; }
"FOREIGN"			{ return T_FOREIGN; }
"EEQMASTER"			{ return T_EEQMASTER; }
"REGION"			{ return T_REGION; }
"REGIONS"			{ return T_REGIONS; }
"NETS"				{ return T_NETS; }
"MUSTJOIN"			{ return T_MUSTJOIN; }
"ORIGINAL"			{ return T_ORIGINAL; }
"USE"				{ return T_USE; }
"STYLE"				{ return T_STYLE; }
"PATTERN"			{ return T_PATTERN; }
"PATTERNNAME"			{ return T_PATTERNNAME; }
"NONDEFAULTRULE"		{ return T_NONDEFAULTRULE; }
"ESTCAP"			{ return T_ESTCAP; }
"ROUTED"			{ return T_ROUTED; }
"ROW"				{ return T_ROW; }
"TAPER"				{ return T_TAPER; }
"TAPERRULE"			{ return T_TAPERRULE; }
"NEW"				{ return T_NEW; }
"SHAPE"				{ return T_SHAPE; }
"SPECIALNETS"			{ return T_SNETS; }
"SPECIALNET"			{ return T_SNET; }
"WIDTH"				{ return T_WIDTH; }
"VOLTAGE"			{ return T_VOLTAGE; }
"SPACING"			{ return T_SPACING; }
"N"				{ return T_N; }
"S"				{ return T_S; }
"E"				{ return T_E; }
"W"				{ return T_W; }
"FN"				{ return T_FN; }
"FS"				{ return T_FS; }
"FE"				{ return T_FE; }
"FW"				{ return T_FW; }
"INPUT"				{ return T_INPUT; }
"OUTPUT"			{ return T_OUTPUT; }
"INOUT"				{ return T_INOUT; }
"FEEDTHRU"			{ return T_FEEDTHRU; }
"GROUPS"			{ return T_GROUPS; }
"GROUP"				{ return T_GROUP; }
"COMPONENT"			{ return T_COMPONENT; }
"SOFT"				{ return T_SOFT; }
"MAXX"				{ return T_MAXX; }
"MAXY"				{ return T_MAXY; }
"MAXHALFPERIMETER"		{ return T_MAXHALFPERIMETER; }
"CONSTRAINTS"			{ return T_CONSTRAINTS; }
"NET"				{ return T_NET; }
"PATH"				{ return T_PATH; }
"SUM"				{ return T_SUM; }
"DIFF"				{ return T_DIFF; }
"RISEMIN"			{ return T_RISEMIN; }
"RISEMAX"			{ return T_RISEMAX; }
"FALLMIN"			{ return T_FALLMIN; }
"FALLMAX"			{ return T_FALLMAX; }
"WIREDLOGIC"			{ return T_WIREDLOGIC; }
"MAXDIST"			{ return T_MAXDIST; }
"ASSERTIONS"			{ return T_ASSERTIONS; }
"END"				{ return T_END; }
"ARRAY"				{ return T_ARRAY; }
"FLOORPLAN"			{ return T_FLOORPLAN; }
"SCANCHAINS"			{ return T_SCANCHAINS; }
"START"				{ return T_START; }
"FLOATING"			{ return T_FLOATING; }
"ORDERED"			{ return T_ORDERED; }
"STOP"				{ return T_STOP; }
"TIMINGDISABLES"		{ return T_TIMINGDISABLES; }
"IOTIMINGS"			{ return T_IOTIMINGS; }
"RISE"				{ return T_RISE; }
"FALL"				{ return T_FALL; }
"VARIABLE"			{ return T_VARIABLE; }
"SLEWRATE"			{ return T_SLEWRATE; }
"CAPACITANCE"			{ return T_CAPACITANCE; }
"DRIVECELL"			{ return T_DRIVECELL; }
"FROMPIN"			{ return T_FROMPIN; }
"TOPIN"				{ return T_TOPIN; }
"PARALLEL"			{ return T_PARALLEL; }
"PARTITIONS"			{ return T_PARTITIONS; }
"TURNOFF"			{ return T_TURNOFF; }
"FROMCLOCKPIN"			{ return T_FROMCLOCKPIN; }
"FROMCOMPPIN"			{ return T_FROMCOMPPIN; }
"FROMIOPIN"			{ return T_FROMIOPIN; }
"TOCLOCKPIN"			{ return T_TOCLOCKPIN; }
"TOCOMPPIN"			{ return T_TOCOMPPIN; }
"TOIOPIN"			{ return T_TOIOPIN; }
"SETUPRISE"			{ return T_SETUPRISE; }
"SETUPFALL"			{ return T_SETUPFALL; }
"HOLDRISE"			{ return T_HOLDRISE; }
"HOLDFALL"			{ return T_HOLDFALL; }
"VPIN"				{ return T_VPIN; }
"SUBNET"			{ return T_SUBNET; }
"XTALK"				{ return T_XTALK; }
"PIN"				{ return T_PIN; }
"SYNTHESIZED"			{ return T_SYNTHESIZED; }
"SPECIAL"			{ return T_SPECIAL; }
"DIRECTION"			{ return T_DIRECTION; }
"RANGE"				{ return T_RANGE; }
"FLOORPLANCONSTRAINTS"		{ return T_FPC; }
"HORIZONTAL"			{ return T_HORIZONTAL; }
"VERTICAL"			{ return T_VERTICAL; }
"ALIGN"				{ return T_ALIGN; }
"MIN"				{ return T_MIN; }
"MAX"				{ return T_MAX; }
"EQUAL"				{ return T_EQUAL; }
"BOTTOMLEFT"			{ return T_BOTTOMLEFT; }
"TOPRIGHT"			{ return T_TOPRIGHT; }
"ROWS"				{ return T_ROWS; }
"PROPERTYDEFINITIONS"		{ return T_PROPERTYDEFINITIONS; }
"PROPERTY"			{ return T_PROPERTY; }
"REAL"				{ return T_REAL; }
"INTEGER"			{ return T_INTEGER; }
"BEGINEXT"			{ return T_BEGINEXT; }
"PINPROPERTIES"			{ return T_PINPROPERTIES; }
"COMMONSCANPINS"		{ return T_COMMONSCANPINS; }
"COMPONENTPIN"			{ return T_COMPONENTPIN; }
"SHIELD"			{ return T_SHIELD; }
"SHIELDNET"			{ return T_SHIELDNET; }
"NOSHIELD"			{ return T_NOSHIELD; }

{L}({L}|{D}|{X})*		{ yylval.s = strdup(yytext); return T_Ident; }
{S}?{D}+			{ yylval.n = atof(yytext); return T_Number; }
{S}?{D}+\.{D}*			{ yylval.n = atof(yytext); return T_Number; }
{S}?{D}*\.{D}+			{ yylval.n = atof(yytext); return T_Number; }
{S}?{D}+{E}{S}?{D}+		{ yylval.n = atof(yytext); return T_Number; }
{S}?{D}+\.{D}*{E}{S}?{D}+	{ yylval.n = atof(yytext); return T_Number; }
{S}?{D}*\.{D}+{E}{S}?{D}+	{ yylval.n = atof(yytext); return T_Number; }

"\""            		{ return string(); }
.				{ return *yytext; }

%%

static int yywrap() { return (1); }

static int
string()
{
    char     *str;
    size_t   max;
    int      c;
    size_t   len;

    if ((str = (char *)malloc(max = 64)) == NULL)
	{ perror("string"); exit (1); }

    len = 0;
    while ((c = input()) >= 0 && c != '"') {
	if (len + 1 == max)
	    if ((str = (char *)realloc(str, max *= 2)) == NULL)
		{ perror ("string"); exit (1); }
	str[len++] = (char)c;
	if (c == '\n') yylineno++;
    }
    str[len] = '\0';
    if (c == EOF) {
	fflush (stdout);
	fprintf (stderr, "unterminated string constant");
	exit (1);
    }
    yylval.s = str;
    return T_String;
}


static int
history()
{
    char     *str;
    size_t   max;
    int      c;
    size_t   len;

    if ((str = (char *)malloc(max = 64)) == NULL)
	{ perror("history"); exit (1); }

    len = 0;
    while ((c = input()) >= 0 && c != ';') {
	if (len + 1 == max)
	    if ((str = (char *)realloc(str, max *= 2)) == NULL)
		{ perror ("history"); exit (1); }
	str[len++] = (char)c;
	if (c == '\n') yylineno++;
    }
    str[len] = '\0';
    if (c == EOF) {
	fflush (stdout);
	fprintf (stderr, "unterminated history line");
	exit (1);
    }
    yylval.s = str;
    return T_HISTORY;
}
