
/*  A Bison parser, made from d_asm.yac
    by GNU Bison version 1.28  */

#define ASMBISON 1  /* Identify Bison output.  */

#define	VIRGULE	257
#define	DEUX_PTS	258
#define	IMMEDIAT	259
#define	DIRECT	260
#define	IND_DEB	261
#define	IND_FIN	262
#define	SYMBL	263
#define	LIGNE_DIR	264
#define	BLOC	265
#define	DATA	266
#define	ALIGN	267
#define	TEXT	268
#define	GLOBL	269
#define	ASCII	270
#define	STATIC	271
#define	SKIP	272
#define	COMM	273
#define	CHAR	274
#define	STRING	275
#define	INT_DIR	276
#define	SHORT	277
#define	FLOAT	278
#define	DOUBLE	279
#define	STRING_DIR	280
#define	FILE_DIR	281
#define	VERSION	282
#define	NAME	283
#define	ETIQ	284
#define	NOMBRE	285
#define	CODE_OP	286
#define	SIGN	287
#define	REGISTRE	288
#define	FLOTTANT	289

#line 1 "d_asm.yac"

#include <stdio.h>
#include "d_codes.h"
#include "d_asm.h"
#include "d_dbx.h"

/* structure instruction */
typedef union {
	struct {
		unsigned char code_inst;
		char mode_op[3];
		int  op[3];
	} inst;
	char tab_inst[16];
} INST, *PINST;

INST inst;		/* instruction en cours de codage */
int num_op;		/* numero de l'operande en cours de traitement */
int nb_op_suiv;	/* nobre d'operandes codees a la suite de l'inst. */

typedef union {
	char  *str;
	char   car;
	int    val;
	double dbl;
} OP_DIR;

ptfonc First_fonction = 0;
ptfonc function, save_function = 0;

OP_DIR	op_dir[5];
int     nb_op_dir;

int  taille_code = 0; 	/* taille du code engendre */
int  taille_data = 0;  /* taille des donnees */

int  new_taille_code; 	/* variables tampon */
int  new_taille_data;

BOOL InData = FALSE;   /* indique si l'on traite du code ou des datas */

extern unsigned char *I_Code;	/* bloc memoire contenant le binaire */
extern int  taille_max_code;	/* taille du bloc pour le code */
	/* bloc contenant les variables globales et statiques */
extern unsigned char *I_Data;
extern int  taille_max_data;   /* taille du bloc des variables */
extern PSYMB TabSymb;		/* table des symboles */

int i,j;			/* variables pour les boucles */

char *nom_glob = "";	/* nom suivant le dernier .global */
char *nom_fonc;	/* nom de la fonction courante    */

#line 55 "d_asm.yac"
typedef union {
	int val;
	char *str;
	double fl;
} ASMSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	ASMFINAL		109
#define	ASMFLAG		-32768
#define	ASMNTBASE	36

#define ASMTRANSLATE(x) ((unsigned)(x) <= 289 ? asmtranslate[x] : 73)

static const char asmtranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35
};

#if ASMDEBUG != 0
static const short asmprhs[] = {     0,
     0,     3,     8,    11,    14,    17,    18,    20,    21,    24,
    26,    27,    32,    33,    37,    39,    41,    43,    45,    47,
    49,    51,    54,    56,    59,    62,    65,    68,    71,    73,
    76,    79,    83,    87,    89,    93,    94,    96,    99,   100,
   105,   106,   111,   112,   117,   118,   123,   127,   131,   132,
   137,   138,   143,   144,   149,   150,   155,   156,   161,   162,
   167,   168,   173,   175,   177,   179,   183,   185,   188,   190,
   192,   195,   197,   199,   201,   203,   206,   208,   211,   214
};

static const short asmrhs[] = {    37,
    38,     0,    27,    21,    28,    21,     0,    38,    54,     0,
    38,    41,     0,    38,    71,     0,     0,    40,     0,     0,
    40,    30,     0,    30,     0,     0,    39,    32,    42,    43,
     0,     0,    43,     3,    44,     0,    44,     0,    47,     0,
    48,     0,    50,     0,    49,     0,    51,     0,    53,     0,
    52,    31,     0,    45,     0,     5,    70,     0,    70,    45,
     0,     5,    45,     0,     5,    35,     0,     5,    70,     0,
    70,     0,     6,    45,     0,     6,    70,     0,     5,    70,
    45,     0,     6,    70,    45,     0,    34,     0,     7,    50,
     8,     0,     0,    33,     0,    45,    51,     0,     0,    39,
    68,    55,    66,     0,     0,    39,     9,    56,    66,     0,
     0,    39,    20,    57,    66,     0,     0,    39,    23,    58,
    66,     0,    39,    22,    46,     0,    39,    22,    70,     0,
     0,    39,    24,    59,    66,     0,     0,    39,    25,    60,
    66,     0,     0,    39,    26,    61,    66,     0,     0,    39,
    13,    62,    66,     0,     0,    39,    18,    63,    66,     0,
     0,    39,    19,    64,    66,     0,     0,    39,    10,    65,
    66,     0,    69,     0,    12,     0,    14,     0,    66,     3,
    67,     0,    67,     0,    52,    31,     0,    21,     0,    35,
     0,     5,    70,     0,    70,     0,    11,     0,    16,     0,
    17,     0,    15,    70,     0,    29,     0,    40,    72,     0,
    72,    70,     0,    70,     0
};

#endif

#if ASMDEBUG != 0
static const short asmrline[] = { 0,
    75,    78,    85,    86,    87,    88,    91,    92,    95,   111,
   138,   145,   156,   156,   157,   160,   161,   162,   163,   164,
   165,   168,   174,   178,   185,   194,   201,   208,   216,   226,
   233,   244,   252,   262,   268,   275,   276,   279,   286,   292,
   292,   298,   304,   305,   313,   314,   322,   331,   341,   342,
   350,   351,   359,   360,   371,   372,   387,   388,   398,   399,
   407,   408,   417,   418,   419,   422,   423,   426,   431,   432,
   433,   434,   437,   438,   439,   442,   445,   457,   460,   469
};
#endif


#if ASMDEBUG != 0 || defined (ASMERROR_VERBOSE)

static const char * const asmtname[] = {   "$","error","$undefined.","VIRGULE",
"DEUX_PTS","IMMEDIAT","DIRECT","IND_DEB","IND_FIN","SYMBL","LIGNE_DIR","BLOC",
"DATA","ALIGN","TEXT","GLOBL","ASCII","STATIC","SKIP","COMM","CHAR","STRING",
"INT_DIR","SHORT","FLOAT","DOUBLE","STRING_DIR","FILE_DIR","VERSION","NAME",
"ETIQ","NOMBRE","CODE_OP","SIGN","REGISTRE","FLOTTANT","prog","prolog","lignes",
"deb_lig","etiq","inst","@1","operandes","op","nombre_arg","int_arg","immediat",
"direct","lab_dep","reg","reg_ind","signe","reg_ind_d","directive","@2","@3",
"@4","@5","@6","@7","@8","@9","@10","@11","@12","op_dir","suite_op_dir","code_dir",
"global","chaine","case","branchs", NULL
};
#endif

static const short asmr1[] = {     0,
    36,    37,    38,    38,    38,    38,    39,    39,    40,    40,
    42,    41,    43,    43,    43,    44,    44,    44,    44,    44,
    44,    45,    46,    46,    46,    47,    47,    47,    47,    48,
    48,    49,    49,    50,    51,    52,    52,    53,    55,    54,
    56,    54,    57,    54,    58,    54,    54,    54,    59,    54,
    60,    54,    61,    54,    62,    54,    63,    54,    64,    54,
    65,    54,    54,    54,    54,    66,    66,    67,    67,    67,
    67,    67,    68,    68,    68,    69,    70,    71,    72,    72
};

static const short asmr2[] = {     0,
     2,     4,     2,     2,     2,     0,     1,     0,     2,     1,
     0,     4,     0,     3,     1,     1,     1,     1,     1,     1,
     1,     2,     1,     2,     2,     2,     2,     2,     1,     2,
     2,     3,     3,     1,     3,     0,     1,     2,     0,     4,
     0,     4,     0,     4,     0,     4,     3,     3,     0,     4,
     0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
     0,     4,     1,     1,     1,     3,     1,     2,     1,     1,
     2,     1,     1,     1,     1,     2,     1,     2,     2,     1
};

static const short asmdefact[] = {     0,
     0,     6,     0,     8,     0,    64,    65,     0,    10,     0,
     7,     4,     3,    63,     5,     2,    77,    76,    41,    61,
    73,    55,    74,    75,    57,    59,    43,    36,    45,    49,
    51,    53,    11,    39,     9,    80,    78,    36,    36,    36,
    36,    36,    36,     0,    37,    23,    47,     0,    48,    36,
    36,    36,    36,    13,    36,    79,     0,    69,    70,     0,
    42,    67,    72,    62,    56,    58,    60,    44,    24,    22,
    25,    46,    50,    52,    54,    36,    36,     0,    34,    12,
    15,     0,    16,    17,    19,    18,    20,    21,    29,    40,
    71,    68,    36,    27,    26,    28,    30,    31,     0,    36,
    38,    66,    32,    33,    35,    14,     0,     0,     0
};

static const short asmdefgoto[] = {   107,
     2,     4,    10,    11,    12,    54,    80,    81,    82,    47,
    83,    84,    85,    86,    87,    60,    88,    13,    55,    38,
    43,    50,    51,    52,    53,    40,    41,    42,    39,    61,
    62,    34,    14,    63,    15,    37
};

static const short asmpact[] = {    -6,
    -2,-32768,    -4,     1,     6,-32768,-32768,    -7,-32768,    80,
   -12,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,     5,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,    -7,     4,     4,     4,
     4,     4,     4,    -7,-32768,-32768,-32768,    -1,   -27,     4,
     4,     4,     4,    50,     4,-32768,    -7,-32768,-32768,     9,
    29,-32768,-32768,    29,    29,    29,    29,    29,-32768,-32768,
-32768,    29,    29,    29,    29,   -21,   -22,     7,-32768,    32,
-32768,    35,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    29,
-32768,-32768,     4,-32768,-32768,   -27,-32768,   -27,    36,    53,
-32768,-32768,-32768,-32768,-32768,-32768,    43,    45,-32768
};

static const short asmpgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -53,   -23,-32768,
-32768,-32768,-32768,   -30,   -20,   -26,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    68,
   -41,-32768,-32768,    -8,-32768,-32768
};


#define	ASMLAST		123


static const short asmtable[] = {    18,
    -1,    48,    36,   -36,    46,    45,    17,    17,    57,    44,
    45,    45,     6,    94,     7,     8,    17,    35,     3,    49,
     1,    17,    48,     5,    58,    71,    16,    48,    56,    70,
     9,    93,    17,    17,   100,    69,    45,    45,    59,    92,
    79,    78,   108,   105,   109,    89,   106,    99,    91,    48,
    48,   102,    95,    97,    76,    77,    78,    76,    77,    78,
     0,   101,     0,     0,     0,     0,     0,    96,    98,    48,
     0,    48,   103,    48,   104,     0,     0,     0,    17,     0,
   -36,    17,    45,    79,     0,    45,    79,     0,    19,    20,
    21,    89,    22,     0,     0,    23,    24,    25,    26,    27,
     0,    28,    29,    30,    31,    32,    64,    65,    66,    67,
    68,    33,     0,     0,     0,     0,     0,    72,    73,    74,
    75,     0,    90
};

static const short asmcheck[] = {     8,
     0,    28,    11,    31,    28,    33,    29,    29,     5,     5,
    33,    33,    12,    35,    14,    15,    29,    30,    21,    28,
    27,    29,    49,    28,    21,    49,    21,    54,    37,    31,
    30,     3,    29,    29,     3,    44,    33,    33,    35,    31,
    34,     7,     0,     8,     0,    54,   100,    78,    57,    76,
    77,    93,    76,    77,     5,     6,     7,     5,     6,     7,
    -1,    82,    -1,    -1,    -1,    -1,    -1,    76,    77,    96,
    -1,    98,    96,   100,    98,    -1,    -1,    -1,    29,    -1,
    31,    29,    33,    34,    -1,    33,    34,    -1,     9,    10,
    11,   100,    13,    -1,    -1,    16,    17,    18,    19,    20,
    -1,    22,    23,    24,    25,    26,    39,    40,    41,    42,
    43,    32,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
    53,    -1,    55
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef ASMSTACK_USE_ALLOCA
#ifdef alloca
#define ASMSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define ASMSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define ASMSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define ASMSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define ASMSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* ASMSTACK_USE_ALLOCA not defined */

#ifdef ASMSTACK_USE_ALLOCA
#define ASMSTACK_ALLOC alloca
#else
#define ASMSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define asmerrok		(asmerrstatus = 0)
#define asmclearin	(asmchar = ASMEMPTY)
#define ASMEMPTY		-2
#define ASMEOF		0
#define ASMACCEPT	goto asmacceptlab
#define ASMABORT 	goto asmabortlab
#define ASMERROR		goto asmerrlab1
/* Like ASMERROR except do call asmerror.
   This remains here temporarily to ease the
   transition to the new meaning of ASMERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define ASMFAIL		goto asmerrlab
#define ASMRECOVERING()  (!!asmerrstatus)
#define ASMBACKUP(token, value) \
do								\
  if (asmchar == ASMEMPTY && asmlen == 1)				\
    { asmchar = (token), asmlval = (value);			\
      asmchar1 = ASMTRANSLATE (asmchar);				\
      ASMPOPSTACK;						\
      goto asmbackup;						\
    }								\
  else								\
    { asmerror ("syntax error: cannot back up"); ASMERROR; }	\
while (0)

#define ASMTERROR	1
#define ASMERRCODE	256

#ifndef ASMPURE
#define ASMLEX		asmlex()
#endif

#ifdef ASMPURE
#ifdef ASMLSP_NEEDED
#ifdef ASMLEX_PARAM
#define ASMLEX		asmlex(&asmlval, &asmlloc, ASMLEX_PARAM)
#else
#define ASMLEX		asmlex(&asmlval, &asmlloc)
#endif
#else /* not ASMLSP_NEEDED */
#ifdef ASMLEX_PARAM
#define ASMLEX		asmlex(&asmlval, ASMLEX_PARAM)
#else
#define ASMLEX		asmlex(&asmlval)
#endif
#endif /* not ASMLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef ASMPURE

int	asmchar;			/*  the lookahead symbol		*/
ASMSTYPE	asmlval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef ASMLSP_NEEDED
ASMLTYPE asmlloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int asmnerrs;			/*  number of parse errors so far       */
#endif  /* not ASMPURE */

#if ASMDEBUG != 0
int asmdebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  ASMINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	ASMINITDEPTH
#define ASMINITDEPTH 200
#endif

/*  ASMMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if ASMMAXDEPTH == 0
#undef ASMMAXDEPTH
#endif

#ifndef ASMMAXDEPTH
#define ASMMAXDEPTH 10000
#endif

/* Define __asm_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __asm_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__asm_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__asm_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/lib/bison.simple"

/* The user can define ASMPARSE_PARAM as the name of an argument to be passed
   into asmparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef ASMPARSE_PARAM
#ifdef __cplusplus
#define ASMPARSE_PARAM_ARG void *ASMPARSE_PARAM
#define ASMPARSE_PARAM_DECL
#else /* not __cplusplus */
#define ASMPARSE_PARAM_ARG ASMPARSE_PARAM
#define ASMPARSE_PARAM_DECL void *ASMPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not ASMPARSE_PARAM */
#define ASMPARSE_PARAM_ARG
#define ASMPARSE_PARAM_DECL
#endif /* not ASMPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef ASMPARSE_PARAM
int asmparse (void *);
#else
int asmparse (void);
#endif
#endif

int
asmparse(ASMPARSE_PARAM_ARG)
     ASMPARSE_PARAM_DECL
{
  register int asmstate;
  register int asmn;
  register short *asmssp;
  register ASMSTYPE *asmvsp;
  int asmerrstatus;	/*  number of tokens to shift before error messages enabled */
  int asmchar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	asmssa[ASMINITDEPTH];	/*  the state stack			*/
  ASMSTYPE asmvsa[ASMINITDEPTH];	/*  the semantic value stack		*/

  short *asmss = asmssa;		/*  refer to the stacks thru separate pointers */
  ASMSTYPE *asmvs = asmvsa;	/*  to allow asmoverflow to reallocate them elsewhere */

#ifdef ASMLSP_NEEDED
  ASMLTYPE asmlsa[ASMINITDEPTH];	/*  the location stack			*/
  ASMLTYPE *asmls = asmlsa;
  ASMLTYPE *asmlsp;

#define ASMPOPSTACK   (asmvsp--, asmssp--, asmlsp--)
#else
#define ASMPOPSTACK   (asmvsp--, asmssp--)
#endif

  int asmstacksize = ASMINITDEPTH;
  int asmfree_stacks = 0;

#ifdef ASMPURE
  int asmchar;
  ASMSTYPE asmlval;
  int asmnerrs;
#ifdef ASMLSP_NEEDED
  ASMLTYPE asmlloc;
#endif
#endif

  ASMSTYPE asmval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int asmlen;

#if ASMDEBUG != 0
  if (asmdebug)
    fprintf(stderr, "Starting parse\n");
#endif

  asmstate = 0;
  asmerrstatus = 0;
  asmnerrs = 0;
  asmchar = ASMEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  asmssp = asmss - 1;
  asmvsp = asmvs;
#ifdef ASMLSP_NEEDED
  asmlsp = asmls;
#endif

/* Push a new state, which is found in  asmstate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
asmnewstate:

  *++asmssp = asmstate;

  if (asmssp >= asmss + asmstacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      ASMSTYPE *asmvs1 = asmvs;
      short *asmss1 = asmss;
#ifdef ASMLSP_NEEDED
      ASMLTYPE *asmls1 = asmls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = asmssp - asmss + 1;

#ifdef asmoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef ASMLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if asmoverflow is a macro.  */
      asmoverflow("parser stack overflow",
		 &asmss1, size * sizeof (*asmssp),
		 &asmvs1, size * sizeof (*asmvsp),
		 &asmls1, size * sizeof (*asmlsp),
		 &asmstacksize);
#else
      asmoverflow("parser stack overflow",
		 &asmss1, size * sizeof (*asmssp),
		 &asmvs1, size * sizeof (*asmvsp),
		 &asmstacksize);
#endif

      asmss = asmss1; asmvs = asmvs1;
#ifdef ASMLSP_NEEDED
      asmls = asmls1;
#endif
#else /* no asmoverflow */
      /* Extend the stack our own way.  */
      if (asmstacksize >= ASMMAXDEPTH)
	{
	  asmerror("parser stack overflow");
	  if (asmfree_stacks)
	    {
	      free (asmss);
	      free (asmvs);
#ifdef ASMLSP_NEEDED
	      free (asmls);
#endif
	    }
	  return 2;
	}
      asmstacksize *= 2;
      if (asmstacksize > ASMMAXDEPTH)
	asmstacksize = ASMMAXDEPTH;
#ifndef ASMSTACK_USE_ALLOCA
      asmfree_stacks = 1;
#endif
      asmss = (short *) ASMSTACK_ALLOC (asmstacksize * sizeof (*asmssp));
      __asm_memcpy ((char *)asmss, (char *)asmss1,
		   size * (unsigned int) sizeof (*asmssp));
      asmvs = (ASMSTYPE *) ASMSTACK_ALLOC (asmstacksize * sizeof (*asmvsp));
      __asm_memcpy ((char *)asmvs, (char *)asmvs1,
		   size * (unsigned int) sizeof (*asmvsp));
#ifdef ASMLSP_NEEDED
      asmls = (ASMLTYPE *) ASMSTACK_ALLOC (asmstacksize * sizeof (*asmlsp));
      __asm_memcpy ((char *)asmls, (char *)asmls1,
		   size * (unsigned int) sizeof (*asmlsp));
#endif
#endif /* no asmoverflow */

      asmssp = asmss + size - 1;
      asmvsp = asmvs + size - 1;
#ifdef ASMLSP_NEEDED
      asmlsp = asmls + size - 1;
#endif

#if ASMDEBUG != 0
      if (asmdebug)
	fprintf(stderr, "Stack size increased to %d\n", asmstacksize);
#endif

      if (asmssp >= asmss + asmstacksize - 1)
	ASMABORT;
    }

#if ASMDEBUG != 0
  if (asmdebug)
    fprintf(stderr, "Entering state %d\n", asmstate);
#endif

  goto asmbackup;
 asmbackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* asmresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  asmn = asmpact[asmstate];
  if (asmn == ASMFLAG)
    goto asmdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* asmchar is either ASMEMPTY or ASMEOF
     or a valid token in external form.  */

  if (asmchar == ASMEMPTY)
    {
#if ASMDEBUG != 0
      if (asmdebug)
	fprintf(stderr, "Reading a token: ");
#endif
      asmchar = ASMLEX;
    }

  /* Convert token to internal form (in asmchar1) for indexing tables with */

  if (asmchar <= 0)		/* This means end of input. */
    {
      asmchar1 = 0;
      asmchar = ASMEOF;		/* Don't call ASMLEX any more */

#if ASMDEBUG != 0
      if (asmdebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      asmchar1 = ASMTRANSLATE(asmchar);

#if ASMDEBUG != 0
      if (asmdebug)
	{
	  fprintf (stderr, "Next token is %d (%s", asmchar, asmtname[asmchar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef ASMPRINT
	  ASMPRINT (stderr, asmchar, asmlval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  asmn += asmchar1;
  if (asmn < 0 || asmn > ASMLAST || asmcheck[asmn] != asmchar1)
    goto asmdefault;

  asmn = asmtable[asmn];

  /* asmn is what to do for this token type in this state.
     Negative => reduce, -asmn is rule number.
     Positive => shift, asmn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (asmn < 0)
    {
      if (asmn == ASMFLAG)
	goto asmerrlab;
      asmn = -asmn;
      goto asmreduce;
    }
  else if (asmn == 0)
    goto asmerrlab;

  if (asmn == ASMFINAL)
    ASMACCEPT;

  /* Shift the lookahead token.  */

#if ASMDEBUG != 0
  if (asmdebug)
    fprintf(stderr, "Shifting token %d (%s), ", asmchar, asmtname[asmchar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (asmchar != ASMEOF)
    asmchar = ASMEMPTY;

  *++asmvsp = asmlval;
#ifdef ASMLSP_NEEDED
  *++asmlsp = asmlloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (asmerrstatus) asmerrstatus--;

  asmstate = asmn;
  goto asmnewstate;

/* Do the default action for the current state.  */
asmdefault:

  asmn = asmdefact[asmstate];
  if (asmn == 0)
    goto asmerrlab;

/* Do a reduction.  asmn is the number of a rule to reduce with.  */
asmreduce:
  asmlen = asmr2[asmn];
  if (asmlen > 0)
    asmval = asmvsp[1-asmlen]; /* implement default value of the action */

#if ASMDEBUG != 0
  if (asmdebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       asmn, asmrline[asmn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = asmprhs[asmn]; asmrhs[i] > 0; i++)
	fprintf (stderr, "%s ", asmtname[asmrhs[i]]);
      fprintf (stderr, " -> %s\n", asmtname[asmr1[asmn]]);
    }
#endif


  switch (asmn) {

case 2:
#line 79 "d_asm.yac"
{
					mbkfree(asmvsp[-2].str);
					mbkfree(asmvsp[0].str);
				;
    break;}
case 7:
#line 91 "d_asm.yac"
{asmval.val = asmvsp[0].val;;
    break;}
case 8:
#line 92 "d_asm.yac"
{asmval.val = -1 ;;
    break;}
case 9:
#line 96 "d_asm.yac"
{
					/*mbkfree($1);  never used, but previously allocated */
					if (InData) {
						asmval.val = GereTabSymb(asmvsp[0].str, taille_data, TRUE);
						mbkfree(asmvsp[0].str);
					} else {
						asmval.val = GereTabSymb(asmvsp[0].str, taille_code, FALSE);
                  /* can't figure out why this code was here!
						if (!strcmp(nom_glob, $2))
							nom_fonc = $2;
						else
						*/
							mbkfree(asmvsp[0].str);
					}	
				;
    break;}
case 10:
#line 112 "d_asm.yac"
{
					if (InData) {
						asmval.val = GereTabSymb(asmvsp[0].str, taille_data, TRUE);
						mbkfree(asmvsp[0].str);
			   	} else {
						asmval.val = GereTabSymb(asmvsp[0].str, taille_code, FALSE);
						if (!strcmp(nom_glob, asmvsp[0].str)) {
							nom_fonc = asmvsp[0].str;
							for (function = First_fonction; function;
									function = function->next_fonc)
								save_function = function;

							function = (ptfonc)mbkalloc(sizeof(fonc));
							function->name = nom_fonc;
							function->code_adr = taille_code;
							function->next_fonc = 0;
							if (save_function)
								save_function->next_fonc = function;
							else
								First_fonction = function;
						} else
							mbkfree(asmvsp[0].str);
					}
				;
    break;}
case 11:
#line 139 "d_asm.yac"
{
					num_op = 0 ;
					nb_op_suiv = 0 ;
					inst.inst.code_inst = asmvsp[0].val ;
				;
    break;}
case 12:
#line 145 "d_asm.yac"
{
					new_taille_code =
						IncTailleTab(&taille_code, (nb_op_suiv+1) * 4,
											&taille_max_code, BLOC_CODE, &I_Code, 4);
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_code, FALSE);
					for (j = 0, i = taille_code; i < taille_code+4+nb_op_suiv*4;)
						I_Code[i++] = inst.tab_inst[j++];
					taille_code = new_taille_code;
				;
    break;}
case 22:
#line 169 "d_asm.yac"
{
					asmval.val = asmvsp[-1].val == '-' ? -asmvsp[0].val : asmvsp[0].val;
				;
    break;}
case 23:
#line 175 "d_asm.yac"
{
					asmval.val = asmvsp[0].val;
				;
    break;}
case 24:
#line 179 "d_asm.yac"
{
					IncTailleTab(&taille_data, 0, &taille_max_data,
										BLOC_DATA, &I_Data, 4);
					AddRef(taille_data, 0, FALSE);
					asmval.val = asmvsp[0].val;
				;
    break;}
case 25:
#line 186 "d_asm.yac"
{
					IncTailleTab(&taille_data, 0, &taille_max_data,
										BLOC_DATA, &I_Data, 4);
					AddRef(taille_data, asmvsp[0].val ,FALSE);
					asmval.val = asmvsp[0].val;
				;
    break;}
case 26:
#line 195 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_IMM;
					inst.inst.op[nb_op_suiv] = asmvsp[0].val;
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 27:
#line 202 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_IMM;
					*((float *)&(inst.inst.op[nb_op_suiv])) = (float)asmvsp[0].fl;
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 28:
#line 209 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_IMM;
					inst.inst.op[nb_op_suiv] = asmvsp[0].val ;
					AddRef(taille_code+4+nb_op_suiv * 4,0,TRUE);
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 29:
#line 217 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_IMM;
					inst.inst.op[nb_op_suiv] = asmvsp[0].val;
					AddRef(taille_code+4+nb_op_suiv * 4,0,TRUE);
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 30:
#line 227 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_DIRECT;
					inst.inst.op[nb_op_suiv] = asmvsp[0].val;
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 31:
#line 234 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_DIRECT;
					inst.inst.op[nb_op_suiv] = asmvsp[0].val;
					AddRef(taille_code+4+nb_op_suiv * 4,0,TRUE);
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 32:
#line 245 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_IMM;
					inst.inst.op[nb_op_suiv] = asmvsp[-1].val;
					AddRef(taille_code+4+nb_op_suiv*4, asmvsp[0].val ,TRUE);
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 33:
#line 253 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_DIRECT;
					inst.inst.op[nb_op_suiv] = asmvsp[-1].val;
					AddRef(taille_code+4+nb_op_suiv*4, asmvsp[0].val ,TRUE);
					num_op++;
					nb_op_suiv++;
				;
    break;}
case 34:
#line 263 "d_asm.yac"
{
					inst.inst.mode_op[num_op] = MODE_REG | asmvsp[0].val;
					num_op++;
				;
    break;}
case 35:
#line 269 "d_asm.yac"
{
					inst.inst.mode_op[num_op-1] &= 0x1F;
					inst.inst.mode_op[num_op-1] |= MODE_IND_REG;
				;
    break;}
case 36:
#line 275 "d_asm.yac"
{asmval.val = ' ';;
    break;}
case 37:
#line 276 "d_asm.yac"
{asmval.val = asmvsp[0].val;;
    break;}
case 38:
#line 280 "d_asm.yac"
{
					inst.inst.mode_op[num_op-1] |= MODE_IND_REG_D;
					inst.inst.op[nb_op_suiv] = asmvsp[-1].val;
					nb_op_suiv++;
				;
    break;}
case 39:
#line 287 "d_asm.yac"
{
					GereTabSymb(SymbInTab(asmvsp[-1].val), taille_code, FALSE);
					nb_op_dir = 0;
				;
    break;}
case 41:
#line 293 "d_asm.yac"
{
					GereTabSymb(SymbInTab(asmvsp[-1].val), taille_code, FALSE);
					nb_op_dir = 0;
				;
    break;}
case 42:
#line 298 "d_asm.yac"
{
					/* memory :
					   op_dir[0].str is freed inside d_dbx.yac, because it's
					   easier when I read more than a single line. */
					AnalysDebug(op_dir[0].str, op_dir[1].val, op_dir[4].val);
				;
    break;}
case 43:
#line 304 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 44:
#line 305 "d_asm.yac"
{
					new_taille_data =
							IncTailleTab(&taille_data, 1, &taille_max_data,
												BLOC_DATA, &I_Data, 1);
					I_Data[taille_data] = (char)op_dir[0].val;
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_data, TRUE);
					taille_data = new_taille_data;
				;
    break;}
case 45:
#line 313 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 46:
#line 314 "d_asm.yac"
{
					new_taille_data =
							IncTailleTab(&taille_data, 2, &taille_max_data,
												BLOC_DATA, &I_Data, 4);
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_data, TRUE);
					*((short *)&(I_Data[taille_data])) =(short)op_dir[0].val;
					taille_data = new_taille_data;
				;
    break;}
case 47:
#line 323 "d_asm.yac"
{
					new_taille_data =
							IncTailleTab(&taille_data, 4, &taille_max_data,
												BLOC_DATA, &I_Data, 4);
					GereTabSymb(SymbInTab(asmvsp[-2].val), taille_data, TRUE);
					*((int *)&(I_Data[taille_data])) = asmvsp[0].val;
					taille_data = new_taille_data;
				;
    break;}
case 48:
#line 332 "d_asm.yac"
{
					new_taille_data =
							IncTailleTab(&taille_data, 4, &taille_max_data,
												BLOC_DATA, &I_Data, 4);
					GereTabSymb(SymbInTab(asmvsp[-2].val), taille_data, TRUE);
					AddRef(taille_data, 0, FALSE);
					*((int *)&(I_Data[taille_data])) = asmvsp[0].val;
					taille_data = new_taille_data;
				;
    break;}
case 49:
#line 341 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 50:
#line 342 "d_asm.yac"
{
					new_taille_data =
							IncTailleTab(&taille_data, 4, &taille_max_data,
												BLOC_DATA, &I_Data, 4);
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_data, TRUE);
					*((float *)&(I_Data[taille_data]))=(float)op_dir[0].dbl;
					taille_data = new_taille_data;
				;
    break;}
case 51:
#line 350 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 52:
#line 351 "d_asm.yac"
{
					new_taille_data =
							IncTailleTab(&taille_data, 8, &taille_max_data,
												BLOC_DATA, &I_Data, 4);
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_data, TRUE);
					*((double *)&(I_Data[taille_data])) = op_dir[0].dbl;
					taille_data = new_taille_data;
				;
    break;}
case 53:
#line 359 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 54:
#line 360 "d_asm.yac"
{
					ConvOct(op_dir[0].str, op_dir[0].str);
					new_taille_data =
							IncTailleTab(&taille_data, strlen(op_dir[0].str) + 1,
												&taille_max_data, BLOC_DATA, &I_Data, 1);
					strcpy(&(I_Data[taille_data]), op_dir[0].str);
					I_Data[taille_data + strlen(op_dir[0].str) + 1] =  '\0';
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_data, TRUE);
					taille_data = new_taille_data;
					mbkfree((void *)op_dir[0].str);
				;
    break;}
case 55:
#line 371 "d_asm.yac"
{ nb_op_dir = 0;;
    break;}
case 56:
#line 372 "d_asm.yac"
{
					if (InData) {
						if (taille_data % op_dir[0].val)
							taille_data = IncTailleTab(&taille_data,
									((taille_data + op_dir[0].val - 1) / op_dir[0].val)
										* op_dir[0].val-taille_data,
									&taille_max_data, BLOC_DATA, &I_Data, 1);
					} else {
						if (taille_code % op_dir[0].val)
							taille_code = IncTailleTab(&taille_code,
									((taille_code + op_dir[0].val - 1) / op_dir[0].val)
										* op_dir[0].val - taille_code,
									&taille_max_code, BLOC_CODE, &I_Code, 1);
					}
				;
    break;}
case 57:
#line 387 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 58:
#line 388 "d_asm.yac"
{
					if(InData)
						taille_data =
							IncTailleTab(&taille_data, op_dir[0].val,
												&taille_max_data, BLOC_DATA, &I_Data, 1);
				  	else
						taille_code =
							IncTailleTab(&taille_code, op_dir[0].val,
												&taille_max_code, BLOC_CODE, &I_Code, 1);
				;
    break;}
case 59:
#line 398 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 60:
#line 399 "d_asm.yac"
{
					new_taille_data =
							IncTailleTab(&taille_data, op_dir[1].val,
												&taille_max_data, BLOC_DATA, &I_Data, 4);
					GereTabSymb(SymbInTab(op_dir[0].val), taille_data, TRUE);
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_data, TRUE);
					taille_data = new_taille_data;
				;
    break;}
case 61:
#line 407 "d_asm.yac"
{nb_op_dir = 0;;
    break;}
case 62:
#line 408 "d_asm.yac"
{
					new_taille_code =
							IncTailleTab(&taille_code, 4, &taille_max_code,
												BLOC_CODE, &I_Code, 4);
					I_Code[taille_code] = LIGNE;
					*((unsigned short *)&(I_Code[taille_code + 2])) = op_dir[2].val;
					GereTabSymb(SymbInTab(asmvsp[-3].val), taille_code, FALSE);
					taille_code = new_taille_code;
				;
    break;}
case 63:
#line 417 "d_asm.yac"
{nom_glob = SymbInTab(asmvsp[0].val);;
    break;}
case 64:
#line 418 "d_asm.yac"
{InData = TRUE;;
    break;}
case 65:
#line 419 "d_asm.yac"
{InData = FALSE;;
    break;}
case 68:
#line 427 "d_asm.yac"
{
					op_dir[nb_op_dir].val = asmvsp[-1].val == '-' ? -asmvsp[0].val : asmvsp[0].val;
					nb_op_dir++;
				;
    break;}
case 69:
#line 431 "d_asm.yac"
{op_dir[nb_op_dir].str = asmvsp[0].str; nb_op_dir++;;
    break;}
case 70:
#line 432 "d_asm.yac"
{op_dir[nb_op_dir].dbl = asmvsp[0].fl; nb_op_dir++;;
    break;}
case 71:
#line 433 "d_asm.yac"
{op_dir[nb_op_dir].val = asmvsp[0].val; nb_op_dir++;;
    break;}
case 72:
#line 434 "d_asm.yac"
{op_dir[nb_op_dir].val = asmvsp[0].val; nb_op_dir++;;
    break;}
case 76:
#line 442 "d_asm.yac"
{asmval.val = asmvsp[0].val;;
    break;}
case 77:
#line 446 "d_asm.yac"
{
					if (inst.inst.code_inst <= CALL && inst.inst.code_inst >= BRA) {
						asmval.val = GereTabSymb(asmvsp[0].str, -1, FALSE);
						mbkfree((void *)asmvsp[0].str);
					} else {
						asmval.val = GereTabSymb(asmvsp[0].str, -1, UNKNOWN);
						mbkfree((void *)asmvsp[0].str);
					}
				;
    break;}
case 79:
#line 461 "d_asm.yac"
{
					new_taille_code =
							IncTailleTab(&taille_code, 4, &taille_max_code,
													BLOC_CODE, &I_Code, 4);
					*((int *)&(I_Code[taille_code])) = asmvsp[0].val;
					AddRef(taille_code, 0 , TRUE);
					taille_code = new_taille_code;
				;
    break;}
case 80:
#line 470 "d_asm.yac"
{
					new_taille_code =
							IncTailleTab(&taille_code, 4, &taille_max_code,
												BLOC_CODE, &I_Code, 4);
					*((int *)&(I_Code[taille_code])) = asmvsp[0].val;
					AddRef(taille_code, 0, TRUE);
					taille_code = new_taille_code;
				;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  asmvsp -= asmlen;
  asmssp -= asmlen;
#ifdef ASMLSP_NEEDED
  asmlsp -= asmlen;
#endif

#if ASMDEBUG != 0
  if (asmdebug)
    {
      short *ssp1 = asmss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != asmssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++asmvsp = asmval;

#ifdef ASMLSP_NEEDED
  asmlsp++;
  if (asmlen == 0)
    {
      asmlsp->first_line = asmlloc.first_line;
      asmlsp->first_column = asmlloc.first_column;
      asmlsp->last_line = (asmlsp-1)->last_line;
      asmlsp->last_column = (asmlsp-1)->last_column;
      asmlsp->text = 0;
    }
  else
    {
      asmlsp->last_line = (asmlsp+asmlen-1)->last_line;
      asmlsp->last_column = (asmlsp+asmlen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  asmn = asmr1[asmn];

  asmstate = asmpgoto[asmn - ASMNTBASE] + *asmssp;
  if (asmstate >= 0 && asmstate <= ASMLAST && asmcheck[asmstate] == *asmssp)
    asmstate = asmtable[asmstate];
  else
    asmstate = asmdefgoto[asmn - ASMNTBASE];

  goto asmnewstate;

asmerrlab:   /* here on detecting error */

  if (! asmerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++asmnerrs;

#ifdef ASMERROR_VERBOSE
      asmn = asmpact[asmstate];

      if (asmn > ASMFLAG && asmn < ASMLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -asmn if nec to avoid negative indexes in asmcheck.  */
	  for (x = (asmn < 0 ? -asmn : 0);
	       x < (sizeof(asmtname) / sizeof(char *)); x++)
	    if (asmcheck[x + asmn] == x)
	      size += strlen(asmtname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (asmn < 0 ? -asmn : 0);
		       x < (sizeof(asmtname) / sizeof(char *)); x++)
		    if (asmcheck[x + asmn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, asmtname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      asmerror(msg);
	      free(msg);
	    }
	  else
	    asmerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* ASMERROR_VERBOSE */
	asmerror("parse error");
    }

  goto asmerrlab1;
asmerrlab1:   /* here on error raised explicitly by an action */

  if (asmerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (asmchar == ASMEOF)
	ASMABORT;

#if ASMDEBUG != 0
      if (asmdebug)
	fprintf(stderr, "Discarding token %d (%s).\n", asmchar, asmtname[asmchar1]);
#endif

      asmchar = ASMEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  asmerrstatus = 3;		/* Each real token shifted decrements this */

  goto asmerrhandle;

asmerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  asmn = asmdefact[asmstate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (asmn) goto asmdefault;
#endif

asmerrpop:   /* pop the current state because it cannot handle the error token */

  if (asmssp == asmss) ASMABORT;
  asmvsp--;
  asmstate = *--asmssp;
#ifdef ASMLSP_NEEDED
  asmlsp--;
#endif

#if ASMDEBUG != 0
  if (asmdebug)
    {
      short *ssp1 = asmss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != asmssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

asmerrhandle:

  asmn = asmpact[asmstate];
  if (asmn == ASMFLAG)
    goto asmerrdefault;

  asmn += ASMTERROR;
  if (asmn < 0 || asmn > ASMLAST || asmcheck[asmn] != ASMTERROR)
    goto asmerrdefault;

  asmn = asmtable[asmn];
  if (asmn < 0)
    {
      if (asmn == ASMFLAG)
	goto asmerrpop;
      asmn = -asmn;
      goto asmreduce;
    }
  else if (asmn == 0)
    goto asmerrpop;

  if (asmn == ASMFINAL)
    ASMACCEPT;

#if ASMDEBUG != 0
  if (asmdebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++asmvsp = asmlval;
#ifdef ASMLSP_NEEDED
  *++asmlsp = asmlloc;
#endif

  asmstate = asmn;
  goto asmnewstate;

 asmacceptlab:
  /* ASMACCEPT comes here.  */
  if (asmfree_stacks)
    {
      free (asmss);
      free (asmvs);
#ifdef ASMLSP_NEEDED
      free (asmls);
#endif
    }
  return 0;

 asmabortlab:
  /* ASMABORT comes here.  */
  if (asmfree_stacks)
    {
      free (asmss);
      free (asmvs);
#ifdef ASMLSP_NEEDED
      free (asmls);
#endif
    }
  return 1;
}
#line 480 "d_asm.yac"

