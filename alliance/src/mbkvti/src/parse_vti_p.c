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
 
/*******************************************************************************
* mbk : vti parser for symbolic layout view : cp format v7r5                   *
*                                                                              *
* Based upon material provided by Alain Greiner in 1990, in lex and yacc       *
*                                                                              *
* version 0.0 written by Hichang Li in june 1991 under the supervision of      *
*             Frederic Petrot                                                  *
* version 1.0 is basically an update of version 0.0 written by Frederic        *
*             Petrot on september 1991                                         *
*                                                                              *
* New versions are mostly bug corrections, or standardization matters and are  *
* done by Frederic Petrot, since middle 1990.                                  *
*                                                                              *
* version : 3.06                                                               *
* date    : 03/08/92                                                           *
*******************************************************************************/

/*
 * $Log: parse_vti_p.c,v $
 * Revision 1.4  2009/06/14 13:51:58  ludo
 * - port 64 bits
 *
 * Revision 1.3  2002/09/30 16:21:00  czo
 * support/users
 *
 * Revision 1.2  2002/03/14 12:34:30  fred
 * Using new headers
 *
 * Revision 1.1.1.1  2002/03/08 14:17:47  fred
 * Importing MBKVTI sources into the new CVS tree
 *
 * Revision 1.7  2001/02/21 15:37:40  fred
 * New layers
 *
 * Revision 1.6  2000/02/22 14:00:12  syf
 * IDEM
 *
 * Revision 1.5  2000/02/09 10:09:45  syf
 * Hello world
 *
 * Revision 1.4  2000/02/07 16:55:36  syf
 * BIGVIA TURNVIA et CALUx
 *
 * Revision 1.3  2000/01/12 10:00:47  syf
 * Nouveau PHVIA
 *
 * Revision 1.2  1999/07/12 16:16:24  ludo
 * Modif pour Franck
 *
 * Revision 1.1.1.1  1998/10/02 15:27:13  alliance
 * Imported by czo
 *
 * Revision 5.1  1998/09/16 14:48:38  mbkvti
 * rcs version number consistency
 *
 * Revision 5.0  1998/09/16 14:46:20  mbkvti
 * rcs version number consistency
 *
 */
#ident "$Id: parse_vti_p.c,v 1.4 2009/06/14 13:51:58 ludo Exp $"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <mut.h>
#include <mph.h>
#include <mpu.h>

#define LSIZE 16384                        /* max line size for fgets */

enum {CPPREF_ERROR, CPIREF_ERROR, CPCREF_ERROR, SYNTAX_ERROR, VIA_ERROR,
      BIGVIA_ERROR, REF_ERROR, OPEN_ERROR, PARSE_ERROR, CLOSE_ERROR};


/*******************************************************************************
* These macros are big but are supposed to speed up the whole thing            *
* All arguments are passed per values, this is different from old versions     *
*******************************************************************************/

/*******************************************************************************
* macro cppref()                                                               *
*******************************************************************************/
#define cppref(px, py, refnum, lineno) \
{ \
point_list *ptpoint; \
 \
   for (ptpoint = head_point; ptpoint; ptpoint = ptpoint->NEXT) \
      if (ptpoint->INDEX == refnum) \
         break; \
 \
   if (ptpoint == (point_list *)NULL) \
      cp_error((int)CPPREF_ERROR, fname, lineno, refnum); \
 \
   px = ptpoint->X; \
   py = ptpoint->Y; \
}

/*******************************************************************************
* macro cpcref()                                                               *
*******************************************************************************/
#define cpcref(px, py, refnum, lineno) \
{ \
phcon_list  *ptcon; \
 \
   for (ptcon = figure->PHCON; ptcon; ptcon = ptcon->NEXT) \
      if (((long)(getptype(ptcon->USER, (long)VTIPARSER)->DATA)) == refnum) \
         break; \
 \
   if (ptcon == (phcon_list *)NULL) \
      cp_error((int)CPCREF_ERROR, fname, lineno, refnum); \
\
   px = ptcon->XCON; \
   py = ptcon->YCON; \
}

/*******************************************************************************
* macro cpiref()                                                               *
*******************************************************************************/
#define cpiref(px, py, insname, refnum, lineno) \
{ \
phfig_list *ptnewfig; \
phcon_list *ptcon; \
phins_list *ptins; \
 \
   ptins = getphins(figure, insname); \
   ptnewfig = getphfig(ptins->FIGNAME, 'P'); \
   for (ptcon = ptnewfig->PHCON; ptcon; ptcon = ptcon->NEXT) \
      if ((long)(getptype(ptcon->USER, (long)VTIPARSER)->DATA) == refnum) \
         break; \
 \
   if (ptcon == (phcon_list *)NULL) \
      cp_error((int)CPIREF_ERROR, fname, lineno, refnum); \
 \
   xyflat(&px, &py, ptcon->XCON, ptcon->YCON, ptins->XINS, ptins->YINS, \
           ptnewfig->XAB1, ptnewfig->YAB1, ptnewfig->XAB2, ptnewfig->YAB2, \
           ptins->TRANSF); \
}
/*******************************************************************************
* macro used to add a point to the point list                                  *
*******************************************************************************/
#define addpoint(xp,yp) \
{ \
point_list *point; \
   point = (point_list *)mbkalloc(sizeof(point_list)); \
   point->INDEX = ++index; \
   point->X = xp; \
   point->Y = yp; \
   point->NEXT = head_point; \
   head_point = point; \
}

/*******************************************************************************
* cp_error : generic error routine for vtiloadphfig function                   *
*******************************************************************************/
static void cp_error(type, name, line, parameter)
int type, line;
long parameter;
char *name;
{

   (void)fflush(stdout);
   (void)fprintf(stderr, "*** mbk error ***\n");
   switch (type) {
      case CPPREF_ERROR :
         (void)fprintf(stderr,
            "illegal segment reference : point P %ld doesn't exist",
            parameter);
         break;
      case CPCREF_ERROR :
         (void)fprintf(stderr,
            "illegal segment reference : connector C %ld doesn't exist",
            parameter);
         break;
      case CPIREF_ERROR :
         (void)fprintf(stderr,
            "illegal segment reference : connector C %ld doesn't exist",
            parameter);
         break;
      case SYNTAX_ERROR :
         (void)fprintf(stderr, "syntax error");
         break;
      case VIA_ERROR :
         (void)fprintf(stderr, "D entry has unknown contact type");
         break;
      case BIGVIA_ERROR :
         (void)fprintf(stderr, "D entry has unimplement automatic generation of mega-contacts");
         break;
      case REF_ERROR :
         (void)fprintf(stderr, "unknown segment reference type");
         break;
      case OPEN_ERROR :
         (void)fprintf(stderr, "can't open file : %s", name);
         break;
      case PARSE_ERROR :
         (void)fprintf(stderr, "abnormal parsing for : %s", name);
         break;
      case CLOSE_ERROR :
         (void)fprintf(stderr, "can't close file : %s", name);
         break;
   }
   if (!line)
      (void)fprintf(stderr, "\nvtiloadphfig : parsing file %s.%s\n",
                  name, IN_PH);
   else
      (void)fprintf(stderr, "\nvtiloadphfig : parsing file %s.%s line %d\n",
                  name, IN_PH, line);
   EXIT(1);
}

#if 0
/* Somehow unuseful right now that double are 64 bits long */
/*******************************************************************************
* shift : shift a floating point number in order to x 1000 float approx failed *
*******************************************************************************/
#define shift(pt) \
   if (*(pt + 1) == '\0') { \
      *pt = '0'; \
      *(pt + 1) = '0'; \
      *(pt + 2) = '0'; \
      *(pt + 3) = '\0'; \
   } else if (*(pt + 2) == '\0') { \
      *pt = *(pt + 1); \
      *(pt + 1) = '0'; \
      *(pt + 2) = '0'; \
      *(pt + 3) = '\0'; \
   } else if (*(pt + 3) == '\0') { \
      *pt = *(pt + 1); \
      *(pt + 1) = *(pt + 2); \
      *(pt + 2) = '0'; \
      *(pt + 3) = '\0'; \
   } else { \
      *pt = *(pt + 1); \
      *(pt + 1) = *(pt + 2); \
      *(pt + 2) = *(pt + 3); \
      *(pt + 3) = '\0'; \
   }
#endif


static void vti_addphseg(phfig_list * figure, char layer, 
		       long l1, long x1, long y1,
                       long x2, long y2, char * sname)
{
	if ( ( x1 == x2 ) || ( y1 == y2 ) ) {
           (void)addphseg(figure, layer, l1, x1, y1, x2, y2, sname);
	}
	else 
		fprintf(stderr, "WARNING : illegal segment layer=%d w=%ld"
				" x1=%ld y1=%ld x2=%ld y2=%ld name=%s\n\n",
				layer, l1/SCALE_X, x1/SCALE_X, y1/SCALE_X, 
				x2/SCALE_X, y2/SCALE_X, 
                                ( sname != (char *)0 ) ? sname : "NULL" );
}

/*******************************************************************************
* stol : convert a string to a long                                            *
*******************************************************************************/
static long   stol(tokenpt)
char *tokenpt;
{
register char *pt;

   if ((pt = strchr(tokenpt, '.')) == (char *)NULL)
      return (long)(SCALE_X * atol(tokenpt));
   else {
#if 0
      shift(pt);
      return (long)(SCALE_X * atol(tokenpt) / 1000);
#endif
      return (long)((double)SCALE_X * atof(tokenpt));
   }
}

/*******************************************************************************
* fonction  codelayer()                                                        *
*******************************************************************************/
static char codelayer(layername)
char *layername;
{
   if (!strcmp(layername, "metal"))
      return ALU1;
   else if (!strcmp(layername, "metal2"))
      return ALU2;
   else if (!strcmp(layername, "metal3"))
      return ALU3;
   else if (!strcmp(layername, "metal4"))
      return ALU4;
   else if (!strcmp(layername, "metal5"))
      return ALU5;
   else if (!strcmp(layername, "metal6"))
      return ALU6;
   else if (!strcmp(layername, "metal7"))
      return ALU7;
   else if (!strcmp(layername, "metal8"))
      return ALU8;
   else if (!strcmp(layername, "metal9"))
      return ALU9;
   else if (!strcmp(layername, "allowM"))
      return TALU1;
   else if (!strcmp(layername, "allowM1"))
      return TALU1;
   else if (!strcmp(layername, "allowM2"))
      return TALU2;
   else if (!strcmp(layername, "allowM3"))
      return TALU3;
   else if (!strcmp(layername, "allowM4"))
      return TALU4;
   else if (!strcmp(layername, "allowM5"))
      return TALU5;
   else if (!strcmp(layername, "allowM6"))
      return TALU6;
   else if (!strcmp(layername, "allowM7"))
      return TALU7;
   else if (!strcmp(layername, "allowM8"))
      return TALU8;
   else if (!strcmp(layername, "allowM9"))
      return TALU9;
   else if (!strcmp(layername, "poly"))
      return POLY;
   else if (!strcmp(layername, "poly2"))
      return POLY2;
   else if (!strcmp(layername, "ndif"))
      return NDIF;
   else if (!strcmp(layername, "pdif"))
      return PDIF;
   else if (!strcmp(layername, "ntie"))
      return NTIE;
   else if (!strcmp(layername, "ptie"))
      return PTIE;
   else if (!strcmp(layername, "ntrans"))
      return NTRANS;
   else if (!strcmp(layername, "ptrans"))
      return PTRANS;
   else if (!strcmp(layername, "nwell"))
      return NWELL;
   else if (!strcmp(layername, "pwell"))
      return PWELL;
   return 255;
}

/*******************************************************************************
* checkname : check for busses                                                 *
*******************************************************************************/
static char *checkname(name)
char *name;
{
char *s, *t;

   s = t = name;
   while (*t) {
      if (*t == '[')
         *t = ' ';
      else if (*t == ']')
         if (*(++t) == '\0') /* ok, it's finished */
            goto end;
         else if (*t == '[') /* multiple array */
            continue;
      *s++ = *t++;
   }
end:
   *s = '\0';
   return name;
}

/*******************************************************************************
* physical parser : vti2mbk                                                    *
*******************************************************************************/
static int parse(figure, mode, in, fname)
phfig_list *figure;
char mode;
FILE *in;
char *fname;
{
char line[LSIZE];
char *t1, *t2, *t3, *t4, *t5;   /* pointers for token on a line  */
char *t6, *t7, *t8, *t9, *t10;  /*                               */
char *t11;                      /* of vti file */
char s[100];                    /* intermediate buffer to process tokens */
long l1, l2, l3, l4;            /* results from string to long */
long x1, x2, y1, y2;
point_list *head_point = (point_list *)NULL;
int index = 0;
char *sname = NULL;
int i = 1; /* file line number */

   while (1) {
      i++;
      if (fgets(line, LSIZE, in) == (char *)NULL)
         cp_error((int)SYNTAX_ERROR, fname, i, 0L);
      switch (line[0]) {
         case ' ' : /* comment */
         case '#' : /* comment */
            break;
         case 'A' :
            if (line[2] == '\\') /* comment */
               break;
            if (strchr(line + 2, '*') == (char *)NULL) { /* A num num num num */
               t1 = strtok(line + 2, " \n\t\"");
               (void)strcpy(s, t1);
               l1 = stol(s);
               t2 = strtok((char *)NULL, " \n\t\"");
               (void)strcpy(s, t2);
               l2 = stol(s);
               t3 = strtok((char *)NULL, " \n\t\"");
               (void)strcpy(s, t3);
               l3 = stol(s);
               t4 = strtok((char *)NULL, " \n\t\"");
               (void)strcpy(s, t4);
               l4 = stol(s);
               t5 = strtok((char *)NULL, " \n\t\"");
               if (t5 != (char *)NULL)
                  cp_error((int)SYNTAX_ERROR, fname, i, 0L);
               defab(figure, l1, l2, l3, l4);
            }
            break;
         case 'B' :   /* doesn't care about bounding box */
            break;
         case 'C' : /* C name num num name num num name name name name */
            if (mode != 'C') {
               t1 = strtok(line + 2, " \n\t\"");
               t2 = strtok((char *)NULL, " \n\t\"");
               (void)strcpy(s, t2);
               l1 = stol(s);
               t3 = strtok((char *)NULL, " \n\t\"");
               (void)strcpy(s, t3);
               l2 = stol(s);
               t4 = strtok((char *)NULL, " \n\t\"");
               t5 = strtok((char *)NULL, " \n\t\"");
               (void)strcpy(s, t5);
               l3 = stol(s);
               t6 = strtok((char *)NULL, " \n\t\"");
               l4 = atol(t6);
               t7 = strtok((char *)NULL, " \n\t\"");
               t8 = strtok((char *)NULL, " \n\t\"");
               t9 = strtok((char *)NULL, " \n\t\"");
               t10 = strtok((char *)NULL, " \n\t\"");
               t11 = strtok((char *)NULL, " \n\t\"");
               if (t11 != (char *)NULL)
                  cp_error((int)SYNTAX_ERROR, fname, i, 0L);
               {
               char   orient = '\0';
               if (!strcmp(t9, "NW"))
                  orient = WEST;
               else if (*t9 == 'N' || *t9 == 'n')
                  orient = NORTH;
               else if (*t9 == 'E' || *t9 == 'e')
                  orient = EAST;
               else if (*t9 == 'S' || *t9 == 's')
                  orient = SOUTH;
               else if (*t9 == 'W' || *t9 == 'w')
                  orient = WEST;
               (void)addphcon(figure, orient, checkname(t10), l1, l2,
                              codelayer(t4), l3);
               figure->PHCON->USER = (ptype_list *)addptype((ptype_list *)NULL,
                                       (long)VTIPARSER, (void *)l4);
               }
            }
            break;
         case 'D' : /* D num num name name num num num */
            t1 = strtok(line + 2, " \n\t\"");
            (void)strcpy(s, t1);
            l1 = stol(s);
            t2 = strtok((char *)NULL, " \n\t\"");
            (void)strcpy(s, t2);
            l2 = stol(s);
            t3 = strtok((char *)NULL, " \n\t\"");
            t4 = strtok((char *)NULL, " \n\t\"");
            t5 = strtok((char *)NULL, " \n\t\"");
            t6 = strtok((char *)NULL, " \n\t\"");
            (void)strcpy(s, t6);
            l3 = stol(s);
            t7 = strtok((char *)NULL, " \n\t\"");
            (void)strcpy(s, t7);
            l4 = stol(s);
            addpoint(l1, l2);
            if (mode != 'P') {
               if (isdigit(*t4)) /* for vti v8 compatibility */
                  break;         /* D num num name num num num */
               if (!strcmp(t4, "VIA"))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
               }
               else if (!strcmp(t4, "CONT"))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
               }
               else if (!strcmp(t4, "CONT2"))
               {
                     (void)addphvia(figure, CONT_POLY2, l1, l2, l3, l4,NULL);
               }
               else if (!strcmp(t4, "VIA2"))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
               }
               else if ((!strcmp(t4, "VIA01"))||(!strcmp(t4, "VI01")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
               }
               else if ((!strcmp(t4, "VIA02"))||(!strcmp(t4, "VI02")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA03"))||(!strcmp(t4, "VI03")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA04"))||(!strcmp(t4, "VI04")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA05"))||(!strcmp(t4, "VI05")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA06"))||(!strcmp(t4, "VI06")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA07"))||(!strcmp(t4, "VI07")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA08"))||(!strcmp(t4, "VI08")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA09"))||(!strcmp(t4, "VI09")))
               {
                     (void)addphvia(figure, CONT_POLY, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA12"))||(!strcmp(t4, "VI12")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
               }
               else if ((!strcmp(t4, "VIA13"))||(!strcmp(t4, "VI13")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA14"))||(!strcmp(t4, "VI14")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA15"))||(!strcmp(t4, "VI15")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA16"))||(!strcmp(t4, "VI16")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA17"))||(!strcmp(t4, "VI17")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA18"))||(!strcmp(t4, "VI18")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA19"))||(!strcmp(t4, "VI19")))
               {
                     (void)addphvia(figure, CONT_VIA, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
                  }

               else if ((!strcmp(t4, "VIA23"))||(!strcmp(t4, "VI23")))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA24"))||(!strcmp(t4, "VI24")))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA25"))||(!strcmp(t4, "VI25")))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA26"))||(!strcmp(t4, "VI26")))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA27"))||(!strcmp(t4, "VI27")))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA28"))||(!strcmp(t4, "VI28")))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA29"))||(!strcmp(t4, "VI29")))
               {
                     (void)addphvia(figure, CONT_VIA2, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA34"))||(!strcmp(t4, "VI34")))
               {
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA35"))||(!strcmp(t4, "VI35")))
               {
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA36"))||(!strcmp(t4, "VI36")))
               {
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA37"))||(!strcmp(t4, "VI37")))
               {
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA38"))||(!strcmp(t4, "VI38")))
               {
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA39"))||(!strcmp(t4, "VI39")))
               {
                     (void)addphvia(figure, CONT_VIA3, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA45"))||(!strcmp(t4, "VI45")))
               {
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA46"))||(!strcmp(t4, "VI46")))
               {
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA47"))||(!strcmp(t4, "VI47")))
               {
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA48"))||(!strcmp(t4, "VI48")))
               {
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA49"))||(!strcmp(t4, "VI49")))
               {
                     (void)addphvia(figure, CONT_VIA4, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA56"))||(!strcmp(t4, "VI56")))
               {
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA57"))||(!strcmp(t4, "VI57")))
               {
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA58"))||(!strcmp(t4, "VI58")))
               {
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA59"))||(!strcmp(t4, "VI59")))
               {
                     (void)addphvia(figure, CONT_VIA5, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA67"))||(!strcmp(t4, "VI67")))
               {
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA68"))||(!strcmp(t4, "VI68")))
               {
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                  }
               else if ((!strcmp(t4, "VIA69"))||(!strcmp(t4, "VI69")))
               {
                     (void)addphvia(figure, CONT_VIA6, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
               }
               else if ((!strcmp(t4, "VIA78"))||(!strcmp(t4, "VI78")))
               {
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
               }
               else if ((!strcmp(t4, "VIA79"))||(!strcmp(t4, "VI79")))
               {
                     (void)addphvia(figure, CONT_VIA7, l1, l2, l3, l4,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
               }
               else if ((!strcmp(t4, "VIA89"))||(!strcmp(t4, "VI89")))
                     (void)addphvia(figure, CONT_VIA8, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN1"))
                     (void)addphvia(figure, CONT_TURN1, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN2"))
                     (void)addphvia(figure, CONT_TURN2, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN3"))
                     (void)addphvia(figure, CONT_TURN3, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN4"))
                     (void)addphvia(figure, CONT_TURN4, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN5"))
                     (void)addphvia(figure, CONT_TURN5, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN6"))
                     (void)addphvia(figure, CONT_TURN6, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN7"))
                     (void)addphvia(figure, CONT_TURN7, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN8"))
                     (void)addphvia(figure, CONT_TURN8, l1, l2, l3, l4,NULL);
               else if (!strcmp(t4, "TURN9"))
                     (void)addphvia(figure, CONT_TURN9, l1, l2, l3, l4,NULL);
               else 
                  cp_error((int)VIA_ERROR, fname, i, 0L);
            }
            break;
         case 'E' : /* E */
            {
            point_list *point;
            phins_list *ins;
            void *sav;

            for (point = head_point; point; point = (point_list *)sav) {
               sav = (void *)point->NEXT;
               mbkfree((void *)point);
            }
            if (mode == 'P') {
               for (ins = figure->PHINS; ins; ins = (phins_list *)sav) {
                  sav = (void *)ins->NEXT;
                  mbkfree((void *)ins);
               }
               figure->PHINS = (phins_list *)NULL;
            }
            return 0;
            }
         case 'F':
            break;
   
         case 'I' :   /*   I name num num num name name name name */
            t1 = strtok(line + 2, " \n\t\"");
            t2 = strtok((char *)NULL, " \n\t\"");
            (void)strcpy(s, t2);
            l1 = stol(s);
            t3 = strtok((char *)NULL, " \n\t\"");
            (void)strcpy(s, t3);
            l2 = stol(s);
            t4 = strtok((char *)NULL, " \n\t\"");
            l3 = atol(t4);
            t5 = strtok((char *)NULL, " \n\t\"");
            t6 = strtok((char *)NULL, " \n\t\"");
            /* load what is referenced by .cp or .scp as .cp */
            if (*t6 == 'c' || *t6 == 's') { /* load instances for any mode */
            char symm;
               switch ((int)l3) {
                  case 0 :
                     symm = NOSYM;
                     break;
                  case 1 :
                     symm = ROT_P;
                     break;
                  case 2 :
                     symm = SYMXY;
                     break;
                  case 3 :
                     symm = ROT_M;
                     break;
                  case 4 :
                     symm = SYM_X;
                     break;
                  case 5 :
                     symm = SY_RM;
                     break;
                  case 6 :
                     symm = SYM_Y;
                     break;
                  case 7 :
                     symm = SY_RP;
                     break;
                  default :
                     symm = -1;
               }
               (void)addphins(figure, t5, t1, symm, l1, l2);
            }
            if (mode != 'C')
               if (*t6 == 'l' && !strncmp(t5, "ref_", 4))
                     (void)addphref(figure, t5, checkname(t1), l1, l2);
            if (mode != 'P') {
               if (*t6 == 'l') {
                  if (!strcmp(t5, "cbn")) {
                     (void)addphvia(figure, CONT_BODY_N, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cbnv")) {
                     (void)addphvia(figure, CONT_BODY_N, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cbp")) {
                     (void)addphvia(figure, CONT_BODY_P, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cbpv")) {
                     (void)addphvia(figure, CONT_BODY_P, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cdn")) {
                     (void)addphvia(figure, CONT_DIF_N, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cdnv")) {
                     (void)addphvia(figure, CONT_DIF_N, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cdp")) {
                     (void)addphvia(figure, CONT_DIF_P, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cdpv")) {
                     (void)addphvia(figure, CONT_DIF_P, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cpf")) {
                     (void)addphvia(figure, CONT_POLY, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cpfv")) {
                     (void)addphvia(figure, CONT_POLY, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cpf2")) {
                     (void)addphvia(figure, CONT_POLY2, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cpf2v")) {
                     (void)addphvia(figure, CONT_POLY2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cvia")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cvia2")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cxn")) {
                     (void)addphvia(figure, C_X_N, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "cxp")) {
                     (void)addphvia(figure, C_X_P, l1, l2,0,0,NULL);
                  /* Bull specific: quite bright, insn't it ? */
                  } else if (!strcmp(t5, "via12")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via13")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via14")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via15")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via16")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via17")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via18")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via19")) {
                     (void)addphvia(figure, CONT_VIA, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via23")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via24")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via25")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via26")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via27")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via28")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via29")) {
                     (void)addphvia(figure, CONT_VIA2, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via34")) {
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via35")) {
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via36")) {
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via37")) {
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via38")) {
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via39")) {
                     (void)addphvia(figure, CONT_VIA3, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via45")) {
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via46")) {
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via47")) {
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via48")) {
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via49")) {
                     (void)addphvia(figure, CONT_VIA4, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via56")) {
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via57")) {
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via58")) {
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via59")) {
                     (void)addphvia(figure, CONT_VIA5, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via67")) {
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via68")) {
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via69")) {
                     (void)addphvia(figure, CONT_VIA6, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via78")) {
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via79")) {
                     (void)addphvia(figure, CONT_VIA7, l1, l2,0,0,NULL);
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "via89")) {
                     (void)addphvia(figure, CONT_VIA8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn1")) {
                     (void)addphvia(figure, CONT_TURN1, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn2")) {
                     (void)addphvia(figure, CONT_TURN2, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn3")) {
                     (void)addphvia(figure, CONT_TURN3, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn4")) {
                     (void)addphvia(figure, CONT_TURN4, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn5")) {
                     (void)addphvia(figure, CONT_TURN5, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn6")) {
                     (void)addphvia(figure, CONT_TURN6, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn7")) {
                     (void)addphvia(figure, CONT_TURN7, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn8")) {
                     (void)addphvia(figure, CONT_TURN8, l1, l2,0,0,NULL);
                  } else if (!strcmp(t5, "turn9")) {
                     (void)addphvia(figure, CONT_TURN9, l1, l2,0,0,NULL);
                  }
               } 
            }
            break;
         case 'J' : /* J num num name name */
            t1 = strtok(line + 2, " \n\t\"");
            (void)strcpy(s, t1);
            l1 = stol(s);
            t2 = strtok((char *)NULL, " \n\t\"");
            (void)strcpy(s, t2);
            l2 = stol(s);
            addpoint(l1, l2);
            break;
         case 'P' : /* P num num name name */
            t1 = strtok(line + 2, " \n\t\"");
            (void)strcpy(s, t1);
            l1 = stol(s);
            t2 = strtok((char *)NULL, " \n\t\"");
            (void)strcpy(s, t2);
            l2 = stol(s);
            addpoint(l1, l2);
            break;
         case 'S' :
            t1 = strtok(line + 2, " \n\t\"");
            (void)strcpy(s, t1);
            l1 = stol(s);
            t2 = strtok((char *)NULL, " \n\t\"");
            t3 = strtok((char *)NULL, " \n\t\"");
            t4 = strtok((char *)NULL, " \n\t\"");
            t5 = strtok((char *)NULL, " \n\t\"");
            t6 = strtok((char *)NULL, " \n\t\"");
            t7 = strtok((char *)NULL, " \n\t\"");
            t8 = strtok((char *)NULL, " \n\t\"");
            t9 = strtok((char *)NULL, " \n\t\"");
            {
            char layer = codelayer(t3);
            if (mode == 'A'
                  || (mode == 'P' &&
                        (
                            layer == TALU1 || layer == TALU2 || layer == TALU3
                         || layer == TALU4 || layer == TALU5 || layer == TALU6
                         || layer == TALU7 || layer == TALU8 || layer == TALU9
                         || layer == CALU1 || layer == CALU2 || layer == CALU3
                         || layer == CALU4 || layer == CALU5 || layer == CALU6
                         || layer == CALU7 || layer == CALU8 || layer == CALU9
                         ))
                  || (mode == 'C' &&
                        (   layer != TALU1 && layer && TALU2 && layer != TALU3
                         && layer != TALU4 && layer != TALU5 && layer != TALU6
                         && layer != TALU7 && layer != TALU8 && layer != TALU9
                         && layer != CALU1 && layer != CALU2 && layer != CALU3
                         && layer != CALU4 && layer != CALU5 && layer != CALU6
                         && layer != CALU7 && layer != CALU8 && layer != CALU9
                        ))
               ) {
               switch (*t4) {
               case 'p' : /* S num name name P_REF num P_REF num */
                  *t4 = 'P';
               case 'P' :
                  switch (*t6) {
                  case 'p' :
                     *t6 = 'P';
                  case 'P' :
                     l2 = atol(t5);
                     l3 = atol(t7);
                     cppref(x1, y1, l2, i);
                     cppref(x2, y2, l3, i);
                     (void)vti_addphseg(figure, layer, l1, x1, y1,
                             x2, y2, sname);
                     break;
                  case 'c' : /* S num name name P_REF num C_REF num */
                     *t6 = 'C';
                  case 'C' :
                     l2 = atol(t5);
                     l3 = atol(t7);
                     cppref(x1, y1, l2, i);
                     cpcref(x2, y2, l3, i);
                     (void)vti_addphseg(figure, layer, l1,
                                         x1, y1, x2, y2, sname);
                     break;
                  case 'i' :/* S num name name P_REF num I_REF name num */
                     *t6 = 'I';
                  case 'I' :
                     l2 = atol(t5);
                     l3 = atol(t8);
                     cppref(x1, y1, l2, i);
                     cpiref(x2, y2, t7, l3, i);
                     (void)vti_addphseg(figure, layer, l1, x1, y1,
                             x2, y2, sname);
                     break;
   
                  default :
                     cp_error((int)REF_ERROR, fname, i, 0L);
                  }
                  break;
   
               case 'c' :   /* S num name name C_REF num P_REF num */
                  *t4 = 'C';
               case 'C' :
                  switch (*t6) {
                  case 'p' :
                     *t6 = 'P';
                  case 'P' :
                     l2 = atol(t5);
                     l3 = atol(t7);
                     cpcref(x1, y1, l2, i);
                     cppref(x2, y2, l3, i);
                     (void)vti_addphseg(figure, layer, (long)l1,
                                      x1, y1, x2, y2, sname);
                     break;
                  case 'c' : /* S num name name C_REF num C_REF num */
                     *t6 = 'C';
                  case 'C' :
                     l2 = atol(t5);
                     l3 = atol(t7);
                     cpcref(x1, y1, l2, i);
                     cpcref(x2, y2, l3, i);
                     (void)vti_addphseg(figure, layer, l1,
                                      x1, y1, x2, y2, sname);
                     break;
                  case 'i' : /* S num name name C_REF num I_REF name num */
                     *t6 = 'I';
                  case 'I' :
                     l2 = atol(t5);
                     l3 = atol(t8);
                     cpcref(x1, y1, l2, i);
                     cpiref(x2, y2, t7, l3, i);
                     (void)vti_addphseg(figure, layer, l1, x1, y1,
                                      x2, y2, sname);
                     break;
                  default :
                     cp_error((int)REF_ERROR, fname, i, 0L);
                  }
                  break;
               case 'i' : /* S num name name I_REF name num P_REF num */
                  *t4 = 'I';
               case 'I' :
                  switch (*t7) {
                  case 'p' :
                     *t7 = 'P';
                  case 'P' :
                     l2 = atol(t6);
                     l3 = atol(t8);
                     cppref(x2, y2, l3, i);
                     cpiref(x1, y1, t5, l2, i);
                     (void)vti_addphseg(figure, layer, l1, x1, y1,
                                      x2, y2, sname);
                     break;
                  case 'c' : /* S num name name I_REF name num C_REF num */
                     *t7 = 'C';
                  case 'C' :
                     l2 = atol(t6);
                     l3 = atol(t8);
                     cpcref(x2, y2, l3, i);
                     cpiref(x1, y1, t5, l2, i);
                     (void)vti_addphseg(figure, layer, l1, x1, y1,
                                      x2, y2, sname);
                     break;
                  case 'i' : /* S num name name I_REF name num I_REF name num */
                     *t7 = 'I';
                  case 'I' :
                     l2 = atol(t6);
                     l3 = atol(t9);
                     cpiref(x1, y1, t5, l2, i);
                     cpiref(x2, y2, t8, l3, i);
                     (void)vti_addphseg(figure, layer, l1, x1, y1,
                                    x2, y2, sname);
                     break;
                  default:
                     cp_error((int)REF_ERROR, fname, i, 0L);
                  }
                  break;
               default :
                  cp_error((int)REF_ERROR, fname, i, 0L);
               }
            }
            }
            break;
         case 'V' : /* V num name num */
            break;
         case 'W' :
            t1 = strtok(line + 2, " \n\t\"");
            t2 = strtok((char *)NULL, " \n\t\"");
            t3 = strtok((char *)NULL, " \n\t\"");
            t4 = strtok((char *)NULL, " \n\t\"");
            t5 = strtok((char *)NULL, " \n\t\"");
            if (t5 != (char *)NULL) /* W num num num num name */
               sname = namealloc(checkname(t5));
            else /* W num num num num */
               sname = (char *)NULL;
            break;
         default :
            cp_error((int)SYNTAX_ERROR, fname, i, 0L);
      }
   }
}

/*******************************************************************************
* This is the only function to be accesible from outside                       *
* Effective call the the cp parser                                             *
*******************************************************************************/
void vtiloadphfig(ptfig, figname, mode)
phfig_list *ptfig;
char *figname;
char mode;
{
FILE *in;

   /* opening file */
   if ((in = mbkfopen(figname, IN_PH, READ_TEXT)) == (FILE *)NULL)
      cp_error((int)OPEN_ERROR, figname, 0, 0L);

   if (TRACE_MODE == 'Y')
      (void)printf("\n--- mbk ---  parsing file %s.%s in mode %c\n",
                 figname, IN_PH, mode);

   /* parsing */
   if (parse(ptfig, mode, in, figname))
      cp_error((int)PARSE_ERROR, figname, 0, 0L);

   if (fclose(in))
      cp_error((int)CLOSE_ERROR, figname, 0, 0L);

   strcpy(PARSER_INFO, "@(#)vti symbolic layout view parser version 3.06");
}
