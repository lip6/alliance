#include <assert.h>
#include <stdlib.h>

/*
   ### -------------------------------------------------- ###
   $Author: hcl $
   $Date: 2002/03/22 16:04:02 $
   $Log: ocrAstar.c,v $
   Revision 1.3  2002/03/22 16:04:02  hcl
   speedups

   Revision 1.2  2002/03/20 13:25:50  hcl
   SymX bug.

   Revision 1.1  2002/03/15 14:37:19  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:16  hcl
   First commit in the new tree.

   Revision 1.5  2002/02/25 11:41:13  hcl
   A la chasse au bug bug bug

   Revision 1.4  2002/02/22 15:20:49  hcl
   A* is ready

   Revision 1.3  2002/02/21 13:17:57  hcl
   Introducing a new algo (A*, dont yet work...)

   Revision 1.2  2002/02/15 12:06:31  hcl
   First attempt

   Revision 1.1  2002/02/12 15:14:14  hcl
   New algo.

   ### -------------------------------------------------- ###
*/

#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"
#include "ocr.h"
#include "ocrUtil.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "mbk_tree.h"
#include "ocrWRoutingUtil.h"
#include "ocrWRouting.h"
#include "display.h"
#include "ocrAstar.h"

extern ocrOption *g_pOption;
#define LEVEL (g_pOption->LEVEL)

/*********************************************************************/
#define TAG_UNDEF            0
#define TAG_KEEP             1
#define TAG_CLEAN            2
#define TAG_FREED            4
#define TAG_TERRA            8
#define TAG_VISITED         16


/*********************************************************************/

/**
 * Variables globales
 **/
ocrWRoutingGrid *grid = NULL;
ocrRoutingParameters *param = NULL;

ocrNaturalInt xs = 0, ys = 0, zs = 0;
ocrNaturalInt CUR_SIG_INDEX = 0;

//ocrNaturalInt (*kost)(ocrWSegment *, ocrWSegment *);

/*********************************************************************/

/**
 *
 * Local data structures
 *
 **/


/*********************************************************************/

/**
 * Functions
 */

/* Cost function (Manhattan distance) */
ocrNaturalInt eval (ocrWSegment *segment_source, ocrWSegment *segment_dest) {

    ocrNaturalInt x1s, x2s, y1s, y2s, x1d, x2d, y1d, y2d, dx, dy, dz;

    x1s = getWSegXCoord (param, segment_source, segment_source->P_MIN);
    y1s = getWSegYCoord (param, segment_source, segment_source->P_MIN);
    x2s = getWSegXCoord (param, segment_source, segment_source->P_MAX);
    y2s = getWSegYCoord (param, segment_source, segment_source->P_MAX);

    x1d = getWSegXCoord (param, segment_dest, segment_dest->P_MIN);
    y1d = getWSegYCoord (param, segment_dest, segment_dest->P_MIN);
    x2d = getWSegXCoord (param, segment_dest, segment_dest->P_MAX);
    y2d = getWSegYCoord (param, segment_dest, segment_dest->P_MAX);

    dx = ( (x2s < x1d)? (x1d - x2s) : ( (x1s > x2d) ? (x1s - x2d) : 0 ) );
    dy = ( (y2s < y1d)? (y1d - y2s) : ( (y1s > y2d) ? (y1s - y2d) : 0 ) );

    dz = (
            (segment_dest->LAYER > segment_source->LAYER) ?
            segment_dest->LAYER - segment_source->LAYER :
            segment_source->LAYER - segment_dest->LAYER
         )
        ;

    return (dx + dy) + dz /* param->VIA_COST*/;
}

/* Distance between segment and nearest segment of an equi */
ocrNaturalInt eval_equi (ocrWSegment *segment_source, ocrWSegment *equi) {
    ocrNaturalInt res = OCRNATURALINT_MAX;
    ocrNaturalInt aux;

    /*return 0;*/
    while (equi) {
        aux = eval (segment_source, equi);
        if (aux < res) {
            res = aux;
        }
        equi = equi->NEXT;
    }
    return res;
}

/* kost of the path from segment_source to segment_dest */
ocrNaturalInt kost (ocrWSegment *segment_source, ocrWSegment *segment_dest) {

    ocrWSegment *ps;
    ocrNaturalInt dest_offset, p;
    ocrNaturalInt res;

    ps = segment_source->ROOT;
    dest_offset = segment_dest->OFFSET;

    if (
            getWSegDirection (param, segment_source)
            ==
            getWSegDirection (param, segment_dest)
       )
    {
        return 0;
    } else if (ps) {
        res = (
               (ps->OFFSET > dest_offset) ?
               (ps->OFFSET - dest_offset) :
               (dest_offset - ps->OFFSET)
              )
            ;
        res += (
                (segment_dest->LAYER > segment_source->LAYER) ?
                100 + segment_dest->LAYER - segment_source->LAYER :
                segment_source->LAYER - segment_dest->LAYER
               )
            /* param->VIA_COST*/
            ;

        return res;
    } else {
        p = (getWSegDirection (param, segment_source) == ocrHorizontal) ?
            xs : ys;

        return
           (
            (p > dest_offset) ?
            (p - dest_offset) :
            (dest_offset - p)
           )
           +
           (
            (zs > segment_source->LAYER) ?
            (zs - segment_source->LAYER) :
            (segment_source->LAYER - zs)
           )
           /*
           param->VIA_COST*/
           ;
    }
}

void tag_keep (ocrWSegment *path) {

    while (path) {
        path->TAG = TAG_KEEP;
        path = path->ROOT;
    }
    return;
}

/**
 * stack
 **/

ocrWSegment *pop (ocrWSegment **list) {
    ocrWSegment *res;

    assert (*list);

    res = (*list);
    (*list) = ( (*list)->AUX );
    res->AUX = NULL;

    return res;
}

void push (ocrWSegment **list, ocrWSegment *elem) {

    assert (elem);

    elem->AUX = (*list);
    (*list) = elem;

    return;
}

/**
 * clean_mem
 **/
void clean_tag () {
    ocrWSegment *seg;
    int i, j, k;

    for (k = 0 ; k < grid->NB_OF_LAYERS ; k += 2)
        for (j = 0 ; j < grid->SIZE_V ; j++)
            for (i = 0 ; i < grid->SIZE_H ; i++) {
                seg = getWSegment (grid, i, j, k);
                seg->TAG = TAG_UNDEF;
                i = seg->P_MAX;
#if 0
                if (
                        (seg->SIGNAL_INDEX != WSEGMENT_FREE)
                        &&
                        (seg->SIGNAL_INDEX != WSEGMENT_OBSTACLE)
                   )
                {
                    if (seg->AUX) {
                        display (LEVEL, DEBUG, "AUX not NULL for free or obstacle segment\n");
                        seg->AUX = NULL;
                    }
                }
#endif
        }

    for (k = 1 ; k < grid->NB_OF_LAYERS ; k += 2)
        for (i = 0 ; i < grid->SIZE_H ; i++)
            for (j = 0 ; j < grid->SIZE_V ; j++) {
                seg = getWSegment (grid, i, j, k);
                seg->TAG = TAG_UNDEF;
                j = seg->P_MAX;
#if 0
                if (
                        (seg->SIGNAL_INDEX != WSEGMENT_FREE)
                        &&
                        (seg->SIGNAL_INDEX != WSEGMENT_OBSTACLE)
                   )
                {
                    if (seg->AUX) {
                        display (LEVEL, DEBUG, "AUX not NULL for free or obstacle segment\n");
                        seg->AUX = NULL;
                    }
                }
#endif
            }

    return;
}

void clean_tag_list (ocrWSegment *list) {

    while (list) {
        list->TAG = TAG_UNDEF;
        list = list->AUX;
    }
    return;
}

/**
 * insert elem keeping the list sorted by increasing cost+h
 **/
ocrWSegment *insert (ocrWSegment *list, ocrWSegment *elem) {
    ocrWSegment *prec, *rval;

    assert (elem);

    if (!list) {
        elem->AUX = NULL;
        return elem;
    } else if (!(list->AUX)) {
        if ( elem->HCOST < list->HCOST ) {
            elem->AUX = list;
            return elem;
        } else if ( elem->HCOST == list->HCOST ) {
            if ( elem->COST < list->COST ) {
                elem->AUX = list;
                return elem;
            } else {
                list->AUX = elem;
                elem->AUX = NULL;
                return list;
            }
        } else {
            list->AUX = elem;
            elem->AUX = NULL;
            return list;
        }
    } else {
        rval = list;
        prec = list;
        list = list->AUX;

        while (list) {
            
            if ( elem->HCOST < list->HCOST ) {
                elem->AUX = list;
                prec->AUX = elem;
                return rval;
            } else if ( elem->HCOST == list->HCOST ) {
                if ( elem->COST < list->COST ) {
                    prec->AUX = elem;
                    elem->AUX = list;
                    return rval;
                } else {
                    prec = list;
                    list = list->AUX;
                }
            } else {
                prec = list;
                list = list->AUX;
            }
        }
        prec->AUX = elem;
        elem->AUX = NULL;
        return rval;
    }

    return NULL;
}


ocrWSegment *_remove (ocrWSegment *list, ocrWSegment *elem) {
    ocrWSegment *prec;
    ocrWSegment *orig;
    
    assert (elem);

    if (!list)
        return list;
    if (elem == list) {
        ocrWSegment *res = elem->AUX;
        elem->AUX = NULL;
        return res;
    }

    orig = list;
    do {
        prec = list;
        list = list->AUX;
        if (elem == list) {
            prec->AUX = list->AUX;
            elem->AUX = NULL;
            return orig;
        }
    } while (list);
            
#if 0
    list->AUX = _remove (list->AUX, elem);
    return list;
#endif
    return orig;
}


#define XPLORE \
{\
    if (isObstructed (seg))\
        if (seg->SIGNAL_INDEX != CUR_SIG_INDEX)\
            continue;\
\
    cost = node->COST + kost (node, seg); /* XXX cout du chemin parcouru ??? */\
\
    switch (seg->TAG) {\
        case TAG_UNDEF: /* not yet seen segment */\
            seg->ROOT = node;\
            seg->COST = cost;\
            seg->H = eval_equi (seg, segment_target);\
            seg->HCOST = seg->H + cost;\
            seg->TAG = TAG_TERRA;\
            terra_incognita = insert (terra_incognita, seg);\
            break;\
        case TAG_TERRA:\
            if (cost < seg->COST) {\
                seg->COST = cost;\
                seg->HCOST = seg->H + cost;\
                seg->ROOT = node;\
                terra_incognita = _remove (terra_incognita, seg);\
                terra_incognita = insert (terra_incognita, seg);\
            } else {\
                /* rien */\
            }\
            break;\
        case TAG_VISITED:\
            if (cost < seg->COST) {\
                seg->COST = cost;\
                seg->HCOST = seg->H + cost;\
                seg->ROOT = node;\
                visited = _remove (visited, seg);\
                seg->TAG = TAG_TERRA;\
                terra_incognita = insert (terra_incognita, seg);\
            } else {\
                /* rien */\
            }\
            break;\
        default:\
            display (LEVEL, ERROR, "*** unexpected TAG !!!\n");\
            abort ();\
    }\
}


ocrWSegment *explore (ocrWSegment *segment_source, ocrWSegment *segment_target) {

    ocrWSegment *visited = NULL;
    ocrWSegment *terra_incognita = NULL;
    ocrNaturalInt p, cost;
    int i;

    assert (segment_source != segment_target);

    display (LEVEL, DEBUG, "d starting exploration\n");

    terra_incognita = segment_source;
    terra_incognita->COST = 0;
    terra_incognita->H = eval (segment_source, segment_target);
    terra_incognita->HCOST = terra_incognita->H;
    terra_incognita->TAG = TAG_TERRA;
    terra_incognita->ROOT = NULL;
    terra_incognita->AUX = NULL;

    while (terra_incognita) {
        ocrWSegment *node = NULL;
        ocrWSegment *seg = NULL;

        node = pop (&terra_incognita);

        if (node == segment_target) {
            /* bingo ! */
            clean_tag_list (visited);
            clean_tag_list (terra_incognita);
            node->TAG = TAG_UNDEF;
            return node;
        }

        node->TAG = TAG_VISITED;
        push (&visited, node);

        /* expansion */
        for (i = -1 ; i <= 1 ; i += 2) {
            switch (i) {

                case -1: if ( ((node->LAYER) ) == 0 )
                            continue;
                        break;
                case  1: if ( ((node->LAYER) + 1) >= (grid->NB_OF_LAYERS) )
                            continue;
                        break;

                default:
                    /*display (LEVEL, ERROR, __FILE__ ":" __LINE__ "exand_eval Oops\n");*/
                    abort ();
            }
            for (p = node->P_MIN ; p <= node->P_MAX ; p++) {

                /*display (LEVEL, DEBUG, "d expanding %ld\n", i);*/
                seg = getWSegment(grid,
                                  getWSegXCoord (param, node, p),
                                  getWSegYCoord (param, node, p),
                                  node->LAYER + i
                                 );

#ifdef DEBUG
                if (!seg) {
                    display (LEVEL, DEBUG, "oddity: null seg at (%ld, %ld, %ld)\n", getWSegXCoord(param, node, p), getWSegXCoord(param, node, p), node->LAYER + i);
                    continue;
                }
#endif

                if (isObstructed (seg))
                    continue;

                cost = node->COST + kost (node, seg); /* XXX cout du chemin parcouru ??? */

                switch (seg->TAG) {
                    case TAG_UNDEF: /* not yet seen segment */
                        seg->ROOT = node;
                        seg->COST = cost;
                        seg->H = eval (seg, segment_target);
                        seg->HCOST = seg->H + cost;
                        seg->TAG = TAG_TERRA;
                        terra_incognita = insert (terra_incognita, seg);
                        break;
                    case TAG_TERRA:
                        if (cost < seg->COST) {
                            seg->COST = cost;
                            seg->HCOST = seg->H + cost;
                            seg->ROOT = node;
                            terra_incognita = _remove (terra_incognita, seg);
                            terra_incognita = insert (terra_incognita, seg);
                        } else {
                            /* rien */
                        }
                        break;
                    case TAG_VISITED:
                        if (cost < seg->COST) {
                            seg->COST = cost;
                            seg->HCOST = seg->H + cost;
                            seg->ROOT = node;
                            visited = _remove (visited, seg);
                            seg->TAG = TAG_TERRA;
                            terra_incognita = insert (terra_incognita, seg);
                        } else {
                            /* rien */
                        }
                        break;
                    default:
                        display (LEVEL, ERROR, "*** unexpected TAG !!!\n");
                        abort ();
                }
            }
        } /* end for i */
    } /* while */

    /* Failed */
    clean_tag ();

    /*isplay (LEVEL, DEBUG, "d exploration failed\n");*/


    return NULL;
}

#if 0
void addseglist (ocrSignal *s, ocrWSegment *l2) {
    ocrWSegment *l1;

    if (!(s->SEGMENT))
        s->SEGMENT = l2;
    else {
        l1 = s->SEGMENT;
        while (l1->NEXT)
            l1 = l1->NEXT;
        l1->NEXT = l2;
    }
    return;
}
#endif

ocrWSegment *explore_equi (ocrWSegment *segment_source, ocrWSegment *segment_target) {

    ocrWSegment *visited = NULL;
    ocrWSegment *terra_incognita = NULL;
    ocrNaturalInt p, cost;
    int i;

    assert (segment_source != segment_target);

    display (LEVEL, DEBUG, "d starting exploration\n");

    terra_incognita = segment_source;
    terra_incognita->COST = 0;
    terra_incognita->H = eval_equi (segment_source, segment_target);
    terra_incognita->HCOST = terra_incognita->H;
    terra_incognita->TAG = TAG_TERRA;
    terra_incognita->ROOT = NULL;
    terra_incognita->AUX = NULL;

    while (terra_incognita) {
        ocrWSegment *node = NULL;
        ocrWSegment *seg = NULL;

        node = pop (&terra_incognita);

        if (node->SIGNAL_INDEX == CUR_SIG_INDEX) {
            /* on a gagne */
            clean_tag_list (visited);
            clean_tag_list (terra_incognita);
            node->TAG = TAG_UNDEF;
            return node;
        }

        node->TAG = TAG_VISITED;
        push (&visited, node);

        /* expansion */
        if (getWSegDirection (param, node) == ocrHorizontal) {
            if ( (node->P_MIN) > 0 ) {
                seg = getWSegment (grid, node->P_MIN - 1, node->OFFSET, node->LAYER);
                if (seg->SIGNAL_INDEX == CUR_SIG_INDEX)
                    XPLORE
            }
            if ( (node->P_MAX) < (grid->SIZE_H - 2) ) {
                seg = getWSegment (grid, node->P_MAX + 1, node->OFFSET, node->LAYER);
                if (seg->SIGNAL_INDEX == CUR_SIG_INDEX)
                    XPLORE
            }
        } else { /* vertical */
            if ( (node->P_MIN) > 0 ) {
                seg = getWSegment (grid, node->OFFSET, node->P_MIN - 1, node->LAYER);
                if (seg->SIGNAL_INDEX == CUR_SIG_INDEX)
                    XPLORE
            }
            if ( (node->P_MAX) < (grid->SIZE_V - 2) ) {
                seg = getWSegment (grid, node->OFFSET, node->P_MAX + 1, node->LAYER);
                if (seg->SIGNAL_INDEX == CUR_SIG_INDEX)
                    XPLORE
            }
        }
            
        for (i = -1 ; i <= 1 ; i += 2) {
            switch (i) {

                case -1: if ( ((node->LAYER) ) == 0 )
                            continue;
                        break;
                case  1: if ( ((node->LAYER) + 1) >= (grid->NB_OF_LAYERS) )
                            continue;
                        break;

                default:
                    /*display (LEVEL, ERROR, __FILE__ ":" __LINE__ "exand_eval Oops\n");*/
                    abort ();
            }
            for (p = node->P_MIN ; p <= node->P_MAX ; p++) {

                /*display (LEVEL, DEBUG, "d expanding %ld\n", i);*/
                seg = getWSegment(grid,
                                  getWSegXCoord (param, node, p),
                                  getWSegYCoord (param, node, p),
                                  node->LAYER + i
                                 );

#ifdef DEBUG
                if (!seg) {
                    display (LEVEL, DEBUG, "oddity: null seg at (%ld, %ld, %ld)\n", getWSegXCoord(param, node, p), getWSegXCoord(param, node, p), node->LAYER + i);
                    continue;
                }
#endif
                XPLORE
            }
        } /* end for i */
    } /* while */

    /* Failed */
    clean_tag ();

    /*isplay (LEVEL, DEBUG, "d exploration failed\n");*/


    return NULL;
}



ocrNaturalInt make_segments (ocrWSegment *segment_dest,
                    ocrWSegment *segment_source,
                    ocrNaturalInt xdest,
                    ocrNaturalInt ydest,
                    ocrNaturalInt zdest,
                    ocrNaturalInt signal_index,
                    ocrSignal * i_pSignal
                   )
{
    ocrWSegment *seg, *nseg, *root, *aux;
    ocrNaturalInt xp, yp, xn, yn;
    ocrNaturalInt p1, p2, pp1, pp2;
    /*ocrNaturalInt pi;*/
    ocrNaturalInt distance = 0;
    ocrNaturalInt nb_segs = 0;

    /* initialisation */

    aux = i_pSignal->SEGMENT;

    seg = segment_dest;
    
    display (LEVEL, DEBUG, "c dest (%ld, %ld, %ld)\n", xdest, ydest, zdest);
    xp = xdest;
    yp = ydest;

    if (seg->SIGNAL_INDEX == signal_index) {
        display (LEVEL, DEBUG, "c joining equi at seg (off=%ld, p_min=%ld, p_max=%ld)\n", seg->OFFSET, seg->P_MIN, seg->P_MAX);

        if (
                getWSegDirection (param, seg)
                ==
                getWSegDirection (param, seg->ROOT)
           )
        {
            xp = getWSegXCoord (param, seg, seg->P_MIN);
            yp = getWSegYCoord (param, seg, seg->P_MIN);
        } else {
            xp = getWSegXCoord (param, seg, seg->ROOT->OFFSET);
            yp = getWSegYCoord (param, seg, seg->ROOT->OFFSET);
        }
        nseg = seg->ROOT;
        seg->ROOT = NULL;
        seg = nseg;
    }
    nseg = NULL;

    /*

       seg, seg->ROOT
       nseg <= split (seg)

       nseg -> NEXT = prevseg;
       prevseg = nseg;

    */

    while (seg->ROOT) {
        root = seg->ROOT;
        xn = getWSegXCoord (param, root, xp);
        yn = getWSegYCoord (param, root, yp);
        display (LEVEL, DEBUG, "p (%ld, %ld, %ld) -> (%ld, %ld, %ld) : d=%ld, c=%ld, h=%ld, hc=%ld\n", xp, yp, seg->LAYER, xn, yn, seg->LAYER, distance, seg->COST, seg->H, seg->HCOST);
        ((getWSegDirection(param, seg) == ocrHorizontal) ? (p1 = xp, p2 = xn) : (p1 = yp, p2 = yn) );
        ( (p1 < p2) ? (pp1 = p1, pp2 = p2) : (pp1 = p2, pp2 = p1) );
        nseg = splitWSegment (param, grid, seg,
                              pp1, pp2,
                              signal_index
                             );
        nseg->NEXT = aux;
        aux = nseg;
        distance += (pp2 - pp1);
        nb_segs ++;
        xp = xn;
        yp = yn;
        /*seg->SIGNAL_INDEX = signal_index;*/
        seg = root;
    }

    /*assert (nseg == segment_source);*/
    xn = xs;
    yn = ys;
    /*root = seg->ROOT;*/
    ((getWSegDirection(param, seg) == ocrHorizontal) ? (p1 = xp, p2 = xn) : (p1 = yp, p2 = yn) );
    ( (p1 < p2) ? (pp1 = p1, pp2 = p2) : (pp1 = p2, pp2 = p1) );
    nseg = splitWSegment (param, grid, seg,
                          pp1, pp2,
                          signal_index
                         );
    nseg->NEXT = aux;
    i_pSignal->SEGMENT = nseg;

    distance += (pp2 - pp1);
    nb_segs ++;
    display (LEVEL, DEBUG, "p (%ld, %ld, %ld) -> (%ld, %ld, %ld) : %ld\n", xp, yp, seg->LAYER, xn, yn, zs, distance);
    display (LEVEL, DEBUG, "c source (%ld, %ld, %ld)\n", xs, ys, zs);
    display (LEVEL, DEBUG, "p num of segments : %ld\n", nb_segs);
    /*seg->SIGNAL_INDEX = signal_index;*/
        



    display (LEVEL, DEBUG, "i path length : %ld\n", distance);

    return distance * 5;
}



ocrNaturalInt check_path (ocrSignal *i_pSignal) {

    ocrWSegment *seg;
    ocrNaturalInt dist;

    seg = i_pSignal->SEGMENT;

    dist = 0;

    while (seg) {
        if ( (seg->SIGNAL_INDEX) != (i_pSignal->INDEX) ) {
            display (LEVEL, ERROR, " ---INVALID Segment->SIGNAL_INDEX\n");
            abort ();
        }

        display (LEVEL, DEBUG, "(%ld, %ld, %ld, %ld)\n", seg->OFFSET, seg->P_MIN, seg->P_MAX, seg->LAYER);

        dist += seg->P_MAX - seg->P_MIN;

        seg = seg->NEXT;
    }

    display (LEVEL, DEBUG, "DIST CHECK --- %ld\n", dist);

    return dist;
}





ocrNaturalInt find_path_astar (ocrRoutingParameters * p_param,
                      ocrWRoutingGrid * p_grid,
                      ocrNaturalInt xsource,
                      ocrNaturalInt ysource,
                      ocrNaturalInt zsource,
                      ocrNaturalInt xtarget,
                      ocrNaturalInt ytarget,
                      ocrNaturalInt ztarget,
                      ocrNaturalInt signal_index,
                      ocrSignal * i_pSignal,
                      ocrNaturalInt mode)
{
    ocrWSegment *path = NULL;
    ocrWSegment *segment_source = NULL;
    ocrWSegment *segment_dest = NULL;
    ocrNaturalInt dist, check_dist;

    grid = p_grid;
    param = p_param;

    xs = xsource;
    ys = ysource;
    zs = zsource;

    CUR_SIG_INDEX = signal_index;
    /*clean_tag ();*/

    /*kost = (
            (mode == AS_K_EQUI) ?
            &eval :
            &eval_equi
           )
        ;*/
            


    segment_source = getWSegment (grid,
                                  xsource,
                                  ysource,
                                  zsource
                                 );
    /* display (LEVEL, DEBUG, "d source : %p\n", segment_source); */

    switch (mode) {
        case AS_K_SEG: 
            segment_dest = getWSegment (grid,
                                        xtarget,
                                        ytarget,
                                        ztarget
                                       );
            break;
        case AS_K_EQUI:
            segment_dest = i_pSignal->SEGMENT;
            break;
    }

    /*display (LEVEL, DEBUG, "d dest : %p\n", segment_dest);*/

    if ((segment_source != segment_dest) && (!isObstructed(segment_source))) {
        switch (mode) {
            case AS_K_SEG: path = explore (segment_source, segment_dest);
                           break;
            case AS_K_EQUI: assert (i_pSignal->SEGMENT);
                            path = explore_equi (segment_source, segment_dest);
                            break;
        }
    } else {
        path = segment_source;
        path->ROOT = NULL;
        path->NEXT = NULL;
        /*display (LEVEL, DEBUG, "o direct link\n");*/
    }

    if (!path) {
        display (LEVEL, DEBUG, "o A* failed\n");
        return OCRNATURALINT_MAX;
    }

    display (LEVEL, DEBUG, "o A* succeeded\n");

    /*clean_tag ();*/

    dist = make_segments (
                          path,
                          segment_source,
                          xtarget,
                          ytarget,
                          ztarget,
                          signal_index,
                          i_pSignal
                         );


    /*assert (((check_dist = check_path (i_pSignal)), dist == check_dist));*/

    return dist;
}


/**
 *
 **/
/*void init_Astar (ocrRoutingDataBase *db) {
    assert (db);

    grid = db->GRID;
    param = db->PARAM;

    return;    
}
*/
