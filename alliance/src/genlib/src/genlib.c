/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
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

/*******************************************************************************
* mbk     : user functions known as genlib                                     *
*                                                                              *
* author  : Frederic Petrot                                                    *
* version : 3.21                                                               *
* date    : 03/09/93                                                           *
* version : 3.26
* date    : 10/06/97 : it's been a while since I got there, ...
* version : 3.28
* date    : 08/06/98 : One year ago, things are going better and better, ...
* version : 3.30
* date    : 02/02/02 : Survived the turn of the century, unexpectedly!         
*******************************************************************************/
/* 3.26 is a major, and probably the last, modification that I'm going
 * to introduce into genlib.
 * Its goal is to provide a consistent interface with genview
 * regarding the way to pass the created objects.
 * Up to version 3.21, this was custom and done on a function per
 * function basis, as I was also mastering genview.
 * From now on, some other stuffs are build on top of genlib, so I 
 * must provide a consistent interface for all functions.

   It is a follows: each function that returns objects returns a
   ptype list of possible types MBK_INSTANCE, MBK_SEGMENT, 
   MBK_CONNECTOR, MBK_REFERENCE and MBK_VIA.
   The current figure and the current instances are still pointed to
   with the global variables.
   If a ptype list with such a type exists, then its DATA pointer is
   not NULL, and points to a chain_list whose NEXT pointer points to
   the next element of the type, and whose DATA pointer points to an
   mbk object of the given type.
   The problem is that the program that uses these functions should
   deallocate the various lists.

   Note: I haven't put genlib under RCS in the files themselves, as I
   do so few modifications that I really don't care.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include <mph.h>
#include <mpu.h>
#include "mbkgen.h"
#define __GENLIB__
#include "mgn.h"
//static char rcsid[] = "$Id: genlib.c,v 1.14 2012/05/14 14:20:13 alliance Exp $";

/*******************************************************************************
* global variables used in genlib                                              *
*******************************************************************************/
phins_list *WORK_PHINS;                        /* physical work figure        */
phfig_list *WORK_PHFIG;                        /* physical work instance      */
lofig_list *WORK_LOFIG;                        /* logical work instance       */

/* Name of the special unconnected signal that skips connections */
#define UNCONNECTED "_/_"

static chain_list *phlist;
static chain_list *lolist;

static char *checkname();
static void toolong();
static int hassep();
int is_bus(char *name);
int bus_decod();

/*******************************************************************************
* physical user functions                                                      *
*******************************************************************************/

static chain_list *g_addphvia( phfig_list *p, char type, long x, long y, long dx, long dy, char *name)
{
chain_list *c = NULL;

   if (name)
      name = checkname(name); 

   switch(type) {
      case CONT_VIA13:
         c = addchain(c,    addphvia(p, CONT_VIA , x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         break;
      case CONT_VIA14:
         c = addchain(c,    addphvia(p, CONT_VIA , x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         break;
      case CONT_VIA15:
         c = addchain(c,    addphvia(p, CONT_VIA , x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         break;
      case CONT_VIA16:
         c = addchain(c,    addphvia(p, CONT_VIA , x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         break;
      case CONT_VIA17:
         c = addchain(c,    addphvia(p, CONT_VIA , x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         break;
      case CONT_VIA18:
         c = addchain(c,    addphvia(p, CONT_VIA , x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         break;
      case CONT_VIA19:
         c = addchain(c,    addphvia(p, CONT_VIA , x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA8, x, y, dx, dy, name));
         break;
      case CONT_VIA24:
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         break;
      case CONT_VIA25:
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         break;
      case CONT_VIA26:
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         break;
      case CONT_VIA27:
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         break;
      case CONT_VIA28:
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         break;
      case CONT_VIA29:
         c = addchain(c,    addphvia(p, CONT_VIA2, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA8, x, y, dx, dy, name));
         break;
      case CONT_VIA35:
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         break;
      case CONT_VIA36:
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         break;
      case CONT_VIA37:
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         break;
      case CONT_VIA38:
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         break;
      case CONT_VIA39:
         c = addchain(c,    addphvia(p, CONT_VIA3, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA8, x, y, dx, dy, name));
         break;
      case CONT_VIA46:
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         break;
      case CONT_VIA47:
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         break;
      case CONT_VIA48:
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         break;
      case CONT_VIA49:
         c = addchain(c,    addphvia(p, CONT_VIA4, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA8, x, y, dx, dy, name));
         break;
      case CONT_VIA57:
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         break;
      case CONT_VIA58:
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         break;
      case CONT_VIA59:
         c = addchain(c,    addphvia(p, CONT_VIA5, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA8, x, y, dx, dy, name));
         break;
      case CONT_VIA68:
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         break;
      case CONT_VIA69:
         c = addchain(c,    addphvia(p, CONT_VIA6, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA8, x, y, dx, dy, name));
         break;
      case CONT_VIA79:
         c = addchain(c,    addphvia(p, CONT_VIA7, x, y, dx, dy, name));
         c = addchain(c,    addphvia(p, CONT_VIA8, x, y, dx, dy, name));
         break;
      default:
         c = addchain(NULL, addphvia(p, type, x, y, dx, dy, name));
   }
   return c;
}

/*******************************************************************************
* function DEF_PHFIG                                                           *
*******************************************************************************/
void genDEF_PHFIG( char  *name )
{
phfig_list *ptfig;
char *s;

   if (!CATA_LIB || !WORK_LIB)
      mbkenv();
   s = namealloc(name); /* Must be performed after mbkenv() */
   for (ptfig = HEAD_PHFIG; ptfig; ptfig = ptfig->NEXT)
      if (ptfig->NAME == s)
         break;
   if (ptfig == NULL)
      WORK_PHFIG = addphfig(s);
   else
      WORK_PHFIG = ptfig;
   phlist = addchain(phlist, WORK_PHFIG);
}

/*******************************************************************************
* function LOAD_PHFIG                                                          *
*******************************************************************************/
void genLOAD_PHFIG(name)
char  *name;
{
   if (!CATA_LIB || !WORK_LIB)
      mbkenv();
   WORK_PHFIG = getphfig(name, 'A');
   phlist = addchain(phlist, WORK_PHFIG);
}

/*******************************************************************************
* function SAVE_PHFIG                                                          *
*******************************************************************************/
void genSAVE_PHFIG()
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SAVE_PHFIG impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   savephfig(WORK_PHFIG);
   phlist = delchain(phlist, phlist);
   if (phlist)
      WORK_PHFIG = (phfig_list *)phlist->DATA;
   else
      WORK_PHFIG = NULL;
}

/*******************************************************************************
* function DEF_AB                                                              *
*******************************************************************************/
void genDEF_AB(dx1, dy1, dx2, dy2)
long dx1, dy1, dx2, dy2;
{
phfig_list *ptfig;
phins_list *ptins;
/*ptype_list *ptype;*/
long x1, y1, x2, y2;
long insx1, insx2, insy1, insy2;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("DEF_AB impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_PHFIG->PHINS == NULL)
      x1 = x2 = y1 = y2 = 0;
   else {
      x2 = x1 = WORK_PHFIG->PHINS->XINS;
      y2 = y1 = WORK_PHFIG->PHINS->YINS;
   }
   for (ptins = WORK_PHFIG->PHINS; ptins; ptins = ptins->NEXT) {
#if 0
      ptype = getptype(ptins->USER, (long)PLACEABOX);
   /* This has been commented out as it should never ever occur, since
    * all instances are placed using genlib. */
      if (ptype == NULL) {
#endif
         ptfig = getphfig(ptins->FIGNAME, 'P');
         if (ptins->TRANSF == ROT_P || ptins->TRANSF == ROT_M
             || ptins->TRANSF == SY_RM || ptins->TRANSF == SY_RP) {
            insx1 = ptins->XINS;
            insy1 = ptins->YINS;
            insx2 = ptins->XINS + ptfig->YAB2 - ptfig->YAB1;
            insy2 = ptins->YINS + ptfig->XAB2 - ptfig->XAB1;
         } else {
            insx1 = ptins->XINS;
            insy1 = ptins->YINS;
            insx2 = ptins->XINS + ptfig->XAB2 - ptfig->XAB1;
            insy2 = ptins->YINS + ptfig->YAB2 - ptfig->YAB1;
         }
#if 0
      } else {
         ptnum = (num_list *)(ptype->DATA);
         /* No need to check for rotation here, as the PLACEABOX field
            contains the expected value, if I ain't mistaking... */
         insx1 = ptins->XINS;
         insy1 = ptins->YINS;
         insx2 = ptnum->DATA;
         insy2 = ptnum->NEXT->DATA;
      }
#endif

      if (x1 > insx1)
         x1 = insx1;
      if (y1 > insy1)
         y1 = insy1;
      if (x2 < insx2)
         x2 = insx2;
      if (y2 < insy2)
         y2 = insy2;
   }
   defab(WORK_PHFIG, x1 + dx1, y1 + dy1, x2 + dx2, y2 + dy2);
}

/*******************************************************************************
* function DEF_PHINS                                                           *
*******************************************************************************/
void genDEF_PHINS(name)
char *name;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("DEF_PHINS impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   WORK_PHINS = getphins(WORK_PHFIG, name);
}

/*******************************************************************************
* function PHCON                                                               *
*******************************************************************************/
ptype_list *genPHCON(layer, width, name, orient, x, y)
char orient;
char *name;
long x, y, width;
char layer;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PHCON impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   return addptype(NULL, MBK_CONNECTOR,
             addphcon(WORK_PHFIG, orient, checkname(name), x, y, layer, width));
}

/*******************************************************************************
* function COPY_UP_CON                                                         *
*******************************************************************************/
ptype_list *genCOPY_UP_CON(index, locname, insname, newname)
long index;
char *insname;
char *locname;
char *newname;
{
phfig_list *ptnewfig;
phins_list *ptins;
phcon_list *ptcon, *con = NULL; /* To make -Wall happy */
phseg_list *seg = NULL;
long xcon, ycon;
long width;
char layer;
char orient;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("COPY_UP_CON impossible :", stderr);
      (void)fputs(" missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }

   ptins = getphins(WORK_PHFIG, insname);
   ptnewfig = getphfig(ptins->FIGNAME, 'P');

   /*  looking for a connector(locname, index) */
   locname = namealloc(checkname(locname));
   for (ptcon = ptnewfig->PHCON; ptcon; ptcon = ptcon->NEXT)
      if (locname == ptcon->NAME && index == ptcon->INDEX)
         break; /* the only one is found here, since(name, index) is unique */

   if (ptcon != NULL) {
      xyflat(&xcon, &ycon, ptcon->XCON, ptcon->YCON, ptins->XINS,
           ptins->YINS, ptnewfig->XAB1, ptnewfig->YAB1, ptnewfig->XAB2,
           ptnewfig->YAB2, ptins->TRANSF);

      width = ptcon->WIDTH;
      layer = ptcon->LAYER;
      orient = instanceface(ptcon->ORIENT, ptins->TRANSF);

      /*  create connector and segment, now UNIQUE  */
      switch (orient) {
      case WEST :
         if (newname)
            con = addphcon(WORK_PHFIG, WEST, checkname(newname),
                        WORK_PHFIG->XAB1, ycon, layer, width);
         if (xcon > WORK_PHFIG->XAB1)
            seg = addphseg(WORK_PHFIG, layer, width,
                        WORK_PHFIG->XAB1,ycon, xcon, ycon, (char *)NULL);
         break;
      case EAST :
         if (newname)
            con = addphcon(WORK_PHFIG, EAST, checkname(newname),
                     WORK_PHFIG->XAB2, ycon, layer, width);
         if (xcon < WORK_PHFIG->XAB2)
            seg = addphseg(WORK_PHFIG, layer, width,
                        xcon, ycon, WORK_PHFIG->XAB2, ycon, (char *)NULL);
         break;
      case SOUTH :
         if (newname)
            con = addphcon(WORK_PHFIG, SOUTH, checkname(newname), xcon,
                     WORK_PHFIG->YAB1, layer, width);
         if (ycon > WORK_PHFIG->YAB1)
            seg = addphseg(WORK_PHFIG, layer, width,
                        xcon, WORK_PHFIG->YAB1, xcon, ycon, (char *)NULL);
         break;
      case NORTH :
         if (newname)
            con = addphcon(WORK_PHFIG, NORTH, checkname(newname), xcon,
                     WORK_PHFIG->YAB2, layer, width);
         if (ycon < WORK_PHFIG->YAB2)
            seg = addphseg(WORK_PHFIG, layer, width,
                        xcon, ycon, xcon, WORK_PHFIG->YAB2, (char *)NULL);
         break;
      }
      return addptype(
                addptype(NULL, MBK_SEGMENT, addchain(NULL, seg)),
                MBK_CONNECTOR,
                addchain(NULL, con));
   }

   (void)fflush(stdout);
   (void)fputs("*** genlib error ***\n", stderr);
   (void)fputs("COPY_UP_CON impossible : ", stderr);
   (void)fprintf(stderr, "connector %s index %ld of instance %s, figure %s,",
                  locname, index, insname, WORK_PHFIG->NAME);
   (void)fputs(" doesn't exist\n", stderr);
   EXIT(1);
   return NULL; /* Never reached */
}

/*******************************************************************************
* function COPY_UP_CON_FACE                                                    *
*******************************************************************************/
ptype_list *genCOPY_UP_CON_FACE(orient, locname, insname, newname)
char orient;
char *insname;
char *locname;
char *newname;
{
phfig_list *ptnewfig;
phins_list *ptins;
phcon_list *ptcon, *ptscan;
chain_list *cons = NULL, *segs = NULL;
long xcon, ycon;
long width;
char layer;
int found = 0;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("COPY_UP_CON_FACE impossible missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (orient != NORTH && orient != SOUTH && orient != EAST && orient != WEST) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "illegal COPY_UP_CON_FACE : orientation is %ld\n",
                         (long)orient);
      EXIT(1);
   }

   ptins = getphins(WORK_PHFIG, insname);
   ptnewfig = getphfig(ptins->FIGNAME, 'P');

   /*  recherche et remontee de tous les connecteurs (locname)  */
   locname = namealloc(checkname(locname));
   newname = namealloc(checkname(newname));
   for (ptscan = ptnewfig->PHCON; ptscan; ptscan = ptscan->NEXT) {
      ptcon = NULL;
      switch (orient) {
      case WEST :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_Y) {
            if (locname == ptscan->NAME && ptscan->ORIENT == WEST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_X) {
            if (locname == ptscan->NAME && ptscan->ORIENT == EAST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RP) {
            if (locname == ptscan->NAME && ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RM) {
            if (locname == ptscan->NAME && ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         }
         break;
      case EAST :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_Y) {
            if (locname == ptscan->NAME && ptscan->ORIENT == EAST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_X) {
            if (locname == ptscan->NAME && ptscan->ORIENT == WEST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RP) {
            if (locname == ptscan->NAME && ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RM) {
            if (locname == ptscan->NAME && ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         }
         break;
      case NORTH :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_X) {
            if (locname == ptscan->NAME && ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_Y) {
            if (locname == ptscan->NAME && ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RM) {
            if (locname == ptscan->NAME && ptscan->ORIENT == EAST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RP) {
            if (locname == ptscan->NAME && ptscan->ORIENT == WEST)
               ptcon = ptscan;
         }
         break;
      case SOUTH :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_X) {
            if (locname == ptscan->NAME && ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_Y) {
            if (locname == ptscan->NAME && ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RM) {
            if (locname == ptscan->NAME && ptscan->ORIENT == WEST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RP) {
            if (locname == ptscan->NAME && ptscan->ORIENT == EAST)
               ptcon = ptscan;
         }
         break;
      }
      if (ptcon != NULL) {
         found++;
         xyflat(&xcon, &ycon, ptcon->XCON, ptcon->YCON,
                  ptins->XINS, ptins->YINS, ptnewfig->XAB1, ptnewfig->YAB1,
                  ptnewfig->XAB2, ptnewfig->YAB2, ptins->TRANSF);
         width = ptcon->WIDTH;
         layer = ptcon->LAYER;

         /*  create connector and segment    */
         switch (orient) {
         case WEST :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG, WEST, newname,
                WORK_PHFIG->XAB1, ycon, layer, width));
            if (xcon > WORK_PHFIG->XAB1)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                   width, WORK_PHFIG->XAB1, ycon, xcon, ycon, (char *)NULL));
            break;
         case EAST :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG, EAST, newname,
                WORK_PHFIG->XAB2, ycon, layer, width));
            if (xcon < WORK_PHFIG->XAB2)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                    width, xcon, ycon, WORK_PHFIG->XAB2, ycon, (char *)NULL));
            break;
         case SOUTH :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG,
                                        SOUTH, newname,
                 xcon, WORK_PHFIG->YAB1, layer, width));
            if (ycon > WORK_PHFIG->YAB1)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                    width, xcon, WORK_PHFIG->YAB1, xcon, ycon, (char *)NULL));
            break;
         case NORTH :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG, NORTH, newname,
                 xcon, WORK_PHFIG->YAB2, layer, width));
            if (ycon < WORK_PHFIG->YAB2)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                    width, xcon, ycon, xcon, WORK_PHFIG->YAB2, (char *)NULL));
            break;
         }
      }
   }

   if (!found) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("COPY_UP_CON_FACE impossible :\n", stderr);
      (void)fprintf(stderr, "connector %s of instance %s", locname, insname);
      (void)fprintf(stderr, ", figure %s, doesn't exist\n", WORK_PHFIG->NAME);
      EXIT(1);
   }
   return addptype(addptype(NULL, MBK_SEGMENT, segs), MBK_CONNECTOR, cons);
}

/*******************************************************************************
* function COPY_UP_ALL_CON                                                     *
*******************************************************************************/
ptype_list *genCOPY_UP_ALL_CON(orient, insname, cheminom)
char orient;
char *insname;
char cheminom;
{
phfig_list *ptnewfig;
phins_list *ptins;
phcon_list *ptcon, *ptscan;
chain_list *cons = NULL, *segs = NULL;
long xcon, ycon, width;
char layer;
int found = 0;
char *newname;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("COPY_UP_ALL_CON impossible missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (orient != NORTH && orient != SOUTH && orient != EAST && orient != WEST) {
      (void)fflush(stdout);
      (void)fputs("\n*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "illegal COPY_UP_ALL_CON : orientation is %ld",
                     (long)orient);
      (void)fprintf(stderr, "on instance %s of figure %s\n",
                     insname, WORK_PHFIG->NAME);
      EXIT(1);
   }

   ptins = getphins(WORK_PHFIG, insname);
   ptnewfig = getphfig(ptins->FIGNAME, 'P');

   /*  recherche et remontee de tous les connecteurs */
   for (ptscan = ptnewfig->PHCON; ptscan; ptscan = ptscan->NEXT) {
      ptcon = NULL;
      switch (orient) {
      case WEST :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_Y) {
            if (ptscan->ORIENT == WEST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_X) {
            if (ptscan->ORIENT == EAST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RP) {
            if (ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RM) {
            if (ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         }
         break;
      case EAST :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_Y) {
            if (ptscan->ORIENT == EAST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_X) {
            if (ptscan->ORIENT == WEST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RP) {
            if (ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RM) {
            if (ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         }
         break;
      case NORTH :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_X) {
            if (ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_Y) {
            if (ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RM) {
            if (ptscan->ORIENT == EAST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RP) {
            if (ptscan->ORIENT == WEST)
               ptcon = ptscan;
         }
         break;
      case SOUTH :
         if (ptins->TRANSF == NOSYM || ptins->TRANSF == SYM_X) {
            if (ptscan->ORIENT == SOUTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == SYMXY || ptins->TRANSF == SYM_Y) {
            if (ptscan->ORIENT == NORTH)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_P || ptins->TRANSF == SY_RM) {
            if (ptscan->ORIENT == WEST)
               ptcon = ptscan;
         } else if (ptins->TRANSF == ROT_M || ptins->TRANSF == SY_RP) {
            if (ptscan->ORIENT == EAST)
               ptcon = ptscan;
         }
         break;
      }
      if (ptcon != NULL) {
         found++;
         xyflat(&xcon, &ycon, ptcon->XCON, ptcon->YCON,
             ptins->XINS, ptins->YINS, ptnewfig->XAB1, ptnewfig->YAB1,
             ptnewfig->XAB2, ptnewfig->YAB2, ptins->TRANSF);
         width = ptcon->WIDTH;
         layer = ptcon->LAYER;
         newname = cheminom == 'Y' ? concatname(insname, ptcon->NAME)
                                    : ptcon->NAME;

         /*  create connector and segment    */
         switch (orient) {
         case WEST :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG, WEST, newname,
                              WORK_PHFIG->XAB1, ycon, layer, width));
            if (xcon > WORK_PHFIG->XAB1)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                                 width, WORK_PHFIG->XAB1, ycon,
                                 xcon, ycon, (char *)NULL));
            break;
         case EAST :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG, EAST, newname,
                              WORK_PHFIG->XAB2, ycon, layer, width));
            if (xcon < WORK_PHFIG->XAB2)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                                 width, xcon, ycon, WORK_PHFIG->XAB2,
                                 ycon, (char *)NULL));
            break;
         case SOUTH :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG, SOUTH, newname,
                              xcon, WORK_PHFIG->YAB1, layer, width));
            if (ycon > WORK_PHFIG->YAB1)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                                 width, xcon, WORK_PHFIG->YAB1,
                                 xcon, ycon, (char *)NULL));
            break;
         case NORTH :
            cons = addchain(cons, (void *)addphcon(WORK_PHFIG, NORTH, newname,
                              xcon, WORK_PHFIG->YAB2, layer, width));
            if (ycon < WORK_PHFIG->YAB2)
               segs = addchain(segs, (void *)addphseg(WORK_PHFIG, layer,
                                 width, xcon, ycon, xcon, WORK_PHFIG->YAB2,
                                 (char *)NULL));
            break;
         }
      }
   }

   if (!found) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal COPY_UP_ALL_CON : ", stderr);
      (void)fprintf(stderr, "instance %s, figure %s, has no connectors\n",
                     insname, WORK_PHFIG->NAME);
      EXIT(1);
   }

   return addptype(addptype(NULL, MBK_SEGMENT, segs), MBK_CONNECTOR, cons);
}

/*******************************************************************************
* function GET_CON_X                                                           *
*******************************************************************************/
long genGET_CON_X(insname, conname, index)
char *insname, *conname;
long index;
{
phfig_list *ptfig; /* pointer on model of instance called insname */
phins_list *ptins; /* pointer on instance called insname */
phcon_list *ptcon; /* pointer on the conname called con inside ptins */
long x_con, y_con; /* return values, y_con here for beauty */

   ptins = getphins(WORK_PHFIG, insname);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   ptcon = getphcon(ptfig, checkname(conname), index);

   xyflat(&x_con, &y_con, ptcon->XCON, ptcon->YCON, ptins->XINS, ptins->YINS,
        ptfig->XAB1, ptfig->YAB1,  ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   return x_con;
}

/*******************************************************************************
* function GET_CON_Y                                                           *
*******************************************************************************/
long genGET_CON_Y(insname, conname, index)
char *insname, *conname;
long index;
{
phfig_list *ptfig; /* pointer on model of instance called insname */
phins_list *ptins; /* pointer on instance called insname */
phcon_list *ptcon; /* pointer on the conname called con inside ptins */
long x_con, y_con; /* return values, x_con here for beauty */

   ptins = getphins(WORK_PHFIG, insname);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   ptcon = getphcon(ptfig, checkname(conname), index);

   xyflat(&x_con, &y_con, ptcon->XCON, ptcon->YCON, ptins->XINS, ptins->YINS,
        ptfig->XAB1, ptfig->YAB1,  ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   return y_con;
}

/*******************************************************************************
* function GET_CON_LAYER                                                       *
*******************************************************************************/
char genGET_CON_LAYER(insname, conname, index)
char *insname, *conname;
long index;
{
phfig_list *ptfig; /* pointer on model of instance called insname */
phins_list *ptins; /* pointer on instance called insname */
phcon_list *ptcon; /* pointer on the conname called con inside ptins */

   ptins = getphins(WORK_PHFIG, insname);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   ptcon = getphcon(ptfig, checkname(conname), index);

   return ptcon->LAYER;
}

/*******************************************************************************
* function PHSEG                                                               *
*******************************************************************************/
ptype_list *genPHSEG(layer, width, name, x1, y1, x2, y2)
char *name;
long x1, y1, x2, y2, width;
char layer;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PHSEG impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   return addptype(NULL, MBK_SEGMENT, 
             addchain(NULL,
                addphseg(WORK_PHFIG, layer, width, x1, y1, x2, y2,
                checkname(name))));
}

/*******************************************************************************
* function COPY_UP_SEG                                                         *
*******************************************************************************/
ptype_list *genCOPY_UP_SEG(locname, insname, newname)
char *insname;
char *locname;
char *newname;
{
phfig_list *ptnewfig;
phins_list *ptins;
phseg_list *ptseg;
long x1, x2, y1, y2;
int  found = 0;
chain_list *c = NULL;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("COPY_UP_SEG impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   ptins = getphins(WORK_PHFIG, insname);
   ptnewfig = getphfig(ptins->FIGNAME, 'A');
   locname = namealloc(checkname(locname));

   for (ptseg = ptnewfig->PHSEG; ptseg; ptseg = ptseg->NEXT) {
      if (ptseg->NAME == locname) {
         /*  create new segment  */
         xyflat(&x1, &y1, ptseg->X1, ptseg->Y1, ptins->XINS, ptins->YINS,
                 ptnewfig->XAB1, ptnewfig->YAB1, ptnewfig->XAB2, ptnewfig->YAB2,
                 ptins->TRANSF);
         xyflat(&x2, &y2, ptseg->X2, ptseg->Y2, ptins->XINS, ptins->YINS,
                 ptnewfig->XAB1, ptnewfig->YAB1, ptnewfig->XAB2, ptnewfig->YAB2,
                 ptins->TRANSF);
         c = addchain(c, addphseg(WORK_PHFIG, ptseg->LAYER, ptseg->WIDTH,
                                  x1, y1, x2, y2, checkname(newname)));
                               
         found = 1;
      }
   }
   if (!found) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nCOPY_UP_SEG impossible :", stderr);
      (void)fprintf(stderr, " segment %s doesn't exist in instance %s\n",
                     locname, insname);
      EXIT(1);
   }

   return addptype(NULL, MBK_SEGMENT, c);
}

/*******************************************************************************
* function  THRU_H                                                             *
*******************************************************************************/
ptype_list *genTHRU_H(layer, width, y)
char  layer;
long  width, y;
{
long  x1, x2;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_H impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   x1 = WORK_PHFIG->XAB1;
   x2 = WORK_PHFIG->XAB2;
   if (x1 == 0 && WORK_PHFIG->YAB1 == 0 && x2 == 0 && WORK_PHFIG->YAB2 == 0) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_H impossible : missing DEF_AB\n", stderr);
      EXIT(1);
   }
   return addptype(NULL, MBK_SEGMENT,
             addchain(NULL, 
                addphseg(WORK_PHFIG, layer, width, x1, y, x2, y, NULL)));
}

/*******************************************************************************
* function  THRU_V                                                             *
*******************************************************************************/
ptype_list *genTHRU_V(layer, width, x)
char  layer;
long  width, x;
{
long  y1, y2;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_V impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   y1 = WORK_PHFIG->YAB1;
   y2 = WORK_PHFIG->YAB2;
   if (y1 == 0 && WORK_PHFIG->XAB1 == 0 && y2 == 0 && WORK_PHFIG->XAB2 == 0) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_V impossible : missing DEF_AB\n", stderr);
      EXIT(1);
   }
   return addptype(NULL, MBK_SEGMENT, 
             addchain(NULL,
                addphseg(WORK_PHFIG, layer, width, x, y1, x, y2, NULL)));
}

/*******************************************************************************
* function  THRU_CON_H                                                         *
*******************************************************************************/
ptype_list *genTHRU_CON_H(layer, width , name, y)
char layer;
char *name;
long width, y;
{
long x1, x2;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_CON_H impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   x1 = WORK_PHFIG->XAB1;
   x2 = WORK_PHFIG->XAB2;
   if (x1 == 0 && WORK_PHFIG->YAB1 == 0 && x2 == 0 && WORK_PHFIG->YAB2 == 0) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_CON_H impossible : missing DEF_AB\n", stderr);
      EXIT(1);
   }
   return addptype(
             addptype(NULL, MBK_CONNECTOR,
                addchain(
                   addchain(NULL,
                      addphcon(WORK_PHFIG, WEST, checkname(name),
                               x1, y, layer, width)),
                   addphcon(WORK_PHFIG, EAST, checkname(name),
                            x2, y, layer, width))),
                MBK_SEGMENT, 
                addchain(NULL,
                   addphseg(WORK_PHFIG, layer, width,
                            x1, y, x2, y, checkname(name))));
}

/*******************************************************************************
* function  THRU_CON_V                                                         *
*******************************************************************************/
ptype_list *genTHRU_CON_V(layer, width, name, x)
char layer;
char *name;
long width, x;
{
long y1, y2;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_CON_V impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   y1 = WORK_PHFIG->YAB1;
   y2 = WORK_PHFIG->YAB2;
   if (y1 == 0 && WORK_PHFIG->XAB1 == 0 && y2 == 0 && WORK_PHFIG->XAB2 == 0) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("THRU_CON_V impossible : missing DEF_AB\n", stderr);
      EXIT(1);
   }
   return addptype(
             addptype(NULL, MBK_CONNECTOR,
                addchain(
                   addchain(NULL,
                      addphcon(WORK_PHFIG, SOUTH, checkname(name),
                               x, y1, layer, width)),
                   addphcon(WORK_PHFIG, NORTH, checkname(name),
                            x, y2, layer, width))),
                MBK_SEGMENT, 
                addchain(NULL,
                   addphseg(WORK_PHFIG, layer, width,
                            x, y1, x, y2, checkname(name))));
}

/*******************************************************************************
* function WIRE1                                                               *
*******************************************************************************/
ptype_list *genWIRE1(layer, width, ins1, con1, index1, ins2, con2, index2)
char layer;
long width;
char *ins1, *ins2, *con1, *con2;
long index1, index2;
{
long xcon1,ycon1, xcon2, ycon2; /* connexion coordinates */
phcon_list *ptcon; /* scanning cons since getphcon exits if not present */
phref_list *ptref; /* scanning refs since getphref exits if not present */
phins_list *ptins;
phfig_list *ptfig;

   con1 = namealloc(checkname(con1));
   ptins = getphins(WORK_PHFIG, ins1);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con1 && ptcon->INDEX == index1) {
         xcon1 = ptcon->XCON;
         ycon1 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con1) {
         xcon1 = ptref->XREF;
         ycon1 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("WIRE1 same name for reference and", stderr);
      (void)fprintf(stderr, " connector '%s' in figure '%s'\n",
                    ptcon->NAME, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nWIRE1 reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                    con1, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&xcon1, &ycon1, xcon1, ycon1, ptins->XINS, ptins->YINS,
       ptfig->XAB1, ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   con2 = namealloc(checkname(con2));
   ptins = getphins(WORK_PHFIG, ins2);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con2 && ptcon->INDEX == index2) {
         xcon2 = ptcon->XCON;
         ycon2 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con2) {
         xcon2 = ptref->XREF;
         ycon2 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("WIRE1 same name for reference and", stderr);
      (void)fprintf(stderr, " connector '%s' in figure '%s'\n",
                    ptcon->NAME, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nWIRE1 reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                    con2, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&xcon2, &ycon2, xcon2, ycon2, ptins->XINS, ptins->YINS,
       ptfig->XAB1, ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);

   return addptype(NULL, MBK_SEGMENT,
             addchain(NULL,
                addphseg(WORK_PHFIG, layer,  width,
                         xcon1, ycon1, xcon2, ycon2, NULL)));
}

/*******************************************************************************
* function WIRE2                                                               *
*******************************************************************************/
ptype_list *genWIRE2(layer, width, ins1, con1, index1, ins2, con2, index2,
 x, y)
char layer;
long width, x, y;
char *ins1, *ins2, *con1, *con2;
long index1, index2;
{
long xcon1, ycon1, xcon2, ycon2; /* connexion coordinates */
phcon_list *ptcon; /* scanning cons since getphcon exits if not present */
phref_list *ptref; /* scanning refs since getphref exits if not present */
phins_list *ptins;
phfig_list *ptfig;

   con1 = namealloc(checkname(con1));
   ptins = getphins(WORK_PHFIG, ins1);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con1 && ptcon->INDEX == index1) {
         xcon1 = ptcon->XCON;
         ycon1 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con1) {
         xcon1 = ptref->XREF;
         ycon1 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("WIRE2 same name for reference and", stderr);
      (void)fprintf(stderr, " connector '%s' in figure '%s'\n",
                    ptcon->NAME, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nWIRE2 reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                    con1, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&xcon1, &ycon1, xcon1, ycon1, ptins->XINS, ptins->YINS,
       ptfig->XAB1, ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   con2 = namealloc(checkname(con2));
   ptins = getphins(WORK_PHFIG, ins2);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con2 && ptcon->INDEX == index2) {
         xcon2 = ptcon->XCON;
         ycon2 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con2) {
         xcon2 = ptref->XREF;
         ycon2 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("WIRE2 same name for reference and", stderr);
      (void)fprintf(stderr, " connector '%s' in figure '%s'\n",
                    ptcon->NAME, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nWIRE2 reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                    con2, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&xcon2, &ycon2, xcon2, ycon2, ptins->XINS, ptins->YINS,
       ptfig->XAB1, ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);

   return addptype(NULL, MBK_SEGMENT,
             addchain(
                addchain(NULL,
                   addphseg(WORK_PHFIG, layer, width,
                            xcon1, ycon1, x, y, (char *)NULL)),
                addphseg(WORK_PHFIG, layer, width,
                         xcon2, ycon2, x, y, (char *)NULL)));
}

/*******************************************************************************
* function WIRE3                                                               *
*******************************************************************************/
ptype_list *genWIRE3(layer, width, ins1, con1, index1, ins2, con2, index2,
x1, y1, x2, y2)
char layer;
long width, x1, y1, x2, y2;
char *ins1, *ins2, *con1, *con2;
long index1, index2;
{
long xcon1, ycon1, xcon2, ycon2; /* connexion coordinates */
phcon_list *ptcon; /* scanning cons since getphcon exits if not present */
phref_list *ptref; /* scanning refs since getphref exits if not present */
phins_list *ptins;
phfig_list *ptfig;

   con1 = namealloc(checkname(con1));
   ptins = getphins(WORK_PHFIG, ins1);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con1 && ptcon->INDEX == index1) {
         xcon1 = ptcon->XCON;
         ycon1 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con1) {
         xcon1 = ptref->XREF;
         ycon1 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("WIRE3 same name for reference and", stderr);
      (void)fprintf(stderr, " connector '%s' in figure '%s'\n",
                    ptcon->NAME, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nWIRE3 reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                    con1, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&xcon1, &ycon1, xcon1, ycon1, ptins->XINS, ptins->YINS,
       ptfig->XAB1, ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   con2 = namealloc(checkname(con2));
   ptins = getphins(WORK_PHFIG, ins2);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con2 && ptcon->INDEX == index2) {
         xcon2 = ptcon->XCON;
         ycon2 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con2) {
         xcon2 = ptref->XREF;
         ycon2 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("WIRE3 same name for reference and", stderr);
      (void)fprintf(stderr, " connector '%s' in figure '%s'\n",
                    ptcon->NAME, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nWIRE3 reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                  con2, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&xcon2, &ycon2, xcon2, ycon2, ptins->XINS, ptins->YINS,
       ptfig->XAB1, ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   return addptype(NULL, MBK_SEGMENT,
             addchain(
                addchain(
                   addchain(NULL, addphseg(WORK_PHFIG, layer, width,
                                           xcon1, ycon1, x1, y1, NULL)),
                   addphseg(WORK_PHFIG, layer, width, x2, y2,  x1, y1, NULL)),
             addphseg(WORK_PHFIG, layer, width, xcon2, ycon2, x2, y2, NULL)));

}

/*******************************************************************************
* function  PHVIA                                                              *
*******************************************************************************/
ptype_list *genPHVIA(type, x, y)
char  type;
long  x, y;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PHVIA impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   return addptype(NULL, MBK_VIA, g_addphvia(WORK_PHFIG, type, x, y, 0, 0, NULL));
}

/*******************************************************************************
* function  PHBIGVIA                                                              *
*******************************************************************************/
ptype_list *genPHBIGVIA(type, x, y, dx, dy, name)
char  type, *name;
long  x, y, dx, dy;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PHVIA impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   return addptype(NULL, MBK_VIA, g_addphvia(WORK_PHFIG, type, x, y, dx, dy, name));
}

/*******************************************************************************
* function  PLACE                                                              *
*******************************************************************************/
ptype_list *genPLACE(figname, insname, sym, x, y)
char *figname;
char *insname;
int sym;
long x, y;
{
phfig_list *ptfig;
long deltax = 0, deltay = 0; /* To make -Wall happy */
num_list *ptnum;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal PLACE : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }
   WORK_PHINS = addphins(WORK_PHFIG, figname, insname, sym, x, y);
   ptfig = getphfig(figname, 'P');
   if (sym == NOSYM || sym == SYM_X || sym == SYM_Y || sym == SYMXY) {
      deltax = x + ptfig->XAB2 - ptfig->XAB1;
      deltay = y + ptfig->YAB2 - ptfig->YAB1;
   }
   if (sym == ROT_P || sym == ROT_M || sym == SY_RP || sym == SY_RM) {
      deltax = x + ptfig->YAB2 - ptfig->YAB1;
      deltay = y + ptfig->XAB2 - ptfig->XAB1;
   }
   ptnum = addnum((num_list *)NULL, deltay);
   ptnum = addnum(ptnum, deltax);
   WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
       (void *)ptnum);
   return addptype(NULL, MBK_INSTANCE, addchain(NULL, WORK_PHINS));
}

/*******************************************************************************
* function  PLACE_RIGHT                                                        *
*******************************************************************************/
ptype_list *genPLACE_RIGHT(figname, insname, sym)
char *figname;
char *insname;
int sym;
{
long x, y;
long deltax = 0, deltay = 0; /* To make -Wall happy */
char trsf;
phfig_list *ptfig = NULL;
ptype_list *ptype = NULL;
num_list *ptnum = NULL;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_RIGHT impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_PHINS == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_RIGHT impossible : no previous instance ", stderr);
      (void)fprintf(stderr, "in figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal PLACE_RIGHT : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }
   ptype = getptype(WORK_PHINS->USER, (long)PLACEABOX);
   if (ptype == NULL) {
      ptfig = getphfig(WORK_PHINS->FIGNAME, 'P');
      trsf = WORK_PHINS->TRANSF;
      if (trsf == NOSYM || trsf == SYM_X || trsf == SYM_Y || trsf == SYMXY) {
         deltax = WORK_PHINS->XINS + ptfig->XAB2 - ptfig->XAB1;
         deltay = WORK_PHINS->YINS + ptfig->YAB2 - ptfig->YAB1;
      }
      if (trsf == ROT_P || trsf == ROT_M || trsf == SY_RP || trsf == SY_RM) {
         deltax = WORK_PHINS->XINS + ptfig->YAB2 - ptfig->YAB1;
         deltay = WORK_PHINS->YINS + ptfig->XAB2 - ptfig->XAB1;
      }
      ptnum = addnum((num_list *)NULL, deltay);
      ptnum = addnum(ptnum, deltax);
      WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
           (void *)ptnum);
      ptype = WORK_PHINS->USER;
   }
   x = ((num_list *)(ptype->DATA))->DATA;
   y = WORK_PHINS->YINS;

   WORK_PHINS = addphins(WORK_PHFIG, figname, insname, sym, x, y);
   ptfig = getphfig(figname, 'P');
   if (sym == NOSYM || sym == SYM_X || sym == SYM_Y || sym == SYMXY) {
      deltax = x + ptfig->XAB2 - ptfig->XAB1;
      deltay = y + ptfig->YAB2 - ptfig->YAB1;
   }
   if (sym == ROT_P || sym == ROT_M || sym == SY_RP || sym == SY_RM) {
      deltax = x + ptfig->YAB2 - ptfig->YAB1;
      deltay = y + ptfig->XAB2 - ptfig->XAB1;
   }
   ptnum = addnum((num_list *)NULL, deltay);
   ptnum = addnum(ptnum, deltax);
   WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
       (void *)ptnum);
   return addptype(NULL, MBK_INSTANCE, addchain(NULL, WORK_PHINS));
}

/*******************************************************************************
* function  PLACE_TOP                                                          *
*******************************************************************************/
ptype_list *genPLACE_TOP(figname, insname, sym)
char *figname;
char *insname;
int sym;
{
long x, y;
long deltax = 0, deltay = 0; /* To make -Wall happy */
char trsf;
phfig_list *ptfig = NULL;
ptype_list *ptype = NULL;
num_list *ptnum = NULL;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_TOP impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_PHINS == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_TOP impossible : no previous instance ", stderr);
      (void)fprintf(stderr, "in figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal PLACE_TOP : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }
   ptype = getptype(WORK_PHINS->USER, (long)PLACEABOX);
   if (ptype == NULL) {
      ptfig = getphfig(WORK_PHINS->FIGNAME, 'P');
      trsf = WORK_PHINS->TRANSF;
      if (trsf == NOSYM || trsf == SYM_X || trsf == SYM_Y || trsf == SYMXY) {
         deltax = WORK_PHINS->XINS + ptfig->XAB2 - ptfig->XAB1;
         deltay = WORK_PHINS->YINS + ptfig->YAB2 - ptfig->YAB1;
      }
      if (trsf == ROT_P || trsf == ROT_M || trsf == SY_RP || trsf == SY_RM) {
         deltax = WORK_PHINS->XINS + ptfig->YAB2 - ptfig->YAB1;
         deltay = WORK_PHINS->YINS + ptfig->XAB2 - ptfig->XAB1;
      }
      ptnum = addnum((num_list *)NULL, deltay);
      ptnum = addnum(ptnum, deltax);
      WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
           (void *)ptnum);
      ptype = WORK_PHINS->USER;
   }
   y = ((num_list *)(ptype->DATA))->NEXT->DATA;
   x = WORK_PHINS->XINS;
   WORK_PHINS = addphins(WORK_PHFIG, figname, insname, sym, x, y);

   ptfig = getphfig(figname, 'P');
   if (sym == NOSYM || sym == SYM_X || sym == SYM_Y || sym == SYMXY) {
      deltax = x + ptfig->XAB2 - ptfig->XAB1;
      deltay = y + ptfig->YAB2 - ptfig->YAB1;
   }
   if (sym == ROT_P || sym == ROT_M || sym == SY_RP || sym == SY_RM) {
      deltax = x + ptfig->YAB2 - ptfig->YAB1;
      deltay = y + ptfig->XAB2 - ptfig->XAB1;
   }
   ptnum = addnum((num_list *)NULL, deltay);
   ptnum = addnum(ptnum, deltax);
   WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
       (void *)ptnum);
   return addptype(NULL, MBK_INSTANCE, addchain(NULL, WORK_PHINS));
}

/*******************************************************************************
* function  PLACE_LEFT                                                         *
*******************************************************************************/
ptype_list *genPLACE_LEFT(figname, insname, sym)
char *figname;
char *insname;
int sym;
{
long x, y;
long deltax = 0, deltay = 0; /* To make -Wall happy */
char trsf;
phfig_list *ptfig = NULL;
ptype_list *ptype = NULL;
num_list *ptnum = NULL;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_LEFT impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_PHINS == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_LEFT impossible : no previous instance ", stderr);
      (void)fprintf(stderr, "in figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal PLACE_LEFT : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }
   ptype = getptype(WORK_PHINS->USER, (long)PLACEABOX);
   if (ptype == NULL) {
      ptfig = getphfig(WORK_PHINS->FIGNAME, 'P');
      trsf = WORK_PHINS->TRANSF;
      if (trsf == NOSYM || trsf == SYM_X || trsf == SYM_Y || trsf == SYMXY) {
         deltax = WORK_PHINS->XINS + ptfig->XAB2 - ptfig->XAB1;
         deltay = WORK_PHINS->YINS + ptfig->YAB2 - ptfig->YAB1;
      }
      if (trsf == ROT_P || trsf == ROT_M || trsf == SY_RP || trsf == SY_RM) {
         deltax = WORK_PHINS->XINS + ptfig->YAB2 - ptfig->YAB1;
         deltay = WORK_PHINS->YINS + ptfig->XAB2 - ptfig->XAB1;
      }
      ptnum = addnum((num_list *)NULL, deltay);
      ptnum = addnum(ptnum, deltax);
      WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
           (void *)ptnum);
   }
   ptfig = getphfig(figname, 'P');
   if (sym == NOSYM || sym == SYM_X || sym == SYM_Y || sym == SYMXY) {
      deltax = ptfig->XAB2 - ptfig->XAB1;
      deltay = ptfig->YAB2 - ptfig->YAB1;
   }
   if (sym == ROT_P || sym == ROT_M || sym == SY_RP || sym == SY_RM) {
      deltax = ptfig->YAB2 - ptfig->YAB1;
      deltay = ptfig->XAB2 - ptfig->XAB1;
   }
   x = WORK_PHINS->XINS - deltax;
   y = WORK_PHINS->YINS;
   WORK_PHINS = addphins(WORK_PHFIG, figname, insname, sym, x, y);
   ptnum = addnum((num_list *)NULL, y + deltay);
   ptnum = addnum(ptnum, x + deltax);
   WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
       (void *)ptnum);
   return addptype(NULL, MBK_INSTANCE, addchain(NULL, WORK_PHINS));
}

/*******************************************************************************
* function  PLACE_BOTTOM                                                       *
*******************************************************************************/
ptype_list *genPLACE_BOTTOM(figname, insname, sym)
char *figname;
char *insname;
int sym;
{
long x, y;
long deltax = 0, deltay = 0; /* To make -Wall happy */
char trsf;
phfig_list *ptfig = NULL;
ptype_list *ptype = NULL;
num_list *ptnum = NULL;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_BOTTOM impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_PHINS == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_BOTTOM impossible : no previous instance ", stderr);
      (void)fprintf(stderr, "in figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal PLACE_BOTTOM : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }
   ptype = getptype(WORK_PHINS->USER, (long)PLACEABOX);
   if (ptype == NULL) {
      ptfig = getphfig(WORK_PHINS->FIGNAME, 'P');
      trsf = WORK_PHINS->TRANSF;
      if (trsf == NOSYM || trsf == SYM_X || trsf == SYM_Y || trsf == SYMXY) {
         deltax = WORK_PHINS->XINS + ptfig->XAB2 - ptfig->XAB1;
         deltay = WORK_PHINS->YINS + ptfig->YAB2 - ptfig->YAB1;
      }
      if (trsf == ROT_P || trsf == ROT_M || trsf == SY_RP || trsf == SY_RM) {
         deltax = WORK_PHINS->XINS + ptfig->YAB2 - ptfig->YAB1;
         deltay = WORK_PHINS->YINS + ptfig->XAB2 - ptfig->XAB1;
      }
      ptnum = addnum((num_list *)NULL, deltay);
      ptnum = addnum(ptnum, deltax);
      WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
           (void *)ptnum);
   }
   ptfig = getphfig(figname, 'P');
   if (sym == NOSYM || sym == SYM_X || sym == SYM_Y || sym == SYMXY) {
      deltax = ptfig->XAB2 - ptfig->XAB1;
      deltay = ptfig->YAB2 - ptfig->YAB1;
   }
   if (sym == ROT_P || sym == ROT_M || sym == SY_RP || sym == SY_RM) {
      deltax = ptfig->YAB2 - ptfig->YAB1;
      deltay = ptfig->XAB2 - ptfig->XAB1;
   }
   x = WORK_PHINS->XINS;
   y = WORK_PHINS->YINS - deltay;
   WORK_PHINS = addphins(WORK_PHFIG, figname, insname, sym, x, y);
   ptnum = addnum((num_list *)NULL, y + deltay);
   ptnum = addnum(ptnum, x + deltax);
   WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
       (void *)ptnum);
   return addptype(NULL, MBK_INSTANCE, addchain(NULL, WORK_PHINS));
}

/*******************************************************************************
* function  PLACE_REF                                                          *
*******************************************************************************/
ptype_list *genPLACE_REF(figname, insname, sym, insref, refname)
char *figname;
char *insname;
char *insref;
char *refname;
int sym;
{
long x, y;
long deltax = 0, deltay = 0; /* To make -Wall happy */
char trsf;
phfig_list *ptfig = NULL;
phins_list *ptins = NULL;
phref_list *ptref = NULL;
ptype_list *ptype = NULL;
num_list *ptnum = NULL;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_REF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_PHINS == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_REF impossible : no previous instance ", stderr);
      (void)fprintf(stderr, "in figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal PLACE_REF : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }
   ptype = getptype(WORK_PHINS->USER, (long)PLACEABOX);
   if (ptype == NULL) {
      ptfig = getphfig(WORK_PHINS->FIGNAME, 'P');
      trsf = WORK_PHINS->TRANSF;
      if (trsf == NOSYM || trsf == SYM_X || trsf == SYM_Y || trsf == SYMXY) {
         deltax = WORK_PHINS->XINS + ptfig->XAB2 - ptfig->XAB1;
         deltay = WORK_PHINS->YINS + ptfig->YAB2 - ptfig->YAB1;
      }
      if (trsf == ROT_P || trsf == ROT_M || trsf == SY_RP || trsf == SY_RM) {
         deltax = WORK_PHINS->XINS + ptfig->YAB2 - ptfig->YAB1;
         deltay = WORK_PHINS->YINS + ptfig->XAB2 - ptfig->XAB1;
      }
      ptnum = addnum((num_list *)NULL, deltay);
      ptnum = addnum(ptnum, deltax);
      WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
           (void *)ptnum);
   }
   ptins = getphins(WORK_PHFIG, insref);
   ptfig = getphfig(ptins->FIGNAME, 'A');
   ptref = getphref(ptfig, checkname(refname));
   xyflat(&x, &y, ptref->XREF, ptref->YREF, ptins->XINS, ptins->YINS,
         ptfig->XAB1, ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   WORK_PHINS = addphins(WORK_PHFIG, figname, insname, sym, x, y);

   ptfig = getphfig(figname, 'P');
   if (sym == NOSYM || sym == SYM_X || sym == SYM_Y || sym == SYMXY) {
      deltax = x + ptfig->XAB2 - ptfig->XAB1;
      deltay = y + ptfig->YAB2 - ptfig->YAB1;
   }
   if (sym == ROT_P || sym == ROT_M || sym == SY_RP || sym == SY_RM) {
      deltax = x + ptfig->YAB2 - ptfig->YAB1;
      deltay = y + ptfig->XAB2 - ptfig->XAB1;
   }
   ptnum = addnum((num_list *)NULL, deltay);
   ptnum = addnum(ptnum, deltax);
   WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
       (void *)ptnum);
   return addptype(NULL, MBK_INSTANCE, addchain(NULL, WORK_PHINS));
}

/*******************************************************************************
* function  PLACE_ON                                                           *
*******************************************************************************/
ptype_list *genPLACE_ON(figname, insname1, con1, index1, sym, insname2, con2,
index2)
char *figname;
char *insname1, *insname2, *con1, *con2;
int sym;
long index1, index2;
{
long x, y, x1, y1, x2, y2;
long xrc1 = 0, yrc1 = 0, xrc2 = 0, yrc2 = 0; /* To make -Wall happy */
long deltax = 0, deltay = 0; /* To make -Wall happy */
char trsf;
phfig_list *ptfig = NULL;
ptype_list *ptype = NULL;
num_list *ptnum = NULL;
phins_list *ptins;
phcon_list *ptcon;
phref_list *ptref;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_ON impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_PHINS == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_ON impossible : no previous instance ", stderr);
      (void)fprintf(stderr, "in figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }
   if (hassep(insname1)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal PLACE_ON : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname1);
      EXIT(1);
   }
   ptins = getphins(WORK_PHFIG, insname2);
   ptype = getptype(ptins->USER, (long)PLACEABOX);
   if (ptype == NULL) {
      ptfig = getphfig(ptins->FIGNAME, 'P');
      trsf = ptins->TRANSF;
      if (trsf == NOSYM || trsf == SYM_X || trsf == SYM_Y || trsf == SYMXY) {
         deltax = ptins->XINS + ptfig->XAB2 - ptfig->XAB1;
         deltay = ptins->YINS + ptfig->YAB2 - ptfig->YAB1;
      }
      if (trsf == ROT_P || trsf == ROT_M || trsf == SY_RP || trsf == SY_RM) {
         deltax = ptins->XINS + ptfig->YAB2 - ptfig->YAB1;
         deltay = ptins->YINS + ptfig->XAB2 - ptfig->XAB1;
      }
      ptnum = addnum((num_list *)NULL, deltay);
      ptnum = addnum(ptnum, deltax);
      ptins->USER = addptype(ptins->USER, (long)PLACEABOX, (void *)ptnum);
      ptype = ptins->USER;
   }

   ptfig = getphfig(ptins->FIGNAME, 'P');
   con2 = namealloc(checkname(con2));
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con2 && ptcon->INDEX == index2) {
         xrc2 = ptcon->XCON;
         yrc2 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con2) {
         xrc2 = ptref->XREF;
         yrc2 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nPLACE_ON : same name for ", stderr);
      (void)fprintf(stderr, "reference and connector '%s' in figure '%s'\n",
                         con2, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nPLACE_ON : reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                         con2, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&x2, &y2, xrc2, yrc2, ptins->XINS, ptins->YINS, ptfig->XAB1,
        ptfig->YAB1, ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   ptfig = getphfig(figname, 'P');
   con1 = namealloc(checkname(con1));
   for (ptcon = ptfig->PHCON; ptcon != NULL; ptcon = ptcon->NEXT)
      if (ptcon->NAME == con1 && ptcon->INDEX == index1) {
         xrc1 = ptcon->XCON;
         yrc1 = ptcon->YCON;
         break;
      }
   for (ptref = ptfig->PHREF; ptref != NULL; ptref = ptref->NEXT)
      if (ptref->NAME == con1) {
         xrc1 = ptref->XREF;
         yrc1 = ptref->YREF;
         break;
      }
   if (ptref != NULL && ptcon != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nPLACE_ON : same name for ", stderr);
      (void)fprintf(stderr, "reference and connector '%s' in figure '%s'\n",
                         con1, ptfig->NAME);
      EXIT(1);
   }
   if (ptref == NULL && ptcon == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nPLACE_ON : reference or", stderr);
      (void)fprintf(stderr, " connector '%s' does not exist in figure '%s'\n",
                         con1, ptfig->NAME);
      EXIT(1);
   }
   xyflat(&x1, &y1, xrc1, yrc1, 0L, 0L, ptfig->XAB1, ptfig->YAB1,
       ptfig->XAB2, ptfig->YAB2, sym);

   if (sym == NOSYM || sym == SYM_X || sym == SYM_Y || sym == SYMXY) {
      deltax = ptfig->XAB2 - ptfig->XAB1;
      deltay = ptfig->YAB2 - ptfig->YAB1;
   }
   if (sym == ROT_P || sym == ROT_M || sym == SY_RP || sym == SY_RM) {
      deltax = ptfig->YAB2 - ptfig->YAB1;
      deltay = ptfig->XAB2 - ptfig->XAB1;
   }

   x = x2 - x1;
   y = y2 - y1;

   ptnum = addnum((num_list *)NULL, y + deltay);
   ptnum = addnum(ptnum, x + deltax);

   WORK_PHINS = addphins(WORK_PHFIG, figname, insname1, sym, x, y);

   WORK_PHINS->USER = addptype(WORK_PHINS->USER, (long)PLACEABOX,
       (void *)ptnum);
   return addptype(NULL, MBK_INSTANCE, addchain(NULL, WORK_PHINS));
}

/*******************************************************************************
* function HEIGHT                                                              *
*******************************************************************************/
long genHEIGHT(cellName)
char *cellName;
{
phfig_list *pf;

   pf = getphfig(cellName, 'P');
   return pf->YAB2 - pf->YAB1;
}

/*******************************************************************************
* function WIDTH                                                               *
*******************************************************************************/
long genWIDTH(cellName)
char *cellName;
{
phfig_list *pf;

   pf = getphfig(cellName, 'P');
   return pf->XAB2 - pf->XAB1;
}

/*******************************************************************************
* function  GET_INS_X                                                          *
*******************************************************************************/
long genGET_INS_X(insname)
char *insname;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("Illegal GET_INS_X : No DEF_PHFIG\n", stderr);
      EXIT(1);
   }

   return getphins(WORK_PHFIG, insname)->XINS;
}

/*******************************************************************************
* function  GET_INS_Y                                                          *
*******************************************************************************/
long genGET_INS_Y(insname)
char *insname;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("Illegal GET_INS_X : No DEF_PHFIG\n", stderr);
      EXIT(1);
   }

   return getphins(WORK_PHFIG, insname)->YINS;
}

/*******************************************************************************
* function FLATTEN_PHFIG                                                       *
*******************************************************************************/
void genFLATTEN_PHFIG(insname, concat)
char *insname;
char concat;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("FLATTEN_PHFIG impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   flattenphfig(WORK_PHFIG, insname, concat);
}

/*******************************************************************************
* function FLATTEN_ALL_PHINS                                                   *
*******************************************************************************/
void genFLATTEN_ALL_PHINS(concat, catal)
char concat, catal;
{
chain_list *c, *namelist = NULL;
phins_list *l;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("Illegal FLATTEN_ALL_PHINS : No DEF_PHFIG\n", stderr);
      EXIT(1);
   }

   for (l = WORK_PHFIG->PHINS; l; l = l->NEXT)
      if (catal == NO || !incatalog(l->FIGNAME))
         namelist = addchain(namelist, (void *)l->INSNAME);

   for (c = namelist; c; c = c->NEXT)
      flattenphfig(WORK_PHFIG, (char *)c->DATA, concat);

   freechain(namelist);
}

/*******************************************************************************
* function  PHREF                                                              *
*******************************************************************************/
ptype_list *genPHREF(type, name, x, y)
char *type;
char *name;
long x, y;
{
   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PHREF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   return addptype(NULL, MBK_REFERENCE,
             addchain(NULL, addphref(WORK_PHFIG, type, checkname(name), x, y)));
}

void genDEL_PHREF(type, pattern)
char *type;
char *pattern;
{
char       *s;
int         n;
phref_list *r, *rr;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("DEL_PHREF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (pattern == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("DEL_PHREF impossible : null pattern not allowed\n", stderr);
      EXIT(1);
   }

   s = strchr(pattern, '*');
   if (s == NULL)
      n = strlen(pattern);
   else
      n = s - pattern;

   for (r = WORK_PHFIG->PHREF; r; r = rr) {
      rr = r->NEXT;
      if (!strcmp(r->FIGNAME, type) && !strncmp(r->NAME, pattern, n))
         delphref(WORK_PHFIG, r);
   }
}

/*******************************************************************************
* function  COPY_UP_REF                                                        *
*******************************************************************************/
ptype_list *genCOPY_UP_REF(refname, insname, newname)
char *refname;
char *insname;
char *newname;
{
phfig_list *ptnewfig;
phins_list *ptins;
phref_list *ptref;
long x, y;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("COPY_UP_REF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   ptins = getphins(WORK_PHFIG, insname);
   ptnewfig = getphfig(ptins->FIGNAME, 'A');
   ptref = getphref(ptnewfig, checkname(refname));

   /*  create reference  */
   xyflat(&x, &y, ptref->XREF, ptref->YREF, ptins->XINS, ptins->YINS,
         ptnewfig->XAB1, ptnewfig->YAB1, ptnewfig->XAB2, ptnewfig->YAB2,
         ptins->TRANSF);

   return addptype(NULL, MBK_REFERENCE,
             addchain(NULL,
                addphref(WORK_PHFIG, ptref->FIGNAME,
                         checkname(newname), x, y)));
}

/*******************************************************************************
* function  COPY_UP_ALL_REF                                                    *
*******************************************************************************/
ptype_list *genCOPY_UP_ALL_REF(reftype, insname, cheminom)
char *reftype;
char *insname;
char cheminom;
{
phfig_list *newfig;
phins_list *ins;
phref_list *ref;
chain_list *refs = NULL;
long x, y;
char *ref_;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("COPY_UP_ALL_REF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if ((WORK_PHFIG->XAB1 | WORK_PHFIG->YAB1 | WORK_PHFIG->XAB2
         | WORK_PHFIG->YAB2) == 0) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs(" COPY_UP_ALL_REF impossible : missing DEF_AB ", stderr);
      (void)fprintf(stderr, "on figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }

   ins = getphins(WORK_PHFIG, insname);
   newfig = getphfig(ins->FIGNAME, 'P');
   reftype = namealloc(reftype);

   for (ref = newfig->PHREF; ref != NULL; ref = ref->NEXT) {
      ref_ = ref->FIGNAME;
      if (ref_ != reftype)
         continue;
      xyflat(&x, &y, ref->XREF, ref->YREF, ins->XINS, ins->YINS,
                  newfig->XAB1, newfig->YAB1, newfig->XAB2, newfig->YAB2,
           ins->TRANSF);
      refs = addchain(refs, (void *)addphref(WORK_PHFIG, ref_,
                  cheminom == 'Y' ? concatname(insname, ref->NAME) :
          ref->NAME, x, y));
   }
   /* Absolutly needed for GenView */
   return addptype(NULL, MBK_REFERENCE, refs);
}

/*******************************************************************************
* function  PLACE_VIA_REF                                                      *
*******************************************************************************/
ptype_list *genPLACE_VIA_REF(insname, refname, type)
char *insname;
char *refname;
char type;
{
phfig_list *ptnewfig;
phins_list *ptins;
phref_list *ptref;
long x, y;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_VIA_REF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   ptins = getphins(WORK_PHFIG, insname);
   ptnewfig = getphfig(ptins->FIGNAME, 'P');
   ptref = getphref(ptnewfig, checkname(refname));

   xyflat(&x, &y, ptref->XREF, ptref->YREF, ptins->XINS, ptins->YINS,
            ptnewfig->XAB1, ptnewfig->YAB1, ptnewfig->XAB2, ptnewfig->YAB2,
            ptins->TRANSF);

   return addptype(NULL, MBK_VIA, g_addphvia(WORK_PHFIG, type, x, y, 0, 0, NULL));
}

/*******************************************************************************
* function  PLACE_CON_REF                                                      *
*******************************************************************************/
ptype_list *genPLACE_CON_REF(insname, refname, conname, layer, width, face)
char *insname;
char *refname;
char *conname;
char layer;
long width;
char face;
{
phfig_list *newfig;
phins_list *ins;
phcon_list *con = NULL; /* To make -Wall happy */
phseg_list *seg = NULL; /* may not be overwritten */
phref_list *ref;
/* chain_list *chain = NULL; */
long x, y;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_CON_REF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (!(WORK_PHFIG->XAB1 | WORK_PHFIG->YAB1 | WORK_PHFIG->XAB2
         | WORK_PHFIG->YAB2)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_CON_REF impossible : missing DEF_AB ", stderr);
      (void)fprintf(stderr, "on figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }

   ins = getphins(WORK_PHFIG, insname);
   newfig = getphfig(ins->FIGNAME, 'P');
   ref = getphref(newfig, checkname(refname));

   xyflat(&x, &y, ref->XREF, ref->YREF, ins->XINS, ins->YINS, newfig->XAB1,
        newfig->YAB1, newfig->XAB2, newfig->YAB2, ins->TRANSF);

   switch (face) {
      case WEST :
         con = addphcon(WORK_PHFIG, face, checkname(conname),
                        WORK_PHFIG->XAB1, y, layer, width);
         if (x > WORK_PHFIG->XAB1)
            seg = addphseg(WORK_PHFIG, layer, width, WORK_PHFIG->XAB1,
                                       y, x, y, (char *)NULL);
         break;
      case EAST :
         con = addphcon(WORK_PHFIG, face, checkname(conname), WORK_PHFIG->XAB2,
                                    y, layer, width);
         if (x < WORK_PHFIG->XAB2)
            seg = addphseg(WORK_PHFIG, layer, width, x, y,
                           WORK_PHFIG->XAB2, y, (char *)NULL);
         break;
      case NORTH :
         con = addphcon(WORK_PHFIG, face, checkname(conname), x,
                        WORK_PHFIG->YAB2, layer, width);
         if (y < WORK_PHFIG->YAB2)
            seg = addphseg(WORK_PHFIG, layer, width, x, y, x,
                           WORK_PHFIG->YAB2, (char *)NULL);
         break;
      case SOUTH :
         con = addphcon(WORK_PHFIG, face, checkname(conname), x,
                        WORK_PHFIG->YAB1, layer, width);
         if (y > WORK_PHFIG->YAB1)
            seg = addphseg(WORK_PHFIG, layer, width, x, y, x,
                           WORK_PHFIG->YAB1, (char *)NULL);
         break;
   }

   /* Absolutly needed for GenView */
   /* if (seg)
    *   chain = addchain(chain, (void *)seg);
    */
   return addptype(
             seg ? addptype(NULL, MBK_SEGMENT, addchain(NULL, seg)) : NULL,
                 MBK_CONNECTOR,
                 addchain(NULL, con));
}

/*******************************************************************************
* function  PLACE_SEG_REF                                                      *
*******************************************************************************/
ptype_list *genPLACE_SEG_REF(insname, refname, layer, width, face)
char *insname;
char *refname;
char layer;
long width;
char face;
{
phfig_list *newfig;
phins_list *ins;
phseg_list *seg = NULL;
phref_list *ref;
long x, y;

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_SEG_REF impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (!(WORK_PHFIG->XAB1 | WORK_PHFIG->YAB1 | WORK_PHFIG->XAB2
         | WORK_PHFIG->YAB2)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("PLACE_SEG_REF impossible : missing DEF_AB ", stderr);
      (void)fprintf(stderr, "on figure %s\n", WORK_PHFIG->NAME);
      EXIT(1);
   }

   ins = getphins(WORK_PHFIG, insname);
   newfig = getphfig(ins->FIGNAME, 'P');
   ref = getphref(newfig, checkname(refname));

   xyflat(&x, &y, ref->XREF, ref->YREF, ins->XINS, ins->YINS, newfig->XAB1,
        newfig->YAB1, newfig->XAB2, newfig->YAB2, ins->TRANSF);

   switch (face) {
   case WEST :
      if (x > WORK_PHFIG->XAB1)
         seg = addphseg(WORK_PHFIG, layer, width, WORK_PHFIG->XAB1,
                                     y, x, y, (char *)NULL);
      break;
   case EAST :
      if (x < WORK_PHFIG->XAB2)
         seg = addphseg(WORK_PHFIG, layer, width, x, y,
             WORK_PHFIG->XAB2, y, (char *)NULL);
      break;
   case NORTH :
      if (y < WORK_PHFIG->YAB2)
         seg = addphseg(WORK_PHFIG, layer, width, x, y,
             WORK_PHFIG->YAB2, y, (char *)NULL);
      break;
   case SOUTH :
      if (y < WORK_PHFIG->YAB1)
         seg = addphseg(WORK_PHFIG, layer, width, x, y,
             WORK_PHFIG->YAB1, y, (char *)NULL);
      break;
   }
   return addptype(NULL, MBK_SEGMENT, addchain(NULL, seg));
}

/*******************************************************************************
* function GET_REF_X                                                           *
*******************************************************************************/
long genGET_PHFIG_REF_X(refname)
char *refname;
{
phref_list *ptref;

   ptref = getphref(WORK_PHFIG, checkname(refname));
   return ptref->XREF;
}

/*******************************************************************************
* function GET_REF_Y                                                           *
*******************************************************************************/
long genGET_PHFIG_REF_Y(refname)
char *refname;
{
phref_list *ptref;

   ptref = getphref(WORK_PHFIG, checkname(refname));
   return ptref->YREF;
}

/*******************************************************************************
* function GET_REF_X                                                           *
*******************************************************************************/
long genGET_REF_X(insname, refname)
char *insname, *refname;
{
phfig_list *ptfig; /* pointer on model of instance called insname */
phins_list *ptins; /* pointer on instance called insname */
phref_list *ptref; /* pointer on the refname called ref inside ptins */
long x_ref, y_ref; /* return values, y_ref here for beauty */

   ptins = getphins(WORK_PHFIG, insname);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   ptref = getphref(ptfig, checkname(refname));

   xyflat(&x_ref, &y_ref, ptref->XREF, ptref->YREF, ptins->XINS, ptins->YINS,
        ptfig->XAB1, ptfig->YAB1,  ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   return x_ref;
}

/*******************************************************************************
* function GET_REF_Y                                                           *
*******************************************************************************/
long genGET_REF_Y(insname, refname)
char *insname, *refname;
{
phfig_list *ptfig; /* pointer on model of instance called insname */
phins_list *ptins; /* pointer on instance called insname */
phref_list *ptref; /* pointer on the refname called ref inside ptins */
long x_ref, y_ref; /* return values, x_ref here for beauty */

   ptins = getphins(WORK_PHFIG, insname);
   ptfig = getphfig(ptins->FIGNAME, 'P');
   ptref = getphref(ptfig, checkname(refname));

   xyflat(&x_ref, &y_ref, ptref->XREF, ptref->YREF, ptins->XINS, ptins->YINS,
        ptfig->XAB1, ptfig->YAB1,  ptfig->XAB2, ptfig->YAB2, ptins->TRANSF);
   return y_ref;
}

/*******************************************************************************
* set of functions to reverse the `logical' ordering of a set of connectors    
* This quite weird function is required by the custom block generators
* so to be able easily to provide big and little endian bit ordering:
* an option usually called -msb0
*******************************************************************************/
#define HARD_CONNECTOR    'H'
#define VIRTUAL_CONNECTOR 'V'
/* connector name :
   since connectors may be either hard, mbk phcon, or virtual, mbk phref,
   we need to do some tricky businness when working on the overall
   connector list. */
#define HVNAME(c) (*((c)->TYPE == HARD_CONNECTOR \
                     ? &((phcon_list *)((c)->DATA))->NAME \
                     : &((phref_list *)((c)->DATA))->NAME))

static int hvconnectorcmp(flc, slc)
ptype_list **flc, **slc;
{
   return naturalstrcmp(HVNAME(*slc), HVNAME(*flc));
}

static ptype_list *sorthvcon(connectors, cmp)
ptype_list *connectors;
int (*cmp)();
{
long n, i;
ptype_list *phc, **tc;

   if (!connectors) 
      return NULL;

   /* first :
      count connectors. */
   for (n = 0, phc = connectors; phc; phc = phc->NEXT)
      n++;
   tc = (ptype_list **)mbkalloc(n * sizeof(ptype_list *));
   for (n = 0, phc = connectors; phc; phc = phc->NEXT)
      tc[n++] = phc;
   /* second :
      sort them. */
   qsort((void *)tc, n, sizeof(ptype_list *), cmp);
   /* tables :
      the last element of the table is t[n - 1].
      relink the list in the sorted order. */
   n--;
   connectors = tc[0];
   for (i = 0; i < n; i++)
      tc[i]->NEXT = tc[i + 1];
   tc[n]->NEXT = (ptype_list *)NULL;
   mbkfree((void *)tc);
   return connectors;
}

void genREVERSE_PHCON(conname)
char *conname;
{
ptype_list *c = NULL, *cstart, *cend = NULL; /* To make -Wall happy */
char *s = namealloc("ref_con"); /* only for a short time, to build the list */
long i, j, k, sign;
void *hvc; /* hard and virtual connectors */

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("REVERSE_PHCON impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }

   /* first :
      create a list containing typed connectors.
      virtual connectors are known as "ref_con", and s is initialized
      to this nameallocated value. */
   for (hvc = (void *)WORK_PHFIG->PHCON; hvc;
         hvc = (void *)((phcon_list *)hvc)->NEXT)
      c = addptype(c, HARD_CONNECTOR, hvc);
   for (hvc = (void *)WORK_PHFIG->PHREF; hvc;
         hvc = (void *)((phref_list *)hvc)->NEXT)
      if (((phref_list *)hvc)->FIGNAME == s)
         c = addptype(c, VIRTUAL_CONNECTOR, hvc);
   
   hvc = (void *)sorthvcon(c, hvconnectorcmp);
   k = strlen(conname);
   for (cstart = (ptype_list *)hvc; cstart; cstart = cstart->NEXT) {
      /* not a vector :
         If the connector doesn't belong to a vector, nobody cares. */
      if ((s = strchr(HVNAME(cstart), ' ')) == NULL)
         continue;
      if (!strcmp(vectorradical(HVNAME(cstart)), conname)) {
         cend = cstart;
         /* simple :
            Only one connector s'got this name. */
         if ((c = cstart->NEXT) == NULL)
            return;
         while (c && !strcmp(vectorradical(HVNAME(c)), conname)) {
            cend = c;
            c = c->NEXT;
         }
         break;
      }
   }
   if (!cstart)
      return;
   i = vectorindex(HVNAME(cstart));
   j = vectorindex(HVNAME(cend));
   /* hypothesys :
      since I assume the vectorized values are correct from a mbk point of
      view, the algorithm is quit simple.
      Given a first vector number and its growing direction, the number of
      elements of the list supplies the range. */
   sign = j > i ? 1 : -1;
   /* delta :
      I need only the delta value between the vector borders. */
   j -= i;
   k = vectorindex(HVNAME(cstart));
   for (c = cstart; c != cend->NEXT; c = c->NEXT) {
      if (k != vectorindex(HVNAME(c))) {
         j -= sign;
         k = vectorindex(HVNAME(c));
      }
      /* Since for obscure reasons the datapath compiler needs virtual
         connector indexing as `name index_track', I have to take care
         of that s**t.
         If I get that to change in the futur, then only the following
         line will be needed :
         HVNAME(c) = vectorize(conname, i + j);
      */
      if (c->TYPE == HARD_CONNECTOR)
         HVNAME(c) = vectorize(conname, i + j);
      else {
         if ((s = strrchr(HVNAME(c), '_')) != NULL) {
         char buffer[256]; /* I don't want to malloc here */
            (void)sprintf(buffer, "%s %ld%s", conname, i + j, s);
            HVNAME(c) = namealloc(buffer);
         } else
            HVNAME(c) = vectorize(conname, i + j);
      }
   }
}

/*******************************************************************************
* OUTLINE                                                                      *
*******************************************************************************/
void genOUTLINE()
{
void **now, *next;

   /* delete instances and vias */
   for (now = (void *)&WORK_PHFIG->PHINS; *now;) {
      next = (void *)(*(phins_list **)now)->NEXT;
      mbkfree(*now);
      *now = next;
   }
   for (now = (void *)&WORK_PHFIG->PHVIA; *now;) {
      next = (void *)(*(phvia_list **)now)->NEXT;
      mbkfree(*now);
      *now = next;
   }
   /* delete the segments other than feed through */
   for (now = (void *)&WORK_PHFIG->PHSEG; *now;) {
      if ((*(phseg_list **)now)->LAYER == TPOLY ||
            (*(phseg_list **)now)->LAYER == TALU1 ||
            (*(phseg_list **)now)->LAYER == TALU2 ||
            (*(phseg_list **)now)->LAYER == TALU3)
         now = (void **)&(*(phseg_list **)now)->NEXT;
      else {
         next = (void *)(*(phseg_list **)now)->NEXT;
         mbkfree(*now);
         *now = next;
      }
   }
}


/*******************************************************************************
* logical user functions                                                       *
*******************************************************************************/
#include <stdarg.h>

long  num_index = 0;

/*******************************************************************************
* function DEF_LOFIG                                                           *
*******************************************************************************/
void genDEF_LOFIG(name)
char *name;
{
char *s;
lofig_list *ptfig;

   if (!CATA_LIB || !WORK_LIB)
      mbkenv();
   s = namealloc(name);

   for (ptfig = HEAD_LOFIG; ptfig; ptfig = ptfig->NEXT)
      if (ptfig->NAME == s)
         break;
   if (ptfig == NULL)
      WORK_LOFIG = addlofig(s);
   else
      WORK_LOFIG = ptfig;

   lolist = addchain(lolist, WORK_LOFIG);
}

/*******************************************************************************
* function LOAD_LOFIG                                                          *
*******************************************************************************/
void genLOAD_LOFIG(name)
char *name;
{
   if (!CATA_LIB || !WORK_LIB)
      mbkenv();
   WORK_LOFIG = getlofig(name, 'A');
   lolist = addchain(lolist, WORK_LOFIG);
}

/*******************************************************************************
* function SAVE_LOFIG                                                          *
*******************************************************************************/
void genSAVE_LOFIG()
{
losig_list *s;
locon_list *c;
chain_list *l;
ptype_list *t;
int external; /* more that one external connector on the same net? */

   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SAVE_LOFIG impossible : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   /* sets correct signal type :
      the use of some genlib function may create figures with badly typed
      external signal. */
   for (c = WORK_LOFIG->LOCON; c; c = c->NEXT)
      c->SIG->TYPE = EXTERNAL;
   lofigchain(WORK_LOFIG); /* builds up a list of connectors on each signals */
   for (s = WORK_LOFIG->LOSIG; s; s = s->NEXT) {
      external = 0; /* no external connector */
      t = getptype(s->USER, (long)LOFIGCHAIN);
      for (l = (chain_list *)t->DATA; l; l = l->NEXT) {
         c = (locon_list *)l->DATA;
         if (c->TYPE == EXTERNAL)
            external++;
      }
      if (external > 1) {
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fputs("The following external connectors are", stderr);
         (void)fputs(" on the same signal: \n", stderr);
         for (l = (chain_list *)t->DATA; l; l = l->NEXT) {
            c = (locon_list *)l->DATA;
            if (c->TYPE == EXTERNAL)
               (void)fprintf(stderr, "\t%s\n", c->NAME);
         }
         (void)fprintf(stderr, "\ton signal %s\n", getsigname(c->SIG));
         EXIT(1);
      }
   }
   checkloconorder(WORK_LOFIG->LOCON);
   sortlosig(&WORK_LOFIG->LOSIG);
   savelofig(WORK_LOFIG);
   lolist = delchain(lolist, lolist);
   if (lolist)
      WORK_LOFIG = (lofig_list *)lolist->DATA;
   else
      WORK_LOFIG = NULL;
}
/*******************************************************************************
* function LOINS                                                               *
*******************************************************************************/
void genLOINS(char *figname, ...)
{
losig_list *ptsig;
chain_list *ptchain = NULL, *ptchain1, *ptchain2 = NULL , *ptchain3;
va_list arg;
char *insname, *signame;
lofig_list *ptfig;

   va_start(arg, figname);
   insname = namealloc(va_arg(arg, char *));
   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "Illegal LOINS %s : ", insname);
      (void)fputs("missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }

   ptfig = getlofig(figname, 'P');
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal LOINS : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }
   /* expanding the bus names */
   while ((signame = va_arg(arg, char *)) != NULL) {
      if (!is_bus(signame)) { /* normal signal */
         if (!strcmp(vectorradical(checkname(signame)), UNCONNECTED))
            continue; /* Skip this signal */
         ptchain2 = addchain(ptchain2, (void *)namealloc(checkname(signame)));
      } else { /* it's a bus */
         long  i, from, to;
         char  *zig, *newzig;
         chain_list *ptchain4 = NULL;

         zig = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));
         if (!bus_decod(signame, zig, &from, &to)) {
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "Bad signal bus name %s\n", signame);
            EXIT(1);
         }
         /* Do not add these signals to the list if they are not
          * required: skip them and handle the next ones */
         if (!strcmp(zig, UNCONNECTED)) {
            mbkfree(zig);
            continue;
         }
         newzig = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));
         if (from > to)
            for (i = from; i >= to; i--) {
               (void)sprintf(newzig, "%s %ld", zig, i);
               ptchain4 = addchain(ptchain4, (void *)namealloc(newzig));
            }
         else
            for (i = from; i <= to; i++) {
               (void)sprintf(newzig, "%s %ld", zig, i);
               ptchain4 = addchain(ptchain4, (void *)namealloc(newzig));
            }
         for (ptchain1 = ptchain4; ptchain1->NEXT != NULL;
             ptchain1 = ptchain1->NEXT);
         ptchain1->NEXT = ptchain2;
         ptchain2 = ptchain4;

         mbkfree(zig);
         mbkfree(newzig);
      }
   }
   ptchain2 = (chain_list *)reverse(ptchain2);

   /* adding the signals if needed */
   for (ptchain3 = ptchain2; ptchain3 != NULL; ptchain3 = ptchain3->NEXT) {
      signame = namealloc((char *)ptchain3->DATA);
      for (ptsig = WORK_LOFIG->LOSIG; ptsig != NULL; ptsig = ptsig->NEXT) {
         for (ptchain1 = ptsig->NAMECHAIN; ptchain1; ptchain1 = ptchain1->NEXT)
            if (ptchain1->DATA == (void *)signame)
               break;
         if (ptchain1 != NULL)
            break;
      }
      if (ptsig == NULL) {
         num_index++;
         ptsig = addlosig(WORK_LOFIG, num_index,
                           addchain((chain_list *)NULL, (void *)signame),
                           INTERNAL);
      }
      ptchain = addchain(ptchain, (void *)ptsig);
   }
   {
   loins_list *ptins;
   ptype_list *pt;

      ptins = addloins(WORK_LOFIG, insname, ptfig, ptchain);
      pt = getptype(ptfig->USER, LOGEN);
      if (pt) {
         logen_list *g;
         ptins->USER = addptype(ptins->USER, LOGEN, NULL);
         for (g = pt->DATA; g; g = g->NEXT) {
            ptins->USER->DATA = (ptype_list *)duplogenlist(g, ptins->USER->DATA, g->NAME);
         }
      }
   }
   va_end(arg);
}

/*******************************************************************************
* function LOINSA                                                              *
  Aceepts an array to pass the signal values instead of a va_list
*******************************************************************************/
#define MAX_ARRAY_SIGS 1024
/* Put that outside of the LOINSA function in order to minimize the
 * stack frame required, and we don't need to be reentrant */
static char *_[MAX_ARRAY_SIGS]; /* Accepts only that many signals, I hope it is
                                   enough for one level of hierarchy */
void genLOINSA(char *model, char *instance, char *signals[])
{
   int  ns;

   for (ns = 0;
        signals[ns];
        ns++);
   if (ns > MAX_ARRAY_SIGS) {
      fprintf(stderr, "LOINSA: Too many signals in array\n");
      exit(1);
   }
   /* And now a very nice call, to be presented in programmation
    * booklets as an exemple */
   memset(_, 0, (ns + 1)  * sizeof(*_));
   memcpy(_, signals, ns * sizeof(*_));
   genLOINS(model, instance,
         _[0], _[1], _[2], _[3], _[4], _[5], _[6], _[7], _[8], _[9], _[10], _[11], 
         _[12], _[13], _[14], _[15], _[16], _[17], _[18], _[19], _[20], _[21], 
         _[22], _[23], _[24], _[25], _[26], _[27], _[28], _[29], _[30], _[31], 
         _[32], _[33], _[34], _[35], _[36], _[37], _[38], _[39], _[40], _[41], 
         _[42], _[43], _[44], _[45], _[46], _[47], _[48], _[49], _[50], _[51], 
         _[52], _[53], _[54], _[55], _[56], _[57], _[58], _[59], _[60], _[61], 
         _[62], _[63], _[64], _[65], _[66], _[67], _[68], _[69], _[70], _[71], 
         _[72], _[73], _[74], _[75], _[76], _[77], _[78], _[79], _[80], _[81], 
         _[82], _[83], _[84], _[85], _[86], _[87], _[88], _[89], _[90], _[91], 
         _[92], _[93], _[94], _[95], _[96], _[97], _[98], _[99], _[100], _[101], 
         _[102], _[103], _[104], _[105], _[106], _[107], _[108], _[109], _[110], _[111], 
         _[112], _[113], _[114], _[115], _[116], _[117], _[118], _[119], _[120], _[121], 
         _[122], _[123], _[124], _[125], _[126], _[127], _[128], _[129], _[130], _[131], 
         _[132], _[133], _[134], _[135], _[136], _[137], _[138], _[139], _[140], _[141], 
         _[142], _[143], _[144], _[145], _[146], _[147], _[148], _[149], _[150], _[151], 
         _[152], _[153], _[154], _[155], _[156], _[157], _[158], _[159], _[160], _[161], 
         _[162], _[163], _[164], _[165], _[166], _[167], _[168], _[169], _[170], _[171], 
         _[172], _[173], _[174], _[175], _[176], _[177], _[178], _[179], _[180], _[181], 
         _[182], _[183], _[184], _[185], _[186], _[187], _[188], _[189], _[190], _[191], 
         _[192], _[193], _[194], _[195], _[196], _[197], _[198], _[199], _[200], _[201], 
         _[202], _[203], _[204], _[205], _[206], _[207], _[208], _[209], _[210], _[211], 
         _[212], _[213], _[214], _[215], _[216], _[217], _[218], _[219], _[220], _[221], 
         _[222], _[223], _[224], _[225], _[226], _[227], _[228], _[229], _[230], _[231], 
         _[232], _[233], _[234], _[235], _[236], _[237], _[238], _[239], _[240], _[241], 
         _[242], _[243], _[244], _[245], _[246], _[247], _[248], _[249], _[250], _[251], 
         _[252], _[253], _[254], _[255], _[256], _[257], _[258], _[259], _[260], _[261], 
         _[262], _[263], _[264], _[265], _[266], _[267], _[268], _[269], _[270], _[271], 
         _[272], _[273], _[274], _[275], _[276], _[277], _[278], _[279], _[280], _[281], 
         _[282], _[283], _[284], _[285], _[286], _[287], _[288], _[289], _[290], _[291], 
         _[292], _[293], _[294], _[295], _[296], _[297], _[298], _[299], _[300], _[301], 
         _[302], _[303], _[304], _[305], _[306], _[307], _[308], _[309], _[310], _[311], 
         _[312], _[313], _[314], _[315], _[316], _[317], _[318], _[319], _[320], _[321], 
         _[322], _[323], _[324], _[325], _[326], _[327], _[328], _[329], _[330], _[331], 
         _[332], _[333], _[334], _[335], _[336], _[337], _[338], _[339], _[340], _[341], 
         _[342], _[343], _[344], _[345], _[346], _[347], _[348], _[349], _[350], _[351], 
         _[352], _[353], _[354], _[355], _[356], _[357], _[358], _[359], _[360], _[361], 
         _[362], _[363], _[364], _[365], _[366], _[367], _[368], _[369], _[370], _[371], 
         _[372], _[373], _[374], _[375], _[376], _[377], _[378], _[379], _[380], _[381], 
         _[382], _[383], _[384], _[385], _[386], _[387], _[388], _[389], _[390], _[391], 
         _[392], _[393], _[394], _[395], _[396], _[397], _[398], _[399], _[400], _[401], 
         _[402], _[403], _[404], _[405], _[406], _[407], _[408], _[409], _[410], _[411], 
         _[412], _[413], _[414], _[415], _[416], _[417], _[418], _[419], _[420], _[421], 
         _[422], _[423], _[424], _[425], _[426], _[427], _[428], _[429], _[430], _[431], 
         _[432], _[433], _[434], _[435], _[436], _[437], _[438], _[439], _[440], _[441], 
         _[442], _[443], _[444], _[445], _[446], _[447], _[448], _[449], _[450], _[451], 
         _[452], _[453], _[454], _[455], _[456], _[457], _[458], _[459], _[460], _[461], 
         _[462], _[463], _[464], _[465], _[466], _[467], _[468], _[469], _[470], _[471], 
         _[472], _[473], _[474], _[475], _[476], _[477], _[478], _[479], _[480], _[481], 
         _[482], _[483], _[484], _[485], _[486], _[487], _[488], _[489], _[490], _[491], 
         _[492], _[493], _[494], _[495], _[496], _[497], _[498], _[499], _[500], _[501], 
         _[502], _[503], _[504], _[505], _[506], _[507], _[508], _[509], _[510], _[511], 
         _[512], _[513], _[514], _[515], _[516], _[517], _[518], _[519], _[520], _[521], 
         _[522], _[523], _[524], _[525], _[526], _[527], _[528], _[529], _[530], _[531], 
         _[532], _[533], _[534], _[535], _[536], _[537], _[538], _[539], _[540], _[541], 
         _[542], _[543], _[544], _[545], _[546], _[547], _[548], _[549], _[550], _[551], 
         _[552], _[553], _[554], _[555], _[556], _[557], _[558], _[559], _[560], _[561], 
         _[562], _[563], _[564], _[565], _[566], _[567], _[568], _[569], _[570], _[571], 
         _[572], _[573], _[574], _[575], _[576], _[577], _[578], _[579], _[580], _[581], 
         _[582], _[583], _[584], _[585], _[586], _[587], _[588], _[589], _[590], _[591], 
         _[592], _[593], _[594], _[595], _[596], _[597], _[598], _[599], _[600], _[601], 
         _[602], _[603], _[604], _[605], _[606], _[607], _[608], _[609], _[610], _[611], 
         _[612], _[613], _[614], _[615], _[616], _[617], _[618], _[619], _[620], _[621], 
         _[622], _[623], _[624], _[625], _[626], _[627], _[628], _[629], _[630], _[631], 
         _[632], _[633], _[634], _[635], _[636], _[637], _[638], _[639], _[640], _[641], 
         _[642], _[643], _[644], _[645], _[646], _[647], _[648], _[649], _[650], _[651], 
         _[652], _[653], _[654], _[655], _[656], _[657], _[658], _[659], _[660], _[661], 
         _[662], _[663], _[664], _[665], _[666], _[667], _[668], _[669], _[670], _[671], 
         _[672], _[673], _[674], _[675], _[676], _[677], _[678], _[679], _[680], _[681], 
         _[682], _[683], _[684], _[685], _[686], _[687], _[688], _[689], _[690], _[691], 
         _[692], _[693], _[694], _[695], _[696], _[697], _[698], _[699], _[700], _[701], 
         _[702], _[703], _[704], _[705], _[706], _[707], _[708], _[709], _[710], _[711], 
         _[712], _[713], _[714], _[715], _[716], _[717], _[718], _[719], _[720], _[721], 
         _[722], _[723], _[724], _[725], _[726], _[727], _[728], _[729], _[730], _[731], 
         _[732], _[733], _[734], _[735], _[736], _[737], _[738], _[739], _[740], _[741], 
         _[742], _[743], _[744], _[745], _[746], _[747], _[748], _[749], _[750], _[751], 
         _[752], _[753], _[754], _[755], _[756], _[757], _[758], _[759], _[760], _[761], 
         _[762], _[763], _[764], _[765], _[766], _[767], _[768], _[769], _[770], _[771], 
         _[772], _[773], _[774], _[775], _[776], _[777], _[778], _[779], _[780], _[781], 
         _[782], _[783], _[784], _[785], _[786], _[787], _[788], _[789], _[790], _[791], 
         _[792], _[793], _[794], _[795], _[796], _[797], _[798], _[799], _[800], _[801], 
         _[802], _[803], _[804], _[805], _[806], _[807], _[808], _[809], _[810], _[811], 
         _[812], _[813], _[814], _[815], _[816], _[817], _[818], _[819], _[820], _[821], 
         _[822], _[823], _[824], _[825], _[826], _[827], _[828], _[829], _[830], _[831], 
         _[832], _[833], _[834], _[835], _[836], _[837], _[838], _[839], _[840], _[841], 
         _[842], _[843], _[844], _[845], _[846], _[847], _[848], _[849], _[850], _[851], 
         _[852], _[853], _[854], _[855], _[856], _[857], _[858], _[859], _[860], _[861], 
         _[862], _[863], _[864], _[865], _[866], _[867], _[868], _[869], _[870], _[871], 
         _[872], _[873], _[874], _[875], _[876], _[877], _[878], _[879], _[880], _[881], 
         _[882], _[883], _[884], _[885], _[886], _[887], _[888], _[889], _[890], _[891], 
         _[892], _[893], _[894], _[895], _[896], _[897], _[898], _[899], _[900], _[901], 
         _[902], _[903], _[904], _[905], _[906], _[907], _[908], _[909], _[910], _[911], 
         _[912], _[913], _[914], _[915], _[916], _[917], _[918], _[919], _[920], _[921], 
         _[922], _[923], _[924], _[925], _[926], _[927], _[928], _[929], _[930], _[931], 
         _[932], _[933], _[934], _[935], _[936], _[937], _[938], _[939], _[940], _[941], 
         _[942], _[943], _[944], _[945], _[946], _[947], _[948], _[949], _[950], _[951], 
         _[952], _[953], _[954], _[955], _[956], _[957], _[958], _[959], _[960], _[961], 
         _[962], _[963], _[964], _[965], _[966], _[967], _[968], _[969], _[970], _[971], 
         _[972], _[973], _[974], _[975], _[976], _[977], _[978], _[979], _[980], _[981], 
         _[982], _[983], _[984], _[985], _[986], _[987], _[988], _[989], _[990], _[991], 
         _[992], _[993], _[994], _[995], _[996], _[997], _[998], _[999], _[1000], _[1001], 
         _[1002], _[1003], _[1004], _[1005], _[1006], _[1007], _[1008], _[1009], _[1010], _[1011], 
         _[1012], _[1013], _[1014], _[1015], _[1016], _[1017], _[1018], _[1019], _[1020], _[1021], 
         _[1022], _[1023]);
}

/*******************************************************************************
* function LOINSE                                                              *
*******************************************************************************/
void genLOINSE(char *figname, ...)
{
va_list arg;
char *insname, *signame;
lofig_list  *ptfig;
char con[100], sig[100];
int icon, jcon, isig, jsig;
int inc, ins, nb, nbs;
ptype_list *consiglist = NULL, *pcon;
locon_list *locon;
chain_list *sigchain = NULL, *chain;
losig_list *ptsig;
chain_list *ptchain = NULL;


   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal LOINSE : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }

   va_start(arg, figname);
   ptfig = getlofig(figname, 'P');
   insname = namealloc(va_arg(arg, char *));
   if (hassep(insname)) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nIllegal LOINSE : the ", stderr);
      (void)fprintf(stderr,
                    "instance name `%s' contains an illegal character\n",
                    insname);
      EXIT(1);
   }

   while ((signame = va_arg(arg, char *)) != NULL) {  
   char  *s, *buffer = mbkstrdup(signame); /* I shall not alter user strings */

      signame = buffer;
      for (s = signame; *s != '\0'; s++)
         if (*s == ':') {
            for (s = signame; *s != '\0'; s++)
               if (*s == '[' || *s == ']')   
                  *s = ' ';
            break;
         }
      if (sscanf(signame, " %99s %11d:%11d => %99s %11d:%11d ", con, &icon, &jcon, sig, &isig, &jsig) == 6) {
         if (abs(icon - jcon) != abs(isig - jsig) || icon == jcon) {

            fprintf(stderr, "Not a valid range : %s[%d:%d] => %s[%d:%d]\n",
                     con, icon, jcon, sig, isig, jsig);
            EXIT(1);
         }
         inc = (icon > jcon) ? -1 : 1;
         ins = (isig > jsig) ? -1 : 1;
         for (nb = icon, nbs = isig; nb != (jcon + inc); nbs += ins, nb += inc)
            consiglist = addptype(consiglist,
                                    (long)vectorize(con, nb),
                                    vectorize(sig, nbs));
      } else if (sscanf(signame, " %99s => %99s ", con, sig) == 2)
         consiglist = addptype(consiglist, (long)namealloc(checkname(con)),
                                 namealloc(checkname(sig)));
      else {
         fprintf(stderr, "Illegal signal affectation syntax : %s\n", signame);
         EXIT(1);
      }
      mbkfree(signame);
   }

   /*  Put sigs and cons in dictionary  */
   for (pcon = consiglist; pcon; pcon = pcon->NEXT) {
      pcon->TYPE = (long)namealloc((char *)pcon->TYPE);
      pcon->DATA = (void *)namealloc((char *)pcon->DATA);
   }

   /*  Check Connectors name consistency  */
   for (locon = ptfig->LOCON; locon; locon = locon->NEXT) {
      for (pcon = consiglist; pcon; pcon = pcon->NEXT)
         if (locon->NAME == (char *)pcon->TYPE) {
            sigchain = addchain(sigchain, pcon->DATA);
            pcon->TYPE = 0L;
            break;
         }
      if (pcon == NULL) {
         fprintf(stderr, "No model connector %s for instance %s\n",
                           locon->NAME, insname);
         EXIT(1);
      }
   }

   for (pcon = consiglist; pcon; pcon = pcon->NEXT)
      if (pcon->TYPE != 0L) {
         fprintf(stderr, "Connector %s in instance %s do not exist in model",
                  (char *)pcon->TYPE, insname);
         fprintf(stderr, " in model %s\n", figname);
         EXIT(1);
      }

   /* adding the signals if needed */
   for (chain = sigchain; chain; chain = chain->NEXT) {
      signame = (char *)chain->DATA;
      for (ptsig = WORK_LOFIG->LOSIG; ptsig; ptsig = ptsig->NEXT) {
         for (sigchain = ptsig->NAMECHAIN; sigchain; sigchain = sigchain->NEXT)
            if (sigchain->DATA == (void *)signame)  
               break;
         if (sigchain != NULL)  
            break;
      }
      if (ptsig == NULL)
         ptsig = addlosig(WORK_LOFIG, ++num_index,
                  addchain((chain_list *)NULL, (void *)signame), 'I');
      ptchain = addchain(ptchain, (void *)ptsig);
   }
   {
   loins_list *ptins;
   ptype_list *pt;

      ptins = addloins(WORK_LOFIG, insname, ptfig, ptchain);
      pt = getptype(ptfig->USER, LOGEN);
      if (pt) {
         logen_list *g;
         ptins->USER = addptype(ptins->USER, LOGEN, NULL);
         for (g = pt->DATA; g; g = g->NEXT) {
            ptins->USER->DATA = (ptype_list *)duplogenlist(g, ptins->USER->DATA, g->NAME);
         }
      }
   }
   va_end(arg);
}

/*******************************************************************************
* function LOSIG                                                               *
*******************************************************************************/
void genLOSIG( char *signame )
{
losig_list *ptsig;
chain_list *ptchain, *ptchain1 = NULL;
char  *s;

   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal LOSIG : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }

   if (!is_bus(signame)) {
      s = namealloc(checkname(signame));
      for (ptsig = WORK_LOFIG->LOSIG; ptsig; ptsig = ptsig->NEXT) {
         for (ptchain = ptsig->NAMECHAIN; ptchain != NULL;
             ptchain = ptchain->NEXT)
            if (ptchain->DATA == (void *)s)
               break;
         if (ptchain != NULL)
            break;
      }
      if (ptsig == NULL) {
         num_index++;
         ptsig = addlosig(WORK_LOFIG, num_index,
                           addchain((chain_list *)NULL, (void *)s),
                           INTERNAL);
      }
   } else { /* expanding signal names */
      long  from, to, i;
      char  *zig, *newzig;

      s = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));
      (void)strcpy(s, signame);

      /* expanding signal names */
      zig = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));
      newzig = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));

      if (!bus_decod(s, zig, &from, &to)) {
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fprintf(stderr, "LOSIG : Bad signal bus name %s\n", s);
         EXIT(1);
      }
      mbkfree((void *)s);
      if (from > to) {
         for (i = from; i >= to; i--) {
            (void)sprintf(newzig, "%s %ld", zig, i);
            ptchain1 = addchain(ptchain1, (void *)namealloc(newzig));
         }
      } else {
         for (i = from; i <= to; i++) {
            (void)sprintf(newzig, "%s %ld", zig, i);
            ptchain1 = addchain(ptchain1, (void *)namealloc(newzig));
         }
      }
      mbkfree((void *)zig);
      mbkfree((void *)newzig);
      while (ptchain1 != NULL) {
         s = (char *)ptchain1->DATA;
         for (ptsig = WORK_LOFIG->LOSIG; ptsig; ptsig = ptsig->NEXT) {
            for (ptchain = ptsig->NAMECHAIN; ptchain
                  != NULL; ptchain = ptchain->NEXT)
               if (ptchain->DATA == (void *)s)
                  break;
            if (ptchain != NULL)
               break;
         }
         if (ptsig == NULL) {
            num_index++;
            ptsig = addlosig(WORK_LOFIG, num_index,
                           addchain((chain_list *)NULL, (void *)s), 
                           INTERNAL);
         }
         ptchain1 = ptchain1->NEXT;
      }
    //ptchain1 = (chain_list *)reverse(ptchain1);
   }
}

/*******************************************************************************
* function LOCON                                                               *
*******************************************************************************/
void genLOCON( char *conname, char direction, char *signame )
{
losig_list * ptsig;
chain_list * ptchain, *ptchain1 = NULL, *ptchain2 = NULL;
char  *s = NULL; /* To make -Wall happy */

   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal LOCON : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   /* one shall not deeclare connectors after instances */
   if (WORK_LOFIG->LOINS != NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal LOCON : ", stderr);
      (void)fputs("all LOCON must be defined before any LOINS\n", stderr);
      EXIT(1);
   }

   if (!is_bus(signame)) {
      s = namealloc(checkname(signame));

      /* Skip this connector and signal if it shall not be connected */
      if (!strcmp(s, UNCONNECTED)) {
         if (is_bus(checkname(conname))) {
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "Illegal LOCON %s: different ", conname);
            (void)fprintf(stderr, "number of connectors and signals %s\n",
                          signame);
            EXIT(1);
         }
         return;
      }
         
      for (ptsig = WORK_LOFIG->LOSIG; ptsig; ptsig = ptsig->NEXT) {
         for (ptchain = ptsig->NAMECHAIN; ptchain != NULL;
             ptchain = ptchain->NEXT)
            if (ptchain->DATA == (void *)s)
               break;
         if (ptchain != NULL)
            break;
      }
      if (ptsig == NULL) {
         num_index++;
         ptsig = addlosig(WORK_LOFIG, num_index,
                           addchain((chain_list *)NULL, (void *)s),
                           EXTERNAL);
      }
      (void)addlocon(WORK_LOFIG, checkname(conname), ptsig, direction);
   } else { /* expanding connector names */
      long  from, to, dc, ds, i;
      char  *gon, *newgon;
      char  *zig, *newzig;

      gon = (char *)mbkalloc((unsigned int)(strlen(conname) + 1));
      if (!bus_decod(conname, gon, &from, &to)) {
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fprintf(stderr, "LOCON : the connector is the alone whereas the signals are many %s\n", conname);
         EXIT(1);
      }
      /* Skip the unconnected connectors: shall I also check the
       * signals in order to ensure consistency of size ?
       * Probably, ... */
      if (!strcmp(gon, UNCONNECTED)) {
         mbkfree(gon);
         dc = from > to ? from - to : to - from;
         zig = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));
         if (!bus_decod(signame, zig, &from, &to)) {
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "LOCON %s : Bad signal bus name %s\n",
                          conname, signame);
            EXIT(1);
         }
         ds = from > to ? from - to : to - from;
         if (dc != ds) {
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "Illegal LOCON %s: different ", conname);
            (void)fprintf(stderr, "number of connectors and signals %s\n",
                          signame);
            EXIT(1);
         }
         return;
      }
      newgon = (char *)mbkalloc((unsigned int)(strlen(conname) + 1));
      if (from > to) {
         dc = from - to;
         for (i = from; i >= to; i--) {
            (void)sprintf(newgon, "%s %ld", gon, i);
            ptchain1 = addchain(ptchain1, (void *)namealloc(newgon));
         }
      } else {
         dc = to - from;
         for (i = from; i <= to; i++) {
            (void)sprintf(newgon, "%s %ld", gon, i);
            ptchain1 = addchain(ptchain1, (void *)namealloc(newgon));
         }
      }
      mbkfree((void *)gon);
      mbkfree((void *)newgon);
      /* expanding signal names */
      zig = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));
      newzig = (char *)mbkalloc((unsigned int)(strlen(signame) + 1));

      if (!bus_decod(signame, zig, &from, &to)) {
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fprintf(stderr, "LOCON : Bad signal bus name %s\n", s);
         EXIT(1);
      }
      if (from > to) {
         ds = from - to;
         for (i = from; i >= to; i--) {
            (void)sprintf(newzig, "%s %ld", zig, i);
            ptchain2 = addchain(ptchain2, (void *)namealloc(newzig));
         }
      } else {
         ds = to - from;
         for (i = from; i <= to; i++) {
            (void)sprintf(newzig, "%s %ld", zig, i);
            ptchain2 = addchain(ptchain2, (void *)namealloc(newzig));
         }
      }
      mbkfree((void *)zig);
      mbkfree((void *)newzig);
      if (dc != ds) {
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fputs(
               "Illegal LOCON : different number of signals and connectors\n",
               stderr);
         EXIT(1);
      }
      ptchain1 = (chain_list *)reverse(ptchain1);
      ptchain2 = (chain_list *)reverse(ptchain2);

      while (ptchain1 != NULL) {
         gon = (char *)ptchain1->DATA;
         s = (char *)ptchain2->DATA;
         for (ptsig = WORK_LOFIG->LOSIG; ptsig; ptsig = ptsig->NEXT) {
            for (ptchain = ptsig->NAMECHAIN; ptchain
                != NULL; ptchain = ptchain->NEXT)
               if (ptchain->DATA == (void *)s)
                  break;
            if (ptchain != NULL)
               break;
         }
         if (ptsig == NULL) {
            num_index++;
            ptsig = addlosig(WORK_LOFIG, num_index,
                              addchain((chain_list *)NULL, (void *)s),
                              EXTERNAL);
         }
         (void)addlocon(WORK_LOFIG, gon, ptsig, direction);
         ptchain1 = ptchain1->NEXT;
         ptchain2 = ptchain2->NEXT;
      }
   }
}

/*******************************************************************************
* function LOTRS                                                               *
*******************************************************************************/
void genLOTRS( char type, unsigned short width, unsigned short length, char *grid, char *source, char *drain )
{
int i = 0;
losig_list *s_grid = NULL, *s_source = NULL, *s_drain = NULL, *ptsig = NULL;
chain_list *ptchain;
char *signame = NULL; /* To make -Wall happy */

   for (i = 0; i < 3; i++) {
      switch (i) {
         case 0 :
            signame = namealloc(checkname(grid));
            break;
         case 1 :
            signame = namealloc(checkname(source));
            break;
         case 2 :
            signame = namealloc(checkname(drain));
            break;
      }
      for (ptsig = WORK_LOFIG->LOSIG; ptsig; ptsig = ptsig->NEXT) {
         for (ptchain = ptsig->NAMECHAIN; ptchain; ptchain = ptchain->NEXT)
            if (ptchain->DATA == (void *)signame)
               break;
         if (ptchain)
            break;
      }
      if (!ptsig) {
         num_index++;
         ptsig = addlosig(WORK_LOFIG, num_index,
                           addchain((chain_list *)NULL, (void *)signame),
                           INTERNAL);
      }
      switch (i) {
         case 0 :
            s_grid = ptsig;
            break;
         case 1 :
            s_source = ptsig;
            break;
         case 2 :
            s_drain = ptsig;
            break;
      }
   }
   (void)addlotrs(WORK_LOFIG, type, 0L, 0L, width, length, 0, 0, 0, 0,
                  s_grid, s_source, s_drain, NULL, NULL);
}

/*******************************************************************************
* function LOSIGMERGE                                                          *
*******************************************************************************/
void genLOSIGMERGE( char *sig1, char *sig2 )
{
losig_list *ls0, *ls1, *ls2;
locon_list *c;
loins_list *i;
lotrs_list *t;

locap_list *cap   = NULL  ;
lores_list *res   = NULL  ;
loself_list *self = NULL  ;

chain_list *chain;

   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal LOMERGE : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   sig1 = namealloc(checkname(sig1));
   sig2 = namealloc(checkname(sig2));
   for (ls1 = WORK_LOFIG->LOSIG; ls1; ls1 = ls1->NEXT) {
      for (chain = ls1->NAMECHAIN; chain; chain = chain->NEXT)
         if (chain->DATA == (void *)sig1)
            break;
      if (chain)
         break;
   }
   for (ls2 = WORK_LOFIG->LOSIG; ls2; ls2 = ls2->NEXT) {
      for (chain = ls2->NAMECHAIN; chain; chain = chain->NEXT)
         if (chain->DATA == (void *)sig2)
            break;
      if (chain)
         break;
   }
   if (ls1 == NULL && ls2 == NULL) { /* none of them exists ! */
      num_index++;
      chain = addchain((chain_list *)NULL, (void *)sig1);
      chain = addchain(chain, (void *)sig2);
      (void)addlosig(WORK_LOFIG, num_index, chain, INTERNAL);
      return;
   }
   if (ls1 == ls2) /* already joined!, just return */
      return;
   if (ls1 == NULL || ls2 == NULL) { /* one of them exists ! */
      if (ls1 == NULL)
         ls2->NAMECHAIN = addchain(ls2->NAMECHAIN, (void *)sig1);
      else
         ls1->NAMECHAIN = addchain(ls1->NAMECHAIN, (void *)sig2);
      return;
   }
   /* if both exists */
   if (ls2->TYPE == EXTERNAL) { /* delete ls1 */
      ls0 = ls2; /* swap ls1 and ls2 for deleting purpose */
      ls2 = ls1;
      ls1 = ls0;
   }
   for (chain = ls2->NAMECHAIN; chain; chain = chain->NEXT)
      ls1->NAMECHAIN = addchain(ls1->NAMECHAIN, chain->DATA);
   
   /* delete ls2, why not */
   for (c = WORK_LOFIG->LOCON; c; c = c->NEXT)
      if (c->SIG == ls2)
         c->SIG = ls1;
   for (i = WORK_LOFIG->LOINS; i; i = i->NEXT)
      for (c = i->LOCON; c; c = c->NEXT)
         if (c->SIG == ls2)
            c->SIG = ls1;
   for (t = WORK_LOFIG->LOTRS; t; t = t->NEXT) {
      if (t->GRID->SIG == ls2)
         t->GRID->SIG = ls1;
      if (t->DRAIN->SIG == ls2)
         t->DRAIN->SIG = ls1;
      if (t->SOURCE->SIG == ls2)
         t->SOURCE->SIG = ls1;
   }

   for(cap = WORK_LOFIG -> LOCAP ; cap != NULL ; cap = cap -> NEXT)
     {
       if(cap -> TCON -> SIG == ls2)
	 {
           cap -> TCON -> SIG = ls1 ;
	 }

      if(cap -> BCON -> SIG == ls2)
	{
          cap -> BCON ->SIG = ls1 ;
	}
     }

   for(res = WORK_LOFIG -> LORES ; res != NULL ; res = res -> NEXT)
     {
       if(res -> RCON1 -> SIG == ls2)
	 {
           res -> RCON1 -> SIG = ls1 ;
	 }

      if(res -> RCON2 -> SIG == ls2)
	{
          res -> RCON2 ->SIG = ls1 ;
	}
     }

   for(self = WORK_LOFIG -> LOSELF ; self != NULL ; self = self -> NEXT)
     {
       if(self -> SCON1 -> SIG == ls2)
	 {
           self -> SCON1 -> SIG = ls1 ;
	 }

      if(self -> SCON2 -> SIG == ls2)
	{
          self -> SCON2 ->SIG = ls1 ;
	}
     }

#if 0
   if (ls2->PRCN) {
      if (!ls1->PRCN)
        addlorcnet(ls1);
      (void)addcapa(ls1, getcapa(ls2));
   }
#endif
   (void)dellosig(WORK_LOFIG, ls2->INDEX);
}

/*******************************************************************************
* function LOGENLIST                                                               *
*******************************************************************************/
logen_list *genLOGENLIST(int type, ...)
{
va_list     ap;
char       *s, *t;
logen_list *g = NULL;

   va_start(ap, type);
   while (type != GENTYPE_EMPTY) {
      g = addlogen(g, NULL);
      g->TYPE = type;
      switch (type) {
         case GENTYPE_VAL:
         case GENTYPE_HEX:
            g->VALUE.VAL = va_arg(ap, long);
            break;
         case GENTYPE_TEXT:
            s = va_arg(ap, char *);
            t = malloc((strlen(s) + 3) * sizeof(char));
            sprintf(t, "\"%s\"", s);
            g->VALUE.TEXT = namealloc(t);
            free(t);
            break;
         case GENTYPE_BIT:
            g->VALUE.CHAR = '0' + (va_arg(ap, long)&1);
            break;
         case GENTYPE_LIST:
            g->VALUE.LIST = va_arg(ap, logen_list *);
            break;
         default:
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "Illegal LOGENLIST: unsupported generic type\n");
            EXIT(1);
      }
      type = va_arg(ap, int);
   }
   return (logen_list *)reverse((chain_list *)g);
}

/*******************************************************************************
* function LOGEN                                                               *
*******************************************************************************/
void genLOGEN(char *name, int type, ...)
{
ptype_list *p = getptype(WORK_LOFIG->USER, LOGEN);
va_list ap; /* We have a single argument, but of unknown type yet! */
char *s, *t;
logen_list *g;

   if (!p)
      p = WORK_LOFIG->USER = addptype(WORK_LOFIG->USER, LOGEN, NULL);

   p->DATA = g = addlogen(p->DATA, name);

   if (type < 0 || type > GENTYPE_MAX) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "Illegal LOGEN: unsupported generic type\n");
      EXIT(1);
   }
   g->TYPE = type;

   va_start(ap, type);
   switch (type) {
      case GENTYPE_VAL:
      case GENTYPE_HEX:
         g->VALUE.VAL = va_arg(ap, long);
         break;
      case GENTYPE_TEXT:
         s = va_arg(ap, char *);
         t = malloc((strlen(s) + 3) * sizeof(char));
         sprintf(t, "\"%s\"", s);
         g->VALUE.TEXT = namealloc(t);
         free(t);
         break;
      case GENTYPE_BIT:
         g->VALUE.CHAR =  '0' + (va_arg(ap, long)&1);
         break;
      case GENTYPE_LIST:
         g->VALUE.LIST = va_arg(ap, logen_list *);
         break;
      default:
          (void)fflush(stdout);
          (void)fputs("*** genlib error ***\n", stderr);
          (void)fprintf(stderr, "Illegal LOGEN: unsupported generic type\n");
          EXIT(1);
   }
   va_end(ap);
 //debuglogen(g, 0);
}

/*******************************************************************************
* function SETLOGEN                                                               *
*******************************************************************************/
void genSETLOGEN(char *instance, char *name, ...)
{
va_list ap; /* We have a single argument, but of unknown type yet! */
loins_list *i = getloins(WORK_LOFIG, instance);
ptype_list *p;
logen_list *g;
char       *s, *t;

   if (!i) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "Illegal SETLOGEN : no instance called '%s' \n",
                   instance);
      EXIT(1);
   }

   p = getptype(i->USER, LOGEN);
   if (!p) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "Illegal SETLOGEN : no logen in model '%s' \n",
                   i->FIGNAME);
      EXIT(1);
   }
   g = getlogen(p->DATA, name);
   if (!g) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "Illegal SETLOGEN : no generic called '%s' \n",
                   name);
      EXIT(1);
   }

   va_start(ap, name);
   switch (g->TYPE) {
      case GENTYPE_VAL:
      case GENTYPE_HEX:
         g->VALUE.VAL = va_arg(ap, long);
         break;
      case GENTYPE_TEXT:
         s = va_arg(ap, char *);
         t = malloc((strlen(s) + 3) * sizeof(char));
         sprintf(t, "\"%s\"", s);
         g->VALUE.TEXT = namealloc(t);
         free(t);
         break;
      case GENTYPE_BIT:
         g->VALUE.CHAR =  '0' + (va_arg(ap, long)&1);
         break;
      case GENTYPE_LIST:
         g->VALUE.LIST = va_arg(ap, logen_list *);
         break;
      default:
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fprintf(stderr, "Illegal SETLOGEN: unsupported generic type\n");
         EXIT(1);
   }
   va_end(ap);
}

/*******************************************************************************
* function FLATTEN_LOFIG                                                       *
*******************************************************************************/
void genFLATTEN_LOFIG( char *insname, char concat )
{
long i = 0;
losig_list *s;

   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal FLATTEN_LOFIG : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   flattenlofig(WORK_LOFIG, insname, concat);
   /* flattenlofig will create many signals, and genlib should be kept
      up to date with the last used index.*/
   for (s = WORK_LOFIG->LOSIG; s; s = s->NEXT)
      if (s->INDEX > i)
         i = s->INDEX;
   num_index = ++i;
}

/*******************************************************************************
* flattens FLATTEN_ALL_LOINS                                                   *
*******************************************************************************/
void genFLATTEN_ALL_LOINS( char concat, int catal )
{
chain_list *c, *namelist = NULL;
loins_list *l;
long i = 0;
losig_list *s;

   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal FLATTEN_ALL_LOINS : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }

   for (l = WORK_LOFIG->LOINS; l; l = l->NEXT)
      if (catal == NO || !incatalog(l->FIGNAME))
         namelist = addchain(namelist, (void *)l->INSNAME);

   for (c = namelist; c; c = c->NEXT)
      flattenlofig(WORK_LOFIG, (char *)c->DATA, concat);

   freechain(namelist);
   /* flattenlofig will create many signals, and genlib should be kept
      up to date with the last used index.*/
   for (s = WORK_LOFIG->LOSIG; s; s = s->NEXT)
      if (s->INDEX > i)
         i = s->INDEX;
   num_index = ++i;
}

/*******************************************************************************
* function UNFLATTEN_LOFIG                                                     *
*******************************************************************************/
void genUNFLATTEN_LOFIG(char *modelname, ...)
{
va_list instancelist;
char *instancename, *iname;
chain_list *ilist = NULL;

   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal UNFLATTEN_LOFIG : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   va_start(instancelist, modelname);
   if (modelname == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal UNFLATTEN_LOFIG : missing arguments\n", stderr);
      EXIT(1);
   };
   if ((instancename = va_arg(instancelist, char *)) == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal UNFLATTEN_LOFIG : only one arguments\n", stderr);
      EXIT(1);
   };
   while ((iname = va_arg(instancelist, char *)) != NULL)
      ilist = addchain(ilist, (void *)getloins(WORK_LOFIG, iname));
   /* this new hierarchy level needs to be saved */
   savelofig(unflattenlofig(WORK_LOFIG, modelname, instancename, ilist));
   freechain(ilist); 
   va_end(instancelist);
}

/*******************************************************************************
* standard-cell user functions                                                 *
*******************************************************************************/

/*******************************************************************************
* function SC_PLACE                                                            *
*******************************************************************************/
void genSC_PLACE( char  *insname, int   sym, long  x, long y )
{
loins_list * ptins;
char  *s = namealloc(insname);

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_PLACE impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_PLACE impossible : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   for (ptins = WORK_LOFIG->LOINS; ptins != NULL; ptins = ptins->NEXT)
      if (ptins->INSNAME == s)
         break;
   if (ptins == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr,
               "SC_PLACE impossible : no logical instance %s in figure %s\n",
               insname, WORK_LOFIG->NAME);
      EXIT(1);
   }
   genPLACE(ptins->FIGNAME, s, sym, x, y);
}

/*******************************************************************************
* function SC_RIGHT                                                            *
*******************************************************************************/
void genSC_RIGHT( char  *insname, int   sym )
{
loins_list * ptins;
char  *s = namealloc(insname);

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_RIGHT impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_RIGHT impossible : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   for (ptins = WORK_LOFIG->LOINS; ptins != NULL; ptins = ptins->NEXT)
      if (ptins->INSNAME == s)
         break;
   if (ptins == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr,
                  "SC_RIGHT impossible : no logical instance %s in figure %s\n",
                  insname, WORK_LOFIG->NAME);
      EXIT(1);
   }
   genPLACE_RIGHT(ptins->FIGNAME, s, sym);
}

/*******************************************************************************
* function SC_LEFT                                                             *
*******************************************************************************/
void genSC_LEFT( char  *insname, int   sym )
{
loins_list *ptins;
char *s = namealloc(insname);

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_LEFT impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_LEFT impossible : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   for (ptins = WORK_LOFIG->LOINS; ptins != NULL; ptins = ptins->NEXT)
      if (ptins->INSNAME == s)
         break;
   if (ptins == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "SC_LEFT impossible : no logical instance %s in figure %s\n", insname, WORK_LOFIG->NAME);
      EXIT(1);
   }
   genPLACE_LEFT(ptins->FIGNAME, s, sym);
}

/*******************************************************************************
* function SC_TOP                                                              *
*******************************************************************************/
void genSC_TOP( char *insname, int sym )
{
loins_list *ptins;
char *s = namealloc(insname);

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_TOP impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_TOP impossible : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   for (ptins = WORK_LOFIG->LOINS; ptins != NULL; ptins = ptins->NEXT)
      if (ptins->INSNAME == s)
         break;
   if (ptins == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "SC_TOP impossible : no logical instance %s in figure %s\n", insname, WORK_LOFIG->NAME);
      EXIT(1);
   }
   genPLACE_TOP(ptins->FIGNAME, s, sym);
}

/*******************************************************************************
* function SC_BOTTOM                                                           *
*******************************************************************************/
void genSC_BOTTOM( char *insname, int sym )
{
loins_list *ptins;
char *s = namealloc(insname);

   if (WORK_PHFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_BOTTOM impossible : missing DEF_PHFIG\n", stderr);
      EXIT(1);
   }
   if (WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_BOTTOM impossible : missing DEF_LOFIG\n", stderr);
      EXIT(1);
   }
   for (ptins = WORK_LOFIG->LOINS; ptins != NULL; ptins = ptins->NEXT)
      if (ptins->INSNAME == s)
         break;
   if (ptins == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("SC_BOTTOM impossible : no logical instance ", stderr);
      (void)fprintf(stderr, "%s in figure %s\n", insname, WORK_LOFIG->NAME);
      EXIT(1);
   }
   genPLACE_BOTTOM(ptins->FIGNAME, s, sym);
}

/*******************************************************************************
* BUS allows the definition of a bus for logical operations                    *
*******************************************************************************/
char *genBUS( char *signame, long from, long to )
{
char sigid[100];

   if (signame == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nBUS with NULL signal name", stderr);
      (void)fprintf(stderr, ", indexes [%ld:%ld]\n", from, to);
      EXIT(-1);
   }
   (void)sprintf(sigid, "%s[%ld:%ld]", signame, from, to);
   return mbkstrdup(sigid); /* must allocate memory for that */
}

/*******************************************************************************
* ELM allows the definition of a bus element for logical operations            *
*******************************************************************************/
char *genELM( char *signame, long number )
{
char sigid[100];

   if (signame == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\nELM with NULL signal name", stderr);
      (void)fprintf(stderr, ", index [%ld]\n", number);
      EXIT(-1);
   }
   (void)sprintf(sigid, "%s[%ld]", signame, number);
   return mbkstrdup(sigid); /* here also, allocation needed */
}
/* Build up a fake name that will be used to check if a signal is to
 * be connected or not.
 * Do not forget to free a vectorized string that comes out from this
 * function ! */

char *genUSED( char *s, int c )
  /* Czo */
{
char *t, *u;

   if ((t = strchr(s, '[')) == NULL)
      u = UNCONNECTED;
   else {
      u = malloc((strlen(UNCONNECTED) + strlen(t) + 1) * sizeof(char));
      sprintf(u, "%s%s", UNCONNECTED, t);
   }
   return c ? s : u;
}

/*******************************************************************************
* function DEF_PHSC                                                            *
* loads the net-list description of a cell and create what's necessary to      *
* physically describe it                                                       *
*******************************************************************************/
void genDEF_PHSC( char *name )
{
lofig_list *ptlofig;

   if (!CATA_LIB || !WORK_LIB)
      mbkenv();
   name = namealloc(name);
   for (ptlofig = HEAD_LOFIG; ptlofig; ptlofig = ptlofig->NEXT)
      if (ptlofig->NAME == name)
         break;
   if (ptlofig == NULL)
      WORK_LOFIG = getlofig(name, 'A');
   else
      WORK_LOFIG = ptlofig;

   lolist = addchain(lolist, WORK_LOFIG);

   genDEF_PHFIG(name);
}

/*******************************************************************************
* function SAVE_PHSC                                                           *
*******************************************************************************/
void genSAVE_PHSC()
{
loins_list *ptloins;
phins_list *ptphins;
chain_list *c, *loname = NULL, *phname = NULL;


   if (WORK_PHFIG == NULL || WORK_LOFIG == NULL) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fputs("Illegal SAVE_PHSC : missing DEF_PHSC\n", stderr);
      EXIT(1);
   }

   for (ptloins = WORK_LOFIG->LOINS; ptloins; ptloins = ptloins->NEXT) {
      for (ptphins = WORK_PHFIG->PHINS; ptphins; ptphins = ptphins->NEXT)
         if (ptloins->INSNAME == ptphins->INSNAME) {
            ptphins->USER = addptype(ptphins->USER, (long)CHECKFEED, (void *)0);
            break;
         }
      if(!ptphins) /* error : no physical instance called ptloins->INSNAME */
         loname = addchain(loname, (void *)ptloins->INSNAME);
   }

   for (ptphins = WORK_PHFIG->PHINS; ptphins; ptphins = ptphins->NEXT)
      if (!getptype(ptphins->USER, (long)CHECKFEED))
         if (!incatalogfeed(ptphins->FIGNAME)) /* error, not a feeding cell */
            phname = addchain(phname, (void *)ptphins->INSNAME);

   if (loname) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "Logical instance%s :\n", loname->NEXT ? "s" : "");
      for (c = loname; c; c = c->NEXT)
         (void)fprintf(stderr, "                      `%s'\n", (char *)c->DATA);
      (void)fprintf(stderr, "%s not physicaly placed\n",
                     loname->NEXT ? "are" : "is");
   }

   if (phname) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "Physical instance%s :\n", phname->NEXT ? "s" : "");
      for (c = phname; c; c = c->NEXT)
         (void)fprintf(stderr, "                      `%s'\n", (char *)c->DATA);
      (void)fprintf(stderr, "%s no logical equivalent and %s not %sfeed%s\n",
                     phname->NEXT ? "have" : "has",
                     phname->NEXT ? "are" : "is",
                     phname->NEXT ? "" : "a ",
                     phname->NEXT ? "s" : "");
   }

   if (loname || phname) { /* free before quiting */
      if (loname)
         freechain(loname);
      if (phname)
         freechain(phname);
      EXIT(1);
   }

   genDEF_AB(0L, 0L, 0L, 0L);
   savephfig(WORK_PHFIG);

   lolist = delchain(lolist, lolist);
   if (lolist)
      WORK_LOFIG = (lofig_list *)lolist->DATA;
   else
      WORK_LOFIG = NULL;
}

/*******************************************************************************
* function NAME                                                                *
*******************************************************************************/
char *genNAME(char *p, ...)
{
va_list stack;
char  *ret, *res;
long  strsize = 0;
char  piece[BUFSIZ], *s;
long  d;

   va_start(stack, p);
   ret = res = (char *)mbkalloc(BUFSIZ);
   while (*p != '\0') {
      if (*p != '%') {
         toolong(strsize ++);
         *res++ = *p++;
         continue;
      }
      switch (*++p) {
      case 'l':
         if (*++p != 'd') {
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "NAME : bad argument %%l%c\n", *p);
            EXIT(1);
         }

      case 'd':
         d = va_arg(stack, long);
         (void)sprintf(piece, "%ld", d);
         s = piece;
         while ((*res++ = *s++))
            toolong(strsize ++);
         res--;
         p++;
         break;

      case 'x':
         d = va_arg(stack, long);
         (void)sprintf(piece, "%lx", d);
         s = piece;
         while ((*res++ = *s++))
            toolong(strsize ++);
         res--;
         p++;
         break;

      case 's':
         s = va_arg(stack, char *);
         while ((*res++ = *s++))
            toolong(strsize ++);
         res--;
         p++;
         break;

      case 'c':
         *res++ = va_arg(stack, int);
         p++;
         break;

      default :
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\nNAME allows", stderr);
         (void)fprintf(stderr, " only %%d, %%c, %%x & %%s typed args\n");
         EXIT(1);
      }
   }
   *res = '\0';
   va_end(stack);
   return ret;
}

/*******************************************************************************
* internal use functions                                                       *
*******************************************************************************/
/*******************************************************************************
* function is_bus                                                              *
* check the validity of the syntax of a bussed signal                          *
* a bus is "abdc[123]" or "abcd[12:34]"                                        *
* something like "abcd[efgh]" will exit on a syntax error                      *
* return 1 if "abcd[23:45]" 0 if "abcd" or "abdc[12]"                          *
*******************************************************************************/
int is_bus( char *signame )
{
char *t = signame, c;
char is_a_bus = 0, flag = 0;
char i, where = 0; /* used to say exactly where */

   while ((c = *t++)) {
      where++; /* increment the position to know where if an error occurs */
      if (c == '[') { /* To make -Wall happy */
         if (!flag) {
            flag = 1;
            continue; /* do not treat '[' as a part of the bus */
         } else {
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "signal name %s is not legal\n", signame);
            for (i = 0; i < where + strlen("signal name"); i++)
               (void)fputs("_", stderr);
            (void)fputs("?\n", stderr);
            EXIT(1);
         }
      }
      if ((c == ':' || c == ']') && !flag) {
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fprintf(stderr, "signal name %s is not legal\n", signame);
         for (i = 0; i < where + strlen("signal name"); i++)
            (void)fputs("_", stderr);
         (void)fputs("?\n", stderr);
         EXIT(1);
      }
      if (flag) {
         if (c == ':') {
            is_a_bus = 1; /* that's a bus, not an element */
            continue; /* do not read the dots but still check for digit */
         }
         if (c == ']') {
            flag = 0; /* do not check for digit anymore */
            continue;
         }
         if (!isdigit(c)) {
            (void)fflush(stdout);
            (void)fputs("*** genlib error ***\n", stderr);
            (void)fprintf(stderr, "signal name %s is not legal\n", signame);
            for (i = 0; i < where + strlen("signal name"); i++)
               (void)fputs("_", stderr);
            (void)fputs("?\n", stderr);
            EXIT(1);
         }
      }
   }
   return is_a_bus;
}

/*******************************************************************************
* function bus_decod                                                           *
* decods the bus name so buses can be used in LOINS & LOCON                    *
*******************************************************************************/
int bus_decod( char *busname, char *signame, long *first, long *last )
{
char  *space;
char *buffer = mbkstrdup(busname);

   if (!(space = strchr(buffer, '[')))
      return 0;
   *space = ' ';
   if (!(space = strchr(buffer, ':')))
      return 0;
   *space = ' ';
   if (!(space = strchr(buffer, ']')))
      return 0;
   *space = ' ';
   (void)sscanf(buffer, "%1024s %22ld %22ld", signame, first, last);
   mbkfree(buffer);
   return 1;
}

/*******************************************************************************
* function toolong                                                             *
* cheks the validity of the lentgh of a string                                 *
*******************************************************************************/
static void toolong( long length )
{
   if (length > BUFSIZ) {
      (void)fflush(stdout);
      (void)fputs("*** genlib error ***\n", stderr);
      (void)fprintf(stderr, "NAME string too long(%d)\n", (int)BUFSIZ);
      EXIT(1);
   }
}

/*******************************************************************************
* checkname : check for busses with single index                               *
*******************************************************************************/
static char *checkname( char *name )
{
static char str[256];
char *s, *t;

   strcpy(str, name); /* do not alter user strings */
   s = t = str;
   while (*t) {
      if (isspace(*t)) {
         (void)fflush(stdout);
         (void)fputs("*** genlib error ***\n", stderr);
         (void)fprintf(stderr, "checkname failed : space detected in '%s'\n",
                        name);
         EXIT(1);
      }
      if (*t == '[')
         *t = ' ';
      else if (*t == ']') { /* To make -Wall happy */
         if (*(++t) == '\0') /* ok, it's finished */
            goto end;
         else if (*t == '[') /* multiple array */
            continue;
      }
      *s++ = *t++;
   }
end:
   *s = '\0';
   return str;
}

/*******************************************************************************
* hassep    : check for legal instance names                                   *
*******************************************************************************/
static int hassep( char *s )
{
   for (; *s != '\0'; s++)
      if (*s == '[' || *s == ']' ||
          *s == '(' || *s == ')' ||
          *s == '.' || *s == ' ')
         return 1;
   return 0;
}

/*******************************************************************************
* function LOCAP                                                               *
*******************************************************************************/

void genLOCAP(char type,double capa,char *tcon,char *bcon,char *name)
{
  int        i         = 0    ;
  losig_list *s_tcon   = NULL ;
  losig_list *s_bcon   = NULL ;
  losig_list *ptsig    = NULL ;
  chain_list *ptchain  = NULL ;
  char       *signame  = NULL ;

  for(i = 0 ; i < 2 ; i++)
    {
      switch(i)
        {
	case 0 :
	  signame = namealloc(checkname(tcon)) ;
	  break ;
	case 1 :
	  signame = namealloc(checkname(bcon)) ;
	  break ;
	}

      for(ptsig = WORK_LOFIG -> LOSIG; ptsig; ptsig = ptsig -> NEXT)
	{
	  for(ptchain = ptsig -> NAMECHAIN; ptchain; ptchain = ptchain -> NEXT)
	    {
	      if(ptchain -> DATA == (void *)signame)
	        break ;
	    }

	  if (ptchain)
	    break ;
	}

      if(!ptsig)
	{
	  num_index++ ;

	  ptsig = addlosig(WORK_LOFIG,num_index,addchain((chain_list *)NULL,
                           (void *)signame),INTERNAL) ;
	}

      switch (i)
	{
	case 0 :
	  s_tcon = ptsig ;
	  break ;
	case 1 :
	  s_bcon = ptsig ;
	  break ;
	}
    }

  (void)addlocap(WORK_LOFIG,type,capa,s_tcon,s_bcon,name) ;
}

/*******************************************************************************
* function genSET_LOCAP                                                               *
*******************************************************************************/

void genSET_LOCAP(char *figname,char *capname,double newcap)
{
  setlocap(getlofig(figname,'A'),capname,newcap) ;
}

/*******************************************************************************
* function LORES                                                               *
*******************************************************************************/

void genLORES(char type,double resi,char *rcon1,char *rcon2,char *name)
{
  int        i         = 0    ;
  losig_list *s_rcon1  = NULL ;
  losig_list *s_rcon2  = NULL ;
  losig_list *ptsig    = NULL ;
  chain_list *ptchain  = NULL ;
  char       *signame  = NULL ;

  for(i = 0 ; i < 2 ; i++)
    {
      switch(i)
        {
	case 0 :
	  signame = namealloc(checkname(rcon1)) ;
	  break ;
	case 1 :
	  signame = namealloc(checkname(rcon2)) ;
	  break ;
	}

      for(ptsig = WORK_LOFIG -> LOSIG; ptsig; ptsig = ptsig -> NEXT)
	{
	  for(ptchain = ptsig -> NAMECHAIN; ptchain; ptchain = ptchain -> NEXT)
	    {
	      if(ptchain -> DATA == (void *)signame)
	        break ;
	    }

	  if (ptchain)
	    break ;
	}

      if(!ptsig)
	{
	  num_index++ ;

	  ptsig = addlosig(WORK_LOFIG,num_index,addchain((chain_list *)NULL,
                           (void *)signame),INTERNAL) ;
	}

      switch (i)
	{
	case 0 :
	  s_rcon1 = ptsig ;
	  break ;
	case 1 :
	  s_rcon2 = ptsig ;
	  break ;
	}
    }

  (void)addlores(WORK_LOFIG,type,resi,s_rcon1,s_rcon2,name) ;
}

/*******************************************************************************
* function genSET_LORES                                                               *
*******************************************************************************/

void genSET_LORES(char *figname,char *resname,double newres)
{
  setlores(getlofig(figname,'A'),resname,newres) ;
}

/*******************************************************************************
* function LOSELF                                                               *
*******************************************************************************/

void genLOSELF(char type,double self,char *scon1,char *scon2,char *name)
{
  int        i         = 0    ;
  losig_list *s_scon1  = NULL ;
  losig_list *s_scon2  = NULL ;
  losig_list *ptsig    = NULL ;
  chain_list *ptchain  = NULL ;
  char       *signame  = NULL ;

  for(i = 0 ; i < 2 ; i++)
    {
      switch(i)
        {
	case 0 :
	  signame = namealloc(checkname(scon1)) ;
	  break ;
	case 1 :
	  signame = namealloc(checkname(scon2)) ;
	  break ;
	}

      for(ptsig = WORK_LOFIG -> LOSIG; ptsig; ptsig = ptsig -> NEXT)
	{
	  for(ptchain = ptsig -> NAMECHAIN; ptchain; ptchain = ptchain -> NEXT)
	    {
	      if(ptchain -> DATA == (void *)signame)
	        break ;
	    }

	  if (ptchain)
	    break ;
	}

      if(!ptsig)
	{
	  num_index++ ;

	  ptsig = addlosig(WORK_LOFIG,num_index,addchain((chain_list *)NULL,
                           (void *)signame),INTERNAL) ;
	}

      switch (i)
	{
	case 0 :
	  s_scon1 = ptsig ;
	  break ;
	case 1 :
	  s_scon2 = ptsig ;
	  break ;
	}
    }

  (void)addloself(WORK_LOFIG,type,self,s_scon1,s_scon2,name) ;
}

/*******************************************************************************
* function genSET_LOSELF                                                               *
*******************************************************************************/

void genSET_LOSELF(char *figname,char *selfname,double newself)
{
  setloself(getlofig(figname,'A'),selfname,newself) ;
}
