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
 * Purpose : access functions to physical structures
 * Date    : 06/03/92
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 */

#ident "$Id: mbk_ph.c,v 1.2 2002/09/30 16:20:49 czo Exp $"

/*
 * $Log: mbk_ph.c,v $
 * Revision 1.2  2002/09/30 16:20:49  czo
 * support/users
 *
 * Revision 1.1.1.1  2002/03/08 13:51:05  fred
 * Importing MBK sources into the new CVS tree
 *
 * Revision 1.12  2001/02/07 11:47:40  fred
 * Quite a few changes:
 *   Four new layers in the physical view: N/P TRANS FAST/HVIO
 *   A new field in the lotrs: char *MODELNAME that's the name of the
 *   spice model (in spice terminology) of the transistor.
 *   This field is supposed to be filled at extraction time and used at
 *   netlist generation time, by the techo dependant formats (ie spice)
 *
 *   This implies recompilation of everything:) Be my guest
 *
 * Revision 1.11  2000/11/15 15:02:07  syf
 * invxyflat ...
 *
 * Revision 1.10  2000/11/07 16:15:10  czo
 * correction des #ident avec ; en trop
 *
 * Revision 1.9  2000/11/06 14:10:39  fred
 * Add a test on name in addphvia to allow vias without names (NULL)
 *
 * Revision 1.8  2000/09/01 23:26:02  jpc
 * * Alors Fred, on fait du sabotage?
 *   - Le bon test pour detecter si un layer est en CALU c'est :
 *
 *              ((layer >= CALU1) && (layer <= CALU9))
 *                                ^^
 *
 *     Ca a du foutre un de ces bordels dans les programmes compiles
 *     aujourd'hui : on pouvait plus faire d'addphseg.
 *
 *   - Y'a aussi une merde dans le rflattenphfig, mais il est temps de faire
 *     dodo.
 *
 * Revision 1.7  2000/08/31 15:29:15  fred
 * Checks that a CLAYER has a name when creating a segment.
 *
 * Revision 1.6  2000/04/13 14:18:29  gregoire
 * Plein de petites choses :
 *
 *   Ajout de fonctions de libération des ressources utilisées.
 *   Traçage des chain_list pour détecter les blocs inutilisés.
 *   Debuggage de flatlofig.
 *
 * Revision 1.5  2000/02/22 13:58:15  syf
 * ATTENTION GROSSE MODIF  !!!
 * les phvias possedent maintenant un champ NAME !!!
 *
 * Revision 1.4  2000/02/04 13:03:11  syf
 * Ajout de nouveaux layers   : CALU1...CALU9
 * Ajout de nouveaux contacts : TURN1...TURN9
 *
 * Revision 1.3  2000/01/12 09:55:24  syf
 * Attention !!
 * -> deux nouveaux champs dans la structure de donnee
 *    phvia, DX et DY.
 * -> deux nouveaux parametres dans la fonction addphvia
 *
 * Revision 1.2  1998/12/04 14:38:20  fred
 * Added the layer info to addphcon output in verbose mode
 *
 * Revision 1.1.1.1  1998/10/02 15:27:10  alliance
 * Imported by czo
 *
 * Revision 7.12  1998/06/15 16:44:56  mbk
 * greg
 *
 * Revision 7.11  1998/04/06  10:06:58  mbk
 * Czo : headers, small modifs
 *
 * Revision 7.11  1998/04/06 10:06:58  mbk
 * Czo : headers, small modifs
 *
 *
 * Revision 7.8  1997/11/05 14:45:15  mbk
 * Slight modification in the index computation, that seems to be buggy
 *
 * Revision 7.7  1997/07/30 07:41:24  mbk
 * Added the POLY2 layer and CONT_POLY2 contact
 * Now accepts connectors inside the abutement box
 * Changes the way the indexes are computed, in order to cope with the
 * connectors that are inside the abutment box
 *
 */

#ident "$Id: mbk_ph.c,v 1.2 2002/09/30 16:20:49 czo Exp $"

#include <unistd.h>
#include <string.h>

#include "mut.h"
#include "mph.h"
#include "mbk_ph.h"

/*******************************************************************************
* global variable for physical view                                            *
*******************************************************************************/
phfig_list *HEAD_PHFIG;                        /* logical figure list head    */

/*******************************************************************************
* fonction addphfig                                                            *
*******************************************************************************/
phfig_list *addphfig(const char *name)
{
phfig_list *ptfig;
char *figname;

   figname = namealloc(name);

   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- addphfig  : %s\n", figname);

   /* scan figure_list */
   for (ptfig = HEAD_PHFIG; ptfig; ptfig = ptfig->NEXT)
      if (ptfig->NAME == figname)
         break;

   if (ptfig != NULL) {  /* figure exists */
      (void)fflush(stdout);
      (void)fprintf(stderr, "\n*** mbk error *** illegal addphfig\n");
      (void)fprintf(stderr, "figure %s already exists\n", figname);
      EXIT(1);
   }
   ptfig = (phfig_list *)mbkalloc(sizeof(phfig_list));
   ptfig->MODE       = 'A';
   ptfig->NAME       = figname;
   ptfig->MODELCHAIN = NULL;
   ptfig->PHSEG      = NULL;
   ptfig->PHCON      = NULL;
   ptfig->PHINS      = NULL;
   ptfig->PHVIA      = NULL;
   ptfig->PHREF      = NULL;
   ptfig->XAB1       = 0;
   ptfig->YAB1       = 0;
   ptfig->XAB2       = 0;
   ptfig->YAB2       = 0;
   ptfig->USER       = NULL;
   ptfig->NEXT       = HEAD_PHFIG;
   HEAD_PHFIG = ptfig;

   return ptfig;
}

/*******************************************************************************
* fonction addphins                                                            *
*******************************************************************************/
phins_list *addphins(phfig_list *ptfig, const char *fname, const char *iname, char sym, long x, long y)
{
phins_list *ptins;
chain_list *ptchain;
char *figname;
char *insname;

   insname = namealloc(iname);
   figname = namealloc(fname);

   /*  check  consistency    */
   if (FAST_MODE != 'Y' && strcmp(insname, "*")) {
      if (sym != NOSYM && sym != SYM_X && sym != SYM_Y && sym != SYMXY &&
            sym != ROT_P && sym != ROT_M && sym != SY_RP && sym != SY_RM) {
         (void)fflush(stdout);
         (void)fprintf(stderr, "\n*** mbk error *** illegal addphins ");
         (void)fprintf(stderr, "transformation %ld in : %s\n",
                        (long)sym, insname);
         EXIT(1);
      }

      /* check instance name unicity  */
      for (ptins = ptfig->PHINS; ptins; ptins = ptins->NEXT)
         if (ptins->INSNAME == insname)
            break;
      if (ptins != NULL) {
         (void)fflush(stdout);
         (void)fprintf(stderr, "\n*** mbk error *** illegal addphins\n");
         (void)fprintf(stderr, "duplicate instance name : %s\n", insname);
         EXIT(1);
      }
      if (ptfig->NAME == figname) {
         (void)fflush(stdout);
         (void)fprintf(stderr, "*** mbk error ***\naddphins\n");
         (void)fprintf(stderr, "figure %s cannot be part of itself\n", figname); 
         EXIT(1);
      }
   }

   /* update instance list  */
   ptins = (phins_list *)mbkalloc(sizeof(phins_list));
   ptins->INSNAME = insname;
   ptins->FIGNAME = figname;
   ptins->TRANSF  = sym;
   ptins->XINS    = x;
   ptins->YINS    = y;
   ptins->USER    = NULL;
   ptins->NEXT    = ptfig->PHINS;
   ptfig->PHINS   = ptins;

   /* update model list   */
   for (ptchain = ptfig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT)
      if (ptchain->DATA == (void *)figname)
         break;

   if (ptchain == NULL)
      ptfig->MODELCHAIN = addchain(ptfig->MODELCHAIN, (void *)figname);

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- addphins : ");
      (void)fprintf(stdout, "%s  x=%ld  y=%ld trans=%ld\n", insname, x, y, 
         (long)(ptins->TRANSF));
   }
   return ptins;
}

/*******************************************************************************
* fonction addphvia                                                            *
*******************************************************************************/
phvia_list *addphvia(phfig_list *ptfig, char viatype, long x, long y, long dx, long dy, const char *vname)
{
phvia_list *ptvia;
char *name;

   if (vname)
      name = namealloc(vname);
   else 
      name = NULL;
/*  check  consistency    */
   if (viatype < 0 || viatype > LAST_CONTACT) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\nillegal addphvia viatype ");
      (void)fprintf(stderr, ": %ld at x=%ld y=%ld\n", (long)viatype, x, y);
      EXIT(1);
   }

   /* update via list  */
   ptvia = (phvia_list *)mbkalloc(sizeof(phvia_list));
   ptvia->TYPE = viatype;
   ptvia->NAME = name;
   ptvia->XVIA = x;
   ptvia->YVIA = y;
   ptvia->DX   = dx;
   ptvia->DY   = dy;
   ptvia->USER = NULL;
   ptvia->NEXT = ptfig->PHVIA;
   ptfig->PHVIA = ptvia;

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- addphvia  : ");
      (void)fprintf(stdout, "type=%ld  x=%ld  y=%ld  dx=%ld  dy=%ld\n",
                    (long)viatype, x, y, dx, dy);
   }
   return ptvia;
}

/*******************************************************************************
* fonction addphref                                                            *
*******************************************************************************/
phref_list *addphref(phfig_list *ptfig, const char *rtype, const char *rname, long x, long y)
{
phref_list *ptref;
char *type;
char *name;

   name = namealloc(rname);
   type = namealloc(rtype);

   /* update ref list  */
   ptref = (phref_list *)mbkalloc(sizeof(phref_list));
   ptref->FIGNAME = type;
   ptref->NAME = name;
   ptref->XREF = x;
   ptref->YREF = y;
   ptref->USER = NULL;
   ptref->NEXT = ptfig->PHREF;
   ptfig->PHREF = ptref;

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- addphref  : ");
      (void)fprintf(stdout, "type %s name %s x=%ld  y=%ld \n", type, name, x, y);
   }
   return ptref;
}

/*******************************************************************************
* fonction addphseg                                                            *
*******************************************************************************/
phseg_list *addphseg(phfig_list *ptfig, char layer, long width, long x1, long y1, long x2, long y2, const char *sname)
{
phseg_list *ptseg;
char *nodename = namealloc(sname);

   if (layer < 0 || layer > LAST_LAYER) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\nillegal addphseg\n");
      (void)fprintf(stderr, "wrong layer code = %ld on segment ", (long)layer);
      (void)fprintf(stderr, "x1=%ld y1=%ld x2=%ld y2=%ld\n", x1, y1, x2, y2);
      EXIT(1);
   }

   if ((layer >= CALU1) && (layer <= CALU9))
      if (nodename == NULL || *nodename == (char)0 || !strcmp(nodename, "*")) {
         (void)fflush(stdout);
         (void)fprintf(stderr, "*** mbk error ***\nillegal addphseg\n");
         (void)fprintf(stderr, "Connection layer (%d) without a name\n",
                               layer);
         (void)fprintf(stderr, "x1=%ld y1=%ld x2=%ld y2=%ld\n", x1, y1, x2, y2);
         EXIT(1);
      }


   /*  update segment list  */
   ptseg = (phseg_list *)mbkalloc(sizeof(phseg_list));

   if (y1 == y2) {
      ptseg->Y1 = ptseg->Y2 = y1;
      if (x1 < x2) {
         ptseg->TYPE = RIGHT;
         ptseg->X1 = x1;
         ptseg->X2 = x2;
      } else {
         ptseg->TYPE = LEFT;
         ptseg->X1 = x2;
         ptseg->X2 = x1;
      }
   } else if (x1 == x2) {
      ptseg->X1 = ptseg->X2 = x1;
      if (y1 < y2) {
         ptseg->TYPE = UP;
         ptseg->Y1 = y1;
         ptseg->Y2 = y2;
      } else {
         ptseg->TYPE = DOWN;
         ptseg->Y1 = y2;
         ptseg->Y2 = y1;
      }
   } else {
      (void)fflush(stdout);
      (void)fprintf(stderr, "\n*** mbk error *** illegal addphseg\n");
      (void)fprintf(stderr, "x1=%ld  y1=%ld  x2=%ld  y2=%ld  layer=%ld\n", 
                     x1, y1, x2, y2, (long)layer);
      EXIT(1);
   }
   ptseg->LAYER = layer;
   ptseg->NAME = nodename;
   ptseg->WIDTH = width;
   ptseg->USER = NULL;
   ptseg->NEXT = ptfig->PHSEG;
   ptfig->PHSEG = ptseg;
   
   if (TRACE_MODE == 'Y') {
   (void)fprintf(stdout, "--- mbk --- addphseg  : ");
   if (nodename == NULL || *nodename == (char)0 || !strcmp(nodename, "*"))
      (void)printf("x1=%ld y1=%ld x2=%ld y2=%ld layer=%ld width=%ld\n", 
                     x1, y1, x2, y2, (long)layer, width);
   else 
      (void)printf("x1=%ld y1=%ld x2=%ld y2=%ld layer=%ld width=%ld name=%s\n",
                     x1, y1, x2, y2, (long)layer, width, nodename);
   }
   return ptseg;
}

/*******************************************************************************
* fonction addsorted                                                           *
*******************************************************************************/
static chain_list *addsorted(chain_list *pa, phcon_list *pco)
{
chain_list *ptc, *oldp;

   if (pa == NULL) /* first connector */
      return addchain((chain_list *)NULL, (void *)pco);
   /* insert in head */
   if (((phcon_list *)pa->DATA)->YCON > pco->YCON)
      return addchain(pa, (void *)pco);
   /* breaking ties */
   if (((phcon_list *)pa->DATA)->YCON == pco->YCON) {
      if (((phcon_list *)pa->DATA)->XCON > pco->XCON)
         return addchain(pa, (void *)pco);
      if (((phcon_list *)pa->DATA)->XCON == pco->XCON) {
         if (((phcon_list *)pa->DATA)->LAYER > pco->LAYER)
             return addchain(pa, (void *)pco);
         else if (((phcon_list *)pa->DATA)->LAYER < pco->LAYER) {
             pa->NEXT = addchain(pa->NEXT, (void *)pco);
             return pa;
         }
         if (((phcon_list *)pa->DATA)->ORIENT > pco->ORIENT)
             return addchain(pa, (void *)pco);
         pa->NEXT = addchain(pa->NEXT, (void *)pco);
         return pa;
      }
   }
   for (ptc = pa->NEXT, oldp = pa; ptc != NULL; /* general case */
         oldp = ptc, ptc = ptc->NEXT) {
      if (((phcon_list *)ptc->DATA)->YCON > pco->YCON) {
         oldp->NEXT = addchain(ptc, (void *)pco);
         return pa;
      } 
      if (((phcon_list *)ptc->DATA)->YCON == pco->YCON) {
         if (((phcon_list *)ptc->DATA)->XCON > pco->XCON) {
            oldp->NEXT = addchain(ptc, (void *)pco);
            return pa;
         }
         if (((phcon_list *)ptc->DATA)->XCON == pco->XCON) {
            if (((phcon_list *)ptc->DATA)->LAYER > pco->LAYER) {
              oldp->NEXT = addchain(ptc, (void *)pco);
              return pa;
            } else if (((phcon_list *)ptc->DATA)->LAYER < pco->LAYER) {
              ptc->NEXT = addchain(ptc->NEXT, (void *)pco);
              return pa;
            }
            if (((phcon_list *)ptc->DATA)->ORIENT > pco->ORIENT) {
                oldp->NEXT = addchain(ptc, (void *)pco);
                return pa;
            }
            ptc->NEXT = addchain(ptc->NEXT, (void *)pco);
            return pa;
         }
      }
   }
   /* put it at the end */
   oldp->NEXT = addchain((chain_list *)NULL, (void *)pco);
   return pa;
}

/*******************************************************************************
* fonction addphcon                                                            *
*******************************************************************************/
phcon_list *addphcon(phfig_list *ptfig, char orient, const char *cname, long x, long y, char layer, long width)
{
phcon_list *ptcon, *ptscan;
chain_list *pt, *ptchain = NULL;
long index;
char *conname;

   conname = namealloc(cname);
   /* check consistency */
   if (layer < 0 || layer > LAST_LAYER) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "\n*** mbk error *** illegal addphcon\n");
      (void)fprintf(stderr, "figure : %s\n", ptfig->NAME);
      (void)fprintf(stderr, "code layer is %ld in %s\n", (long)layer, conname);
      EXIT(1);
   }
   if (orient != NORTH && orient != EAST && orient != SOUTH &&
       orient != WEST) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "\n*** mbk error *** illegal addphcon\n");
      (void)fprintf(stderr, "figure : %s\n", ptfig->NAME);
      (void)fprintf(stderr, "orientation is %ld in : %s\n", (long)orient, 
           conname);
      EXIT(1);
   }

#if 0
   if (x < ptfig->XAB1 || x > ptfig->XAB2 || y < ptfig->YAB1
       || y > ptfig->YAB2) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "\n*** mbk error ***    illegal addphcon\n");
      (void)fprintf(stderr, "connector %s (%ld, %ld) not in abutement box\n", 
           conname, x, y);
      EXIT(1);
   }
#endif

   /* update connector list */
   ptcon = (phcon_list *)mbkalloc(sizeof(phcon_list));
   ptcon->INDEX = 0;
   ptcon->XCON = x;
   ptcon->YCON = y;
   ptcon->WIDTH = width;
   ptcon->ORIENT = orient;
   ptcon->NAME = conname;
   ptcon->LAYER = layer;
   ptcon->USER = NULL;
   ptcon->NEXT = ptfig->PHCON;
   ptfig->PHCON = ptcon;

   /* update index list with topological sort */
   for (ptscan = ptfig->PHCON; ptscan; ptscan = ptscan->NEXT) {
      if (conname == ptscan->NAME) /* if such a connector already exists*/
         ptchain = addsorted(ptchain, ptscan);
   }
   for (pt = ptchain, index = 0; pt != NULL; index++, pt = pt->NEXT)
         ((phcon_list *)pt->DATA)->INDEX = index;
   freechain(ptchain);
 
   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- addphcon  : ");
      (void)fprintf(stdout, "%s.%ld x=%ld  y=%ld w=%ld layer=%d\n", conname, 
           index, x, y, width, layer);
   }
   return ptcon;
}

/*******************************************************************************
* fonction defab                                                               *
*******************************************************************************/
void defab(ptfig, x1, y1, x2, y2)
phfig_list    *ptfig;
long           x1, y1, x2, y2;
{
   ptfig->XAB1 = x1;
   ptfig->YAB1 = y1;
   ptfig->XAB2 = x2;
   ptfig->YAB2 = y2;

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- defab    : ");
      (void)fprintf(stdout, "%s x1= %ld y1=%ld x2=%ld y2=%ld\n",
                     ptfig->NAME, x1, y1, x2, y2);
   }
}

/*******************************************************************************
* fonction getphins                                                            *
*******************************************************************************/
phins_list *getphins(phfig_list *ptfig, const char *iname)
{
phins_list *ptins;
char *insname = namealloc(iname);
   for (ptins = ptfig->PHINS; ptins; ptins = ptins->NEXT)
      if (ptins->INSNAME == insname)
         return ptins;
   (void)fflush(stdout);
   (void)fprintf(stderr, "\n*** mbk error ***\nillegal getphins ");
   (void)fprintf(stderr, "instance %s doesn't exist in figure %s\n", 
                  insname, ptfig->NAME);
   EXIT(1);
   return NULL; /* never reached */
}

/*******************************************************************************
* fonction getphcon                                                            *
*******************************************************************************/
phcon_list *getphcon(phfig_list *ptfig, const char *cname, long index)
{
phcon_list *ptcon;
char *conname = namealloc(cname);
 
   if (ptfig->PHCON != NULL)
      for (ptcon = ptfig->PHCON; ptcon; ptcon = ptcon->NEXT)
         if (ptcon->NAME == conname && ptcon->INDEX == index)
            return ptcon;
 
   (void)fflush(stdout);
   (void)fprintf(stderr, "\n*** mbk error ***   illegal getphcon\n");
   (void)fprintf(stderr, "connector %s.%ld doesn't exist in figure %s\n", 
        conname, index, ptfig->NAME);
   EXIT(1);
   return NULL; /* never reached */
}

/*******************************************************************************
* fonction getphref                                                            *
*******************************************************************************/
phref_list *getphref(phfig_list *ptfig, const char *rname)
{
phref_list *ptref;
char *refname = namealloc(rname);

   for (ptref = ptfig->PHREF; ptref; ptref = ptref->NEXT)
      if (ptref->NAME == refname)
         return ptref;
   (void)fflush(stdout);
   (void)fprintf(stderr, "\n*** mbk error ***\nillegal getphref ");
   (void)fprintf(stderr, "reference %s doesn't exist in figure %s\n", 
                  refname, ptfig->NAME);
   EXIT(1);
   return NULL; /* never reached */
}

/*******************************************************************************
* fonction delphcon                                                            *
*******************************************************************************/
int delphcon(phfig_list *ptfig, phcon_list *ptdelcon)
{
phcon_list *ptsav;
phcon_list *ptcon, *ptscan;
chain_list *pt, *ptchain = NULL;
long index;
char  *conname;
 
   for (ptcon = ptfig->PHCON; ptcon; ptcon = ptcon->NEXT) {
      if (ptcon == ptdelcon)
         break;
      ptsav = ptcon;
   }
   if (ptcon == NULL)
      return 0;
   else if (ptcon == ptfig->PHCON) {
      ptfig->PHCON = ptcon->NEXT;
      conname = ptcon->NAME;
   } else {
      ptsav->NEXT = ptcon->NEXT;
      conname = ptcon->NAME;
   }
   /* update index list with topological sort */
   for (ptscan = ptfig->PHCON; ptscan; ptscan = ptscan->NEXT) {
      if (conname == ptscan->NAME) /* if such a connector already exists*/
         ptchain = addsorted(ptchain, ptscan);
   }
   for (pt = ptchain, index = 0; pt != NULL; index++, pt = pt->NEXT)
         ((phcon_list *)pt->DATA)->INDEX = index;
   freechain(ptchain);
 
   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- delphcon  : ");
      (void)fprintf(stdout, "%s.%ld\n", ptdelcon->NAME, ptdelcon->INDEX);
   }
   mbkfree((void *)ptdelcon);
   return 1;
}

/*******************************************************************************
* fonction delphseg                                                            *
*******************************************************************************/
int delphseg(phfig_list *ptfig, phseg_list *ptseg)
{
phseg_list *ptsav;
phseg_list *pttmp;

   for (pttmp = ptfig->PHSEG; pttmp; pttmp = pttmp->NEXT) {
      if (pttmp == ptseg)
         break;
      ptsav = pttmp;
   }

   if (pttmp == NULL)
      return 0;
   else if (ptseg == ptfig->PHSEG)
      ptfig->PHSEG = pttmp->NEXT;
   else
      ptsav->NEXT = pttmp->NEXT;

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- delphseg  : ");
      (void)fprintf(stdout, "%s \n", ptseg->NAME);
   }

   mbkfree((void *)ptseg);
   return 1;
}

/*******************************************************************************
* fonction delphvia                                                            *
*******************************************************************************/
int delphvia(phfig_list *ptfig, phvia_list *ptvia)
{
phvia_list *ptsav;
phvia_list *pttmp;

   for (pttmp = ptfig->PHVIA; pttmp; pttmp = pttmp->NEXT) {
      if (pttmp == ptvia)
         break;
      ptsav = pttmp;
   }

   if (pttmp == NULL)
      return 0;
   else if (ptvia == ptfig->PHVIA)
      ptfig->PHVIA = pttmp->NEXT;
   else
      ptsav->NEXT = pttmp->NEXT;

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- delphvia  : ");
      (void)fprintf(stdout, "%ld \n", (long)(ptvia->TYPE));
   }
   mbkfree((void *)ptvia);
   return 1;
}

/*******************************************************************************
* fonction delphref                                                            *
*******************************************************************************/
int delphref(phfig_list *ptfig, phref_list *ptref)
{
phref_list *ptsav;
phref_list *pttmp;

   for (pttmp = ptfig->PHREF; pttmp; pttmp = pttmp->NEXT) {
      if (pttmp == ptref)
         break;
      ptsav = pttmp;
   }

   if (pttmp == NULL)
      return 0;
   else if (ptref == ptfig->PHREF)
      ptfig->PHREF = pttmp->NEXT;
   else
      ptsav->NEXT = pttmp->NEXT;

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- delphref  : ");
      (void)fprintf(stdout, "%s \n", ptref->NAME);
   }
   mbkfree((void *)ptref); 
   return 1;
}

/*******************************************************************************
* fonction delphins                                                            *
*******************************************************************************/
int delphins(phfig_list *ptfig, const char *iname)
{
phins_list *ptsav;
phins_list *ptins;
chain_list *ptchain;
chain_list *pttmpchain;
char *figname;
char *insname = namealloc(iname);

   for (ptins = ptfig->PHINS; ptins; ptins = ptins->NEXT) {
      if (ptins->INSNAME == insname)
         break;
      ptsav = ptins;
   }

   if (ptins == NULL)
      return 0;
   else if (ptins == ptfig->PHINS) {
      figname = ptins->FIGNAME;
      ptfig->PHINS = ptins->NEXT;
   } else {
      figname = ptins->FIGNAME;
      ptsav->NEXT = ptins->NEXT;
   }

   mbkfree((void *)ptins);

   for (ptins = ptfig->PHINS; ptins; ptins = ptins->NEXT)
      if (ptins->FIGNAME == figname)
         break;
   if (ptins == NULL)   {
      for (ptchain = ptfig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT) {
         if (ptchain->DATA == (void *)figname)
            break;
         pttmpchain = ptchain;
      }
      if (ptchain ==  ptfig->MODELCHAIN && ptchain != NULL)
         ptfig->MODELCHAIN = ptchain ->NEXT;
      else if (ptchain != NULL)
         pttmpchain->NEXT = ptchain->NEXT;
   }
   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- delphins  : ");
      (void)fprintf(stdout, "%s of figure %s\n", insname, figname);
   }
   return 1;
}
 
/*******************************************************************************
* fonction delphfig                                                            *
*******************************************************************************/
int delphfig(const char *fname)
{
phfig_list *ptfig;
phcon_list *ptcon;
phseg_list *ptseg;
phvia_list *ptvia;
phins_list *ptins;
phref_list *ptref;
phfig_list *ptfig_sav;
phcon_list *ptcon_sav;
phseg_list *ptseg_sav;
phvia_list *ptvia_sav;
phins_list *ptins_sav;
phref_list *ptref_sav;
char       *name = namealloc(fname);

   for (ptfig = HEAD_PHFIG; ptfig; ptfig = ptfig->NEXT) {
      if (ptfig->NAME == name)
         break;
      ptfig_sav = ptfig;
   }

   if (ptfig == NULL)
      return 0;
   else if (ptfig == HEAD_PHFIG)
      HEAD_PHFIG = ptfig->NEXT;
   else
      ptfig_sav->NEXT = ptfig->NEXT;

   for (ptins = ptfig->PHINS; ptins; ptins = ptins_sav) {
      ptins_sav = ptins->NEXT;
      mbkfree((void *)ptins);
   }
   for (ptseg = ptfig->PHSEG; ptseg; ptseg = ptseg_sav) {
      ptseg_sav = ptseg->NEXT;
      mbkfree((void *)ptseg);
   }
   for (ptref = ptfig->PHREF; ptref; ptref = ptref_sav) {
      ptref_sav = ptref->NEXT;
      mbkfree((void *)ptref);
   }
   for (ptvia = ptfig->PHVIA; ptvia; ptvia = ptvia_sav) {
      ptvia_sav = ptvia->NEXT;
      mbkfree((void *)ptvia);
   }
   for (ptcon = ptfig->PHCON; ptcon; ptcon = ptcon_sav) {
      ptcon_sav = ptcon->NEXT;
      mbkfree((void *)ptcon);
   }

   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- delphfig  : %s\n", name);
   mbkfree((void *)ptfig);
   return 1;
}

/*******************************************************************************
* fonction xyflat                                                              *
*******************************************************************************/
void xyflat(xout, yout, x, y, xins, yins, x1, y1, x2, y2, trsf)
long *xout, *yout;
long x, y;
long xins, yins;
long x1, y1, x2, y2;
char trsf;
{
   switch (trsf) {
      case NOSYM :
         *xout = xins + x - x1;
         *yout = yins + y - y1;
         break;
      case SYM_X :
         *xout = xins - x + x2;
         *yout = yins + y - y1;
         break;
      case SYM_Y :
         *xout = xins + x - x1;
         *yout = yins - y + y2;
         break;
      case SYMXY :
         *xout = xins - x + x2;
         *yout = yins - y + y2;
         break;
      case ROT_P :
         *xout = xins - y + y2;
         *yout = yins + x - x1;
         break;
      case ROT_M :
         *xout = xins + y - y1;
         *yout = yins - x + x2;
         break;
      case SY_RP :
         *xout = xins + y - y1;
         *yout = yins + x - x1;
         break;
      case SY_RM :
         *xout = xins - y + y2;
         *yout = yins - x + x2;
      break;
   }
}

/*******************************************************************************
* fonction invxyflat                                                              *
*******************************************************************************/
void invxyflat(x, y, xout, yout, xins, yins, x1, y1, x2, y2, trsf)
long *x, *y;
long xout, yout;
long xins, yins;
long x1, y1, x2, y2;
char trsf;
{
   switch (trsf) {
      case NOSYM :
         *x =   xout - xins + x1;
         *y =   yout - yins + y1;
         break;
      case SYM_X :
         *x = - xout + xins + x2;
         *y =   yout - yins + y1;
         break;
      case SYM_Y :
         *x =   xout - xins + x1;
         *y = - yout + yins + y2;
         break;
      case SYMXY :
         *x = - xout + xins + x2;
         *y = - yout + yins + y2;
         break;
      case ROT_P :
         *y = - xout + xins + y2;
         *x =   yout - yins + x1;
         break;
      case ROT_M :
         *y =   xout - xins + y1;
         *x = - yout + yins + x2;
         break;
      case SY_RP :
         *y =   xout - xins + y1;
         *x =   yout - yins + x1;
         break;
      case SY_RM :
         *y = - xout + xins + y2;
         *x = - yout + yins + x2;
      break;
   }
}

/*******************************************************************************
* fonction viewph                                                            *
*                                                                             *
* display on screen the content of all physical figures                       *
*******************************************************************************/
static const char *layername[LAST_LAYER + 2] = {
   "NWELL",
   "PWELL",
   "NTIE",
   "PTIE",
   "NDIF",
   "PDIF",
   "NTRANS",
   "PTRANS",
   "NTRANS_FAST",
   "PTRANS_FAST",
   "NTRANS_HVIO",
   "PTRANS_HVIO",
   "POLY",
   "POLY2",
   "ALU1",
   "ALU2",
   "ALU3",
   "ALU4",
   "ALU5",
   "ALU6",
   "ALU7",
   "ALU8",
   "ALU9",
   "TPOLY",
   "TPOLY2",
   "TALU1",
   "TALU2",
   "TALU3",
   "TALU4",
   "TALU5",
   "TALU6",
   "TALU7",
   "TALU8",
   "TALU9",
   "CALU1",
   "CALU2",
   "CALU3",
   "CALU4",
   "CALU5",
   "CALU6",
   "CALU7",
   "CALU8",
   "CALU9",
   "Unknown"
};

static const char *vianame[LAST_CONTACT + 2] = {
   "CONT_POLY",
   "CONT_DIF_N",
   "CONT_DIF_P",
   "CONT_BODY_N",
   "CONT_BODY_P",
   "CONT_VIA",
   "CONT_VIA2",
   "CONT_VIA3",
   "CONT_VIA4",
   "CONT_VIA5",
   "CONT_VIA6",
   "CONT_VIA7",
   "CONT_VIA8",
   "CONT_POLY2",
   "C_X_N",
   "C_X_P",
   "CONT_TURN1",
   "CONT_TURN2",
   "CONT_TURN3",
   "CONT_TURN4",
   "CONT_TURN5",
   "CONT_TURN6",
   "CONT_TURN7",
   "CONT_TURN8",
   "CONT_TURN9",
   "Unknown"
};


void viewph()
{
phfig_list  *pt_fig     = NULL;

   if (HEAD_PHFIG == NULL)
      (void)fprintf(stderr, "\n!!! viewph : empty list of figures !!!\n");
   else {
      for (pt_fig = HEAD_PHFIG; pt_fig; pt_fig = pt_fig->NEXT)
         viewphfig(pt_fig);
   }
}

/************************************************************************
*  fonction viewphfig()                                               *
*                                                                      *
* display on screen the content of physical figure ptfig              *
************************************************************************/
void viewphfig(pt_fig)
phfig_list *pt_fig;
{
phcon_list *pt_con = NULL;
phins_list *pt_ins = NULL;
chain_list *pt_chain = NULL;
phseg_list *pt_seg = NULL;
phvia_list *pt_via = NULL;
phref_list *pt_ref = NULL;

   if (!pt_fig)
      return;

   if (pt_fig->NAME == NULL)
      (void)printf("!!! figure without name !!!\n");
   else
      (void)printf("\nfigure   : %-20s  mode : %c\n", pt_fig->NAME, pt_fig->MODE);

   (void)printf(" |---abutment box : %ld %ld %ld %ld\n", 
                  pt_fig->XAB1, pt_fig->YAB1, pt_fig->XAB2, pt_fig->YAB2);

   if (pt_fig->MODELCHAIN == NULL)
      (void)printf(" |---empty list of instance names\n");
   else {
      (void)printf(" |---models \n");
      for (pt_chain = pt_fig->MODELCHAIN; pt_chain != NULL; pt_chain = pt_chain->NEXT)
         (void)printf(" |      |---%s \n", (char *)pt_chain->DATA);
   }

   if (pt_fig->PHCON == NULL) 
      (void)printf(" |---empty list of connectors\n");      
   else 
      for (pt_con = pt_fig->PHCON; pt_con != NULL; pt_con = pt_con->NEXT)
         viewphcon(pt_con);

   if (pt_fig->PHINS == NULL)
      (void)printf(" |---empty list of instances\n");
   else 
      for (pt_ins = pt_fig->PHINS; pt_ins != NULL; pt_ins = pt_ins->NEXT)
         viewphins(pt_ins);

   if (pt_fig->PHSEG == NULL)
      (void)printf(" |---empty list of segments\n");
   else 
      for (pt_seg = pt_fig->PHSEG; pt_seg != NULL; pt_seg = pt_seg->NEXT)
         viewphseg(pt_seg);

   if (pt_fig->PHVIA == NULL)
      (void)printf(" |---empty list of vias\n");
   else 
      for (pt_via = pt_fig->PHVIA; pt_via != NULL; pt_via = pt_via->NEXT)
         viewphvia(pt_via);

   if (pt_fig->PHREF == NULL)
      (void)printf(" |---empty list of references\n");
   else 
      for (pt_ref = pt_fig->PHREF; pt_ref != NULL; pt_ref = pt_ref->NEXT)
          viewphref(pt_ref);

   if (pt_fig->USER != NULL)
      (void)printf(" |--- non empty USER field\n");
   (void)printf(" | \n");
}

/******************************************************************************/

void viewphcon(pt_con)
phcon_list  *pt_con; 
{ 
   if (pt_con == NULL) {
      (void)printf("!!! viewphcon : null connector pointer !!!\n");
      return;
   }
   if (pt_con->NAME == NULL)
      (void)printf(" |--- !!! connector without name !!!\n");
   else
      (void)printf(" |---connector : %s (%ld) \n", pt_con->NAME, pt_con->INDEX);

   (void)printf(" |       |---coordinates   : X = %ld, Y = %ld \n",
                                             pt_con->XCON, pt_con->YCON);
   (void)printf(" |       |---orientation   : %c \n", pt_con->ORIENT);
   (void)printf(" |       |---width         : %ld \n", pt_con->WIDTH);

   (void)printf(" |       |---layer         : %s\n", layername[(int)pt_con->LAYER]);
   if (pt_con->USER != NULL)
      (void)printf(" |       |---non empty USER field\n");
   (void)printf(" | \n");
}

/******************************************************************************/
void viewphseg(pt_seg)
phseg_list  *pt_seg;
{ 
   if (pt_seg == NULL) {
      (void)printf("!!! viewphseg : null segment pointer !!!\n");
      return;
   }

   if (pt_seg->NAME == NULL)
      (void)printf(" |--- !!! segment without name !!! \n");
   else
      (void)printf(" |---segment : %s \n", pt_seg->NAME);

   (void)printf(" |       |---type          : %c \n", pt_seg->TYPE);

   (void)printf(" |       |---coordinates   : X1 = %ld, Y1 = %ld, X2 = %ld, Y2 = %ld \n", pt_seg->X1, pt_seg->Y1, pt_seg->X2, pt_seg->Y2);

   (void)printf(" |       |---width         : %ld \n", pt_seg->WIDTH);

   (void)printf(" |       |---layer         : %s\n", layername[(int)pt_seg->LAYER]);
   (void)printf(" |       |---layer         ");
   if (pt_seg->USER != NULL)
      (void)printf(" |       |---non empty USER field\n");
   (void)printf(" | \n");
}

void viewphvia(pt_via) 
phvia_list *pt_via;
{
   if (pt_via == NULL) {
      (void)printf("!!! viewphvia : null via pointeur !!!\n");
      return;
   }
   (void)printf(" |---via \n");
   (void)printf(" |       |---type       :%s\n", vianame[(int)pt_via->TYPE]);
   (void)printf(" |       |---coordinates: X = %ld, Y = %ld, DX = %ld, DY = %ld\n",
                pt_via->XVIA, pt_via->YVIA, pt_via->DX, pt_via->DY);
   if (pt_via->USER != NULL)
      (void)printf(" |       |---non empty USER field\n");
   (void)printf(" | \n");
}

void viewphins(pt_ins)
phins_list *pt_ins;
{ 
   if (pt_ins == NULL) {
      (void)printf("!!! viewphins : null instance pointer !!! \n");
      return;
   }
   if (pt_ins->INSNAME == NULL)
      (void)printf(" |--- !!! instance without name !!! \n");
   else
      (void)printf(" |---instance   : %s \n", pt_ins->INSNAME);

   if (pt_ins->FIGNAME == NULL)
      (void)printf(" |      |--- !!! instance without model !!! \n");
   else
      (void)printf(" |      |---model        : %s \n", pt_ins->FIGNAME);

   (void)printf(" |      |---coordinates  : X = %ld Y = %ld \n",
                                          pt_ins->XINS, pt_ins->YINS);

   (void)printf(" |      |---operation    \n");
   switch(pt_ins->TRANSF) {
      case NOSYM :
         (void)printf(": NOSYM\n");
         break;
      case ROT_P :
         (void)printf(": ROT_P\n");
         break;
      case SYMXY :
         (void)printf(": SYMXY\n");
         break;
      case ROT_M :
         (void)printf(": ROT_M\n");
         break;
      case SYM_X :
         (void)printf(": SYM_X\n");
         break;
      case SYM_Y :
         (void)printf(": SYM_Y\n");
         break;
      case SY_RP :
         (void)printf(": SY_RP\n");
         break;
      case SY_RM :
         (void)printf(": SY_RM\n");
         break;
      default :
         (void)printf(": unknown\n");
      }

   if (pt_ins->USER != NULL)
      (void)printf(" |      |---non empty USER field\n");
   (void)printf(" | \n");
}

void viewphref(pt_ref) 
phref_list *pt_ref;
{
   if (pt_ref == NULL) {
      (void)printf("!!! viewphref : null reference pointer !!!\n");
      return;
   }
   (void)printf(" |---reference \n");
   (void)printf(" |       |---type       : %s\n", pt_ref->FIGNAME);
   (void)printf(" |       |---name       : %s\n", pt_ref->NAME);
   if (pt_ref->USER != NULL)
      (void)printf(" |      |---non empty USER field\n");
   (void)printf(" |\n");
}
