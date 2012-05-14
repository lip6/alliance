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

/* 
 * Purpose : services functions and global variables
 * Date    : 06/03/92
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 */

#ident "$Id: mbk_util.c,v 1.8 2012/05/14 14:20:23 alliance Exp $"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "mut.h"
#include "mbk_util.h"


/* Normally defined in values.h, but not available on all systems */
#ifndef BITS
#define  BITS(type)  (8 * (int)sizeof(type))
#endif

/*******************************************************************************
* global variables                                                             *
*******************************************************************************/
chain_list *HEAD_CHAIN = NULL;                 /* chain buffer head           */
chain_list *HEAD_BLOCK_CHAIN = NULL; 
ptype_list *HEAD_PTYPE = NULL;                 /* ptype buffer head           */
num_list *HEAD_NUM = NULL;                     /* num   buffer head           */
char TRACE_MODE  = 'N';                        /* trace if 'Y'                */
int  TRACE_FILE  = 0;                          /* trace if >0                 */
int  TRACE_GETENV  = 0;                          /* trace if >0                 */
char DEBUG_MODE  = 'N';                        /* debug if 'Y'                */
char FAST_MODE   = 'N';                        /* no consistency check if 'Y' */
char SEPAR  = '.';                             /* char used in concatenation  */
char *WORK_LIB = NULL;                         /* working directory           */
char **CATA_LIB = NULL;                        /* read only directories       */
char *CATAL = NULL;                            /* catalog file                */
char IN_LO[5] = "al";                          /* input logical format        */
char IN_PH[5] = "ap";                          /* input physical format       */
char OUT_LO[5] = "al";                         /* output logical format       */
char OUT_PH[5] = "ap";                         /* output physical format      */
long SCALE_X = 100;                            /* distance scale definition   */
long MBK_X_GRID             = 5;
long MBK_Y_GRID             = 5;
long MBK_Y_SLICE            = 50;
long MBK_WIDTH_VSS          = 6;
long MBK_WIDTH_VDD          = 6;
long MBK_TRACK_WIDTH_ALU1   = 2;
long MBK_TRACK_WIDTH_ALU2   = 2;
long MBK_TRACK_WIDTH_ALU3   = 2;
long MBK_TRACK_WIDTH_ALU4   = 2;
long MBK_TRACK_WIDTH_ALU5   = 2;
long MBK_TRACK_WIDTH_ALU6   = 2;
long MBK_TRACK_WIDTH_ALU7   = 2;
long MBK_TRACK_WIDTH_ALU8   = 0;
long MBK_TRACK_SPACING_ALU1 = 3;
long MBK_TRACK_SPACING_ALU2 = 3;
long MBK_TRACK_SPACING_ALU3 = 3;
long MBK_TRACK_SPACING_ALU4 = 3;
long MBK_TRACK_SPACING_ALU5 = 8;
long MBK_TRACK_SPACING_ALU6 = 8;
long MBK_TRACK_SPACING_ALU7 = 8;
long MBK_TRACK_SPACING_ALU8 = 0;
char PARSER_INFO[100] = "nothing yet";         /* version number, and so on   */
char *VDD = NULL;                              /* user name for power high    */
char *VSS = NULL;                              /* user name for power ground  */
char *CK = NULL;                               /* user name for clock         */
char *IN_FILTER = NULL ;
char *OUT_FILTER = NULL ;
char *FILTER_SFX = NULL ;
/* table de hash de namealloc() et namefind() */
static chain_list *NAME_HASHTABLE[HASHVAL];
static char buffer[BUFSIZ];                    /* buffer for namealloc strcpy */
static char str[BUFSIZ];                       /* buffer for concatname       */
static char tolowertable[1 << BITS(char)];     /* number of chars             */
static char touppertable[1 << BITS(char)];     /* number of chars             */
/*
** Added by Ludovic Jacomme (The slave)
** in order to "trap" exit with Graal/Dreal etc ...
*/
 void (*MBK_EXIT_FUNCTION)() = 0;

static void dflhandler(int);
static void loadcatalog(char ***table, int *size, char type);
static void read_lib(void);

/*******************************************************************************
* fonction handler()                                                           *
*******************************************************************************/
static void dflhandler(int sig)
{
   exit(100);
}

/*******************************************************************************
* fonction mbkwaitpid()                                                        *
* Verifie parmis les processus fils termines si pid en fait partie. Si status  *
* est non nul, le code de retour est place dans cette variable. si mode vaut   *
* 1, cet appel est bloquant, sinon il est non bloquant.                        *
*******************************************************************************/
int mbkwaitpid( int pid, int mode, int *status )
{
  endchld_list *scan, *prev;
 
  while(1)
  {
    prev = NULL;
    for( scan = HEAD_ENDCHLD ; scan ; scan = scan->NEXT )
    {
      if( scan->PID == pid )
      {
        if( status )
          *status = scan->STATUS;

        if( prev )
          prev->NEXT = scan->NEXT;
        else
          HEAD_ENDCHLD = scan->NEXT;

        mbkfreeendchld( scan );
        return( 1 );
      }
      prev = scan;
    }

    if( mode != 1 )
      return( 0 );

    sleep(3600); /* le sleep est interruptible : on ne perd rien */
  }
  
  return( 0 );
}

/*******************************************************************************
* fonction mbkenv()                                                            *
*******************************************************************************/
void mbkenv(void)
{
char *str;
long nchar;

static char MBK_RAND_SEED[] = { 0x62,
  0x37, 0x34, 0x30, 0x30, 0x32, 0x31, 0x38, 0x61, 0x31, 0x37, 0x34,
  0x64, 0x34, 0x64, 0x36, 0x36, 0x65, 0x32, 0x35, 0x38, 0x30, 0x34, 0x63,
  0x31, 0x36, 0x32, 0x38, 0x34, 0x65, 0x37, 0x61 } ;


   str = mbkgetenv("MBK_TRACE_GETENV");
   if (str != NULL)
   {
      if( !strcmp(str, "yes" ) )
         TRACE_GETENV = 1;
      else if (!strcmp(str,"Y"))
         TRACE_GETENV = 1;
   }

   read_lib(); /* read the contents of MBK_WORK_LIB and MBK_CATA_LIB */

   str = mbkgetenv("MBK_DEBUG_MODE");
   if (str != NULL)
      if (!strcmp(str,"yes"))
         DEBUG_MODE = 'Y';

   str = mbkgetenv("MBK_TRACE_MODE");
   if (str != NULL)
      if (!strcmp(str,"yes"))
         TRACE_MODE = 'Y';

   str = mbkgetenv("MBK_TRACE_FILE");
   if (str != NULL)
   {
      if( !strcmp(str, "true" ) )
         TRACE_FILE = 3;
      else if (!strcmp(str,"yes"))
         TRACE_FILE = 2;
      else if( !strcmp(str, "Y" ) )
         TRACE_FILE = 1;
      else if( !strcmp(str, "all" ) )
         TRACE_FILE = 1;
   }
   
   str = mbkgetenv("MBK_FAST_MODE");
   if (str != NULL)
      if (!strcmp(str,"yes")) 
         FAST_MODE = 'Y';

   str = mbkgetenv("MBK_SCALE_X");
   if (str != NULL)
      SCALE_X = (long)atoi(str);

   str = mbkgetenv("MBK_X_GRID");
   if (str != NULL)
      MBK_X_GRID = (long)atoi(str);

   str = mbkgetenv("MBK_Y_GRID");
   if (str != NULL)
      MBK_Y_GRID = (long)atoi(str);

   str = mbkgetenv("MBK_Y_SLICE");
   if (str != NULL)
      MBK_Y_SLICE = (long)atoi(str);

   str = mbkgetenv("MBK_WIDTH_VSS");
   if (str != NULL)
      MBK_WIDTH_VSS = (long)atoi(str);

   str = mbkgetenv("MBK_WIDTH_VDD");
   if (str != NULL)
      MBK_WIDTH_VDD = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU1");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU1 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU2");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU2 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU3");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU3 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU4");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU4 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU5");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU5 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU6");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU6 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU7");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU7 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_WIDTH_ALU8");
   if (str != NULL)
      MBK_TRACK_WIDTH_ALU8 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU1");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU1 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU2");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU2 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU3");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU3 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU4");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU4 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU5");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU5 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU6");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU6 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU7");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU7 = (long)atoi(str);

   str = mbkgetenv("MBK_TRACK_SPACING_ALU8");
   if (str != NULL)
      MBK_TRACK_SPACING_ALU8 = (long)atoi(str);

   srand((unsigned int) MBK_RAND_SEED);

   str = mbkgetenv("MBK_IN_LO");
   if (str != NULL) {
      if (!strcmp(str, "hns"))
         (void)strcpy(IN_LO, "hns");
      else if (!strcmp(str, "fne"))
         (void)strcpy(IN_LO, "fne");
      else if (!strcmp(str, "hdn"))
         (void)strcpy(IN_LO, "hdn");
      else if (!strcmp(str, "fdn"))
         (void)strcpy(IN_LO, "fdn");
      else if (!strcmp(str, "al" ))
         (void)strcpy(IN_LO, "al");
      else if (!strcmp(str, "alx"))
         (void)strcpy(IN_LO, "alx");
      else if (!strcmp(str, "spi"))
         (void)strcpy(IN_LO, "spi");
      else if (!strcmp(str, "sp"))
         (void)strcpy(IN_LO, "sp");
      else if (!strcmp(str, "cir"))
         (void)strcpy(IN_LO, "cir");
      else if (!strcmp(str, "edi"))
         (void)strcpy(IN_LO, "edi");
      else if (!strcmp(str, "vst"))
         (void)strcpy(IN_LO, "vst");
      else {
         (void)fflush(stdout);
         (void)fprintf(stderr,"*** mbk error ***\n");
         (void)fprintf(stderr,"netlist input format '%s' not supported\n",str);
         EXIT(1);
      }
   }

   str = mbkgetenv("MBK_OUT_LO");
   if (str != NULL) {
      if (!strcmp(str, "hns"))
         (void)strcpy(OUT_LO, "hns");
      else if (!strcmp(str, "fne"))
         (void)strcpy(OUT_LO, "fne");
      else if (!strcmp(str, "hdn"))
         (void)strcpy(OUT_LO, "hdn");
      else if (!strcmp(str, "fdn"))
         (void)strcpy(OUT_LO, "fdn");
      else if (!strcmp(str, "al" ))
         (void)strcpy(OUT_LO, "al");
      else if (!strcmp(str, "alx"))
         (void)strcpy(OUT_LO, "alx");
      else if (!strcmp(str, "spi"))
         (void)strcpy(OUT_LO, "spi");
      else if (!strcmp(str, "sp"))
         (void)strcpy(OUT_LO, "sp");
      else if (!strcmp(str, "cir"))
         (void)strcpy(OUT_LO, "cir");
      else if (!strcmp(str, "edi"))
         (void)strcpy(OUT_LO, "edi");
      else if (!strcmp(str, "vst"))
         (void)strcpy(OUT_LO, "vst");
      else if (!strcmp(str, "cct"))
         (void)strcpy(OUT_LO, "cct");
                else if (!strcmp(str, "vlg"))
                      (void)strcpy(OUT_LO, "vlg");
      else {
         (void)fflush(stdout);
         (void)fprintf(stderr,"*** mbk error ***\n");
         (void)fprintf(stderr,"netlist output format '%s' not supported\n",str);
         EXIT(1);
      }
   }

   str = mbkgetenv("MBK_IN_PH");
   if (str != NULL) {
      if (!strcmp(str, "cp"))
         (void)strcpy(IN_PH, "cp");
      else if (!strcmp(str, "ap"))
         (void)strcpy(IN_PH, "ap");
      else if (!strcmp(str, "mg"))
         (void)strcpy(IN_PH, "mg");
      else {
         (void)fflush(stdout);
         (void)fprintf(stderr,"*** mbk error ***\n");
         (void)fprintf(stderr,"layout input format '%s' not supported\n",str);
         EXIT(1);
      }
   }

   str = mbkgetenv("MBK_OUT_PH");
   if (str != NULL) {
      if (!strcmp(str, "cp"))
         (void)strcpy(OUT_PH, "cp");
      else if (!strcmp(str, "ap"))
         (void)strcpy(OUT_PH, "ap");
      else if (!strcmp(str, "mg"))
         (void)strcpy(OUT_PH, "mg");
      else {
         (void)fflush(stdout);
         (void)fprintf(stderr,"*** mbk error ***\n");
         (void)fprintf(stderr,"layout output format '%s' not supported\n",str);
         EXIT(1);
      }
   }

   str = mbkgetenv("MBK_CATAL_NAME");
   if (str != NULL)
      CATAL = str;
   else /* default value for catalog name */
      CATAL = "CATAL";

   str = mbkgetenv("MBK_SEPAR");
   if (str != NULL)
      SEPAR = *str;

   str = mbkgetenv("MBK_VDD");
   if (str != NULL)
      VDD = str;
   else /* default value for power high */
      VDD = "vdd";

   str = mbkgetenv("MBK_VSS");
   if (str != NULL)
      VSS = str;
   else /* default value for power high */
      VSS = "vss";
   
   str = mbkgetenv("MBK_CK");
   if (str != NULL)
      CK = str;
   else /* default value for clock */
      CK = "ck";

   str = mbkgetenv( "MBK_IN_FILTER" );
   if( str )
     IN_FILTER = str;
   else
     IN_FILTER = NULL;

   str = mbkgetenv( "MBK_OUT_FILTER" );
   if( str )
     OUT_FILTER = str;
   else
     OUT_FILTER = NULL;

   str = mbkgetenv( "MBK_FILTER_SFX" );
   if( str )
     FILTER_SFX = str;
   else
     FILTER_SFX = NULL;

   /* EXIT shall produce a real exit if not trapped */
   signal(SIGTERM, dflhandler);

   /* Initialize a table of lower case characters for the machine encoding */
   for (nchar = 0; nchar < sizeof(tolowertable); nchar++)
      tolowertable[nchar] = isupper(nchar) ? tolower(nchar) : nchar;

   /* Initialize a table of upper case characters for the machine encoding */
   for (nchar = 0; nchar < sizeof(touppertable); nchar++)
      touppertable[nchar] = islower(nchar) ? toupper(nchar) : nchar;

   mbkinitendchld();
   mbkinitautoackchld();
   /* Sous Solaris, certaines fonctions de haut niveau (fgets...) sont 
    * interruptible par la reception d'un signal. Ceci n'a jamais ete pris
    * en compte auparavant : on conserve donc explicitement le comportement
    * "normal" qui est que ces fonctions ne sont normalement pas
    * interruptible. D'ou le SA_RESTART.
    */

   #ifndef AUTO_HAS_SA_RESTART
     signal( SIGCHLD, mbkackchld );
   #else
     sigemptyset( &ens );
     sgct.sa_handler  = mbkackchld;
     sgct.sa_mask     = ens;
     sgct.sa_flags    = SA_RESTART;
     sigaction( SIGCHLD, &sgct , NULL );
   #endif
}

/*******************************************************************************
* function nameindex()                                                         *
* return a string that is the concatenation of the name argument, the mbk      *
* separator, and an index                                                      *
*******************************************************************************/
char *nameindex(char *name, long index)
{
char str[100];

   (void)sprintf(str,"%s%c%ld", name, SEPAR, index);
   return namealloc(str);
}

/*******************************************************************************
* fonction addnum()                                                            *
* num list specialized allocator to avoid too many mallocs                     *
*******************************************************************************/
num_list *addnum(num_list *ptnum, long data)
{
num_list *pt;
register int i;

   if (HEAD_NUM == NULL) {
      HEAD_NUM = (num_list *)mbkalloc(BUFSIZE*sizeof(num_list));
      pt = HEAD_NUM;
      for (i = 1; i < BUFSIZE; i++) {
         pt->NEXT = pt + 1;
         pt++;
      }
      pt->NEXT = NULL;
   }

   pt = HEAD_NUM;
   HEAD_NUM = HEAD_NUM->NEXT;
   pt->NEXT = ptnum;
   pt->DATA = data;
   return pt;
}

/*******************************************************************************
* function freenum()                                                           *
* gives back freed block to the num memory allocator                           *
*******************************************************************************/
void freenum(num_list *pt)
{
   HEAD_NUM = (num_list *)append((chain_list *)pt, (chain_list *)HEAD_NUM);
}

/*******************************************************************************
* function addchain()                                                          *
* chain list specialized allocator to avoid too many mallocs                   *
*******************************************************************************/
#ifdef DEBUGCHAIN
chain_list *fnaddchain(chain_list *pthead, void *ptdata, int line, char *file)
#else
chain_list *addchain(chain_list *pthead, void *ptdata)
#endif
{
#ifdef DEBUGCHAIN
chain_list *block;
#endif
chain_list *pt;
register int i;

   if (HEAD_CHAIN == NULL) {
      pt = (chain_list *)mbkalloc(BUFSIZE*sizeof(chain_list));

#ifdef DEBUGCHAIN
      block = (chain_list*)mbkalloc(sizeof(chain_list));
      block->DATA = pt;
      block->NEXT = HEAD_BLOCK_CHAIN;
      HEAD_BLOCK_CHAIN = block;
#endif

      HEAD_CHAIN = pt;
      for (i = 1; i < BUFSIZE; i++) {
         pt->NEXT = pt + 1;
#ifdef DEBUGCHAIN
         pt->LINE = 0;
#endif
         pt++;
      }
      pt->NEXT = NULL;
#ifdef DEBUGCHAIN
      pt->LINE = 0;
#endif
   }

   pt = HEAD_CHAIN;
   HEAD_CHAIN = HEAD_CHAIN->NEXT;
   pt->NEXT = pthead;
   pt->DATA = ptdata;
#ifdef DEBUGCHAIN
   pt->LINE = line;
   pt->FILE = file;
#endif
   return pt;
}

/*******************************************************************************
* function freechain()                                                         *
* gives back freed block or blocks to the chain_list memory allocator          *
*******************************************************************************/
void freechain(chain_list *pt)
{
#ifdef DEBUGCHAIN
   chain_list *scanchain;
   for( scanchain = pt ; scanchain ; scanchain = scanchain->NEXT )
     scanchain->LINE=0;
#endif
   HEAD_CHAIN = append(pt, HEAD_CHAIN);
}

/*******************************************************************************
* function delchain()                                                          *
* delete a single element of a chain_list and gives it back to freechain       *
*******************************************************************************/
chain_list *delchain(chain_list *pthead, chain_list *ptdel)
{
chain_list *pt;
chain_list *ptsav = NULL; /* To make gcc -Wall silent */

   if (pthead == NULL || ptdel == NULL) {
      (void)fflush(stdout);
      (void)fprintf(stderr,"*** mbk error ***");
      (void)fprintf(stderr,"  delchain() impossible : pointer = NULL !\n");
      EXIT(1);
   }

   if (ptdel == pthead) {
      pt = pthead->NEXT;
      pthead->NEXT = NULL;
      freechain(pthead);
      return pt;
   } else {
      for (pt = pthead; pt; pt = pt->NEXT) {
         if (pt == ptdel)
            break;
         ptsav = pt;
      }
      if (pt != NULL) {
         ptsav->NEXT = pt->NEXT;
         ptdel->NEXT = NULL;
         freechain(ptdel);
         return pthead;
      } else
         return NULL;
   }
}

/*******************************************************************************
* function addptype()                                                          *
*******************************************************************************/
void testchain(void)
{
#ifdef DEBUGCHAIN
  chain_list *scanchain;
  int i;
  for( scanchain = HEAD_BLOCK_CHAIN ; scanchain ; scanchain = scanchain->NEXT )
  {
    for( i = 0 ; i < BUFSIZE ; i++)
      if( ((chain_list*)(scanchain->DATA))[i].LINE != 0 )
        printf( "%50s : %5d\n", ((chain_list*)(scanchain->DATA))[i].FILE,
                                ((chain_list*)(scanchain->DATA))[i].LINE
              );
  }
#endif
}

/*******************************************************************************
* function addptype()                                                          *
*******************************************************************************/
ptype_list *addptype(ptype_list *pthead, long type, void *ptdata)
{
ptype_list *pt;
register int i;

   if (HEAD_PTYPE == NULL) {
      pt = (ptype_list *)mbkalloc(BUFSIZE * sizeof(ptype_list));
      HEAD_PTYPE = pt;
      for (i = 1; i < BUFSIZE; i++) {
         pt->NEXT = pt + 1;
         pt++;
      }
      pt->NEXT = NULL;
   }

   pt = HEAD_PTYPE;
   HEAD_PTYPE = HEAD_PTYPE->NEXT;
   pt->NEXT = pthead;
   pt->DATA = ptdata;
   pt->TYPE = type;
   return pt;
}
  
/*******************************************************************************
* function delptype()                                                          *
*******************************************************************************/
ptype_list *delptype(ptype_list *pthead, long type)
{
ptype_list *pt;
ptype_list *ptsav = NULL; /* To make gcc -Wall silent */

   if (pthead == NULL) {
      (void)fflush(stdout);
      (void)fprintf(stderr,"*** mbk error ***\n");
      (void)fprintf(stderr,"delptype() impossible : pthead = NULL !\n");
      EXIT(1);
   }

   if (pthead->TYPE == type) {
      pt = pthead->NEXT;
      pthead->NEXT = NULL;
      freeptype(pthead);
      return pt;
   } else {
      for (pt = pthead; pt; pt = pt->NEXT) {
         if (pt->TYPE == type)
            break;
         ptsav = pt;
      }
      if (pt != NULL) {
         ptsav->NEXT = pt->NEXT;
         pt->NEXT = NULL;
         freeptype(pt);
         return pthead;
      } else
         return NULL;
   }
}

/*******************************************************************************
* function freeptype()                                                         *
*******************************************************************************/
void freeptype(ptype_list  *pt)
{
   HEAD_PTYPE = (ptype_list *)append((chain_list *)pt,(chain_list *)HEAD_PTYPE);
}

/*******************************************************************************
* function getptype()                                                          *
*******************************************************************************/
ptype_list *getptype(ptype_list *pthead, long type)
{
ptype_list  *pt;

   for (pt = pthead; pt; pt = pt->NEXT)
      if (pt->TYPE == type)
         return pt;
   return NULL;
}

/*******************************************************************************
* function append()                                                            *
*******************************************************************************/
chain_list *append(chain_list *pt1, chain_list *pt2)
{
chain_list *pt;

   if (pt1 == NULL)
      return pt2;
   else { 
      for (pt = pt1; pt->NEXT; pt = pt->NEXT);
      pt->NEXT = pt2; /* append the list 2 at the end of list 1 */
      return pt1;
   }
}

/*******************************************************************************
* Dictonnary related functions                                                 *
*******************************************************************************/

/* Random hash function due to Don. E. Knuth, The Stanford Graph Base.
 * Truly better than the previous one from my own experimentations. */
#define HASH_MULT 314159
#define HASH_PRIME 516595003

#define HASH_FUNC(inputname, name, code)                                     \
   do {                                                                      \
      while (*inputname) {                                                   \
         *name = tolowertable[(int)*inputname++];                            \
         code += (code ^ (code >> 1)) + HASH_MULT * (unsigned char) *name++; \
         while (code >= HASH_PRIME)                                          \
            code -= HASH_PRIME;                                              \
      }                                                                      \
      *name = '\0';                                                          \
      code %= HASHVAL;                                                       \
   } while (0)

/*******************************************************************************
* function namealloc()                                                         *
*******************************************************************************/
char *namealloc(const char *inputname)
{
chain_list *pt;
char *name = buffer; /* ensure no modification of parameter string */
int code = 0;
static char *lastname;

   if (inputname == NULL)
      return NULL;

   if (inputname == lastname)
      return lastname; /* To make const acceptable, strange */

   /* Beware, that's a define, ... */
   HASH_FUNC(inputname, name, code);

   for (pt = NAME_HASHTABLE[code]; pt; pt = pt->NEXT)
      if (!strcmp(buffer, (char *)pt->DATA))
         return (char *)pt->DATA;

   name = (char *)mbkalloc((unsigned int)(strlen(buffer) + 1));
   (void)strcpy(name, buffer);
   NAME_HASHTABLE[code] = addchain(NAME_HASHTABLE[code], (void *)name);
#ifdef DEBUGCHAIN
   NAME_HASHTABLE[code]->LINE=0; /* pas de trace de debuggage dessus */
#endif
   lastname = (char *)(NAME_HASHTABLE[code]->DATA);
   return lastname;
}


/*******************************************************************************
* function namefind()                                                         *
*******************************************************************************/
char *namefind(const char *inputname)
{
chain_list *pt;
char *name = buffer; /* ensure no modification of parameter string */
register int code = 0;

   if (inputname == NULL)
      return NULL;

   HASH_FUNC(inputname, name, code);

   for (pt = NAME_HASHTABLE[code]; pt; pt = pt->NEXT)
      if (!strcmp(buffer, (char *)pt->DATA))
         return (char *)pt->DATA;

   return NULL ; 
}

/*******************************************************************************
* function downstr()                                                           *
*******************************************************************************/
void downstr(const char *s, char *t)
{
   for (; *s; s++, t++)
      *t = tolowertable[(int)*s];
   *t = *s;
}

/*******************************************************************************
* function upstr()                                                             *
*******************************************************************************/
void upstr(const char *s, char *t)
{
   for (; *s; s++, t++)
      *t = touppertable[(int)*s];
   *t = *s;
}

/*******************************************************************************
* function isck()                                                              *
*******************************************************************************/
int  isck(char *name)
{
    if (!name || !CK)
        return 0;
    else
        if (!strcmp(name, CK))
            return 1;
        else
            return 0;
}

/*******************************************************************************
* function instr() : about strstr, but some systems don't have it              *
*******************************************************************************/
char *instr(char *s, const char *find, char separ)
{
char *t, c, sc;
int len;

   if (!s || !find)
      return NULL;
   if (separ) {
      t = s;
      if ((s = strrchr(s, separ)) == NULL)
         s = t;
      else
         s++; /* skip the separator itself */
   }
   if ((c = *find++) != 0) {
      len = strlen(find);
      do {
         do {
            if ((sc = *s++) == 0)
               return NULL;
         } while (sc != c);
      } while (strncmp(s, find, len) != 0);
      s--;
   }
   return s;
}

/*******************************************************************************
* function vectorize : builds a legal mbk vector from a name and an index      *
*******************************************************************************/
char *vectorize(const char *radical, long index)
{
  (void)sprintf(str,"%s %ld", radical, index);
  return namealloc(str);
}

/*******************************************************************************
* function vectorradical : returns the radical of an mbk vector                *
*******************************************************************************/
char *vectorradical(char *name)
{
char *s;

   strcpy(str, name);
   s = strchr(str, ' ');

   if (!s)
      return name;

   *s = '\0';
   return namealloc(str);
}

/*******************************************************************************
* function vectorindex : returns the index of an mbk vector                    *
*******************************************************************************/
int vectorindex(const char *name)
{
char *s = strchr(name, ' ');

   if (!s)
      return -1;

   return atoi(s);
}

/*******************************************************************************
*  generic compaison function :                                                *
*  unlike strcmp, ensure that 10 > 2 for vectorized strings.                   *
*  first, check strings, then check numerical values as numbers, not strings.  *
*******************************************************************************/
int naturalstrcmp(const char *s, const char *t)
{
char *spt, *tpt;
const char *st = s, *tt = t;
int u, ls, lt;

   spt = buffer, tpt = str;

   while ((!isspace((int)*st)) && *st)
      *spt++ = *st++;
   *spt = '\0';

   while ((!isspace((int)*tt)) && *tt)
      *tpt++ = *tt++;
   *tpt = '\0';

   if ((u = strcmp(buffer, str)) != 0)
      return u;

   if ((ls = strlen(s)) == (lt = strlen(t)))
      return strcmp(s, t);

   return ls - lt;
}

/*******************************************************************************
* function concatname()                                                        *
*******************************************************************************/
char *concatname(const char *name1, const char *name2)
{
  (void)sprintf(str,"%s%c%s", name1, SEPAR, name2);
  return namealloc(str);
}

/*******************************************************************************
* mbkstrdup : since brain damaged system we aim at do not have it              *
*******************************************************************************/
char *mbkstrdup(const char *s)
{
  char *t;

  if (s == NULL)
    return NULL;
  
  t = (char *)mbkalloc((size_t)(strlen(s) + 1));
  return strcpy(t, s);
}

/*******************************************************************************
* function reverse                                                             *
*******************************************************************************/
chain_list *reverse(chain_list *head)
{
chain_list *p;
chain_list *q = (chain_list *)NULL;

   if (!head)
      return NULL;
   while ((p = head->NEXT)) {
      head->NEXT = q;
      q = head;
      head = p;
   }
   head->NEXT = q;
   return head;
}

/*******************************************************************************
* function pstrcmp                                                             *
* used for qsort and bsearch use for catalog sorting and acessing              *
*******************************************************************************/
static int pstrcmp(const void *s, const void *t)
{
   return strcmp(*(const char **)s, *(const char **)t);
}

/*******************************************************************************
* function incatalogfeed                                                       *
* tests if a model is present in the catalog with the F attribut               *
*******************************************************************************/
int incatalogfeed(const char *figname)
{
static int size;
static char **table;

   if (!size)
      loadcatalog(&table, &size, 'F');
   return size ?
            (long)bsearch(&figname, table, size, sizeof(char *), pstrcmp) : 0;
}

/*******************************************************************************
* function incataloggds                                                        *
* tests if a model is present in the catalog with the G attribut               *
******************************************************************************/
int incataloggds(const char *figname)
{
static int size;
static char **table;

   if (!size)
      loadcatalog(&table, &size, 'G');
   return size ?
            (long)bsearch(&figname, table, size, sizeof(char *), pstrcmp) : 0;
}

/*******************************************************************************
* function incatalog                                                           *
* tests if a model is present in the catalog with the C attribut               *
*******************************************************************************/
int incatalog(const char *figname)
{
static int size;
static char **table;

   if (!size)
      loadcatalog(&table, &size, 'C');
   return size ?
            (long)bsearch(&figname, table, size, sizeof(char *), pstrcmp) : 0;
}

/*******************************************************************************
* function incatalogdelete                                                     *
* tests if a model is present in the catalog with the D attribut               *
*******************************************************************************/
int incatalogdelete(const char *figname)
{
static int size;
static char **table;

   if (!size)
      loadcatalog(&table, &size, 'D');
   return size ?
            (long)bsearch(&figname, table, size, sizeof(char *), pstrcmp) : 0;
}

/*******************************************************************************
* function loadcatalog                                                         *
* read the catalog from disk checking the given type                           *
*******************************************************************************/

static void loadcatalog(char ***table, int *size, char type)
{
void *pt;
char attrib;
int nb;
int nb_input; 
int i = 0;
chain_list *files = (chain_list *)NULL;
/* Tables for quick cell search :
   The catalog file is read only once, and sorted for speed.
   The later calls to loadcatalog only return the approriate table. */
static chain_list *cells[4];
static char **tabs[4];
static int sizes[4];
static int read;
 
   if (!read) {
      read++;
      (void)sprintf(buffer, "%s/%s", WORK_LIB, CATAL ? CATAL : "CATAL");
      files = addchain(files, (void *)fopen(buffer, READ_TEXT));
      while (CATA_LIB[i]) {
         (void)sprintf(buffer, "%s/CATAL", CATA_LIB[i++]);
         files = addchain(files, (void *)fopen(buffer, READ_TEXT));
      }
      /* error message :
         To avoid malloc/free on file name, the error message is output
         relatively to its order. */
      files = reverse(files);
      for (i = 0, pt = (void *)files; pt;
            pt = (void *)((chain_list *)pt)->NEXT, i++) {
         if (!((chain_list *)pt)->DATA)
            continue;
         nb = 0;
         while (!feof((FILE *)((chain_list *)pt)->DATA)) {
            nb++; /* count lines */
            nb_input = fscanf((FILE *)((chain_list *)pt)->DATA, "%s %c\n",
                              buffer, &attrib);
            if (nb_input == 0) /* skip white lines */
               continue;
            if (nb_input != 2) {
               (void)fflush(stdout);
               (void)fprintf(stderr, "*** mbk error ***\n");
               (void)fprintf(stderr, "loadcatalog syntax error line %d ", nb);
               if (i == 0)
                  (void)fprintf(stderr, "in file %s/%s\n", WORK_LIB, CATAL);
               else
                  (void)fprintf(stderr, "in file %s/CATAL\n", CATA_LIB[i - 1]);
               EXIT(1);
            }   
            attrib = islower((int)attrib) ? (char)toupper((int)attrib) : attrib;
            switch (attrib) {
               case 'C' :
                  cells[0] = addchain(cells[0], namealloc(buffer));
                  sizes[0]++;
                  break;
               case 'G' :
                  cells[1] = addchain(cells[1], namealloc(buffer));
                  sizes[1]++;
                  break;
               case 'F' :
                  cells[2] = addchain(cells[2], namealloc(buffer));
                  sizes[2]++;
                  break;
               case 'D' :
                  cells[3] = addchain(cells[3], namealloc(buffer));
                  sizes[3]++;
                  break;
               default :
                  (void)fflush(stdout);
                  (void)fprintf(stderr, "*** mbk error ***\n");
                  (void)fprintf(stderr, "loadcatalog syntax error line %d ",
                                 nb);
                  if (i == 0)
                     (void)fprintf(stderr, "in file %s/%s\n", WORK_LIB, CATAL);
                  else
                     (void)fprintf(stderr, "in file %s/CATAL\n",
                                    CATA_LIB[i - 1]);
                  (void)fprintf(stderr,"unknown attribut %c\n", attrib);
                  EXIT(1);
            }
         }
         (void)fclose((FILE *)((chain_list *)pt)->DATA);
      }
      for (nb = 0; nb < 4; nb++) {
         if (sizes[nb]) {
            tabs[nb] = (char **)mbkalloc(sizes[nb] * sizeof(char *));
            for (i = 0, pt = (void *)cells[nb]; pt;
                  pt = (void *)((chain_list *)pt)->NEXT, i++)
               tabs[nb][i] = (char *)((chain_list *)pt)->DATA;
            qsort(tabs[nb], sizes[nb], sizeof(char *), pstrcmp);
            freechain(cells[nb]);
         }
      }
   }

   switch (type) {
      case 'C' :
         *table = tabs[0];
         *size = sizes[0];
         break;
      case 'G' :
         *table = tabs[1];
         *size = sizes[1];
         break;
      case 'F' :
         *table = tabs[2];
         *size = sizes[2];
         break;
      case 'D' :
         *table = tabs[3];
         *size = sizes[3];
         break;
   }
}

/*******************************************************************************
* function read_lib()                                                          *
* fills an array of char * in order to have a list of names as CATA_LIB        *
*******************************************************************************/
static void read_lib(void)
{
char *str, *s, *stc, *c;
int argc = 0;

   str = mbkgetenv("MBK_WORK_LIB");
   if (str != NULL) {
      WORK_LIB =
         (char *)mbkalloc((unsigned int)(strlen(str) + 1) * sizeof(char));
      (void)strcpy(WORK_LIB, str);
   } else { /* no specific path is given */
      WORK_LIB = (char *)mbkalloc((unsigned int)2 * sizeof(char));
      (void)strcpy(WORK_LIB, ".");
   }

   str = mbkgetenv("MBK_CATA_LIB");
   if(str != NULL) {
      s = (char *)mbkalloc((unsigned int)(strlen(str) + 1) * sizeof(char));
      (void)strcpy(s, str);
      str = s;   /* let's not modify the environement values */
      stc = str; /* for counting purposes */
      while (1) {
         if ((c = strchr(stc, ':')) == NULL)
            break;
         argc++;
         stc = ++c;
      }
      CATA_LIB = (char **)mbkalloc((unsigned int)(argc + 2) * sizeof(char *));
      argc = 0;
      while (1) {
         if ((s = strchr(str, ':')) == NULL)
            break;
         *(s++) = '\0';
         CATA_LIB[argc++] = str; /* no allocation necessary */
         str = s;
      }
      if (s == NULL)
         CATA_LIB[argc++] = str == NULL || *str == '\0' ? NULL : str;
      CATA_LIB[argc] = NULL;
   } else { /* no specific path is given */
      CATA_LIB = (char **)mbkalloc((unsigned int)2 * sizeof(char *));
      CATA_LIB[0] = ".";
      CATA_LIB[1] = NULL;
   }
}
/*******************************************************************************
* Hash tables management functions, contributed to by Luc Burgun on 20/06/92   *
*******************************************************************************/
static void reallocht(ht *pTable);
/*******************************************************************************
* dilution function for the table accesses                                     *
*******************************************************************************/
static unsigned long hash(void *p)
{
   return abs((long)p * ((long)p >> 5) >> 4);
}

/*******************************************************************************
* function addht, create a hash table                                          *
*******************************************************************************/
ht *addht(unsigned long len)
{
ht *pTable;
htitem *pEl;
int i;

   if (len == 0) {
      fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "addht impossible : hash table size is '0'\n");
      EXIT(1);
   }
   pTable = (ht *)mbkalloc(sizeof(struct htable));
   pTable->length = len;
   pEl = (htitem *)mbkalloc(len * (sizeof(struct htitem)));
   pTable->pElem = pEl;
   for (i = 0; i < len; i++) {
      pEl[i].key = NULL;
      pEl[i].value = EMPTYHT;
   }
   pTable->count = 0;
   return pTable;
}

/*******************************************************************************
* function delht, delete a hash table                                          *
*******************************************************************************/
void delht(ht *pTable)
{
htitem * pEl;

   pEl = pTable->pElem;
   mbkfree(pEl);
   mbkfree(pTable);
}

/*******************************************************************************
* function gethtitem, get an element in a hash table                            *
*******************************************************************************/
long gethtitem(ht *pTable, void *key)
{
long co = 0;
long indice = 0;
htitem * pEl;

   indice = hash(key) % pTable->length;
   do {
      if (co++ > HMAX_CALLS) {
         reallocht(pTable);
         return gethtitem(pTable, key);
      }

      pEl = (pTable->pElem) + indice;
      if (pEl->value != EMPTYHT && pEl->value != DELETEHT) {
         if ((long) key == (long) pEl->key)
            return pEl->value;
      } else if (pEl->value == EMPTYHT)
         return EMPTYHT;
      indice = (indice + 1) % pTable->length;
   } while (1);
}

/*******************************************************************************
* function addhtitem, get an element in a hash table                            *
*******************************************************************************/
long addhtitem(ht *pTable, void *key, long value)
{
int indice = 0;
htitem *pEl;
int co = 0;

   if (value == EMPTYHT || value == DELETEHT) {
      fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "addhtitem impossible : value is EMPTYHT or DELETEHT\n");
      EXIT(1);
   }
   if (pTable->count++ > (pTable->length) * 8 / 10) {
      reallocht(pTable);
      return addhtitem(pTable, key, value);
   }

   indice = hash(key) % pTable->length;
   do {
      if (co++ > HMAX_CALLS) {
         reallocht(pTable);
         return addhtitem(pTable, key, value);
      }
      pEl = (pTable->pElem) + indice;
      if (pEl->value == EMPTYHT || pEl->value == DELETEHT) {
         pEl->value = value;
         pEl->key = key;
         return value;
      } else if ((long) pEl->key == (long) key) {
         pTable->count--;
         pEl->value = value;
         return value;
      }
      indice = (indice + 1) % pTable->length;
   } while (1);
}

/*******************************************************************************
* function sethtitem, test if an element exists in a hash table,                *
* adds it anyway, and returns 1 if it used to exist, 0 else.                   *
*******************************************************************************/
long sethtitem(ht *pTable, void *key, long value)
{
int indice = 0;
htitem *pEl;
int co = 0;

   if (value == EMPTYHT || value == DELETEHT) {
      fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "sethtitem impossible : value is EMPTYHT or DELETEHT\n");
      EXIT(1);
   }
   if (pTable->count++ > (pTable->length) * 8 / 10) {
      reallocht(pTable);
      return sethtitem(pTable, key, value);
   }

   indice = hash(key) % pTable->length;
   do {
      if (co++ > HMAX_CALLS) {
         reallocht(pTable);
         return sethtitem(pTable, key, value);
      }
      pEl = (pTable->pElem) + indice;
      if (pEl->value == EMPTYHT || pEl->value == DELETEHT) {
         pEl->value = value;
         pEl->key = key;
         return 0;
      } else if ((long) pEl->key == (long) key) {
         pTable->count--;
         pEl->value = value;
         return 1;
      }
      indice = (indice + 1) % pTable->length;
   } while (1);
}

/*******************************************************************************
* function delhtitem, delete an element in a hash table                         *
*******************************************************************************/
long delhtitem(ht *pTable, void *key)
{
int indice = 0;
htitem *pEl;
int co = 0;

   indice = hash(key) % pTable->length;
   do {
      if (co++ > HMAX_CALLS) {
         reallocht(pTable);
         return delhtitem(pTable, key);
      }
      pEl = (pTable->pElem) + indice;
      if (pEl->value != EMPTYHT && pEl->value != DELETEHT) {
         if ((long) key == (long)pEl->key) {
            pTable->count--;
            pEl->value = DELETEHT;
            return pEl->value;
         }
      } else if (pEl->value == EMPTYHT)
         return EMPTYHT;
      indice = (indice + 1) % pTable->length;
   } while (1);
}

/*******************************************************************************
* display contents of an hash table                                            *
*******************************************************************************/
void viewht(ht *pTable, char *(*pout)(void *))
{
long i;
htitem *pEl = pTable->pElem;

   (void)printf("================== viewht ================\n");
   (void)printf("length = %ld\t     count = %ld\n",
                  pTable->length, pTable->count);
   (void)printf("==========================================\n");
   for (i = 0; i < pTable->length; i++) {
      if (pEl->value != EMPTYHT && pEl->value != DELETEHT) {
         printf("index  %ld\t", i);
         printf("key    %s\t", pout(pEl->key));
         printf("value  %ld \n", pEl->value);
      }
      pEl++;
   }
}

/*******************************************************************************
* realloc space to adapt hash table size to number of entries                  *
*******************************************************************************/
static void reallocht(ht *pTable)
{
ht *tabBis;
htitem *pEl;
int i;

   pEl = pTable->pElem;
   tabBis = addht((pTable->length) * 5);
   for (i = 0; i < pTable->length; i++) {
      if (pEl->value != EMPTYHT && pEl->value != DELETEHT)
         addhtitem(tabBis, pEl->key, pEl->value);
      pEl++;
   }
   mbkfree(pTable->pElem);
   pTable->length = tabBis->length;
   pTable->pElem = tabBis->pElem;
   pTable->count = tabBis->count;
   mbkfree(tabBis);
}

/*******************************************************************************
* All that needed for a cute banner, by Frederic Petrot                        *
* Used to be a standalone library                                              *
*******************************************************************************/
#include <time.h>
#define WINDOW_SIZE 81
#define LINES 15
#define ASCENT 13
static char screen[LINES][WINDOW_SIZE];

static int indx(char c)
{
   return c >= '0' && c <= '9' ? (int)c - '0'
                              : isupper((int)c) ? (int)10 + c - 'A'
                              : islower((int)c) ? (int)11 + 'Z' - 'A' + c - 'a'
                              : -1;
}

static void banner(const char *s, const char *police[][62], int nl)
{
int i, j, k, l, m;
const char *line;

   /* rince off :
      the buffer is filled with nul characteres. */
   for (j = 0; j < nl; j++)
      for (i = 0; i < WINDOW_SIZE; i++)
         screen[j][i] = '\0';
   /* first :
      filling the buffer with direct table output. */
   while (*s) {
      for (i = 0; i < nl; i++) {
         if ((j = indx(*s)) == -1) {
            fprintf(stderr,
               "alliancebanner: Error: Character out of [0-9A-Za-z] range\n");
            exit(1);
         }
         line = police[j][i];
         if (strlen(line) + strlen(screen[i]) >= WINDOW_SIZE) {
            fprintf(stderr,
   "alliancebanner: Error: Resulting size bigger than %d columns not allowed\n",
                              WINDOW_SIZE - 1);
            exit(1);
         }
         strcat(screen[i], line);
         if (*(s + 1) != '\0')
            strcat(screen[i], " ");
      }
      s++;
   }
   for (m = l = -1, j = 0; j < nl; j++)
      for (i = 0; i < WINDOW_SIZE; i++)
         if (screen[j][i] == '@') {
            if (m == -1)
               m = j;
            l = j;
            break;
         }
   k = strlen(screen[0]);
   /* banner :
      output on stdout. */
   putc('\n', stdout);
   for (j = m; j <= l; j++) {
      for (i = 0; i < (WINDOW_SIZE - k) / 2; i++)
         putc(' ', stdout);
      for (i = 0; i < k; i++)
         putc(screen[j][i], stdout);
      putc('\n', stdout);
   }
}

static void cartouche(const char *tool, const char *tv, const char *comment, const char *date, const char *av, const char *authors, const char *contrib)
{
int i, j, k, l;
static char *msg[6] = {
   "%s",
   "Alliance CAD System %s,\"%s %s",
   "Copyright (c) %s-%d,\"ASIM/LIP6/UPMC",
   "Author(s):\"%s",
   "Contributor(s):\"%s",
   "E-mail        :\"alliance-users@asim.lip6.fr"
};
int msgl[6];
char *str;
time_t timer;
char day[4], month[4];
int year, nday, hour, minute, second;

   (void)time(&timer);
   (void)sscanf(ctime(&timer), "%s %s %d %d:%d:%d %d",
                       day, month, &nday, &hour, &minute, &second, &year);
   /* rince off :
      the buffer is filled with nul characteres. */
   for (j = 0; j < 12; j++)
      for (i = 0; i < WINDOW_SIZE; i++)
         screen[j][i] = '\0';
   i = strlen(tool);
   str = mbkstrdup(tool);
   for (k = 0; k < i; k++)
      str[k] = isupper((int)tool[k]) ? tolower((int)tool[k]) : tool[k];
   sprintf(screen[0], msg[0], comment);
   sprintf(screen[1], msg[1], av, str, tv);
   mbkfree(str);
   sprintf(screen[2], msg[2], date, year);
   if (authors != (char *)0 )
    sprintf(screen[3], msg[3],authors);
   else
    screen[3][0] = '\0';
   if (contrib != (char *)0 )
    sprintf(screen[4], msg[4],contrib);
   else
    screen[4][0] = '\0';

   strcat(screen[5], msg[5]);

   for (i = 1; i < 6; i++) {
      msgl[i] = strlen(screen[i]);
      j = j < msgl[i] ? msgl[i] : j;
   }
   for (i = 1; i < 6; i++)
      for (l = 0, k = 0; k < WINDOW_SIZE; k++) {
         if (screen[i][k] == '\0') {
            screen[i + 6][k + l] = '\0';
            break;
         }
         if (screen[i][k] == '"') { /* only once per line */
            for (; l <= j - msgl[i]; l++)
               screen[i + 6][k + l] = ' ';
            continue;
         }
         screen[i + 6][k + l] = screen[i][k];
      }
   /* cartouche :
      output on stdout. */
   i = strlen(comment);
   putc('\n', stdout);
   for (k = 0; k < (WINDOW_SIZE - i) / 2; k++)
      putc(' ', stdout);
   puts(screen[0]);
   putc('\n', stdout);
   for (i = 1; i < 6; i++) {
      if (screen[i][0]=='\0') continue;
      for (k = 0; k < (WINDOW_SIZE - j) / 2; k++)
         putc(' ', stdout);
      for (k = 0; k <= j; k++)
         if (screen[i + 6][k] != 0) /* not so nice, but */
            putc(screen[i + 6][k], stdout);
      putc('\n', stdout);
   }
   putc('\n', stdout);
}

void alliancebanner_with_contrib(const char *tool, const char *tv, const char *comment, const char *date, const char *av, const char *authors, const char *contrib)
{
  banner(tool, Unknown_Bold_Normal_14, 15);
  cartouche(tool, tv, comment, date, av, authors,contrib);
}

void alliancebanner_with_authors(const char *tool, const char *tv, const char *comment, const char *date, const char *av, const char *authors)
{
  alliancebanner_with_contrib( tool, tv, comment, date, av, authors, NULL );
}


void alliancebanner(const char *tool, const char *tv, const char *comment, const char *date, const char *av)
{
  alliancebanner_with_contrib( tool, tv, comment, date, av, NULL, NULL );
}

/******************
* Contributed to by Ludovic Jacomme
**************************/
ptype_list *HEAD_MBKDEBUG = NULL;
char        MBK_DEBUG_ON  = 0;

static void trapdebug(int dummy)
{
  ptype_list *ScanDebug;

  for ( ScanDebug  = HEAD_MBKDEBUG;
        ScanDebug != (ptype_list *)NULL;
        ScanDebug  = ScanDebug->NEXT )
  {
    fprintf( stdout, "mbkdebug: file %s line %ld\n", 
             (char *)ScanDebug->DATA, ScanDebug->TYPE );
  }

  fflush( stdout );

  signal( SIGQUIT, trapdebug );
  signal( SIGSEGV, SIG_DFL      );
  signal( SIGBUS , SIG_DFL      );
  signal( SIGILL , SIG_DFL      );
}

void mbkdebug(void)
{
  signal( SIGSEGV, trapdebug );
  signal( SIGBUS,  trapdebug );
  signal( SIGILL,  trapdebug );
  signal( SIGQUIT, trapdebug );

  MBK_DEBUG_ON = 1;
}

/*
** Added by Ludovic Jacomme (The slave)
** in order to "trap" exit with Graal/Dreal etc ...
*/

void mbkexit( int ExitValue )
{
  if ( MBK_EXIT_FUNCTION != NULL )
  {
    (*MBK_EXIT_FUNCTION)( ExitValue );
  }

  kill( getpid(), SIGTERM );
}


/* To compile with gcc under SunOS */
#ifdef SunOS
void *dlopen(char *path, int mode)
{
}
void *dlsym(void *handle, char *symbol)
{
}
char *dlerror(void)
{
}
int dlclose(void *handle)
{
}
#endif
