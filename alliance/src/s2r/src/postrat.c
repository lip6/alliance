/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 *
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/****************************************************************************/
/*                                                                          */
/*                      ALLIANCE CAO & VLSI CAD                             */
/*                                                                          */
/*    Product : (S)ymbolic (2)to (R)eal translater                          */
/*    File    : postreat.c                                                  */
/*                                                                          */
/*    (c) copyright 1992 MASI laboratory CAO & VLSI team                    */
/*    All Right Reserved                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*                     Post treatement functions                            */
/*                                                                          */
/****************************************************************************/
/* fonction void couron_layer ()                                            */
/* ATTENTION! dx et dy ne sont pas les longueurs mais les positions hautes  */
/*            et droites des couronnes dans couronne[]                      */
/****************************************************************************/

#include "generic.h"
#include  "mph.h"
#include  "mut.h"
#include  "rds.h"
#include  "rpr.h"
#include  "rtl.h"
#include  "rfm.h"
#include "rdsacces.h"
#include "postrat.h"
#include "merge.h"
#include "hash.h"
#include "math.h"

/*************************************************************************
 * resize_layer : extends the sides of all the rectangles in the specified
 *                layer in the specified model by the amount "qty" at each
 *                extreme.
 *************************************************************************/
static void resize_layer (model, layer_num)
     rdsfig_list *model;
     int layer_num;
{
   rdsrec_list *rectp;
   long qty;                    /* given in rds_unit */

   qty = GET_S2R_OVERSIZE (layer_num);

   for (rectp = model->LAYERTAB[layer_num]; rectp; rectp = rectp->NEXT)
   {
      (long) rectp->X -= qty;
      (long) rectp->Y -= qty;
      (long) rectp->DX += 2 * qty;
      (long) rectp->DY += 2 * qty;
   }
}

/*****************************************************************************
 * inv_resize_layer : minimizes the sides of all the rectangles in the specified
 *                    layer in the specified model by the amount "qty" at each
 *                    extreme.
 *****************************************************************************/
static void inv_resize_layer (model, layer_num)
     rdsfig_list *model;
     int layer_num;
{
   rdsrec_list *rectp;
   long qty;                    /* given in rds_unit */

   qty = GET_S2R_OVERSIZE (layer_num);

   for (rectp = model->LAYERTAB[layer_num]; rectp; rectp = rectp->NEXT)
   {
      (long) rectp->X += qty;
      (long) rectp->Y += qty;
      (long) rectp->DX -= 2 * qty;
      (long) rectp->DY -= 2 * qty;
   }
}

/******************************************************************************
 * adjust_rectangle: adjusts the data of the rectangle considering the symmetry
 *                   of the instance containning it.
 *                   The given symmetry might be different from the symmetry
 *                   of the instance in its father ( the case of several
 *                   hierarchical levels).
 *                   This function is used when copying the rectangle in higher
 *                   hierarchical level.
 *****************************************************************************/
static void adjust_rectangle (x, y, dx, dy, xins, yins, sym)
     long *x, *y, *dx, *dy;
     long xins, yins;
     char sym;
{
   long tx, ty, tdx, tdy;

   tx = *x;
   ty = *y;
   tdx = *dx;
   tdy = *dy;

   switch (sym)
   {
   case NOSYM:
      *x += xins;
      *y += yins;
      break;
   case SYM_X:
      *x = xins - tx - tdx;
      *y += yins;
      break;
   case SYM_Y:
      *x += xins;
      *y = yins - ty - tdy;
      break;
   case SYMXY:
      *x = xins - tx - tdx;
      *y = yins - ty - tdy;
      break;
   case ROT_P:
      *x = xins - ty - tdy;
      *y = yins + tx;
      *dx = tdy;
      *dy = tdx;
      break;
   case ROT_M:
      *x = xins + ty;
      *y = yins - tx - tdx;
      *dx = tdy;
      *dy = tdx;
      break;
   case SY_RP:
      *x = xins + ty;
      *y = yins + tx;
      *dx = tdy;
      *dy = tdx;
      break;
   case SY_RM:
      *x = xins - ty - tdy;
      *y = yins - tx - tdx;
      *dx = tdy;
      *dy = tdx;
      break;
   default:
      (void) fprintf (stderr, "**post_trat**adj_rect:unknown symetry number %d\n", sym);
      exit (1);
   }
}

/******************************************************************************
 * readjust_rectangle: adjusts the data of the rectangle considering the
 *                     symmetry of the instance that will contain it.
 *                     The given symmetry might be different from the symmetry
 *                     of the instance in its father ( the case of several
 *                     hierarchical levels).
 *                     This function is used when copying the rectangle in lower
 *                     hirerichal level.
 *****************************************************************************/
static void readjust_rectangle (x, y, dx, dy, xins, yins, sym)
     long *x, *y, *dx, *dy;
     long xins, yins;
     char sym;
{
   long tx, ty, tdx, tdy;

   tx = *x;
   ty = *y;
   tdx = *dx;
   tdy = *dy;
   switch (sym)
   {
   case NOSYM:
      *x -= xins;
      *y -= yins;
      break;
   case SYM_X:
      *x = xins - tx - tdx;
      *y -= yins;
      break;
   case SYM_Y:
      *x -= xins;
      *y = yins - ty - tdy;
      break;
   case SYMXY:
      *x = xins - tx - tdx;
      *y = yins - ty - tdy;
      break;
   case ROT_P:
      *x = ty - yins;
      *y = xins - tdx - tx;
      *dx = tdy;
      *dy = tdx;
      break;
   case ROT_M:
      *x = yins - tdy - ty;
      *y = tx - xins;
      *dx = tdy;
      *dy = tdx;
      break;
   case SY_RP:
      *x = ty - yins;
      *y = tx - xins;
      *dx = tdy;
      *dy = tdx;
      break;
   case SY_RM:
      *x = yins - tdy - ty;
      *y = xins - tdx - tx;
      *dx = tdy;
      *dy = tdx;
      break;
   default:
      (void) fprintf (stderr, "**post_trat**readj_rect:unknown symetry number %d\n", sym);
      exit (1);
   }
}

/******************************************************************************
 * result_sym: gives the result of 2 successive symmetry.
 *****************************************************************************/
static char result_sym (first_sym, second_sym)
     char first_sym, second_sym;
{
   switch (first_sym)
   {
   case NOSYM:
      switch (second_sym)
      {
      case NOSYM:
         return (NOSYM);
      case SYM_X:
         return (SYM_X);
      case SYM_Y:
         return (SYM_Y);
      case SYMXY:
         return (SYMXY);
      case ROT_P:
         return (ROT_P);
      case ROT_M:
         return (ROT_M);
      case SY_RP:
         return (SY_RP);
      case SY_RM:
         return (SY_RM);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   case SYM_X:
      switch (second_sym)
      {
      case NOSYM:
         return (SYM_X);
      case SYM_X:
         return (NOSYM);
      case SYM_Y:
         return (SYMXY);
      case SYMXY:
         return (SYM_Y);
      case ROT_P:
         return (SY_RM);
      case ROT_M:
         return (SY_RP);
      case SY_RP:
         return (ROT_M);
      case SY_RM:
         return (ROT_P);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   case SYM_Y:
      switch (second_sym)
      {
      case NOSYM:
         return (SYM_Y);
      case SYM_X:
         return (SYMXY);
      case SYM_Y:
         return (NOSYM);
      case SYMXY:
         return (SYM_X);
      case ROT_P:
         return (SY_RP);
      case ROT_M:
         return (SY_RM);
      case SY_RP:
         return (ROT_P);
      case SY_RM:
         return (ROT_M);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   case SYMXY:
      switch (second_sym)
      {
      case NOSYM:
         return (SYMXY);
      case SYM_X:
         return (SYM_Y);
      case SYM_Y:
         return (SYM_X);
      case SYMXY:
         return (NOSYM);
      case ROT_P:
         return (ROT_M);
      case ROT_M:
         return (ROT_P);
      case SY_RP:
         return (SY_RM);
      case SY_RM:
         return (SY_RP);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   case ROT_P:
      switch (second_sym)
      {
      case NOSYM:
         return (ROT_P);
      case SYM_X:
         return (SY_RP);
      case SYM_Y:
         return (SY_RM);
      case SYMXY:
         return (ROT_M);
      case ROT_P:
         return (SYMXY);
      case ROT_M:
         return (NOSYM);
      case SY_RP:
         return (SYM_Y);
      case SY_RM:
         return (SYM_X);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   case ROT_M:
      switch (second_sym)
      {
      case NOSYM:
         return (ROT_M);
      case SYM_X:
         return (SY_RM);
      case SYM_Y:
         return (SY_RP);
      case SYMXY:
         return (ROT_P);
      case ROT_P:
         return (NOSYM);
      case ROT_M:
         return (SYMXY);
      case SY_RP:
         return (SYM_X);
      case SY_RM:
         return (SYM_Y);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   case SY_RP:
      switch (second_sym)
      {
      case NOSYM:
         return (SY_RP);
      case SYM_X:
         return (ROT_P);
      case SYM_Y:
         return (ROT_M);
      case SYMXY:
         return (SY_RM);
      case ROT_P:
         return (SYM_X);
      case ROT_M:
         return (SYM_Y);
      case SY_RP:
         return (NOSYM);
      case SY_RM:
         return (SYMXY);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   case SY_RM:
      switch (second_sym)
      {
      case NOSYM:
         return (SY_RM);
      case SYM_X:
         return (ROT_M);
      case SYM_Y:
         return (ROT_P);
      case SYMXY:
         return (SY_RP);
      case ROT_P:
         return (SYM_Y);
      case ROT_M:
         return (SYM_X);
      case SY_RP:
         return (SYMXY);
      case SY_RM:
         return (NOSYM);
      default:
         (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", second_sym);
         exit (1);
      }
   default:
      (void) fprintf (stderr, "**post_trat**result_sym:unknown symetry number %d\n", first_sym);
      exit (1);
   }
   return (NOSYM);
}

/*****************************************************************************
 * flatten_instance_layer: copies to the given layer_tail, all the segments
 *                         in the given instance that intersect the couron of
 *                         the given ancetre, using the given father_sym
 *                         to compose the resulting symmetry of the instance
 *                         in the ancetre.
 *                         The segments of the sons instances are copied also
 *                         as it is a recursive function.
 ******************************************************************************/
static void flatten_instance_layer (instance, ancetre, comp_sym, origin_x, origin_y, layer_tail, layer_num)
     rdsins_list *instance;     /* instance where there is rectangles to flatten */
     rdsins_list *ancetre;      /* first instance where the ring is defined */
     char comp_sym;             /* symetry comp of instances without the first */
     long origin_x, origin_y;   /* origin comp of instances without the first */
     rds_rectanglep_typ *layer_tail;
     int layer_num;
{
   rdsfig_list *modelp;
   rdsins_list *instancef;
   long nx, ny, ndx, ndy;
   long dummy1, dummy2;
   long new_origin_x, new_origin_y;
   rdsrec_list *rectp;

   modelp = rds_model_out (instance->FIGNAME);
   if (incataloggds (instance->FIGNAME))
      return;

   dummy1 = 0l;
   dummy2 = 0l;
   new_origin_x = origin_x;
   new_origin_y = origin_y;
   origin_x = instance->X;
   origin_y = instance->Y;
   adjust_rectangle (&origin_x, &origin_y, &dummy1, &dummy2, new_origin_x, new_origin_y, comp_sym);
   comp_sym = result_sym (instance->TRANSF, comp_sym);

   for (instancef = modelp->INSTANCE; instancef; instancef = instancef->NEXT)
      flatten_instance_layer (instancef, ancetre, comp_sym, origin_x, origin_y, layer_tail, layer_num);

   for (rectp = modelp->LAYERTAB[layer_num]; rectp; rectp = rectp->NEXT)
   {
      if (IsRdsConExter (rectp))
         continue;
      if (IsRdsRefRef (rectp))
         continue;
      if (IsRdsRefCon (rectp))
         continue;

      nx = rectp->X;
      ny = rectp->Y;
      ndx = rectp->DX;
      ndy = rectp->DY;

      adjust_rectangle (&nx, &ny, &ndx, &ndy, origin_x, origin_y, comp_sym);
      if ((nx <= ancetre->COURONNE[layer_num]->X)
          || (ny <= ancetre->COURONNE[layer_num]->Y)
          || ((nx + ndx) >= ancetre->COURONNE[layer_num]->DX) || ((ny + ndy) >= ancetre->COURONNE[layer_num]->DY))
      {
         adjust_rectangle (&nx, &ny, &ndx, &ndy, ancetre->X, ancetre->Y, ancetre->TRANSF);

         *layer_tail = rds_rectangle_in (*layer_tail, nx, ny, ndx, ndy, layer_num, rectp->FLAGS, NULL);
         mark_flattenres (*layer_tail);
      }
   }
}

/******************************************************************************
 * flatten_layer: copies from: all the instances in the specified model in the
 *                             the specified layer any rectangle that intersects
 *                             the couron of the instance or even touches it.
 *                         to: the list of rectangles of the specified model in
 *                             th specified layer.
 *                A flattened rectangle is marked (flattened) so that if it is
 *                not used in the merging stage, it will be removed in the
 *                cleanning stage.
 *                remark: if there is more than 2 hierarchical levels, there
 *                will be a call to the flatten_instance_layer() function.
 *****************************************************************************/
static void flatten_layer (model, layer_num)
     rdsfig_list *model;
     int layer_num;
{
   rdsins_list *instance, *instancef;
   rdsrec_list *rectp;
   rds_rectanglep_typ *layer_tail;
   rdsfig_list *modelp;
   long nx, ny, ndx, ndy;

   /* get the tail of the list of segments of the model */
   for (rectp = model->LAYERTAB[layer_num]; (rectp && rectp->NEXT); rectp = rectp->NEXT)
      ;
   if (rectp)
      layer_tail = &(rectp->NEXT);
   else
      layer_tail = &(model->LAYERTAB[layer_num]);

   for (instance = model->INSTANCE; instance; instance = instance->NEXT)
   {
      modelp = rds_model_out (instance->FIGNAME);
      if (incataloggds (instance->FIGNAME))
         continue;
      /* flatten lower levels of the hierarchy */
      for (instancef = modelp->INSTANCE; instancef; instancef = instancef->NEXT)
      {
         flatten_instance_layer (instancef, instance, NOSYM, /* no symetry for the model */
                                 0l, 0l, layer_tail, layer_num);
      }
      /* flatten the instances of the model */
      for (rectp = modelp->LAYERTAB[layer_num]; rectp; rectp = rectp->NEXT)
      {
         nx = rectp->X;
         ny = rectp->Y;
         ndx = rectp->DX;
         ndy = rectp->DY;
         if ((nx <= instance->COURONNE[layer_num]->X)
             || (ny <= instance->COURONNE[layer_num]->Y)
             || ((nx + ndx) >= instance->COURONNE[layer_num]->DX) || ((ny + ndy) >= instance->COURONNE[layer_num]->DY))
         {
            adjust_rectangle (&nx, &ny, &ndx, &ndy, instance->X, instance->Y, instance->TRANSF);
            *layer_tail = rds_rectangle_in (*layer_tail, nx, ny, ndx, ndy, layer_num, rectp->FLAGS, NULL);
            mark_flattenres (*layer_tail);
         }
      }
   }
}

/*****************************************************************************
 * envelop_layer: defines the envelop of the specified model in the specified
 *                layer.
 *                i.e. the minimum rectangle that contains all the rectangles
 *                and all the envelops of all instances of the specified model
 *                in the specified layer.
 *                This function as it takes into consideration the envelops
 *                of the instances of the model it must be applied to the
 *                instances before the father model; what is done implicitly
 *                by the post_treat function.
 *                This function once applied on a model at a layer, marks that
 *                model enveloped at that layer.
 *                If the model at that layer does not contain any rectangles,
 *                it is marked empty at that layer.
 ****************************************************************************/
static void envelop_layer (model, layer_num)
     rdsfig_list *model;
     int layer_num;
{
   rdsfig_list *modelp;
   rdsins_list *instancep;
   long lx, ly, gx, gy;
   rdsrec_list *rectp;
   char init = 0;
   long nx, ny, ndx, ndy;

   /* printf("envelop %d\n",layer_num); */

   lx = ly = gx = gy = 0;

   /* segments of the model */
   if (model->LAYERTAB[layer_num])
   {
      lx = model->LAYERTAB[layer_num]->X;
      ly = model->LAYERTAB[layer_num]->Y;
      gx = model->LAYERTAB[layer_num]->X + model->LAYERTAB[layer_num]->DX;
      gy = model->LAYERTAB[layer_num]->Y + model->LAYERTAB[layer_num]->DY;
      init = 1;
      for (rectp = model->LAYERTAB[layer_num]; rectp; rectp = rectp->NEXT)
      {
         if (rectp->X < lx)
            lx = rectp->X;
         if (rectp->Y < ly)
            ly = rectp->Y;
         if ((rectp->X + rectp->DX) > gx)
            gx = rectp->X + rectp->DX;
         if ((rectp->Y + rectp->DY) > gy)
            gy = rectp->Y + rectp->DY;
      }
   }
   /* envelops of the instances of the model */
   for (instancep = model->INSTANCE; instancep; instancep = instancep->NEXT)
   {
      modelp = rds_model_out (instancep->FIGNAME);
      if (!(is_empty (modelp, layer_num)) && !(incataloggds (instancep->FIGNAME)))
      {
         rectp = modelp->BBOXTAB[layer_num];
         nx = rectp->X;
         ny = rectp->Y;
         ndx = rectp->DX;
         ndy = rectp->DY;
         adjust_rectangle (&nx, &ny, &ndx, &ndy, instancep->X, instancep->Y, instancep->TRANSF);
         if (init)
         {
            if (nx < lx)
               lx = nx;
            if (ny < ly)
               ly = ny;
            if ((nx + ndx) > gx)
               gx = nx + ndx;
            if ((ny + ndy) > gy)
               gy = ny + ndy;
         }
         else
         {
            lx = nx;
            ly = ny;
            gx = nx + ndx;
            gy = ny + ndy;
            init = 1;
         }
      }
   }
   if (init)
      model->BBOXTAB[layer_num] =
/* A VOIR 5 mai 1994
        rds_rectangle_in( model->BBOXTAB[ layer_num], lx,  ly,
                          gx - lx,  gy - ly, layer_num, rectp->FLAG, NULL );
 */
         rds_rectangle_in (model->BBOXTAB[layer_num], lx, ly, gx - lx, gy - ly, layer_num, NULL);
   else
      mark_empty (model, layer_num);
}

/*****************************************************************************
 * mod_ins_couron: given an instance, a layer and the rectangle of intersection
 *                 of this instance with another instance; it modifies the
 *                 couron of this instance in that layer if the rectangle of
 *                 intersection makes the couron wider at any side.
 *****************************************************************************/
static void mod_ins_couron (instance, layer_num, xi, yi, dxi, dyi)
     rdsins_list *instance;
     int layer_num;
     long xi, yi, dxi, dyi;
{
   rdsfig_list *model;
   long corn_width, xe, ye, dxe, dye;

   model = rds_model_out (instance->FIGNAME);

   corn_width = GET_S2R_RING_WIDTH (layer_num) + GET_S2R_LAYER_WIDTH (layer_num) / 2;
   xe = model->BBOXTAB[layer_num]->X;
   ye = model->BBOXTAB[layer_num]->Y;
   dxe = model->BBOXTAB[layer_num]->DX;
   dye = model->BBOXTAB[layer_num]->DY;

   readjust_rectangle (&xi, &yi, &dxi, &dyi, instance->X, instance->Y, instance->TRANSF);

   if ((xi - xe) > ((xe + dxe) - (xi + dxi)))
   {
      if ((xi - corn_width) < instance->COURONNE[layer_num]->DX)
         instance->COURONNE[layer_num]->DX = xi - corn_width;
   }
   else if ((xi + dxi + corn_width) > instance->COURONNE[layer_num]->X)
      instance->COURONNE[layer_num]->X = xi + dxi + corn_width;

   if ((yi - ye) > ((ye + dye) - (yi + dyi)))
   {
      if ((yi - corn_width) < instance->COURONNE[layer_num]->DY)
         instance->COURONNE[layer_num]->DY = yi - corn_width;
   }
   else if ((yi + dyi + corn_width) > instance->COURONNE[layer_num]->Y)
      instance->COURONNE[layer_num]->Y = yi + dyi + corn_width;
}

/*****************************************************************************
 * couron_layer: defines the courons of every instance in the specified model
 *               in the specified layer.
 *               The couron is determined by 4 limits: x, y, dx
 *               and dy beyond which is the couron area.
 * ATTENTION! dx et dy ne sont pas les longueurs mais les positions hautes
 *            et droites des couronnes dans couronne[]
 ****************************************************************************/
static void couron_layer (model, layer_num, verbose)
     rdsfig_list *model;
     int layer_num;
     int verbose;
{
   register rdsins_list *instance1, *instance2;
   long x1, x2, y1, y2, dx1, dx2, dy1, dy2, rx, rdx, ry, rdy;
   register rdsfig_list *model1, *model2;
   long corn_width;
   long dot = 0, nb_inst = 0, prc = 0, new_prc = 0, iteration, first = 1;

   corn_width = GET_S2R_RING_WIDTH (layer_num) + GET_S2R_LAYER_WIDTH (layer_num) / 2;

/*\
 * initial couron based on the abutment box and the envelop
\*/
   for (instance1 = model->INSTANCE, nb_inst = 0; instance1; instance1 = instance1->NEXT, nb_inst++)
   {
      model1 = rds_model_out (instance1->FIGNAME);
      if (incataloggds (instance1->FIGNAME))
         continue;
/*\
 * allocation d'une structure rectancle pour la couronne[layer_num]
\*/
      instance1->COURONNE[layer_num] = (rdsrec_list *) mbkalloc (sizeof (rdsrec_list));
      instance1->COURONNE[layer_num]->DY = model1->LAYERTAB[RDS_ABOX]->Y + model1->LAYERTAB[RDS_ABOX]->DY - corn_width;
      instance1->COURONNE[layer_num]->Y = model1->LAYERTAB[RDS_ABOX]->Y + corn_width;
      instance1->COURONNE[layer_num]->DX = model1->LAYERTAB[RDS_ABOX]->X + model1->LAYERTAB[RDS_ABOX]->DX - corn_width;
      instance1->COURONNE[layer_num]->X = model1->LAYERTAB[RDS_ABOX]->X + corn_width;

      if (!is_empty (model1, layer_num))
      {
         if (instance1->COURONNE[layer_num]->DY > (model1->BBOXTAB[layer_num]->Y + model1->BBOXTAB[layer_num]->DY - corn_width))
         {
            instance1->COURONNE[layer_num]->DY = model1->BBOXTAB[layer_num]->Y + model1->BBOXTAB[layer_num]->DY - corn_width;
         }
         if (instance1->COURONNE[layer_num]->Y < (model1->BBOXTAB[layer_num]->Y + corn_width))
         {
            instance1->COURONNE[layer_num]->Y = model1->BBOXTAB[layer_num]->Y + corn_width;
         }
         if (instance1->COURONNE[layer_num]->DX > (model1->BBOXTAB[layer_num]->X + model1->BBOXTAB[layer_num]->DX - corn_width))
         {
            instance1->COURONNE[layer_num]->DX = model1->BBOXTAB[layer_num]->X + model1->BBOXTAB[layer_num]->DX - corn_width;
         }
         if (instance1->COURONNE[layer_num]->X < (model1->BBOXTAB[layer_num]->X + corn_width))
         {
            instance1->COURONNE[layer_num]->X = model1->BBOXTAB[layer_num]->X + corn_width;
         }
      }
   }
/*\
 * final couron based on instance intersections
\*/
   for (instance1 = model->INSTANCE, iteration = 0; instance1; instance1 = instance1->NEXT, iteration++)
   {
      model1 = rds_model_out (instance1->FIGNAME);

      if (incataloggds (instance1->FIGNAME))
         continue;

      if (verbose)
      {
         if (first)
         {
            printf ("\t    . %s ", RDS_LAYER_NAME[layer_num]);
            first = 0;
         }
         new_prc = (iteration * 100 / nb_inst) / 3; /* one dot each 3% */
         if (new_prc > prc)
         {
            dot++;
            printf (".");
            fflush (stdout);
            prc = new_prc;
         }
      }

      if (!(is_empty (model1, layer_num)))
      {
         x1 = model1->BBOXTAB[layer_num]->X;
         y1 = model1->BBOXTAB[layer_num]->Y;
         dx1 = model1->BBOXTAB[layer_num]->DX;
         dy1 = model1->BBOXTAB[layer_num]->DY;

         adjust_rectangle (&x1, &y1, &dx1, &dy1, instance1->X, instance1->Y, instance1->TRANSF);

         for (instance2 = instance1->NEXT; instance2; instance2 = instance2->NEXT)
         {
            model2 = rds_model_out (instance2->FIGNAME);

            if (incataloggds (instance2->FIGNAME))
            {
               continue;
            }

            if (!(is_empty (model2, layer_num)))
            {
               x2 = model2->BBOXTAB[layer_num]->X;
               y2 = model2->BBOXTAB[layer_num]->Y;
               dx2 = model2->BBOXTAB[layer_num]->DX;
               dy2 = model2->BBOXTAB[layer_num]->DY;

               adjust_rectangle (&x2, &y2, &dx2, &dy2, instance2->X, instance2->Y, instance2->TRANSF);

               if (rect_intersec (x1, x2, dx1, dx2, y1, y2, dy1, dy2, &rx, &rdx, &ry, &rdy))
               {
                  mod_ins_couron (instance1, layer_num, rx, ry, rdx, rdy);
                  mod_ins_couron (instance2, layer_num, rx, ry, rdx, rdy);
               }
            }
         }
      }
   }
   if (verbose && !first)
   {
      for (; dot < 33; dot++)
         printf (".");
      printf ("\n");
      fflush (stdout);
   }
}

/******************************************************************************
 * cutcut : cut off layer shape from layer board in model
 *****************************************************************************/

void cutcut (model, board, shape)
     rdsfig_list *model;
     int board;                 /* layer number to cut */
     int shape;                 /* layer number of shapes to cut */
{
  rdsrec_list *rshp, *rbrd, *rbrd_end;
  int f = MBK_SEGMENT_MASK | RDS_FIG_REC_MASK;
  ht_t *dejavu;
  char buffer[1000];

  for (rbrd_end=model->LAYERTAB[board]; rbrd_end->NEXT; rbrd_end=rbrd_end->NEXT);

  dejavu = htinit (63179);
  for (rbrd = model->LAYERTAB[board]; rbrd; rbrd = rbrd->NEXT)
  {
    for (rshp = model->LAYERTAB[shape]; rshp; rshp = rshp->NEXT)
    {
      int rx, rdx, ry, rdy;
      if (rect_intersec (rshp->X, rbrd->X, rshp->DX, rbrd->DX, rshp->Y, rbrd->Y, rshp->DY, rbrd->DY, &rx, &rdx, &ry, &rdy))
      {
        if ((rdx > 0) && (rdy > 0))
        {
          mark_remove (rbrd);
          if (rx > (rbrd->X))  /* need right rectangle */
          {
            int nx = rbrd->X, ny = rbrd->Y, ndx = rx - (rbrd->X), ndy = rbrd->DY;
            sprintf (buffer, "%d %d %d %d", nx, ny, ndx, ndy);
            if (!htget (dejavu, buffer))
            {
              htset (dejavu, buffer);
              rbrd_end->NEXT = rds_rectangle_in (NULL, nx, ny, ndx, ndy, board, f, NULL);
              rbrd_end = rbrd_end->NEXT;
            }
          }
          if (ry > (rbrd->Y))  /* need bottom rectangle */
          {
            int nx = rbrd->X, ny = rbrd->Y, ndx = rbrd->DX, ndy = ry - (rbrd->Y);
            sprintf (buffer, "%d %d %d %d", nx, ny, ndx, ndy);
            if (!htget (dejavu, buffer))
            {
              htset (dejavu, buffer);
              rbrd_end->NEXT = rds_rectangle_in (NULL, nx, ny, ndx, ndy, board, f, NULL);
              rbrd_end = rbrd_end->NEXT;
            }
          }
          if (ry + rdy < (rbrd->Y + rbrd->DY)) /* need top rectangle */
          {
            int nx = rbrd->X, ny = ry + rdy, ndx = rbrd->DX, ndy = (rbrd->Y + rbrd->DY) - (ry + rdy);
            sprintf (buffer, "%d %d %d %d", nx, ny, ndx, ndy);
            if (!htget (dejavu, buffer))
            {
              htset (dejavu, buffer);
              rbrd_end->NEXT = rds_rectangle_in (NULL, nx, ny, ndx, ndy, board, f, NULL);
              rbrd_end = rbrd_end->NEXT;
            }
          }
          if (rx + rdx < (rbrd->X + rbrd->DX)) /* need left rectangle */
          {
            int nx = rx + rdx, ny = rbrd->Y, ndx = (rbrd->X + rbrd->DX) - (rx + rdx), ndy = rbrd->DY;
            sprintf (buffer, "%d %d %d %d", nx, ny, ndx, ndy);
            if (!htget (dejavu, buffer))
            {
              htset (dejavu, buffer);
              rbrd_end->NEXT = rds_rectangle_in (NULL, nx, ny, ndx, ndy, board, f, NULL);
              rbrd_end = rbrd_end->NEXT;
            }
          }
        }
      }
    }
  }
  rds_remove_rectangle (&(model->LAYERTAB[board]));
  htremove (dejavu);
}

/******************************************************************************
 * addpwell: define RDS_PWELL <- RDS_ABOX - RDS_NWELL
 *        only on terminal cells (without instance) having NWELL rectangle
 *****************************************************************************/
static void addpwell (model, verbose)
     rdsfig_list *model;
     int verbose;
{
  if (model->INSTANCE == NULL)
  {
    if ((model->LAYERTAB[RDS_ABOX]) == NULL)
    {
      (void) fprintf (stderr, "**post_trat**addpwell:cell %s has none ABOX\n", model->NAME);
      exit (1);
    }
    if (((model->LAYERTAB[RDS_ABOX])->NEXT) != NULL)
    {
      (void) fprintf (stderr, "**post_trat**addpwell:cell %s has more than one ABOX\n", model->NAME);
      exit (1);
    }
    if (model->LAYERTAB[RDS_NWELL] != NULL)
    {
      rdsrec_list *rectab = model->LAYERTAB[RDS_ABOX];
      int f = MBK_SEGMENT_MASK | RDS_FIG_REC_MASK;
      long qty = GET_S2R_OVERSIZE (RDS_NIMP);

      // ajout d'un rectangle de PWELL avec la taille de l'ABOX + OVERSIZE(RDS_NIMP)
      // puis on on elimine le PWELL qui intersecte le NWELL
      model->LAYERTAB[RDS_PWELL] = rds_rectangle_in (NULL, rectab->X-qty, rectab->Y-qty, rectab->DX+2*qty, rectab->DY+2*qty, RDS_PWELL,f,NULL);
      cutcut (model, RDS_PWELL, RDS_NWELL);

      // on recalcule de NWELL a partir du PWELL
      model->LAYERTAB[RDS_NWELL] = rds_rectangle_in (NULL, rectab->X-qty, rectab->Y-qty, rectab->DX+2*qty, rectab->DY+2*qty, RDS_PWELL,f,NULL);
      cutcut (model, RDS_NWELL, RDS_PWELL);
    }
  }
}

/******************************************************************************
 * addpimp: define on NWELL RDS_PIMP <- RDS_WELL - RDS_NIMP
 *        only on terminal cells (without instance) having NWELL rectangle
 * put RDS_PIMP  all over NWELL 
 * put RDS_NIMP  all over PWELL 
 *****************************************************************************/
static void addimp (model, verbose)
     rdsfig_list *model;
     int verbose;
{
  rdsrec_list *r;
  int f = MBK_SEGMENT_MASK | RDS_FIG_REC_MASK;

  if (model->INSTANCE == NULL && model->LAYERTAB[RDS_NWELL] != NULL)
  {
    rdsrec_list *rectab = model->LAYERTAB[RDS_ABOX];
    long qty = 0;
    
    model->LAYERTAB[RDS_PIMP] = NULL;
    model->LAYERTAB[RDS_NIMP] = NULL;
    
    for (r = model->LAYERTAB[RDS_NWELL]; r; r = r->NEXT)
      model->LAYERTAB[RDS_PIMP] = rds_rectangle_in (model->LAYERTAB[RDS_PIMP], r->X, r->Y, r->DX, r->DY, RDS_PIMP, f, NULL);
    for (r = model->LAYERTAB[RDS_PWELL]; r; r = r->NEXT)
      model->LAYERTAB[RDS_NIMP] = rds_rectangle_in (model->LAYERTAB[RDS_NIMP], r->X, r->Y, r->DX, r->DY, RDS_NIMP, f, NULL);

    if (model->LAYERTAB[RDS_NIMP] != NULL)
       cutcut (model, RDS_NIMP, RDS_PTIE);
    if (model->LAYERTAB[RDS_PIMP] != NULL) 
       cutcut (model, RDS_PIMP, RDS_NTIE);
  }
}

/******************************************************************************
 * envelop: envelops the post_treatable layers of the given model.
 *****************************************************************************/
static void envelop (model)
     rdsfig_list *model;
{
   int layer_num;

   for (layer_num = 0; layer_num < RDS_MAX_LAYER; layer_num++)
   {
      if (GET_S2R_POST_Y_OR_N (layer_num) == S2R_TREAT)
      {
         envelop_layer (model, layer_num);
      }
   }
}

/******************************************************************************
 * couron: courons the post_treatable layers of the given model.
 *****************************************************************************/
static void couron (model, verbose)
     rdsfig_list *model;
     int verbose;
{
   int layer_num;

   for (layer_num = 0; layer_num < RDS_MAX_LAYER; layer_num++)
   {
      if ((GET_S2R_POST_Y_OR_N (layer_num) == S2R_TREAT) && (GET_S2R_RING_WIDTH (layer_num) != 0))
      {
         couron_layer (model, layer_num, verbose);
      }
   }
}

/******************************************************************************
 * flatten: flattens the post_treatable layers of the model.
 *****************************************************************************/
static void flatten (model)
     rdsfig_list *model;
{
   int layer_num;

   for (layer_num = 0; layer_num < RDS_MAX_LAYER; layer_num++)
   {
      if ((GET_S2R_POST_Y_OR_N (layer_num) == S2R_TREAT) && (GET_S2R_RING_WIDTH (layer_num) != 0))
      {
         flatten_layer (model, layer_num);
      }
   }
}

/******************************************************************************
 * resize: oversize all post_treatable layers of the model
 *****************************************************************************/
static void resize (model)
     rdsfig_list *model;
{
   int layer_num;

   for (layer_num = 0; layer_num < RDS_MAX_LAYER; layer_num++)
   {
      if (GET_S2R_POST_Y_OR_N (layer_num) == S2R_TREAT)
      {
         resize_layer (model, layer_num);
      }
   }
}

/******************************************************************************
 * merge: merges all post_treatable layers of the model.
 *        If the layer to be merged is of the type that needs scotchs
 *        ( indicated by having a complementary layer in its postreat table),
 *        merge_layer_2() will be called instead of merge_layer().
 *****************************************************************************/
static void merge (model, scotch_on_flag, verbose)
     rdsfig_list *model;
     int scotch_on_flag, verbose;
{
   int layer_num;

   for (layer_num = 0; layer_num < RDS_MAX_LAYER; layer_num++)
   {
      if (GET_S2R_POST_Y_OR_N (layer_num) == S2R_TREAT)
      {
         if ((GET_S2R_POST_COMPLEMENTARY (layer_num) != RDS_S2R_POST_TREAT_EMPTY) && scotch_on_flag)
         {
            merge_layer_2 (model, layer_num, verbose);
         }
         else
         {
            merge_layer (model, layer_num, verbose);
         }
      }
   }
}

/******************************************************************************
 * clean: cleans all post_treatable layers.
 *****************************************************************************/
static void clean (model)
     rdsfig_list *model;
{
   int layer_num;

   for (layer_num = 0; layer_num < RDS_MAX_LAYER; layer_num++)
   {
      if (GET_S2R_POST_Y_OR_N (layer_num) == S2R_TREAT)
      {
         rds_clean_layer (&(model->LAYERTAB[layer_num]));
      }
   }
}

/******************************************************************************
 * inv_resize: undersizes all post_treatable layers.
 *****************************************************************************/
static void inv_resize (model)
     rdsfig_list *model;
{
   int layer_num;

   for (layer_num = 0; layer_num < RDS_MAX_LAYER; layer_num++)
   {
      if (GET_S2R_POST_Y_OR_N (layer_num) == S2R_TREAT)
      {
         inv_resize_layer (model, layer_num);
      }
   }
}

/*****************************************************************************
 * post_treat: post_treats the specified model.
 *             After completion it marks this model at this layer post_treated.
 *             This is a recursive function: if an instance of this model
 *             has its model not yet post treated at this layer, it is
 *             post treated first.
 ****************************************************************************/
void post_treat (model, scotch_on_flag, verbose, autoimp)
     rdsfig_list *model;
     int scotch_on_flag;
     int verbose;
     int autoimp;
{
   rdsins_list *instance;
   rdsfig_list *modelp;

   for (instance = model->INSTANCE; instance; instance = instance->NEXT)
   {
      modelp = rds_model_out (instance->FIGNAME);

      if (!modelp->FLAGS && !incataloggds (modelp->NAME))
      {
         post_treat (modelp, scotch_on_flag, verbose);
      }
   }
   if (verbose)
      printf ("\t--> post-treating model %s\n", model->NAME);
   if (verbose && model->INSTANCE)
   {
      printf ("\t    ring flattenning  : \n");
      fflush (stdout);
   }
   if (!incataloggds (model->NAME))
   {
      envelop (model);
      couron (model, verbose);
      flatten (model);
      resize (model);
      if (verbose)
      {
         printf ("\t    rectangle merging : \n");
         fflush (stdout);
      }
      merge (model, scotch_on_flag, verbose);
      clean (model);
      inv_resize (model);
      if (autoimp)
      {
         addpwell (model, verbose);
         addimp (model, verbose);
      }
      mark_treat (model);
   }
}

/*******************************************************************************
 * replace_cells: traverses the list of models and replaces the models that are
 *                found in the gds catalog by the corresponding models parsed
 *                from this catalog.
 *                remarks:
 ******************************************************************************/
void replace_cells (verbose)
     int verbose;
{
   rdsfig_list *model, *model_next;
   static int not_print_yet = 1;
   char *model_name;

   for (model = HEAD_RDSFIG; model;)
   {
      model_next = model->NEXT;
      if (incataloggds (model->NAME))
      {
         if (not_print_yet)
         {
            printf ("\to replacing black boxes\n");
            not_print_yet = 0;
         }
         if (verbose)
            printf ("\t--> replace cell %s \n", model->NAME);
         model_name = model->NAME;
         delrdsfig (model_name);
         (void) getrdsfig (model_name, 'A', 0);
      }
      model = model_next;
   }
}
