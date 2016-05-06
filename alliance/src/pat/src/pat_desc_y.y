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

/* ###--------------------------------------------------------------### */
/* file		: pat_desc.yac						*/
/* date		: Mar  9 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: parser-driver function				*/
/* ###--------------------------------------------------------------### */

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mut.h"
#include "pat.h"
#include "ppt.h"
#include "pat_type.h"
#include "pat_desc.h"

extern  int  pat_desc_y_parse ();
extern  int  pat_decl_y_lex   ();
extern  void pat_decl_y_error ();

/* ###--------------------------------------------------------------### */
/* function	: lcl_addpacom						*/
/* description	: create PACOM structure if there is no more structure	*/
/*		  available in free list				*/
/* called func.	: pat_addpacom						*/
/* ###--------------------------------------------------------------### */

static struct pacom *lcl_addpacom (lastpacom, text, position)

struct pacom   *lastpacom;	/* pointer on the last pacom structure	*/
char           *text;		/* text of the comment			*/
unsigned short  position;	/* # of statement since last comment	*/

  {
  struct pacom *ptcom = NULL;

  if (FRE_COMPNT == NULL)
     ptcom = pat_addpacom (lastpacom, text, position);
  else
    {
    ptcom           = FRE_COMPNT;
    FRE_COMPNT      = ptcom->NEXT;

    ptcom->TEXT     = text;
    ptcom->POSITION = position;
    ptcom->NEXT     = lastpacom;
    }

  return (ptcom);
  }

/* ###--------------------------------------------------------------### */
/* function	: lcl_addpapat						*/
/* description	: create PAPAT structure if there is no more structure	*/
/*		  available in free list				*/
/* called func.	: pat_addpapat						*/
/* ###--------------------------------------------------------------### */

static struct papat *lcl_addpapat (lastpapat, label, line)

struct papat   *lastpapat;	/* pointer on the last papat structure	*/
char           *label;		/* pattern's label			*/
unsigned short  line;		/* pattern' line number			*/

  {
  struct papat *ptpat;

  if (FRE_PATPNT == NULL)
     ptpat = pat_addpapat (lastpapat, label, line);
  else
    {
    ptpat      = FRE_PATPNT;
    FRE_PATPNT = ptpat->NEXT;

    ptpat->LABEL   = label;
    ptpat->PAEVT   = NULL;
    ptpat->PAINI   = NULL;
    ptpat->LINE    = line;
    ptpat->BLANK   =  0 ;
    ptpat->ACTFLAG = 'U';
    ptpat->SIMFLAG = 'U';
    ptpat->NEXT    = lastpapat;
    }

  return (ptpat);
  }

/* ###--------------------------------------------------------------### */
/* function	: lcl_addpaini						*/
/* description	: create PAINI structure if there is no more structure	*/
/*		  available in free list				*/
/* called func.	: pat_addpaini						*/
/* ###--------------------------------------------------------------### */

static struct paini *lcl_addpaini (lastpaini, name, value)

struct paini *lastpaini;	/* pointer on the last paini structure	*/
char         *name;		/* register's name			*/
char          value;		/* value to be forced into the register	*/

  {
  struct paini *ptini;

  if (FRE_INIPNT == NULL)
     ptini =  pat_addpaini (lastpaini, name, value);
  else
    {
    ptini      = FRE_INIPNT;
    FRE_INIPNT = ptini->NEXT;

    ptini->NAME  = namealloc (name);
    ptini->VALUE = value;
    ptini->NEXT  = lastpaini;
    }

  return (ptini);
  }

/* ###--------------------------------------------------------------### */
/* function	: lcl_addpaevt						*/
/* description	: create PAEVT structure if there is no more structure	*/
/*		  available in free list				*/
/* called func.	: pat_addpaevt						*/
/* ###--------------------------------------------------------------### */

static struct paevt *lcl_addpaevt (lastpaevt, index, usrval)

struct paevt   *lastpaevt;	/* pointer on the last paevt structure	*/
unsigned short  index;		/* index of the input-output		*/
char            usrval;		/* user predicted value			*/

  {
  struct paevt *ptevt;


  if (FRE_EVTPNT == NULL)
     ptevt = pat_addpaevt (lastpaevt, index, usrval);
  else
    {
    ptevt      = FRE_EVTPNT;
    FRE_EVTPNT = ptevt->NEXT;

    ptevt->INDEX  = index;
    ptevt->USRVAL = usrval;
    ptevt->SIMVAL = usrval;
    ptevt->NEXT   = lastpaevt;
    }

  return (ptevt);
  }

/* ###--------------------------------------------------------------### */
/* function	: yyerror						*/
/* description	: print an error message (errors detected by yacc)	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: tobin							*/
/* description	: change to binary					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void tobin (value, str1, str2)

char   value;
char **str1 ;
char **str2 ;

  {
  switch (value)
    {
    case '*':
      *str1 = "****"; *str2 = "****"; break;
    case '-':
    case '0':
      *str1 = "0000"; *str2 = "----"; break;
    case '+':
    case '1':
      *str1 = "0001"; *str2 = "---+"; break;
    case '2':
      *str1 = "0010"; *str2 = "--+-"; break;
    case '3':
      *str1 = "0011"; *str2 = "--++"; break;
    case '4':
      *str1 = "0100"; *str2 = "-+--"; break;
    case '5':
      *str1 = "0101"; *str2 = "-+-+"; break;
    case '6':
      *str1 = "0110"; *str2 = "-++-"; break;
    case '7':
      *str1 = "0111"; *str2 = "-+++"; break;
    case '8':
      *str1 = "1000"; *str2 = "+---"; break;
    case '9':
      *str1 = "1001"; *str2 = "+--+"; break;
    case 'A':
    case 'a':
      *str1 = "1010"; *str2 = "+-+-"; break;
    case 'B':
    case 'b':
      *str1 = "1011"; *str2 = "+-++"; break;
    case 'C':
    case 'c':
      *str1 = "1100"; *str2 = "++--"; break;
    case 'D':
    case 'd':
      *str1 = "1101"; *str2 = "++-+"; break;
    case 'E':
    case 'e':
      *str1 = "1110"; *str2 = "+++-"; break;
    case 'F':
    case 'f':
      *str1 = "1111"; *str2 = "++++"; break;

    default : 
      *str1 = "0000"; *str2 = "----"; break;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: pshnam						*/
/* description	: push a name on the NAMTAB stack to build a		*/
/*		  hierarchical name					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void pshnam (str)

char *str;

  {
  if (NAM_IDX > 30)
    ERR_FLG += pat_error (14, NULL, ' ', PAT_LINNUM);
  else
    {
    NAMTAB [NAM_IDX] = str;
    NAM_IDX ++;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: addini						*/
/* description	: add a list of PAINI structure at the top of the list	*/
/* called func.	: lcl_addpaini, tobin					*/
/* ###--------------------------------------------------------------### */

static struct paini *addini (lastini, gname, value)

struct paini *lastini;
struct array  gname  ;
char         *value  ;

  {
  struct paini *ptini        = lastini;
  char         *frc          ;
  char         *cmp          ;
  long           length       ;
  long           size         = 1;
  long           index        = 1;
  long           i            ;
  long           inc          ;
  char          extname [100];
  char          format       = 'B';

  if (gname.left < gname.right)
    {
    length = gname.right - gname.left + 1;
    inc    = 1;
    }
  else
    {
    length = gname.left - gname.right + 1;
    inc    = -1;
    }

  switch (value [0])
    {
    case 'X' :
    case 'x' :
      size   = (strlen (value) - 3) * 4;
      format = 'X';
      index  = 2  ;
      break;
    case 'O' :
    case 'o' :
      size   = (strlen (value) - 3) * 3;
      format = 'O';
      index  = 2  ;
      break;
    case 'B' :
    case 'b' :
      size   = strlen (value) - 3;
      format = 'B';
      index  = 2  ;
      break;
    case '"' :
      size   = strlen (value) - 2;
      format = 'B';
      index  = 1  ;
      break;
    case '\'' :
      size   = 1  ;
      format = 'B';
      index  = 1  ;
      break;
    }

  if (length != size)
    ERR_FLG += pat_error (12, gname.ident, ' ', PAT_LINNUM);
  else
    {
    if (gname.left == -1)
      ptini = lcl_addpaini (ptini, gname.ident, value[index]);
    else
      {
      for (i=gname.left ; i!=(gname.right+inc) ;)
        {
        tobin  (value[index], &frc, &cmp);
        switch (format)
          {
          case 'X':
          case 'x':
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[0]);
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[1]);
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[2]);
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[3]);
            break;
          case 'O':
          case 'o':
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[1]);
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[2]);
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[3]);
            break;
          case 'B':
          case 'b':
            sprintf (extname, "%s %ld", gname.ident, i);
            i += inc;
            ptini = lcl_addpaini (ptini, extname, frc[3]);
            break;
          }
        index++;
        }
      }
    }
  return (ptini);
  }

/* ###--------------------------------------------------------------### */
/* function	: isevent						*/
/* description	: detect an event on an input-output and add if needed	*/
/*		  a PAEVT structure at the top of the list		*/
/* called func.	: tobin, lcl_addpaevt					*/
/* ###--------------------------------------------------------------### */

static struct paevt *isevent (lastevt, index, compare, value, rank)

struct paevt *lastevt;
short         index  ;
char          compare;
char          value  ;
long           rank   ;

  {
  struct paiol *ptiol  ;
  long           i      ;
  char          cmp_val;
  char          frc_val;
  char         *frc    ;
  char         *cmp    ;

  ptiol = PAT_SEQPNT->PAIOL + index;
  if ((value == '*') || (value == '-') || (value == '+'))
    compare = 'C';

  tobin (value, &frc, &cmp);
  switch (ptiol->MODE)
    {
    case 'I':
      for (i=rank ; i<4 ; i++)
        {
        frc_val = frc [i];
        if (VALUE [index] != frc_val)
          {
          lastevt       = lcl_addpaevt (lastevt, index, frc_val); 
          VALUE [index] = frc_val;
          }
        index++;
        }
      break;
    case 'O':
    case 'B':
    case 'Z':
    case 'S':
    case 'W':
    case 'X':
    case 'U':
    case 'R':
      for (i=rank ; i<4 ; i++)
        {
        cmp_val = cmp[i];
        if ((VALUE[index] != cmp_val) || (cmp_val == '*'))
          {
          lastevt = lcl_addpaevt (lastevt, index, cmp_val); 
          VALUE[index]  = cmp_val;
          }
        index++;
        }
      break;
    case 'T':
      if (compare == 'F')
        {
        for (i=rank ; i<4 ; i++)
          {
          frc_val = frc[i];
          if (VALUE[index] != frc_val)
            {
            lastevt = lcl_addpaevt (lastevt, index, frc_val); 
            VALUE[index]  = frc_val;
            }
          index++;
          }
        }
      else
        {
        for (i=rank ; i<4 ; i++)
          {
          cmp_val = cmp[i];
          if ((VALUE[index] != cmp_val) || (cmp_val == '*'))
            {
            lastevt = lcl_addpaevt (lastevt, index, cmp_val); 
            VALUE[index]  = cmp_val;
            }
          index++;
          }
        }
      break;
    }
  return (lastevt);
  }

/* ###--------------------------------------------------------------### */
/* function	: islegal						*/
/* description	: check if a value is legal				*/
/* called func.	: pat_error						*/
/* ###--------------------------------------------------------------### */

static void islegal (index, value, flag, status)

long  index;				/* ptiol's index		*/
char value;				/* value (+,-,*,0,...,9,a,...,f)*/
char flag;				/* comparison flag (C,F)	*/
long  status;				/* (0,1,2) (bit,array,inside)	*/

  {
  long           code = -1;
  char          comp = flag;
  struct paiol *ptiol;

	/* ###------------------------------------------------------### */
	/*   Giving a code to each group of values			*/
	/* ###------------------------------------------------------### */

  switch (value)
    {
    case '*':
      code = 0; comp = 'C'; break;
    case '0':
    case '1':
      code = 1; break;
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      code = 2; break;
    case '8':
    case '9':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      code = 3; break;
    case '+':
    case '-':
      code = 4; comp = 'C'; break;
    }

	/* ###------------------------------------------------------### */
	/*   Checking parameters' consistency :				*/
	/*     '+' or '-' preceded by a '?'		-> error	*/
	/*     '+' or '-' for an array			-> error	*/
	/*	unknown value				-> error	*/
	/* ###------------------------------------------------------### */

  if ((code == 4) && (flag == 'C'))
    ERR_FLG += pat_error (2, NULL, '?', PAT_LINNUM);

  if ((code == 4) && (status != 0))
    ERR_FLG += pat_error (2, NULL, value, PAT_LINNUM);

  if (code == -1)
    ERR_FLG += pat_error (2, NULL, value, PAT_LINNUM);
  else
    {

	/* ###------------------------------------------------------### */
	/*   Checking coherence of value and input-output format :	*/
	/*     2,...,9,a,...,f and binary format	-> error	*/
	/*     a,...,f,+,-     and octal  format	-> error	*/
	/*     +,-             and hexa.  format	-> error	*/
	/* ###------------------------------------------------------### */

    flag  = comp;
    ptiol = PAT_SEQPNT->PAIOL + index;
    switch (ptiol->FORMAT)
      {
      case 'b' :
      case 'B' :
        if ((code == 2) || (code == 3))
          ERR_FLG += pat_error (10, ptiol->NAME, 'B', PAT_LINNUM);
        break;
      case 'o' :
      case 'O' :
        if (code >= 3)
          ERR_FLG += pat_error (10, ptiol->NAME, 'O', PAT_LINNUM);
        break;
      case 'x' :
      case 'X' :
        if (code == 4)
          ERR_FLG += pat_error (10, ptiol->NAME, 'X', PAT_LINNUM);
        break;
      }

	/* ###------------------------------------------------------### */
	/*   Checking coherence of value and input-output mode		*/
	/*     compare a value on an input		-> error	*/
	/*     put an undefined value on an input	-> error	*/
	/*     force a value on an output		-> error	*/
	/* ###------------------------------------------------------### */

    switch (ptiol->MODE)
      {
      case 'I' :
        if ((flag == 'C') || (code == 4) || (code == 0))
          ERR_FLG += pat_error (5, ptiol->NAME, ' ', PAT_LINNUM);
        break;
      case 'O' :
      case 'R' :
      case 'S' :
        if (flag != 'C')
          ERR_FLG += pat_error (4, ptiol->NAME, ' ', PAT_LINNUM);
        break;
      }
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: pat_descpat						*/
/* description	: parse a file in pat format and return a PASEQ		*/
/* called func.	: yyparse, reverse, mbkalloc, mbkfree, pat_error	*/
/* ###--------------------------------------------------------------### */

struct paseq *pat_descpat (fp, ptseq, maxpat, mode)

FILE          *fp    ;
struct paseq  *ptseq ;
unsigned long   maxpat;
unsigned char  mode  ;			/* the description style	*/

  {
  struct papat *ptpat  ;
  struct pacom *ptcom  ;
  struct paevt *ptevt  ;
  struct paini *ptini  ;

  struct papat *lastpat;
  extern FILE  *pat_decl_y_in;
  long           i      ;

  if ((ptseq == NULL) || (ptseq->ERRFLG != 0))
    ERR_FLG += pat_error (100, NULL, ' ', 0);
  else
    {
    if (ptseq->SUBSEQ == 0)
      {
	/* ###------------------------------------------------------### */
	/*   for the first subsequence initialize:			*/
	/*     - the parsing style (strict/non strict delays - ignore	*/
	/*       delays)						*/
	/*     - the current date					*/
	/*     - the time unit conversion variables			*/
	/*     - an array for values					*/
	/* ###------------------------------------------------------### */

      PAT_MODE = mode;
      CURDATE  = 0   ;

      switch (PAT_TIMEUNIT)
        {
        case PAT_TU__FS :
          PAT_CNVFS = 1      ;
          PAT_CNVPS = 1.0E+3 ;
          PAT_CNVNS = 1.0E+6 ;
          PAT_CNVUS = 1.0E+9 ;
          PAT_CNVMS = 1.0E+12;
          break;

        case PAT_TU__PS :
          PAT_CNVFS = 1.0E-3 ;
          PAT_CNVPS = 1      ;
          PAT_CNVNS = 1.0E+3 ;
          PAT_CNVUS = 1.0E+6 ;
          PAT_CNVMS = 1.0E+9 ;
          break;

        case PAT_TU__NS :
          PAT_CNVFS = 1.0E-6 ;
          PAT_CNVPS = 1.0E-3 ;
          PAT_CNVNS = 1      ;
          PAT_CNVUS = 1.0E+3 ;
          PAT_CNVMS = 1.0E+6 ;
          break;

        case PAT_TU__US :
          PAT_CNVFS = 1.0E-9 ;
          PAT_CNVPS = 1.0E-6 ;
          PAT_CNVNS = 1.0E-3 ;
          PAT_CNVUS = 1      ;
          PAT_CNVMS = 1.0E+3 ;
          break;

        case PAT_TU__MS :
          PAT_CNVFS = 1.0E-12;
          PAT_CNVPS = 1.0E-9 ;
          PAT_CNVNS = 1.0E-6 ;
          PAT_CNVUS = 1.0E-3 ;
          PAT_CNVMS = 1      ;
          break;

        default :
          PAT_CNVFS = 1      ;
          PAT_CNVPS = 1      ;
          PAT_CNVNS = 1      ;
          PAT_CNVUS = 1      ;
          PAT_CNVMS = 1      ;
          ERR_FLG  += pat_error (200, NULL, 0, 0);
        }

      if (ERR_FLG == 0)
        {
        PAT_CNVFS = PAT_CNVFS / PAT_TIMESTEP;
        PAT_CNVPS = PAT_CNVPS / PAT_TIMESTEP;
        PAT_CNVNS = PAT_CNVNS / PAT_TIMESTEP;
        PAT_CNVUS = PAT_CNVUS / PAT_TIMESTEP;
        PAT_CNVMS = PAT_CNVMS / PAT_TIMESTEP;
        }

      VALUE = mbkalloc (sizeof (char) * (ptseq->IOLNBR));

      for (i=0 ; i<(ptseq->IOLNBR) ; i++)
         VALUE [i] = '*';
      }

    else
      {
	/* ###------------------------------------------------------### */
	/*   for other subsequences release old PAPAT, PACOM, PAINI	*/
	/* and PAEVT structures						*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*   release PACOM of the old subsequence. Move the comments of	*/
	/* the previous subsequence to old.				*/
	/* ###------------------------------------------------------### */

      ptcom = ptseq->OLDCOM;

      if (ptcom != NULL)
        {
        while (ptcom->NEXT != NULL)
          ptcom = ptcom->NEXT;
        }

      if (ptcom != NULL)
        {
        ptcom->NEXT   = FRE_COMPNT;
        FRE_COMPNT    = ptseq->OLDCOM;
        }

      ptseq->OLDCOM = ptseq->CURCOM;
      ptseq->CURCOM = NULL;

	/* ###------------------------------------------------------### */
	/*   scan the PAPAT list of the old subsequence to release it	*/
	/* and ...							*/
	/* ###------------------------------------------------------### */

      lastpat = NULL;
      ptpat   = ptseq->OLDPAT;

      while ((ptpat != NULL) && (ptpat != ptseq->CURPAT))
        {
	/* ###------------------------------------------------------### */
	/*   release PAEVT of the current PAPAT				*/
	/* ###------------------------------------------------------### */

        ptevt = ptpat->PAEVT;

        if (ptevt != NULL)
          {
          while (ptevt->NEXT != NULL)
            ptevt = ptevt->NEXT;
          }

        if (ptevt != NULL)
          {
          ptevt->NEXT = FRE_EVTPNT  ;
          FRE_EVTPNT  = ptpat->PAEVT;
          }

	/* ###------------------------------------------------------### */
	/*   release PAINI of the current PAPAT				*/
	/* ###------------------------------------------------------### */

        ptini = ptpat->PAINI;

        if (ptini != NULL)
          {
          while (ptini->NEXT != NULL)
            ptini = ptini->NEXT;
          }

        if (ptini != NULL)
          {
          ptini->NEXT = FRE_INIPNT  ;
          FRE_INIPNT  = ptpat->PAINI;
          }

        ptpat->PAINI = NULL;
        ptpat->PAEVT = NULL;

        lastpat = ptpat      ;
        ptpat   = ptpat->NEXT;
        }

      if (lastpat != NULL)
        {
        lastpat->NEXT = FRE_PATPNT   ;
        FRE_PATPNT    = ptseq->OLDPAT;
        }

      ptseq->OLDPAT = ptseq->CURPAT;
      ptseq->CURPAT = ptseq->NXTPAT;
      }

	/* ###------------------------------------------------------### */
	/*   initialization						*/
	/* ###------------------------------------------------------### */

    if (ERR_FLG != 0)
      ptseq->ERRFLG = 1;
    else
      {
      if (maxpat == 0)
        {
        DECVAL = 0;
        MAXPAT = 1;
        }
      else
        {
        DECVAL = 1     ;
        MAXPAT = maxpat;
        }

      PAT_SEQPNT         = ptseq;
      PAT_SEQPNT->PATNBR = 0    ;

      PAT_COMPNT         = ptseq->CURCOM;
      PAT_PATPNT         = ptseq->CURPAT;
      PAT_INIPNT         = NULL         ;

      if (ptseq->NXTPAT == NULL)
        STT_NBR = 0;
      else
        STT_NBR = 1;

      if (ptseq->SUBSEQ == 0)
        MAXPAT++;

      PAT_LINNUM = ptseq->LINENO;
      NAM_IDX    = 0            ;

      pat_decl_y_in       = fp;

	/* ###------------------------------------------------------### */
	/*   call the parser and, prepare the returned structure	*/
	/* ###------------------------------------------------------### */

      pat_desc_y_parse ();

      if (ptseq->SUBSEQ == 0)
        PAT_SEQPNT->PATNBR --;

      if (PAT_SEQPNT->ENDFLG == 'Y')
        {
        PAT_SEQPNT->NXTPAT = NULL;
        PAT_SEQPNT->PATNBR ++;
        }
      else
        {
        PAT_SEQPNT->NXTPAT = PAT_PATPNT;
        }

      PAT_SEQPNT->CURPAT = (struct papat *)reverse ((struct chain *)PAT_PATPNT);
      PAT_SEQPNT->CURCOM = (struct pacom *)reverse ((struct chain *)PAT_COMPNT);
      PAT_SEQPNT->LINENO = PAT_LINNUM;

      if (ERR_FLG == 0)
        PAT_SEQPNT->ERRFLG = 0;
      else
        PAT_SEQPNT->ERRFLG = 1;

      PAT_SEQPNT->SUBSEQ++;

	/* ###------------------------------------------------------### */
	/*   if the end of file has bee reached release the value array	*/
	/* ###------------------------------------------------------### */

      if (PAT_SEQPNT->ENDFLG == 'Y')
        {
        mbkfree (VALUE);
        }

      ptseq = PAT_SEQPNT;
      }
    }

  return (ptseq);
  }

%}

%union
  {
  long           valu;
  float         fval;
  char          immd;
  char         *text;
  struct array  arra;
  struct label  labe;
  }

%token        _GTSym
%token        _IN
%token        _INOUT
%token        _LESym
%token        _LTSym
%token        _OUT
%token <valu> AbstractLit
%token <text> BitStringLit
%token        BEGIN_
%token        Colon
%token        Comma
%token <text> Comment
%token        DOWNTO
%token        Dot
%token        END_
%token        FS
%token <immd> Format
%token        INSPECT
%token <text> Identifier
%token        LeftParen
%token <text> Literal
%token        NS
%token        MS
%token        PS
%token        Plus
%token        REGISTER
%token        RightParen
%token        SAVE
%token        SIGNAL
%token <valu> Semicolons
%token        SPY
%token        TO
%token        TRACE
%token        US
%token        UnknownChar

%type  <arra>  name
%type  <arra>  simple_name
%type  <arra>  indexed_name
%type  <arra>  slice_name
%type  <labe> .label.
%type  <text>  Identifier...Identifier..
%type  <valu>  Semicolons_ERR
%type  <valu>  date
%type  <valu>  relative_date
%type  <valu>  absolute_date
%type  <fval>  time_unit
%type  <text> .Identifier.

/* %pure_parser */
%start        pat_format_file

%%
pat_format_file
	: description_block
	  .save_statement.
	  END_ERR
	  Semicolons_ERR
		{ PAT_SEQPNT->ENDFLG = 'Y'; }
	;

description_block
	: ..description_item..
	;

..description_item..
	: /*empty*/
	| ..description_item..
	  description_item
	;

description_item
	: pattern_description
	| other_statement
	| error
	  Semicolons_ERR
	;

other_statement
	: initialization_statement
	| Comment
		{
		PAT_COMPNT = lcl_addpacom (PAT_COMPNT, $1, STT_NBR);
		STT_NBR    = 0;
		}
	;

name
	: simple_name
		{ $$ = $1; }
	| indexed_name
		{ $$ = $1; }
	| slice_name
		{ $$ = $1; }
	;

simple_name
	: Identifier...Identifier..
		{
		$$.ident = $1;
		$$.left  = -1;
		$$.right = -1;
		}
	;

indexed_name
	: Identifier...Identifier..
	  LeftParen
	  AbstractLit
	  RightParen
		{
		$$.ident = $1;
		$$.left  = $3;
		$$.right = $3;
		}
	;

slice_name
	: Identifier...Identifier..
	  LeftParen
	  AbstractLit
	  direction
	  AbstractLit
	  RightParen
		{
		$$.ident = $1;
		$$.left  = $3;
		$$.right = $5;
		}
	;

Identifier...Identifier..
	: Identifier
	  ...Identifier..
		{
		char name[256];
		long  i;

		strcpy (name, $1);
		for (i=0; i<NAM_IDX ; i++)
		 {
		 strcat (name, ".");
		 strcat (name, NAMTAB [i]);
		 }
		NAM_IDX = 0;
		$$ = namealloc (name);
		}
	;

...Identifier..
	: /*empty*/
	| ...Identifier..
	  Dot
	  Identifier
		{ pshnam ($3); }
	;

direction
	: TO
	| DOWNTO
	;

initialization_statement
	: name
	  _LESym
	  BitStringLit
	  Semicolons_ERR
		{ PAT_INIPNT = addini (PAT_INIPNT, $1, $3); }
	;

.save_statement.
	: /*empty*/
	| SAVE
	  Semicolons_ERR
		{ PAT_SEQPNT->SAVFLG = 'Y'; }
	;

pattern_description
	: .label.
	  unlabeled_pattern
		{
		if (PAT_INIPNT == NULL)
		  PAT_PATPNT->ACTFLAG = 'U';
		else
		  PAT_PATPNT->ACTFLAG = 'I';

		PAT_PATPNT->TIME  = $1.date ;
		PAT_PATPNT->LABEL = $1.label;

		PAT_PATPNT->PAINI = (struct paini *)
		                    reverse ((struct chain *) PAT_INIPNT);
		PAT_INIPNT        = NULL;

		MAXPAT -= DECVAL;
		PAT_SEQPNT->PATNBR++;

		if (MAXPAT == 0)
		  YYACCEPT;
		}
	;

unlabeled_pattern
	: Colon
	  Literal
	  Semicolons_ERR
		{
		char            c     = '\0';
		long             i     = 0   ;
		unsigned short  idx   = 0   ;
		char            endfl = 'Y' ;
		char            cmpfl = 'F' ;
		struct pagrp   *ptgrp = PAT_SEQPNT->PAGRP;
		struct paiol   *ptiol = PAT_SEQPNT->PAIOL;
		struct paevt   *ptevt = NULL;

		while (idx < PAT_SEQPNT->IOLNBR)
		  {
		  if (((ptiol + idx)->FLAG & PAT_IOL__TRACE) != 0)
		    c = '*';
		  else
		    c = $2 [i++];

		  if (c == '\0')
		    break;

		/* ###----------------------------------------------### */
		/*   if the end of the current array is reached go to	*/
		/* the next group					*/
		/* ###----------------------------------------------### */

		  if ((ptgrp !=  NULL                          ) &&
		      (idx   == (ptgrp->LENGTH + ptgrp->FINDEX))   )
		    {
		    ptgrp = ptgrp->NEXT;
		    endfl = 'Y';
		    }

		/* ###----------------------------------------------### */
		/*   otherwise, continue scanning the group		*/
		/* ###----------------------------------------------### */

		  if (endfl == 'N')
		    {
		    islegal (idx, c, cmpfl, 2);
		    switch  ((ptiol + idx)->FORMAT)
		      {
		      case 'X':
		        ptevt  = isevent (ptevt, idx, cmpfl, c, 0);
		        idx   += 4;
		        break;
		      case 'O':
		        ptevt  = isevent (ptevt, idx, cmpfl, c, 1);
		        idx   += 3;
		        break;
		      case 'B':
		        ptevt = isevent (ptevt, idx, cmpfl, c, 3);
		        idx++;
		        break;
		      }
		    }

		  else
		    {
		/* ###----------------------------------------------### */
		/*   beginning a new array or a new single bit		*/
		/* ###----------------------------------------------### */

		    if (c == '?')
		      {
		      cmpfl = 'C';
		      endfl = 'n';
		      }
		    else
		      {
		      if (endfl == 'Y')
		        cmpfl = 'F';

		/* ###----------------------------------------------### */
		/*   if beginning a new array				*/
		/* ###----------------------------------------------### */

		      if ((ptgrp != NULL) && (idx == ptgrp->FINDEX))
		        {
		        islegal (idx, c, cmpfl, 1);
		        endfl = 'N';
		        switch ((ptiol + idx)->FORMAT)
		          {
		          case 'X':
		            switch (ptgrp->LENGTH % 4)
		              {
		              case 1:
		                ptevt = isevent (ptevt, idx, cmpfl, c, 3);
		                idx++;
		                break;
		              case 2:
		                ptevt  = isevent (ptevt, idx, cmpfl, c, 2);
		                idx   += 2;
		                break;
		              case 3:
		                ptevt  = isevent (ptevt, idx, cmpfl, c, 1);
		                idx   += 3;
		                break;
		              case 0:
		                ptevt  = isevent (ptevt, idx, cmpfl, c, 0);
		                idx   += 4;
		                break;
		              }
		            break;
		          case 'O':
		            switch (ptgrp->LENGTH % 3)
		              {
		              case 1:
		                ptevt = isevent (ptevt, idx, cmpfl, c, 3);
		                idx++;
		                break;
		              case 2:
		                ptevt  = isevent (ptevt, idx, cmpfl, c, 2);
		                idx   += 2;
		                break;
		              case 0:
		                ptevt  = isevent (ptevt, idx, cmpfl, c, 1);
		                idx   += 3;
		                break;
		              }
		            break;
		          case 'B':
		            ptevt = isevent (ptevt, idx, cmpfl, c, 3);
		            idx++;
		            break;
		          }
		        }

		      else
		        {
		/* ###----------------------------------------------### */
		/*    Beginning a new single bit				*/
		/* ###----------------------------------------------### */

		        islegal (idx, c, cmpfl, 0);
		        ptevt = isevent (ptevt, idx, cmpfl, c, 3);
		        cmpfl = 'F';
		        idx++;
		        }
		      }
		    }
		  }

		if ((c != '\0') && ($2 [i] != '\0'))
{
printf ("pat : %s < i : %ld < index : %d\n", $2, i, idx);
		  ERR_FLG += pat_error (3, NULL, ' ', PAT_LINNUM);
}

		if (idx < PAT_SEQPNT->IOLNBR)
		  ERR_FLG += pat_error (15, NULL, ' ', PAT_LINNUM);

		PAT_PATPNT = lcl_addpapat (PAT_PATPNT, NULL, PAT_LINNUM);

		PAT_PATPNT->PAEVT = ptevt;
		PAT_PATPNT->BLANK = $3;
		STT_NBR++;
		}
	;

.label.
	: .Identifier.
		{
		if ((PAT_MODE & PAT_DLY__STRICT) != 0)
		  ERR_FLG += pat_error (16, NULL, ' ', PAT_LINNUM);

		$$.label = $1     ;
		$$.date  = CURDATE;
		}
	| _LTSym
	  date
	  _GTSym
	  .Identifier.
		{
		if ((PAT_MODE & PAT_DLY__IGNORE) != 0)
		  {
		  $$.label = $4;
		  $$.date  = 0 ;
		  }
		else
		  {
		  if ($2 < 0)
		    ERR_FLG += pat_error (17, NULL, ' ', PAT_LINNUM);

		  if ($2 != 0)
		    PAT_SEQPNT->FLAGS &= ~PAT_SEQ__ZERODELAY;

		  $$.label = $4;
		  $$.date  = $2;
		  }
		}
	;

.Identifier.
	: /*empty*/
		{ $$ = NULL; }
	| Identifier
		{ $$ = $1  ; }
	;

date
	: absolute_date
		{
		if ((PAT_MODE & PAT_DLY__STRICT) != 0)
		  {
		  if (($1                 <= CURDATE) &&
		      (PAT_SEQPNT->SUBSEQ == 0      ) &&
		      (PAT_SEQPNT->CURPAT == NULL   )   )
		    ERR_FLG += pat_error (16, NULL, ' ', PAT_LINNUM);
		  }

		CURDATE = $1;
		$$      = $1;
		}
	| relative_date
		{
		if ((PAT_MODE & PAT_DLY__STRICT) != 0)
		  {
		  if (($1                 <= CURDATE) &&
		      (PAT_SEQPNT->SUBSEQ == 0      ) &&
		      (PAT_SEQPNT->CURPAT == NULL   )   )
		    ERR_FLG += pat_error (16, NULL, ' ', PAT_LINNUM);
		  }

		CURDATE += $1     ;
		$$       = CURDATE;
		}
	;

absolute_date
	: AbstractLit
	  time_unit
		{ $$ = $1 * $2; }
	;

relative_date
	: Plus
	  AbstractLit
	  time_unit
		{ $$ = $2 * $3; }
	;

time_unit
	: FS
		{ $$ = PAT_CNVFS; }
	| PS
		{ $$ = PAT_CNVPS; }
	| NS
		{ $$ = PAT_CNVNS; }
	| US
		{ $$ = PAT_CNVUS; }
	| MS
		{ $$ = PAT_CNVMS; }
	;

END_ERR
	: END_
		{ yyerrok; }
	;

Semicolons_ERR
	: Semicolons
		{
		if (ERR_FLG > 30)
		  YYABORT;
		else
		   yyerrok;
		}
	;
%%
