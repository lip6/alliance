/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:24 $
   $Log: ocrWSegLabeling.c,v $
   Revision 1.1  2002/03/15 14:37:24  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:22  hcl
   First commit in the new tree.

   Revision 1.2  2001/12/14 15:58:39  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.1  2001/09/26 14:27:51  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***


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
#include "ocrWSegLabeling.h"

static char *res_id =
    "$Id: ocrWSegLabeling.c,v 1.1 2002/03/15 14:37:24 hcl Exp $";

/** delta comparison function for red-black tree **/

static int deltaCompare(void *first, void *second)
{

    return ((ocrNaturalInt) first == (ocrNaturalInt) second) ?
        0 : (((ocrNaturalInt) first > (ocrNaturalInt) second) ? 1 : -1);
}

/** remove redundant next labels **/

static rbtree *removeRedundantLabels(ocrRoutingParameters * param,
                                     ocrWSegment * segment,
                                     ocrNaturalInt xtarget,
                                     ocrNaturalInt ytarget,
                                     rbtree * root, rbtree * node)
{
    rbtree *next_node = getnextrbtree(node);
    rbtree *del;

    while ((next_node != RBTREENULL) &&
           (DELTA(next_node) >=
            inducedDelta(param,
                         segment,
                         xtarget, ytarget, node, POINT(next_node)))) {
        del = next_node;
        next_node = getnextrbtree(next_node);
	if (root)
            root = delrbtree(root, del);
    }
    return root;
}

/** add a label in a segment **/

static int
addLabel(ocrRoutingParameters * param,
         ocrWSegment * segment,
         ocrNaturalInt xtarget,
         ocrNaturalInt ytarget,
         rbtree ** ptroot, ocrNaturalInt delta, ocrNaturalInt point)
{
    rbtree *node = *ptroot;
    rbtree *parent = RBTREENULL;
    rbtree *previous_node = RBTREENULL;

    while (node != RBTREENULL) {
        parent = node;
        if (delta == DELTA(node)) {
            if (delta <
                inducedDelta(param, segment, xtarget, ytarget, node,
                             point)) {
                node->DATA = (void *) point;
                *ptroot = removeRedundantLabels(param,
                                                segment,
                                                xtarget,
                                                ytarget, *ptroot, node);
                return 1;
            } else
                return 0;
        }
        if (delta < DELTA(node))
            node = node->Left;
        else {
            previous_node = node;
            node = node->Right;
        }
    }
    if (parent == RBTREENULL) { // ajoute delta au segment
        *ptroot =
            addrbtree(RBTREENULL, (void *) delta, (void *) point,
                      deltaCompare);
        return 1;
    }
    if ((previous_node == RBTREENULL) ||
        (delta < inducedDelta(param,
                              segment,
                              xtarget, ytarget, previous_node, point))) {
        *ptroot =
            addrbtree2(*ptroot, parent, (void *) delta, (void *) point,
                       deltaCompare);
        if (delta < DELTA(parent))
            *ptroot = removeRedundantLabels(param,
                                            segment,
                                            xtarget,
                                            ytarget,
                                            *ptroot, parent->Left);
        else
            *ptroot = removeRedundantLabels(param,
                                            segment,
                                            xtarget,
                                            ytarget,
                                            *ptroot, parent->Right);
        return 1;
    }
    return 0;
}

/** label segment function **/
int
labelWSegment(ocrRoutingParameters * param,
              ocrWSegment * segment,
              ocrNaturalInt xtarget,
              ocrNaturalInt ytarget,
              ocrNaturalInt delta, ocrNaturalInt point)
{
    int ret_LT = 0;
    int ret_RB = 0;

    if (getWSegDirection(param, segment) == ocrHorizontal) {
        if (point <= xtarget) {
            ret_LT = addLabel(param,
                              segment,
                              xtarget,
                              ytarget,
                              &segment->LABELS_LEFT_TOP, delta, point);
            if (segment->P_MAX > xtarget)
                ret_RB = addLabel(param,
                                  segment,
                                  xtarget,
                                  ytarget,
                                  &segment->LABELS_RIGHT_BOTTOM,
                                  delta + param->PITCH_H * 2, xtarget + 1);
        } else {
            ret_RB = addLabel(param,
                              segment,
                              xtarget,
                              ytarget,
                              &segment->LABELS_RIGHT_BOTTOM, delta, point);
            if (segment->P_MIN <= xtarget)
                ret_LT = addLabel(param,
                                  segment,
                                  xtarget,
                                  ytarget,
                                  &segment->LABELS_LEFT_TOP,
                                  delta, xtarget);
        }
    } else                      // VERTICAL
    {
        if (point <= ytarget) {
            ret_LT = addLabel(param,
                              segment,
                              xtarget,
                              ytarget,
                              &segment->LABELS_LEFT_TOP, delta, point);
            if (segment->P_MAX > ytarget)
                ret_RB = addLabel(param,
                                  segment,
                                  xtarget,
                                  ytarget,
                                  &segment->LABELS_RIGHT_BOTTOM,
                                  delta + param->PITCH_V * 2, ytarget + 1);
        } else {
            ret_RB = addLabel(param,
                              segment,
                              xtarget,
                              ytarget,
                              &segment->LABELS_RIGHT_BOTTOM, delta, point);
            if (segment->P_MIN <= ytarget)
                ret_LT = addLabel(param,
                                  segment,
                                  xtarget,
                                  ytarget,
                                  &segment->LABELS_LEFT_TOP,
                                  delta, ytarget);
        }
    }
    return MAX(ret_LT, ret_RB);
}

/** unlabel segment function **/

void unlabelWSegment(ocrWSegment * segment)
{

    freerbtree(segment->LABELS_LEFT_TOP, NULL, NULL);
    freerbtree(segment->LABELS_RIGHT_BOTTOM, NULL, NULL);
    segment->LABELS_LEFT_TOP = RBTREENULL;
    segment->LABELS_RIGHT_BOTTOM = RBTREENULL;
}
