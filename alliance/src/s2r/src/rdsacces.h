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

extern int s2rdebug;

/******************************************************************************
 *                                                                            *
 *    Rectangle Data Base : Structures and Access functions                   *
 *                                                                            *
 *****************************************************************************/

#include "maxima.h"

/******************************************************************************
 * definitions of flags
 *****************************************************************************/

#define is_remove(rectangle)        ( (rectangle)->FLAGS & REMOVE)
#define is_flattenres(rectangle)    ( (rectangle)->FLAGS & FLATTENRES)
#define is_used(rectangle)          ( (rectangle)->FLAGS & USED)
#define is_scotch(rectangle)        ( (rectangle)->FLAGS & SCOTCH)
#define is_old_scotch(rectangle)    ( (rectangle)->FLAGS & OLD_SCOTCH)

#define	REC_INIT	0x00000000  /* rectangle: default */
#define	REMOVE		0x01000000  /* rectangle: to be removed */
#define	FLATTENRES	0x02000000  /* rectangle: resulted from a flatten */
#define	USED		0x04000000  /* rectangle: used in merge */
#define	SCOTCH		0x08000000  /* rectangle: a scotch */
#define	OLD_SCOTCH	0x10000000  /* rectangle: an old scotch */

#define	EMPTY		1           /* model(specific for a layer): 
                                       it contains nothing in this layer */
#define	POST		1           /* model->FLAGS: the model is postrated */
#define	READ		2           /* model->FLAGS: model already read from disk */
                                    /* when a cell contains a model which has been */
                                    /* already read in another cell, this flag */
                                    /* prevents from reading it twice from disk */

/* Rectangles */

typedef rdsrec_list *rds_rectanglep_typ;


/*****************************************************************************
 * Instances :
 * The coordinates and symetry semantics are the gds & cif 's one, that's 
 * completly different from mbk's 
 *****************************************************************************/

/* Models */

typedef rdsfig_list *rds_figurep_typ;

/************************************************************************
* types and definitions of rectangle data base access
*************************************************************************/

/************************************************************************
 * free list of rectangles removed to be 
 * allocated when a creation of a rectangle is requested 
 ************************************************************************/
extern rdsrec_list *Free_List;

/************************************************************************
 * declaration of the functions defined in rds_access.c
 ************************************************************************/

extern rdsfig_list *S2Rfigmbkrds ();
extern void zero ();
extern rdsrec_list *rds_rectangle_in ();
extern rdsins_list *rds_instance_in ();
extern void rds_remove_model ();
extern rdsfig_list *rds_create_model ();
extern rdsfig_list *rds_model_out ();
extern rdsins_list *rds_instance_out ();
extern void rds_mbk_kill ();
extern void mark_treat ();
extern void mark_remove ();
extern void mark_flattenres ();
extern void mark_used ();
extern void mark_scotch ();
extern void mark_old_scotch ();
extern void mark_empty ();
extern void mark_post ();
extern void demark_flattenres ();
extern void demark_scotch ();
extern void demark_used ();
extern void push_free_list ();
extern rdsrec_list *pull_free_list ();
extern void rds_remove_rectangle ();
extern void rds_clean_layer ();
extern int side_intersec ();
extern int rect_intersec ();

/*******************************************************************************
 * macro definitions to test the flags fields of rectangles and models
 ******************************************************************************/

/******************************************************************************
 * rectangles
 *****************************************************************************/

#define is_remove(rectangle)  		( (rectangle)->FLAGS & REMOVE)
#define is_flattenres(rectangle)  	( (rectangle)->FLAGS & FLATTENRES)
#define is_used(rectangle)  		   ( (rectangle)->FLAGS & USED)
#define is_scotch(rectangle)  		( (rectangle)->FLAGS & SCOTCH)
#define is_old_scotch(rectangle)  	( (rectangle)->FLAGS & OLD_SCOTCH)

/*****************************************************************************
 * are_rect_intersec: tests if the 2 given rectangles ( given by their 
 *                    coordinates) intersect or not.
 ******************************************************************************/

#define are_rect_intersec(x1,y1,dx1,dy1,x2,y2,dx2,dy2)     (\
                 (( ((x2) >= (x1)) && ((x2) <= ((x1)+(dx1))))\
            ||\
                 (( (x2) <= (x1)) && ( ((x2)+(dx2)) >= (x1))))\
        &&\
                 (( ((y2) >= (y1)) && ((y2) <= ((y1)+(dy1))))\
            ||\
                 (( (y2) <= (y1)) && ( ((y2)+(dy2)) >= (y1))))\
                                                           )

/****************************************************************************
 * models
 ****************************************************************************/

#define is_empty(model,layer_num)  	( (model)->FLAGTAB[layer_num] & EMPTY)
#define is_post(model)  		( (model)->FLAGS & POST)
#define is_read(model)  		( (model)->FLAGS & READ)
