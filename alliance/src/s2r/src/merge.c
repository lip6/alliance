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

/****************************************************************************/
/*                                                                          */
/*                      ALLIANCE CAO & VLSI CAD                             */
/*                                                                          */
/*    Product : (S)ymbolic (2)to (R)eal translater                          */
/*    File    : merge.c                                                     */
/*                                                                          */
/*    (c) copyright 1992 MASI laboratory CAO & VLSI team                    */
/*    All Right Reserved                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*                             Merge functions                              */
/*                                                                          */
/****************************************************************************/

#include "generic.h"
#include  <mph.h>
#include  <mut.h>
#include  <rds.h>
#include  <rfm.h>
#include  <rpr.h>
#include "rdsacces.h"
#include "maxima.h"
#include "statistics.h"
#include "merge.h"

/*****************************************************************************
 * name_transfert: given two rectangles, return name to keep after merge
 *                 if the names are different, a warning is done, to prevent
 *                 that the second one is erased
 ****************************************************************************/

static char *name_transfert (desc1, desc2)
     rdsrec_list *desc1, *desc2;
{
   if (desc1->NAME == NULL)
      return desc2->NAME;
/*\
 * some segments in different chanels made by scr(1) bloc has
 * different name but are linked together throught cells, s2r
 * merged then, but can't say it's a error!
 *
 * if (desc1->u_rec.name != desc2->u_rec.name)
 *    fprintf(stderr,
 *            "**s2r error**different nodes merged, %s erased\n",
 *            desc2->u_rec.name);
\*/
   return (desc1->NAME);
}

/***** macro definitions  *****/

#define lgx1 (x1+dx1)
#define lgy1 (yy1+dy1)
#define lgx2 (x2+dx2)
#define lgy2 (y2+dy2)

/******************************************************************************
 * ext_win_segs: given a model, a layer and a window; it returns a pointer to
 *               a list of rectangles ( formed by the rectangles in the
 *               specified model in the specified layer intersecting this window
 *               - or touching it).
 *               For consistency purpose these rectangles - which are in the
 *               list - are extracted from the model and the new list merged is
 *               returned back to the model.
 *****************************************************************************/

static rdsrec_list *ext_win_segs (model, layer_num, wx, wy, dx, dy)
     register rdsfig_list *model;
     register int layer_num;
     register long wx, wy, dx, dy;
{
   typedef rdsrec_list *rectangle_typ;
   register rectangle_typ *rectanglep;
   register rdsrec_list *temp_list;
   register rdsrec_list *temp;

   temp_list = NULL;
   for (rectanglep = &(model->LAYERTAB[layer_num]); *rectanglep;)
   {
      if (are_rect_intersec ((*rectanglep)->X, (*rectanglep)->Y, (*rectanglep)->DX, (*rectanglep)->DY, wx, wy, dx, dy))
      {
         temp = (*rectanglep)->NEXT;
         (*rectanglep)->NEXT = temp_list;
         temp_list = *rectanglep;
         *rectanglep = temp;
      }
      else
         rectanglep = &((*rectanglep)->NEXT);
   }
   return (temp_list);
}

/**********************************************************************
 * inclus: determines if one of the given rectangles is included in the
 *         other and if it is the case:
 *       - it marks the one included "to be removed"
 *       - it marks the one that includes "used"
 *       - it sets modif to 1 to indicate that a modification happened
 *       - if rectangle1(desc1) was the one included it sets modif1 to1
 *       - if rectangle2(desc2) was the one included it sets modif2 to1
 **********************************************************************/
static void inclus (modif, modif1, modif2, desc1, desc2)
     unsigned char *modif;      /*indique une modification de la base de donnees */
     unsigned char *modif1;     /* indique que le desc 1 a ete elimine */
     unsigned char *modif2;     /* indique que le desc 2 a ete elimine */
     rdsrec_list *desc1, *desc2;
{
   register long x1, dx1, yy1, dy1, x2, dx2, y2, dy2;

   x1 = desc1->X;
   yy1 = desc1->Y;
   dx1 = desc1->DX;
   dy1 = desc1->DY;
   x2 = desc2->X;
   y2 = desc2->Y;
   dx2 = desc2->DX;
   dy2 = desc2->DY;

   if (x1 == x2)
   {
      if (yy1 == y2)
      {
         if (lgx1 == lgx2)
         {
            if (lgy1 == lgy2)
            {
               /* desc1 equal desc2 : only one must be erased,
                  but, because all flattened rectangles will be erased
                  it's better to delete the not flattened one */
               *modif = 1;
               *modif2 = 1;
               *modif1 = 1;
               if (!is_flattenres (desc2))
               {
                  mark_remove (desc2);
               }
               else
               {
                  mark_remove (desc1);
               }
               return;
            }
         }
      }
   }
   if (x1 >= x2)
   {
      if (yy1 >= y2)
      {
         if (lgx1 <= lgx2)
         {
            if (lgy1 <= lgy2)
            {
               if (!is_flattenres (desc2))
               {
                  desc2->NAME = name_transfert (desc1, desc2);
               }
               mark_remove (desc1);
               *modif = 1;
               *modif1 = 1;
               return;
            }
         }
      }
   }
   if (x2 >= x1)
   {
      if (y2 >= yy1)
      {
         if (lgx2 <= lgx1)
         {
            if (lgy2 <= lgy1)
            {
               /* desc2 include in desc1 */
               if (!is_flattenres (desc1))
               {
                  desc1->NAME = name_transfert (desc1, desc2);
               }
               mark_remove (desc2);
               *modif = 1;
               *modif2 = 1;
            }
         }
      }
   }
   return;
}

/************************************************************************
 * prolong: determines if the 2 given rectangles have a dimension in common
 *          and if it is possible to extend the second one so as to include
 *          the first one. If it is the case:
 *              - the first rectangle (desc1) is marked "to be removed".
 *              - the second rectangle (desc2) is extended and is marked "used".
 *              - modif and modif1 are set to 1.
 *************************************************************************/

static void prolong (modif, modif1, desc1, desc2)
     unsigned char *modif;      /* indique une modificatin de la base de donnees */
     unsigned char *modif1;     /* indique que desc 1 a ete elimine */
     rdsrec_list *desc1, *desc2;
{
   register long x1, dx1, yy1, dy1, x2, dx2, y2, dy2;

   x1 = desc1->X;
   yy1 = desc1->Y;
   dx1 = desc1->DX;
   dy1 = desc1->DY;
   x2 = desc2->X;
   y2 = desc2->Y;
   dx2 = desc2->DX;
   dy2 = desc2->DY;

   if (yy1 == y2)
   {
      if (dy1 == dy2)
      {
         if (x1 < x2)
         {
            if (lgx1 >= x2)
            {
/* cas 1                                                              */
/*            ________________                                        */
/*           | 1    | | 2     |                                       */
/*           |      | |       |                                       */
/*           |______|_|_______|                                       */
               *modif = 1;
               *modif1 = 1;
               desc2->X = x1;
               desc2->DX = lgx2 - x1;
               mark_used (desc2);
               mark_remove (desc1);
               if (!is_flattenres (desc2))
               {
                  desc2->NAME = name_transfert (desc1, desc2);
               }
            }
         }
         else
         {
            if (x2 < x1)
            {
               if (lgx2 >= x1)
               {
/* cas 2                                                              */
/*            ________________                                        */
/*           | 2    | | 1     |                                       */
/*           |      | |       |                                       */
/*           |______|_|_______|                                       */
                  *modif = 1;
                  *modif1 = 1;
                  desc2->DX = lgx1 - x2;
                  mark_used (desc2);
                  mark_remove (desc1);
                  if (!is_flattenres (desc2))
                  {
                     desc2->NAME = name_transfert (desc1, desc2);
                  }
               }
            }
         }
      }
      return;
   }
   if (x1 == x2)
   {
      if (dx1 == dx2)
      {
         if (yy1 < y2)
         {
            if (lgy1 >= y2)
            {
/* cas 3                                                              */
/*               ________                                             */
/*              | 2      |                                            */
/*              |        |                                            */
/*              |________|                                            */
/*              |________|                                            */
/*              |        |                                            */
/*              | 1      |                                            */
/*              |________|                                            */
               *modif = 1;
               *modif1 = 1;
               desc2->Y = yy1;
               desc2->DY = lgy2 - yy1;
               mark_used (desc2);
               mark_remove (desc1);
               if (!is_flattenres (desc2))
               {
                  desc2->NAME = name_transfert (desc1, desc2);
               }
            }
         }
         else
         {
            if (y2 < yy1)
            {
               if (lgy2 >= yy1)
               {
/* cas 4                                                              */
/*               ________                                             */
/*              | 1      |                                            */
/*              |        |                                            */
/*              |________|                                            */
/*              |________|                                            */
/*              |        |                                            */
/*              | 2      |                                            */
/*              |________|                                            */
                  *modif = 1;
                  *modif1 = 1;
                  desc2->DY = lgy1 - y2;
                  mark_used (desc2);
                  mark_remove (desc1);
                  if (!is_flattenres (desc2))
                  {
                     desc2->NAME = name_transfert (desc1, desc2);
                  }
               }
            }
         }
      }
   }
   return;
}

/*****************************************************************************
 * supegl_dimin: determines if the given dimension is greater than twice the
 *               resize quantity plus the minimum width of a segment in this
 *               layer ( returns 1) or not (returns 0).
 ****************************************************************************/

static int supegl_dimin (val, layer_num) /* used in contact */
     long val;
     int layer_num;
{
   if (val >= (long) (2 * GET_S2R_OVERSIZE (layer_num) + GET_S2R_LAYER_WIDTH (layer_num)))
   {
      return (1);
   }
   else
   {
      return (0);
   }
}

/*************************************************************************
 * contact: determines if the 2 given rectangles intersect each other.
 *          If it is the case:
 *              - it extends one of them and marks it "used".
 *          or  - it creates 1 or 2 new compensating rectangles and adds them
 *                to the given list.( rectangles are not created unless they
 *                have the dimensions greater than twice the resize quantity
 *                and only in the first pass (passage).).
 **************************************************************************/

static void contact (temp_list, layer_num, passage, desc1, desc2)
     rds_rectanglep_typ *temp_list;
     int layer_num;
     char passage;
     rdsrec_list *desc1, *desc2;
{
   long mbk_type;
   register long x1, dx1, yy1, dy1, x2, dx2, y2, dy2;

   mbk_type = GetRdsMbkType (desc1);
   x1 = desc1->X;
   yy1 = desc1->Y;
   dx1 = desc1->DX;
   dy1 = desc1->DY;
   x2 = desc2->X;
   y2 = desc2->Y;
   dx2 = desc2->DX;
   dy2 = desc2->DY;

   if (x1 >= x2)
   {
      if (yy1 < y2)
      {
         if (lgy1 < lgy2)
         {
            if (lgx1 <= lgx2)
            {
               if (lgy1 >= y2)
               {
/* cas 1                                                              */
/*             _________                                              */
/*            |    2    |                                             */
/*            |   ___   |                                             */
/*            |__|___|__|                                             */
/*               | 1 |                                                */
/*               |___|                                                */
                  desc1->DY = lgy2 - yy1;
                  mark_used (desc1);
                  return;
               }
            }
            else if (passage == 0)
            {
               if (x1 <= lgx2)
               {
                  if (lgy1 > y2)
                  {
/* cas 2                                                              */
/*           ______________                                           */
/*          | 2        ____|____                                      */
/*          |         |    | 1  |                                     */
/*          |_________|____|    |                                     */
/*                    |         |                                     */
/*                    |_________|                                     */
                     if (x1 != lgx2)
                     {
                        if (supegl_dimin ((lgx2 - x1), layer_num))
                        {
                           (*temp_list) = rds_rectangle_in ((*temp_list),
                                                            x1, yy1, lgx2 - x1, lgy2 - yy1, layer_num, mbk_type, NULL);
                        }
                     }
                     if (supegl_dimin ((lgy1 - y2), layer_num))
                     {
                        (*temp_list) = rds_rectangle_in ((*temp_list), x2, y2, lgx1 - x2, lgy1 - y2, layer_num, mbk_type, NULL);
                     }
                     return;
                  }
               }
            }
         }
      }
      if (yy1 > y2)
      {
         if (lgx1 <= lgx2)
         {
            if (yy1 <= lgy2)
            {
               if (lgy1 > lgy2)
               {
/* cas 3                                                              */
/*                   _______                                          */
/*                  | 1     |                                         */
/*               ___|_______|____                                     */
/*              | 2 |_______|    |                                    */
/*              |                |                                    */
/*              |________________|                                    */
                  desc1->Y = y2;
                  desc1->DY = lgy1 - y2;
                  mark_used (desc1);
                  return;
               }
            }
         }
      }
   }
   else
   {                            /*  x1<x2 */
      if (lgx1 >= x2)
      {
         if (yy1 < y2)
         {
            if (passage == 0)
            {
               if (lgx1 <= lgx2)
               {
                  if (lgy1 > y2)
                  {
                     if (lgx1 >= x2)
                     {
                        if (lgy1 < lgy2)
                        {
/* cas 4                                                              */
/*                      _____________                                 */
/*                     | 2           |                                */
/*             ________|_______      |                                */
/*            | 1      |_______|_____|                                */
/*            |                |                                      */
/*            |________________|                                      */
                           if (x2 != lgx1)
                           {
                              if (supegl_dimin ((lgx1 - x2), layer_num))
                              {
                                 (*temp_list) = rds_rectangle_in ((*temp_list),
                                                                  x2, yy1, lgx1 - x2, lgy2 - yy1, layer_num, mbk_type, NULL);
                              }
                           }
                           if (supegl_dimin ((lgy1 - y2), layer_num))
                           {
                              (*temp_list) = rds_rectangle_in ((*temp_list),
                                                               x1, y2, lgx2 - x1, lgy1 - y2, layer_num, mbk_type, NULL);
                           }
                           return;
                        }
                     }
                  }
               }
            }
         }
         else
         {                      /* yy1>=y2 */
            if (lgx1 >= x2)
            {
               if (lgx1 < lgx2)
               {
                  if (lgy1 <= lgy2)
                  {
/* cas 5                                                              */
/*                       _____________                                */
/*                      | 2           |                               */
/*               _______|_______      |                               */
/*              | 1     |       |     |                               */
/*              |_______|_______|     |                               */
/*                      |_____________|                               */
                     desc1->DX = lgx2 - x1;
                     mark_used (desc1);
                     return;
                  }
               }
            }
         }
         if (lgx1 >= x2)
         {
            if (lgx1 < lgx2)
            {
               if (yy1 <= y2)
               {
                  if (lgy1 >= lgy2)
                  {
/* cas 6                                                              */
/*           _____________                                            */
/*          | 1           |                                           */
/*          |       ______|______                                     */
/*          |      |      |    2 |                                    */
/*          |      |______|______|                                    */
/*          |_____________|                                           */
                     desc2->X = x1;
                     desc2->DX = lgx2 - x1;
                     mark_used (desc2);
                     return;
                  }
               }
            }
         }
      }
   }
   if (x1 <= x2)
   {
      if (lgx1 < lgx2)
      {
         if (passage == 0)
         {
            if (lgx1 >= x2)
            {
               if (yy1 > y2)
               {
                  if (lgy1 > lgy2)
                  {
                     if (yy1 <= lgy2)
                     {
/* cas 7                                                              */
/*           ______________                                           */
/*          | 1        ____|____                                      */
/*          |         |    | 2  |                                     */
/*          |_________|____|    |                                     */
/*                    |_________|                                     */
                        if (supegl_dimin ((lgy2 - yy1), layer_num))
                        {
                           (*temp_list) = rds_rectangle_in ((*temp_list),
                                                            x1, yy1, lgx2 - x1, lgy2 - yy1, layer_num, mbk_type, NULL);
                        }
                        if (x2 != lgx1)
                        {
                           if (supegl_dimin ((lgx1 - x2), layer_num))
                           {
                              (*temp_list) = rds_rectangle_in ((*temp_list),
                                                               x2, y2, lgx1 - x2, lgy1 - y2, layer_num, mbk_type, NULL);
                           }
                           return;
                        }
                     }
                  }
               }
            }
         }
      }
      else
      {                         /* x1+dx1>=x2+dx2 */
         if (yy1 < y2)
         {
            if (lgy1 >= y2)
            {
               if (lgy1 < lgy2)
               {
/* cas 8                                                              */
/*                   _________                                        */
/*                  | 2       |                                       */
/*            ______|_________|_______                                */
/*           | 1    |_________|       |                               */
/*           |                        |                               */
/*           |________________________|                               */
                  desc2->Y = yy1;
                  desc2->DY = lgy2 - yy1;
                  mark_used (desc2);
                  return;
               }
            }
         }
         if (yy1 > y2)
         {
            if (lgy1 > lgy2)
            {
               if (yy1 <= lgy2)
               {
/* cas 9                                                              */
/*             _________                                              */
/*            |    1    |                                             */
/*            |   ___   |                                             */
/*            |__|___|__|                                             */
/*               | 2 |                                                */
/*               |___|                                                */
                  desc2->DY = lgy1 - y2;
                  mark_used (desc2);
                  return;
               }
            }
         }
      }
   }
   if (x1 > x2)
   {
      if (x1 <= lgx2)
      {
         if (lgx1 > lgx2)
         {
            if (yy1 <= y2)
            {
               if (lgy1 >= lgy2)
               {
/* cas 10                                                             */
/*                       _____________                                */
/*                      | 1           |                               */
/*               _______|_______      |                               */
/*              | 2     |       |     |                               */
/*              |_______|_______|     |                               */
/*                      |_____________|                               */
                  desc2->DX = lgx1 - x2;
                  mark_used (desc2);
                  return;
               }
            }
            if (yy1 >= y2)
            {
               if (lgy1 <= lgy2)
               {
/* cas 11                                                             */
/*           _____________                                            */
/*          | 2           |                                           */
/*          |       ______|______                                     */
/*          |      |      |    1 |                                    */
/*          |      |______|______|                                    */
/*          |_____________|                                           */
                  desc1->X = x2;
                  desc1->DX = lgx1 - x2;
                  mark_used (desc1);
                  return;
               }
            }
         }
         if (passage == 0)
         {
            if (lgx1 >= lgx2)
            {
               if (yy1 > y2)
               {
                  if (lgy1 > lgy2)
                  {
                     if (yy1 <= lgy2)
                     {
/* cas 12                                                             */
/*                      _____________                                 */
/*                     | 1           |                                */
/*             ________|_______      |                                */
/*            | 2      |_______|_____|                                */
/*            |                |                                      */
/*            |________________|                                      */
                        if (x1 != lgx2)
                        {
                           if (supegl_dimin ((lgx2 - x1), layer_num))
                           {
                              (*temp_list) = rds_rectangle_in ((*temp_list),
                                                               x1, y2, lgx2 - x1, lgy1 - y2, layer_num, mbk_type, NULL);
                           }
                        }
                        if (supegl_dimin ((lgy2 - yy1), layer_num))
                        {
                           (*temp_list) = rds_rectangle_in ((*temp_list),
                                                            x2, yy1, lgx1 - x2, lgy2 - yy1, layer_num, mbk_type, NULL);
                        }
                        return;
                     }
                  }
               }
            }
         }
      }
      return;
   }
}                               /* END contac() */


/*****************************************************************************
 * rect_list_sup: compares the given rectangle ( given by its coordinates) to
 *                every rectangle in the given list, using their quantities of
 *                oversize, and determines if they are intersecting in the
 *                normal case ( not oversized). If they are, it returns 1, else
 *                it returns 0.
 ******************************************************************************/
static int rect_list_sup (x, y, dx, dy, list, qty1, qty2)
     register long x, y, dx, dy;
     rdsrec_list *list;
     register long qty1, qty2;
{
   register rdsrec_list *rectp;

   for (rectp = list; rectp; rectp = rectp->NEXT)
   {
      /* 1 is added as intersec is true if the 2 rect. touch which is not our case */
      if (are_rect_intersec (x + qty1 + 1, y + qty1 + 1,
                             dx - 2 * qty1 - 2, dy - 2 * qty1 - 2,
                             rectp->X + qty2, rectp->Y + qty2, rectp->DX - 2 * qty2, rectp->DY - 2 * qty2))
      {
         break;
      }
   }
   if (rectp)
   {
      return (1);
   }
   else
   {
      return (0);
   }
}


/*************************************************************************
 * contact_2: determines if the 2 given rectangles intersect each other.
 *          If it is the case:
 *              - it extends one of them and marks it "used".
 *          or  - it creates 1 or 2 new compensating rectangles and adds them
 *                to the given list.
 *              - If there is a need for compensating rectangles and they
 *                cannot be created as they would have a dimension less than
 *                the minimum width permitted for a segment in the given layer:
 *                  + a pair of scotch will be considered and will be compared
 *                    to all rectangles in the complementary layers (given
 *                    by temp_list_s and temp_list_t).
 *                      * If it does not intersect any of them one rectangle
 *                        of the pair is created and the other will be created
 *                        implicitly in following passes.
 *                      * If it intersects one of them, another pair will be
 *                        considered.
 *                  + The other pair will pass the same test.
 *                      * If it does not intersect any of them one rectangle
 *                        of the pair is created and the other will be created
 *                        implicitly in following passes.
 *                      * If it intersects one of them an error will be
 *                        signaled.
 *             Remarks:
 *                - Rectangles are created only in the first pass except if
 *                  one or both of them is a scotch.
 *                - If a scotch is created then the variable "modif" is set
 *                  to 1 to permit another pass; so as to consider the
 *                  possibility of new merges due to the newly created scotch.
 ******************************************************************************/

static void contact_2 (temp_list_p, temp_list_s, temp_list_t, layer_num, passage, desc1, desc2, modif)
     rds_rectanglep_typ *temp_list_p;
     rdsrec_list *temp_list_s, *temp_list_t;
     int layer_num;
     char passage;
     rdsrec_list *desc1, *desc2;
     unsigned char *modif;
{
   long mbk_type;
   register long x1, dx1, yy1, dy1, x2, dx2, y2, dy2;
   register long sx, sdx, sy, sdy;
   char need_scotch = 0;
   long minwdth_os, qty1, qty2;

   mbk_type = GetRdsMbkType (desc1);
   x1 = desc1->X;
   yy1 = desc1->Y;
   dx1 = desc1->DX;
   dy1 = desc1->DY;
   x2 = desc2->X;
   y2 = desc2->Y;
   dx2 = desc2->DX;
   dy2 = desc2->DY;

   if (x1 >= x2)
   {
      if (yy1 < y2)
      {
         if (lgy1 < lgy2)
         {
            if (lgx1 <= lgx2)
            {
               if (lgy1 >= y2)
               {
/* cas 1                                                              */
/*             _________                                              */
/*            |    2    |                                             */
/*            |   ___   |                                             */
/*            |__|___|__|                                             */
/*               | 1 |                                                */
/*               |___|                                                */
                  desc1->DY = lgy2 - yy1;
                  mark_used (desc1);
                  return;
               }
            }
            else if ((passage == 0) || (is_scotch (desc1)) || (is_scotch (desc2)))
            {
               if (x1 <= lgx2)
               {
                  if (lgy1 > y2)
                  {
/* cas 2                                                              */
/*           ______________                                           */
/*          | 2        ____|____                                      */
/*          |         |    | 1  |                                     */
/*          |_________|____|    |                                     */
/*                    |         |                                     */
/*                    |_________|                                     */
                     if (x1 != lgx2)
                     {
                        if (supegl_dimin ((lgx2 - x1), layer_num))
                        {
                           (*temp_list_p) = rds_rectangle_in ((*temp_list_p),
                                                              x1, yy1, lgx2 - x1, lgy2 - yy1, layer_num, mbk_type, NULL);
                        }
                        else
                        {
                           need_scotch = 1;
                        }
                     }
                     if (supegl_dimin ((lgy1 - y2), layer_num))
                     {
                        (*temp_list_p) = rds_rectangle_in ((*temp_list_p), x2, y2, lgx1 - x2, lgy1 - y2, layer_num, mbk_type, NULL);
                        need_scotch = 0;
                     }
                     if (need_scotch)
                     {
                        qty1 = GET_S2R_OVERSIZE (layer_num);
                        qty2 = GET_S2R_POST_COMPLEMENTARY (layer_num);

                        if (qty2 != RDS_S2R_POST_TREAT_EMPTY)
                        {
                           qty2 = GET_S2R_OVERSIZE (qty2);
                        }
                        else
                        {
                           qty2 = 0;
                        }
                        minwdth_os = GET_S2R_LAYER_WIDTH (layer_num) + 2 * qty1;
                        STAT_SCTCH_RQRD++;
                        sx = lgx2 - minwdth_os;
                        sy = lgy1 - minwdth_os;
                        sdx = minwdth_os;
                        sdy = lgy2 - lgy1 + minwdth_os;
                        if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                        {
                           (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);

                           mark_scotch (*temp_list_p);
                           STAT_SCTCH_CRTD++;
                           *modif = 1;
                        }
                        else
                        {
                           sx = x2;
                           sy = y2;
                           sdx = x1 - x2 + minwdth_os;
                           sdy = minwdth_os;
                           if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                           {
                              (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);
                              mark_scotch (*temp_list_p);
                              STAT_SCTCH_CRTD++;
                              *modif = 1;
                           }
                           else
                           {
                              fprintf (stderr, "***s2r error***, scotch error (%.1f, %.1f)\n",
                                       (float) sx / RDS_LAMBDA, (float) sy / RDS_LAMBDA);
                           }
                        }
                     }
                     return;
                  }
               }
            }
         }
      }
      if (yy1 > y2)
      {
         if (lgx1 <= lgx2)
         {
            if (yy1 <= lgy2)
            {
               if (lgy1 > lgy2)
               {
/* cas 3                                                              */
/*                   _______                                          */
/*                  | 1     |                                         */
/*               ___|_______|____                                     */
/*              | 2 |_______|    |                                    */
/*              |                |                                    */
/*              |________________|                                    */
                  desc1->Y = y2;
                  desc1->DY = lgy1 - y2;
                  mark_used (desc1);
                  return;
               }
            }
         }
      }
   }
   else
   {                            /*  x1<x2 */
      if (lgx1 >= x2)
      {
         if (yy1 < y2)
         {
            if ((passage == 0) || (is_scotch (desc1)) || (is_scotch (desc2)))
            {
               if (lgx1 <= lgx2)
               {
                  if (lgy1 > y2)
                  {
                     if (lgx1 >= x2)
                     {
                        if (lgy1 < lgy2)
                        {
/* cas 4                                                              */
/*                      _____________                                 */
/*                     | 2           |                                */
/*             ________|_______      |                                */
/*            | 1      |_______|_____|                                */
/*            |                |                                      */
/*            |________________|                                      */
                           if (x2 != lgx1)
                           {
                              if (supegl_dimin ((lgx1 - x2), layer_num))
                              {
                                 (*temp_list_p) = rds_rectangle_in ((*temp_list_p),
                                                                    x2, yy1, lgx1 - x2, lgy2 - yy1, layer_num, mbk_type, NULL);
                              }
                              else
                              {
                                 need_scotch = 1;
                              }
                           }
                           if (supegl_dimin ((lgy1 - y2), layer_num))
                           {
                              (*temp_list_p) = rds_rectangle_in ((*temp_list_p),
                                                                 x1, y2, lgx2 - x1, lgy1 - y2, layer_num, mbk_type, NULL);
                              need_scotch = 0;
                           }
                           if (need_scotch)
                           {
                              qty1 = GET_S2R_OVERSIZE (layer_num);
                              qty2 = GET_S2R_POST_COMPLEMENTARY (layer_num);

                              if (qty2 != RDS_S2R_POST_TREAT_EMPTY)
                              {
                                 qty2 = GET_S2R_OVERSIZE (qty2);
                              }
                              else
                              {
                                 qty2 = 0;
                              }
                              minwdth_os = GET_S2R_LAYER_WIDTH (layer_num) + 2 * qty1;
                              STAT_SCTCH_RQRD++;
                              sx = lgx1 - minwdth_os;
                              sy = y2;
                              sdx = lgx2 - lgx1 + minwdth_os;
                              sdy = minwdth_os;
                              if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                              {
                                 (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);
                                 mark_scotch (*temp_list_p);
                                 STAT_SCTCH_CRTD++;
                                 *modif = 1;
                              }
                              else
                              {
                                 sx = x2;
                                 sy = lgy1 - minwdth_os;
                                 sdx = minwdth_os;
                                 sdy = lgy2 - lgy1 + minwdth_os;
                                 if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                                 {
                                    (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);
                                    mark_scotch (*temp_list_p);
                                    STAT_SCTCH_CRTD++;
                                    *modif = 1;
                                 }
                                 else
                                 {
                                    fprintf (stderr, "***s2r error***, scotch error (%.1f, %.1f)\n",
                                             (float) sx / RDS_LAMBDA, (float) sy / RDS_LAMBDA);
                                 }
                              }
                           }
                           return;
                        }
                     }
                  }
               }
            }
         }
         else
         {                      /* yy1>=y2 */
            if (lgx1 >= x2)
            {
               if (lgx1 < lgx2)
               {
                  if (lgy1 <= lgy2)
                  {
/* cas 5                                                              */
/*                       _____________                                */
/*                      | 2           |                               */
/*               _______|_______      |                               */
/*              | 1     |       |     |                               */
/*              |_______|_______|     |                               */
/*                      |_____________|                               */
                     desc1->DX = lgx2 - x1;
                     mark_used (desc1);
                     return;
                  }
               }
            }
         }
         if (lgx1 >= x2)
         {
            if (lgx1 < lgx2)
            {
               if (yy1 <= y2)
               {
                  if (lgy1 >= lgy2)
                  {
/* cas 6                                                              */
/*           _____________                                            */
/*          | 1           |                                           */
/*          |       ______|______                                     */
/*          |      |      |    2 |                                    */
/*          |      |______|______|                                    */
/*          |_____________|                                           */
                     desc2->X = x1;
                     desc2->DX = lgx2 - x1;
                     mark_used (desc2);
                     return;
                  }
               }
            }
         }
      }
   }
   if (x1 <= x2)
   {
      if (lgx1 < lgx2)
      {
         if ((passage == 0) || (is_scotch (desc1)) || (is_scotch (desc2)))
         {
            if (lgx1 >= x2)
            {
               if (yy1 > y2)
               {
                  if (lgy1 > lgy2)
                  {
                     if (yy1 <= lgy2)
                     {
/* cas 7                                                              */
/*           ______________                                           */
/*          | 1        ____|____                                      */
/*          |         |    | 2  |                                     */
/*          |_________|____|    |                                     */
/*                    |_________|                                     */
                        if (supegl_dimin ((lgy2 - yy1), layer_num))
                        {
                           (*temp_list_p) = rds_rectangle_in ((*temp_list_p),
                                                              x1, yy1, lgx2 - x1, lgy2 - yy1, layer_num, mbk_type, NULL);
                        }
                        else
                        {
                           need_scotch = 1;
                        }
                        if (x2 != lgx1)
                        {
                           if (supegl_dimin ((lgx1 - x2), layer_num))
                           {
                              (*temp_list_p) = rds_rectangle_in ((*temp_list_p),
                                                                 x2, y2, lgx1 - x2, lgy1 - y2, layer_num, mbk_type, NULL);
                              need_scotch = 0;
                           }
                        }
                        if (need_scotch)
                        {
                           qty1 = GET_S2R_OVERSIZE (layer_num);
                           qty2 = GET_S2R_POST_COMPLEMENTARY (layer_num);

                           if (qty2 != RDS_S2R_POST_TREAT_EMPTY)
                           {
                              qty2 = GET_S2R_OVERSIZE (qty2);
                           }
                           else
                           {
                              qty2 = 0;
                           }
                           minwdth_os = GET_S2R_LAYER_WIDTH (layer_num) + 2 * qty1;
                           STAT_SCTCH_RQRD++;
                           sx = lgx1 - minwdth_os;
                           sy = lgy2 - minwdth_os;
                           sdx = minwdth_os;
                           sdy = lgy1 - lgy2 + minwdth_os;
                           if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                           {
                              (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);
                              mark_scotch (*temp_list_p);
                              STAT_SCTCH_CRTD++;
                              *modif = 1;
                           }
                           else
                           {
                              sx = x2;
                              sy = y2;
                              sdx = minwdth_os;
                              sdy = yy1 - y2 + minwdth_os;
                              if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                              {
                                 (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);
                                 mark_scotch (*temp_list_p);
                                 STAT_SCTCH_CRTD++;
                                 *modif = 1;
                              }
                              else
                              {
                                 fprintf (stderr, "***s2r error***, scotch error (%.1f, %.1f)\n",
                                          (float) sx / RDS_LAMBDA, (float) sy / RDS_LAMBDA);
                              }
                           }
                        }
                        return;
                     }
                  }
               }
            }
         }
      }
      else
      {                         /* x1+dx1>=x2+dx2 */
         if (yy1 < y2)
         {
            if (lgy1 >= y2)
            {
               if (lgy1 < lgy2)
               {
/* cas 8                                                              */
/*                   _________                                        */
/*                  | 2       |                                       */
/*            ______|_________|_______                                */
/*           | 1    |_________|       |                               */
/*           |                        |                               */
/*           |________________________|                               */
                  desc2->Y = yy1;
                  desc2->DY = lgy2 - yy1;
                  mark_used (desc2);
                  return;
               }
            }
         }
         if (yy1 > y2)
         {
            if (lgy1 > lgy2)
            {
               if (yy1 <= lgy2)
               {
/* cas 9                                                              */
/*             _________                                              */
/*            |    1    |                                             */
/*            |   ___   |                                             */
/*            |__|___|__|                                             */
/*               | 2 |                                                */
/*               |___|                                                */
                  desc2->DY = lgy1 - y2;
                  mark_used (desc2);
                  return;
               }
            }
         }
      }
   }
   if (x1 > x2)
   {
      if (x1 <= lgx2)
      {
         if (lgx1 > lgx2)
         {
            if (yy1 <= y2)
            {
               if (lgy1 >= lgy2)
               {
/* cas 10                                                             */
/*                       _____________                                */
/*                      | 1           |                               */
/*               _______|_______      |                               */
/*              | 2     |       |     |                               */
/*              |_______|_______|     |                               */
/*                      |_____________|                               */
                  desc2->DX = lgx1 - x2;
                  mark_used (desc2);
                  return;
               }
            }
            if (yy1 >= y2)
            {
               if (lgy1 <= lgy2)
               {
/* cas 11                                                             */
/*           _____________                                            */
/*          | 2           |                                           */
/*          |       ______|______                                     */
/*          |      |      |    1 |                                    */
/*          |      |______|______|                                    */
/*          |_____________|                                           */
                  desc1->X = x2;
                  desc1->DX = lgx1 - x2;
                  mark_used (desc1);
                  return;
               }
            }
         }
         if ((passage == 0) || (is_scotch (desc1)) || (is_scotch (desc2)))
         {
            if (lgx1 >= lgx2)
            {
               if (yy1 > y2)
               {
                  if (lgy1 > lgy2)
                  {
                     if (yy1 <= lgy2)
                     {
/* cas 12                                                             */
/*                      _____________                                 */
/*                     | 1           |                                */
/*             ________|_______      |                                */
/*            | 2      |_______|_____|                                */
/*            |                |                                      */
/*            |________________|                                      */
                        if (x1 != lgx2)
                        {
                           if (supegl_dimin ((lgx2 - x1), layer_num))
                           {
                              (*temp_list_p) = rds_rectangle_in ((*temp_list_p),
                                                                 x1, y2, lgx2 - x1, lgy1 - y2, layer_num, mbk_type, NULL);
                           }
                           else
                           {
                              need_scotch = 1;
                           }
                        }
                        if (supegl_dimin ((lgy2 - yy1), layer_num))
                        {
                           (*temp_list_p) = rds_rectangle_in ((*temp_list_p),
                                                              x2, yy1, lgx1 - x2, lgy2 - yy1, layer_num, mbk_type, NULL);
                           need_scotch = 0;
                        }
                        if (need_scotch)
                        {
                           qty1 = GET_S2R_OVERSIZE (layer_num);
                           qty2 = GET_S2R_POST_COMPLEMENTARY (layer_num);

                           if (qty2 != RDS_S2R_POST_TREAT_EMPTY)
                           {
                              qty2 = GET_S2R_OVERSIZE (qty2);
                           }
                           else
                           {
                              qty2 = 0;
                           }
                           minwdth_os = GET_S2R_LAYER_WIDTH (layer_num) + 2 * qty1;
                           STAT_SCTCH_RQRD++;
                           sx = lgx2 - minwdth_os;
                           sy = yy1;
                           sdx = lgx1 - lgx2 + minwdth_os;
                           sdy = minwdth_os;
                           if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                           {
                              (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);
                              mark_scotch (*temp_list_p);
                              STAT_SCTCH_CRTD++;
                              *modif = 1;
                           }
                           else
                           {
                              sx = x2;
                              sy = lgy2 - minwdth_os;
                              sdx = x1 - x2 + minwdth_os;
                              sdy = minwdth_os;
                              if (!rect_list_sup (sx, sy, sdx, sdy, temp_list_s, qty1, qty2))
                              {
                                 (*temp_list_p) = rds_rectangle_in ((*temp_list_p), sx, sy, sdx, sdy, layer_num, mbk_type, NULL);

                                 mark_scotch (*temp_list_p);
                                 STAT_SCTCH_CRTD++;
                                 *modif = 1;
                              }
                              else
                              {
                                 fprintf (stderr, "***s2r error***, scotch error (%.1f, %.1f)\n",
                                          (float) sx / RDS_LAMBDA, (float) sy / RDS_LAMBDA);
                              }
                           }
                        }
                        return;
                     }
                  }
               }
            }
         }
      }
   }
   return;
}                               /* END contact_2() */

/*****************************************************************************
 * merge_layer_win: merges the rectangles of the given list - given also their
 *                  layer.
 *                  Merge consists of:
 *                      - eliminating redundancy (inclusion).
 *                      - extending rectangles.
 *                      - creating compensation rectangles.
 ****************************************************************************/

static void merge_layer_win (temp_list, layer_num)
     register rds_rectanglep_typ *temp_list;
     register int layer_num;
{
   unsigned char modif, modif1, modif2;
   register char passage = 0;
   register rdsrec_list *desc1;
   register rdsrec_list *desc2;

   do
   {
      modif = 0;
      for (desc1 = (*temp_list); desc1; desc1 = desc1->NEXT)
      {
         if (is_remove (desc1) || IsRdsConExter (desc1) || IsRdsRefRef (desc1) || IsRdsRefCon (desc1))
            continue;

         for (desc2 = desc1->NEXT; desc2; desc2 = desc2->NEXT)
         {
            if (is_remove (desc2) || IsRdsConExter (desc2) || IsRdsRefRef (desc2) || IsRdsRefCon (desc2))
               continue;

            modif1 = 0;
            modif2 = 0;
            /* appel des tests permettant l'unification  */
            inclus (&modif, &modif1, &modif2, desc1, desc2);

            if (modif1 == 0)
            {
               if (modif2 == 0)
               {
                  prolong (&modif, &modif1, desc1, desc2);
                  if (modif1 == 0)
                  {
                     contact (temp_list, layer_num, passage, desc1, desc2);
                  }
                  else
                     break;
               }
            }
            else
               break;
         }                      /* fin for desc2 */
      }                         /* fin for desc1 */
      /* les creations de masques sont faites dans la premiere passe du while(), passage=0.
         Ceci signifie que un masque genere n'entraine pas de creation de masques. Le fait de
         travailler avec un desc2 toujours > a desc1, elimine toute redondance d'unification
       */
      rds_remove_rectangle (temp_list);
      passage++;
   }
   while (modif == 1);
}                               /* END merge_layer_win() */


/*****************************************************************************
 * merge_layer_win_2: merges the rectangles of the given list - given also their
 *                  layer.
 *                  Merge consists of:
 *                      - eliminating redundancy (inclusion).
 *                      - extending rectangles.
 *                      - creating compensation rectangles.
 *                      - creating scotchs.
 ****************************************************************************/

static void merge_layer_win_2 (layer_num, temp_list_p, temp_list_s, temp_list_t)
     register int layer_num;
     register rds_rectanglep_typ *temp_list_p;
     register rdsrec_list *temp_list_s, *temp_list_t;
{
   unsigned char modif, modif1, modif2;
   register char passage = 0;
   register rdsrec_list *desc1;
   register rdsrec_list *desc2;

   do
   {
      modif = 0;
      for (desc1 = (*temp_list_p); desc1; desc1 = desc1->NEXT)
      {
         if (is_remove (desc1) || IsRdsConExter (desc1) || IsRdsRefRef (desc1) || IsRdsRefCon (desc1))
            continue;

         for (desc2 = desc1->NEXT; desc2; desc2 = desc2->NEXT)
         {
            if (is_remove (desc2) || IsRdsConExter (desc2) || IsRdsRefRef (desc2) || IsRdsRefCon (desc2))
               continue;
            modif1 = 0;
            modif2 = 0;

            /* appel des tests permettant l'unification  */
            inclus (&modif, &modif1, &modif2, desc1, desc2);
            if (modif1 == 0)
            {
               if (modif2 == 0)
               {
                  prolong (&modif, &modif1, desc1, desc2);
                  if (modif1 == 0)
                     contact_2 (temp_list_p, temp_list_s, temp_list_t, layer_num, passage, desc1, desc2, &modif);
                  else
                     break;
               }
            }
            else
               break;
         }                      /* fin for desc2 */
         if (is_scotch (desc1))
            demark_scotch (desc1);
      }                         /* fin for desc1 */
      /* les creations de masques sont faites dans la premiere passe du while(), passage=0.
         Ceci signifie que un masque genere n'entraine pas de creation de masques. Le fait de
         travailler avec un desc2 toujours > a desc1, elimine toute redondance d'unification
       */
      rds_remove_rectangle (temp_list_p);
      passage++;
   }
   while (modif == 1);
}                               /* END merge_layer_win2 () */


/****************************************************************************
 * calculate_window: calculates the window used in merging the rectangles of
 *                   the specified layer of the specified model.
 *                   The dimensions of this window are put in dx and dy; and
 *                   the number of times it will partition the x dimension
 *                   and the y dimension of the envelop of the model are put
 *                   in nx and ny respectively.
 ***************************************************************************/

static void calculate_window (model, layer_num, dx, dy, nx, ny)
     rdsfig_list *model;
     int layer_num;
     long *dx, *dy, *nx, *ny;
{
   double tot_num_seg, nb_win, sq_win;
   rdsrec_list *rect;

   if (is_empty (model, layer_num))
      printf ("There is an error\n\n");
   for (rect = model->LAYERTAB[layer_num], tot_num_seg = 0; rect; rect = rect->NEXT, tot_num_seg++);
   if (tot_num_seg == 0)
   {
      *dx = *dy = *nx = *ny = 0;
   }
   else
   {
      nb_win = 2 * (double) sqrt ((double) tot_num_seg);
      sq_win = (double) sqrt ((double) nb_win);
      *nx = (long) sq_win + 1;
      *ny = *nx;
      *dx = model->BBOXTAB[layer_num]->DX / (*nx) + 1;
      *dy = model->BBOXTAB[layer_num]->DY / (*ny) + 1;
   }
}


/******************************************************************************
 * concat_list: concatenates the rectangle list 2 to the rectangle list 1.
 *****************************************************************************/

static void concat_list (list1, list2)
     rds_rectanglep_typ *list1;
     rdsrec_list *list2;
{
   typedef rdsrec_list *rds_rectangle_typ;
   rds_rectangle_typ *layer_tail;
   register rdsrec_list *rectp;

   /* get the tail of list1 */
   for (rectp = (*list1); rectp && rectp->NEXT; rectp = rectp->NEXT);
   if (rectp)
      layer_tail = &(rectp->NEXT);
   else
      layer_tail = list1;
   *layer_tail = list2;
}

/******************************************************************************
 * merge_layer: merges the rectangles of the specified layer of the specified
 *              model.
 *****************************************************************************/

void merge_layer (model, layer_num, verbose)
     rdsfig_list *model;
     int layer_num;
     int verbose;
{
   long corn_width, dx, dy, nx, ny, wx, wy, countx, county;
   rdsrec_list *temp_list;
   long nb_win, prc = 0, new_prc = 0, iteration, dot = 0;

   if (model->LAYERTAB[layer_num])
   {
      if (verbose)
      {
         printf ("\t    . %s ", RDS_LAYER_NAME[layer_num]);
         fflush (stdout);
      }
      /* corn_width       = GET_S2R_RING_WIDTH( layer_num ); */
      corn_width = 0;
      calculate_window (model, layer_num, &dx, &dy, &nx, &ny);
      nb_win = nx * ny;
      iteration = 0;

      for (countx = 0; countx < nx; countx++)
      {
         for (county = 0; county < ny; county++)
         {
            iteration++;
            if (verbose)
            {
               new_prc = (iteration * 100 / nb_win) / 3; /* one dot each 3% */
               if (new_prc > prc)
               {
                  dot++;
                  printf (".");
                  fflush (stdout);
                  prc = new_prc;
               }
            }
            wx = model->BBOXTAB[layer_num]->X + countx * dx - corn_width;
            wy = model->BBOXTAB[layer_num]->Y + county * dy - corn_width;

            temp_list = ext_win_segs (model, layer_num, wx, wy, dx + 2 * corn_width, dy + 2 * corn_width);

            merge_layer_win (&(temp_list), layer_num);
            concat_list (&(model->LAYERTAB[layer_num]), temp_list);
         }
      }
      if (verbose)
      {
         for (; dot < 33; dot++)
            printf (".");
         printf ("\n");
         fflush (stdout);
      }
   }
}

/******************************************************************************
 * merge_layer_2: merges the rectangles of the specified layer of the specified
 *              model.
 *              remarks:
 *              - It differs with merge_layer() in that it calls
 *              merge_layer_win_2() instead of merge_layer_win().
 *              - IT makes another merge in the reverse windowing direction
 *              to compensate changes that might be done by scotchs.
 *****************************************************************************/

void merge_layer_2 (model, layer_num, verbose)
     rdsfig_list *model;
     int layer_num;
     int verbose;
{
   long corn_width, countx, county, wx, wy, dx, dy, nx, ny;
   rdsrec_list *temp_list_p, *temp_list_s, *temp_list_t;
   int comp_layer;
   long dot = 0, nb_win, prc = 0, new_prc = 0, iteration;

   temp_list_t = (rdsrec_list *) 0;
   temp_list_p = (rdsrec_list *) 0;
   temp_list_s = (rdsrec_list *) 0;

   if (model->LAYERTAB[layer_num])
   {
      if (verbose)
      {
         printf ("\t    . %s ", RDS_LAYER_NAME[layer_num]);
         fflush (stdout);
      }
/*  corn_width       = GET_S2R_RING_WIDTH( layer_num ); */
      corn_width = 0;
      calculate_window (model, layer_num, &dx, &dy, &nx, &ny);
      nb_win = nx * ny;
      iteration = 0;
      for (countx = 0; countx < nx; countx++)
      {
         for (county = 0; county < ny; county++)
         {
            iteration++;
            if (verbose)
            {
               new_prc = (iteration * 100 / nb_win) / 6; /* one dot each 6% */
               if (new_prc > prc)
               {
                  dot++;
                  printf (".");
                  fflush (stdout);
                  prc = new_prc;
               }
            }
            wx = model->BBOXTAB[layer_num]->X + countx * dx - corn_width;
            wy = model->BBOXTAB[layer_num]->Y + county * dy - corn_width;
            temp_list_p = ext_win_segs (model, layer_num, wx, wy, dx + 2 * corn_width, dy + 2 * corn_width);

            comp_layer = GET_S2R_POST_COMPLEMENTARY (layer_num);
            if (comp_layer != RDS_S2R_POST_TREAT_EMPTY)
            {
               temp_list_s = model->LAYERTAB[comp_layer];
            }
            else
            {
               temp_list_s = NULL;
            }
            merge_layer_win_2 (layer_num, &(temp_list_p), temp_list_s, temp_list_t);
            concat_list (&(model->LAYERTAB[layer_num]), temp_list_p);
         }
      }
      iteration = 0;
      prc = 0;

      /* another pass beginning with the windows in opposite direction
       * to resolve the problems that might appear if creating of scotchs
       * result in creation of rectangles in a window previously treated
       */

      for (countx = nx - 1; countx >= 0; countx--)
      {
         for (county = ny - 1; county >= 0; county--)
         {
            iteration++;
            if (verbose)
            {
               new_prc = (iteration * 100 / nb_win) / 6; /* one dot each 6% */
               if (new_prc > prc)
               {
                  dot++;
                  printf (".");
                  fflush (stdout);
                  prc = new_prc;
               }
            }
            wx = model->BBOXTAB[layer_num]->X + countx * dx - corn_width;
            wy = model->BBOXTAB[layer_num]->Y + county * dy - corn_width;

            temp_list_p = ext_win_segs (model, layer_num, wx, wy, dx + 2 * corn_width, dy + 2 * corn_width);
            merge_layer_win_2 (layer_num, &(temp_list_p), temp_list_s, temp_list_t);
            concat_list (&(model->LAYERTAB[layer_num]), temp_list_p);
         }
      }
      if (verbose)
      {
         for (; dot < 33; dot++)
            printf (".");
         printf ("\n");
         fflush (stdout);
      }
   }
}
