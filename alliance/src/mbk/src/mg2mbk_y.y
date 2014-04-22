/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/******************************************************************************
* small modgen parser for absolute placement only                             *
* no control flow statement allowed, nor fancy stuff.                         *
* Written By Frederic Petrot, petrot@masi.ibp.fr for Masi/cao-vlsi team       *
* Done for the IDPS project                                                   *
******************************************************************************/
%{
#include <stdio.h>
#include <string.h>
#include <mut.h>
#include <mph.h>
#define MAXLAYER (LAST_LAYER + 1)

extern void yyrestart();
extern int yylex();
extern void yyerror();

static phfig_list *mgleaf; /* modgen leaf cell parsed */
static char mbk_layer();
static char *checkinsname();
static char *checkconname();
static char *checkrefname();
static chain_list *pl;
static int Extension[MAXLAYER] = {
	/*	NWELL	 */ 0,
	/*	PWELL	 */ 0,
	/*	NTIE	 */ 1,
	/*	PTIE	 */ 1,
	/*	NDIF	 */ 1,
	/*	PDIF	 */ 1,
	/*	NTRANS */ 0, /* meant for N transistor grid */
	/*	PTRANS */ 0, /* meant for P transistor grid */
	/*	POLY	 */ 1,
	/*	ALU1	 */ 1,
	/*	ALU2	 */ 2,
	/*	ALU3	 */ 2,
	/*	TPOLY	 */ 1,
	/*	TALU1	 */ 1,
	/*	TALU2	 */ 2,
	/*	TALU3	 */ 2 };

#define O_PS_AA 1.5
#define O_AA_PS 2

#ifdef GRIDDED
#define TWO 2
#else
#define TWO 1
#endif

/* note :
   All STRINGs are allocated using mbkstrdup since the pointer to yytext
   will change when more than a STRING is used in the grammar.
   So all STRINGs are freed once used. */

%}

%union {
	char *s;
	long n;
	struct pxy {
		long x;
		long y;
	} *p;
}

%token CELL
%token PLACE
%token TERMINAL
%token RA
%token STRETCH
%token WIRE
%token MWIDTH
%token HORIZ
%token VERTI
%token MLEFT
%token MRIGHT
%token MTOP
%token MBOTTOM
%token AT
%token MX
%token MY
%token R
%token NTR
%token PTR
%token COP
%token CON
%token COPS
%token COPW
%token CONW
%token PVIA
%token CXN
%token CXP
%token STRING
%token NUM

%type <p> point
%type <s> STRING
%type <n> NUM
%type <n> face
%type <n> dir
%type <n> op

%start design

%%
design : cells
		;

cells : leafcell
		| cells leafcell
		;

leafcell : CELL STRING '{' lines '}'
		{
			mbkfree($2);
		}
		;

lines : line
		| line lines
		;

line : PLACE STRING '=' elm ';'
		{
			mbkfree($2);
		}
		| PLACE STRING '=' STRING op AT '(' point ')' ';'
		{
			if (!strcmp($4, "ref_ref") || !strcmp($4, "ref_con"))
				addphref(mgleaf, $4, checkrefname($2), $8->x, $8->y);
			else /* unknown internally! */
				addphins(mgleaf, $4, checkinsname($2), $5, $8->x, $8->y);
			mbkfree($2);
			mbkfree($4);
			mbkfree($8);
		}
		| elm ';'
		;

elm : RA '(' STRING ',' point ',' point ')'
		{
			if (strncmp($3, "zd", 2)) {
				fprintf(stderr, "modgen parser warning : ra not for ab box\n");
				/* exit(18); */
			} else
				defab(mgleaf, $5->x, $7->x, $5->y, $7->y);
			mbkfree($3);
			mbkfree($5);
			mbkfree($7);
		}
		| TERMINAL STRING '(' MWIDTH '=' NUM ')' face STRING  AT '(' point ')'
		{
			if (strcmp($2, "ul") && strcmp($2, "ur")
					&& strcmp($2, "lr") && strcmp($2, "ll"))
				addphcon(mgleaf, (char)$8, checkconname($2),
							$12->x, $12->y, mbk_layer($9), $6); 
			mbkfree($2);
			mbkfree($9);
			mbkfree($12);
		}
		| STRETCH '(' STRING ',' NUM ',' dir ',' point ',' point ')'
		{
		char layer = mbk_layer($3);
		int x1 = $9->x, y1 = $9->y, x2 = $11->x, y2 = $11->y;

			mbkfree($3);
			mbkfree($9);
			mbkfree($11);
			if ($7 == VERTI) {
				if (y2 > y1) {
					y2 -= Extension[(int)layer] * SCALE_X * TWO / 2;
					y1 += Extension[(int)layer] * SCALE_X * TWO / 2;
				} else {
					y2 += Extension[(int)layer] * SCALE_X * TWO / 2;
					y1 -= Extension[(int)layer] * SCALE_X * TWO / 2;
				}
			} else {
				if (x2 > x1) {
					x2 -= Extension[(int)layer] * SCALE_X * TWO / 2;
					x1 += Extension[(int)layer] * SCALE_X * TWO / 2;
				} else {
					x2 += Extension[(int)layer] * SCALE_X * TWO / 2;
					x1 -= Extension[(int)layer] * SCALE_X * TWO / 2;
				}
			}
			addphseg(mgleaf, layer, $5, x1, y1, x2, y2, NULL);
		}
		| WIRE {pl = NULL;} '(' STRING ',' NUM ',' point_list ')'
		{
		chain_list *l;
		int x1, y1, x2, y2;
		char layer = mbk_layer($4);

			mbkfree($4);
			/* points in inverted order, but doesn't matter, same algo */
			for (l = pl; l->NEXT; l = l->NEXT) {
				x1 = (int)((struct pxy *)(l->DATA))->x;
				y1 = (int)((struct pxy *)(l->DATA))->y;
				x2 = (int)((struct pxy *)(l->NEXT->DATA))->x;
				y2 = (int)((struct pxy *)(l->NEXT->DATA))->y;
				if (l == pl) { /* last point */
					if (x1 == x2) {
						if (y2 > y1) {
							y1 += Extension[(int)layer] * SCALE_X * TWO / 2;
						} else {
							y1 -= Extension[(int)layer] * SCALE_X * TWO / 2;
						}
					} else {
						if (x2 > x1) {
							x1 += Extension[(int)layer] * SCALE_X * TWO / 2;
						} else {
							x1 -= Extension[(int)layer] * SCALE_X * TWO / 2;
						}
					}
				}
				if (!l->NEXT->NEXT) { /* first point */
					if (x1 == x2) {
						if (y2 > y1) {
							y2 -= Extension[(int)layer] * SCALE_X * TWO / 2;
						} else {
							y2 += Extension[(int)layer] * SCALE_X * TWO / 2;
						}
					} else {
						if (x2 > x1) {
							x2 -= Extension[(int)layer] * SCALE_X * TWO / 2;
						} else {
							x2 += Extension[(int)layer] * SCALE_X * TWO / 2;
						}
					}
				}
				addphseg(mgleaf, layer, $6, x1, y1, x2, y2, NULL);
			}
		}
		| NTR '(' NUM ',' NUM ')' op AT '(' point ')'
		{ /* bounding box of transistors is knows for idps technology */
		float tx; /* used to compute transistor bbox */
		int x1, y1, x2, y2;

			/* +-----------+ when not placed yet */
			tx = ((float)$3 / 2 + (float)O_PS_AA * SCALE_X * TWO);

			switch ((char)$7) {
				case NOSYM:
				case SYM_X:
				case SYM_Y:
				case SYMXY:
					y2 = y1 = $10->y;
					x1 = $10->x + tx;
					x2 = $10->x - tx;
					addphseg(mgleaf, NTRANS, $5, x1, y1, x2, y2, NULL);
					break;
				case ROT_P:
				case ROT_M:
				case SY_RP:
				case SY_RM:
					x2 = x1 = $10->x;
					y1 = $10->y + tx;
					y2 = $10->y - tx;
					addphseg(mgleaf, NTRANS, $5, x1, y1, x2, y2, NULL);
					break;
			}
			mbkfree($10);
		}
		| PTR '(' NUM ',' NUM ')' op AT '(' point ')'
		{ /* bounding box of transistors is knows for idps technology */
		float tx; /* used to compute transistor bbox */
		int x1, y1, x2, y2;

			/* +-----------+ when not placed yet */
			tx = ((float)$3 / 2 + (float)O_PS_AA * SCALE_X * TWO);

			switch ((char)$7) {
				case NOSYM:
				case SYM_X:
				case SYM_Y:
				case SYMXY:
					y2 = y1 = $10->y;
					x1 = $10->x + tx;
					x2 = $10->x - tx;
					addphseg(mgleaf, PTRANS, $5, x1, y1, x2, y2, NULL);
					break;
				case ROT_P:
				case ROT_M:
				case SY_RP:
				case SY_RM:
					x2 = x1 = $10->x;
					y1 = $10->y + tx;
					y2 = $10->y - tx;
					addphseg(mgleaf, PTRANS, $5, x1, y1, x2, y2, NULL);
					break;
			}
			mbkfree($10);
		}
		| COP AT '(' point ')'
		{
			addphvia(mgleaf, CONT_DIF_P, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		| CON AT '(' point ')'
		{
			addphvia(mgleaf, CONT_DIF_N, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		| COPS AT '(' point ')'
		{
			addphvia(mgleaf, CONT_POLY, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		| COPW AT '(' point ')'
		{
			addphvia(mgleaf, CONT_BODY_P, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		| CONW AT '(' point ')'
		{
			addphvia(mgleaf, CONT_BODY_N, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		| PVIA AT '(' point ')'
		{
			addphvia(mgleaf, CONT_VIA, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		| CXN AT '(' point ')'
		{
			addphvia(mgleaf, C_X_N, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		| CXP AT '(' point ')'
		{
			addphvia(mgleaf, C_X_P, $4->x, $4->y, 0, 0,NULL);
			mbkfree($4);
		}
		;

point : NUM ',' NUM
		{
		struct pxy *pp;

			pp = (struct pxy *)mbkalloc(sizeof(struct pxy));
			pp->x = $1;
			pp->y = $3;
			$$ = pp;
		}
		;

point_list : point
		{
			pl = addchain(pl, (void *)$1);
		}
		| point ',' point_list
		{
			pl = addchain(pl, (void *)$1);
		}
		;

dir :   HORIZ  {$$ = (long)HORIZ;}
		| VERTI  {$$ = (long)VERTI;}
		;

face :  MLEFT   {$$ = (long)WEST;}
		| MRIGHT  {$$ = (long)EAST;}
		| MBOTTOM {$$ = (long)SOUTH;}
		| MTOP    {$$ = (long)NORTH;}
		;

op :	{$$ = (long)NOSYM;}
		| MX
		{$$ = (long)SYM_Y;}
		| MY
		{$$ = (long)SYM_X;}
		| MX MY
		{$$ = (long)SYMXY;}
		| R NUM
		{
			if ($2 / SCALE_X / TWO == 90)
				$$ = (long)ROT_P;
			if ($2 / SCALE_X / TWO == 270)
				$$ = (long)ROT_M;
		}
		| MX R NUM
		{
			if ($3 / SCALE_X / TWO == 90)
				$$ = (long)SY_RM;
			if ($3 / SCALE_X / TWO == 270)
				$$ = (long)SY_RP;
		}
		| MY R NUM
		{
			if ($3 / SCALE_X / TWO == 90)
				$$ = (long)SY_RP;
			if ($3 / SCALE_X / TWO == 270)
				$$ = (long)SY_RM;
		}
		;

%%
char *
	filenameforlex()
{
	return mgleaf->NAME;
}

void
	mgnloadphfig(fig, name, mode)
phfig_list *fig;
char *name;
char mode;
{
extern FILE *yyin; /* declared in the lex.yy.c by default */
extern int yylineno;
chain_list *c;

	mgleaf = fig;
	if ((yyin = mbkfopen(name, IN_PH, READ_TEXT)) == NULL) {
		fflush(stdout);
		fprintf(stderr,
			"*** mbk error *** modgen parser failed opening file %s.%s\n",
			name, IN_PH);
		EXIT(1);
	}
	yylineno = 0;
	if (yyparse()) {
		fflush(stdout);
		fprintf(stderr,
			"*** mbk error *** modgen parser failed while reading file %s\n",
			filepath(name, IN_PH));
		EXIT(1);
	}
	(void)fclose(yyin);
	/* free :
	   temporary space for point list must be freed. */
	for (c = pl; c; c = c->NEXT)
		mbkfree(c->DATA);
	freechain(pl);
	/* not so nice :
	   It would be nice if I'd parse only the necessary stuff when reading
	   the file, but it's a little painful, and would need some checks
	   I can't afford right now. */
	if (mode == 'P') {
	void **ppobj;
	void *next;

		/* delete instances */
		for (ppobj = (void**)&HEAD_PHFIG->PHINS; *ppobj != NULL;) {
			next = (void *)((phins_list *)*ppobj)->NEXT;
			mbkfree((phins_list *)*ppobj);
			*ppobj = next;
		}

		/* delete vias */
		for (ppobj = (void**)&HEAD_PHFIG->PHVIA; *ppobj != NULL;) {
			next = (void *)((phvia_list *)*ppobj)->NEXT;
			mbkfree((phvia_list *)*ppobj);
			*ppobj = next;
		}

		/* delete segments */
		for (ppobj = (void**)&HEAD_PHFIG->PHSEG; *ppobj != NULL;) {
			if (((phseg_list *)*ppobj)->LAYER != TPOLY
					&& ((phseg_list *)*ppobj)->LAYER != TALU1
					&& ((phseg_list *)*ppobj)->LAYER != TALU2
					&& ((phseg_list *)*ppobj)->LAYER != TALU3) {
				next = (void *)((phseg_list *)*ppobj)->NEXT;
				mbkfree((phseg_list *)*ppobj);
				*ppobj = next;
			} else
				ppobj = (void **)&((phseg_list *)*ppobj)->NEXT;
		}
	}
	/* mbk strategy :
	   only one cell per modgen file. */
	fig = HEAD_PHFIG;
}

static char
	mbk_layer(mg_layer)
char *mg_layer;
{
	if (!strcmp(mg_layer, "nw"))
		return NWELL;
	if (!strcmp(mg_layer, "pw"))
		return PWELL;
	if (!strcmp(mg_layer, "xn"))
		return NTIE;
	if (!strcmp(mg_layer, "xp"))
		return PTIE;
	if (!strcmp(mg_layer, "nn"))
		return NDIF;
	if (!strcmp(mg_layer, "pp"))
		return PDIF;
	if (!strcmp(mg_layer, "ps"))
		return POLY;
	if (!strcmp(mg_layer, "m1"))
		return ALU1;
	if (!strcmp(mg_layer, "m2"))
		return ALU2;
	if (!strcmp(mg_layer, "m3"))
		return ALU3;
	if (!strcmp(mg_layer, "am1"))
		return TALU1;
	if (!strcmp(mg_layer, "am2"))
		return TALU2;
	if (!strcmp(mg_layer, "am3"))
		return TALU3;
    return NWELL;
}

/* decode a reference name :
   xxx[?]          -> xxx
   xxx[?][??]      -> xxx ?,
   xxx[?][??][???] -> xxx ?_??.
   I always have added an index since here also two instances can't have
   the same name. */

static char
	*checkrefname(name)
char *name;
{
char *s, *t, *u = NULL;
char one = 0;

   s = t = name;

   while (*t) {
     if (*t == '[') {
       if (!one) {
         *t = ' ';
         u = s;
         one = 1;
       } else if (one == 1) {
         *t = '_';
         u = s;
         one++;
       } else {
         *t = ' ';
         u = s;
       }
     }
     else if (*t == ']') {
       if (*(++t) == '\0') /* ok, it's finished */
         goto end;
       else if (*t == '[') /* multiple array */
         continue;
     }
     *s++ = *t++;
   }
end:
   *s = '\0';
	/* last vectorized value :
	   it shall be skipped since I introduced it while driving the file. */
	if (u)
   	*u = '\0';
   return name;
}

/* decode a connector name :
   in modgen, 'cause we haven't the same conventions, a by me driven
   connector has the forms :
   xxx[?] -> xxx, where ? is a number, because I suffix all connectors that
                  way.
   xxx[??][?] -> xxx[??], because this was previously a vector. */

static char
	*checkconname(name)
char *name;
{
char *s, *t, *u = NULL;

   s = t = name;
   while (*t) {
      if (*t == '[') {
        	*t = ' ';
			u = s; /* last vector at that time */
      } else if (*t == ']') {
         if (*(++t) == '\0') /* ok, it's finished */
            goto end;
         else if (*t == '[') /* multiple array */
            continue;
		}
      *s++ = *t++;
   }
end:
   *s = '\0';
	/* last vectorized value :
	   it shall be skipped since I introduced it while driving the file. */
	if (u)
   	*u = '\0';
   return name;
}

/* decode an instance name :
   in modgen, 'cause we haven't the same conventions, a by me driven
   instance has the forms :
   xxx.corner
	since I arranged to put the corner in such a place that it makes it
   automatically good for mbk, I just forgot about the corner. */

static char
	*checkinsname(name)
char *name;
{
char *s = name;

	s = strrchr(name, '.');
   *s = '\0';
   return name;
}
