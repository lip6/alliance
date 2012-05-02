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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* 
 * Purpose : constantes, externs, and data-structures
 * Date    : 31/08/93
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * $Id: mut.h,v 1.10 2012/05/02 15:23:46 jpc Exp $
 */

#ifndef _MUT_H_
#define _MUT_H_

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <libgen.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__) || defined(__cplusplus)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/* namealloc hash table size: the 1230th prime */
#define HASHVAL     10007

/* hash tables management defines */
#define EMPTYHT     -1
#define DELETEHT    -2


/* read-write text-files opening codes: */
#define READ_TEXT   "r" /* read  access */
#define WRITE_TEXT  "w" /* write access */

/* chain buffer size */
#define BUFSIZE     1024
#define SIGSIZE     1024

/* USER field codes  */
#define LOFIGCHAIN  4592
#define PLACEABOX   5749
#define VTIPARSER   9716
#define CHECKFEED   6996
#define UNFLATLOFIG 1992
#define PTSIGSIZE   8111

/* Maximum number of simultaneous terminated process */
#define ENDCHLD_MAX     1024
/* Maximum number of autoacknowledge process ( should be the same as
 *  END_CHLD_MAX ) */
#define AUTOACKCHLD_MAX 1024

/* concatenate names while flattening or copying up */
#define YES         'Y'
#define NO          'N'

/* enable debugging of chainlist */
/*
#define DEBUGCHAIN 1
*/

/*******************************************************************************
* macro definitions                                                            *
*******************************************************************************/
#define EXIT(x)      mbkexit(x)

#define isvdd(name)	instr(name, VDD, SEPAR)
#define isvss(name)	instr(name, VSS, SEPAR)
#define funcin()     if (MBK_DEBUG_ON)                                \
                        HEAD_MBKDEBUG = addptype(HEAD_MBKDEBUG,       \
                                                 __LINE__,            \
                                                 (void *)__FILE__)
#define funcout()    if (MBK_DEBUG_ON && HEAD_MBKDEBUG)               \
                        HEAD_MBKDEBUG = delptype(HEAD_MBKDEBUG,       \
                                                 HEAD_MBKDEBUG->TYPE)

/*******************************************************************************
* service structures types                                                     *
*******************************************************************************/

typedef struct point                           /* point list                  */
{
struct point *NEXT;                            /* next point                  */
long         INDEX;                            /* index (unique)              */
long         X,Y;                              /* coordinates                 */
}
point_list;

typedef struct chain                           /* pointer list, lisp lists    */
{
struct chain *NEXT;                            /* next pointer                */
void         *DATA;                            /* generic pointer             */
#ifdef DEBUGCHAIN
int           LINE;
char         *FILE;
#endif
}
chain_list;

typedef struct ptype                           /* typed pointer list          */
{
struct ptype *NEXT;                            /* next pointer                */
void         *DATA;                            /* generic pointer             */
long         TYPE;                             /* type                        */
}
ptype_list;

typedef struct num                             /* integer list                */
{
struct num *NEXT;                              /* next integer                */
long       DATA;                               /* integer value               */
}
num_list;

typedef struct endchld                         /* list of ended process       */
{
  struct endchld *NEXT;
  int             PID;
  int             STATUS;
} endchld_list;

typedef struct autoackchld                     /* list of process auto-ack    */
{
  struct autoackchld *NEXT;
  int                 PID;
} autoackchld_list;

/******************************************************************************
* hash table structures types                                                 *
******************************************************************************/
typedef struct htitem {                        /* hash table element          */
	void *key;                                  /* pointer for access          */
	long value;                                 /* associated value            */
} htitem;

typedef struct htable {                        /* hash table itself           */
	long length;                                /* table length                */
	htitem *pElem;                              /* pointer to table start      */
	long count;                                 /* number of inserted elements */
} ht;

/*******************************************************************************
* mbk externals for the global variables defined in mbk_util.c                 *
*******************************************************************************/
extern chain_list *HEAD_CHAIN;                 /* chain list buffer head      */
extern chain_list *HEAD_BLOCK_CHAIN;
extern ptype_list *HEAD_PTYPE;                 /* ptype list buffer head      */
extern num_list   *HEAD_NUM;                   /* num list buffer head        */
extern char       TRACE_MODE;                  /* trace if Y                  */
extern int        TRACE_FILE;                  /* trace if >0                 */
extern int        TRACE_GETENV;                /* trace if >0                 */
extern char       DEBUG_MODE;                  /* debug if Y                  */
extern char       FAST_MODE;                   /* no consistency control if Y */
extern char       *WORK_LIB;                   /* working directory           */
extern char       **CATA_LIB;                  /* read only directory         */
extern char       *CATAL;                      /* catalog file name           */
extern char       SEPAR;                       /* char used for concatenation */
extern char       IN_LO[];                     /* logical input format        */
extern char       IN_PH[];                     /* physical input format       */
extern char       OUT_LO[];                    /* logical output format       */
extern char       OUT_PH[];                    /* physical output format      */
extern long       SCALE_X;                     /* distance scale parameter    */
extern long MBK_X_GRID;
extern long MBK_Y_GRID;
extern long MBK_Y_SLICE;
extern long MBK_WIDTH_VSS;
extern long MBK_WIDTH_VDD;
extern long MBK_TRACK_WIDTH_ALU1;
extern long MBK_TRACK_WIDTH_ALU2;
extern long MBK_TRACK_WIDTH_ALU3;
extern long MBK_TRACK_WIDTH_ALU4;
extern long MBK_TRACK_WIDTH_ALU5;
extern long MBK_TRACK_WIDTH_ALU6;
extern long MBK_TRACK_WIDTH_ALU7;
extern long MBK_TRACK_WIDTH_ALU8;
extern long MBK_TRACK_SPACING_ALU1;
extern long MBK_TRACK_SPACING_ALU2;
extern long MBK_TRACK_SPACING_ALU3;
extern long MBK_TRACK_SPACING_ALU4;
extern long MBK_TRACK_SPACING_ALU5;
extern long MBK_TRACK_SPACING_ALU6;
extern long MBK_TRACK_SPACING_ALU7;
extern long MBK_TRACK_SPACING_ALU8;
extern char       PARSER_INFO[];               /* version number, and so on   */
extern char       *VDD;                        /* user name for power high    */
extern char       *VSS;                        /* user name for power ground  */
extern char       *CK;                         /* user name for ck            */
extern ptype_list *HEAD_MBKDEBUG;              /* function debug list         */
extern char       MBK_DEBUG_ON;                /* mode for debug on/off       */
extern char       *IN_FILTER;
extern char       *OUT_FILTER;
extern char       *FILTER_SFX;
extern char       MBKFOPEN_NAME[];             /* dernier fichier ouvert      */
extern char       MBKFOPEN_FILTER;
extern endchld_list *HEAD_ENDCHLD;             /* list of terminated process  */
extern endchld_list *HEAD_ENDCHLD_FREE;
extern int        STAT_ENDCHLD;
extern autoackchld_list *HEAD_AUTOACKCHLD;     /* list of terminated process  */
extern autoackchld_list *HEAD_AUTOACKCHLD_FREE;
extern int        STAT_AUTOACKCHLD;


extern void (*MBK_EXIT_FUNCTION)();           /* Added by Ludovic Jacomme */
extern void mbkexit __P(( int ExitValue ));   /* new exit handler system  */

/*******************************************************************************
* externals for mbk_util.c functions                                           *
*******************************************************************************/

  extern           void  mbkenv __P(());
  extern          char * nameindex __P((char *name, long index));
  extern      num_list * addnum __P((num_list *ptnum, long data));
  extern           void  freenum __P((num_list *pt));
  extern            int  isck    __P((char *name));

#if (defined(__STDC__) ||  defined(__GNUC__)) && defined(DEBUGCHAIN)
#define addchain( a, b ) (chain_list*)fnaddchain(a,b,__LINE__,__FILE__)
#elif defined(DEBUGCHAIN) 
#define addchain( a, b ) (chain_list*)fnaddchain(a,b,0,"")
#endif

#ifdef DEBUGCHAIN 
  extern    chain_list * fnaddchain __P((chain_list *pthead, void *ptdata, int, char*));
#else
  extern    chain_list * addchain __P((chain_list *pthead, void *ptdata));
#endif

  extern    chain_list * delchain __P((chain_list *pthead, chain_list *ptdel));
  extern           void  freechain __P((chain_list *pt));
  extern           void testchain();
  extern    ptype_list * addptype __P((ptype_list *pthead, long type, void *ptdata));
  extern    ptype_list * delptype __P((ptype_list *pthead, long type));
  extern           void  freeptype __P((ptype_list *pt));
  extern    ptype_list * getptype __P((ptype_list *pthead, long type));
  extern    chain_list * append __P((chain_list *pt1, chain_list *pt2));
  extern          char * namealloc __P((const char *inputname));
  extern          char * namefind __P((const char *inputname));
  extern          char * mbkstrdup __P((const char *s));
  extern           void  downstr __P((const char *s, char *t));
  extern           void  upstr __P((const char *s, char *t));
  extern          char * instr __P((char *s, const char *find, char separ));
  extern          char * vectorize __P((const char *radical, long index));
  extern          char * vectorradical __P((char *name));
  extern            int  vectorindex __P((const char *name));
  extern          char * concatname __P((const char *name1, const char *name2));
  extern    chain_list * reverse __P((chain_list *head));
  extern            int  naturalstrcmp __P((const char *s, const char *t));
  extern            ht * addht __P((unsigned long len));
  extern           long  gethtitem __P((ht *pTable, void *key));
  extern           long  addhtitem __P((ht *pTable, void *key, long value));
  extern           long  sethtitem __P((ht *pTable, void *key, long value));
  extern           long  delhtitem __P((ht *pTable, void *key));
  extern           void  viewht __P((ht *pTable, char *(*pout)()));
  extern           void  delht __P((ht *pTable));
  extern            int  incatalog __P((const char *figname));
  extern            int  incataloggds __P((const char *figname));
  extern            int  incatalogfeed __P((const char *figname));
  extern            int  incatalogdelete __P((const char *figname));
  extern           void  alliancebanner __P((const char *tool, const char *tv, const char *comment, const char *date, const char *av));
  extern           void  alliancebanner_with_authors __P((const char *tool, const char *tv, const char *comment, const char *date, const char *av, const char *authors));
  extern           void  alliancebanner_with_contrib __P((const char *tool, const char *tv, const char *comment, const char *date, const char *av, const char *authors, const char *contrib));
  extern           int   mbkwaitpid __P(( int, int, int* ));
  extern           void mbkinitendchld __P((void));
  extern           endchld_list* mbkgetendchld __P((void));
  extern           void mbkfreeendchld __P((endchld_list*));
  extern           void mbksetautoackchld __P(( int ));
  extern           void mbkinitautoackchld __P(( void ));
  extern           void mbkfreeautoackchld __P(( autoackchld_list* ));
  extern           autoackchld_list* mbkgetautoackchld __P(( void ));

/*******************************************************************************
* externals for mbk_sys.c functions                                            *
*******************************************************************************/
  extern          void  *mbkalloc __P((size_t nbytes));
  extern          void  *mbkrealloc __P((void *pt, size_t nbytes));
  extern          void  mbkfree __P((void *ptr));
  extern          char  *mbkgetenv __P((const char *name));
  extern          FILE  *mbkfopen __P((const char *name, const char *extension, const char *mode));
  extern            int  mbkunlink __P((const char *name, const char *extension));
  extern          char  *filepath __P((const char *name, const char *extension));
  extern          char **decompfilter __P((const char*));
  extern          FILE  *mbkpopen __P((const char*, const char*, char));
  extern           void  mbkps __P((void));
  extern           void  mbkackchld __P(( int ));

/*******************************************************************************
* externals for mbk_fake.c  or parsers drivers functions                       *
*******************************************************************************/
extern void vtiloadphfig();
extern void vtiloadlofig();
extern void alcloadphfig();
extern void alcloadlofig();
extern void spiceloadlofig();
extern void vhdlloadlofig();
extern void hiloloadlofig();
extern void edifloadlofig();
extern void vtisavephfig();
extern void vtisavelofig();
extern void alcsavephfig();
extern void alcsavelofig();
extern void spicesavelofig();
extern void vhdlsavelofig();
extern void hilosavelofig();
extern void edifsavelofig();
extern void vlogsavelofig();
extern void mgnloadphfig();
extern void mgnsavephfig();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !MUTL */
