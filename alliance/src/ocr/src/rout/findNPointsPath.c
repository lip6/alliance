/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/20 13:25:50 $
   $Log: findNPointsPath.c,v $
   Revision 1.2  2002/03/20 13:25:50  hcl
   SymX bug.

   Revision 1.1  2002/03/15 14:37:19  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:16  hcl
   First commit in the new tree.

   Revision 1.14  2002/02/25 11:41:12  hcl
   A la chasse au bug bug bug

   Revision 1.13  2002/02/21 13:17:56  hcl
   Introducing a new algo (A*, dont yet work...)

   Revision 1.12  2002/02/20 16:31:47  xtof
   syntax errors ....

   Revision 1.11  2002/02/12 15:14:14  hcl
   New algo.

   Revision 1.10  2002/01/25 13:45:47  hcl
   Bug affichage.

   Revision 1.9  2002/01/14 10:34:33  hcl
   OCR should be MBK_SCALE_X - compliant

   Revision 1.8  2001/12/20 13:04:03  hcl
   Cosmetique.

   Revision 1.7  2001/12/19 14:22:49  hcl
   Moult petits changements.

   Revision 1.6  2001/12/14 15:58:35  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.5  2001/12/12 14:50:58  hcl
   Prise en compte du placement des connecteurs par OCP.

   Revision 1.4  2001/12/10 13:08:52  hcl
   Un bigbug en moins.

   Revision 1.3  2001/12/03 14:31:21  hcl
   Pour la route.

   Revision 1.2  2001/11/20 09:41:09  hcl
   chez Francis, on y route le mips ...

   Revision 1.1  2001/09/26 14:27:48  hcl
   premier commit....

   Revision 1.3  2000/02/26 00:25:22  root
   2 points OK

   Revision 1.2  2000/02/19 14:45:26  root
   *** empty log message ***

   Revision 1.1  2000/02/19 00:13:07  root
   Initial revision

   ### -------------------------------------------------- ###
 */

#include <stdlib.h>
#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"

#include "ocr.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "ocrPath.h"
#include "display.h"

#include "ocrConnectorUtil.h"
#include "ocrNpoints.h"
#include "ocrSignalUtil.h"

#include "ocrAstar.h"

static char *res_id =
    "$Id: findNPointsPath.c,v 1.2 2002/03/20 13:25:50 hcl Exp $";

#define MAX_HT 500

#define FINDPATH findPathBetween2Points
#define FINDNPATH findPathNPoints

extern ocrOption *g_pOption;

#define LEVEL (g_pOption->LEVEL)

void
unifyPoint(ocrWRoutingGrid * i_pGrid,
           ocrRoutingParameters * i_pParam,
           ocrSignal * i_pSignal,
           ocrNaturalInt i_uXPoint,
           ocrNaturalInt i_uYPoint, ocrNaturalInt i_uZPoint)
{
    ocrWSegment *l_pSegment;
    ocrWSegment *l_pSegment1;
    ocrWSegment *l_pSegment2;
    ocrNaturalInt i;
    ocrNaturalInt l_uIndex;

    l_pSegment1 = getWSegment(i_pGrid, i_uXPoint, i_uYPoint, i_uZPoint);
    l_uIndex = l_pSegment1->SIGNAL_INDEX;

    if (getWSegDirection(i_pParam, l_pSegment1) == ocrHorizontal) {
        if (l_pSegment1->P_MAX < i_pGrid->SIZE_H - 1) {
            l_pSegment2 = getWSegment(i_pGrid,
                                      l_pSegment1->P_MAX + 1,
                                      i_uYPoint, i_uZPoint);

            if (l_pSegment2->SIGNAL_INDEX == l_uIndex) {
                l_pSegment = createWSegment(i_uYPoint,
                                            i_uZPoint,
                                            l_pSegment1->P_MIN,
                                            l_pSegment2->P_MAX, l_uIndex);

                // mise à jour de la liste des segments du signal
                deleteSegmentSignalList(i_pSignal, l_pSegment1);
                deleteSegmentSignalList(i_pSignal, l_pSegment2);

                addSegmentSignalList(i_pSignal, l_pSegment);

                // mise à jour de la grille
                for (i = l_pSegment1->P_MIN; i <= l_pSegment2->P_MAX; i++)
                    setWGrid(i_pGrid, l_pSegment, i, i_uYPoint, i_uZPoint);
            }
        }

        if (l_pSegment1->P_MIN > 0) {
            l_pSegment2 = getWSegment(i_pGrid,
                                      l_pSegment1->P_MIN - 1,
                                      i_uYPoint, i_uZPoint);

            if (l_pSegment2->SIGNAL_INDEX == l_uIndex) {
                l_pSegment = createWSegment(i_uYPoint,
                                            i_uZPoint,
                                            l_pSegment2->P_MIN,
                                            l_pSegment1->P_MAX, l_uIndex);

                // mise à jour de la liste des segments du signal
                deleteSegmentSignalList(i_pSignal, l_pSegment1);
                deleteSegmentSignalList(i_pSignal, l_pSegment2);

                addSegmentSignalList(i_pSignal, l_pSegment);

                // mise à jour de la grille
                for (i = l_pSegment2->P_MIN; i <= l_pSegment1->P_MAX; i++)
                    setWGrid(i_pGrid, l_pSegment, i, i_uYPoint, i_uZPoint);
            }
        }
    } else {
        if (l_pSegment1->P_MAX < i_pGrid->SIZE_V - 1) {
            l_pSegment2 = getWSegment(i_pGrid,
                                      i_uXPoint,
                                      l_pSegment1->P_MAX + 1, i_uZPoint);

            if (l_pSegment2->SIGNAL_INDEX == l_uIndex) {
                l_pSegment = createWSegment(i_uXPoint,
                                            i_uZPoint,
                                            l_pSegment1->P_MIN,
                                            l_pSegment2->P_MAX, l_uIndex);

                // mise à jour de la liste des segments du signal
                deleteSegmentSignalList(i_pSignal, l_pSegment1);
                deleteSegmentSignalList(i_pSignal, l_pSegment2);

                addSegmentSignalList(i_pSignal, l_pSegment);

                for (i = l_pSegment1->P_MIN; i <= l_pSegment2->P_MAX; i++)
                    setWGrid(i_pGrid, l_pSegment, i_uXPoint, i, i_uZPoint);
            }
        }

        if (l_pSegment1->P_MIN > 0) {
            l_pSegment2 = getWSegment(i_pGrid,
                                      i_uXPoint,
                                      l_pSegment1->P_MIN - 1, i_uZPoint);

            if (l_pSegment2->SIGNAL_INDEX == l_uIndex) {
                l_pSegment = createWSegment(i_uXPoint,
                                            i_uZPoint,
                                            l_pSegment2->P_MIN,
                                            l_pSegment1->P_MAX, l_uIndex);

                // mise à jour de la liste des segments du signal
                deleteSegmentSignalList(i_pSignal, l_pSegment1);
                deleteSegmentSignalList(i_pSignal, l_pSegment2);

                addSegmentSignalList(i_pSignal, l_pSegment);

                for (i = l_pSegment2->P_MIN; i <= l_pSegment1->P_MAX; i++)
                    setWGrid(i_pGrid, l_pSegment, i_uXPoint, i, i_uZPoint);
            }
        }
    }
}

void
addSegmentToEqui(ocrRoutingParameters * i_pParam,
                 ocrWRoutingGrid * i_pGrid,
                 ocrWSegment * i_pSegment,
                 ocrVirtualConnector ** i_pVirConList)
{
    ocrNaturalInt i;
    ocrWSegment *l_pSegment;

    if (getWSegDirection(i_pParam, i_pSegment) == ocrHorizontal) {
        for (i = i_pSegment->P_MIN; i <= i_pSegment->P_MAX; i++) {
            if (i_pSegment->LAYER > 0) {
                l_pSegment = getWSegment(i_pGrid,
                                         i,
                                         i_pSegment->OFFSET,
                                         i_pSegment->LAYER - 1);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {

                    addVirtualConnector(i_pVirConList, createVirtualConnector(i, i_pSegment->OFFSET, i_pSegment->LAYER, // XXX - 1
                                                                              0,
                                                                              0));

                }
            } else if (i_pSegment->LAYER < i_pGrid->NB_OF_LAYERS - 2) {
                l_pSegment = getWSegment(i_pGrid,
                                         i,
                                         i_pSegment->OFFSET,
                                         i_pSegment->LAYER + 1);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {
                    addVirtualConnector(i_pVirConList, createVirtualConnector(i, i_pSegment->OFFSET, i_pSegment->LAYER + 2,     // XXX + 1
                                                                              0,
                                                                              0));

                }
            }
        }

        // Connexion par aboutement en ALU 0 ?

        if (i_pSegment->LAYER == 0) {
            if (i_pSegment->P_MIN > 0) {
                l_pSegment = getWSegment(i_pGrid,
                                         i_pSegment->P_MIN - 1,
                                         i_pSegment->OFFSET,
                                         i_pSegment->LAYER);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {
                    addVirtualConnector(i_pVirConList, createVirtualConnector(i_pSegment->P_MIN - 1, i_pSegment->OFFSET, i_pSegment->LAYER + 1, 0, 0)); // XXX ± 0
                }
            }

            if (i_pSegment->P_MAX < i_pGrid->SIZE_H - 1) {
                l_pSegment = getWSegment(i_pGrid,
                                         i_pSegment->P_MAX + 1,
                                         i_pSegment->OFFSET,
                                         i_pSegment->LAYER);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {
                    addVirtualConnector(i_pVirConList, createVirtualConnector(i_pSegment->P_MAX + 1, i_pSegment->OFFSET, i_pSegment->LAYER + 1, 0, 0)); // XXX ± 0
                }
            }

        }
    } else {
        for (i = i_pSegment->P_MIN; i <= i_pSegment->P_MAX; i++) {
            if (i_pSegment->LAYER > 0) {
                l_pSegment = getWSegment(i_pGrid,
                                         i_pSegment->OFFSET,
                                         i, i_pSegment->LAYER - 1);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {
                    addVirtualConnector(i_pVirConList, createVirtualConnector(i_pSegment->OFFSET, i, i_pSegment->LAYER, 0, 0)); // XXX - 1
                }
            } else if (i_pSegment->LAYER < i_pGrid->NB_OF_LAYERS - 2) {
                l_pSegment = getWSegment(i_pGrid,
                                         i_pSegment->OFFSET,
                                         i, i_pSegment->LAYER + 1);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {
                    addVirtualConnector(i_pVirConList, createVirtualConnector(i_pSegment->OFFSET, i, i_pSegment->LAYER + 2, 0, 0));     // XXX + 1
                }
            }
        }

        // Connexion par aboutement en ALU 1 ?

        if (i_pSegment->LAYER == 1) {
            if (i_pSegment->P_MIN > 0) {
                l_pSegment = getWSegment(i_pGrid,
                                         i_pSegment->OFFSET,
                                         i_pSegment->P_MIN - 1,
                                         i_pSegment->LAYER);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {
                    addVirtualConnector(i_pVirConList, createVirtualConnector(i_pSegment->OFFSET, (i_pSegment->P_MIN - 1), i_pSegment->LAYER + 1, 0, 0));       // XXX ± 0
                }
            }

            if (i_pSegment->P_MAX < i_pGrid->SIZE_V - 1) {
                l_pSegment = getWSegment(i_pGrid,
                                         i_pSegment->OFFSET,
                                         i_pSegment->P_MAX + 1,
                                         i_pSegment->LAYER);

                if ((l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) ||
                    (l_pSegment->SIGNAL_INDEX == i_pSegment->SIGNAL_INDEX))
                {
                    addVirtualConnector(i_pVirConList, createVirtualConnector(i_pSegment->OFFSET, (i_pSegment->P_MAX + 1), i_pSegment->LAYER + 1, 0, 0));       // XXX ± 0
                }
            }
        }
    }
}

void
deleteSegment(ocrRoutingParameters * i_pParam,
              ocrWRoutingGrid * i_pGrid, ocrWSegment * i_pSegment)
{
    ocrWSegment *l_pSegment;
    ocrWSegment *l_pNewSegment;
    ocrNaturalInt i;

    //printf ("delete seg %ld\n", i_pSegment->SIGNAL_INDEX);
    // libération du segment
    i_pSegment->SIGNAL_INDEX = WSEGMENT_FREE;

    // Concaténation avec d'autres segments libres eux aussi !
    if (getWSegDirection(i_pParam, i_pSegment) == ocrHorizontal) {
        if (i_pSegment->P_MIN > 0) {
            l_pSegment = getWSegment(i_pGrid,
                                     i_pSegment->P_MIN - 1,
                                     i_pSegment->OFFSET,
                                     i_pSegment->LAYER);

            if (l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) {
                l_pNewSegment = createWSegment(i_pSegment->OFFSET,
                                               i_pSegment->LAYER,
                                               l_pSegment->P_MIN,
                                               i_pSegment->P_MAX,
                                               WSEGMENT_FREE);

                for (i = l_pSegment->P_MIN; i <= i_pSegment->P_MAX; i++)
                    setWGrid(i_pGrid,
                             l_pNewSegment,
                             i, i_pSegment->OFFSET, i_pSegment->LAYER);
/*
   printf ("index --> %ld\n",
   getWSegment (i_pGrid,
   690 / 5,
   220 / 5,
   0)->SIGNAL_INDEX);
 */
                i_pSegment = l_pNewSegment;
            }
        }

        if (i_pSegment->P_MAX < i_pGrid->SIZE_H - 1) {
            l_pSegment = getWSegment(i_pGrid,
                                     i_pSegment->P_MAX + 1,
                                     i_pSegment->OFFSET,
                                     i_pSegment->LAYER);

            if (l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) {
                //            dumpSeg (l_pSegment);
                l_pNewSegment = createWSegment(i_pSegment->OFFSET,
                                               i_pSegment->LAYER,
                                               i_pSegment->P_MIN,
                                               l_pSegment->P_MAX,
                                               WSEGMENT_FREE);

                for (i = i_pSegment->P_MIN; i <= l_pSegment->P_MAX; i++)
                    setWGrid(i_pGrid,
                             l_pNewSegment,
                             i, i_pSegment->OFFSET, i_pSegment->LAYER);

                i_pSegment = l_pNewSegment;
            }
        }
    } else {
        if (i_pSegment->P_MIN > 0) {
            l_pSegment = getWSegment(i_pGrid,
                                     i_pSegment->OFFSET,
                                     i_pSegment->P_MIN - 1,
                                     i_pSegment->LAYER);

            if (l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) {
                l_pNewSegment = createWSegment(i_pSegment->OFFSET,
                                               i_pSegment->LAYER,
                                               l_pSegment->P_MIN,
                                               i_pSegment->P_MAX,
                                               WSEGMENT_FREE);

                for (i = l_pSegment->P_MIN; i <= i_pSegment->P_MAX; i++)
                    setWGrid(i_pGrid,
                             l_pNewSegment,
                             i_pSegment->OFFSET, i, i_pSegment->LAYER);

                i_pSegment = l_pNewSegment;
            }
        }

        if (i_pSegment->P_MAX < i_pGrid->SIZE_H - 1) {
            l_pSegment = getWSegment(i_pGrid,
                                     i_pSegment->OFFSET,
                                     i_pSegment->P_MAX + 1,
                                     i_pSegment->LAYER);

            if (l_pSegment->SIGNAL_INDEX == WSEGMENT_FREE) {
                l_pNewSegment = createWSegment(i_pSegment->OFFSET,
                                               i_pSegment->LAYER,
                                               i_pSegment->P_MIN,
                                               l_pSegment->P_MAX,
                                               WSEGMENT_FREE);

                for (i = i_pSegment->P_MIN; i <= l_pSegment->P_MAX; i++)
                    setWGrid(i_pGrid,
                             l_pNewSegment,
                             i_pSegment->OFFSET, i, i_pSegment->LAYER);
            }
        }
    }
/*
   printf ("D index --> %ld\n",
   getWSegment (i_pGrid,
   690 / 5,
   220 / 5,
   0)->SIGNAL_INDEX);
 */
}

void
deleteEquipotentielle(ocrRoutingParameters * i_pParam,
                      ocrWRoutingGrid * i_pGrid, ocrSignal * i_pSignal)
{
    ocrVirtualConnector *l_pVirCon;
    ocrConnector *l_pCon;
    ocrWSegment *l_pSegment;

    // mise à jour de la grille de routage
    for (l_pSegment = i_pSignal->SEGMENT;
         l_pSegment; l_pSegment = l_pSegment->NEXT)
        deleteSegment(i_pParam, i_pGrid, l_pSegment);

    // Suppression des SEGMENTS & VIA associés
    //  i_pSignal->SEGMENT = NULL;
    i_pSignal->VIA = NULL;

    // Suppression des TAGS
    for (l_pCon = i_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
        l_pCon->CON = NULL;
        for (l_pVirCon = l_pCon->VIR_CON_LIST;
             l_pVirCon;
             l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT)
            l_pVirCon->TAG = 0;
    }
}

void
createEquipotentielle(ocrRoutingParameters * i_pParam,
                      ocrWRoutingGrid * i_pGrid,
                      ocrSignal * i_pSignal,
                      ocrVirtualConnector ** i_pVirConList)
{
    ocrWSegment *l_pSegment;

    for (l_pSegment = i_pSignal->SEGMENT;
         l_pSegment; l_pSegment = l_pSegment->NEXT)
        addSegmentToEqui(i_pParam, i_pGrid, l_pSegment, i_pVirConList);
}

#define DELTA 	1
#define COEF	1.3
void
makeExtEquipotentielle(ocrRoutingParameters * i_pParam,
                       ocrWRoutingGrid * i_pGrid,
                       ocrWindow * i_pWindow, ocrConnector * i_pCon)
{
    ocrNaturalInt l_uOffset = 3;
    float l_uDelta = 3.0;
    //ocrNaturalInt l_uDelta = 3;
    ocrVirtualConnector *l_pVirConList = NULL;
    ocrVirtualConnector *l_pVirCon;
    float i;
    //ocrNaturalInt i;

    // Si le connecteur est lié, sa position est déterminée.
    if (IS_MARK_AS_LINKED(i_pCon)) {
        if (!i_pCon->CON) {
            display(LEVEL, ERROR, "Internal error at %s:%d\n", __FILE__, __LINE__);
            abort ();
        }
        return;
    }
    // awfull hack... XXX
    return;
    if (i_pCon->VIR_CON_LIST) {
        //i_pCon->CON = i_pCon->VIR_CON_LIST;
        //i_pCon->VIR_CON_LIST->Z = 2;
        return;
    }
    // si besoin est !
    if (i_pCon->VIR_CON_LIST) {
        deleteVirtualConnectorList(i_pCon->VIR_CON_LIST);
        i_pCon->VIR_CON_LIST = NULL;
        i_pCon->CON = NULL;
    }
    // face SOUTH
    if (i_pCon->FACE & OCR_FACE_SOUTH) {
#if DELTA
        if (i_pWindow->NB_SIG_FACE[OCR_FACE_SOUTH] != 0)
            l_uDelta =
                ((float) (i_pWindow->XMAX - i_pWindow->XMIN)) / COEF /
                i_pWindow->NB_SIG_FACE[OCR_FACE_SOUTH];
        else
            l_uDelta = 2.0;
        if (l_uDelta > 2.0)
            l_uDelta = 2.0;
#endif

        for (i = l_uOffset; i < i_pGrid->SIZE_H - l_uOffset; i += l_uDelta) {
            if (IS_MARK_AS_FACE_CON(i_pCon))
                l_pVirCon = createVirtualConnector(i, 0, 1, 0, SOUTH);
            else
                l_pVirCon = createVirtualConnector(i, 0, 0, 0, SOUTH);

            if (isFreePoint(i_pCon, i_pGrid, l_pVirCon)) {
                (ocrVirtualConnector *) l_pVirCon->NEXT = l_pVirConList;
                l_pVirConList = l_pVirCon;
            }
            // FIXME : pas de free ???
        }
    }
    // face NORTH
    if (i_pCon->FACE & OCR_FACE_NORTH) {
#if DELTA
        if (i_pWindow->NB_SIG_FACE[OCR_FACE_NORTH] != 0)
            l_uDelta = ((i_pWindow->XMAX - i_pWindow->XMIN)) / COEF /
                i_pWindow->NB_SIG_FACE[OCR_FACE_NORTH];
        else
            l_uDelta = 2.0;
        if (l_uDelta > 2.0)
            l_uDelta = 2.0;
#endif


        for (i = l_uOffset; i < i_pGrid->SIZE_H - l_uOffset; i += l_uDelta) {
            if (IS_MARK_AS_FACE_CON(i_pCon))
                l_pVirCon = createVirtualConnector(i,
                                                   (i_pGrid->SIZE_V - 1),
                                                   1, 0, NORTH);
            else
                l_pVirCon = createVirtualConnector(i,
                                                   (i_pGrid->SIZE_V - 1),
                                                   0, 0, NORTH);

            if (isFreePoint(i_pCon, i_pGrid, l_pVirCon)) {
                (ocrVirtualConnector *) l_pVirCon->NEXT = l_pVirConList;
                l_pVirConList = l_pVirCon;
            }
        }
    }
    // face EAST
    if (i_pCon->FACE & OCR_FACE_EAST) {
#if DELTA
        if (i_pWindow->NB_SIG_FACE[OCR_FACE_EAST] != 0)
            l_uDelta = ((i_pWindow->YMAX - i_pWindow->YMIN)) / COEF /
                i_pWindow->NB_SIG_FACE[OCR_FACE_EAST];
        else
            l_uDelta = 2.0;
        if (l_uDelta > 2.0)
            l_uDelta = 2.0;
#endif


        for (i = l_uOffset; i < i_pGrid->SIZE_V - l_uOffset; i += l_uDelta) {
            l_pVirCon = createVirtualConnector((i_pGrid->SIZE_H - 1),
                                               i, 0, 0, EAST);
            if (isFreePoint(i_pCon, i_pGrid, l_pVirCon)) {
                (ocrVirtualConnector *) l_pVirCon->NEXT = l_pVirConList;
                l_pVirConList = l_pVirCon;
            }
        }
    }
    // face WEST
    if (i_pCon->FACE & OCR_FACE_WEST) {
#if DELTA
        if (i_pWindow->NB_SIG_FACE[OCR_FACE_WEST] != 0)
            l_uDelta = ((i_pWindow->YMAX - i_pWindow->YMIN)) / COEF /
                i_pWindow->NB_SIG_FACE[OCR_FACE_WEST];
        else
            l_uDelta = 2.0;
        if (l_uDelta > 2.0)
            l_uDelta = 2.0;
#endif


        for (i = l_uOffset; i < i_pGrid->SIZE_V - l_uOffset; i += l_uDelta) {
            l_pVirCon = createVirtualConnector(0, i, 0, 0, WEST);
            if (isFreePoint(i_pCon, i_pGrid, l_pVirCon)) {
                (ocrVirtualConnector *) l_pVirCon->NEXT = l_pVirConList;
                l_pVirConList = l_pVirCon;
            }
        }
    }

    i_pCon->VIR_CON_LIST = l_pVirConList;
}

void
addVirConToEqui(ocrRoutingParameters * i_pParam,
                ocrWRoutingGrid * i_pGrid,
                ocrSignal * i_pSignal,
                ocrVirtualConnector ** i_pVirConList)
{
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVirCon;
    ocrVirtualConnector *l_pVia;

    for (l_pCon = i_pSignal->CON_LIST;
         l_pCon && l_pCon->CON; l_pCon = l_pCon->NEXT)
        if (l_pCon->INTEXT == INTERNAL) {
            for (l_pVirCon = l_pCon->VIR_CON_LIST;
                 l_pVirCon;
                 l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT)
                if (l_pVirCon != l_pCon->CON) {

                    l_pVia = createVirtualConnector(l_pVirCon->X,
                                                    l_pVirCon->Y,
                                                    l_pVirCon->Z, 0, 0);
                    l_pVia->DIST = 1;
                    if (isFreePoint(l_pCon, i_pGrid, l_pVia))
                        addVirtualConnector(i_pVirConList, l_pVia);
                    // FIXME : pas de free ?
                }
        }
}


ocrVirtualConnector *makeEquipotentielle(ocrRoutingParameters * i_pParam,
                                         ocrWRoutingGrid * i_pGrid,
                                         ocrSignal * i_pSignal)
{
    ocrVirtualConnector *l_pVirCon = NULL;

    // Destruction de la liste si besoin est !
    //  deleteVirtualConnectorList (g_pVirConList);
    //  g_uIndexVirCon = 0;
    //  g_pVirConList = NULL;

    createEquipotentielle(i_pParam, i_pGrid, i_pSignal, &l_pVirCon);

//  addVirConToEqui (i_pParam, i_pGrid, i_pSignal, &l_pVirCon);

    return l_pVirCon;

}

ocrNaturalInt biroute (ocrRoutingParameters *i_pParam,
                       ocrWRoutingGrid *i_pGrid,
                       ocrConnector *l_pCon1,
                       ocrConnector *l_pCon2,
                       ocrSignal *i_pSignal,
                       ocrNaturalInt mode
                      )
{
    ocrNaturalInt l_uLength = 0;

    switch (g_pOption->ALGO) {
        case 0:
            if (mode == AS_K_EQUI) {
                l_uLength = find_path_astar(i_pParam, i_pGrid,
                                            l_pCon1->CON->X,
                                            l_pCon1->CON->Y,
                                            (l_pCon1->CON->Z ? l_pCon1->CON->Z - 1 : l_pCon1->CON->Z),
                                            0,
                                            0,
                                            0,
                                            i_pSignal->INDEX,
                                            i_pSignal,
                                            mode
                                           );
            } else {
                l_uLength = FINDPATH(i_pParam, i_pGrid,
                                     l_pCon1->CON->X,
                                     l_pCon1->CON->Y,
                                     (l_pCon1->CON->Z ? l_pCon1->CON->Z - 1 : l_pCon1->CON->Z),
                                     l_pCon2->CON->X,
                                     l_pCon2->CON->Y,
                                     (l_pCon2->CON->Z ? l_pCon2->CON->Z - 1 : l_pCon2->CON->Z),
                                     i_pSignal->INDEX,
                                     i_pSignal
                                    );
            }
            break;
        case 1:
            display (LEVEL, DEBUG, "\no Launching A*\n");
            if (l_pCon2) {
                l_uLength = find_path_astar(i_pParam, i_pGrid,
                                            l_pCon1->CON->X,
                                            l_pCon1->CON->Y,
                                            (l_pCon1->CON->Z ? l_pCon1->CON->Z - 1 : l_pCon1->CON->Z),
                                            l_pCon2->CON->X,
                                            l_pCon2->CON->Y,
                                            (l_pCon2->CON->Z ? l_pCon2->CON->Z - 1 : l_pCon2->CON->Z),
                                            i_pSignal->INDEX,
                                            i_pSignal,
                                            mode
                                           );
            } else {
                l_uLength = find_path_astar(i_pParam, i_pGrid,
                                            l_pCon1->CON->X,
                                            l_pCon1->CON->Y,
                                            (l_pCon1->CON->Z ? l_pCon1->CON->Z - 1 : l_pCon1->CON->Z),
                                            0,
                                            0,
                                            0,
                                            i_pSignal->INDEX,
                                            i_pSignal,
                                            mode
                                           );
            }
            display (LEVEL, DEBUG, "o back from A*\n");
            break;
        default:
            display (LEVEL, ERROR, "unknown algorithm\n");
            exit (-1);
    }
    return l_uLength;
}

ocrNaturalInt
findPathNPoints(ocrRoutingParameters * i_pParam,
                ocrWRoutingGrid * i_pGrid,
                ocrSignal * i_pSignal, ocrWindow * i_pWindow)
{
    ocrConnector *l_pCon;
    ocrConnector *con1, *con2;
    ocrConnector l_NewCon;
    ocrNaturalShort l_bOk;
    ocrNaturalInt l_uLength;
    ocrNaturalInt l_uLengthTotal = 0;
    ocrVirtualConnector *l_pVia;
    ocrVirtualConnector *l_pViaList = i_pSignal->VIA;

    l_pCon = i_pSignal->CON_LIST;

    do {
        con1 = l_pCon;
        l_pCon = l_pCon->NEXT;
        if (!l_pCon) {
            i_pSignal->NICHT_ZU_ROUTIEREN = 1;
            i_pSignal->ROUTED = 1;
            return 0;
        }
    } while ((con1->INTEXT == EXTERNAL) && (con1->VIR_CON_LIST == NULL));

    do {
        con2 = l_pCon;
        if (!con2) {
            i_pSignal->NICHT_ZU_ROUTIEREN = 1;
            i_pSignal->ROUTED = 1;
            return 0;
        }
        l_pCon = l_pCon->NEXT;
    } while ((con2->INTEXT == EXTERNAL) && (con2->VIR_CON_LIST == NULL));

#if 0
    // Création des connecteurs externes 
    if (i_pSignal->CON_LIST == NULL)
        return 0;

    if (i_pSignal->CON_LIST->INTEXT == EXTERNAL)
        makeExtEquipotentielle(i_pParam, i_pGrid, i_pWindow, l_pCon);

    if (i_pSignal->CON_LIST->NEXT == NULL) {
        display(LEVEL, ERROR, "Sig %s has only one connector\n", i_pSignal->NAME);
        //return OCRNATURALINT_MAX;
        exit(1);
    }

    if (i_pSignal->CON_LIST->NEXT->INTEXT == EXTERNAL)
        makeExtEquipotentielle(i_pParam, i_pGrid, i_pWindow, l_pCon->NEXT);
#endif

    do {
#if 0
        if (i_pSignal->INDEX == 1) {
            dumpConList(l_pCon, stdout);
        }
#endif

        // Choix des 2 premiers connecteurs
        l_bOk = chooseInternalConnector(i_pGrid, con1, con2, 0);

        if (l_bOk != OCR_OK) {
            display(LEVEL, DEBUG, "%s\n%s%ld\n",
                    "o No more connectors :", "  unable to route ", i_pSignal->INDEX);
            return OCRNATURALINT_MAX;
        }

#if 0
        display(LEVEL, DEBUG, "(%ld,%ld,%d) -> (%ld,%ld,%d)\n",
                l_pCon->CON->X * 5/* 1 * SCALE_X*/,
                l_pCon->CON->Y * 5/* 1 * SCALE_X*/,
                l_pCon->CON->Z,
                l_pCon->NEXT->CON->X * 5/* 1 * SCALE_X*/,
                l_pCon->NEXT->CON->Y * 5/* 1 * SCALE_X*/,
                l_pCon->NEXT->CON->Z);
#endif

        /* routage bipoint */
        l_uLength = biroute (i_pParam, i_pGrid, con1, con2, i_pSignal, AS_K_SEG);

#if 0
        l_uLength = FINDPATH (i_pParam, i_pGrid,
                              l_pCon->CON->X,
                              l_pCon->CON->Y,
                              l_pCon->CON->Z,
                              l_pCon->NEXT->CON->X,
                              l_pCon->NEXT->CON->Y,
                              l_pCon->NEXT->CON->Z,
                              i_pSignal->INDEX, i_pSignal);
      if ( ((l_pCon->CON->Z) > 0) || ((l_pCon->NEXT->CON->Z) > 0) )
              printf ("routed Z1=%d, Z2=%d for %s\n", l_pCon->CON->Z, l_pCon->NEXT->CON->Z, i_pSignal->NAME);
#endif

    }
    while (l_uLength == OCRNATURALINT_MAX);

    l_uLengthTotal += l_uLength;

    // ajout de 2 VIAs
    if (con1->INTEXT == INTERNAL) {
        if (con1->CON->Z == 0) {
            l_pVia = createVirtualConnector(con1->CON->X,
                                            con1->CON->Y,
                                            con1->CON->Z, 0, 0);
            addVirtualConnector(&l_pViaList, l_pVia);
        }
    }

    if (con2->INTEXT == INTERNAL) {
        if (con2->CON->Z == 0) {
            l_pVia = createVirtualConnector(con2->CON->X,
                                            con2->CON->Y,
                                            con2->CON->Z, 0, 0);
            addVirtualConnector(&l_pViaList, l_pVia);
        }
    }

    l_pCon = con2;
    /*goto findNPointsPath_fin;*/ // XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX

    while (l_pCon->NEXT != NULL) {
        ocrVirtualConnector *l_pVirCon;

        //    if (i_pSignal->INDEX == 598)
        //      dumpSegList (i_pSignal->SEGMENT, stdout);

        l_pVirCon = makeEquipotentielle(i_pParam, i_pGrid, i_pSignal);
        //if (i_pSignal->INDEX == 344)
        //          dumpVirConList (l_pVirCon, stdout);
        l_NewCon.VIR_CON_LIST = l_pVirCon;
        l_NewCon.CON = NULL;

        do {
#if 0
            // Création des connecteurs virtuels externes 
            // si le connecteur est EXTERNE
            if (l_pCon->NEXT->INTEXT == EXTERNAL)
                makeExtEquipotentielle(i_pParam, i_pGrid, i_pWindow,
                                       l_pCon->NEXT);
#endif

            // Recherche d'un point de contact
            l_bOk = 0;

            for (l_pVirCon = l_pCon->NEXT->VIR_CON_LIST;
                 l_pVirCon && !l_bOk;
                 l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
                if (getWSegment(i_pGrid,
                                l_pVirCon->X,
                                l_pVirCon->Y,
                                l_pVirCon->Z)->SIGNAL_INDEX
                    == i_pSignal->INDEX) {
                    ocrVirtualConnector *l_pVia;

                    l_bOk = 1;
                    l_pCon->NEXT->CON = l_pVirCon;
                    l_NewCon.CON = l_pVirCon;
                    l_uLength = 0;

                    // ajout d'un VIA
                    l_pVia = createVirtualConnector(l_pVirCon->X,
                                                    l_pVirCon->Y,
                                                    l_pVirCon->Z, 0, 0);
                    addVirtualConnector(&i_pSignal->VIA, l_pVia);

                } else {
                    if (l_pVirCon->Z)
                        if (getWSegment(i_pGrid,
                                        l_pVirCon->X,
                                        l_pVirCon->Y,
                                        l_pVirCon->Z - 1)->SIGNAL_INDEX
                            == i_pSignal->INDEX) {
                            //ocrVirtualConnector *l_pVia;
                            //printf ("Je suis deja connecte\n");

                            l_bOk = 1;
                            l_pCon->NEXT->CON = l_pVirCon;
                            l_NewCon.CON = l_pVirCon;
                            l_uLength = 0;

                            // ajout d'un VIA
                            //l_pVia = createVirtualConnector (l_pVirCon->X,
                            //                               l_pVirCon->Y,
                            //                               l_pVirCon->Z, 0, 0);
                            //addVirtualConnector (&i_pSignal->VIA, l_pVia);

                        }
//                        ocrWSegment *seg;
//                        seg = (getWSegment (i_pGrid,
//                              l_pVirCon->X,
//                              l_pVirCon->Y,
//                              l_pVirCon->Z));
//                        if ( (l_pCon->NEXT->critVC) && (seg->SIGNAL_INDEX != WSEGMENT_FREE) ) {
//                              printf ("quoi ? : %s:%s:(%ld,%ld,%ld)\n", i_pSignal->NAME, l_pCon->NEXT->NAME, 5*l_pVirCon->X, 5*l_pVirCon->Y, l_pVirCon->Z);
//                              printf ("segment: (%d,%d,%d), %s, sig: %ld\n", seg->LAYER, seg->P_MIN, seg->P_MAX, seg->NAME, seg->SIGNAL_INDEX);
//                        }
                }
            }

            if (!l_bOk) {



                // Choix de 2 autres connecteurs
                l_bOk =
                    chooseInternalConnector2(i_pGrid, l_pCon->NEXT);
                if (l_bOk != OCR_OK) {


//                      printf ("l_pCon:%s!\n", l_pCon->NEXT->NAME);
//                      dumpVirConList (l_pCon->NEXT->VIR_CON_LIST, stdout);
//                      dumpVirConList (l_pCon->NEXT->CON, stdout);
//                      printf ("l_NewCon :\n");
//                      dumpVirConList (l_NewCon.VIR_CON_LIST, stdout);
//                      dumpVirConList (l_NewCon.CON, stdout);
                    
                    display(LEVEL, DEBUG, "o No more connectors :\n  unable to route %ld\n", i_pSignal->INDEX);
                    return OCRNATURALINT_MAX;
                }

                l_uLength = biroute (i_pParam, i_pGrid, l_pCon->NEXT, NULL, i_pSignal, AS_K_EQUI);
#if 0

                display(LEVEL, DEBUG, "(%ld,%ld,%d) -> (%ld,%ld,%d)\n",
                        l_NewCon.CON->X * 5,
                        l_NewCon.CON->Y * 5,
                        l_NewCon.CON->Z,
                        l_pCon->NEXT->CON->X * 5,
                        l_pCon->NEXT->CON->Y * 5,
                        l_pCon->NEXT->CON->Z);

                /* routage bipoint */
                l_uLength = biroute (i_pParam, i_pGrid, &l_NewCon, l_pCon->NEXT, i_pSignal, AS_K_EQUI);

//                l_uLength = FINDPATH(i_pParam, i_pGrid,
//                                     l_NewCon.CON->X,
//                                     l_NewCon.CON->Y,
//                                     (l_NewCon.CON->Z ? l_NewCon.CON->Z -
//                                      1 : l_NewCon.CON->Z),
//                                     l_pCon->NEXT->CON->X,
//                                     l_pCon->NEXT->CON->Y,
//                                     (l_pCon->NEXT->CON->Z ? l_pCon->NEXT->
//                                      CON->Z - 1 : l_pCon->NEXT->CON->Z),
//                                     i_pSignal->INDEX, i_pSignal);
                //printf ("2points - out\n");
#endif
            }
        }
        while (l_uLength == OCRNATURALINT_MAX);

        // ajout de 2 VIAs
        if (l_pCon->NEXT->INTEXT == INTERNAL) {
            if (l_pCon->NEXT->CON->Z == 0) {    // XXX HCl : via seulement pour acceder a l'ALU1
                l_pVia = createVirtualConnector(l_pCon->NEXT->CON->X,
                                                l_pCon->NEXT->CON->Y,
                                                l_pCon->NEXT->CON->Z, 0,
                                                0);
                addVirtualConnector(&l_pViaList, l_pVia);
            }
        }

#if 0
        if (l_NewCon.CON->DIST == 1) {
            l_pVia = createVirtualConnector(l_NewCon.CON->X,
                                            l_NewCon.CON->Y,
                                            l_NewCon.CON->Z, 0, 0);
            addVirtualConnector(&l_pViaList, l_pVia);
        }


        unifyPoint(i_pGrid, i_pParam,
                   i_pSignal,
                   l_NewCon.CON->X, l_NewCon.CON->Y,
                   (l_NewCon.CON->Z ? l_NewCon.CON->Z -
                    1 : l_NewCon.CON->Z));
#endif

        deleteVirtualConnectorList(l_NewCon.VIR_CON_LIST);
        l_pCon = l_pCon->NEXT;
        l_uLengthTotal += l_uLength;
    }
findNPointsPath_fin:
    i_pSignal->VIA = l_pViaList;

//   {
//         int zzz=0;
//   for (l_pCon = i_pSignal->CON_LIST ; l_pCon ; l_pCon = l_pCon->NEXT) {
//         ocrVirtualConnector *l_pVirCon;
//         for (l_pVirCon = l_pCon->VIR_CON_LIST; l_pVirCon; l_pVirCon = l_pVirCon->NEXT) {
//                 if (l_pVirCon->Z > 0) {
//                         zzz = 1;
//                         goto zzz_skip;
//                 }
//         }
//   }
//zzz_skip:
//   if (zzz)
//         printf ("ah que oh yeah !!!\n");
//   }
    return l_uLengthTotal;
}
