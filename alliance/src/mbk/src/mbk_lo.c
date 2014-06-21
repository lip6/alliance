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
 * Purpose : acces functions to logical structures
 * Date    : 30/04/91
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * Modified by <Gregoire.Avot@lip6.fr> 1997,98
 * Modified by pnt <Pierre.Nguyen-Tuong@lip6.fr> 2002
 */

#ident "$Id: mbk_lo.c,v 1.9 2012/05/14 14:20:23 alliance Exp $"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "mut.h"
#include "mlo.h"
#include "rcn.h"
#include "mbk_lo.h"

long NB_TAT_LOSIG = 0;
/*******************************************************************************
* global variable for netlist view                                             *
*******************************************************************************/
lofig_list *HEAD_LOFIG;            /* physical figure list head   */

/*******************************************************************************
* function addlofig()                                                          *
*******************************************************************************/
lofig_list *addlofig(const char *fname)
{
lofig_list *ptfig;
char *name;

   name = namealloc(fname);
   /* scan figure list */
   ptfig = HEAD_LOFIG;
   while (ptfig != NULL && ptfig->NAME != name)
      ptfig = ptfig->NEXT;

   if (ptfig != NULL) {   /* figure exists */
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "addlofig impossible : ");
      (void)fprintf(stderr, "figure %s already exists\n", name);
      EXIT(1);
   }
   ptfig = (lofig_list *)mbkalloc(sizeof(lofig_list));
   ptfig->MODE   = 'A';
   ptfig->NAME   = name;
   ptfig->MODELCHAIN  = NULL;
   ptfig->LOINS  = NULL;
   ptfig->LOTRS  = NULL;
   ptfig->LOCAP  = NULL;
   ptfig->LORES  = NULL;
   ptfig->LOSELF = NULL;
   ptfig->LOCON  = NULL;
   ptfig->LOSIG  = NULL;
   ptfig->BKSIG  = NULL;
   ptfig->USER   = NULL;
   ptfig->NEXT   = HEAD_LOFIG;
   HEAD_LOFIG = ptfig;

   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- addlofig  : %s\n", name);

   return ptfig;
}

/*******************************************************************************
* function addlomodel()                                                        *
* used by the parsers to construct a temporary model                           *
*******************************************************************************/
lofig_list *addlomodel(lofig_list *model, const char *fname)
{
lofig_list *fig;
char *name = namealloc(fname);

   /* check to see if the model has already been loaded */
   for (fig = model; fig && fig->NAME != name; fig = fig->NEXT);

   if (fig) { /* figure exists */
      (void)fflush(stdout);
        (void)fprintf(stderr, "*** mbk error ***\n");
        (void)fprintf(stderr, "addlomodel impossible : ");
        (void)fprintf(stderr, "model %s already exists \n", name);
        EXIT(1);
   }
     fig = (lofig_list *)mbkalloc(sizeof(lofig_list));
     fig->MODE = 'A';
     fig->NAME = name;
     fig->MODELCHAIN = NULL;
     fig->LOINS = NULL;
     fig->LOTRS = NULL;
     fig->LOCAP = NULL;
     fig->LORES = NULL;
     fig->LOSELF = NULL;
     fig->LOCON = NULL;
     fig->LOSIG = NULL;
     fig->BKSIG = NULL;
     fig->USER = NULL;
     fig->NEXT = model;
     model = fig;

   if (TRACE_MODE == 'Y')
        (void)fprintf(stdout, "--- mbk --- addlomodel  : %s\n", name);

   return  model;
}
  
/*******************************************************************************
* function addlotrs()                                                          *
*******************************************************************************/
lotrs_list *addlotrs(lofig_list *ptfig, char type, long x, long y, long width, long length, long ps, long pd, long xs, long xd, losig_list *ptgrid, losig_list *ptsource, losig_list *ptdrain, losig_list *ptbulk, const char *name)
{
static char *grid, *drain, *source, *bulk;
lotrs_list  *pttrs;
locon_list  *ptcon;

   if (!grid) {
      grid = namealloc("grid");
      drain = namealloc("drain");
      source = namealloc("source");
      bulk = namealloc("bulk");
   }

#if 0
   if (type != TRANSN && type != TRANSP) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "  illegal transistor type : %ld\n", (long)type);
      EXIT(1);
   }
#endif

   pttrs = (lotrs_list *)mbkalloc(sizeof(lotrs_list));
   pttrs->X    = x;
   pttrs->Y    = y;
   pttrs->WIDTH   = width;
   pttrs->LENGTH  = length;
   pttrs->PS   = ps;
   pttrs->PD   = pd;
   pttrs->XS   = xs;
   pttrs->XD   = xd;
   pttrs->TYPE   = type;
   pttrs->NEXT   = ptfig->LOTRS;
   ptfig->LOTRS   = pttrs;
   pttrs->USER   = NULL;

   ptcon = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon->NAME   = grid;
   ptcon->SIG    = ptgrid;
   ptcon->ROOT   = (void *)pttrs;
   ptcon->TYPE   = TRANSCV;
   ptcon->DIRECTION = INTERNAL;
   ptcon->USER   = NULL;
   ptcon->PNODE  = NULL;
   pttrs->GRID   = ptcon;
   
   ptcon = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon->NAME   = drain;
   ptcon->SIG    = ptdrain;
   ptcon->ROOT   = (void *)pttrs;
   ptcon->TYPE   = TRANSCV;
   ptcon->DIRECTION = INTERNAL;
   ptcon->USER   = NULL;
   ptcon->PNODE  = NULL;
   pttrs->DRAIN    = ptcon;
   
   ptcon = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon->NAME   = source;
   ptcon->SIG    = ptsource;
   ptcon->ROOT   = (void *)pttrs;
   ptcon->TYPE   = TRANSCV;
   ptcon->DIRECTION = INTERNAL;
   ptcon->USER   = NULL;
   ptcon->PNODE  = NULL;
   pttrs->SOURCE   = ptcon;
   
   ptcon = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon->NAME   = bulk;
   ptcon->SIG    = ptbulk;
   ptcon->ROOT   = (void *)pttrs;
   ptcon->TYPE   = TRANSCV;
   ptcon->DIRECTION = INTERNAL;
   ptcon->USER   = NULL;
   ptcon->PNODE  = NULL;
   pttrs->BULK   = ptcon;

   /* No check is done for transistor name unicity */
   pttrs->TRNAME = namealloc(name);

   if ((TRACE_MODE == 'Y'))
      (void)fprintf(stdout,
                     "--- mbk --- addlotrs  : %s  X=%ld, Y=%ld, W=%ld, L=%ld, PS = %ld, PD = %ld, XS = %ld, XD = %ld\n",
                     type == TRANSN ? "TRANSN" : "TRANSP", x, y, width, length,
                     ps, pd, xs, xd);

   return pttrs;
}

/*******************************************************************************
* function addloins()                                                          *
*******************************************************************************/
loins_list *addloins(lofig_list *ptfig, const char *iname, lofig_list *ptnewfig, chain_list *sigchain)
{
locon_list *ptcon = NULL;
locon_list *ptnewcon = NULL;
loins_list *ptins = NULL;
char *figname;
char *insname;
chain_list *ptchain;

   insname = namealloc(iname);
   figname = ptnewfig->NAME;

   /* check insname  unicity */
   if (strcmp(insname, "*") && FAST_MODE != 'Y') {
      for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT) {
         if (ptins->INSNAME == insname) {
            (void)fflush(stdout);
            (void)fprintf(stderr, "*** mbk error ***\n");
            (void)fprintf(stderr, "addloins impossible : ");
            (void)fprintf(stderr, "instance %s already exist in figure %s\n", 
                           insname, ptfig->NAME);
            EXIT(1);
         }
      }
   }

   if (ptfig->NAME == figname) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "addloins impossible :\n");
      (void)fprintf(stderr, "instance model is the figure %s itself\n", 
                     figname);
      EXIT(1);
   }

   ptins = (loins_list *)mbkalloc(sizeof(loins_list));

   ptins->INSNAME  = insname;
   ptins->FIGNAME  = figname;
   ptins->LOCON   = NULL;
   ptins->USER    = NULL;
   ptins->NEXT    = ptfig->LOINS;
   ptfig->LOINS   = ptins;

   /* update model list   */
   for (ptchain = ptfig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT)
      if (ptchain->DATA == (void *)figname)
         break;

   if (!ptchain)
      ptfig->MODELCHAIN = addchain(ptfig->MODELCHAIN, (void *)figname);

   for (ptcon = ptnewfig->LOCON; ptcon; ptcon = ptcon->NEXT) {
      if (sigchain == NULL) {
         (void)fflush(stdout);
         (void)fprintf(stderr, "*** mbk error ***\n");
         (void)fprintf(stderr, "addloins impossible : connector number ");
         (void)fprintf(stderr, "discrepancy between figure %s", 
                        ptnewfig->NAME);
         (void)fprintf(stderr, " and instance %s in figure %s\n",
                        insname, ptfig->NAME);
         EXIT(1);
      }
      ptnewcon = (locon_list *)mbkalloc(sizeof(locon_list));
      ptnewcon->NAME = ptcon->NAME;
      ptnewcon->DIRECTION = ptcon->DIRECTION;
      ptnewcon->TYPE = 'I';
      ptnewcon->SIG  = (losig_list *)sigchain->DATA;
      ptnewcon->ROOT = (void *)ptins;
      ptnewcon->USER = NULL;
      ptnewcon->PNODE= NULL;
      ptnewcon->NEXT = ptins->LOCON;
      ptins->LOCON  = ptnewcon;
      sigchain = sigchain->NEXT;
   }
   if (sigchain != NULL) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "addloins impossible : ");
      (void)fprintf(stderr, "connector number discrepancy between figure %s ", 
                     ptnewfig->NAME);
      (void)fprintf(stderr, "and instance %s in figure %s\n",
                     insname, ptfig->NAME);
      EXIT(1);
   }

   ptins->LOCON = (locon_list *)reverse((chain_list *)ptins->LOCON);

   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- addloins  : %s of figure %s\n", 
                     insname, figname);
   return ptins;
}

/*******************************************************************************
* function addlocon()                                                          *
*******************************************************************************/
locon_list *addlocon(lofig_list *ptfig, const char *cname, losig_list *ptsig, char dir)
{
locon_list *ptcon;
char *name = namealloc(cname);

   /* check name  unicity */
   if (FAST_MODE != 'Y') {
      for (ptcon = ptfig->LOCON; ptcon; ptcon = ptcon->NEXT) {
         if (ptcon->NAME == name) {
            (void)fflush(stdout);
            (void)fprintf(stderr, "*** mbk error ***\n");
            (void)fprintf(stderr, "addlocon impossible :\n");
            (void)fprintf(stderr, "connector %s already exists in figure %s\n", 
                           name, ptfig->NAME);
            EXIT(1);
         }
      }
      if (dir != 'I' && dir != 'O' && dir != 'X' && dir != 'B' 
            && dir != 'Z' && dir != 'T') {
         (void)fflush(stdout);
         (void)fprintf(stderr, "*** mbk error ***\n");
         (void)fprintf(stderr, "addlocon impossible :\n");
         (void)fprintf(stderr, "bad direction %c in figure %s\n",
                        dir, ptfig->NAME);
         EXIT(1);
      }
   }

   ptcon            = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon->NAME      = name;
   ptcon->TYPE      = 'E';
   ptcon->SIG       = ptsig;
   ptcon->ROOT      = (void *)ptfig;
   ptcon->DIRECTION = dir;
   ptcon->USER      = NULL;
   ptcon->NEXT      = ptfig->LOCON;
   ptcon->PNODE     = NULL;
   ptfig->LOCON     = ptcon;

   if (TRACE_MODE == 'Y')
        {
      if (ptsig!=NULL)
         (void)fprintf(stdout, "--- mbk --- addlocon  : %s linked to %ld\n", 
                        name, ptsig->INDEX);
      else
         (void)fprintf(stdout, "--- mbk --- addlocon  : %s  no signal\n", name);
        }

   return ptcon;
}

/*******************************************************************************
* function addlosig()                                                       *
*******************************************************************************/

losig_list *addlosig(lofig_list *ptfig, long index, chain_list *namechain, char type)
{
losig_list *ptsig;
losig_list *pthead;
ptype_list *pt;
chain_list *ptchain;
long high;
long low;
long i;
long sigsize;

        sigsize = getsigsize( ptfig );

   low = (index % sigsize);
   high = (index / sigsize);
   for (pt = ptfig->BKSIG; pt; pt = pt->NEXT)
      if (pt->TYPE == high)
         break;
   if (pt == NULL) {
      pthead = (losig_list *)mbkalloc(sigsize * sizeof(losig_list));
      ptsig = pthead;
      for (i = 0; i < sigsize; i++) {
         ptsig->INDEX = 0;
         ptsig++;
      }
      ptfig->BKSIG = (ptype_list *)addptype(ptfig->BKSIG,
                                             (long)high, (void *)pthead);
      pt = ptfig->BKSIG;
   }
   ptsig = (losig_list *)(pt->DATA) + low;

   /* check index  unicity */
   if (ptsig->INDEX != 0L) {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "addlosig impossible :\n");
      (void)fprintf(stderr, "signal %ld already exist in figure %s\n", 
                     index, ptfig->NAME);
      EXIT(1);
   }
   for (ptchain = namechain; ptchain; ptchain = ptchain->NEXT)
      ptchain->DATA = (void *)namealloc((char *)ptchain->DATA);

   ptsig->NAMECHAIN  = namechain;
   ptsig->INDEX      = index;
   ptsig->USER       = NULL;
   ptsig->PRCN       = NULL;
   ptsig->TYPE       = type;
   ptsig->NEXT       = ptfig->LOSIG;
   ptfig->LOSIG      = ptsig;

   if (TRACE_MODE == 'Y') {
      (void)fprintf(stdout, "--- mbk --- addlosig : %ld ", index);
      if (ptsig->NAMECHAIN)
         if (ptsig->NAMECHAIN->DATA)
            (void)fprintf(stdout, "name : %s",
                           (char *)((chain_list *)(ptsig->NAMECHAIN))->DATA);
      (void)fprintf(stdout, "\n");
   }
   
   return ptsig;
}

/*******************************************************************************
* function dellofig()                                                          *
*******************************************************************************/
int dellofig(const char *fname)
{
lofig_list *ptfig;
losig_list *ptsig;
loins_list *ptins;
locon_list *ptcon;
lotrs_list *pttrs;
locap_list *ptcap;
lores_list *ptres;
loself_list *ptself;
ptype_list *pt;
char       *name = namealloc(fname);

void *ptold=NULL, *ptt;

   for (ptfig = HEAD_LOFIG; ptfig; ptfig = ptfig->NEXT) {
      if (ptfig->NAME == name)
         break;
      ptold = (void *)ptfig;
   }

   if (ptfig == NULL)
      return 0;
   else if (ptfig == HEAD_LOFIG) 
      HEAD_LOFIG = ptfig->NEXT;
   else 
      ((lofig_list *)ptold)->NEXT = ptfig->NEXT;

  freectclist( ptfig, NULL );
   for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT)
  {
      freechain(ptsig->NAMECHAIN);
      if(ptsig->PRCN)
         freelorcnet(ptsig);
    dellosiguser( ptsig );
   }
   
   for (ptins = ptfig->LOINS; ptins; ptins = (loins_list *)ptold) {
    delloinsuser( ptins );
      for (ptcon = ptins->LOCON; ptcon; ptcon = (locon_list *)ptt) {
         ptt = (void *)ptcon->NEXT;
      delloconuser( ptcon );
      mbkfree((void *)ptcon);
    }
     ptold = (void *)ptins->NEXT;
     mbkfree((void *)ptins);
   }

   for (pt = ptfig->BKSIG; pt; pt = pt->NEXT) {
      mbkfree((void *)pt->DATA);
   }
   freeptype(ptfig->BKSIG);
   for (ptcon = ptfig->LOCON; ptcon; ptcon = (locon_list *)ptold) {
      ptold = (void *)ptcon->NEXT;
     delloconuser( ptcon );
      mbkfree((void *)ptcon);
   }
   for (pttrs = ptfig->LOTRS; pttrs; pttrs = (lotrs_list *)ptold) {
    delloconuser( pttrs->GRID );
    delloconuser( pttrs->SOURCE );
    delloconuser( pttrs->DRAIN );
    delloconuser( pttrs->BULK );
      mbkfree((void *)pttrs->GRID);
      mbkfree((void *)pttrs->SOURCE);
      mbkfree((void *)pttrs->DRAIN);
      mbkfree((void *)pttrs->BULK);
      ptold = (void *)pttrs->NEXT;
    dellotrsuser( pttrs );
      mbkfree((void *)pttrs);
   }

   for(ptcap = ptfig -> LOCAP ; ptcap != NULL ; ptcap = (locap_list *)ptold)
      {
        delloconuser(ptcap -> TCON) ;
        delloconuser(ptcap -> BCON) ;

        mbkfree((void *)ptcap -> TCON) ;
        mbkfree((void *)ptcap -> BCON) ;
        ptold = (void *)ptcap -> NEXT ;

        dellocapuser(ptcap) ;
        mbkfree((void *)ptcap) ;
      }

   for(ptres = ptfig -> LORES ; ptres != NULL ; ptres = (lores_list *)ptold)
      {
        delloconuser(ptres -> RCON1) ;
        delloconuser(ptres -> RCON2) ;

        mbkfree((void *)ptres -> RCON1) ;
        mbkfree((void *)ptres -> RCON2) ;
        ptold = (void *)ptres -> NEXT ;

        delloresuser(ptres) ;
        mbkfree((void *)ptres) ;
      }

   for(ptself = ptfig -> LOSELF ; ptself != NULL ; ptself = (loself_list *)ptold)
      {
        delloconuser(ptself -> SCON1) ;
        delloconuser(ptself -> SCON2) ;

        mbkfree((void *)ptself -> SCON1) ;
        mbkfree((void *)ptself -> SCON2) ;
        ptold = (void *)ptself -> NEXT ;

        delloselfuser(ptself) ;
        mbkfree((void *)ptself) ;
      }

  dellofiguser( ptfig );
   freechain(ptfig->MODELCHAIN);
   mbkfree((void *)ptfig);
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- dellofig  : %s\n", name);
   return 1;
}

/*******************************************************************************
* function  freelomodel()                                                      *
*******************************************************************************/
void freelomodel(lofig_list *ptmodel)
{
lofig_list *ptfig;
locon_list *ptcon;
locon_list *ptc;

   if (!ptmodel) /* nothing to be freed */
      return;

   ptfig = ptmodel->NEXT;
   while (1) { 
      for (ptcon = ptmodel->LOCON; ptcon; ptcon = ptc) {
         ptc = ptcon->NEXT;
         mbkfree((void *)ptcon);
      }
      mbkfree((void *)ptmodel);
      ptmodel = ptfig;
      if (!ptfig)
         break;
      ptfig = ptfig->NEXT;
   }

   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- freelomodel\n");
}

/*******************************************************************************
* function  dellosig()                                                         *
*******************************************************************************/
int dellosig(lofig_list *ptfig, long index)
{
losig_list *ptsav=NULL;
losig_list *ptsig;

   for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT) {
      if (ptsig->INDEX == index)
         break;
      ptsav = ptsig;
   }

   if (ptsig == NULL)
      return 0;
   else if (ptsig == ptfig->LOSIG)
      ptfig->LOSIG = ptsig->NEXT;
   else
      ptsav->NEXT = ptsig->NEXT;

   if( ptsig->PRCN )
      freelorcnet( ptsig );
   ptsig->INDEX = 0L;

  freechain(ptsig->NAMECHAIN);
  ptsig->NAMECHAIN=NULL;

  dellosiguser( ptsig );

   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- dellosig  : %ld\n", index);

   return 1;
}

/*******************************************************************************
* function dellotrs()                                                          *
*******************************************************************************/
int dellotrs(lofig_list  *ptfig, lotrs_list  *pttrs)
{
lotrs_list  *pt;
lotrs_list  *ptsav=NULL;

   for (pt = ptfig->LOTRS; pt; pt = pt->NEXT) {
      if (pt == pttrs)
         break;
      ptsav = pt;
   }
   if (pt == NULL)
      return 0;
   else if (pt == ptfig->LOTRS)
      ptfig->LOTRS = pt->NEXT;
   else
      ptsav->NEXT = pt->NEXT;

   if( pt->GRID->PNODE )
      delrcnlocon( pt->GRID );
  delloconuser( pt->GRID );
   mbkfree((void *)pt->GRID);

   if( pt->SOURCE->PNODE )
      delrcnlocon( pt->SOURCE );
  delloconuser( pt->SOURCE );
   mbkfree((void *)pt->SOURCE);

   if( pt->DRAIN->PNODE )
      delrcnlocon( pt->DRAIN );
  delloconuser( pt->DRAIN );
   mbkfree((void *)pt->DRAIN);

   if( pt->BULK->PNODE )
      delrcnlocon( pt->BULK );
  delloconuser( pt->BULK );
   mbkfree((void *)pt->BULK);

  dellotrsuser( pt );
   mbkfree((void *)pt);
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- dellotrs  : \n");
   return 1;
}  

/*******************************************************************************
* function delloins()                                                          *
*******************************************************************************/
int delloins(lofig_list *ptfig, const char *iname)
{
loins_list *ptins;
locon_list *ptcon;
chain_list *ptchain;
chain_list *pttmpchain=NULL;
void    *pt=NULL;
char *figname;
char *insname;

   insname = namealloc(iname);
   for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT) {
      if (ptins->INSNAME == insname)
         break;
      pt = (void *)ptins;
   }
   if (ptins == NULL)   
      return 0;
   if (ptins == ptfig->LOINS) {
      figname = ptins->FIGNAME;
      ptfig->LOINS = ptins->NEXT;
   } else {
      figname = ptins->FIGNAME;
      ((loins_list *)pt)->NEXT = ptins->NEXT;
   }
   for (ptcon = ptins->LOCON; ptcon; ptcon = (locon_list *)pt) {
      if( ptcon->PNODE )
         delrcnlocon( ptcon );
      pt = (void *)ptcon->NEXT;
     delloconuser( ptcon );
      mbkfree((void *)ptcon);
   }

  delloinsuser( ptins );
   mbkfree((void *)ptins);

   for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
      if (ptins->FIGNAME == figname)
         break;
   if (ptins == NULL) { /* if no more instance have this model, destroy */
      for (ptchain = ptfig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT) {
         if (ptchain->DATA == (void *)figname)
            break;
         pttmpchain = ptchain;
      }
      if (ptchain == ptfig->MODELCHAIN && ptchain != NULL)
         ptfig->MODELCHAIN = ptchain ->NEXT;
      else if (ptchain)
         pttmpchain->NEXT = ptchain->NEXT;

     if( ptchain )
     {
       ptchain->NEXT = NULL;
       freechain( ptchain );
     }
   }

   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- delloins  : %s\n", insname);
   return 1;
}

/*******************************************************************************
* function dellocon()                                                          *
*******************************************************************************/
int dellocon(lofig_list *ptfig, const char *cname)
{
locon_list *ptcon;
locon_list *ptsav=NULL;
char *name = namealloc(cname);

   for (ptcon = ptfig->LOCON; ptcon; ptcon = ptcon->NEXT) {
      if (ptcon->NAME == name)
         break;
      ptsav = ptcon;
   }
   if (ptcon == NULL)    
      return 0;
   else if (ptcon == ptfig->LOCON)   
      ptfig->LOCON = ptcon->NEXT;
   else 
      ptsav->NEXT = ptcon->NEXT;

   if( ptcon->PNODE )
      delrcnlocon( ptcon );

  delloconuser( ptcon );
   mbkfree((void *)ptcon);
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- dellocon  : %s\n", name);

   return 1;
}

/*******************************************************************************
* function getlomodel                              *
* gives a pointer to a model or NULL if it doesn't exist             *
*******************************************************************************/
lofig_list *getlomodel(lofig_list *ptmodel, const char *mname)
{
lofig_list *ptfig;

char *name = namealloc(mname);
   for (ptfig = ptmodel; ptfig; ptfig = ptfig->NEXT)
      if (ptfig->NAME == name)
         return ptfig;
   return NULL;
}

/*******************************************************************************
* function getloins()                                                          *
*******************************************************************************/
loins_list *getloins(lofig_list *ptfig, const char *iname)
{
loins_list *ptins;
char *name = namealloc(iname);

   for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
      if (ptins->INSNAME == name)
         return ptins;

   (void)fflush(stdout);
   (void)fprintf(stderr, "*** mbk error ***\n");
   (void)fprintf(stderr, "getloins impossible :\n");
   (void)fprintf(stderr, "instance %s doesn't exist in figure %s\n", 
                    name, ptfig->NAME);
   EXIT(1);
   return NULL; /* never reached */
}

/*******************************************************************************
* function getlotrs()                                                          *
*******************************************************************************/
lotrs_list *getlotrs(lofig_list *ptfig, const char *tname)
{
lotrs_list  *pttrs;
char *name = namealloc(tname);

   for (pttrs = ptfig->LOTRS; pttrs; pttrs = pttrs->NEXT)
      if (pttrs->TRNAME == name)
   return pttrs;

   (void)fflush(stdout);
   (void)fprintf(stderr, "*** mbk error ***\n");
   (void)fprintf(stderr, "getlotrs impossible :\n");
   (void)fprintf(stderr, "transistor %s doesn't exist in figure %s\n", 
                    name, ptfig->NAME);
   EXIT(1);
   return NULL; /* never reached */
}

/*******************************************************************************
* function getlocon()                                                          *
*******************************************************************************/
locon_list *getlocon(lofig_list *ptfig, const char *cname)
{
locon_list  *ptcon;
char *name = namealloc(cname);

   for (ptcon = ptfig->LOCON; ptcon; ptcon = ptcon->NEXT)
      if (ptcon->NAME == name)
   return ptcon;

   (void)fflush(stdout);
   (void)fprintf(stderr, "*** mbk error ***\n");
   (void)fprintf(stderr, "getlocon impossible :\n");
   (void)fprintf(stderr, "connector %s doesn't exist in figure %s\n", 
                    name, ptfig->NAME);
   EXIT(1);
   return NULL; /* never reached */
}

/*******************************************************************************
* function getlosig()                                                          *
*******************************************************************************/
losig_list *getlosig(lofig_list *ptfig, long index)
{
losig_list *ptsig;
ptype_list *pt;
long low;
long high;
long sigsize;

        sigsize = getsigsize( ptfig );
   low = (index % sigsize);
   high = (index / sigsize);
   for (pt = ptfig->BKSIG; pt; pt = pt->NEXT)
      if (pt->TYPE == high) break;
   if (pt != NULL) {
      ptsig = (losig_list *)(pt->DATA) + low;
      if (ptsig->INDEX == index)
         return ptsig;
   }

   (void)fflush(stdout);
   (void)fprintf(stderr, "*** mbk error ***\n");
   (void)fprintf(stderr, "getlosig impossible :\n");
   (void)fprintf(stderr, "signal %ld doesn't exist in figure %s\n", 
                    index, ptfig->NAME);
   EXIT(1);
   return NULL; /* never reached */
}

/*******************************************************************************
* function  lofigchain()                                                       *
* For each signal, construct the list of connectors associated                 *
* with the signal.(Field   USER  of the "losig" structure with                 *
* the code "LOFIGCHAIN")                                                       *
* This provide the "dual" representation of the net-list                       *
*******************************************************************************/
void lofigchain(lofig_list *ptfig)
{
locon_list *ptcon = NULL;
losig_list *ptsig = NULL;
loins_list *ptins = NULL;
lotrs_list *pttrs = NULL;
locap_list *ptcap = NULL;
lores_list *ptres = NULL;
loself_list *ptself = NULL;
ptype_list *ptype = NULL;

   if (ptfig->MODE == 'P') {
           /*
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\nlofigchain impossible ");
      (void)fprintf(stderr, ": figure %s is interface only\n", 
                     ptfig->NAME);
      EXIT(1);
           */
           return;
   }

   /*  cleaning ...   */
   for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT) {
      ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
      if (ptype != (ptype_list *)NULL) {
         freechain((chain_list *)ptype->DATA);
         ptype->DATA = (void *)NULL;
      } else
         ptsig->USER = addptype(ptsig->USER, (long)LOFIGCHAIN, (void *)NULL);
   }

   /*  scan connector list  */
   for (ptcon=ptfig->LOCON; ptcon; ptcon=ptcon->NEXT) {
      ptsig = ptcon->SIG;
      ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
      ptype->DATA = (void *)addchain((chain_list *)ptype->DATA, (void *)ptcon);
      if (DEBUG_MODE == 'Y')
        (void)printf("--- mbk ---    lofigchain (lofig): on signal <%s> add con <%s>\n", getsigname(ptsig), ptcon->NAME);
   }

   /*  scan instance  list    */ 
   for (ptins =ptfig->LOINS; ptins; ptins=ptins->NEXT) {
      for (ptcon=ptins->LOCON; ptcon; ptcon=ptcon->NEXT) {
         ptsig=ptcon->SIG;
         ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
         ptype->DATA = (void *)addchain((chain_list *)ptype->DATA, (void *)ptcon);
         if (DEBUG_MODE == 'Y')
           (void)printf("--- mbk ---    lofigchain (loins): on signal <%s> add con <%s>\n", getsigname(ptsig), ptcon->NAME);
      }
   }

   /*  scan transistor list   */
   for (pttrs = ptfig->LOTRS; pttrs; pttrs = pttrs->NEXT) {
      ptsig = pttrs->GRID->SIG;
      ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
      ptype->DATA = (void *)addchain((chain_list *)ptype->DATA, 
                                       (void *)pttrs->GRID);
      ptsig = pttrs->SOURCE->SIG;
      ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
      ptype->DATA = (void *)addchain((chain_list *)ptype->DATA, 
                                       (void *)pttrs->SOURCE);
      ptsig = pttrs->DRAIN->SIG;
      ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
      ptype->DATA = (void *)addchain((chain_list *)ptype->DATA, 
                                       (void *)pttrs->DRAIN);
      ptsig = pttrs->BULK->SIG;
      if( ptsig ) {
         ptype = getptype(ptsig->USER, (long)LOFIGCHAIN);
         ptype->DATA = (void *)addchain((chain_list *)ptype->DATA, 
                                        (void *)pttrs->BULK);
      }
   }

   /*  scan capacitor list   */

   for(ptcap = ptfig -> LOCAP ; ptcap != NULL ; ptcap = ptcap -> NEXT)
     {
       ptsig = ptcap -> TCON -> SIG ;
       ptype = getptype(ptsig -> USER,(long)LOFIGCHAIN) ;
       ptype -> DATA = (void *)addchain((chain_list *)ptype -> DATA,(void *)ptcap -> TCON) ;

       ptsig = ptcap -> BCON -> SIG ;
       ptype = getptype(ptsig -> USER,(long)LOFIGCHAIN) ;
       ptype -> DATA = (void *)addchain((chain_list *)ptype -> DATA,(void *)ptcap -> BCON) ;
     }

   /*  scan resistor list   */

   for(ptres = ptfig -> LORES ; ptres != NULL ; ptres = ptres -> NEXT)
     {
       ptsig = ptres -> RCON1 -> SIG ;
       ptype = getptype(ptsig -> USER,(long)LOFIGCHAIN) ;
       ptype -> DATA = (void *)addchain((chain_list *)ptype -> DATA,(void *)ptres -> RCON1) ;

       ptsig = ptres -> RCON2 -> SIG ;
       ptype = getptype(ptsig -> USER,(long)LOFIGCHAIN) ;
       ptype -> DATA = (void *)addchain((chain_list *)ptype -> DATA,(void *)ptres -> RCON2) ;
     }

   /*  scan inductor list   */

   for(ptself = ptfig -> LOSELF ; ptself != NULL ; ptself = ptself -> NEXT)
     {
       ptsig = ptself -> SCON1 -> SIG ;
       ptype = getptype(ptsig -> USER,(long)LOFIGCHAIN) ;
       ptype -> DATA = (void *)addchain((chain_list *)ptype -> DATA,(void *)ptself -> SCON1) ;

       ptsig = ptself -> SCON2 -> SIG ;
       ptype = getptype(ptsig -> USER,(long)LOFIGCHAIN) ;
       ptype -> DATA = (void *)addchain((chain_list *)ptype -> DATA,(void *)ptself -> SCON2) ;
     }

   if (DEBUG_MODE == 'Y')
     (void)printf("--- mbk --- lofigchain %s\n", ptfig -> NAME) ;
}

/*******************************************************************************
* function getsigname()                                                        *
* choose the least concatened signal name                                      *
*******************************************************************************/
char *getsigname(losig_list *ptsig)
{
chain_list *ptscan;
char *ptchar;
char buffer[50];
char *result = NULL;
register long nseparmin = 1000;

   if (ptsig == NULL)
      return NULL;

   for (ptscan = ptsig->NAMECHAIN; ptscan; ptscan = ptscan->NEXT) {
   register long nsepar = 0;
      if (ptscan->DATA != NULL) {
         for (ptchar = (char *)ptscan->DATA; *ptchar != '\0'; ptchar++)
            if (*ptchar == SEPAR)
               nsepar++;
         if (nsepar < nseparmin) {
            nseparmin = nsepar;
            result = (char *)ptscan->DATA;
         }
      }
   }
   if (!result) {
      (void)sprintf(buffer, "%ld", ptsig->INDEX);
      result = namealloc(buffer);
   }
   return result;
}

/*******************************************************************************
* function viewlo()                                                            *
* display on screen the content of all logical figures                         *
*******************************************************************************/
void viewlo(void)
{
lofig_list  *ptfig;

   if (HEAD_LOFIG == NULL) {
      (void)printf("\n!!! viewlo : empty list of figure !!!\n");
      return;
   } else for (ptfig = HEAD_LOFIG; ptfig; ptfig = ptfig->NEXT)
      viewlofig(ptfig);
}

/*******************************************************************************
* function viewlofig()                                                         *
* display on screen the content of logical figure ptfig                        *
*******************************************************************************/
void viewlofig(lofig_list  *ptfig)
{
locon_list *ptcon;
losig_list *ptsig;
loins_list *ptins;
lotrs_list *pttrs;
locap_list *ptcap;
lores_list *ptres;
loself_list *ptself;
chain_list *scanchain;
ptype_list *pt;

   if (ptfig->NAME == NULL)
      (void)printf("!!! figure without name !!! /n");
   else
      (void)printf("\nfigure   : %-20s  mode : %c\n", ptfig->NAME, ptfig->MODE);

   if (ptfig->LOCON == NULL) 
      (void)printf("   |---empty connector list\n");   
   else 
        {
          for (ptcon = ptfig->LOCON; ptcon; ptcon = ptcon->NEXT)
       viewlofigcon(ptcon);

          pt = getptype( ptfig->USER, PH_INTERF );
          if( pt )
          {
            (void)printf("   |---connector list physical order\n");   
            for( scanchain = (chain_list*)(pt->DATA); scanchain ; scanchain = scanchain->NEXT )
              (void)printf("   |    |---%s\n",(char*)(scanchain->DATA) );   
            (void)printf("   |\n");   
          }
        }

   if (ptfig->LOSIG == NULL)
      (void)printf("   |---empty signal list\n");
   else for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT)
      viewlosig(ptsig);

   if (ptfig->LOINS == NULL)
      (void)printf("   |---empty instance list\n");
   else for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
      viewloins(ptins);

   if (ptfig->LOTRS == NULL)
      (void)printf("   |---empty transistor list\n");
   else for (pttrs = ptfig->LOTRS; pttrs; pttrs = pttrs->NEXT)
      viewlotrs(pttrs);

   if(ptfig -> LOCAP == NULL)
     {
       (void)printf("   |---empty capacitor list\n") ;
     }
   else
     {
       for(ptcap = ptfig -> LOCAP ; ptcap ; ptcap = ptcap -> NEXT)
	 {
           viewlocap(ptcap) ;
	 }
     }

   if(ptfig -> LORES == NULL)
     {
       (void)printf("   |---empty resistor list\n") ;
     }
   else
     {
       for(ptres = ptfig -> LORES ; ptres ; ptres = ptres -> NEXT)
	 {
           viewlores(ptres) ;
	 }
     }

   if(ptfig -> LOSELF == NULL)
     {
       (void)printf("   |---empty inductor list\n") ;
     }
   else
     {
       for(ptself = ptfig -> LOSELF ; ptself ; ptself = ptself -> NEXT)
	 {
           viewloself(ptself) ;
	 }
     }

   if (ptfig->USER != NULL)
        {
      (void)printf("   |---USER field\n");
                for( pt = ptfig->USER ; pt ; pt = pt->NEXT )
            (void)printf("   |    |-- %ld\n",pt->TYPE);
        }
        else
      (void)printf("   |---empty USER field\n");

   (void)printf("   | \n");
}   

/*******************************************************************************
* function viewlofigcon()                                                      *
*******************************************************************************/
void viewlofigcon(locon_list  *ptcon)
{ 
   num_list *scannum;
        ptype_list      *scanptype;
        chain_list      *scanchain;

   if (ptcon->NAME == NULL)
      (void)printf("   |--- !!! connector without name !!!\n");
   else
      (void)printf("   |---connector  : \"%s\"\n", ptcon->NAME);
   (void)printf("   |    |---direction  : %c\n", ptcon->DIRECTION);
   if (ptcon->SIG == NULL)
      (void)printf("   |    |---no signal\n");
   else
      (void)printf("   |    |---signal    : %ld\n", ptcon->SIG->INDEX);
   if (ptcon->ROOT == NULL)
      (void)printf("   |---no root\n");
   else
      (void)printf("   |    |---root    : %s\n",
                           ((char *)(((lofig_list *)(ptcon->ROOT))->NAME)));
   (void)printf("   |    |---type    : %c \n", ptcon->TYPE);

   (void)printf("   |    |---USER list\n");
        for( scanptype = ptcon->USER ; scanptype ; scanptype = scanptype->NEXT )
        {
                switch( scanptype->TYPE )
                {
                case PNODENAME:
            (void)printf("   |    |   |---PNODENAME\n");
                        for( scanchain = (chain_list*)(scanptype->DATA);
                             scanchain;
                             scanchain = scanchain->NEXT )
                     (void)printf("   |    |   |   |---%s\n", (char*)(scanchain->DATA)?(char*)(scanchain->DATA):"NULL" );
                        break;
                default:
            (void)printf("   |    |    |   |---%ld\n",scanptype->TYPE);
                }
        }

   if(ptcon->PNODE == NULL)
      (void)printf("   |    |---no node\n");
   else {
      (void)printf("   |    |---node\n");
      for(scannum = ptcon->PNODE; scannum; scannum = scannum->NEXT )
         (void)printf("   |    |    |---%ld\n",scannum->DATA);

   }

   (void)printf("   |\n");
}

/*******************************************************************************
* function viewlosig()                                                         *
*******************************************************************************/
void viewlosig(losig_list  *ptsig)
{ 
chain_list  *ptchain;
lowire_list *scanwire;
chain_list  *scanctc;
loctc_list  *ptctc;
char         levelrc;
ptype_list  *ptptype;
num_list    *scannum;

   (void)printf("   |---signal    : %ld \n" , ptsig->INDEX);
   if (ptsig->NAMECHAIN == NULL)
      (void)printf("   |    |---no alias names\n");
   else for (ptchain = ptsig->NAMECHAIN; ptchain; ptchain = ptchain->NEXT) {  
      if (ptchain->DATA == NULL)
         (void)printf("   |    |--- !!! empty name !!!\n");
      else
         (void)printf("   |    |---alias   : \"%s\"\n", (char *)ptchain->DATA);
   }
   (void)printf("   |    |---type    : %c\n" , ptsig->TYPE);

   levelrc = rclevel( ptsig );
   
   if( levelrc == MBK_RC_A )
      (void)printf("   |    |---no parasitic data\n");
   else {
      (void)printf("   |    |---parasitic data\n");
      (void)printf("   |    |    |---capacitance : %g\n" , ptsig->PRCN->CAPA);
      if( levelrc == MBK_RC_C || levelrc == MBK_RC_E ) {
         (void)printf("   |    |    |---wire list\n");
         for( scanwire = ptsig->PRCN->PWIRE ; scanwire != NULL ; scanwire = scanwire->NEXT )
                           (void)printf("   |    |    |    |- %s n1=%3ld n2=%3ld r=%g c=%g\n",
                                             scanwire->LAYER == RCN_WIRE_ALU1 ? "ALU1" :
                                             scanwire->LAYER == RCN_WIRE_ALU2 ? "ALU2" :
                                             scanwire->LAYER == RCN_WIRE_POLY ? "POLY" :
                                             scanwire->LAYER == RCN_WIRE_CONT_POLY ? "CONT_POLY" :
                                             scanwire->LAYER == RCN_WIRE_CONT_DIF_N ? "CONT_DIF_N" :
                                             scanwire->LAYER == RCN_WIRE_CONT_VIA ? "CONT_VIA" :
                                             scanwire->LAYER == RCN_WIRE_CONT_VIA2 ? "CONT_VIA2" :
                                             "UNKNOWN",
                         scanwire->NODE1,
                         scanwire->NODE2,
                         scanwire->RESI,
                         scanwire->CAPA   );
      }
      else
         (void)printf("   |    |    |---no wire\n");
      if( levelrc == MBK_RC_D || levelrc == MBK_RC_E ) {
         (void)printf("   |    |    |---crosstalk capacitance list\n");
         for( scanctc = ptsig->PRCN->PCTC ; scanctc != NULL ; scanctc = scanctc->NEXT ) {
                                ptctc = (loctc_list*)scanctc->DATA;
            (void)printf("   |    |    |    |-S1=%3ld N1=%3ld S2=%3ld N2=%3ld Capa=%g.\n", 
                                             ptctc->SIG1->INDEX,
                                             ptctc->NODE1,
                                             ptctc->SIG2->INDEX,
                                             ptctc->NODE2,
                                             ptctc->CAPA
                                             );
                        }
                }
      else
         (void)printf("   |    |    |---no crosstalk capacitance\n");
      (void)printf("   |    |    |---%ld node\n",ptsig->PRCN->NBNODE);
   }
   (void)printf("   |    |---USER list\n");
        for( ptptype = ptsig->USER ; ptptype ; ptptype = ptptype->NEXT )
        {
                switch( ptptype->TYPE )
                {
                case LOFIGCHAIN:
            printf( "   |    |   |-LOFIGCHAIN\n" );
               for( ptchain = (chain_list*)ptptype->DATA ; ptchain ; ptchain = ptchain->NEXT )
                        {
               printf( "   |    |   |   |-%s ",((locon_list*)ptchain->DATA)->NAME);
                                switch( ((locon_list*)ptchain->DATA)->TYPE )
                                {
                                        case 'T':
                                                printf( "(T%c : %s) ",((lotrs_list*)((locon_list*)ptchain->DATA)->ROOT)->TYPE==TRANSN?'N':'P',((lotrs_list*)((locon_list*)ptchain->DATA)->ROOT)->TRNAME ? ((lotrs_list*)((locon_list*)ptchain->DATA)->ROOT)->TRNAME : "No name" );
                                                break;
                                        case 'P':
                                                printf( "(C%s : %s) ",((locap_list*)((locon_list*)ptchain->DATA)->ROOT)->TYPE==CAPMIM?"CAPMIM":"CAPPNWELL",((locap_list*)((locon_list*)ptchain->DATA)->ROOT)-> NAME ? ((locap_list*)((locon_list*)ptchain->DATA)->ROOT)->NAME : "No name" );
                                                break;
                                        case 'R':
                                                printf( "(C%s : %s) ",((lores_list*)((locon_list*)ptchain->DATA)->ROOT)->TYPE==RESMIM?"RESMIM":"RESMIM",((lores_list*)((locon_list*)ptchain->DATA)->ROOT)-> NAME ? ((lores_list*)((locon_list*)ptchain->DATA)->ROOT)->NAME : "No name" );
                                                break;
                                        case 'L':
                                                printf( "(C%s : %s) ",((loself_list*)((locon_list*)ptchain->DATA)->ROOT)->TYPE==SELFMIM?"SELFMIM":"SELFMIM",((loself_list*)((locon_list*)ptchain->DATA)->ROOT)-> NAME ? ((loself_list*)((locon_list*)ptchain->DATA)->ROOT)->NAME : "No name" );
                                                break;
                                        case 'I':
                                                printf( "(I : %s) ",((loins_list*)((locon_list*)ptchain->DATA)->ROOT)->INSNAME ? ((loins_list*)((locon_list*)ptchain->DATA)->ROOT)->INSNAME : "No name" );
                                                break;
                                        case 'E':
                                                printf( "(Figure) " );
                                                break;
                                        default:
                                                printf( "No valid type.\n" );
                                }
                                for( scannum = ((locon_list*)ptchain->DATA)->PNODE ; scannum ; scannum = scannum->NEXT )
                                        printf( "%ld ", scannum->DATA );
                                printf( "\n" );
                        }
                        break;
                default:
                        break;
                }
        }
   {
   }
   (void)printf("   | \n");
}


/*******************************************************************************
* function viewloins()                                                         *
*******************************************************************************/
void viewloins(loins_list  *ptins)
{ 
locon_list  *ptcon;
chain_list *scanchain;
ptype_list *pt;

if (ptins->INSNAME == NULL)
   (void)printf("   |--- !!! no instance name !!!\n");
else
   (void)printf("   |---instance   : %s\n", ptins->INSNAME);
if (ptins->FIGNAME == NULL)
   (void)printf("   |   |--- !!! no instance model !!!\n");
else
   (void)printf("   |   |---model     : %s \n" , ptins->FIGNAME);
if (ptins->LOCON == NULL)
   (void)printf("   |   |---empty list of connectors\n");
else
{
  for (ptcon = ptins->LOCON; ptcon != NULL; ptcon = ptcon->NEXT)
    viewloinscon(ptcon);
  pt = getptype( ptins->USER, PH_INTERF );
  if( pt )
  {
    (void)printf("   |   |---connector list physical order\n");   
    for( scanchain = (chain_list*)(pt->DATA); scanchain ; scanchain = scanchain->NEXT )
      (void)printf("   |   |    |---%s\n",(char*)(scanchain->DATA) );   
      (void)printf("   |   |\n");   
  }
}
if (ptins->USER != NULL)
   (void)printf("   |   |---non empty USER field\n");
   (void)printf("   | \n");
}

/*******************************************************************************
* function viewlotrs()                                                         *
*******************************************************************************/
void viewlotrs(lotrs_list  *pttrs)
{ 
   (void)printf("   |---transistor\n");
   (void)printf("   |   |---type  : %c %s %s\n",
               IsTransN(pttrs->TYPE)?'N':'P',
               IsTransFast(pttrs->TYPE)?"FAST":"SLOW",
               IsTransHvio(pttrs->TYPE)?"HVIO":"----"
              );
if (pttrs->TRNAME != NULL )
   (void)printf("   |   |---name : %s\n", pttrs->TRNAME );
else
   (void)printf("   |   |---no name\n" );

if (pttrs->SOURCE == NULL)
   (void)printf("   |   |--- !!! no signal on source !!! \n");
else
        viewloinscon( pttrs->SOURCE );

if (pttrs->GRID == NULL)
   (void)printf("   |   |--- !!! no signal on grid !!!\n");
else
        viewloinscon( pttrs->GRID );

if (pttrs->DRAIN == NULL)
   (void)printf("   |   |--- !!! no signal on drain !!!\n");
else
        viewloinscon( pttrs->DRAIN );

if (pttrs->BULK == NULL)
   (void)printf("   |   |--- !!! no signal on bulk !!!\n");
else
        viewloinscon( pttrs->BULK );

   (void)printf("   |   |---x      : %ld\n", pttrs->X);
   (void)printf("   |   |---y      : %ld\n", pttrs->Y);
   (void)printf("   |   |---width  : %ld\n", pttrs->WIDTH);
   (void)printf("   |   |---length : %ld\n", pttrs->LENGTH);
   (void)printf("   |   |---ps     : %ld\n", pttrs->PS);
   (void)printf("   |   |---pd     : %ld\n", pttrs->PD);
   (void)printf("   |   |---xs     : %ld\n", pttrs->XS);
   (void)printf("   |   |---xd     : %ld\n", pttrs->XD);
if (pttrs->USER != NULL)
   (void)printf("   |   |---non empty USER field\n");
   (void)printf("   |\n");
}

/*******************************************************************************
* function viewloinscon()                                                      *
*******************************************************************************/
void viewloinscon(locon_list *ptcon)
{
num_list   *scannum;
ptype_list *scanptype;
chain_list *scanchain;

if (ptcon->NAME == NULL)
   (void)printf("   |   |---no connector name\n");
else
   (void)printf("   |   |---connector  : \"%s\"\n" , ptcon->NAME);
   (void)printf("   |   |    |---direction  : %c\n" , ptcon->DIRECTION);
if (ptcon->SIG == NULL)
   (void)printf("   |   |    |---no signal\n");
else
   (void)printf("   |   |    |---signal    : %ld\n" , ptcon->SIG->INDEX);
if ((ptcon->ROOT) == NULL)
   (void)printf("   |   |    |---no root\n");
else
  switch( ptcon->TYPE )
  {
    case 'I':
   (void)printf("   |   |    |---root    : %s\n" , ((char*)(((loins_list *)(ptcon->ROOT))->INSNAME)));
        break;
    case 'T':
   (void)printf("   |   |    |---root    : %s\n" , 
                     ((lotrs_list*)(ptcon->ROOT))->TRNAME?((lotrs_list*)(ptcon->ROOT))->TRNAME:"no transistor name" );
        break;
    case 'C':
   (void)printf("   |   |    |---root    : %s\n" , 
                     ((locap_list*)(ptcon->ROOT))->NAME?((locap_list*)(ptcon->ROOT))->NAME:"no capacitor name" );
        break;
    case 'R':
   (void)printf("   |   |    |---root    : %s\n" , 
                     ((lores_list*)(ptcon->ROOT))->NAME?((lores_list*)(ptcon->ROOT))->NAME:"no resistor name" );
        break;
    case 'S':
   (void)printf("   |   |    |---root    : %s\n" , 
                     ((loself_list*)(ptcon->ROOT))->NAME?((loself_list*)(ptcon->ROOT))->NAME:"no inductor name" );
        break;
    default:
   (void)printf("   |   |    |---root    : *** BAD TYPE ***\n" );
  }
   (void)printf("   |   |    |---type    : %c\n" , ptcon->TYPE);
   if(ptcon->PNODE == NULL)
      (void)printf("   |   |    |---no node\n");
   else {
      (void)printf("   |   |    |---node\n");
      for(scannum = ptcon->PNODE; scannum; scannum = scannum->NEXT )
         (void)printf("   |   |    |    |---%ld\n",scannum->DATA);
   }
   (void)printf("   |   |    |---USER list\n");
        for( scanptype = ptcon->USER ; scanptype ; scanptype = scanptype->NEXT )
        {
                switch( scanptype->TYPE )
                {
                case PNODENAME:
            (void)printf("   |   |    |    |---PNODENAME\n");
                        for( scanchain = (chain_list*)(scanptype->DATA);
                             scanchain;
                             scanchain = scanchain->NEXT )
                     (void)printf("   |   |    |    |   |---%s\n", (char*)(scanchain->DATA)?(char*)(scanchain->DATA):"NULL" );
                        break;
                default:
            (void)printf("   |   |    |    |---%ld\n",scanptype->TYPE);
                }
        }
   (void)printf("   |   | \n");
}

/*******************************************************************************
* function getsigsize()                                                        *
*******************************************************************************/

long getsigsize(lofig_list *ptfig)
{
  ptype_list    *ptl;

  ptl = getptype( ptfig->USER, PTSIGSIZE );
  if( ptl )
    return( (long)(ptl->DATA) );

  return( SIGSIZE );
}

/*******************************************************************************
* function setsigsize()                                                        *
*******************************************************************************/

void setsigsize(lofig_list *ptfig, long nb)
{
  ptype_list    *ptl;

  if( ptfig->BKSIG )
  {
    fflush( stdout );
    fprintf( stderr,"*** mbk error ***\n" );
    fprintf( stderr,"setsigsize() impossible\n" ); 
    fprintf( stderr,"BKSIG not NULL.\n" );
    EXIT(1);
  }
  
  ptl = getptype( ptfig->USER, PTSIGSIZE );
  if( !ptl )
    ptfig->USER = addptype( ptfig->USER, PTSIGSIZE, (void*)nb );
  else
    ptl->DATA = (void*)nb;
}


/*******************************************************************************
* function for cleaning USER field of structures.                              *
*******************************************************************************/

/*#define MBK_TRACE_BAD_PTYPE 0*/

void delloconuser(locon_list *ptlocon)
{
  ptype_list *scanptype;
  ptype_list *next;
  ptype_list *prev;
  long         del;

  prev = NULL;
  for( scanptype = ptlocon->USER ; scanptype ; scanptype = next )
  {
    next = scanptype->NEXT;
    del  = 0;
    
    switch( scanptype->TYPE )
    {
      case PNODENAME:
        freechain( scanptype->DATA );
        del = 1;
        break;
      default:
#ifdef MBK_TRACE_BAD_PTYPE
        fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n",
                 scanptype->TYPE
               );
#endif
        break;
    }

    if( del )
    {
      if( !prev )
        ptlocon->USER = next;
      else
        prev->NEXT = next;
      scanptype->NEXT = NULL;
      freeptype( scanptype );
    }
    else
      prev = scanptype;
  }
}

void dellofiguser(lofig_list *ptlofig)
{
  ptype_list *scanptype;
  ptype_list *next;
  ptype_list *prev;
  long         del;

  prev = NULL;
  for( scanptype = ptlofig->USER ; scanptype ; scanptype = next )
  {
    next = scanptype->NEXT;
    del  = 0;

    switch( scanptype->TYPE )
    {
      case PH_INTERF:
        freechain( scanptype->DATA );
        del = 1;
        break;
      case PTSIGSIZE:
        del = 1;
        break;
      default:
#ifdef MBK_TRACE_BAD_PTYPE
        fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n",
                 scanptype->TYPE
               );
#endif
        break;
    }

    if( del )
    {
      if( !prev )
        ptlofig->USER = next;
      else
        prev->NEXT = next;
      scanptype->NEXT = NULL;
      freeptype( scanptype );
    }
    else
      prev = scanptype;
  }
}

void delloinsuser(loins_list *ptloins)
{
  ptype_list *scanptype;
  ptype_list *next;
  ptype_list *prev;
  long         del;

  prev = NULL;
  for( scanptype = ptloins->USER ; scanptype ; scanptype = next )
  {
    next = scanptype->NEXT;
    del  = 0;

    switch( scanptype->TYPE )
    {
      case PH_INTERF:
        freechain( scanptype->DATA );
        del = 1;
        break;
      default:
#ifdef MBK_TRACE_BAD_PTYPE
        fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n",
                 scanptype->TYPE
               );
#endif
        break;
    }

    if( del )
    {
      if( !prev )
        ptloins->USER = next;
      else
        prev->NEXT = next;
      scanptype->NEXT = NULL;
      freeptype( scanptype );
    }
    else
      prev = scanptype;
  }
}

void dellotrsuser(lotrs_list *ptlotrs)
{
  ptype_list *scanptype;
  ptype_list *next;
  ptype_list *prev;
  long         del;
 
  prev = NULL; 
  for( scanptype = ptlotrs->USER ; scanptype ; scanptype = next )
  {
    next = scanptype->NEXT;
    del  = 0;

    switch( scanptype->TYPE )
    {
      default:
#ifdef MBK_TRACE_BAD_PTYPE
        fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n",
                 scanptype->TYPE
               );
#endif
        break;
    }

    if( del )
    {
      if( !prev )
        ptlotrs->USER = next;
      else
        prev->NEXT = next;
      scanptype->NEXT = NULL;
      freeptype( scanptype );
    }
    else
      prev = scanptype;
  }
}

void dellosiguser(losig_list *ptlosig)
{
  ptype_list *scanptype;
  ptype_list *next;
  ptype_list *prev;
  long         del;

  prev = NULL;
  for( scanptype = ptlosig->USER ; scanptype ; scanptype = next )
  {
    next = scanptype->NEXT;
    del  = 0;

    switch( scanptype->TYPE )
    {
      case LOFIGCHAIN:
        freechain( scanptype->DATA );
        del = 1;
        break;
      default:
#ifdef MBK_TRACE_BAD_PTYPE
        fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n",
                 scanptype->TYPE
               );
#endif
        break;
    }

    if( del )
    {
      if( !prev )
        ptlosig->USER = next;
      else
        prev->NEXT = next;
      scanptype->NEXT = NULL;
      freeptype( scanptype );
    }
    else
      prev = scanptype;
  }
}

/***********************************************************************************************/
/*********************************** Analogical World ******************************************/
/***********************************************************************************************/

/*********************************** Capacitor : add *******************************************/

locap_list *addlocap(lofig_list *ptfig,char type,double capa,losig_list *pttplate,
                     losig_list *ptbplate,char *name)
{
  static char *tplate = NULL ;
  static char *bplate = NULL ;
  locap_list  *ptcap  = NULL ;
  locon_list  *ptcon  = NULL ;

  tplate = namealloc("tplate");
  bplate = namealloc("bplate");

  if((type != CAPMIM) && (type != CAPPNWELL))
    {
      (void)fflush(stdout) ;
      (void)fprintf(stderr, "*** mbk error ***\n") ;
      (void)fprintf(stderr, "  illegal capacitor type : %ld\n",(long)type) ;
      EXIT(1) ;
    }

   ptcap = (locap_list *)mbkalloc(sizeof(locap_list)) ;
   ptcap -> TYPE  = type ;
   ptcap -> CAPA  = capa ;
   ptcap -> NEXT  = ptfig -> LOCAP ;
   ptfig -> LOCAP = ptcap ;
   ptcap -> USER  = NULL ;

   ptcon = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon -> NAME      = tplate ;
   ptcon -> SIG       = pttplate ;
   ptcon -> ROOT      = (void *)ptcap ;
   ptcon -> TYPE      = TRANSCV2 ;
   ptcon -> DIRECTION = INTERNAL ;
   ptcon -> USER      = NULL ;
   ptcon -> PNODE     = NULL ;
   ptcap -> TCON      = ptcon ;
   
   ptcon = (locon_list *)mbkalloc(sizeof(locon_list)) ;
   ptcon -> NAME      = bplate ;
   ptcon -> SIG       = ptbplate ;
   ptcon -> ROOT      = (void *)ptcap ;
   ptcon -> TYPE      = TRANSCV2 ;
   ptcon -> DIRECTION = INTERNAL ;
   ptcon -> USER      = NULL ;
   ptcon -> PNODE     = NULL ;
   ptcap -> BCON      = ptcon ;
   
   /* No check is done for capacitor name unicity */
   ptcap -> NAME = namealloc(name) ;

   if(TRACE_MODE == 'Y')
     {
       (void)fprintf(stdout,"--- mbk --- addlocap  : %s\n",type == CAPMIM ? "CAPMIM" : "CAPPNWELL") ;
     }

   return ptcap ;
}

/*********************************** Capacitor : del *******************************************/

int dellocap(lofig_list *ptfig,locap_list  *ptcap)
{
  locap_list  *pt    = NULL ;
  locap_list  *ptsav = NULL ;

  for(pt = ptfig -> LOCAP ; pt != NULL ; pt = pt -> NEXT)
    {
      if(pt == ptcap)
	{
          break ;
	}

      ptsav = pt ;
   }

  if(pt == NULL)
    {
      return 0 ;
    }
  else
    {
      if(pt == ptfig -> LOCAP)
	{
          ptfig -> LOCAP = pt -> NEXT ;
	}
      else
	{
          ptsav -> NEXT = pt -> NEXT ;
	}
    }

  if(pt -> TCON -> PNODE)
    {
      delrcnlocon(pt -> TCON) ;
    }

  delloconuser(pt -> TCON) ;
  mbkfree((void *)pt -> TCON) ;

  if(pt -> BCON -> PNODE)
    {
      delrcnlocon(pt -> BCON) ;
    }

  delloconuser(pt -> BCON) ;
  mbkfree((void *)pt -> BCON) ;

  dellocapuser(pt) ;
  mbkfree((void *)pt) ;

  if(TRACE_MODE == 'Y')
    {
      (void)fprintf(stdout, "--- mbk --- dellocap  : \n") ;
    }

  return 1 ;
}  

/*********************************** Capacitor : getlocap **************************************/

locap_list *getlocap(lofig_list *ptfig, const char *cname)
{
  locap_list *ptcap = NULL ;
  char       *name  = namealloc(cname) ;

  for(ptcap = ptfig -> LOCAP ; ptcap != NULL ; ptcap = ptcap -> NEXT)
    {
      if(ptcap -> NAME == name)
	{
          return ptcap ;
	}
    }

  (void)fflush(stdout) ;
  (void)fprintf(stderr,"*** mbk error ***\n") ;
  (void)fprintf(stderr,"getlocap impossible :\n") ;
  (void)fprintf(stderr,"capacitor %s doesn't exist in figure %s\n",name,ptfig -> NAME) ;
  EXIT(1) ;

  return NULL ; /* never reached */
}

/*********************************** Capacitor : setlocap **************************************/

void setlocap(lofig_list *ptfig, const char *cname,double capa)
{
  locap_list *ptcap = NULL ;
  locap_list *cap   = NULL ;
  char       *name  = namealloc(cname) ;

  for(ptcap = ptfig -> LOCAP ; ptcap != NULL ; ptcap = ptcap -> NEXT)
    {
      if(ptcap -> NAME == name)
	{
          cap = ptcap ;
          break ;
	}
    }

  if(cap == NULL)
    {
      (void)fflush(stdout) ;
      (void)fprintf(stderr,"*** mbk error ***\n") ;
      (void)fprintf(stderr,"setlocap impossible :\n") ;
      (void)fprintf(stderr,"capacitor %s doesn't exist in figure %s\n",name,ptfig -> NAME) ;
      EXIT(1) ;
    }

  cap -> CAPA = capa ;
}

/*********************************** Capacitor : viewlocap *************************************/

void viewlocap(locap_list  *ptcap)
{ 
  (void)printf("   |---capacitor\n") ;
  (void)printf("   |   |---type  : %s\n",IsCapMIM(ptcap->TYPE)?"MIM":"POLY/NWELL") ;

  if(ptcap -> NAME != NULL)
    {
      (void)printf("   |   |---name : %s\n", ptcap -> NAME) ;
    }
  else
    {
      (void)printf("   |   |---no name\n") ;
    }

  if(ptcap -> TCON == NULL)
    {
      (void)printf("   |   |--- !!! no signal on top plate !!! \n") ;
    }
  else
    {
      viewloinscon(ptcap -> TCON) ;
    }

  if(ptcap -> BCON == NULL)
    {
      (void)printf("   |   |--- !!! no signal on bottom plate !!! \n") ;
    }
  else
    {
      viewloinscon(ptcap -> BCON) ;
    }

   (void)printf("   |   |---capa   : %g\n", ptcap -> CAPA) ;

  if(ptcap -> USER != NULL)
    {
      (void)printf("   |   |---non empty USER field\n") ;
    }

   (void)printf("   |\n") ;
}

/*********************************** Capacitor : dellocapuser **********************************/

void dellocapuser ( locap_list *ptlocap )
{
  ptype_list *scanptype = NULL;
  ptype_list *next      = NULL;
  ptype_list *prev      = NULL;
  long         del      = 0;
 
  for (scanptype = ptlocap->USER ; scanptype != NULL ; scanptype = next) {
    next = scanptype->NEXT ;
    del  = 0 ;

    switch (scanptype->TYPE) {
      case LOCAP_INFO :
        mbkfree( scanptype->DATA );
        del = 1 ;
        break ;
      default :
#ifdef MBK_TRACE_BAD_PTYPE
        fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n", scanptype->TYPE );
#endif
        break;
	}

    if (del) {
	  if (prev == NULL) {
        ptlocap->USER = next;
      } else {
        prev->NEXT = next;
      }

	  scanptype->NEXT = NULL;
	  freeptype( scanptype );
	} else {
	  prev = scanptype ;
	}
  }
}

/*********************************** Resistor : add ********************************************/

lores_list *addlores(lofig_list *ptfig,char type,double resi,losig_list *ptrcon1,
                     losig_list *ptrcon2,char *name)
{
  static char *rcon1 = NULL ;
  static char *rcon2 = NULL ;
  lores_list  *ptres  = NULL ;
  locon_list  *ptcon  = NULL ;

  rcon1 = namealloc("rcon1");
  rcon2 = namealloc("rcon2");

  if(type != RESMIM)
    {
      (void)fflush(stdout) ;
      (void)fprintf(stderr, "*** mbk error ***\n") ;
      (void)fprintf(stderr, "  illegal resistor type : %ld\n",(long)type) ;
      EXIT(1) ;
    }

   ptres = (lores_list *)mbkalloc(sizeof(lores_list)) ;
   ptres -> TYPE  = type ;
   ptres -> RESI  = resi ;
   ptres -> NEXT  = ptfig -> LORES ;
   ptfig -> LORES = ptres ;
   ptres -> USER  = NULL ;

   ptcon = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon -> NAME      = rcon1 ;
   ptcon -> SIG       = ptrcon1 ;
   ptcon -> ROOT      = (void *)ptres ;
   ptcon -> TYPE      = TRANSCV3 ;
   ptcon -> DIRECTION = INTERNAL ;
   ptcon -> USER      = NULL ;
   ptcon -> PNODE     = NULL ;
   ptres -> RCON1      = ptcon ;
   
   ptcon = (locon_list *)mbkalloc(sizeof(locon_list)) ;
   ptcon -> NAME      = rcon2 ;
   ptcon -> SIG       = ptrcon2 ;
   ptcon -> ROOT      = (void *)ptres ;
   ptcon -> TYPE      = TRANSCV3 ;
   ptcon -> DIRECTION = INTERNAL ;
   ptcon -> USER      = NULL ;
   ptcon -> PNODE     = NULL ;
   ptres -> RCON2      = ptcon ;
   
   /* No check is done for resistor name unicity */
   ptres -> NAME = namealloc(name) ;

   if(TRACE_MODE == 'Y')
     {
       (void)fprintf(stdout,"--- mbk --- addlores  : %s\n",type == RESMIM ? "RESMIM" : "RESMIM") ;
     }

   return ptres ;
}

/*********************************** Resistor : del ********************************************/

int dellores(lofig_list *ptfig,lores_list  *ptres)
{
  lores_list  *pt    = NULL ;
  lores_list  *ptsav = NULL ;

  for(pt = ptfig -> LORES ; pt != NULL ; pt = pt -> NEXT)
    {
      if(pt == ptres)
	{
          break ;
	}

      ptsav = pt ;
   }

  if(pt == NULL)
    {
      return 0 ;
    }
  else
    {
      if(pt == ptfig -> LORES)
	{
          ptfig -> LORES = pt -> NEXT ;
	}
      else
	{
          ptsav -> NEXT = pt -> NEXT ;
	}
    }

  if(pt -> RCON1 -> PNODE)
    {
      delrcnlocon(pt -> RCON1) ;
    }

  delloconuser(pt -> RCON1) ;
  mbkfree((void *)pt -> RCON1) ;

  if(pt -> RCON2 -> PNODE)
    {
      delrcnlocon(pt -> RCON2) ;
    }

  delloconuser(pt -> RCON2) ;
  mbkfree((void *)pt -> RCON2) ;

  delloresuser(pt) ;
  mbkfree((void *)pt) ;

  if(TRACE_MODE == 'Y')
    {
      (void)fprintf(stdout, "--- mbk --- dellores  : \n") ;
    }

  return 1 ;
}  

/*********************************** Resistor : getlores ***************************************/

lores_list *getlores(lofig_list *ptfig, const char *cname)
{
  lores_list *ptres = NULL ;
  char       *name  = namealloc(cname) ;

  for(ptres = ptfig -> LORES ; ptres != NULL ; ptres = ptres -> NEXT)
    {
      if(ptres -> NAME == name)
	{
          return ptres ;
	}
    }

  (void)fflush(stdout) ;
  (void)fprintf(stderr,"*** mbk error ***\n") ;
  (void)fprintf(stderr,"getlores impossible :\n") ;
  (void)fprintf(stderr,"resistor %s doesn't exist in figure %s\n",name,ptfig -> NAME) ;
  EXIT(1) ;

  return NULL ; /* never reached */
}

/*********************************** Resistor : setlores **************************************/

void setlores(lofig_list *ptfig, const char *cname,double resi)
{
  lores_list *ptres = NULL ;
  lores_list *res   = NULL ;
  char       *name  = namealloc(cname) ;

  for(ptres = ptfig -> LORES ; ptres != NULL ; ptres = ptres -> NEXT)
    {
      if(ptres -> NAME == name)
	{
          res = ptres ;
          break ;
	}
    }

  if(res == NULL)
    {
      (void)fflush(stdout) ;
      (void)fprintf(stderr,"*** mbk error ***\n") ;
      (void)fprintf(stderr,"setlores impossible :\n") ;
      (void)fprintf(stderr,"resistor %s doesn't exist in figure %s\n",name,ptfig -> NAME) ;
      EXIT(1) ;
    }

  res -> RESI = resi ;
}

/*********************************** Resistor : viewlores ***************************************/

void viewlores(lores_list  *ptres)
{ 
  (void)printf("   |---resistor\n") ;
  (void)printf("   |   |---type  : %s\n",IsResMIM(ptres->TYPE)?"MIM":"POLY/NWELL") ;

  if(ptres -> NAME != NULL)
    {
      (void)printf("   |   |---name : %s\n", ptres -> NAME) ;
    }
  else
    {
      (void)printf("   |   |---no name\n") ;
    }

  if(ptres -> RCON1 == NULL)
    {
      (void)printf("   |   |--- !!! no signal on first connector !!! \n") ;
    }
  else
    {
      viewloinscon(ptres -> RCON1) ;
    }

  if(ptres -> RCON2 == NULL)
    {
      (void)printf("   |   |--- !!! no signal on second connector !!! \n") ;
    }
  else
    {
      viewloinscon(ptres -> RCON2) ;
    }

   (void)printf("   |   |---resi   : %g\n", ptres -> RESI) ;

  if(ptres -> USER != NULL)
    {
      (void)printf("   |   |---non empty USER field\n") ;
    }

   (void)printf("   |\n") ;
}

/*********************************** Resistor : delloresuser ***********************************/

void delloresuser(lores_list *ptlores)
{
  ptype_list *scanptype = NULL ;
  ptype_list *next      = NULL ;
  ptype_list *prev      = NULL ;
  long         del       = 0    ;
 
  for(scanptype = ptlores -> USER ; scanptype != NULL ; scanptype = next)
    {
      next = scanptype -> NEXT ;
      del = 0 ;

      switch(scanptype -> TYPE)
	{
	case LORES_INFO : mbkfree(scanptype -> DATA) ;
	                  del = 1 ;
                          break ;
	default :
#ifdef MBK_TRACE_BAD_PTYPE
	  fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n",scanptype -> TYPE) ;
#endif
          break;
	}

      if(del)
	{
	  if(prev == NULL)
	    {
	      ptlores -> USER = next ;
	    }
	  else
	    {
	      prev -> NEXT = next ;
	    }

	  scanptype -> NEXT = NULL ;
	  freeptype(scanptype ) ;
	}
      else
	{
	  prev = scanptype ;
	}
    }
}

/*********************************** Inductor : add ********************************************/

loself_list *addloself(lofig_list *ptfig,char type,double self,losig_list *ptscon1,
                     losig_list *ptscon2,char *name)
{
  static char *scon1 = NULL ;
  static char *scon2 = NULL ;
  loself_list  *ptself  = NULL ;
  locon_list  *ptcon  = NULL ;

  scon1 = namealloc("scon1");
  scon2 = namealloc("scon2");

  if(type != SELFMIM)
    {
      (void)fflush(stdout) ;
      (void)fprintf(stderr, "*** mbk error ***\n") ;
      (void)fprintf(stderr, "  illegal inductor type : %ld\n",(long)type) ;
      EXIT(1) ;
    }

   ptself = (loself_list *)mbkalloc(sizeof(loself_list)) ;
   ptself -> TYPE  = type ;
   ptself -> SELF  = self ;
   ptself -> NEXT  = ptfig -> LOSELF ;
   ptfig -> LOSELF = ptself ;
   ptself -> USER  = NULL ;

   ptcon = (locon_list *)mbkalloc(sizeof(locon_list));
   ptcon -> NAME      = scon1 ;
   ptcon -> SIG       = ptscon1 ;
   ptcon -> ROOT      = (void *)ptself ;
   ptcon -> TYPE      = TRANSCV4 ;
   ptcon -> DIRECTION = INTERNAL ;
   ptcon -> USER      = NULL ;
   ptcon -> PNODE     = NULL ;
   ptself -> SCON1      = ptcon ;
   
   ptcon = (locon_list *)mbkalloc(sizeof(locon_list)) ;
   ptcon -> NAME      = scon2 ;
   ptcon -> SIG       = ptscon2 ;
   ptcon -> ROOT      = (void *)ptself ;
   ptcon -> TYPE      = TRANSCV4 ;
   ptcon -> DIRECTION = INTERNAL ;
   ptcon -> USER      = NULL ;
   ptcon -> PNODE     = NULL ;
   ptself -> SCON2      = ptcon ;
   
   /* No check is done for inductor name unicity */
   ptself -> NAME = namealloc(name) ;

   if(TRACE_MODE == 'Y')
     {
       (void)fprintf(stdout,"--- mbk --- addloself  : %s\n",type == SELFMIM ? "SELFMIM" : "SELFMIM") ;
     }

   return ptself ;
}

/*********************************** Inductor : del ********************************************/

int delloself(lofig_list *ptfig,loself_list  *ptself)
{
  loself_list  *pt    = NULL ;
  loself_list  *ptsav = NULL ;

  for(pt = ptfig -> LOSELF ; pt != NULL ; pt = pt -> NEXT)
    {
      if(pt == ptself)
	{
          break ;
	}

      ptsav = pt ;
   }

  if(pt == NULL)
    {
      return 0 ;
    }
  else
    {
      if(pt == ptfig -> LOSELF)
	{
          ptfig -> LOSELF = pt -> NEXT ;
	}
      else
	{
          ptsav -> NEXT = pt -> NEXT ;
	}
    }

  if(pt -> SCON1 -> PNODE)
    {
      delrcnlocon(pt -> SCON1) ;
    }

  delloconuser(pt -> SCON1) ;
  mbkfree((void *)pt -> SCON1) ;

  if(pt -> SCON2 -> PNODE)
    {
      delrcnlocon(pt -> SCON2) ;
    }

  delloconuser(pt -> SCON2) ;
  mbkfree((void *)pt -> SCON2) ;

  delloselfuser(pt) ;
  mbkfree((void *)pt) ;

  if(TRACE_MODE == 'Y')
    {
      (void)fprintf(stdout, "--- mbk --- delloself  : \n") ;
    }

  return 1 ;
}  

/*********************************** Inductor : getloself ***************************************/

loself_list *getloself(lofig_list *ptfig, const char *cname)
{
  loself_list *ptself = NULL ;
  char       *name  = namealloc(cname) ;

  for(ptself = ptfig -> LOSELF ; ptself != NULL ; ptself = ptself -> NEXT)
    {
      if(ptself -> NAME == name)
	{
          return ptself ;
	}
    }

  (void)fflush(stdout) ;
  (void)fprintf(stderr,"*** mbk error ***\n") ;
  (void)fprintf(stderr,"getloself impossible :\n") ;
  (void)fprintf(stderr,"inductor %s doesn't exist in figure %s\n",name,ptfig -> NAME) ;
  EXIT(1) ;

  return NULL ; /* never reached */
}

/*********************************** Inductor : setloself **************************************/

void setloself(lofig_list *ptfig, const char *cname,double self)
{
  loself_list *ptself  = NULL ;
  loself_list *theself = NULL ;
  char        *name    = namealloc(cname) ;

  for(ptself = ptfig -> LOSELF ; ptself != NULL ; ptself = ptself -> NEXT)
    {
      if(ptself -> NAME == name)
	{
          theself = ptself ;
          break ;
	}
    }

  if(theself == NULL)
    {
      (void)fflush(stdout) ;
      (void)fprintf(stderr,"*** mbk error ***\n") ;
      (void)fprintf(stderr,"setloself impossible :\n") ;
      (void)fprintf(stderr,"inductor %s doesn't exist in figure %s\n",name,ptfig -> NAME) ;
      EXIT(1) ;
    }

  theself -> SELF = self ;
}

/*********************************** Inductor : viewloself ***************************************/

void viewloself(loself_list *ptself)
{ 
  (void)printf("   |---inductor\n") ;
  (void)printf("   |   |---type  : %s\n",IsSelfMIM(ptself->TYPE)?"MIM":"POLY/NWELL") ;

  if(ptself -> NAME != NULL)
    {
      (void)printf("   |   |---name : %s\n", ptself -> NAME) ;
    }
  else
    {
      (void)printf("   |   |---no name\n") ;
    }

  if(ptself -> SCON1 == NULL)
    {
      (void)printf("   |   |--- !!! no signal on first connector !!! \n") ;
    }
  else
    {
      viewloinscon(ptself -> SCON1) ;
    }

  if(ptself -> SCON2 == NULL)
    {
      (void)printf("   |   |--- !!! no signal on second connector !!! \n") ;
    }
  else
    {
      viewloinscon(ptself -> SCON2) ;
    }

   (void)printf("   |   |---self   : %g\n", ptself -> SELF) ;

  if(ptself -> USER != NULL)
    {
      (void)printf("   |   |---non empty USER field\n") ;
    }

   (void)printf("   |\n") ;
}

/*********************************** Inductor : delloselfuser ***********************************/

void delloselfuser(loself_list *ptloself)
{
  ptype_list *scanptype = NULL ;
  ptype_list *next      = NULL ;
  ptype_list *prev      = NULL ;
  long         del       = 0    ;
 
  for(scanptype = ptloself -> USER ; scanptype != NULL ; scanptype = next)
    {
      next = scanptype -> NEXT ;
      del = 0 ;

      switch(scanptype -> TYPE)
	{
	default :
#ifdef MBK_TRACE_BAD_PTYPE
	  fprintf( stderr, "WARNING in delloconuser() : unknown ptype %ld\n",scanptype -> TYPE) ;
#endif
          break;
	}

      if(del)
	{
	  if(prev == NULL)
	    {
	      ptloself -> USER = next ;
	    }
	  else
	    {
	      prev -> NEXT = next ;
	    }

	  scanptype -> NEXT = NULL ;
	  freeptype(scanptype ) ;
	}
      else
	{
	  prev = scanptype ;
	}
    }
}

