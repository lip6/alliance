/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Purpose : utilites functions for physical view
 * Date    : 06/03/92
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 */

#ident "$Id: mbk_ph_util.c,v 1.1 2002/03/08 13:51:05 fred Exp $"

#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "mut.h"
#include "mph.h"
#include "mpu.h"
#include "mbk_ph_util.h"

/*******************************************************************************
* fonction getphfig                                                            *
* if mode == 'A' all the figure is requested                                   *
* if mode != 'A' interface only is requested                                   *
*******************************************************************************/
phfig_list *getphfig(const char *fname, char mode)
{
phfig_list   *ptfig;
char *figname = namealloc(fname);

   /* scan figure_list   */
   for (ptfig = HEAD_PHFIG; ptfig; ptfig = ptfig->NEXT)
      if (ptfig->NAME == figname)
         break;

   if (ptfig == NULL) { /* figure doesn't exist */
      ptfig = (phfig_list *)mbkalloc(sizeof(phfig_list));
      ptfig->NAME       = figname;
      ptfig->MODELCHAIN = NULL;
      ptfig->PHINS      = NULL;
      ptfig->PHCON      = NULL;
      ptfig->PHVIA      = NULL;
      ptfig->PHREF      = NULL;
      ptfig->PHSEG      = NULL;
      ptfig->USER       = NULL;
      ptfig->XAB1     = 0;
      ptfig->XAB1     = 0;
      ptfig->XAB2     = 0;
      ptfig->YAB1     = 0;
      ptfig->YAB2     = 0;
      ptfig->NEXT       = HEAD_PHFIG;
      HEAD_PHFIG  = ptfig;
      if (mode == 'A') {
         ptfig->MODE = 'A';
         loadphfig(ptfig, figname, 'A');
         return ptfig;
      } else {
         ptfig->MODE = 'P';
         loadphfig(ptfig, figname, 'P');
         return ptfig;
      }
   }
   if (ptfig->MODE != 'A') { /* figure exists interface only */
      if (mode == 'A') {
         ptfig->MODE = 'A';
         loadphfig(ptfig, figname, 'C');
         return ptfig;
      } else
         return ptfig;
   } else           /* figure exists interface and body */
      return ptfig;
}

/*******************************************************************************
* fonction flattenphfig                                                        *
* newtrsf = trsf o ptins->TRANSF
*******************************************************************************/
void flattenphfig(phfig_list *ptfig, const char *insname, char concat)
{
phins_list *ptins = NULL;
phseg_list *ptseg = NULL;
phvia_list *ptvia = NULL;
phref_list *ptref = NULL;
phins_list *ptinstbf = NULL;
phfig_list *ptfigtbf = NULL;
phfig_list *pt = NULL;
long x1, y1, x2, y2;
long xins, yins;
long xab1, yab1, xab2, yab2;
long dx, dy;
long vx1, vx2, vy1, vy2;
char trsf;
char newtrsf;
int  Layer;

   ptinstbf = getphins(ptfig, insname);
   ptfigtbf = getphfig(ptinstbf->FIGNAME, 'A');
   xins = ptinstbf->XINS;
   yins = ptinstbf->YINS;
   xab1 = ptfigtbf->XAB1;
   xab2 = ptfigtbf->XAB2;
   yab1 = ptfigtbf->YAB1;
   yab2 = ptfigtbf->YAB2;
   trsf = ptinstbf->TRANSF;

   /*  segments  */
   for (ptseg = ptfigtbf->PHSEG; ptseg; ptseg = ptseg->NEXT) {
      xyflat(&x1, &y1, ptseg->X1, ptseg->Y1, xins, yins,
               xab1, yab1, xab2, yab2, trsf);
      xyflat(&x2, &y2, ptseg->X2, ptseg->Y2, xins, yins,
               xab1, yab1, xab2, yab2, trsf);
      /* we must take care of the segment orientation at that time.
         dx and dy are just temporary variables here, no semantic meaning
         for sure! */
      switch (trsf) {
         case NOSYM :
         case SYMXY :
            switch (ptseg->TYPE) {
               case UP:
               case DOWN:  dy = y1; y1 = y2; y2 = dy; break;
               case RIGHT:
               case LEFT:  dx = x1; x1 = x2; x2 = dx; break;
            }
         case ROT_M :
         case ROT_P :
            switch (ptseg->TYPE) {
               case UP:
               case DOWN:  dx = x1; x1 = x2; x2 = dx; break;
               case RIGHT:
               case LEFT:  dy = y1; y1 = y2; y2 = dy; break;
            }
         case SYM_X :
         case SYM_Y :
            switch (ptseg->TYPE) {
               case UP:    dy = y1; y1 = y2; y2 = dy; break;
               case RIGHT: dx = x1; x1 = x2; x2 = dx; break;
               case DOWN:
               case LEFT: break;
            }
         case SY_RP :
         case SY_RM :
            switch (ptseg->TYPE) {
               case UP:    dx = x1; x1 = x2; x2 = dx; break;
               case RIGHT: dy = y1; y1 = y2; y2 = dy; break;
               case LEFT:
               case DOWN: break;
            }
      }
    Layer = ptseg->LAYER;

    if ( Layer == CALU1 ) Layer = ALU1;
    else
    if ( Layer == CALU2 ) Layer = ALU2;
    else
    if ( Layer == CALU3 ) Layer = ALU3;
    else
    if ( Layer == CALU4 ) Layer = ALU4;
    else
    if ( Layer == CALU5 ) Layer = ALU5;
    else
    if ( Layer == CALU6 ) Layer = ALU6;
    else
    if ( Layer == CALU7 ) Layer = ALU7;
    else
    if ( Layer == CALU8 ) Layer = ALU8;
    else
    if ( Layer == CALU9 ) Layer = ALU9;

      if (ptseg->NAME != NULL)
         (void)addphseg(ptfig, Layer, ptseg->WIDTH, x1, y1, x2, y2, 
                        concat == NO ? 
                           ptseg->NAME : concatname(insname, ptseg->NAME));
      else
         (void)addphseg(ptfig, Layer, ptseg->WIDTH, x1, y1, x2, y2,
                        (char *)NULL);
   }

   /*   vias     */
   for (ptvia = ptfigtbf->PHVIA; ptvia; ptvia = ptvia->NEXT) {
      dx = ptvia->DX >> 1;
      dy = ptvia->DY >> 1;
      vx1 = ptvia->XVIA - dx;
      vy1 = ptvia->YVIA - dy;
      vx2 = ptvia->XVIA + dx;
      vy2 = ptvia->YVIA + dy;
      xyflat(&x1, &y1, vx1, vy1, xins, yins, xab1, yab1, xab2, yab2, trsf);
      xyflat(&x2, &y2, vx2, vy2, xins, yins, xab1, yab1, xab2, yab2, trsf);

      if ( x1 > x2 ) { vx1 = x2; vx2 = x1; }
      else           { vx1 = x1; vx2 = x2; }
      
      if ( y1 > y2 ) { vy1 = y2; vy2 = y1; }
      else           { vy1 = y1; vy2 = y2; }

      dx = vx2 - vx1;
      dy = vy2 - vy1;

    if (ptvia->NAME != NULL)
        (void)addphvia(ptfig, ptvia->TYPE, vx1 + (dx >> 1), vy1 + (dy >> 1), dx, dy,
                        concat == NO ?  ptvia->NAME : concatname(insname, ptvia->NAME));
    else
        (void)addphvia(ptfig, ptvia->TYPE, vx1 + (dx >> 1), vy1 + (dy >> 1), dx, dy, NULL );
   }

   /*   references     */
   for (ptref = ptfigtbf->PHREF; ptref; ptref = ptref->NEXT) {
      /* ref_con :
         since ref_con is connector like, I shall not copy it up! */
      if (!strcmp(ptref->FIGNAME, "ref_con"))
         continue;
      xyflat(&x1, &y1, ptref->XREF, ptref->YREF, xins, yins,
               xab1, yab1, xab2, yab2, trsf);
      if (ptref->NAME != NULL)
         (void)addphref(ptfig, ptref->FIGNAME, 
                        concat == NO ? 
                           ptref->NAME : 
                           concatname(insname, ptref->NAME), x1, y1);
      else
         (void)addphref(ptfig, ptref->FIGNAME, (char *)NULL, x1, y1);
   }

   /* instances */
   for (ptins = ptfigtbf->PHINS; ptins; ptins = ptins->NEXT) {
      xyflat(&x1, &y1, ptins->XINS, ptins->YINS, xins, yins,
               xab1, yab1, xab2, yab2, trsf);
      pt = getphfig(ptins->FIGNAME, 'P');
      dx = pt->XAB2 - pt->XAB1;
      dy = pt->YAB2 - pt->YAB1;
      switch (trsf) {
         case NOSYM :
            newtrsf = ptins->TRANSF;
            break;
         case SYM_X :
            switch (ptins->TRANSF) {
               case NOSYM : newtrsf = SYM_X; x1 -= dx; break;
               case SYM_X : newtrsf = NOSYM; x1 -= dx; break;
               case SYM_Y : newtrsf = SYMXY; x1 -= dx; break;
               case SYMXY : newtrsf = SYM_Y; x1 -= dx; break;
               case ROT_P : newtrsf = SY_RP; x1 -= dy; break;
               case ROT_M : newtrsf = SY_RM; x1 -= dy; break;
               case SY_RP : newtrsf = ROT_P; x1 -= dy; break;
               case SY_RM : newtrsf = ROT_M; x1 -= dy; break;
            }
            break;
         case SYM_Y :
            switch (ptins->TRANSF) {
               case NOSYM : newtrsf = SYM_Y; y1 -= dy; break;
               case SYM_X : newtrsf = SYMXY; y1 -= dy; break;
               case SYM_Y : newtrsf = NOSYM; y1 -= dy; break;
               case SYMXY : newtrsf = SYM_X; y1 -= dy; break;
               case ROT_P : newtrsf = SY_RM; y1 -= dx; break;
               case ROT_M : newtrsf = SY_RP; y1 -= dx; break;
               case SY_RP : newtrsf = ROT_M; y1 -= dx; break;
               case SY_RM : newtrsf = ROT_P; y1 -= dx; break;
            }
            break;
         case SYMXY :
            switch (ptins->TRANSF) {
               case NOSYM : newtrsf = SYMXY; x1 -= dx; y1 -= dy; break;
               case SYM_X : newtrsf = SYM_Y; x1 -= dx; y1 -= dy; break;
               case SYM_Y : newtrsf = SYM_X; x1 -= dx; y1 -= dy; break;
               case SYMXY : newtrsf = NOSYM; x1 -= dx; y1 -= dy; break;
               case ROT_P : newtrsf = ROT_M; x1 -= dy; y1 -= dx; break;
               case ROT_M : newtrsf = ROT_P; x1 -= dy; y1 -= dx; break;
               case SY_RP : newtrsf = SY_RM; x1 -= dy; y1 -= dx; break;
               case SY_RM : newtrsf = SY_RP; x1 -= dy; y1 -= dx; break;
            }
            break;
         case ROT_P :
            switch (ptins->TRANSF) {
               case NOSYM : newtrsf = ROT_P; x1 -= dy; break;
               case SYM_X : newtrsf = SY_RM; x1 -= dy; break;
               case SYM_Y : newtrsf = SY_RP; x1 -= dy; break;
               case SYMXY : newtrsf = ROT_M; x1 -= dy; break;
               case ROT_P : newtrsf = SYMXY; x1 -= dx; break;
               case ROT_M : newtrsf = NOSYM; x1 -= dx; break;
               case SY_RP : newtrsf = SYM_X; x1 -= dx; break;
               case SY_RM : newtrsf = SYM_Y; x1 -= dx; break;
            }
            break;
         case ROT_M :
            switch (ptins->TRANSF) {
               case NOSYM : newtrsf = ROT_M; y1 -= dx; break;
               case SYM_X : newtrsf = SY_RP; y1 -= dx; break;
               case SYM_Y : newtrsf = SY_RM; y1 -= dx; break;
               case SYMXY : newtrsf = ROT_P; y1 -= dx; break;
               case ROT_P : newtrsf = NOSYM; y1 -= dy; break;
               case ROT_M : newtrsf = SYMXY; y1 -= dy; break;
               case SY_RP : newtrsf = SYM_Y; y1 -= dy; break;
               case SY_RM : newtrsf = SYM_X; y1 -= dy; break;
            }
            break;
         case SY_RP :
            switch (ptins->TRANSF) {
               case NOSYM : newtrsf = SY_RP; break;
               case SYM_X : newtrsf = ROT_M; break;
               case SYM_Y : newtrsf = ROT_P; break;
               case SYMXY : newtrsf = SY_RM; break;
               case ROT_P : newtrsf = SYM_Y; break;
               case ROT_M : newtrsf = SYM_X; break;
               case SY_RP : newtrsf = NOSYM; break;
               case SY_RM : newtrsf = SYMXY; break;
            }
            break;
         case SY_RM :
            switch (ptins->TRANSF) {
               case NOSYM : newtrsf = SY_RM; x1 -= dy; y1 -= dx; break;
               case SYM_X : newtrsf = ROT_P; x1 -= dy; y1 -= dx; break;
               case SYM_Y : newtrsf = ROT_M; x1 -= dy; y1 -= dx; break;
               case SYMXY : newtrsf = SY_RP; x1 -= dy; y1 -= dx; break;
               case ROT_P : newtrsf = SYM_Y; x1 -= dx; y1 -= dy; break;
               case ROT_M : newtrsf = SYM_X; x1 -= dx; y1 -= dy; break;
               case SY_RP : newtrsf = SYMXY; x1 -= dx; y1 -= dy; break;
               case SY_RM : newtrsf = NOSYM; x1 -= dx; y1 -= dy; break;
            }
            break;
      }
      (void)addphins(ptfig, ptins->FIGNAME, concat == NO ? ptins->INSNAME :
                     concatname(insname, ptins->INSNAME), newtrsf, x1, y1);
   }
   (void)delphins(ptfig, insname);
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout, "--- mbk --- flatten_phfig \n");
}

/*******************************************************************************
* fonction loadphfig                                                           *
*******************************************************************************/
void loadphfig(phfig_list *ptfig, const char *figname, char mode)
{
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout,
                     "--- mbk --- loadphfig : reading file %s.%s mode %c\n",
                     figname, IN_PH, mode);
   if (!strcmp(IN_PH, "cp"))
      vtiloadphfig(ptfig, figname, mode);
   else if (!strcmp(IN_PH, "ap"))
      alcloadphfig(ptfig, figname, mode);
   else if (!strcmp(IN_PH, "mg"))
      mgnloadphfig(ptfig, figname, mode);
   else {
      (void)fflush(stdout);
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "Unknown physical input format %s\n", IN_PH);
      EXIT(1);
   }
   if (TRACE_MODE == 'Y')
      (void)fprintf(stdout,
                     "--- mbk --- loadphfig : done reading file %s.%s\n",
                     figname, IN_PH);
}

/*******************************************************************************
* fonction  savephfig                                                          *
*******************************************************************************/
void savephfig(phfig_list *ptfig)
{
   if (!strcmp(OUT_PH, "cp"))
      vtisavephfig(ptfig);
   else if (!strcmp(OUT_PH, "ap"))
      alcsavephfig(ptfig);
   else if (!strcmp(OUT_PH, "mg"))
      mgnsavephfig(ptfig);
   else {
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "Unknown physical output format %s\n", OUT_PH);
      EXIT(1);
   }
}

/*******************************************************************************
* function rflattenphfig                                                       *
*******************************************************************************/
void rflattenphfig(phfig_list *ptfig, char concat, char catal)
{
struct phins *p;

   catal = catal == NO ? 0 : 1;
   for (p = ptfig->PHINS; p != NULL;) {
      if (!catal || !incatalog(p->FIGNAME) || incatalogdelete(p->FIGNAME)) {
         flattenphfig(ptfig, p->INSNAME, concat);
         p = ptfig->PHINS;
      }
   else
      p = p->NEXT;
   }
}

/*******************************************************************************
* function instanceface                                                        *
* return the orientation of a model connector knowing its face and symetry     *
*******************************************************************************/
char instanceface(face, sym)
char face, sym;
{
   switch (sym) {
      case NOSYM :
         return face;
      case ROT_P :
         switch (face) {
            case NORTH :
               return WEST;
            case EAST :
               return NORTH;
            case SOUTH :
               return EAST;
            case WEST :
               return SOUTH;
         }
      case SYMXY :
         switch (face) {
            case NORTH :
               return SOUTH;
            case EAST :
               return WEST;
            case SOUTH :
               return NORTH;
            case WEST :
               return EAST;
         }
      case ROT_M :
         switch (face) {
            case NORTH :
               return EAST;
            case EAST :
               return SOUTH;
            case SOUTH :
               return WEST;
            case WEST :
               return NORTH;
         }
      case SYM_X :
         switch (face) {
            case NORTH :
               return NORTH;
            case EAST :
               return WEST;
            case SOUTH :
               return SOUTH;
            case WEST :
               return EAST;
         }
      case SY_RM :
         switch (face) {
            case NORTH :
               return WEST;
            case EAST :
               return SOUTH;
            case SOUTH :
               return EAST;
            case WEST :
               return NORTH;
         }
      case SYM_Y :
         switch (face) {
            case NORTH :
               return SOUTH;
            case EAST :
               return EAST;
            case SOUTH :
               return NORTH;
            case WEST :
               return WEST;
         }
      case SY_RP :
         switch (face) {
            case NORTH :
               return EAST;
            case EAST :
               return NORTH;
            case SOUTH :
               return WEST;
            case WEST :
               return SOUTH;
         }
   }

   (void)fflush(stdout);
   (void)fprintf(stderr, "*** mbk error ***\n");
   (void)fprintf(stderr, "instanceface impossible :\n");
   (void)fprintf(stderr, "face: %c, sym %c\n", face, sym);
   EXIT(1);
   return (char)NULL; /* never reached */
}

#define BV_VIA_VIA    6 /* must be even, whatever!                */
                        /* 99/11/9 change from 4 to 6 because of new rules */
#define BV_VIASIZE    3 /* design rule for equipotential vias actually 2 but somestimes 3 */
#define BV_SLAYERSIZE 2 /* size of the small layer in the contact */
#define BV_BLAYERSIZE 2 /* size of the big layer in the contact   */
#define MAX(a,b) ((a) > (b) ? (a) : (b))
/*******************************************************************************
* this creates a mega-via, the best way we can do it, I hope                   *
* the envelop of the bigvia is calculated, and then its center is placed       *
* as close as possible of the x, y coordinates given as arguments.             *
*******************************************************************************/
void bigvia(f, via, x, y, dx, dy)
phfig_list *f;
char via;
long x, y, dx, dy;
{
int i, j;
long stepx, stepy, xv, yv, dxv, dyv;
char slayer, blayer; /* small and big layers */

   if (dx < 0 || dy < 0) {
      fflush(stdout);
      fprintf(stderr, "*** mbk error ***\n bigvia impossible :");
      fprintf(stderr, " negative values dx = %ld, dy = %ld\n", dx, dy);
      fprintf(stderr, "in figure '%s' at (%ld, %ld)\n", f->NAME, x, y);
      EXIT(1);
   }
   dy = (dy < (BV_SLAYERSIZE*SCALE_X)) ? BV_SLAYERSIZE*SCALE_X : dy;
   dx = (dx < (BV_BLAYERSIZE*SCALE_X)) ? BV_BLAYERSIZE*SCALE_X : dx;
   switch (via) {
      case CONT_POLY :
         slayer = ALU1;
         blayer = POLY;
         break;
      case CONT_POLY2 :
         slayer = ALU1;
         blayer = POLY2;
         break;
      case CONT_VIA :
         slayer = ALU1;
         blayer = ALU2;
         break;
      case CONT_VIA2 :
         slayer = ALU2;
         blayer = ALU3;
         break;
      case CONT_VIA3 :
         slayer = ALU3;
         blayer = ALU4;
         break;
      case CONT_VIA4 :
         slayer = ALU4;
         blayer = ALU5;
         break;
      case CONT_VIA5 :
         slayer = ALU5;
         blayer = ALU6;
         break;
      case CONT_VIA6 :
         slayer = ALU6;
         blayer = ALU7;
         break;
      case CONT_VIA7 :
         slayer = ALU7;
         blayer = ALU8;
         break;
      case CONT_VIA8 :
         slayer = ALU8;
         blayer = ALU9;
         break;
      case CONT_DIF_N :
         slayer = ALU1;
         blayer = NDIF;
         break;
      case CONT_DIF_P :
         slayer = ALU1;
         blayer = PDIF;
         break;
      case CONT_BODY_N :
         slayer = ALU1;
         blayer = NTIE;
         break;
      case CONT_BODY_P :
         slayer = ALU1;
         blayer = PTIE;
         break;
      default :; /* error later on */
   }
   /* euclidian division :
      dividing and then multipliing doesn't give the initial value.
      I calculate the center of the bunch of vias and adjust it to the given
      x, y coordinates. */
   stepx = (dx - BV_VIASIZE * SCALE_X) / (BV_VIA_VIA * SCALE_X);
   stepy = (dy - BV_VIASIZE * SCALE_X) / (BV_VIA_VIA * SCALE_X);
   dxv = stepx * BV_VIA_VIA * SCALE_X; 
   dyv = stepy * BV_VIA_VIA * SCALE_X; 
   xv = x - dxv / 2;
   yv = y - dyv / 2;
   /* draw the vias */
   for (i = 0; i <= stepx; i++)
      for (j = 0; j <= stepy; j++)
         addphvia(f, via, xv + i * BV_VIA_VIA * SCALE_X,
                     yv + j * BV_VIA_VIA * SCALE_X, 0, 0, NULL);
   /* fill the leftover space :
      we assume that layer extension is less or equal to the half size of the
      layer in the contact. */
   if (dy) {
/*
 * if (dyv) {
 *    addphseg(f, slayer, dxv + BV_SLAYERSIZE * SCALE_X,
 *             x, yv, x, yv + dyv, NULL);
 *    addphseg(f, blayer, dxv + BV_BLAYERSIZE * SCALE_X,
 *             x, yv, x, yv + dyv, NULL);
 */
      addphseg(f, slayer, dx, 
                x, y - dy/2 + BV_SLAYERSIZE * SCALE_X / 2, 
                x, y + dy/2 - BV_SLAYERSIZE * SCALE_X / 2, NULL);
      addphseg(f, blayer, dx, 
                          x, y - dy/2 + BV_BLAYERSIZE * SCALE_X / 2, 
                x, y + dy/2 - BV_BLAYERSIZE * SCALE_X / 2, NULL);
   }
   if (dx) {
/*
 * if (dxv) {
 *    addphseg(f, slayer, dyv + BV_SLAYERSIZE * SCALE_X,
 *             xv, y, xv + dxv, y, NULL);
 *    addphseg(f, blayer, dyv + BV_BLAYERSIZE * SCALE_X,
 *             xv, y, xv + dxv, y, NULL);
 */
      addphseg(f, slayer, dy, 
                          x - dx/2 + BV_SLAYERSIZE * SCALE_X / 2, y, 
                          x + dx/2 - BV_SLAYERSIZE * SCALE_X / 2, y, NULL);
      addphseg(f, blayer, dy, 
                               x - dx/2 + BV_BLAYERSIZE * SCALE_X / 2, y, 
                          x + dx/2 - BV_BLAYERSIZE * SCALE_X / 2, y, NULL);
   }
}
#undef BV_VIA_VIA
#undef BV_VIASIZE
#undef BV_SLAYERSIZE
#undef BV_BLAYERSIZE
#undef MAX

/*******************************************************************************
* User interactive on-line debugger, for the one unaware of dbx use            *
*******************************************************************************/
struct typoin           /* structure used by dast_dbg */
  {               /* to store its state      */
  short  type;          /* code of the current structure*/
  short  mark;          /* stop mark         */
  void  *data;          /* pointer of the structure   */
  };

#define VHD_ERRDFN  -1
#define VHD__XTDFN  0
#define VHD__UPDFN  1
#define VHD__TPDFN  2
#define VHD__SPDFN  3
#define VHD__BKDFN  4

#define VHD_phfigDFN 5
#define VHD_nextDFN 6
#define VHD_modelchainDFN 7
#define VHD_phsegDFN 8
#define VHD_phviaDFN 9
#define VHD_phrefDFN 10
#define VHD_phinsDFN 11
#define VHD_phconDFN 12
#define VHD_nameDFN 13
#define VHD_modeDFN 14
#define VHD_xab1DFN 15
#define VHD_yab1DFN 16
#define VHD_xab2DFN 17
#define VHD_yab2DFN 18
#define VHD_userDFN 19
#define VHD_indexDFN 20
#define VHD_xconDFN 21
#define VHD_yconDFN 22
#define VHD_widthDFN 23
#define VHD_orientDFN 24
#define VHD_layerDFN 25
#define VHD_x1DFN 26
#define VHD_y1DFN 27
#define VHD_x2DFN 28
#define VHD_y2DFN 29
#define VHD_typeDFN 30
#define VHD_xviaDFN 31
#define VHD_yviaDFN 32
#define VHD_fignameDFN 33
#define VHD_xrefDFN 34
#define VHD_yrefDFN 35
#define VHD_insnameDFN 36
#define VHD_xinsDFN 37
#define VHD_yinsDFN 38
#define VHD_transfDFN 39
#define VHD_ptypeDFN 40
#define VHD_dataDFN 41

#define VHD_MAXDFN 42

static int vhd_getcmd();
static int vhd_hash();

void mphdebug(head_pnt,stru_name)

void  *head_pnt;
char  *stru_name;

  {

  int     i;
  int     state;
  int     newsta0;
  int     readcmd = 0;
  char       readtab[3][20];
  int     stk_pnt = 0;
  int            type = VHD__XTDFN;
  struct chain  *ptchain;

  struct typoin  current_pnt;
  struct typoin  stk[200];

  char           empty[4];
  char           avail[12];

  int     key[VHD_MAXDFN];
  char      *tab[VHD_MAXDFN];
  void      *nxt[VHD_MAXDFN];
  short      typ[VHD_MAXDFN];

  struct phfig *phfig_pnt;
  struct phseg *phseg_pnt;
  struct phvia *phvia_pnt;
  struct phref *phref_pnt;
  struct phins *phins_pnt;
  struct phcon *phcon_pnt;
  struct ptype *ptype_pnt;

  key[VHD_phfigDFN]      = vhd_hash ("phfig");
  key[VHD_nextDFN]       = vhd_hash ("next");
  key[VHD_modelchainDFN] = vhd_hash ("modelchain");
  key[VHD_phsegDFN]      = vhd_hash ("phseg");
  key[VHD_phviaDFN]      = vhd_hash ("phvia");
  key[VHD_phrefDFN]      = vhd_hash ("phref");
  key[VHD_phinsDFN]      = vhd_hash ("phins");
  key[VHD_phconDFN]      = vhd_hash ("phcon");
  key[VHD_nameDFN]       = vhd_hash ("name");
  key[VHD_modeDFN]       = vhd_hash ("mode");
  key[VHD_xab1DFN]       = vhd_hash ("xab1");
  key[VHD_yab1DFN]       = vhd_hash ("yab1");
  key[VHD_xab2DFN]       = vhd_hash ("xab2");
  key[VHD_yab2DFN]       = vhd_hash ("yab2");
  key[VHD_userDFN]       = vhd_hash ("user");
  key[VHD_indexDFN]      = vhd_hash ("index");
  key[VHD_xconDFN]       = vhd_hash ("xcon");
  key[VHD_yconDFN]       = vhd_hash ("ycon");
  key[VHD_widthDFN]      = vhd_hash ("width");
  key[VHD_orientDFN]     = vhd_hash ("orient");
  key[VHD_layerDFN]      = vhd_hash ("layer");
  key[VHD_x1DFN]         = vhd_hash ("x1");
  key[VHD_y1DFN]         = vhd_hash ("y1");
  key[VHD_x2DFN]         = vhd_hash ("x2");
  key[VHD_y2DFN]         = vhd_hash ("y2");
  key[VHD_typeDFN]       = vhd_hash ("type");
  key[VHD_xviaDFN]       = vhd_hash ("xvia");
  key[VHD_yviaDFN]       = vhd_hash ("yvia");
  key[VHD_fignameDFN]    = vhd_hash ("figname");
  key[VHD_xrefDFN]       = vhd_hash ("xref");
  key[VHD_yrefDFN]       = vhd_hash ("yref");
  key[VHD_insnameDFN]    = vhd_hash ("insname");
  key[VHD_xinsDFN]       = vhd_hash ("xins");
  key[VHD_yinsDFN]       = vhd_hash ("yins");
  key[VHD_transfDFN]     = vhd_hash ("transf");
  key[VHD_ptypeDFN]      = vhd_hash ("ptype");
  key[VHD_dataDFN]       = vhd_hash ("data");

   /* ###------------------------------------------------------### */
   /*    Set of predefined commands          */
   /* ###------------------------------------------------------### */

  key[0] = vhd_hash ("_exit");
  key[1] = vhd_hash ("_up");
  key[2] = vhd_hash ("_top");
  key[3] = vhd_hash ("_stop");
  key[4] = vhd_hash ("_back");

  (void)strcpy (empty,"");
  (void)strcpy (avail,"AVAILABLE");

  for (i=0 ; i<VHD_MAXDFN ; i++)
    typ[i] = i;

   /* ###------------------------------------------------------### */
   /*    Find the type of the head structure       */
   /* ###------------------------------------------------------### */

  readcmd = vhd_hash (stru_name);
  for (i=0 ; i<VHD_MAXDFN ; i++)
    {
    if (readcmd == key[i])
      {
      type = typ[i];
      break;
      }
    }

   /* ###------------------------------------------------------### */
   /*    Exit if the head structure is empty       */
   /* ###------------------------------------------------------### */

  if (head_pnt == NULL)
    type = VHD__XTDFN;

  current_pnt.data = head_pnt;
  current_pnt.type = type;
  state        = type;

  while (state != VHD__XTDFN)
    {
   /* ###------------------------------------------------------### */
   /*    Print structure's field until the exit command is read   */
   /* ###------------------------------------------------------### */

    for (i=0 ; i<VHD_MAXDFN ; i++)
      {
      tab[i] = empty;
      nxt[i] = NULL;
      }

   /* ###------------------------------------------------------### */
   /*    _exit and _stop commands are allways available     */
   /* ###------------------------------------------------------### */

    tab[VHD__XTDFN] = avail;
    tab[VHD__SPDFN] = avail;

   /* ###------------------------------------------------------### */
   /*    _up, _top, and _back commands are available only if the  */
   /* stack is not empty                  */
   /* ###------------------------------------------------------### */

    if (stk_pnt != 0)
      {
      tab[VHD__UPDFN] = avail;
      tab[VHD__TPDFN] = avail;
      tab[VHD__BKDFN] = avail;
      }

    switch (state)
      {

    case (VHD_phfigDFN):

      /* ###--------- phfig ---------### */

      phfig_pnt = (struct phfig *)(current_pnt.data);

      if (phfig_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)phfig_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_phfigDFN;
        }
      if (phfig_pnt->PHSEG != NULL)
        {
        tab[VHD_phsegDFN] = avail;
        nxt[VHD_phsegDFN] = (void *)phfig_pnt->PHSEG;
        typ[VHD_phsegDFN] = VHD_phsegDFN;
        }
      if (phfig_pnt->PHVIA != NULL)
        {
        tab[VHD_phviaDFN] = avail;
        nxt[VHD_phviaDFN] = (void *)phfig_pnt->PHVIA;
        typ[VHD_phviaDFN] = VHD_phviaDFN;
        }
      if (phfig_pnt->PHREF != NULL)
        {
        tab[VHD_phrefDFN] = avail;
        nxt[VHD_phrefDFN] = (void *)phfig_pnt->PHREF;
        typ[VHD_phrefDFN] = VHD_phrefDFN;
        }
      if (phfig_pnt->PHINS != NULL)
        {
        tab[VHD_phinsDFN] = avail;
        nxt[VHD_phinsDFN] = (void *)phfig_pnt->PHINS;
        typ[VHD_phinsDFN] = VHD_phinsDFN;
        }
      if (phfig_pnt->PHCON != NULL)
        {
        tab[VHD_phconDFN] = avail;
        nxt[VHD_phconDFN] = (void *)phfig_pnt->PHCON;
        typ[VHD_phconDFN] = VHD_phconDFN;
        }
      if (phfig_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)phfig_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   name        : %s\n",phfig_pnt->NAME);
      (void)printf ("   modelchain  :\n");
      ptchain = phfig_pnt->MODELCHAIN;
      while (ptchain != NULL)
        {
        (void)printf ("                 %s\n",(char *)ptchain->DATA);
        ptchain = ptchain->NEXT;
        }
      (void)printf ("-> phseg       : %s\n",tab[VHD_phsegDFN]);
      (void)printf ("-> phvia       : %s\n",tab[VHD_phviaDFN]);
      (void)printf ("-> phref       : %s\n",tab[VHD_phrefDFN]);
      (void)printf ("-> phins       : %s\n",tab[VHD_phinsDFN]);
      (void)printf ("-> phcon       : %s\n",tab[VHD_phconDFN]);
      (void)printf ("   mode        : %c\n",phfig_pnt->MODE);
      (void)printf ("   yab2        : %ld\n",phfig_pnt->YAB2);
      (void)printf ("   xab2        : %ld\n",phfig_pnt->XAB2);
      (void)printf ("   yab1        : %ld\n",phfig_pnt->YAB1);
      (void)printf ("   xab1        : %ld\n",phfig_pnt->XAB1);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_phconDFN):

      /* ###--------- phcon ---------### */

      phcon_pnt = (struct phcon *)(current_pnt.data);

      if (phcon_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)phcon_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_phconDFN;
        }
      if (phcon_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)phcon_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   name        : %s\n",phcon_pnt->NAME ? phcon_pnt->NAME : "NULL" );
      (void)printf ("   index       : %ld\n",phcon_pnt->INDEX);
      (void)printf ("   ycon        : %ld\n",phcon_pnt->YCON);
      (void)printf ("   xcon        : %ld\n",phcon_pnt->XCON);
      (void)printf ("   width       : %ld\n",phcon_pnt->WIDTH);
      (void)printf ("   orient      : %c\n",phcon_pnt->ORIENT);
      (void)printf ("   layer       : %c\n",phcon_pnt->LAYER);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_phsegDFN):

      /* ###--------- phseg ---------### */

      phseg_pnt = (struct phseg *)(current_pnt.data);

      if (phseg_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)phseg_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_phsegDFN;
        }
      if (phseg_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)phseg_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   name        : %s\n",phseg_pnt->NAME ? phseg_pnt->NAME : "NULL" );
      (void)printf ("   y2          : %ld\n",phseg_pnt->Y2);
      (void)printf ("   x2          : %ld\n",phseg_pnt->X2);
      (void)printf ("   y1          : %ld\n",phseg_pnt->Y1);
      (void)printf ("   x1          : %ld\n",phseg_pnt->X1);
      (void)printf ("   width       : %ld\n",phseg_pnt->WIDTH);
      (void)printf ("   type        : %c\n",phseg_pnt->TYPE);
      (void)printf ("   layer       : %c\n",phseg_pnt->LAYER);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_phviaDFN):

      /* ###--------- phvia ---------### */

      phvia_pnt = (struct phvia *)(current_pnt.data);

      if (phvia_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)phvia_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_phviaDFN;
        }
      if (phvia_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)phvia_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   name        : %s\n",phvia_pnt->NAME ? phvia_pnt->NAME : "NULL" );
      (void)printf ("   type        : %c\n",phvia_pnt->TYPE);
      (void)printf ("   yvia        : %ld\n",phvia_pnt->YVIA);
      (void)printf ("   xvia        : %ld\n",phvia_pnt->XVIA);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_phrefDFN):

      /* ###--------- phref ---------### */

      phref_pnt = (struct phref *)(current_pnt.data);

      if (phref_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)phref_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_phrefDFN;
        }
      if (phref_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)phref_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   figname     : %s\n",phref_pnt->FIGNAME);
      (void)printf ("   name        : %s\n",phref_pnt->NAME ? phref_pnt->NAME : "NULL" );
      (void)printf ("   yref        : %ld\n",phref_pnt->YREF);
      (void)printf ("   xref        : %ld\n",phref_pnt->XREF);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_phinsDFN):

      /* ###--------- phins ---------### */

      phins_pnt = (struct phins *)(current_pnt.data);

      if (phins_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)phins_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_phinsDFN;
        }
      if (phins_pnt->USER != NULL)
        {
        tab[VHD_userDFN] = avail;
        nxt[VHD_userDFN] = (void *)phins_pnt->USER;
        typ[VHD_userDFN] = VHD_ptypeDFN;
        }

      (void)printf ("   insname     : %s\n",phins_pnt->INSNAME);
      (void)printf ("   figname     : %s\n",phins_pnt->FIGNAME);
      (void)printf ("   yins        : %ld\n",phins_pnt->YINS);
      (void)printf ("   xins        : %ld\n",phins_pnt->XINS);
      (void)printf ("   transf      : %c\n",phins_pnt->TRANSF);
      (void)printf ("-> user        : %s\n",tab[VHD_userDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

    case (VHD_ptypeDFN):

      /* ###--------- ptype ---------### */

      ptype_pnt = (struct ptype *)(current_pnt.data);

      if (ptype_pnt->NEXT != NULL)
        {
        tab[VHD_nextDFN] = avail;
        nxt[VHD_nextDFN] = (void *)ptype_pnt->NEXT;
        typ[VHD_nextDFN] = VHD_ptypeDFN;
        }
      if (ptype_pnt->DATA != NULL)
        {
        tab[VHD_dataDFN] = avail;
        nxt[VHD_dataDFN] = (void *)ptype_pnt->DATA;
        typ[VHD_dataDFN] = VHD_ERRDFN;
/*......typ[VHD_dataDFN] = .......................*/
        }

      (void)printf ("   type        : %ld\n",ptype_pnt->TYPE);
      (void)printf ("-> data        : %s\n",tab[VHD_dataDFN]);
      (void)printf ("-> next        : %s\n",tab[VHD_nextDFN]);

      break;

      }

   /* ###------------------------------------------------------### */
   /*    Reading new command              */
   /* ###------------------------------------------------------### */

    readcmd = vhd_getcmd (readtab);

    for (i=0 ; i<VHD_MAXDFN ; i++)
      {
      if (readcmd == key[i])
        break;
      }
    if ((i != VHD_MAXDFN+1) && (tab[i] == avail))
      newsta0 = i;
    else
      newsta0 = -1;

   /* ###------------------------------------------------------### */
   /*    Analysing new command               */
   /* ###------------------------------------------------------### */

    switch (newsta0)
      {
      case (VHD__UPDFN):
        current_pnt        = stk[--stk_pnt] ;
        break;

      case (VHD__BKDFN):
        while ((--stk_pnt != 0) && (stk[stk_pnt].mark != 1));
        current_pnt        = stk[stk_pnt] ;
        break;

      case (VHD__SPDFN):
        current_pnt.mark   = 1;
        break;

      case (VHD__TPDFN):
        current_pnt        = stk[0] ;
        stk_pnt        = 0 ;
        break;

      case (VHD__XTDFN):
        current_pnt.data   = NULL;
        current_pnt.type   = VHD__XTDFN;
        current_pnt.mark   = 0;
        break;

      case (VHD_ERRDFN):
        break;

      default:
        stk[stk_pnt++] = current_pnt;
        current_pnt.type   = typ[newsta0];
        current_pnt.mark   = 0;
        current_pnt.data   = nxt[newsta0];
        break;

      }

    state  = current_pnt.type;
    }
  }


static int vhd_getcmd (prvcmd)

char prvcmd[3][20];

  {
  char readstr[60];
  char comd0[20];
  char comd1[20];
  char comd2[20];
  int  code;

  (void)printf ("\n\n\n COMMAND >> ");
  comd0[0] = '\0';
  comd1[0] = '\0';
  comd2[0] = '\0';
  (void)fgets( readstr, 60, stdin );
  (void)sscanf (readstr,"%s%s%s",comd0,comd1,comd2);

  if (strcmp(comd0,"."))
    {
    (void)strcpy (prvcmd[0], comd0);
    (void)strcpy (prvcmd[1], comd1);
    (void)strcpy (prvcmd[2], comd2);
    }
  code = vhd_hash (prvcmd[0]);

  (void)printf ("\n\n\n\n\n");
  return (code);
  }



static int vhd_hash (str)

char *str;

  {
  int code = 0;

  while(*str != '\0')
    code =  code<<1 ^ *str++;

  return (code);
  }
