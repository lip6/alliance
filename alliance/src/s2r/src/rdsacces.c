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
/*    File    : rdsacces+.c                                                 */
/*                                                                          */
/*    (c) copyright 1992 MASI laboratory CAO & VLSI team                    */
/*    All Right Reserved                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*               Access functions to the rds database objects               */
/*                                                                          */
/****************************************************************************/

#include "generic.h"
#include  <rds.h>
#include  <mph.h>
#include  <mut.h>
#include  <rfm.h>
#include  <rpr.h>
#include "rdsacces.h"
#include "statistics.h"

int s2rdebug = 0;

rdsrec_list *Free_List = NULL;  /* a global variable pointing to the free list
                                   of rectangles */
void view_rec (rec)
     rdsrec_list *rec;
{
   viewrdsrec (rec);
}

/****************************************************************************
 * rds_rectangle_in: adds the rectangle given by its data to the given list
 *                   and returns a pointer to the new list.
 *                   Before allocating memory for the new rectangle, the free
 *                   list is checked and if it is not empty : we use the memory
 *                   area pointed by the first element of this list.
 ****************************************************************************/

rdsrec_list *rds_rectangle_in (layer_list, x, y, dx, dy, layer_num, flags, name)
     rdsrec_list *layer_list;
     long x, y, dx, dy;
     int layer_num;
     long flags;
     char *name;
{
   rdsrec_list *npr;

   STAT_RECT_REQRD++;
   if (Free_List)
      npr = pull_free_list ();
   else
   {
      npr = allocrdsrec (0);
      STAT_RECT_ALLOC++;
   }

   flags = flags & 0xFFFFFF00;

   npr->NEXT = layer_list;
   npr->X = x;
   npr->Y = y;
   npr->DX = dx;
   npr->DY = dy;
   npr->FLAGS = flags | (long) (layer_num);
   npr->NAME = name;

   return layer_list = npr;
}


/****************************************************************************
 * rds_instance_in: adds the instance given by its data to the given list
 *                   and returns a pointer to the new list.
 ****************************************************************************/

rdsins_list *rds_instance_in (instance, model, name, x, y, sym)
     rdsins_list *instance;
     char *model, *name;
     long x, y;
     char sym;
{
   rdsins_list *npr;
   int i;

   npr = allocrdsins ();

   npr->NEXT = instance;
   npr->FIGNAME = namealloc (model);
   npr->INSNAME = namealloc (name);
   npr->X = x;
   npr->Y = y;
   npr->TRANSF = sym;
   for (i = 0; i < RDS_MAX_LAYER; i++)
      npr->COURONNE[i] = NULL;

   return instance = npr;
}


/******************************************************************************
 * rds_remove_model: removes the model with the specified name from the list
 *                   of models.
 ****************************************************************************/

void rds_remove_model (name)
     char *name;
{

   rds_figurep_typ *modelp;

   name = namealloc (name);
   for (modelp = &(HEAD_RDSFIG); *modelp;)
      if ((*modelp)->NAME == name)
         *modelp = (*modelp)->NEXT;
    /*****************************************************
     *** return to system the free memory would be cue ***
     *****************************************************/
      else
         modelp = &((*modelp)->NEXT);
}


/****************************************************************************
 * rds_create_model: creates a new model with the given name and adds it to
 *                   the list of models pointed by HeadFigureRds and returns
 *                   a pointer to the created model.
 *                   Note: if the model exists it creates a new model and
 *                   remove the existing one.
 ****************************************************************************/

rdsfig_list *rds_create_model (name)
     char *name;
{
   rdsfig_list *npr;

   rds_remove_model (name);
   npr = allocrdsfig ();

   npr->NEXT = HEAD_RDSFIG;
   npr->NAME = namealloc (name);
   npr->INSTANCE = NULL;

   return HEAD_RDSFIG = npr;
}


/******************************************************************************
 * rds_model_out: given a model name it returns a pointer to it by inspecting
 *                the model list pointed by HeadFigureRds.
 ****************************************************************************/

rdsfig_list *rds_model_out (name)
     char *name;
{
   rdsfig_list *pt;

   name = namealloc (name);

   for (pt = HEAD_RDSFIG; pt; pt = pt->NEXT)
      if (pt->NAME == name)
         return pt;
   return NULL;
}


/*****************************************************************************
 * rds_instance_out: given an instance name and a pointer to its father model
 *                   it returns a poiner to the instance.
 ****************************************************************************/

rdsins_list *rds_instance_out (rds_model, name)
     rdsfig_list *rds_model;
     char *name;
{
   rdsins_list *pt;

   name = namealloc (name);

   for (pt = rds_model->INSTANCE; pt; pt = pt->NEXT)
      if (pt->INSNAME == name)
         return pt;
   return NULL;
}


/*****************************************************************************
* rds_mbk_kill: destroies mbk database. May take time ...
*****************************************************************************/

void rds_mbk_kill ()
{
   phfig_list *pt, *pt_next;

   for (pt = HEAD_PHFIG; pt; pt = pt_next)
   {
      pt_next = pt->NEXT;
      delphfig (pt->NAME);
   }
}


/*******************************************************************************
 * mark_remove: marks the given rectangle as to be removed
 ******************************************************************************/

void mark_remove (rectangle)
     rdsrec_list *rectangle;
{
   if (!(is_remove (rectangle)))
      rectangle->FLAGS += REMOVE;
}


/*******************************************************************************
 * demark_flattenres: removes the flag of the given rectangle
 ******************************************************************************/

void demark_flattenres (rectangle)
     rdsrec_list *rectangle;
{
   if (is_flattenres (rectangle))
      rectangle->FLAGS -= FLATTENRES;
}


/*******************************************************************************
 * mark_flattenres: marks the given rectangle as a result of a flatten operation
 ******************************************************************************/

void mark_flattenres (rectangle)
     rdsrec_list *rectangle;
{
   if (!(is_flattenres (rectangle)))
      rectangle->FLAGS += FLATTENRES;
}


/*******************************************************************************
 * demark_used: removes the used flag
 ******************************************************************************/

void demark_used (rectangle)
     rdsrec_list *rectangle;
{
   if (is_used (rectangle))
      rectangle->FLAGS -= USED;
}


/*******************************************************************************
 * mark_used: marks the given rectangle as used
 ******************************************************************************/

void mark_used (rectangle)
     rdsrec_list *rectangle;
{
   if (!(is_used (rectangle)))
      rectangle->FLAGS += USED;
}


/******************************************************************************
 * mark_scotch: marks the given rectangle as a scotch.
 ******************************************************************************/

void mark_scotch (rectangle)
     rdsrec_list *rectangle;
{
   if (!(is_scotch (rectangle)))
   {
      rectangle->FLAGS += SCOTCH;
   }
}


/******************************************************************************
 * demark_scotch: removes the scotch flag from the rectangle.
 ******************************************************************************/

void demark_scotch (rectangle)
     rdsrec_list *rectangle;
{
   if (is_scotch (rectangle))
      rectangle->FLAGS -= SCOTCH;
}


/******************************************************************************
 * mark_old_scotch: marks the given rectangle as an old scotch
 ******************************************************************************/

void mark_old_scotch (rectangle)
     rdsrec_list *rectangle;
{
   if (!(is_old_scotch (rectangle)))
      rectangle->FLAGS += OLD_SCOTCH;
}


/*****************************************************************************
 * mark_empty: marks the given model at the specified layer as empty
 ****************************************************************************/

void mark_empty (model, layer_num)
     rdsfig_list *model;
     int layer_num;
{
   if (!(is_empty (model, layer_num)))
      model->FLAGTAB[layer_num] += EMPTY;
}


/*****************************************************************************
 * mark_post: marks the given model at the specified layer as post_treated
 ****************************************************************************/

void mark_treat (model)
     rdsfig_list *model;
{
   model->FLAGS = S2R_TREAT;
}


/*******************************************************************************
 * push_free_list: pushes the given pointer to a deleted rectangle into the free
 *                 list.
 ******************************************************************************/

void push_free_list (rectp)
     rdsrec_list *rectp;
{
   rectp->NEXT = Free_List;
   Free_List = rectp;
}


/*******************************************************************************
 * pull_free_list: pulls from the free list - if not empty - a pointer to
 *                 a ( rectangle) memory space area not used anymore.
 ******************************************************************************/

rdsrec_list *pull_free_list ()
{
   rdsrec_list *temp;

   if (Free_List)
   {
      temp = Free_List;
      Free_List = Free_List->NEXT;
      temp->X = 0;
      temp->Y = 0;
      temp->DX = 0;
      temp->DY = 0;
      temp->NAME = NULL;
      temp->USER = (void *) NULL;
      temp->FLAGS = 0;
      return (temp);
   }
   else
      return (NULL);
}


/***************************************************************************
 * rds_remove_rectangle: removes from the specified list at the specified
 *              layer all the rectangles that are marked to be removed.
 *              The removed rectangles are put in a free list, to minimize
 *              memory consumption: if we need to allocate memory for a new
 *              rectangle we check the free list first.
 ***************************************************************************/

void rds_remove_rectangle (list)
     rds_rectanglep_typ *list;
{
   register rds_rectanglep_typ *rectanglep;
   register rdsrec_list *temp;

   for (rectanglep = list; *rectanglep;)
   {
      if (is_remove (*rectanglep))
      {
         temp = (*rectanglep)->NEXT;
         push_free_list ((*rectanglep));
         *rectanglep = temp;
      }
      else
         rectanglep = &((*rectanglep)->NEXT);
   }
}


/***************************************************************************
 * rds_clean_layer: removes from the specified model at the specified layer
 *              all the rectangles that have been flattened and were not used.
 *              The removed rectangles are put in a free list, to minimize
 *              memory consumption: if we need to allocate memory for a new
 *              rectangle we check the free list first.
 ***************************************************************************/

void rds_clean_layer (list)
     rds_rectanglep_typ *list;
{
   register rds_rectanglep_typ *rectanglep;
   register rdsrec_list *temp;

   for (rectanglep = list; *rectanglep;)
   {
      /* when a rectangle is issued from a subcell after a ring flattening
         if is to be erased if it was not used by merge */
      if ((is_flattenres (*rectanglep)) && (!(is_used (*rectanglep))))
      {
      /**** remove it ****/
         temp = (*rectanglep)->NEXT;
         push_free_list ((*rectanglep));
         *rectanglep = temp;
      }
      else
      {
      /**** just erase using flag and flattenres ****/
      /**** the rectangle owns to the current hierararchical level ***/
         demark_flattenres (*rectanglep);
         demark_used (*rectanglep);
         rectanglep = &((*rectanglep)->NEXT);
      }
   }
}


/*****************************************************************************
 * side_intersec: determines if 2 rectangles could intersect in the dimension
 *                of the specified sides.
 *                If yes it returns (1) and puts where and how long is the side
 *                of their intersection in rs and rds respectively.
 *                If not it returns (0).
 *****************************************************************************/

int side_intersec (s1, s2, ds1, ds2, rs, rds)
     long s1, s2, ds1, ds2, *rs, *rds;
{
   if ((s2 >= s1) && (s2 <= (s1 + ds1)))
   {
      if ((s2 + ds2) >= (s1 + ds1))
      {
         *rs = s2;
         *rds = s1 + ds1 - s2;
      }
      else
      {
         *rs = s2;
         *rds = ds2;
      }
      return (1);
   }
   if ((s2 <= s1) && ((s2 + ds2) >= s1))
   {
      if ((s2 + ds2) >= (s1 + ds1))
      {
         *rs = s1;
         *rds = ds1;
      }
      else
      {
         *rs = s1;
         *rds = s2 + ds2 - s1;
      }
      return (1);
   }
   return (0);
}


/*****************************************************************************
 * rect_intersec: determines if 2 rectangles intersect.
 *                If yes it returns (1) and puts the data of the rectangle
 *                of their intersection in rx, ry, rdx and rdy.
 *                If not it returns (0).
 *****************************************************************************/

int rect_intersec (x1, x2, dx1, dx2, y1, y2, dy1, dy2, rx, rdx, ry, rdy)
     long x1, x2, dx1, dx2, y1, y2, dy1, dy2, *rx, *rdx, *ry, *rdy;
{
   if (side_intersec (x1, x2, dx1, dx2, rx, rdx))
      return (side_intersec (y1, y2, dy1, dy2, ry, rdy));
   else
      return (0);
}

static chain_list *MODEL_CHAIN;

static chain_list *rmake_model_list (pt_phfig)
     phfig_list *pt_phfig;

{
   phins_list *pt_ins;
   chain_list *pt_chain;

   for (pt_ins = pt_phfig->PHINS; pt_ins != NULL; pt_ins = pt_ins->NEXT)
   {
      for (pt_chain = MODEL_CHAIN; pt_chain != NULL; pt_chain = pt_chain->NEXT)
      {
         if (((phfig_list *) pt_chain->DATA)->NAME == pt_ins->FIGNAME)
         {
            break;
         }
      }
      if (pt_chain == NULL)
      {
         /* if not already in model list , add a new model to it */
         phfig_list *pt_model;
         pt_model = getphfig (pt_ins->FIGNAME, 'A');
         /* cells whose have got gds equivalent must have been shifted to zero */
         if (incataloggds (pt_model->NAME))
            zero (pt_model);
         (void) rmake_model_list (pt_model);
      }
   }
   MODEL_CHAIN = addchain (MODEL_CHAIN, (char *) pt_phfig);

   return (MODEL_CHAIN);
}

static chain_list *make_model_list (fig)
     phfig_list *fig;
{
   if (MODEL_CHAIN != NULL)
      freechain (MODEL_CHAIN);
   MODEL_CHAIN = NULL;
   return rmake_model_list (fig);
}

/*------------------------------------------------------\
|       Transform a hierarchy Mbk -> Rds                |
\------------------------------------------------------*/

rdsfig_list *S2Rfigmbkrds (FigureMbk, All)
     phfig_list *FigureMbk;
     char All;
{
   rdsfig_list *FigureRds;
   chain_list *Pt;

   if (All)
   {
      Pt = make_model_list (FigureMbk);

      if (s2rdebug)
      {
         chain_list *pc = Pt;
         fprintf (stderr, "MODELS :\n");
         for (; pc; pc = pc->NEXT)
         {
            fprintf (stderr, "  %s\t", ((phfig_list *) (pc->DATA))->NAME);
            if ((((phfig_list *) (pc->DATA))->PHINS) == NULL)
               fprintf (stderr, "FEUILLE");
            fprintf (stderr, "\n");
         }
      }

      FigureRds = figmbkrds ((phfig_list *) Pt->DATA, 0, 0);
      for (Pt = Pt->NEXT; Pt != NULL; Pt = Pt->NEXT)
         figmbkrds ((phfig_list *) Pt->DATA, 0, 0);
   }
   else
   {
      FigureRds = figmbkrds (FigureMbk, 0, 0);
      for (Pt = FigureMbk->MODELCHAIN; Pt != NULL; Pt = Pt->NEXT)
         figmbkrds (getphfig ((char *) Pt->DATA, 'P'), 0, 0);
   }
   return FigureRds;
}

void zero (phfig)
     phfig_list *phfig;
{
   long x, y;
   phins_list *phins;
   phcon_list *phcon;
   phseg_list *phseg;
   phvia_list *phvia;
   phref_list *phref;

   x = phfig->XAB1;
   y = phfig->YAB1;

   phfig->XAB1 = phfig->YAB1 = 0;
   phfig->XAB2 -= x;
   phfig->YAB2 -= y;

   for (phins = phfig->PHINS; phins != NULL; phins = phins->NEXT)
   {
      phins->XINS -= x;
      phins->YINS -= y;
   }

   for (phcon = phfig->PHCON; phcon != NULL; phcon = phcon->NEXT)
   {
      phcon->XCON -= x;
      phcon->YCON -= y;
   }

   for (phseg = phfig->PHSEG; phseg != NULL; phseg = phseg->NEXT)
   {
      phseg->X1 -= x;
      phseg->Y1 -= y;
      phseg->X2 -= x;
      phseg->Y2 -= y;
   }

   for (phvia = phfig->PHVIA; phvia != NULL; phvia = phvia->NEXT)
   {
      phvia->XVIA -= x;
      phvia->YVIA -= y;
   }

   for (phref = phfig->PHREF; phref != NULL; phref = phref->NEXT)
   {
      phref->XREF -= x;
      phref->YREF -= y;
   }
}
