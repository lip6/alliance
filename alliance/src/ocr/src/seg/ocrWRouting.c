/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:23 $
   $Log: ocrWRouting.c,v $
   Revision 1.1  2002/03/15 14:37:23  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:21  hcl
   First commit in the new tree.

   Revision 1.6  2002/01/14 10:34:34  hcl
   OCR should be MBK_SCALE_X - compliant

   Revision 1.5  2001/12/14 15:58:39  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.4  2001/12/10 13:09:02  hcl
   Un bigbug en moins.

   Revision 1.3  2001/12/03 14:31:23  hcl
   Pour la route.

   Revision 1.2  2001/11/20 09:41:34  hcl
   segmentation fault

   Revision 1.1  2001/09/26 14:27:51  hcl
   premier commit....

   Revision 1.3  2000/02/26 00:24:09  root
   2 points OK

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */
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
#include "ocrWSegLabeling.h"
#include "ocrWPlaneLabeling.h"
#include "ocrWRouting.h"

//#define DEBUG     

static char *res_id =
    "$Id: ocrWRouting.c,v 1.1 2002/03/15 14:37:23 hcl Exp $";


#ifdef DEBUG
void func(rbtree * r)
{
    printf("key = %d\n", (int) (r->KEY));
    printf("data = %d\n", (int) (r->DATA));
}
#endif

static ocrNaturalInt
backtrack(ocrRoutingParameters * param,
          ocrWRoutingGrid * grid,
          ocrNaturalInt xtarget,
          ocrNaturalInt ytarget,
          ocrWSegment * segment_target,
          ocrWSegment * segment_source,
          ocrNaturalInt delta,
          ocrNaturalInt signal_index, ocrSignal * i_pSignal)
{
    ocrWSegment *l_pSegList = i_pSignal->SEGMENT;
//  ocrVirtualConnector *l_pViaList = i_pSignal->VIA;
//  ocrVirtualConnector *l_pVia;

    rbtree *label;
    ocrWSegment *segment = segment_target;
    ocrWSegment *l_pOldSegment = segment_target;
    ocrWSegment *neighbouring_segment;
    ocrNaturalInt neighbouring_segment_delta;
    ocrNaturalInt point1 =
        (getWSegDirection(param, segment_target) ==
         ocrHorizontal) ? xtarget : ytarget;
    ocrNaturalInt point2 = POINT(getLabel(param,
                                          xtarget,
                                          ytarget,
                                          segment_target,
                                          delta,
                                          point1));

#ifdef DEBUG
    printf("segment Source %p\n", segment_target);
    dumpSeg(segment_target);
    printf("segment Target %p\n", segment_source);
    dumpSeg(segment_source);
#endif
/*
   // ajout d'un VIA
   l_pVia = createVirtualConnector (xtarget * 500,
   ytarget * 500,
   segment_target->LAYER,
   0,
   0);
   addVirtualConnector (&l_pViaList, l_pVia);
 */

    while (segment != segment_source) {
        if (point1 < point2)
            segment = splitWSegment(param,
                                    grid,
                                    segment, point1, point2, signal_index);
        else
            segment = splitWSegment(param,
                                    grid,
                                    segment, point2, point1, signal_index);
        point1 = segment->OFFSET;

        // Ajout du nouveau segment de l'équipotentielle dans la liste
        segment->NEXT = l_pSegList;
        l_pSegList = segment;

#ifdef DEBUG
        dumpSeg(segment);

//   printf ("LAYER = %ld\n", segment->LAYER);
        fflush(stdout);
#endif
        if (segment->LAYER < grid->NB_OF_LAYERS - 1) {
            neighbouring_segment = getWSegment(grid,
                                               getWSegXCoord(param,
                                                             segment,
                                                             point2),
                                               getWSegYCoord(param,
                                                             segment,
                                                             point2),
                                               segment->LAYER + 1);
            neighbouring_segment_delta =
                (segment->LAYER <
                 segment_target->LAYER) ? delta -
                2 * param->VIA_COST : delta;

#ifdef DEBUG
            printf("point1 %d, point2 %d delta %d\n", point1, point2,
                   delta);
#endif

            if ((label = getLabel(param,
                                  xtarget,
                                  ytarget,
                                  neighbouring_segment,
                                  neighbouring_segment_delta,
                                  point1)) != RBTREENULL) {
                /*
                   // ajout d'un VIA
                   if (getWSegDirection (param, segment) == ocrHorizontal)
                   l_pVia = createVirtualConnector (point2 * 500,
                   segment->OFFSET * 500,
                   neighbouring_segment->LAYER, 0, 0);
                   else
                   l_pVia = createVirtualConnector (segment->OFFSET * 500,
                   point2 * 500,
                   neighbouring_segment->LAYER, 0, 0);
                   addVirtualConnector (&l_pViaList, l_pVia);
                 */
                delta = neighbouring_segment_delta;
                point2 = POINT(label);
                segment = neighbouring_segment;

#ifdef DEBUG
                mapleasttomostrbtree(segment->LABELS_LEFT_TOP, func);
                mapleasttomostrbtree(segment->LABELS_RIGHT_BOTTOM, func);
                printf("point1 %d, point2 %d delta %d\n", point1, point2,
                       delta);

                printf("#########delta = %ld\n", DELTA(label));
#endif
                delta = DELTA(label);
                continue;
            }
        }

        delta = (segment->LAYER > segment_target->LAYER) ?
            delta - 2 * param->VIA_COST : delta;


        l_pOldSegment = segment;

        if (segment->LAYER == 0) {
            // impossible de continuer a router
            return 1;
        }
        segment = getWSegment(grid,
                              getWSegXCoord(param, segment, point2),
                              getWSegYCoord(param, segment, point2),
                              segment->LAYER - 1);
/*
   // ajout d'un VIA
   if (getWSegDirection (param, l_pOldSegment) == ocrHorizontal)
   l_pVia = createVirtualConnector (point2 * 500,
   l_pOldSegment->OFFSET * 500,
   MAX (segment->LAYER, l_pOldSegment->LAYER), 0, 0);
   else
   l_pVia = createVirtualConnector (l_pOldSegment->OFFSET * 500,
   point2 * 500,
   MAX (segment->LAYER, l_pOldSegment->LAYER), 0, 0);
   addVirtualConnector (&l_pViaList, l_pVia);
 */


#ifdef OCR_DEBUG
        printf("segment %p\n", segment);
        //dumpSeg (segment);
        fflush(stdout);
        printf("point1 %d, point2 %d delta %d\n", point1, point2, delta);
        printf("Target  x=%d, y=%d\n", xtarget, ytarget);
        //mapleasttomostrbtree (segment->LABELS_LEFT_TOP, func);
        //mapleasttomostrbtree (segment->LABELS_RIGHT_BOTTOM, func);
#endif

        point2 = POINT(getLabel(param,
                                xtarget, ytarget, segment, delta, point1));


#ifdef DEBUG
        printf("#########delta = %ld\n", DELTA(getLabel(param,
                                                        xtarget,
                                                        ytarget,
                                                        segment,
                                                        delta, point1)));
#endif

        delta = DELTA(getLabel(param,
                               xtarget, ytarget, segment, delta, point1));

#ifdef DEBUG
        // WARNING ....
        if (point2 == 0)
            puts("ZERO");

        printf("point1 %d, point2 %d\n", point1, point2);
        fflush(stdout);
#endif
    }

    if (segment->SIGNAL_INDEX != signal_index) {
        if (point1 < point2) {
            segment =
                splitWSegment(param, grid, segment, point1, point2,
                              signal_index);

//            if (segment->LAYER > 2) {
//                if (getWSegDirection (param, segment) == ocrHorizontal) {
//                    splitWSegment(param, grid
//                } else {
//
//                }
//            }
        } else {
            segment =
                splitWSegment(param, grid, segment, point2, point1,
                              signal_index);
        }

        // Ajout du dernier segment de l'équipotentielle dans la liste
        segment->NEXT = l_pSegList;
        i_pSignal->SEGMENT = segment;
/*
   // ajout d'un VIA
   if (getWSegDirection (param, segment) == ocrHorizontal)
   l_pVia = createVirtualConnector (point2 * 500,
   segment->OFFSET * 500,
   segment->LAYER, 0, 0);
   else
   l_pVia = createVirtualConnector (segment->OFFSET * 500,
   point2 * 500,
   segment->LAYER, 0, 0);
   addVirtualConnector (&l_pViaList, l_pVia);

   i_pSignal->VIA = l_pViaList;
 */
    }
    return 0;
}

ocrNaturalInt
findPathBetween2Points(ocrRoutingParameters * param,
                       ocrWRoutingGrid * grid,
                       ocrNaturalInt xsource,
                       ocrNaturalInt ysource,
                       ocrNaturalInt zsource,
                       ocrNaturalInt xtarget,
                       ocrNaturalInt ytarget,
                       ocrNaturalInt ztarget,
                       ocrNaturalInt signal_index, ocrSignal * i_pSignal)
{
    chain_list **labeledSegmentList;
    chain_list **nextDeltaSubSegmentList;
    ocrNaturalInt *nbOfNextDeltaSubSegments;
    ocrNaturalInt l_uNbIt = 0;

    ocrNaturalInt delta =
        manhattan3Distance(param, xsource, ysource, zsource,
                           xtarget, ytarget, ztarget);

    ocrWSegment *segmentSource =
        getWSegment(grid, xsource, ysource, zsource);
    ocrNaturalInt pointSource =
        (getWSegDirection(param, segmentSource) ==
         ocrHorizontal) ? xsource : ysource;

    ocrWSegment *segmentTarget =
        getWSegment(grid, xtarget, ytarget, ztarget);

    chain_list *segmentGarbage = NULL;

    ocrNaturalInt plane = ztarget;

    //printf ("2points\n");
    labelWSegment(param, segmentSource, xtarget, ytarget, delta,
                  pointSource);

    // initialise le tableau de liste et ajoute segmentSource
    initLabeledWSegList(grid, &labeledSegmentList, segmentSource);

    // initialise le tableau de liste des sub segment
    // Ajoute le premier
    initNextDeltaSubWSegList(param,
                             grid,
                             xtarget,
                             ytarget,
                             &nextDeltaSubSegmentList,
                             &nbOfNextDeltaSubSegments,
                             segmentSource, xsource, ysource);

    do {
        do {
            plane = getNextPlane(grid, ztarget, plane);
            if (plane < grid->NB_OF_LAYERS - 1) // via vers le haut ?


                //printf (" o * labelPlane vers le haut\n");
                labelPlane(param,
                           grid,
                           xtarget,
                           ytarget,
                           ztarget,
                           delta,
                           plane,
                           plane + 1,
                           nextDeltaSubSegmentList,
                           nbOfNextDeltaSubSegments, labeledSegmentList);
            if (plane > 0)      // via vers le bas ?

                //printf (" o * labelPlane vers le bas\n");
                labelPlane(param,
                           grid,
                           xtarget,
                           ytarget,
                           ztarget,
                           delta,
                           plane,
                           plane - 1,
                           nextDeltaSubSegmentList,
                           nbOfNextDeltaSubSegments, labeledSegmentList);
        }
        while (plane != ztarget);

        if (isLabeled(segmentTarget))   // fin

        {
            if (backtrack(param,
                          grid,
                          xtarget,
                          ytarget,
                          segmentTarget,
                          segmentSource, delta, signal_index, i_pSignal)
                ) {
                cleanLabeledWSegments(grid, labeledSegmentList,
                                      &segmentGarbage);
                mbkfree(labeledSegmentList);
                freeNextDeltaSubWSegList(nextDeltaSubSegmentList,
                                         nbOfNextDeltaSubSegments,
                                         grid->NB_OF_LAYERS - 1);
                mbkfree(nextDeltaSubSegmentList);
                mbkfree(nbOfNextDeltaSubSegments);

                return OCRNATURALINT_MAX;
            }

            cleanLabeledWSegments(grid, labeledSegmentList,
                                  &segmentGarbage);
            mbkfree(labeledSegmentList);
            freeNextDeltaSubWSegList(nextDeltaSubSegmentList,
                                     nbOfNextDeltaSubSegments,
                                     grid->NB_OF_LAYERS - 1);
            mbkfree(nextDeltaSubSegmentList);
            mbkfree(nbOfNextDeltaSubSegments);
            //printf("IT : %ld\n",l_uNbIt);
            return delta;
        }

        l_uNbIt++;

        if (l_uNbIt > 50)
            goto fin;

        delta = processNextDelta(param,
                                 grid,
                                 xtarget,
                                 ytarget,
                                 delta,
                                 labeledSegmentList,
                                 nextDeltaSubSegmentList,
                                 nbOfNextDeltaSubSegments,
                                 &segmentGarbage);
    }
    while (isNextDeltaSubWSegListNotEmpty(grid, nextDeltaSubSegmentList));

  fin:
    cleanLabeledWSegments(grid, labeledSegmentList, &segmentGarbage);
    mbkfree(labeledSegmentList);
    mbkfree(nextDeltaSubSegmentList);
    mbkfree(nbOfNextDeltaSubSegments);

//  printf ("It : %ld\n", l_uNbIt);
    return OCRNATURALINT_MAX;
}

void
displayPath(ocrRoutingParameters * param,
            ocrWRoutingGrid * grid,
            ocrNaturalInt xsource,
            ocrNaturalInt ysource,
            ocrNaturalInt zsource,
            ocrNaturalInt xtarget,
            ocrNaturalInt ytarget, ocrNaturalInt ztarget)
{
    ocrWSegment *segment = getWSegment(grid, xsource, ysource, zsource);
    ocrWSegment *segment_target =
        getWSegment(grid, xtarget, ytarget, ztarget);
    ocrWSegment *neighbouring_segment;
    ocrNaturalInt point1 =
        (getWSegDirection(param, segment) ==
         ocrHorizontal) ? xsource : ysource;
    ocrNaturalInt point2 =
        (point1 == segment->P_MIN) ? segment->P_MAX : segment->P_MIN;

    printf("source point : <%lu,%lu,%lu>\n", xsource, ysource, zsource);
    printf("target point : <%lu,%lu,%lu>\n", xtarget, ytarget, ztarget);
    printf("path : ");

    while (segment != segment_target) {
        printf("<%lu,%lu,%lu> to <%lu,%lu,%lu> via ", getWSegXCoord(param, segment, point1),    // si HORZ -> point1
               getWSegYCoord(param, segment, point1),   // si HORZ -> OFFSET
               segment->LAYER,
               getWSegXCoord(param, segment, point2),
               getWSegYCoord(param, segment, point2), segment->LAYER);

        point1 = segment->OFFSET;
        // suite vers le haut ?
        if ((segment->LAYER < grid->NB_OF_LAYERS - 1) &&
            ((neighbouring_segment = getWSegment(grid,
                                                 getWSegXCoord(param,
                                                               segment,
                                                               point2),
                                                 getWSegYCoord(param,
                                                               segment,
                                                               point2),
                                                 segment->LAYER +
                                                 1))->SIGNAL_INDEX ==
             segment_target->SIGNAL_INDEX)) {
            point2 =
                (point1 ==
                 neighbouring_segment->P_MIN) ? neighbouring_segment->
                P_MAX : neighbouring_segment->P_MIN;
            segment = neighbouring_segment;
        } else                  // suite vers le bas ?

        {
            segment = getWSegment(grid,
                                  getWSegXCoord(param, segment, point2),
                                  getWSegYCoord(param, segment, point2),
                                  segment->LAYER - 1);
            point2 =
                (point1 ==
                 segment->P_MIN) ? segment->P_MAX : segment->P_MIN;
        }
    }
    printf("<%lu,%lu,%lu> to <%lu,%lu,%lu>\n",
           getWSegXCoord(param, segment_target, point1),
           getWSegYCoord(param, segment_target, point1),
           segment_target->LAYER, xtarget, ytarget, ztarget);
    fflush(stdout);
}
