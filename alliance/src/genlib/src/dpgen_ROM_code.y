/*
 * This file has been stolen from the old full custom ROM generator : Grog...
 * Courtesy of Mr Fred Petrot.
 *
 */

%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mut.h>

typedef struct vhdl_constant {
		struct vhdl_constant *NEXT;
		char                 *string;
		char                 *value;
} dptr;

static char oct2bin[8][4] = {
		"000",
		"001",
		"010",
		"011",
		"100",
		"101",
		"110",
		"111"
};

static char hex2bin[16][5] = {
		"0000",
		"0001",
		"0010",
		"0011",
		"0100",
		"0101",
		"0110",
		"0111",
		"1000",
		"1001",
		"1010",
		"1011",
		"1100",
		"1101",
		"1110",
		"1111"
};

static char *vhdl_line;
static dptr *asc = NULL;
static int line = 0;
static int notempty = 0;
static char datbuf[65];
static char adrbuf[33];
static long nwords, nbits, rf, ibmbull;
static long *ydata;
static long used[8912];
static char aorder; /* vector order for addresses */
static char forder; /* vector order for data out */
static void readvhdlfile();
static int yylex();
static int yyerror();

/* danger :
   These three functions use an internal buffer when called, so
   avoid calling them twice in the same function call. */
static char *octbin(s)
char *s;
{
static char b[256];
char t[2];

	b[0] = t[1] = '\0';
	while (*s) {
		*t = *s;
		strcat(b, oct2bin[strtol(t, (char **)NULL, 8)]);
		s++;
	}
	return b;
}

static char *hexbin(s)
char *s;
{
static char b[256];
char t[2];

	b[0] = t[1] = '\0';
	while (*s) {
		*t = *s;
		strcat(b, hex2bin[strtol(t, (char **)NULL, 16)]);
		s++;
	}
	return b;
}

static char *binvert(s)
char *s;
{
static char b[256];
int l = strlen(s) - 1, i = 0;

	while (l >= 0)
		b[i++] = s[l--];
	b[i] = '\0';
	return b;
}

static dptr *adddptr(h, s, v)
dptr *h;
char *s, *v;
{
dptr *p;

	p = (dptr *)mbkalloc(sizeof(dptr));
	p->string = (char *)mbkalloc(strlen(s) + 1);
	(void)strcpy(p->string, s);
	p->value = (char *)mbkalloc(strlen(v) + 1);
	(void)strcpy(p->value, v);
	p->NEXT = h;
	return p;
}

static char *getdptr(h, s)
dptr *h;
char *s;
{
	while (h != NULL) {
		if (!strcmp(h->string, s))
			return h->value;
		h = h->NEXT;
	}
	return NULL;
}

%}

%union {
	int integer;
	char string[256];
}
%token CONSTANT OTHERS WHEN SELECT WITH
%token <string> STRING BSTRING OSTRING XSTRING
%type  <string> value dvalues avalues
/* needed only to avoid bison `type clash', as I do not care about it */
%type  <string> line constant

%%

line		: 	constant
			|	STRING /* forget it */ 
			|	WITH STRING '(' STRING STRING STRING ')' SELECT
			{
				if (atoi($4) != 0 && atoi($6) != 0)
					yyerror("illegal vector boundaries for GRoG");
				/* address order choice :
				   depends upon the bit ordering convention, and what is read
				   from the file. */
				if ((!ibmbull && !strcmp($5, "TO"))
						|| (ibmbull && !strcmp($5, "DOWNTO")))
					aorder = -1;
				else
					aorder = 1;
			}
			|	STRING '(' STRING STRING STRING ')' '<' '=' dvalues WHEN avalues ','
			{
			int i;
#ifdef PARSEOUT
				fprintf(stdout, "\n%s when %s\n", datbuf, adrbuf);
#endif
				if (atoi($3) != 0 && atoi($5) != 0)
					yyerror("illegal vector boundaries for GRoG");
				/* output ordering :
				   internally, the data is seen in the ibmbull convention, since
				   the generator was designed with this convention I hate
				   demanded. */
				if ((rf && !strcmp($4, "TO")) || (!rf && !strcmp($4, "DOWNTO")))
					forder = -1;
				else
					forder = 1;
				notempty = 1;
				if (strlen(datbuf) > nbits)
					yyerror("data too wide for the number of bits");
				if (strlen(adrbuf) > nwords)
					yyerror("address too high for the number of words");
				if (aorder == -1)
					strcpy(adrbuf, binvert(adrbuf));
				if (forder == -1)
					strcpy(datbuf, binvert(datbuf));
				if (nbits < 32) {
					i = strtol(adrbuf, 0, 2);
					ydata[(i << 1)    ] = strtol(datbuf, 0, 2);
					ydata[(i << 1) + 1] = 0;
				} else {
					i = strtol(adrbuf, 0, 2);
					ydata[(i << 1)    ] = strtol(&datbuf[nbits - 32], 0, 2);
					datbuf[nbits - 32] = '\0';
					ydata[(i << 1) + 1] = strtol(datbuf, 0, 2);
				}

				if (!used[i])
					used[i] = 1;
				else
					yyerror("same address used twice");
			}
			|	dvalues WHEN avalues ','
			{
			int i;
#ifdef PARSEOUT
				fprintf(stdout, "\n%s when %s\n", datbuf, adrbuf);
#endif
				if (strlen(datbuf) > nbits)
					yyerror("data too wide for the number of bits");
				if (strlen(adrbuf) > nwords)
					yyerror("address too high for the number of words");
				if (aorder == -1)
					strcpy(adrbuf, binvert(adrbuf));
				if (forder == -1)
					strcpy(datbuf, binvert(datbuf));
				if (nbits < 32) {
					i = strtol(adrbuf, 0, 2);
					ydata[(i << 1)    ] = strtol(datbuf, 0, 2);
					ydata[(i << 1) + 1] = 0;
				} else {
					i = strtol(adrbuf, 0, 2);
					ydata[(i << 1)    ] = strtol(&datbuf[nbits - 32], 0, 2);
					datbuf[nbits - 32] = '\0';
					ydata[(i << 1) + 1] = strtol(datbuf, 0, 2);
				}
				if (!used[i])
					used[i] = 1;
				else
					yyerror("same address used twice");
			}
			|	dvalues WHEN avalues ';'
			{
			int i;
#ifdef PARSEOUT
				fprintf(stdout, "\n%s when %s\n", datbuf, adrbuf);
#endif
				if (strlen(datbuf) > nbits)
					yyerror("data too wide for the number of bits");
				if (strlen(adrbuf) > nwords)
					yyerror("address too high for the number of words");
				if (aorder == -1)
					strcpy(adrbuf, binvert(adrbuf));
				if (forder == -1)
					strcpy(datbuf, binvert(datbuf));
				if (nbits < 32) {
					i = strtol(adrbuf, 0, 2);
					ydata[(i << 1)    ] = strtol(datbuf, 0, 2);
					ydata[(i << 1) + 1] = 0;
				} else {
					i = strtol(adrbuf, 0, 2);
					ydata[(i << 1)    ] = strtol(&datbuf[nbits - 32], 0, 2);
					datbuf[nbits - 32] = '\0';
					ydata[(i << 1) + 1] = strtol(datbuf, 0, 2);
				}
				if (!used[i])
					used[i] = 1;
				else
					yyerror("same address used twice");
			}
			|	dvalues WHEN OTHERS ';'
			{
			int  i;
         	long j[2];
#ifdef PARSEOUT
				fprintf(stdout, "\n%s when others\n", datbuf);
#endif
				if (strlen(datbuf) > nbits)
					yyerror("data too wide for the number of bits");
				if (strlen(adrbuf) > nwords)
					yyerror("address too high for the number of words");
				if (forder == -1)
					strcpy(datbuf, binvert(datbuf));
				if (nbits < 32) {
					j[0] = strtol(datbuf, 0, 2);
					j[1] = 0;
				} else {
					j[0] = strtol(&datbuf[nbits - 32], 0, 2);
					datbuf[nbits - 32] = '\0';
					j[1] = strtol(datbuf, 0, 2);
				}
				for (i = 0; i < nwords; i++)
					if (!used[i])
						ydata[(i << 1)    ] = j[0],
						ydata[(i << 1) + 1] = j[1];
			}
			|	{strcpy($$, "Just to have bison shut up");}
			;

constant :	CONSTANT STRING '=' value ';'
			{
				asc = adddptr(asc, $2, $4);
			}
			;

value		: BSTRING {strcpy($$, $1);}
			| OSTRING {strcpy($$, octbin($1));}
			| XSTRING {strcpy($$, hexbin($1));}
			;

dvalues	:	value {strcat(datbuf, $1);}
			|	value {strcat(datbuf, $1);} '&' dvalues
			|	STRING
			{
			char *p = getdptr(asc, $1);

				if (p == NULL) {
					sprintf(datbuf, "unknown constant %s", $1);
					yyerror(datbuf);
				}
				strcat(datbuf, p);
			}
			|	STRING {
			char *p = getdptr(asc, $1);

				if (p == NULL) {
					sprintf(datbuf, "unknown constant %s", $1);
					yyerror(datbuf);
				}
				strcat(datbuf, p);
			} '&' dvalues
			;

avalues	:	value {strcat(adrbuf, $1);}
			|	value {strcat(adrbuf, $1);} '&' avalues
			|	STRING
			{
			char *p = getdptr(asc, $1);

				if (p == NULL) {
					sprintf(adrbuf, "unknown constant %s", $1);
					yyerror(adrbuf);
				}
				strcat(adrbuf, p);
			}
			|	STRING {
			char *p = getdptr(asc, $1);

				if (p == NULL) {
					sprintf(adrbuf, "unknown constant %s", $1);
					yyerror(adrbuf);
				}
				strcat(adrbuf, p);
			} '&' avalues
			;

%%

void dpgen_ROM_code(s, nb, nw, r, msb, data)
char *s;
long nb, nw, r, msb;
long *data;
{
FILE *f;

	if ((f = mbkfopen(s, "vbe", "r")) == NULL) {
		fprintf(stderr, "DPGEN_ROM error : cannot open file %s.vbe\n", s);
		EXIT(1);
	}
	readvhdlfile(f, s, nb, nw, r, msb, data);
	if (!notempty) {
		(void)fflush(stdout);
		(void)fprintf(stdout, "DPGEN_ROM : the file was empty of usable data\n");
		(void)fprintf(stdout, "            the generated ROM will contain zeros\n");
	}
	fclose(f);
}

static FILE *vhdlfile;

static void nextvhdlline(f, str)
FILE *f;
char *str;
{
char *s, *t;
int space;

		if (fgets(str, 512, f) == (char *)NULL)
			return;
		s = t = str;
		space = isspace(*t) ? 0 : 1;
		while (*t)
			if (!isspace(*t) || space) {
				space = isspace(*t) || *t == ':' ? 0 : 1;
				if (islower(*t))
					*t = toupper(*t);
				*s++ = *t++;
			} else {
				t++;
				space = 0;
			}
		*s = '\0';
}

static char *nextline()
{
static char str[256];

	nextvhdlline(vhdlfile, str);
	return str;
}

static void readvhdlfile(f, s, nb, nw, r, msb, data)
FILE *f;
char *s;
long nb, nw, r, msb;
long *data;
{
char str[256];
char *t;
int space, i;

	/* globalize variables for yacc */
	nwords = nw;
	nbits = nb;
	rf = r;
	ibmbull = msb;
	ydata = data;
	vhdlfile = f;

	/* initialize stuff for OTHERS */
	for (i = 0; i < 8912; i++)
		used[i] = 0;

	while (!feof(f)) {
		line++;
		nextvhdlline(f, str); 
		vhdl_line = str;
		datbuf[0] = adrbuf[0] = '\0';
		yyparse();
	}
}

/* lexical analyser */
static int yylex()
{
static int c;
int i = 0;
char *s = vhdl_line; /* remember where to start from */

	while (*s && isspace(*s))
		s++;

	switch (*s) {
		case '\0':

#ifdef PARSEOUT
			printf("\n");
#endif

			return -1; /* expected by yacc as end of entries */

		case ';':
			c = '\0'; /* reset to nul state */
		case '<':
		case '=':
		case ',':
		case '&':
		case '(':
		case ')':

#ifdef PARSEOUT
			printf("(%c)", *s);
#endif

			if (*s == '<' && c == SELECT)
				c = WITH;
			vhdl_line = s;
			return *vhdl_line++;

		case ':':
			s++;
			while (*s && *s != '=')
				s++;

#ifdef PARSEOUT
			printf("(%c)", *s);
#endif

			vhdl_line = ++s;
			return '=';

		case '-':
			if (*++s == '-') {
#ifdef PARSEOUT
				printf("COMMENT\n");
#endif
				return -1;
			} 
			return *vhdl_line++;

		case 'C':
			while (*s && (isalnum(*s) || *s == '_'))
				yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;
			if (!strcmp(yylval.string, "CONSTANT")) {

#ifdef PARSEOUT
				printf("TOKEN[%s]", yylval.string);
#endif

				return c = CONSTANT;
			}

#ifdef PARSEOUT
			printf("STRING{%s}", yylval.string);
#endif
			if (c != SELECT && c != WITH && c != CONSTANT)
				*vhdl_line = '\0';
			return STRING;


		case 'G':
			while (*s && (isalnum(*s) || *s == '_'))
				yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;
			if (!strcmp(yylval.string, "GENERIC")) {
				while (1) {
					if (!*s)
						s = nextline();
					if (*s++ == '(') {
						s--;
						i = 0;
						break;
					}
				}
				while (1) {
					if (!*s)
						s = nextline();
					switch (*s++) {
						case '(':
							i++;
							break;
						case ')':
							i--;
							break;
						case '\n':
							line++;
					} 
					if (i == 0)
						break;
				}
			
				*vhdl_line = '\0';
				return STRING;
			}

#ifdef PARSEOUT
			printf("STRING{%s}", yylval.string);
#endif
			if (c != SELECT && c != WITH && c != CONSTANT)
				*vhdl_line = '\0';
			return STRING;

		case 'W':
			while (*s && (isalnum(*s) || *s == '_'))
				yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;
			if (!strcmp(yylval.string, "WHEN")) {

#ifdef PARSEOUT
				printf("TOKEN[%s]", yylval.string);
#endif

				return WHEN;
			} else if (!strcmp(yylval.string, "WITH")) {

#ifdef PARSEOUT
				printf("TOKEN[%s]", yylval.string);
#endif

				c = SELECT;
				return WITH;
			} else {

#ifdef PARSEOUT
				printf("STRING{%s}", yylval.string);
#endif
				if (c != SELECT && c != WITH && c != CONSTANT)
					*vhdl_line = '\0';
				return STRING;
			}

		case 'S':
			while (*s && (isalnum(*s) || *s == '_'))
				yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;
			if (!strcmp(yylval.string, "SELECT")) {

#ifdef PARSEOUT
				printf("TOKEN[%s]", yylval.string);
#endif

				*vhdl_line = '\0';
				return SELECT;
			} else {

#ifdef PARSEOUT
				printf("STRING{%s}", yylval.string);
#endif
				if (c != SELECT && c != WITH && c != CONSTANT)
					*vhdl_line = '\0';
				return STRING;
			}

		case 'B':
			if (c != SELECT && c != WITH && c != CONSTANT) {
				*vhdl_line = '\0';
				return STRING;
			}
			if (*(s + 1) == '"') {
				s += 2;
				while (*s && *s != '"')
					if (*s != '0' && *s != '1')
						yyerror("bad binary number");
					else
						yylval.string[i++] = *s++;
				yylval.string[i] = '\0';
				vhdl_line = ++s;

#ifdef PARSEOUT
				printf("BSTRING{%s}", yylval.string);
#endif
				return BSTRING;
			}
			while (*s && (isalnum(*s) || *s == '_'))
				yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;

#ifdef PARSEOUT
			printf("STRING{%s}", yylval.string);
#endif
			return STRING;

		case 'O':
			if (c != SELECT && c != WITH && c != CONSTANT) {
				*vhdl_line = '\0';
				return STRING;
			}
				
			if (*(s + 1) == '"') {
				s += 2;
				while (*s && *s != '"')
					if (!isdigit(*s) && *s != '8' && *s != '9')
						yyerror("bad octal number");
					else
						yylval.string[i++] = *s++;
				yylval.string[i] = '\0';
				vhdl_line = ++s;

#ifdef PARSEOUT
				printf("OSTRING{%s}", yylval.string);
#endif
				return OSTRING;
			}
			while (*s && (isalnum(*s) || *s == '_'))
				yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;
			if (!strcmp(yylval.string, "OTHERS")) {

#ifdef PARSEOUT
				printf("TOKEN[%s]", yylval.string);
#endif

				return OTHERS;
			}

#ifdef PARSEOUT
			printf("STRING{%s}", yylval.string);
#endif
			return STRING;

		case 'X':
			if (c != SELECT && c != WITH && c != CONSTANT) {
				*vhdl_line = '\0';
				return STRING;
			}
			if (*(s + 1) == '"') {
				s += 2;
				while (*s && *s != '"')
					if (!isxdigit(*s))
						yyerror("bad hexadecimal number");
					else
						yylval.string[i++] = *s++;
				yylval.string[i] = '\0';
				vhdl_line = ++s;

#ifdef PARSEOUT
				printf("XSTRING{%s}", yylval.string);
#endif
				return XSTRING;
			}
			while (*s && (isalnum(*s) || *s == '_'))
				yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;

#ifdef PARSEOUT
			printf("STRING{%s}", yylval.string);
#endif
			return STRING;

		default:
			if (c == SELECT)
				while (*s && !isspace(*s) && *s != '(' && *s != ')')
					yylval.string[i++] = *s++;
			else
				while (*s && !isspace(*s))
					yylval.string[i++] = *s++;
			yylval.string[i] = '\0';
			vhdl_line = s;
			if (c != SELECT && c != WITH && c != CONSTANT)
				*vhdl_line = '\0';

#ifdef PARSEOUT
			printf("STRING{%s}", yylval.string);
#endif

			return STRING;
	}
}

static int yyerror(s)
char *s;
{
	fflush(stdout);
	fprintf(stderr, "DPGEN_ROM : %s on line %d (%s), '%c'\n",
				     s, line, vhdl_line, yychar);
	exit(1);
}
