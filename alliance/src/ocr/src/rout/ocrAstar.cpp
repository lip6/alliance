/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/04/25 13:41:32 $

   $Log: ocrAstar.cpp,v $
   Revision 1.3  2002/04/25 13:41:32  hcl
   New ripup/reroute loop, bug-kill (CALU&TALU).


   ### -------------------------------------------------- ### 
*/

extern "C" {
    #include <assert.h>
    #include <stdlib.h>
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
    #include "ocrNpoints.h"
    #include "ocrConnectorUtil.h"
    #include "display.h"
    #include "ocrAstar.h"
}

#include <set>
#include <list>

ocrNaturalInt reRoute (
        ocrRoutingDataBase * i_pDataBase,
        ocrRoutingParameters * i_pParam,
        ocrSignal * i_pSignal
        )
;

using namespace std;

extern ocrOption *g_pOption;
#define LEVEL (g_pOption->LEVEL)

/*********************************************************************/
struct is_a_better_seg {
    bool operator() (ocrWSegment *s1, ocrWSegment *s2) { 

        return (
                (s1->HCOST < s2->HCOST)
                ||
                (
                 (s1->HCOST == s2->HCOST)
                 &&
                 (
                  (s1->COST < s2->COST)
                  ||
                  (
                   (s1->COST == s2->COST)
                   &&
                   (s1 < s2)
                  )
                 )
                )
               )
            ;
    }
};

typedef set<ocrWSegment*, is_a_better_seg> ocrSegmentKeySet;
typedef set<ocrWSegment *> ocrSegmentAdrSet;
/*********************************************************************/
#define FINDNPATH findPathNPoints

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
ocrRoutingDataBase *ocr_db = NULL;

ocrWRoutingGrid *grid = NULL;
ocrRoutingParameters *param = NULL;

ocrNaturalInt xs = 0, ys = 0, zs = 0;
ocrNaturalInt CUR_SIG_INDEX = 0;

ocrWSegment *ze_best = NULL;
ocrWSegment *ze_target = NULL;

ocrNaturalInt ripup = 1;

//ocrNaturalInt (*kost)(ocrWSegment *, ocrWSegment *);

/*********************************************************************/

/**
 *
 * Local data structures
 *
 **/

typedef struct s_node_cost {
    ocrNaturalInt H;
    ocrNaturalInt C;
    ocrNaturalInt HC;
} node_cost;


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
            segment_source->LAYER
            ==
            segment_dest->LAYER
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
                segment_dest->LAYER - segment_source->LAYER :
                segment_source->LAYER - segment_dest->LAYER
               )
            /* param->VIA_COST*/
            ;

        return res;
    } else {
#if 1
        p = (getWSegDirection (param, segment_source) == ocrHorizontal) ?
            xs : ys;
#else
        /* perf optim... */
        p = ( (segment_source->LAYER & ((ocrNaturalInt) 1)) ) ?
            xs : ys
            ;
#endif

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

#if 0
bool is_a_better_seg (ocrWSegment *s1, ocrWSegment *s2) {

    return (
            (s1->HCOST < s2->HCOST)
            ||
            (
             (s1->HCOST == s2->HCOST)
             &&
             (s1->COST < s2->COST)
            )
           )
        ;
}
#endif

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

void clean_tag_list (ocrSegmentAdrSet &set) {

    for (ocrSegmentAdrSet::iterator sit = set.begin();
            sit != set.end();
            sit++)
    {
        (*sit)->TAG = TAG_UNDEF;
    }

    set.clear();
    return;
}

void clean_tag_set (ocrSegmentKeySet &set) {

    for (ocrSegmentKeySet::iterator sit = set.begin();
            sit != set.end();
            sit++)
    {
        (*sit)->TAG = TAG_UNDEF;
    }

    set.clear();
    return;
}

/**
 * insert elem keeping the list sorted by increasing cost+h
 **/


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
            terra_incognita.insert (seg);\
            break;\
        case TAG_TERRA:\
            if (cost < seg->COST) {\
                terra_incognita.erase (seg);\
                seg->COST = cost;\
                seg->HCOST = seg->H + cost;\
                seg->ROOT = node;\
                terra_incognita.insert (seg);\
            } else {\
                /* rien */\
            }\
            break;\
        case TAG_VISITED:\
            if (cost < seg->COST) {\
                visited.erase (seg);\
                seg->COST = cost;\
                seg->HCOST = seg->H + cost;\
                seg->ROOT = node;\
                seg->TAG = TAG_TERRA;\
                terra_incognita.insert (seg);\
            } else {\
                /* rien */\
            }\
            break;\
        default:\
            display (LEVEL, ERROR, "*** unexpected TAG !!!\n");\
            abort ();\
    }\
}

ocrWSegment *explore_equi (ocrWSegment *segment_source, ocrWSegment *segment_target) {

    ocrNaturalInt cost;

    ocrSegmentKeySet terra_incognita;
    ocrSegmentAdrSet visited;
    
    assert (segment_source != segment_target);
    /*display (LEVEL, DEBUG, "d starting exploration\n");*/
debut:

    segment_source->COST = 0;
    segment_source->H = eval_equi (segment_source, segment_target);
    segment_source->HCOST = segment_source->H + 1000000;
    segment_source->TAG = TAG_TERRA;
    segment_source->ROOT = NULL;
    //segment_source->AUX = NULL;
    ze_best = segment_source;

    terra_incognita.insert(segment_source);

    
    while (!(terra_incognita.empty())) {

        ocrWSegment *node = NULL;
        ocrWSegment *seg = NULL;

        node = *terra_incognita.begin();

        terra_incognita.erase(node);
        
        if (node->SIGNAL_INDEX == CUR_SIG_INDEX) {
            /* on a gagne */

            clean_tag_list (visited);
            clean_tag_set (terra_incognita);
            node->TAG = TAG_UNDEF;
            return node;
        }

        node->TAG = TAG_VISITED;
        visited.insert(node);

        /* enregistrement */
        if (node->H < ze_best->H)
            ze_best = node;

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
            
        for (int i = -1 ; i <= 1 ; i += 2) {
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
            for (ocrNaturalInt p = node->P_MIN ; p <= node->P_MAX ; p++) {

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
    
    /* dig the path to the target */
    clean_tag ();

    if (dig_around ()) {
        return NULL;
    }
    clean_tag ();
    goto debut;
    /*display (LEVEL, DEBUG, "d exploration failed\n");*/


    return NULL;
}

ocrSignal *findSignal(ocrNaturalInt i_uIndex)
{
    ocrNaturalInt i;

    if (i_uIndex == OCRNATURALINT_MAX)
        return NULL;

    for (i = 0; i < ocr_db->NB_GSIGNAL - 1; i++) {
        if (ocr_db->GSIGNAL[i]->INDEX == i_uIndex)
            return ocr_db->GSIGNAL[i];
    }
    return NULL;
}



#define INTERROGE if (                                                         \
                      (suspect->SIGNAL_INDEX != CUR_SIG_INDEX)                 \
                      &&                                                       \
                      (suspect->SIGNAL_INDEX != WSEGMENT_OBSTACLE)             \
                      &&                                                       \
                      (suspect->SIGNAL_INDEX != WSEGMENT_FREE)                 \
                      &&                                                       \
                      (suspect->TAG != TAG_TERRA)                              \
                     )                                                         \
                  {                                                            \
                      if (((findSignal (suspect->SIGNAL_INDEX))->HARD) < 2)    \
                      {                                                        \
                          suspect->TAG = TAG_TERRA;                            \
                          suspect->COST = ze_best->COST + kost (ze_best, suspect); \
                          suspect->H = eval_equi (suspect, ze_target);         \
                          suspects.insert (suspect);                           \
                      }                                                        \
                  }

ocrNaturalInt dig_around () {
    ocrWSegment *suspect;
    ocrSignal *victime;

    ocrSegmentKeySet suspects;

        /* expansion */
    if (getWSegDirection (param, ze_best) == ocrHorizontal) {
        if ( (ze_best->P_MIN) > 0 ) {
            suspect = getWSegment (grid, ze_best->P_MIN - 1, ze_best->OFFSET, ze_best->LAYER);
            INTERROGE
        }
        if ( (ze_best->P_MAX) < (grid->SIZE_H - 2) ) {
            suspect = getWSegment (grid, ze_best->P_MAX + 1, ze_best->OFFSET, ze_best->LAYER);
            INTERROGE
        }
    } else { /* vertical */
        if ( (ze_best->P_MIN) > 0 ) {
            suspect = getWSegment (grid, ze_best->OFFSET, ze_best->P_MIN - 1, ze_best->LAYER);
            INTERROGE
        }
        if ( (ze_best->P_MAX) < (grid->SIZE_V - 2) ) {
            suspect = getWSegment (grid, ze_best->OFFSET, ze_best->P_MAX + 1, ze_best->LAYER);
            INTERROGE
        }
    }
        
    for (int i = -1 ; i <= 1 ; i += 2) {
        switch (i) {

            case -1: if ( ((ze_best->LAYER) ) == 0 )
                        continue;
                    break;
            case  1: if ( ((ze_best->LAYER) + 1) >= (grid->NB_OF_LAYERS) )
                        continue;
                    break;

            default:
                /*display (LEVEL, ERROR, __FILE__ ":" __LINE__ "exand_eval Oops\n");*/
                abort ();
        }
        for (ocrNaturalInt p = ze_best->P_MIN ; p <= ze_best->P_MAX ; p++) {

            /*display (LEVEL, DEBUG, "d expanding %ld\n", i);*/
            suspect = getWSegment(grid,
                              getWSegXCoord (param, ze_best, p),
                              getWSegYCoord (param, ze_best, p),
                              ze_best->LAYER + i
                             );
            INTERROGE
        }
    } /* end for i */

    if (suspects.empty()) {
        return 1;
        abort ();
    }

    do {
        victime = findSignal((*suspects.begin())->SIGNAL_INDEX);
        suspects.erase (suspects.begin());

        if (victime) {
            deleteEquipotentielle(param, grid, victime);
            ocr_db->RIPUP = addchain (ocr_db->RIPUP, (void *) victime);
            deleteEquipotentielle(param, grid, victime);
            victime->SEGMENT = NULL;
            victime->ROUTED = 0;
            ocr_db->NB_ROUTED--;
            unMarkSegmentAsFree(ocr_db, victime, victime->INDEX);
            display (LEVEL, VVERB, "\no rip up signal %s", victime->NAME);
            countFreeVC (ocr_db);
            goto fin;
        }

        /*
            detruire equipotentielle victime
            relancer la recherche de chemin du signal a router ---> si nouvel echec ?
            relancer le routage du signal occi ----> ???
        */
    } while ( !(suspects.empty()) );

fin:

    clean_tag_set (suspects);

    return 0;
}



/*********************************************************************/
/* stocker le chemin dans la structure                               */

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
    ocrNaturalInt distance = 0;
    ocrNaturalInt nb_segs = 0;

    /* initialisation */

    aux = i_pSignal->SEGMENT;

    seg = segment_dest;
    
    /*display (LEVEL, DEBUG, "c dest (%ld, %ld, %ld)\n", xdest, ydest, zdest);*/
    xp = xdest;
    yp = ydest;

    if (seg->SIGNAL_INDEX == signal_index) {
        /*display (LEVEL, DEBUG, "c joining equi at seg (off=%ld, p_min=%ld, p_max=%ld)\n", seg->OFFSET, seg->P_MIN, seg->P_MAX);*/

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
        /*display (LEVEL, DEBUG, "p (%ld, %ld, %ld) -> (%ld, %ld, %ld) : d=%ld, c=%ld, h=%ld, hc=%ld\n", xp, yp, seg->LAYER, xn, yn, seg->LAYER, distance, seg->COST, seg->H, seg->HCOST);*/
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
#if 0
    display (LEVEL, DEBUG, "p (%ld, %ld, %ld) -> (%ld, %ld, %ld) : %ld\n", xp, yp, seg->LAYER, xn, yn, zs, distance);
    display (LEVEL, DEBUG, "c source (%ld, %ld, %ld)\n", xs, ys, zs);
    display (LEVEL, DEBUG, "p num of segments : %ld\n", nb_segs);
    display (LEVEL, DEBUG, "i path length : %ld\n", distance);
#endif
    /*seg->SIGNAL_INDEX = signal_index;*/
        

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

/* reroutage */
ocrNaturalInt re_route () {
    ocrSignal *l_pSignal;
    ocrNaturalInt l_uLength;
    ocrNaturalInt l_uReturnValue = 1;
    chain_list *l_pSignalList;
    ocrConnector *l_pCon;



    ripup = 0;

    // tant qu'il reste des signaux à re router
    l_pSignalList = ocr_db->RIPUP;
    while (l_pSignalList) {

        l_pSignal = (ocrSignal *) l_pSignalList->DATA;

//    fprintf (stdout, "reRoute : Routage du signal %ld \n",
        //             l_pSignal->INDEX);
        markSegmentAsFree(ocr_db, l_pSignal, l_pSignal->INDEX);
        for (l_pCon = l_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
            if (l_pCon->critVC) {
                unProtectVC(ocr_db, l_pCon->critVC);
            }
        }
        l_uLength = FINDNPATH(param, ocr_db->GRID,
                              l_pSignal,
                              NULL); 

        if (l_uLength == OCRNATURALINT_MAX) {
            l_uReturnValue = 0;
            display(LEVEL, VVERB, "o Echec reRoute %ld\n", l_pSignal->INDEX);
            l_pSignal->ROUTED = 2;

            deleteEquipotentielle(param, ocr_db->GRID, l_pSignal);

            l_pSignal->SEGMENT = NULL;

            ocr_db->NB_UNROUTED++;
            l_pSignal->PRIORITY = ocr_db->NB_IT+1;
        } else {
            ocr_db->NB_ROUTED++;
            l_pSignal->ROUTED = 1;
        }
        unMarkSegmentAsFree(ocr_db, l_pSignal, l_pSignal->INDEX);

        l_pSignalList = l_pSignalList->NEXT;
    }

    freechain(ocr_db->RIPUP);
    ocr_db->RIPUP = NULL;
    ripup = 1;
    return l_uReturnValue;
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
            case AS_K_SEG: //path = explore (segment_source, segment_dest);
                           segment_dest->SIGNAL_INDEX = CUR_SIG_INDEX;
                           path = explore_equi (segment_source, segment_dest);
                           segment_dest->SIGNAL_INDEX = WSEGMENT_FREE;
                           break;
            case AS_K_EQUI: assert (i_pSignal->SEGMENT);
                            //ze_best = NULL;
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
        ze_target = segment_dest;

#if 0
        if (g_pOption->RIPUP) {



        }
#endif
        /*display (LEVEL, DEBUG, "o A* failed\n");*/
        return OCRNATURALINT_MAX;
    }

    /*display (LEVEL, DEBUG, "o A* succeeded\n");*/

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

    //unMarkSegmentAsFree(ocr_db, i_pSignal, i_pSignal->INDEX);

    //re_route ();

    /*assert (((check_dist = check_path (i_pSignal)), dist == check_dist));*/

    return dist;
}


/**
 *
 **/
void init_Astar (ocrRoutingDataBase *db) {
    assert (db);

    ocr_db = db;

    return;    
}
