
/*  A Bison parser, made from d_dbx.yac
    by GNU Bison version 1.28  */

#define D_DBXBISON 1  /* Identify Bison output.  */

#define	SYMBOL	257
#define	INTEGER	258
#define	FLAG	259

#line 1 "d_dbx.yac"

/* dbx syntax parser : Frederic Petrot, december 91 */
/* version 1.0 written in C and failed because of my lack of knowledge */
/* in writting recursive grammar parsers */
/* lexical analyser in C, very context dependant */
/* rules analyse, yacc */
/* syntax extracted from a sample of outputs of C headers and files */
/* thru the use of gcc -S -g */
#include <stdio.h>
#include <string.h>
#include MUT_H
#include "d_dbx.h"

#define TERMINAL    0
#define STRUCTURE   1 /* also good for union, only fields address change */
#define TYPEDEF     4
#define POINTER     8
#define ARRAY       16
#define FUNCTION    32
#define ENUMERATION 64
#define XREF        128

static char *dbx_line; /* dbx line to be analyzed, not splitted by '\\' */

PTVARIABLE firstVariable = NULL; /* list of variables */
PTSTRUCT firstStruct = NULL; /* list of structures */
static PTSTRUCT gst; /* current global structure to work on */
static PTSTRUCT ost; /* previous global structure to work on */
static PTSTRUCT fst; /* previous global structure to work on */
static PTVARIABLE gv; /* current global variable to work on */
static PTELEMENT elm, e; /* current element */

static char *name; /* current structure/variable name */
static int varAddress;
static int varClass;
static PTELEMENT create_element();
static PTSTRUCT create_struct();
static void delete_struct();
static PTVARIABLE create_variable();
static void fill_dbx_info();
static int d_dbxlex();

#line 44 "d_dbx.yac"
typedef union {
   int integer;
   char string[255]; /* why 255? Good question! */
} D_DBXSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	D_DBXFINAL		52
#define	D_DBXFLAG		-32768
#define	D_DBXNTBASE	17

#define D_DBXTRANSLATE(x) ((unsigned)(x) <= 259 ? d_dbxtranslate[x] : 26)

static const char d_dbxtranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     8,     2,    16,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     6,    11,     2,
     7,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,    15,     2,     2,     2,
    10,     9,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,    13,     2,    14,     2,     2,    12,
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
     2,     2,     2,     2,     2,     1,     3,     4,     5
};

#if D_DBXDEBUG != 0
static const short d_dbxprhs[] = {     0,
     0,     2,     4,     5,    11,    12,    14,    16,    21,    26,
    32,    38,    44,    50,    60,    62,    65,    74,    84,    86,
    89
};

static const short d_dbxrhs[] = {    18,
     0,     3,     0,     0,     3,    19,     6,    20,    21,     0,
     0,     5,     0,     4,     0,     4,     7,     8,    21,     0,
     4,     7,     9,    21,     0,     4,     7,    10,    24,    11,
     0,     4,     7,    12,     3,     6,     0,     4,     7,    13,
     4,    22,     0,     4,     7,    14,     4,    22,     0,     4,
     7,    15,    11,     4,    11,     4,    11,    21,     0,    23,
     0,    23,    22,     0,     3,     6,    21,    16,     4,    16,
     4,    11,     0,     3,     6,    21,    16,     4,    16,     4,
    11,    11,     0,    25,     0,    25,    24,     0,     3,     6,
     4,    16,     0
};

#endif

#if D_DBXDEBUG != 0
static const short d_dbxrline[] = { 0,
    56,    57,    60,    60,    77,    78,    82,    88,   102,   116,
   121,   126,   143,   160,   176,   177,   180,   192,   206,   207,
   210
};
#endif


#if D_DBXDEBUG != 0 || defined (D_DBXERROR_VERBOSE)

static const char * const d_dbxtname[] = {   "$","error","$undefined.","SYMBOL",
"INTEGER","FLAG","':'","'='","'*'","'f'","'e'","';'","'x'","'s'","'u'","'a'",
"','","dbx","line","@1","class","type","fields","field","lists","list", NULL
};
#endif

static const short d_dbxr1[] = {     0,
    17,    17,    19,    18,    20,    20,    21,    21,    21,    21,
    21,    21,    21,    21,    22,    22,    23,    23,    24,    24,
    25
};

static const short d_dbxr2[] = {     0,
     1,     1,     0,     5,     0,     1,     1,     4,     4,     5,
     5,     5,     5,     9,     1,     2,     8,     9,     1,     2,
     4
};

static const short d_dbxdefact[] = {     0,
     2,     1,     0,     5,     6,     0,     7,     4,     0,     0,
     0,     0,     0,     0,     0,     0,     8,     9,     0,     0,
    19,     0,     0,     0,     0,     0,    10,    20,    11,     0,
    12,    15,    13,     0,     0,     0,    16,     0,    21,     0,
     0,     0,     0,     0,    14,     0,     0,    17,    18,     0,
     0,     0
};

static const short d_dbxdefgoto[] = {    50,
     2,     3,     6,     8,    31,    32,    20,    21
};

static const short d_dbxpact[] = {     0,
    -2,-32768,     9,     1,-32768,    12,    10,-32768,    -1,    12,
    12,     2,    15,    16,    17,     8,-32768,-32768,    18,    11,
     2,    19,    20,    20,    23,    24,-32768,-32768,-32768,    25,
-32768,    20,-32768,    21,    13,    12,-32768,    26,-32768,    22,
    28,    30,    12,    27,-32768,    31,    29,    33,-32768,    36,
    37,-32768
};

static const short d_dbxpgoto[] = {-32768,
-32768,-32768,-32768,   -10,   -22,-32768,    32,-32768
};


#define	D_DBXLAST		53


static const short d_dbxtable[] = {    17,
    18,    33,     1,    -3,    19,     5,    10,    11,    12,    37,
    13,    14,    15,    16,     4,     7,     9,    22,    25,    23,
    24,    27,    30,    26,    29,    40,    34,    35,    39,    41,
    36,    38,    45,    44,    47,    51,    52,    42,    43,    48,
     0,     0,    46,    49,     0,     0,     0,     0,     0,     0,
     0,     0,    28
};

static const short d_dbxcheck[] = {    10,
    11,    24,     3,     6,     3,     5,     8,     9,    10,    32,
    12,    13,    14,    15,     6,     4,     7,     3,    11,     4,
     4,    11,     3,     6,     6,    36,     4,     4,    16,     4,
     6,    11,    43,     4,     4,     0,     0,    16,    11,    11,
    -1,    -1,    16,    11,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    21
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

#ifndef D_DBXSTACK_USE_ALLOCA
#ifdef alloca
#define D_DBXSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define D_DBXSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define D_DBXSTACK_USE_ALLOCA
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
#define D_DBXSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define D_DBXSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* D_DBXSTACK_USE_ALLOCA not defined */

#ifdef D_DBXSTACK_USE_ALLOCA
#define D_DBXSTACK_ALLOC alloca
#else
#define D_DBXSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define d_dbxerrok		(d_dbxerrstatus = 0)
#define d_dbxclearin	(d_dbxchar = D_DBXEMPTY)
#define D_DBXEMPTY		-2
#define D_DBXEOF		0
#define D_DBXACCEPT	goto d_dbxacceptlab
#define D_DBXABORT 	goto d_dbxabortlab
#define D_DBXERROR		goto d_dbxerrlab1
/* Like D_DBXERROR except do call d_dbxerror.
   This remains here temporarily to ease the
   transition to the new meaning of D_DBXERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define D_DBXFAIL		goto d_dbxerrlab
#define D_DBXRECOVERING()  (!!d_dbxerrstatus)
#define D_DBXBACKUP(token, value) \
do								\
  if (d_dbxchar == D_DBXEMPTY && d_dbxlen == 1)				\
    { d_dbxchar = (token), d_dbxlval = (value);			\
      d_dbxchar1 = D_DBXTRANSLATE (d_dbxchar);				\
      D_DBXPOPSTACK;						\
      goto d_dbxbackup;						\
    }								\
  else								\
    { d_dbxerror ("syntax error: cannot back up"); D_DBXERROR; }	\
while (0)

#define D_DBXTERROR	1
#define D_DBXERRCODE	256

#ifndef D_DBXPURE
#define D_DBXLEX		d_dbxlex()
#endif

#ifdef D_DBXPURE
#ifdef D_DBXLSP_NEEDED
#ifdef D_DBXLEX_PARAM
#define D_DBXLEX		d_dbxlex(&d_dbxlval, &d_dbxlloc, D_DBXLEX_PARAM)
#else
#define D_DBXLEX		d_dbxlex(&d_dbxlval, &d_dbxlloc)
#endif
#else /* not D_DBXLSP_NEEDED */
#ifdef D_DBXLEX_PARAM
#define D_DBXLEX		d_dbxlex(&d_dbxlval, D_DBXLEX_PARAM)
#else
#define D_DBXLEX		d_dbxlex(&d_dbxlval)
#endif
#endif /* not D_DBXLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef D_DBXPURE

int	d_dbxchar;			/*  the lookahead symbol		*/
D_DBXSTYPE	d_dbxlval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef D_DBXLSP_NEEDED
D_DBXLTYPE d_dbxlloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int d_dbxnerrs;			/*  number of parse errors so far       */
#endif  /* not D_DBXPURE */

#if D_DBXDEBUG != 0
int d_dbxdebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  D_DBXINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	D_DBXINITDEPTH
#define D_DBXINITDEPTH 200
#endif

/*  D_DBXMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if D_DBXMAXDEPTH == 0
#undef D_DBXMAXDEPTH
#endif

#ifndef D_DBXMAXDEPTH
#define D_DBXMAXDEPTH 10000
#endif

/* Define __d_dbx_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __d_dbx_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__d_dbx_memcpy (to, from, count)
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
__d_dbx_memcpy (char *to, char *from, unsigned int count)
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

/* The user can define D_DBXPARSE_PARAM as the name of an argument to be passed
   into d_dbxparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef D_DBXPARSE_PARAM
#ifdef __cplusplus
#define D_DBXPARSE_PARAM_ARG void *D_DBXPARSE_PARAM
#define D_DBXPARSE_PARAM_DECL
#else /* not __cplusplus */
#define D_DBXPARSE_PARAM_ARG D_DBXPARSE_PARAM
#define D_DBXPARSE_PARAM_DECL void *D_DBXPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not D_DBXPARSE_PARAM */
#define D_DBXPARSE_PARAM_ARG
#define D_DBXPARSE_PARAM_DECL
#endif /* not D_DBXPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef D_DBXPARSE_PARAM
int d_dbxparse (void *);
#else
int d_dbxparse (void);
#endif
#endif

int
d_dbxparse(D_DBXPARSE_PARAM_ARG)
     D_DBXPARSE_PARAM_DECL
{
  register int d_dbxstate;
  register int d_dbxn;
  register short *d_dbxssp;
  register D_DBXSTYPE *d_dbxvsp;
  int d_dbxerrstatus;	/*  number of tokens to shift before error messages enabled */
  int d_dbxchar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	d_dbxssa[D_DBXINITDEPTH];	/*  the state stack			*/
  D_DBXSTYPE d_dbxvsa[D_DBXINITDEPTH];	/*  the semantic value stack		*/

  short *d_dbxss = d_dbxssa;		/*  refer to the stacks thru separate pointers */
  D_DBXSTYPE *d_dbxvs = d_dbxvsa;	/*  to allow d_dbxoverflow to reallocate them elsewhere */

#ifdef D_DBXLSP_NEEDED
  D_DBXLTYPE d_dbxlsa[D_DBXINITDEPTH];	/*  the location stack			*/
  D_DBXLTYPE *d_dbxls = d_dbxlsa;
  D_DBXLTYPE *d_dbxlsp;

#define D_DBXPOPSTACK   (d_dbxvsp--, d_dbxssp--, d_dbxlsp--)
#else
#define D_DBXPOPSTACK   (d_dbxvsp--, d_dbxssp--)
#endif

  int d_dbxstacksize = D_DBXINITDEPTH;
  int d_dbxfree_stacks = 0;

#ifdef D_DBXPURE
  int d_dbxchar;
  D_DBXSTYPE d_dbxlval;
  int d_dbxnerrs;
#ifdef D_DBXLSP_NEEDED
  D_DBXLTYPE d_dbxlloc;
#endif
#endif

  D_DBXSTYPE d_dbxval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int d_dbxlen;

#if D_DBXDEBUG != 0
  if (d_dbxdebug)
    fprintf(stderr, "Starting parse\n");
#endif

  d_dbxstate = 0;
  d_dbxerrstatus = 0;
  d_dbxnerrs = 0;
  d_dbxchar = D_DBXEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  d_dbxssp = d_dbxss - 1;
  d_dbxvsp = d_dbxvs;
#ifdef D_DBXLSP_NEEDED
  d_dbxlsp = d_dbxls;
#endif

/* Push a new state, which is found in  d_dbxstate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
d_dbxnewstate:

  *++d_dbxssp = d_dbxstate;

  if (d_dbxssp >= d_dbxss + d_dbxstacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      D_DBXSTYPE *d_dbxvs1 = d_dbxvs;
      short *d_dbxss1 = d_dbxss;
#ifdef D_DBXLSP_NEEDED
      D_DBXLTYPE *d_dbxls1 = d_dbxls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = d_dbxssp - d_dbxss + 1;

#ifdef d_dbxoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef D_DBXLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if d_dbxoverflow is a macro.  */
      d_dbxoverflow("parser stack overflow",
		 &d_dbxss1, size * sizeof (*d_dbxssp),
		 &d_dbxvs1, size * sizeof (*d_dbxvsp),
		 &d_dbxls1, size * sizeof (*d_dbxlsp),
		 &d_dbxstacksize);
#else
      d_dbxoverflow("parser stack overflow",
		 &d_dbxss1, size * sizeof (*d_dbxssp),
		 &d_dbxvs1, size * sizeof (*d_dbxvsp),
		 &d_dbxstacksize);
#endif

      d_dbxss = d_dbxss1; d_dbxvs = d_dbxvs1;
#ifdef D_DBXLSP_NEEDED
      d_dbxls = d_dbxls1;
#endif
#else /* no d_dbxoverflow */
      /* Extend the stack our own way.  */
      if (d_dbxstacksize >= D_DBXMAXDEPTH)
	{
	  d_dbxerror("parser stack overflow");
	  if (d_dbxfree_stacks)
	    {
	      free (d_dbxss);
	      free (d_dbxvs);
#ifdef D_DBXLSP_NEEDED
	      free (d_dbxls);
#endif
	    }
	  return 2;
	}
      d_dbxstacksize *= 2;
      if (d_dbxstacksize > D_DBXMAXDEPTH)
	d_dbxstacksize = D_DBXMAXDEPTH;
#ifndef D_DBXSTACK_USE_ALLOCA
      d_dbxfree_stacks = 1;
#endif
      d_dbxss = (short *) D_DBXSTACK_ALLOC (d_dbxstacksize * sizeof (*d_dbxssp));
      __d_dbx_memcpy ((char *)d_dbxss, (char *)d_dbxss1,
		   size * (unsigned int) sizeof (*d_dbxssp));
      d_dbxvs = (D_DBXSTYPE *) D_DBXSTACK_ALLOC (d_dbxstacksize * sizeof (*d_dbxvsp));
      __d_dbx_memcpy ((char *)d_dbxvs, (char *)d_dbxvs1,
		   size * (unsigned int) sizeof (*d_dbxvsp));
#ifdef D_DBXLSP_NEEDED
      d_dbxls = (D_DBXLTYPE *) D_DBXSTACK_ALLOC (d_dbxstacksize * sizeof (*d_dbxlsp));
      __d_dbx_memcpy ((char *)d_dbxls, (char *)d_dbxls1,
		   size * (unsigned int) sizeof (*d_dbxlsp));
#endif
#endif /* no d_dbxoverflow */

      d_dbxssp = d_dbxss + size - 1;
      d_dbxvsp = d_dbxvs + size - 1;
#ifdef D_DBXLSP_NEEDED
      d_dbxlsp = d_dbxls + size - 1;
#endif

#if D_DBXDEBUG != 0
      if (d_dbxdebug)
	fprintf(stderr, "Stack size increased to %d\n", d_dbxstacksize);
#endif

      if (d_dbxssp >= d_dbxss + d_dbxstacksize - 1)
	D_DBXABORT;
    }

#if D_DBXDEBUG != 0
  if (d_dbxdebug)
    fprintf(stderr, "Entering state %d\n", d_dbxstate);
#endif

  goto d_dbxbackup;
 d_dbxbackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* d_dbxresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  d_dbxn = d_dbxpact[d_dbxstate];
  if (d_dbxn == D_DBXFLAG)
    goto d_dbxdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* d_dbxchar is either D_DBXEMPTY or D_DBXEOF
     or a valid token in external form.  */

  if (d_dbxchar == D_DBXEMPTY)
    {
#if D_DBXDEBUG != 0
      if (d_dbxdebug)
	fprintf(stderr, "Reading a token: ");
#endif
      d_dbxchar = D_DBXLEX;
    }

  /* Convert token to internal form (in d_dbxchar1) for indexing tables with */

  if (d_dbxchar <= 0)		/* This means end of input. */
    {
      d_dbxchar1 = 0;
      d_dbxchar = D_DBXEOF;		/* Don't call D_DBXLEX any more */

#if D_DBXDEBUG != 0
      if (d_dbxdebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      d_dbxchar1 = D_DBXTRANSLATE(d_dbxchar);

#if D_DBXDEBUG != 0
      if (d_dbxdebug)
	{
	  fprintf (stderr, "Next token is %d (%s", d_dbxchar, d_dbxtname[d_dbxchar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef D_DBXPRINT
	  D_DBXPRINT (stderr, d_dbxchar, d_dbxlval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  d_dbxn += d_dbxchar1;
  if (d_dbxn < 0 || d_dbxn > D_DBXLAST || d_dbxcheck[d_dbxn] != d_dbxchar1)
    goto d_dbxdefault;

  d_dbxn = d_dbxtable[d_dbxn];

  /* d_dbxn is what to do for this token type in this state.
     Negative => reduce, -d_dbxn is rule number.
     Positive => shift, d_dbxn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (d_dbxn < 0)
    {
      if (d_dbxn == D_DBXFLAG)
	goto d_dbxerrlab;
      d_dbxn = -d_dbxn;
      goto d_dbxreduce;
    }
  else if (d_dbxn == 0)
    goto d_dbxerrlab;

  if (d_dbxn == D_DBXFINAL)
    D_DBXACCEPT;

  /* Shift the lookahead token.  */

#if D_DBXDEBUG != 0
  if (d_dbxdebug)
    fprintf(stderr, "Shifting token %d (%s), ", d_dbxchar, d_dbxtname[d_dbxchar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (d_dbxchar != D_DBXEOF)
    d_dbxchar = D_DBXEMPTY;

  *++d_dbxvsp = d_dbxlval;
#ifdef D_DBXLSP_NEEDED
  *++d_dbxlsp = d_dbxlloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (d_dbxerrstatus) d_dbxerrstatus--;

  d_dbxstate = d_dbxn;
  goto d_dbxnewstate;

/* Do the default action for the current state.  */
d_dbxdefault:

  d_dbxn = d_dbxdefact[d_dbxstate];
  if (d_dbxn == 0)
    goto d_dbxerrlab;

/* Do a reduction.  d_dbxn is the number of a rule to reduce with.  */
d_dbxreduce:
  d_dbxlen = d_dbxr2[d_dbxn];
  if (d_dbxlen > 0)
    d_dbxval = d_dbxvsp[1-d_dbxlen]; /* implement default value of the action */

#if D_DBXDEBUG != 0
  if (d_dbxdebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       d_dbxn, d_dbxrline[d_dbxn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = d_dbxprhs[d_dbxn]; d_dbxrhs[i] > 0; i++)
	fprintf (stderr, "%s ", d_dbxtname[d_dbxrhs[i]]);
      fprintf (stderr, " -> %s\n", d_dbxtname[d_dbxr1[d_dbxn]]);
    }
#endif


  switch (d_dbxn) {

case 3:
#line 60 "d_dbx.yac"
{ name = d_dbxvsp[0].string;;
    break;}
case 4:
#line 61 "d_dbx.yac"
{
               switch (d_dbxvsp[-1].integer) {
                  case 'T': /* that's only a structure definition */
                  case 't': /* or some informations for casts, not variables */
                     break;
                  case 'G':
                     if (gst && gst->object == STRUCTURE)
                        break;
                  default : /* we now have a variable to create */
                     gv = create_variable(mbkstrdup(name), d_dbxvsp[-1].integer, d_dbxvsp[0].integer, getsize(d_dbxvsp[0].integer));
                     /* address is given as argument */
                     /* class is also given as argument for address offset */
               }
            ;
    break;}
case 5:
#line 77 "d_dbx.yac"
{ d_dbxval.integer = 0; /* automatic variable */;
    break;}
case 6:
#line 79 "d_dbx.yac"
{ d_dbxval.integer = d_dbxvsp[0].integer; /* type given in the string */;
    break;}
case 7:
#line 83 "d_dbx.yac"
{
               /* this is a terminal element of a rule
                  either typeNumber or typeFrom */
               d_dbxval.integer = d_dbxvsp[0].integer; /* just give it back */
            ;
    break;}
case 8:
#line 89 "d_dbx.yac"
{
               ost = gst; /* stack for imbricated declarations : push */
               gst = create_struct("*", d_dbxvsp[-3].integer, d_dbxvsp[0].integer, POINTER, 32);
               if (fst == NULL)
                  fst = gst;
               if (ost)
                  gst = ost; /* stack for imbricated declarations : pop */
               else {
                  gst = fst;
                  fst = NULL;
               }
               d_dbxval.integer = d_dbxvsp[-3].integer;
            ;
    break;}
case 9:
#line 103 "d_dbx.yac"
{
               ost = gst; /* stack for imbricated declarations : push */
               gst = create_struct("function", d_dbxvsp[-3].integer, d_dbxvsp[0].integer, FUNCTION, 32);
               if (fst == NULL)
                  fst = gst;
               if (ost)
                  ost = gst; /* stack for imbricated declarations : push */
               else {
                  gst = fst;
                  fst = NULL;
               }
               d_dbxval.integer = d_dbxvsp[-3].integer;
            ;
    break;}
case 10:
#line 117 "d_dbx.yac"
{
               /* enum are better than #define since symbolic values may
                  be used for debug. Later on, perhaps */
            ;
    break;}
case 11:
#line 122 "d_dbx.yac"
{
               /* xref are not neccessay but no easy to get rid of */
               /* keep same type value for all fields */
            ;
    break;}
case 12:
#line 127 "d_dbx.yac"
{
               ost = gst; /* stack for imbricated declarations : push */
               delete_struct(d_dbxvsp[-4].integer); /* if it has existed, it's thru xref */
               gst = create_struct(mbkstrdup(name), d_dbxvsp[-4].integer, TERMINAL, STRUCTURE, d_dbxvsp[-1].integer);
               gst->member = elm;
               elm = NULL;
               if (fst == NULL)
                  fst = gst;
               if (ost)
                  gst = ost; /* stack for imbricated declarations : pop */
               else {
                  gst = fst;
                  fst = NULL;
               }
               d_dbxval.integer = d_dbxvsp[-4].integer;
            ;
    break;}
case 13:
#line 144 "d_dbx.yac"
{
               ost = gst; /* stack for imbricated declarations : push */
               delete_struct(d_dbxvsp[-4].integer); /* if it has existed, it's thru xref */
               gst = create_struct(mbkstrdup(name), d_dbxvsp[-4].integer, TERMINAL, STRUCTURE, d_dbxvsp[-1].integer);
               gst->member = elm;
               elm = NULL;
               if (fst == NULL)
                  fst = gst;
               if (ost)
                  gst = ost; /* stack for imbricated declarations : pop */
               else {
                  gst = fst;
                  fst = NULL;
               }
               d_dbxval.integer = d_dbxvsp[-4].integer;
            ;
    break;}
case 14:
#line 161 "d_dbx.yac"
{
               ost = gst; /* stack for imbricated declarations : push */
               gst = create_struct("array", d_dbxvsp[-8].integer, d_dbxvsp[0].integer, ARRAY, (d_dbxvsp[-2].integer + 1) * 8);
               if (fst == NULL)
                  fst = gst;
               if (ost)
                  gst = ost; /* stack for imbricated declarations : pop */
               else {
                  gst = fst;
                  fst = NULL;
               }
               d_dbxval.integer = d_dbxvsp[-8].integer;
            ;
    break;}
case 17:
#line 181 "d_dbx.yac"
{
            PTELEMENT pel;

               pel = create_element(mbkstrdup(d_dbxvsp[-7].string), d_dbxvsp[-5].integer, d_dbxvsp[-3].integer, d_dbxvsp[-1].integer);
               if (!elm)
                  elm = pel;
               else {
                  for (e = elm; e->next; e = e->next); /* find last */
                  e->next = pel;
               }
            ;
    break;}
case 18:
#line 193 "d_dbx.yac"
{
            PTELEMENT pel;

               pel = create_element(mbkstrdup(d_dbxvsp[-8].string), d_dbxvsp[-6].integer, d_dbxvsp[-4].integer, d_dbxvsp[-2].integer);
               if (!elm)
                  elm = pel;
               else {
                  for (e = elm; e->next; e = e->next); /* find last */
                  e->next = pel;
               }
            ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  d_dbxvsp -= d_dbxlen;
  d_dbxssp -= d_dbxlen;
#ifdef D_DBXLSP_NEEDED
  d_dbxlsp -= d_dbxlen;
#endif

#if D_DBXDEBUG != 0
  if (d_dbxdebug)
    {
      short *ssp1 = d_dbxss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != d_dbxssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++d_dbxvsp = d_dbxval;

#ifdef D_DBXLSP_NEEDED
  d_dbxlsp++;
  if (d_dbxlen == 0)
    {
      d_dbxlsp->first_line = d_dbxlloc.first_line;
      d_dbxlsp->first_column = d_dbxlloc.first_column;
      d_dbxlsp->last_line = (d_dbxlsp-1)->last_line;
      d_dbxlsp->last_column = (d_dbxlsp-1)->last_column;
      d_dbxlsp->text = 0;
    }
  else
    {
      d_dbxlsp->last_line = (d_dbxlsp+d_dbxlen-1)->last_line;
      d_dbxlsp->last_column = (d_dbxlsp+d_dbxlen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  d_dbxn = d_dbxr1[d_dbxn];

  d_dbxstate = d_dbxpgoto[d_dbxn - D_DBXNTBASE] + *d_dbxssp;
  if (d_dbxstate >= 0 && d_dbxstate <= D_DBXLAST && d_dbxcheck[d_dbxstate] == *d_dbxssp)
    d_dbxstate = d_dbxtable[d_dbxstate];
  else
    d_dbxstate = d_dbxdefgoto[d_dbxn - D_DBXNTBASE];

  goto d_dbxnewstate;

d_dbxerrlab:   /* here on detecting error */

  if (! d_dbxerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++d_dbxnerrs;

#ifdef D_DBXERROR_VERBOSE
      d_dbxn = d_dbxpact[d_dbxstate];

      if (d_dbxn > D_DBXFLAG && d_dbxn < D_DBXLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -d_dbxn if nec to avoid negative indexes in d_dbxcheck.  */
	  for (x = (d_dbxn < 0 ? -d_dbxn : 0);
	       x < (sizeof(d_dbxtname) / sizeof(char *)); x++)
	    if (d_dbxcheck[x + d_dbxn] == x)
	      size += strlen(d_dbxtname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (d_dbxn < 0 ? -d_dbxn : 0);
		       x < (sizeof(d_dbxtname) / sizeof(char *)); x++)
		    if (d_dbxcheck[x + d_dbxn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, d_dbxtname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      d_dbxerror(msg);
	      free(msg);
	    }
	  else
	    d_dbxerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* D_DBXERROR_VERBOSE */
	d_dbxerror("parse error");
    }

  goto d_dbxerrlab1;
d_dbxerrlab1:   /* here on error raised explicitly by an action */

  if (d_dbxerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (d_dbxchar == D_DBXEOF)
	D_DBXABORT;

#if D_DBXDEBUG != 0
      if (d_dbxdebug)
	fprintf(stderr, "Discarding token %d (%s).\n", d_dbxchar, d_dbxtname[d_dbxchar1]);
#endif

      d_dbxchar = D_DBXEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  d_dbxerrstatus = 3;		/* Each real token shifted decrements this */

  goto d_dbxerrhandle;

d_dbxerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  d_dbxn = d_dbxdefact[d_dbxstate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (d_dbxn) goto d_dbxdefault;
#endif

d_dbxerrpop:   /* pop the current state because it cannot handle the error token */

  if (d_dbxssp == d_dbxss) D_DBXABORT;
  d_dbxvsp--;
  d_dbxstate = *--d_dbxssp;
#ifdef D_DBXLSP_NEEDED
  d_dbxlsp--;
#endif

#if D_DBXDEBUG != 0
  if (d_dbxdebug)
    {
      short *ssp1 = d_dbxss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != d_dbxssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

d_dbxerrhandle:

  d_dbxn = d_dbxpact[d_dbxstate];
  if (d_dbxn == D_DBXFLAG)
    goto d_dbxerrdefault;

  d_dbxn += D_DBXTERROR;
  if (d_dbxn < 0 || d_dbxn > D_DBXLAST || d_dbxcheck[d_dbxn] != D_DBXTERROR)
    goto d_dbxerrdefault;

  d_dbxn = d_dbxtable[d_dbxn];
  if (d_dbxn < 0)
    {
      if (d_dbxn == D_DBXFLAG)
	goto d_dbxerrpop;
      d_dbxn = -d_dbxn;
      goto d_dbxreduce;
    }
  else if (d_dbxn == 0)
    goto d_dbxerrpop;

  if (d_dbxn == D_DBXFINAL)
    D_DBXACCEPT;

#if D_DBXDEBUG != 0
  if (d_dbxdebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++d_dbxvsp = d_dbxlval;
#ifdef D_DBXLSP_NEEDED
  *++d_dbxlsp = d_dbxlloc;
#endif

  d_dbxstate = d_dbxn;
  goto d_dbxnewstate;

 d_dbxacceptlab:
  /* D_DBXACCEPT comes here.  */
  if (d_dbxfree_stacks)
    {
      free (d_dbxss);
      free (d_dbxvs);
#ifdef D_DBXLSP_NEEDED
      free (d_dbxls);
#endif
    }
  return 0;

 d_dbxabortlab:
  /* D_DBXABORT comes here.  */
  if (d_dbxfree_stacks)
    {
      free (d_dbxss);
      free (d_dbxvs);
#ifdef D_DBXLSP_NEEDED
      free (d_dbxls);
#endif
    }
  return 1;
}
#line 213 "d_dbx.yac"


/* memory allocating routines */
static PTELEMENT create_element(name, number, address, size)
char *name;
int number, address, size;
{
PTELEMENT member;
   
   member = (PTELEMENT)mbkalloc(sizeof(ELEMENT));
   member->name = name;
   member->typeNumber = number;
   member->address = address;
   member->size = size;
   member->next = NULL;

   return member;
}

static PTSTRUCT create_struct(name, number, from, type, size)
char *name;
int number, from, type, size;
{
static PTSTRUCT lastStruct;
PTSTRUCT s;

   for (s = firstStruct; s; s = s->next)
      if (s->typeNumber == number)
         fprintf(stderr, "Type already there (%d)!\n", number);

   s = (PTSTRUCT)mbkalloc(sizeof(STRUCT));
   s->name = name;
   s->typeNumber = number;
   s->typeFrom = from;
   s->object = type;
   s->size = size;
   s->member = NULL;
   s->next = NULL;
   /* order may be interesting for array and pointer printout */
   if (!firstStruct) /* first add */
      lastStruct = firstStruct = s;
   else { /* insert in end of list */
      lastStruct->next = s;
      lastStruct = s;
   }
   return s;
}

static void delete_struct(number)
int number;
{
PTSTRUCT s, ps;

   for (s = firstStruct; s; ps = s, s = s->next)
      if (s->typeNumber == number)
         break;

   if (!s)
      return;  
   else if (s == firstStruct)
      firstStruct = s->next;
   else
      ps->next = s->next;
   mbkfree((void *)s->name);
   mbkfree((void *)s);
}

static PTVARIABLE create_variable(name, scope, number, size)
char *name;
int scope, number, size;
{
static PTVARIABLE lastVariable; /* add at then end, order needed */
PTVARIABLE v;
 
   v =(PTVARIABLE)mbkalloc(sizeof(VARIABLE));
   v->name = name;
   v->typeNumber = number;
   v->size = size;
   v->address = varAddress;
   v->class = varClass; /* both needed because of static arrays */
   v->scope = scope;    /* and variables that must be distinguished */
   v->next = NULL;
   /* must have the same order than declaration, for 'main' parameters */
   if (!firstVariable) /* first add */
      lastVariable = firstVariable = v;
   else { /* insert in end of list */
      lastVariable->next = v;
      lastVariable = v;
   }
   return v;
}

/* check if a type is a C terminal one, simplify the grammar because of void */
int isknowntype(s)
char *s;
{
int i = 0;
   while (*s && *s != ':')
      d_dbxlval.string[i++] = *s++;
   d_dbxlval.string[i] = '\0';

   if (!strcmp(d_dbxlval.string, "int")
         || !strcmp(d_dbxlval.string, "char")
         || !strcmp(d_dbxlval.string, "longint")
         || !strcmp(d_dbxlval.string, "unsignedint")
         || !strcmp(d_dbxlval.string, "longunsignedint")
         || !strcmp(d_dbxlval.string, "shortint")
         || !strcmp(d_dbxlval.string, "longlongint")
         || !strcmp(d_dbxlval.string, "shortunsignedint")
         || !strcmp(d_dbxlval.string, "longlongunsignedint")
         || !strcmp(d_dbxlval.string, "signedchar")
         || !strcmp(d_dbxlval.string, "unsignedchar")
         || !strcmp(d_dbxlval.string, "float")
         || !strcmp(d_dbxlval.string, "double")
         || !strcmp(d_dbxlval.string, "longdouble")
         || !strcmp(d_dbxlval.string, "void"))
      return 1;
   return 0;
}

int getsize(type)
int type;
{
PTSTRUCT s;

   if (type < 13) /* gcc predefined types, all coded on 32 bits */
      return 32; /* even if only part of the 32 bits are used */
   else if (type == 13 || type == 14)
      return 64; /* 'double' size for the machine */
   else if (type == 15) /* that's 'void' */
      return 0;

   for (s = firstStruct; s; s = s->next)
      if (s->typeNumber == type)
         return s->size;

   fprintf(stderr, "Error : variable with undefined type (%d)!\n", type);
   EXIT(-5);
}

static void fill_dbx_info(str)
char *str;
{
char *t = str, *s = str, *u;
static int length;
static char *buffer; /* contains what is to be parsed */
 
   /* kill spaces in the line, needed because of default: in d_dbxlex */
   while (*t)
      if (!isspace(*t))
         *s++ = *t++;
      else
         t++;
   *s = '\0';

   if (*(s - 2)  == '\\') {
      *(s - 2) = '\0';
      if (!length) {
         length = t - str + 1;
         buffer = (char *)mbkalloc(length);
         strcpy(buffer, str);
      } else {
         length += t - str + 1;
         buffer = mbkrealloc(buffer, length);
         strcat(buffer, str);
      }
      mbkfree((void *)str);
      return; /* get more information for structure filling */
   } else if (length
              && *(s - 1) == ';'
              && (*(s - 2) == ';' || *(s - 2) == ',')) {
      length += t - str + 1;
      buffer = mbkrealloc(buffer, length);
      strcat(buffer, str);
      mbkfree((void *)str);
      length = 0;
      dbx_line = buffer; /* since it's end of structure, globalize it! */
   } else if (isknowntype(str)) { /* skip terminal types */
      mbkfree((void *)str);
      return;
   } else 
      dbx_line = str; /* globalize it! */
   u = dbx_line;
   d_dbxparse();
   mbkfree((void *)u);
}

/* lexical analyser */
static int d_dbxlex()
{
static char c;
int i = 0;
char *s = dbx_line; /* remember where to start from */

   switch (*s) {
      case '\0':

#ifdef OUT
         printf("\n\n");
#endif

         return -1; /* expected by yacc as end of entries */

      case ';':
      case ',': /* easy, because such symbols are not legal in C identifiers */
      case ':':
      case '=':
      case '*':

#ifdef OUT
         printf("(%c)", *s);
         if (*s == ';' && c == ';')
            printf("\n\n");
#endif

         return c = *dbx_line++;

      case 'T':
      case 't':
      case 'S':
      case 'V':
      case 'G':
      case 'p':
      case 'F':
         if (c == ':') { /* context dependant return value : storage class */

#ifdef OUT
            printf("FLAG(%c)", *s);
#endif
            d_dbxlval.integer = c = *dbx_line++;
            return FLAG;
         }

         c = *s; /* just to warrant correctness of state */
         while (*s && *s != ':')
            d_dbxlval.string[i++] = *s++;
         d_dbxlval.string[i] = '\0';
         dbx_line = s;

#ifdef OUT
         printf("SYMBOL{%s}", d_dbxlval.string);
#endif

         return SYMBOL;

      case 's':
      case 'u':
      case 'e':
         if (c == '=') { /* context dependant return value : storage class */

#ifdef OUT
            printf("(%c)", *s);
#endif
            return c = *dbx_line++;
         }

         c = *s; /* just to warrant correctness of state */
         while (*s && *s != ':')
            d_dbxlval.string[i++] = *s++;
         d_dbxlval.string[i] = '\0';
         dbx_line = s; /* points on ':' now */

#ifdef OUT
         printf("SYMBOL{%s}", d_dbxlval.string);
#endif

         return SYMBOL;

      case 'f':
         if (c == ':') { /* context dependant return value : storage class */

#ifdef OUT
            printf("FLAG(%c)", *s);
#endif
            d_dbxlval.integer = c = *dbx_line++;
            return FLAG;
         }
         if (c == '=') { /* context dependant return value : storage class */

#ifdef OUT
            printf("(%c)", *s);
#endif
            return c = *dbx_line++;
         }

         c = *s; /* just to warrant correctness of state */
         while (*s && *s != ':')
            d_dbxlval.string[i++] = *s++;
         d_dbxlval.string[i] = '\0';
         dbx_line = s; /* points on ':' now */

#ifdef OUT
         printf("SYMBOL{%s}", d_dbxlval.string);
#endif

         return SYMBOL;

      case 'a':
      case 'x':
      case 'r':
         if (c == '=') { /* context dependant return value : storage class */
            c = *s;
            if (c == 'a')
               dbx_line += 3; /* skip 'ar1' : always here */
            else
               dbx_line += 2; /* skip 'u', 's' or 1, unnecessary information */

#ifdef OUT
            printf("(%c)", c);
#endif
            return c;
         }

         c = *s; /* just to warrant correctness of state */
         while (*s && *s != ':')
            d_dbxlval.string[i++] = *s++;
         d_dbxlval.string[i] = '\0';
         dbx_line = s; /* points on ':' now */

#ifdef OUT
         printf("SYMBOL{%s}", d_dbxlval.string);
#endif

         return SYMBOL;

      case '-': /* what a shit of a syntax, this dbx */
         s++; /* skip it an go to numbers */
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
         c = *s; /* just to warrant correctness of state */
         while (isdigit(*s++)); /* skip digits */
         d_dbxlval.integer = atoi(dbx_line); /* counts only digits */
         dbx_line = --s; /* points at end of numbers */

#ifdef OUT
         printf("INTEGER[%d]", d_dbxlval.integer);
#endif

         return INTEGER;

      default:
         c = *s; /* just to warrant correctness of state */
         while (*s && *s != ':')
            d_dbxlval.string[i++] = *s++;
         d_dbxlval.string[i] = '\0';
         dbx_line = s; /* points on ':' now */

#ifdef OUT
         printf("SYMBOL{%s}", d_dbxlval.string);
#endif

         return SYMBOL;
   }
}

d_dbxerror(s)
{
   fflush(stdout);
   fprintf(stderr, "syntax error on line %s, char '%c'\n",
            dbx_line, d_dbxchar);
   EXIT(-6);
}

/*******************************************************************************
* free space concerned with dbx informations                                   *
*******************************************************************************/
free_dbx_info()
{
PTVARIABLE var, v;
PTSTRUCT str, s;
PTELEMENT elm, e;

   /* Do not free memory yet as it dumps a core */
   firstVariable = NULL;
   firstStruct = NULL;

   for (var = firstVariable; var; var = v) {
      v = var->next;
      mbkfree((void *)var->name);
      mbkfree((void *)var);
   }

   for (str = firstStruct; str; str = s) {
      s = str->next;
      elm = str->member;
      while (elm) {
         e = elm->next;
         mbkfree((void *)elm->name);
         mbkfree((void *)elm);
         elm = e;
      }
      mbkfree((void *)str->name);
      mbkfree((void *)str);
   }

   /* need to be null before any dbx stuff */
   firstVariable = NULL;
   firstStruct = NULL;
}

AnalysDebug(str, class, address)
char *str;
int class, address;
{
   varClass = class;
   varAddress = address;
   fill_dbx_info(str);
}
